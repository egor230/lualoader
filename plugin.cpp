#define _CRT_SECURE_NO_WARNINGS
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

#include <algorithm>
#include<lua.hpp>
#include<LuaBridge.h>

#include "common.h"
#include "plugin.h"
#include "CWorld.h"
#include "extensions\KeyCheck.h"
#include "extensions\ScriptCommands.h"
#include "eScriptCommands.h"
#include "CCivilianPed.h"
#include "CMessages.h"
#include "ePedModel.h"
#include "ePedType.h"
#include "CModelInfo.h"

#include "CStreaming.h"
#include "CTheScripts.h"
#include "eWeaponType.h"
#include "eWeaponModel.h"
#include "CPickups.h"
#include "CClock.h"
#include "CUserDisplay.h"
#include "CAnimManager.cpp"
#include "CBaseModelInfo.cpp"

using namespace plugin;
using namespace std;
using namespace luabridge;
static int iters = 0;
list<char*>listfile;//Список Lua файлов.

list<lua_State*>luastate;// list для lua состояний.
static map<int, lua_State*>markeron;// map для маркеров.
static map<int, lua_State*>spheres;// map для маркеров кругов.

static map<int, lua_State*>pickupsids;// map для пикапов.
static map<CPed*, lua_State*>mappeds;// map для педов.
static map<CVehicle*, lua_State*>mapcars;// map для авто.
static map<CObject*, lua_State*>mapobjs;// map для объектов.

static map<int, CPed*>ptr_for_ped;//
static string cheatstr;// символы введенные с клавиатуры.
string getkey(int key);

struct star_thread {
	static bool star_second_thread;// запускать второй поток.

	static void set(bool star_second_thread1) {
		star_second_thread = star_second_thread1;
	}
	static bool get() {
		return star_second_thread;
	};//;
};
bool star_thread::star_second_thread;

struct star_coroutine {
	static bool coroutine;// выкл/вкл корутин.

	static void set(bool coroutine1) {
		coroutine = coroutine1;
	}
	static bool get() {
		return coroutine;
	};//;
};
bool star_coroutine::coroutine;

struct spite {
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
	static void active(bool switc1, CPed* p1) {
		bool switc = switc1;
		CPed* p = p1;
	}
	static void draw() { //
		if (printgame == true) {
			gamefont::Print({ Format(strprintgame) }, abc_x, ord_y, spacing, font, size_x, size_y, CRGBA(red, blue, green));
		}
		if (switc == true) {
			p->Attack();
		}
	}//;
};
bool spite::switc;
CPed* spite::p;
bool spite::printgame;// флаг для вывода экран.
string spite::strprintgame;// вывод на экран.
int spite::abc_x; int spite::ord_y; float spite::spacing; int spite::font; float spite::size_x;
float spite::size_y; int spite::red; int spite::blue; int spite::green;

struct corona {
	static bool switc;
	static double abc_x; static double ord_y; static double pos_z; static double radius;
	static int type; static int glow_flare; static int red; static int blue;
	static int green;

	static void set(bool switc1, double abc_x1, double ord_y1, double pos_z1, double radius1,
		int type1, int glow_flare1, int red1, int blue1, int green1) { // Включить событие двери
		switc = switc1;
		abc_x = abc_x1; ord_y = ord_y1;	pos_z = pos_z1; type = type1; radius = radius1; glow_flare = glow_flare1;
		red = red1; blue = blue1; green = green1;
	}
	static void draw() {
		double x = abc_x;	double y = ord_y;	double z = pos_z;
		double radius1 = radius; int type1 = type;	int glow_flare1 = glow_flare;
		int red1 = red;  int blue1 = blue; int green1 = green;
		if (switc == true) {
			Command<COMMAND_DRAW_CORONA>(x, y, z, radius1, type1, glow_flare1, red1, green1, blue1);
		}
	}//;
};

bool corona::switc; // флаг для вывода экран.
double corona::abc_x; double corona::ord_y; double corona::pos_z; double corona::radius;
int corona::type; int corona::glow_flare;
int corona::red; int corona::blue; int corona::green; const float ACTION_TIME_STEP = 0.05f;
const unsigned int TIME_FOR_KEYPRESS = 500;
struct Doorse {
	static int componentByDoorId[6]; // Таблица перевода eDoors в Id компонента
	static int m_nLastTimeWhenAnyActionWasEnabled; // Последнее время запуска события
	enum eDoorEventType { DOOR_EVENT_OPEN, DOOR_EVENT_CLOSE };// Тип события

	struct DoorEvent { // Класс события
		bool m_active;	eDoorEventType m_type;	float m_openingState;
		DoorEvent() { m_active = false;	m_type = DOOR_EVENT_CLOSE; }
	};

	struct VehicleDoors {
		DoorEvent events[6]; // События для всех 6 дверей
		VehicleDoors(CVehicle*) {}
	};
};
struct DoorsExample {
	static int componentByDoorId[6]; // Таблица перевода eDoors в Id компонента
	static int m_nLastTimeWhenAnyActionWasEnabled; // Последнее время запуска события

	enum eDoorEventType { DOOR_EVENT_OPEN, DOOR_EVENT_CLOSE };// Тип события

	struct DoorEvent {	bool m_active;	eDoorEventType m_type;	float m_openingState;
		DoorEvent() { m_active = false;	m_type = DOOR_EVENT_CLOSE; }// Класс события
	};

	struct VehicleDoors {
		DoorEvent events[6]; // События для всех 6 дверей
		VehicleDoors(CVehicle*) {}
	};

	static VehicleExtendedData<VehicleDoors> VehDoors; // Наше расширение

	static void EnableDoorEvent(CAutomobile* automobile, eDoors doorId) { // Включить событие двери
		if (automobile->IsComponentPresent(componentByDoorId[doorId])) {
			CDamageManager* p;
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
int DoorsExample::componentByDoorId[6] = { CAR_BONNET, CAR_BOOT, CAR_DOOR_LF, CAR_DOOR_RF, CAR_DOOR_LR, CAR_DOOR_RR };
int DoorsExample::m_nLastTimeWhenAnyActionWasEnabled = 0;
VehicleExtendedData<DoorsExample::VehicleDoors> DoorsExample::VehDoors;

int& var_$3402 = *(int*)0x8247A8; // глобальнная переменная таймера.

CPed* findpedinpool(const void* p); // найти педа в пуле.
CVehicle* findcarinpool(const void* p); // найти авто в пуле.
CObject* findobjinpool(const void* p);// найти объект в пуле.

void funs(lua_State* L);// список функций.
void writelog(const char x[]);// запись ошибок в файл.
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

int key(lua_State* L);// проверка на нажатия клавиши.
int lockstatus(lua_State* L);// проверка на нажатия клавиши.
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
int worldcoord(lua_State* L); // Перевод в мировые координаты.
int getpedcoordinates_on_abscissa(lua_State* L); // Получить мировую координату по x для педа.

int getpedcoordinates_on_ordinate(lua_State* L); // Получить мировую координату по y для педа.
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
int getcarcoordinates_on_abscissa(lua_State* L);// Получить мировую координату по x для авто.
int getcarcoordinates_on_ordinate(lua_State* L); // Получить мировую координату по y для авто.
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
int setsizemarker(lua_State* L); //установить размер отображение маркера на карте.
int checkcheat(lua_State* L);//чит код введен?.
int destroy(lua_State* L);// удаления объектов из памяти при перезагрузки скрипта. 

int my_yield(lua_State* L);//приостановить выполнение скрипта.
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
int setobjоcoordes(lua_State* L); // установить координаты для объект.
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

int set_camera_position(lua_State* L); //уст камеру в координатах.
int restore_camera(lua_State* L); // восстановить камеру.
int is_wanted_level(lua_State* L); // проверить уровень розыска.
int ped_atack(lua_State* L); // пед бьет.

int flash_hud(lua_State* L); // Мигание элементов HUD.
int set_radio(lua_State* L); // уст радио.
int set_car_tires(lua_State* L); // проколоть шину авто.

int create_spec_ped(lua_State* L); // создать спец педа.
int set_wheel_status(lua_State* L); // уст состояния шин авто.
int set_skin(lua_State* L); // уст скин педа.
int remove_spec_ped(lua_State* L);  // удалить спец педа.

int go_to_route(lua_State* L); //уст маршрут авто.
int add_stuck_car_check(lua_State* L); // условия для того, чтобы авто считалась застрявшей.
int load_scene(lua_State* L); // загрузить модели на координатах заранее.
int ped_anim(lua_State* L); // анимация.

int del_anim(lua_State* L); // удалить анимацию.
int get_current_name_luascript(lua_State* L); // получить имя текущего lua файла.
int star_mission_marker(lua_State* L); // создать маркер для миссии.
int getobjcoordinates_on_abscissa(lua_State* L); // Получить мировую координату по x для объекта.

int getobjcoordinates_on_ordinate(lua_State* L); // Получить мировую координату по y для объекта.
int set_widescreen(lua_State* L); // вкл/выкл широкий экран.
int follow_the_leader(lua_State* L); //01DE / 01DF	следовать за лидером.
int getcarspeed(lua_State* L); // получить скорость авто.

int newthread(lua_State* L);// запуск функции в новом потоке.

void reversestack(lua_State* L); // реверс стека.
void showstack(lua_State* L);
void showstack1(lua_State* L);

int my_yield(lua_State* L) {
	return lua_yield(L, 0);//приостановить выполнение скрипта.
};
lua_KFunction cont(lua_State* L);
int my_yield_with_res(lua_State* L, int res) {
	return lua_yieldk(L, 0, lua_yield(L, res), cont(L));/* int lua_yieldk(lua_State * L, int res, lua_KContext ctx, lua_KFunction k);
	Приостанавливает выполнение сопрограммы(поток).	Когда функция C вызывает lua_yieldk, работающая
	сопрограмма приостанавливает свое выполнение и вызывает lua_resume, которая начинает возврат данной сопрограммы.
	Параметр res - это число значений из стека, которые будут переданы в качестве результатов в lua_resume.
	Когда сопрограмма снова возобновит выполнение, Lua вызовет заданную функцию продолжения k для продолжения выполнения
	приостановленной C функции(смотрите §4.7). */
};
int hookFunc(lua_State* L, lua_Debug* ar) {	return my_yield_with_res(L, 0);// хук./
};

lua_KFunction cont(lua_State* L) {// функция продолжения.
	lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
	return 0;
};

void startscipt(string res, char* luafile, list<lua_State*>& luastate) {
	lua_State* L = luaL_newstate();	luaL_openlibs(L);
	funs(L); // список функций.	
	int status = luaL_loadfile(L, luafile);// проверка есть ли ошибки в файле.
	try {
		if (status == 0) {// если нет ошибки в файле.	
			//lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
			lua_pushlightuserdata(L, L);  /* ключ в реестр указатель на L. */
			lua_pushstring(L, luafile);  /* отправить имя текущего lua файла в реестр.*/
			lua_settable(L, LUA_REGISTRYINDEX);  /* установить ключа и значение таблице реестре.  */
			string er0 = "loaded " + res;// перед имени текущего lua файла добавить loaded.
			char* x = strdup(er0.c_str());// преобразовать строку в char*.
			writelog(x);// запись резуальтат проверки на ошибки.
			lua_pcall(L, 0, 0, 0);// запуск файла.
			lua_State* L1 = lua_newthread(L);// создать новый поток.
			lua_getglobal(L, "main");
			if (LUA_TFUNCTION == lua_type(L, -1)) {	bool coroutine = true; star_coroutine::set(coroutine);
				luastate.push_back(L);// добавить указатель на lua состояния в list.
				lua_resume(L, NULL, 0);	//Основной поток
				int args = lua_gettop(L);// получить аргументы для второго потока.
				lua_xmove(L, L1, args);	args--;	reversestack(L1); //инвертировать содержимое стека.
				while (LUA_OK != lua_status(L)) {// Пока основной поток не закончен.
					this_thread::sleep_for(chrono::milliseconds(1)); // задержка.
					if (LUA_TFUNCTION == lua_type(L1, -1) && LUA_YIELD == lua_status(L) && star_coroutine::get()){

						for (int i = 1; i <= args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.
						lua_resume(L1, L, args);
					}
					if (LUA_YIELD == lua_status(L1)) { lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 800); //вызов функции с заданной паузой.
						lua_resume(L, L1, 0);// возобновить основной поток.
					}
					if (LUA_OK == lua_status(L1)) {// если второй поток завершен.      
						lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
						lua_resume(L, NULL, 0);
					}
				}
			}
			else { /*lua_close(L);*/ }
		}
		else { string er1 = lua_tostring(L, -1); string er0 = "could not load " + er1;
			char* x = strdup(er1.c_str());  throw x;	}
	}
	catch (const char* x) { writelog(x); }
};
void search() {
	for (auto const& de : std::experimental::filesystem::recursive_directory_iterator{
		std::experimental::filesystem::current_path() / "lualoader" }) { // папка для поиска
		if (de.path().extension() == ".lua" || de.path().extension() == ".LUA"){
			string res = de.path().string();// перевод имя файла в строку.
			char* luafile = strdup(res.c_str());// Текущий lua файл.
			listfile.push_back(luafile);// добавить текущий lua файл в list.
			std::thread(startscipt, res, luafile, std::ref(luastate)).detach();// независимым поток для запуска lua файла..
		}
	};
};
char q;
static	string faststr = "";
void getkeyenvent() {// считывания символов клавиатуры.
	bool star_second_thread = star_thread::get();// получить флаг запуска второго потока.
	while (star_second_thread == true) { this_thread::sleep_for(chrono::milliseconds(1));
		for (q = 8; q <= 190; q++) {
			string faststr = getkey(q);// получаем символ нажатой клавиши.
			while (true) {	this_thread::sleep_for(chrono::milliseconds(1));
				string f2 = getkey(q);// Ждем отпускание клавиши.
				if (faststr != f2) {
					break;
				}
			};
			if (faststr != "") { cheatstr = cheatstr + faststr; }
			int size = cheatstr.size();// Если длина строки больше 10 символов, удаляем строку.
			if (size > 9) {	cheatstr.clear();	}
		}
	}
};
void second() {
	dellod(); // удалить лог ошибок.	
	search(); // найти все lua файлы.
	std::thread(getkeyenvent).detach();// функция считывание на клавиатуры.
	static unsigned int time = 0;// обнулить таймер.
	while (true) {	this_thread::sleep_for(chrono::milliseconds(1));
		if (KeyPressed(VK_CONTROL)) {// перезагрузка скрипта    
			bool coroutine = false;
			star_coroutine::set(coroutine);
			break;
		};
	};
	for (auto L : luastate) { 
		if (LUA_TFUNCTION == lua_type(L, -1)) {
			lua_setglobal(L, "lualoader");// установить значение переменной в lua.
			lua_pushinteger(L, 110); 
			//my_yield(L);//приостановить выполнение скрипта.
			destroy(L);
			//lua_close(L);
		}
		else {//destroy(L); 		//luastate.remove(L);
		}
	};
	if (CTimer::m_snTimeInMilliseconds - time > 500) {	time = 0;// обнулить таймер
		CMessages::AddMessageJumpQ(L"Script reloaded", 2000, 3);
	};
	while (true) {	this_thread::sleep_for(chrono::milliseconds(1));
		if (!KeyPressed(VK_CONTROL)) {
			break;
		}
	}; 
	for (auto L : luastate) {	luastate.pop_front();	};
    // cleanstl();// очистка всех массивов при перезагрузке скрипта.
	this_thread::sleep_for(chrono::milliseconds(10));

	bool k = false;	star_thread::set(k); // флаг, что можно запускать новый поток.
};
bool getstatusmission() {// проверка флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);// получить флаг миссии.
	return getflagmission;
};

int setstatusmission(bool flag) { // уcтановить флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	CTheScripts::ScriptSpace[OnAMissionFlag] = flag;
	return 0;
};
int all_destroy(); // удаления объектов из памяти. 
class Message {//имя класса.
public: Message() {
	static unsigned int time = 0;// обнулить таймер.
	Events::gameProcessEvent += [] {//обработчик событий игры.
		Events::gameProcessEvent += spite::draw; Events::gameProcessEvent += corona::draw;
		bool star_second_thread = star_thread::get();// получить флаг запуска второго потока.
		iters++;
		Events::vehicleRenderEvent += DoorsExample::ProcessDoors; // Тут обрабатываем события, а также выключаем их
		CPed* player = FindPlayerPed();// найти педа.
		if (player != NULL) {// проверка найден пед.

			if ((Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus)) && star_second_thread == false && CTimer::m_snTimeInMilliseconds - time > 136500) {
				time = 0;// обнулить таймер.
				bool k = true;// флаг, что уже запущен поток. 
				star_thread::set(k);
				this_thread::sleep_for(chrono::milliseconds(16));// задержка
				thread th(second); th.detach();// независимый поток.       
			}
		}
	};
}
} message;

void funs(lua_State* L) {// список функций.
	getGlobalNamespace(L)//Пространства имен LuaBridge для регистрации функции и классов, видны только сценариям Lua.

		.beginClass<CPed>("cped")// имя класса пед в lua.
		.addFunction("atack", &CPed::Attack)
		.endClass()// закрыть регистрацию класса.  
		.beginClass<CVehicle>("CVehicle")// имя класса авто в lua.
		.endClass()// закрыть регистрацию класса.
		.beginClass<CObject>("CObject")// имя класса объекта в lua.
		.endClass()// закрыть регистрацию класса.
		.addCFunction("findplayer", findplayer)// возвращает указатель педа.

		.addCFunction("setpedhealth", setpedhealth)//  установить здоровье педу.
		.addCFunction("setarmour", setarmour)//  установить броню педу.
		.addCFunction("wait", wait)//  задержка.
		.addCFunction("getpedhealth", getpedhealth)// название функции в lua и c++. получить здоровье педа.

		.addCFunction("getpedangle", getpedangle)// получить угол педа.
		.addCFunction("worldcoord", worldcoord)// Перевод в мировые координаты.
		.addCFunction("getpedcoordinates_on_x", getpedcoordinates_on_abscissa)// Получить мировую координату по x для педа.
		.addCFunction("getpedcoordinates_on_y", getpedcoordinates_on_ordinate) // Получить мировую координату по y для педа.

		.addCFunction("setarmour", setarmour)//  получить броню.
		.addCFunction("givemoney", givemoney)//  дать денег педу.
		.addCFunction("keypress", key)//  проверка на нажатие клавиш.
		.addCFunction("printmessage", printmessage)//  вывод сообщение.

		.addCFunction("getpedcoordes", getpedcoordes)//  получить координаты педа.
		.addCFunction("randomfindped", randomfindped)//  получить рандомного педа.
		.addCFunction("incar", incar)//  проверка пед в авто?.
		.addCFunction("loadmodel", loadmodel)// загрузить модель.

		.addCFunction("availablemodel", availablemodel)// проверка на загруженность модели.
		.addCFunction("releasemodel", releasemodel)// удалить модель из памяти.
		.addCFunction("createcar", createcar)// создать авто на координатах на координатах.
		.addCFunction("createped", &createped) // создать педа на координатах.

		.addCFunction("load_requested_models", load_requested_models)// поставить модель на загрузить вне очереди.
		.addCFunction("giveweaponped", giveweaponped)// дать педу оружие.
		.addCFunction("ped_sprint_to_point", ped_sprint_to_point)// пед делает спринт к точке.
		.addCFunction("ped_walk_to_point", ped_walk_to_point)//Пед идет к точке.

		.addCFunction("kill_ped_on_foot", kill_ped_on_foot)// убить педа пешком.
		.addCFunction("kill_char_any_means", kill_char_any_means)// убить педа любыми средствами.
		.addCFunction("ped_aim_at_ped", ped_aim_at_ped)// пед целиться в педе.
		.addCFunction("is_current_weapon_ped", is_current_weapon_ped)// проверить текущее оружие.

		.addCFunction("create_marker_actor", create_marker_actor)// создать маркер над педом.
		.addCFunction("removemarker", removemarker)// удалить маркер.
		.addCFunction("setpedcoordes", setpedcoordes) // установить координаты для педа.
		.addCFunction("remove_car", remove_car) // удалить авто.

		.addCFunction("car_in_water", car_in_water) // проверка авто в воде.
		.addCFunction("set_wanted", set_wanted) // уcтановить уровень розыска.
		.addCFunction("ped_in_point_in_radius", ped_in_point_in_radius) // проверить находится пед в координатах с радиусом.
		.addCFunction("create_sphere", &create_sphere) //создать сферу.

		.addCFunction("clear_wanted", clear_wanted)// убрать уровень розыска.
		.addCFunction("getcarhealth", getcarhealth) // получить кол-во здоровья авто.
		.addCFunction("setcarhealth", setcarhealth) // установить здоровье авто.
		.addCFunction("remove_sphere", remove_sphere) // удалить сферу.

		.addCFunction("remove_ped", remove_ped) // удалить педа.
		.addCFunction("kill_ped", kill_ped) // убить педа.
		.addCFunction("getflagmission", getflagmission) // проверка флага миссии.
		.addCFunction("setflagmission", setflagmission) // уcтановить флага миссии.

		.addCFunction("showtext", showtext)// Вывод особого текста на экран.
		.addCFunction("remove_blip", remove_blip)// удалить метку с карты.
		.addCFunction("createblip", createblip) // создать метку карте.
		.addCFunction("play_sound", play_sound)// проиграть мелодию.

		.addCFunction("isped", isped)// проверка это пед?
		.addCFunction("isvehicle", isvehicle) // проверка это транспорт?.
		.addCFunction("cardrive", &cardrive) // авто едет в точку.
		.addCFunction("setcarspeed", setcarspeed) // установить скорость авто.

		.addCFunction("opendoorcar", opendoorcar) // открыть дверь авто.
		.addCFunction("randomfindcar", randomfindcar) //Найти случайное авто.
		.addCFunction("getcarcoordes", getcarcoordes) // получить координаты авто.
		.addCFunction("create_money_pickup", create_money_pickup) //создать пачку денег.

		.addCFunction("getcarcoordinates_on_x", getcarcoordinates_on_abscissa)// Получить мировую координату по x для авто.
		.addCFunction("getcarcoordinates_on_y", getcarcoordinates_on_ordinate)// Получить мировую координату по y для авто.
		.addCFunction("car_in_point_in_radius", car_in_point_in_radius) // проверить находится авто в координатах с радиусом.
		.addCFunction("setdrivingstyle", setdrivingstyle)// установить стиль езды авто.

		.addCFunction("findped", findped)// найти педа в пуле.
		.addCFunction("create_weapon_pickup", create_weapon_pickup) //создать пикап оружие.
		.addCFunction("create_pickup", create_pickup) //создать пикап.
		.addCFunction("remove_pickup", remove_pickup) // удалить пикап.

		.addCFunction("picked_up", picked_up)// проверка пикап подобран.
		.addCFunction("play_voice", play_voice) // Проиграть голос.
		.addCFunction("fade", fade) //затенение, просветления.
		.addCFunction("draw_corona", draw_corona) // создать корону(чекпойнт).

		.addCFunction("sound_coordinate", sound_coordinate) // Проиграть звук в координатах
		.addCFunction("show_text_styled", show_text_styled) // Вывести игровой текст.
		.addCFunction("setcarangle", setcarangle)// установить угол авто.
		.addCFunction("createmarker", createmarker) // создать маркер на карте.

		.addCFunction("setsizemarker", setsizemarker)//установить размер маркера.
		.addCFunction("cheat", checkcheat) //чит код введен.
		.addCFunction("destroy", destroy) // удаления объектов из памяти при перезагрузки скрипта. 
		.addCFunction("yield", my_yield) //приостановить выполнение скрипта.

		.addCFunction("setcardrive", setcardrive) // установить водителя для авто.
		.addCFunction("setcarpassenger", setcarpassenger) // установить пассажира для авто.
		.addCFunction("setcarfirstcolor", setcarfirstcolor) // установить первый цвет авто.
		.addCFunction("setcarseconscolor", setcarseconscolor)// установить второй цвет авто.

		.addCFunction("set_traffic", set_traffic) // установить плотномть трафика транспорта.
		.addCFunction("create_marker_car", create_marker_car)//создать маркер над авто.
		.addCFunction("car_explode", car_explode) // взрывать авто.
		.addCFunction("is_car_stopped", is_car_stopped) // авто остановилось. 

		.addCFunction("create_explosion", create_explosion) // Создать взрыв на координатах.
		.addCFunction("set_status_engine", set_status_engine) // установить состояние двигателя авто.
		.addCFunction("player_defined", player_defined) // пед существует.
		.addCFunction("setclock", setclock) //  задать время.

		.addCFunction("arrested", arrested) // пед арестован?
		.addCFunction("lockstatus", lockstatus)// статус двери авто.
		.addCFunction("create_marker_pickup", create_marker_pickup)// создать маркер над пикапом.
		.addCFunction("create_obj", &createobj) // создать объект.

		.addCFunction("remove_obj", remove_obj) // удалить объект.
		.addCFunction("setobjоcoordes", setobjоcoordes) // установить координаты для объект.
		.addCFunction("getobjcoordes", getobjcoordes) // получить координаты объекта.
		.addCFunction("create_marker_obj", create_marker_obj) //создать маркер над объектом.

		.addCFunction("isobject", isobject) // проверка это объект?.
		.addCFunction("setpedangle", setpedangle) // установить угол педа.
		.addCFunction("setcaraction", setcaraction)// установить поведение авто.
		.addCFunction("move_obj", move_obj) //двигать объект.

		.addCFunction("move_rotate", move_rotate) //вращать объект.
		.addCFunction("getobjangle", getobjangle) // получить угол объекта.
		.addCFunction("findcar", findcar)//Найти авто.
		.addCFunction("setcartask", setcartask) // установить задачу авто.

		.addCFunction("setcarcoordes", setcarcoordes)// установить координаты авто.
		.addCFunction("is_car_stuck", is_car_stuck)//03CE: car 12@ stuck если машина застряла.
		.addCFunction("is_car_upsidedown", is_car_upsidedown)//01F4: car 12@ flipped если машина перевернута.
		.addCFunction("is_car_upright", is_car_upright) // 020D: car 12@ flipped если указанный автомобиль перевернут.

		.addCFunction("find_road_for_car", find_road_for_car) // найти дорогу.
		.addCFunction("setcarstrong", setcarstrong) // сделать авто устойчивым.
		.addCFunction("putincar", putincar)// переместить педа в авто.
		.addCFunction("print_front", game_font_print) // вывести особенный игровой текст.

		.addCFunction("star_timer", star_timer) // включить таймер.
		.addCFunction("stop_timer", stop_timer) // остановить таймер.
		.addCFunction("timer_donw", timer_donw) //  таймер на уменьшение.
		.addCFunction("ped_attack_car", ped_attack_car) // пед атакует авто.

		.addCFunction("ped_frozen", ped_frozen)  // заморозить игpока.
		.addCFunction("hold_cellphone", hold_cellphone) // поднять телефон.
		.addCFunction("car_lastweapondamage", car_lastweapondamage)// номер оружие, которое нанесло урон авто.
		.addCFunction("car_currentgear", car_currentgear) // текущая передача авто.

		.addCFunction("getcar_model", getcar_model) // получить модель авто.
		.addCFunction("setcarsiren", setcarsiren) // установить сирену для авто.
		.addCFunction("ped_car_as_driver", ped_car_as_driver) // пед садится в авто как водитель.
		.addCFunction("ped_car_as_passenger", ped_car_as_passenger) // пед садится в авто как пассажир.
		.addCFunction("ped_atack", ped_atack) // пед бьет.

		.addCFunction("show_text_gtx", show_text_gtx)// вывести игровой текст.
		.addCFunction("camera_at_point", camera_at_point) //переместить камеру в координатах.
		.addCFunction("restore_camera", restore_camera) // восстановить камеру.
		.addCFunction("is_wanted_level", is_wanted_level) // проверить уровень розыска.

		.addCFunction("set_camera_position", set_camera_position) //уст камеру в координатах.
		.addCFunction("flash_hud", flash_hud) // Мигание элементов HUD.
		.addCFunction("set_radio", set_radio) // уст радио.			
		.addCFunction("set_car_tires", set_car_tires)// проколоть шину.

		.addCFunction("create_spec_ped", create_spec_ped) // создать спец педа.
		.addCFunction("set_wheel_status", set_wheel_status)// уст состояния шин авто.
		.addCFunction("set_skin", set_skin) // уст скин педа.
		.addCFunction("remove_spec_ped", remove_spec_ped) // удалить спец педа.

		.addCFunction("go_to_route", go_to_route) //уст маршрут авто.
		.addCFunction("add_stuck_car_check", add_stuck_car_check) // условия для того, чтобы авто считалась застрявшей.
		.addCFunction("load_scene", load_scene)// загрузить модели на координатах заранее.
		.addCFunction("ped_anim", ped_anim)// анимация.

		.addCFunction("del_anim", del_anim)// удалить анимацию.
		.addCFunction("get_current_name_luascript", get_current_name_luascript)// получить имя текущего lua файла.
		.addCFunction("star_mission_marker", star_mission_marker) // создать маркер для миссии.
		.addCFunction("getobjcoordinates_on_x", getobjcoordinates_on_abscissa) // Получить мировую координату по x для объекта.

		.addCFunction("getobjcoordinates_on_y", getobjcoordinates_on_ordinate) // Получить мировую координату по y для объекта.
		.addCFunction("set_widescreen", set_widescreen)// вкл/выкл широкий экран.
		.addCFunction("follow_the_leader", follow_the_leader) //01DE / 01DF	следовать за лидером.
		.addCFunction("getcarspeed", getcarspeed) // получить скорость авто.

		.addCFunction("newthread", newthread)// запуск функции в новом потоке.
		.addCFunction("exitcar", exitcar);// название функции в lua и c++. выйти из авто.
};

void writelog(const char x[]) {// запись ошибок в файл.
	string path = "lualoader\\log.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[80]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %I:%M:%S %d-%m-%Y", timeinfo);
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};

wchar_t* getwchat(const char* c) {// перевод в строку.
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize]; mbstowcs(wc, c, cSize);
	return wc;// вернуть строку.
};

int findplayer(lua_State* L) {// получить указатель на педа.
	CPed* player = FindPlayerPed();// найти томми.
	lua_pushlightuserdata(L, player);// отправить в стек указатель на педа.
	return 1;
};

int cardrive(lua_State* L) {// авто едет в точку.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			CVehicle* vehicle = Stack<CVehicle*>::get(L, 1);// модель авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };
			Command<COMMAND_CAR_GOTO_COORDINATES>(vehicle, pos.x, pos.y, pos.z);// авто едет на координаты.
			return 0;
		}// int

		else { throw "bad argument in function cardrive"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int wait(lua_State* L) {
	static int delay = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int time = lua_tointeger(L, -1); // время задержки.
			this_thread::sleep_for(chrono::milliseconds(time));
			return 0;
		}// int
		if (LUA_TSTRING == lua_type(L, -1) || LUA_TBOOLEAN == lua_type(L, -1)) {
			throw "bad argument in function wait";
		}
		if (LUA_TBOOLEAN == lua_type(L, -1)) {
			throw "bad argument in function wait";
		}
		else {
			this_thread::sleep_for(chrono::milliseconds(1)); return 0;
			/*	delay = iters;
				while (true)
				{
				if (iters- delay >1){
					delay = 0;
					break;
				}
				}*/
		}
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setpedhealth(lua_State* L) {// установить здоровье педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа. 
			if (LUA_TNUMBER == lua_type(L, 2)) {
				const void* p = lua_topointer(L, 1);

				CPed* ped = findpedinpool(p);// получить указатель на педа.

				float health = lua_tonumber(L, 2);// если число.

				health += 0.99f; ped->m_fHealth = health;
				return 0;
				//}
			}// установить здоровье педа.
			else { throw "bad argument in function setpedhealth option health"; }
		}
		else { throw "bad argument in function setpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setarmour(lua_State* L) {// установить броню педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на педа.
			if (LUA_TNUMBER == lua_type(L, -1)) {
				const void* p = lua_topointer(L, -2);

				CPed* ped = findpedinpool(p);// получить указатель на педа.
				float armour = lua_tonumber(L, 2);
				armour += 0.10f; ped->m_fArmour = armour; return 0;
			}// установить броню педа.
			else { throw "bad argument in function setarmour option health"; }
		}
		else { throw "bad argument in function setarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int getpedarmour(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float armour = ped->m_fArmour;

			Stack<int>::push(L, armour);// отправить в стек.  
			return 1;
		}// получить броню педа.
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedhealth(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -2);

			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int health = ped->m_fHealth; // получить кол-во здоровья педа.
			Stack<int>::push(L, health);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function getpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getcarhealth(lua_State* L) { // получить кол-во здоровья авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.
			const void* p = lua_topointer(L, -1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int health = car->m_fHealth; // получить кол-во здоровья авто.
			Stack<int>::push(L, health);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function getpedhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int opendoorcar(lua_State* L) { // открыть дверь авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на авто.
			const void* p = lua_topointer(L, -2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			CAutomobile* automobile = reinterpret_cast<CAutomobile*>(car); // опять же, приведение типов. Т.к. мы будет юзать damageManager, нам нужно убедиться, что транспорт - это автомобиль (CAutomobile)

			int door = lua_tointeger(L, 2);
			switch (door) {
			case 0: {DoorsExample::EnableDoorEvent(automobile, BONNET); // 0 капот
				break;	 }
			case 1: {DoorsExample::EnableDoorEvent(automobile, BOOT); // 1 багажник
				break;	 }
			case 2: { DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_LEFT); // 2 левая передняя дверь
				break;	 }
			case 3: {DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_RIGHT); // 3 правая передняя дверь
				break;	 }
			case 4: {DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_LEFT); // 4 левая задняя дверь
				break;	 }
			case 5: {DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_RIGHT); // 5 правая задняя дверь
				break; }
			default: {}
			}
			return 0;
		}
		else { throw "bad argument in function opendoorcar option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setcarhealth(lua_State* L) {// установить здоровье авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -1)) {// здоровье авто.
				const void* p = lua_topointer(L, -2);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				float health = lua_tonumber(L, 2);// если число.
				health += 0.99f; car->m_fHealth = health; return 0;
			}// установить здоровье авто.
			else { throw "bad argument in function setcarhealth option health"; }
		}
		else { throw "bad argument in function setcarhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarangle(lua_State* L) {// установить угол авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -1)) {

				const void* p = lua_topointer(L, -2);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				float angle = lua_tonumber(L, 2);// если число.

				Command<COMMAND_SET_CAR_HEADING>(CPools::GetVehicleRef(car), angle);
				return 0;
			}
			else { throw "bad argument in function setcarangle option angle"; }
		}
		else { throw "bad argument in function setcarangle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setpedangle(lua_State* L) {// установить угол педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float angle = lua_tonumber(L, 2);// если число.
			CPed* player = FindPlayerPed();// найти педа.
			if (ped == player) {
				Command<COMMAND_SET_PLAYER_HEADING>(CWorld::PlayerInFocus, angle);
				return 0;
			}
			else {
				Command<COMMAND_SET_CHAR_HEADING>(CPools::GetPedRef(ped), angle);
				return 0;
			}
		}
		else { throw "bad argument in function setcarangle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setdrivingstyle(lua_State* L) {// установить стиль езды авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -1)) {

				const void* p = lua_topointer(L, -2);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				int style = lua_tointeger(L, 2);// если число.
				switch (style) {
				case 0: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS;
					break;	 }//	ОСТАНОВКА СТИЛЯ ВОЖДЕНИЯ ДЛЯ АВТОМОБИЛЕЙ; 
				case 1: {	car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_SLOW_DOWN_FOR_CARS;
					break;	 }// СТИЛЬ ВОЖДЕНИЯ ЗАМЕДЛИТЬ ДЛЯ АВТОМОБИЛЕЙ;,
				case 2: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_AVOID_CARS;
					break;	 }// СТИЛЬ ВОЖДЕНИЯ ИЗБЕГАЙТЕ АВТОМОБИЛЕЙ;,
				case 3: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_PLOUGH_THROUGH;
					break; }//	СТИЛЬ ВОЖДЕНИЯ ПЛУГ ЧЕРЕЗ;,
				case 4: {	car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS_IGNORE_LIGHTS;
					break; }//	СТИЛЬ ВОЖДЕНИЯ СТОП ДЛЯ АВТОМОБИЛЕЙ ИГНОРИРОВАТЬ ОГНИ;
				default: {}
				}
				return 0;
			}// установить стиль езды авто.
			else { throw "bad argument in function setdrivingstyle option style"; }
		}
		else { throw "bad argument in function setdrivingstyle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcaraction(lua_State* L) {// установить поведение авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -3)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {

				const void* p = lua_topointer(L, -3);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				int style = lua_tointeger(L, 2);// если число.
				unsigned int t = lua_tointeger(L, 3);// если число.
				unsigned int time = t * 10;

				for (int i = 0; i < CPools::ms_pVehiclePool->m_nSize; i++) {
					CVehicle* vehicle = CPools::ms_pVehiclePool->GetAt(i);
					if (vehicle == car) {

						if (style == 0) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_NONE, time);
							return 0;
						}
						if (style == 1) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_WAIT, time);
							return 0;
						}
						if (style == 2) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_REVERSE, time);
							return 0;
						}
						if (style == 3) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKETURNLEFT, time);
							return 0;
						}
						if (style == 4) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKETURNRIGHT, time);
							return 0;
						}
						if (style == 5) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKESTRAIGHT, time);
							return 0;
						}
						if (style == 6) {//влево.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNLEFT, time);
							return 0;
						}
						if (style == 7) {// вправо.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNRIGHT, time);
							return 0;
						}
						if (style == 8) {// вперед.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_GOFORWARD, time);
							return 0;
						}
						if (style == 9) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_SWERVELEFT, time);
							return 0;
						}
						if (style == 10) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_SWERVERIGHT, time);
							return 0;
						}
					}
				}
			}// установить стиль езды авто.
			else { throw "bad argument in function setcaraction"; }
		}
		else { throw "bad argument in function setcaraction option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarspeed(lua_State* L) {// установить скорость авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -1)) {

				const void* p = lua_topointer(L, -2);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				float speed = lua_tonumber(L, 2);// если число.
				Command<COMMAND_SET_CAR_CRUISE_SPEED>(CPools::GetVehicleRef(car), speed);
				return 0;
			}
			else { throw "bad argument in function setcarspeed option speed"; }
		}
		else { throw "bad argument in function setcarspeed option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcartask(lua_State* L) {// установить задачу авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на авто. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				const void* p = lua_topointer(L, -2);

				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				int task = lua_tointeger(L, 2);// если число.
				if (task == 0) {
					car->m_autoPilot.m_nCarMission = MISSION_NONE;
					return 0;
				}
				if (task == 1) {
					car->m_autoPilot.m_nCarMission = MISSION_CRUISE;
					return 0;
				}
				if (task == 2) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMPLAYER_FARAWAY;
					return 0;
				}
				if (task == 3) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMPLAYER_CLOSE;//ехать за педом
					return 0;
				}
				if (task == 4) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_FARAWAY;
					return 0;
				}
				if (task == 5) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_CLOSE;// подъехать к педу. 
					return 0;
				}
				if (task == 6) {//влево.
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_HANDBRAKESTOP;
					return 0;
				}
				if (task == 7) {// вправо.
					car->m_autoPilot.m_nCarMission = MISSION_WAITFORDELETION;
					return 0;
				}
				if (task == 8) {// вперед.
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS;
					return 0;
				}
				if (task == 9) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_STRAIGHT;
					return 0;
				}
				if (task == 10) {
					car->m_autoPilot.m_nCarMission = MISSION_EMERGENCYVEHICLE_STOP;
					return 0;
				}
				if (task == 11) {
					car->m_autoPilot.m_nCarMission = MISSION_STOP_FOREVER;
					return 0;
				}
				if (task == 12) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ACCURATE;
					return 0;
				}
				if (task == 13) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTO_COORDS_STRAIGHT_ACCURATE;
					return 0;
				}
				if (task == 14) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ASTHECROWSWIMS;
					return 0;
				}
				if (task == 15) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMCAR_FARAWAY;
					return 0;
				}
				if (task == 16) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMCAR_CLOSE;
					return 0;
				}
				if (task == 17) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_FARAWAY;
					return 0;
				}
				if (task == 18) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_CLOSE;
					return 0;
				}
				if (task == 19) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_HANDBRAKESTOP;
					return 0;
				}
				if (task == 20) {
					car->m_autoPilot.m_nCarMission = MISSION_HELI_FLYTOCOORS;
					return 0;
				}
				if (task == 21) {
					car->m_autoPilot.m_nCarMission = MISSION_ATTACKPLAYER;
					return 0;
				}
				if (task == 22) {
					car->m_autoPilot.m_nCarMission = MISSION_PLANE_FLYTOCOORS;
					return 0;
				}
				if (task == 23) {
					car->m_autoPilot.m_nCarMission = MISSION_HELI_LAND;
					return 0;
				}
				if (task == 24) {
					car->m_autoPilot.m_nCarMission = MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_1;
					return 0;
				}
				if (task == 25) {
					car->m_autoPilot.m_nCarMission = MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_2;
					return 0;
				}
				if (task == 26) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_FORWARDANDBACK;
					return 0;
				}
				return 0;
			}
			else { throw "bad argument in function setcartask option task"; }
		}
		else { throw "bad argument in function setcartask option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int key(lua_State* L) {// проверка нажата ли клавиша?
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			double key = lua_tonumber(L, -1);
			int x2 = (int)key;
			if (key == x2) {
				if (GetAsyncKeyState(key) == -32767) {
					lua_pushboolean(L, true);// клавиша нажата.
					return 1;
				}
				else {
					lua_pushboolean(L, false);
					return 1;
				}
			}
			else { throw "bad argument in function key"; }
		}
		else { throw "bad argument in function key"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int lockstatus(lua_State* L) {// статус двери авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на авто. 

			const void* p = lua_topointer(L, -2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int status = lua_tointeger(L, 2);// если число.
			car->m_nLockStatus = status;
			return 0;
		}
		else { throw "bad argument in function lockstatus"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int givemoney(lua_State* L) {
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {//кол-во денег.
			int money = lua_tonumber(L, -1);
			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += money;// дать денег  
		}
		else { throw "bad argument in function givemoney"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedcoordes(lua_State* L) {// получить координаты педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			Stack<double>::push(L, ped->GetPosition().x);// отправить в стек.
			Stack<double>::push(L, ped->GetPosition().y);// отправить в стек.
			Stack<double>::push(L, ped->GetPosition().z);// отправить в стек.
			return 3;
		}// получить координаты педа.

		else { throw "bad argument in function getpedcoordes option of the player"; }
	}
	catch (const char* x) { writelog(x); }
};
int getcarcoordes(lua_State* L) {// получить координаты авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			Stack<double>::push(L, car->GetPosition().x);// отправить в стек.
			Stack<double>::push(L, car->GetPosition().y);// отправить в стек.
			Stack<double>::push(L, car->GetPosition().z);// отправить в стек.
			return 3;
		}// получить координаты авто.

		else { throw "bad argument in function getcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }
};
int printmessage(lua_State* L) {// аргументы текст и и время вывода на экран.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//строка.
			const char* c = lua_tostring(L, -3);// строка.
			int time = lua_tointeger(L, -2);// время вывода текста.
			int style = lua_tointeger(L, -1);// стиль текста.
			wchar_t* str = getwchat(c);
			CMessages::AddMessageJumpQ(str, time, style);// вывести сообщение на экран.
			return 0;
		}
		else { throw "bad argument in function printmessage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int randomfindped(lua_State* L) {// найти педа в радиусе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {

			const void* p1 = lua_topointer(L, -2);

			CPed* p = findpedinpool(p1);//  получить указатель на педа.
			double radius = lua_tonumber(L, -1);
			CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;
			}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
							return 2;
						}
					}
				}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), p->GetPosition()) < radius && ped->m_fHealth > 50) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
					return 2;
				}
			}//    
			CPed* p2 = nullptr; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function randomfindped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};
int randomfindcar(lua_State* L) {//Найти случайное авто в радиусе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {

			const void* p1 = lua_topointer(L, -1);
			CPed* p = findpedinpool(p1);//  получить указатель на авто.

			double radius = lua_tointeger(L, 2);// радиус.
			CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;
			}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {

					lua_pushboolean(L, true); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 2;
				}
			}
		}
		else { throw "bad argument in function randomfindcar"; }
	}
	catch (const char* x) {
		writelog(x);// записать ошибку в файл.
	}
};
int findcar(lua_State* L) {//Найти авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {
			CVehicle* v = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// получить указатель на педа.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car == v) {
					lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 1;
				}
			}
		}
		else { throw "bad argument in function findcar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};
int findped(lua_State* L) {
	try {
		lua_newtable(L);
		luaL_newmetatable(L, "mt");
		for (auto ped : CPools::ms_pPedPool) {
			Stack<CPed*>::push(L, ped);
			Stack<CPed*>::push(L, ped);// отправить в стек и получить из стека можно
			lua_rawset(L, -3);
			luaL_setmetatable(L, "mt");
		}
		return 1;
	}

	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int incar(lua_State* L) {// пед в авто?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped->m_bInVehicle && ped->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = ped->m_pVehicle;
				lua_pushboolean(L, true);
				lua_pushlightuserdata(L, v);// отправить в стек true и указатель на авто.
				return 2;
			}
			else {
				CVehicle* v = NULL;//если пед не в авто вернуть null;
				lua_pushboolean(L, false);
				lua_pushlightuserdata(L, v);// отправить в стек и получить из стека можно
				return 2;// получить указатель на хенлд авто в котором сидит пед.
			}
		}
		else { throw "bad argument in function incar"; }
	}
	catch (const char* x) { writelog(x); }
};

int exitcar(lua_State* L) {// пед выходит из машины.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetObjective(OBJECTIVE_LEAVE_CAR);
			return 0;
		} // выйти из авто.
		else { throw "bad argument in function exitcar"; }
	}
	catch (const char* x) { writelog(x); }
};
int loadmodel(lua_State* L) {//Загрузка моделей.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число(модель).
			int model = lua_tointeger(L, -1);
			Command<COMMAND_REQUEST_MODEL>(model);
			return 0;
		}// int

		else { throw "bad argument in function loadmodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int releasemodel(lua_State* L) {// Удалить модель из памяти.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int model = lua_tointeger(L, -1);
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			return 0;
		}// int
		else { throw "bad argument in function releasemodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int availablemodel(lua_State* L) {// проверка на загруженность модели.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int model = lua_tointeger(L, -1);
			bool ava = Command<COMMAND_HAS_MODEL_LOADED>(model);
			lua_pushboolean(L, ava);
			return 1;
		}// int

		else { throw "bad argument in function availablemodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int createcar(lua_State* L) {// создать авто на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3)
			&& LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int model = lua_tointeger(L, -4);// модель авто.
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1); CVector pos = { x, y, z };
			CVehicle* vehicle = nullptr;
			Command<COMMAND_CREATE_CAR>(model, pos.x, pos.y, pos.z, &vehicle);
			mapcars.emplace(vehicle, L);// добавить в map для авто.
			lua_pushlightuserdata(L, vehicle);// отправить в стек указатель на авто.
			return 1;
		}// int

		else { throw "bad argument in function createcar"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createobj(lua_State* L) {// создать объект.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int model = lua_tointeger(L, -4);// модель авто.
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1); CVector pos = { x, y, z };
			CObject* obj = nullptr;
			Command<COMMAND_CREATE_OBJECT>(model, pos.x, pos.y, pos.z, &obj);
			int obj1 = (int)& obj;
			mapobjs.emplace(obj, L);// добавить в map для авто.
			lua_pushlightuserdata(L, obj);// отправить в стек указатель на объект.
			return 1;
		}// int

		else { throw "bad argument in function createobj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_actor(lua_State* L) {//создать маркер над педом.
	int marker;// храним id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			Command<COMMAND_ADD_BLIP_FOR_CHAR>(CPools::GetPedRef(ped), &marker);
			markeron.emplace(marker, L);// добавить в map для маркеров.
			Stack<int>::push(L, marker);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function create_marker_actor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_car(lua_State* L) {//создать маркер над авто.
	int marker;// храним id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			Command<COMMAND_ADD_BLIP_FOR_CAR>(CPools::GetVehicleRef(car), &marker);

			markeron.emplace(marker, L);// добавить в map для маркеров.
			Stack<int>::push(L, marker);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function create_marker_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int removemarker(lua_State* L) {// удалить маркер.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int marker = lua_tointeger(L, -1);// получить id маркера.
			Command<COMMAND_REMOVE_BLIP>(marker);
			return 0;
		}
		else { throw "bad argument in function removemarker option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int ped_sprint_to_point(lua_State* L) {// пед делает спринт к точке.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -4);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1); CVector pos = { x, y, z };// вектор для координат.
			ped->SetObjective(OBJECTIVE_SPRINT_TO_AREA, pos);// пед делает спринт к точке.
		}
		else { throw "bad argument in function ped_run_to_point"; }
	}
	catch (const char* x) { writelog(x); }
};
int ped_walk_to_point(lua_State* L) {// пед идет пешком.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -4);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float x = lua_tonumber(L, -3);
			float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);// пед идет пешком.
		}
		else { throw "bad argument in function ped_walk_to_point"; }
	}
	catch (const char* x) { writelog(x); }
};
int getobjangle(lua_State* L) {// получить угол объекта.
	try {
		double angle;
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на объект.

			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.
			Command<COMMAND_GET_OBJECT_HEADING>(CPools::GetObjectRef(obj), angle);
			Stack<double>::push(L, angle);// отправить в стек.
			return 1;
		}// получить угол объекта.

		else { throw "bad argument in function getobjangle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedangle(lua_State* L) {// получить угол педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float angle;// переменная хранить угол педа.
			CPed* player = FindPlayerPed();// найти педа.
			if (ped == player) {
				Command<COMMAND_GET_PLAYER_HEADING>(CWorld::PlayerInFocus, &angle);//  получить угол педа.
				Stack<int>::push(L, angle);// отправить в стек.  
				return 1;
			}
			else {
				Command<COMMAND_GET_CHAR_HEADING>(CPools::GetPedRef(ped), &angle);//  получить угол педа.
				Stack<int>::push(L, angle);// отправить в стек.  
				return 1;
			}
		}
		else { throw "bad argument in function getpedangle option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int getpedcoordinates_on_abscissa(lua_State* L) {// Получить мировую координату по x для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float x = lua_tonumber(L, -1);
			CVector pos = ped->m_placement.pos;
			pos += ped->m_placement.right * x;
			Stack<float>::push(L, pos.x); Stack<float>::push(L, pos.y); Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getpedcoordinates_on_x"; }
	}
	catch (const char* x) { writelog(x); }
};
int getpedcoordinates_on_ordinate(lua_State* L) {// // Получить мировую координату по y для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			float y = lua_tonumber(L, -1);
			CVector pos = ped->m_placement.pos;
			pos += ped->m_placement.up * y;   Stack<float>::push(L, pos.x);
			Stack<float>::push(L, pos.y);   Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getpedcoordinates_on_y"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getcarcoordinates_on_abscissa(lua_State* L) {// Получить мировую координату по x для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			float x = lua_tonumber(L, -1);
			CVector pos = car->m_placement.pos;
			pos += car->m_placement.right * x;
			Stack<float>::push(L, pos.x); Stack<float>::push(L, pos.y); Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_abscissa"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getcarcoordinates_on_ordinate(lua_State* L) {// // Получить мировую координату по y для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			float y = lua_tonumber(L, -1);
			CVector pos = car->m_placement.pos;
			pos += car->m_placement.up * y;   Stack<float>::push(L, pos.x);
			Stack<float>::push(L, pos.y);   Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_y"; }
	}
	catch (const char* x) { writelog(x); }
};

int worldcoord(lua_State* L) {// Перевод в мировые координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -3);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			float x = lua_tonumber(L, -2); float y = lua_tonumber(L, -1);
			CVector pos = ped->m_placement.pos + ped->m_placement.right * x + ped->m_placement.up * y;
			Stack<float>::push(L, pos.x);   Stack<float>::push(L, pos.y);
			return 2;
		}
		else { throw "bad argument in function worldcoord"; }
	}
	catch (const char* x) { writelog(x); }
};
int load_requested_models(lua_State* L) {// Загрузка модели в не очереди.
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	return 0;
};
int giveweaponped(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -4)) {// указатель на педа.
			if (LUA_TNUMBER == lua_type(L, -1) && (LUA_TNUMBER == lua_type(L, -2))) {
				unsigned int model = Stack<unsigned int>::get(L, -3);// модель оружие.
				unsigned int WEAPONTYPE = Stack<unsigned int>::get(L, -2);// тип оружи.
				int ammo = lua_tointeger(L, -1);// число патронов.

				const void* p = lua_topointer(L, -4);
				CPed* ped = findpedinpool(p);// получить указатель на педа.
				CPed* player = FindPlayerPed();// найти педа
				if (ped == player) { Command<COMMAND_GIVE_WEAPON_TO_PLAYER>(CWorld::PlayerInFocus, WEAPONTYPE, ammo); }
				else {
					Command<COMMAND_GIVE_WEAPON_TO_CHAR>(CPools::GetPedRef(ped), WEAPONTYPE, ammo);// Дать оружие педу.
				}
			}
			else { throw "bad argument in function giveweaponped option weapons"; }
		}
		else { throw "bad argument in function giveweaponped option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_ped_on_foot(lua_State* L) {
	static int numberped;// счетчик педов для реализация атаки.
	static CPed* pedfoe;// хранить указатель врага.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			if (numberped != 1) {
				numberped = 1; // увеличить номер педа, чтобы работать с 2.

				const void* p = lua_topointer(L, -1);
				CPed* ped = findpedinpool(p);// получить указатель на педа.
				pedfoe = ped;
				return 0;
			};
			if (numberped == 1) {
				const void* p = lua_topointer(L, -1);
				CPed* ped2 = findpedinpool(p);// получить указатель на педа.
				pedfoe->SetObjective(OBJECTIVE_KILL_CHAR_ON_FOOT, ped2);
				numberped = NULL;
				return 0;
			}
		}
		else { throw "bad argument in function kill_ped_on_foot option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_char_any_means(lua_State* L) {
	static int numberped;;// счетчик педов для реализация атаки.
	static CPed* pedfoe;// хранить указатель врага.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			if (numberped != 1) {
				numberped = 1; // увеличить номер педа, чтобы работать с 2.
				const void* p = lua_topointer(L, -1);
				CPed* ped = findpedinpool(p);// получить указатель на педа.
				pedfoe = ped;
				return 0;
			};
			if (numberped == 1) {
				const void* p = lua_topointer(L, -1);
				CPed* ped2 = findpedinpool(p);// получить указатель на педа.
				pedfoe->SetObjective(OBJECTIVE_KILL_CHAR_ANY_MEANS, ped2);
				numberped = NULL;
				return 0;
			}
		}
		else { throw "bad argument in function kill_char_any_means option of the ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int ped_aim_at_ped(lua_State* L) {//Пед целиться в педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1) && LUA_TLIGHTUSERDATA == lua_type(L, -2)) {// указатель на педа.

			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			const void* p1 = lua_topointer(L, -1);
			CPed* ped2 = findpedinpool(p1);// получить указатель на педа.
			ped->SetObjective(OBJECTIVE_AIM_GUN_AT, ped2);// заставить педа целиться в другого педа.
			return 0;
		}
		else { throw "bad argument in function ped_aim_at_ped option of the ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_current_weapon_ped(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {// указатель на педа.
			unsigned int weapon_type = Stack<unsigned int>::get(L, -1);// тип оружие.

			const void* p = lua_topointer(L, -2);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CPed* player = FindPlayerPed();// найти педа
			if (ped != player) {
				bool charweapontype = Command<COMMAND_IS_CURRENT_CHAR_WEAPON>(CPools::GetPedRef(ped), weapon_type);
				lua_pushboolean(L, charweapontype);
				return 1;
			}
			else {
				bool playerweapontype = Command<COMMAND_IS_CURRENT_PLAYER_WEAPON>(CWorld::PlayerInFocus, weapon_type);
				lua_pushboolean(L, playerweapontype);// отправить булевое значение сравнением с текущим оружие педа.
				return 1;
			}
		}
		else { throw "bad argument in function is_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_sphere(lua_State* L) {//создать сферу.
	try { 
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  float radius = lua_tonumber(L, 4);// радиус.
			int sphere;// переменная, которая хранить id сферы.  
			CVector pos = { x, y, z };
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere); //создать, удалить, создать сферу  
			Command<COMMAND_REMOVE_SPHERE>(sphere);// нужно, чтобы обойти глюк.
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere);
			lua_settop(L, 0);// очистить стек.
			spheres.emplace(sphere, L);
			Stack<int>::push(L, sphere);// отправить id сферы в стек.  
			return 1;
		}
		else { throw "bad argument in function create_sphere "; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_money_pickup(lua_State* L) {//создать пачку денег.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int money = lua_tointeger(L, 1); // количество денег в пикапе 
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);
			int idpickup;// переменная, которая хранить id пикапа денег.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_MONEY_PICKUP>(pos.x, pos.y, pos.z, money, &idpickup);
			pickupsids.emplace(idpickup, L);
			Stack<int>::push(L, idpickup);// отправить id пикапа в стек.  
			return 1;
		}
		else { throw "bad argument in function create_money_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_weapon_pickup(lua_State* L) {//создать пикап оружие.
	try {
		if (LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5)
			&& LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int model = lua_tointeger(L, 1);// модель оружие. 
			int type = lua_tointeger(L, 2);// тип пикапа. 
			int ammo = lua_tointeger(L, 3); // начальное кол-во патронов в оружие.
			float x = lua_tonumber(L, 4);
			float y = lua_tonumber(L, 5); float z = lua_tonumber(L, 6);
			int idpickup;// переменная, которая хранить id пикапа.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP_WITH_AMMO>(model, type, ammo, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			Stack<int>::push(L, idpickup);// отправить id пикапа в стек.  
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_pickup(lua_State* L) {//создать пикап.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int model = lua_tointeger(L, 1); // модель пикапа. 
			int type = lua_tointeger(L, 2);// тип пикапа. 
			float x = lua_tonumber(L, 3);	float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);
			int idpickup;// переменная, которая хранить id пикапа.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP>(model, type, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			Stack<int>::push(L, idpickup);// отправить id пикапа в стек.  
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_sphere(lua_State* L) {// удалить сферу.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int sphere = lua_tointeger(L, 1);
			Command<COMMAND_REMOVE_SPHERE>(sphere);// удалить сферу.
			return 0;
		}
		else { throw "bad argument in function remove_sphere"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int picked_up(lua_State* L) {// проверка пикап подобран.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение id пикапа.
			int pickup = lua_tointeger(L, 1);
			bool checkpickup = Command<COMMAND_HAS_PICKUP_BEEN_COLLECTED>(pickup);
			lua_pushboolean(L, checkpickup);
			return 1;
		}
		else { throw "bad argument in function picked_up"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_pickup(lua_State* L) {// удалить пикап.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение id пикапа.
			int pickup = lua_tointeger(L, 1);
			Command<COMMAND_REMOVE_PICKUP>(pickup);// удалить пикап.
			return 0;
		}
		else { throw "bad argument in function remove_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_car(lua_State* L) {// удалить авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			return 0;
		}
		else { throw "bad argument in function remove_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_obj(lua_State* L) {// удалить объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение объект.
			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			if (obj != NULL) {//obj->Remove();
				Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
			}
			return 0;
		}
		else { throw "bad argument in function remove_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int car_in_water(lua_State* L) {// проверка авто в воде.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто..

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_in_wanter = Command<COMMAND_IS_CAR_IN_WATER>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_in_wanter);
			return 1;
		}
		else { throw "bad argument in function car_in_water"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_wanted(lua_State* L) {// уcтановить уровень розыска.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int wanted = lua_tointeger(L, -1);// кол-во звезд розыска.
			if (wanted < 5) {
				Command<COMMAND_ALTER_WANTED_LEVEL>(CWorld::PlayerInFocus, wanted);
				return 0;
			}
			else
			{
				Command<COMMAND_ALTER_WANTED_LEVEL_NO_DROP>(CWorld::PlayerInFocus, wanted);
				return 0;
			}
		}
		else { throw "bad argument in function set_wanted"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_widescreen(lua_State* L) {// вкл/выкл широкий экран.
	try {
		if (LUA_TBOOLEAN == lua_type(L, -1)) {// значение булевое.
			bool switc = lua_toboolean(L, -1);// 
			Command<COMMAND_SWITCH_WIDESCREEN>(switc);
			return 0;
		}
		else { throw "bad argument in function set_widescreen"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_wanted_level(lua_State* L) {// проверить уровень розыска.
	static int delay = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int wanted = lua_tointeger(L, 1); wanted--;
			if (delay == 0) {
				delay = iters;
				lua_pushboolean(L, false);
				return 1;
			}
			else {
				if (iters - delay > 5) {
					delay = 0;
					lua_pushboolean(L, Command<COMMAND_IS_WANTED_LEVEL_GREATER>(CWorld::PlayerInFocus, wanted));
					return 1;
				}
			}
		}
		else { throw "bad argument in function is_wanted_level"; }
	}
	catch (const char* x) { writelog(x); }

	return 0;
};
int clear_wanted(lua_State* L) {// убрать уровень розыска.
	Command<COMMAND_CLEAR_WANTED_LEVEL>(CWorld::PlayerInFocus);
	return 0;
};

int remove_ped(lua_State* L) {// удалить педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearInvestigateEvent();// пед уходит, опустить педа.
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
			return 0;
		}
		else { throw "bad argument in function remove_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_spec_ped(lua_State* L) {// удалить спец педа.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение пед.

			int idped = lua_tointeger(L, 1);
			CStreaming::SetMissionDoesntRequireSpecialChar(idped); // 0296: unload_special_actor 21 
			return 0;
		}
		else { throw "bad argument in function remove_spec_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_ped(lua_State* L) {// убить педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			Command<COMMAND_EXPLODE_CHAR_HEAD>(CPools::GetPedRef(ped));// убить педа.
			return 0;
		}
		else { throw "bad argument in function kill_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setpedcoordes(lua_State* L) {// установить координаты для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//число.

			const void* p = lua_topointer(L, -4);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CPed* player = FindPlayerPed();// найти педа  
			if (ped != player) {
				Command<COMMAND_SET_CHAR_COORDINATES>(CPools::GetPedRef(ped), x, y, z);
				return 0;
			}
			else {
				Command<COMMAND_SET_PLAYER_COORDINATES>(CWorld::PlayerInFocus, x, y, z);
				return 0;
			}
		}
		else { throw "bad argument in function setpedcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int setobjоcoordes(lua_State* L) {// установить координаты для объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//указатель на объект и координаты.

			const void* p = lua_topointer(L, -4);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_SET_OBJECT_COORDINATES>(CPools::GetObjectRef(obj), x, y, z);
			return 0;
		}
		else { throw "bad argument in function setobjоcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int create_marker_obj(lua_State* L) {//создать маркер над объектом.
	int marker;// id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на объект.
			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.

			Command<COMMAND_ADD_BLIP_FOR_OBJECT>(CPools::GetObjectRef(obj), &marker);//создать маркер над объектом.
			markeron.emplace(marker, L);// добавить в map для маркеров.
			Stack<int>::push(L, marker);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function create_marker_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int move_obj(lua_State* L) {//двигать объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			float speedx = lua_tonumber(L, 5);//
			float speedy = lua_tonumber(L, 6);// скорости по осам.
			float speedz = lua_tonumber(L, 7);//
			int collision_check = lua_tointeger(L, 8);// вкл/выкл коллизию объекта.
			Command<COMMAND_SLIDE_OBJECT>(CPools::GetObjectRef(obj), x, y, z, speedx, speedy, speedz, collision_check);

			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int move_rotate(lua_State* L) {//вращать объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			lua_settop(L, 4);

			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.
			float Angle1 = lua_tonumber(L, 2); float Angle2 = lua_tonumber(L, 3);// углы вращения объекта.
			int flag = lua_tointeger(L, 4);
			Command<COMMAND_ROTATE_OBJECT>(CPools::GetObjectRef(obj), Angle1, Angle1, flag);

			lua_settop(L, 0);
			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int getobjcoordes(lua_State* L) {// получить координаты объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.

			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			Stack<double>::push(L, obj->GetPosition().x);// отправить в стек.
			Stack<double>::push(L, obj->GetPosition().y);// отправить в стек.
			Stack<double>::push(L, obj->GetPosition().z);// отправить в стек.
			return 3;
		}// получить координаты автоа.

		else { throw "bad argument in function getcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }
};

int getobjcoordinates_on_abscissa(lua_State* L) {// Получить мировую координату по x для объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на объекта.

			const void* p = lua_topointer(L, -2);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			float x = lua_tonumber(L, -1);
			CVector pos = obj->m_placement.pos;
			pos += obj->m_placement.right * x;
			Stack<float>::push(L, pos.x); Stack<float>::push(L, pos.y); Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getobjcoordinates_on_abscissa"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};

int getobjcoordinates_on_ordinate(lua_State* L) {// Получить мировую координату по y для объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// указатель на объект.

			const void* p = lua_topointer(L, -2);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			float y = lua_tonumber(L, -1);
			CVector pos = obj->m_placement.pos;
			pos += obj->m_placement.up * y; Stack<float>::push(L, pos.x);
			Stack<float>::push(L, pos.y);   Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getobjcoordinates_on_ordinate"; }
	}
	catch (const char* x) { writelog(x); }
};

int ped_in_point_in_radius(lua_State* L) {// проверить находится пед в координатах с радиусом.	
	static int delay = 0;
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -7) && LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5) &&
			LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//строка.

			const void* p = lua_topointer(L, -7);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			float x1 = lua_tonumber(L, 2);	float y1 = lua_tonumber(L, 3);	float z1 = lua_tonumber(L, 4);
			float rx = lua_tonumber(L, 5);	float ry = lua_tonumber(L, 6);	float rz = lua_tonumber(L, 7);

			this_thread::sleep_for(chrono::milliseconds(10));
			double x = ped->GetPosition().x;
			double y = ped->GetPosition().y;
			double z = ped->GetPosition().z;
			double r2 = rx * rx + ry * ry + rz * rz;
			x = x - x1; y = y - y1; z = z - z1;
			double res = x * x + y * y + z * z;
			if (res < r2) {

				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function ped_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int car_in_point_in_radius(lua_State* L) {// проверить находится авто в координатах с радиусом.
	static int delay = 0;
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3)
			&& LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//строка.

			const void* p = lua_topointer(L, -5);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			float radius = lua_tonumber(L, 5);// радиус.
			lua_pop(L, lua_gettop(L));
			lua_pushboolean(L, car->IsSphereTouchingVehicle(x, y, z, radius));
			//lua_pushboolean(L, Command<COMMAND_LOCATE_CAR_3D>(CPools::GetVehicleRef(car), x, y, z, rx, ry, rz));
			return 1;
			//	.}
		//	}
		}
		else { throw "bad argument in function car_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

void dellod() {// удалить лог ошибок.
	string path = "lualoader\\log.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	if (f1.is_open()) {// если файл есть удалить.
		f1.close();	remove("lualoader\\log.txt");
	}
	}
}
int cleanstl() {//удаления объектов из всех stl.
	if (!markeron.empty()) {// если не пусть.
		markeron.clear();// маркеры
	}
	if (!spheres.empty()) {// если не пусть.
		spheres.clear();// сферы.
	}
	if (!pickupsids.empty()) {// если не пусть.
		pickupsids.clear();//пикапы.
	}
	if (!mapcars.empty()) {// если не пусть.
		mapcars.clear();//авто.
	}
	if (!mapobjs.empty()) {// если не пусть.
		mapobjs.clear();//объект.
	}

	return 0;
};

int getflagmission(lua_State* L) {// проверка флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);	
	lua_pushboolean(L, getflagmission);// получить флаг миссии.
	return 1;
};
int setflagmission(lua_State* L) {// уcтановить флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	try {
		if (LUA_TBOOLEAN == lua_type(L, -1)) {// значение число.
			bool flag = lua_toboolean(L, -1);
			CTheScripts::ScriptSpace[OnAMissionFlag] = flag;
			return 0;
		}
		else { throw "bad argument in function setflagmission"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int showtext(lua_State* L) {// Вывод особого текста на экран.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//строка.
			const char* c = lua_tostring(L, -3);
			int time = lua_tointeger(L, -2);// время вывода текста.
			int style = lua_tointeger(L, -1);// стиль текста.
			wchar_t* str = getwchat(c);
			CMessages::AddBigMessage(str, time, style);/*0 большими, розовые как миссии пройдена,
			1 надпись как названия миссии, 2 зеленым большими*/
			return 0;
		}
		else { throw "bad argument in function showtext"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_blip(lua_State* L) {// удалить метку с карты.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int blip = lua_tointeger(L, -1);// получить id метки.
			Command<COMMAND_REMOVE_BLIP>(blip);// удалить метку на карте.
			return 0;
		}
		else { throw "bad argument in function remove_blip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createblip(lua_State* L) {// создать метку карте.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int point;// id метке.
			int t = lua_tointeger(L, -4);// id метки на карте.
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1); CVector p = { x, y, z };
			Command<COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT>(p.x, p.y, p.z, t, &point);
			Stack<int>::push(L, point);// отправить в стек и получить из стека можно
			markeron.emplace(point, L);// добавить в map для маркеров.
			return 1;
		}// int

		else { throw "bad argument in function createblip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createmarker(lua_State* L) {// создать маркер на карте.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int point; //id маркера.
			int t = lua_tointeger(L, 1);// тип маркер.
			int size = lua_tointeger(L, 2);// размер маркера на карте.
			float x = lua_tonumber(L, 3); float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5); CVector p = { x, y, z };
			Command<COMMAND_ADD_BLIP_FOR_COORD_OLD>(p.x, p.y, p.z, t, size, &point);
			markeron.emplace(point, L);// добавить в map для маркеров.
			Stack<int>::push(L, point);// отправить в стек и получить из стека можно
			return 1;
		}// int

		else { throw "bad argument in function createmarker"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int play_sound(lua_State* L) {// проиграть мелодию.
	try { 
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int music = lua_tointeger(L, -1);// получить id мелодии.
			Command<COMMAND_PLAY_MISSION_PASSED_TUNE>(music);
			return 0;
		}
		else { throw "bad argument in function play_sound"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_pickup(lua_State* L) {// создать маркер над пикапом.
	try {
		int marker;//id маркера.
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int pickup = lua_tointeger(L, 1);// получить id пикапа.
			Command<COMMAND_ADD_BLIP_FOR_PICKUP>(pickup, &marker);
			markeron.emplace(marker, L);// добавить в map для маркеров.
			Stack<int>::push(L, marker);// отправить в стек и получить из стека можно
			return 1;
		}
		else { throw "bad argument in function play_sound"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int play_voice(lua_State* L) {// проиграть реплику педа.
	try {
		if (LUA_TSTRING == lua_type(L, -1)) {// значение число.
			const char* voice = Stack<const char*>::get(L, -1);
			Command<COMMAND_LOAD_MISSION_AUDIO>(1, voice);// загрузить реплику.
			while (true) {
				this_thread::sleep_for(chrono::milliseconds(1));
				if (Command<COMMAND_HAS_MISSION_AUDIO_LOADED>(1)) {
					Command<COMMAND_PLAY_MISSION_AUDIO>(1);
					if (Command<COMMAND_HAS_MISSION_AUDIO_FINISHED>(1)) {
						Command<COMMAND_CLEAR_MISSION_AUDIO>(1);
						break;
					}
				}
			}
			return 0;
		}
		else { throw "bad argument in function play_voice"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createped(lua_State* L) {// создать педа.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			int model = lua_tointeger(L, -5);// модель педа
			int type = lua_tointeger(L, -4);// тип педа.
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);	float z = lua_tonumber(L, -1);
			CVector pos = { x, y, z };
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR>(type, model, pos.x, pos.y, pos.z, &ped);
			mappeds.emplace(ped, L);// добавить map для педов.
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно.
			return 1;
		}// int

		else { throw "bad argument in function createped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int create_spec_ped(lua_State* L) {// создать спец педа.
	try {
		if (LUA_TSTRING == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			CPed* ped;
			//int idmodel = lua_tointeger(L, -7);// спец id пед.
			//int specmodel = lua_tointeger(L, -6);// модель педа.
			//int type = lua_tointeger(L, -5);// тип педа.
			//int slot  = lua_tointeger(L, -4);// слот педа.
			char const* model = lua_tostring(L, -4);// модель, например, "sam".
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1);
			int idped = 0;
			Command<COMMAND_REQUEST_MODEL>(idped);
			while (!Command<COMMAND_HAS_MODEL_LOADED>(idped)) {

				this_thread::sleep_for(chrono::milliseconds(1));
			}
			Command<COMMAND_CREATE_CHAR>(4, idped, x, y, z, &ped);
			CPed* p = findpedinpool(ped);// получить указатель на педа.
			Command<COMMAND_UNDRESS_CHAR>(CPools::GetPedRef(p), model);
			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_DRESS_CHAR>(CPools::GetPedRef(p));
			lua_pushlightuserdata(L, p);// отправить в стек и получить из стека можно.
			return 1;
			//	}// int
		}
		else { throw "bad argument in function create_spec_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isped(lua_State* L) {// проверка это пед?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.
			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			if (ped != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isvehicle(lua_State* L) {// проверка это транспорт?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.
			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);// получить указатель на педа.

			if (car != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}

		}
		else { throw "bad argument in function isvehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isobject(lua_State* L) {// проверка это объект?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.
			const void* p = lua_topointer(L, -1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			if (obj != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isobject"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int fade(lua_State* L) {//затенение, просветления.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {
			int lihgt = lua_tointeger(L, 1);// Время.
			int time = lua_tointeger(L, 2);// свет = 1 тьма = 0.
			Command<COMMAND_DO_FADE>(time, lihgt);// затенение.
			return 0;
		}
		else { throw "bad argument in function fade"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setsizemarker(lua_State* L) {// установить размер маркера.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {
			int marker = lua_tointeger(L, 1);// Время.
			int size = lua_tointeger(L, 2);// размер маркера.
			Command<COMMAND_CHANGE_BLIP_SCALE>(marker, size);
			return 0;
		}
		else { throw "bad argument in function setsizemarker"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int draw_corona(lua_State* L) {// создать корону.
	try {
		if (LUA_TTABLE == lua_type(L, -1)) {// получаем таблицу из 9 элементов.
			for (int i = 1; i < 11; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2); lua_insert(L, i);
			}

			bool switc = Stack<bool>::get(L, 1);// вкл\выкл корону.
			double radius = Stack<double>::get(L, 2);// радиус короны.
			int type = lua_tointeger(L, 3);// тип.
			int glow_flare = lua_tointeger(L, 4); // свечение. 
			int red = lua_tointeger(L, 5);// цвета 
			int green = lua_tointeger(L, 6);
			int blue = lua_tointeger(L, 7);
			double x = Stack<double>::get(L, 8); // координаты.
			double y = Stack<double>::get(L, 9);
			double z = Stack<double>::get(L, 10);
			lua_pop(L, lua_gettop(L));

			corona::set(switc, x, y, z, radius, type, glow_flare, red, blue, green);

			/*	CVector pos = { x,y,z };

				Command<COMMAND_DRAW_CORONA>(pos.x, pos.y, pos.z, radius, type, glow_flare, red, green, blue);*/
			return 0;
		}// int

		else { throw "bad argument in function draw_corona"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int sound_coordinate(lua_State* L) {// Проиграть звук в координатах
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.

			int sound = lua_tointeger(L, 1);// id звука.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_ADD_ONE_OFF_SOUND>(x, y, z, sound);// Проиграть звук в координатах
			lua_settop(L, 0);
			return 0;
		}// int

		else { throw "bad argument in function sound_coordinate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int load_scene(lua_State* L) {// загрузить модели на координатах заранее.
	try {
		if (LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// значение число.

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			Command<COMMAND_LOAD_SCENE>(x, y, z);// Проиграть звук в координатах
			lua_settop(L, 0);
			return 0;
		}// int

		else { throw "bad argument in function load_scene"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int show_text_styled(lua_State* L) {// вывести игровой текст.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			const char* text = Stack<const char*>::get(L, 1);// текст.
			int time = lua_tointeger(L, 2);	int type = lua_tointeger(L, 3);
			Command<COMMAND_PRINT_BIG>(text, time, type);// числа для старта гонки.
			lua_settop(L, 0);// очистить стек.	
			return 0;
		}

		else { throw "bad argument in function show_text_styled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int show_text_gtx(lua_State* L) {// вывести игровой текст.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			const char* text = Stack<const char*>::get(L, 1);// текст.
			int time = lua_tointeger(L, 2);	int type = lua_tointeger(L, 3);
			Command<COMMAND_LOAD_MISSION_TEXT>("GENERA1");
			Command<COMMAND_PRINT_NOW>(text, time, type);// числа для старта гонки.
			lua_settop(L, 0);// очистить стекю	
			return 0;
		}

		else { throw "bad argument in function show_text_gtx"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcardrive(lua_State* L) {// установить водителя для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//строка.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int model = lua_tointeger(L, 2);// модель педа.
			int type = lua_tointeger(L, 3);// тип педа.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_INSIDE_CAR>(CPools::GetVehicleRef(car), type, model, &ped);
			car->m_autoPilot.m_nCarMission = MISSION_NONE;
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно.
			return 1;
		}
		else { throw "bad argument in function setcardrive"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int setcarpassenger(lua_State* L) {// установить пассажира для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 3)) {//число.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int model = lua_tointeger(L, 2);
			int type = lua_tointeger(L, 3);
			int place = lua_tointeger(L, 4);//место пассажира.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_AS_PASSENGER>(CPools::GetVehicleRef(car), type, model, place, &ped);
			lua_pushlightuserdata(L, ped);
			return 1;
		}
		else { throw "bad argument in function setcarpassenger"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int setcarfirstcolor(lua_State* L) {// установить первый цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int firstcolor = lua_tointeger(L, 2);
			car->m_nPrimaryColor = firstcolor;// установить первый цвет авто.
			return 0;
		}
		else { throw "bad argument in function setcarfirstcolor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarseconscolor(lua_State* L) {// установить второй цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int secondcolor = lua_tointeger(L, 2);
			car->m_nSecondaryColor = secondcolor;
			return 0;
		}// установить второй цвет авто.

		else { throw "bad argument in function setcarseconscolor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_traffic(lua_State* L) {// установить плотномть трафика транспорта.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// значение число.
			float trafic = lua_tonumber(L, -1);
			Command<COMMAND_SET_CAR_DENSITY_MULTIPLIER>(trafic);
			return 0;
		}
		else { throw "bad argument in function set_traffic"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_explode(lua_State* L) {// взрывать авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто. 

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			Command<COMMAND_EXPLODE_CAR>(CPools::GetVehicleRef(car));
			return 0;
		}
		else { throw "bad argument in function car_explode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_stopped(lua_State* L) {// авто остановилось? 
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто. 

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			lua_pushboolean(L, Command<COMMAND_IS_CAR_STOPPED>(CPools::GetVehicleRef(car)));
			return 1;
		}
		else { throw "bad argument in function is_car_stopped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setclock(lua_State* L) {//  задать время.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//числа.

			int hours = lua_tointeger(L, 1);// часы.
			int minutes = Stack< int>::get(L, 2);// минуты.

			CClock::SetGameClock(hours, minutes);// задать время.
			return 0;
		}
		else { throw "bad argument in function setclock"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
}

int create_explosion(lua_State* L) {// Создать взрыв на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//число.

			int tipe = lua_tointeger(L, 1);// тип взрыва.
			double x = Stack<double>::get(L, 2);
			double y = Stack<double>::get(L, 3);
			double z = Stack<double>::get(L, 4);
			/*
			0 Стандартный средний взрыв, используется чаще всего Highslide JS
			1 Взрыв как от коктейля Молотова. По-этому после него появлятся много огня и звук тихий. Следует также учитывать то, что именно этот взрыв визуально появляется вне зависимости высоты у самой земли. Highslide JS
			2 Похож на тип 0 Highslide JS
			3 Похож на тип 0 Highslide JS
			4 Один-три взрывных линий. Следует иметь в виду, что в типах 4 и 5 не звучит грохот взрыва. Так что комбинируйте этот тип с другими типами или создавайте звук отдельно опкодом 0565 (описан выше) Highslide JS
			5 Похоже на тип 4 Highslide JS
			6 Большой взрыв, один из самых больших Highslide JS
			7 Чуть покороче чем тип 6 Highslide JS
			8 Звука нет, эффекта нет, трясение камеры есть, здоровье отнимает. Highslide JS
			9 То же самое, что и тип 8 Highslide JS
			10 Наряду с типом 6 самый большой взрыв Highslide JS
			11 Как тип 0, но чуть пожиже Highslide JS
			12 Самый маленький взрыв, как от взрыва RC-машинки. Highslide JS
			13+ Тринадцатый и последующие типы, по-видимому, не поддерживаются игрой: взрывы без звука, без эффекта.
			*/
			Command<COMMAND_ADD_EXPLOSION>(x, y, z, tipe);
			return 0;
		}
		else { throw "bad argument in function create_explosion"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int set_status_engine(lua_State* L) {// установить состояние двигателя авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int switcher = lua_tointeger(L, 2);
			Command<COMMAND_BOAT_STOP>(CPools::GetVehicleRef(car), switcher);// 
			return 0;
		}//

		else { throw "bad argument in function set_status_engine"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int player_defined(lua_State* L) {// пед существует.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);//  получить указатель на авто.

			float health = ped->m_fHealth;
			if (health > 1.0f) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function player_defined"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int follow_the_leader(lua_State* L) {// //01DE / 01DF	следовать за лидером
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.

			const void* p = lua_topointer(L, -1);
			CPed* ped = findpedinpool(p);//  получить указатель на авто.
			CPed* player = FindPlayerPed();// найти педа.
			ped->SetObjective(OBJECTIVE_SET_LEADER, player);
			return 0;
		}
		else { throw "bad argument in function follow_the_leader"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int arrested(lua_State* L) { // пед арестован?
	if (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED) {
		lua_pushboolean(L, true);
		return 1;
	}
	else {
		lua_pushboolean(L, false);
		return 1;
	}
};

int setcarcoordes(lua_State* L) {// установить координаты авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {//число.
			const void* p = lua_topointer(L, 1);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);

			CVector pos = { x, y, z };
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), pos.x, pos.y, pos.z);// установить координаты авто.
		}
		else { throw "bad argument in function setcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int is_car_stuck(lua_State* L) {//03CE: car 12@ stuck если машина застряла.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_stuck = Command<COMMAND_IS_CAR_STUCK>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_stuck);
			return 1;
		}
		else { throw "bad argument in function is_car_stuck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_upsidedown(lua_State* L) {//01F4: car 12@ flipped если машина перевернута.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_upsidedown = Command<COMMAND_IS_CAR_UPSIDEDOWN>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_upsidedown);
			return 1;
		}
		else { throw "bad argument in function is_car_upsidedown"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_upright(lua_State* L) {// 020D: car 12@ flipped если указанный автомобиль перевернут.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_upright = Command<COMMAND_IS_CAR_UPRIGHT>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_upright);
			return 1;
		}
		else { throw "bad argument in function is_car_upright"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int find_road_for_car(lua_State* L) {// найти дорогу.
	try {
		if (LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//число.

			float x1 = lua_tonumber(L, 1);
			float y1 = lua_tonumber(L, 2);
			float z1 = lua_tonumber(L, 3);
			double x, y, z;
			Command<COMMAND_GET_CLOSEST_CAR_NODE>(x1, y1, z1, x, y, z);// найти дорогу.
			Stack<double>::push(L, x);	Stack<double>::push(L, y);	Stack<double>::push(L, z);
			return 3;
		}
		else { throw "bad argument in function find_road_for_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int add_stuck_car_check(lua_State* L) {// условия для того, чтобы авто считалась застрявшей.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//число.

			const void* p = lua_topointer(L, -3);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			float distance = lua_tonumber(L, -2);// дистанция. 
			int time = lua_tointeger(L, -1);// время.
			Command<COMMAND_ADD_STUCK_CAR_CHECK>(time, distance, CPools::GetVehicleRef(car));// условия для того, чтобы авто считалась застрявшей.
			return 0;
		}
		else { throw "bad argument in function add_stuck_car_check"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarstrong(lua_State* L) {// сделать авто устойчивым.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//указатель на авто.

			const void* p = lua_topointer(L, -2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int value = lua_tointeger(L, 2);
			if (value == 1) {
				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), true);// авто устойчивое - да.
				return 0;
			}
			if (value == 0) {
				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), false);// авто устойчивым - нет.
				return 0;
			}
		}
		else { throw "bad argument in function setcarstrong"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int putincar(lua_State* L) {// переместить педа в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, -1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			double x = car->GetPosition().x; // отправить в стек.
			double y = car->GetPosition().y; // отправить в стек.
			double z = car->GetPosition().z; // отправить в стек.

			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car);
			ped->WarpPedIntoCar(car); // переместить педа в авто
			this_thread::sleep_for(chrono::milliseconds(20));
			Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), x, y, z);// установить координаты авто.
			//CPed* player = FindPlayerPed();// найти педа  
			//if (ped != player) {
			//	Command<COMMAND_WARP_CHAR_INTO_CAR>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car));
			//	return 0;
			//}
			//else {
			//	Command<COMMAND_WARP_PLAYER_INTO_CAR>(CWorld::PlayerInFocus, CPools::GetVehicleRef(car));}
			return 0;

		}
		else { throw "bad argument in function putincar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int ped_attack_car(lua_State* L) {// пед атакует авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатели на педа и авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			ped->SetObjective(OBJECTIVE_DESTROY_CAR, car); //уничтожить машину 01D9
			return 0;

		}
		else { throw "bad argument in function ped_attack_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int checkcheat(lua_State* L) {//чит код введен?.
	try {
		if (LUA_TSTRING == lua_type(L, -1)) {// если строка
			string cheat = Stack<string>::get(L, 1);// 
			if (-1 != cheatstr.find(cheat)) {
				cheatstr.clear();
				lua_pushboolean(L, true); cheatstr.erase(0, 1);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
}
int destroy(lua_State* L) {// удаления объектов из памяти при перезагрузки скрипта. 
	int i;
	map<int, lua_State*>::iterator it;

	for (auto it = markeron.begin(); it != markeron.end(); ++it) {
		if (L == it->second) {
			i = it->first;
			Command<COMMAND_REMOVE_BLIP>(i);//удалить маркер.
			markeron.erase(i);
		}
	}

	for (auto it = spheres.begin(); it != spheres.end(); ++it) {
		if (L == it->second) {
			i = it->first;
			Command<COMMAND_REMOVE_SPHERE>(i);// удалить сферу.
			spheres.erase(i);
		}
	}

	//map<int, lua_State*>::iterator pick;

	for (auto pick = pickupsids.begin(); pick != pickupsids.end(); ++pick) {
		if (L == pick->second) {
			i = pick->first;
			Command<COMMAND_REMOVE_PICKUP>(i);// удалить пикап.
			pickupsids.erase(i);
		}
	}

	for (auto pick = mappeds.begin(); pick != mappeds.end(); ++pick) {
		if (L == pick->second) {
			CPed* ped = pick->first; ped->ClearInvestigateEvent();// пед уходит, опустить педа.
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
			mappeds.erase(pick);
		}
	}

	for (auto cars = mapcars.begin(); cars != mapcars.end(); ++cars) {
		if (L == cars->second) {
			CVehicle* car = cars->first;
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			mapcars.erase(cars);
		}
	}

	for (auto objs = mapobjs.begin(); objs != mapobjs.end(); ++objs) {
		if (L == objs->second) {
			CObject* obj = objs->first;
			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
			mapobjs.erase(objs);
		}
	}

	return 0;
};
int all_destroy() {// удаления объектов из памяти. при перезагрузки скрипта. 
	int i;
	map<int, lua_State*>::iterator it;

	for (auto it = markeron.begin(); it != markeron.end(); ++it) {
			i = it->first;
			Command<COMMAND_REMOVE_BLIP>(i);//удалить маркер.
			markeron.erase(i);
	}

	for (auto it = spheres.begin(); it != spheres.end(); ++it) {
			i = it->first;
			Command<COMMAND_REMOVE_SPHERE>(i);// удалить сферу.
			spheres.erase(i);
	}

	//map<int, lua_State*>::iterator pick;

	for (auto pick = pickupsids.begin(); pick != pickupsids.end(); ++pick) {
			i = pick->first;
			Command<COMMAND_REMOVE_PICKUP>(i);// удалить пикап.
			pickupsids.erase(i);
	}

	for (auto pick = mappeds.begin(); pick != mappeds.end(); ++pick) {
			CPed* ped = pick->first; ped->ClearInvestigateEvent();// пед уходит, опустить педа.
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
			mappeds.erase(pick);
	}

	for (auto cars = mapcars.begin(); cars != mapcars.end(); ++cars) {
			CVehicle* car = cars->first;
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			mapcars.erase(cars);
	}

	for (auto objs = mapobjs.begin(); objs != mapobjs.end(); ++objs) {
			CObject* obj = objs->first;
			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
			mapobjs.erase(objs);
	}

	return 0;
};

int game_font_print(lua_State* L) {// вывести особенный игровой текст.
	try {
		if (LUA_TTABLE == lua_type(L, -1)) {// получаем таблицу из 9 элементов. 
			for (int i = 1; i < 12; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2); lua_insert(L, i);
			}
			lua_pop(L, 1);
			bool printgame = Stack<bool>::get(L, 1);//включить отобржения на экране
			string strprintgame = Stack<string>::get(L, 2);
			int abc_x = lua_tointeger(L, 3);
			int ord_y = lua_tointeger(L, 4);
			int font = lua_tointeger(L, 5);
			float spacing = lua_tonumber(L, 6);
			float size_x = lua_tonumber(L, 7);
			float size_y = lua_tonumber(L, 8);
			int red = lua_tointeger(L, 9);

			int blue = lua_tointeger(L, 10);
			int green = lua_tointeger(L, 11);
			lua_pop(L, lua_gettop(L));
			spite::set(printgame, strprintgame, abc_x, ord_y, spacing, font, size_x,
				size_y, red, blue, green);
			//showstack(L);
			return 0;
		}

		else { throw "bad argument in function game_font_print"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int star_timer(lua_State* L) {
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// если число
			const char* str = lua_tostring(L, 1);// строка.
			Command<COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING>(str);
			Stack<const char*>::push(L, str);
			return 1;
		}
		else { throw "bad argument in function  star_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int stop_timer(lua_State* L) {// остановить таймер.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// если число
			const char* str = lua_tostring(L, 1);// строка.
			static int timer = 13608;
			CUserDisplay::OnscnTimer.ClearClock(timer); //014F: stop_timer $3402
			Command<COMMAND_CLEAR_ONSCREEN_TIMER>(str);
			var_$3402 = 0;
			return 0;
		}
		else { throw "bad argument in function stop_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int timer_donw(lua_State* L) {//  таймер на уменьшение.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			int time = lua_tointeger(L, 1);// если число
			const char* str1 = lua_tostring(L, 2);// строка.
			char* str = (char*)str1;
			var_$3402 = 1000 * time; //in CLEO $3402 = 120000

			int tipe = lua_tointeger(L, 3);// если число
			static int timer = 13608;
			CUserDisplay::OnscnTimer.AddClock(timer, str, tipe);
			return 0;
		}
		else { throw "bad argument in function timer_donw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int ped_frozen(lua_State* L) {// заморозить игpока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// если число

			Command<COMMAND_SET_PLAYER_CONTROL>(CWorld::PlayerInFocus, status);

			return 0;
		}
		else { throw "bad argument in function timer_donw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int hold_cellphone(lua_State* L) {// поднять телефон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на авто.
			int status = lua_tointeger(L, 2);// если число.
			Command<COMMAND_SET_CHAR_ANSWERING_MOBILE>(ped, status);
			return 0;
		}
		else { throw "bad argument in function hold_cellphone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_lastweapondamage(lua_State* L) {// номер оружие, которое нанесло урон авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.
			const void* p = lua_topointer(L, -1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			unsigned char c = car->m_nLastWeaponDamage;

			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;
		}
		else { throw "bad argument in function car_lastweapondamage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int car_currentgear(lua_State* L) {// текущая передача авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			unsigned char c = car->m_nCurrentGear;
			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;

		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int getcar_model(lua_State* L) {// получить модель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			unsigned char c = car->m_nModelIndex;
			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;

		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int setcarsiren(lua_State* L) {// установить сирену для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, -1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int tipe = lua_tointeger(L, 2);
			car->m_nSirenOrAlarm = tipe;
			//car->m_bSirenOrAlarm = tipe;
			return 0;
		}
		else { throw "bad argument in function setcarsiren"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int ped_car_as_passenger(lua_State* L) {// пед садится в авто как пассажир.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_PASSENGER, car); //сьесть в машину. 01D9
			return 0;

		}
		else { throw "bad argument in function ped_car_as_passenger"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int ped_car_as_driver(lua_State* L) {// пед садится в авто как водитель.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);

			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car); //пед садится в авто как водитель.
			return 0;
		}
		else { throw "bad argument in function ped_car_as_driver"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_at_point(lua_State* L) {//переместить камеру в координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  int tipe = lua_tointeger(L, 4);
			CVector pos = { x, y, z };
			Command<COMMAND_POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, tipe); //  
			return 0;
		}
		else { throw "bad argument in function camera_at_point"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_camera_position(lua_State* L) {//уст камеру в координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4)
			&& LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  float rx = lua_tonumber(L, 4);
			float ry = lua_tonumber(L, 5); float rz = lua_tonumber(L, 6);
			CVector pos = { x, y, z };
			Command<COMMAND_SET_FIXED_CAMERA_POSITION>(pos.x, pos.y, pos.z, rx, ry, rz); //  
			return 0;
		}
		else { throw "bad argument in function set_camera_position"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
static int getcord(queue<double>q, const void* p) {
	CVehicle* car = findcarinpool(p);//  получить указатель на авто.
	while (!q.empty()) {
		this_thread::sleep_for(chrono::milliseconds(1));
		double x = q.front(); q.pop();
		double y = q.front(); q.pop();
		double z = q.front(); q.pop();

		Command<COMMAND_CAR_GOTO_COORDINATES>(car, x, y, z);// авто едет на координаты.
		while (!car->IsSphereTouchingVehicle(x, y, z, 3.0)) {
			this_thread::sleep_for(chrono::milliseconds(1));
		};
	};
	return 0;
};
void writelog1(double x) {// запись ошибок в файл.
	string path = "lualoader\\queqe.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x;  f1 << "\n"; }
	f1.close();
};
int go_to_route(lua_State* L) {//уст маршрут авто.
	try {
		if (LUA_TTABLE == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			lua_State* L1 = luaL_newstate();
			luaL_openlibs(L1);// открыть допю. библиотеки.
			int stacksize = lua_gettop(L);// кол-во элементов в  стек.	
			stacksize++;
			for (int i = 1; i < stacksize; i++) {
				lua_xmove(L, L1, 1);
			};
			if (LUA_TLIGHTUSERDATA == lua_type(L1, -1)) {
				const void* p = lua_topointer(L1, -1);
				lua_pop(L1, 1);	int counts = luaL_len(L1, 1);
				counts += 2; queue<double>q = {};// очередь для хранение всех значение координат из файла.
				for (int i = 1; i < counts; i++) {
					lua_pushinteger(L1, i);
					lua_gettable(L1, -2);
					if (LUA_TNUMBER == lua_type(L1, -1)) {
						double cord = lua_tonumber(L1, -1);
						q.push(cord); lua_pop(L1, 1);
					}
				};
				lua_close(L1);
				std::thread(getcord, q, p).detach();
				return 0;
			}
		}
		else { throw "bad argument in function go_to_route"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_current_name_luascript(lua_State* L) {// получить имя текущего lua файла.
	lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
	lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
	char const* luaname = lua_tostring(L, -1);//имя lua скрипта.
	lua_pushstring(L, luaname);
	return 1;
};

int restore_camera(lua_State* L) {// восстановить камеру.
	Command<COMMAND_RESTORE_CAMERA>();
	Command<COMMAND_RESTORE_CAMERA_JUMPCUT>();
	return 0;
};
int ped_atack(lua_State* L) {// пед бьет.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {

			const void* p1 = lua_topointer(L, 1);

			CPed* p = findpedinpool(p1);//  получить указатель на педа.

			for (auto ped : CPools::ms_pPedPool) {
				if (ped == p) {
					//ped->Attack();
					this_thread::sleep_for(chrono::milliseconds(1));
					//spite::active(true, ped);
					//this_thread::sleep_for(chrono::milliseconds(1000));
					//unsigned int x =	ped->Attack();// правой рукой.
					return 0;
				}
			}
		}
		else { throw "bad argument in function ped_atack"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int flash_hud(lua_State* L) {// Мигание элементов HUD.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// если число

			Command<COMMAND_FLASH_HUD_OBJECT>(status);
			//Мигание элементов HUD'а номер 8
			//	Номера элементов :
			//10 - полоса воздуха
			//	8 - радар
			//	4 - полоса здоровья
			//	"-1" - убрать мигание
			return 0;
		}
		else { throw "bad argument in function flash_hud"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_radio(lua_State* L) {// уст радио.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			int status = lua_tointeger(L, 1);// если число
			int type = lua_tointeger(L, 2);// если число
			//Указатель радиостанции - 1 = не начинать с начала файла станции, 0 = начинать с начала
			//Этот код операции устанавливает текущую радиостанцию ​​для воспроизведения в автомобиле, который поддерживает эту станцию.
			//В vc индекс станции тесно связан со списком радиостанций, за исключением индекса 9. Если используется индекс 9, 
			// станция устанавливает себя на 10, предотвращая воспроизведение станции MP3 с этим кодом операции.
			Command<COMMAND_SET_RADIO_CHANNEL>(status, type);
			return 0;
		}
		else { throw "bad argument in function set_radio"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_car_tires(lua_State* L) {// проколоть шину.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//строка.

			const void* p1 = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			int wheel = lua_tointeger(L, 2);// номер колеса.
			Command<COMMAND_BURST_CAR_TYRE>(CPools::GetVehicleRef(car), wheel);
			return 0;
		}
		else { throw "bad argument in function set_car_tires"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int set_wheel_status(lua_State* L) {// уст состояния шин авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//строка.
			/* первый указатель на авто. второй параметр номер колесо.	третий  статус, 0 = починка.
			*/
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int wheel = lua_tointeger(L, 2);
			int status = lua_tointeger(L, 3);
			if (car && car->m_nVehicleClass == VEHICLE_AUTOMOBILE) {
				CAutomobile* automobile = reinterpret_cast<CAutomobile*>(car);
				automobile->m_carDamage.SetWheelStatus(wheel, status);
				return 0;
			}
		}
		else { throw "bad argument in function set_car_tires"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int set_skin(lua_State* L) {// уст скин педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			char const* model = Stack<char const*>::get(L, 2);// модель скина

			Command<COMMAND_UNDRESS_CHAR>(CPools::GetPedRef(ped), model);
			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_DRESS_CHAR>(CPools::GetPedRef(ped));
			this_thread::sleep_for(chrono::milliseconds(1));
			return 0;
		}
		else { throw "bad argument in function ped_atack"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int ped_anim(lua_State* L) {// анимация.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {//число.

			const void* p = lua_topointer(L, 1);
			int tipe = lua_tointeger(L, 2);
			int idanimation = lua_tointeger(L, 3);
			int time = lua_tointeger(L, 4);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			RpClump* pe = ped->m_pRwClump;
			CAnimManager::BlendAnimation(pe, tipe, idanimation, time);
			return 0;
		}
		else { throw "bad argument in function ped_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int del_anim(lua_State* L) {// удалить анимацию.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {//число.
			//const void* p = lua_topointer(L, 1);
			//CPed* ped = findpedinpool(p);//  получить указатель на педа.
			//
			int tipe = lua_tointeger(L, 1);

			//CAnimManager::RemoveAnimBlockRef(tipe);
			//b->RemoveAnimBlockRefWithoutDelete(tipe);
			return 0;
		}
		else { throw "bad argument in function del_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarspeed(lua_State* L) {// получить скорость авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на авто.
			const void* p = lua_topointer(L, -1);
			float speed;// переменная хранить скорость авто.
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_GET_CAR_SPEED>(CPools::GetVehicleRef(car), &speed);//  получить скорость авто.
			Stack<float>::push(L, speed);// отправить в стек.  
			return 1;
		}
		else { throw "bad argument in function getcarspeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};






int check_defined_and_arest() {
	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10;// дать денег  
		CPed* player = FindPlayerPed();// найти игрока
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(10));
		bool arest = CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED;
		if (arest || (player->m_fHealth < 0.10f)) {
			CMessages::AddBigMessage(L"mission failed!", 2500, 0);//0 большими, розовые как миссии пройдена,
			this_thread::sleep_for(chrono::milliseconds(10));
			setflagmission(0);
			break;
		}
	};
	return 0;
	//std::thread(check_defined_and_arest);
};

int star_mission_marker(lua_State* L) {// создать маркер для миссии.
	static int point;	static int create = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// значение число.

			int t = lua_tointeger(L, -4);// id метки на карте.
			float x = lua_tonumber(L, -3); float y = lua_tonumber(L, -2);
			float z = lua_tonumber(L, -1);  CVector p = { x, y, z };
			this_thread::sleep_for(chrono::milliseconds(10));
			CPed* player = FindPlayerPed();
			unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
			bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);
			bool arest = CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED;
			if (player->m_bInVehicle && create == 0 && !getflagmission) {// в авто пед?
				create = 1;
				Command<COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT>(p.x, p.y, p.z, t, &point);
				lua_pushboolean(L, false);
				return 1;
			}
			if (!player->m_bInVehicle || getflagmission || (!player->m_fHealth > 0.10f) || (arest) && (create == 1)) {// в авто пед?
				create = 0;
				Command<COMMAND_REMOVE_BLIP>(point);// удалить метку на карте.>(p.x, p.y, p.z, t, size, &point);
				lua_pushboolean(L, false);
				return 1;
			}
			if (player->m_bInVehicle && !getflagmission && (player->m_fHealth > 0.10f) && (!arest) && (create == 1)) {// в авто пед?
				CVehicle* car = player->m_pVehicle;
				bool place = car->IsSphereTouchingVehicle(x, y, z, 3.0);
				if (place && (car->m_fTotSpeed < 0.1f)) {
					create = 0;
					Command<COMMAND_REMOVE_BLIP>(point);// удалить метку на карте.>(p.x, p.y, p.z, t, size, &point);
					//setstatusmission(true);
					//CTheScripts::ScriptSpace[OnAMissionFlag] = 1;
					//Command<COMMAND_DO_FADE>(500, 0);// затенение.
					//while (player->m_bInVehicle) {
					//	player->SetObjective(OBJECTIVE_LEAVE_CAR);
					//	this_thread::sleep_for(chrono::milliseconds(1000));
					//};// выйти из авто.
					//Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), 0.0f, 0.0f, 0.0f);// установить координаты авто.
					lua_pushboolean(L, true); 	//std::thread(check_defined_and_arest);
					return 1;
				}
			}
		}// int

		else { throw "bad argument in function star_mission_marker"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

CPed* findpedinpool(const void* p) {// найти педа в пуле.
	for (auto ped : CPools::ms_pPedPool) {
		if (ped == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return ped;
		}
	};
	CPed* ped2 = NULL;
	return ped2;
};
CVehicle* findcarinpool(const void* p) {// найти авто в пуле.
	for (auto car : CPools::ms_pVehiclePool) {
		if (car == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return car;
		}
	};
	CVehicle* car2 = NULL;
	return car2;
};
CObject* findobjinpool(const void* p) {// найти объект в пуле.
	for (auto obj : CPools::ms_pObjectPool) {
		if (obj == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return obj;
		}
	};
	CObject* obj2 = NULL;
	return obj2;
};
void showstack(lua_State* L) {
	int i = lua_gettop(L);/* получаем количество элементов в стеке.*/
	string path = "stack.txt";
	fstream f2; {f2.open("stack.txt", fstream::in | fstream::out | fstream::app);
	if (f2.is_open()) {// если файл есть удалить.
		f2.close();	remove("stack.txt");
	}}
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	int j = (i) * -1 - 1;
	i = -1;
	for (i; i > j; i--) {
		int t = lua_type(L, i);
		if (LUA_TSTRING == t) {
			f1 << "str ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TNUMBER == t) {
			f1 << "number ";
			f1 << i; f1 << "\n";
		}

		if (LUA_TBOOLEAN == t) {
			f1 << "LUA_TBOOLEAN ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "LIGHTUSERDATA ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTABLE == t) {
			f1 << "LUA_TTABLE ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TFUNCTION == t) {

			f1 << "funs ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			cout << "LUA_TTHREAD " << endl;
		}
		if (LUA_TNIL == t) {
			cout << "LUA_TNIL " << endl;
		}
	}
	f1.close(); }
};
void showstack1(lua_State* L) {
	int i = lua_gettop(L);/* получаем количество элементов в стеке.*/
	string path = "stack1.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	int j = (i) * -1 - 1;
	i = -1;
	for (i; i > j; i--) {
		int t = lua_type(L, i);
		if (LUA_TSTRING == t) {
			f1 << "str ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TNUMBER == t) {
			f1 << "number ";
			f1 << i; f1 << "\n";
		}

		if (LUA_TBOOLEAN == t) {
			f1 << "LUA_TBOOLEAN ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "LIGHTUSERDATA ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTABLE == t) {
			f1 << "LUA_TTABLE ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TFUNCTION == t) {
			f1 << "funs ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			cout << "LUA_TTHREAD " << endl;
		}
		if (LUA_TNIL == t) {
			cout << "LUA_TNIL " << endl;
		}
	}
	f1.close(); }
};// кол-во аргументов.
void reversestack(lua_State* L) {// реверс стека.
	int stacksize = lua_gettop(L);
	stacksize++;
	for (int i = 1; i < stacksize; i++) { lua_insert(L, i); }
};
string getkey(int key) {

	if (GetAsyncKeyState(0x30)) {
		return "0";
	}
	if (GetAsyncKeyState(0x31)) {
		return "1";
	}
	if (GetAsyncKeyState(0x32)) {
		return "2";
	}
	if (GetAsyncKeyState(0x33)) {
		return "3";
	}
	if (GetAsyncKeyState(0x34)) {
		return "4";
	}
	if (GetAsyncKeyState(0x35)) {
		return "5";
	}
	if (GetAsyncKeyState(0x36)) {
		return "6";
	}
	if (GetAsyncKeyState(0x37)) {
		return "7";
	}
	if (GetAsyncKeyState(0x38)) {
		return "8";
	}
	if (GetAsyncKeyState(0x39)) {
		return "9";
	}
	if (GetAsyncKeyState(0x41)) {
		return "a";
	}
	if (GetAsyncKeyState(0x42)) {
		return "b";
	}
	if (GetAsyncKeyState(0x43)) {
		return "c";
	}
	if (GetAsyncKeyState(0x44)) {
		return "d";
	}
	if (GetAsyncKeyState(0x45)) {
		return "e";
	}
	if (GetAsyncKeyState(0x46)) {
		return "f";
	}
	if (GetAsyncKeyState(0x47)) {
		return "g";
	}
	if (GetAsyncKeyState(0x48)) {
		return "h";
	}
	if (GetAsyncKeyState(0x49)) {
		return "i";
	}
	if (GetAsyncKeyState(0x4a)) {
		return "j";
	}
	if (GetAsyncKeyState(0x4b)) {
		return "k";
	}
	if (GetAsyncKeyState(0x4c)) {
		return "l";
	}
	if (GetAsyncKeyState(0x4d)) {
		return "m";
	}
	if (GetAsyncKeyState(0x4e)) {
		return "n";
	}
	if (GetAsyncKeyState(0x4f)) {
		return "o";
	}
	if (GetAsyncKeyState(0x50)) {
		return "p";
	}
	if (GetAsyncKeyState(0x51)) {
		return "q";
	}
	if (GetAsyncKeyState(0x52)) {
		return "r";
	}
	if (GetAsyncKeyState(0x53)) {
		return "s";
	}
	if (GetAsyncKeyState(0x54)) {
		return "t";
	}
	if (GetAsyncKeyState(0x55)) {
		return "u";
	}
	if (GetAsyncKeyState(0x56)) {
		return "v";
	}
	if (GetAsyncKeyState(0x57)) {
		return "w";
	}
	if (GetAsyncKeyState(0x58)) {
		return "x";
	}
	if (GetAsyncKeyState(0x59)) {
		return "y";
	}
	if (GetAsyncKeyState(0x5a)) {
		return "z";
	}
	if (GetAsyncKeyState(0x0d)) {
		return "\n";
	}
	if (GetAsyncKeyState(0x20)) {
		return " ";
	}
	return "";
};
int newthread(lua_State* L) {// новый поток.
	try {
		if (LUA_TFUNCTION == lua_type(L, 1)) {
			int stacksize = lua_gettop(L);
			//lua_pushinteger(L, stacksize);
			return lua_yield(L, stacksize);
		}
		else { throw "bad argument in function newthread"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

//CPed* player = FindPlayerPed();// найти педа  
//if (ped != player ) { 
//	bool point = Command<COMMAND_LOCATE_CHAR_ANY_MEANS_3D>(CPools::GetPedRef(ped), x, y, z, rx, ry, rz);
//	if (delay == 0) {
//		delay = iters;
//		lua_settop(L, 0);
//		lua_pushboolean(L, false);
//		return 1;
//	}
//	else if (iters - delay > 5) {
//			delay = 0;
//			lua_settop(L, 0);
//			lua_pushboolean(L, point);
//			return 1;
//		}
//	}
//else {
//	bool point = Command<COMMAND_LOCATE_PLAYER_ANY_MEANS_3D>(CWorld::PlayerInFocus, x, y, z, rx, ry, rz);
//	if (delay == 0) {
//		delay = iters;
//		//lua_settop(L, 0);
//		lua_pushboolean(L, false);
//		return 1;
//	}
//	else if (iters - delay > 5) {
//		delay = 0;
//	//	lua_settop(L, 0);
//		lua_pushboolean(L, point);
//		return 1;
//	}
//}
//static char x[256]; 
//snprintf(x, 256, "%.d", iters);

//wchar_t* str = getwchat(x);
//CMessages::AddMessageJumpQ(str, 1000, 1);// вывести сообщение на экран.




/*		wchar_t* str = L"kjhb";
		CVector pos = { x,y,z };
		"Э.03C3: set_timer_with_text_to 16@ type 0 text L'R_TIME'  // pem¬ ?ohk?:
		Command<COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING>(10, 0, L'R_TIME');*/



		//lua_State* copystack(lua_State* L); // Копирование стека и создания нового состояния.
		//const char* newname(char* str) {
			//
			//	if (!std::experimental::filesystem::exists("./threads")) {// Если нет папки создаем
			//		std::experimental::filesystem::create_directories("./threads");
			//	}
			//	char n[225] = "threads\\1.lua";
			//	fstream f0;
			//	f0.open(n, fstream::in | fstream::out | fstream::app);
			//
			//	if (f0.is_open()) {
			//		f0.close();
			//		remove(n);
			//	};// удалим дубликат, если он есть.
			//
			//	fstream f;	f.open(str);
			//	fstream f1;
			//	f1.open(n, fstream::in | fstream::out | fstream::app);
			//	vector<string>strf;
			//	if (f.is_open() && f1.is_open()) {
			//		string str10;
			//		int number = 0;
			//		while (!f.eof()) {
			//			number++;
			//			getline(f, str10);
			//			strf.push_back(str10);
			//		};
			//		int lastst = number - 1;
			//		for (int i = 0; i < number; i++)
			//		{
			//			if (i != lastst) { f1 << strf[i] + "\n"; }
			//			else { f1 << strf[i]; }
			//		}
			//	}; f.close(); f1.close();
			//	const char* wor = "threads\\1.lua";// (char*)newnane;
			//	return wor;
			//};
			//
			//void star(lua_State* L1) {
			//	lua_pcall(L1, 1, 0, 0);
			//};
			//static int g = 1;
			//		g++;
			//
			//void m(lua_State* L, const char* func, int args) {
			//	lua_State* L1 = lua_newthread(L);
			//
			//	int ret, ret1;//	this_thread::sleep_for(chrono::milliseconds(100));  
			//	lua_sethook(L, LUAHook, LUA_MASKCOUNT, 30);	// Добавить подсчет счетчика, который сработает после указания числа
			//	lua_sethook(L1, LUAHook1, LUA_MASKCOUNT, 30);	// Добавить подсчет счетчика, который сработает после указания числа
			//
			//	lua_getglobal(L, "main");
			//	ret1 = lua_resume(L, L1, 0);
			//	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += ret1;// дать денег 	
			//	while (true) {
			//		if (ret == LUA_YIELD) {//&& ret1 != LUA_YIELD
			//	lua_getglobal(L1, func);
			//	ret = lua_resume(L1, L, args);//запуск FUNC
			//			this_thread::sleep_for(chrono::milliseconds(10));
			//		}
			//			if (ret1 == LUA_OK) {
			//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 100;// дать денег 	
			//			this_thread::sleep_for(chrono::milliseconds(10));
			//			break;
			//			
			//		}
			//		else { continue; }
			//		if (ret1 == LUA_YIELD) {
			//		//.	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег 	
			//			lua_getglobal(L1, func);
			//			ret = lua_resume(L1, L, args); //this_thread::sleep_for(chrono::milliseconds(100));
			//		}
			//		if (ret == LUA_OK) {// Успешно завершение функции.
			//			break;
			//		}
			//
			//	};
			//};
			//
			//lua_State* cop(lua_State* L) {
			//	lua_State* L1 = luaL_newstate();
			//	luaL_openlibs(L1);// открыть допю. библиотеки.
			//	funs(L1);// список весь функций.
			//
			//	int stacksize = lua_gettop(L);// кол-во элементов в  стек.	
			//	stacksize++;
			//	for (int i = 1; i < stacksize; i++) {
			//		if (LUA_TLIGHTUSERDATA == lua_type(L, i)) {// значение число.
			//			const void* value = lua_topointer(L, i);// получить неопределенный указатель на польз.данные.	
			//			int value1 = (int)& value;
			//			lua_pushinteger(L, value1);  /*отправить адрес, который является ключом в стек. */
			//			lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
			//			int type1 = lua_type(L, -1);
			//			static char x[256];
			//			snprintf(x, 256, "type1 = %.d", type1);
			//			wchar_t* s1 = getwchat(x);
			//			CMessages::AddMessageJumpQ(s1, 3000, 0);
			//			if (LUA_TSTRING == lua_type(L, -1)) {
			//				CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// дать денег 
			//				const char* clas = lua_tostring(L, -1);// имя класс польз.данных в индексе стека.
			//				lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
			//				const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
			//				if (strcmp(clas, st) == 0) {
			//					CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег 
			//					CPed* ped = (CPed*)Userdata::get<CPed>(L, i, false);
			//					Stack<CPed*>::push(L1, ped);// отправить в стек указатель на педа.
			//					lua_pop(L, 1);
			//				}
			//			}
			//		}
			//	}
			//	return L1;
			//};
		//lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
		//lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
		//char const* luaname = lua_tostring(L, -1);//имя lua скрипта.
		//wchar_t* str = getwchat(luaname);

		//lua_State* L1 = luaL_newstate();
		//luaL_openlibs(L1);
		//funs(L1);// список весь функций.

		//int stacksize = lua_gettop(L);
		//stacksize++;
		//for (int i = 1; i < stacksize; i++) {
		//	lua_pushvalue(L, i);// копировать на вершину стека.
		//	lua_xmove(L, L1, 1);// Снимает с L1 элементов передает L.
		//};

		//luastate.push_back(L1);// добавить новое состояние в list
		////int args = lua_gettop(L1);
		//		if (0 == luaL_loadfile(L1, luaname)) {// Текущий lua файл.      
		//			lua_pcall(L1, 0, 0, 0);// запуск файла.
		//			lua_pushvalue(L1, 1);//скопировать имена функции, отправить на вершину стека.
		//			std::thread t([=]() {lua_pcall(L1, args, 0, 0); });
		//			t.detach();

		//int timer(int time, int t) {
		//	if (CTimer::m_snTimeInMilliseconds - time > t) {//t = 0; // обнулить таймер
		//		return 0;
		//	};
		//	if (CTimer::m_snTimeInMilliseconds - time < t) {
		//		this_thread::sleep_for(chrono::milliseconds(1));
		//		timer(t, time);
		//	}
		//};
		//int stacksize = lua_gettop(L);
		//stacksize++;
		//for (int i = 1; i < stacksize; i++) {
		//	lua_pushvalue(L, i);// копировать на вершину стека.
		//	lua_xmove(L, L1, 1);// Снимает с L1 элементов передает L.
		//};
		//luastate.push_back(L1);// добавить новое состояние в list
		//int args = lua_gettop(L1);
		//int args = lua_gettop(L); args++;
		//lua_State* L1 = lua_newthread(L);
		//lua_pushthread(L1);
		//return lua_yield(L, args);/* Когда функция C вызывает lua_yield таким образом, запущенная сопрограмма приостанавливает
		//свое выполнение, и вызов lua_resume этой запущенной процедуры возвращается.*/
		//lua_insert(L, 1);//Перемещает поток в основание стека.
		//int stacksize = lua_gettop(L);
		//for (int i = 1; i < stacksize; i++) {
		//	lua_xmove(L, L1, 1);// Снимает с L1 элементов передает L.
		//}
		//allstate.emplace(L1, L);// добавить в map.
		//lua_State* L1 = luaL_newstate();
		//luaL_openlibs(L1);// открыть допю. библиотеки.
		//funs(L1);// список весь функций.
		//lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
		//lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
		//char const* luaname = lua_tostring(L, -1);//имя lua скрипта.	
		//lua_pop(L, 1);
		//luastate.push_back(L1);// добавить новое состояние в list	
		//char* name = (char*)luaname;//старое имя.
		//const char* namelua = newname(name);
		//	if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// значение число.
		//		CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег 
		//		CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
		//		Stack<CPed*>::push(L1, ped);// отправить в стек указатель на педа.			
		//	}
		//};
		//	int status = luaL_loadfile(L1, namelua);
		//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10;// дать денег 
						//			lua_pushvalue(L1, 1);//скопировать имена функции, отправить на вершину стека.

				//if (LUA_TFUNCTION == lua_type(L1, 1)) {
		//		}



		//const void* value = lua_topointer(L, i);// получить неопределенный указатель на польз.данные.
					//lua_pushinteger(L, (int)& value);  /*отправить адрес, который является ключом в стек. */
					//lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
					//if (LUA_TSTRING == lua_type(L, -1)) {//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// дать денег 
					//	const char* clas = lua_tostring(L, -1);// имя класс польз.данных в индексе стека.
					//	lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
					//	const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
					//	if (strcmp(clas, st) == 0) {
					//		lua_pop(L, 1);
					//	     }
					//    }


		//bool keyup(unsigned int key) {
		//	if (!KeyCheck::CheckWithDelay(key, 200)) { return true; }
		//	else { return false; }
		//};







		//lua_State* L1 = luaL_newstate();
		//luaL_openlibs(L1);// открыть допю. библиотеки.
		//funs(L1);// список весь функций.
		//lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
		//lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
		//char const* luaname = lua_tostring(L, -1);//имя lua скрипта.	
		//lua_pop(L, 1);
		//int stacksize = lua_gettop(L);// кол-во элементов в  стек.	
		//stacksize++;
		//for (int i = 1; i < stacksize; i++) {
		//	if (LUA_TLIGHTUSERDATA == lua_type(L, i)) {// значение число.
		//		const void* value = lua_topointer(L, i);// получить неопределенный указатель на польз.данные.
		//		lua_pushinteger(L, (int)& value);  /*отправить адрес, который является ключом в стек. */
		//		lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
		//		if (LUA_TSTRING == lua_type(L, -1)) {
		//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// дать денег 
		//			const char* clas = lua_tostring(L, -1);// имя класс польз.данных в индексе стека.
		//			lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
		//			const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
		//			if (strcmp(clas, st) == 0) {
		//				CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег 
		//				CPed* ped = (CPed*)Userdata::get<CPed>(L, i, false);
		//				Stack<CPed*>::push(L1, ped);// отправить в стек указатель на педа.
		//				lua_pop(L, 1);
		//			}
		//		}
		//	}
		//};
		//luastate.push_back(L1);// добавить новое состояние в list	
			//	if (res == LUA_YIELD) {
			//		res = lua_resume(L, L1, 0); // main
			//		lua_pop(L, 1);
			//		if (res == 0) { break; }
			//		else {res1 = lua_resume(L1, L, 0);// foo
			//		}
			//			/*L1 = lua_tothread(L, -1); lua_pop(L, 1); lua_xmove(L, L1, lua_gettop(L)); lua_remove(L1, 1); args = lua_gettop(L1);
			//			for (int i = 1; i > args; i++) { lua_pushvalue(L1, i); }
			//			args--;	res1 = lua_resume(L1, L, args);	lua_xmove(L, L1, 1);
			//		lua_sethook(L, LUAHook, LUA_MASKCOUNT, 6);*/
			//			for (mll it = allstate.begin(); it != allstate.end(); ++it) {
			//				if (L == it->second) {	L1 = it->first;
			//	if (LUA_TTHREAD == lua_type(L, -1) && LUA_TNUMBER == lua_type(L, -2)) {
			//		args = lua_tointeger(L, -2);
			//		showstack(L);
			//		CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 100;
			//		this_thread::sleep_for(chrono::milliseconds(5200));
			//	}
			//			lua_gettop(L1);//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;
			//		if (LUA_TFUNCTION == lua_type(L1, -2)) {
			//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;
			//					for (int i = 1; i < args; i++) { lua_insert(L1, i);	//	lua_pushvalue(L, i);
			//					}//Перемещает поток в основание стека.
			//					args--;/* Аргументы.*/  //lua_sethook(L1, LUAHook1, LUA_MASKCOUNT, 1);
			//					res1 = lua_resume(L1, L, args); //lua_settop(L1, args++);;	
			//					showstack1(L1);
			//					args++;					} 					} 
			//				}
			//        }
			//   if ( res1 == LUA_YIELD ||  res1 == LUA_OK ) {
			//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;	
			//	   lua_sethook(L, LUAHook, LUA_MASKCOUNT, 10);	
			//	   lua_getglobal(L, "main");
			//   res = lua_resume(L, L1, 0); //int li =  //  
			//   lua_sethook(L, LUAHook, LUA_MASKCOUNT, 0);
			//   showstack(L); lua_pop(L1, 1); 
			//   this_thread::sleep_for(chrono::milliseconds(100));
			//   break;
			//   
			// lua_pop(L1, 1); 
			//   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// size =  args-size;   
			//   lua_pop(L1, 1); if (LUA_TFUNCTION == lua_type(L1, -1)) {
			//   int size;	
			//   size = lua_gettop(L1); 
			//   if (size > args) {
			//	 if (size >5)		 {
			//	 size = size- args;  
			//	 size--;
			//   lua_pop(L1, size);
			//	 }
			//   }
			//  
			//   args++;
			//   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;
			//    this_thread::sleep_for(chrono::milliseconds(5200));
			//	   size++;
			//	   size = size - args;
			//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += size;
			//   cout << "lkj " << endl;
			//   lua_settop(L1, args);
			//	   for (int i = 1; i > size; i++) {
			//		    if (i > 2 )  {
			//		  
			//   }
			//	   this_thread::sleep_for(chrono::milliseconds(5)); 
			//	   size =  args-size;
			//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += size;// дать денег size++;
			//   args--;
			//	   lua_pop(L1, 1);
			//.	   this_thread::sleep_for(chrono::milliseconds(1));
			//   }
			//	}
			//}

			//lua_pushinteger(L, (int)& value);  /*отправить адрес, который является ключом в стек. */
			//lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
			//if (LUA_TSTRING == lua_type(L, -1)) {
			//	const char* clas = lua_tostring(L, -1);// имя класс польз.данных в индексе стека.
			//	lua_pop(L, 1);	const char* st = "cped";//	
			//	if (strcmp(clas, st) == 0) {
				//CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// получить указатель на педа.
				//if (LUA_TSTRING == lua_type(L, -1)) {
				//	string str  = lua_tostring(L, -1);
				//	str.erase(0, 10);
				//	
				//	const char* x = strdup(str.c_str());
				//	wchar_t* s1 = getwchat(x);
				//	CMessages::AddMessageJumpQ(s1, 3000, 0);  
				//		for (auto ped : CPools::ms_pPedPool) {
				//			int p = (int)ped;
				//			std::string sped = to_string(p);
				//			if (str == sped) {
				//				lua_pushboolean(L, true); Stack<CPed*>::push(L, ped);// отправить в стек и получить из стека можно
				//				return 2;
				//			}
				//		}
				//		CPed* p2 = nullptr;   lua_pushboolean(L, false);
				//		Stack<CPed*>::push(L, p2);// отправить в стек и получить из стека можно
				//		return 2;
				//		}

		//Добавляет транспортное средство в массив застрявших автомобилей
		//Синтаксис
		//
		//03CC: add_stuck_car_check[ручка автомобиля] расстояние[float] время[int]
		//параметр
		//
		//ручка автомобиля
		//Ручка транспортного средства
		//поплавок
		//Минимальное расстояние, которое автомобиль должен проехать в единицах
		//ИНТ
		//Продолжительность времени в мс
		//Родной аналог
		//
		//ADD_STUCK_CAR_CHECK
		//Этот код операции сохраняет дескриптор транспортного средства вместе с дополнительными параметрами в специальном массиве, чтобы проверить, не застрял ли он.Игра постоянно проверяет, все ли машины из этого массива соответствуют требованиям.Транспортное средство помечается как застрявшее, если оно не проезжает минимальное расстояние, установленное в качестве второго параметра в течение указанного периода времени, установленного в качестве третьего параметра.Если транспортное средство уничтожено, оно удаляется из массива застрявших автомобилей.Массив застрявших автомобилей может вместить до 6 ручек автомобиля.
		//
		// 