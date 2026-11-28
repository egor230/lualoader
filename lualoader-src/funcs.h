
#include<fstream>
#include <windows.h>
#include <winuser.h>
#include<thread> // std::thread
#include<chrono> // std::thread
#include <process.h>

#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <mutex>
#include <atomic> // std::atomic — сторож миссии

#include <algorithm>
#include<lua.hpp>

// ==== Быстрая загрузка моделей (18.10.2026) ====
// Менеджер стриминга (compat/ma_streaming.cpp): REQUEST-команды шлёт кадровый
// обработчик на игровом потоке, worker только ждёт событие — без кругов очереди.
// Хук регистрирует Message в plugin.cpp (ma_stream_install_hook).
#include "compat/ma_streaming.h"

// ==== Планировщик скриптов (2021, директива юзера) ====
// ОДИН OS-ПОТОК НА КАЖДЫЙ .lua (plugin.cpp): каждый скрипт — своё lua_State и
// свой поток создания; wait(ms) спит ЭТИМ потоком (2021-модель). Планировщик
// ma_sched кооперативной парковки удалён; streams/keyboard — прежние compat-модули.

// ==== Обёртка регистрации Lua-API (18.10.2026) ====
// Каждая C-функция получает крошку «файл:строка скрипта -> имя API» для отчёта о
// вылете (ma_diag::lua_note в compat/diag.cpp). Макрос перекрывает lua_register
// из lua.h; реально его зовёт только register.cpp (funcs.cpp/plugin.cpp — нет).
int ma_lua_register(lua_State* L, const char* name, lua_CFunction fn); // внутренняя обёртка регистрации Lua API (макрос в funcs.h)
void ma_lua_crumb(lua_State* L, const char* api);
#undef lua_register
#define lua_register ma_lua_register

#include "common.h"
#include "plugin.h"
#include "CWorld.h"
#include "extensions/KeyCheck.h"
#include "extensions/ScriptCommands.h"
#include "eScriptCommands.h"
#include "CCivilianPed.h"
#include "CMessages.h"
#include "ePedType.h"
#include "CModelInfo.h"

#include "CStreaming.h"
#include "CTheScripts.h"
#include "eWeaponType.h"
#include "CPickups.h"
#include "CClock.h"
#include "CUserDisplay.h"
#include "CCamera.h"
#include "CWeapon.h"
#include "CPlayerPed.h"
#include "CWeaponInfo.h"
#include "CShotInfo.h"
#include "CMenuManager.h"
#include "CGame.h"// gGameState для plugin.cpp (замена хардкода 0x9B5F08).
#include "CTimer.h"// CTimer::m_snTimeInMilliseconds для plugin.cpp (замена хардкода 0x974B2C).
#include "tHandlingData.h"
#include "CMatrix.h"
#include "CVector.h"
#include "CCopPed.h"
#include "CHeli.h"
#include "CWeather.h"
#include "CProjectileInfo.h"
#include "CBulletTraces.h"
#include "C3dMarkers.h"
#include "CAutomobile.h"
#include "CHud.h"
#include "CPlayerInfo.h"
#include "CBoat.h"
#include "CBike.h"
#include "CTrafficLights.h"
#include "CCutsceneMgr.h"
#include "CCarCtrl.h"
#include "CBrightLights.h"
#include "CTrain.h"
#include "CGangs.h"
#include "CMotionBlurStreaks.h"
#include "CCarAI.h"// статик ИИ авто (въезд/парковка) для новых функций 2026-09-21.
#include "CPad.h"// тряска геймпада для новых функций 2026-09-21.
#include "CParticle.h"// частицы для новых функций 2026-09-21.
#include "CPointLights.h"// точечный свет для новых функций 2026-09-21.
#include "CCoronas.h"
#include "CDraw.h"
#include "CWanted.h"
#include "CFire.h"
#include "CFireManager.h"
#include "CStats.h"
#include "CRadar.h"
#include "CPopulation.h"
#include "CGeneral.h"
#include "CGangs.h"
#include "CGameLogic.h"
#include "CFont.h"
#include "eLevelName.h"
#include "CZone.h"
#include "CTheZones.h"
#include "CEscalators.h"
#include "CCranes.h"


using namespace plugin;
using namespace std;

// extern-объявления глобалов, общих с plugin.cpp (определения — в funcs.cpp).
extern unsigned int iters;
extern list<char*>listfile;
extern list<lua_State*>luastate;
extern string cheatstr;
// Эпоха перезагрузки: поколение состояний. publish() штампует состояние текущей эпохой,
// teardown инкрементирует эпоху — все старые состояния мгновенно становятся «устаревшими»,
// и их wait() отдаёт ход владельцу, который сам закрывает СВОЁ состояние (асинхронно).
extern std::atomic<unsigned int> g_lua_epoch;
void lua_state_stamp(lua_State* L);        // публикация: привязать состояние к текущей эпохе (funcs.cpp).
void lua_state_unstamp(lua_State* L);      // закрытие владельцем: снять привязку.
bool lua_state_obsolete(lua_State* L);     // состояние из старого поколения? (функция — в funcs.cpp, читает epoch-карту).
// ПАУЗА: флаг «идёт разборка» (определение — в plugin.cpp). Его проверяют БЛОКИРУЮЩИЕ API
// (play_voice, load_model_before_avalible, getcord, expectations) — у них нет доступа к эпохе
// состояния (getcord живёт вообще на своём detached-потоке), поэтому требуется глобальный
// сигнал «всем срочно выйти из длинных циклов». Ставится ПЕРВЫМ шагом teardown (пауза всех
// скриптов ДО разборки), снимается когда разборка запущена.
extern std::atomic<bool> teardown_active;
// (25.10, финал-6) ЕДИНЫЙ мьютекс всех Lua-API: guard в теле каждой зарегистрированной
// функции. wait() и yield() его НЕ берут (паркуются изнутри — держать = зависание).
extern std::recursive_mutex g_lua_api_mx;
// ПАУЗА по yield: поставлена pause_scripts() (определение — в plugin.cpp). wait() внутри
// скриптов при этом делает return lua_yield(L,0) — скрипт БЕЗОПАСНО замирает со своего же
// кадра (yield из СВОЕГО wait), pump-поток паркуется и ничего не закрывает. Обратимо.
extern std::atomic<bool> scripts_paused;
string getkey(int key);
int setobjocoordes(lua_State* L); // установить координаты для объект. (определение в funcs.cpp)
int create_newthread(lua_State* L); // создания нового потока. (определение в plugin.cpp)

// общие структуры для funcs.cpp/plugin.cpp (определения статических полей — в funcs.cpp).
struct spite {// для текста на экране.
	static bool printgame;	static string strprintgame;// вывод на экран.
	static int abc_x; static int ord_y;	static float spacing; static int font;
	static float size_x; static float size_y; static int red; static int blue;
	static int green;

	static bool switc; static CPed* p;
	static void set(bool printgame1, string strprintgame1, int abc_x1, int ord_y1, float spacing1,
		int font1, float size_x1, float size_y1, int red1, int blue1, int green1) { // Включить событие двери
		printgame = printgame1; strprintgame = strprintgame1;// вывод на экран.
		abc_x = abc_x1; ord_y = ord_y1;	spacing = spacing1; font = font1; size_x = size_x1;
		size_y = size_y1; red = red1; blue = blue1; green = green1;
	}
	static void active(bool switc1, CPed* p1) {	bool switc = switc1;
		CPed* p = p1;
	}
	static void draw() { //
		if (printgame == true) {
			gamefont::Print({ Format(strprintgame) }, abc_x, ord_y, spacing, font, size_x, size_y, CRGBA(red, blue, green));
		}
		if (switc == true) {	p->Attack();
		} }//;
};

struct corona {// структура для короны.
	static bool switc;
	static float abc_x; static float ord_y; static float pos_z; static float radius;
	static int type; static int glow_flare; static int red; static int blue;
	static int green;

	static void set(bool switc1, float abc_x1, float ord_y1, float pos_z1, float radius1,
		int type1, int glow_flare1, int red1, int blue1, int green1) { // установить цвет короны.
		switc = switc1;
		abc_x = abc_x1; ord_y = ord_y1;	pos_z = pos_z1; type = type1; radius = radius1; glow_flare = glow_flare1;
		red = red1; blue = blue1; green = green1;
	}
	static void draw() {// вывод короны.
		float x = abc_x;	float y = ord_y;	float z = pos_z;
		float radius1 = radius; int type1 = type;	int glow_flare1 = glow_flare;
		int red1 = red;  int blue1 = blue; int green1 = green;
		if (switc == true) {
			Command<COMMAND_DRAW_CORONA>(x, y, z, radius1, type1, glow_flare1, red1, green1, blue1);
		}
	}//;
};

const float ACTION_TIME_STEP = 0.05f;
struct DoorsExample {
	static int componentByDoorId[6]; // Таблица перевода eDoors в Id компонента
	static int m_nLastTimeWhenAnyActionWasEnabled; // Последнее время запуска события

	enum eDoorEventType { DOOR_EVENT_OPEN, DOOR_EVENT_CLOSE };// Тип события

	struct DoorEvent {
		bool m_active;	eDoorEventType m_type;	float m_openingState;
		DoorEvent() { m_active = false;	m_type = DOOR_EVENT_CLOSE; }// Класс события
	};

	struct VehicleDoors {
		DoorEvent events[6]; // События для всех 6 дверей
		VehicleDoors(CVehicle*) {}
	};

	static VehicleExtendedData<VehicleDoors> VehDoors; // Наше расширение

	static void EnableDoorEvent(CAutomobile* automobile, eDoors doorId) { // Включить событие двери
		if (automobile->IsComponentPresent(componentByDoorId[doorId])) {
			CDamageManager* p = &automobile->m_carDamage;// менеджер урона этого авто (раньше указатель был не инициализирован - краш).
			if (p->GetDoorStatus(doorId) != DAMSTATE_NOTPRESENT) {
				DoorEvent& event = VehDoors.Get(automobile).events[doorId];
				if (event.m_type == DOOR_EVENT_OPEN)
					event.m_type = DOOR_EVENT_CLOSE; // Если последнее событие - открытие, то закрываем
				else
					event.m_type = DOOR_EVENT_OPEN; // Если последнее событие закрытие - то открываем
				event.m_active = true; // Включаем обработку
				m_nLastTimeWhenAnyActionWasEnabled = CTimer::m_snTimeInMilliseconds;
			}
		}
	};

	static void ProcessDoors(CVehicle* vehicle) { // Обработка событий для конкретного авто
		if (vehicle->m_nVehicleClass == VEHICLE_AUTOMOBILE) {
			CAutomobile* automobile = reinterpret_cast<CAutomobile*>(vehicle);
			for (unsigned int i = 0; i < 6; i++) { // Обрабатываем все события
				eDoors doorId = static_cast<eDoors>(i);
				DoorEvent& event = VehDoors.Get(automobile).events[doorId];
				if (event.m_active) { // Если событие активно
					if (event.m_type == DOOR_EVENT_OPEN) {
						event.m_openingState += ACTION_TIME_STEP;
						if (event.m_openingState > 1.0f) { // Если полностью открыли
							event.m_active = false; // Отключаем обработку
							automobile->OpenDoor(componentByDoorId[doorId], doorId, 1.0f); // Полностью открываем
							event.m_openingState = 1.0f;
						}
						else
							automobile->OpenDoor(componentByDoorId[doorId], doorId, event.m_openingState);
					}
					else {
						event.m_openingState -= ACTION_TIME_STEP;
						if (event.m_openingState < 0.0f) { // Если полностью открыли
							event.m_active = false; // Отключаем обработку
							automobile->OpenDoor(componentByDoorId[doorId], doorId, 0.0f); // Полностью открываем
							event.m_openingState = 0.0f;
						}
						else
							automobile->OpenDoor(componentByDoorId[doorId], doorId, event.m_openingState);
					}
				}
			}
		}
	};
};

int find_in_map(std::map<string, int>& carlist, const char* search);// найти id авто.
int find_model_in_map(std::map<int, int>& type_and_model, int search);// найти модель авто.


CPed* findpedinpool(const void* p); // найти педа в пуле.
CVehicle* findcarinpool(const void* p); // найти авто в пуле.
CObject* findobjinpool(const void* p);// найти объект в пуле.
void warp_out_car_occupants(CVehicle* car);// выкинуть всех сидящих в авто.
void safe_remove_car(CVehicle* car);// безопасно удалить авто.
void warp_ped_out_of_car(CPed* ped);// выкинуть педа из авто, если сидит.

void getkeyenvent(); // считывания символов клавиатуры.
int timerstar(); // запуск скриптов после загрузки сэйва (определение в plugin.cpp).
wchar_t* getwchat(const char* c); // перевод в строку.

int funs(lua_State* L);// список функций.
void writelog(const char x[]);// запись ошибок в файл.
void dellod(); // удалить лог ошибок.
int cleanstl();// очистка stl.

int wait(lua_State* L);// пауза корутины: wait([мс]); без аргумента — 1 мс (park + yield)
int wait_key(lua_State* L);// (26.10) обработчик нажатия клавиш через планировщик: ждёт press→release, паркуя корутину (скрипты переключаются, игра не блокируется)

// (26.10, P1) ЕДИНЫЙ СНИМОК СОСТОЯНИЯ КЛАВИАТУРЫ: единственный опрос Win32
// клавиш (GetAsyncKeyState) делает ma_keys_poll() — раз в кадр на игровом
// потоке из клавиатурной задачи ПЕРЕД скриптами. C++ API getkey/keypress/
// wait_key читают только снимок: без системных вызовов в пути скриптов и
// без ~183 GetAsyncKeyState на опрос чек-стринга каждый кадр.
struct ma_key_state {
	unsigned char held[256];// клавиша удержана (бит 0x8000) на последнем опросе
	unsigned char was_held[256];// была удержана кадром раньше (диагностика)
};
ma_key_state& ma_keys();// снимок (function-local static в funcs.cpp)
void ma_keys_poll();// единственный GetAsyncKeyState (игровой поток, задача-клавиатура)
int findplayer(lua_State* L);// найти педа.
int getpedhealth(lua_State* L);// получить здоровье педа.
int getpedhealthf(lua_State* L); // получить здоровье педа (float)
int setpedhealth(lua_State* L);// установить здоровье педа.

int getpedarmour(lua_State* L); // получить броню педа.
int setarmour(lua_State* L);// установить броню педа.
int getcarhealth(lua_State* L); // получить кол-во здоровья авто.
int setcarhealth(lua_State* L); // установить здоровье авто.

int create_door_object(lua_State* L); // (26.11) создать объект-дверь с удержанием модели/коллизии.
int get_model_col_bounds(lua_State* L); // (26.11) min/max COL boundingBox модели (6 чисел или nil).
int create_door_object_no_offset(lua_State* L); // (26.11) дверь через 029B, Z = origin, статичная, collide.
int loadmodel(lua_State* L);// загрузить модель.
int createcar(lua_State* L);// создать авто на координатах на координатах.
int availablemodel(lua_State* L);// проверка на загруженность модели.
int releasemodel(lua_State* L);// удалить модель из памяти.

int keypress(lua_State* L);// проверка на нажатия клавиши.
int lockstatus(lua_State* L);// статус двери авто.
int stop_car_radio(lua_State* L);   // выключить играющее радио (DMAudio.SetRadioInCar, 9=OFF)
int givemoney(lua_State* L);// дать денег.
int getpedcoordes(lua_State* L);// получить координаты.

int printmessage(lua_State* L);// вывести сообщение на экран.
int randomfindped(lua_State* L);// найти случайнного педа.
int incar(lua_State* L);// пед в авто?
int exitcar(lua_State* L);// выйти из авто.

int createped(lua_State* L);// создать педа.
int create_marker_actor(lua_State* L);// создать маркер над педа.
int removemarker(lua_State* L);// удалить маркер.
int ped_sprint_to_point(lua_State* L);//пед делает спринт к точке.

int ped_walk_to_point(lua_State* L);// пед идет к точке.
int getpedangle(lua_State* L);// получить угол педа.
int load_requested_models(lua_State* L);// загрузить модель вне очереди.
int giveweaponped(lua_State* L);// дать педу оружие.

int ped_aim_at_ped(lua_State* L);// пед целиться в педе.
int is_current_weapon_ped(lua_State* L);// проверить текущее оружие.
int set_current_weapon_ped(lua_State* L); // установить текущий оружие пешеход


int set_ped_wait_state(lua_State* L);// уст. ожидания педа (0372: SET_CHAR_WAIT_STATE).
int set_ped_personality(lua_State* L);// уст. личность педа (0243: SET_CHAR_PERSONALITY).
int set_ped_only_damaged_by_player(lua_State* L);// пед ранен только игроком (02A9: SET_CHAR_ONLY_DAMAGED_BY_PLAYER).
int is_ped_health_greater(lua_State* L);// здоровье педа больше? (0184: IS_CHAR_HEALTH_GREATER, возвращает bool).
int make_obj_targettable(lua_State* L);// в объект можно целиться (035D: MAKE_OBJECT_TARGETTABLE).
int set_object_collision(lua_State* L);// объект сталкивается с миром (0382: SET_OBJECT_COLLISION).
int set_object_dynamic(lua_State* L);// объект динамический: падает/двигается (0392: SET_OBJECT_DYNAMIC).
int get_dead_char_pickup_coords(lua_State* L);// координаты трупного пикапа (04A5: GET_DEAD_CHAR_PICKUP_COORDS, 3 float).
int set_ped_mood(lua_State* L); // установить пешеход mood
int request_animation(lua_State* L);// запросить анимацию по имени (04ED: REQUEST_ANIMATION).
int remove_animation(lua_State* L); // удалить анимация
int set_ped_stay_when_attacked(lua_State* L);// пед держит позицию при атаке (0350).
int worldcoord(lua_State* L); // Перевод в мировые координаты.
int getpedcoordinates_on_x(lua_State* L); // получить координату X педа

int getpedcoordinates_on_y(lua_State* L); // получить координату Y педа
int kill_ped_on_foot(lua_State* L);// убить педа пешком.
int kill_char_any_means(lua_State* L);// убить педа любыми средствами.

int create_sphere(lua_State* L); //создать сферу.
int remove_sphere(lua_State* L); // удалить сферу.
int remove_ped(lua_State* L); // удалить педа.
int remove_car(lua_State* L); // удалить авто.

int get_water_level(lua_State* L); // (25.11) Z поверхности воды в точке (x,y) или nil (0x5C2C80).
int iscar_in_water(lua_State* L); // проверка авто в воде.
int set_wanted(lua_State* L); // установить розыск
int clear_wanted(lua_State* L);// убрать уровень розыска.
int set_police_ignore_player(lua_State* L);// копы игнорируют игрока (01F7: SET_POLICE_IGNORE_PLAYER).
int kill_ped(lua_State* L); // убить педа.

int setpedcoordes(lua_State* L); // установить координаты для педа.
int ped_in_point_in_radius(lua_State* L); // проверить находится пед в координатах с радиусом.
int cardrive(lua_State* L); // авто едет в точку.

int setcarspeed(lua_State* L); // установить скорость авто.
int getflagmission(lua_State* L); // проверка флага миссии.
int setflagmission(lua_State* L); // Установить флаг миссии
int register_mission_given(lua_State* L); // отметить миссию выданной (0317: REGISTER_MISSION_GIVEN).
int register_mission_passed(lua_State* L); // отметить миссию пройденной (0318: REGISTER_MISSION_PASSED).
int showtext(lua_State* L);// Вывод особого текста на экран.

int remove_blip(lua_State* L);// удалить метку с карты.
int createblip(lua_State* L); // создать метку на карте.
int play_sound(lua_State* L);// проиграть мелодию.
int isped(lua_State* L); // проверка это пед?.

int isvehicle(lua_State* L); // проверка это транспорт?.
int opendoorcar(lua_State* L); // открыть дверь авто.
int randomfindcar(lua_State* L); //Найти случайное авто.
int create_money_pickup(lua_State* L); //создать пачку денег.

int getcarcoordes(lua_State* L); // получить координаты авто.
int getcarcoordinates_on_x(lua_State* L); // получить координату X авто
int getcarcoordinates_on_y(lua_State* L); // получить координату Y авто
int car_in_point_in_radius(lua_State* L); // проверить находится авто в координатах с радиусом.

int setdrivingstyle(lua_State* L); // установить стиль езды авто.
int findped(lua_State* L); // найти педа в пуле.
int create_weapon_pickup(lua_State* L); //создать пикап оружие.
int create_pickup(lua_State* L); //создать пикап.

int picked_up(lua_State* L); // проверка пикап подобран.
int remove_pickup(lua_State* L); // удалить пикап.
int play_voice(lua_State* L); // Проиграть голос(реплику).
int fade(lua_State* L);//затенение, просветления.

int draw_corona(lua_State* L); // создать корону(гоночный чекпойнт).
int sound_coordinate(lua_State* L); // Проиграть звук в координатах
int show_text_styled(lua_State* L); // вывести игровой текст.
int setcarangle(lua_State* L); // установить угол авто.

int createmarker(lua_State* L); // создать маркер на карте.
int setsizemarker(lua_State* L); // установить размер отображение маркера на карте.
int reset_cheat(lua_State* L); // (28.11) обнулить строку ввода чит-кода (пробел).
int checkcheat(lua_State* L); // чит код введен?.
int destroy(lua_State* L); // удаления объектов из памяти при перезагрузки скрипта.

int my_yield(lua_State* L); // приостановить выполнение скрипта.
int setcardrive(lua_State* L); //  установить водителя для авто.
int setcarpassenger(lua_State* L); // установить пассажира для авто.
int setcarfirstcolor(lua_State* L); // установить первый цвет авто.

int setcarseconscolor(lua_State* L);// установить второй цвет авто.
int set_traffic(lua_State* L); // установить плотность трафика транспорта.
int create_marker_car(lua_State* L); //создать маркер над авто.
int car_explode(lua_State* L); // взорвать авто.

int is_car_stopped(lua_State* L); // авто остановилось.
int create_explosion(lua_State* L); // Создать взрыв на координатах.
int set_status_engine(lua_State* L); // установить состояние двигателя авто.
int player_defined(lua_State* L); // пед существует?

int setclock(lua_State* L); // задать время.
int arrested(lua_State* L); // пед арестован?
int create_marker_pickup(lua_State* L);// создать маркер над пикапом.
int createobj(lua_State* L); // создать объект.

int remove_obj(lua_State* L); // удалить объект.
int getobjcoordes(lua_State* L); // получить координаты объекта.
int create_marker_obj(lua_State* L); //создать маркер над объектом.

int isobject(lua_State* L); // проверка это объект?.
int setpedangle(lua_State* L); // установить угол педа.
int setcaraction(lua_State* L);// установить поведение авто.
int move_obj(lua_State* L); //двигать объект.
int move_obj_to(lua_State* L); //двигать объект в заданные координаты с заданной скоростью (свой поток).

int move_rotate(lua_State* L); // вращать объект.
int getobjangle(lua_State* L); // получить угол объекта.
int findcar(lua_State* L); // Найти авто.
int setcartask(lua_State* L);// установить задачу авто.

int setcarcoordes(lua_State* L);// установить координаты авто.
int is_car_stuck(lua_State* L); // проверить авто застрял
int is_car_upsidedown(lua_State* L); //01F4: car 12@ flipped если машина перевернута.
int is_car_upright(lua_State* L); // 020D: car 12@ flipped если указанный автомобиль перевернут.

int find_road_for_car(lua_State* L); // найти дорогу.
int setcarstrong(lua_State* L); // сделать авто устойчивым.
int putincar(lua_State* L);// переместить педа в авто.
int game_font_print(lua_State* L); // вывести особенный игровой текст.

int star_timer(lua_State* L); // включить таймер.
int stop_timer(lua_State* L); // остановить таймер.

int timer_donw(lua_State* L); //  таймер на уменьшение.
int ped_attack_car(lua_State* L); // пед атакует авто.
int ped_frozen(lua_State* L); // Пешеход frozen
int hold_cellphone(lua_State* L); // поднять телефон.

int car_lastweapondamage(lua_State* L); // номер оружие, которое нанесло урон авто.
int car_currentgear(lua_State* L); // текущая передача авто.
int getcar_model(lua_State* L); // получить модель авто.
int setcarsiren(lua_State* L); // установить сирену для авто.

int ped_car_as_driver(lua_State* L); // пед садится в авто как водитель.
int ped_car_as_passenger(lua_State* L); // пед садится в авто как пассажир.
int show_text_gtx(lua_State* L); // вывести игровой текст.
int camera_at_point(lua_State* L); //переместить камеру в координатах.

int set_camera_position(lua_State* L); //установить камеру в координатах.
int restore_camera(lua_State* L); // восстановить камеру.
int is_wanted_level(lua_State* L); // проверить уровень розыска.
int ped_atack(lua_State* L); // пед бьет.

int flash_hud(lua_State* L); // Вспышка hud
int set_radio(lua_State* L); // установить радио.
int set_car_tires(lua_State* L); // проколоть шину авто.

int create_spec_ped(lua_State* L); // создать спец педа.
int set_wheel_status(lua_State* L); // установить состояния шин авто.
int set_skin(lua_State* L); // установить скин педа.
int remove_spec_ped(lua_State* L);  // удалить спец педа.

int go_to_route(lua_State* L); //установить маршрут авто.
int add_stuck_car_check(lua_State* L); // условия для того, чтобы авто считалась застрявшей.
int load_scene(lua_State* L); // загрузить модели на координатах заранее.
int ped_anim(lua_State* L); // анимация.

int del_anim(lua_State* L); // удалить анимацию.
int get_current_name_luascript(lua_State* L); // получить текущий имя luascript
int star_mission_marker(lua_State* L); // создать маркер для миссии.
int getobjcoordinates_on_x(lua_State* L); // получить координату X объекта

int getobjcoordinates_on_y(lua_State* L); // получить координату Y объекта
int set_widescreen(lua_State* L); // вкл/выкл широкий экран.
int follow_the_leader(lua_State* L); // Следовать лидер
int getcarspeed(lua_State* L); // получить скорость авто.

int Getcameracoordes(lua_State* L);// получить координаты камеры.
int remove_all_weapons_ped(lua_State* L); // удалить все оружия педа.
int Getweaponslot(lua_State* L); // получить номер слота по типу оружия.
int get_damage_weapon_ped(lua_State* L); // получить последний урон педа от оружия.

int get_aimgun_ped(lua_State* L); // получить показатель цели педа.
int get_ammo_weapon_ped(lua_State* L);// получить кол-во патроны текущего оружие педа.
int createfireonped(lua_State* L);// создать огонь на педе.
int createfireoncar(lua_State* L);// создать огонь на авто.

int createfireoncords(lua_State* L); // создать огонь на координатах.
int remove_fire(lua_State* L); // удалить огонь.
int ped_shutdown(lua_State* L); // завершить пешеход
int is_ped_damage_from_ped(lua_State* L); // получил ли пед урон от педа.

int is_targetting_in_ped(lua_State* L); // игрок целиться в педа.
int Remove_weapon_model(lua_State* L); // удалить оружие у педа.
int Createped(lua_State* L); // макрос создать педа.
int Createcar(lua_State* L); // макрос создать авто на координатах.

int Giveweaponped(lua_State* L); // макрос дать педу оружие и патроны.
int Opendoorcar(lua_State* L); // Макрос открыть все двери авто.
int Create_weapon_pickup(lua_State* L); // макрос создать пикап оружие.
int Create_pickup(lua_State* L); // макрос создать пикап.

int Get_model_and_type_current_weapon_ped(lua_State* L); // макрос получить модель и тип текущего оружие педа.
int is_ped_in_car(lua_State* L); // игрок в авто?
int ped_car(lua_State* L); // авто педа.
int wanted_level(lua_State* L); // получить уровень розыска.

int get_model_current_weapon_ped(lua_State* L); // макрос получить модель текущего оружие педа.
int get_type_current_weapon_ped(lua_State* L); // макрос получить тип текущего оружие педа.
int set_camera_and_point(lua_State* L); // установить и переместить камеру в координатах.
int get_damage_current_weapon(lua_State* L); // получить уровень урона текущего оружие.

int set_damage_current_weapon(lua_State* L); // установить уровень урона текущего оружие педа.
int set_clip_current_weapon(lua_State* L); // установить объём обоймы текущего оружие педа (только огнестрельного).
int ped_in_targetting(lua_State* L); // пед на прицеле.
int heli_set_orientation(lua_State* L); // Вертолёт установить ориентация
int heli_clear_orientation(lua_State* L); // Вертолёт очистить ориентация
int heli_set_stabiliser(lua_State* L); // Вертолёт установить стабилизатор
int heli_make_crashing_down(lua_State* L); // Вертолёт сделать авария
int heli_is_player_in_any_heli(lua_State* L); // Вертолёт игрок вертолёт
int heli_is_char_in_any_heli(lua_State* L); // Вертолёт персонаж вертолёт
int heli_switch(lua_State* L); // Вертолёт переключатель
int catalina_heli_start(lua_State* L); // начать catalina вертолёт
int catalina_heli_take_off(lua_State* L); // Catalina вертолёт взять
int catalina_heli_remove(lua_State* L); // удалить catalina вертолёт
int catalina_heli_grab(lua_State* L); // Catalina вертолёт grab
int catalina_heli_fly_away(lua_State* L); // Catalina вертолёт лететь прочь
int catalina_heli_has_shot_down(lua_State* L); // Catalina вертолёт имеет выстрел
int plane_goto_coords(lua_State* L); // Самолёт перейти координаты
int plane_is_player_in_any_plane(lua_State* L); // Самолёт игрок самолёт
int plane_is_char_in_any_plane(lua_State* L); // Самолёт персонаж самолёт
int find_drug_plane_coords(lua_State* L); // найти drug самолёт координаты
int find_drop_off_plane_coords(lua_State* L); // найти бросить самолёт координаты
int has_drug_plane_shot_down(lua_State* L); // проверить drug самолёт выстрел
int has_drop_off_plane_shot_down(lua_State* L); // проверить бросить самолёт выстрел
int boat_goto_coords(lua_State* L); // Лодка перейти координаты
int boat_set_cruise_speed(lua_State* L); // Лодка установить круиз скорость
int boat_anchor(lua_State* L); // Лодка anchor
int boat_start_foam_animation(lua_State* L); // Лодка начать foam анимация
int boat_update_foam_animation(lua_State* L); // Лодка обновить foam анимация
int boat_is_player_in_any_boat(lua_State* L); // Лодка игрок лодка
int boat_is_char_in_any_boat(lua_State* L); // Лодка персонаж лодка
int is_vehicle_boat(lua_State* L); // проверить автомобиль лодка
int train_catch_char_obj(lua_State* L); // Train catch персонаж объект
int train_catch_coll_obj(lua_State* L); // Train catch коллизия объект
int rc_give_remote_controlled_car_to_player(lua_State* L); // Радиоуправляемый дать remote controlled авто игрок
int rc_give_remote_controlled_model_to_player(lua_State* L); // Радиоуправляемый дать remote controlled модель игрок
int rc_get_remote_controlled_car(lua_State* L); // Радиоуправляемый получить remote controlled авто
int rc_buggy_blow_up(lua_State* L); // Радиоуправляемый buggy blow
int rc_buggy_remove(lua_State* L); // удалить радиоуправляемый buggy
int rc_detonate_enable(lua_State* L); // включить радиоуправляемый подорвать
int rc_detonate_on_contact_enable(lua_State* L); // включить радиоуправляемый подорвать контакт
int player_is_in_remote_mode(lua_State* L); // Игрок remote режим
int set_fade_jumpcut_after_rc_explosion(lua_State* L); // установить затемнение jumpcut после радиоуправляемый взрыв
int bike_is_player_on_any_bike(lua_State* L); // Мотоцикл игрок мотоцикл
int bike_is_char_on_any_bike(lua_State* L); // Мотоцикл персонаж мотоцикл
int is_char_in_flying_vehicle(lua_State* L); // проверить персонаж flying автомобиль
int is_player_in_flying_vehicle(lua_State* L); // проверить игрок flying автомобиль
int crane_activate(lua_State* L); // активировать кран
int crane_is_carrying_car(lua_State* L); // Кран carrying авто
int crane_is_targetting_car(lua_State* L); // Кран targetting авто
int escalators_count(lua_State* L); // посчитать escalators
int zone_find_index(lua_State* L); // Зона найти индекс
int zone_get_level_from_position(lua_State* L); // Зона получить уровень позиция
// ================= ЭТАП 12: Остальной транспорт GTA VC =================

// КРАНЫ
int crusher_crane_activate(lua_State* L); // активировать пресс кран
int military_crane_activate(lua_State* L); // активировать военный кран
int crane_deactivate(lua_State* L); // Кран деактивировать
int crane_military_collected_all(lua_State* L); // Кран военный собрано
int crane_is_lifting_car(lua_State* L); // Кран подъём авто

// КАТЕРА
int boat_stop(lua_State* L); // остановить лодка

// ВЕРТОЛЕТЫ
int heli_goto_coords(lua_State* L); // Вертолёт перейти координаты
int heli_create_dust_effect(lua_State* L); // Вертолёт создать пыль эффект

// УПРАВЛЕНИЕ АВТО
int car_goto_accurate(lua_State* L); // Авто перейти точный
int car_set_idle(lua_State* L); // Авто установить холостой
int car_wander_randomly(lua_State* L); // Авто бродить случайно
int car_face_coord(lua_State* L); // Авто лицо координата
int freeze_car_position(lua_State* L); // заморозить авто позиция
int freeze_car_no_collide(lua_State* L); // заморозить авто столкнуться
int set_car_status(lua_State* L); // установить авто статус
int set_car_mission(lua_State* L); // установить авто миссия
int car_block_car(lua_State* L); // Авто блок авто
int car_ram_car(lua_State* L); // Авто таран авто
int car_change_lane(lua_State* L); // Авто изменить полоса движения
int car_drive_straight(lua_State* L); // Авто ездить прямо
int set_car_forward_speed(lua_State* L); // установить авто вперёд скорость
int car_wait(lua_State* L); // ждать авто
int car_route_seed(lua_State* L); // Авто маршрут семя
int car_straight_dist(lua_State* L); // Авто прямо расстояние
int car_convoy(lua_State* L); // Авто конвой
int car_make_stronger(lua_State* L); // Авто сделать сильнее
int car_to_player(lua_State* L); // Авто игрок
int car_fade_in(lua_State* L); // Авто затемнение
int car_on_trailer(lua_State* L); // Авто прицеп

// СВОЙСТВА АВТО
int set_car_visible(lua_State* L); // установить авто видимый
int set_car_watertight(lua_State* L); // установить авто водонепроницаемый
int set_car_big_wheels(lua_State* L); // установить авто большой колёса
int set_car_bproof(lua_State* L); // установить авто защита от пуль
int set_car_fproof(lua_State* L); // установить авто защита от огня
int set_car_rproof(lua_State* L); // установить авто защита от огня
int set_car_no_collide(lua_State* L); // установить авто столкнуться
int set_car_burst_tires(lua_State* L); // установить авто прокол шины
int set_car_dmg_player_only(lua_State* L); // установить авто урон игрок только
int car_load_collision(lua_State* L); // Авто загрузить столкновение
int car_upside_safe(lua_State* L); // Авто вверх дном безопасно
int car_change_color(lua_State* L); // Авто изменить цвет
int car_set_comp(lua_State* L); // Авто установить компонент

// ЗДОРОВЬЕ И УГОЛ
int set_car_health(lua_State* L); // установить авто здоровье
int get_car_health(lua_State* L); // получить авто здоровье
int get_car_heading(lua_State* L); // получить авто курс

// ВЕКТОРА
int get_car_forward_x(lua_State* L); // получить авто вперёд X
int get_car_forward_y(lua_State* L); // получить авто вперёд Y

// ПРОВЕРКИ АВТО (boolean)
int is_car_in_air(lua_State* L); // проверить авто воздух
int is_car_in_air_proper(lua_State* L); // проверить авто воздух правильный
int car_health_greater(lua_State* L); // Авто здоровье больше
int is_car_crushed(lua_State* L); // проверить авто раздавлен
int is_car_remap(lua_State* L); // проверить авто переопределить
int is_car_stuck_roof(lua_State* L); // проверить авто застрял крыша
int car_is_tire_burst(lua_State* L); // Авто шина прокол
int is_car_visibly_damaged(lua_State* L); // проверить авто видимо повреждён
int car_waiting_collision(lua_State* L); // Авто ожидание столкновение
int car_is_door_closed(lua_State* L); // Авто дверь закрыто
int is_car_on_trailer(lua_State* L); // проверить авто прицеп
int car_damaged_by_car(lua_State* L); // Авто повреждён авто
int car_damaged_by_ped(lua_State* L); // Авто повреждён пешеход
int car_just_sunk(lua_State* L); // Авто только затонул
int car_drowning(lua_State* L); // Авто утопление
int car_has_weapon(lua_State* L); // Авто имеет оружие
int car_any_bomb(lua_State* L); // Авто бомба
int car_has_bomb(lua_State* L); // Авто имеет бомба
int is_car_in_water(lua_State* L); // проверить авто вода
int fire_button_pressed(lua_State* L); // стрелять кнопка нажат
int is_car_alive(lua_State* L); // проверить авто жив
int does_vehicle_exist(lua_State* L); // Автомобиль существовать
int is_car_in_garage(lua_State* L); // проверить авто гараж
int was_ever_police(lua_State* L); // Когда-либо полиция

// ОЧИСТКА
int clear_car_dmg(lua_State* L); // очистить авто урон
int clear_car_wep_dmg(lua_State* L); // очистить авто оружие урон
int remove_stuck_check(lua_State* L); // удалить застрял проверить
int remove_upside_check(lua_State* L); // удалить вверх дном проверить
int add_upside_check(lua_State* L); // добавить вверх дном проверить
int unlock_doors_area(lua_State* L); // разблокировать двери область

// ДВЕРИ/БАГАЖНИК
int close_all_doors(lua_State* L); // закрыть двери
int close_car_boot(lua_State* L); // закрыть авто багажник
int pop_car_boot(lua_State* L); // Всплытие авто багажник
int pop_car_boot_physics(lua_State* L); // Всплытие авто багажник физика
int lock_doors(lua_State* L); // заблокировать двери
int change_car_lock(lua_State* L); // изменить авто lock

// ПЕД/ВОДИТЕЛЬ
int get_car_driver_ref(lua_State* L); // получить авто водитель ссылка
int get_char_in_seat(lua_State* L); // получить персонаж сиденье
int order_driver_out(lua_State* L); // Порядок водитель снаружи
int set_ped_max_drive_speed(lua_State* L); // установить пешеход максимум ездить скорость
int set_ped_drive_aggression(lua_State* L); // установить пешеход ездить aggression
int shuffle_to_driver(lua_State* L); // Перемешать водитель
int stay_when_jacked(lua_State* L); // Stay угнан
int is_ped_in_police_car(lua_State* L); // проверить пешеход полиция авто
int is_ped_touching_vehicle(lua_State* L); // проверить пешеход касание автомобиль
int is_ped_leaving_to_die(lua_State* L); // проверить пешеход выход умереть
int is_ped_drowning(lua_State* L); // проверить пешеход утопление
int is_ped_in_water(lua_State* L); // проверить пешеход вода
int is_ped_in_zone(lua_State* L); // проверить пешеход зона
int is_player_in_zone(lua_State* L); // проверить игрок зона

// ТЕЛЕПОРТАЦИЯ
int warp_ped_from_car(lua_State* L); // Телепорт пешеход авто
int warp_ped_passenger(lua_State* L); // Телепорт пешеход пассажир
int warp_player_from_car(lua_State* L); // Телепорт игрок авто

// ОРУЖИЕ ТРАНСПОРТА
int set_car_ammo(lua_State* L); // установить авто патроны
int switch_car_weapons(lua_State* L); // Переключатель авто оружия
int set_car_weapon(lua_State* L); // установить авто оружие
int fire_hunter_gun(lua_State* L); // стрелять охотник пушка
int set_tank_detonate(lua_State* L); // установить танк подорвать
int ensure_drive_by(lua_State* L); // Обеспечить ездить

// БОМБЫ/СИГНАЛИЗАЦИЯ
int arm_car_bomb(lua_State* L); // Вооружить авто бомба
int disarm_car_bomb(lua_State* L); // Обезоружить авто бомба
int give_car_alarm(lua_State* L); // дать авто сигнализация

// ДОПОЛНИТЕЛЬНЫЕ
int switch_car_siren(lua_State* L); // Переключатель авто сирена
int set_driving_style_op(lua_State* L); // установить вождение стиль операция
int set_char_obj_wait(lua_State* L); // нет задач (ждёт на ногах)
int set_char_obj_flee_coord(lua_State* L); // убежать от точки пешком до безопасности
int set_char_obj_guard_spot(lua_State* L); // охранять точку
int set_char_obj_guard_area(lua_State* L); // охранять зону (радиус)
int set_char_obj_wait_in_car(lua_State* L); // ждать в машине
int set_char_obj_leave_car(lua_State* L); // выйти из машины
int set_char_obj_enter_car_passenger(lua_State* L); // сесть в авто как пассажир
int set_char_obj_enter_car_driver(lua_State* L); // сесть в авто как водитель
int set_char_obj_follow_car_in_car(lua_State* L); // следовать за авто из авто
int set_char_obj_destroy_object(lua_State* L); // уничтожить объект
int set_char_obj_goto_area_in_car(lua_State* L); // ехать к зоне
int set_char_obj_follow_car_foot_offset(lua_State* L); // следовать за авто пешком смещение
int set_char_obj_guard_attack(lua_State* L); // охрана — атаковать противника
int set_char_obj_goto_coord_on_foot(lua_State* L); // идти к координатам пешком
int set_char_obj_goto_coord_in_car(lua_State* L); // ехать к координатам
int set_char_obj_run_to_coord(lua_State* L); // бежать к координатам
int set_char_obj_goto_area_any_means(lua_State* L); // добираться любыми средствами
int set_char_obj_follow_formation(lua_State* L); // следовать в строю за лидером
int set_char_obj_hail_taxi(lua_State* L); // вызвать такси
int set_char_obj_steal_any_car(lua_State* L); // украсть любую машину
int set_char_obj_leave_any_car(lua_State* L); // выйти из любого авто
int set_char_obj_flee_car(lua_State* L); // убежать от машины
int set_char_obj_walk_to_char(lua_State* L); // идти к другому педу
int set_char_obj_steal_any_car_mission(lua_State* L); // украсть любую миссионную машину
int set_char_obj_sprint_to_coord(lua_State* L); // спринт к координатам
int set_char_obj_goto_car_on_foot(lua_State* L); // идти к машине пешком
int get_projectile_count(lua_State* L); // число активных снарядов
int get_projectile_weapon(lua_State* L); // тип оружия снаряда
int get_projectile_source(lua_State* L); // источник снаряда (указатель)
int get_projectile_pos(lua_State* L); // получить projectile позиция
int set_projectile_pos(lua_State* L); // уст позицию снаряда (направление ракеты)
int is_projectile_in_use(lua_State* L); // слот снаряда занят
int remove_all_projectiles(lua_State* L); // удалить все снаряды
int is_projectile_in_area(lua_State* L); // снаряд в зоне (6 координат)
int destroy_projectiles_in_area(lua_State* L); // уничтожить снаряды в зоне
int add_projectile(lua_State* L); // создать снаряд (источник, оружие, x,y,z, сила)
int find_car_by_model(lua_State* L); // найти первое авто по модели
int find_ped_by_model(lua_State* L); // найти первого педа по модели
int find_object_by_model(lua_State* L); // найти объект по модели
int get_vehicle_pool_size(lua_State* L); // размер пула транспорта
int get_car_pool_index(lua_State* L); // индекс авто в пуле
int get_car_by_pool_index(lua_State* L); // авто по индексу в пуле
int get_car_velocity(lua_State* L); // получить авто скорость
int set_car_velocity(lua_State* L); // установить авто скорость
int remove_weapon_from_char(lua_State* L); // удалить оружие персонаж
int get_forced_weather_type(lua_State* L); // принудительный тип погоды
int is_lightning_burst(lua_State* L); // проверить молния прокол
int set_char_obj_kill_char_on_foot(lua_State* L); // убить педа пешком (ближний бой)
int set_char_obj_kill_char_any_means(lua_State* L); // убить педа любыми средствами
int set_char_obj_flee_char_on_foot_till_safe(lua_State* L); // убежать от педа до безопасного места
int set_char_obj_flee_char_on_foot_always(lua_State* L); // всегда убегать от педа
int set_char_obj_goto_char_on_foot(lua_State* L); // идти к педу пешком
int set_char_obj_aim_gun_at_char(lua_State* L); // целиться из оружия в педа
int set_char_obj_kill_player_on_foot(lua_State* L); // убить игрока пешком
int set_char_obj_kill_player_any_means(lua_State* L); // убить игрока любыми средствами
int set_char_obj_flee_player_on_foot_till_safe(lua_State* L); // убежать от игрока до безопасного места
int set_char_obj_flee_player_on_foot_always(lua_State* L); // всегда убегать от игрока
int set_char_obj_goto_player_on_foot(lua_State* L); // идти к игроку пешком
int set_char_obj_no_obj(lua_State* L); // снять задачу с педа
int set_char_obj_catch_train(lua_State* L); // сесть на поезд
int set_char_obj_buy_ice_cream(lua_State* L); // купить мороженое
int set_char_obj_goto_area_on_foot(lua_State* L); // идти к зоне пешком
int set_char_obj_destroy_car(lua_State* L); // уничтожить машину
int set_char_obj_fire_at_object_from_vehicle(lua_State* L); // стрелять по объекту из авто
int set_char_obj_follow_route(lua_State* L); // следовать по маршруту
int get_heli_forced_z(lua_State* L); // принудительная высота вертолёта (зависание)
int set_heli_forced_z(lua_State* L); // уст. принудительная высота вертолёта (зависание)
int get_heli_searchlight_x(lua_State* L); // получить вертолёт searchlight X
int set_heli_searchlight_x(lua_State* L); // установить вертолёт searchlight X
int get_heli_searchlight_y(lua_State* L); // получить вертолёт searchlight Y
int set_heli_searchlight_y(lua_State* L); // установить вертолёт searchlight Y
int get_heli_searchlight_intensity(lua_State* L); // яркость прожектора вертолёта (0..1)
int set_heli_searchlight_intensity(lua_State* L); // уст. яркость прожектора вертолёта (0..1)
int get_heli_rotor_angle(lua_State* L); // угол вращения винта вертолёта
int set_heli_rotor_angle(lua_State* L); // уст. угол вращения винта вертолёта
int heli_send_down_swat(lua_State* L); // спецназ спускается с вертолёта на верёвке
int heli_get_num_random(lua_State* L); // число случайных вертолётов
int heli_set_num_random(lua_State* L); // уст. число случайных вертолётов
int heli_is_catalina_on(lua_State* L); // вертолёт Катарины активен
int heli_set_catalina_on(lua_State* L); // вкл/выкл вертолёт Катарины
int get_ped_velocity(lua_State* L); // получить пешеход скорость
int set_ped_velocity(lua_State* L); // установить пешеход скорость
int get_object_velocity(lua_State* L); // получить объект скорость
int set_object_velocity(lua_State* L); // установить объект скорость
int get_car_speed(lua_State* L); // скалярная скорость авто (м/с)
int get_ped_speed(lua_State* L); // скалярная скорость педа (м/с)
int get_object_speed(lua_State* L); // скалярная скорость объекта (м/с)
int add_ammo_to_char(lua_State* L); // добавить патронов педу в оружие
int set_char_ammo(lua_State* L); // установить патроны педу в оружие
int get_ammo_in_char_weapon(lua_State* L); // патроны в оружии педа
int get_char_weapon_in_slot(lua_State* L); // оружие в слоте педа
int set_player_fast_reload(lua_State* L); // быстрая перезарядка игроку
int is_char_shooting_in_area(lua_State* L); // пед стреляет в зоне
int set_char_stop_shoot_dont_seek_entity(lua_State* L); // пед перестаёт стрелять и не ищет цель
int add_bullet_trace_radius(lua_State* L); // след пули: от точки к точке (радиус, время, прозрачность)
int add_bullet_trace_weapon(lua_State* L); // след пули: оружием и сущностью
int get_bullet_trace_count(lua_State* L); // число активных следов пуль
int get_bullet_trace_origin(lua_State* L); // точка начала следа пули
int get_bullet_trace_target(lua_State* L); // точка конца следа пули
int clear_bullet_traces(lua_State* L); // удалить все следы пуль
int place_3d_marker(lua_State* L); // Место 3d маркер
int place_3d_marker_set(lua_State* L); // установить место 3D маркер
int get_num_active_3d_markers(lua_State* L); // получить активный 3D markers
int get_pedstat_m_fFleeDistance(lua_State* L); // дистанция бегства педа
int set_pedstat_m_fFleeDistance(lua_State* L); // уст. дистанция бегства педа
int get_pedstat_m_fHeadingChangeRate(lua_State* L); // скорость поворота педа
int set_pedstat_m_fHeadingChangeRate(lua_State* L); // уст. скорость поворота педа
int get_pedstat_m_fAttackStrength(lua_State* L); // сила атаки педа
int set_pedstat_m_fAttackStrength(lua_State* L); // уст. сила атаки педа
int get_pedstat_m_fDefendWeakness(lua_State* L); // слабость защиты педа
int set_pedstat_m_fDefendWeakness(lua_State* L); // уст. слабость защиты педа
int get_car_door_status(lua_State* L); // состояние двери авто
int set_car_door_status(lua_State* L); // уст. состояние двери авто
int get_car_panel_status(lua_State* L); // состояние панели авто
int get_car_light_status(lua_State* L); // состояние фары авто
int set_car_light_status(lua_State* L); // уст. состояние фары авто
int get_car_wheel_status(lua_State* L); // состояние колеса авто
int set_car_wheel_status(lua_State* L); // уст. состояние колеса авто
int get_car_engine_status(lua_State* L); // состояние двигателя авто
int set_car_engine_status(lua_State* L); // уст. состояние двигателя авто
int apply_car_damage(lua_State* L); // повредить компонент авто
int fuck_car_completely(lua_State* L); // уничтожить авто визуально
int reset_car_damage(lua_State* L); // сбросить повреждения авто
int add_AutoPaintingBudget(lua_State* L); // бюджет на покраску авто
int add_PropertyBudget(lua_State* L); // бюджет на недвижимость
int add_FashionBudget(lua_State* L); // бюджет на одежду
int add_WeaponBudget(lua_State* L); // бюджет на оружие
int add_BloodRingKills(lua_State* L); // добавить кровь кольцо убийства
int add_PizzasDelivered(lua_State* L); // доставленные пиццы
int add_Assassinations(lua_State* L); // заказные убийства
int add_MovieStunts(lua_State* L); // киноскунты
int add_LoanSharks(lua_State* L); // визиты к ростовщикам
int clear_all_char_anims(lua_State* L); // очистить все анимации педа
int clear_char_follow_path(lua_State* L); // очистить маршрут педа
int clear_char_last_damage_entity(lua_State* L); // забыть последнюю повредившую сущность
int clear_char_last_weapon_damage(lua_State* L); // забыть последнее оружие урона
int char_set_idle(lua_State* L); // пед в режим ожидания
int add_armour_to_char(lua_State* L); // добавить брони педу
int add_armour_to_player(lua_State* L); // добавить брони игроку
int add_ammo_to_player(lua_State* L); // добавить патронов игроку
int clear_area_of_cars(lua_State* L); // очистить зону от машин
int clear_area_of_chars(lua_State* L); // очистить зону от педов
int get_population_total_peds(lua_State* L); // всего педов в мире
int get_population_civ_peds(lua_State* L); // мирных педов
int get_population_gang_peds(lua_State* L); // бандитских педов
int get_population_car_passengers(lua_State* L); // пассажиров в авто
int get_population_mission_peds(lua_State* L); // миссионных педов
int get_population_civ_male(lua_State* L); // мужчин-мирных
int get_population_civ_female(lua_State* L); // женщин-мирных
int get_population_max_in_use(lua_State* L); // макс. число используемых педов
int get_population_countdown(lua_State* L); // обратный отсчёт до спавна педов
int get_population_all_random_type(lua_State* L); // тип всех случайных педов
int set_population_all_random_type(lua_State* L); // уст. тип всех случайных педов
int get_hud_m_WantedTimer(lua_State* L); // получить HUD розыск таймер
int set_hud_m_WantedTimer(lua_State* L); // установить HUD розыск таймер
int get_hud_m_WeaponFadeTimer(lua_State* L); // получить поле nFadeTimer (затемнение, таймер)
int set_hud_m_WeaponFadeTimer(lua_State* L); // установить поле nFadeTimer (затемнение, таймер)
int get_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int set_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int get_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int set_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int get_hud_wanted_state(lua_State* L); // получить HUD розыск состояние
int get_hud_help_message_state(lua_State* L); // получить HUD помощь сообщение состояние
int get_player_taxi_timer(lua_State* L); // таймер такси игрока
int get_player_death_fade_timer(lua_State* L); // таймер затухания при смерти
int get_player_2wheels_time(lua_State* L); // время на двух колёсах (мс)
int get_player_2wheels_distance(lua_State* L); // дистанция на двух колёсах
int get_player_road_density(lua_State* L); // плотность дорог вокруг игрока
int set_player_road_density(lua_State* L); // уст. плотность дорог вокруг игрока
int get_pedik_m_fYaw(lua_State* L); // угол наклона головы педа
int set_pedik_m_fYaw(lua_State* L); // уст. угол наклона головы педа
int get_pedik_m_fPitch(lua_State* L); // наклон головы педа по тангажу
int set_pedik_m_fPitch(lua_State* L); // уст. наклон головы педа по тангажу
int get_boat_m_fPropRotation(lua_State* L); // вращение винта лодки
int set_boat_m_fPropRotation(lua_State* L); // уст. вращение винта лодки
int get_boat_m_fPropSpeed(lua_State* L); // скорость винта лодки
int set_boat_m_fPropSpeed(lua_State* L); // уст. скорость винта лодки
int get_boat_m_fForcedZRotation(lua_State* L); // получить поле fForcedZRotation (принудительно, Z, вращение)
int set_boat_m_fForcedZRotation(lua_State* L); // установить поле fForcedZRotation (принудительно, Z, вращение)
int get_boat_m_fBoatGasPedal(lua_State* L); // газ лодки
int set_boat_m_fBoatGasPedal(lua_State* L); // уст. газ лодки
int get_boat_m_fBoatSteeringLeftRight(lua_State* L); // руль лодки
int set_boat_m_fBoatSteeringLeftRight(lua_State* L); // уст. руль лодки
int get_bike_anim_group(lua_State* L); // группа анимаций мотоцикла
int set_bike_anim_group(lua_State* L); // уст. группу анимаций мотоцикла
int get_bike_damage_flags(lua_State* L); // флаги повреждений мотоцикла
int set_bike_damage_flags(lua_State* L); // уст. флаги повреждений мотоцикла
int get_time_step_non_clipped(lua_State* L); // шаг времени без клиппинга
int get_previous_time_ms(lua_State* L); // предыдущее время в мс
int get_time_ms_non_clipped(lua_State* L); // время в мс без клиппинга
int is_slow_motion_active(lua_State* L); // активен ли слоумоушен
int get_cycles_per_millisecond(lua_State* L); // циклы на миллисекунду
int get_clock_hours(lua_State* L); // часы игрового таймера
int get_clock_minutes(lua_State* L); // минуты игрового таймера
int get_clock_seconds(lua_State* L); // секунды игрового таймера
int get_clock_stored_hours(lua_State* L); // сохранённые часы
int get_clock_stored_minutes(lua_State* L); // сохранённые минуты
int get_clock_minutes_until(lua_State* L); // минут до заданного времени
int restore_clock(lua_State* L); // восстановить сохранённое время
int get_streaming_disabled(lua_State* L); // стриминг отключён
int set_streaming_disabled(lua_State* L); // отключить стриминг моделей
int get_streaming_models_requested(lua_State* L); // запрошено моделей
int get_streaming_priority_requests(lua_State* L); // приоритетных запросов
int get_streaming_image_size(lua_State* L); // размер образа стриминга
int get_streaming_buffer_size(lua_State* L); // размер буфера стриминга
int get_streaming_current_ped_grp(lua_State* L); // текущая группа педов
int get_streaming_loading_big_model(lua_State* L); // загружается большая модель
int get_streaming_peds_loaded(lua_State* L); // загружено педов
int get_streaming_channel_error(lua_State* L); // ошибка канала стриминга
int get_streaming_last_image_read(lua_State* L); // последний прочитанный образ
int get_streaming_old_sector_x(lua_State* L); // получить streaming старый sector X
int get_streaming_old_sector_y(lua_State* L); // получить streaming старый sector Y
int get_zone_level(lua_State* L); // остров по координатам
int find_information_zone(lua_State* L); // индекс информационной зоны
int get_navigation_zone(lua_State* L); // навигационная зона по индексу
int init_zones(lua_State* L); // инициализировать зоны
int traffic_lights_for_peds(lua_State* L); // светофор для пешеходов
int traffic_scan_for_lights(lua_State* L); // сканировать светофоры
int traffic_should_car_stop_for_bridge(lua_State* L); // машина должна остановиться у моста
int should_car_stop_for_light(lua_State* L); // должна ли машина остановиться на свет
int add_gun_flash_big(lua_State* L); // вспышка выстрела крупного оружия
int initialise_weapons(lua_State* L); // инициализировать оружие
int shutdown_weapons(lua_State* L); // завершить работу оружия
int update_weapons(lua_State* L); // обновить оружие
int do_doom_aiming(lua_State* L); // аим-траверс для оружия
int create_some_money(lua_State* L); // создать деньги на координатах
int give_player_goodies(lua_State* L); // выдать игроку бонусы по модели
int do_pickup_effects(lua_State* L); // эффект поднятия пикапа
int do_money_effects(lua_State* L); // эффект денег
int do_mine_effects(lua_State* L); // эффект мины
int get_obj_m_fAttachForce(lua_State* L); // сила привязки объекта
int set_obj_m_fAttachForce(lua_State* L); // уст. сила привязки объекта
int get_obj_m_CollisionDamageType(lua_State* L); // тип урона от столкновений
int set_obj_m_CollisionDamageType(lua_State* L); // уст. тип урона от столкновений
int get_obj_m_nSpecialCollisionType(lua_State* L); // специальный тип столкновений
int set_obj_m_nSpecialCollisionType(lua_State* L); // уст. специальный тип столкновений
int get_obj_m_nBounceScore(lua_State* L); // счёт отскока объекта
int set_obj_m_nBounceScore(lua_State* L); // уст. счёт отскока объекта
int get_obj_m_dwObjectTimer(lua_State* L); // таймер объекта
int set_obj_m_dwObjectTimer(lua_State* L); // уст. таймер объекта
int get_radar_range(lua_State* L); // дальность радара
int set_radar_range(lua_State* L); // уст. дальность радара
int set_sun_blocked_by_clouds(lua_State* L); // уст. солнце закрыто облаками
int update_corona_coors(lua_State* L); // обновить координаты короны
int is_cutscene_running(lua_State* L); // катсцена идёт
int get_num_cutscene_objs(lua_State* L); // число объектов катсцены
int get_cutscene_time_ms(lua_State* L); // время катсцены в мс
int choose_police_car_model(lua_State* L); // выбрать модель полицейской машины
int generate_emergency_services(lua_State* L); // вызвать экстренные службы
int clear_interesting_vehicles(lua_State* L); // очистить список интересных машин
int drag_car_to_point(lua_State* L); // тащить машину к точке
int find_max_speed_in_traffic(lua_State* L); // макс. скорость машины в трафике
int get_num_bright_lights(lua_State* L); // число ярких огней
int get_car_m_nPrimaryColor(lua_State* L); // m_nPrimaryColor (CVehicle) — получить поле nPrimaryColor (основной, цвет) (CVehicle)
int set_car_m_nPrimaryColor(lua_State* L); // установить поле nPrimaryColor (основной, цвет)
int get_car_m_nSecondaryColor(lua_State* L); // m_nSecondaryColor (CVehicle) — получить поле nSecondaryColor (дополнительный, цвет) (CVehicle)
int set_car_m_nSecondaryColor(lua_State* L); // установить поле nSecondaryColor (дополнительный, цвет)
int get_car_m_wWantedStarsOnEnter(lua_State* L); // m_wWantedStarsOnEnter (CVehicle) — получить поле wWantedStarsOnEnter (розыск, звёзды розыска, войти) (CVehicle)
int set_car_m_wWantedStarsOnEnter(lua_State* L); // установить поле wWantedStarsOnEnter (розыск, звёзды розыска, войти)
int get_car_m_wMissionValue(lua_State* L); // m_wMissionValue (CVehicle) — получить поле wMissionValue (миссия, значение) (CVehicle)
int set_car_m_wMissionValue(lua_State* L); // установить поле wMissionValue (миссия, значение)
int get_car_m_nNumPassengers(lua_State* L); // m_nNumPassengers (CVehicle) — получить поле nNumPassengers (пассажиры) (CVehicle)
int set_car_m_nNumPassengers(lua_State* L); // установить поле nNumPassengers (пассажиры)
int get_car_m_nNumGettingIn(lua_State* L); // m_nNumGettingIn (CVehicle) — получить поле nNumGettingIn (получение) (CVehicle)
int set_car_m_nNumGettingIn(lua_State* L); // установить поле nNumGettingIn (получение)
int get_car_m_nGettingInFlags(lua_State* L); // m_nGettingInFlags (CVehicle) — получить поле nGettingInFlags (получение, флаги) (CVehicle)
int set_car_m_nGettingInFlags(lua_State* L); // установить поле nGettingInFlags (получение, флаги)
int get_car_m_nGettingOutFlags(lua_State* L); // m_nGettingOutFlags (CVehicle) — получить поле nGettingOutFlags (получение, снаружи, флаги) (CVehicle)
int set_car_m_nGettingOutFlags(lua_State* L); // установить поле nGettingOutFlags (получение, снаружи, флаги)
int get_car_m_nMaxPassengers(lua_State* L); // m_nMaxPassengers (CVehicle) — получить поле nMaxPassengers (максимум, пассажиры) (CVehicle)
int set_car_m_nMaxPassengers(lua_State* L); // установить поле nMaxPassengers (максимум, пассажиры)
int get_car_m_fSteerAngle(lua_State* L); // m_fSteerAngle (CVehicle) — получить поле fSteerAngle (руль, угол) (CVehicle)
int set_car_m_fSteerAngle(lua_State* L); // установить поле fSteerAngle (руль, угол)
int get_car_m_fGasPedal(lua_State* L); // m_fGasPedal (CVehicle) — получить поле fGasPedal (газ, педаль) (CVehicle)
int set_car_m_fGasPedal(lua_State* L); // установить поле fGasPedal (газ, педаль)
int get_car_m_fBreakPedal(lua_State* L); // m_fBreakPedal (CVehicle) — получить поле fBreakPedal (тормоз, педаль) (CVehicle)
int set_car_m_fBreakPedal(lua_State* L); // установить поле fBreakPedal (тормоз, педаль)
int get_car_m_nCreatedBy(lua_State* L); // m_nCreatedBy (CVehicle) — получить поле nCreatedBy (создано) (CVehicle)
int set_car_m_nCreatedBy(lua_State* L); // установить поле nCreatedBy (создано)
int get_car_m_nAmmoInClip(lua_State* L); // m_nAmmoInClip (CVehicle) — получить поле nAmmoInClip (патроны, обойма) (CVehicle)
int set_car_m_nAmmoInClip(lua_State* L); // установить поле nAmmoInClip (патроны, обойма)
int get_car_m_fHealth(lua_State* L); // m_fHealth (CVehicle) — получить поле fHealth (здоровье) (CVehicle)
int set_car_m_fHealth(lua_State* L); // установить поле fHealth (здоровье)
int get_car_m_nCurrentGear(lua_State* L); // m_nCurrentGear (CVehicle) — получить поле nCurrentGear (текущий, передача) (CVehicle)
int set_car_m_nCurrentGear(lua_State* L); // установить поле nCurrentGear (текущий, передача)
int get_car_m_nTimeTillWeNeedThisCar(lua_State* L); // m_nTimeTillWeNeedThisCar (CVehicle) — получить поле nTimeTillWeNeedThisCar (время, до, мы, нужно, авто) (CVehicle)
int set_car_m_nTimeTillWeNeedThisCar(lua_State* L); // установить поле nTimeTillWeNeedThisCar (время, до, мы, нужно, авто)
int get_car_m_nTimeOfDeath(lua_State* L); // m_nTimeOfDeath (CVehicle) — получить поле nTimeOfDeath (время, смерть) (CVehicle)
int set_car_m_nTimeOfDeath(lua_State* L); // установить поле nTimeOfDeath (время, смерть)
int get_car_m_wBombTimer(lua_State* L); // m_wBombTimer (CVehicle) — получить поле wBombTimer (бомба, таймер) (CVehicle)
int set_car_m_wBombTimer(lua_State* L); // установить поле wBombTimer (бомба, таймер)
int get_car_m_nLastWeaponDamage(lua_State* L); // m_nLastWeaponDamage (CVehicle) — получить поле nLastWeaponDamage (последний, оружие, повреждение) (CVehicle)
int set_car_m_nLastWeaponDamage(lua_State* L); // установить поле nLastWeaponDamage (последний, оружие, повреждение)
int get_car_m_nRadioStation(lua_State* L); // m_nRadioStation (CVehicle) — получить поле nRadioStation (радио, станция) (CVehicle)
int set_car_m_nRadioStation(lua_State* L); // установить поле nRadioStation (радио, станция)
int get_car_m_bHornEnabled(lua_State* L); // m_bHornEnabled (CVehicle) — получить поле bHornEnabled (гудок, включено) (CVehicle)
int set_car_m_bHornEnabled(lua_State* L); // установить поле bHornEnabled (гудок, включено)
int get_car_m_nSirenOrAlarm(lua_State* L); // m_nSirenOrAlarm (CVehicle) — получить поле nSirenOrAlarm (сирена, сигнализация) (CVehicle)
int set_car_m_nSirenOrAlarm(lua_State* L); // установить поле nSirenOrAlarm (сирена, сигнализация)
int get_car_m_nSirenExtra(lua_State* L); // m_nSirenExtra (CVehicle) — получить поле nSirenExtra (сирена, дополнительный) (CVehicle)
int set_car_m_nSirenExtra(lua_State* L); // установить поле nSirenExtra (сирена, дополнительный)
int get_car_m_fSteerRatio(lua_State* L); // m_fSteerRatio (CVehicle) — получить поле fSteerRatio (руль, коэффициент) (CVehicle)
int set_car_m_fSteerRatio(lua_State* L); // установить поле fSteerRatio (руль, коэффициент)
int get_car_m_nVehicleClass(lua_State* L); // m_nVehicleClass (CVehicle) — получить поле nVehicleClass (автомобиль, класс) (CVehicle)
int set_car_m_nVehicleClass(lua_State* L); // установить поле nVehicleClass (автомобиль, класс)
int get_ped_m_fCollisionSpeed(lua_State* L); // m_fCollisionSpeed (CPed) — получить поле fCollisionSpeed (столкновение, скорость) (CPed)
int set_ped_m_fCollisionSpeed(lua_State* L); // установить поле fCollisionSpeed (столкновение, скорость)
int get_ped_m_nGangFlags(lua_State* L); // m_nGangFlags (CPed) — получить поле nGangFlags (банда, флаги) (CPed)
int set_ped_m_nGangFlags(lua_State* L); // установить поле nGangFlags (банда, флаги)
int get_ped_m_nPedStatus(lua_State* L); // m_nPedStatus (CPed) — получить поле nPedStatus (пешеход, статус) (CPed)
int set_ped_m_nPedStatus(lua_State* L); // установить поле nPedStatus (пешеход, статус)
int get_ped_m_vecObjective(lua_State* L); // m_vecObjective (CPed) (x,y,z) — получить поле vecObjective (цель) (CPed)
int set_ped_m_vecObjective(lua_State* L); // установить поле vecObjective (цель)
int get_ped_m_fObjectiveAngle(lua_State* L); // m_fObjectiveAngle (CPed) — получить поле fObjectiveAngle (цель, угол) (CPed)
int set_ped_m_fObjectiveAngle(lua_State* L); // установить поле fObjectiveAngle (цель, угол)
int get_ped_m_nPedFormation(lua_State* L); // m_nPedFormation (CPed) — получить поле nPedFormation (пешеход, формация) (CPed)
int set_ped_m_nPedFormation(lua_State* L); // установить поле nPedFormation (пешеход, формация)
int get_ped_m_nFearFlags(lua_State* L); // m_nFearFlags (CPed) — получить поле nFearFlags (страх, флаги) (CPed)
int set_ped_m_nFearFlags(lua_State* L); // установить поле nFearFlags (страх, флаги)
int get_ped_m_nEventType(lua_State* L); // m_nEventType (CPed) — получить поле nEventType (событие, тип) (CPed)
int set_ped_m_nEventType(lua_State* L); // установить поле nEventType (событие, тип)
int get_ped_m_fAngleToEvent(lua_State* L); // m_fAngleToEvent (CPed) — получить поле fAngleToEvent (угол, событие) (CPed)
int set_ped_m_fAngleToEvent(lua_State* L); // установить поле fAngleToEvent (угол, событие)
int get_ped_m_nAnimGroupId(lua_State* L); // m_nAnimGroupId (CPed) — получить поле nAnimGroupId (анимация, группа) (CPed)
int set_ped_m_nAnimGroupId(lua_State* L); // установить поле nAnimGroupId (анимация, группа)
int get_ped_m_vecOffsetSeek(lua_State* L); // m_vecOffsetSeek (CPed) (x,y,z) — получить поле vecOffsetSeek (смещение, искать) (CPed)
int set_ped_m_vecOffsetSeek(lua_State* L); // установить поле vecOffsetSeek (смещение, искать)
int get_ped_m_nActionTimer(lua_State* L); // m_nActionTimer (CPed) — получить поле nActionTimer (действие, таймер) (CPed)
int set_ped_m_nActionTimer(lua_State* L); // установить поле nActionTimer (действие, таймер)
int get_ped_m_nWaitState(lua_State* L); // m_nWaitState (CPed) — получить поле nWaitState (ждать, состояние) (CPed)
int set_ped_m_nWaitState(lua_State* L); // установить поле nWaitState (ждать, состояние)
int get_ped_m_nWaitTimer(lua_State* L); // m_nWaitTimer (CPed) — получить поле nWaitTimer (ждать, таймер) (CPed)
int set_ped_m_nWaitTimer(lua_State* L); // установить поле nWaitTimer (ждать, таймер)
int get_ped_m_wPathNodes(lua_State* L); // m_wPathNodes (CPed) — получить поле wPathNodes (путь, узлы) (CPed)
int set_ped_m_wPathNodes(lua_State* L); // установить поле wPathNodes (путь, узлы)
int get_ped_m_wCurPathNode(lua_State* L); // m_wCurPathNode (CPed) — получить поле wCurPathNode (текущий, путь, узел) (CPed)
int set_ped_m_wCurPathNode(lua_State* L); // установить поле wCurPathNode (текущий, путь, узел)
int get_ped_m_nPathNodeTimer(lua_State* L); // m_nPathNodeTimer (CPed) — получить поле nPathNodeTimer (путь, узел, таймер) (CPed)
int set_ped_m_nPathNodeTimer(lua_State* L); // установить поле nPathNodeTimer (путь, узел, таймер)
int get_ped_m_vecPathNextNode(lua_State* L); // m_vecPathNextNode (CPed) (x,y,z) — получить поле vecPathNextNode (путь, следующий, узел) (CPed)
int set_ped_m_vecPathNextNode(lua_State* L); // установить поле vecPathNextNode (путь, следующий, узел)
int get_ped_m_fPathNextNodeDir(lua_State* L); // m_fPathNextNodeDir (CPed) — получить поле fPathNextNodeDir (путь, следующий, узел, направление) (CPed)
int set_ped_m_fPathNextNodeDir(lua_State* L); // установить поле fPathNextNodeDir (путь, следующий, узел, направление)
int get_ped_m_nPathNodeType(lua_State* L); // m_nPathNodeType (CPed) — получить поле nPathNodeType (путь, узел, тип) (CPed)
int set_ped_m_nPathNodeType(lua_State* L); // установить поле nPathNodeType (путь, узел, тип)
int get_ped_m_fHealth(lua_State* L); // m_fHealth (CPed) — получить поле fHealth (здоровье) (CPed)
int set_ped_m_fHealth(lua_State* L); // установить поле fHealth (здоровье)
int get_ped_m_fArmour(lua_State* L); // m_fArmour (CPed) — получить поле fArmour (броня) (CPed)
int set_ped_m_fArmour(lua_State* L); // установить поле fArmour (броня)
int get_ped_m_nShadowUpdateTimer(lua_State* L); // m_nShadowUpdateTimer (CPed) — получить поле nShadowUpdateTimer (тень, обновить, таймер) (CPed)
int set_ped_m_nShadowUpdateTimer(lua_State* L); // установить поле nShadowUpdateTimer (тень, обновить, таймер)
int get_ped_wRouteLastPoint(lua_State* L); // wRouteLastPoint (CPed) — получить поле wRouteLastPoint (маршрут, последний, точка) (CPed)
int set_ped_wRouteLastPoint(lua_State* L); // установить поле wRouteLastPoint (маршрут, последний, точка)
int get_ped_wRoutePoints(lua_State* L); // wRoutePoints (CPed) — получить поле wRoutePoints (маршрут, очки) (CPed)
int set_ped_wRoutePoints(lua_State* L); // установить поле wRoutePoints (маршрут, очки)
int get_ped_wRoutePos(lua_State* L); // wRoutePos (CPed) — получить поле wRoutePos (маршрут, позиция) (CPed)
int set_ped_wRoutePos(lua_State* L); // установить поле wRoutePos (маршрут, позиция)
int get_ped_wRouteType(lua_State* L); // wRouteType (CPed) — получить поле wRouteType (маршрут, тип) (CPed)
int set_ped_wRouteType(lua_State* L); // установить поле wRouteType (маршрут, тип)
int get_ped_wRouteCurDir(lua_State* L); // wRouteCurDir (CPed) — получить поле wRouteCurDir (маршрут, текущий, направление) (CPed)
int set_ped_wRouteCurDir(lua_State* L); // установить поле wRouteCurDir (маршрут, текущий, направление)
int get_ped_m_fHeadingCurrent(lua_State* L); // m_fHeadingCurrent (CPed) — получить поле fHeadingCurrent (курс, текущий) (CPed)
int set_ped_m_fHeadingCurrent(lua_State* L); // установить поле fHeadingCurrent (курс, текущий)
int get_ped_m_fHeadingGoal(lua_State* L); // m_fHeadingGoal (CPed) — получить поле fHeadingGoal (курс, цель) (CPed)
int set_ped_m_fHeadingGoal(lua_State* L); // установить поле fHeadingGoal (курс, цель)
int get_ped_m_fHeadingChangeRate(lua_State* L); // m_fHeadingChangeRate (CPed) — получить поле fHeadingChangeRate (курс, изменить, частота) (CPed)
int set_ped_m_fHeadingChangeRate(lua_State* L); // установить поле fHeadingChangeRate (курс, изменить, частота)
int get_ped_m_nEnterType(lua_State* L); // m_nEnterType (CPed) — получить поле nEnterType (войти, тип) (CPed)
int set_ped_m_nEnterType(lua_State* L); // установить поле nEnterType (войти, тип)
int get_ped_m_nWalkAroundType(lua_State* L); // m_nWalkAroundType (CPed) — получить поле nWalkAroundType (ходить, вокруг, тип) (CPed)
int set_ped_m_nWalkAroundType(lua_State* L); // установить поле nWalkAroundType (ходить, вокруг, тип)
int get_ped_m_vecOffsetFromPhysSurface(lua_State* L); // m_vecOffsetFromPhysSurface (CPed) (x,y,z) — получить поле vecOffsetFromPhysSurface (смещение, физика, поверхность) (CPed)
int set_ped_m_vecOffsetFromPhysSurface(lua_State* L); // установить поле vecOffsetFromPhysSurface (смещение, физика, поверхность)
int get_ped_vecSeekVehicle(lua_State* L); // vecSeekVehicle (CPed) (x,y,z) — получить поле vecSeekVehicle (искать, автомобиль) (CPed)
int set_ped_vecSeekVehicle(lua_State* L); // установить поле vecSeekVehicle (искать, автомобиль)
int get_ped_m_bInVehicle(lua_State* L); // m_bInVehicle (CPed) — получить поле bInVehicle (автомобиль) (CPed)
int set_ped_m_bInVehicle(lua_State* L); // установить поле bInVehicle (автомобиль)
int get_ped_m_fSeatPrecisionX(lua_State* L); // m_fSeatPrecisionX (CPed) — получить поле fSeatPrecisionX (сиденье, точность, X) (CPed)
int set_ped_m_fSeatPrecisionX(lua_State* L); // установить поле fSeatPrecisionX (сиденье, точность, X)
int get_ped_m_fSeatPrecisionY(lua_State* L); // m_fSeatPrecisionY (CPed) — получить поле fSeatPrecisionY (сиденье, точность, Y) (CPed)
int set_ped_m_fSeatPrecisionY(lua_State* L); // установить поле fSeatPrecisionY (сиденье, точность, Y)
int get_ped_m_nSeatType(lua_State* L); // m_nSeatType (CPed) — получить поле nSeatType (сиденье, тип) (CPed)
int set_ped_m_nSeatType(lua_State* L); // установить поле nSeatType (сиденье, тип)
int get_ped_m_bHasPhone(lua_State* L); // m_bHasPhone (CPed) — получить поле bHasPhone (имеет, телефон) (CPed)
int set_ped_m_bHasPhone(lua_State* L); // установить поле bHasPhone (имеет, телефон)
int get_ped_m_wPhoneId(lua_State* L); // m_wPhoneId (CPed) — получить поле wPhoneId (телефон) (CPed)
int set_ped_m_wPhoneId(lua_State* L); // установить поле wPhoneId (телефон)
int get_ped_m_nLookingForPhone(lua_State* L); // m_nLookingForPhone (CPed) — получить поле nLookingForPhone (смотрит, телефон) (CPed)
int set_ped_m_nLookingForPhone(lua_State* L); // установить поле nLookingForPhone (смотрит, телефон)
int get_ped_m_nPhoneTalkTimer(lua_State* L); // m_nPhoneTalkTimer (CPed) — получить поле nPhoneTalkTimer (телефон, разговор, таймер) (CPed)
int set_ped_m_nPhoneTalkTimer(lua_State* L); // установить поле nPhoneTalkTimer (телефон, разговор, таймер)
int get_ped_m_fFleeFromPosX(lua_State* L); // m_fFleeFromPosX (CPed) — получить поле fFleeFromPosX (побег, позиция, X) (CPed)
int set_ped_m_fFleeFromPosX(lua_State* L); // установить поле fFleeFromPosX (побег, позиция, X)
int get_ped_m_fFleeFromPosY(lua_State* L); // m_fFleeFromPosY (CPed) — получить поле fFleeFromPosY (побег, позиция, Y) (CPed)
int set_ped_m_fFleeFromPosY(lua_State* L); // установить поле fFleeFromPosY (побег, позиция, Y)
int get_ped_m_nFleeTimer(lua_State* L); // m_nFleeTimer (CPed) — получить поле nFleeTimer (побег, таймер) (CPed)
int set_ped_m_nFleeTimer(lua_State* L); // установить поле nFleeTimer (побег, таймер)
int get_ped_m_nLastThreatTimer(lua_State* L); // m_nLastThreatTimer (CPed) — получить поле nLastThreatTimer (последний, угроза, таймер) (CPed)
int set_ped_m_nLastThreatTimer(lua_State* L); // установить поле nLastThreatTimer (последний, угроза, таймер)
int get_ped_m_nStateUnused(lua_State* L); // m_nStateUnused (CPed) — получить поле nStateUnused (состояние, неиспользуемый) (CPed)
int set_ped_m_nStateUnused(lua_State* L); // установить поле nStateUnused (состояние, неиспользуемый)
int get_ped_m_nTimerUnused(lua_State* L); // m_nTimerUnused (CPed) — получить поле nTimerUnused (таймер, неиспользуемый) (CPed)
int set_ped_m_nTimerUnused(lua_State* L); // установить поле nTimerUnused (таймер, неиспользуемый)
int get_ped_m_nAtchStoredWep(lua_State* L); // m_nAtchStoredWep (CPed) — получить поле nAtchStoredWep (прикрепить, сохранено, оружие) (CPed)
int set_ped_m_nAtchStoredWep(lua_State* L); // установить поле nAtchStoredWep (прикрепить, сохранено, оружие)
int get_ped_m_nStoredGiveWep(lua_State* L); // m_nStoredGiveWep (CPed) — получить поле nStoredGiveWep (сохранено, дать, оружие) (CPed)
int set_ped_m_nStoredGiveWep(lua_State* L); // установить поле nStoredGiveWep (сохранено, дать, оружие)
int get_ped_m_nStoredGiveAmmo(lua_State* L); // m_nStoredGiveAmmo (CPed) — получить поле nStoredGiveAmmo (сохранено, дать, патроны) (CPed)
int set_ped_m_nStoredGiveAmmo(lua_State* L); // установить поле nStoredGiveAmmo (сохранено, дать, патроны)
int get_ped_m_nCurrentWeapon(lua_State* L); // m_nCurrentWeapon (CPed) — получить поле nCurrentWeapon (текущий, оружие) (CPed)
int set_ped_m_nCurrentWeapon(lua_State* L); // установить поле nCurrentWeapon (текущий, оружие)
int get_ped_m_nWepSkills(lua_State* L); // m_nWepSkills (CPed) — получить поле nWepSkills (оружие, навыки) (CPed)
int set_ped_m_nWepSkills(lua_State* L); // установить поле nWepSkills (оружие, навыки)
int get_ped_m_nWeaponAccuracy(lua_State* L); // m_nWeaponAccuracy (CPed) — получить поле nWeaponAccuracy (оружие, точность) (CPed)
int set_ped_m_nWeaponAccuracy(lua_State* L); // установить поле nWeaponAccuracy (оружие, точность)
int get_ped_m_nBodyPart(lua_State* L); // m_nBodyPart (CPed) — получить поле nBodyPart (кузов, часть) (CPed)
int set_ped_m_nBodyPart(lua_State* L); // установить поле nBodyPart (кузов, часть)
int get_ped_m_vecHitLastPos(lua_State* L); // m_vecHitLastPos (CPed) (x,y,z) — получить поле vecHitLastPos (попадание, последний, позиция) (CPed)
int set_ped_m_vecHitLastPos(lua_State* L); // установить поле vecHitLastPos (попадание, последний, позиция)
int get_ped_m_nHitCounter(lua_State* L); // m_nHitCounter (CPed) — получить поле nHitCounter (попадание, счётчик) (CPed)
int set_ped_m_nHitCounter(lua_State* L); // установить поле nHitCounter (попадание, счётчик)
int get_ped_m_nLastHitState(lua_State* L); // m_nLastHitState (CPed) — получить поле nLastHitState (последний, попадание, состояние) (CPed)
int set_ped_m_nLastHitState(lua_State* L); // установить поле nLastHitState (последний, попадание, состояние)
int get_ped_m_nFightFlags1(lua_State* L); // m_nFightFlags1 (CPed) — получить поле nFightFlags1 (драка, флаги 1) (CPed)
int set_ped_m_nFightFlags1(lua_State* L); // установить поле nFightFlags1 (драка, флаги 1)
int get_ped_m_nFightFlags2(lua_State* L); // m_nFightFlags2 (CPed) — получить поле nFightFlags2 (драка, флаги 2) (CPed)
int set_ped_m_nFightFlags2(lua_State* L); // установить поле nFightFlags2 (драка, флаги 2)
int get_ped_m_nFightFlags3(lua_State* L); // m_nFightFlags3 (CPed) — получить поле nFightFlags3 (драка, флаги 3) (CPed)
int set_ped_m_nFightFlags3(lua_State* L); // установить поле nFightFlags3 (драка, флаги 3)
int get_ped_m_nBleedCounter(lua_State* L); // m_nBleedCounter (CPed) — получить поле nBleedCounter (кровотечение, счётчик) (CPed)
int set_ped_m_nBleedCounter(lua_State* L); // установить поле nBleedCounter (кровотечение, счётчик)
int get_ped_m_fLookDirection(lua_State* L); // m_fLookDirection (CPed) — получить поле fLookDirection (смотреть, направление) (CPed)
int set_ped_m_fLookDirection(lua_State* L); // установить поле fLookDirection (смотреть, направление)
int get_ped_m_nWepModelID(lua_State* L); // m_nWepModelID (CPed) — получить поле nWepModelID (оружие, модель) (CPed)
int set_ped_m_nWepModelID(lua_State* L); // установить поле nWepModelID (оружие, модель)
int get_ped_m_nLeaveCarTimer(lua_State* L); // m_nLeaveCarTimer (CPed) — получить поле nLeaveCarTimer (оставить, авто, таймер) (CPed)
int set_ped_m_nLeaveCarTimer(lua_State* L); // установить поле nLeaveCarTimer (оставить, авто, таймер)
int get_ped_m_nGetUpTimer(lua_State* L); // m_nGetUpTimer (CPed) — получить поле nGetUpTimer (получить, таймер) (CPed)
int set_ped_m_nGetUpTimer(lua_State* L); // установить поле nGetUpTimer (получить, таймер)
int get_ped_m_nLookTimer(lua_State* L); // m_nLookTimer (CPed) — получить поле nLookTimer (смотреть, таймер) (CPed)
int set_ped_m_nLookTimer(lua_State* L); // установить поле nLookTimer (смотреть, таймер)
int get_ped_m_nStandardTimer(lua_State* L); // m_nStandardTimer (CPed) — получить поле nStandardTimer (стандартный, таймер) (CPed)
int set_ped_m_nStandardTimer(lua_State* L); // установить поле nStandardTimer (стандартный, таймер)
int get_ped_m_nAttackTimer(lua_State* L); // m_nAttackTimer (CPed) — получить поле nAttackTimer (атака, таймер) (CPed)
int set_ped_m_nAttackTimer(lua_State* L); // установить поле nAttackTimer (атака, таймер)
int get_ped_m_nLastHitTime(lua_State* L); // m_nLastHitTime (CPed) — получить поле nLastHitTime (последний, попадание, время) (CPed)
int set_ped_m_nLastHitTime(lua_State* L); // установить поле nLastHitTime (последний, попадание, время)
int get_ped_m_nHitRecoverTimer(lua_State* L); // m_nHitRecoverTimer (CPed) — получить поле nHitRecoverTimer (попадание, восстановить, таймер) (CPed)
int set_ped_m_nHitRecoverTimer(lua_State* L); // установить поле nHitRecoverTimer (попадание, восстановить, таймер)
int get_ped_m_nObjectiveTimer(lua_State* L); // m_nObjectiveTimer (CPed) — получить поле nObjectiveTimer (цель, таймер) (CPed)
int set_ped_m_nObjectiveTimer(lua_State* L); // установить поле nObjectiveTimer (цель, таймер)
int get_ped_m_nDuckTimer(lua_State* L); // m_nDuckTimer (CPed) — получить поле nDuckTimer (утка, таймер) (CPed)
int set_ped_m_nDuckTimer(lua_State* L); // установить поле nDuckTimer (утка, таймер)
int get_ped_m_nDuckAndCoverTimer(lua_State* L); // m_nDuckAndCoverTimer (CPed) — получить поле nDuckAndCoverTimer (утка, покрытие, таймер) (CPed)
int set_ped_m_nDuckAndCoverTimer(lua_State* L); // установить поле nDuckAndCoverTimer (утка, покрытие, таймер)
int get_ped_m_nBloodyTimer(lua_State* L); // m_nBloodyTimer (CPed) — получить поле nBloodyTimer (кровавый, таймер) (CPed)
int set_ped_m_nBloodyTimer(lua_State* L); // установить поле nBloodyTimer (кровавый, таймер)
int get_ped_m_nShotTime(lua_State* L); // m_nShotTime (CPed) — получить поле nShotTime (выстрел, время) (CPed)
int set_ped_m_nShotTime(lua_State* L); // установить поле nShotTime (выстрел, время)
int get_ped_m_nShotTimeAdd(lua_State* L); // m_nShotTimeAdd (CPed) — получить поле nShotTimeAdd (выстрел, время, добавить) (CPed)
int set_ped_m_nShotTimeAdd(lua_State* L); // установить поле nShotTimeAdd (выстрел, время, добавить)
int get_ped_m_nPanicCounter(lua_State* L); // m_nPanicCounter (CPed) — получить поле nPanicCounter (паника, счётчик) (CPed)
int set_ped_m_nPanicCounter(lua_State* L); // установить поле nPanicCounter (паника, счётчик)
int get_ped_m_nDeadBleeding(lua_State* L); // m_nDeadBleeding (CPed) — получить поле nDeadBleeding (мёртв, кровотечение) (CPed)
int set_ped_m_nDeadBleeding(lua_State* L); // установить поле nDeadBleeding (мёртв, кровотечение)
int get_ped_m_nBodyPartBleeding(lua_State* L); // m_nBodyPartBleeding (CPed) — получить поле nBodyPartBleeding (кузов, часть, кровотечение) (CPed)
int set_ped_m_nBodyPartBleeding(lua_State* L); // установить поле nBodyPartBleeding (кузов, часть, кровотечение)
int get_ped_m_nNumNearPeds(lua_State* L); // m_nNumNearPeds (CPed) — получить поле nNumNearPeds (около, пешеходы) (CPed)
int set_ped_m_nNumNearPeds(lua_State* L); // установить поле nNumNearPeds (около, пешеходы)
int get_ped_m_nPedMoney(lua_State* L); // m_nPedMoney (CPed) — получить поле nPedMoney (пешеход, деньги) (CPed)
int set_ped_m_nPedMoney(lua_State* L); // установить поле nPedMoney (пешеход, деньги)
int get_ped_m_nLastDamWep(lua_State* L); // m_nLastDamWep (CPed) — получить поле nLastDamWep (последний, урон, оружие) (CPed)
int set_ped_m_nLastDamWep(lua_State* L); // установить поле nLastDamWep (последний, урон, оружие)
int get_ped_m_vecAttachOffset(lua_State* L); // m_vecAttachOffset (CPed) (x,y,z) — получить поле vecAttachOffset (прикрепить, смещение) (CPed)
int set_ped_m_vecAttachOffset(lua_State* L); // установить поле vecAttachOffset (прикрепить, смещение)
int get_ped_m_nAttachType(lua_State* L); // m_nAttachType (CPed) — получить поле nAttachType (прикрепить, тип) (CPed)
int set_ped_m_nAttachType(lua_State* L); // установить поле nAttachType (прикрепить, тип)
int get_ped_m_fAttachRot(lua_State* L); // m_fAttachRot (CPed) — получить поле fAttachRot (прикрепить, поворот) (CPed)
int set_ped_m_fAttachRot(lua_State* L); // установить поле fAttachRot (прикрепить, поворот)
int get_ped_m_nAttachWepAmmo(lua_State* L); // m_nAttachWepAmmo (CPed) — получить поле nAttachWepAmmo (прикрепить, оружие, патроны) (CPed)
int set_ped_m_nAttachWepAmmo(lua_State* L); // установить поле nAttachWepAmmo (прикрепить, оружие, патроны)
int get_ped_m_nThreatFlags(lua_State* L); // m_nThreatFlags (CPed) — получить поле nThreatFlags (угроза, флаги) (CPed)
int set_ped_m_nThreatFlags(lua_State* L); // установить поле nThreatFlags (угроза, флаги)
int get_ped_m_nThreatCheck(lua_State* L); // m_nThreatCheck (CPed) — получить поле nThreatCheck (угроза, проверить) (CPed)
int set_ped_m_nThreatCheck(lua_State* L); // установить поле nThreatCheck (угроза, проверить)
int get_ped_m_nLastThreatCheck(lua_State* L); // m_nLastThreatCheck (CPed) — получить поле nLastThreatCheck (последний, угроза, проверить) (CPed)
int set_ped_m_nLastThreatCheck(lua_State* L); // установить поле nLastThreatCheck (последний, угроза, проверить)
int get_ped_m_nSayType(lua_State* L); // m_nSayType (CPed) — получить поле nSayType (сказать, тип) (CPed)
int set_ped_m_nSayType(lua_State* L); // установить поле nSayType (сказать, тип)
int get_ped_m_nSayTimer(lua_State* L); // m_nSayTimer (CPed) — получить поле nSayTimer (сказать, таймер) (CPed)
int set_ped_m_nSayTimer(lua_State* L); // установить поле nSayTimer (сказать, таймер)
int get_ped_m_nTalkTimerLast(lua_State* L); // m_nTalkTimerLast (CPed) — получить поле nTalkTimerLast (разговор, таймер, последний) (CPed)
int set_ped_m_nTalkTimerLast(lua_State* L); // установить поле nTalkTimerLast (разговор, таймер, последний)
int get_ped_m_nTalkTimer(lua_State* L); // m_nTalkTimer (CPed) — получить поле nTalkTimer (разговор, таймер) (CPed)
int set_ped_m_nTalkTimer(lua_State* L); // установить поле nTalkTimer (разговор, таймер)
int get_ped_m_wTalkTypeLast(lua_State* L); // m_wTalkTypeLast (CPed) — получить поле wTalkTypeLast (разговор, тип, последний) (CPed)
int set_ped_m_wTalkTypeLast(lua_State* L); // установить поле wTalkTypeLast (разговор, тип, последний)
int get_ped_m_wTalkType(lua_State* L); // m_wTalkType (CPed) — получить поле wTalkType (разговор, тип) (CPed)
int set_ped_m_wTalkType(lua_State* L); // установить поле wTalkType (разговор, тип)
int get_ped_m_bCanPedTalk(lua_State* L); // m_bCanPedTalk (CPed) — получить поле bCanPedTalk (может, пешеход, разговор) (CPed)
int set_ped_m_bCanPedTalk(lua_State* L); // установить поле bCanPedTalk (может, пешеход, разговор)
int get_ped_m_nPedLastComment(lua_State* L); // m_nPedLastComment (CPed) — получить поле nPedLastComment (пешеход, последний, комментарий) (CPed)
int set_ped_m_nPedLastComment(lua_State* L); // установить поле nPedLastComment (пешеход, последний, комментарий)
int get_ped_m_vecSeekPosEx(lua_State* L); // m_vecSeekPosEx (CPed) (x,y,z) — получить поле vecSeekPosEx (искать, позиция) (CPed)
int set_ped_m_vecSeekPosEx(lua_State* L); // установить поле vecSeekPosEx (искать, позиция)
int get_ped_m_fSeekExAngle(lua_State* L); // m_fSeekExAngle (CPed) — получить поле fSeekExAngle (искать, угол) (CPed)
int set_ped_m_fSeekExAngle(lua_State* L); // установить поле fSeekExAngle (искать, угол)
int get_obj_m_nObjectType(lua_State* L); // m_nObjectType (CObject) — получить поле nObjectType (объект, тип) (CObject)
int set_obj_m_nObjectType(lua_State* L); // установить поле nObjectType (объект, тип)
int get_obj_m_nBonusValue(lua_State* L); // m_nBonusValue (CObject) — получить поле nBonusValue (бонус, значение) (CObject)
int set_obj_m_nBonusValue(lua_State* L); // установить поле nBonusValue (бонус, значение)
int get_obj_m_wCostValue(lua_State* L); // m_wCostValue (CObject) — получить поле wCostValue (стоимость, значение) (CObject)
int set_obj_m_wCostValue(lua_State* L); // установить поле wCostValue (стоимость, значение)
int get_obj_m_fDamageMultiplier(lua_State* L); // m_fDamageMultiplier (CObject) — получить поле fDamageMultiplier (повреждение, множитель) (CObject)
int set_obj_m_fDamageMultiplier(lua_State* L); // установить поле fDamageMultiplier (повреждение, множитель)
int get_obj_m_bCameraAvoids(lua_State* L); // m_bCameraAvoids (CObject) — получить поле bCameraAvoids (камера, избегает) (CObject)
int set_obj_m_bCameraAvoids(lua_State* L); // установить поле bCameraAvoids (камера, избегает)
int get_obj_m_wRefModelId(lua_State* L); // m_wRefModelId (CObject) — получить поле wRefModelId (ссылка, модель) (CObject)
int set_obj_m_wRefModelId(lua_State* L); // установить поле wRefModelId (ссылка, модель)
int get_car_m_nAudioEntityId(lua_State* L); // m_nAudioEntityId (CPhysical) — получить поле nAudioEntityId (звук, сущность) (CPhysical)
int set_car_m_nAudioEntityId(lua_State* L); // установить поле nAudioEntityId (звук, сущность)
int get_car_m_vecMoveSpeed(lua_State* L); // m_vecMoveSpeed (CPhysical) (x,y,z) — получить поле vecMoveSpeed (переместить, скорость) (CPhysical)
int set_car_m_vecMoveSpeed(lua_State* L); // установить поле vecMoveSpeed (двигать, скорость)
int get_car_m_vecTurnSpeed(lua_State* L); // m_vecTurnSpeed (CPhysical) (x,y,z) — получить поле vecTurnSpeed (повернуть, скорость) (CPhysical)
int set_car_m_vecTurnSpeed(lua_State* L); // установить поле vecTurnSpeed (повернуть, скорость)
int get_car_m_vecFrictionMoveForce(lua_State* L); // m_vecFrictionMoveForce (CPhysical) (x,y,z) — получить поле vecFrictionMoveForce (трение, переместить, сила) (CPhysical)
int set_car_m_vecFrictionMoveForce(lua_State* L); // установить поле vecFrictionMoveForce (трение, двигать, сила)
int get_car_m_vecFrictionTurnForce(lua_State* L); // m_vecFrictionTurnForce (CPhysical) (x,y,z) — получить поле vecFrictionTurnForce (трение, повернуть, сила) (CPhysical)
int set_car_m_vecFrictionTurnForce(lua_State* L); // установить поле vecFrictionTurnForce (трение, повернуть, сила)
int get_car_m_vecForce(lua_State* L); // m_vecForce (CPhysical) (x,y,z) — получить поле vecForce (сила) (CPhysical)
int set_car_m_vecForce(lua_State* L); // установить поле vecForce (сила)
int get_car_m_vecTorque(lua_State* L); // m_vecTorque (CPhysical) (x,y,z) — получить поле vecTorque (крутящий момент) (CPhysical)
int set_car_m_vecTorque(lua_State* L); // установить поле vecTorque (крутящий момент)
int get_car_m_fMass(lua_State* L); // m_fMass (CPhysical) — получить поле fMass (масса) (CPhysical)
int set_car_m_fMass(lua_State* L); // установить поле fMass (масса)
int get_car_m_fTurnMass(lua_State* L); // m_fTurnMass (CPhysical) — получить поле fTurnMass (повернуть, масса) (CPhysical)
int set_car_m_fTurnMass(lua_State* L); // установить поле fTurnMass (повернуть, масса)
int get_car_m_fVelocityFrequency(lua_State* L); // m_fVelocityFrequency (CPhysical) — получить поле fVelocityFrequency (скорость, частота) (CPhysical)
int set_car_m_fVelocityFrequency(lua_State* L); // установить поле fVelocityFrequency (скорость, частота)
int get_car_m_fAirResistance(lua_State* L); // m_fAirResistance (CPhysical) — получить поле fAirResistance (воздух, сопротивление) (CPhysical)
int set_car_m_fAirResistance(lua_State* L); // установить поле fAirResistance (воздух, сопротивление)
int get_car_m_fElasticity(lua_State* L); // m_fElasticity (CPhysical) — получить поле fElasticity (упругость) (CPhysical)
int set_car_m_fElasticity(lua_State* L); // установить поле fElasticity (упругость)
int get_car_m_fBuoyancyConstant(lua_State* L); // m_fBuoyancyConstant (CPhysical) — получить поле fBuoyancyConstant (плавучесть, константа) (CPhysical)
int set_car_m_fBuoyancyConstant(lua_State* L); // установить поле fBuoyancyConstant (плавучесть, константа)
int get_car_m_vecCentreOfMass(lua_State* L); // m_vecCentreOfMass (CPhysical) (x,y,z) — получить поле vecCentreOfMass (центр, масса) (CPhysical)
int set_car_m_vecCentreOfMass(lua_State* L); // установить поле vecCentreOfMass (центр, масса)
int get_car_m_nNumCollisionRecords(lua_State* L); // m_nNumCollisionRecords (CPhysical) — получить поле nNumCollisionRecords (столкновение, рекорды) (CPhysical)
int set_car_m_nNumCollisionRecords(lua_State* L); // установить поле nNumCollisionRecords (столкновение, рекорды)
int get_car_m_fTotSpeed(lua_State* L); // m_fTotSpeed (CPhysical) — получить поле fTotSpeed (всего, скорость) (CPhysical)
int set_car_m_fTotSpeed(lua_State* L); // установить поле fTotSpeed (всего, скорость)
int get_car_m_fCollisionPower(lua_State* L); // m_fCollisionPower (CPhysical) — получить поле fCollisionPower (столкновение, мощность) (CPhysical)
int set_car_m_fCollisionPower(lua_State* L); // установить поле fCollisionPower (столкновение, мощность)
int get_car_m_vecCollisionPower(lua_State* L); // m_vecCollisionPower (CPhysical) (x,y,z) — получить поле vecCollisionPower (столкновение, мощность) (CPhysical)
int set_car_m_vecCollisionPower(lua_State* L); // установить поле vecCollisionPower (столкновение, мощность)
int get_car_m_wComponentCol(lua_State* L); // m_wComponentCol (CPhysical) — получить поле wComponentCol (компонент, коллизия) (CPhysical)
int set_car_m_wComponentCol(lua_State* L); // установить поле wComponentCol (компонент, коллизия)
int get_car_m_nMoveFlags(lua_State* L); // m_nMoveFlags (CPhysical) — получить поле nMoveFlags (переместить, флаги) (CPhysical)
int set_car_m_nMoveFlags(lua_State* L); // установить поле nMoveFlags (переместить, флаги)
int get_car_m_nCollFlags(lua_State* L); // m_nCollFlags (CPhysical) — получить поле nCollFlags (коллизия, флаги) (CPhysical)
int set_car_m_nCollFlags(lua_State* L); // установить поле nCollFlags (коллизия, флаги)
int get_car_m_nLastCollType(lua_State* L); // m_nLastCollType (CPhysical) — получить поле nLastCollType (последний, коллизия, тип) (CPhysical)
int set_car_m_nLastCollType(lua_State* L); // установить поле nLastCollType (последний, коллизия, тип)
int get_car_m_nZoneLevel(lua_State* L); // m_nZoneLevel (CPhysical) — получить поле nZoneLevel (зона, уровень) (CPhysical)
int set_car_m_nZoneLevel(lua_State* L); // установить поле nZoneLevel (зона, уровень)
int get_car_fNegSpeed(lua_State* L); // fNegSpeed (CAutomobile) — получить поле fNegSpeed (отрицательный, скорость) (CAutomobile)
int set_car_fNegSpeed(lua_State* L); // установить поле fNegSpeed (отрицательный, скорость)
int get_car_fAIGripMultiplier(lua_State* L); // fAIGripMultiplier (CAutomobile) — получить поле fAIGripMultiplier (ИИ, сцепление, множитель) (CAutomobile)
int set_car_fAIGripMultiplier(lua_State* L); // установить поле fAIGripMultiplier (ИИ, сцепление, множитель)
int get_car_fSpecialWepRotH(lua_State* L); // fSpecialWepRotH (CAutomobile) — получить поле fSpecialWepRotH (специальный, оружие, поворот) (CAutomobile)
int set_car_fSpecialWepRotH(lua_State* L); // установить поле fSpecialWepRotH (специальный, оружие, поворот)
int get_car_fSpecialWepRotV(lua_State* L); // fSpecialWepRotV (CAutomobile) — получить поле fSpecialWepRotV (специальный, оружие, поворот) (CAutomobile)
int set_car_fSpecialWepRotV(lua_State* L); // установить поле fSpecialWepRotV (специальный, оружие, поворот)
int get_car_fSpecialSteering(lua_State* L); // fSpecialSteering (CAutomobile) — получить поле fSpecialSteering (специальный, рулевое управление) (CAutomobile)
int set_car_fSpecialSteering(lua_State* L); // установить поле fSpecialSteering (специальный, рулевое управление)
int get_car_fSpecialMoveState(lua_State* L); // fSpecialMoveState (CAutomobile) — получить поле fSpecialMoveState (специальный, переместить, состояние) (CAutomobile)
int set_car_fSpecialMoveState(lua_State* L); // установить поле fSpecialMoveState (специальный, двигать, состояние)
int get_car_fSkidMarkDensity(lua_State* L); // fSkidMarkDensity (CAutomobile) — получить поле fSkidMarkDensity (след шин, маркер, плотность) (CAutomobile)
int set_car_fSkidMarkDensity(lua_State* L); // установить поле fSkidMarkDensity (след шин, маркер, плотность)
int get_car_m_fPropRotation(lua_State* L); // m_fPropRotation (CBoat) — получить поле fPropRotation (реквизит, вращение) (CBoat)
int set_car_m_fPropRotation(lua_State* L); // установить поле fPropRotation (реквизит, вращение)
int get_car_m_fPropSpeed(lua_State* L); // m_fPropSpeed (CBoat) — получить поле fPropSpeed (реквизит, скорость) (CBoat)
int set_car_m_fPropSpeed(lua_State* L); // установить поле fPropSpeed (реквизит, скорость)
int get_car_m_fForcedZRotation(lua_State* L); // m_fForcedZRotation (CBoat) — получить поле fForcedZRotation (принудительно, Z, вращение) (CBoat)
int set_car_m_fForcedZRotation(lua_State* L); // установить поле fForcedZRotation (принудительно, Z, вращение)
int get_car_m_nAttackPlayerTime(lua_State* L); // m_nAttackPlayerTime (CBoat) — получить поле nAttackPlayerTime (атака, игрок, время) (CBoat)
int set_car_m_nAttackPlayerTime(lua_State* L); // установить поле nAttackPlayerTime (атака, игрок, время)
int get_car_m_fBurningTimer(lua_State* L); // m_fBurningTimer (CBoat) — получить поле fBurningTimer (горение, таймер) (CBoat)
int set_car_m_fBurningTimer(lua_State* L); // установить поле fBurningTimer (горение, таймер)
int get_car_m_fBoatGasPedal(lua_State* L); // m_fBoatGasPedal (CBoat) — получить поле fBoatGasPedal (лодка, газ, педаль) (CBoat)
int set_car_m_fBoatGasPedal(lua_State* L); // установить поле fBoatGasPedal (лодка, газ, педаль)
int get_car_m_fBoatBrakePedal(lua_State* L); // m_fBoatBrakePedal (CBoat) — получить поле fBoatBrakePedal (лодка, тормоз, педаль) (CBoat)
int set_car_m_fBoatBrakePedal(lua_State* L); // установить поле fBoatBrakePedal (лодка, тормоз, педаль)
int get_car_m_fBoatSteeringLeftRight(lua_State* L); // m_fBoatSteeringLeftRight (CBoat) — получить поле fBoatSteeringLeftRight (лодка, рулевое управление, влево, вправо) (CBoat)
int set_car_m_fBoatSteeringLeftRight(lua_State* L); // установить поле fBoatSteeringLeftRight (лодка, рулевое управление, влево, вправо)
int get_car_m_nPadNumber(lua_State* L); // m_nPadNumber (CBoat) — получить поле nPadNumber (панель) (CBoat)
int set_car_m_nPadNumber(lua_State* L); // установить поле nPadNumber (панель)
int get_car_m_nNumWaterTrailPoints(lua_State* L); // m_nNumWaterTrailPoints (CBoat) — получить поле nNumWaterTrailPoints (вода, след, очки) (CBoat)
int set_car_m_nNumWaterTrailPoints(lua_State* L); // установить поле nNumWaterTrailPoints (вода, след, очки)
int get_car_nBikeAnimGroup(lua_State* L); // nBikeAnimGroup (CBike) — получить поле nBikeAnimGroup (мотоцикл, анимация, группа) (CBike)
int set_car_nBikeAnimGroup(lua_State* L); // установить поле nBikeAnimGroup (мотоцикл, анимация, группа)
int get_car_m_nDamageFlags(lua_State* L); // m_nDamageFlags (CBike) — получить поле nDamageFlags (повреждение, флаги) (CBike)
int set_car_m_nDamageFlags(lua_State* L); // установить поле nDamageFlags (повреждение, флаги)
int get_car_m_fSearchLightX(lua_State* L); // m_fSearchLightX (CHeli) — получить поле fSearchLightX (поиск, свет, X) (CHeli)
int set_car_m_fSearchLightX(lua_State* L); // установить поле fSearchLightX (поиск, свет, X)
int get_car_m_fSearchLightY(lua_State* L); // m_fSearchLightY (CHeli) — получить поле fSearchLightY (поиск, свет, Y) (CHeli)
int set_car_m_fSearchLightY(lua_State* L); // установить поле fSearchLightY (поиск, свет, Y)
int get_car_m_nExplosionTime(lua_State* L); // m_nExplosionTime (CHeli) — получить поле nExplosionTime (взрыв, время) (CHeli)
int set_car_m_nExplosionTime(lua_State* L); // установить поле nExplosionTime (взрыв, время)
int get_car_m_fRotationX(lua_State* L); // m_fRotationX (CHeli) — получить поле fRotationX (вращение, X) (CHeli)
int set_car_m_fRotationX(lua_State* L); // установить поле fRotationX (вращение, X)
int get_car_m_fHeliForcedZ(lua_State* L); // m_fHeliForcedZ (CHeli) — получить поле fHeliForcedZ (вертолёт, принудительно, Z) (CHeli)
int set_car_m_fHeliForcedZ(lua_State* L); // установить поле fHeliForcedZ (вертолёт, принудительно, Z)
int get_car_m_fSearchLightIntensity(lua_State* L); // m_fSearchLightIntensity (CHeli) — получить поле fSearchLightIntensity (поиск, свет, интенсивность) (CHeli)
int set_car_m_fSearchLightIntensity(lua_State* L); // установить поле fSearchLightIntensity (поиск, свет, интенсивность)
int get_car_m_nDamagePoints(lua_State* L); // m_nDamagePoints (CHeli) — получить поле nDamagePoints (повреждение, очки) (CHeli)
int set_car_m_nDamagePoints(lua_State* L); // установить поле nDamagePoints (повреждение, очки)
int get_car_m_fRotorAngle(lua_State* L); // m_fRotorAngle (CHeli) — получить поле fRotorAngle (ротор, угол) (CHeli)
int set_car_m_fRotorAngle(lua_State* L); // установить поле fRotorAngle (ротор, угол)
int get_wanted_m_nLastTimeWantedDecreased(lua_State* L); // последнее снижение розыска
int get_wanted_m_nLastTimeWantedLevelChanged(lua_State* L); // последняя смена уровня розыска
int get_wanted_m_dwTimeOfParole(lua_State* L); // таймер условного освобождения
int set_wanted_m_dwTimeOfParole(lua_State* L); // уст. таймер условного освобождения
int get_wanted_m_fMultiplier(lua_State* L); // множитель розыска
int set_wanted_m_fMultiplier(lua_State* L); // уст. множитель розыска
int get_wanted_m_nCopsInPursuit(lua_State* L); // копов в погоне
int get_wanted_m_nMaxCopsInPursuit(lua_State* L); // макс. копов в погоне
int set_wanted_m_nMaxCopsInPursuit(lua_State* L); // уст. макс. копов в погоне
int get_wanted_m_nMaxCopCarsInPursuit(lua_State* L); // макс. машин копов в погоне
int set_wanted_m_nMaxCopCarsInPursuit(lua_State* L); // уст. макс. машин копов в погоне
int get_wanted_m_nCopsBeatingSuspect(lua_State* L); // копов избивают подозреваемого
int choose_gang_ped_model(lua_State* L); // выбрать модель педа банды
int set_gang_attack_player_with_cops(lua_State* L); // банда атакует игрока с копами
int init_gangs(lua_State* L); // инициализировать банды
int register_motion_streak(lua_State* L); // зарегистрировать motion streak
int init_trains(lua_State* L); // инициализировать поезда
int update_trains(lua_State* L); // обновить поезда
int shutdown_trains(lua_State* L); // завершить поезда
int Ped_in_targetting(lua_State* L); // Макрос пед на прицеле.
int Get_model_and_type_ped(lua_State* L); // макрос получить модель и тип педа.

int get_model_ped(lua_State* L); // получить модель педа.
int get_type_ped(lua_State* L); // получить тип педа.
int ped_clear_investigate_event(lua_State* L); // пед уходит, опустить педа.
int ped_clear_objective(lua_State* L); // снять задачи с педа.

int setbomb_in_car(lua_State* L); // установить бомбу в авто.
int getbomb_in_car(lua_State* L); // получить тип бомбы в авто.
int activetbomb(lua_State* L); // активировать бомбу в авто.
int is_ped_in_this_car(lua_State* L); // игрок в этом авто?

int ishorncar(lua_State* L); // проверить гудит ли авто.
int getcarfirstcolor(lua_State* L); // получить первый цвет авто.
int getcarseconscolor(lua_State* L); // получить второй цвет авто.
int ped_char_spotted_player(lua_State* L); // если пед увидел игрока.

int set_timer_bomb_car(lua_State* L); // установить таймер на взрыв бомбы в авто.
int is_ped_objective(lua_State* L); // если пед выполнил задачу.
int set_time_scale(lua_State* L); // установить ход времени в игре.
int is_ped_in_air(lua_State* L); // если пед в воздухе.

int kill_peds_in_car(lua_State* L); // убить педов в авто.
// (28.11) Выносливость игрока, уличное освещение, типы шин/двигателя
int get_stamina(lua_State* L); // получить выносливость
int set_stamina(lua_State* L); // установить выносливость
int get_max_stamina(lua_State* L); // получить максимум выносливость
int set_max_stamina(lua_State* L); // установить максимум выносливость
int get_stamina_duration(lua_State* L); // получить выносливость длительность
int set_stamina_duration(lua_State* L); // установить выносливость длительность
int are_street_lights_on(lua_State* L); // проверить улица фары
int get_vehicle_wheel_size(lua_State* L); // получить автомобиль колесо размер
int set_vehicle_wheel_size(lua_State* L); // установить автомобиль колесо размер
int get_vehicle_wheel_type(lua_State* L); // получить автомобиль колесо тип
int set_vehicle_wheel_type(lua_State* L); // установить автомобиль колесо тип
int get_vehicle_engine_type(lua_State* L); // получить автомобиль двигатель тип
int set_vehicle_engine_type(lua_State* L); // установить автомобиль двигатель тип
int getstamina_ped(lua_State* L); // получить выносливость педа.
int setstamina_ped(lua_State* L); // установить выносливость педа.
int get_radio_car(lua_State* L); // получить текущую радиостанцию авто.

int is_ped_passenger_car(lua_State* L); // пед пассажир авто.
int is_ped_driver_car(lua_State* L); // пед пассажир авто.
int ped_in_turret_on_car(lua_State* L); // переместить педа в турель авто.
int ped_from_turret_on_car(lua_State* L); // убрать педа из турели авто.

int is_button_pressed(lua_State* L); // если спец клавиша нажата.
int getmoney_ped(lua_State* L); // получить кол-во деньги педа.
int setmoney_ped(lua_State* L); // установить кол-во деньги педа.
int addmoney_ped(lua_State* L); // добавить кол-во деньги педа.

int getgravity(lua_State* L); // получить значения гравитации.
int setgravity(lua_State* L); // установить значения гравитации.
int gravity_default(lua_State* L); // вернуть значения по умолчанию гравитации.
int create_shots_on_ped(lua_State* L); // создать выстрелы.

int get_wheel_status(lua_State* L); // получить состояния шины авто.
int create_shots_on_car(lua_State* L); // создать выстрелы.
int ped_save_pos_attack(lua_State* L); // пед сохраняет ли свою позицию при атаке.
int ped_rebuff(lua_State* L); // пед атакует любого, кто нападает на него.

int ped_search_threat(lua_State* L); // пед ищет угрозу.
int ped_clean_threat(lua_State* L); // очистить угрозы для педа.
int save_car_color(lua_State* L); // авто сохраняет цвет при перекраске.
int is_car_passenger_free(lua_State* L); // Проверяет, свободно ли место пассажира в авто.

int set_ped_bleeding(lua_State* L); // вкл\выкл крованные у педа.
int all_exit_from_car(lua_State* L); // все педы выходят из авто.
int is_ped_duck(lua_State* L); // Проверяет, пед присел.
int set_car_tank(lua_State* L); // установить способность танка детонировать авто при контакте.

int set_display_radar(lua_State* L); // показать скрывать радар.
int set_ped_in_car(lua_State* L); // установить педа в авто как водителя или пассажира.
int set_ped_traffic(lua_State* L); // установить пешеход traffic
int heli_to_fly(lua_State* L); // вертолет летит на координаты.

int set_ped_immunities(lua_State* L); // уст иммунитеты педу.
int set_car_immunities(lua_State* L); // уст иммунитеты авто.
int setcolourmarker(lua_State* L); // установить цвет маркера.

int set_camera_on_car(lua_State* L); // уст камеру на авто.
int ped_run_to_point(lua_State* L); // пед бежит к точке пешком.
int select_interiour(lua_State* L); // выбрать interiour
int set_ped_stats_to(lua_State* L); // уст поведения педа.

int ped_maintain_position_when_attacked(lua_State* L); // пед удерживает позицию при атаке.
int set_timer_ped_attack(lua_State* L); // установить таймер атаки педа.
int set_cops_ignored(lua_State* L); // установить игнор копов.
int set_camera_near_clip(lua_State* L); // установить обрезку камеры.

int setpedcrouch(lua_State* L); // пед сел.
int is_ped_incar(lua_State* L);  // пед в авто или нет?
int delete_entity(lua_State* L); // удалить сущность сразу.
int clean_leader(lua_State* L); // перестать следовать за лидером.

int show_save_menu(lua_State* L); // показать меню сохранения (только если игрок жив, не в миссии, не в машине).
int save_game_slot(lua_State* L); // сохранить игру в слот 1..8 (1 параметр).
int load_game_slot(lua_State* L); // загрузить игру из слота 1..8 (1 параметр).
int is_ped_shooting(lua_State* L); // пед стреляет?.
int set_ped_visible(lua_State* L); // уст невидимость педу.
int set_player_mod(lua_State* L); // уст настроения игрока.

int setped_damaged_player(lua_State* L); // педу может нанести ущерб только игрок.
int set_ped_reaction_threat(lua_State* L); // уст реакцию педа на ответную угрозу.
int obj_target(lua_State* L); // в объект можно целиться.
int clean_ped_wait(lua_State* L); // пед больше не ждет.

int set_ped_accuracy(lua_State* L); // уст меткость педу.
int textbox(lua_State* L); // текстовое окно
int remove_textbox(lua_State* L); // удалить textbox
int get_controller(lua_State* L); // получить контролер от игрока.

int create_pickup_clothes(lua_State* L); // создать пикап одежды.
int set_riot_intensity(lua_State* L); // уст уровень шума беспорядков.
int set_ped_walk_style(lua_State* L); // установить стиль ходьбы педа.
int check_car_resray(lua_State* L); // проверить авто игрока было перекрашена в гараже.

int set_car_range(lua_State* L); // установить множитель диапазона на угрозу для автомобиля.
int set_ped_range(lua_State* L); // установить множитель диапазона на угрозу для педа.
int getcarangle(lua_State* L); // получить угол авто.
int cleanarea(lua_State* L); //очистить арену.

int set_brakes_car(lua_State* L); //уст тормоза авто игрока.
int setmarker_brightness(lua_State* L); // уст яркость маркера.
int Createobj(lua_State* L); // макрос создать объект.
int setpednode_seek(lua_State* L); // пед игнорирорует пути педов.

int ispedscreen(lua_State* L); // пед виден.
int iscarscreen(lua_State* L); // авто видно.
int isobjscreen(lua_State* L); // объект виден.
int ispeddead(lua_State* L); // пед мертв.

int iscardead(lua_State* L); // авто уничтожено.
int ped_follow_ped(lua_State* L); // пед следует за педом.
int set_cars_damaged(lua_State* L); // Все авто повреждены.
int set_ped_targetted(lua_State* L); // запрет целиться в педа.

int set_ped_friend(lua_State* L); // уст дружественное отношения педа.
int set_ped_running(lua_State* L); // пед может бежать.
int set_ped_damaged_gang(lua_State* L); // уст педа уязвимым для членов банды.
int is_ped_damaged_weapon(lua_State* L); // пед получает от определенного вида оружие.

int is_car_damaged_weapon(lua_State* L); // авто получает от определенного вида оружие.
int isped_in_air(lua_State* L); // пед в воздухе.
int set_threat_for_ped_type(lua_State* L); // уст враждебность типа педа к другим типам педа.
int clean_threat_for_ped_type(lua_State* L); // Убрать враждебность типа педа к другим типам педа.

int create_phone(lua_State* L); // создать телефон на координатах.
int on_phone(lua_State* L); // вкл телефон.
int off_phone(lua_State* L); // выкл телефон.
int read_memory(lua_State* L); // читать адрес памяти.

int write_memory(lua_State* L); // записать адрес памяти.
int is_sphere_onscreen(lua_State* L); // видна точка на радаре.
int set_ped_can_shot_in_car(lua_State* L); // водитель авто может получить пули.
int iscarfire(lua_State* L); // авто горит?

int is_car_damage_by_car(lua_State* L); // Если авто получило урон от другого авто.
int show_text_gtx_table(lua_State* L); // показать текст GXT текст таблица
int show_text_key(lua_State* L);   // 23.11: GXT-ключ -> сама грузит нужную таблицу // вывести текст из таблицы.
int ped_road_off(lua_State* L); // выкл путь педов.
int ped_road_on(lua_State* L); // вкл путь педов.

int car_road_off(lua_State* L); // выкл путь авто.
int car_road_on(lua_State* L); // вкл путь авто.
int getcardamage(lua_State* L); // получить кол-во урона авто.
int randomfindobj(lua_State* L); // найти случайный объект в радиусе.

int findped_in_cord(lua_State* L); // найти педа в радиусе с координатами.
int findcar_in_cord(lua_State* L); // найти авто в радиусе с координатами.
int findobj_in_cord(lua_State* L); // найти объект в радиусе с координатами.
int findped_nearest_in_cord(lua_State* L); // найти ближайшего педа в радиусе с координатами.

int findcar_nearest_in_cord(lua_State* L); // найти ближайшее авто в радиусе с координатами.
int findobj_nearest_in_cord(lua_State* L); // найти ближайший объект в радиусе с координатами.
int getmodelindex(lua_State* L); // получить индекс модели
int check_ped_in_cord(lua_State* L); // есть ли педа в радиусе с координатами.

int get_ped_in_cord(lua_State* L); // получить педа в радиусе с координатами.
int check_car_in_cord(lua_State* L); // есть ли авто в радиусе с координатами.
int get_car_in_cord(lua_State* L); // найти авто в радиусе с координатами.
int check_obj_in_cord(lua_State* L); // есть ли объект в радиусе с координатами.

int get_obj_in_cord(lua_State* L); // получить объект в радиусе с координатам.
int setobjangle(lua_State* L); // уст угол объекта.
int getcardimension(lua_State* L); // получить размер авто.

int carmoveforce(lua_State* L);  // двигать авто с силой.
int carturnforce(lua_State* L);  // повернуть авто с силой.
int car_turn_speed(lua_State* L); // повернуть авто с применением скорости.
int carturnfrictionforce(lua_State* L); // повернуть авто с силой трения.

int car_move_speed(lua_State* L); // двигать авто с применением скорости.
int car_turnforce_on_x(lua_State* L); // Авто turnforce x
int car_turnforce_on_y(lua_State* L); // Авто turnforce y
int car_turnforce_on_z(lua_State* L); // Авто turnforce z

int car_moveforce_on_x(lua_State* L); // Авто moveforce x
int car_moveforce_on_y(lua_State* L); // Авто moveforce y
int car_moveforce_on_z(lua_State* L); // Авто moveforce z
int setcarrotate(lua_State* L);  // уст вращения авто.

int getcar_turn_on_x(lua_State* L); // Getcar повернуть x
int getcar_turn_on_y(lua_State* L); // Getcar повернуть y
int getcar_turn_on_z(lua_State* L); // Getcar повернуть z

int carmovespeed_on_x(lua_State* L); // двигать авто со скоростью по X
int carmovespeed_on_y(lua_State* L); // двигать авто со скоростью по Y
int carmovespeed_on_z(lua_State* L); // двигать авто со скоростью по Z

int car_rotate_on_x(lua_State* L); // Авто rotate x
int car_rotate_on_y(lua_State* L); // Авто rotate y
int car_rotate_on_z(lua_State* L); // Авто rotate z

int car_setrotate_on_x(lua_State* L); // Авто setrotate x
int car_setrotate_on_y(lua_State* L); // Авто setrotate y
int car_setrotate_on_z(lua_State* L); // Авто setrotate z

int getcar_resistance(lua_State* L); // Получить сопротивления авто трению.
int setcar_resistance(lua_State* L); // Уст силу сопротивления авто трению.
int getcar_turns_on_axes(lua_State* L); // получить углы авто поворота по осям.

int setcar_on_x(lua_State* L); // установить авто по координате X
int setcar_on_y(lua_State* L); // установить авто по координате Y
int setcar_on_z(lua_State* L); // установить авто по координате Z

int car_rotate_turn_on_x(lua_State* L); // Авто rotate повернуть x
int car_rotate_turn_on_y(lua_State* L); // Авто rotate повернуть y
int car_rotate_turn_on_z(lua_State* L); // Авто rotate повернуть z

int car_turn_on_x_with_speed(lua_State* L); // Авто повернуть x скорость
int car_turn_on_y_with_speed(lua_State* L); // Авто повернуть y скорость
int car_turn_on_z_with_speed(lua_State* L); // Авто повернуть z скорость


int car_turn_on_x_with_delay(lua_State* L); // Авто повернуть x задержка
int car_turn_on_y_with_delay(lua_State* L); // Авто повернуть y задержка
int car_turn_on_z_with_delay(lua_State* L); // Авто повернуть z задержка
int set_car_mass(lua_State* L); // set_car_mass(car, 3000) - увеличить массу авто (значение как в handling.cfg, 3000 ~ грузовик).
int set_car_acceleration(lua_State* L); // set_car_acceleration(car, 0.2) - установить ускорение авто (0.1..10.0).
int set_car_max_speed(lua_State* L); // set_car_max_speed(car, 100) - установить макс. скорость авто.
int set_car_traction(lua_State* L); // set_car_traction(car, 0.9) - установить сцепление авто (0.8..1.0).

int create_rope_on_cords_with_swat(lua_State* L); // создать веревку на координатах с бойцом спецназа.
int create_rope_on_cords(lua_State* L); // создать веревку на координатах.
int heli_change_height(lua_State* L); // изменить высоту вертолета.

int set_path_to_module(lua_State* L);// уст путь к модулю.
int load_and_start_luascript(lua_State* L, char* luafile, string res); // загрузка и запуск скрипта.

int newthread(lua_State* L);// запуск функции в новом потоке.

void reversestack(lua_State* L); // реверс стека.
void showstack(lua_State* L);
void showstack1(lua_State* L);

// прототипы функций массового расширения API (этапы 13–17); реализации — в funcs.cpp.
// в едином TU определение само служило объявлением; при разделении на 3 файла прототипы
// понадобились здесь, чтобы register.cpp видел имена для lua_register.
int anim_set_group_ref(lua_State* L); // Анимация установить группа ссылка
int camera_calculate_derived_values(lua_State* L); // Камера calculate производный значения
int camera_clear_player_weapon_mode(lua_State* L); // Камера очистить игрок оружие режим
int camera_dont_process_obbe_cinema(lua_State* L); // Камера не process obbe кинорежим
int camera_draw_borders_widescreen(lua_State* L); // Камера рисовать полосы широкоэкранный
int camera_shake(lua_State* L); // Камера тряска
int car_activate_bomb(lua_State* L); // Авто activate бомба
int car_activate_bomb_when_entered(lua_State* L); // Авто activate бомба вошёл
int car_can_be_deleted(lua_State* L); // Авто может удалён
int car_can_doors_be_damaged(lua_State* L); // Авто может двери повреждён
int car_can_ped_enter(lua_State* L); // Авто может пешеход войти
int car_extinguish_fire(lua_State* L); // стрелять авто погасить
int car_get_height_above_road(lua_State* L); // Авто получить высота над дорога
int car_has_roof(lua_State* L); // Авто имеет крыша
int car_is_clear_to_drive_away(lua_State* L); // Авто очистить ездить прочь
int car_is_door_fully_open(lua_State* L); // открыть авто дверь полностью
int car_is_door_missing(lua_State* L); // Авто дверь отсутствует
int car_is_door_ready(lua_State* L); // Авто дверь готов
int car_is_law_enforcement(lua_State* L); // Авто закон правоприменение
int car_is_on_its_side(lua_State* L); // Авто это сторона
int car_is_open_top(lua_State* L); // Авто открыть верх
int car_is_upside_down(lua_State* L); // Авто вверх дном
int car_is_vehicle_normal(lua_State* L); // Авто автомобиль обычный
int car_play_horn(lua_State* L); // Авто проиграть гудок
int car_shuffle_passengers(lua_State* L); // Авто перемешать пассажиры
int car_update_passenger_list(lua_State* L); // Авто обновить пассажир список
int car_uses_siren(lua_State* L); // Авто использует сирена
int clock_restore(lua_State* L); // Часы восстановить
int clock_store(lua_State* L); // сохранить часы
int create_explosion_no_sound(lua_State* L); // создать взрыв звук
int fire_find_nearest(lua_State* L); // стрелять найти ближайший
int fire_is_active(lua_State* L); // стрелять активный
int fire_start_on_point(lua_State* L); // стрелять начать точка
int font_set_alpha_fade(lua_State* L); // Шрифт установить alpha затемнение
int font_set_background_off(lua_State* L); // Шрифт установить фон
int font_set_background_on(lua_State* L); // Шрифт установить фон
int font_set_centre_size(lua_State* L); // Шрифт установить центр размер
int font_set_drop_shadow(lua_State* L); // Шрифт установить бросить тень
int font_set_prop_off(lua_State* L); // Шрифт установить реквизит
int font_set_prop_on(lua_State* L); // Шрифт установить реквизит
int font_set_right_justify_off(lua_State* L); // Шрифт установить вправо выравнивание
int font_set_right_justify_on(lua_State* L); // Шрифт установить вправо выравнивание
int font_set_style(lua_State* L); // Шрифт установить стиль
int font_set_wrapx(lua_State* L); // Шрифт установить зацикливание x
int force_rain(lua_State* L); // Сила дождь
int force_weather(lua_State* L); // Сила погода
int force_weather_now(lua_State* L); // Сила погода сейчас
int gamelogic_after_death_arrest_shortcut(lua_State* L); // Игровая логика после смерть арест сокращение
int gamelogic_pass_time(lua_State* L); // Игровая логика проходить время
int gamelogic_update(lua_State* L); // обновить игровая логика
int gangs_choose_ped_model(lua_State* L); // Банды выбрать модель пешехода
int gangs_get_attack_with_cops(lua_State* L); // Банды получить атака полиция
int gangs_initialise(lua_State* L); // Банды инициализировать
int gangs_set_attack_with_cops(lua_State* L); // Банды установить атака полиция
int gangs_set_ped_models(lua_State* L); // Банды установить пешеход модели
int gangs_set_vehicle_model(lua_State* L); // Банды установить модель автомобиля
int gangs_set_weapons(lua_State* L); // Банды установить оружия
int general_angle_between_points(lua_State* L); // Общий угол между очки
int general_atan_of_xy(lua_State* L); // Общий арктангенс xy
int general_limit_angle(lua_State* L); // Общий предел угол
int general_limit_radian_angle(lua_State* L); // Общий предел радиан угол
int general_node_heading_from_vector(lua_State* L); // Общий узел курс вектор
int general_radian_angle_between_points(lua_State* L); // Общий радиан угол между очки
int get_all_random_peds_type(lua_State* L); // получить случайный пешеходы тип
int get_ammo_in_clip_ped(lua_State* L); // получить патроны обойма пешеход
int get_anim_block_index(lua_State* L); // получить анимация блок индекс
int get_car_brake_pedal(lua_State* L); // получить авто тормоз педаль
int get_car_created_by(lua_State* L); // получить авто создано
int get_car_door_lock(lua_State* L); // получить авто дверь lock
int get_car_driver(lua_State* L); // получить авто водитель
int get_car_engine_on(lua_State* L); // получить авто двигатель
int get_car_gas_pedal(lua_State* L); // получить авто газ педаль
int get_car_handbrake_on(lua_State* L); // получить авто ручной тормоз
int get_car_is_big(lua_State* L); // получить авто большой
int get_car_is_bus(lua_State* L); // получить авто автобус
int get_car_is_van(lua_State* L); // получить авто фургон
int get_car_last_damager(lua_State* L); // получить авто последний damager
int get_car_lights_on(lua_State* L); // получить авто фары
int get_car_locked(lua_State* L); // получить авто заблокировано
int get_car_max_passengers(lua_State* L); // получить авто максимум пассажиры
int get_car_num_passengers(lua_State* L); // получить авто пассажиры
int get_car_passenger(lua_State* L); // получить авто пассажир
int get_car_radio_station(lua_State* L); // получить авто радио станция
int get_car_steer_angle(lua_State* L); // получить авто руль угол
int get_car_time_of_death(lua_State* L); // получить авто время смерть
int get_car_tires_invulnerable(lua_State* L); // получить авто шины неуязвимый
int get_car_vehicle_class(lua_State* L); // получить авто автомобиль класс
int get_car_wanted_stars_on_enter(lua_State* L); // получить авто розыск звёзды розыска войти
int get_car_weapon_damage(lua_State* L); // получить авто оружие повреждение
int get_chance_on_roadblock(lua_State* L); // получить шанс блокпост
int get_cops_in_pursuit(lua_State* L); // получить полиция погоня
int get_fade_color(lua_State* L); // получить затемнение цвет
int get_far_clip(lua_State* L); // получить далеко обойма
int get_fire_position(lua_State* L); // получить огонь позиция
int get_fire_target(lua_State* L); // получить огонь цель
int get_fire_time_to_burn(lua_State* L); // получить огонь время гореть
int Getflagmission(lua_State* L); // Получить флаг миссии
int get_fov(lua_State* L); // получить угол обзора
int get_frame_counter(lua_State* L); // получить кадр счётчик
int get_game_hours(lua_State* L); // получить игра часы
int get_game_minutes(lua_State* L); // получить игра минуты
int get_game_seconds(lua_State* L); // получить игра секунды
int get_lod_distance(lua_State* L); // получить детализация расстояние
int get_max_cop_cars_in_pursuit(lua_State* L); // получить максимум полицейский авто погоня
int get_max_cops_in_pursuit(lua_State* L); // получить максимум полиция погоня
int get_milliseconds_per_game_minute(lua_State* L); // получить миллисекунды на игра минута
int get_mouse_accel_horizontal(lua_State* L); // получить мышь ускорение горизонтальный
int get_mouse_accel_vertical(lua_State* L); // получить мышь ускорение вертикальный
int get_near_clip(lua_State* L); // получить около обойма
int get_new_weather_type(lua_State* L); // получить новый погода тип
int get_obj_attach_force(lua_State* L); // получить объект прикрепить сила
int get_obj_bonus_value(lua_State* L); // получить объект бонус значение
int get_obj_camera_avoids(lua_State* L); // получить объект камера избегает
int get_obj_collision_damage_type(lua_State* L); // получить объект столкновение повреждение тип
int get_obj_cost_value(lua_State* L); // получить объект стоимость значение
int get_obj_damage_multiplier(lua_State* L); // получить объект повреждение множитель
int get_obj_ref_model_id(lua_State* L); // получить объект ссылка модель
int get_obj_timer(lua_State* L); // получить объект таймер
int get_obj_type(lua_State* L); // получить объект тип
int get_old_weather_type(lua_State* L); // получить старый погода тип
int get_ped_density_multiplier(lua_State* L); // получить пешеход плотность множитель
int get_ped_enter_type(lua_State* L); // получить пешеход войти тип
int get_ped_from_vehicle(lua_State* L); // получить пешеход автомобиль
int get_ped_heading(lua_State* L); // получить пешеход курс
int get_ped_heading_goal(lua_State* L); // получить пешеход курс цель
int get_ped_heading_rate(lua_State* L); // получить пешеход курс частота
int get_ped_last_state(lua_State* L); // получить пешеход последний состояние
int get_ped_stat_cheated(lua_State* L); // получить пешеход характеристика с читом
int get_ped_state(lua_State* L); // получить пешеход состояние
int get_ped_stat_kills(lua_State* L); // получить пешеход характеристика убийства
int get_player_display_money(lua_State* L); // получить игрок показать деньги
int get_player_money_info(lua_State* L); // получить игрок деньги информация
int get_radio_channel(lua_State* L); // получить радио канал
int get_stat_assassinations(lua_State* L); // получить характеристика заказные убийства
int get_stat_auto_painting_budget(lua_State* L); // получить характеристика авто картина бюджет
int get_stat_bloodring_kills(lua_State* L); // получить характеристика Bloodring убийства
int get_stat_bloodring_time(lua_State* L); // получить характеристика Bloodring время
int get_stat_fashion_budget(lua_State* L); // получить характеристика мода бюджет
int get_stat_flight_time(lua_State* L); // получить характеристика полёт время
int get_stat_frenzies_passed(lua_State* L); // получить характеристика бешенство пройдено
int get_stat_frenzies_total(lua_State* L); // получить характеристика бешенство всего
int get_stat_garbage_pickups(lua_State* L); // получить характеристика мусор пикапы
int get_stat_ice_cream_sold(lua_State* L); // получить характеристика лёд крем продано
int get_stat_loan_sharks(lua_State* L); // получить характеристика ссуда акулы
int get_stat_longest_2wheel_dist(lua_State* L); // получить характеристика самый длинный два колеса расстояние
int get_stat_longest_stoppie_dist(lua_State* L); // получить характеристика самый длинный стоппи расстояние
int get_stat_longest_wheelie_dist(lua_State* L); // получить характеристика самый длинный вилли расстояние
int get_stat_longest_wheelie_time(lua_State* L); // получить характеристика самый длинный вилли время
int get_stat_missions_total(lua_State* L); // получить характеристика миссии всего
int get_stat_movie_stunts(lua_State* L); // получить характеристика кино трюки
int get_stat_num_property_owned(lua_State* L); // получить характеристика собственность принадлежит
int get_stat_pizzas_delivered(lua_State* L); // получить характеристика пиццы доставлено
int get_stat_property_budget(lua_State* L); // получить характеристика собственность бюджет
int get_stat_property_destroyed(lua_State* L); // получить характеристика собственность уничтожен
int get_stat_seagulls_killed(lua_State* L); // получить характеристика чайки убит
int get_stat_shooting_rank(lua_State* L); // получить характеристика стрельба ранг
int get_stat_stores_knocked_off(lua_State* L); // получить характеристика магазины сбит
int get_stat_times_drowned(lua_State* L); // получить характеристика раз утоплен
int get_stat_top_shooting_score(lua_State* L); // получить характеристика верх стрельба очки
int get_stat_weapon_budget(lua_State* L); // получить характеристика оружие бюджет
int get_sun_blocked_by_clouds(lua_State* L); // получить солнце заблокирован облака
int get_timer_ms(lua_State* L); // получить таймер мс
int get_timer_ms_pause_mode(lua_State* L); // получить таймер мс пауза режим
int get_time_step(lua_State* L); // получить время шаг
int get_time_until(lua_State* L); // получить время до
int get_use_mouse_3rd_person(lua_State* L); // получить использовать мышь третий человек
int get_wanted_chaos_level(lua_State* L); // получить розыск хаос уровень
int get_wanted_last_decreased_time(lua_State* L); // получить розыск последний уменьшено время
int get_wanted_level_before_parole(lua_State* L); // получить розыск уровень before условное освобождение
int get_wanted_multiplier(lua_State* L); // получить розыск множитель
int get_weapon_firing_rate(lua_State* L); // получить оружие стрельба частота
int get_weapon_lifespan(lua_State* L); // получить оружие время жизни
int get_weapon_radius(lua_State* L); // получить оружие радиус
int get_weapon_range(lua_State* L); // получить оружие дальность
int get_weapon_reload(lua_State* L); // получить оружие перезарядка
int get_weapon_slot_info(lua_State* L); // получить оружие слот информация
int get_weapon_speed(lua_State* L); // получить оружие скорость
int get_weapon_spread(lua_State* L); // получить оружие разброс
int get_weapon_state(lua_State* L); // получить оружие состояние
int is_lightning_flash(lua_State* L); // проверить молния вспышка
int is_time_in_range(lua_State* L); // проверить время дальность
int is_user_pause(lua_State* L); // проверить пользователь пауза
int message_add(lua_State* L); // добавить сообщение
int message_add_big(lua_State* L); // Сообщение добавить большой
int message_add_big_queue(lua_State* L); // Сообщение добавить большой очередь
int message_add_jump_queue(lua_State* L); // Сообщение добавить прыжок очередь
int message_add_with_number(lua_State* L); // Сообщение добавить
int message_clear_all(lua_State* L); // Сообщение очистить
int message_clear_small_only(lua_State* L); // Сообщение очистить малый только
int message_clear_this_big_print(lua_State* L); // вывести сообщение очистить большой
int message_clear_this_print(lua_State* L); // вывести сообщение очистить
int obj_can_be_deleted(lua_State* L); // Объект может удалён
int obj_damage(lua_State* L); // Объект повреждение
int obj_delete_all_mission(lua_State* L); // Объект delete миссия
int obj_delete_all_temp(lua_State* L); // Объект delete температура
int obj_delete_all_temp_in_area(lua_State* L); // Объект delete температура область
int ped_answer_mobile(lua_State* L); // Пешеход ответ мобильный
int ped_blend_anim(lua_State* L); // Пешеход смешивание анимация
int ped_buy_ice_cream(lua_State* L); // Пешеход купить лёд крем
int ped_calculate_new_orientation(lua_State* L); // Пешеход calculate новый ориентация
int ped_can_be_deleted(lua_State* L); // Пешеход может удалён
int ped_clear_aim_flag(lua_State* L); // Пешеход очистить прицеливание флаг
int ped_clear_answer_mobile(lua_State* L); // Пешеход очистить ответ мобильный
int ped_clear_attack(lua_State* L); // Пешеход очистить атака
int ped_clear_attack_anim(lua_State* L); // Пешеход очистить атака анимация
int ped_clear_chat(lua_State* L); // Пешеход очистить чат
int ped_clear_follow_path(lua_State* L); // Пешеход очистить следовать путь
int ped_clear_look_flag(lua_State* L); // Пешеход очистить смотреть флаг
int ped_clear_point_gun_at(lua_State* L); // Пешеход очистить точка пушка
int ped_clear_seek(lua_State* L); // Пешеход очистить искать
int ped_clear_wait_state(lua_State* L); // Пешеход очистить ждать состояние
int ped_dress(lua_State* L); // Пешеход одежда
int ped_face_phone(lua_State* L); // Пешеход лицо телефон
int ped_fall(lua_State* L); // Пешеход падение
int ped_is_gang_member(lua_State* L); // Пешеход банда участник
int ped_is_in_control(lua_State* L); // Пешеход управление
int ped_is_player(lua_State* L); // Пешеход игрок
int ped_is_pointer_valid(lua_State* L); // Пешеход указатель действительный
int ped_is_shootable(lua_State* L); // Пешеход стреляемый
int ped_look_for_sexy_cars(lua_State* L); // Пешеход смотреть sexy авто
int ped_look_for_sexy_peds(lua_State* L); // Пешеход смотреть sexy пешеходы
int ped_make_phonecall(lua_State* L); // Пешеход сделать телефонный звонок
int ped_mug(lua_State* L); // Пешеход ограбление
int ped_pause(lua_State* L); // Пешеход пауза
int ped_play_anim(lua_State* L); // Пешеход проиграть анимация
int ped_restart_non_partial_anims(lua_State* L); // Пешеход перезапуск не частичный анимации
int ped_restore_heading_rate(lua_State* L); // Пешеход восстановить курс частота
int ped_restore_head_position(lua_State* L); // Пешеход восстановить голова позиция
int ped_restore_previous_objective(lua_State* L); // Пешеход восстановить предыдущий цель
int ped_restore_previous_state(lua_State* L); // Пешеход восстановить предыдущий состояние
int ped_set_dead(lua_State* L); // Пешеход установить мёртв
int ped_set_get_up(lua_State* L); // Пешеход установить получить
int ped_set_idle(lua_State* L); // Пешеход установить холостой
int ped_set_jump(lua_State* L); // прыгнуть пешеход установить
int ped_set_landing(lua_State* L); // Пешеход установить посадка
int ped_set_move_anim(lua_State* L); // Пешеход установить переместить анимация
int ped_solicit(lua_State* L); // Пешеход приставать
int ped_stop_non_partial_anims(lua_State* L); // Пешеход остановить не частичный анимации
int ped_turn_body(lua_State* L); // Пешеход повернуть кузов
int ped_update_position(lua_State* L); // Пешеход обновить позиция
int play_continuous_sound(lua_State* L); // проиграть непрерывный звук
int play_one_off_sound(lua_State* L); // проиграть один звук
int police_radio_message(lua_State* L); // Полиция радио сообщение
int population_convert_all_objects_to_dummy(lua_State* L); // Население преобразовать объекты манекен
int population_is_female(lua_State* L); // Население женский
int population_is_male(lua_State* L); // Население мужской
int population_is_sunbather(lua_State* L); // Население загорающий
int population_manage(lua_State* L); // Население управлять
int population_remove_peds_full_pool(lua_State* L); // Население удалить пешеходы полный бассейн
int radar_change_blip_brightness(lua_State* L); // Радар изменить блип радара яркость
int radar_change_blip_colour(lua_State* L); // Радар изменить блип радара цвет
int radar_change_blip_display(lua_State* L); // Радар изменить отображение блипа
int radar_change_blip_scale(lua_State* L); // Радар изменить блип радара масштаб
int radar_clear_blip(lua_State* L); // Радар очистить блип радара
int radar_clear_blip_for_entity(lua_State* L); // Радар очистить блип радара сущность
int radar_draw_radar_sprite(lua_State* L); // Радар рисовать радар спрайт
int radar_get_radar_range(lua_State* L); // Радар получить радар дальность
int radar_set_blip_sprite(lua_State* L); // Радар установить блип радара спрайт
int radar_add_sprite_blip_for_coord(lua_State* L);// 02A8 ADD_SPRITE_BLIP_FOR_COORD (x,y,z,sprite) -> id: спрайт-блайп на карте (рабочий, как в оригинале).
int radar_add_blip_for_coord(lua_State* L);// 018A ADD_BLIP_FOR_COORD (x,y,z) -> id: обычный координатный блайп на карте (рабочий, как в оригинале).
int radar_set_coord_blip(lua_State* L); // Радар установить координата блип радара
int radar_set_entity_blip(lua_State* L); // Радар установить сущность блип радара
int radar_set_marker_state(lua_State* L); // Радар установить маркер состояние
int radar_set_radar_range(lua_State* L); // Радар установить радар дальность
int radar_set_short_range_blip(lua_State* L); // Радар установить короткий дальность блип радара
int radar_show_marker(lua_State* L); // Радар показать маркер
int release_weather(lua_State* L); // освободить погода
int remove_sound_by_id(lua_State* L); // удалить звук
int set_all_random_peds_type(lua_State* L); // установить случайный пешеходы тип
int set_ammo_in_clip_ped(lua_State* L); // установить патроны обойма пешеход
int set_car_comedy_controls(lua_State* L); // установить авто комедия управление
int set_car_door_lock(lua_State* L); // установить авто дверь lock
int set_car_engine_on(lua_State* L); // установить авто двигатель
int set_car_handbrake_on(lua_State* L); // установить авто ручной тормоз
int set_car_hide_occupants(lua_State* L); // установить авто hide пассажиры
int set_car_lights_on(lua_State* L); // установить авто фары
int set_car_locked(lua_State* L); // установить авто заблокировано
int set_car_radio_station(lua_State* L); // установить авто радио станция
int set_car_steer_angle(lua_State* L); // установить авто руль угол
int set_car_tires_invulnerable(lua_State* L); // установить авто шины неуязвимый
int set_car_wanted_stars_on_enter(lua_State* L); // установить авто розыск звёзды розыска войти
int set_chance_on_roadblock(lua_State* L); // установить шанс блокпост
int set_fade_color(lua_State* L); // установить затемнение цвет
int set_far_clip(lua_State* L); // установить далеко обойма
int set_fire_time_to_burn(lua_State* L); // установить огонь время гореть
int Setflagmission(lua_State* L); // Установить флаг миссии
void stop_mission_watch();// остановить сторож миссии (файл-локальная, доступ из plugin.cpp).
int set_fov(lua_State* L); // установить угол обзора
int set_game_hours(lua_State* L); // установить игра часы
int set_lod_distance(lua_State* L); // установить детализация расстояние
int set_max_cop_cars_in_pursuit(lua_State* L); // установить максимум полицейский авто погоня
int set_max_cops_in_pursuit(lua_State* L); // установить максимум полиция погоня
int set_milliseconds_per_game_minute(lua_State* L); // установить миллисекунды на игра минута
int set_mouse_accel_horizontal(lua_State* L); // установить мышь ускорение горизонтальный
int set_mouse_accel_vertical(lua_State* L); // установить мышь ускорение вертикальный
int set_music_does_fade(lua_State* L); // установить музыка does затемнение
int set_near_clip(lua_State* L); // установить около обойма
int set_obj_attach_force(lua_State* L); // установить объект прикрепить сила
int set_obj_bonus_value(lua_State* L); // установить объект бонус значение
int set_obj_camera_avoids(lua_State* L); // установить объект камера избегает
int set_obj_collision_damage_type(lua_State* L); // установить объект столкновение повреждение тип
int set_obj_cost_value(lua_State* L); // установить объект стоимость значение
int set_obj_damage_multiplier(lua_State* L); // установить объект повреждение множитель
int set_obj_timer(lua_State* L); // установить объект таймер
int set_ped_density_multiplier(lua_State* L); // установить пешеход плотность множитель
int set_ped_heading_goal(lua_State* L); // установить пешеход курс цель
int set_ped_heading_rate(lua_State* L); // установить пешеход курс частота
int set_ped_state(lua_State* L); // установить пешеход состояние
int set_player_drive_by(lua_State* L); // установить игрок ездить
int set_player_money_info(lua_State* L); // установить игрок деньги информация
int set_radio_channel(lua_State* L); // установить радио канал
int set_wanted_chaos_level(lua_State* L); // установить розыск хаос уровень
int set_wanted_multiplier(lua_State* L); // установить розыск множитель
int set_weapon_firing_rate(lua_State* L); // установить оружие стрельба частота
int set_weapon_lifespan(lua_State* L); // установить оружие время жизни
int set_weapon_radius(lua_State* L); // установить оружие радиус
int set_weapon_range(lua_State* L); // установить оружие дальность
int set_weapon_reload(lua_State* L); // установить оружие перезарядка
int set_weapon_speed(lua_State* L); // установить оружие скорость
int set_weapon_spread(lua_State* L); // установить оружие разброс
int set_weather_type(lua_State* L); // установить погода тип
int wanted_are_army_required(lua_State* L); // Розыск армия требуется
int wanted_are_fbi_required(lua_State* L); // Розыск фбр требуется
int wanted_are_swat_required(lua_State* L); // Розыск swat требуется
int wanted_cheat_level(lua_State* L); // Розыск чит уровень
int wanted_clear_crimes(lua_State* L); // Розыск очистить преступления
int wanted_num_helis_required(lua_State* L); // Розыск вертолёты требуется
int wanted_reset_police_pursuit(lua_State* L); // Розыск сбросить полиция погоня
int wanted_set_no_drop(lua_State* L); // Розыск установить бросить
int wanted_update_wanted_level(lua_State* L); // Розыск обновить розыск уровень
int weapon_has_ammo_to_be_used(lua_State* L); // Оружие имеет патроны используется
int weapon_is_melee(lua_State* L); // Оружие рукопашная
int weapon_is_two_handed(lua_State* L); // Оружие два передан
int weapon_reload_ped(lua_State* L); // Оружие перезарядка пешеход
int weather_add_rain(lua_State* L); // Погода добавить дождь
int weather_add_stream_after_rain(lua_State* L); // Погода добавить поток после дождь
int weather_render_rain_streaks(lua_State* L); // Погода рендер дождь полосы
int world_add_entity(lua_State* L); // Мир добавить сущность
int world_call_off_chase_for_area(lua_State* L); // Мир звонок погоня область
int world_clear_cars_from_area(lua_State* L); // Мир очистить авто область
int world_clear_exciting_stuff_from_area(lua_State* L); // Мир очистить захватывающий вещи область
int world_clear_peds_from_area(lua_State* L); // Мир очистить пешеходы область
int world_clear_scan_codes(lua_State* L); // Мир очистить сканировать коды
int world_extinguish_all_car_fires_in_area(lua_State* L); // Мир погасить авто пожары область
int world_find_ground_z_for_3d_coord(lua_State* L); // Мир найти земля z 3d координата
int world_find_ground_z_for_coord(lua_State* L); // Мир найти земля z координата
int world_find_roof_z_for_3d_coord(lua_State* L); // Мир найти крыша z 3d координата
int world_get_is_line_of_sight_clear(lua_State* L); // очистить мир получить линия зрение
int get_ped_model_index(lua_State* L); // получить ID базовой модели педа (одежда рабочего = PLAYER3 и др.).
int get_current_skin_name(lua_State* L); // получить имя последнего установленного скина игрока (set_skin).
int get_player_skin(lua_State* L); // получить НАСТОЯЩЕЕ имя текущей модели игрока (CModelInfo).

// ---- НОВЫЕ ФУНКЦИИ (добавлены 2026-09-21 из gta vc база / reVC / IDA-отчёта) ----
int ped_set_look(lua_State* L); // заставить педа смотреть на сущность (CPed::SetLook).
int ped_set_look_flag(lua_State* L); // Пешеход установить смотреть флаг
int ped_can_see_entity(lua_State* L); // может ли пед видеть сущность (CPed::CanSeeEntity).
int ped_set_move_state(lua_State* L); // задать скорость передвижения педа (CPed::SetMoveState).
int ped_set_wander_path(lua_State* L); // вкл/выкл блуждание педа по пути (CPed::SetWanderPath).
int ped_set_follow_path(lua_State* L); // пед следует по пути к точке (CPed::SetFollowPath).
int ped_attach_to_entity(lua_State* L); // прикрепить педа к сущности (CPed::AttachPedToEntity).
int ped_point_gun_at(lua_State* L); // пед целится оружием в сущность (CPed::SetPointGunAt).
int car_join_road_system(lua_State* L); // встроить авто в систему дорог (CCarCtrl::JoinCarWithRoadSystem).
int car_join_road_goto(lua_State* L); // встроить авто в дороги с целью ехать к точке (CCarCtrl::JoinCarWithRoadSystemGotoCoors).
int car_switch_real_physics(lua_State* L); // вернуть авто настоящую физику (CCarCtrl::SwitchVehicleToRealPhysics).
int car_update_on_rails(lua_State* L); // обновить движение авто по рельсам (CCarCtrl::UpdateCarOnRails).
int car_set_component_rotation(lua_State* L); // повернуть компонент авто (CVehicle::SetComponentRotation).
int car_open_door(lua_State* L); // открыть/закрыть дверь авто (CVehicle::OpenDoor).
int car_goto_coors(lua_State* L); // поручить авто ИИ ехать к точке (CCarAI::GetCarToGoToCoors).
int car_park_at_coors(lua_State* L); // поручить авто ИИ припарковаться у точки (CCarAI::GetCarToParkAtCoors).
int player_nearby_peds_interact(lua_State* L); // педы рядом взаимодействуют с игроком (CPlayerPed::SetNearbyPedsToInteractWithPlayer).
int clear_adrenaline(lua_State* L); // снять адреналин игрока (CPlayerPed::ClearAdrenaline).
int annoy_player_ped(lua_State* L); // разозлить педа на игрока (CPlayerPed::AnnoyPlayerPed).
int set_max_wanted_level(lua_State* L); // задать максимум уровня розыска (CWanted::SetMaximumWantedLevel).
int register_crime(lua_State* L); // зарегистрировать преступление игрока (CWanted::RegisterCrime).
int register_crime_immediately(lua_State* L); // мгновенно зарегистрировать преступление (CWanted::RegisterCrime_Immediately).
int pad_shake(lua_State* L); // Панель тряска
int stop_pad_shaking(lua_State* L); // прекратить тряску геймпадов (CPad::StopPadsShaking).
int extinguish_point(lua_State* L); // потушить огонь в точке (CFireManager::ExtinguishPoint).
int extinguish_point_with_water(lua_State* L); // потушить огонь в точке водой (CFireManager::ExtinguishPointWithWater).
int is_script_fire_extinguished(lua_State* L); // потушен ли скриптовый огонь (CFireManager::IsScriptFireExtinguished).
int remove_script_fire(lua_State* L); // удалить скриптовый огонь (CFireManager::RemoveScriptFire).
int remove_all_script_fires(lua_State* L); // удалить все скриптовые огни (CFireManager::RemoveAllScriptFires).
int start_script_fire(lua_State* L); // запустить скриптовый огонь в точке (CFireManager::StartScriptFire).
int set_help_message(lua_State* L); // показать сообщение помощи (CHud::SetHelpMessage).
int get_percentage_progress(lua_State* L); // процент прохождения игры (CStats::GetPercentageProgress).
int get_criminal_rating(lua_State* L); // уровень преступника (CStats::FindCriminalRatingNumber).
int point_light_add(lua_State* L); // добавить точечный источник света (CPointLights::AddLight).
int shot_add_tracer(lua_State* L); // Выстрел добавить tracer
int add_jet_explosion(lua_State* L); // добавить взрыв-струю (CParticle::AddJetExplosion).
int add_particle(lua_State* L); // добавить частица

// ---- МИССИЯ "TREACHEROUS SWINE" (07, GENERL1): недостающие опкоды оригинала ----
int set_free_resprays(lua_State* L); // 0335: вкл/выкл бесплатные респреи (провал → респрай → PASS).
int has_respray_happened(lua_State* L); // 0329: был ли респрей в указанном гараже.
int set_camera_behind_player(lua_State* L); // 0373: камера за игроком.
int get_fading_status(lua_State* L); // получить затемнение статус
int get_game_timer(lua_State* L); // получить игра таймер
int set_chars_chatting(lua_State* L); // установить персонажи chatting
int create_random_char(lua_State* L); // 0376: создать случайного педа.
int dont_remove_object(lua_State* L); // Не удалить объект
int char_follow_path(lua_State* L); // Персонаж следовать путь
int is_char_wander_path_clear(lua_State* L); // 0510: свободен ли путь педа.
int turn_player_to_face_char(lua_State* L); // 0210: игрок лицом к педу.
int turn_char_to_face_player(lua_State* L); // повернуть персонаж лицо игрок
int turn_char_to_face_coord(lua_State* L); // повернуть персонаж лицо координата
int set_car_density_multiplier(lua_State* L); // установить авто плотность множитель
int request_collision(lua_State* L); // запросить столкновение
int switch_rubbish(lua_State* L); // Переключатель мусор
int switch_streaming(lua_State* L); // Переключатель streaming
int mark_char_as_no_longer_needed(lua_State* L); // пометить персонаж длиннее требуется
int set_visibility_of_closest_object_of_type(lua_State* L); // 0363: видимость ближайшего объекта типа.
int get_player_wanted_level(lua_State* L); // уровень розыска игрока (детектор респрея Swine). // 0363: видимость ближайшего объекта типа.

// ---- (06.11) Чистка при выходе из цикла Getflagmission (директива юзера) ----
// Тело destroy() вынесено из funcs.cpp: его зовут destroy() (end_mission/
// перезагрузка), Getflagmission() (миссия кончилась — флаг сменился или игрок
// погиб/арестован) и сторож миссии. Реестры per-lua_State, повторный вызов — no-op.
void purge_script_entities(lua_State* L);

// ---- (06.11) Новые функции и вариации прежних (по опкодам 07 - Treacherous Swine / main.scm) ----
int load_mission_audio(lua_State* L); // 03CF: загрузить миссийную аудиофразу (слот, имя).
int has_mission_audio_loaded(lua_State* L); // проверить миссия звук загружен
int play_mission_audio(lua_State* L); // проиграть миссия звук
int has_mission_audio_finished(lua_State* L); // проверить миссия звук завершено
int clear_mission_audio(lua_State* L); // очистить миссия звук
int set_everyone_ignore_player(lua_State* L); // 03BF: все игнорируют игрока (вариация set_police_ignore_player).
int alter_wanted_level(lua_State* L); // 010D: поставить уровень розыска (вариация set_wanted, с авто-спадом).
int alter_wanted_level_no_drop(lua_State* L); // 010E: уровень розыска без авто-спада (погоня после убийства цели).
int is_wanted_level_greater(lua_State* L); // 010F: розыск выше N? (вариация get_player_wanted_level).
int remove_char_elegantly(lua_State* L); // 034F: мягкое удаление педа (MISSION→RANDOM + fade) — вариация remove_ped.
int locate_char_any_means_2d(lua_State* L); // 00EC: пед (пешком/в авто) в круге — вариация ped_in_point_in_radius.
int locate_char_any_means_3d(lua_State* L); // найти персонаж любым способом в 3D
int locate_player_any_means_char_2d(lua_State* L); // 00E9: игрок рядом с педом (2D) — погоня Swine.
int locate_player_any_means_char_3d(lua_State* L); // найти игрок способ персонаж 3D
int is_player_in_area_3d(lua_State* L); // 0057: игрок в 3D-зоне ПО ДВУМ УГЛАМ (вариация ped_in_point_in_radius).
int is_char_in_area_3d(lua_State* L); // проверить персонаж область 3D
int player_made_progress(lua_State* L); // 030C: процент прохождения игры (CStats::ProgressMade).

// (доп.) Обёртки по значениям: стиль вождения, временные действия и миссии авто
int set_car_driving_style_stop_for_cars(lua_State* L); // установить авто вождение стиль остановить авто
int set_car_driving_style_slow_down_for_cars(lua_State* L); // установить авто вождение стиль медленно авто
int set_car_driving_style_avoid_cars(lua_State* L); // установить авто вождение стиль избегать авто
int set_car_driving_style_plough_through(lua_State* L); // установить авто вождение стиль вспахивать через
int set_car_driving_style_stop_for_cars_ignore_lights(lua_State* L); // установить авто вождение стиль остановить авто игнорировать фары
int car_temp_action_none(lua_State* L); // Авто температура действие нет
int car_temp_action_wait(lua_State* L); // ждать авто температура действие
int car_temp_action_reverse(lua_State* L); // Авто температура действие реверс
int car_temp_action_handbrake_turn_left(lua_State* L); // Авто температура действие ручной тормоз повернуть влево
int car_temp_action_handbrake_turn_right(lua_State* L); // Авто температура действие ручной тормоз повернуть вправо
int car_temp_action_handbrake_straight(lua_State* L); // Авто температура действие ручной тормоз прямо
int car_temp_action_turn_left(lua_State* L); // Авто температура действие повернуть влево
int car_temp_action_turn_right(lua_State* L); // Авто температура действие повернуть вправо
int car_temp_action_go_forward(lua_State* L); // Авто температура действие идти вперёд
int car_temp_action_swerve_left(lua_State* L); // Авто температура действие резкий поворот влево
int car_temp_action_swerve_right(lua_State* L); // Авто температура действие резкий поворот вправо
int car_mission_none(lua_State* L); // Авто миссия нет
int car_mission_cruise(lua_State* L); // Авто миссия круиз
int car_mission_ram_player_faraway(lua_State* L); // Авто миссия таран игрок далёкий
int car_mission_ram_player_close(lua_State* L); // закрыть авто миссия таран игрок
int car_mission_block_player_faraway(lua_State* L); // Авто миссия блок игрок далёкий
int car_mission_block_player_close(lua_State* L); // закрыть авто миссия блок игрок
int car_mission_block_player_handbrake_stop(lua_State* L); // остановить авто миссия блок игрок ручной тормоз
int car_mission_wait_for_deletion(lua_State* L); // Авто миссия ждать удаление
int car_mission_goto_coords(lua_State* L); // Авто миссия перейти координаты
int car_mission_goto_coords_straight(lua_State* L); // Авто миссия перейти координаты прямо
int car_mission_emergency_vehicle_stop(lua_State* L); // остановить авто миссия экстренный автомобиль
int car_mission_stop_forever(lua_State* L); // Авто миссия остановить навсегда
int car_mission_goto_coords_accurate(lua_State* L); // Авто миссия перейти координаты точный
int car_mission_goto_coords_straight_accurate(lua_State* L); // Авто миссия перейти координаты прямо точный
int car_mission_goto_coords_as_the_crow_swims(lua_State* L); // Авто миссия перейти координаты ворона плавает
int car_mission_ram_car_faraway(lua_State* L); // Авто миссия таран авто далёкий
int car_mission_ram_car_close(lua_State* L); // закрыть авто миссия таран авто
int car_mission_block_car_faraway(lua_State* L); // Авто миссия блок авто далёкий
int car_mission_block_car_close(lua_State* L); // закрыть авто миссия блок авто
int car_mission_block_car_handbrake_stop(lua_State* L); // остановить авто миссия блок авто ручной тормоз
int car_mission_heli_fly_to_coords(lua_State* L); // Авто миссия вертолёт лететь координаты
int car_mission_attack_player(lua_State* L); // Авто миссия атака игрок
int car_mission_plane_fly_to_coords(lua_State* L); // Авто миссия самолёт лететь координаты
int car_mission_heli_land(lua_State* L); // Авто миссия вертолёт приземлиться
int car_mission_slowly_drive_towards_player_1(lua_State* L); // Авто миссия медленно ездить к игрок 1
int car_mission_slowly_drive_towards_player_2(lua_State* L); // Авто миссия медленно ездить к игрок 2
int car_mission_block_player_forward_and_back(lua_State* L); // Авто миссия блок игрок вперёд назад

// (доп.) Обёртки: скорость передвижения педа, pedstat, банды
int ped_move_state_none(lua_State* L); // Пешеход двигать состояние нет
int ped_move_state_still(lua_State* L); // Пешеход двигать состояние всё ещё
int ped_move_state_walk(lua_State* L); // Пешеход двигать состояние ходить
int ped_move_state_run(lua_State* L); // Пешеход двигать состояние бежать
int ped_move_state_sprint(lua_State* L); // Пешеход двигать состояние спринт
int set_ped_stat_player(lua_State* L); // установить пешеход характеристика игрок
int set_ped_stat_cop(lua_State* L); // установить пешеход характеристика полицейский
int set_ped_stat_medic(lua_State* L); // установить пешеход характеристика медик
int set_ped_stat_fire(lua_State* L); // установить пешеход характеристика огонь
int set_ped_stat_gang1(lua_State* L); // установить пешеход характеристика gang1
int set_ped_stat_gang2(lua_State* L); // установить пешеход характеристика gang2
int set_ped_stat_gang3(lua_State* L); // установить пешеход характеристика gang3
int set_ped_stat_gang4(lua_State* L); // установить пешеход характеристика gang4
int set_ped_stat_gang5(lua_State* L); // установить пешеход характеристика gang5
int set_ped_stat_gang6(lua_State* L); // установить пешеход характеристика gang6
int set_ped_stat_gang7(lua_State* L); // установить пешеход характеристика gang7
int set_ped_stat_street_guy(lua_State* L); // установить пешеход характеристика улица парень
int set_ped_stat_suit_guy(lua_State* L); // установить пешеход характеристика костюм парень
int set_ped_stat_sensible_guy(lua_State* L); // установить пешеход характеристика разумный парень
int set_ped_stat_geek_guy(lua_State* L); // установить пешеход характеристика geek парень
int set_ped_stat_old_guy(lua_State* L); // установить пешеход характеристика старый парень
int set_ped_stat_tough_guy(lua_State* L); // установить пешеход характеристика жёсткий парень
int set_ped_stat_street_girl(lua_State* L); // установить пешеход характеристика улица девушка
int set_ped_stat_suit_girl(lua_State* L); // установить пешеход характеристика костюм девушка
int set_ped_stat_sensible_girl(lua_State* L); // установить пешеход характеристика разумный девушка
int set_ped_stat_geek_girl(lua_State* L); // установить пешеход характеристика geek девушка
int set_ped_stat_old_girl(lua_State* L); // установить пешеход характеристика старый девушка
int set_ped_stat_tough_girl(lua_State* L); // установить пешеход характеристика жёсткий девушка
int set_ped_stat_tramp_male(lua_State* L); // установить пешеход характеристика бродяга мужской
int set_ped_stat_tramp_female(lua_State* L); // установить пешеход характеристика бродяга женский
int set_ped_stat_tourist(lua_State* L); // установить пешеход характеристика турист
int set_ped_stat_prostitute(lua_State* L); // установить пешеход характеристика проститутка
int set_ped_stat_criminal(lua_State* L); // установить пешеход характеристика преступник
int set_ped_stat_busker(lua_State* L); // установить пешеход характеристика уличный музыкант
int set_ped_stat_taxidriver(lua_State* L); // установить пешеход характеристика таксист
int set_ped_stat_psycho(lua_State* L); // установить пешеход характеристика псих
int set_ped_stat_steward(lua_State* L); // установить пешеход характеристика стюард
int set_ped_stat_sportsfan(lua_State* L); // установить пешеход характеристика спортивный фанат
int set_ped_stat_shopper(lua_State* L); // установить пешеход характеристика покупатель
int set_ped_stat_oldshopper(lua_State* L); // установить пешеход характеристика старый покупатель
int set_ped_stat_beach_guy(lua_State* L); // установить пешеход характеристика пляж парень
int set_ped_stat_beach_girl(lua_State* L); // установить пешеход характеристика пляж девушка
int set_ped_stat_skater(lua_State* L); // установить пешеход характеристика скейтер
int set_ped_stat_std_mission(lua_State* L); // установить пешеход характеристика стандарт миссия
int set_ped_stat_coward(lua_State* L); // установить пешеход характеристика трус
int gang_enable_attack_player_with_cops(lua_State* L); // Банда enable атака игрок полиция
int gang_disable_attack_player_with_cops(lua_State* L); // Банда отключить атака игрок полиция
int reset_gangs(lua_State* L); // сбросить банды

// (20.11) Реализации без регистрации — добавить прототипы (иначе register.cpp их не видит).
int carmovespeed(lua_State* L); // двигать авто с заданной скоростью
int get_clip_current_weapon(lua_State* L); // получить обойма текущий оружие
int getpedarmour(lua_State* L); // Получить броню педа
int is_car_model(lua_State* L); // проверить авто модель
int is_ped_sitting_in_car(lua_State* L); // проверить пешеход сидит авто
int load_gtx_table(lua_State* L); // загрузить GXT текст таблица
int set_path_to_module(lua_State* L); // установить путь модуль
int set_player_control(lua_State* L); // установить игрок управление

// === AUTO-ADDED OPCODE WRAPPERS (20.11) ===
int print_with_number_big(lua_State* L); // вывести число крупным текстом
int add_score(lua_State* L); // добавить очки
int has_deatharrest_been_executed(lua_State* L); // проверить арест при смерти выполнен
int clear_prints(lua_State* L); // очистить тексты
int set_fading_colour(lua_State* L); // установить цвет затемнения
int load_special_character(lua_State* L); // загрузить специальный персонаж
int has_special_character_loaded(lua_State* L); // проверить специальный персонаж загружен
int set_cutscene_offset(lua_State* L); // установить смещение сцены
int load_cutscene(lua_State* L); // загрузить сцена
int create_cutscene_object(lua_State* L); // создать объект сцены
int set_cutscene_anim(lua_State* L); // установить анимация сцены
int start_cutscene(lua_State* L); // начать сцена
int get_cutscene_time(lua_State* L); // получить время сцены
int clear_cutscene(lua_State* L); // очистить сцена
int set_shortcut_dropoff_point_for_mission(lua_State* L); // установить точка высадки такси миссия
int clear_this_print(lua_State* L); // очистить текст с экрана
int is_player_in_any_car(lua_State* L); // проверить игрок в любом авто
int is_player_in_car(lua_State* L); // проверить игрок в авто
int get_player_coordinates(lua_State* L); // получить координаты игрока
int load_special_model(lua_State* L); // загрузить специальная модель
int is_char_in_car(lua_State* L); // проверить персонаж в авто
int is_char_in_any_car(lua_State* L); // проверить персонаж в любом авто
int locate_player_any_means_3d(lua_State* L); // найти игрок любым способом в 3D
int get_car_coordinates(lua_State* L); // получить координаты авто
int store_car_player_is_in(lua_State* L); // сохранить авто игрок
int clear_onscreen_counter(lua_State* L); // очистить счётчик на экране
int clear_extra_colours(lua_State* L); // очистить дополнительные цвета
int register_oddjob_mission_passed(lua_State* L); // зарегистрировать случайная работа пройдена
int locate_char_on_foot_2d(lua_State* L); // найти персонаж пешком 2D
int locate_car_2d(lua_State* L); // найти авто 2D
int display_onscreen_counter_with_string(lua_State* L); // Показать счётчик на экране со строкой
int set_extra_colours(lua_State* L); // установить дополнительные цвета
int set_char_obj_enter_car_as_driver(lua_State* L); // установить персонаж цель: сесть в авто за руль
int create_object_no_offset(lua_State* L); // создать объект без смещения
int change_blip_display(lua_State* L); // изменить отображение блипа
int set_char_health(lua_State* L); // установить персонаж здоровье
int locate_player_on_foot_3d(lua_State* L); // найти игрок пешком 3D
int get_char_coordinates(lua_State* L); // получить координаты персонажа
int register_fastest_time(lua_State* L); // зарегистрировать самый быстрый время
int print(lua_State* L); // вывести
int display_onscreen_timer(lua_State* L); // Показать на экране таймер
int locate_player_in_car_3d(lua_State* L); // найти игрок в авто 3D
int locate_player_any_means_car_2d(lua_State* L); // найти игрок способ авто 2D
int print_with_2_numbers_now(lua_State* L); // вывести 2 числа сейчас
int clear_small_prints(lua_State* L); // очистить малый тексты
int is_player_in_model(lua_State* L); // проверить игрок модель
int switch_car_generator(lua_State* L); // Переключатель авто генератор
int turn_char_to_face_char(lua_State* L); // повернуть персонаж лицо персонаж
int is_player_playing(lua_State* L); // проверить игрок играет
int is_point_obscured_by_a_mission_entity(lua_State* L); // проверить точка затемнён миссия сущность
int sqrt(lua_State* L); // Квадратный корень
int set_camera_in_front_of_player(lua_State* L); // установить камера передняя часть игрок
int locate_char_on_foot_3d(lua_State* L); // найти персонаж пешком 3D
int print_with_number_now(lua_State* L); // вывести сейчас
int was_cutscene_skipped(lua_State* L); // Сцена пропущено
int set_zone_ped_info(lua_State* L); // установить зона пешеход информация
int add_money_spent_on_property(lua_State* L); // добавить деньги потрачено собственность
int set_property_as_owned(lua_State* L); // установить собственность принадлежит
int get_object_coordinates(lua_State* L); // получить координаты объекта
int generate_random_int_in_range(lua_State* L); // Создать случайный целое число дальность
int set_char_cant_be_dragged_out(lua_State* L); // установить персонаж не может перетащен снаружи
int set_cutscene_anim_to_loop(lua_State* L); // установить анимация сцены цикл
int is_player_in_area_2d(lua_State* L); // проверить игрок область 2D
int set_time_of_day(lua_State* L); // установить время день
int locate_stopped_player_in_car_3d(lua_State* L); // найти остановлен игрок в авто 3D
int set_char_obj_enter_car_as_passenger(lua_State* L); // установить персонаж объект войти авто пассажир
int set_char_suffers_critical_hits(lua_State* L); // установить персонаж страдает критический попадания
int store_car_char_is_in(lua_State* L); // сохранить авто персонаж
int locate_player_any_means_2d(lua_State* L); // найти игрок способ 2D
int locate_stopped_player_on_foot_3d(lua_State* L); // найти остановлен игрок пешком 3D
int set_player_as_leader(lua_State* L); // установить игрок лидер
int add_short_range_sprite_blip_for_coord(lua_State* L); // добавить короткий дальность спрайт блип радара координата
int stop_char_looking(lua_State* L); // остановить персонаж смотрит
int has_cutscene_finished(lua_State* L); // проверить сцена завершено
int shut_player_up(lua_State* L); // Закрыть игрок
int is_char_health_greater(lua_State* L); // проверить персонаж здоровье больше
int store_wanted_level(lua_State* L); // сохранить розыск уровень
int is_char_in_players_group(lua_State* L); // проверить персонаж игроки группа
int make_player_safe_for_cutscene(lua_State* L); // сделать игрок безопасно сцена
int create_protection_pickup(lua_State* L); // создать защита пикап
int locate_stopped_player_any_means_3d(lua_State* L); // найти остановлен игрок любым способом в 3D
int char_look_at_player_always(lua_State* L); // Персонаж смотреть игрок всегда
int stop_player_looking(lua_State* L); // остановить игрок смотрит
int store_car_player_is_in_no_save(lua_State* L); // сохранить авто игрок сохранить
int get_offset_from_car_in_world_coords(lua_State* L); // получить смещение авто мир координаты
int shut_char_up(lua_State* L); // Закрыть персонаж
int draw_weaponshop_corona(lua_State* L); // нарисовать магазин оружия корона
int does_char_exist(lua_State* L); // Персонаж существовать
int point_camera_at_player(lua_State* L); // Точка камера игрок
int mark_object_as_no_longer_needed(lua_State* L); // пометить объект длиннее требуется
int get_player_char(lua_State* L); // получить игрок персонаж
int get_ground_z_for_3d_coord(lua_State* L); // получить земля Z 3D координата
int place_object_relative_to_car(lua_State* L); // Место объект относительный авто
int add_moving_particle_effect(lua_State* L); // добавить движущийся частица эффект
int change_garage_type(lua_State* L); // изменить гараж тип
int has_object_been_damaged(lua_State* L); // проверить объект повреждён
int is_player_sitting_in_car(lua_State* L); // проверить игрок сидит авто
int is_char_on_foot(lua_State* L); // проверить персонаж пешком
int is_car_in_area_2d(lua_State* L); // проверить авто область 2D
int locate_player_on_foot_2d(lua_State* L); // найти игрок пешком 2D
int locate_stopped_char_any_means_3d(lua_State* L); // найти остановлен персонаж любым способом в 3D
int set_char_as_leader(lua_State* L); // установить персонаж лидер
int locate_char_any_means_car_2d(lua_State* L); // найти персонаж способ авто 2D
int generate_random_float_in_range(lua_State* L); // Создать случайный вещественное число дальность
int get_char_health(lua_State* L); // получить персонаж здоровье
int char_look_at_char_always(lua_State* L); // Персонаж смотреть персонаж всегда
int player_look_at_char_always(lua_State* L); // Игрок смотреть персонаж всегда
int is_player_in_angled_area_3d(lua_State* L); // проверить игрок под углом область 3D
int get_closest_char_node(lua_State* L); // получить ближайший персонаж узел
int add_to_object_velocity(lua_State* L); // добавить объект скорость
int is_player_sitting_in_any_car(lua_State* L); // проверить игрок сидит авто
int is_char_sitting_in_any_car(lua_State* L); // проверить персонаж сидит авто
int disable_cutscene_shadows(lua_State* L); // отключить сцена тени
int add_number_of_assassinations(lua_State* L); // добавить заказные убийства
int load_uncompressed_anim(lua_State* L); // загрузить несжатый анимация
int locate_stopped_char_on_foot_2d(lua_State* L); // найти остановлен персонаж пешком 2D
int locate_char_any_means_char_2d(lua_State* L); // найти персонаж способ персонаж 2D
int is_player_dead(lua_State* L); // проверить игрок мёртв
int set_char_obj_wait_on_foot(lua_State* L); // установить персонаж объект ждать пешком
int add_route_point(lua_State* L); // добавить маршрут точка
int get_maximum_number_of_passengers(lua_State* L); // получить максимум пассажиры
int get_random_car_of_type_in_area(lua_State* L); // получить случайный авто тип область
int remove_route(lua_State* L); // удалить маршрут
int set_interpolation_parameters(lua_State* L); // установить интерполяция параметры
int has_animation_loaded(lua_State* L); // проверить анимация загружен
int display_nth_onscreen_counter_with_string(lua_State* L); // Показать n-й счётчик на экране со строкой
int is_char_obj_no_obj(lua_State* L); // проверить персонаж объект
int get_distance_between_coords_2d(lua_State* L); // получить расстояние между координаты 2D
int shake_cam(lua_State* L); // Тряска камера
int is_char_in_area_2d(lua_State* L); // проверить персонаж область 2D
int set_zone_car_info(lua_State* L); // установить зона авто информация
int add_blip_for_car_old(lua_State* L); // добавить блип радара авто старый
int add_blip_for_char_old(lua_State* L); // добавить блип радара персонаж старый
int set_char_obj_flee_on_foot_till_safe(lua_State* L); // установить персонаж объект побег пешком до безопасно
int locate_stopped_car_3d(lua_State* L); // найти остановлен авто 3D
int print_with_number(lua_State* L); // вывести
int get_number_of_passengers(lua_State* L); // получить пассажиры
int set_player_health(lua_State* L); // установить игрок здоровье
int get_player_health(lua_State* L); // получить игрок здоровье
int sin(lua_State* L); // Синус
int cos(lua_State* L); // Косинус
int explode_player_head(lua_State* L); // взорвать игрок голова
int is_area_occupied(lua_State* L); // проверить область занято
int swap_nearest_building_model(lua_State* L); // заменить ближайший здание модель
int store_car_char_is_in_no_save(lua_State* L); // сохранить авто персонаж сохранить
int get_closest_car_node_with_heading(lua_State* L); // получить ближайший авто узел курс
int set_mission_audio_position(lua_State* L); // установить миссия звук позиция
int get_ammo_in_player_weapon(lua_State* L); // получить патроны игрок оружие
int freeze_char_position(lua_State* L); // заморозить персонаж позиция
int add_big_gun_flash(lua_State* L); // добавить большой пушка вспышка
int is_player_pressing_horn(lua_State* L); // проверить игрок нажатие гудок
int point_camera_at_char(lua_State* L); // Точка камера персонаж
int set_player_ammo(lua_State* L); // установить игрок патроны
int is_player_health_greater(lua_State* L); // проверить игрок здоровье больше
int locate_player_any_means_car_3d(lua_State* L); // найти игрок способ авто 3D
int set_gang_weapons(lua_State* L); // установить банда оружия
int is_player_stopped(lua_State* L); // проверить игрок остановлен
int is_char_in_chars_group(lua_State* L); // проверить персонаж персонажи группа
int set_camera_zoom(lua_State* L); // установить камера масштаб
int set_text_scale(lua_State* L); // установить текст масштаб
int set_text_colour(lua_State* L); // установить текст цвет
int set_text_justify(lua_State* L); // установить текст выравнивание
int set_text_centre(lua_State* L); // установить текст центр
int set_text_wrapx(lua_State* L); // установить текст зацикливание X
int set_text_background(lua_State* L); // установить текст фон
int set_text_proportional(lua_State* L); // установить текст пропорциональный
int freeze_onscreen_timer(lua_State* L); // заморозить на экране таймер
int draw_sphere(lua_State* L); // нарисовать сфера
int set_text_right_justify(lua_State* L); // установить текст вправо выравнивание
int set_generate_cars_around_camera(lua_State* L); // установить создать авто вокруг камера
int use_text_commands(lua_State* L); // использовать текст команды
int set_car_can_be_damaged(lua_State* L); // установить авто может повреждён
int get_offset_from_object_in_world_coords(lua_State* L); // получить смещение объект мир координаты
int register_highest_score(lua_State* L); // зарегистрировать наибольший очки
int create_single_particle(lua_State* L); // создать одиночный частица
int is_player_on_foot(lua_State* L); // проверить игрок пешком
int set_object_rotation(lua_State* L); // установить объект вращение
int display_text_with_number(lua_State* L); // Показать текст
int get_random_cop_in_area(lua_State* L); // получить случайный полицейский область
int get_offset_from_char_in_world_coords(lua_State* L); // получить смещение персонаж мир координаты
int switch_security_camera(lua_State* L); // Переключатель охрана камера
int set_zone_civilian_car_info(lua_State* L); // установить зона гражданский авто информация
int is_char_waiting_for_world_collision(lua_State* L); // проверить персонаж ожидание мир столкновение
int attach_char_to_object(lua_State* L); // прикрепить персонаж объект
int is_player_wearing(lua_State* L); // проверить игрок носит
int attach_cutscene_object_to_bone(lua_State* L); // прикрепить объект сцены кость
int set_player_is_in_stadium(lua_State* L); // установить игрок стадион
int char_wander_dir(lua_State* L); // Персонаж бродить направление
int get_time_of_day(lua_State* L); // получить время день
int locate_player_in_car_2d(lua_State* L); // найти игрок в авто 2D
int locate_stopped_player_on_foot_2d(lua_State* L); // найти остановлен игрок пешком 2D
int locate_player_on_foot_char_2d(lua_State* L); // найти игрок пешком персонаж 2D
int locate_player_in_car_char_2d(lua_State* L); // найти игрок в авто персонаж 2D
int locate_char_in_car_2d(lua_State* L); // найти персонаж в авто 2D
int locate_player_on_foot_char_3d(lua_State* L); // найти игрок пешком персонаж 3D
int locate_player_in_car_char_3d(lua_State* L); // найти игрок в авто персонаж 3D
int is_score_greater(lua_State* L); // проверить очки больше
int store_score(lua_State* L); // сохранить очки
int set_deatharrest_state(lua_State* L); // установить арест при смерти состояние
int create_car_generator(lua_State* L); // создать авто генератор
int add_hospital_restart(lua_State* L); // добавить больница перезапуск
int add_police_restart(lua_State* L); // добавить полиция перезапуск
int add_blip_for_contact_point(lua_State* L); // добавить блип радара контакт точка
int locate_player_in_car_car_2d(lua_State* L); // найти игрок в авто авто 2D
int locate_char_on_foot_car_2d(lua_State* L); // найти персонаж пешком авто 2D
int locate_char_any_means_car_3d(lua_State* L); // найти персонаж способ авто 3D
int set_gang_ped_models(lua_State* L); // установить банда пешеход модели
int is_player_in_angled_area_2d(lua_State* L); // проверить игрок под углом область 2D
int set_zone_group(lua_State* L); // установить зона группа
int display_text(lua_State* L); // Показать текст
int open_garage(lua_State* L); // открыть гараж
int is_sniper_bullet_in_area(lua_State* L); // проверить снайпер пуля область
int print_string_in_string_now(lua_State* L); // вывести строка сейчас
int remove_particle_effects_in_area(lua_State* L); // удалить частица эффекты область
int is_garage_closed(lua_State* L); // проверить гараж закрыто
int does_object_exist(lua_State* L); // Объект существовать
int clear_this_big_print(lua_State* L); // очистить большой вывести текст
int no_special_camera_for_this_garage(lua_State* L); // Специальный камера гараж
int set_car_avoid_level_transitions(lua_State* L); // установить авто избегать уровень переходы
int get_car_model(lua_State* L); // получить авто модель
int display_text_with_2_numbers(lua_State* L); // Показать текст 2 числа
int get_number_of_followers(lua_State* L); // получить последователи
int get_current_char_weapon(lua_State* L); // получить текущий персонаж оружие
int remove_all_char_weapons(lua_State* L); // удалить персонаж оружия
int has_player_got_weapon(lua_State* L); // проверить игрок получено оружие
int get_position_of_analogue_sticks(lua_State* L); // получить позиция аналоговый палки
int set_object_path_speed(lua_State* L); // установить объект путь скорость
int get_closest_straight_road(lua_State* L); // получить ближайший прямо дорога
int get_nth_closest_car_node(lua_State* L); // получить n-й ближайший авто узел
int set_object_records_collisions(lua_State* L); // установить объект рекорды столкновения
int has_object_collided_with_anything(lua_State* L); // проверить объект столкнулся что-либо
int set_heli_stabiliser(lua_State* L); // установить вертолёт стабилизатор
int is_object_in_water(lua_State* L); // проверить объект вода
int set_player_can_do_drive_by(lua_State* L); // установить игрок может do ездить
int sort_out_object_collision_with_car(lua_State* L); // отсортировать снаружи объект столкновение авто
int print_help_forever(lua_State* L); // вывести помощь навсегда
int create_forsale_property_pickup(lua_State* L); // создать продаётся собственность пикап
int increase_player_max_health(lua_State* L); // увеличить игрок максимум здоровье
int increase_player_max_armour(lua_State* L); // увеличить игрок максимум броня
int create_random_char_as_passenger(lua_State* L); // создать случайный персонаж пассажир
int set_object_area_visible(lua_State* L); // установить объект область видимый
int set_char_crouch_when_threatened(lua_State* L); // установить персонаж присесть под угрозой
int set_allow_hurricanes(lua_State* L); // установить разрешить ураганы
int play_announcement(lua_State* L); // проиграть объявление
int create_player(lua_State* L); // создать игрок
int generate_random_int(lua_State* L); // Создать случайный целое число
int is_car_in_area_3d(lua_State* L); // проверить авто область 3D
int get_minutes_to_time_of_day(lua_State* L); // получить минуты время день
int is_char_in_model(lua_State* L); // проверить персонаж модель
int locate_char_on_foot_char_2d(lua_State* L); // найти персонаж пешком персонаж 2D
int locate_char_in_car_3d(lua_State* L); // найти персонаж в авто 3D
int locate_stopped_char_on_foot_3d(lua_State* L); // найти остановлен персонаж пешком 3D
int locate_char_any_means_char_3d(lua_State* L); // найти персонаж способ персонаж 3D
int override_next_restart(lua_State* L); // Переопределить следующий перезапуск
int declare_mission_flag(lua_State* L); // Объявить миссия флаг
int dont_remove_char(lua_State* L); // Не удалить персонаж
int cancel_override_restart(lua_State* L); // отменить переопределить перезапуск
int start_kill_frenzy(lua_State* L); // начать убить бешенство
int read_kill_frenzy_status(lua_State* L); // прочитать убить бешенство статус
int set_taxi_lights(lua_State* L); // установить такси фары
int print_big_q(lua_State* L); // вывести большой
int set_garage(lua_State* L); // установить гараж
int set_target_car_for_mission_garage(lua_State* L); // установить цель авто миссия гараж
int set_gang_car_model(lua_State* L); // установить банда авто модель
int store_clock(lua_State* L); // сохранить часы
int reset_num_of_models_killed_by_player(lua_State* L); // сбросить модели убит игрок
int get_num_of_models_killed_by_player(lua_State* L); // получить модели убит игрок
int is_player_in_angled_area_on_foot_3d(lua_State* L); // проверить игрок под углом область пешком 3D
int is_player_in_angled_area_in_car_3d(lua_State* L); // проверить игрок под углом область авто 3D
int is_player_shooting_in_area(lua_State* L); // проверить игрок стрельба область
int get_random_char_in_zone(lua_State* L); // получить случайный персонаж зона
int is_player_in_taxi(lua_State* L); // проверить игрок такси
int create_collectable1(lua_State* L); // создать собираемый предмет 1
int set_collectable1_total(lua_State* L); // установить собираемый предмет 1 всего
int is_char_model(lua_State* L); // проверить персонаж модель
int print_with_3_numbers(lua_State* L); // вывести 3 числа
int print_with_4_numbers(lua_State* L); // вывести 4 числа
int print_with_6_numbers(lua_State* L); // вывести 6 числа
int set_progress_total(lua_State* L); // установить прогресс всего
int register_jump_distance(lua_State* L); // зарегистрировать прыжок расстояние
int register_jump_height(lua_State* L); // зарегистрировать прыжок высота
int register_jump_flips(lua_State* L); // зарегистрировать прыжок перевороты
int register_jump_spins(lua_State* L); // зарегистрировать прыжок вращения
int register_jump_stunt(lua_State* L); // зарегистрировать прыжок трюк
int register_unique_jump_found(lua_State* L); // зарегистрировать уникальный прыжок найден
int set_unique_jumps_total(lua_State* L); // установить уникальный прыжки всего
int register_passenger_dropped_off_taxi(lua_State* L); // зарегистрировать пассажир выпущен такси
int register_money_made_taxi(lua_State* L); // зарегистрировать деньги сделано такси
int set_player_never_gets_tired(lua_State* L); // установить игрок никогда получить устал
int close_garage(lua_State* L); // закрыть гараж
int print_with_2_numbers_big(lua_State* L); // вывести 2 числа большой
int is_icecream_jingle_on(lua_State* L); // проверить мороженое джингл
int is_char_male(lua_State* L); // проверить персонаж мужской
int is_garage_open(lua_State* L); // проверить гараж открыть
int switch_world_processing(lua_State* L); // Переключатель мир обработка
int remove_all_player_weapons(lua_State* L); // удалить игрок оружия
int set_rotating_garage_door(lua_State* L); // установить вращение гараж дверь
int create_random_car_for_car_park(lua_State* L); // создать случайный авто парковать
int has_import_garage_slot_been_filled(lua_State* L); // проверить импорт гараж слот заполнен
int has_save_game_finished(lua_State* L); // проверить сохранить игра завершено
int force_random_ped_type(lua_State* L); // Сила случайный пешеход тип
int get_collectable1s_collected(lua_State* L); // получить собираемые предметы 1 собрано
int get_car_colours(lua_State* L); // получить авто цвета
int set_drunk_input_delay(lua_State* L); // установить пьян ввод задержка
int register_life_saved(lua_State* L); // зарегистрировать жизнь сохранено
int register_criminal_caught(lua_State* L); // зарегистрировать преступник пойман
int register_ambulance_level(lua_State* L); // зарегистрировать скорая помощь уровень
int register_fire_extinguished(lua_State* L); // зарегистрировать огонь погашен
int set_total_number_of_kill_frenzies(lua_State* L); // установить всего убить бешенство
int set_free_health_care(lua_State* L); // установить свободный здоровье забота
int load_and_launch_mission_internal(lua_State* L); // загрузить запустить миссия внутренний
int set_object_draw_last(lua_State* L); // установить объект рисовать последний
int convert_metres_to_feet(lua_State* L); // преобразовать метры ноги
int set_total_number_of_missions(lua_State* L); // установить всего миссии
int convert_metres_to_feet_int(lua_State* L); // преобразовать метры ноги целое число
int set_char_is_chris_criminal(lua_State* L); // установить персонаж Chris преступник
int start_credits(lua_State* L); // начать титры
int stop_credits(lua_State* L); // остановить титры
int are_credits_finished(lua_State* L); // проверить титры завершено
int play_end_of_game_tune(lua_State* L); // проиграть конец игра тюнинг
int stop_end_of_game_tune(lua_State* L); // остановить конец игра тюнинг
int are_any_car_cheats_activated(lua_State* L); // проверить авто читы активирован
int load_splash_screen(lua_State* L); // загрузить всплеск экран
int load_end_of_game_tune(lua_State* L); // загрузить конец игра тюнинг
int terminate_all_scripts_with_this_name(lua_State* L); // Прекратить скрипты имя
int get_current_player_weapon(lua_State* L); // получить текущий игрок оружие
int locate_char_any_means_object_2d(lua_State* L); // найти персонаж способ объект 2D
int locate_char_on_foot_object_2d(lua_State* L); // найти персонаж пешком объект 2D
int can_char_see_dead_char(lua_State* L); // проверить персонаж видеть мёртв персонаж
int is_any_pickup_at_coords(lua_State* L); // проверить пикап координаты
int initialise_object_path(lua_State* L); // Инициализировать объект путь
int start_object_on_path(lua_State* L); // начать объект путь
int set_object_path_position(lua_State* L); // установить объект путь позиция
int clear_object_path(lua_State* L); // очистить объект путь
int reset_havoc_caused_by_player(lua_State* L); // сбросить разрушение вызвано игрок
int get_havoc_caused_by_player(lua_State* L); // получить разрушение вызвано игрок
int create_script_roadblock(lua_State* L); // создать скрипт блокпост
int clear_all_script_roadblocks(lua_State* L); // очистить скрипт блокпосты
int has_char_been_photographed(lua_State* L); // проверить персонаж сфотографирован
int add_money_spent_on_clothes(lua_State* L); // добавить деньги потрачено одежда
int set_heli_orientation(lua_State* L); // установить вертолёт ориентация
int clear_heli_orientation(lua_State* L); // очистить вертолёт ориентация
int set_char_drowns_in_water(lua_State* L); // установить персонаж утопление вода
int get_char_armour(lua_State* L); // получить персонаж броня
int locate_object_2d(lua_State* L); // найти объект 2D
int locate_object_3d(lua_State* L); // найти объект 3D
int is_object_in_area_3d(lua_State* L); // проверить объект область 3D
int add_set_piece(lua_State* L); // добавить установить кусок
int get_wheelie_stats(lua_State* L); // получить вилли статистика
int switch_lift_camera(lua_State* L); // Переключатель подъёмник камера
int create_locked_property_pickup(lua_State* L); // создать заблокировано собственность пикап
int has_glass_been_shattered_nearby(lua_State* L); // проверить стекло разбито рядом
int attach_cutscene_object_to_component(lua_State* L); // прикрепить объект сцены компонент
int add_money_spent_on_weapons(lua_State* L); // добавить деньги потрачено оружия
int set_player_drunkenness(lua_State* L); // установить игрок опьянение
int add_stores_knocked_off(lua_State* L); // добавить магазины сбит
int add_pizzas_delivered(lua_State* L); // добавить пиццы доставлено
int add_ice_creams_sold(lua_State* L); // добавить лёд кремы продано
int set_char_in_players_group_can_fight(lua_State* L); // установить персонаж игроки группа может драка
int get_random_car_of_type_in_area_no_save(lua_State* L); // получить случайный авто тип область сохранить
int set_player_auto_aim(lua_State* L); // установить игрок авто прицеливание
int add_blood_ring_kills(lua_State* L); // добавить кровь кольцо убийства
int set_longest_time_in_blood_ring(lua_State* L); // установить самый длинный время кровь кольцо
int remove_everything_for_huge_cutscene(lua_State* L); // удалить всё огромный сцена
int is_player_touching_vehicle(lua_State* L); // проверить игрок касание автомобиль
int check_for_ped_model_around_player(lua_State* L); // проверить модель пешехода вокруг игрок
int attach_cutscene_object_to_vehicle(lua_State* L); // прикрепить объект сцены автомобиль
int set_tonights_event(lua_State* L); // установить сегодня вечером событие
int freeze_object_position(lua_State* L); // заморозить объект позиция
int set_player_has_met_debbie_harry(lua_State* L); // установить игрок имеет встретил Debbie Harry
int set_up_taxi_shortcut(lua_State* L); // установить такси сокращение
int clear_taxi_shortcut(lua_State* L); // очистить такси сокращение
int add_porn_leaflet_to_rubbish(lua_State* L); // добавить порно листовка мусор
int make_player_fire_proof(lua_State* L); // сделать игрок огонь защита
int create_random_char_as_driver(lua_State* L); // создать случайный персонаж водитель
int set_char_ignore_threats_behind_objects(lua_State* L); // установить персонаж игнорировать угрозы позади объекты
int make_heli_come_crashing_down(lua_State* L); // сделать вертолёт прийти авария
int is_char_stuck(lua_State* L); // проверить персонаж застрял
int set_all_taxis_have_nitro(lua_State* L); // установить такси иметь нитро
int register_vigilante_level(lua_State* L); // зарегистрировать vigilante уровень
int set_maximum_number_of_cars_in_garage(lua_State* L); // установить максимум авто гараж
int wanted_stars_are_flashing(lua_State* L); // Розыск звёзды розыска мигание
int get_bus_fares_collected_by_player(lua_State* L); // получить автобус плата за проезд собрано игрок
int register_best_position(lua_State* L); // зарегистрировать лучший позиция
int is_player_in_info_zone(lua_State* L); // проверить игрок информация зона
int has_char_attempted_attractor(lua_State* L); // проверить персонаж попытка притягиватель
int set_load_collision_for_char_flag(lua_State* L); // установить загрузить столкновение персонаж флаг
int has_char_bought_ice_cream(lua_State* L); // проверить персонаж куплено лёд крем
int get_progress_percentage(lua_State* L); // получить прогресс процент
int set_shortcut_pickup_point(lua_State* L); // установить сокращение пикап точка
int get_random_ice_cream_customer_in_area(lua_State* L); // получить случайный лёд крем клиент область
int set_char_frightened_in_jacked_car(lua_State* L); // установить персонаж напуган угнан авто
int is_player_in_shortcut_taxi(lua_State* L); // проверить игрок сокращение такси
int register_fire_level(lua_State* L); // зарегистрировать огонь уровень
// === AUTO-ADDED OPCODE WRAPPERS (20.11) ===

// === COMMUNITY ADDRESSES + HEADLIGHTS (20.11) ===
int patch_headlights(lua_State* L); // Патч фары
int set_headlights_on(lua_State* L); // установить фары
int set_headlights_off(lua_State* L); // установить фары
int is_headlights_on(lua_State* L); // проверить фары
int get_m_timepassingmeby_faster_time(lua_State* L); // получить чит TIMEPASSINGMEBY быстрее время
int set_m_timepassingmeby_faster_time(lua_State* L); // установить чит TIMEPASSINGMEBY быстрее время
int get_m_youwonttakemealive_wanted_level(lua_State* L); // получить чит YOUWONTTAKEMEALIVE розыск уровень
int set_m_youwonttakemealive_wanted_level(lua_State* L); // установить чит YOUWONTTAKEMEALIVE розыск уровень
int get_m_leavemealone_wanted_level(lua_State* L); // получить чит LEAVEMEALONE розыск уровень
int set_m_leavemealone_wanted_level(lua_State* L); // установить чит LEAVEMEALONE розыск уровень
int get_m_apleasantday_weather(lua_State* L); // получить чит APLEASANTDAY погода
int set_m_apleasantday_weather(lua_State* L); // установить чит APLEASANTDAY погода
int get_m_alovelyday_weather(lua_State* L); // получить чит ALOVELYDAY погода
int set_m_alovelyday_weather(lua_State* L); // установить чит ALOVELYDAY погода
int get_m_abitdrieg_weather(lua_State* L); // получить чит ABITDRIEG погода
int set_m_abitdrieg_weather(lua_State* L); // установить чит ABITDRIEG погода
int get_m_catsanddogs_weather(lua_State* L); // получить чит CATSANDDOGS погода
int set_m_catsanddogs_weather(lua_State* L); // установить чит CATSANDDOGS погода
int get_m_cantseeathing_weather(lua_State* L); // получить чит CANTSEEATHING погода
int set_m_cantseeathing_weather(lua_State* L); // установить чит CANTSEEATHING погода
int get_m_panzer_rhino_model(lua_State* L); // получить чит PANZER Rhino модель
int set_m_panzer_rhino_model(lua_State* L); // установить чит PANZER Rhino модель
int get_m_travelinstyle_bloodring_a_model(lua_State* L); // получить чит TRAVELINSTYLE Bloodring модель
int set_m_travelinstyle_bloodring_a_model(lua_State* L); // установить чит TRAVELINSTYLE Bloodring модель
int get_m_thelastride_romero_hearse_model(lua_State* L); // получить чит THELASTRIDE Romero катафалк модель
int set_m_thelastride_romero_hearse_model(lua_State* L); // установить чит THELASTRIDE Romero катафалк модель
int get_m_rockandrollcar_love_fist_model(lua_State* L); // получить чит ROCKANDROLLCAR любовь кулак модель
int set_m_rockandrollcar_love_fist_model(lua_State* L); // установить чит ROCKANDROLLCAR любовь кулак модель
int get_m_rubbishcar_trashmaster_model(lua_State* L); // получить чит RUBBISHCAR мусоровоз модель
int set_m_rubbishcar_trashmaster_model(lua_State* L); // установить чит RUBBISHCAR мусоровоз модель
int get_m_gettherefast_sabre_turbo_model(lua_State* L); // получить чит GETTHEREFAST Sabre турбо модель
int set_m_gettherefast_sabre_turbo_model(lua_State* L); // установить чит GETTHEREFAST Sabre турбо модель
int get_m_betterthanwalking_caddy_model(lua_State* L); // получить чит BETTERTHANWALKING Caddy модель
int set_m_betterthanwalking_caddy_model(lua_State* L); // установить чит BETTERTHANWALKING Caddy модель
int get_m_gettherequickly_bloodring_b_model(lua_State* L); // получить чит GETTHEREQUICKLY Bloodring модель
int set_m_gettherequickly_bloodring_b_model(lua_State* L); // установить чит GETTHEREQUICKLY Bloodring модель
int get_m_getthereveryfastindeed_hotring_a_model(lua_State* L); // получить чит GETTHEREVERYFASTINDEED Hotring модель
int set_m_getthereveryfastindeed_hotring_a_model(lua_State* L); // установить чит GETTHEREVERYFASTINDEED Hotring модель
int get_m_getthereamazinglyfast_hotring_b_model(lua_State* L); // получить чит GETTHEREAMAZINGLYFAST Hotring модель
int set_m_getthereamazinglyfast_hotring_b_model(lua_State* L); // установить чит GETTHEREAMAZINGLYFAST Hotring модель
int get_m_nuttertools_chainsaw_model(lua_State* L); // получить чит NUTTERTOOLS бензопила модель
int set_m_nuttertools_chainsaw_model(lua_State* L); // установить чит NUTTERTOOLS бензопила модель
int get_m_nuttertools_chainsaw_weapon_note(lua_State* L); // получить чит NUTTERTOOLS бензопила оружие примечание
int set_m_nuttertools_chainsaw_weapon_note(lua_State* L); // установить чит NUTTERTOOLS бензопила оружие примечание
int get_m_nuttertools_m4_weapon_note(lua_State* L); // получить чит NUTTERTOOLS M4 оружие примечание
int set_m_nuttertools_m4_weapon_note(lua_State* L); // установить чит NUTTERTOOLS M4 оружие примечание
int get_m_nuttertools_sniper_weapon_note(lua_State* L); // получить чит NUTTERTOOLS снайпер оружие примечание
int set_m_nuttertools_sniper_weapon_note(lua_State* L); // установить чит NUTTERTOOLS снайпер оружие примечание
int get_m_nuttertools_minigun_weapon_note(lua_State* L); // получить чит NUTTERTOOLS миниган оружие примечание
int set_m_nuttertools_minigun_weapon_note(lua_State* L); // установить чит NUTTERTOOLS миниган оружие примечание
int get_m_professionaltools_katana_model(lua_State* L); // получить чит PROFESSIONALTOOLS катана модель
int set_m_professionaltools_katana_model(lua_State* L); // установить чит PROFESSIONALTOOLS катана модель
int get_m_professionaltools_katana_weapon_note(lua_State* L); // получить чит PROFESSIONALTOOLS катана оружие примечание
int set_m_professionaltools_katana_weapon_note(lua_State* L); // установить чит PROFESSIONALTOOLS катана оружие примечание
int get_m_professionaltools_m4_weapon_note(lua_State* L); // получить чит PROFESSIONALTOOLS M4 оружие примечание
int set_m_professionaltools_m4_weapon_note(lua_State* L); // установить чит PROFESSIONALTOOLS M4 оружие примечание
int get_m_professionaltools_sniper_weapon_note(lua_State* L); // получить чит PROFESSIONALTOOLS снайпер оружие примечание
int set_m_professionaltools_sniper_weapon_note(lua_State* L); // установить чит PROFESSIONALTOOLS снайпер оружие примечание
int get_m_professionaltools_rocket_launcher_weapon(lua_State* L); // получить чит PROFESSIONALTOOLS ракета пусковая установка оружие
int set_m_professionaltools_rocket_launcher_weapon(lua_State* L); // установить чит PROFESSIONALTOOLS ракета пусковая установка оружие
int get_m_thugstools_brass_knuckles_model(lua_State* L); // получить чит THUGSTOOLS латунные кастеты модель
int set_m_thugstools_brass_knuckles_model(lua_State* L); // установить чит THUGSTOOLS латунные кастеты модель
int get_m_thugstools_brass_knuckles_weapon(lua_State* L); // получить чит THUGSTOOLS латунные кастеты оружие
int set_m_thugstools_brass_knuckles_weapon(lua_State* L); // установить чит THUGSTOOLS латунные кастеты оружие
int get_m_thugstools_tec_weapon_note(lua_State* L); // получить чит THUGSTOOLS Tec-9 оружие примечание
int set_m_thugstools_tec_weapon_note(lua_State* L); // установить чит THUGSTOOLS Tec-9 оружие примечание
int get_m_thugstools_kruger_weapon_note(lua_State* L); // получить чит THUGSTOOLS Kruger оружие примечание
int set_m_thugstools_kruger_weapon_note(lua_State* L); // установить чит THUGSTOOLS Kruger оружие примечание
int get_m_thugstools_sniper_rifle_weapon(lua_State* L); // получить чит THUGSTOOLS снайпер винтовка оружие
int set_m_thugstools_sniper_rifle_weapon(lua_State* L); // установить чит THUGSTOOLS снайпер винтовка оружие
int get_m_thugstools_flame_thrower_weapon(lua_State* L); // получить чит THUGSTOOLS пламя огнемёт оружие
int set_m_thugstools_flame_thrower_weapon(lua_State* L); // установить чит THUGSTOOLS пламя огнемёт оружие
int get_m_deepfriedmarsbars_programmer_tommy_body(lua_State* L); // получить чит DEEPFRIEDMARSBARS чит PROGRAMMER Томми кузов
int set_m_deepfriedmarsbars_programmer_tommy_body(lua_State* L); // установить чит DEEPFRIEDMARSBARS чит PROGRAMMER Томми кузов
int get_m_onspeed_game_speed(lua_State* L); // получить чит ONSPEED игра скорость
int set_m_onspeed_game_speed(lua_State* L); // установить чит ONSPEED игра скорость
int get_m_onspeed_game_speed_multiplier(lua_State* L); // получить чит ONSPEED игра скорость множитель
int set_m_onspeed_game_speed_multiplier(lua_State* L); // установить чит ONSPEED игра скорость множитель
int get_m_booooooring_game_speed(lua_State* L); // получить чит BOOOOOORING игра скорость
int set_m_booooooring_game_speed(lua_State* L); // установить чит BOOOOOORING игра скорость
int get_m_booooooring_game_speed_multiplier(lua_State* L); // получить чит BOOOOOORING игра скорость множитель
int set_m_booooooring_game_speed_multiplier(lua_State* L); // установить чит BOOOOOORING игра скорость множитель
int get_m_chasestat(lua_State* L); // получить чит CHASESTAT
int set_m_chasestat(lua_State* L); // установить чит CHASESTAT
int get_m_ourgodgivenrighttobeararms(lua_State* L); // получить чит OURGODGIVENRIGHTTOBEARARMS
int set_m_ourgodgivenrighttobeararms(lua_State* L); // установить чит OURGODGIVENRIGHTTOBEARARMS
int get_m_greenlight(lua_State* L); // получить чит GREENLIGHT
int set_m_greenlight(lua_State* L); // установить чит GREENLIGHT
int get_m_gripiseverything(lua_State* L); // получить чит GRIPISEVERYTHING
int set_m_gripiseverything(lua_State* L); // установить чит GRIPISEVERYTHING
int get_m_airship(lua_State* L); // получить дирижабль
int set_m_airship(lua_State* L); // установить дирижабль
int get_m_certaindeath(lua_State* L); // получить чит CERTAINDEATH
int set_m_certaindeath(lua_State* L); // установить чит CERTAINDEATH
int get_m_ahairdresserscar(lua_State* L); // получить чит AHAIRDRESSERS CAR
int set_m_ahairdresserscar(lua_State* L); // установить чит AHAIRDRESSERS CAR
int get_m_comeflywithme(lua_State* L); // получить чит COMEFLYWITHME
int set_m_comeflywithme(lua_State* L); // установить чит COMEFLYWITHME
int get_m_used_flag(lua_State* L); // получить используется флаг
int set_m_used_flag(lua_State* L); // установить используется флаг
int get_m_wheelsareallineed(lua_State* L); // получить чит WHEELSAREALLINEED
int set_m_wheelsareallineed(lua_State* L); // установить чит WHEELSAREALLINEED
int get_m_miamitraffic(lua_State* L); // получить чит MIAMITRAFFIC
int set_m_miamitraffic(lua_State* L); // установить чит MIAMITRAFFIC
int get_m_fannymagnet(lua_State* L); // получить чит FANNYMAGNET
int set_m_fannymagnet(lua_State* L); // установить чит FANNYMAGNET
int get_m_seaways(lua_State* L); // получить чит SEAWAYS
int set_m_seaways(lua_State* L); // установить чит SEAWAYS
int get_m_iwantitpaintedblack(lua_State* L); // получить чит IWANTITPAINTEDBLACK
int set_m_iwantitpaintedblack(lua_State* L); // установить чит IWANTITPAINTEDBLACK
int get_m_lifeispassingmeby(lua_State* L); // получить чит LIFEISPASSINGMEBY
int set_m_lifeispassingmeby(lua_State* L); // установить чит LIFEISPASSINGMEBY
int get_m_opcode_de_check_if(lua_State* L); // получить опкод DE проверить
int set_m_opcode_de_check_if(lua_State* L); // установить опкод DE проверить
int get_m_cash_when_entering_a(lua_State* L); // получить деньги вход
int set_m_cash_when_entering_a(lua_State* L); // установить деньги вход
int get_m_kaufman_cabs_radio_flag(lua_State* L); // получить Kaufman такси радио флаг
int set_m_kaufman_cabs_radio_flag(lua_State* L); // установить Kaufman такси радио флаг
int get_m_kaufman_cabs_spawn_note(lua_State* L); // получить Kaufman такси спавн примечание
int set_m_kaufman_cabs_spawn_note(lua_State* L); // установить Kaufman такси спавн примечание
int get_m_kaufman_cabs_model_load(lua_State* L); // получить Kaufman такси модель загрузить
int set_m_kaufman_cabs_model_load(lua_State* L); // установить Kaufman такси модель загрузить
int get_m_blue_fade_in_color(lua_State* L); // получить синий затемнение цвет
int set_m_blue_fade_in_color(lua_State* L); // установить синий затемнение цвет
int get_m_taxi_text_duration(lua_State* L); // получить такси текст длительность
int set_m_taxi_text_duration(lua_State* L); // установить такси текст длительность
int get_m_x_coord_taxi_destination(lua_State* L); // получить X координата такси пункт назначения
int set_m_x_coord_taxi_destination(lua_State* L); // установить X координата такси пункт назначения
int get_m_taxi_spawn(lua_State* L); // получить такси спавн
int set_m_taxi_spawn(lua_State* L); // установить такси спавн
int get_m_angle_taxi_destination(lua_State* L); // получить угол такси пункт назначения
int set_m_angle_taxi_destination(lua_State* L); // установить угол такси пункт назначения
int get_m_activate_mission_restart_taxi(lua_State* L); // получить activate миссия перезапуск такси
int set_m_activate_mission_restart_taxi(lua_State* L); // установить activate миссия перезапуск такси
int get_m_camera_move_when_entering(lua_State* L); // получить камера двигать вход
int set_m_camera_move_when_entering(lua_State* L); // установить камера двигать вход
int get_m_same(lua_State* L); // получить
int set_m_same(lua_State* L); // установить
int get_m_camera_distance_from_only(lua_State* L); // получить камера расстояние только
int set_m_camera_distance_from_only(lua_State* L); // установить камера расстояние только
int get_m_cinema_top_border_height(lua_State* L); // получить кинорежим верх полоса высота
int set_m_cinema_top_border_height(lua_State* L); // установить кинорежим верх полоса высота
int get_m_cinema_bottom_border_height(lua_State* L); // получить кинорежим низ полоса высота
int set_m_cinema_bottom_border_height(lua_State* L); // установить кинорежим низ полоса высота
int get_m_controls_radar_map(lua_State* L); // получить управление радар карта
int set_m_controls_radar_map(lua_State* L); // установить управление радар карта
int get_m_radar_width(lua_State* L); // получить радар ширина
int set_m_radar_width(lua_State* L); // установить радар ширина
int get_m_only_radar_ring_visible(lua_State* L); // получить только радар кольцо видимый
int set_m_only_radar_ring_visible(lua_State* L); // установить только радар кольцо видимый
int get_m_radar_size_xy(lua_State* L); // получить радар размер XY
int set_m_radar_size_xy(lua_State* L); // установить радар размер XY
int get_m_radar_x_position(lua_State* L); // получить радар X позиция
int set_m_radar_x_position(lua_State* L); // установить радар X позиция
int get_m_radar_y_position(lua_State* L); // получить радар Y позиция
int set_m_radar_y_position(lua_State* L); // установить радар Y позиция
int get_m_radar_shape(lua_State* L); // получить радар форма
int set_m_radar_shape(lua_State* L); // установить радар форма
int get_m_addr_68FD3C(lua_State* L); // получить адрес 68 FD3 C
int set_m_addr_68FD3C(lua_State* L); // установить адрес 68 FD3 C
int get_m_ambient_lighting(lua_State* L); // получить окружающий освещение
int set_m_ambient_lighting(lua_State* L); // установить окружающий освещение
int get_m_camera_x_position(lua_State* L); // получить камера X позиция
int set_m_camera_x_position(lua_State* L); // установить камера X позиция
int get_m_camera_y_position(lua_State* L); // получить камера Y позиция
int set_m_camera_y_position(lua_State* L); // установить камера Y позиция
int get_m_camera_z_position(lua_State* L); // получить камера Z позиция
int set_m_camera_z_position(lua_State* L); // установить камера Z позиция
int get_m_wide_screen_borders(lua_State* L); // получить широкий экран полосы
int set_m_wide_screen_borders(lua_State* L); // установить широкий экран полосы
int get_m_car_camera(lua_State* L); // получить авто камера
int set_m_car_camera(lua_State* L); // установить авто камера
int get_m_camera(lua_State* L); // получить камера
int set_m_camera(lua_State* L); // установить камера
int get_m_frame_limiter(lua_State* L); // получить кадр ограничитель
int set_m_frame_limiter(lua_State* L); // установить кадр ограничитель
int get_m_subtitles(lua_State* L); // получить субтитры
int set_m_subtitles(lua_State* L); // установить субтитры
int get_m_widescreen(lua_State* L); // получить широкоэкранный
int set_m_widescreen(lua_State* L); // установить широкоэкранный
int get_m_hud_display(lua_State* L); // получить HUD показать
int set_m_hud_display(lua_State* L); // установить HUD показать
int get_m_replay_display(lua_State* L); // получить повтор показать
int set_m_replay_display(lua_State* L); // установить повтор показать
int get_m_white_scanlines(lua_State* L); // получить белый линии развёртки
int set_m_white_scanlines(lua_State* L); // установить белый линии развёртки
int get_m_green_scanlines(lua_State* L); // получить зелёный линии развёртки
int set_m_green_scanlines(lua_State* L); // установить зелёный линии развёртки
int get_m_resolution_width(lua_State* L); // получить разрешение ширина
int set_m_resolution_width(lua_State* L); // установить разрешение ширина
int get_m_resolution_height(lua_State* L); // получить разрешение высота
int set_m_resolution_height(lua_State* L); // установить разрешение высота
int get_m_color_bit(lua_State* L); // получить цвет глубина
int set_m_color_bit(lua_State* L); // установить цвет глубина
int get_m_mouse_x_movement(lua_State* L); // получить мышь X движение
int set_m_mouse_x_movement(lua_State* L); // установить мышь X движение
int get_m_mouse_y_movement(lua_State* L); // получить мышь Y движение
int set_m_mouse_y_movement(lua_State* L); // установить мышь Y движение
int get_m_total_stores_knock(lua_State* L); // получить всего магазины ограбление
int set_m_total_stores_knock(lua_State* L); // установить всего магазины ограбление
int get_m_total_assassination_contracts_complete(lua_State* L); // получить всего убийство контракты завершить
int set_m_total_assassination_contracts_complete(lua_State* L); // установить всего убийство контракты завершить
int get_m_police_helicopter_state(lua_State* L); // получить полиция вертолёт состояние
int set_m_police_helicopter_state(lua_State* L); // установить полиция вертолёт состояние
int get_m_size_police_em_lights(lua_State* L); // получить размер полиция проблесковый фары
int set_m_size_police_em_lights(lua_State* L); // установить размер полиция проблесковый фары
int get_m_first_police_car(lua_State* L); // получить первый полиция авто
int set_m_first_police_car(lua_State* L); // установить первый полиция авто
int get_m_second_police_car(lua_State* L); // получить второй полиция авто
int set_m_second_police_car(lua_State* L); // установить второй полиция авто
int get_m_enforcer(lua_State* L); // получить Enforcer
int set_m_enforcer(lua_State* L); // установить Enforcer
int get_m_vicechee(lua_State* L); // получить Vicechee
int set_m_vicechee(lua_State* L); // установить Vicechee
int get_m_addr_4268D3(lua_State* L); // получить адрес 4268 D3
int set_m_addr_4268D3(lua_State* L); // установить адрес 4268 D3
int get_m_addr_4268EE(lua_State* L); // получить адрес 4268 EE
int set_m_addr_4268EE(lua_State* L); // установить адрес 4268 EE
int get_m_addr_426909(lua_State* L); // получить адрес 426909
int set_m_addr_426909(lua_State* L); // установить адрес 426909
int get_m_fbi_rancher(lua_State* L); // получить ФБР Rancher
int set_m_fbi_rancher(lua_State* L); // установить ФБР Rancher
int get_m_barracks_ol(lua_State* L); // получить казарма OL
int set_m_barracks_ol(lua_State* L); // установить казарма OL
// === COMMUNITY ADDRESSES + HEADLIGHTS (20.11) ===

// === ENGINE / LUA INTROSPECTION (21.11) ===
int get_lua_version(lua_State* L); // получить Lua версия
int lua_memory_used(lua_State* L); // Lua память используется
int lua_gc_collect(lua_State* L); // Lua сбор мусора собирать
int lua_gc_stop(lua_State* L); // остановить Lua сбор мусора
int lua_gc_restart(lua_State* L); // Lua сбор мусора перезапуск
int lua_gc_step(lua_State* L); // Lua сбор мусора шаг
int lua_is_yieldable(lua_State* L); // Lua потоковый
int lua_stack_top(lua_State* L); // Lua стопка верх
int get_current_script_name(lua_State* L); // получить текущий скрипт имя
int get_game_speed(lua_State* L); // получить игра скорость
int set_gamespeed(lua_State* L); // установить скорость игры
int get_actor_health(lua_State* L); // получить актёр здоровье
int set_actor_health(lua_State* L); // установить актёр здоровье
// === ENGINE / LUA INTROSPECTION (21.11) ===

// === LOG LINE (21.11) ===
int log_line(lua_State* L); // Журнал линия
// === LOG LINE (21.11) ===

// === AIRSHIP FLIGHT FLAG (21.11) ===
int set_airship_cheat(lua_State* L); // установить дирижабль чит
int is_airship_cheat(lua_State* L); // проверить дирижабль чит
// === AIRSHIP FLIGHT FLAG (21.11) ===

// === CAR GAS PEDAL (21.11) ===
int set_car_gas(lua_State* L); // установить авто газ
// === CAR GAS PEDAL (21.11) ===

// === MISSION LAUNCH (21.11) ===
int start_mission(lua_State* L); // начать миссия
int start_mission_orig(lua_State* L); // начать миссия оригинал
// === MISSION LAUNCH (21.11) ===

// === ORIGINAL MISSION NAMES (21.11) ===
int get_original_mission_name(lua_State* L); // получить исходный миссия имя
int get_original_mission_script(lua_State* L); // получить исходный миссия скрипт
int show_original_mission(lua_State* L); // показать исходный миссия
int show_original_missions(lua_State* L); // показать исходный миссии
int start_mission_original(lua_State* L); // начать миссия исходный
void ma_mission_list_draw();
int hide_original_missions(lua_State* L); // кадровая отрисовка списка миссий
// === ORIGINAL MISSION NAMES (21.11) ===
int get_model_id_by_name(lua_State* L); // получить модель имя
int check_studio_gates(lua_State* L); // проверить студия ворота
int open_studio_gates(lua_State* L); // открыть студия ворота
int set_car_only_damaged_by_player(lua_State* L); // установить авто только повреждён игрок
int open_diaz_gates(lua_State* L); // открыть Diaz ворота
int open_west_diaz_gates(lua_State* L); // открыть запад Diaz ворота
int open_east_diaz_gates(lua_State* L); // открыть восток Diaz ворота
int get_flag(lua_State* L); // получить флаг
int set_flag(lua_State* L); // установить флаг
int flag_addr(lua_State* L); // Флаг адрес
int get_flag_airship(lua_State* L); // получить флаг дирижабль
int set_flag_airship(lua_State* L); // установить флаг дирижабль
int get_flag_rc_zone(lua_State* L); // получить флаг радиоуправляемый зона
int set_flag_rc_zone(lua_State* L); // установить флаг радиоуправляемый зона
int get_flag_rc_mode(lua_State* L); // получить флаг радиоуправляемый режим
int set_flag_rc_mode(lua_State* L); // установить флаг радиоуправляемый режим
int get_flag_airship2(lua_State* L); // получить флаг дирижабль 2
int set_flag_airship2(lua_State* L); // установить флаг дирижабль 2
int get_flag_cheat82(lua_State* L); // получить флаг чит 82
int set_flag_cheat82(lua_State* L); // установить флаг чит 82
int get_flag_cheat26(lua_State* L); // получить флаг чит 26
int set_flag_cheat26(lua_State* L); // установить флаг чит 26
int get_flag_cheatadc(lua_State* L); // получить флаг чит ADC
int set_flag_cheatadc(lua_State* L); // установить флаг чит ADC
int get_flag_cheat23(lua_State* L); // получить флаг чит 23
int set_flag_cheat23(lua_State* L); // установить флаг чит 23
int get_flag_cheat47(lua_State* L); // получить флаг чит 47
int set_flag_cheat47(lua_State* L); // установить флаг чит 47
int get_flag_cheat5f(lua_State* L); // получить флаг чит 5F
int set_flag_cheat5f(lua_State* L); // установить флаг чит 5F
int get_flag_cheatstate(lua_State* L); // получить флаг состояние чита
int set_flag_cheatstate(lua_State* L); // установить флаг состояние чита
int get_flag_named(lua_State* L); // получить флаг по имени
int set_flag_named(lua_State* L); // установить флаг по имени
int list_flags(lua_State* L); // Список флаги
int keep_rc(lua_State* L); // удерживать радиоуправляемый
int get_mem_addr(lua_State* L); // получить память адрес
int force_rc_autopilot(lua_State* L); // Сила радиоуправляемый автопилот
int press_fire(lua_State* L); // стрелять нажать
int release_fire(lua_State* L); // освободить огонь
int set_script_int(lua_State* L); // установить скрипт целое число
int get_script_int(lua_State* L); // получить скрипт целое число
int angle_entity_to_entity(lua_State* L); // 23.11: угол 2-й сущности к 1-й (любые пары)
int angle_to_entity(lua_State* L); // Угол сущность
int get_angle_to_target(lua_State* L); // получить угол цель
int set_all_timers_99(lua_State* L);   // все убывающие таймеры миссий -> 99 минут (сохранить original)
int restore_all_timers(lua_State* L);  // вернуть таймеры к значениям до вызова set_all_timers_99
int map_overlay_on(lua_State* L);      // 23.11: показать карту меню
int map_overlay_off(lua_State* L);     // скрыть карту меню
int map_overlay_toggle(lua_State* L);  // переключить карту меню
int map_overlay_is_on(lua_State* L); // Карта overlay
int map_crosshair_world(lua_State* L); // Карта crosshair мир
int map_coords_diag(lua_State* L); // Карта координаты diag
int map_cross_set(lua_State* L); // установить карта cross
int CreateSpecialActor(lua_State* L);   // 23.11: макрос — сам грузит/создаёт/выгружает
int map_cross_get(lua_State* L); // получить карта cross
int play_wav_file(lua_State* L);       // 23.11: play_wav_file("Audio/fin_1a.wav") — проиграть wav файл
int stop_wav_file(lua_State* L);       // 23.11: остановить звук
int set_pad_controls(lua_State* L); // установить панель управление
int sys_key_down(lua_State* L); // Система клавиша
int sys_key_up(lua_State* L); // Система клавиша
int sys_mouse_down(lua_State* L); // Система мышь
int sys_mouse_up(lua_State* L); // Система мышь
int find_all_peds_by_model(lua_State* L); // найти пешеходы модель

// (28.11) GTAMods адреса (26 штук)
int get_m_remove_distant_cars(lua_State* L); // получить удалить далёкий авто
int set_m_remove_distant_cars(lua_State* L); // установить удалить далёкий авто
int get_m_spawn_with_siren_horn(lua_State* L); // получить спавн сирена гудок
int set_m_spawn_with_siren_horn(lua_State* L); // установить спавн сирена гудок
int get_m_rhino_cannon_reaction(lua_State* L); // получить Rhino пушка реакция
int set_m_rhino_cannon_reaction(lua_State* L); // установить Rhino пушка реакция
int get_m_rhino_cannon_range(lua_State* L); // получить Rhino пушка дальность
int set_m_rhino_cannon_range(lua_State* L); // установить Rhino пушка дальность
int get_m_rhino_firetrail_radius(lua_State* L); // получить Rhino огненный след радиус
int set_m_rhino_firetrail_radius(lua_State* L); // установить Rhino огненный след радиус
int get_m_rhino_smoke_radius(lua_State* L); // получить Rhino дым радиус
int set_m_rhino_smoke_radius(lua_State* L); // установить Rhino дым радиус
int get_m_rhino_spark_radius(lua_State* L); // получить Rhino искра радиус
int set_m_rhino_spark_radius(lua_State* L); // установить Rhino искра радиус
int get_m_cuban_exhaust_z(lua_State* L); // получить Cuban выхлоп Z
int set_m_cuban_exhaust_z(lua_State* L); // установить Cuban выхлоп Z
int get_m_cuban_exhaust_x(lua_State* L); // получить Cuban выхлоп X
int set_m_cuban_exhaust_x(lua_State* L); // установить Cuban выхлоп X
int get_m_cuban_exhaust_y(lua_State* L); // получить Cuban выхлоп Y
int set_m_cuban_exhaust_y(lua_State* L); // установить Cuban выхлоп Y
int get_m_radio_font_style(lua_State* L); // получить радио шрифт стиль
int set_m_radio_font_style(lua_State* L); // установить радио шрифт стиль
int get_m_ped_car_accident_chance(lua_State* L); // получить пешеход авто авария шанс
int set_m_ped_car_accident_chance(lua_State* L); // установить пешеход авто авария шанс
int get_m_force_law_seconds(lua_State* L); // получить сила закон секунды
int set_m_force_law_seconds(lua_State* L); // установить сила закон секунды
int get_m_tick_rate(lua_State* L); // получить тик частота
int set_m_tick_rate(lua_State* L); // установить тик частота
int get_m_entity_size(lua_State* L); // получить сущность размер
int set_m_entity_size(lua_State* L); // установить сущность размер
int get_m_moon_size(lua_State* L); // получить луна размер
int set_m_moon_size(lua_State* L); // установить луна размер
int get_m_car_friction(lua_State* L); // получить авто трение
int set_m_car_friction(lua_State* L); // установить авто трение
int get_m_wheel_camber(lua_State* L); // получить колесо развал
int set_m_wheel_camber(lua_State* L); // установить колесо развал
int get_m_car_door_speed(lua_State* L); // получить авто дверь скорость
int set_m_car_door_speed(lua_State* L); // установить авто дверь скорость
int get_m_max_heli_height(lua_State* L); // получить максимум вертолёт высота
int set_m_max_heli_height(lua_State* L); // установить максимум вертолёт высота
int get_m_car_gear(lua_State* L); // получить авто передача
int set_m_car_gear(lua_State* L); // установить авто передача
int get_m_water_level_1(lua_State* L); // получить вода уровень 1
int set_m_water_level_1(lua_State* L); // установить вода уровень 1
int get_m_water_level_2(lua_State* L); // получить вода уровень 2
int set_m_water_level_2(lua_State* L); // установить вода уровень 2
int get_m_respawn_depth(lua_State* L); // получить возрождение глубина
int set_m_respawn_depth(lua_State* L); // установить возрождение глубина
int get_m_player_pitch(lua_State* L); // получить игрок угол наклона
int set_m_player_pitch(lua_State* L); // установить игрок угол наклона
int get_m_player_yaw(lua_State* L); // получить игрок угол поворота
int set_m_player_yaw(lua_State* L); // установить игрок угол поворота
