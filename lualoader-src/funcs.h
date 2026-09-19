
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
#include <mutex>
#include <atomic> // std::atomic — сторож миссии

#include <algorithm>
#include<lua.hpp>

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
string getkey(int key);
int setobjоcoordes(lua_State* L); // установить координаты для объект. (определение в plugin.cpp)
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
void cpp_trace(const char x[]);// запись трейса C++ (в тот же trace.txt).
void cpp_tracef(const char* fmt, ...);// форматированная запись трейса C++.
void dellod(); // удалить лог ошибок.
int cleanstl();// очистка stl.

int wait(lua_State* L);// задержка
int findplayer(lua_State* L);// найти педа.
int getpedhealth(lua_State* L);// получить здоровье педа.
int setpedhealth(lua_State* L);// установить здоровье педа.

int getpedarmour(lua_State* L); // получить броню педа.
int setarmour(lua_State* L);// установить броню педа.
int getcarhealth(lua_State* L); // получить кол-во здоровья авто.
int setcarhealth(lua_State* L); // установить здоровье авто.

int loadmodel(lua_State* L);// загрузить модель.
int createcar(lua_State* L);// создать авто на координатах на координатах.
int availablemodel(lua_State* L);// проверка на загруженность модели.
int releasemodel(lua_State* L);// удалить модель из памяти.

int keypress(lua_State* L);// проверка на нажатия клавиши.
int lockstatus(lua_State* L);// статус двери авто.
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
int set_current_weapon_ped(lua_State* L);// установить текущее оружие педа (01B9).


int set_ped_wait_state(lua_State* L);// уст. ожидания педа (0372: SET_CHAR_WAIT_STATE).
int set_ped_personality(lua_State* L);// уст. личность педа (0243: SET_CHAR_PERSONALITY).
int set_ped_only_damaged_by_player(lua_State* L);// пед ранен только игроком (02A9: SET_CHAR_ONLY_DAMAGED_BY_PLAYER).
int is_ped_health_greater(lua_State* L);// здоровье педа больше? (0184: IS_CHAR_HEALTH_GREATER, возвращает bool).
int make_obj_targettable(lua_State* L);// в объект можно целиться (035D: MAKE_OBJECT_TARGETTABLE).
int get_dead_char_pickup_coords(lua_State* L);// координаты трупного пикапа (04A5: GET_DEAD_CHAR_PICKUP_COORDS, 3 float).
int set_ped_mood(lua_State* L);// настроение педа (04E3: SET_PLAYER_MOOD).
int request_animation(lua_State* L);// запросить анимацию по имени (04ED: REQUEST_ANIMATION).
int remove_animation(lua_State* L);// удалить анимацию (04EF: REMOVE_ANIMATION).
int set_ped_stay_when_attacked(lua_State* L);// пед держит позицию при атаке (0350).
int worldcoord(lua_State* L); // Перевод в мировые координаты.
int getpedcoordinates_on_x(lua_State* L); // Получить мировую координату по x для педа.

int getpedcoordinates_on_y(lua_State* L); // Получить мировую координату по y для педа.
int kill_ped_on_foot(lua_State* L);// убить педа пешком.
int kill_char_any_means(lua_State* L);// убить педа любыми средствами.

int create_sphere(lua_State* L); //создать сферу.
int remove_sphere(lua_State* L); // удалить сферу.
int remove_ped(lua_State* L); // удалить педа.
int remove_car(lua_State* L); // удалить авто.

int car_in_water(lua_State* L); // проверка авто в воде?.
int set_wanted(lua_State* L); // уcтановить уровень розыска.
int clear_wanted(lua_State* L);// убрать уровень розыска.
int kill_ped(lua_State* L); // убить педа.

int setpedcoordes(lua_State* L); // установить координаты для педа.
int ped_in_point_in_radius(lua_State* L); // проверить находится пед в координатах с радиусом.
int cardrive(lua_State* L); // авто едет в точку.

int setcarspeed(lua_State* L); // установить скорость авто.
int getflagmission(lua_State* L); // проверка флага миссии.
int setflagmission(lua_State* L); // уcтановить флага миссии.
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
int getcarcoordinates_on_x(lua_State* L);// Получить мировую координату по x для авто.
int getcarcoordinates_on_y(lua_State* L); // Получить мировую координату по y для авто.
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

int move_rotate(lua_State* L); // вращать объект.
int getobjangle(lua_State* L); // получить угол объекта.
int findcar(lua_State* L); // Найти авто.
int setcartask(lua_State* L);// установить задачу авто.

int setcarcoordes(lua_State* L);// установить координаты авто.
int is_car_stuck(lua_State* L);//03CE: car 12@ stuck если машина застряла.
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
int ped_frozen(lua_State* L); // заморозить игpока.
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

int flash_hud(lua_State* L); // Мигание элементов HUD.
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
int get_current_name_luascript(lua_State* L); // получить имя текущего lua файла.
int star_mission_marker(lua_State* L); // создать маркер для миссии.
int getobjcoordinates_on_x(lua_State* L); // Получить мировую координату по x для объекта.

int getobjcoordinates_on_y(lua_State* L); // Получить мировую координату по y для объекта.
int set_widescreen(lua_State* L); // вкл/выкл широкий экран.
int follow_the_leader(lua_State* L); //01DE / 01DF	следовать за лидером.
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
int ped_shutdown(lua_State* L);
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
int heli_set_orientation(lua_State* L); // транспорт/краны/зоны
int heli_clear_orientation(lua_State* L); // транспорт/краны/зоны
int heli_set_stabiliser(lua_State* L); // транспорт/краны/зоны
int heli_make_crashing_down(lua_State* L); // транспорт/краны/зоны
int heli_is_player_in_any_heli(lua_State* L); // транспорт/краны/зоны
int heli_is_char_in_any_heli(lua_State* L); // транспорт/краны/зоны
int heli_switch(lua_State* L); // транспорт/краны/зоны
int catalina_heli_start(lua_State* L); // транспорт/краны/зоны
int catalina_heli_take_off(lua_State* L); // транспорт/краны/зоны
int catalina_heli_remove(lua_State* L); // транспорт/краны/зоны
int catalina_heli_grab(lua_State* L); // транспорт/краны/зоны
int catalina_heli_fly_away(lua_State* L); // транспорт/краны/зоны
int catalina_heli_has_shot_down(lua_State* L); // транспорт/краны/зоны
int plane_goto_coords(lua_State* L); // транспорт/краны/зоны
int plane_is_player_in_any_plane(lua_State* L); // транспорт/краны/зоны
int plane_is_char_in_any_plane(lua_State* L); // транспорт/краны/зоны
int find_drug_plane_coords(lua_State* L); // транспорт/краны/зоны
int find_drop_off_plane_coords(lua_State* L); // транспорт/краны/зоны
int has_drug_plane_shot_down(lua_State* L); // транспорт/краны/зоны
int has_drop_off_plane_shot_down(lua_State* L); // транспорт/краны/зоны
int boat_goto_coords(lua_State* L); // транспорт/краны/зоны
int boat_set_cruise_speed(lua_State* L); // транспорт/краны/зоны
int boat_anchor(lua_State* L); // транспорт/краны/зоны
int boat_start_foam_animation(lua_State* L); // транспорт/краны/зоны
int boat_update_foam_animation(lua_State* L); // транспорт/краны/зоны
int boat_is_player_in_any_boat(lua_State* L); // транспорт/краны/зоны
int boat_is_char_in_any_boat(lua_State* L); // транспорт/краны/зоны
int is_vehicle_boat(lua_State* L); // транспорт/краны/зоны
int train_catch_char_obj(lua_State* L); // транспорт/краны/зоны
int train_catch_coll_obj(lua_State* L); // транспорт/краны/зоны
int rc_give_remote_controlled_car_to_player(lua_State* L); // транспорт/краны/зоны
int rc_give_remote_controlled_model_to_player(lua_State* L); // транспорт/краны/зоны
int rc_get_remote_controlled_car(lua_State* L); // транспорт/краны/зоны
int rc_buggy_blow_up(lua_State* L); // транспорт/краны/зоны
int rc_buggy_remove(lua_State* L); // транспорт/краны/зоны
int rc_detonate_enable(lua_State* L); // транспорт/краны/зоны
int rc_detonate_on_contact_enable(lua_State* L); // транспорт/краны/зоны
int player_is_in_remote_mode(lua_State* L); // транспорт/краны/зоны
int set_fade_jumpcut_after_rc_explosion(lua_State* L); // транспорт/краны/зоны
int bike_is_player_on_any_bike(lua_State* L); // транспорт/краны/зоны
int bike_is_char_on_any_bike(lua_State* L); // транспорт/краны/зоны
int is_char_in_flying_vehicle(lua_State* L); // транспорт/краны/зоны
int is_player_in_flying_vehicle(lua_State* L); // транспорт/краны/зоны
int crane_activate(lua_State* L); // транспорт/краны/зоны
int crane_is_carrying_car(lua_State* L); // транспорт/краны/зоны
int crane_is_targetting_car(lua_State* L); // транспорт/краны/зоны
int escalators_count(lua_State* L); // транспорт/краны/зоны
int zone_find_index(lua_State* L); // транспорт/краны/зоны
int zone_get_level_from_position(lua_State* L); // транспорт/краны/зоны
// ================= ЭТАП 12: Остальной транспорт GTA VC =================

// КРАНЫ
int crusher_crane_activate(lua_State* L);
int military_crane_activate(lua_State* L);
int crane_deactivate(lua_State* L);
int crane_military_collected_all(lua_State* L);
int crane_is_lifting_car(lua_State* L);

// КАТЕРА
int boat_stop(lua_State* L);

// ВЕРТОЛЕТЫ
int heli_goto_coords(lua_State* L);
int heli_create_dust_effect(lua_State* L);

// УПРАВЛЕНИЕ АВТО
int car_goto_accurate(lua_State* L);
int car_set_idle(lua_State* L);
int car_wander_randomly(lua_State* L);
int car_face_coord(lua_State* L);
int freeze_car_position(lua_State* L);
int freeze_car_no_collide(lua_State* L);
int set_car_status(lua_State* L);
int set_car_mission(lua_State* L);
int car_block_car(lua_State* L);
int car_ram_car(lua_State* L);
int car_change_lane(lua_State* L);
int car_drive_straight(lua_State* L);
int set_car_forward_speed(lua_State* L);
int car_wait(lua_State* L);
int car_route_seed(lua_State* L);
int car_straight_dist(lua_State* L);
int car_convoy(lua_State* L);
int car_make_stronger(lua_State* L);
int car_to_player(lua_State* L);
int car_fade_in(lua_State* L);
int car_on_trailer(lua_State* L);

// СВОЙСТВА АВТО
int set_car_visible(lua_State* L);
int set_car_watertight(lua_State* L);
int set_car_big_wheels(lua_State* L);
int set_car_bproof(lua_State* L);
int set_car_fproof(lua_State* L);
int set_car_rproof(lua_State* L);
int set_car_no_collide(lua_State* L);
int set_car_burst_tires(lua_State* L);
int set_car_dmg_player_only(lua_State* L);
int car_load_collision(lua_State* L);
int car_upside_safe(lua_State* L);
int car_change_color(lua_State* L);
int car_set_comp(lua_State* L);

// ЗДОРОВЬЕ И УГОЛ
int set_car_health(lua_State* L);
int get_car_health(lua_State* L);
int get_car_heading(lua_State* L);

// ВЕКТОРА
int get_car_forward_x(lua_State* L);
int get_car_forward_y(lua_State* L);

// ПРОВЕРКИ АВТО (boolean)
int is_car_in_air(lua_State* L);
int is_car_in_air_proper(lua_State* L);
int car_health_greater(lua_State* L);
int is_car_crushed(lua_State* L);
int is_car_remap(lua_State* L);
int is_car_stuck_roof(lua_State* L);
int car_is_tire_burst(lua_State* L);
int is_car_visibly_damaged(lua_State* L);
int car_waiting_collision(lua_State* L);
int car_is_door_closed(lua_State* L);
int is_car_on_trailer(lua_State* L);
int car_damaged_by_car(lua_State* L);
int car_damaged_by_ped(lua_State* L);
int car_just_sunk(lua_State* L);
int car_drowning(lua_State* L);
int car_has_weapon(lua_State* L);
int car_any_bomb(lua_State* L);
int car_has_bomb(lua_State* L);
int is_car_in_water(lua_State* L);
int fire_button_pressed(lua_State* L);
int is_car_alive(lua_State* L);
int does_vehicle_exist(lua_State* L);
int is_car_in_garage(lua_State* L);
int was_ever_police(lua_State* L);

// ОЧИСТКА
int clear_car_dmg(lua_State* L);
int clear_car_wep_dmg(lua_State* L);
int remove_stuck_check(lua_State* L);
int remove_upside_check(lua_State* L);
int add_upside_check(lua_State* L);
int unlock_doors_area(lua_State* L);

// ДВЕРИ/БАГАЖНИК
int close_all_doors(lua_State* L);
int close_car_boot(lua_State* L);
int pop_car_boot(lua_State* L);
int pop_car_boot_physics(lua_State* L);
int lock_doors(lua_State* L);
int change_car_lock(lua_State* L);

// ПЕД/ВОДИТЕЛЬ
int get_car_driver_ref(lua_State* L);
int get_char_in_seat(lua_State* L);
int order_driver_out(lua_State* L);
int set_ped_max_drive_speed(lua_State* L);
int set_ped_drive_aggression(lua_State* L);
int shuffle_to_driver(lua_State* L);
int stay_when_jacked(lua_State* L);
int is_ped_in_police_car(lua_State* L);
int is_ped_touching_vehicle(lua_State* L);
int is_ped_leaving_to_die(lua_State* L);
int is_ped_drowning(lua_State* L);
int is_ped_in_water(lua_State* L);
int is_ped_in_zone(lua_State* L);
int is_player_in_zone(lua_State* L);

// ТЕЛЕПОРТАЦИЯ
int warp_ped_from_car(lua_State* L);
int warp_ped_passenger(lua_State* L);
int warp_player_from_car(lua_State* L);

// ОРУЖИЕ ТРАНСПОРТА
int set_car_ammo(lua_State* L);
int switch_car_weapons(lua_State* L);
int set_car_weapon(lua_State* L);
int fire_hunter_gun(lua_State* L);
int set_tank_detonate(lua_State* L);
int ensure_drive_by(lua_State* L);

// БОМБЫ/СИГНАЛИЗАЦИЯ
int arm_car_bomb(lua_State* L);
int disarm_car_bomb(lua_State* L);
int give_car_alarm(lua_State* L);

// ДОПОЛНИТЕЛЬНЫЕ
int switch_car_siren(lua_State* L);
int set_driving_style_op(lua_State* L);
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
int get_projectile_pos(lua_State* L); // позиция снаряда x,y,z
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
int get_car_velocity(lua_State* L); // скорость авто m_vecMoveSpeed (x,y,z)
int set_car_velocity(lua_State* L); // уст скорость авто m_vecMoveSpeed (x,y,z)
int remove_weapon_from_char(lua_State* L); // удалить оружие у педа (0x555)
int get_forced_weather_type(lua_State* L); // принудительный тип погоды
int is_lightning_burst(lua_State* L); // вспышка молнии (CWeather::LightningBurst)
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
int get_heli_searchlight_x(lua_State* L); // координата X прожектора вертолёта
int set_heli_searchlight_x(lua_State* L); // уст. координата X прожектора вертолёта
int get_heli_searchlight_y(lua_State* L); // координата Y прожектора вертолёта
int set_heli_searchlight_y(lua_State* L); // уст. координата Y прожектора вертолёта
int get_heli_searchlight_intensity(lua_State* L); // яркость прожектора вертолёта (0..1)
int set_heli_searchlight_intensity(lua_State* L); // уст. яркость прожектора вертолёта (0..1)
int get_heli_rotor_angle(lua_State* L); // угол вращения винта вертолёта
int set_heli_rotor_angle(lua_State* L); // уст. угол вращения винта вертолёта
int heli_send_down_swat(lua_State* L); // спецназ спускается с вертолёта на верёвке
int heli_get_num_random(lua_State* L); // число случайных вертолётов
int heli_set_num_random(lua_State* L); // уст. число случайных вертолётов
int heli_is_catalina_on(lua_State* L); // вертолёт Катарины активен
int heli_set_catalina_on(lua_State* L); // вкл/выкл вертолёт Катарины
int get_ped_velocity(lua_State* L); // скорость педа m_vecMoveSpeed (x,y,z)
int set_ped_velocity(lua_State* L); // уст. скорость педа m_vecMoveSpeed (x,y,z)
int get_object_velocity(lua_State* L); // скорость объекта m_vecMoveSpeed (x,y,z)
int set_object_velocity(lua_State* L); // уст. скорость объекта m_vecMoveSpeed (x,y,z)
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
int place_3d_marker(lua_State* L); // 3D-маркер (стрелка/цилиндр)
int place_3d_marker_set(lua_State* L); // набор 3D-маркеров
int get_num_active_3d_markers(lua_State* L); // число активных 3D-маркеров
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
int add_BloodRingKills(lua_State* L); // убийства в Bloodring
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
int get_hud_m_WantedTimer(lua_State* L); // таймер розыска на HUD
int set_hud_m_WantedTimer(lua_State* L); // таймер розыска на HUD
int get_hud_m_WeaponFadeTimer(lua_State* L); // таймер затухания оружия на HUD
int set_hud_m_WeaponFadeTimer(lua_State* L); // таймер затухания оружия на HUD
int get_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int set_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int get_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int set_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int get_hud_wanted_state(lua_State* L); // состояние розыска на HUD
int get_hud_help_message_state(lua_State* L); // состояние подсказки на HUD
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
int get_boat_m_fForcedZRotation(lua_State* L); // принудительный поворот лодки по Z
int set_boat_m_fForcedZRotation(lua_State* L); // уст. принудительный поворот лодки по Z
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
int get_streaming_old_sector_x(lua_State* L); // старый сектор X
int get_streaming_old_sector_y(lua_State* L); // старый сектор Y
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
int get_car_m_nPrimaryColor(lua_State* L); // m_nPrimaryColor (CVehicle)
int set_car_m_nPrimaryColor(lua_State* L); // уст. m_nPrimaryColor (CVehicle)
int get_car_m_nSecondaryColor(lua_State* L); // m_nSecondaryColor (CVehicle)
int set_car_m_nSecondaryColor(lua_State* L); // уст. m_nSecondaryColor (CVehicle)
int get_car_m_wWantedStarsOnEnter(lua_State* L); // m_wWantedStarsOnEnter (CVehicle)
int set_car_m_wWantedStarsOnEnter(lua_State* L); // уст. m_wWantedStarsOnEnter (CVehicle)
int get_car_m_wMissionValue(lua_State* L); // m_wMissionValue (CVehicle)
int set_car_m_wMissionValue(lua_State* L); // уст. m_wMissionValue (CVehicle)
int get_car_m_nNumPassengers(lua_State* L); // m_nNumPassengers (CVehicle)
int set_car_m_nNumPassengers(lua_State* L); // уст. m_nNumPassengers (CVehicle)
int get_car_m_nNumGettingIn(lua_State* L); // m_nNumGettingIn (CVehicle)
int set_car_m_nNumGettingIn(lua_State* L); // уст. m_nNumGettingIn (CVehicle)
int get_car_m_nGettingInFlags(lua_State* L); // m_nGettingInFlags (CVehicle)
int set_car_m_nGettingInFlags(lua_State* L); // уст. m_nGettingInFlags (CVehicle)
int get_car_m_nGettingOutFlags(lua_State* L); // m_nGettingOutFlags (CVehicle)
int set_car_m_nGettingOutFlags(lua_State* L); // уст. m_nGettingOutFlags (CVehicle)
int get_car_m_nMaxPassengers(lua_State* L); // m_nMaxPassengers (CVehicle)
int set_car_m_nMaxPassengers(lua_State* L); // уст. m_nMaxPassengers (CVehicle)
int get_car_m_fSteerAngle(lua_State* L); // m_fSteerAngle (CVehicle)
int set_car_m_fSteerAngle(lua_State* L); // уст. m_fSteerAngle (CVehicle)
int get_car_m_fGasPedal(lua_State* L); // m_fGasPedal (CVehicle)
int set_car_m_fGasPedal(lua_State* L); // уст. m_fGasPedal (CVehicle)
int get_car_m_fBreakPedal(lua_State* L); // m_fBreakPedal (CVehicle)
int set_car_m_fBreakPedal(lua_State* L); // уст. m_fBreakPedal (CVehicle)
int get_car_m_nCreatedBy(lua_State* L); // m_nCreatedBy (CVehicle)
int set_car_m_nCreatedBy(lua_State* L); // уст. m_nCreatedBy (CVehicle)
int get_car_m_nAmmoInClip(lua_State* L); // m_nAmmoInClip (CVehicle)
int set_car_m_nAmmoInClip(lua_State* L); // уст. m_nAmmoInClip (CVehicle)
int get_car_m_fHealth(lua_State* L); // m_fHealth (CVehicle)
int set_car_m_fHealth(lua_State* L); // уст. m_fHealth (CVehicle)
int get_car_m_nCurrentGear(lua_State* L); // m_nCurrentGear (CVehicle)
int set_car_m_nCurrentGear(lua_State* L); // уст. m_nCurrentGear (CVehicle)
int get_car_m_nTimeTillWeNeedThisCar(lua_State* L); // m_nTimeTillWeNeedThisCar (CVehicle)
int set_car_m_nTimeTillWeNeedThisCar(lua_State* L); // уст. m_nTimeTillWeNeedThisCar (CVehicle)
int get_car_m_nTimeOfDeath(lua_State* L); // m_nTimeOfDeath (CVehicle)
int set_car_m_nTimeOfDeath(lua_State* L); // уст. m_nTimeOfDeath (CVehicle)
int get_car_m_wBombTimer(lua_State* L); // m_wBombTimer (CVehicle)
int set_car_m_wBombTimer(lua_State* L); // уст. m_wBombTimer (CVehicle)
int get_car_m_nLastWeaponDamage(lua_State* L); // m_nLastWeaponDamage (CVehicle)
int set_car_m_nLastWeaponDamage(lua_State* L); // уст. m_nLastWeaponDamage (CVehicle)
int get_car_m_nRadioStation(lua_State* L); // m_nRadioStation (CVehicle)
int set_car_m_nRadioStation(lua_State* L); // уст. m_nRadioStation (CVehicle)
int get_car_m_bHornEnabled(lua_State* L); // m_bHornEnabled (CVehicle)
int set_car_m_bHornEnabled(lua_State* L); // уст. m_bHornEnabled (CVehicle)
int get_car_m_nSirenOrAlarm(lua_State* L); // m_nSirenOrAlarm (CVehicle)
int set_car_m_nSirenOrAlarm(lua_State* L); // уст. m_nSirenOrAlarm (CVehicle)
int get_car_m_nSirenExtra(lua_State* L); // m_nSirenExtra (CVehicle)
int set_car_m_nSirenExtra(lua_State* L); // уст. m_nSirenExtra (CVehicle)
int get_car_m_fSteerRatio(lua_State* L); // m_fSteerRatio (CVehicle)
int set_car_m_fSteerRatio(lua_State* L); // уст. m_fSteerRatio (CVehicle)
int get_car_m_nVehicleClass(lua_State* L); // m_nVehicleClass (CVehicle)
int set_car_m_nVehicleClass(lua_State* L); // уст. m_nVehicleClass (CVehicle)
int get_ped_m_fCollisionSpeed(lua_State* L); // m_fCollisionSpeed (CPed)
int set_ped_m_fCollisionSpeed(lua_State* L); // уст. m_fCollisionSpeed (CPed)
int get_ped_m_nGangFlags(lua_State* L); // m_nGangFlags (CPed)
int set_ped_m_nGangFlags(lua_State* L); // уст. m_nGangFlags (CPed)
int get_ped_m_nPedStatus(lua_State* L); // m_nPedStatus (CPed)
int set_ped_m_nPedStatus(lua_State* L); // уст. m_nPedStatus (CPed)
int get_ped_m_vecObjective(lua_State* L); // m_vecObjective (CPed) (x,y,z)
int set_ped_m_vecObjective(lua_State* L); // уст. m_vecObjective (CPed) (x,y,z)
int get_ped_m_fObjectiveAngle(lua_State* L); // m_fObjectiveAngle (CPed)
int set_ped_m_fObjectiveAngle(lua_State* L); // уст. m_fObjectiveAngle (CPed)
int get_ped_m_nPedFormation(lua_State* L); // m_nPedFormation (CPed)
int set_ped_m_nPedFormation(lua_State* L); // уст. m_nPedFormation (CPed)
int get_ped_m_nFearFlags(lua_State* L); // m_nFearFlags (CPed)
int set_ped_m_nFearFlags(lua_State* L); // уст. m_nFearFlags (CPed)
int get_ped_m_nEventType(lua_State* L); // m_nEventType (CPed)
int set_ped_m_nEventType(lua_State* L); // уст. m_nEventType (CPed)
int get_ped_m_fAngleToEvent(lua_State* L); // m_fAngleToEvent (CPed)
int set_ped_m_fAngleToEvent(lua_State* L); // уст. m_fAngleToEvent (CPed)
int get_ped_m_nAnimGroupId(lua_State* L); // m_nAnimGroupId (CPed)
int set_ped_m_nAnimGroupId(lua_State* L); // уст. m_nAnimGroupId (CPed)
int get_ped_m_vecOffsetSeek(lua_State* L); // m_vecOffsetSeek (CPed) (x,y,z)
int set_ped_m_vecOffsetSeek(lua_State* L); // уст. m_vecOffsetSeek (CPed) (x,y,z)
int get_ped_m_nActionTimer(lua_State* L); // m_nActionTimer (CPed)
int set_ped_m_nActionTimer(lua_State* L); // уст. m_nActionTimer (CPed)
int get_ped_m_nWaitState(lua_State* L); // m_nWaitState (CPed)
int set_ped_m_nWaitState(lua_State* L); // уст. m_nWaitState (CPed)
int get_ped_m_nWaitTimer(lua_State* L); // m_nWaitTimer (CPed)
int set_ped_m_nWaitTimer(lua_State* L); // уст. m_nWaitTimer (CPed)
int get_ped_m_wPathNodes(lua_State* L); // m_wPathNodes (CPed)
int set_ped_m_wPathNodes(lua_State* L); // уст. m_wPathNodes (CPed)
int get_ped_m_wCurPathNode(lua_State* L); // m_wCurPathNode (CPed)
int set_ped_m_wCurPathNode(lua_State* L); // уст. m_wCurPathNode (CPed)
int get_ped_m_nPathNodeTimer(lua_State* L); // m_nPathNodeTimer (CPed)
int set_ped_m_nPathNodeTimer(lua_State* L); // уст. m_nPathNodeTimer (CPed)
int get_ped_m_vecPathNextNode(lua_State* L); // m_vecPathNextNode (CPed) (x,y,z)
int set_ped_m_vecPathNextNode(lua_State* L); // уст. m_vecPathNextNode (CPed) (x,y,z)
int get_ped_m_fPathNextNodeDir(lua_State* L); // m_fPathNextNodeDir (CPed)
int set_ped_m_fPathNextNodeDir(lua_State* L); // уст. m_fPathNextNodeDir (CPed)
int get_ped_m_nPathNodeType(lua_State* L); // m_nPathNodeType (CPed)
int set_ped_m_nPathNodeType(lua_State* L); // уст. m_nPathNodeType (CPed)
int get_ped_m_fHealth(lua_State* L); // m_fHealth (CPed)
int set_ped_m_fHealth(lua_State* L); // уст. m_fHealth (CPed)
int get_ped_m_fArmour(lua_State* L); // m_fArmour (CPed)
int set_ped_m_fArmour(lua_State* L); // уст. m_fArmour (CPed)
int get_ped_m_nShadowUpdateTimer(lua_State* L); // m_nShadowUpdateTimer (CPed)
int set_ped_m_nShadowUpdateTimer(lua_State* L); // уст. m_nShadowUpdateTimer (CPed)
int get_ped_wRouteLastPoint(lua_State* L); // wRouteLastPoint (CPed)
int set_ped_wRouteLastPoint(lua_State* L); // уст. wRouteLastPoint (CPed)
int get_ped_wRoutePoints(lua_State* L); // wRoutePoints (CPed)
int set_ped_wRoutePoints(lua_State* L); // уст. wRoutePoints (CPed)
int get_ped_wRoutePos(lua_State* L); // wRoutePos (CPed)
int set_ped_wRoutePos(lua_State* L); // уст. wRoutePos (CPed)
int get_ped_wRouteType(lua_State* L); // wRouteType (CPed)
int set_ped_wRouteType(lua_State* L); // уст. wRouteType (CPed)
int get_ped_wRouteCurDir(lua_State* L); // wRouteCurDir (CPed)
int set_ped_wRouteCurDir(lua_State* L); // уст. wRouteCurDir (CPed)
int get_ped_m_fHeadingCurrent(lua_State* L); // m_fHeadingCurrent (CPed)
int set_ped_m_fHeadingCurrent(lua_State* L); // уст. m_fHeadingCurrent (CPed)
int get_ped_m_fHeadingGoal(lua_State* L); // m_fHeadingGoal (CPed)
int set_ped_m_fHeadingGoal(lua_State* L); // уст. m_fHeadingGoal (CPed)
int get_ped_m_fHeadingChangeRate(lua_State* L); // m_fHeadingChangeRate (CPed)
int set_ped_m_fHeadingChangeRate(lua_State* L); // уст. m_fHeadingChangeRate (CPed)
int get_ped_m_nEnterType(lua_State* L); // m_nEnterType (CPed)
int set_ped_m_nEnterType(lua_State* L); // уст. m_nEnterType (CPed)
int get_ped_m_nWalkAroundType(lua_State* L); // m_nWalkAroundType (CPed)
int set_ped_m_nWalkAroundType(lua_State* L); // уст. m_nWalkAroundType (CPed)
int get_ped_m_vecOffsetFromPhysSurface(lua_State* L); // m_vecOffsetFromPhysSurface (CPed) (x,y,z)
int set_ped_m_vecOffsetFromPhysSurface(lua_State* L); // уст. m_vecOffsetFromPhysSurface (CPed) (x,y,z)
int get_ped_vecSeekVehicle(lua_State* L); // vecSeekVehicle (CPed) (x,y,z)
int set_ped_vecSeekVehicle(lua_State* L); // уст. vecSeekVehicle (CPed) (x,y,z)
int get_ped_m_bInVehicle(lua_State* L); // m_bInVehicle (CPed)
int set_ped_m_bInVehicle(lua_State* L); // уст. m_bInVehicle (CPed)
int get_ped_m_fSeatPrecisionX(lua_State* L); // m_fSeatPrecisionX (CPed)
int set_ped_m_fSeatPrecisionX(lua_State* L); // уст. m_fSeatPrecisionX (CPed)
int get_ped_m_fSeatPrecisionY(lua_State* L); // m_fSeatPrecisionY (CPed)
int set_ped_m_fSeatPrecisionY(lua_State* L); // уст. m_fSeatPrecisionY (CPed)
int get_ped_m_nSeatType(lua_State* L); // m_nSeatType (CPed)
int set_ped_m_nSeatType(lua_State* L); // уст. m_nSeatType (CPed)
int get_ped_m_bHasPhone(lua_State* L); // m_bHasPhone (CPed)
int set_ped_m_bHasPhone(lua_State* L); // уст. m_bHasPhone (CPed)
int get_ped_m_wPhoneId(lua_State* L); // m_wPhoneId (CPed)
int set_ped_m_wPhoneId(lua_State* L); // уст. m_wPhoneId (CPed)
int get_ped_m_nLookingForPhone(lua_State* L); // m_nLookingForPhone (CPed)
int set_ped_m_nLookingForPhone(lua_State* L); // уст. m_nLookingForPhone (CPed)
int get_ped_m_nPhoneTalkTimer(lua_State* L); // m_nPhoneTalkTimer (CPed)
int set_ped_m_nPhoneTalkTimer(lua_State* L); // уст. m_nPhoneTalkTimer (CPed)
int get_ped_m_fFleeFromPosX(lua_State* L); // m_fFleeFromPosX (CPed)
int set_ped_m_fFleeFromPosX(lua_State* L); // уст. m_fFleeFromPosX (CPed)
int get_ped_m_fFleeFromPosY(lua_State* L); // m_fFleeFromPosY (CPed)
int set_ped_m_fFleeFromPosY(lua_State* L); // уст. m_fFleeFromPosY (CPed)
int get_ped_m_nFleeTimer(lua_State* L); // m_nFleeTimer (CPed)
int set_ped_m_nFleeTimer(lua_State* L); // уст. m_nFleeTimer (CPed)
int get_ped_m_nLastThreatTimer(lua_State* L); // m_nLastThreatTimer (CPed)
int set_ped_m_nLastThreatTimer(lua_State* L); // уст. m_nLastThreatTimer (CPed)
int get_ped_m_nStateUnused(lua_State* L); // m_nStateUnused (CPed)
int set_ped_m_nStateUnused(lua_State* L); // уст. m_nStateUnused (CPed)
int get_ped_m_nTimerUnused(lua_State* L); // m_nTimerUnused (CPed)
int set_ped_m_nTimerUnused(lua_State* L); // уст. m_nTimerUnused (CPed)
int get_ped_m_nAtchStoredWep(lua_State* L); // m_nAtchStoredWep (CPed)
int set_ped_m_nAtchStoredWep(lua_State* L); // уст. m_nAtchStoredWep (CPed)
int get_ped_m_nStoredGiveWep(lua_State* L); // m_nStoredGiveWep (CPed)
int set_ped_m_nStoredGiveWep(lua_State* L); // уст. m_nStoredGiveWep (CPed)
int get_ped_m_nStoredGiveAmmo(lua_State* L); // m_nStoredGiveAmmo (CPed)
int set_ped_m_nStoredGiveAmmo(lua_State* L); // уст. m_nStoredGiveAmmo (CPed)
int get_ped_m_nCurrentWeapon(lua_State* L); // m_nCurrentWeapon (CPed)
int set_ped_m_nCurrentWeapon(lua_State* L); // уст. m_nCurrentWeapon (CPed)
int get_ped_m_nWepSkills(lua_State* L); // m_nWepSkills (CPed)
int set_ped_m_nWepSkills(lua_State* L); // уст. m_nWepSkills (CPed)
int get_ped_m_nWeaponAccuracy(lua_State* L); // m_nWeaponAccuracy (CPed)
int set_ped_m_nWeaponAccuracy(lua_State* L); // уст. m_nWeaponAccuracy (CPed)
int get_ped_m_nBodyPart(lua_State* L); // m_nBodyPart (CPed)
int set_ped_m_nBodyPart(lua_State* L); // уст. m_nBodyPart (CPed)
int get_ped_m_vecHitLastPos(lua_State* L); // m_vecHitLastPos (CPed) (x,y,z)
int set_ped_m_vecHitLastPos(lua_State* L); // уст. m_vecHitLastPos (CPed) (x,y,z)
int get_ped_m_nHitCounter(lua_State* L); // m_nHitCounter (CPed)
int set_ped_m_nHitCounter(lua_State* L); // уст. m_nHitCounter (CPed)
int get_ped_m_nLastHitState(lua_State* L); // m_nLastHitState (CPed)
int set_ped_m_nLastHitState(lua_State* L); // уст. m_nLastHitState (CPed)
int get_ped_m_nFightFlags1(lua_State* L); // m_nFightFlags1 (CPed)
int set_ped_m_nFightFlags1(lua_State* L); // уст. m_nFightFlags1 (CPed)
int get_ped_m_nFightFlags2(lua_State* L); // m_nFightFlags2 (CPed)
int set_ped_m_nFightFlags2(lua_State* L); // уст. m_nFightFlags2 (CPed)
int get_ped_m_nFightFlags3(lua_State* L); // m_nFightFlags3 (CPed)
int set_ped_m_nFightFlags3(lua_State* L); // уст. m_nFightFlags3 (CPed)
int get_ped_m_nBleedCounter(lua_State* L); // m_nBleedCounter (CPed)
int set_ped_m_nBleedCounter(lua_State* L); // уст. m_nBleedCounter (CPed)
int get_ped_m_fLookDirection(lua_State* L); // m_fLookDirection (CPed)
int set_ped_m_fLookDirection(lua_State* L); // уст. m_fLookDirection (CPed)
int get_ped_m_nWepModelID(lua_State* L); // m_nWepModelID (CPed)
int set_ped_m_nWepModelID(lua_State* L); // уст. m_nWepModelID (CPed)
int get_ped_m_nLeaveCarTimer(lua_State* L); // m_nLeaveCarTimer (CPed)
int set_ped_m_nLeaveCarTimer(lua_State* L); // уст. m_nLeaveCarTimer (CPed)
int get_ped_m_nGetUpTimer(lua_State* L); // m_nGetUpTimer (CPed)
int set_ped_m_nGetUpTimer(lua_State* L); // уст. m_nGetUpTimer (CPed)
int get_ped_m_nLookTimer(lua_State* L); // m_nLookTimer (CPed)
int set_ped_m_nLookTimer(lua_State* L); // уст. m_nLookTimer (CPed)
int get_ped_m_nStandardTimer(lua_State* L); // m_nStandardTimer (CPed)
int set_ped_m_nStandardTimer(lua_State* L); // уст. m_nStandardTimer (CPed)
int get_ped_m_nAttackTimer(lua_State* L); // m_nAttackTimer (CPed)
int set_ped_m_nAttackTimer(lua_State* L); // уст. m_nAttackTimer (CPed)
int get_ped_m_nLastHitTime(lua_State* L); // m_nLastHitTime (CPed)
int set_ped_m_nLastHitTime(lua_State* L); // уст. m_nLastHitTime (CPed)
int get_ped_m_nHitRecoverTimer(lua_State* L); // m_nHitRecoverTimer (CPed)
int set_ped_m_nHitRecoverTimer(lua_State* L); // уст. m_nHitRecoverTimer (CPed)
int get_ped_m_nObjectiveTimer(lua_State* L); // m_nObjectiveTimer (CPed)
int set_ped_m_nObjectiveTimer(lua_State* L); // уст. m_nObjectiveTimer (CPed)
int get_ped_m_nDuckTimer(lua_State* L); // m_nDuckTimer (CPed)
int set_ped_m_nDuckTimer(lua_State* L); // уст. m_nDuckTimer (CPed)
int get_ped_m_nDuckAndCoverTimer(lua_State* L); // m_nDuckAndCoverTimer (CPed)
int set_ped_m_nDuckAndCoverTimer(lua_State* L); // уст. m_nDuckAndCoverTimer (CPed)
int get_ped_m_nBloodyTimer(lua_State* L); // m_nBloodyTimer (CPed)
int set_ped_m_nBloodyTimer(lua_State* L); // уст. m_nBloodyTimer (CPed)
int get_ped_m_nShotTime(lua_State* L); // m_nShotTime (CPed)
int set_ped_m_nShotTime(lua_State* L); // уст. m_nShotTime (CPed)
int get_ped_m_nShotTimeAdd(lua_State* L); // m_nShotTimeAdd (CPed)
int set_ped_m_nShotTimeAdd(lua_State* L); // уст. m_nShotTimeAdd (CPed)
int get_ped_m_nPanicCounter(lua_State* L); // m_nPanicCounter (CPed)
int set_ped_m_nPanicCounter(lua_State* L); // уст. m_nPanicCounter (CPed)
int get_ped_m_nDeadBleeding(lua_State* L); // m_nDeadBleeding (CPed)
int set_ped_m_nDeadBleeding(lua_State* L); // уст. m_nDeadBleeding (CPed)
int get_ped_m_nBodyPartBleeding(lua_State* L); // m_nBodyPartBleeding (CPed)
int set_ped_m_nBodyPartBleeding(lua_State* L); // уст. m_nBodyPartBleeding (CPed)
int get_ped_m_nNumNearPeds(lua_State* L); // m_nNumNearPeds (CPed)
int set_ped_m_nNumNearPeds(lua_State* L); // уст. m_nNumNearPeds (CPed)
int get_ped_m_nPedMoney(lua_State* L); // m_nPedMoney (CPed)
int set_ped_m_nPedMoney(lua_State* L); // уст. m_nPedMoney (CPed)
int get_ped_m_nLastDamWep(lua_State* L); // m_nLastDamWep (CPed)
int set_ped_m_nLastDamWep(lua_State* L); // уст. m_nLastDamWep (CPed)
int get_ped_m_vecAttachOffset(lua_State* L); // m_vecAttachOffset (CPed) (x,y,z)
int set_ped_m_vecAttachOffset(lua_State* L); // уст. m_vecAttachOffset (CPed) (x,y,z)
int get_ped_m_nAttachType(lua_State* L); // m_nAttachType (CPed)
int set_ped_m_nAttachType(lua_State* L); // уст. m_nAttachType (CPed)
int get_ped_m_fAttachRot(lua_State* L); // m_fAttachRot (CPed)
int set_ped_m_fAttachRot(lua_State* L); // уст. m_fAttachRot (CPed)
int get_ped_m_nAttachWepAmmo(lua_State* L); // m_nAttachWepAmmo (CPed)
int set_ped_m_nAttachWepAmmo(lua_State* L); // уст. m_nAttachWepAmmo (CPed)
int get_ped_m_nThreatFlags(lua_State* L); // m_nThreatFlags (CPed)
int set_ped_m_nThreatFlags(lua_State* L); // уст. m_nThreatFlags (CPed)
int get_ped_m_nThreatCheck(lua_State* L); // m_nThreatCheck (CPed)
int set_ped_m_nThreatCheck(lua_State* L); // уст. m_nThreatCheck (CPed)
int get_ped_m_nLastThreatCheck(lua_State* L); // m_nLastThreatCheck (CPed)
int set_ped_m_nLastThreatCheck(lua_State* L); // уст. m_nLastThreatCheck (CPed)
int get_ped_m_nSayType(lua_State* L); // m_nSayType (CPed)
int set_ped_m_nSayType(lua_State* L); // уст. m_nSayType (CPed)
int get_ped_m_nSayTimer(lua_State* L); // m_nSayTimer (CPed)
int set_ped_m_nSayTimer(lua_State* L); // уст. m_nSayTimer (CPed)
int get_ped_m_nTalkTimerLast(lua_State* L); // m_nTalkTimerLast (CPed)
int set_ped_m_nTalkTimerLast(lua_State* L); // уст. m_nTalkTimerLast (CPed)
int get_ped_m_nTalkTimer(lua_State* L); // m_nTalkTimer (CPed)
int set_ped_m_nTalkTimer(lua_State* L); // уст. m_nTalkTimer (CPed)
int get_ped_m_wTalkTypeLast(lua_State* L); // m_wTalkTypeLast (CPed)
int set_ped_m_wTalkTypeLast(lua_State* L); // уст. m_wTalkTypeLast (CPed)
int get_ped_m_wTalkType(lua_State* L); // m_wTalkType (CPed)
int set_ped_m_wTalkType(lua_State* L); // уст. m_wTalkType (CPed)
int get_ped_m_bCanPedTalk(lua_State* L); // m_bCanPedTalk (CPed)
int set_ped_m_bCanPedTalk(lua_State* L); // уст. m_bCanPedTalk (CPed)
int get_ped_m_nPedLastComment(lua_State* L); // m_nPedLastComment (CPed)
int set_ped_m_nPedLastComment(lua_State* L); // уст. m_nPedLastComment (CPed)
int get_ped_m_vecSeekPosEx(lua_State* L); // m_vecSeekPosEx (CPed) (x,y,z)
int set_ped_m_vecSeekPosEx(lua_State* L); // уст. m_vecSeekPosEx (CPed) (x,y,z)
int get_ped_m_fSeekExAngle(lua_State* L); // m_fSeekExAngle (CPed)
int set_ped_m_fSeekExAngle(lua_State* L); // уст. m_fSeekExAngle (CPed)
int get_obj_m_nObjectType(lua_State* L); // m_nObjectType (CObject)
int set_obj_m_nObjectType(lua_State* L); // уст. m_nObjectType (CObject)
int get_obj_m_nBonusValue(lua_State* L); // m_nBonusValue (CObject)
int set_obj_m_nBonusValue(lua_State* L); // уст. m_nBonusValue (CObject)
int get_obj_m_wCostValue(lua_State* L); // m_wCostValue (CObject)
int set_obj_m_wCostValue(lua_State* L); // уст. m_wCostValue (CObject)
int get_obj_m_fDamageMultiplier(lua_State* L); // m_fDamageMultiplier (CObject)
int set_obj_m_fDamageMultiplier(lua_State* L); // уст. m_fDamageMultiplier (CObject)
int get_obj_m_bCameraAvoids(lua_State* L); // m_bCameraAvoids (CObject)
int set_obj_m_bCameraAvoids(lua_State* L); // уст. m_bCameraAvoids (CObject)
int get_obj_m_wRefModelId(lua_State* L); // m_wRefModelId (CObject)
int set_obj_m_wRefModelId(lua_State* L); // уст. m_wRefModelId (CObject)
int get_car_m_nAudioEntityId(lua_State* L); // m_nAudioEntityId (CPhysical)
int set_car_m_nAudioEntityId(lua_State* L); // уст. m_nAudioEntityId (CPhysical)
int get_car_m_vecMoveSpeed(lua_State* L); // m_vecMoveSpeed (CPhysical) (x,y,z)
int set_car_m_vecMoveSpeed(lua_State* L); // уст. m_vecMoveSpeed (CPhysical) (x,y,z)
int get_car_m_vecTurnSpeed(lua_State* L); // m_vecTurnSpeed (CPhysical) (x,y,z)
int set_car_m_vecTurnSpeed(lua_State* L); // уст. m_vecTurnSpeed (CPhysical) (x,y,z)
int get_car_m_vecFrictionMoveForce(lua_State* L); // m_vecFrictionMoveForce (CPhysical) (x,y,z)
int set_car_m_vecFrictionMoveForce(lua_State* L); // уст. m_vecFrictionMoveForce (CPhysical) (x,y,z)
int get_car_m_vecFrictionTurnForce(lua_State* L); // m_vecFrictionTurnForce (CPhysical) (x,y,z)
int set_car_m_vecFrictionTurnForce(lua_State* L); // уст. m_vecFrictionTurnForce (CPhysical) (x,y,z)
int get_car_m_vecForce(lua_State* L); // m_vecForce (CPhysical) (x,y,z)
int set_car_m_vecForce(lua_State* L); // уст. m_vecForce (CPhysical) (x,y,z)
int get_car_m_vecTorque(lua_State* L); // m_vecTorque (CPhysical) (x,y,z)
int set_car_m_vecTorque(lua_State* L); // уст. m_vecTorque (CPhysical) (x,y,z)
int get_car_m_fMass(lua_State* L); // m_fMass (CPhysical)
int set_car_m_fMass(lua_State* L); // уст. m_fMass (CPhysical)
int get_car_m_fTurnMass(lua_State* L); // m_fTurnMass (CPhysical)
int set_car_m_fTurnMass(lua_State* L); // уст. m_fTurnMass (CPhysical)
int get_car_m_fVelocityFrequency(lua_State* L); // m_fVelocityFrequency (CPhysical)
int set_car_m_fVelocityFrequency(lua_State* L); // уст. m_fVelocityFrequency (CPhysical)
int get_car_m_fAirResistance(lua_State* L); // m_fAirResistance (CPhysical)
int set_car_m_fAirResistance(lua_State* L); // уст. m_fAirResistance (CPhysical)
int get_car_m_fElasticity(lua_State* L); // m_fElasticity (CPhysical)
int set_car_m_fElasticity(lua_State* L); // уст. m_fElasticity (CPhysical)
int get_car_m_fBuoyancyConstant(lua_State* L); // m_fBuoyancyConstant (CPhysical)
int set_car_m_fBuoyancyConstant(lua_State* L); // уст. m_fBuoyancyConstant (CPhysical)
int get_car_m_vecCentreOfMass(lua_State* L); // m_vecCentreOfMass (CPhysical) (x,y,z)
int set_car_m_vecCentreOfMass(lua_State* L); // уст. m_vecCentreOfMass (CPhysical) (x,y,z)
int get_car_m_nNumCollisionRecords(lua_State* L); // m_nNumCollisionRecords (CPhysical)
int set_car_m_nNumCollisionRecords(lua_State* L); // уст. m_nNumCollisionRecords (CPhysical)
int get_car_m_fTotSpeed(lua_State* L); // m_fTotSpeed (CPhysical)
int set_car_m_fTotSpeed(lua_State* L); // уст. m_fTotSpeed (CPhysical)
int get_car_m_fCollisionPower(lua_State* L); // m_fCollisionPower (CPhysical)
int set_car_m_fCollisionPower(lua_State* L); // уст. m_fCollisionPower (CPhysical)
int get_car_m_vecCollisionPower(lua_State* L); // m_vecCollisionPower (CPhysical) (x,y,z)
int set_car_m_vecCollisionPower(lua_State* L); // уст. m_vecCollisionPower (CPhysical) (x,y,z)
int get_car_m_wComponentCol(lua_State* L); // m_wComponentCol (CPhysical)
int set_car_m_wComponentCol(lua_State* L); // уст. m_wComponentCol (CPhysical)
int get_car_m_nMoveFlags(lua_State* L); // m_nMoveFlags (CPhysical)
int set_car_m_nMoveFlags(lua_State* L); // уст. m_nMoveFlags (CPhysical)
int get_car_m_nCollFlags(lua_State* L); // m_nCollFlags (CPhysical)
int set_car_m_nCollFlags(lua_State* L); // уст. m_nCollFlags (CPhysical)
int get_car_m_nLastCollType(lua_State* L); // m_nLastCollType (CPhysical)
int set_car_m_nLastCollType(lua_State* L); // уст. m_nLastCollType (CPhysical)
int get_car_m_nZoneLevel(lua_State* L); // m_nZoneLevel (CPhysical)
int set_car_m_nZoneLevel(lua_State* L); // уст. m_nZoneLevel (CPhysical)
int get_car_fNegSpeed(lua_State* L); // fNegSpeed (CAutomobile)
int set_car_fNegSpeed(lua_State* L); // уст. fNegSpeed (CAutomobile)
int get_car_fAIGripMultiplier(lua_State* L); // fAIGripMultiplier (CAutomobile)
int set_car_fAIGripMultiplier(lua_State* L); // уст. fAIGripMultiplier (CAutomobile)
int get_car_fSpecialWepRotH(lua_State* L); // fSpecialWepRotH (CAutomobile)
int set_car_fSpecialWepRotH(lua_State* L); // уст. fSpecialWepRotH (CAutomobile)
int get_car_fSpecialWepRotV(lua_State* L); // fSpecialWepRotV (CAutomobile)
int set_car_fSpecialWepRotV(lua_State* L); // уст. fSpecialWepRotV (CAutomobile)
int get_car_fSpecialSteering(lua_State* L); // fSpecialSteering (CAutomobile)
int set_car_fSpecialSteering(lua_State* L); // уст. fSpecialSteering (CAutomobile)
int get_car_fSpecialMoveState(lua_State* L); // fSpecialMoveState (CAutomobile)
int set_car_fSpecialMoveState(lua_State* L); // уст. fSpecialMoveState (CAutomobile)
int get_car_fSkidMarkDensity(lua_State* L); // fSkidMarkDensity (CAutomobile)
int set_car_fSkidMarkDensity(lua_State* L); // уст. fSkidMarkDensity (CAutomobile)
int get_car_m_fPropRotation(lua_State* L); // m_fPropRotation (CBoat)
int set_car_m_fPropRotation(lua_State* L); // уст. m_fPropRotation (CBoat)
int get_car_m_fPropSpeed(lua_State* L); // m_fPropSpeed (CBoat)
int set_car_m_fPropSpeed(lua_State* L); // уст. m_fPropSpeed (CBoat)
int get_car_m_fForcedZRotation(lua_State* L); // m_fForcedZRotation (CBoat)
int set_car_m_fForcedZRotation(lua_State* L); // уст. m_fForcedZRotation (CBoat)
int get_car_m_nAttackPlayerTime(lua_State* L); // m_nAttackPlayerTime (CBoat)
int set_car_m_nAttackPlayerTime(lua_State* L); // уст. m_nAttackPlayerTime (CBoat)
int get_car_m_fBurningTimer(lua_State* L); // m_fBurningTimer (CBoat)
int set_car_m_fBurningTimer(lua_State* L); // уст. m_fBurningTimer (CBoat)
int get_car_m_fBoatGasPedal(lua_State* L); // m_fBoatGasPedal (CBoat)
int set_car_m_fBoatGasPedal(lua_State* L); // уст. m_fBoatGasPedal (CBoat)
int get_car_m_fBoatBrakePedal(lua_State* L); // m_fBoatBrakePedal (CBoat)
int set_car_m_fBoatBrakePedal(lua_State* L); // уст. m_fBoatBrakePedal (CBoat)
int get_car_m_fBoatSteeringLeftRight(lua_State* L); // m_fBoatSteeringLeftRight (CBoat)
int set_car_m_fBoatSteeringLeftRight(lua_State* L); // уст. m_fBoatSteeringLeftRight (CBoat)
int get_car_m_nPadNumber(lua_State* L); // m_nPadNumber (CBoat)
int set_car_m_nPadNumber(lua_State* L); // уст. m_nPadNumber (CBoat)
int get_car_m_nNumWaterTrailPoints(lua_State* L); // m_nNumWaterTrailPoints (CBoat)
int set_car_m_nNumWaterTrailPoints(lua_State* L); // уст. m_nNumWaterTrailPoints (CBoat)
int get_car_nBikeAnimGroup(lua_State* L); // nBikeAnimGroup (CBike)
int set_car_nBikeAnimGroup(lua_State* L); // уст. nBikeAnimGroup (CBike)
int get_car_m_nDamageFlags(lua_State* L); // m_nDamageFlags (CBike)
int set_car_m_nDamageFlags(lua_State* L); // уст. m_nDamageFlags (CBike)
int get_car_m_fSearchLightX(lua_State* L); // m_fSearchLightX (CHeli)
int set_car_m_fSearchLightX(lua_State* L); // уст. m_fSearchLightX (CHeli)
int get_car_m_fSearchLightY(lua_State* L); // m_fSearchLightY (CHeli)
int set_car_m_fSearchLightY(lua_State* L); // уст. m_fSearchLightY (CHeli)
int get_car_m_nExplosionTime(lua_State* L); // m_nExplosionTime (CHeli)
int set_car_m_nExplosionTime(lua_State* L); // уст. m_nExplosionTime (CHeli)
int get_car_m_fRotationX(lua_State* L); // m_fRotationX (CHeli)
int set_car_m_fRotationX(lua_State* L); // уст. m_fRotationX (CHeli)
int get_car_m_fHeliForcedZ(lua_State* L); // m_fHeliForcedZ (CHeli)
int set_car_m_fHeliForcedZ(lua_State* L); // уст. m_fHeliForcedZ (CHeli)
int get_car_m_fSearchLightIntensity(lua_State* L); // m_fSearchLightIntensity (CHeli)
int set_car_m_fSearchLightIntensity(lua_State* L); // уст. m_fSearchLightIntensity (CHeli)
int get_car_m_nDamagePoints(lua_State* L); // m_nDamagePoints (CHeli)
int set_car_m_nDamagePoints(lua_State* L); // уст. m_nDamagePoints (CHeli)
int get_car_m_fRotorAngle(lua_State* L); // m_fRotorAngle (CHeli)
int set_car_m_fRotorAngle(lua_State* L); // уст. m_fRotorAngle (CHeli)
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
int register_motion_streak(lua_State* L); // размытие-полоса (id, левая/правая точки, rgb)
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
int set_ped_traffic(lua_State* L); // установить плотноcть трафика педов.
int heli_to_fly(lua_State* L); // вертолет летит на координаты.

int set_ped_immunities(lua_State* L); // уст иммунитеты педу.
int set_car_immunities(lua_State* L); // уст иммунитеты авто.
int setcolourmarker(lua_State* L); // установить цвет маркера.

int set_camera_on_car(lua_State* L); // уст камеру на авто.
int ped_run_to_point(lua_State* L); // пед бежит к точке пешком.
int select_interiour(lua_State* L); // уcтановить интерьер.
int set_ped_stats_to(lua_State* L); // уст поведения педа.

int ped_maintain_position_when_attacked(lua_State* L); // пед удерживает позицию при атаке.
int set_timer_ped_attack(lua_State* L); // установить таймер атаки педа.
int set_cops_ignored(lua_State* L); // установить игнор копов.
int set_camera_near_clip(lua_State* L); // установить обрезку камеры.

int setpedcrouch(lua_State* L); // пед сел.
int is_ped_incar(lua_State* L);  // пед в авто или нет?
int delete_entity(lua_State* L); // удалить сущность сразу.
int clean_leader(lua_State* L); // перестать следовать за лидером.

int show_save_menu(lua_State* L); // показать меню сохранения.
int is_ped_shooting(lua_State* L); // пед стреляет?.
int set_ped_visible(lua_State* L); // уст невидимость педу.
int set_player_mod(lua_State* L); // уст настроения игрока.

int setped_damaged_player(lua_State* L); // педу может нанести ущерб только игрок.
int set_ped_reaction_threat(lua_State* L); // уст реакцию педа на ответную угрозу.
int obj_target(lua_State* L); // в объект можно целиться.
int clean_ped_wait(lua_State* L); // пед больше не ждет.

int set_ped_accuracy(lua_State* L); // уст меткость педу.
int textbox(lua_State* L); // вывести сообщение в textbox.
int remove_textbox(lua_State* L); // удалить сообщение на textbox.
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
int show_text_gtx_table(lua_State* L); // вывести текст из таблицы.
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
int getmodelindex(lua_State* L); // получить id модели.
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
int car_turnforce_on_x(lua_State* L); // Повернуть авто по оси x.
int car_turnforce_on_y(lua_State* L); // Повернуть авто по оси y.
int car_turnforce_on_z(lua_State* L); // Повернуть авто по оси z.

int car_moveforce_on_x(lua_State* L); // Двигать авто по оси x.
int car_moveforce_on_y(lua_State* L); // Двигать авто по оси y.
int car_moveforce_on_z(lua_State* L); // Двигать авто по оси z.
int setcarrotate(lua_State* L);  // уст вращения авто.

int getcar_turn_on_x(lua_State* L); // Получить угол поворота авто по оси x.
int getcar_turn_on_y(lua_State* L); // Получить угол поворота авто по оси y.
int getcar_turn_on_z(lua_State* L); // Получить угол поворота авто по оси z.

int carmovespeed_on_x(lua_State* L); // двигать авто с скоростью по x.
int carmovespeed_on_y(lua_State* L); // двигать авто с скоростью по y.
int carmovespeed_on_z(lua_State* L); // двигать авто с скоростью по z.

int car_rotate_on_x(lua_State* L); // Поворот угла авто по оси x.
int car_rotate_on_y(lua_State* L); // Поворот угла авто по оси y.
int car_rotate_on_z(lua_State* L); // Поворот угла авто по оси z.

int car_setrotate_on_x(lua_State* L); // уст авто по оси x.
int car_setrotate_on_y(lua_State* L); // уст авто по оси y.
int car_setrotate_on_z(lua_State* L); // уст авто по оси z.

int getcar_resistance(lua_State* L); // Получить сопротивления авто трению.
int setcar_resistance(lua_State* L); // Уст силу сопротивления авто трению.
int getcar_turns_on_axes(lua_State* L); // получить углы авто поворота по осям.

int setcar_on_x(lua_State* L); // уст авто по оси x.
int setcar_on_y(lua_State* L); // уст авто по оси y.
int setcar_on_z(lua_State* L); // уст авто по оси z.

int car_rotate_turn_on_x(lua_State* L); // уст угол поворота авто по оси x.
int car_rotate_turn_on_y(lua_State* L); // уст угол поворота авто по оси y.
int car_rotate_turn_on_z(lua_State* L); // уст угол поворота авто по оси z.

int car_turn_on_x_with_speed(lua_State* L); //повeрнуть авто по оси x на угол со скорость.
int car_turn_on_y_with_speed(lua_State* L); //повeрнуть авто по оси y на угол со скорость.
int car_turn_on_z_with_speed(lua_State* L); //повeрнуть авто по оси z на угол со скорость.


int car_turn_on_x_with_delay(lua_State* L); //повeрнуть авто по оси x на угол со задержкой.
int car_turn_on_y_with_delay(lua_State* L); //повeрнуть авто по оси y на угол со задержкой.
int car_turn_on_z_with_delay(lua_State* L); //повeрнуть авто по оси z на угол со задержкой.
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
int anim_set_group_ref(lua_State* L);
int camera_calculate_derived_values(lua_State* L);
int camera_clear_player_weapon_mode(lua_State* L);
int camera_dont_process_obbe_cinema(lua_State* L);
int camera_draw_borders_widescreen(lua_State* L);
int camera_shake(lua_State* L);
int car_activate_bomb(lua_State* L);
int car_activate_bomb_when_entered(lua_State* L);
int car_can_be_deleted(lua_State* L);
int car_can_doors_be_damaged(lua_State* L);
int car_can_ped_enter(lua_State* L);
int car_extinguish_fire(lua_State* L);
int car_get_height_above_road(lua_State* L);
int car_has_roof(lua_State* L);
int car_is_clear_to_drive_away(lua_State* L);
int car_is_door_fully_open(lua_State* L);
int car_is_door_missing(lua_State* L);
int car_is_door_ready(lua_State* L);
int car_is_law_enforcement(lua_State* L);
int car_is_on_its_side(lua_State* L);
int car_is_open_top(lua_State* L);
int car_is_upside_down(lua_State* L);
int car_is_vehicle_normal(lua_State* L);
int car_play_horn(lua_State* L);
int car_shuffle_passengers(lua_State* L);
int car_update_passenger_list(lua_State* L);
int car_uses_siren(lua_State* L);
int clock_restore(lua_State* L);
int clock_store(lua_State* L);
int create_explosion_no_sound(lua_State* L);
int fire_find_nearest(lua_State* L);
int fire_is_active(lua_State* L);
int fire_start_on_point(lua_State* L);
int font_set_alpha_fade(lua_State* L);
int font_set_background_off(lua_State* L);
int font_set_background_on(lua_State* L);
int font_set_centre_size(lua_State* L);
int font_set_drop_shadow(lua_State* L);
int font_set_prop_off(lua_State* L);
int font_set_prop_on(lua_State* L);
int font_set_right_justify_off(lua_State* L);
int font_set_right_justify_on(lua_State* L);
int font_set_style(lua_State* L);
int font_set_wrapx(lua_State* L);
int force_rain(lua_State* L);
int force_weather(lua_State* L);
int force_weather_now(lua_State* L);
int gamelogic_after_death_arrest_shortcut(lua_State* L);
int gamelogic_pass_time(lua_State* L);
int gamelogic_update(lua_State* L);
int gangs_choose_ped_model(lua_State* L);
int gangs_get_attack_with_cops(lua_State* L);
int gangs_initialise(lua_State* L);
int gangs_set_attack_with_cops(lua_State* L);
int gangs_set_ped_models(lua_State* L);
int gangs_set_vehicle_model(lua_State* L);
int gangs_set_weapons(lua_State* L);
int general_angle_between_points(lua_State* L);
int general_atan_of_xy(lua_State* L);
int general_limit_angle(lua_State* L);
int general_limit_radian_angle(lua_State* L);
int general_node_heading_from_vector(lua_State* L);
int general_radian_angle_between_points(lua_State* L);
int get_all_random_peds_type(lua_State* L);
int get_ammo_in_clip_ped(lua_State* L);
int get_anim_block_index(lua_State* L);
int get_car_brake_pedal(lua_State* L);
int get_car_created_by(lua_State* L);
int get_car_door_lock(lua_State* L);
int get_car_driver(lua_State* L);
int get_car_engine_on(lua_State* L);
int get_car_gas_pedal(lua_State* L);
int get_car_handbrake_on(lua_State* L);
int get_car_is_big(lua_State* L);
int get_car_is_bus(lua_State* L);
int get_car_is_van(lua_State* L);
int get_car_last_damager(lua_State* L);
int get_car_lights_on(lua_State* L);
int get_car_locked(lua_State* L);
int get_car_max_passengers(lua_State* L);
int get_car_num_passengers(lua_State* L);
int get_car_passenger(lua_State* L);
int get_car_radio_station(lua_State* L);
int get_car_steer_angle(lua_State* L);
int get_car_time_of_death(lua_State* L);
int get_car_tires_invulnerable(lua_State* L);
int get_car_vehicle_class(lua_State* L);
int get_car_wanted_stars_on_enter(lua_State* L);
int get_car_weapon_damage(lua_State* L);
int get_chance_on_roadblock(lua_State* L);
int get_cops_in_pursuit(lua_State* L);
int get_fade_color(lua_State* L);
int get_far_clip(lua_State* L);
int get_fire_position(lua_State* L);
int get_fire_target(lua_State* L);
int get_fire_time_to_burn(lua_State* L);
int Getflagmission(lua_State* L);
int get_fov(lua_State* L);
int get_frame_counter(lua_State* L);
int get_game_hours(lua_State* L);
int get_game_minutes(lua_State* L);
int get_game_seconds(lua_State* L);
int get_lod_distance(lua_State* L);
int get_max_cop_cars_in_pursuit(lua_State* L);
int get_max_cops_in_pursuit(lua_State* L);
int get_milliseconds_per_game_minute(lua_State* L);
int get_mouse_accel_horizontal(lua_State* L);
int get_mouse_accel_vertical(lua_State* L);
int get_near_clip(lua_State* L);
int get_new_weather_type(lua_State* L);
int get_obj_attach_force(lua_State* L);
int get_obj_bonus_value(lua_State* L);
int get_obj_camera_avoids(lua_State* L);
int get_obj_collision_damage_type(lua_State* L);
int get_obj_cost_value(lua_State* L);
int get_obj_damage_multiplier(lua_State* L);
int get_obj_ref_model_id(lua_State* L);
int get_obj_timer(lua_State* L);
int get_obj_type(lua_State* L);
int get_old_weather_type(lua_State* L);
int get_ped_density_multiplier(lua_State* L);
int get_ped_enter_type(lua_State* L);
int get_ped_from_vehicle(lua_State* L);
int get_ped_heading(lua_State* L);
int get_ped_heading_goal(lua_State* L);
int get_ped_heading_rate(lua_State* L);
int get_ped_last_state(lua_State* L);
int get_ped_stat_cheated(lua_State* L);
int get_ped_state(lua_State* L);
int get_ped_stat_kills(lua_State* L);
int get_player_display_money(lua_State* L);
int get_player_money_info(lua_State* L);
int get_radio_channel(lua_State* L);
int get_stat_assassinations(lua_State* L);
int get_stat_auto_painting_budget(lua_State* L);
int get_stat_bloodring_kills(lua_State* L);
int get_stat_bloodring_time(lua_State* L);
int get_stat_fashion_budget(lua_State* L);
int get_stat_flight_time(lua_State* L);
int get_stat_frenzies_passed(lua_State* L);
int get_stat_frenzies_total(lua_State* L);
int get_stat_garbage_pickups(lua_State* L);
int get_stat_ice_cream_sold(lua_State* L);
int get_stat_loan_sharks(lua_State* L);
int get_stat_longest_2wheel_dist(lua_State* L);
int get_stat_longest_stoppie_dist(lua_State* L);
int get_stat_longest_wheelie_dist(lua_State* L);
int get_stat_longest_wheelie_time(lua_State* L);
int get_stat_missions_total(lua_State* L);
int get_stat_movie_stunts(lua_State* L);
int get_stat_num_property_owned(lua_State* L);
int get_stat_pizzas_delivered(lua_State* L);
int get_stat_property_budget(lua_State* L);
int get_stat_property_destroyed(lua_State* L);
int get_stat_seagulls_killed(lua_State* L);
int get_stat_shooting_rank(lua_State* L);
int get_stat_stores_knocked_off(lua_State* L);
int get_stat_times_drowned(lua_State* L);
int get_stat_top_shooting_score(lua_State* L);
int get_stat_weapon_budget(lua_State* L);
int get_sun_blocked_by_clouds(lua_State* L);
int get_timer_ms(lua_State* L);
int get_timer_ms_pause_mode(lua_State* L);
int get_time_step(lua_State* L);
int get_time_until(lua_State* L);
int get_use_mouse_3rd_person(lua_State* L);
int get_wanted_chaos_level(lua_State* L);
int get_wanted_last_decreased_time(lua_State* L);
int get_wanted_level_before_parole(lua_State* L);
int get_wanted_multiplier(lua_State* L);
int get_weapon_firing_rate(lua_State* L);
int get_weapon_lifespan(lua_State* L);
int get_weapon_radius(lua_State* L);
int get_weapon_range(lua_State* L);
int get_weapon_reload(lua_State* L);
int get_weapon_slot_info(lua_State* L);
int get_weapon_speed(lua_State* L);
int get_weapon_spread(lua_State* L);
int get_weapon_state(lua_State* L);
int is_lightning_flash(lua_State* L);
int is_time_in_range(lua_State* L);
int is_user_pause(lua_State* L);
int message_add(lua_State* L);
int message_add_big(lua_State* L);
int message_add_big_queue(lua_State* L);
int message_add_jump_queue(lua_State* L);
int message_add_with_number(lua_State* L);
int message_clear_all(lua_State* L);
int message_clear_small_only(lua_State* L);
int message_clear_this_big_print(lua_State* L);
int message_clear_this_print(lua_State* L);
int obj_can_be_deleted(lua_State* L);
int obj_damage(lua_State* L);
int obj_delete_all_mission(lua_State* L);
int obj_delete_all_temp(lua_State* L);
int obj_delete_all_temp_in_area(lua_State* L);
int ped_answer_mobile(lua_State* L);
int ped_blend_anim(lua_State* L);
int ped_buy_ice_cream(lua_State* L);
int ped_calculate_new_orientation(lua_State* L);
int ped_can_be_deleted(lua_State* L);
int ped_clear_aim_flag(lua_State* L);
int ped_clear_answer_mobile(lua_State* L);
int ped_clear_attack(lua_State* L);
int ped_clear_attack_anim(lua_State* L);
int ped_clear_chat(lua_State* L);
int ped_clear_follow_path(lua_State* L);
int ped_clear_look_flag(lua_State* L);
int ped_clear_point_gun_at(lua_State* L);
int ped_clear_seek(lua_State* L);
int ped_clear_wait_state(lua_State* L);
int ped_dress(lua_State* L);
int ped_face_phone(lua_State* L);
int ped_fall(lua_State* L);
int ped_is_gang_member(lua_State* L);
int ped_is_in_control(lua_State* L);
int ped_is_player(lua_State* L);
int ped_is_pointer_valid(lua_State* L);
int ped_is_shootable(lua_State* L);
int ped_look_for_sexy_cars(lua_State* L);
int ped_look_for_sexy_peds(lua_State* L);
int ped_make_phonecall(lua_State* L);
int ped_mug(lua_State* L);
int ped_pause(lua_State* L);
int ped_play_anim(lua_State* L);
int ped_restart_non_partial_anims(lua_State* L);
int ped_restore_heading_rate(lua_State* L);
int ped_restore_head_position(lua_State* L);
int ped_restore_previous_objective(lua_State* L);
int ped_restore_previous_state(lua_State* L);
int ped_set_dead(lua_State* L);
int ped_set_get_up(lua_State* L);
int ped_set_idle(lua_State* L);
int ped_set_jump(lua_State* L);
int ped_set_landing(lua_State* L);
int ped_set_move_anim(lua_State* L);
int ped_solicit(lua_State* L);
int ped_stop_non_partial_anims(lua_State* L);
int ped_turn_body(lua_State* L);
int ped_update_position(lua_State* L);
int play_continuous_sound(lua_State* L);
int play_one_off_sound(lua_State* L);
int police_radio_message(lua_State* L);
int population_convert_all_objects_to_dummy(lua_State* L);
int population_is_female(lua_State* L);
int population_is_male(lua_State* L);
int population_is_sunbather(lua_State* L);
int population_manage(lua_State* L);
int population_remove_peds_full_pool(lua_State* L);
int radar_change_blip_brightness(lua_State* L);
int radar_change_blip_colour(lua_State* L);
int radar_change_blip_display(lua_State* L);
int radar_change_blip_scale(lua_State* L);
int radar_clear_blip(lua_State* L);
int radar_clear_blip_for_entity(lua_State* L);
int radar_draw_radar_sprite(lua_State* L);
int radar_get_radar_range(lua_State* L);
int radar_set_blip_sprite(lua_State* L);
int radar_set_coord_blip(lua_State* L);
int radar_set_entity_blip(lua_State* L);
int radar_set_marker_state(lua_State* L);
int radar_set_radar_range(lua_State* L);
int radar_set_short_range_blip(lua_State* L);
int radar_show_marker(lua_State* L);
int release_weather(lua_State* L);
int remove_sound_by_id(lua_State* L);
int set_all_random_peds_type(lua_State* L);
int set_ammo_in_clip_ped(lua_State* L);
int set_car_comedy_controls(lua_State* L);
int set_car_door_lock(lua_State* L);
int set_car_engine_on(lua_State* L);
int set_car_handbrake_on(lua_State* L);
int set_car_hide_occupants(lua_State* L);
int set_car_lights_on(lua_State* L);
int set_car_locked(lua_State* L);
int set_car_radio_station(lua_State* L);
int set_car_steer_angle(lua_State* L);
int set_car_tires_invulnerable(lua_State* L);
int set_car_wanted_stars_on_enter(lua_State* L);
int set_chance_on_roadblock(lua_State* L);
int set_fade_color(lua_State* L);
int set_far_clip(lua_State* L);
int set_fire_time_to_burn(lua_State* L);
int Setflagmission(lua_State* L);
int set_fov(lua_State* L);
int set_game_hours(lua_State* L);
int set_lod_distance(lua_State* L);
int set_max_cop_cars_in_pursuit(lua_State* L);
int set_max_cops_in_pursuit(lua_State* L);
int set_milliseconds_per_game_minute(lua_State* L);
int set_mouse_accel_horizontal(lua_State* L);
int set_mouse_accel_vertical(lua_State* L);
int set_music_does_fade(lua_State* L);
int set_near_clip(lua_State* L);
int set_obj_attach_force(lua_State* L);
int set_obj_bonus_value(lua_State* L);
int set_obj_camera_avoids(lua_State* L);
int set_obj_collision_damage_type(lua_State* L);
int set_obj_cost_value(lua_State* L);
int set_obj_damage_multiplier(lua_State* L);
int set_obj_timer(lua_State* L);
int set_ped_density_multiplier(lua_State* L);
int set_ped_heading_goal(lua_State* L);
int set_ped_heading_rate(lua_State* L);
int set_ped_state(lua_State* L);
int set_player_drive_by(lua_State* L);
int set_player_money_info(lua_State* L);
int set_radio_channel(lua_State* L);
int set_wanted_chaos_level(lua_State* L);
int set_wanted_multiplier(lua_State* L);
int set_weapon_firing_rate(lua_State* L);
int set_weapon_lifespan(lua_State* L);
int set_weapon_radius(lua_State* L);
int set_weapon_range(lua_State* L);
int set_weapon_reload(lua_State* L);
int set_weapon_speed(lua_State* L);
int set_weapon_spread(lua_State* L);
int set_weather_type(lua_State* L);
int wanted_are_army_required(lua_State* L);
int wanted_are_fbi_required(lua_State* L);
int wanted_are_swat_required(lua_State* L);
int wanted_cheat_level(lua_State* L);
int wanted_clear_crimes(lua_State* L);
int wanted_num_helis_required(lua_State* L);
int wanted_reset_police_pursuit(lua_State* L);
int wanted_set_no_drop(lua_State* L);
int wanted_update_wanted_level(lua_State* L);
int weapon_has_ammo_to_be_used(lua_State* L);
int weapon_is_melee(lua_State* L);
int weapon_is_two_handed(lua_State* L);
int weapon_reload_ped(lua_State* L);
int weather_add_rain(lua_State* L);
int weather_add_stream_after_rain(lua_State* L);
int weather_render_rain_streaks(lua_State* L);
int world_add_entity(lua_State* L);
int world_call_off_chase_for_area(lua_State* L);
int world_clear_cars_from_area(lua_State* L);
int world_clear_exciting_stuff_from_area(lua_State* L);
int world_clear_peds_from_area(lua_State* L);
int world_clear_scan_codes(lua_State* L);
int world_extinguish_all_car_fires_in_area(lua_State* L);
int world_find_ground_z_for_3d_coord(lua_State* L);
int world_find_ground_z_for_coord(lua_State* L);
int world_find_roof_z_for_3d_coord(lua_State* L);
int world_get_is_line_of_sight_clear(lua_State* L);

