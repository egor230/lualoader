#define _CRT_SECURE_NO_WARNINGS
#include "funcs.h"

using namespace plugin;
using namespace std;

int setobjоcoordes(lua_State* L); // установить координаты для объект.
int create_newthread(lua_State* L); // создания нового потока.

struct star_thread {
	static bool star_second_thread;// запускать второй поток.

	static void set(bool star_second_thread1) { star_second_thread = star_second_thread1; }

	static bool get() { return star_second_thread; };//
};
bool star_thread::star_second_thread;

struct star_coroutine {
	static bool coroutine;// выкл/вкл корутин.

	static void set(bool coroutine1) { coroutine = coroutine1; }

	static bool get() { return coroutine; };//
};
bool star_coroutine::coroutine;

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
bool spite::switc; CPed* spite::p; bool spite::printgame;// флаг для вывода экран.
string spite::strprintgame;// вывод на экран.
int spite::abc_x; int spite::ord_y; float spite::spacing; int spite::font; float spite::size_x;
float spite::size_y; int spite::red; int spite::blue; int spite::green;

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

bool corona::switc; // флаг для вывода экран.
float corona::abc_x; float corona::ord_y; float corona::pos_z; float corona::radius;
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

int DoorsExample::componentByDoorId[6] = { CAR_BONNET, CAR_BOOT, CAR_DOOR_LF, CAR_DOOR_RF, CAR_DOOR_LR, CAR_DOOR_RR };
int DoorsExample::m_nLastTimeWhenAnyActionWasEnabled = 0;
VehicleExtendedData<DoorsExample::VehicleDoors> DoorsExample::VehDoors;

lua_KFunction cont(lua_State* L);
int my_yield_with_res(lua_State* L, int res) {
	return lua_yieldk(L, 0, lua_yield(L, res), cont(L));/* int lua_yieldk(lua_State * L, int res, lua_KContext ctx, lua_KFunction k);
	Приостанавливает выполнение сопрограммы(поток).	Когда функция C вызывает lua_yieldk, работающая
	сопрограмма приостанавливает свое выполнение и вызывает lua_resume, которая начинает возврат данной сопрограммы.
	Параметр res - это число значений из стека, которые будут переданы в качестве результатов в lua_resume.
	Когда сопрограмма снова возобновит выполнение, Lua вызовет заданную функцию продолжения k для продолжения выполнения
	приостановленной C функции(смотрите §4.7). */
};

int hookFunc(lua_State* L, lua_Debug* ar) { return my_yield_with_res(L, 0); }; // хук.

lua_KFunction cont(lua_State* L) {// функция продолжения.
	lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
	return 0;
};

struct state {	lua_State* L = NULL;
	state() {	lua_State* L1 = luaL_newstate();
		luaL_openlibs(L1);		this->L = L1;	}
	lua_State* get() {	this->L = L; return L;	}
	//~state() {	lua_close(L);}
};

int reload(); // перегрузка по нажатию клавиши.
int start_lualoder(); // запуск скриптов.
int final_scripts();// завершить скрипты.

void writelog3(const char x[]) {// запись ошибок в файл.
	string path = "log.txt";// куда пишем ошибки.
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);// датируем загрузки скриптов.
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};
void writelog3(int x) {// запись ошибок в файл.
	string path = "log.txt";// куда пишем ошибки.
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);// датируем загрузки скриптов.
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};
int startscipt(string res, char* luafile, list<lua_State*>& luastate) {// запуска скрипта.

	state Lua; lua_State* L = Lua.get(); lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
	char str123[255]; auto j = std::filesystem::current_path();
	string c1 = j.string();	c1 = c1 + "\\?.lua"; strcpy(str123, c1.c_str());
	lua_pushstring(L, str123);	lua_setglobal(L, "fullpath");	luaL_dostring(L, "package.path = fullpath");

	funs(L); // список функций.	
	int status = luaL_loadfile(L, luafile);// проверка есть ли ошибки в файле.
		if (status == 0) {// если нет ошибки в файле.	
			string er0 = "loaded " + res;// перед имени текущего lua файла добавить loaded.
			char* x = strdup(er0.c_str());// преобразовать строку в char*.
			writelog(x);// запись резуальтат проверки на ошибки.

			bool coroutine = true; star_coroutine::set(coroutine);// разрешить запускать второй поток в скрипте.
		    lua_pushlightuserdata(L, L); // ключ в реестр указатель на L. 
			lua_pushstring(L, luafile); // отправить имя текущего lua файла в реестр.
			lua_settable(L, LUA_REGISTRYINDEX); // установить ключа и значение таблице реестре. 

			lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
			//Command<COMMAND_SCRIPT_NAME>(x); 
				//Command<COMMAND_TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME>(luafile);
			lua_pcall(L, 0, 0, 0);// запуск файла.
			lua_getglobal(L, "main");	//writelog3("star scpipt");
			if (LUA_TFUNCTION == lua_type(L, -1)) {
				luastate.push_back(L);// добавить указатель на lua состояния в list.

			    lua_resume(L, NULL, 0, NULL);	// запуск файла.
				lua_State* L1 = lua_newthread(L);// создать новый поток.

				if (!star_coroutine::get()){// если нельзя запустить второой поток в скрипте.
				
					//writelog3("exit");
					return 0;
				}
				else {
					int args = lua_gettop(L);// получить аргументы для второго потока.
					lua_xmove(L, L1, args);	args--;	reversestack(L1); //инвертировать содержимое стека.
					while (LUA_OK != lua_status(L)) {// Пока основной поток не закончен.
						this_thread::sleep_for(chrono::milliseconds(1)); // задержка.
						if (LUA_TFUNCTION == lua_type(L1, -1) && LUA_YIELD == lua_status(L) && star_coroutine::get()) {

							for (int i = 1; i <= args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.
							lua_resume(L1, L, args, NULL);
						}
						if (LUA_YIELD == lua_status(L1)) {// если второй на паузе.
							lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 800); //вызов функции с заданной паузой.
							lua_resume(L, L1, 0, NULL);// возобновить основной поток.
						}
						if (LUA_OK == lua_status(L1)) {// если второй поток завершен.      
							lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
							lua_resume(L, NULL, 0, NULL);
						}
						if (LUA_YIELD == lua_status(L) || LUA_YIELD == lua_status(L1) && (!star_coroutine::get())) {
							break;
		}   }	}	}	else { /*lua_close(L);*/ }

		}
		else {	string er1 = lua_tostring(L, -1); string er0 = "could not load " + er1;
			char* x = strdup(er1.c_str());		writelog(x);}// записать ошибку в файл.
		return 0;
};

void search_scripts() {// поиск всех lua файлов для запуска.
    dellod();

	for (auto const& de : std::filesystem::recursive_directory_iterator{
		std::filesystem::current_path() / "lualoader" }) { // папка для поиска
		if (de.path().extension() == ".lua" || de.path().extension() == ".LUA") {
			string res = de.path().string();// перевод имя файла в строку.
			char* luafile = strdup(res.c_str());// Текущий lua файл.
			listfile.push_back(luafile);// добавить текущий lua файл в list.

			std::thread(startscipt, res, luafile, std::ref(luastate)).detach();// независимым поток для запуска lua файла.
			
		}
	};
	std::thread(reload).detach(); // перегрузка скрипта по нажатию клавиши.

	std::thread(getkeyenvent).detach();// считывания символов клавиатуры.
};

bool s = true;
int start_lualoder() { // найти все lua файлы. меню 12,	старт новой игры 1.	
	star_thread::set(s);// Новая игра 7	 загрузка 8 точно загрузка 10 в игре 32. 8, 1, 10 загрузка.  1, 7 новая игра. 32 в игр
	CMenuManager& MenuManager = FrontEndMenuManager;// менеджер меню из SDK (раньше хардкод 0x869630).
	int step = 0;
   std::thread(search_scripts).detach();// поиск и запуск lua файлов.
   while (true) {
	   this_thread::sleep_for(chrono::milliseconds(1));
	   if (MenuManager.m_nCurrentMenuPage == 32 && step==0) {
		   step = 1;
	   }

      if ((MenuManager.m_nCurrentMenuPage == 10) || (MenuManager.m_nCurrentMenuPage == 7) && step == 1 ) {// перезагрузка скрипта.
			step = 2;
		}
	  if ((MenuManager.m_nCurrentMenuPage == 8) || (MenuManager.m_nCurrentMenuPage == 10) && step == 1) {// точно загрузка и новая игра.
		   final_scripts();
			 break;		}
		}; 
	
	  std::thread(timerstar).detach(); // запуск через загрузку сэйва.
	return 0;
}; 

class Message {//имя класса.
public: Message() {	

	Events::gameProcessEvent += [] {//обработчик событий игры.
		CPed* player = FindPlayerPed();// найти игрока.
		Events::gameProcessEvent += spite::draw; Events::gameProcessEvent += corona::draw; Events::vehicleRenderEvent += DoorsExample::ProcessDoors; // Тут обрабатываем события, а также выключаем их
		int number_save_slot = gGameState;// состояние игры из SDK (раньше хардкод 0x9B5F08). 9 = в игре.
		int gtg = CTimer::m_snTimeInMilliseconds;// игровой таймер из SDK (раньше хардкод 0x974B2C).
		if (number_save_slot == 9 && !star_thread::get()) {// скрипты запрещены и второй поток запущен.

			if ((Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus)) && gtg < 1000) { // новая игра
				star_thread::set(s);
				std::thread(start_lualoder).detach();
				  //writelog3("new");
			}

			else {// загруженая игра.
				if (gtg > 1000) { //writelog3("load gtg > 1000");
				star_thread::set(s);
				std::thread(start_lualoder).detach();	}// разрешить запускать скрипты.
			}
		}
		if (iters > 4294967200) { iters = 300; }
		iters++;

	};
   }

		~Message() {	}
} message;

int funs(lua_State* L) {// список функций.

	//set_path_to_module(L); // уст путь к модулю.
	lua_register(L, "findplayer", findplayer); // 1 найти томми.
	lua_register(L, "setpedhealth", setpedhealth); // 2 установить здоровье педу.
	lua_register(L, "setarmour", setarmour); // 3 установить броню педу.
	lua_register(L, "wait", wait); // 4 задержка.
	lua_register(L, "getpedhealth", getpedhealth); // 5 получить здоровье педа.
	lua_register(L, "getpedangle", getpedangle); // 6 получить угол педа.
	lua_register(L, "worldcoord", worldcoord); // 7 Перевод в мировые координаты.
	lua_register(L, "getpedcoordinates_on_x", getpedcoordinates_on_x); // 8 Получить мировую координату по x для педа.
	lua_register(L, "getpedcoordinates_on_y", getpedcoordinates_on_y); // 9 Получить мировую координату по y для педа.
	lua_register(L, "setarmour", setarmour); // 10 получить броню.
	lua_register(L, "givemoney", givemoney); // 11 дать денег педу.
	lua_register(L, "keypress", keypress); // 12 проверка на нажатие клавиш.
	lua_register(L, "printmessage", printmessage); // 13 вывод сообщение.
	lua_register(L, "getpedcoordes", getpedcoordes); // 14 получить координаты педа.
	lua_register(L, "randomfindped", randomfindped); // 15 получить рандомного педа.
	lua_register(L, "incar", incar); // 16  проверка пед в авто?.
	lua_register(L, "loadmodel", loadmodel); // 17 загрузить модель.
	lua_register(L, "availablemodel", availablemodel); // 18 проверка на загруженность модели.
	lua_register(L, "releasemodel", releasemodel); // 19 удалить модель из памяти.
	lua_register(L, "createcar", createcar); // 20 создать авто на координатах на координатах.
	lua_register(L, "createped", createped); // 21 создать педа на координатах.
	lua_register(L, "load_requested_models", load_requested_models); // 22 поставить модель на загрузить вне очереди.
	lua_register(L, "giveweaponped", giveweaponped); // 23 дать педу оружие.
	lua_register(L, "ped_sprint_to_point", ped_sprint_to_point); // 24 пед делает спринт к точке.
	lua_register(L, "ped_walk_to_point", ped_walk_to_point); // 25 Пед идет к точке.
	lua_register(L, "kill_ped_on_foot", kill_ped_on_foot); // 26 убить педа пешком.
	lua_register(L, "kill_char_any_means", kill_char_any_means); // 27 убить педа любыми средствами.
	lua_register(L, "ped_aim_at_ped", ped_aim_at_ped); // 28 пед целиться в педе.
	lua_register(L, "is_current_weapon_ped", is_current_weapon_ped); // 29 проверить текущее оружие.
	lua_register(L, "create_marker_actor", create_marker_actor); // 30 создать маркер над педом.
	lua_register(L, "removemarker", removemarker); // 31 удалить маркер.
	lua_register(L, "setpedcoordes", setpedcoordes); // 32 установить координаты для педа.
	lua_register(L, "remove_car", remove_car); // 33 удалить авто.
	lua_register(L, "car_in_water", car_in_water); // 34 проверка авто в воде.
	lua_register(L, "set_wanted", set_wanted); // 35 уcтановить уровень розыска.
	lua_register(L, "ped_in_point_in_radius", ped_in_point_in_radius); // 36 проверить находится пед в координатах с радиусом.
	lua_register(L, "create_sphere", create_sphere); // 37 создать сферу.
	lua_register(L, "clear_wanted", clear_wanted); // 38 убрать уровень розыска.
	lua_register(L, "getcarhealth", getcarhealth); // 39 получить кол-во здоровья авто.
	lua_register(L, "setcarhealth", setcarhealth); // 40 установить здоровье авто.
	lua_register(L, "remove_sphere", remove_sphere); // 41 удалить сферу.
	lua_register(L, "remove_ped", remove_ped); // 42 удалить педа.
	lua_register(L, "kill_ped", kill_ped); // 43 убить педа.
	lua_register(L, "getflagmission", getflagmission); // 44 проверка флага миссии.
	lua_register(L, "setflagmission", setflagmission); // 45 уcтановить флага миссии.
	lua_register(L, "showtext", showtext); // 46 Вывод особого текста на экран.
	lua_register(L, "remove_blip", remove_blip); // 47 удалить метку с карты.
	lua_register(L, "createblip", createblip); // 48 создать метку карте.
	lua_register(L, "play_sound", play_sound); // 49 проиграть мелодию.
	lua_register(L, "isped", isped); // 50 проверка это пед?
	lua_register(L, "isvehicle", isvehicle); // 51 проверка это транспорт?.
	lua_register(L, "cardrive", cardrive); // 52 авто едет в точку.
	lua_register(L, "setcarspeed", setcarspeed); // 53 установить скорость авто.
	lua_register(L, "opendoorcar", opendoorcar); // 54 открыть дверь авто.
	lua_register(L, "randomfindcar", randomfindcar); // 55 Найти случайное авто.
	lua_register(L, "getcarcoordes", getcarcoordes); // 56 получить координаты авто.
	lua_register(L, "create_money_pickup", create_money_pickup); // 57 создать пачку денег.
	lua_register(L, "getcarcoordinates_on_x", getcarcoordinates_on_x); // 58 Получить мировую координату по x для авто.
	lua_register(L, "getcarcoordinates_on_y", getcarcoordinates_on_y); // 59 Получить мировую координату по y для авто.
	lua_register(L, "car_in_point_in_radius", car_in_point_in_radius); // 60 проверить находится авто в координатах с радиусом.
	lua_register(L, "setdrivingstyle", setdrivingstyle); // 61 установить стиль езды авто.
	lua_register(L, "findped", findped); // 62 найти педа в пуле.
	lua_register(L, "create_weapon_pickup", create_weapon_pickup); // 63 создать пикап оружие.
	lua_register(L, "create_pickup", create_pickup); // 64 создать пикап.
	lua_register(L, "remove_pickup", remove_pickup); // 65 удалить пикап.
	lua_register(L, "picked_up", picked_up); // 66 проверка пикап подобран.
	lua_register(L, "play_voice", play_voice); // 67 Проиграть голос.
	lua_register(L, "fade", fade); // 68 затенение, просветления.
	lua_register(L, "draw_corona", draw_corona); // 69 создать корону(чекпойнт);.
	lua_register(L, "sound_coordinate", sound_coordinate); // 70 Проиграть звук в координатах
	lua_register(L, "show_text_styled", show_text_styled); // 71 Вывести игровой текст.
	lua_register(L, "setcarangle", setcarangle); // 72 установить угол авто.
	lua_register(L, "createmarker", createmarker); // 73 создать маркер на карте.
	lua_register(L, "setsizemarker", setsizemarker); // 74 установить размер маркера.
	lua_register(L, "cheat", checkcheat); // 75 чит код введен.
	lua_register(L, "destroy", destroy); // 76 удаления объектов из памяти при перезагрузки скрипта. 
	lua_register(L, "yield", my_yield); // 77 приостановить выполнение скрипта.
	lua_register(L, "setcardrive", setcardrive); // 78 установить водителя для авто.
	lua_register(L, "setcarpassenger", setcarpassenger); // 79 установить пассажира для авто.
	lua_register(L, "setcarfirstcolor", setcarfirstcolor); // 80 установить первый цвет авто.
	lua_register(L, "setcarseconscolor", setcarseconscolor); // 81 установить второй цвет авто.
	lua_register(L, "set_traffic", set_traffic); // 82 установить плотномть трафика транспорта.
	lua_register(L, "create_marker_car", create_marker_car); // 83 создать маркер над авто.
	lua_register(L, "car_explode", car_explode); // 84 взрывать авто.
	lua_register(L, "is_car_stopped", is_car_stopped); // 85 авто остановилось. 
	lua_register(L, "create_explosion", create_explosion); // 86 Создать взрыв на координатах.
	lua_register(L, "set_status_engine", set_status_engine); // 87 установить состояние двигателя авто.
	lua_register(L, "player_defined", player_defined); // 88 пед существует.
	lua_register(L, "setclock", setclock); // 89  задать время.
	lua_register(L, "arrested", arrested); // 90 пед арестован?
	lua_register(L, "lockstatus", lockstatus); // 91 статус двери авто.
	lua_register(L, "create_marker_pickup", create_marker_pickup); // 92 создать маркер над пикапом.
	lua_register(L, "create_obj", createobj); // 93 создать объект.
	lua_register(L, "remove_obj", remove_obj); // 94 удалить объект.
	lua_register(L, "setobjоcoordes", setobjоcoordes); // 95 установить координаты для объект.
	lua_register(L, "getobjcoordes", getobjcoordes); // 96 получить координаты объекта.
	lua_register(L, "create_marker_obj", create_marker_obj); // 97 создать маркер над объектом.
	lua_register(L, "isobject", isobject); // 98 проверка это объект?.
	lua_register(L, "setpedangle", setpedangle); // 99 установить угол педа.
	lua_register(L, "setcaraction", setcaraction); // 100 установить поведение авто.
	lua_register(L, "move_obj", move_obj); // 101 двигать объект.
	lua_register(L, "move_rotate", move_rotate); // 102 вращать объект.
	lua_register(L, "getobjangle", getobjangle); // 103 получить угол объекта.
	lua_register(L, "findcar", findcar); // 104 Найти авто.
	lua_register(L, "setcartask", setcartask); // 105 установить задачу авто.
	lua_register(L, "setcarcoordes", setcarcoordes); // 106 установить координаты авто.
	lua_register(L, "is_car_stuck", is_car_stuck); // 107 03CE: car 12@ stuck если машина застряла.
	lua_register(L, "is_car_upsidedown", is_car_upsidedown); // 108 01F4: car 12@ flipped если машина перевернута.
	lua_register(L, "is_car_upright", is_car_upright); // 109 020D: car 12@ flipped если указанный автомобиль стоит на колесах.
	lua_register(L, "find_road_for_car", find_road_for_car); // 110 найти дорогу.
	lua_register(L, "setcarstrong", setcarstrong); // 111 сделать авто устойчивым.
	lua_register(L, "putincar", putincar); // 112 переместить педа в авто.
	lua_register(L, "print_front", game_font_print); // 113 вывести особенный игровой текст.
	lua_register(L, "star_timer", star_timer); // 114 включить таймер.
	lua_register(L, "stop_timer", stop_timer); // 115 остановить таймер.
	lua_register(L, "timer_donw", timer_donw); // 116  таймер на уменьшение.
	lua_register(L, "ped_attack_car", ped_attack_car); // 117 пед атакует авто.
	lua_register(L, "ped_frozen", ped_frozen);  // 118 заморозить игpока.
	lua_register(L, "hold_cellphone", hold_cellphone); // 119 поднять телефон.
	lua_register(L, "car_lastweapondamage", car_lastweapondamage); // 120 номер оружие, которое нанесло урон авто.
	lua_register(L, "car_currentgear", car_currentgear); // 121 текущая передача авто.
	lua_register(L, "getcar_model", getcar_model); // 122 получить модель авто.
	lua_register(L, "setcarsiren", setcarsiren); // 123 установить сирену для авто.
	lua_register(L, "ped_car_as_driver", ped_car_as_driver); // 124 пед садится в авто как водитель.
	lua_register(L, "ped_car_as_passenger", ped_car_as_passenger); // 125 пед садится в авто как пассажир.
	lua_register(L, "ped_atack", ped_atack); // 126 пед бьет.
	lua_register(L, "show_text_gtx", show_text_gtx); // 127 вывести игровой текст.
	lua_register(L, "camera_at_point", camera_at_point); // 128 переместить камеру в координатах.
	lua_register(L, "restore_camera", restore_camera); // 129 восстановить камеру.
	lua_register(L, "is_wanted_level", is_wanted_level); // 130 проверить уровень розыска.
	lua_register(L, "set_camera_position", set_camera_position); // 131 установить камеру в координатах.
	lua_register(L, "flash_hud", flash_hud); // 132 Мигание элементов HUD.
	lua_register(L, "set_radio", set_radio); // 133 установить радио.			
	lua_register(L, "set_car_tires", set_car_tires); // 134 проколоть шину.
	lua_register(L, "create_spec_ped", create_spec_ped); // 135 создать спец педа.
	lua_register(L, "set_wheel_status", set_wheel_status); // 136 установить состояния шин авто.
	lua_register(L, "set_skin", set_skin); // 137 установить скин педа.
	lua_register(L, "remove_spec_ped", remove_spec_ped); // 138 удалить спец педа.
	lua_register(L, "go_to_route", go_to_route); // 139 установить маршрут авто.
	lua_register(L, "add_stuck_car_check", add_stuck_car_check); // 140 условия для того, чтобы авто считалась застрявшей.
	lua_register(L, "load_scene", load_scene); // 141 загрузить модели на координатах заранее.
	lua_register(L, "ped_anim", ped_anim); // 142 анимация.
	lua_register(L, "del_anim", del_anim); // 143 удалить анимацию.
	lua_register(L, "get_current_name_luascript", get_current_name_luascript); // 144 получить имя текущего lua файла.
	lua_register(L, "star_mission_marker", star_mission_marker); // 145 создать маркер для миссии.
	lua_register(L, "getobjcoordinates_on_x", getobjcoordinates_on_x); // 146 Получить мировую координату по x для объекта.
	lua_register(L, "getobjcoordinates_on_y", getobjcoordinates_on_y); // 147 Получить мировую координату по y для объекта.
	lua_register(L, "set_widescreen", set_widescreen); // вк// 148 вкл/выкл широкий экран.
	lua_register(L, "follow_the_leader", follow_the_leader); //149 //01DE// 01DE / 01DF следовать за лидером.
	lua_register(L, "getcarspeed", getcarspeed); // 150 получить скорость авто.
	lua_register(L, "newthread", newthread); // 151 запуск функции в новом потоке.		
	lua_register(L, "Getcameracoordes", Getcameracoordes); // 152 получить координаты камеры.

	lua_register(L, "remove_all_weapons_ped", remove_all_weapons_ped); // 154 удалить все оружия педа.
	lua_register(L, "Getweaponslot", Getweaponslot); // 155 получить номер слота по типу оружия.
	lua_register(L, "get_damage_weapon_ped", get_damage_weapon_ped); // 156 получить последний урон педа от оружия.
	lua_register(L, "get_aimgun_ped", get_aimgun_ped);// 157 получить показатель цели педа.
	lua_register(L, "get_ammo_weapon_ped", get_ammo_weapon_ped);// 158 получить кол-во патроны текущего оружие педа.
	lua_register(L, "createfireonped", createfireonped);// 159 создать огонь на педе.
	lua_register(L, "createfireoncar", createfireoncar);// 160 создать огонь на авто.
	lua_register(L, "createfireoncords", createfireoncords);// 161 создать огонь на координатах.
	lua_register(L, "remove_fire", remove_fire); // 162 удалить огонь.
	lua_register(L, "ped_shutdown", ped_shutdown);
	lua_register(L, "is_ped_damage_from_ped", is_ped_damage_from_ped); // 164 получил ли пед урон от педа.
	lua_register(L, "is_targetting_in_ped", is_targetting_in_ped); // 165 игрок целиться в педа.
	lua_register(L, "Remove_weapon_model", Remove_weapon_model);// 166 удалить оружие у педа.
	lua_register(L, "Createped", Createped);// 167 макрос создать педа.
	lua_register(L, "Createcar", Createcar);// 168 макрос создать авто на координатах.
	lua_register(L, "Giveweaponped", Giveweaponped); // 169 макрос дать педу оружие и патроны.
	lua_register(L, "Opendoorcar", Opendoorcar); // 170 Макрос открыть все двери авто.
	lua_register(L, "Create_weapon_pickup", Create_weapon_pickup); // 171 макрос создать пикап оружие.
	lua_register(L, "Create_pickup", Create_pickup); // 172 макрос создать пикап.
	lua_register(L, "Get_model_and_type_current_weapon_ped", Get_model_and_type_current_weapon_ped); // 173 макрос получить модель и тип текущего оружие педа.
	lua_register(L, "is_ped_in_car", is_ped_in_car); // 174 игрок в авто?
	lua_register(L, "ped_car", ped_car); // 175 авто педа.
	lua_register(L, "wanted_level", wanted_level); // 176 получить уровень розыска.
	lua_register(L, "get_model_current_weapon_ped", get_model_current_weapon_ped); // 177 макрос получить модель текущего оружие педа.
	lua_register(L, "get_type_current_weapon_ped", get_type_current_weapon_ped); // 178 макрос получить тип текущего оружие педа.
	lua_register(L, "set_camera_and_point", set_camera_and_point); // 179 установить и переместить камеру в координатах.
	lua_register(L, "get_damage_current_weapon", get_damage_current_weapon); // 180 получить уровень урона текущего оружие.
	lua_register(L, "set_damage_current_weapon", set_damage_current_weapon); // 181 установить уровень урона текущего оружие педа.
	lua_register(L, "set_clip_current_weapon", set_clip_current_weapon); // 181а установить объём обоймы текущего оружие педа.
	lua_register(L, "get_ped_state", get_ped_state); // 181б получить состояние педа.
	lua_register(L, "get_ped_last_state", get_ped_last_state); // 181в получить прошлое состояние педа.
	lua_register(L, "set_ped_state", set_ped_state); // 181г установить состояние педа.
	lua_register(L, "get_ped_heading", get_ped_heading); // 181д получить текущий угол поворота педа.
	lua_register(L, "set_ped_heading_goal", set_ped_heading_goal); // 181е установить желаемый угол поворота педа.
	lua_register(L, "get_ped_heading_goal", get_ped_heading_goal); // 181ж получить желаемый угол поворота педа.
	lua_register(L, "set_ped_heading_rate", set_ped_heading_rate); // 181з установить скорость поворота педа.
	lua_register(L, "get_ped_heading_rate", get_ped_heading_rate); // 181и получить скорость поворота педа.
	lua_register(L, "get_ped_enter_type", get_ped_enter_type); // 181к получить тип посадки педа в авто.
	lua_register(L, "get_ped_from_vehicle", get_ped_from_vehicle); // 181л получить авто из которого вышел пед.
	lua_register(L, "ped_can_be_deleted", ped_can_be_deleted); // 181м можно ли удалить педа из мира.
	lua_register(L, "ped_is_gang_member", ped_is_gang_member); // 181н пед член банды.
	lua_register(L, "ped_is_player", ped_is_player); // 181о пед это игрок.
	lua_register(L, "ped_is_in_control", ped_is_in_control); // 181п пед управляем.
	lua_register(L, "ped_is_shootable", ped_is_shootable); // 181р в педа можно стрелять.
	lua_register(L, "ped_is_pointer_valid", ped_is_pointer_valid); // 181с указатель на педа валиден.
	lua_register(L, "ped_set_dead", ped_set_dead); // 181т пометить педа мертвым.
	lua_register(L, "ped_dress", ped_dress); // 181у пед надевает одежду.
	lua_register(L, "ped_calculate_new_orientation", ped_calculate_new_orientation); // 181ф пересчитать ориентацию педа.
	lua_register(L, "ped_set_move_anim", ped_set_move_anim); // 181х задать педу анимацию движения.
	lua_register(L, "ped_set_get_up", ped_set_get_up); // 181ц пед встает с земли.
	lua_register(L, "ped_set_jump", ped_set_jump); // 181ч пед прыгает.
	lua_register(L, "ped_set_landing", ped_set_landing); // 181ш пед приземляется.
	lua_register(L, "ped_set_idle", ped_set_idle); // 181щ пед переходит в состояние покоя.
	lua_register(L, "ped_fall", ped_fall); // 181э пед падает.
	lua_register(L, "ped_turn_body", ped_turn_body); // 181ю пед поворачивает корпус к цели.
	lua_register(L, "ped_update_position", ped_update_position); // 181я пед обновить позицию по анимации.
	lua_register(L, "ped_restore_heading_rate", ped_restore_heading_rate); // 182а вернуть скорость поворота педа.
	lua_register(L, "ped_restore_head_position", ped_restore_head_position); // 182б вернуть голове положение.
	lua_register(L, "ped_restore_previous_state", ped_restore_previous_state); // 182в вернуть педу прошлое состояние.
	lua_register(L, "ped_restore_previous_objective", ped_restore_previous_objective); // 182г вернуть педу прошлую цель.
	lua_register(L, "ped_stop_non_partial_anims", ped_stop_non_partial_anims); // 182д остановить базовые анимации педа.
	lua_register(L, "ped_restart_non_partial_anims", ped_restart_non_partial_anims); // 182е возобновить базовые анимации педа.
	lua_register(L, "ped_clear_aim_flag", ped_clear_aim_flag); // 182ж сбросить флаг прицеливания.
	lua_register(L, "ped_clear_look_flag", ped_clear_look_flag); // 182з сбросить флаг взгляда.
	lua_register(L, "ped_clear_chat", ped_clear_chat); // 182и пед прекращает болтать.
	lua_register(L, "ped_clear_attack", ped_clear_attack); // 182к сбросить у педы атаку.
	lua_register(L, "ped_clear_attack_anim", ped_clear_attack_anim); // 182л сбросить атаку с удалением анимации.
	lua_register(L, "ped_clear_seek", ped_clear_seek); // 182м сбросить у педа поиск цели.
	lua_register(L, "ped_clear_follow_path", ped_clear_follow_path); // 182н сбросить следование по маршруту.
	lua_register(L, "ped_clear_wait_state", ped_clear_wait_state); // 182о сбросить состояние ожидания.
	lua_register(L, "ped_clear_point_gun_at", ped_clear_point_gun_at); // 182п опустить наведенное оружие.
	lua_register(L, "ped_answer_mobile", ped_answer_mobile); // 182р пед отвечает на телефон.
	lua_register(L, "ped_clear_answer_mobile", ped_clear_answer_mobile); // 182с сбросить разговор по телефону.
	lua_register(L, "ped_face_phone", ped_face_phone); // 182т пед поворачивается к телефону.
	lua_register(L, "ped_make_phonecall", ped_make_phonecall); // 182у пед звонит по телефону.
	lua_register(L, "ped_pause", ped_pause); // 182ф пед замирает.
	lua_register(L, "ped_mug", ped_mug); // 182х пед грабит ближайшего педа.
	lua_register(L, "ped_solicit", ped_solicit); // 182ц пед предлагает услуги.
	lua_register(L, "ped_buy_ice_cream", ped_buy_ice_cream); // 182ч пед покупает мороженое.
	lua_register(L, "ped_look_for_sexy_cars", ped_look_for_sexy_cars); // 182ш пед ищет красивые авто.
	lua_register(L, "ped_look_for_sexy_peds", ped_look_for_sexy_peds); // 182щ пед ищет красивых педов.
	lua_register(L, "get_car_driver", get_car_driver); // 364а получить водителя авто.
	lua_register(L, "get_car_passenger", get_car_passenger); // 364б получить пассажира авто по месту.
	lua_register(L, "get_car_num_passengers", get_car_num_passengers); // 364в получить кол-во пассажиров авто.
	lua_register(L, "get_car_max_passengers", get_car_max_passengers); // 364г получить макс кол-во пассажиров авто.
	lua_register(L, "get_car_steer_angle", get_car_steer_angle); // 364д получить угол руля авто.
	lua_register(L, "set_car_steer_angle", set_car_steer_angle); // 364е установить угол руля авто.
	lua_register(L, "get_car_gas_pedal", get_car_gas_pedal); // 364ж получить педаль газа авто.
	lua_register(L, "get_car_brake_pedal", get_car_brake_pedal); // 364з получить педаль тормоза авто.
	lua_register(L, "get_car_engine_on", get_car_engine_on); // 364и работает ли двигатель авто.
	lua_register(L, "set_car_engine_on", set_car_engine_on); // 364к вкл/выкл двигатель авто.
	lua_register(L, "get_car_lights_on", get_car_lights_on); // 364л включены ли фары авто.
	lua_register(L, "set_car_lights_on", set_car_lights_on); // 364м вкл/выкл фары авто.
	lua_register(L, "get_car_handbrake_on", get_car_handbrake_on); // 364н включен ли ручник авто.
	lua_register(L, "set_car_handbrake_on", set_car_handbrake_on); // 364о вкл/выкл ручник авто.
	lua_register(L, "get_car_locked", get_car_locked); // 364п заблокировано ли авто.
	lua_register(L, "set_car_locked", set_car_locked); // 364р заблокировать/разблокировать авто.
	lua_register(L, "get_car_is_van", get_car_is_van); // 364с авто фургон.
	lua_register(L, "get_car_is_bus", get_car_is_bus); // 364т авто автобус.
	lua_register(L, "get_car_is_big", get_car_is_big); // 364у авто большая машина.
	lua_register(L, "set_car_comedy_controls", set_car_comedy_controls); // 364ф шуточное управление авто.
	lua_register(L, "set_car_hide_occupants", set_car_hide_occupants); // 364х скрыть седоков авто.
	lua_register(L, "get_car_tires_invulnerable", get_car_tires_invulnerable); // 364ц шины авто неуязвимы.
	lua_register(L, "set_car_tires_invulnerable", set_car_tires_invulnerable); // 364ч шины авто неуязвимы вкл/выкл.
	lua_register(L, "get_car_door_lock", get_car_door_lock); // 364ш получить блокировку дверей.
	lua_register(L, "set_car_door_lock", set_car_door_lock); // 364щ установить блокировку дверей.
	lua_register(L, "get_car_last_damager", get_car_last_damager); // 365а получить последний урон авто.
	lua_register(L, "get_car_time_of_death", get_car_time_of_death); // 365б получить время смерти авто.
	lua_register(L, "get_car_weapon_damage", get_car_weapon_damage); // 365в тип оружия последнего урона авто.
	lua_register(L, "get_car_vehicle_class", get_car_vehicle_class); // 365г получить класс авто.
	lua_register(L, "car_play_horn", car_play_horn); // 365д авто сигналит.
	lua_register(L, "car_extinguish_fire", car_extinguish_fire); // 365е потушить пожар на авто.
	lua_register(L, "car_uses_siren", car_uses_siren); // 365ж есть ли сирена у авто.
	lua_register(L, "car_is_law_enforcement", car_is_law_enforcement); // 365з авто правопорядка.
	lua_register(L, "car_has_roof", car_has_roof); // 365и у авто есть крыша.
	lua_register(L, "car_is_open_top", car_is_open_top); // 365к авто кабриолет.
	lua_register(L, "car_is_upside_down", car_is_upside_down); // 365л авто перевернуто.
	lua_register(L, "car_is_on_its_side", car_is_on_its_side); // 365м авто на боку.
	lua_register(L, "car_is_vehicle_normal", car_is_vehicle_normal); // 365н авто в норме.
	lua_register(L, "car_can_be_deleted", car_can_be_deleted); // 365о можно удалить авто.
	lua_register(L, "car_can_ped_enter", car_can_ped_enter); // 365п можно сесть в авто.
	lua_register(L, "car_can_doors_be_damaged", car_can_doors_be_damaged); // 365р двери могут быть повреждены.
	lua_register(L, "car_activate_bomb", car_activate_bomb); // 365с активировать бомбу.
	lua_register(L, "car_activate_bomb_when_entered", car_activate_bomb_when_entered); // 365т бомба при посадке.
	lua_register(L, "car_get_height_above_road", car_get_height_above_road); // 365у высота авто над дорогой.
	lua_register(L, "car_is_clear_to_drive_away", car_is_clear_to_drive_away); // 365ф можно свободно уехать.
	lua_register(L, "car_is_door_ready", car_is_door_ready); // 365х дверь готова открыться.
	lua_register(L, "car_is_door_fully_open", car_is_door_fully_open); // 365ц дверь полностью открыта.
	lua_register(L, "car_is_door_closed", car_is_door_closed); // 365ч дверь закрыта.
	lua_register(L, "car_is_door_missing", car_is_door_missing); // 365ш дверь отсутствует.
	lua_register(L, "car_shuffle_passengers", car_shuffle_passengers); // 365щ пересадить пассажиров.
	lua_register(L, "car_update_passenger_list", car_update_passenger_list); // 366а обновить список пассажиров.
	lua_register(L, "get_car_radio_station", get_car_radio_station); // 366б получить радиостанцию авто.
	lua_register(L, "set_car_radio_station", set_car_radio_station); // 366в установить радиостанцию авто.
	lua_register(L, "get_car_created_by", get_car_created_by); // 366г кем создано авто.
	lua_register(L, "get_car_wanted_stars_on_enter", get_car_wanted_stars_on_enter); // 366д звёзды при входе в авто.
	lua_register(L, "set_car_wanted_stars_on_enter", set_car_wanted_stars_on_enter); // 366е установить звёзды при входе.
	lua_register(L, "get_weapon_range", get_weapon_range); // 366ж получить дальность оружия.
	lua_register(L, "set_weapon_range", set_weapon_range); // 366з установить дальность оружия.
	lua_register(L, "get_weapon_firing_rate", get_weapon_firing_rate); // 366и получить скорость стрельбы.
	lua_register(L, "set_weapon_firing_rate", set_weapon_firing_rate); // 366к установить скорость стрельбы.
	lua_register(L, "get_weapon_reload", get_weapon_reload); // 366л получить время перезарядки.
	lua_register(L, "set_weapon_reload", set_weapon_reload); // 366м установить время перезарядки.
	lua_register(L, "get_weapon_speed", get_weapon_speed); // 366н получить скорость пули.
	lua_register(L, "set_weapon_speed", set_weapon_speed); // 366о установить скорость пули.
	lua_register(L, "get_weapon_spread", get_weapon_spread); // 366п получить разброс оружия.
	lua_register(L, "set_weapon_spread", set_weapon_spread); // 366р установить разброс оружия.
	lua_register(L, "get_weapon_radius", get_weapon_radius); // 366с получить радиус поражения.
	lua_register(L, "set_weapon_radius", set_weapon_radius); // 366т установить радиус поражения.
	lua_register(L, "get_weapon_lifespan", get_weapon_lifespan); // 366у получить время жизни снаряда.
	lua_register(L, "set_weapon_lifespan", set_weapon_lifespan); // 366ф установить время жизни снаряда.
	lua_register(L, "get_weapon_slot_info", get_weapon_slot_info); // 366х получить слот оружия.
	lua_register(L, "get_ammo_in_clip_ped", get_ammo_in_clip_ped); // 366ц получить патроны в обойме.
	lua_register(L, "set_ammo_in_clip_ped", set_ammo_in_clip_ped); // 366ч установить патроны в обойме.
	lua_register(L, "get_weapon_state", get_weapon_state); // 366ш получить состояние оружия.
	lua_register(L, "weapon_reload_ped", weapon_reload_ped); // 366щ перезарядить оружие педа.
	lua_register(L, "weapon_is_melee", weapon_is_melee); // 367а оружие рукопашное.
	lua_register(L, "weapon_is_two_handed", weapon_is_two_handed); // 367б оружие двуручное.
	lua_register(L, "weapon_has_ammo_to_be_used", weapon_has_ammo_to_be_used); // 367в можно использовать оружие.
	lua_register(L, "get_player_money_info", get_player_money_info); // 367г получить деньги игрока.
	lua_register(L, "set_player_money_info", set_player_money_info); // 367д установить деньги игроку.
	lua_register(L, "get_player_display_money", get_player_display_money); // 367е получить отображаемые деньги.
	lua_register(L, "set_player_drive_by", set_player_drive_by); // 367ж стрельба на ходу вкл/выкл.
	lua_register(L, "get_wanted_chaos_level", get_wanted_chaos_level); // 367з получить очки хаоса.
	lua_register(L, "set_wanted_chaos_level", set_wanted_chaos_level); // 367и установить очки хаоса.
	lua_register(L, "get_wanted_multiplier", get_wanted_multiplier); // 367к получить множитель розыска.
	lua_register(L, "set_wanted_multiplier", set_wanted_multiplier); // 367л установить множитель розыска.
	lua_register(L, "get_cops_in_pursuit", get_cops_in_pursuit); // 367м получить копов в погоне.
	lua_register(L, "get_max_cops_in_pursuit", get_max_cops_in_pursuit); // 367н получить макс копов в погоне.
	lua_register(L, "set_max_cops_in_pursuit", set_max_cops_in_pursuit); // 367о установить макс копов.
	lua_register(L, "get_max_cop_cars_in_pursuit", get_max_cop_cars_in_pursuit); // 367п получить макс полицейских авто.
	lua_register(L, "set_max_cop_cars_in_pursuit", set_max_cop_cars_in_pursuit); // 367р установить макс полицейских авто.
	lua_register(L, "get_chance_on_roadblock", get_chance_on_roadblock); // 367с получить шанс блокпоста.
	lua_register(L, "set_chance_on_roadblock", set_chance_on_roadblock); // 367т установить шанс блокпоста.
	lua_register(L, "wanted_are_swat_required", wanted_are_swat_required); // 367у нужны ли SWAT.
	lua_register(L, "wanted_are_fbi_required", wanted_are_fbi_required); // 367ф нужны ли FBI.
	lua_register(L, "wanted_are_army_required", wanted_are_army_required); // 367х нужна ли армия.
	lua_register(L, "wanted_num_helis_required", wanted_num_helis_required); // 367ц нужны ли вертолеты.
	lua_register(L, "wanted_clear_crimes", wanted_clear_crimes); // 367ч очистить очередь преступлений.
	lua_register(L, "wanted_reset_police_pursuit", wanted_reset_police_pursuit); // 367ш сбросить полицейскую погоню.
	lua_register(L, "wanted_update_wanted_level", wanted_update_wanted_level); // 367щ обновить уровень розыска.
	lua_register(L, "wanted_set_no_drop", wanted_set_no_drop); // 368а уровень розыска без понижения.
	lua_register(L, "wanted_cheat_level", wanted_cheat_level); // 368б читерский уровень розыска.
	lua_register(L, "get_wanted_level_before_parole", get_wanted_level_before_parole); // 368в уровень розыска до пароля.
	lua_register(L, "get_wanted_last_decreased_time", get_wanted_last_decreased_time); // 368г время последнего понижения.
	lua_register(L, "force_weather", force_weather); // 368д плавно сменить погоду.
	lua_register(L, "force_weather_now", force_weather_now); // 368е мгновенно сменить погоду.
	lua_register(L, "release_weather", release_weather); // 368ж отпустить погоду.
	lua_register(L, "get_old_weather_type", get_old_weather_type); // 368з получить прошлую погоду.
	lua_register(L, "get_new_weather_type", get_new_weather_type); // 368и получить текущую погоду.
	lua_register(L, "set_weather_type", set_weather_type); // 368к установить тип погоды.
	lua_register(L, "is_lightning_flash", is_lightning_flash); // 368л вспышка молнии.
	lua_register(L, "force_rain", force_rain); // 368м установить силу дождя.
	lua_register(L, "get_game_hours", get_game_hours); // 368н получить часы.
	lua_register(L, "get_game_minutes", get_game_minutes); // 368о получить минуты.
	lua_register(L, "get_game_seconds", get_game_seconds); // 368п получить секунды.
	lua_register(L, "set_game_hours", set_game_hours); // 368р установить часы.
	lua_register(L, "set_milliseconds_per_game_minute", set_milliseconds_per_game_minute); // 368с скорость хода времени.
	lua_register(L, "get_milliseconds_per_game_minute", get_milliseconds_per_game_minute); // 368т получить скорость времени.
	lua_register(L, "clock_store", clock_store); // 368у запомнить время.
	lua_register(L, "clock_restore", clock_restore); // 368ф восстановить время.
	lua_register(L, "get_time_until", get_time_until); // 368х минуты до указанного времени.
	lua_register(L, "is_time_in_range", is_time_in_range); // 368ц время в диапазоне.
	lua_register(L, "get_fov", get_fov); // 368ч получить угол обзора.
	lua_register(L, "set_fov", set_fov); // 368ш установить угол обзора.
	lua_register(L, "get_far_clip", get_far_clip); // 368щ получить дальность прорисовки.
	lua_register(L, "set_far_clip", set_far_clip); // 369а установить дальность прорисовки.
	lua_register(L, "get_near_clip", get_near_clip); // 369б получить ближнюю границу.
	lua_register(L, "set_near_clip", set_near_clip); // 369в установить ближнюю границу.
	lua_register(L, "get_lod_distance", get_lod_distance); // 369г получить дистанцию LOD.
	lua_register(L, "set_lod_distance", set_lod_distance); // 369д установить дистанцию LOD.
	lua_register(L, "get_fade_color", get_fade_color); // 369е получить цвет затемнения.
	lua_register(L, "set_fade_color", set_fade_color); // 369ж установить цвет затемнения.
	lua_register(L, "camera_shake", camera_shake); // 369з тряхнуть камеру.
	lua_register(L, "camera_draw_borders_widescreen", camera_draw_borders_widescreen); // 369и границы широкоэкранного.
	lua_register(L, "camera_clear_player_weapon_mode", camera_clear_player_weapon_mode); // 369к сбросить режим оружия.
	lua_register(L, "camera_calculate_derived_values", camera_calculate_derived_values); // 369л пересчитать камеру.
	lua_register(L, "camera_dont_process_obbe_cinema", camera_dont_process_obbe_cinema); // 369м отключить кино-камеру.
	lua_register(L, "get_mouse_accel_vertical", get_mouse_accel_vertical); // 369н вертикальное ускорение мыши.
	lua_register(L, "set_mouse_accel_vertical", set_mouse_accel_vertical); // 369о установить ускорение мыши.
	lua_register(L, "get_mouse_accel_horizontal", get_mouse_accel_horizontal); // 369п горизонтальное ускорение мыши.
	lua_register(L, "set_mouse_accel_horizontal", set_mouse_accel_horizontal); // 369р установить ускорение мыши.
	lua_register(L, "get_use_mouse_3rd_person", get_use_mouse_3rd_person); // 369с мышь в третьем лице.
	lua_register(L, "get_sun_blocked_by_clouds", get_sun_blocked_by_clouds); // 369т солнце за облаками.
	lua_register(L, "weather_add_rain", weather_add_rain); // 369у добавить дождь.
	lua_register(L, "weather_add_stream_after_rain", weather_add_stream_after_rain); // 369ф пар после дождя.
	lua_register(L, "weather_render_rain_streaks", weather_render_rain_streaks); // 369х отрисовать полосы дождя.
	lua_register(L, "get_obj_type", get_obj_type); // 369ц получить тип объекта.
	lua_register(L, "get_obj_bonus_value", get_obj_bonus_value); // 369ч получить бонус объекта.
	lua_register(L, "set_obj_bonus_value", set_obj_bonus_value); // 369ш установить бонус объекта.
	lua_register(L, "get_obj_cost_value", get_obj_cost_value); // 369щ получить стоимость объекта.
	lua_register(L, "set_obj_cost_value", set_obj_cost_value); // 370а установить стоимость объекта.
	lua_register(L, "get_obj_damage_multiplier", get_obj_damage_multiplier); // 370б получить множитель урона.
	lua_register(L, "set_obj_damage_multiplier", set_obj_damage_multiplier); // 370в установить множитель урона.
	lua_register(L, "get_obj_collision_damage_type", get_obj_collision_damage_type); // 370г тип урона от столкновений.
	lua_register(L, "set_obj_collision_damage_type", set_obj_collision_damage_type); // 370д установить тип урона.
	lua_register(L, "get_obj_camera_avoids", get_obj_camera_avoids); // 370е камера обходит объект.
	lua_register(L, "set_obj_camera_avoids", set_obj_camera_avoids); // 370ж установить обход камерой.
	lua_register(L, "get_obj_timer", get_obj_timer); // 370з получить таймер объекта.
	lua_register(L, "set_obj_timer", set_obj_timer); // 370и установить таймер объекта.
	lua_register(L, "get_obj_ref_model_id", get_obj_ref_model_id); // 370к модель на которую ссылается объект.
	lua_register(L, "obj_can_be_deleted", obj_can_be_deleted); // 370л можно удалить объект.
	lua_register(L, "obj_damage", obj_damage); // 370м нанести урон объекту.
	lua_register(L, "obj_delete_all_mission", obj_delete_all_mission); // 370н удалить миссионные объекты.
	lua_register(L, "obj_delete_all_temp", obj_delete_all_temp); // 370о удалить временные объекты.
	lua_register(L, "obj_delete_all_temp_in_area", obj_delete_all_temp_in_area); // 370п удалить временные в радиусе.
	lua_register(L, "get_obj_attach_force", get_obj_attach_force); // 370р сила прикрепления объекта.
	lua_register(L, "set_obj_attach_force", set_obj_attach_force); // 370с установить силу прикрепления.
	lua_register(L, "fire_find_nearest", fire_find_nearest); // 370т найти ближайший пожар.
	lua_register(L, "fire_start_on_point", fire_start_on_point); // 370у поджечь точку.
	lua_register(L, "get_fire_time_to_burn", get_fire_time_to_burn); // 370ф время горения пожара.
	lua_register(L, "set_fire_time_to_burn", set_fire_time_to_burn); // 370х установить время горения.
	lua_register(L, "get_fire_target", get_fire_target); // 370ц цель пожара.
	lua_register(L, "fire_is_active", fire_is_active); // 370ч пожар активен.
	lua_register(L, "get_fire_position", get_fire_position); // 370ш координаты пожара.
	lua_register(L, "ped_play_anim", ped_play_anim); // 370щ запустить анимацию педа.
	lua_register(L, "ped_blend_anim", ped_blend_anim); // 371а запустить анимацию со смешиванием.
	lua_register(L, "get_anim_block_index", get_anim_block_index); // 371в индекс блока анимаций.
	lua_register(L, "anim_set_group_ref", anim_set_group_ref); // 371г задать группу анимаций.
	lua_register(L, "get_ped_stat_kills", get_ped_stat_kills); // 371д законные убийства игрока.
	lua_register(L, "get_ped_stat_cheated", get_ped_stat_cheated); // 371е число читов игрока.
	lua_register(L, "get_stat_property_destroyed", get_stat_property_destroyed); // 371ж разрушенное имущество.
	lua_register(L, "get_stat_longest_wheelie_dist", get_stat_longest_wheelie_dist); // 371з дистанция вилли.
	lua_register(L, "get_stat_longest_stoppie_dist", get_stat_longest_stoppie_dist); // 371и дистанция стоппи.
	lua_register(L, "get_stat_longest_2wheel_dist", get_stat_longest_2wheel_dist); // 371к дистанция на двух колесах.
	lua_register(L, "get_stat_longest_wheelie_time", get_stat_longest_wheelie_time); // 371л время вилли.
	lua_register(L, "get_stat_seagulls_killed", get_stat_seagulls_killed); // 371м убитые чайки.
	lua_register(L, "get_stat_times_drowned", get_stat_times_drowned); // 371н кол-во утоплений.
	lua_register(L, "get_stat_flight_time", get_stat_flight_time); // 371о время полетов.
	lua_register(L, "get_stat_missions_total", get_stat_missions_total); // 371п общее число миссий.
	lua_register(L, "get_stat_frenzies_total", get_stat_frenzies_total); // 371р общее число безумий.
	lua_register(L, "get_stat_frenzies_passed", get_stat_frenzies_passed); // 371с пройденные безумия.
	lua_register(L, "get_stat_ice_cream_sold", get_stat_ice_cream_sold); // 371т проданное мороженое.
	lua_register(L, "get_stat_pizzas_delivered", get_stat_pizzas_delivered); // 371у доставленные пиццы.
	lua_register(L, "get_stat_assassinations", get_stat_assassinations); // 371ф заказные убийства.
	lua_register(L, "get_stat_movie_stunts", get_stat_movie_stunts); // 371х кинотрюки.
	lua_register(L, "get_stat_stores_knocked_off", get_stat_stores_knocked_off); // 371ц ограбленные магазины.
	lua_register(L, "get_stat_loan_sharks", get_stat_loan_sharks); // 371ч сделки с ростовщиками.
	lua_register(L, "get_stat_garbage_pickups", get_stat_garbage_pickups); // 371ш собранный мусор.
	lua_register(L, "get_stat_weapon_budget", get_stat_weapon_budget); // 371щ бюджет на оружие.
	lua_register(L, "get_stat_fashion_budget", get_stat_fashion_budget); // 372а бюджет на одежду.
	lua_register(L, "get_stat_property_budget", get_stat_property_budget); // 372б бюджет на собственность.
	lua_register(L, "get_stat_auto_painting_budget", get_stat_auto_painting_budget); // 372в бюджет на покраску.
	lua_register(L, "get_stat_bloodring_kills", get_stat_bloodring_kills); // 372г убийства на Bloodring.
	lua_register(L, "get_stat_bloodring_time", get_stat_bloodring_time); // 372д время на Bloodring.
	lua_register(L, "get_stat_shooting_rank", get_stat_shooting_rank); // 372е ранг стрельбы.
	lua_register(L, "get_stat_top_shooting_score", get_stat_top_shooting_score); // 372ж лучший счет в тире.
	lua_register(L, "get_stat_num_property_owned", get_stat_num_property_owned); // 372з кол-во собственности.
	lua_register(L, "radar_set_coord_blip", radar_set_coord_blip); // 372и создать метку на карте.
	lua_register(L, "radar_set_short_range_blip", radar_set_short_range_blip); // 372к короткая метка.
	lua_register(L, "radar_set_entity_blip", radar_set_entity_blip); // 372л метка на сущности.
	lua_register(L, "radar_clear_blip", radar_clear_blip); // 372м удалить метку.
	lua_register(L, "radar_change_blip_colour", radar_change_blip_colour); // 372н цвет метки.
	lua_register(L, "radar_change_blip_brightness", radar_change_blip_brightness); // 372о яркость метки.
	lua_register(L, "radar_change_blip_scale", radar_change_blip_scale); // 372п размер метки.
	lua_register(L, "radar_change_blip_display", radar_change_blip_display); // 372р показ метки.
	lua_register(L, "radar_set_blip_sprite", radar_set_blip_sprite); // 372с иконка метки.
	lua_register(L, "radar_show_marker", radar_show_marker); // 372т показать маркер в мире.
	lua_register(L, "radar_set_marker_state", radar_set_marker_state); // 372у состояние маркера миссии.
	lua_register(L, "radar_draw_radar_sprite", radar_draw_radar_sprite); // 372ф отрисовать спрайт радара.
	lua_register(L, "radar_get_radar_range", radar_get_radar_range); // 372х дальность радара.
	lua_register(L, "radar_set_radar_range", radar_set_radar_range); // 372ц установить дальность радара.
	lua_register(L, "radar_clear_blip_for_entity", radar_clear_blip_for_entity); // 372ч удалить метку сущности.
	lua_register(L, "play_one_off_sound", play_one_off_sound); // 372ш одноразовый звук.
	lua_register(L, "play_continuous_sound", play_continuous_sound); // 372щ непрерывный звук.
	lua_register(L, "remove_sound_by_id", remove_sound_by_id); // 373а удалить звук.
	lua_register(L, "set_radio_channel", set_radio_channel); // 373б радиостанция игроку.
	lua_register(L, "get_radio_channel", get_radio_channel); // 373в текущая радиостанция.
	lua_register(L, "set_music_does_fade", set_music_does_fade); // 373г затухание музыки.
	lua_register(L, "police_radio_message", police_radio_message); // 373д полицейское радио.
	lua_register(L, "create_explosion_no_sound", create_explosion_no_sound); // 373е взрыв без звука.
	lua_register(L, "get_ped_density_multiplier", get_ped_density_multiplier); // 373ж плотность пешеходов.
	lua_register(L, "set_ped_density_multiplier", set_ped_density_multiplier); // 373з установить плотность пешеходов.
	lua_register(L, "get_all_random_peds_type", get_all_random_peds_type); // 373и тип случайных педов.
	lua_register(L, "set_all_random_peds_type", set_all_random_peds_type); // 373к установить тип случайных педов.
	lua_register(L, "population_manage", population_manage); // 373л обработать популяцию.
	lua_register(L, "population_remove_peds_full_pool", population_remove_peds_full_pool); // 373м удалить педов при переполнении.
	lua_register(L, "population_convert_all_objects_to_dummy", population_convert_all_objects_to_dummy); // 373н конвертировать объекты в dummy.
	lua_register(L, "population_is_female", population_is_female); // 373о модель педа женская.
	lua_register(L, "population_is_male", population_is_male); // 373п модель педа мужская.
	lua_register(L, "population_is_sunbather", population_is_sunbather); // 373р модель педа загорающий.
	lua_register(L, "world_find_ground_z_for_coord", world_find_ground_z_for_coord); // 373с высота земли под точкой.
	lua_register(L, "world_find_ground_z_for_3d_coord", world_find_ground_z_for_3d_coord); // 373т высота земли под 3d точкой.
	lua_register(L, "world_find_roof_z_for_3d_coord", world_find_roof_z_for_3d_coord); // 373у высота крыши под точкой.
	lua_register(L, "world_get_is_line_of_sight_clear", world_get_is_line_of_sight_clear); // 373ф прямая видимость между точками.
	lua_register(L, "world_clear_peds_from_area", world_clear_peds_from_area); // 373х удалить педов из области.
	lua_register(L, "world_clear_cars_from_area", world_clear_cars_from_area); // 373ц удалить авто из области.
	lua_register(L, "world_extinguish_all_car_fires_in_area", world_extinguish_all_car_fires_in_area); // 373ч потушить пожары в области.
	lua_register(L, "world_clear_exciting_stuff_from_area", world_clear_exciting_stuff_from_area); // 373ш очистить область от эффектов.
	lua_register(L, "world_call_off_chase_for_area", world_call_off_chase_for_area); // 373щ отменить погоню в области.
	lua_register(L, "world_add_entity", world_add_entity); // 374а добавить сущность в мир.
	lua_register(L, "world_clear_scan_codes", world_clear_scan_codes); // 374б сбросить коды сканирования.
	lua_register(L, "general_atan_of_xy", general_atan_of_xy); // 374в арктангенс от x,y.
	lua_register(L, "general_angle_between_points", general_angle_between_points); // 374г угол между точками.
	lua_register(L, "general_radian_angle_between_points", general_radian_angle_between_points); // 374д угол между точками в радианах.
	lua_register(L, "general_limit_angle", general_limit_angle); // 374е нормализовать угол.
	lua_register(L, "general_limit_radian_angle", general_limit_radian_angle); // 374ж нормализовать угол в радианах.
	lua_register(L, "general_node_heading_from_vector", general_node_heading_from_vector); // 374з направление из вектора.
	lua_register(L, "get_timer_ms", get_timer_ms); // 374и игровое время в мс.
	lua_register(L, "get_timer_ms_pause_mode", get_timer_ms_pause_mode); // 374к время с учетом паузы.
	lua_register(L, "get_frame_counter", get_frame_counter); // 374л номер кадра.
	lua_register(L, "get_time_step", get_time_step); // 374м шаг времени кадра.
	lua_register(L, "is_user_pause", is_user_pause); // 374н игра на паузе.
	lua_register(L, "gangs_set_weapons", gangs_set_weapons); // 374о оружие банды.
	lua_register(L, "gangs_set_vehicle_model", gangs_set_vehicle_model); // 374п авто банды.
	lua_register(L, "gangs_set_ped_models", gangs_set_ped_models); // 374р модели педов банды.
	lua_register(L, "gangs_choose_ped_model", gangs_choose_ped_model); // 374с модель педа банды.
	lua_register(L, "gangs_set_attack_with_cops", gangs_set_attack_with_cops); // 374т банда атакует с копами.
	lua_register(L, "gangs_get_attack_with_cops", gangs_get_attack_with_cops); // 374у банда атакует с копами.
	lua_register(L, "gangs_initialise", gangs_initialise); // 374ф переинициализация банд.
	lua_register(L, "gamelogic_pass_time", gamelogic_pass_time); // 374х пропустить время.
	lua_register(L, "gamelogic_update", gamelogic_update); // 374ц обновить логику.
	lua_register(L, "gamelogic_after_death_arrest_shortcut", gamelogic_after_death_arrest_shortcut); // 374ч такси-шорткат после смерти.
	lua_register(L, "message_add_big", message_add_big); // 374ш большое сообщение.
	lua_register(L, "message_add_big_queue", message_add_big_queue); // 374щ большое сообщение в очередь.
	lua_register(L, "message_add", message_add); // 375а маленькое сообщение.
	lua_register(L, "message_add_jump_queue", message_add_jump_queue); // 375б сообщение вне очереди.
	lua_register(L, "message_add_with_number", message_add_with_number); // 375в сообщение с числом.
	lua_register(L, "message_clear_all", message_clear_all); // 375г очистить все сообщения.
	lua_register(L, "message_clear_small_only", message_clear_small_only); // 375д очистить маленькие сообщения.
	lua_register(L, "message_clear_this_big_print", message_clear_this_big_print); // 375е удалить большое сообщение.
	lua_register(L, "message_clear_this_print", message_clear_this_print); // 375ж удалить маленькое сообщение.
	lua_register(L, "font_set_style", font_set_style); // 375з стиль шрифта.
	lua_register(L, "font_set_drop_shadow", font_set_drop_shadow); // 375и тень шрифта.
	lua_register(L, "font_set_alpha_fade", font_set_alpha_fade); // 375к прозрачность шрифта.
	lua_register(L, "font_set_centre_size", font_set_centre_size); // 375л размер центр. текста.
	lua_register(L, "font_set_wrapx", font_set_wrapx); // 375м ширина переноса шрифта.
	lua_register(L, "font_set_prop_on", font_set_prop_on); // 375н пропорц. шрифт вкл.
	lua_register(L, "font_set_prop_off", font_set_prop_off); // 375о пропорц. шрифт выкл.
	lua_register(L, "font_set_background_on", font_set_background_on); // 375п фон шрифта вкл.
	lua_register(L, "font_set_background_off", font_set_background_off); // 375р фон шрифта выкл.
	lua_register(L, "font_set_right_justify_on", font_set_right_justify_on); // 375с выравнивание вправо вкл.
	lua_register(L, "font_set_right_justify_off", font_set_right_justify_off); // 375т выравнивание вправо выкл.
	lua_register(L, "heli_set_orientation", heli_set_orientation); // 37700 установить ориентацию вертолета
	lua_register(L, "heli_clear_orientation", heli_clear_orientation); // 37701 очистить ориентацию вертолета
	lua_register(L, "heli_set_stabiliser", heli_set_stabiliser); // 37702 установить стабилизатор вертолета
	lua_register(L, "heli_make_crashing_down", heli_make_crashing_down); // 37703 вертолет падает
	lua_register(L, "heli_is_player_in_any_heli", heli_is_player_in_any_heli); // 37704 игрок в любом вертолете
	lua_register(L, "heli_is_char_in_any_heli", heli_is_char_in_any_heli); // 37705 пед в любом вертолете
	lua_register(L, "heli_switch", heli_switch); // 37706 переключить вертолет
	lua_register(L, "catalina_heli_start", catalina_heli_start); // 37707 начать вертолет Catalina
	lua_register(L, "catalina_heli_take_off", catalina_heli_take_off); // 37708 Catalina взлетает
	lua_register(L, "catalina_heli_remove", catalina_heli_remove); // 37709 удалить Catalina
	lua_register(L, "catalina_heli_grab", catalina_heli_grab); // 37710 сесть в Catalina
	lua_register(L, "catalina_heli_fly_away", catalina_heli_fly_away); // 37711 Catalina улетает
	lua_register(L, "catalina_heli_has_shot_down", catalina_heli_has_shot_down); // 37712 Catalina сбит
	lua_register(L, "plane_goto_coords", plane_goto_coords); // 37713 самолет летит на координаты
	lua_register(L, "plane_is_player_in_any_plane", plane_is_player_in_any_plane); // 37714 игрок в любом самолете
	lua_register(L, "plane_is_char_in_any_plane", plane_is_char_in_any_plane); // 37715 пед в любом самолете
	lua_register(L, "find_drug_plane_coords", find_drug_plane_coords); // 37716 координаты самолета с наркотиками
	lua_register(L, "find_drop_off_plane_coords", find_drop_off_plane_coords); // 37717 координаты самолета-доставщика
	lua_register(L, "has_drug_plane_shot_down", has_drug_plane_shot_down); // 37718 самолет с наркотиками сбит
	lua_register(L, "has_drop_off_plane_shot_down", has_drop_off_plane_shot_down); // 37719 самолет-доставщик сбит
	lua_register(L, "boat_goto_coords", boat_goto_coords); // 37720 катер плывет на координаты
	lua_register(L, "boat_set_cruise_speed", boat_set_cruise_speed); // 37721 крейсерская скорость катера
	lua_register(L, "boat_anchor", boat_anchor); // 37722 якорить катер
	lua_register(L, "boat_start_foam_animation", boat_start_foam_animation); // 37723 включить пену катера
	lua_register(L, "boat_update_foam_animation", boat_update_foam_animation); // 37724 обновить пену катера
	lua_register(L, "boat_is_player_in_any_boat", boat_is_player_in_any_boat); // 37725 игрок в любом катере
	lua_register(L, "boat_is_char_in_any_boat", boat_is_char_in_any_boat); // 37726 пед в любом катере
	lua_register(L, "is_vehicle_boat", is_vehicle_boat); // 37727 транспорт это катер
	lua_register(L, "train_catch_char_obj", train_catch_char_obj); // 37728 пед ловит поезд
	lua_register(L, "train_catch_coll_obj", train_catch_coll_obj); // 37729 объект ловит поезд
	lua_register(L, "rc_give_remote_controlled_car_to_player", rc_give_remote_controlled_car_to_player); // 37730 выдать RC-машину
	lua_register(L, "rc_give_remote_controlled_model_to_player", rc_give_remote_controlled_model_to_player); // 37731 выдать RC-модель
	lua_register(L, "rc_get_remote_controlled_car", rc_get_remote_controlled_car); // 37732 получить RC-машину
	lua_register(L, "rc_buggy_blow_up", rc_buggy_blow_up); // 37733 взорвать RC-багги
	lua_register(L, "rc_buggy_remove", rc_buggy_remove); // 37734 удалить RC-багги
	lua_register(L, "rc_detonate_enable", rc_detonate_enable); // 37735 включить детонацию RC
	lua_register(L, "rc_detonate_on_contact_enable", rc_detonate_on_contact_enable); // 37736 детонация RC при контакте
	lua_register(L, "player_is_in_remote_mode", player_is_in_remote_mode); // 37737 игрок в режиме пульта
	lua_register(L, "set_fade_jumpcut_after_rc_explosion", set_fade_jumpcut_after_rc_explosion); // 37738 затухание после взрыва RC
	lua_register(L, "bike_is_player_on_any_bike", bike_is_player_on_any_bike); // 37739 игрок на любом мотоцикле
	lua_register(L, "bike_is_char_on_any_bike", bike_is_char_on_any_bike); // 37740 пед на любом мотоцикле
	lua_register(L, "is_char_in_flying_vehicle", is_char_in_flying_vehicle); // 37741 пед в летающем транспорте
	lua_register(L, "is_player_in_flying_vehicle", is_player_in_flying_vehicle); // 37742 игрок в летающем транспорте
	lua_register(L, "crane_activate", crane_activate); // 37743 активировать кран
	lua_register(L, "crane_is_carrying_car", crane_is_carrying_car); // 37744 кран несет авто
	lua_register(L, "crane_is_targetting_car", crane_is_targetting_car); // 37745 кран целится в авто
	lua_register(L, "escalators_count", escalators_count); // 37746 количество эскалаторов
	lua_register(L, "zone_find_index", zone_find_index); // 37747 найти индекс зоны по имени
	lua_register(L, "zone_get_level_from_position", zone_get_level_from_position); // 37748 уровень зоны по позиции
	lua_register(L, "crusher_crane_activate", crusher_crane_activate); // 37800 активировать пресс-кран
	lua_register(L, "military_crane_activate", military_crane_activate); // 37801 активировать военный кран
	lua_register(L, "crane_deactivate", crane_deactivate); // 37802 деактивировать кран
	lua_register(L, "crane_military_collected_all", crane_military_collected_all); // 37803 военный кран собрал все авто
	lua_register(L, "crane_is_lifting_car", crane_is_lifting_car); // 37804 кран поднимает авто
	lua_register(L, "boat_stop", boat_stop); // 37805 остановить катер
	lua_register(L, "heli_goto_coords", heli_goto_coords); // 37806 вертолет на координаты
	lua_register(L, "heli_create_dust_effect", heli_create_dust_effect); // 37807 пыль катсценного вертолета
	lua_register(L, "car_goto_accurate", car_goto_accurate); // 37808 точная езда на координаты
	lua_register(L, "car_set_idle", car_set_idle); // 37809 авто в ожидание
	lua_register(L, "car_wander_randomly", car_wander_randomly); // 37810 авто едет случайно
	lua_register(L, "car_face_coord", car_face_coord); // 37811 повернуть к координатам
	lua_register(L, "freeze_car_position", freeze_car_position); // 37812 заморозить позицию
	lua_register(L, "freeze_car_no_collide", freeze_car_no_collide); // 37813 заморозить без коллизий
	lua_register(L, "set_car_status", set_car_status); // 37814 статус авто
	lua_register(L, "set_car_mission", set_car_mission); // 37815 миссия авто
	lua_register(L, "car_block_car", car_block_car); // 37816 блокировать авто
	lua_register(L, "car_ram_car", car_ram_car); // 37817 таранить
	lua_register(L, "car_change_lane", car_change_lane); // 37818 смена полосы
	lua_register(L, "car_drive_straight", car_drive_straight); // 37819 ехать прямо
	lua_register(L, "set_car_forward_speed", set_car_forward_speed); // 37820 скорость вперед
	lua_register(L, "car_wait", car_wait); // 37821 приказать ждать ms
	lua_register(L, "car_route_seed", car_route_seed); // 37822 seed маршрута
	lua_register(L, "car_straight_dist", car_straight_dist); // 37823 дистанция прямой линии
	lua_register(L, "car_convoy", car_convoy); // 37824 конвойное авто
	lua_register(L, "car_make_stronger", car_make_stronger); // 37825 усилить авто Крейга
	lua_register(L, "car_to_player", car_to_player); // 37826 авто к игроку
	lua_register(L, "car_fade_in", car_fade_in); // 37827 затухание
	lua_register(L, "car_on_trailer", car_on_trailer); // 37828 на прицеп
	lua_register(L, "set_car_visible", set_car_visible); // 37829 видимость
	lua_register(L, "set_car_watertight", set_car_watertight); // 37830 водонепроницаемость
	lua_register(L, "set_car_big_wheels", set_car_big_wheels); // 37831 большие колеса
	lua_register(L, "set_car_bproof", set_car_bproof); // 37832 пуленепробиваемость
	lua_register(L, "set_car_fproof", set_car_fproof); // 37833 огнеупорность
	lua_register(L, "set_car_rproof", set_car_rproof); // 37834 ракетоупорность
	lua_register(L, "set_car_no_collide", set_car_no_collide); // 37835 без коллизий
	lua_register(L, "set_car_burst_tires", set_car_burst_tires); // 37836 пробиваемость шин
	lua_register(L, "set_car_dmg_player_only", set_car_dmg_player_only); // 37837 урон только от игрока
	lua_register(L, "car_load_collision", car_load_collision); // 37838 загрузка коллизий
	lua_register(L, "car_upside_safe", car_upside_safe); // 37839 перевернутое не ломается
	lua_register(L, "car_change_color", car_change_color); // 37840 сменить цвет
	lua_register(L, "car_set_comp", car_set_comp); // 37841 компоненты модели
	lua_register(L, "set_car_health", set_car_health); // 37842 здоровье авто
	lua_register(L, "get_car_health", get_car_health); // 37843 получить здоровье
	lua_register(L, "get_car_heading", get_car_heading); // 37844 получить угол
	lua_register(L, "get_car_forward_x", get_car_forward_x); // 37845 вектор X
	lua_register(L, "get_car_forward_y", get_car_forward_y); // 37846 вектор Y
	lua_register(L, "is_car_in_air", is_car_in_air); // 37847 в воздухе?
	lua_register(L, "is_car_in_air_proper", is_car_in_air_proper); // 37848 в воздухе точно?
	lua_register(L, "car_health_greater", car_health_greater); // 37849 здоровье больше?
	lua_register(L, "is_car_crushed", is_car_crushed); // 37850 прессовано?
	lua_register(L, "is_car_remap", is_car_remap); // 37851 перекрашено?
	lua_register(L, "is_car_stuck_roof", is_car_stuck_roof); // 37852 на крыше?
	lua_register(L, "car_is_tire_burst", car_is_tire_burst); // 37853 шина пробита?
	lua_register(L, "is_car_visibly_damaged", is_car_visibly_damaged); // 37854 видимо повреждено?
	lua_register(L, "car_waiting_collision", car_waiting_collision); // 37855 ждет коллизии?
	lua_register(L, "is_car_on_trailer", is_car_on_trailer); // 37857 на прицепе?
	lua_register(L, "car_damaged_by_car", car_damaged_by_car); // 37858 повреждено авто?
	lua_register(L, "car_damaged_by_ped", car_damaged_by_ped); // 37859 повреждено педом?
	lua_register(L, "car_just_sunk", car_just_sunk); // 37860 утонуло?
	lua_register(L, "car_drowning", car_drowning); // 37861 тонет?
	lua_register(L, "car_has_weapon", car_has_weapon); // 37862 оружие на борту?
	lua_register(L, "car_any_bomb", car_any_bomb); // 37863 есть бомба?
	lua_register(L, "car_has_bomb", car_has_bomb); // 37864 тип бомбы?
	lua_register(L, "is_car_in_water", is_car_in_water); // 37865 в воде?
	lua_register(L, "fire_button_pressed", fire_button_pressed); // 37866 кнопка огня?
	lua_register(L, "is_car_alive", is_car_alive); // 37867 живо?
	lua_register(L, "does_vehicle_exist", does_vehicle_exist); // 37868 существует?
	lua_register(L, "is_car_in_garage", is_car_in_garage); // 37869 в гараже?
	lua_register(L, "was_ever_police", was_ever_police); // 37870 был полицейским?
	lua_register(L, "clear_car_dmg", clear_car_dmg); // 37871 очистить урон
	lua_register(L, "clear_car_wep_dmg", clear_car_wep_dmg); // 37872 очистить урон оружием
	lua_register(L, "remove_stuck_check", remove_stuck_check); // 37873 убрать проверку застревания
	lua_register(L, "remove_upside_check", remove_upside_check); // 37874 убрать проверку переворота
	lua_register(L, "add_upside_check", add_upside_check); // 37875 добавить проверку переворота
	lua_register(L, "unlock_doors_area", unlock_doors_area); // 37876 открыть двери
	lua_register(L, "close_all_doors", close_all_doors); // 37877 закрыть все двери
	lua_register(L, "close_car_boot", close_car_boot); // 37878 закрыть багажник
	lua_register(L, "pop_car_boot", pop_car_boot); // 37879 открыть багажник
	lua_register(L, "pop_car_boot_physics", pop_car_boot_physics); // 37880 открыть физикой
	lua_register(L, "lock_doors", lock_doors); // 37881 запереть двери
	lua_register(L, "change_car_lock", change_car_lock); // 37882 сменить замок
	lua_register(L, "get_car_driver_ref", get_car_driver_ref); // 37883 водитель
	lua_register(L, "get_char_in_seat", get_char_in_seat); // 37884 пассажир
	lua_register(L, "order_driver_out", order_driver_out); // 37885 выгнать водителя
	lua_register(L, "set_ped_max_drive_speed", set_ped_max_drive_speed); // 37886 макс скорость
	lua_register(L, "set_ped_drive_aggression", set_ped_drive_aggression); // 37887 агрессия
	lua_register(L, "shuffle_to_driver", shuffle_to_driver); // 37888 пересесть за руль
	lua_register(L, "stay_when_jacked", stay_when_jacked); // 37889 не покидать при угоне
	lua_register(L, "is_ped_in_police_car", is_ped_in_police_car); // 37890 в полицейском?
	lua_register(L, "is_ped_touching_vehicle", is_ped_touching_vehicle); // 37891 касается?
	lua_register(L, "is_ped_leaving_to_die", is_ped_leaving_to_die); // 37892 умирает из авто?
	lua_register(L, "is_ped_drowning", is_ped_drowning); // 37893 тонет?
	lua_register(L, "is_ped_in_water", is_ped_in_water); // 37894 в воде?
	lua_register(L, "is_ped_in_zone", is_ped_in_zone); // 37895 в зоне?
	lua_register(L, "is_player_in_zone", is_player_in_zone); // 37896 игрок в зоне?
	lua_register(L, "warp_ped_from_car", warp_ped_from_car); // 37897 телепорт из авто
	lua_register(L, "warp_ped_passenger", warp_ped_passenger); // 37898 в пассажиры
	lua_register(L, "warp_player_from_car", warp_player_from_car); // 37899 телепорт игрока
	lua_register(L, "set_car_ammo", set_car_ammo); // 378100 боезапас
	lua_register(L, "switch_car_weapons", switch_car_weapons); // 378101 переключить оружие
	lua_register(L, "set_car_weapon", set_car_weapon); // 378102 текущее оружие
	lua_register(L, "fire_hunter_gun", fire_hunter_gun); // 378103 выстрел Hunter
	lua_register(L, "set_tank_detonate", set_tank_detonate); // 378104 танк взрывает авто
	lua_register(L, "ensure_drive_by", ensure_drive_by); // 378105 дать drive-by
	lua_register(L, "arm_car_bomb", arm_car_bomb); // 378106 установить бомбу
	lua_register(L, "disarm_car_bomb", disarm_car_bomb); // 378107 обезвредить
	lua_register(L, "give_car_alarm", give_car_alarm); // 378108 сигнализация
	lua_register(L, "switch_car_siren", switch_car_siren); // 378109 сирена
	lua_register(L, "set_driving_style_op", set_driving_style_op); // 378110 стиль вождения (опкод)
	lua_register(L, "set_char_obj_wait", set_char_obj_wait); // 378111 нет задач (ждёт на ногах)
	lua_register(L, "set_char_obj_flee_coord", set_char_obj_flee_coord); // 378112 убежать от точки пешком до безопасности
	lua_register(L, "set_char_obj_guard_spot", set_char_obj_guard_spot); // 378113 охранять точку
	lua_register(L, "set_char_obj_guard_area", set_char_obj_guard_area); // 378114 охранять зону (радиус)
	lua_register(L, "set_char_obj_wait_in_car", set_char_obj_wait_in_car); // 378115 ждать в машине
	lua_register(L, "set_char_obj_leave_car", set_char_obj_leave_car); // 378116 выйти из машины
	lua_register(L, "set_char_obj_enter_car_passenger", set_char_obj_enter_car_passenger); // 378117 сесть в авто как пассажир
	lua_register(L, "set_char_obj_enter_car_driver", set_char_obj_enter_car_driver); // 378118 сесть в авто как водитель
	lua_register(L, "set_char_obj_follow_car_in_car", set_char_obj_follow_car_in_car); // 378119 следовать за авто из авто
	lua_register(L, "set_char_obj_destroy_object", set_char_obj_destroy_object); // 378120 уничтожить объект
	lua_register(L, "set_char_obj_goto_area_in_car", set_char_obj_goto_area_in_car); // 378121 ехать к зоне
	lua_register(L, "set_char_obj_follow_car_foot_offset", set_char_obj_follow_car_foot_offset); // 378122 следовать за авто пешком смещение
	lua_register(L, "set_char_obj_guard_attack", set_char_obj_guard_attack); // 378123 охрана — атаковать противника
	lua_register(L, "set_char_obj_goto_coord_on_foot", set_char_obj_goto_coord_on_foot); // 378124 идти к координатам пешком
	lua_register(L, "set_char_obj_goto_coord_in_car", set_char_obj_goto_coord_in_car); // 378125 ехать к координатам
	lua_register(L, "set_char_obj_run_to_coord", set_char_obj_run_to_coord); // 378126 бежать к координатам
	lua_register(L, "set_char_obj_goto_area_any_means", set_char_obj_goto_area_any_means); // 378127 добираться любыми средствами
	lua_register(L, "set_char_obj_follow_formation", set_char_obj_follow_formation); // 378128 следовать в строю за лидером
	lua_register(L, "set_char_obj_hail_taxi", set_char_obj_hail_taxi); // 378129 вызвать такси
	lua_register(L, "set_char_obj_steal_any_car", set_char_obj_steal_any_car); // 378130 украсть любую машину
	lua_register(L, "set_char_obj_leave_any_car", set_char_obj_leave_any_car); // 378131 выйти из любого авто
	lua_register(L, "set_char_obj_flee_car", set_char_obj_flee_car); // 378132 убежать от машины
	lua_register(L, "set_char_obj_walk_to_char", set_char_obj_walk_to_char); // 378133 идти к другому педу
	lua_register(L, "set_char_obj_steal_any_car_mission", set_char_obj_steal_any_car_mission); // 378134 украсть любую миссионную машину
	lua_register(L, "set_char_obj_sprint_to_coord", set_char_obj_sprint_to_coord); // 378135 спринт к координатам
	lua_register(L, "set_char_obj_goto_car_on_foot", set_char_obj_goto_car_on_foot); // 378136 идти к машине пешком
	lua_register(L, "get_projectile_count", get_projectile_count); // 378137 число активных снарядов
	lua_register(L, "get_projectile_weapon", get_projectile_weapon); // 378138 тип оружия снаряда
	lua_register(L, "get_projectile_source", get_projectile_source); // 378139 источник снаряда (указатель)
	lua_register(L, "get_projectile_pos", get_projectile_pos); // 378140 позиция снаряда x,y,z
	lua_register(L, "set_projectile_pos", set_projectile_pos); // 378141 уст позицию снаряда (направление ракеты)
	lua_register(L, "is_projectile_in_use", is_projectile_in_use); // 378142 слот снаряда занят
	lua_register(L, "remove_all_projectiles", remove_all_projectiles); // 378143 удалить все снаряды
	lua_register(L, "is_projectile_in_area", is_projectile_in_area); // 378144 снаряд в зоне (6 координат)
	lua_register(L, "destroy_projectiles_in_area", destroy_projectiles_in_area); // 378145 уничтожить снаряды в зоне
	lua_register(L, "add_projectile", add_projectile); // 378146 создать снаряд (источник, оружие, x,y,z, сила)
	lua_register(L, "find_car_by_model", find_car_by_model); // 378147 найти первое авто по модели
	lua_register(L, "find_ped_by_model", find_ped_by_model); // 378148 найти первого педа по модели
	lua_register(L, "find_object_by_model", find_object_by_model); // 378149 найти объект по модели
	lua_register(L, "get_vehicle_pool_size", get_vehicle_pool_size); // 378150 размер пула транспорта
	lua_register(L, "get_car_pool_index", get_car_pool_index); // 378151 индекс авто в пуле
	lua_register(L, "get_car_by_pool_index", get_car_by_pool_index); // 378152 авто по индексу в пуле
	lua_register(L, "get_car_velocity", get_car_velocity); // 378153 скорость авто m_vecMoveSpeed (x,y,z)
	lua_register(L, "set_car_velocity", set_car_velocity); // 378154 уст скорость авто m_vecMoveSpeed (x,y,z)
	lua_register(L, "remove_weapon_from_char", remove_weapon_from_char); // 378155 удалить оружие у педа (0x555)
	lua_register(L, "get_forced_weather_type", get_forced_weather_type); // 378156 принудительный тип погоды
	lua_register(L, "is_lightning_burst", is_lightning_burst); // 378157 вспышка молнии (CWeather::LightningBurst)
	lua_register(L, "set_char_obj_kill_char_on_foot", set_char_obj_kill_char_on_foot); // 378158 убить педа пешком (ближний бой)
	lua_register(L, "set_char_obj_kill_char_any_means", set_char_obj_kill_char_any_means); // 378159 убить педа любыми средствами
	lua_register(L, "set_char_obj_flee_char_on_foot_till_safe", set_char_obj_flee_char_on_foot_till_safe); // 378160 убежать от педа до безопасного места
	lua_register(L, "set_char_obj_flee_char_on_foot_always", set_char_obj_flee_char_on_foot_always); // 378161 всегда убегать от педа
	lua_register(L, "set_char_obj_goto_char_on_foot", set_char_obj_goto_char_on_foot); // 378162 идти к педу пешком
	lua_register(L, "set_char_obj_aim_gun_at_char", set_char_obj_aim_gun_at_char); // 378163 целиться из оружия в педа
	lua_register(L, "set_char_obj_kill_player_on_foot", set_char_obj_kill_player_on_foot); // 378164 убить игрока пешком
	lua_register(L, "set_char_obj_kill_player_any_means", set_char_obj_kill_player_any_means); // 378165 убить игрока любыми средствами
	lua_register(L, "set_char_obj_flee_player_on_foot_till_safe", set_char_obj_flee_player_on_foot_till_safe); // 378166 убежать от игрока до безопасного места
	lua_register(L, "set_char_obj_flee_player_on_foot_always", set_char_obj_flee_player_on_foot_always); // 378167 всегда убегать от игрока
	lua_register(L, "set_char_obj_goto_player_on_foot", set_char_obj_goto_player_on_foot); // 378168 идти к игроку пешком
	lua_register(L, "set_char_obj_no_obj", set_char_obj_no_obj); // 378169 снять задачу с педа
	lua_register(L, "set_char_obj_catch_train", set_char_obj_catch_train); // 378170 сесть на поезд
	lua_register(L, "set_char_obj_buy_ice_cream", set_char_obj_buy_ice_cream); // 378171 купить мороженое
	lua_register(L, "set_char_obj_goto_area_on_foot", set_char_obj_goto_area_on_foot); // 378172 идти к зоне пешком
	lua_register(L, "set_char_obj_destroy_car", set_char_obj_destroy_car); // 378173 уничтожить машину
	lua_register(L, "set_char_obj_fire_at_object_from_vehicle", set_char_obj_fire_at_object_from_vehicle); // 378174 стрелять по объекту из авто
	lua_register(L, "set_char_obj_follow_route", set_char_obj_follow_route); // 378175 следовать по маршруту
	lua_register(L, "get_heli_forced_z", get_heli_forced_z); // 378176 принудительная высота вертолёта (зависание)
	lua_register(L, "set_heli_forced_z", set_heli_forced_z); // 378177 уст. принудительная высота вертолёта (зависание)
	lua_register(L, "get_heli_searchlight_x", get_heli_searchlight_x); // 378178 координата X прожектора вертолёта
	lua_register(L, "set_heli_searchlight_x", set_heli_searchlight_x); // 378179 уст. координата X прожектора вертолёта
	lua_register(L, "get_heli_searchlight_y", get_heli_searchlight_y); // 378180 координата Y прожектора вертолёта
	lua_register(L, "set_heli_searchlight_y", set_heli_searchlight_y); // 378181 уст. координата Y прожектора вертолёта
	lua_register(L, "get_heli_searchlight_intensity", get_heli_searchlight_intensity); // 378182 яркость прожектора вертолёта (0..1)
	lua_register(L, "set_heli_searchlight_intensity", set_heli_searchlight_intensity); // 378183 уст. яркость прожектора вертолёта (0..1)
	lua_register(L, "get_heli_rotor_angle", get_heli_rotor_angle); // 378184 угол вращения винта вертолёта
	lua_register(L, "set_heli_rotor_angle", set_heli_rotor_angle); // 378185 уст. угол вращения винта вертолёта
	lua_register(L, "heli_send_down_swat", heli_send_down_swat); // 378186 спецназ спускается с вертолёта
	lua_register(L, "heli_get_num_random", heli_get_num_random); // 378187 число случайных вертолётов
	lua_register(L, "heli_set_num_random", heli_set_num_random); // 378188 уст. число случайных вертолётов
	lua_register(L, "heli_is_catalina_on", heli_is_catalina_on); // 378189 вертолёт Катарины активен
	lua_register(L, "heli_set_catalina_on", heli_set_catalina_on); // 378190 вкл/выкл вертолёт Катарины
	lua_register(L, "get_ped_velocity", get_ped_velocity); // 378191 скорость педа m_vecMoveSpeed
	lua_register(L, "set_ped_velocity", set_ped_velocity); // 378192 уст. скорость педа m_vecMoveSpeed
	lua_register(L, "get_object_velocity", get_object_velocity); // 378193 скорость объекта m_vecMoveSpeed
	lua_register(L, "set_object_velocity", set_object_velocity); // 378194 уст. скорость объекта m_vecMoveSpeed
	lua_register(L, "get_car_speed", get_car_speed); // 378195 скалярная скорость авто (м/с)
	lua_register(L, "get_ped_speed", get_ped_speed); // 378196 скалярная скорость педа (м/с)
	lua_register(L, "get_object_speed", get_object_speed); // 378197 скалярная скорость объекта (м/с)
	lua_register(L, "add_ammo_to_char", add_ammo_to_char); // 378198 добавить патронов педу в оружие
	lua_register(L, "set_char_ammo", set_char_ammo); // 378199 установить патроны педу в оружие
	lua_register(L, "get_ammo_in_char_weapon", get_ammo_in_char_weapon); // 378200 патроны в оружии педа
	lua_register(L, "get_char_weapon_in_slot", get_char_weapon_in_slot); // 378201 оружие в слоте педа
	lua_register(L, "set_player_fast_reload", set_player_fast_reload); // 378202 быстрая перезарядка игроку
	lua_register(L, "is_char_shooting_in_area", is_char_shooting_in_area); // 378203 пед стреляет в зоне
	lua_register(L, "set_char_stop_shoot_dont_seek_entity", set_char_stop_shoot_dont_seek_entity); // 378204 пед перестаёт стрелять и не ищет цель
	lua_register(L, "add_bullet_trace_radius", add_bullet_trace_radius); // 378205 след пули: от точки к точке (радиус, время, прозрачность)
	lua_register(L, "add_bullet_trace_weapon", add_bullet_trace_weapon); // 378206 след пули: оружием и сущностью
	lua_register(L, "get_bullet_trace_count", get_bullet_trace_count); // 378207 число активных следов пуль
	lua_register(L, "get_bullet_trace_origin", get_bullet_trace_origin); // 378208 точка начала следа пули
	lua_register(L, "get_bullet_trace_target", get_bullet_trace_target); // 378209 точка конца следа пули
	lua_register(L, "clear_bullet_traces", clear_bullet_traces); // 378210 удалить все следы пуль
	lua_register(L, "place_3d_marker", place_3d_marker); // 378211 3D-маркер (стрелка/цилиндр)
	lua_register(L, "place_3d_marker_set", place_3d_marker_set); // 378212 набор 3D-маркеров
	lua_register(L, "get_num_active_3d_markers", get_num_active_3d_markers); // 378213 число активных 3D-маркеров
	lua_register(L, "get_pedstat_m_fFleeDistance", get_pedstat_m_fFleeDistance); // 378214 дистанция бегства педа
	lua_register(L, "set_pedstat_m_fFleeDistance", set_pedstat_m_fFleeDistance); // 378215 уст. дистанция бегства педа
	lua_register(L, "get_pedstat_m_fHeadingChangeRate", get_pedstat_m_fHeadingChangeRate); // 378216 скорость поворота педа
	lua_register(L, "set_pedstat_m_fHeadingChangeRate", set_pedstat_m_fHeadingChangeRate); // 378217 уст. скорость поворота педа
	lua_register(L, "get_pedstat_m_fAttackStrength", get_pedstat_m_fAttackStrength); // 378218 сила атаки педа
	lua_register(L, "set_pedstat_m_fAttackStrength", set_pedstat_m_fAttackStrength); // 378219 уст. сила атаки педа
	lua_register(L, "get_pedstat_m_fDefendWeakness", get_pedstat_m_fDefendWeakness); // 378220 слабость защиты педа
	lua_register(L, "set_pedstat_m_fDefendWeakness", set_pedstat_m_fDefendWeakness); // 378221 уст. слабость защиты педа
	lua_register(L, "get_car_door_status", get_car_door_status); // 378222 состояние двери авто
	lua_register(L, "set_car_door_status", set_car_door_status); // 378223 уст. состояние двери авто
	lua_register(L, "get_car_panel_status", get_car_panel_status); // 378224 состояние панели авто
	lua_register(L, "get_car_light_status", get_car_light_status); // 378225 состояние фары авто
	lua_register(L, "set_car_light_status", set_car_light_status); // 378226 уст. состояние фары авто
	lua_register(L, "get_car_wheel_status", get_car_wheel_status); // 378227 состояние колеса авто
	lua_register(L, "set_car_wheel_status", set_car_wheel_status); // 378228 уст. состояние колеса авто
	lua_register(L, "get_car_engine_status", get_car_engine_status); // 378229 состояние двигателя авто
	lua_register(L, "set_car_engine_status", set_car_engine_status); // 378230 уст. состояние двигателя авто
	lua_register(L, "apply_car_damage", apply_car_damage); // 378231 повредить компонент авто
	lua_register(L, "fuck_car_completely", fuck_car_completely); // 378232 уничтожить авто визуально
	lua_register(L, "reset_car_damage", reset_car_damage); // 378233 сбросить повреждения авто
	lua_register(L, "add_AutoPaintingBudget", add_AutoPaintingBudget); // 378234 бюджет на покраску авто
	lua_register(L, "add_PropertyBudget", add_PropertyBudget); // 378235 бюджет на недвижимость
	lua_register(L, "add_FashionBudget", add_FashionBudget); // 378236 бюджет на одежду
	lua_register(L, "add_WeaponBudget", add_WeaponBudget); // 378237 бюджет на оружие
	lua_register(L, "add_BloodRingKills", add_BloodRingKills); // 378238 убийства в Bloodring
	lua_register(L, "add_PizzasDelivered", add_PizzasDelivered); // 378239 доставленные пиццы
	lua_register(L, "add_Assassinations", add_Assassinations); // 378240 заказные убийства
	lua_register(L, "add_MovieStunts", add_MovieStunts); // 378241 киноскунты
	lua_register(L, "add_LoanSharks", add_LoanSharks); // 378242 визиты к ростовщикам
	lua_register(L, "clear_all_char_anims", clear_all_char_anims); // 378243 очистить все анимации педа
	lua_register(L, "clear_char_follow_path", clear_char_follow_path); // 378244 очистить маршрут педа
	lua_register(L, "clear_char_last_damage_entity", clear_char_last_damage_entity); // 378245 забыть последнюю повредившую сущность
	lua_register(L, "clear_char_last_weapon_damage", clear_char_last_weapon_damage); // 378246 забыть последнее оружие урона
	lua_register(L, "char_set_idle", char_set_idle); // 378247 пед в режим ожидания
	lua_register(L, "add_armour_to_char", add_armour_to_char); // 378248 добавить брони педу
	lua_register(L, "add_armour_to_player", add_armour_to_player); // 378249 добавить брони игроку
	lua_register(L, "add_ammo_to_player", add_ammo_to_player); // 378250 добавить патронов игроку
	lua_register(L, "clear_area_of_cars", clear_area_of_cars); // 378251 очистить зону от машин
	lua_register(L, "clear_area_of_chars", clear_area_of_chars); // 378252 очистить зону от педов
	lua_register(L, "get_population_total_peds", get_population_total_peds); // 378253 всего педов в мире
	lua_register(L, "get_population_civ_peds", get_population_civ_peds); // 378254 мирных педов
	lua_register(L, "get_population_gang_peds", get_population_gang_peds); // 378255 бандитских педов
	lua_register(L, "get_population_car_passengers", get_population_car_passengers); // 378256 пассажиров в авто
	lua_register(L, "get_population_mission_peds", get_population_mission_peds); // 378257 миссионных педов
	lua_register(L, "get_population_civ_male", get_population_civ_male); // 378258 мужчин-мирных
	lua_register(L, "get_population_civ_female", get_population_civ_female); // 378259 женщин-мирных
	lua_register(L, "get_population_max_in_use", get_population_max_in_use); // 378260 макс. число используемых педов
	lua_register(L, "get_population_countdown", get_population_countdown); // 378261 обратный отсчёт до спавна педов
	lua_register(L, "get_population_all_random_type", get_population_all_random_type); // 378262 тип всех случайных педов
	lua_register(L, "set_population_all_random_type", set_population_all_random_type); // 378263 уст. тип всех случайных педов
	lua_register(L, "get_hud_m_WantedTimer", get_hud_m_WantedTimer); // 378264 таймер розыска на HUD
	lua_register(L, "set_hud_m_WantedTimer", set_hud_m_WantedTimer); // 378265 таймер розыска на HUD
	lua_register(L, "get_hud_m_WeaponFadeTimer", get_hud_m_WeaponFadeTimer); // 378266 таймер затухания оружия на HUD
	lua_register(L, "set_hud_m_WeaponFadeTimer", set_hud_m_WeaponFadeTimer); // 378267 таймер затухания оружия на HUD
	lua_register(L, "get_hud_m_EnergyLostFadeTimer", get_hud_m_EnergyLostFadeTimer); // 378268 таймер затухания потери энергии
	lua_register(L, "set_hud_m_EnergyLostFadeTimer", set_hud_m_EnergyLostFadeTimer); // 378269 таймер затухания потери энергии
	lua_register(L, "get_hud_m_DisplayScoreFadeTimer", get_hud_m_DisplayScoreFadeTimer); // 378270 таймер затухания счёта
	lua_register(L, "set_hud_m_DisplayScoreFadeTimer", set_hud_m_DisplayScoreFadeTimer); // 378271 таймер затухания счёта
	lua_register(L, "get_hud_wanted_state", get_hud_wanted_state); // 378272 состояние розыска на HUD
	lua_register(L, "get_hud_help_message_state", get_hud_help_message_state); // 378273 состояние подсказки на HUD
	lua_register(L, "get_player_taxi_timer", get_player_taxi_timer); // 378274 таймер такси игрока
	lua_register(L, "get_player_death_fade_timer", get_player_death_fade_timer); // 378275 таймер затухания при смерти
	lua_register(L, "get_player_2wheels_time", get_player_2wheels_time); // 378276 время на двух колёсах (мс)
	lua_register(L, "get_player_2wheels_distance", get_player_2wheels_distance); // 378277 дистанция на двух колёсах
	lua_register(L, "get_player_road_density", get_player_road_density); // 378278 плотность дорог вокруг игрока
	lua_register(L, "set_player_road_density", set_player_road_density); // 378279 уст. плотность дорог вокруг игрока
	lua_register(L, "get_pedik_m_fYaw", get_pedik_m_fYaw); // 378280 угол наклона головы педа
	lua_register(L, "set_pedik_m_fYaw", set_pedik_m_fYaw); // 378281 уст. угол наклона головы педа
	lua_register(L, "get_pedik_m_fPitch", get_pedik_m_fPitch); // 378282 наклон головы педа по тангажу
	lua_register(L, "set_pedik_m_fPitch", set_pedik_m_fPitch); // 378283 уст. наклон головы педа по тангажу
	lua_register(L, "get_boat_m_fPropRotation", get_boat_m_fPropRotation); // 378284 вращение винта лодки
	lua_register(L, "set_boat_m_fPropRotation", set_boat_m_fPropRotation); // 378285 уст. вращение винта лодки
	lua_register(L, "get_boat_m_fPropSpeed", get_boat_m_fPropSpeed); // 378286 скорость винта лодки
	lua_register(L, "set_boat_m_fPropSpeed", set_boat_m_fPropSpeed); // 378287 уст. скорость винта лодки
	lua_register(L, "get_boat_m_fForcedZRotation", get_boat_m_fForcedZRotation); // 378288 принудительный поворот лодки по Z
	lua_register(L, "set_boat_m_fForcedZRotation", set_boat_m_fForcedZRotation); // 378289 уст. принудительный поворот лодки по Z
	lua_register(L, "get_boat_m_fBoatGasPedal", get_boat_m_fBoatGasPedal); // 378290 газ лодки
	lua_register(L, "set_boat_m_fBoatGasPedal", set_boat_m_fBoatGasPedal); // 378291 уст. газ лодки
	lua_register(L, "get_boat_m_fBoatSteeringLeftRight", get_boat_m_fBoatSteeringLeftRight); // 378292 руль лодки
	lua_register(L, "set_boat_m_fBoatSteeringLeftRight", set_boat_m_fBoatSteeringLeftRight); // 378293 уст. руль лодки
	lua_register(L, "get_bike_anim_group", get_bike_anim_group); // 378294 группа анимаций мотоцикла
	lua_register(L, "set_bike_anim_group", set_bike_anim_group); // 378295 уст. группу анимаций мотоцикла
	lua_register(L, "get_bike_damage_flags", get_bike_damage_flags); // 378296 флаги повреждений мотоцикла
	lua_register(L, "set_bike_damage_flags", set_bike_damage_flags); // 378297 уст. флаги повреждений мотоцикла
	lua_register(L, "get_time_step_non_clipped", get_time_step_non_clipped); // 378298 шаг времени без клиппинга
	lua_register(L, "get_previous_time_ms", get_previous_time_ms); // 378299 предыдущее время в мс
	lua_register(L, "get_time_ms_non_clipped", get_time_ms_non_clipped); // 378300 время в мс без клиппинга
	lua_register(L, "is_slow_motion_active", is_slow_motion_active); // 378301 активен ли слоумоушен
	lua_register(L, "get_cycles_per_millisecond", get_cycles_per_millisecond); // 378302 циклы на миллисекунду
	lua_register(L, "get_clock_hours", get_clock_hours); // 378303 часы игрового таймера
	lua_register(L, "get_clock_minutes", get_clock_minutes); // 378304 минуты игрового таймера
	lua_register(L, "get_clock_seconds", get_clock_seconds); // 378305 секунды игрового таймера
	lua_register(L, "get_clock_stored_hours", get_clock_stored_hours); // 378306 сохранённые часы
	lua_register(L, "get_clock_stored_minutes", get_clock_stored_minutes); // 378307 сохранённые минуты
	lua_register(L, "get_clock_minutes_until", get_clock_minutes_until); // 378308 минут до заданного времени
	lua_register(L, "restore_clock", restore_clock); // 378309 восстановить сохранённое время
	lua_register(L, "get_streaming_disabled", get_streaming_disabled); // 378310 стриминг отключён
	lua_register(L, "set_streaming_disabled", set_streaming_disabled); // 378311 отключить стриминг моделей
	lua_register(L, "get_streaming_models_requested", get_streaming_models_requested); // 378312 запрошено моделей
	lua_register(L, "get_streaming_priority_requests", get_streaming_priority_requests); // 378313 приоритетных запросов
	lua_register(L, "get_streaming_image_size", get_streaming_image_size); // 378314 размер образа стриминга
	lua_register(L, "get_streaming_buffer_size", get_streaming_buffer_size); // 378315 размер буфера стриминга
	lua_register(L, "get_streaming_current_ped_grp", get_streaming_current_ped_grp); // 378316 текущая группа педов
	lua_register(L, "get_streaming_loading_big_model", get_streaming_loading_big_model); // 378317 загружается большая модель
	lua_register(L, "get_streaming_peds_loaded", get_streaming_peds_loaded); // 378318 загружено педов
	lua_register(L, "get_streaming_channel_error", get_streaming_channel_error); // 378319 ошибка канала стриминга
	lua_register(L, "get_streaming_last_image_read", get_streaming_last_image_read); // 378320 последний прочитанный образ
	lua_register(L, "get_streaming_old_sector_x", get_streaming_old_sector_x); // 378321 старый сектор X
	lua_register(L, "get_streaming_old_sector_y", get_streaming_old_sector_y); // 378322 старый сектор Y
	lua_register(L, "get_zone_level", get_zone_level); // 378323 остров по координатам
	lua_register(L, "find_information_zone", find_information_zone); // 378324 индекс информационной зоны
	lua_register(L, "get_navigation_zone", get_navigation_zone); // 378325 навигационная зона по индексу
	lua_register(L, "init_zones", init_zones); // 378326 инициализировать зоны
	lua_register(L, "traffic_lights_for_peds", traffic_lights_for_peds); // 378327 светофор для пешеходов
	lua_register(L, "traffic_scan_for_lights", traffic_scan_for_lights); // 378328 сканировать светофоры
	lua_register(L, "traffic_should_car_stop_for_bridge", traffic_should_car_stop_for_bridge); // 378329 машина должна остановиться у моста
	lua_register(L, "should_car_stop_for_light", should_car_stop_for_light); // 378330 должна ли машина остановиться на свет
	lua_register(L, "add_gun_flash_big", add_gun_flash_big); // 378331 вспышка выстрела крупного оружия
	lua_register(L, "initialise_weapons", initialise_weapons); // 378332 инициализировать оружие
	lua_register(L, "shutdown_weapons", shutdown_weapons); // 378333 завершить работу оружия
	lua_register(L, "update_weapons", update_weapons); // 378334 обновить оружие
	lua_register(L, "do_doom_aiming", do_doom_aiming); // 378335 аим-траверс для оружия
	lua_register(L, "create_some_money", create_some_money); // 378336 создать деньги на координатах
	lua_register(L, "give_player_goodies", give_player_goodies); // 378337 выдать игроку бонусы по модели
	lua_register(L, "do_pickup_effects", do_pickup_effects); // 378338 эффект поднятия пикапа
	lua_register(L, "do_money_effects", do_money_effects); // 378339 эффект денег
	lua_register(L, "do_mine_effects", do_mine_effects); // 378340 эффект мины
	lua_register(L, "get_obj_m_fAttachForce", get_obj_m_fAttachForce); // 378341 сила привязки объекта
	lua_register(L, "set_obj_m_fAttachForce", set_obj_m_fAttachForce); // 378342 уст. сила привязки объекта
	lua_register(L, "get_obj_m_CollisionDamageType", get_obj_m_CollisionDamageType); // 378343 тип урона от столкновений
	lua_register(L, "set_obj_m_CollisionDamageType", set_obj_m_CollisionDamageType); // 378344 уст. тип урона от столкновений
	lua_register(L, "get_obj_m_nSpecialCollisionType", get_obj_m_nSpecialCollisionType); // 378345 специальный тип столкновений
	lua_register(L, "set_obj_m_nSpecialCollisionType", set_obj_m_nSpecialCollisionType); // 378346 уст. специальный тип столкновений
	lua_register(L, "get_obj_m_nBounceScore", get_obj_m_nBounceScore); // 378347 счёт отскока объекта
	lua_register(L, "set_obj_m_nBounceScore", set_obj_m_nBounceScore); // 378348 уст. счёт отскока объекта
	lua_register(L, "get_obj_m_dwObjectTimer", get_obj_m_dwObjectTimer); // 378349 таймер объекта
	lua_register(L, "set_obj_m_dwObjectTimer", set_obj_m_dwObjectTimer); // 378350 уст. таймер объекта
	lua_register(L, "get_radar_range", get_radar_range); // 378351 дальность радара
	lua_register(L, "set_radar_range", set_radar_range); // 378352 уст. дальность радара
	lua_register(L, "set_sun_blocked_by_clouds", set_sun_blocked_by_clouds); // 378353 уст. солнце закрыто облаками
	lua_register(L, "update_corona_coors", update_corona_coors); // 378354 обновить координаты короны
	lua_register(L, "is_cutscene_running", is_cutscene_running); // 378355 катсцена идёт
	lua_register(L, "get_num_cutscene_objs", get_num_cutscene_objs); // 378356 число объектов катсцены
	lua_register(L, "get_cutscene_time_ms", get_cutscene_time_ms); // 378357 время катсцены в мс
	lua_register(L, "choose_police_car_model", choose_police_car_model); // 378358 выбрать модель полицейской машины
	lua_register(L, "generate_emergency_services", generate_emergency_services); // 378359 вызвать экстренные службы
	lua_register(L, "clear_interesting_vehicles", clear_interesting_vehicles); // 378360 очистить список интересных машин
	lua_register(L, "drag_car_to_point", drag_car_to_point); // 378361 тащить машину к точке
	lua_register(L, "find_max_speed_in_traffic", find_max_speed_in_traffic); // 378362 макс. скорость машины в трафике
	lua_register(L, "get_num_bright_lights", get_num_bright_lights); // 378363 число ярких огней
	lua_register(L, "get_car_m_nPrimaryColor", get_car_m_nPrimaryColor); // 378367 m_nPrimaryColor (CVehicle)
	lua_register(L, "set_car_m_nPrimaryColor", set_car_m_nPrimaryColor); // 378368 уст. m_nPrimaryColor (CVehicle)
	lua_register(L, "get_car_m_nSecondaryColor", get_car_m_nSecondaryColor); // 378369 m_nSecondaryColor (CVehicle)
	lua_register(L, "set_car_m_nSecondaryColor", set_car_m_nSecondaryColor); // 378370 уст. m_nSecondaryColor (CVehicle)
	lua_register(L, "get_car_m_wWantedStarsOnEnter", get_car_m_wWantedStarsOnEnter); // 378371 m_wWantedStarsOnEnter (CVehicle)
	lua_register(L, "set_car_m_wWantedStarsOnEnter", set_car_m_wWantedStarsOnEnter); // 378372 уст. m_wWantedStarsOnEnter (CVehicle)
	lua_register(L, "get_car_m_wMissionValue", get_car_m_wMissionValue); // 378373 m_wMissionValue (CVehicle)
	lua_register(L, "set_car_m_wMissionValue", set_car_m_wMissionValue); // 378374 уст. m_wMissionValue (CVehicle)
	lua_register(L, "get_car_m_nNumPassengers", get_car_m_nNumPassengers); // 378375 m_nNumPassengers (CVehicle)
	lua_register(L, "set_car_m_nNumPassengers", set_car_m_nNumPassengers); // 378376 уст. m_nNumPassengers (CVehicle)
	lua_register(L, "get_car_m_nNumGettingIn", get_car_m_nNumGettingIn); // 378377 m_nNumGettingIn (CVehicle)
	lua_register(L, "set_car_m_nNumGettingIn", set_car_m_nNumGettingIn); // 378378 уст. m_nNumGettingIn (CVehicle)
	lua_register(L, "get_car_m_nGettingInFlags", get_car_m_nGettingInFlags); // 378379 m_nGettingInFlags (CVehicle)
	lua_register(L, "set_car_m_nGettingInFlags", set_car_m_nGettingInFlags); // 378380 уст. m_nGettingInFlags (CVehicle)
	lua_register(L, "get_car_m_nGettingOutFlags", get_car_m_nGettingOutFlags); // 378381 m_nGettingOutFlags (CVehicle)
	lua_register(L, "set_car_m_nGettingOutFlags", set_car_m_nGettingOutFlags); // 378382 уст. m_nGettingOutFlags (CVehicle)
	lua_register(L, "get_car_m_nMaxPassengers", get_car_m_nMaxPassengers); // 378383 m_nMaxPassengers (CVehicle)
	lua_register(L, "set_car_m_nMaxPassengers", set_car_m_nMaxPassengers); // 378384 уст. m_nMaxPassengers (CVehicle)
	lua_register(L, "get_car_m_fSteerAngle", get_car_m_fSteerAngle); // 378385 m_fSteerAngle (CVehicle)
	lua_register(L, "set_car_m_fSteerAngle", set_car_m_fSteerAngle); // 378386 уст. m_fSteerAngle (CVehicle)
	lua_register(L, "get_car_m_fGasPedal", get_car_m_fGasPedal); // 378387 m_fGasPedal (CVehicle)
	lua_register(L, "set_car_m_fGasPedal", set_car_m_fGasPedal); // 378388 уст. m_fGasPedal (CVehicle)
	lua_register(L, "get_car_m_fBreakPedal", get_car_m_fBreakPedal); // 378389 m_fBreakPedal (CVehicle)
	lua_register(L, "set_car_m_fBreakPedal", set_car_m_fBreakPedal); // 378390 уст. m_fBreakPedal (CVehicle)
	lua_register(L, "get_car_m_nCreatedBy", get_car_m_nCreatedBy); // 378391 m_nCreatedBy (CVehicle)
	lua_register(L, "set_car_m_nCreatedBy", set_car_m_nCreatedBy); // 378392 уст. m_nCreatedBy (CVehicle)
	lua_register(L, "get_car_m_nAmmoInClip", get_car_m_nAmmoInClip); // 378393 m_nAmmoInClip (CVehicle)
	lua_register(L, "set_car_m_nAmmoInClip", set_car_m_nAmmoInClip); // 378394 уст. m_nAmmoInClip (CVehicle)
	lua_register(L, "get_car_m_fHealth", get_car_m_fHealth); // 378395 m_fHealth (CVehicle)
	lua_register(L, "set_car_m_fHealth", set_car_m_fHealth); // 378396 уст. m_fHealth (CVehicle)
	lua_register(L, "get_car_m_nCurrentGear", get_car_m_nCurrentGear); // 378397 m_nCurrentGear (CVehicle)
	lua_register(L, "set_car_m_nCurrentGear", set_car_m_nCurrentGear); // 378398 уст. m_nCurrentGear (CVehicle)
	lua_register(L, "get_car_m_nTimeTillWeNeedThisCar", get_car_m_nTimeTillWeNeedThisCar); // 378399 m_nTimeTillWeNeedThisCar (CVehicle)
	lua_register(L, "set_car_m_nTimeTillWeNeedThisCar", set_car_m_nTimeTillWeNeedThisCar); // 378400 уст. m_nTimeTillWeNeedThisCar (CVehicle)
	lua_register(L, "get_car_m_nTimeOfDeath", get_car_m_nTimeOfDeath); // 378401 m_nTimeOfDeath (CVehicle)
	lua_register(L, "set_car_m_nTimeOfDeath", set_car_m_nTimeOfDeath); // 378402 уст. m_nTimeOfDeath (CVehicle)
	lua_register(L, "get_car_m_wBombTimer", get_car_m_wBombTimer); // 378403 m_wBombTimer (CVehicle)
	lua_register(L, "set_car_m_wBombTimer", set_car_m_wBombTimer); // 378404 уст. m_wBombTimer (CVehicle)
	lua_register(L, "get_car_m_nLastWeaponDamage", get_car_m_nLastWeaponDamage); // 378405 m_nLastWeaponDamage (CVehicle)
	lua_register(L, "set_car_m_nLastWeaponDamage", set_car_m_nLastWeaponDamage); // 378406 уст. m_nLastWeaponDamage (CVehicle)
	lua_register(L, "get_car_m_nRadioStation", get_car_m_nRadioStation); // 378407 m_nRadioStation (CVehicle)
	lua_register(L, "set_car_m_nRadioStation", set_car_m_nRadioStation); // 378408 уст. m_nRadioStation (CVehicle)
	lua_register(L, "get_car_m_bHornEnabled", get_car_m_bHornEnabled); // 378409 m_bHornEnabled (CVehicle)
	lua_register(L, "set_car_m_bHornEnabled", set_car_m_bHornEnabled); // 378410 уст. m_bHornEnabled (CVehicle)
	lua_register(L, "get_car_m_nSirenOrAlarm", get_car_m_nSirenOrAlarm); // 378411 m_nSirenOrAlarm (CVehicle)
	lua_register(L, "set_car_m_nSirenOrAlarm", set_car_m_nSirenOrAlarm); // 378412 уст. m_nSirenOrAlarm (CVehicle)
	lua_register(L, "get_car_m_nSirenExtra", get_car_m_nSirenExtra); // 378413 m_nSirenExtra (CVehicle)
	lua_register(L, "set_car_m_nSirenExtra", set_car_m_nSirenExtra); // 378414 уст. m_nSirenExtra (CVehicle)
	lua_register(L, "get_car_m_fSteerRatio", get_car_m_fSteerRatio); // 378415 m_fSteerRatio (CVehicle)
	lua_register(L, "set_car_m_fSteerRatio", set_car_m_fSteerRatio); // 378416 уст. m_fSteerRatio (CVehicle)
	lua_register(L, "get_car_m_nVehicleClass", get_car_m_nVehicleClass); // 378417 m_nVehicleClass (CVehicle)
	lua_register(L, "set_car_m_nVehicleClass", set_car_m_nVehicleClass); // 378418 уст. m_nVehicleClass (CVehicle)
	lua_register(L, "get_ped_m_fCollisionSpeed", get_ped_m_fCollisionSpeed); // 378419 m_fCollisionSpeed (CPed)
	lua_register(L, "set_ped_m_fCollisionSpeed", set_ped_m_fCollisionSpeed); // 378420 уст. m_fCollisionSpeed (CPed)
	lua_register(L, "get_ped_m_nGangFlags", get_ped_m_nGangFlags); // 378421 m_nGangFlags (CPed)
	lua_register(L, "set_ped_m_nGangFlags", set_ped_m_nGangFlags); // 378422 уст. m_nGangFlags (CPed)
	lua_register(L, "get_ped_m_nPedStatus", get_ped_m_nPedStatus); // 378423 m_nPedStatus (CPed)
	lua_register(L, "set_ped_m_nPedStatus", set_ped_m_nPedStatus); // 378424 уст. m_nPedStatus (CPed)
	lua_register(L, "get_ped_m_vecObjective", get_ped_m_vecObjective); // 378425 m_vecObjective (CPed)
	lua_register(L, "set_ped_m_vecObjective", set_ped_m_vecObjective); // 378426 уст. m_vecObjective (CPed)
	lua_register(L, "get_ped_m_fObjectiveAngle", get_ped_m_fObjectiveAngle); // 378427 m_fObjectiveAngle (CPed)
	lua_register(L, "set_ped_m_fObjectiveAngle", set_ped_m_fObjectiveAngle); // 378428 уст. m_fObjectiveAngle (CPed)
	lua_register(L, "get_ped_m_nPedFormation", get_ped_m_nPedFormation); // 378429 m_nPedFormation (CPed)
	lua_register(L, "set_ped_m_nPedFormation", set_ped_m_nPedFormation); // 378430 уст. m_nPedFormation (CPed)
	lua_register(L, "get_ped_m_nFearFlags", get_ped_m_nFearFlags); // 378431 m_nFearFlags (CPed)
	lua_register(L, "set_ped_m_nFearFlags", set_ped_m_nFearFlags); // 378432 уст. m_nFearFlags (CPed)
	lua_register(L, "get_ped_m_nEventType", get_ped_m_nEventType); // 378433 m_nEventType (CPed)
	lua_register(L, "set_ped_m_nEventType", set_ped_m_nEventType); // 378434 уст. m_nEventType (CPed)
	lua_register(L, "get_ped_m_fAngleToEvent", get_ped_m_fAngleToEvent); // 378435 m_fAngleToEvent (CPed)
	lua_register(L, "set_ped_m_fAngleToEvent", set_ped_m_fAngleToEvent); // 378436 уст. m_fAngleToEvent (CPed)
	lua_register(L, "get_ped_m_nAnimGroupId", get_ped_m_nAnimGroupId); // 378437 m_nAnimGroupId (CPed)
	lua_register(L, "set_ped_m_nAnimGroupId", set_ped_m_nAnimGroupId); // 378438 уст. m_nAnimGroupId (CPed)
	lua_register(L, "get_ped_m_vecOffsetSeek", get_ped_m_vecOffsetSeek); // 378439 m_vecOffsetSeek (CPed)
	lua_register(L, "set_ped_m_vecOffsetSeek", set_ped_m_vecOffsetSeek); // 378440 уст. m_vecOffsetSeek (CPed)
	lua_register(L, "get_ped_m_nActionTimer", get_ped_m_nActionTimer); // 378441 m_nActionTimer (CPed)
	lua_register(L, "set_ped_m_nActionTimer", set_ped_m_nActionTimer); // 378442 уст. m_nActionTimer (CPed)
	lua_register(L, "get_ped_m_nWaitState", get_ped_m_nWaitState); // 378443 m_nWaitState (CPed)
	lua_register(L, "set_ped_m_nWaitState", set_ped_m_nWaitState); // 378444 уст. m_nWaitState (CPed)
	lua_register(L, "get_ped_m_nWaitTimer", get_ped_m_nWaitTimer); // 378445 m_nWaitTimer (CPed)
	lua_register(L, "set_ped_m_nWaitTimer", set_ped_m_nWaitTimer); // 378446 уст. m_nWaitTimer (CPed)
	lua_register(L, "get_ped_m_wPathNodes", get_ped_m_wPathNodes); // 378447 m_wPathNodes (CPed)
	lua_register(L, "set_ped_m_wPathNodes", set_ped_m_wPathNodes); // 378448 уст. m_wPathNodes (CPed)
	lua_register(L, "get_ped_m_wCurPathNode", get_ped_m_wCurPathNode); // 378449 m_wCurPathNode (CPed)
	lua_register(L, "set_ped_m_wCurPathNode", set_ped_m_wCurPathNode); // 378450 уст. m_wCurPathNode (CPed)
	lua_register(L, "get_ped_m_nPathNodeTimer", get_ped_m_nPathNodeTimer); // 378451 m_nPathNodeTimer (CPed)
	lua_register(L, "set_ped_m_nPathNodeTimer", set_ped_m_nPathNodeTimer); // 378452 уст. m_nPathNodeTimer (CPed)
	lua_register(L, "get_ped_m_vecPathNextNode", get_ped_m_vecPathNextNode); // 378453 m_vecPathNextNode (CPed)
	lua_register(L, "set_ped_m_vecPathNextNode", set_ped_m_vecPathNextNode); // 378454 уст. m_vecPathNextNode (CPed)
	lua_register(L, "get_ped_m_fPathNextNodeDir", get_ped_m_fPathNextNodeDir); // 378455 m_fPathNextNodeDir (CPed)
	lua_register(L, "set_ped_m_fPathNextNodeDir", set_ped_m_fPathNextNodeDir); // 378456 уст. m_fPathNextNodeDir (CPed)
	lua_register(L, "get_ped_m_nPathNodeType", get_ped_m_nPathNodeType); // 378457 m_nPathNodeType (CPed)
	lua_register(L, "set_ped_m_nPathNodeType", set_ped_m_nPathNodeType); // 378458 уст. m_nPathNodeType (CPed)
	lua_register(L, "get_ped_m_fHealth", get_ped_m_fHealth); // 378459 m_fHealth (CPed)
	lua_register(L, "set_ped_m_fHealth", set_ped_m_fHealth); // 378460 уст. m_fHealth (CPed)
	lua_register(L, "get_ped_m_fArmour", get_ped_m_fArmour); // 378461 m_fArmour (CPed)
	lua_register(L, "set_ped_m_fArmour", set_ped_m_fArmour); // 378462 уст. m_fArmour (CPed)
	lua_register(L, "get_ped_m_nShadowUpdateTimer", get_ped_m_nShadowUpdateTimer); // 378463 m_nShadowUpdateTimer (CPed)
	lua_register(L, "set_ped_m_nShadowUpdateTimer", set_ped_m_nShadowUpdateTimer); // 378464 уст. m_nShadowUpdateTimer (CPed)
	lua_register(L, "get_ped_wRouteLastPoint", get_ped_wRouteLastPoint); // 378465 wRouteLastPoint (CPed)
	lua_register(L, "set_ped_wRouteLastPoint", set_ped_wRouteLastPoint); // 378466 уст. wRouteLastPoint (CPed)
	lua_register(L, "get_ped_wRoutePoints", get_ped_wRoutePoints); // 378467 wRoutePoints (CPed)
	lua_register(L, "set_ped_wRoutePoints", set_ped_wRoutePoints); // 378468 уст. wRoutePoints (CPed)
	lua_register(L, "get_ped_wRoutePos", get_ped_wRoutePos); // 378469 wRoutePos (CPed)
	lua_register(L, "set_ped_wRoutePos", set_ped_wRoutePos); // 378470 уст. wRoutePos (CPed)
	lua_register(L, "get_ped_wRouteType", get_ped_wRouteType); // 378471 wRouteType (CPed)
	lua_register(L, "set_ped_wRouteType", set_ped_wRouteType); // 378472 уст. wRouteType (CPed)
	lua_register(L, "get_ped_wRouteCurDir", get_ped_wRouteCurDir); // 378473 wRouteCurDir (CPed)
	lua_register(L, "set_ped_wRouteCurDir", set_ped_wRouteCurDir); // 378474 уст. wRouteCurDir (CPed)
	lua_register(L, "get_ped_m_fHeadingCurrent", get_ped_m_fHeadingCurrent); // 378475 m_fHeadingCurrent (CPed)
	lua_register(L, "set_ped_m_fHeadingCurrent", set_ped_m_fHeadingCurrent); // 378476 уст. m_fHeadingCurrent (CPed)
	lua_register(L, "get_ped_m_fHeadingGoal", get_ped_m_fHeadingGoal); // 378477 m_fHeadingGoal (CPed)
	lua_register(L, "set_ped_m_fHeadingGoal", set_ped_m_fHeadingGoal); // 378478 уст. m_fHeadingGoal (CPed)
	lua_register(L, "get_ped_m_fHeadingChangeRate", get_ped_m_fHeadingChangeRate); // 378479 m_fHeadingChangeRate (CPed)
	lua_register(L, "set_ped_m_fHeadingChangeRate", set_ped_m_fHeadingChangeRate); // 378480 уст. m_fHeadingChangeRate (CPed)
	lua_register(L, "get_ped_m_nEnterType", get_ped_m_nEnterType); // 378481 m_nEnterType (CPed)
	lua_register(L, "set_ped_m_nEnterType", set_ped_m_nEnterType); // 378482 уст. m_nEnterType (CPed)
	lua_register(L, "get_ped_m_nWalkAroundType", get_ped_m_nWalkAroundType); // 378483 m_nWalkAroundType (CPed)
	lua_register(L, "set_ped_m_nWalkAroundType", set_ped_m_nWalkAroundType); // 378484 уст. m_nWalkAroundType (CPed)
	lua_register(L, "get_ped_m_vecOffsetFromPhysSurface", get_ped_m_vecOffsetFromPhysSurface); // 378485 m_vecOffsetFromPhysSurface (CPed)
	lua_register(L, "set_ped_m_vecOffsetFromPhysSurface", set_ped_m_vecOffsetFromPhysSurface); // 378486 уст. m_vecOffsetFromPhysSurface (CPed)
	lua_register(L, "get_ped_vecSeekVehicle", get_ped_vecSeekVehicle); // 378487 vecSeekVehicle (CPed)
	lua_register(L, "set_ped_vecSeekVehicle", set_ped_vecSeekVehicle); // 378488 уст. vecSeekVehicle (CPed)
	lua_register(L, "get_ped_m_bInVehicle", get_ped_m_bInVehicle); // 378489 m_bInVehicle (CPed)
	lua_register(L, "set_ped_m_bInVehicle", set_ped_m_bInVehicle); // 378490 уст. m_bInVehicle (CPed)
	lua_register(L, "get_ped_m_fSeatPrecisionX", get_ped_m_fSeatPrecisionX); // 378491 m_fSeatPrecisionX (CPed)
	lua_register(L, "set_ped_m_fSeatPrecisionX", set_ped_m_fSeatPrecisionX); // 378492 уст. m_fSeatPrecisionX (CPed)
	lua_register(L, "get_ped_m_fSeatPrecisionY", get_ped_m_fSeatPrecisionY); // 378493 m_fSeatPrecisionY (CPed)
	lua_register(L, "set_ped_m_fSeatPrecisionY", set_ped_m_fSeatPrecisionY); // 378494 уст. m_fSeatPrecisionY (CPed)
	lua_register(L, "get_ped_m_nSeatType", get_ped_m_nSeatType); // 378495 m_nSeatType (CPed)
	lua_register(L, "set_ped_m_nSeatType", set_ped_m_nSeatType); // 378496 уст. m_nSeatType (CPed)
	lua_register(L, "get_ped_m_bHasPhone", get_ped_m_bHasPhone); // 378497 m_bHasPhone (CPed)
	lua_register(L, "set_ped_m_bHasPhone", set_ped_m_bHasPhone); // 378498 уст. m_bHasPhone (CPed)
	lua_register(L, "get_ped_m_wPhoneId", get_ped_m_wPhoneId); // 378499 m_wPhoneId (CPed)
	lua_register(L, "set_ped_m_wPhoneId", set_ped_m_wPhoneId); // 378500 уст. m_wPhoneId (CPed)
	lua_register(L, "get_ped_m_nLookingForPhone", get_ped_m_nLookingForPhone); // 378501 m_nLookingForPhone (CPed)
	lua_register(L, "set_ped_m_nLookingForPhone", set_ped_m_nLookingForPhone); // 378502 уст. m_nLookingForPhone (CPed)
	lua_register(L, "get_ped_m_nPhoneTalkTimer", get_ped_m_nPhoneTalkTimer); // 378503 m_nPhoneTalkTimer (CPed)
	lua_register(L, "set_ped_m_nPhoneTalkTimer", set_ped_m_nPhoneTalkTimer); // 378504 уст. m_nPhoneTalkTimer (CPed)
	lua_register(L, "get_ped_m_fFleeFromPosX", get_ped_m_fFleeFromPosX); // 378505 m_fFleeFromPosX (CPed)
	lua_register(L, "set_ped_m_fFleeFromPosX", set_ped_m_fFleeFromPosX); // 378506 уст. m_fFleeFromPosX (CPed)
	lua_register(L, "get_ped_m_fFleeFromPosY", get_ped_m_fFleeFromPosY); // 378507 m_fFleeFromPosY (CPed)
	lua_register(L, "set_ped_m_fFleeFromPosY", set_ped_m_fFleeFromPosY); // 378508 уст. m_fFleeFromPosY (CPed)
	lua_register(L, "get_ped_m_nFleeTimer", get_ped_m_nFleeTimer); // 378509 m_nFleeTimer (CPed)
	lua_register(L, "set_ped_m_nFleeTimer", set_ped_m_nFleeTimer); // 378510 уст. m_nFleeTimer (CPed)
	lua_register(L, "get_ped_m_nLastThreatTimer", get_ped_m_nLastThreatTimer); // 378511 m_nLastThreatTimer (CPed)
	lua_register(L, "set_ped_m_nLastThreatTimer", set_ped_m_nLastThreatTimer); // 378512 уст. m_nLastThreatTimer (CPed)
	lua_register(L, "get_ped_m_nStateUnused", get_ped_m_nStateUnused); // 378513 m_nStateUnused (CPed)
	lua_register(L, "set_ped_m_nStateUnused", set_ped_m_nStateUnused); // 378514 уст. m_nStateUnused (CPed)
	lua_register(L, "get_ped_m_nTimerUnused", get_ped_m_nTimerUnused); // 378515 m_nTimerUnused (CPed)
	lua_register(L, "set_ped_m_nTimerUnused", set_ped_m_nTimerUnused); // 378516 уст. m_nTimerUnused (CPed)
	lua_register(L, "get_ped_m_nAtchStoredWep", get_ped_m_nAtchStoredWep); // 378517 m_nAtchStoredWep (CPed)
	lua_register(L, "set_ped_m_nAtchStoredWep", set_ped_m_nAtchStoredWep); // 378518 уст. m_nAtchStoredWep (CPed)
	lua_register(L, "get_ped_m_nStoredGiveWep", get_ped_m_nStoredGiveWep); // 378519 m_nStoredGiveWep (CPed)
	lua_register(L, "set_ped_m_nStoredGiveWep", set_ped_m_nStoredGiveWep); // 378520 уст. m_nStoredGiveWep (CPed)
	lua_register(L, "get_ped_m_nStoredGiveAmmo", get_ped_m_nStoredGiveAmmo); // 378521 m_nStoredGiveAmmo (CPed)
	lua_register(L, "set_ped_m_nStoredGiveAmmo", set_ped_m_nStoredGiveAmmo); // 378522 уст. m_nStoredGiveAmmo (CPed)
	lua_register(L, "get_ped_m_nCurrentWeapon", get_ped_m_nCurrentWeapon); // 378523 m_nCurrentWeapon (CPed)
	lua_register(L, "set_ped_m_nCurrentWeapon", set_ped_m_nCurrentWeapon); // 378524 уст. m_nCurrentWeapon (CPed)
	lua_register(L, "get_ped_m_nWepSkills", get_ped_m_nWepSkills); // 378525 m_nWepSkills (CPed)
	lua_register(L, "set_ped_m_nWepSkills", set_ped_m_nWepSkills); // 378526 уст. m_nWepSkills (CPed)
	lua_register(L, "get_ped_m_nWeaponAccuracy", get_ped_m_nWeaponAccuracy); // 378527 m_nWeaponAccuracy (CPed)
	lua_register(L, "set_ped_m_nWeaponAccuracy", set_ped_m_nWeaponAccuracy); // 378528 уст. m_nWeaponAccuracy (CPed)
	lua_register(L, "get_ped_m_nBodyPart", get_ped_m_nBodyPart); // 378529 m_nBodyPart (CPed)
	lua_register(L, "set_ped_m_nBodyPart", set_ped_m_nBodyPart); // 378530 уст. m_nBodyPart (CPed)
	lua_register(L, "get_ped_m_vecHitLastPos", get_ped_m_vecHitLastPos); // 378531 m_vecHitLastPos (CPed)
	lua_register(L, "set_ped_m_vecHitLastPos", set_ped_m_vecHitLastPos); // 378532 уст. m_vecHitLastPos (CPed)
	lua_register(L, "get_ped_m_nHitCounter", get_ped_m_nHitCounter); // 378533 m_nHitCounter (CPed)
	lua_register(L, "set_ped_m_nHitCounter", set_ped_m_nHitCounter); // 378534 уст. m_nHitCounter (CPed)
	lua_register(L, "get_ped_m_nLastHitState", get_ped_m_nLastHitState); // 378535 m_nLastHitState (CPed)
	lua_register(L, "set_ped_m_nLastHitState", set_ped_m_nLastHitState); // 378536 уст. m_nLastHitState (CPed)
	lua_register(L, "get_ped_m_nFightFlags1", get_ped_m_nFightFlags1); // 378537 m_nFightFlags1 (CPed)
	lua_register(L, "set_ped_m_nFightFlags1", set_ped_m_nFightFlags1); // 378538 уст. m_nFightFlags1 (CPed)
	lua_register(L, "get_ped_m_nFightFlags2", get_ped_m_nFightFlags2); // 378539 m_nFightFlags2 (CPed)
	lua_register(L, "set_ped_m_nFightFlags2", set_ped_m_nFightFlags2); // 378540 уст. m_nFightFlags2 (CPed)
	lua_register(L, "get_ped_m_nFightFlags3", get_ped_m_nFightFlags3); // 378541 m_nFightFlags3 (CPed)
	lua_register(L, "set_ped_m_nFightFlags3", set_ped_m_nFightFlags3); // 378542 уст. m_nFightFlags3 (CPed)
	lua_register(L, "get_ped_m_nBleedCounter", get_ped_m_nBleedCounter); // 378543 m_nBleedCounter (CPed)
	lua_register(L, "set_ped_m_nBleedCounter", set_ped_m_nBleedCounter); // 378544 уст. m_nBleedCounter (CPed)
	lua_register(L, "get_ped_m_fLookDirection", get_ped_m_fLookDirection); // 378545 m_fLookDirection (CPed)
	lua_register(L, "set_ped_m_fLookDirection", set_ped_m_fLookDirection); // 378546 уст. m_fLookDirection (CPed)
	lua_register(L, "get_ped_m_nWepModelID", get_ped_m_nWepModelID); // 378547 m_nWepModelID (CPed)
	lua_register(L, "set_ped_m_nWepModelID", set_ped_m_nWepModelID); // 378548 уст. m_nWepModelID (CPed)
	lua_register(L, "get_ped_m_nLeaveCarTimer", get_ped_m_nLeaveCarTimer); // 378549 m_nLeaveCarTimer (CPed)
	lua_register(L, "set_ped_m_nLeaveCarTimer", set_ped_m_nLeaveCarTimer); // 378550 уст. m_nLeaveCarTimer (CPed)
	lua_register(L, "get_ped_m_nGetUpTimer", get_ped_m_nGetUpTimer); // 378551 m_nGetUpTimer (CPed)
	lua_register(L, "set_ped_m_nGetUpTimer", set_ped_m_nGetUpTimer); // 378552 уст. m_nGetUpTimer (CPed)
	lua_register(L, "get_ped_m_nLookTimer", get_ped_m_nLookTimer); // 378553 m_nLookTimer (CPed)
	lua_register(L, "set_ped_m_nLookTimer", set_ped_m_nLookTimer); // 378554 уст. m_nLookTimer (CPed)
	lua_register(L, "get_ped_m_nStandardTimer", get_ped_m_nStandardTimer); // 378555 m_nStandardTimer (CPed)
	lua_register(L, "set_ped_m_nStandardTimer", set_ped_m_nStandardTimer); // 378556 уст. m_nStandardTimer (CPed)
	lua_register(L, "get_ped_m_nAttackTimer", get_ped_m_nAttackTimer); // 378557 m_nAttackTimer (CPed)
	lua_register(L, "set_ped_m_nAttackTimer", set_ped_m_nAttackTimer); // 378558 уст. m_nAttackTimer (CPed)
	lua_register(L, "get_ped_m_nLastHitTime", get_ped_m_nLastHitTime); // 378559 m_nLastHitTime (CPed)
	lua_register(L, "set_ped_m_nLastHitTime", set_ped_m_nLastHitTime); // 378560 уст. m_nLastHitTime (CPed)
	lua_register(L, "get_ped_m_nHitRecoverTimer", get_ped_m_nHitRecoverTimer); // 378561 m_nHitRecoverTimer (CPed)
	lua_register(L, "set_ped_m_nHitRecoverTimer", set_ped_m_nHitRecoverTimer); // 378562 уст. m_nHitRecoverTimer (CPed)
	lua_register(L, "get_ped_m_nObjectiveTimer", get_ped_m_nObjectiveTimer); // 378563 m_nObjectiveTimer (CPed)
	lua_register(L, "set_ped_m_nObjectiveTimer", set_ped_m_nObjectiveTimer); // 378564 уст. m_nObjectiveTimer (CPed)
	lua_register(L, "get_ped_m_nDuckTimer", get_ped_m_nDuckTimer); // 378565 m_nDuckTimer (CPed)
	lua_register(L, "set_ped_m_nDuckTimer", set_ped_m_nDuckTimer); // 378566 уст. m_nDuckTimer (CPed)
	lua_register(L, "get_ped_m_nDuckAndCoverTimer", get_ped_m_nDuckAndCoverTimer); // 378567 m_nDuckAndCoverTimer (CPed)
	lua_register(L, "set_ped_m_nDuckAndCoverTimer", set_ped_m_nDuckAndCoverTimer); // 378568 уст. m_nDuckAndCoverTimer (CPed)
	lua_register(L, "get_ped_m_nBloodyTimer", get_ped_m_nBloodyTimer); // 378569 m_nBloodyTimer (CPed)
	lua_register(L, "set_ped_m_nBloodyTimer", set_ped_m_nBloodyTimer); // 378570 уст. m_nBloodyTimer (CPed)
	lua_register(L, "get_ped_m_nShotTime", get_ped_m_nShotTime); // 378571 m_nShotTime (CPed)
	lua_register(L, "set_ped_m_nShotTime", set_ped_m_nShotTime); // 378572 уст. m_nShotTime (CPed)
	lua_register(L, "get_ped_m_nShotTimeAdd", get_ped_m_nShotTimeAdd); // 378573 m_nShotTimeAdd (CPed)
	lua_register(L, "set_ped_m_nShotTimeAdd", set_ped_m_nShotTimeAdd); // 378574 уст. m_nShotTimeAdd (CPed)
	lua_register(L, "get_ped_m_nPanicCounter", get_ped_m_nPanicCounter); // 378575 m_nPanicCounter (CPed)
	lua_register(L, "set_ped_m_nPanicCounter", set_ped_m_nPanicCounter); // 378576 уст. m_nPanicCounter (CPed)
	lua_register(L, "get_ped_m_nDeadBleeding", get_ped_m_nDeadBleeding); // 378577 m_nDeadBleeding (CPed)
	lua_register(L, "set_ped_m_nDeadBleeding", set_ped_m_nDeadBleeding); // 378578 уст. m_nDeadBleeding (CPed)
	lua_register(L, "get_ped_m_nBodyPartBleeding", get_ped_m_nBodyPartBleeding); // 378579 m_nBodyPartBleeding (CPed)
	lua_register(L, "set_ped_m_nBodyPartBleeding", set_ped_m_nBodyPartBleeding); // 378580 уст. m_nBodyPartBleeding (CPed)
	lua_register(L, "get_ped_m_nNumNearPeds", get_ped_m_nNumNearPeds); // 378581 m_nNumNearPeds (CPed)
	lua_register(L, "set_ped_m_nNumNearPeds", set_ped_m_nNumNearPeds); // 378582 уст. m_nNumNearPeds (CPed)
	lua_register(L, "get_ped_m_nPedMoney", get_ped_m_nPedMoney); // 378583 m_nPedMoney (CPed)
	lua_register(L, "set_ped_m_nPedMoney", set_ped_m_nPedMoney); // 378584 уст. m_nPedMoney (CPed)
	lua_register(L, "get_ped_m_nLastDamWep", get_ped_m_nLastDamWep); // 378585 m_nLastDamWep (CPed)
	lua_register(L, "set_ped_m_nLastDamWep", set_ped_m_nLastDamWep); // 378586 уст. m_nLastDamWep (CPed)
	lua_register(L, "get_ped_m_vecAttachOffset", get_ped_m_vecAttachOffset); // 378587 m_vecAttachOffset (CPed)
	lua_register(L, "set_ped_m_vecAttachOffset", set_ped_m_vecAttachOffset); // 378588 уст. m_vecAttachOffset (CPed)
	lua_register(L, "get_ped_m_nAttachType", get_ped_m_nAttachType); // 378589 m_nAttachType (CPed)
	lua_register(L, "set_ped_m_nAttachType", set_ped_m_nAttachType); // 378590 уст. m_nAttachType (CPed)
	lua_register(L, "get_ped_m_fAttachRot", get_ped_m_fAttachRot); // 378591 m_fAttachRot (CPed)
	lua_register(L, "set_ped_m_fAttachRot", set_ped_m_fAttachRot); // 378592 уст. m_fAttachRot (CPed)
	lua_register(L, "get_ped_m_nAttachWepAmmo", get_ped_m_nAttachWepAmmo); // 378593 m_nAttachWepAmmo (CPed)
	lua_register(L, "set_ped_m_nAttachWepAmmo", set_ped_m_nAttachWepAmmo); // 378594 уст. m_nAttachWepAmmo (CPed)
	lua_register(L, "get_ped_m_nThreatFlags", get_ped_m_nThreatFlags); // 378595 m_nThreatFlags (CPed)
	lua_register(L, "set_ped_m_nThreatFlags", set_ped_m_nThreatFlags); // 378596 уст. m_nThreatFlags (CPed)
	lua_register(L, "get_ped_m_nThreatCheck", get_ped_m_nThreatCheck); // 378597 m_nThreatCheck (CPed)
	lua_register(L, "set_ped_m_nThreatCheck", set_ped_m_nThreatCheck); // 378598 уст. m_nThreatCheck (CPed)
	lua_register(L, "get_ped_m_nLastThreatCheck", get_ped_m_nLastThreatCheck); // 378599 m_nLastThreatCheck (CPed)
	lua_register(L, "set_ped_m_nLastThreatCheck", set_ped_m_nLastThreatCheck); // 378600 уст. m_nLastThreatCheck (CPed)
	lua_register(L, "get_ped_m_nSayType", get_ped_m_nSayType); // 378601 m_nSayType (CPed)
	lua_register(L, "set_ped_m_nSayType", set_ped_m_nSayType); // 378602 уст. m_nSayType (CPed)
	lua_register(L, "get_ped_m_nSayTimer", get_ped_m_nSayTimer); // 378603 m_nSayTimer (CPed)
	lua_register(L, "set_ped_m_nSayTimer", set_ped_m_nSayTimer); // 378604 уст. m_nSayTimer (CPed)
	lua_register(L, "get_ped_m_nTalkTimerLast", get_ped_m_nTalkTimerLast); // 378605 m_nTalkTimerLast (CPed)
	lua_register(L, "set_ped_m_nTalkTimerLast", set_ped_m_nTalkTimerLast); // 378606 уст. m_nTalkTimerLast (CPed)
	lua_register(L, "get_ped_m_nTalkTimer", get_ped_m_nTalkTimer); // 378607 m_nTalkTimer (CPed)
	lua_register(L, "set_ped_m_nTalkTimer", set_ped_m_nTalkTimer); // 378608 уст. m_nTalkTimer (CPed)
	lua_register(L, "get_ped_m_wTalkTypeLast", get_ped_m_wTalkTypeLast); // 378609 m_wTalkTypeLast (CPed)
	lua_register(L, "set_ped_m_wTalkTypeLast", set_ped_m_wTalkTypeLast); // 378610 уст. m_wTalkTypeLast (CPed)
	lua_register(L, "get_ped_m_wTalkType", get_ped_m_wTalkType); // 378611 m_wTalkType (CPed)
	lua_register(L, "set_ped_m_wTalkType", set_ped_m_wTalkType); // 378612 уст. m_wTalkType (CPed)
	lua_register(L, "get_ped_m_bCanPedTalk", get_ped_m_bCanPedTalk); // 378613 m_bCanPedTalk (CPed)
	lua_register(L, "set_ped_m_bCanPedTalk", set_ped_m_bCanPedTalk); // 378614 уст. m_bCanPedTalk (CPed)
	lua_register(L, "get_ped_m_nPedLastComment", get_ped_m_nPedLastComment); // 378615 m_nPedLastComment (CPed)
	lua_register(L, "set_ped_m_nPedLastComment", set_ped_m_nPedLastComment); // 378616 уст. m_nPedLastComment (CPed)
	lua_register(L, "get_ped_m_vecSeekPosEx", get_ped_m_vecSeekPosEx); // 378617 m_vecSeekPosEx (CPed)
	lua_register(L, "set_ped_m_vecSeekPosEx", set_ped_m_vecSeekPosEx); // 378618 уст. m_vecSeekPosEx (CPed)
	lua_register(L, "get_ped_m_fSeekExAngle", get_ped_m_fSeekExAngle); // 378619 m_fSeekExAngle (CPed)
	lua_register(L, "set_ped_m_fSeekExAngle", set_ped_m_fSeekExAngle); // 378620 уст. m_fSeekExAngle (CPed)
	lua_register(L, "get_obj_m_nObjectType", get_obj_m_nObjectType); // 378621 m_nObjectType (CObject)
	lua_register(L, "set_obj_m_nObjectType", set_obj_m_nObjectType); // 378622 уст. m_nObjectType (CObject)
	lua_register(L, "get_obj_m_nBonusValue", get_obj_m_nBonusValue); // 378623 m_nBonusValue (CObject)
	lua_register(L, "set_obj_m_nBonusValue", set_obj_m_nBonusValue); // 378624 уст. m_nBonusValue (CObject)
	lua_register(L, "get_obj_m_wCostValue", get_obj_m_wCostValue); // 378625 m_wCostValue (CObject)
	lua_register(L, "set_obj_m_wCostValue", set_obj_m_wCostValue); // 378626 уст. m_wCostValue (CObject)
	lua_register(L, "get_obj_m_fDamageMultiplier", get_obj_m_fDamageMultiplier); // 378627 m_fDamageMultiplier (CObject)
	lua_register(L, "set_obj_m_fDamageMultiplier", set_obj_m_fDamageMultiplier); // 378628 уст. m_fDamageMultiplier (CObject)
	lua_register(L, "get_obj_m_bCameraAvoids", get_obj_m_bCameraAvoids); // 378629 m_bCameraAvoids (CObject)
	lua_register(L, "set_obj_m_bCameraAvoids", set_obj_m_bCameraAvoids); // 378630 уст. m_bCameraAvoids (CObject)
	lua_register(L, "get_obj_m_wRefModelId", get_obj_m_wRefModelId); // 378631 m_wRefModelId (CObject)
	lua_register(L, "set_obj_m_wRefModelId", set_obj_m_wRefModelId); // 378632 уст. m_wRefModelId (CObject)
	lua_register(L, "get_car_m_nAudioEntityId", get_car_m_nAudioEntityId); // 378633 m_nAudioEntityId (CPhysical)
	lua_register(L, "set_car_m_nAudioEntityId", set_car_m_nAudioEntityId); // 378634 уст. m_nAudioEntityId (CPhysical)
	lua_register(L, "get_car_m_vecMoveSpeed", get_car_m_vecMoveSpeed); // 378635 m_vecMoveSpeed (CPhysical)
	lua_register(L, "set_car_m_vecMoveSpeed", set_car_m_vecMoveSpeed); // 378636 уст. m_vecMoveSpeed (CPhysical)
	lua_register(L, "get_car_m_vecTurnSpeed", get_car_m_vecTurnSpeed); // 378637 m_vecTurnSpeed (CPhysical)
	lua_register(L, "set_car_m_vecTurnSpeed", set_car_m_vecTurnSpeed); // 378638 уст. m_vecTurnSpeed (CPhysical)
	lua_register(L, "get_car_m_vecFrictionMoveForce", get_car_m_vecFrictionMoveForce); // 378639 m_vecFrictionMoveForce (CPhysical)
	lua_register(L, "set_car_m_vecFrictionMoveForce", set_car_m_vecFrictionMoveForce); // 378640 уст. m_vecFrictionMoveForce (CPhysical)
	lua_register(L, "get_car_m_vecFrictionTurnForce", get_car_m_vecFrictionTurnForce); // 378641 m_vecFrictionTurnForce (CPhysical)
	lua_register(L, "set_car_m_vecFrictionTurnForce", set_car_m_vecFrictionTurnForce); // 378642 уст. m_vecFrictionTurnForce (CPhysical)
	lua_register(L, "get_car_m_vecForce", get_car_m_vecForce); // 378643 m_vecForce (CPhysical)
	lua_register(L, "set_car_m_vecForce", set_car_m_vecForce); // 378644 уст. m_vecForce (CPhysical)
	lua_register(L, "get_car_m_vecTorque", get_car_m_vecTorque); // 378645 m_vecTorque (CPhysical)
	lua_register(L, "set_car_m_vecTorque", set_car_m_vecTorque); // 378646 уст. m_vecTorque (CPhysical)
	lua_register(L, "get_car_m_fMass", get_car_m_fMass); // 378647 m_fMass (CPhysical)
	lua_register(L, "set_car_m_fMass", set_car_m_fMass); // 378648 уст. m_fMass (CPhysical)
	lua_register(L, "get_car_m_fTurnMass", get_car_m_fTurnMass); // 378649 m_fTurnMass (CPhysical)
	lua_register(L, "set_car_m_fTurnMass", set_car_m_fTurnMass); // 378650 уст. m_fTurnMass (CPhysical)
	lua_register(L, "get_car_m_fVelocityFrequency", get_car_m_fVelocityFrequency); // 378651 m_fVelocityFrequency (CPhysical)
	lua_register(L, "set_car_m_fVelocityFrequency", set_car_m_fVelocityFrequency); // 378652 уст. m_fVelocityFrequency (CPhysical)
	lua_register(L, "get_car_m_fAirResistance", get_car_m_fAirResistance); // 378653 m_fAirResistance (CPhysical)
	lua_register(L, "set_car_m_fAirResistance", set_car_m_fAirResistance); // 378654 уст. m_fAirResistance (CPhysical)
	lua_register(L, "get_car_m_fElasticity", get_car_m_fElasticity); // 378655 m_fElasticity (CPhysical)
	lua_register(L, "set_car_m_fElasticity", set_car_m_fElasticity); // 378656 уст. m_fElasticity (CPhysical)
	lua_register(L, "get_car_m_fBuoyancyConstant", get_car_m_fBuoyancyConstant); // 378657 m_fBuoyancyConstant (CPhysical)
	lua_register(L, "set_car_m_fBuoyancyConstant", set_car_m_fBuoyancyConstant); // 378658 уст. m_fBuoyancyConstant (CPhysical)
	lua_register(L, "get_car_m_vecCentreOfMass", get_car_m_vecCentreOfMass); // 378659 m_vecCentreOfMass (CPhysical)
	lua_register(L, "set_car_m_vecCentreOfMass", set_car_m_vecCentreOfMass); // 378660 уст. m_vecCentreOfMass (CPhysical)
	lua_register(L, "get_car_m_nNumCollisionRecords", get_car_m_nNumCollisionRecords); // 378661 m_nNumCollisionRecords (CPhysical)
	lua_register(L, "set_car_m_nNumCollisionRecords", set_car_m_nNumCollisionRecords); // 378662 уст. m_nNumCollisionRecords (CPhysical)
	lua_register(L, "get_car_m_fTotSpeed", get_car_m_fTotSpeed); // 378663 m_fTotSpeed (CPhysical)
	lua_register(L, "set_car_m_fTotSpeed", set_car_m_fTotSpeed); // 378664 уст. m_fTotSpeed (CPhysical)
	lua_register(L, "get_car_m_fCollisionPower", get_car_m_fCollisionPower); // 378665 m_fCollisionPower (CPhysical)
	lua_register(L, "set_car_m_fCollisionPower", set_car_m_fCollisionPower); // 378666 уст. m_fCollisionPower (CPhysical)
	lua_register(L, "get_car_m_vecCollisionPower", get_car_m_vecCollisionPower); // 378667 m_vecCollisionPower (CPhysical)
	lua_register(L, "set_car_m_vecCollisionPower", set_car_m_vecCollisionPower); // 378668 уст. m_vecCollisionPower (CPhysical)
	lua_register(L, "get_car_m_wComponentCol", get_car_m_wComponentCol); // 378669 m_wComponentCol (CPhysical)
	lua_register(L, "set_car_m_wComponentCol", set_car_m_wComponentCol); // 378670 уст. m_wComponentCol (CPhysical)
	lua_register(L, "get_car_m_nMoveFlags", get_car_m_nMoveFlags); // 378671 m_nMoveFlags (CPhysical)
	lua_register(L, "set_car_m_nMoveFlags", set_car_m_nMoveFlags); // 378672 уст. m_nMoveFlags (CPhysical)
	lua_register(L, "get_car_m_nCollFlags", get_car_m_nCollFlags); // 378673 m_nCollFlags (CPhysical)
	lua_register(L, "set_car_m_nCollFlags", set_car_m_nCollFlags); // 378674 уст. m_nCollFlags (CPhysical)
	lua_register(L, "get_car_m_nLastCollType", get_car_m_nLastCollType); // 378675 m_nLastCollType (CPhysical)
	lua_register(L, "set_car_m_nLastCollType", set_car_m_nLastCollType); // 378676 уст. m_nLastCollType (CPhysical)
	lua_register(L, "get_car_m_nZoneLevel", get_car_m_nZoneLevel); // 378677 m_nZoneLevel (CPhysical)
	lua_register(L, "set_car_m_nZoneLevel", set_car_m_nZoneLevel); // 378678 уст. m_nZoneLevel (CPhysical)
	lua_register(L, "get_car_fNegSpeed", get_car_fNegSpeed); // 378679 fNegSpeed (CAutomobile)
	lua_register(L, "set_car_fNegSpeed", set_car_fNegSpeed); // 378680 уст. fNegSpeed (CAutomobile)
	lua_register(L, "get_car_fAIGripMultiplier", get_car_fAIGripMultiplier); // 378681 fAIGripMultiplier (CAutomobile)
	lua_register(L, "set_car_fAIGripMultiplier", set_car_fAIGripMultiplier); // 378682 уст. fAIGripMultiplier (CAutomobile)
	lua_register(L, "get_car_fSpecialWepRotH", get_car_fSpecialWepRotH); // 378683 fSpecialWepRotH (CAutomobile)
	lua_register(L, "set_car_fSpecialWepRotH", set_car_fSpecialWepRotH); // 378684 уст. fSpecialWepRotH (CAutomobile)
	lua_register(L, "get_car_fSpecialWepRotV", get_car_fSpecialWepRotV); // 378685 fSpecialWepRotV (CAutomobile)
	lua_register(L, "set_car_fSpecialWepRotV", set_car_fSpecialWepRotV); // 378686 уст. fSpecialWepRotV (CAutomobile)
	lua_register(L, "get_car_fSpecialSteering", get_car_fSpecialSteering); // 378687 fSpecialSteering (CAutomobile)
	lua_register(L, "set_car_fSpecialSteering", set_car_fSpecialSteering); // 378688 уст. fSpecialSteering (CAutomobile)
	lua_register(L, "get_car_fSpecialMoveState", get_car_fSpecialMoveState); // 378689 fSpecialMoveState (CAutomobile)
	lua_register(L, "set_car_fSpecialMoveState", set_car_fSpecialMoveState); // 378690 уст. fSpecialMoveState (CAutomobile)
	lua_register(L, "get_car_fSkidMarkDensity", get_car_fSkidMarkDensity); // 378691 fSkidMarkDensity (CAutomobile)
	lua_register(L, "set_car_fSkidMarkDensity", set_car_fSkidMarkDensity); // 378692 уст. fSkidMarkDensity (CAutomobile)
	lua_register(L, "get_car_m_fPropRotation", get_car_m_fPropRotation); // 378693 m_fPropRotation (CBoat)
	lua_register(L, "set_car_m_fPropRotation", set_car_m_fPropRotation); // 378694 уст. m_fPropRotation (CBoat)
	lua_register(L, "get_car_m_fPropSpeed", get_car_m_fPropSpeed); // 378695 m_fPropSpeed (CBoat)
	lua_register(L, "set_car_m_fPropSpeed", set_car_m_fPropSpeed); // 378696 уст. m_fPropSpeed (CBoat)
	lua_register(L, "get_car_m_fForcedZRotation", get_car_m_fForcedZRotation); // 378697 m_fForcedZRotation (CBoat)
	lua_register(L, "set_car_m_fForcedZRotation", set_car_m_fForcedZRotation); // 378698 уст. m_fForcedZRotation (CBoat)
	lua_register(L, "get_car_m_nAttackPlayerTime", get_car_m_nAttackPlayerTime); // 378699 m_nAttackPlayerTime (CBoat)
	lua_register(L, "set_car_m_nAttackPlayerTime", set_car_m_nAttackPlayerTime); // 378700 уст. m_nAttackPlayerTime (CBoat)
	lua_register(L, "get_car_m_fBurningTimer", get_car_m_fBurningTimer); // 378701 m_fBurningTimer (CBoat)
	lua_register(L, "set_car_m_fBurningTimer", set_car_m_fBurningTimer); // 378702 уст. m_fBurningTimer (CBoat)
	lua_register(L, "get_car_m_fBoatGasPedal", get_car_m_fBoatGasPedal); // 378703 m_fBoatGasPedal (CBoat)
	lua_register(L, "set_car_m_fBoatGasPedal", set_car_m_fBoatGasPedal); // 378704 уст. m_fBoatGasPedal (CBoat)
	lua_register(L, "get_car_m_fBoatBrakePedal", get_car_m_fBoatBrakePedal); // 378705 m_fBoatBrakePedal (CBoat)
	lua_register(L, "set_car_m_fBoatBrakePedal", set_car_m_fBoatBrakePedal); // 378706 уст. m_fBoatBrakePedal (CBoat)
	lua_register(L, "get_car_m_fBoatSteeringLeftRight", get_car_m_fBoatSteeringLeftRight); // 378707 m_fBoatSteeringLeftRight (CBoat)
	lua_register(L, "set_car_m_fBoatSteeringLeftRight", set_car_m_fBoatSteeringLeftRight); // 378708 уст. m_fBoatSteeringLeftRight (CBoat)
	lua_register(L, "get_car_m_nPadNumber", get_car_m_nPadNumber); // 378709 m_nPadNumber (CBoat)
	lua_register(L, "set_car_m_nPadNumber", set_car_m_nPadNumber); // 378710 уст. m_nPadNumber (CBoat)
	lua_register(L, "get_car_m_nNumWaterTrailPoints", get_car_m_nNumWaterTrailPoints); // 378711 m_nNumWaterTrailPoints (CBoat)
	lua_register(L, "set_car_m_nNumWaterTrailPoints", set_car_m_nNumWaterTrailPoints); // 378712 уст. m_nNumWaterTrailPoints (CBoat)
	lua_register(L, "get_car_nBikeAnimGroup", get_car_nBikeAnimGroup); // 378713 nBikeAnimGroup (CBike)
	lua_register(L, "set_car_nBikeAnimGroup", set_car_nBikeAnimGroup); // 378714 уст. nBikeAnimGroup (CBike)
	lua_register(L, "get_car_m_nDamageFlags", get_car_m_nDamageFlags); // 378715 m_nDamageFlags (CBike)
	lua_register(L, "set_car_m_nDamageFlags", set_car_m_nDamageFlags); // 378716 уст. m_nDamageFlags (CBike)
	lua_register(L, "get_car_m_fSearchLightX", get_car_m_fSearchLightX); // 378717 m_fSearchLightX (CHeli)
	lua_register(L, "set_car_m_fSearchLightX", set_car_m_fSearchLightX); // 378718 уст. m_fSearchLightX (CHeli)
	lua_register(L, "get_car_m_fSearchLightY", get_car_m_fSearchLightY); // 378719 m_fSearchLightY (CHeli)
	lua_register(L, "set_car_m_fSearchLightY", set_car_m_fSearchLightY); // 378720 уст. m_fSearchLightY (CHeli)
	lua_register(L, "get_car_m_nExplosionTime", get_car_m_nExplosionTime); // 378721 m_nExplosionTime (CHeli)
	lua_register(L, "set_car_m_nExplosionTime", set_car_m_nExplosionTime); // 378722 уст. m_nExplosionTime (CHeli)
	lua_register(L, "get_car_m_fRotationX", get_car_m_fRotationX); // 378723 m_fRotationX (CHeli)
	lua_register(L, "set_car_m_fRotationX", set_car_m_fRotationX); // 378724 уст. m_fRotationX (CHeli)
	lua_register(L, "get_car_m_fHeliForcedZ", get_car_m_fHeliForcedZ); // 378725 m_fHeliForcedZ (CHeli)
	lua_register(L, "set_car_m_fHeliForcedZ", set_car_m_fHeliForcedZ); // 378726 уст. m_fHeliForcedZ (CHeli)
	lua_register(L, "get_car_m_fSearchLightIntensity", get_car_m_fSearchLightIntensity); // 378727 m_fSearchLightIntensity (CHeli)
	lua_register(L, "set_car_m_fSearchLightIntensity", set_car_m_fSearchLightIntensity); // 378728 уст. m_fSearchLightIntensity (CHeli)
	lua_register(L, "get_car_m_nDamagePoints", get_car_m_nDamagePoints); // 378729 m_nDamagePoints (CHeli)
	lua_register(L, "set_car_m_nDamagePoints", set_car_m_nDamagePoints); // 378730 уст. m_nDamagePoints (CHeli)
	lua_register(L, "get_car_m_fRotorAngle", get_car_m_fRotorAngle); // 378731 m_fRotorAngle (CHeli)
	lua_register(L, "set_car_m_fRotorAngle", set_car_m_fRotorAngle); // 378732 уст. m_fRotorAngle (CHeli)
	lua_register(L, "get_wanted_m_nLastTimeWantedDecreased", get_wanted_m_nLastTimeWantedDecreased); // 378733 последнее снижение розыска
	lua_register(L, "get_wanted_m_nLastTimeWantedLevelChanged", get_wanted_m_nLastTimeWantedLevelChanged); // 378734 последняя смена уровня розыска
	lua_register(L, "get_wanted_m_dwTimeOfParole", get_wanted_m_dwTimeOfParole); // 378735 таймер условного освобождения
	lua_register(L, "set_wanted_m_dwTimeOfParole", set_wanted_m_dwTimeOfParole); // 378736 уст. таймер условного освобождения
	lua_register(L, "get_wanted_m_fMultiplier", get_wanted_m_fMultiplier); // 378737 множитель розыска
	lua_register(L, "set_wanted_m_fMultiplier", set_wanted_m_fMultiplier); // 378738 уст. множитель розыска
	lua_register(L, "get_wanted_m_nCopsInPursuit", get_wanted_m_nCopsInPursuit); // 378739 копов в погоне
	lua_register(L, "get_wanted_m_nMaxCopsInPursuit", get_wanted_m_nMaxCopsInPursuit); // 378740 макс. копов в погоне
	lua_register(L, "set_wanted_m_nMaxCopsInPursuit", set_wanted_m_nMaxCopsInPursuit); // 378741 уст. макс. копов в погоне
	lua_register(L, "get_wanted_m_nMaxCopCarsInPursuit", get_wanted_m_nMaxCopCarsInPursuit); // 378742 макс. машин копов в погоне
	lua_register(L, "set_wanted_m_nMaxCopCarsInPursuit", set_wanted_m_nMaxCopCarsInPursuit); // 378743 уст. макс. машин копов в погоне
	lua_register(L, "get_wanted_m_nCopsBeatingSuspect", get_wanted_m_nCopsBeatingSuspect); // 378744 копов избивают подозреваемого
	lua_register(L, "choose_gang_ped_model", choose_gang_ped_model); // 378745 выбрать модель педа банды
	lua_register(L, "set_gang_attack_player_with_cops", set_gang_attack_player_with_cops); // 378746 банда атакует игрока с копами
	lua_register(L, "init_gangs", init_gangs); // 378747 инициализировать банды
	lua_register(L, "register_motion_streak", register_motion_streak); // 378748 размытие-полоса (id, левая/правая точки, rgb)
	lua_register(L, "init_trains", init_trains); // 378364 инициализировать поезда
	lua_register(L, "update_trains", update_trains); // 378365 обновить поезда
	lua_register(L, "shutdown_trains", shutdown_trains); // 378366 завершить поезда
	lua_register(L, "ped_in_targetting", ped_in_targetting); // 182 пед на прицеле.
	lua_register(L, "Ped_in_targetting", Ped_in_targetting); // макрос 183 пед на прицеле.
	lua_register(L, "Get_model_and_type_ped", Get_model_and_type_ped); // 184 макрос получить модель и тип педа.
	lua_register(L, "get_model_ped", get_model_ped); // 185 получить модель педа.
	lua_register(L, "get_type_ped", get_type_ped); // 186 получить тип педа.
	lua_register(L, "ped_clear_investigate_event", ped_clear_investigate_event); // 187 пед уходит, опустить педа.
	lua_register(L, "ped_clear_objective", ped_clear_objective); // 188 снять задачи с педа.
	lua_register(L, "setbomb_in_car", setbomb_in_car); // 189 установить бомбу в авто.
	lua_register(L, "getbomb_in_car", getbomb_in_car); // 190 получить тип бомбы в авто.
	lua_register(L, "activetbomb", activetbomb); // 191 активировать бомбу в авто.
	lua_register(L, "is_ped_in_this_car", is_ped_in_this_car); // 192 игрок в этом авто?
	lua_register(L, "ishorncar", ishorncar); // 193 проверить гудит ли авто.
	lua_register(L, "getcarfirstcolor", getcarfirstcolor); // 194 получить первый цвет авто.
	lua_register(L, "getcarseconscolor", getcarseconscolor); // 195 получить второй цвет авто.
	lua_register(L, "ped_char_spotted_player", ped_char_spotted_player); // 196 если пед увидел игрока.
	lua_register(L, "set_timer_bomb_car", set_timer_bomb_car); // 197 установить таймер на взрыв бомбы в авто.
	lua_register(L, "is_ped_objective", is_ped_objective); // 198 если пед выполнил задачу.	
	lua_register(L, "set_time_scale", set_time_scale); // 199 установить ход времени в игре.
	lua_register(L, "is_ped_in_air", is_ped_in_air); // 200 если пед в воздухе.	
	lua_register(L, "kill_peds_in_car", kill_peds_in_car); // 201 убить педов в авто.
	lua_register(L, "getstamina_ped", getstamina_ped); // 202 получить выносливость педа.
	lua_register(L, "setstamina_ped", setstamina_ped); // 203 установить выносливость педа.
	lua_register(L, "get_radio_car", get_radio_car); // 204 получить текущую радиостанцию авто.
	lua_register(L, "is_ped_passenger_car", is_ped_passenger_car); // 205 пед пассажир авто.
	lua_register(L, "is_ped_driver_car", is_ped_driver_car); // 206 пед пассажир авто.
	lua_register(L, "ped_in_turret_on_car", ped_in_turret_on_car); // 207 переместить педа в турель авто.
	lua_register(L, "ped_from_turret_on_car", ped_from_turret_on_car); // 208 убрать педа из турели авто.
	lua_register(L, "is_button_pressed", is_button_pressed); // 209 если спец клавиша нажата.	
	lua_register(L, "getmoney_ped", getmoney_ped); // 210 получить кол-во деньги педа.
	lua_register(L, "setmoney_ped", setmoney_ped); // 211 установить кол-во деньги педа.
	lua_register(L, "addmoney_ped", addmoney_ped); // 212 добавить кол-во деньги педа.
	lua_register(L, "getgravity", getgravity); // 213 получить значения гравитации.
	lua_register(L, "setgravity", setgravity); // 214 установить значения гравитации.
	lua_register(L, "gravity_default", gravity_default); // 215 вернуть значения по умолчанию гравитации.
	lua_register(L, "create_shots_on_ped", create_shots_on_ped); // 216 создать выстрелы.
	lua_register(L, "get_wheel_status", get_wheel_status); // 217 получить состояния шины авто.
	lua_register(L, "create_shots_on_car", create_shots_on_car);// 218 создать выстрелы.
	lua_register(L, "ped_save_pos_attack", ped_save_pos_attack);// 219 пед сохраняет ли свою позицию при атаке.	
	lua_register(L, "ped_rebuff", ped_rebuff);// 220 пед атакует любого, кто нападает на него.
	lua_register(L, "ped_search_threat", ped_search_threat);// 221 пед ищет угрозу.	
	lua_register(L, "ped_clean_threat", ped_clean_threat);// 222 очистить угрозы для педа.	
	lua_register(L, "save_car_color", save_car_color);// 223 авто сохраняет цвет при перекраске.	
	lua_register(L, "is_car_passenger_free", is_car_passenger_free);// 224 Проверяет, свободно ли место пассажира в авто.
	lua_register(L, "set_ped_bleeding", set_ped_bleeding);// 225 вкл\выкл крованные у педа.	
	lua_register(L, "all_exit_from_car", all_exit_from_car);// 226 все педы выходят из авто.
	lua_register(L, "is_ped_duck", is_ped_duck);// 227 Проверяет, пед присел.
	lua_register(L, "set_car_tank", set_car_tank);// 228 установить способность танка детонировать авто при контакте.
	lua_register(L, "set_display_radar", set_display_radar);// 229 показать скрывать радар.
	lua_register(L, "set_ped_in_car", set_ped_in_car);// 230 установить педа в авто как водителя или пассажира.
	lua_register(L, "set_ped_traffic", set_ped_traffic); // 231 установить плотноcть трафика педов.
	lua_register(L, "heli_to_fly", heli_to_fly); // 232 вертолет летит на координаты.
	lua_register(L, "set_ped_immunities", set_ped_immunities); // 233 уст иммунитеты педу.
	lua_register(L, "set_car_immunities", set_car_immunities); // 234 уст иммунитеты авто.
	lua_register(L, "setcolourmarker", setcolourmarker); // 235 установить цвет маркера.
	lua_register(L, "set_camera_on_car", set_camera_on_car); // 236 уст камеру на авто.
	lua_register(L, "ped_run_to_point", ped_run_to_point); // 237 пед бежит к точке пешком.
	lua_register(L, "select_interiour", select_interiour); // 238 уcтановить интерьер.
	lua_register(L, "set_ped_stats_to", set_ped_stats_to); // 239 уст поведения педа.	
	lua_register(L, "ped_maintain_position_when_attacked", ped_maintain_position_when_attacked); // 240 пед удерживает позицию при атаке.		
	lua_register(L, "set_timer_ped_attack", set_timer_ped_attack); // 241 установить таймер атаки педа.		
	lua_register(L, "set_cops_ignored", set_cops_ignored); // 242 установить игнор копов.
   	lua_register(L, "set_camera_near_clip", set_camera_near_clip); // 243 установить обрезку камеры.
	lua_register(L, "setpedcrouch", setpedcrouch); //244 пед сел.
	lua_register(L, "is_ped_incar", is_ped_incar); // 245 пед в авто или нет?
	lua_register(L, "delete_entity", delete_entity); // 246 удалить сущность сразу.
	lua_register(L, "clean_leader", clean_leader); // 247 перестать следовать за лидером.
	lua_register(L, "show_save_menu", show_save_menu); // 248 показать меню сохранения.
	lua_register(L, "is_ped_shooting", is_ped_shooting); // 249 пед стреляет?.
	lua_register(L, "set_ped_visible", set_ped_visible); // 250 уст невидимость педу.
	lua_register(L, "set_player_mod", set_player_mod); // 251 уст настроения игрока.
	lua_register(L, "setped_damaged_player", setped_damaged_player); // 252 педу может нанести ущерб только игрок.
	lua_register(L, "set_ped_reaction_threat", set_ped_reaction_threat); // 253 уст реакцию педа на ответную угрозу.
	lua_register(L, "obj_target", obj_target); // 254 в объект можно целиться.
	lua_register(L, "clean_ped_wait", clean_ped_wait); // 255 пед больше не ждет.
	lua_register(L, "set_ped_accuracy", set_ped_accuracy); // 256 уст меткость педу.
	lua_register(L, "textbox", textbox); // 258 вывести сообщение в textbox.
	lua_register(L, "remove_textbox", remove_textbox); // 259 удалить сообщение на textbox.
	lua_register(L, "get_controller", get_controller); // 260 получить контролер от игрока.
	lua_register(L, "create_pickup_clothes", create_pickup_clothes); // 260 создать пикап одежды.
	lua_register(L, "set_riot_intensity", set_riot_intensity); // 261 уст уровень шума беспорядков.
	lua_register(L, "set_ped_walk_style", set_ped_walk_style); // 262 установить стиль ходьбы педа.
	lua_register(L, "check_car_resray", check_car_resray); // 263 проверить авто игрока было перекрашена в гараже.
	lua_register(L, "set_car_range", set_car_range); // 264 установить множитель диапазона на угрозу для автомобиля.
	lua_register(L, "set_ped_range", set_ped_range); // 265 установить множитель диапазона на угрозу для педа.
	lua_register(L, "getcarangle", getcarangle); // 266 получить угол авто.
	lua_register(L, "create_newthread", create_newthread); // 267 создания нового потока.
	lua_register(L, "cleanarea", cleanarea); // 268 очистить арену.
	lua_register(L, "set_brakes_car", set_brakes_car); // 269 уст тормоза авто игрока.
	lua_register(L, "setmarker_brightness", setmarker_brightness); // 270 уст яркость маркера.
	lua_register(L, "Createobj", Createobj); // 271 макрос создать объект.
	lua_register(L, "setpednode_seek", setpednode_seek); // 272 пед игнорирорует пути педов. 
	lua_register(L, "ispedscreen", ispedscreen); // 273 пед виден.
	lua_register(L, "iscarscreen", iscarscreen); // 274 авто видно.
	lua_register(L, "isobjscreen", isobjscreen); // 276 объект виден.
	lua_register(L, "ispeddead", ispeddead); // 277 пед мертв.
	lua_register(L, "iscardead", iscardead); // 278 авто уничтожено.
	lua_register(L, "ped_follow_ped", ped_follow_ped); // 279 пед следует за педом.
	lua_register(L, "set_cars_damaged", set_cars_damaged); // 280 Все авто повреждены.
	lua_register(L, "set_ped_targetted", set_ped_targetted); // 281 запрет целиться в педа.
	lua_register(L, "set_ped_friend", set_ped_friend); // 282 уст дружественное отношения педа.
	lua_register(L, "set_ped_running", set_ped_running); // 283 пед может бежать.
	lua_register(L, "set_ped_damaged_gang", set_ped_damaged_gang); // 284 уст педа уязвимым для членов банды.
	lua_register(L, "is_ped_damaged_weapon", is_ped_damaged_weapon); // 285 пед получает от определенного вида оружие.
	lua_register(L, "is_car_damaged_weapon", is_car_damaged_weapon); // 286 авто получает от определенного вида оружие.
	lua_register(L, "isped_in_air", isped_in_air); // 287 пед в воздухе.
	lua_register(L, "set_threat_for_ped_type", set_threat_for_ped_type); // 288 уст враждебность типа педа к другим типам педа.
	lua_register(L, "clean_threat_for_ped_type", clean_threat_for_ped_type); // 289 Убрать враждебность типа педа к другим типам педа.
	lua_register(L, "create_phone", create_phone); // 290 создать телефон на координатах.
	lua_register(L, "on_phone", on_phone); // 291 вкл телефон.
	lua_register(L, "off_phone", off_phone); // 292 выкл телефон.
	lua_register(L, "read_memory", read_memory); // 293 читать адрес памяти.
	lua_register(L, "write_memory", write_memory); // 294 записать адрес памяти.
	lua_register(L, "is_sphere_onscreen", is_sphere_onscreen); // 295 видна точка на радаре.
	lua_register(L, "set_ped_can_shot_in_car", set_ped_can_shot_in_car); // 296 водитель авто может получить пули.
	lua_register(L, "iscarfire", iscarfire); // 297 авто горит?
	lua_register(L, "is_car_damage_by_car", is_car_damage_by_car);// 298 Если авто получило урон от другого авто.
	lua_register(L, "show_text_gtx_table", show_text_gtx_table); // 299 вывести текст из таблицы.
	lua_register(L, "ped_road_off", ped_road_off); // 300 выкл путь педов.
	lua_register(L, "ped_road_on", ped_road_on); // 301 вкл путь педов.
	lua_register(L, "car_road_off", car_road_off); // 302 выкл путь авто.
	lua_register(L, "car_road_on", car_road_on); // 303 вкл путь авто.
	lua_register(L, "getcardamage", getcardamage); // 304 получить кол-во урона авто.
	lua_register(L, "randomfindobj", randomfindobj); // 305 найти случайный объект в радиусе.
	lua_register(L, "findped_in_cord", findped_in_cord); // 306 найти педа в радиусе с координатами.
	lua_register(L, "findcar_in_cord", findcar_in_cord); // 307 найти авто в радиусе с координатами.
	lua_register(L, "findobj_in_cord", findobj_in_cord); // 308 найти объект в радиусе с координатами.
	lua_register(L, "findped_nearest_in_cord", findped_nearest_in_cord); // 309 найти ближайшего педа в радиусе с координатами.
	lua_register(L, "findcar_nearest_in_cord", findcar_nearest_in_cord); // 310 найти ближайшее авто в радиусе с координатами.
	lua_register(L, "findobj_nearest_in_cord", findobj_nearest_in_cord); // 311 найти ближайший объект в радиусе с координатами.
	lua_register(L, "getmodelindex", getmodelindex); // 312 получить id модели.
	lua_register(L, "check_ped_in_cord", check_ped_in_cord); // 313 есть ли педа в радиусе с координатами.
	lua_register(L, "get_ped_in_cord", get_ped_in_cord); // 314 получить педа в радиусе с координатами.
	lua_register(L, "check_car_in_cord", check_car_in_cord); // 315 есть ли авто в радиусе с координатами.
	lua_register(L, "get_car_in_cord", get_car_in_cord); // 316 найти авто в радиусе с координатами.
	lua_register(L, "check_obj_in_cord", check_obj_in_cord); // 317 есть ли объект в радиусе с координатами.
	lua_register(L, "get_obj_in_cord", get_obj_in_cord); // 318 получить объект в радиусе с координатам.
	lua_register(L, "setobjangle", setobjangle); // 319  уст угол объекта.
	lua_register(L, "getcardimension", getcardimension); // 320 получить размер авто.
	lua_register(L, "carmoveforce", carmoveforce); // 321 двигать авто с силой.
	lua_register(L, "carturnforce", carturnforce); // 322 повернуть авто с силой.
	lua_register(L, "car_turn_speed", car_turn_speed); // 323 повернуть авто с применением скорости.
	lua_register(L, "carturnfrictionforce", carturnfrictionforce); // 324 повернуть авто с силой трения.
	lua_register(L, "car_move_speed", car_move_speed); // 325 двигать авто с применением скорости.

	lua_register(L, "car_turnforce_on_x", car_turnforce_on_x); // 326 Повернуть авто по оси x (вверх).
	lua_register(L, "car_turnforce_on_y", car_turnforce_on_y); // 327 Повернуть авто по оси y (вбок).
	lua_register(L, "car_turnforce_on_z", car_turnforce_on_z); // 328 Повернуть авто по оси z (вокруг оси).

	lua_register(L, "car_moveforce_on_x", car_moveforce_on_x); // 329 Двигать авто по оси x.
	lua_register(L, "car_moveforce_on_y", car_moveforce_on_y); // 330 Двигать авто по оси y.
	lua_register(L, "car_moveforce_on_z", car_moveforce_on_z); // 331 Двигать авто по оси z.
	
	lua_register(L, "setcarrotate", setcarrotate); // 332 уст вращения авто.
	
	lua_register(L, "getcar_turn_on_x", getcar_turn_on_x); // 333 Получить угол поворота авто по оси x.
	lua_register(L, "getcar_turn_on_y", getcar_turn_on_y); // 334 Получить угол поворота авто по оси y.
	lua_register(L, "getcar_turn_on_z", getcar_turn_on_z); // 335 Получить угол поворота авто по оси z.
	
	lua_register(L, "carmovespeed_on_x", carmovespeed_on_x); // 336 двигать авто с скоростью по x.
	lua_register(L, "carmovespeed_on_y", carmovespeed_on_y); // 337 двигать авто с скоростью по y.
	lua_register(L, "carmovespeed_on_z", carmovespeed_on_z); // 338 двигать авто с скоростью по z.

	lua_register(L, "car_rotate_on_x", car_rotate_on_x); // 339 двигать авто по оси x.
	lua_register(L, "car_rotate_on_y", car_rotate_on_y); // 340 двигать авто по оси y.
	lua_register(L, "car_rotate_on_z", car_rotate_on_z); // 341 двигать угла авто по оси z.
	lua_register(L, "car_setrotate_on_x", car_setrotate_on_x); // 342 уст авто по оси x.
	
	lua_register(L, "car_setrotate_on_y", car_setrotate_on_y); // 343 уст авто по оси y.
	lua_register(L, "car_setrotate_on_z", car_setrotate_on_z); // 344 уст авто по оси z.
	lua_register(L, "getcar_resistance", getcar_resistance); // 345 Получить сопротивления авто трению.
	lua_register(L, "car_rotate_turn_on_x", car_rotate_turn_on_x); // 346 уст угол поворота авто по оси x.

	lua_register(L, "car_rotate_turn_on_y", car_rotate_turn_on_y); // 347 уст угол поворота авто по оси y.
	lua_register(L, "car_rotate_turn_on_z", car_rotate_turn_on_z); // 348 уст угол поворота авто по оси z.

	lua_register(L, "setcar_resistance", setcar_resistance); // 349 Уст силу сопротивления авто трению.
	lua_register(L, "getcar_turns_on_axes", getcar_turns_on_axes); // 350 получить углы авто поворота по осям.
	lua_register(L, "setcar_at_on_x", setcar_on_x); // 351 уст авто по оси x.
	lua_register(L, "setcar_at_on_y", setcar_on_y); // 352 уст авто по оси y.

	lua_register(L, "setcar_at_on_z", setcar_on_z); // 353 уст авто по оси z.
	lua_register(L, "car_turn_on_x_with_speed", car_turn_on_x_with_speed); // 354 повeрнуть авто по оси x на угол со скорость.
	lua_register(L, "car_turn_on_y_with_speed", car_turn_on_y_with_speed); // 355 повeрнуть авто по оси y на угол со скорость.
	lua_register(L, "car_turn_on_z_with_speed", car_turn_on_z_with_speed); // 356 повeрнуть авто по оси z на угол со скорость.

	lua_register(L, "car_turn_on_x_with_delay", car_turn_on_x_with_delay); // 357 повeрнуть авто по оси x на угол со задержкой.
	lua_register(L, "car_turn_on_y_with_delay", car_turn_on_y_with_delay); // 358 повeрнуть авто по оси y на угол со задержкой.
	lua_register(L, "car_turn_on_z_with_delay", car_turn_on_z_with_delay); // 359 повeрнуть авто по оси z на угол со задержкой.
	lua_register(L, "set_car_mass", set_car_mass); // 360 установить массу авто на значение.
	lua_register(L, "set_car_acceleration", set_car_acceleration); // 361 установить ускорение авто на значение.
	lua_register(L, "set_car_max_speed", set_car_max_speed); // 362 установить максимальную скорость авто на значение.
	lua_register(L, "set_car_traction", set_car_traction); // 363 установить сцепление авто на значение.

	lua_register(L, "create_rope_on_cords_with_swat", create_rope_on_cords_with_swat); // 364 создать веревку на координатах с бойцом спецназа.
	lua_register(L, "create_rope_on_cords", create_rope_on_cords); // 365 создать веревку на координатах.
	lua_register(L, "heli_change_height", heli_change_height); // 366 изменить высоту вертолета.

	lua_register(L, "exitcar", exitcar); // 367 выйти из авто.

	return 0;
};

int final_scripts() {
	bool k = false;	star_coroutine::set(k);// запретить вторые потоки в lua скриптах.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
	CTheScripts::ScriptSpace[OnAMissionFlag] = k;// выключить флаг миссии.
	for (auto L : luastate) {
		lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 100);// отключить хук.
		while ((LUA_YIELD == lua_status(L)) || (LUA_OK != lua_status(L))) { this_thread::sleep_for(chrono::milliseconds(1)); }
		destroy(L);// удалить все объекты.
		lua_gc(L, LUA_GCCOLLECT, 100); // включить сборку мусора.
		cleanstl();	//
	};
	for (auto L : luastate) { luastate.pop_front(); };
	   
	return 0;
};
int pause_scripts() {
	bool k = false;	star_coroutine::set(k);// запретить вторые потоки в lua скриптах.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
	CTheScripts::ScriptSpace[OnAMissionFlag] = k;// выключить флаг миссии.
	for (auto L : luastate) {
		lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 100);// отключить хук.
	}
	return 0;
};

int reload() {// перегрузка по нажатию клавиши.
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (KeyPressed(VK_CONTROL) || !star_thread::get()) {
			break;
		}
	};
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1)); //|| m == 7 || m == 10
		if (!KeyPressed(VK_CONTROL) ) {// перезагрузка скрипта.
			CMessages::AddMessageJumpQ(L"Script reloaded", 2000, 1);

			bool k = false;	star_coroutine::set(k);// запретить вторые потоки в lua скриптах.
			unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
			CTheScripts::ScriptSpace[OnAMissionFlag] = k;// выключить флаг миссии.
			for (auto L : luastate) {
				lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 100);// отключить хук.
				while ((LUA_YIELD == lua_status(L)) || (LUA_OK != lua_status(L))) { this_thread::sleep_for(chrono::milliseconds(1)); }
				destroy(L);// удалить все объекты.
				lua_gc(L, LUA_GCCOLLECT, 100); // включить сборку мусора.
				cleanstl();	//
			};
			for (auto L : luastate) { luastate.pop_front(); };

			this_thread::sleep_for(chrono::milliseconds(100));
			//writelog3("reload");
			std::thread(start_lualoder).detach();
			break;
			}
		if (!star_thread::get()) {
			break;
		  
		}
	};

	return 0;
};

int star_mission_marker(lua_State* L) {// создать маркер для миссии.
	static int point;	static int create = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.

			int t = lua_tointeger(L, 1);// id метки на карте.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);  CVector p = { x, y, z };
			//this_thread::sleep_for(chrono::milliseconds(10));
			CPed* player = FindPlayerPed();
			unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
			bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);
			bool arest = CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED;
			if ((!player->m_bInVehicle) || getflagmission || (!player->m_fHealth > 0.10f) || (arest) && (create == 1)) {// в авто пед?

				create = 0;
				Command<COMMAND_REMOVE_BLIP>(point);// удалить метку на карте.>(p.x, p.y, p.z, t, size, &point);
				lua_pushboolean(L, false);
				return 1;
			}
			if (player->m_bInVehicle && create == 0 && !getflagmission) {// в авто пед?
				create = 1;
				Command<COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT>(p.x, p.y, p.z, t, &point);
				lua_pushboolean(L, false);
				return 1;
			}
			if (player->m_bInVehicle && !getflagmission && (player->m_fHealth > 0.10f) && (!arest) && (create == 1)) {// в авто пед?
				CVehicle* car = player->m_pVehicle;
				bool place = car->IsSphereTouchingVehicle(x, y, z, 3.0);
				if (place && (car->m_fTotSpeed < 0.1f)) {
					create = 0;
					Command<COMMAND_REMOVE_BLIP>(point);// удалить метку на карте.>(p.x, p.y, p.z, t, size, &point);
					lua_pushboolean(L, true); 	//std::thread(check_defined_and_arest);
					return 1;
				}

				if (!player->m_bInVehicle || getflagmission || (!player->m_fHealth > 0.10f) || (arest) && (create == 1)) {// в авто пед?
						//create = 0; CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;
					Command<COMMAND_REMOVE_BLIP>(point);// удалить метку на карте.>(p.x, p.y, p.z, t, size, &point);
					lua_pushboolean(L, false);
					return 1;
				}
			}
		}// int

		else { throw "bad argument in function star_mission_marker"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int opendoorcar(lua_State* L) { // открыть дверь авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
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
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int draw_corona(lua_State* L) {// создать корону.
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)
			&& LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8)
			&& LUA_TNUMBER == lua_type(L, 9) && LUA_TNUMBER == lua_type(L, 10)) {

			bool switc = lua_toboolean(L, 1);// вкл\выкл корону.
			float radius = lua_tonumber(L, 2);// радиус короны.
			int type = lua_tointeger(L, 3);// тип.
			int glow_flare = lua_tointeger(L, 4); // свечение. 
			int red = lua_tointeger(L, 5);// цвета 
			int green = lua_tointeger(L, 6);
			int blue = lua_tointeger(L, 7);
			float x = lua_tonumber(L, 8); // координаты.
			float y = lua_tonumber(L, 9);
			float z = lua_tonumber(L, 10);
			
			corona::set(switc, x, y, z, radius, type, glow_flare, red, blue, green);

			/*	CVector pos = { x,y,z };
				Command<COMMAND_DRAW_CORONA>(pos.x, pos.y, pos.z, radius, type, glow_flare, red, green, blue);*/
			return 0;
		}// int

		else { throw "bad argument in function draw_corona"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int game_font_print(lua_State* L) {// вывести особенный игровой текст.
	try {
		if (LUA_TTABLE == lua_type(L, -1)) {// получаем таблицу из 12 элементов. 
			for (int i = 1; i < 12; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2); lua_insert(L, i);
			}
			lua_pop(L, 1);
			bool printgame = lua_toboolean(L, 1);//включить отобржения на экране
			string strprintgame = lua_tostring(L, 2);
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
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Opendoorcar(lua_State* L) { // Макрос открыть все двери авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			CAutomobile* automobile = reinterpret_cast<CAutomobile*>(car); // опять же, приведение типов. Т.к. мы будет юзать damageManager, нам нужно убедиться, что транспорт - это автомобиль (CAutomobile)
			DoorsExample::EnableDoorEvent(automobile, BONNET); // 0 капот
			DoorsExample::EnableDoorEvent(automobile, BOOT); // 1 багажник
			DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_LEFT); // 2 левая передняя дверь
			DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_RIGHT); // 3 правая передняя дверь
			DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_LEFT); // 4 левая задняя дверь
			DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_RIGHT); // 5 правая задняя дверь
			return 0;
		}
		else { throw "bad argument in function Opendoorcar option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_anim(lua_State* L) {// анимация.
	try {// CAnimManager::BlendAnimation(PlayerPed->rwObject, 0, 14, 10000.0f);
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

char q;
static string faststr = "";
void getkeyenvent() {// считывания символов клавиатуры.

	while (star_thread::get()) {
		this_thread::sleep_for(chrono::milliseconds(1));
		for (q = 8; q <= 190; q++) {
			string faststr = getkey(q);// получаем символ нажатой клавиши.
			while (star_thread::get()) {
				this_thread::sleep_for(chrono::milliseconds(1));
				string f2 = getkey(q);// Ждем отпускание клавиши.
				if (faststr != f2) {
					break;
				}
			};
			if (faststr != "") { cheatstr = cheatstr + faststr; }
			int size = cheatstr.size();// Если длина строки больше 10 символов, удаляем строку.
			if (size > 19) { cheatstr.clear(); }
		}
	}
};

int timerstar() {
	CMenuManager& MenuManager = FrontEndMenuManager;// менеджер меню из SDK (раньше хардкод 0x869630).
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (MenuManager.m_nCurrentMenuPage == 12) {
		/*	writelog3("menu");*/
			break;
		}
	};
	//iters = 0;
	//while (iters < 280) {
	//	this_thread::sleep_for(chrono::milliseconds(1));
	//}
	std::thread(start_lualoder).detach();
	return 0;
};


int setobjоcoordes(lua_State* L) {// установить координаты для объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 1)) {//указатель на объект и координаты.

			const void* p = lua_topointer(L, 1);
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
	for (auto it = firesids.begin(); it != firesids.end(); ++it) {
		if (L == it->second) {
			i = it->first;
			Command<COMMAND_REMOVE_SCRIPT_FIRE>(i);// удалить огонь.
			firesids.erase(i);
		}
	}

	for (auto pick = mappeds.begin(); pick != mappeds.end(); ++pick) {
		if (L == pick->second) {
			CPed* ped = pick->first; ped->ClearInvestigateEvent();// пед уходит, опустить педа.

			ped->ClearObjective(); // снять задачи с педа.
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
			mappeds.erase(pick);
			CEntity* p1 = (CEntity*)ped;
			if (p1 != NULL) {//obj->Remove();
				CWorld::Remove(p1);
			}
		}
	}

	for (auto cars = mapcars.begin(); cars != mapcars.end(); ++cars) {
		if (L == cars->second) {
			CVehicle* car = cars->first;
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			mapcars.erase(cars);
			CEntity* p1 = (CEntity*)car;
			CPed* player = FindPlayerPed();// найти томми.
			if (p1 != NULL && p1 != player->m_pVehicle) {//obj->Remove();
				CWorld::Remove(p1);
			}
		}
	}

	for (auto objs = mapobjs.begin(); objs != mapobjs.end(); ++objs) {
		if (L == objs->second) {
			CObject* obj = objs->first;
			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
			mapobjs.erase(objs);

			CEntity* p1 = (CEntity*)obj;
			if (p1 != NULL) {//obj->Remove();
				CWorld::Remove(p1);
			}
		}
	}
	bool s = false;
	corona::set(s, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
		CEntity* p1 = (CEntity*)ped;
		if (p1 != NULL) {//obj->Remove();
			CWorld::Remove(p1);
		}

		for (auto cars = mapcars.begin(); cars != mapcars.end(); ++cars) {
			CVehicle* car = cars->first;
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			mapcars.erase(cars);
			CEntity* p1 = (CEntity*)car;

			CPed* player = FindPlayerPed();// найти томми.
			if (p1 != NULL && p1 != player->m_pVehicle) {//obj->Remove();
				CWorld::Remove(p1);
			}
		}

		for (auto objs = mapobjs.begin(); objs != mapobjs.end(); ++objs) {
			CObject* obj = objs->first;
			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
			mapobjs.erase(objs);
			CEntity* p1 = (CEntity*)obj;
			if (p1 != NULL) {//obj->Remove();
				CWorld::Remove(p1);
			}
		}
		bool s = false;
		corona::set(s, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	return 0;
};
int f(lua_State* L1, char const* luaname) {
	

	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;// дать денег 
	//reversestack(L1); //инвертировать содержимое стека.
	//lua_pop(L1, 1);
	//args = lua_gettop(L1);// получить аргументы для второго потока.

	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег  
	showstack(L1);
	//reversestack(L1); //инвертировать содержимое стека.

	//if (LUA_TFUNCTION == lua_type(L1, -1)) {
		//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;// дать денег  

			//for (int i = 1; i < args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.

		//args--;
		//lua_resume(L1, NULL, args);
	//	return 0;
	//}
	return 0;
};
int create_newthread(lua_State* L) {// создания нового потока.
	try {
		if (LUA_TFUNCTION == lua_type(L, 1)) {
			lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
			lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
			char const* luaname = lua_tostring(L, -1);//имя lua скрипта.
			lua_pop(L, 1);	lua_State* L1 = luaL_newstate();
			luaL_openlibs(L1);	funs(L1);// список весь функций.

			int stacksize = lua_gettop(L);	stacksize++;
			for (int i = 1; i < stacksize; i++) {lua_pushvalue(L, i);// копировать на вершину стека.
				lua_xmove(L, L1, 1);// Снимает с L1 элементов передает L.
			};

			luastate.push_back(L1);// добавить новое состояние в list
			int args = lua_gettop(L1);
			if (0 == luaL_loadfile(L1, luaname)) {// Текущий lua файл.      
				lua_pcall(L1, 0, 0, 0);// запуск файла.
				lua_pushvalue(L1, 1);//скопировать имена функции, отправить на вершину стека.
				std::thread([=]() {lua_resume(L1, NULL, args, NULL); }).detach();
				this_thread::sleep_for(chrono::milliseconds(301));// задержка
				lua_settop(L, 0);
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function create_newthread"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};


// 0x9B48EC 60 4 карды в секунду

//if ((iters < 1) && (star_thread::get())) { //bool k = false;	star_thread::set(k);
//}
//	bool star_second_thread = star_thread::get();// получить флаг запуска второго потока.
	//CPed* player = FindPlayerPed();// найти игрока.
	//if (player != NULL) {// проверка найден пед.
/*Events::initGameEvent +=[]{
	writelog("new game");
};*/
//Events::initScriptsEvent += [] {
//	writelog("load");
//};

		//bool k = false;// флаг, что уже запущен поток. 
		//star_thread::set(k);
//if ((Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus)) && star_second_thread == false && CTimer::m_snTimeInMilliseconds - time > 13500) {

/*CVector pos1 = { x, y, z };
CVector* pos = (CVector*)& pos1;*/
//eWeaponType type = weapon.m_nType;
//CShotInfo* aShotInfos;
//Command<COMMAND_SET_CHAR_HEED_THREATS>(CPools::GetPedRef(ped), 1);// пед атакует любого, кто нападает на него.

	//aShotInfos->AddShot(ped, type, ped->GetPosition(), pos1);
//aShotInfos->AddShot(ped, type, ped->GetPosition(), pos1);
//.aShotInfos->Update();

//int check_defined_and_arest() {
//	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10;// дать денег  
//	CPed* player = FindPlayerPed();// найти игрока
//	while (true) {
//		this_thread::sleep_for(chrono::milliseconds(10));
//		bool arest = CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED;
//		if (arest || (player->m_fHealth < 0.10f)) {
//			CMessages::AddBigMessage(L"mission failed!", 2500, 0);//0 большими, розовые как миссии пройдена,
//			this_thread::sleep_for(chrono::milliseconds(10));
//			setflagmission(0);
//			break;
//		}
//	};
//	return 0;
//};

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
	//	if (!std::filesystem::exists("./threads")) {// Если нет папки создаем
	//		std::filesystem::create_directories("./threads");
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
	//	ret1 = lua_resume(L, L1, 0, NULL);
	//	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += ret1;// дать денег 	
	//	while (true) {
	//		if (ret == LUA_YIELD) {//&& ret1 != LUA_YIELD
	//	lua_getglobal(L1, func);
	//	ret = lua_resume(L1, L, args, NULL);//запуск FUNC
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
	//			ret = lua_resume(L1, L, args, NULL); //this_thread::sleep_for(chrono::milliseconds(100));
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
	//		res = lua_resume(L, L1, 0, NULL); // main
	//		lua_pop(L, 1);
	//		if (res == 0) { break; }
	//		else {res1 = lua_resume(L1, L, 0);// foo
	//		}
	//			/*L1 = lua_tothread(L, -1); lua_pop(L, 1); lua_xmove(L, L1, lua_gettop(L)); lua_remove(L1, 1); args = lua_gettop(L1);
	//			for (int i = 1; i > args; i++) { lua_pushvalue(L1, i); }
	//			args--;	res1 = lua_resume(L1, L, args, NULL);	lua_xmove(L, L1, 1);
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
	//					res1 = lua_resume(L1, L, args, NULL); //lua_settop(L1, args++);;	
	//					showstack1(L1);
	//					args++;					} 					} 
	//				}
	//        }
	//   if ( res1 == LUA_YIELD ||  res1 == LUA_OK ) {
	//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;	
	//	   lua_sethook(L, LUAHook, LUA_MASKCOUNT, 10);	
	//	   lua_getglobal(L, "main");
	//   res = lua_resume(L, L1, 0, NULL); //int li =  //  
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
