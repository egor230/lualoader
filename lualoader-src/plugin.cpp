#define _CRT_SECURE_NO_WARNINGS
#include "funcs.h"
using namespace plugin;
using namespace std;

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

int f(lua_State* L1, char const* luaname) {

	showstack(L1);
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


	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;// дать денег
	//reversestack(L1); //инвертировать содержимое стека.
	//lua_pop(L1, 1);
	//args = lua_gettop(L1);// получить аргументы для второго потока.

	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// дать денег
//reversestack(L1); //инвертировать содержимое стека.

//if (LUA_TFUNCTION == lua_type(L1, -1)) {
	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;// дать денег

		//for (int i = 1; i < args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.

	//args--;
	//lua_resume(L1, NULL, args);
//	return 0;
//}
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
