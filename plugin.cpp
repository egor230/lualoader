#define _CRT_SECURE_NO_WARNINGS
#include<funcs.h>

using namespace plugin;
using namespace std;

int setobjоcoordes(lua_State* L); // установить координаты для объект.
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
	static double abc_x; static double ord_y; static double pos_z; static double radius;
	static int type; static int glow_flare; static int red; static int blue;
	static int green;

	static void set(bool switc1, double abc_x1, double ord_y1, double pos_z1, double radius1,
		int type1, int glow_flare1, int red1, int blue1, int green1) { // установить цвет короны.
		switc = switc1;
		abc_x = abc_x1; ord_y = ord_y1;	pos_z = pos_z1; type = type1; radius = radius1; glow_flare = glow_flare1;
		red = red1; blue = blue1; green = green1;
	}
	static void draw() {// вывод короны.
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


	mtx.lock();// заблок.
	state Lua; lua_State* L = Lua.get();

	lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
	char str123[255]; auto j = std::experimental::filesystem::current_path();
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
			lua_pcall(L, 0, 0, 0);// запуск файла.

			mtx.unlock();// разблок.
			lua_getglobal(L, "main");
			if (LUA_TFUNCTION == lua_type(L, -1)) {
				luastate.push_back(L);// добавить указатель на lua состояния в list.
				lua_resume(L, NULL, 0);	// запуск файла.
				lua_State* L1 = lua_newthread(L);// создать новый поток.

				if (!star_coroutine::get())// если нельзя запустить второой поток в скрипте.
				{
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
							lua_resume(L1, L, args);
						}
						if (LUA_YIELD == lua_status(L1)) {// если второй на паузе.
							lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 800); //вызов функции с заданной паузой.
							lua_resume(L, L1, 0);// возобновить основной поток.
						}
						if (LUA_OK == lua_status(L1)) {// если второй поток завершен.      
							lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
							lua_resume(L, NULL, 0);
						}
						if (LUA_YIELD == lua_status(L) || LUA_YIELD == lua_status(L1) && (!star_coroutine::get())) {
							break;
		}   }	}	}	else { /*lua_close(L);*/ }

		}
		else {	string er1 = lua_tostring(L, -1); string er0 = "could not load " + er1;
			char* x = strdup(er1.c_str());
			mtx.unlock();// разблок.
			writelog(x);}// записать ошибку в файл.
	return 0;
};

void search() {// поиск всех lua файлов для запуска.
    dellod();

	for (auto const& de : std::experimental::filesystem::recursive_directory_iterator{
		std::experimental::filesystem::current_path() / "lualoader" }) { // папка для поиска
		if (de.path().extension() == ".lua" || de.path().extension() == ".LUA") {
			string res = de.path().string();// перевод имя файла в строку.
			char* luafile = strdup(res.c_str());// Текущий lua файл.
			listfile.push_back(luafile);// добавить текущий lua файл в list.

			std::thread(startscipt, res, luafile, std::ref(luastate)).detach();// независимым поток для запуска lua файла.
			
		}
	};
};

int start_lualoder() { // найти все lua файлы. меню 12,	старт новой игры 1.	
    std::thread(search).detach();// поиск и запуск lua файлов.
	std::thread(reload).detach(); // перегрузка скрипта по нажатию клавиши.

	std::thread(getkeyenvent).detach();// считывания символов клавиатуры.
	// Новая игра 7	 загрузка 8 точно загрузка 10 в игре 32.
	// 8, 1, 10 загрузка. // 1, 7	новая игра. 32 в игр
	CPed* player = FindPlayerPed();// найти игрока

	CMenuManager& MenuManager = *(CMenuManager*)0x869630;
   while (true) {	this_thread::sleep_for(chrono::milliseconds(1));
		if (MenuManager.m_nCurrentPage = 32) {
			break;
		}

	};
   while (true) {
	   this_thread::sleep_for(chrono::milliseconds(1));
	   if (MenuManager.m_nCurrentPage = 12) {
		   break;
	   }

   };
   while (true) {
	   this_thread::sleep_for(chrono::milliseconds(1));
	   if (MenuManager.m_nCurrentPage = 32) {
		   break;
	   }

   };
	while (true) {	this_thread::sleep_for(chrono::milliseconds(1)); //|| m == 7 || m == 10
		if ((MenuManager.m_nCurrentPage == 10) || (MenuManager.m_nCurrentPage == 7)) {// перезагрузка скрипта.
			break;	}
	};

	while (true) {	this_thread::sleep_for(chrono::milliseconds(1));
		if ((MenuManager.m_nCurrentPage == 8) || (MenuManager.m_nCurrentPage == 1)) {// точно загрузка и новая игра.
			 final_scripts();
		     break;		}
		}; 
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		int m = MenuManager.m_nCurrentPage;
		writelog3(m);
		if (MenuManager.m_nCurrentPage = 12) {
			break;
		}
	};
	writelog3("reload");
	this_thread::sleep_for(chrono::milliseconds(100));

	  std::thread(timerstar).detach(); // запуск через загрузку сэйва.
	return 0;
}; 

bool s = true;
class Message {//имя класса.
public: Message() {	

	Events::gameProcessEvent += [] {//обработчик событий игры.
		CPed* player = FindPlayerPed();// найти игрока.
		Events::gameProcessEvent += spite::draw; Events::gameProcessEvent += corona::draw; Events::vehicleRenderEvent += DoorsExample::ProcessDoors; // Тут обрабатываем события, а также выключаем их
		int number_save_slot = patch::GetUShort(0x9B5F08);// номер слота.
		int gtg = patch::GetUShort(0x974B2C);// глобальный таймер.
		if (number_save_slot == 9 && !star_thread::get()) {// скрипты запрещены и второй поток запущен.

			if ((Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus)) && gtg < 1000) { // новая игра
				star_thread::set(s);
				std::thread(start_lualoder).detach();
				  //writelog3("new");
			}

			else {// загруженая игра.
				if (gtg > 1000) { writelog3("load gtg > 1000");
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

//if ( star_scripts::get() && !star_thread::get()) {// скрипты запрещены и второй поток запущен.
//					writelog3("reload load gtg > 1000");
//	if ((Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus)) && gtg < 1000) { // новая игра
//		//.std::thread(start_lualoder).detach();
//		//star_scripts::set(s);// разрешить запускать скрипты.			   //writelog3("new");
//	}

//	else {// загруженая игра.
//		if (gtg > 1000) {
//			//std::thread(start_lualoder).detach();
//			//star_scripts::set(s);
//		}// разрешить запускать скрипты.
//	}
//}
int funs(lua_State* L) {// список функций.

	//set_path_to_module(L); // уст путь к модулю.
	lua_register(L, "findplayer", findplayer); // 1 возвращает указатель педа.
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
	lua_register(L, "is_car_upright", is_car_upright); // 109 020D: car 12@ flipped если указанный автомобиль перевернут.
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

	lua_register(L, "exitcar", exitcar); // 245 выйти из авто.

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

int reload() {// перегрузка по нажатию клавиши.
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (KeyPressed(VK_CONTROL)) {
			break;
		}
	};
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1)); //|| m == 7 || m == 10
		if (!KeyPressed(VK_CONTROL)) {// перезагрузка скрипта.
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
	};

	//CPed* player = FindPlayerPed();// найти игрока
	//while (star_scripts::get() && !player) {
	//	this_thread::sleep_for(chrono::milliseconds(1));
	//}
	//bool flag_script = true;
	//star_scripts::set(flag_script);// запретить запускать скрипты.
	//writelog3("reload key");

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
		if (LUA_TTABLE == lua_type(L, -1)) {// получаем таблицу из 9 элементов.
			for (int i = 1; i < 11; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2); lua_insert(L, i);
			}

			bool switc = lua_toboolean(L, 1);// вкл\выкл корону.
			double radius = lua_tonumber(L, 2);// радиус короны.
			int type = lua_tointeger(L, 3);// тип.
			int glow_flare = lua_tointeger(L, 4); // свечение. 
			int red = lua_tointeger(L, 5);// цвета 
			int green = lua_tointeger(L, 6);
			int blue = lua_tointeger(L, 7);
			double x = lua_tonumber(L, 8); // координаты.
			double y = lua_tonumber(L, 9);
			double z = lua_tonumber(L, 10);
			lua_pop(L, lua_gettop(L));

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
	CMenuManager& MenuManager = *(CMenuManager*)0x869630;
	writelog3("reload");	
	while (iters < 280) {
		this_thread::sleep_for(chrono::milliseconds(1));
	}
	bool k = false;	star_thread::set(k);
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

				//weapon.Update(54, ped); 
				//weapon. ();
				//aShotInfos->Initialise();
				//this_thread::sleep_for(chrono::milliseconds(1));
				//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег 
				//weapon.Shutdown();
				//weapon.UpdateWeapons();

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
