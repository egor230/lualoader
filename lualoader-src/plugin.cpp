#define _CRT_SECURE_NO_WARNINGS
#include "funcs.h"
#include <map>
#include <mutex>
#include <set>
#include <vector>
#include <thread>
#include <atomic>
#include <exception>// std::current_exception/rethrow_exception в lua_terminate_handler
#include "compat/diag.h"// диагностика + ОЧЕРЕДЬ КОМАНД (реализация — compat/diag.cpp, cmdq_hooks.cpp)
using namespace plugin;
using namespace std;

static map<lua_State*, bool> scriptRunning;// флаг «поток скрипта ещё жив» (по lua_State).
static mutex scriptMutex;
static void script_started(lua_State* L) { lock_guard<mutex> lk(scriptMutex); scriptRunning[L] = true; }
static void script_stopped(lua_State* L) { lock_guard<mutex> lk(scriptMutex); scriptRunning[L] = false; }
static bool script_is_running(lua_State* L) { lock_guard<mutex> lk(scriptMutex); auto it = scriptRunning.find(L); return it != scriptRunning.end() && it->second; }
static std::atomic<bool> reload_busy(false);// защита от одновременных перезагрузок.
static std::atomic<bool> scripts_disabled(false);// Ctrl = ВЫКЛ: скрипты выгружены и сами больше не запускаются.
std::atomic<bool> scripts_paused(false);// ПАУЗА по yield (extern в funcs.h): wait() делает lua_yield, pump-потоки паркуются.
std::atomic<bool> teardown_active(false);// ПАУЗА на время разборки (extern в funcs.h): блокирующие API рвут свои циклы.
// --- Teardown «по владельцу» (async): каждый pump-поток закрывает СВОЁ lua_State. ---
// Поколения состояний: эпоха (g_lua_epoch) + stamp-карта (функции в funcs.cpp).
// teardown инкрементирует эпоху — все существующие состояния становятся устаревшими
// МГНОВЕННО (без ожидания); их wait() отдаёт ход владельцу на ближайшем ~10мс-границе,
// владелец сам закрывает своё состояние. Новые скрипты получают новую эпоху и не трогаются.
static std::atomic<int>  active_pumps(0);       // живые pump-потоки (включая ещё не стартовавшие).
static std::set<lua_State*> pumpLive;           // состояния, чей владелец-поток жив (опубликованы в luastate).
static std::mutex pumpLive_mx;
static std::mutex luastate_mx;                  // доступы к luastate — только под этим мутексом.

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
	string path = "lualoader\\log.txt";// 19.10: единый журнал (отдельный log.txt в корне игры запрещён).
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);// датируем загрузки скриптов.
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};
void writelog3(int x) {// запись ошибок в файл.
	string path = "lualoader\\log.txt";// 19.10: единый журнал (отдельный log.txt в корне игры запрещён).
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);// датируем загрузки скриптов.
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};
// Опубликовать состояние: список + владелец регистрируются ДО того, как состояние станет
// видно другим потокам/свипу — закрытие чужого состояния исключено по построению.
static void publish_state(lua_State* L) {
	std::lock_guard<std::mutex> lk(luastate_mx); luastate.push_back(L);
	std::lock_guard<std::mutex> pk(pumpLive_mx); pumpLive.insert(L);
	active_pumps.fetch_add(1);
	lua_state_stamp(L);// состояние относится к ТЕКУЩЕМУ поколению — reload его не тронет.
}
// Владелец закончил НОРМАЛЬНО (не reload): состояние остаётся в luastate (его подберёт след. reload-свип).
static void drop_owner(lua_State* L) {
	std::lock_guard<std::mutex> pk(pumpLive_mx); pumpLive.erase(L);
	active_pumps.fetch_sub(1);
	lua_state_unstamp(L);
}
// reload: владелец полностью разбирает СВОЁ состояние (отослать terminate, destroy, убрать из списков).
// lua_close НЕ зовём (закрытие живого состояния роняло игру) — состояние остаётся, но никому не мешает.
static void owner_selfclose(lua_State* L) {
	cpp_tracef("owner_selfclose: состояние %p разбирает его владелец", (void*)L);
	lua_getglobal(L, "onScriptTerminate");
	if (lua_type(L, -1) == LUA_TFUNCTION) {
		if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
			const char* err = lua_tostring(L, -1);
			if (err) { writelog(err); ma_diag::lua_error_note("onScriptTerminate: %s", err); }
			lua_pop(L, 1);
		}
	}
	else { lua_pop(L, 1); }
	destroy(L);
	std::lock_guard<std::mutex> lk(luastate_mx); luastate.remove(L);
	std::lock_guard<std::mutex> pk(pumpLive_mx); pumpLive.erase(L);
	active_pumps.fetch_sub(1);
	lua_state_unstamp(L);
	cpp_trace("owner_selfclose: состояние закрыто владельцем");
}
// Единый хостинг pump-потока. Состояние ДОЛЖНО быть уже опубликовано publish_state().
// L1 — «второй поток» (lua_newthread внутри L); для create_newthread приходим как
// pump_owner(L1, nullptr, args, true) — свой поток уже сам первый resume делает сам.
static void pump_owner(lua_State* L, lua_State* L1, int args, bool first_resume) {
	script_started(L);
	if (first_resume) lua_resume(L, NULL, args, NULL);
	while (LUA_OK != lua_status(L) && !lua_state_obsolete(L)) {
		this_thread::sleep_for(chrono::milliseconds(1)); // задержка.
		if (scripts_paused.load()) continue;// ПАУЗА по yield: скрипт сам замер (wait() -> lua_yield),
		                                     // ничего не закрываем и не ресумеим — просто паркуемся (обратимо).
		if (L1 != NULL && LUA_TFUNCTION == lua_type(L1, -1) && LUA_YIELD == lua_status(L) && star_coroutine::get()) {
			for (int i = 1; i <= args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.
			lua_resume(L1, L, args, NULL);
		}
		if (L1 != NULL && LUA_YIELD == lua_status(L1)) {// если второй на паузе.
			lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 800); //вызов функции с заданной паузой.
			lua_resume(L, L1, 0, NULL);// возобновить основной поток.
		}
		if (L1 != NULL && LUA_OK == lua_status(L1)) {// если второй поток завершен.
			lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// отключить хук.
			lua_resume(L, NULL, 0, NULL);
		}
		if (LUA_YIELD == lua_status(L) || (L1 != NULL && LUA_YIELD == lua_status(L1) && !star_coroutine::get())) {
			break;
		}
	}
	script_stopped(L);
	if (lua_state_obsolete(L)) { owner_selfclose(L); }
	else { drop_owner(L); }
}
// Единая разборка — АСИНХРОННАЯ: сначала ПАУЗА ВСЕХ СКРИПТОВ, потом закрытие по владельцам.
// ПАУЗА (шаги 1-2): глобальный флаг teardown_active рвёт блокирующие API
// (play_voice/load_model/getcord/expectations), флаг миссии+star_coroutine выгоняют из главных
// циклов, эпоха+1 заставляет wait() отдать ход. БЕЗ форс-hook: hook-йield внутри C-вызова
// рвёт корутину с ошибкой (вылет) — скрипты останавливаются только yield из СВОЕГО wait().
// Теardown НИКОГО не ждёт (никакого зависания), сирот (поток уже мёртв) закрываем сейчас.
static void teardown_all(bool restart) {
	if (reload_busy.exchange(true)) { cpp_trace("teardown: другая разборка уже идёт — пропуск"); return; }
	cpp_tracef("teardown: вход (restart=%d), pump-потоков: %d, состояний: %u", (int)restart, active_pumps.load(), (unsigned)luastate.size());
	// ---- 1. ПАУЗА ВСЕХ СКРИПТОВ (до любой разборки) ----
	teardown_active.store(true);// блокирующие API рвут свои длинные циклы (тела скриптов успокаиваются).
	bool k = false;	star_coroutine::set(k);// запретить вторые потоки в lua скриптах.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
	CTheScripts::ScriptSpace[OnAMissionFlag] = k;// выключить флаг миссии (скрипты выходят из своих циклов).
	stop_mission_watch();// сторож миссии не должен трогать закрываемые состояния.
	// 2. РАЗБОРКА: переводим все существующие состояния в «устаревшее» поколение (эпоха+1).
	g_lua_epoch.fetch_add(1);// ВСЕ существующие состояния устарели — мгновенно, без ожидания.
	// 3. Свип сирот (ждут только состояния без живого владельца). Живых владельцев НЕ трогаем.
	std::vector<lua_State*> snapshot;
	{
		std::lock_guard<std::mutex> lk(luastate_mx);
		for (auto L : luastate) snapshot.push_back(L);
	}
	for (auto L : snapshot) {
		bool owned = false;
		{ std::lock_guard<std::mutex> pk(pumpLive_mx); owned = pumpLive.count(L) != 0; }
		// Живым владельцам break не ставим (форс-hook-йield внутри C-вызова рвёт корутину с ошибкой) —
		// они сами сворачиваются: wait() даёт yield по устаревшей эпохе, блокирующие API рвутся по
		// teardown_active. Сирот (поток уже мёртв) закрываем сейчас.
		if (owned) { continue; }
		// Сирота: владелец-поток уже завершился — состояние осиротело. Закрываем сейчас (своего потока нет — безопасно).
		cpp_tracef("teardown: сирота %p — закрываю", (void*)L);
		lua_getglobal(L, "onScriptTerminate");
		if (lua_type(L, -1) == LUA_TFUNCTION) {
			if (lua_pcall(L, 0, 0, 0) != LUA_OK) { const char* err = lua_tostring(L, -1); if (err) { writelog(err); ma_diag::lua_error_note("onScriptTerminate (сирота): %s", err); } lua_pop(L, 1); }
		} else { lua_pop(L, 1); }
		destroy(L);
		std::lock_guard<std::mutex> lk(luastate_mx); luastate.remove(L);
		lua_state_unstamp(L);
	}
	// Старые владельцы ещё живы и закроют свои состояния САМИ (асинхронно, через owner_selfclose) —
	// cleanstl() НЕ зовём: их destroy() создаёт СВОИ объекты, а освобождённые STL-хранилища будут
	// им только мешать. scriptRunning/pumpLive не чистим — сведения о живых владельцах ещё актуальны.
	reload_busy.store(false);// разблокируем для возможного нового reload (старые владельцы докончат сами).
	if (restart) {
		this_thread::sleep_for(chrono::milliseconds(100));// дать старым владельцам начать сворачиваться.
		std::thread(start_lualoder).detach();
	}
	// 4. ПАУЗА снята: новые скрипты (если есть) больше не видят флаг. Старые владельцы всё равно
	// остаются устаревшими по эпохе — они доводят закрытие своих состояний, когда доберутся до wait.
	teardown_active.store(false);
	cpp_trace("teardown: разборка запущена (владельцы закрывают свои состояния сами после yield)");
}
int startscipt(string res, char* luafile, list<lua_State*>& luastate) {// запуска скрипта.

	state Lua; lua_State* L = Lua.get(); lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
	script_started(L);// пометить состояние: поток скрипта жив.
	{ // Имя потока для отчёта о вылете (18.10.2026): set_thread_name существовал,
	  // но его никто не звал — в отчёте все скрипт-потоки были безымянными «?».
		string nm = std::filesystem::path(res).filename().string();
		ma_diag::set_thread_name(nm.c_str());
	}
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
			// Ошибку верхнего блока скрипта фиксируем в диагностику (18.10.2026):
			// раньше она молча оставалась на стеке и в отчёте о вылете не была видна.
			int pst = lua_pcall(L, 0, 0, 0);// запуск файла.
			if (pst != LUA_OK) { const char* perr = lua_tostring(L, -1); if (perr) { writelog(perr); ma_diag::lua_error_note("скрипт %s: %s", luafile ? luafile : "?", perr); } }
			lua_getglobal(L, "main");	//writelog3("star scpipt");
			if (LUA_TFUNCTION == lua_type(L, -1)) {
				publish_state(L);// владелец+список регистрируются ДО видимости состояния (teardown-по-владельцу).

			    lua_resume(L, NULL, 0, NULL);	// запуск файла.
				lua_State* L1 = lua_newthread(L);// создать новый поток.
				lua_state_stamp(L1);// второй поток тоже привязан к поколению (не считается «устаревшим»).

				if (!star_coroutine::get()){// если нельзя запустить второой поток в скрипте.

					//writelog3("exit");
					script_stopped(L);// поток скрипта завершается.
					drop_owner(L);// состояние остаётся в luastate — подберёт следующий reload.
					return 0;
				}
				else {
					int args = lua_gettop(L);// получить аргументы для второго потока.
					lua_xmove(L, L1, args);	args--;	reversestack(L1); //инвертировать содержимое стека.
					pump_owner(L, L1, args, false);// помп на СВОЁМ потоке: при reload владелец сам разберёт СВОЁ состояние.
				}
			}
			else { /*lua_close(L);*/ }

		}
		else {	string er1 = lua_tostring(L, -1); string er0 = "could not load " + er1;
			char* x = strdup(er1.c_str());		writelog(x);}// записать ошибку в файл.
		script_stopped(L);// поток скрипта завершается.
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
	static bool background_started = false;// reload/getkeyenvent спавним ОДИН раз на сессию:
	if (!background_started) {              // иначе на каждый старт скриптов плодятся новые потоки
		background_started = true;          // и при Ctrl-перезагрузке срабатывают сразу несколько reload.
		std::thread(reload).detach();       // одновременная чистка luastate несколькими reload = краш.
		std::thread(getkeyenvent).detach();// считывания символов клавиатуры.
	}
};

bool s = true;
int start_lualoder() { // найти все lua файлы. меню 12,	старт новой игры 1.
	if (scripts_disabled.load()) { star_thread::set(false); return 0; }// Ctrl выключил скрипты: сами не запускаемся.
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

static void lua_crash_write(const char* tag, const char* line);
// ==== Crash handler (SetUnhandledExceptionFilter) ====
// Пишем диагностику вылета ПРЯМЫМ WinAPI (CreateFileA/WriteFile, без CRT-буферов): на момент
// аварии fstream-буфер может быть не сброшен, а CRT-функции — не поддерживать потокобезопасность.
// Возвращаем EXCEPTION_CONTINUE_SEARCH — игра сама решает (диалог/завершение), НО мы успели
// оставить след в log.txt. Асинхронно-безопасно: никаких аллокаций/heap-обходов.
static void lua_crash_write(const char* tag, const char* line) {
	HANDLE h = CreateFileA("lualoader\\log.txt", FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		DWORD w = 0;
		if (tag) { WriteFile(h, tag, (DWORD)strlen(tag), &w, NULL); }
		WriteFile(h, line, (DWORD)strlen(line), &w, NULL);
		WriteFile(h, "\n", 1, &w, NULL);
		CloseHandle(h);
	}
}
static LONG WINAPI lua_crash_filter(EXCEPTION_POINTERS* ep) {
	EXCEPTION_RECORD* rec = ep ? ep->ExceptionRecord : NULL;
	unsigned long tid = GetCurrentThreadId();
	char buf[512];
	int n = 0;
	n += _snprintf(buf + n, sizeof(buf) - (size_t)n,
		"C++| CRASH: code=0x%08X addr=0x%p thread=%lu EXCEPTION.",
		rec ? (unsigned)rec->ExceptionCode : 0u,
		rec ? rec->ExceptionAddress : (void*)0,
		tid);
	if (rec) {
		switch ((unsigned)rec->ExceptionCode) {
			case EXCEPTION_ACCESS_VIOLATION: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ACCESS_VIOLATION (0xC0000005)"); break;
			case EXCEPTION_ILLEGAL_INSTRUCTION: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ILLEGAL_INSTRUCTION"); break;
			case EXCEPTION_STACK_OVERFLOW: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " STACK_OVERFLOW"); break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " INT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ARRAY_BOUNDS_EXCEEDED"); break;
			default: break;
		}
		// 0xC0000005: первый параметр — код нарушения (0=read,1=write,8=exec), второй — адрес.
		if ((unsigned)rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2) {
			n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " %s @0x%p",
				rec->ExceptionInformation[0] == 1 ? "WRITE" :
				rec->ExceptionInformation[0] == 0 ? "READ" : "EXEC",
				(void*)rec->ExceptionInformation[1]);
		}
	}
	// Lua-контекст (18.10.2026): какой скрипт/строка/API выполнялись в упавшем
	// потоке — прямой ответ «на каком месте скрипта вылет». "" если поток не Lua.
	{
		char lc[192];
		ma_diag::lua_context(lc, sizeof(lc));
		if (lc[0]) n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " %s", lc);
	}
	if (n < 0) n = 0; if (n > (int)sizeof(buf) - 2) n = (int)sizeof(buf) - 2;
	buf[n] = '\n'; buf[n + 1] = '\0'; n++;
	lua_crash_write(NULL, buf);
	return EXCEPTION_CONTINUE_SEARCH;
};
// Vectored-обработчик регистрируется с FIRST=1: вызывается ДО любых SEH по всей цепочке,
// включая собственный __try/__except reVC и его SetUnhandledExceptionFilter, перезаписанный
// после нашего (критично: в свежем трейсе пауза применилась, а наш фильтр НЕ отписал CRASH —
// значит вылет прервало что-то ВЫШЕ нас; vectored FIRST увидит его первым).
static LONG WINAPI lua_vectored_filter(EXCEPTION_POINTERS* ep) {
	EXCEPTION_RECORD* rec = ep ? ep->ExceptionRecord : NULL;
	unsigned code = rec ? (unsigned)rec->ExceptionCode : 0u;
	// Логируем ТОЛЬКО настоящие вылеты. First-chance исключения вида 0x40010006
	// (DBG_PRINTEXCEPTION) идут тысячами за прогон: они засоряют log.txt И,
	// что важнее, вызывают обработчик на каждом таком событии, сбивая тайминги
	// потоков — это само по себе меняет поведение гонок.
	if (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_ILLEGAL_INSTRUCTION ||
		code == EXCEPTION_STACK_OVERFLOW || code == EXCEPTION_INT_DIVIDE_BY_ZERO ||
		code == EXCEPTION_ARRAY_BOUNDS_EXCEEDED || code == EXCEPTION_DATATYPE_MISALIGNMENT) {
		// Lua-контекст (18.10.2026): какой скрипт/строка/API выполнялись.
		char lc[192];
		ma_diag::lua_context(lc, sizeof(lc));
		char buf[512];
		int n = _snprintf(buf, sizeof(buf), "VECTORED: code=0x%08X addr=0x%p thread=%lu%s%s",
			code, rec ? rec->ExceptionAddress : (void*)0, (unsigned long)GetCurrentThreadId(),
			lc[0] ? " " : "", lc);
		if (n < 0) n = 0;
		if (n > (int)sizeof(buf) - 2) n = (int)sizeof(buf) - 2;
		buf[n] = '\0';
		lua_crash_write("C++| ", buf);
	}
	return EXCEPTION_CONTINUE_SEARCH;// не мешаем игре обработать (только диагностика)
}

static void lua_terminate_handler() {
	lua_crash_write("C++| ", "STD::TERMINATE вызван (abort/необработанное C++ исключение).");
	// Текст исключения (18.10.2026): раньше в trace.txt не было видно, ЧТО именно
	// не поймано (часто это bad_alloc/out_of_range из-за испорченной кучи).
	// std::current_exception() при отсутствии активного исключения вернёт nullptr —
	// rethrow не будет.
	if (std::current_exception()) {
		try { std::rethrow_exception(std::current_exception()); }
		catch (const std::exception& e) { lua_crash_write("C++| what(): ", e.what()); }
		catch (...) { lua_crash_write("C++| ", "не-std::exception (чужой тип исключения)."); }
	}
	abort();
}

static void lua_atexit_handler() {
	lua_crash_write("C++| ", "ATEXIT: нормальный выход процесса.");
}
static void install_crash_filter() {
	AddVectoredExceptionHandler(1, lua_vectored_filter);// FIRST=1: видим любой вылет раньше reVC.
	SetUnhandledExceptionFilter(lua_crash_filter);// последняя инстанция фильтра (если reVC не перезаписал).
	std::set_terminate(lua_terminate_handler);
	atexit(lua_atexit_handler);
};// ставим ОДИН раз при инициализации.
// Опубликовать состояние: список + владелец регистрируются ДО того, как состояние станет
class Message {//имя класса.
public: Message() {
		install_crash_filter();// VEH + SetUnhandledExceptionFilter: пишем код/адрес вылета в log.txt.
		// compat/*.cpp не имеют глобальных конструкторов (иначе игра не стартовала:
		// RtlpWaitForCriticalSection "... blocked by 0000"). Регистрация — только отсюда,
		// из Message, когда SDK уже инициализирован.
		ma_diag::install();// VEH + журнал lualoader\log.txt + сторожевой поток (сердцебиение).
		ma_cmdq_install_hook();// РАЗБОР ОЧЕРЕДИ Command<> в gameProcessEvent (игровой поток).
		                       // Регистрируем ДО обработчика ниже, чтобы drain выполнился
		                       // первым в кадре и выставил id игрового потока (см. cmdq_hooks.cpp).
		ma_stream_install_hook();// КАДРОВЫЙ ОБРАБОТЧИК МЕНЕДЖЕРА СТРИМИНГА (18.10.2026):
		                         // REQUEST_MODEL/RequestSpecialChar по заявкам скриптов шлются
		                         // на игровом потоке (после drain: id игрового потока уже
		                         // выставлен) — worker-потоки больше не ходят кругами Command<>.

	Events::gameProcessEvent += [] {//обработчик событий игры.
		CPed* player = FindPlayerPed();// найти игрока.
		// ma_diag::heartbeat(0);// «игра жива»: кадры/время для отчёта сторожа (две атомарные записи).
		// // 18.10.2026: раз в кадр, ДО отрисовки — освободить отрисованные тексты сообщений
		// // (игра хранит УКАЗАТЕЛЬ на строку, поэтому освобождать сразу после Add*Message*
		// // было use-after-free) и обнулить мусорный стиль шрифта, если он появился
		// // (иначе PrintChar индексирует таблицу глифов мусором -> AV 0x551E0D).
		// ma_diag::msg_keepalive_purge();
		// ma_diag::font_style_guard();
		// ma_diag::radar_list_guard();// обнулённый head CList 0x704858 -> AV 0x4CA523 (сторож).
		// ma_diag::intro_text_guard();// мусорный font intro-строки -> AV 0x551E0D (сторож + карантин).
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
	teardown_all(false);// единая разборка по владельцам (без рестарта: загрузка сейва / выход в меню).
	return 0;
};
int pause_scripts() {
	// ПАУЗА всех скриптов через НАТИВНЫЙ yield: скрипты сами делают return lua_yield(L,0) в СВОЁМ wait().
	// Никакой форс-hook: hook-йield внутри не-yieldable C-вызова рвёт корутину с ошибкой (вылет).
	// Ничего не закрываем, не разрушаем — просто замораживаем (обратимо).
	bool k = false;	star_coroutine::set(k);// запретить вторые потоки в lua скриптах.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
	CTheScripts::ScriptSpace[OnAMissionFlag] = k;// выключить флаг миссии (скрипты выходят из своих циклов).
	scripts_paused.store(true);// wait() внутри скриптов теперь делает lua_yield на ближайшем вызове.
	cpp_trace("ПАУЗА: вход, скрипты замораживаются (yield-пауза без teardown)\n");// маркер для диагностики.
	return 0;
};

int reload() {// перегрузка по нажатию клавиши.
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (KeyPressed(VK_CONTROL) || !star_thread::get()) {
			break;
		}
	};
	if (scripts_disabled.load()) { return 0; }// скрипты уже остановлены — повторно не трогаем.
	// РЕЖИМ 2021 (восстановление эталона коммита 06.01.2021): отпустил Ctrl — ПОЛНАЯ
	// перезагрузка скриптов: «Script reloaded», запрет вторых lua-потоков, снятие флага
	// миссии (всё это внутри teardown_all), разборка состояний по владельцам
	// (owner_selfclose: onScriptTerminate + destroy(L) — эквивалент 2021-.destroy) и
	// ПОВТОРНЫЙ ПРОГОН search (asyc, таймаут 100 мс как в 2021).
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (!KeyPressed(VK_CONTROL)) {// отпускание Ctrl: ПЕРЕЗАГРУЗКА ВСЕХ СКРИПТОВ КАК В 2021.
			CMessages::AddMessageJumpQ(L"Script reloaded", 2000, 1);
			// scripts_disabled/store(true) и star_thread::set(false) из «режима паузы» НЕ
			// ставим — иначе рестарт (start_lualoder) выйдет на первой же проверке и
			// перезагрузка не случится.
			teardown_all(true);// пауза всех + разборка + std::thread(start_lualoder) — рестарт.
			// Функция «просто выключить скрипты» (пауза) осталась как pause_scripts() — её
			// никто не вызывает, но она не удалена.
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

	while (true) { // живёт весь сеанс: теперь поток один и не должен умирать при флипах star_thread.
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
	this_thread::sleep_for(chrono::milliseconds(100));
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

			int args = lua_gettop(L1);
			if (0 == luaL_loadfile(L1, luaname)) {// Текущий lua файл.
				int pst1 = lua_pcall(L1, 0, 0, 0);// запуск файла.
				if (pst1 != LUA_OK) { const char* perr1 = lua_tostring(L1, -1); if (perr1) { writelog(perr1); ma_diag::lua_error_note("скрипт %s: %s", luaname ? luaname : "?", perr1); } }
				lua_pushvalue(L1, 1);//скопировать имена функции, отправить на вершину стека.
				// публикация ИНСАЙД потока: состояние не видно свипу, пока не зарегистрирован владелец-поток
				// (иначе teardown-свип закрыл бы чужое состояние). Владелец сам разберёт СВОЁ при reload.
				std::thread([=]() {
					publish_state(L1);
					pump_owner(L1, nullptr, args, true);
				}).detach();
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
