#define _CRT_SECURE_NO_WARNINGS
#include "funcs.h"
#include <map>
#include <mutex>
#include <set>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
using namespace plugin;
using namespace std;

static map<lua_State*, bool> scriptRunning;// флаг «поток скрипта ещё жив» (по lua_State).
static mutex scriptMutex;
static void script_started(lua_State* L) { lock_guard<mutex> lk(scriptMutex); scriptRunning[L] = true; }
static void script_stopped(lua_State* L) { lock_guard<mutex> lk(scriptMutex); scriptRunning[L] = false; }
static bool script_is_running(lua_State* L) { lock_guard<mutex> lk(scriptMutex); auto it = scriptRunning.find(L); return it != scriptRunning.end() && it->second; }
static std::atomic<bool> reload_busy(false);// защита от одновременных перезагрузок.
std::atomic<bool> scripts_paused(false);// (extern в funcs.h; зарезервировано) кооперативные wait() в C-API.
std::atomic<bool> teardown_active(false);// разборка (extern в funcs.h): блокирующие API рвут свои циклы.
// clrfl: ГЛОБАЛЬНЫЙ ФЛАГ ОСТАНОВКИ ВСЕХ СКРИПТОВ + мьютекс для безопасной работы с Lua VM из разных потоков.
std::atomic<bool> g_shouldStopAllScripts(false);
std::mutex g_luaMutex;
// ФЛАГ ПЕРЕЗАПУСКА СКРИПТОВ (вариант «soft restart» поверх существующей структуры):
// teardown_all(true) (Ctrl) поднимает этот флаг — скриптов больше нет.
// gameProcessEvent на ИГРОВОМ потоке видит флаг и перезапускает скрипты через search_scripts() —
// только так безопасно (запуск скриптов не с игрового потока = data race с игровым состоянием = краш).
// Цикл: «скрипты убились -> флаг поднялся -> игра на следующем кадре запустила их снова».
// ВАЖНО: флаг НЕ сбрасывает star_thread (и teardown тоже его не гасит) — иначе сработал бы и
// нижний путь «number_save_slot==9 && !star_thread::get()», и скрипты стартовали бы ДВАЖДЫ.
// star_thread остаётся true; единственный обработчик рестарта — этот флаг.
std::atomic<bool> g_scripts_teared_down(false);// выставляет teardown, гасит gameProcessEvent.
// Запросы на игровые операции, ВЫПОЛНЯЕМЫЕ ТОЛЬКО ИЗ ИГРОВОГО ПОТОКА (gameProcessEvent).
// reload/teardown/final-потоки НЕ имеют права писать игровую память (ScriptSpace/AddMessageJumpQ) —
// это data race с игровым потоком (краш «мгновенный выход после ПАУЗА»). Они ставят только флаг,
// а реальную запись делает gameProcessEvent на игровом потоке в начале следующего кадра.
static std::atomic<bool> teardown_mission_flag_pending(false);// teardown: обнулить флаг миссии в игровой памяти.
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

int hookFunc(lua_State* L, lua_Debug* ar) {
	// StopHook (MASKCOUNT, count=1000): поднят g_shouldStopAllScripts -> бросаем luaL_error.
	// Ошибка из count-хука БЕЗОПАСНА: любое выполнение скрипта идёт под защищённым вызовом
	// (lua_resume с ВАЛИДНЫМ 4-м арг nres, или lua_pcall). lua_resume ловит ошибку хука и
	// возвращает LUA_ERRRUN (проверено на lua/*, EXIT=0) — корутина НЕ рвётся. lua_yield из хука
	// НЕ используем (в count-хуке не разрешён — ломает корутину). Хук выключить/включить нельзя:
	// он ВСЕГДА активен с count=1000, а сам лишь проверяет атомарный флаг (дёшево).
	if (g_shouldStopAllScripts.load()) {
		return luaL_error(L, "SCRIPT_STOPPED_BY_CPP");
	}
	return 0;
};

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
static void lua_crash_write(const char* tag, const char* line);
// ==== Crash handler (SetUnhandledExceptionFilter) ====
// Пишем диагностику вылета ПРЯМЫМ WinAPI (CreateFileA/WriteFile, без CRT-буферов): на момент
// аварии fstream-буфер может быть не сброшен, а CRT-функции — не поддерживать потокобезопасность.
// Возвращаем EXCEPTION_CONTINUE_SEARCH — игра сама решает (диалог/завершение), НО мы успели
// оставить след в trace.txt. Асинхронно-безопасно: никаких аллокаций/heap-обходов.
static void lua_crash_write(const char* tag, const char* line) {
	HANDLE h = CreateFileA("lualoader\\trace.txt", FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
	char buf[256];
	int n = _snprintf(buf, sizeof(buf), "VECTORED: code=0x%08X addr=0x%p thread=%lu",
		rec ? (unsigned)rec->ExceptionCode : 0u,
		rec ? rec->ExceptionAddress : (void*)0,
		(unsigned long)GetCurrentThreadId());
	if (n < 0) n = 0; if (n > (int)sizeof(buf) - 2) n = (int)sizeof(buf) - 2;
	buf[n] = '\0';
	lua_crash_write("C++| ", buf);
	return EXCEPTION_CONTINUE_SEARCH;// не мешаем игре обработать (только диагностика).
}
// std::terminate — вызов abort/необработанное исключение C++ ПРОХОДИТ мимо SEH-фильтров.
static void lua_terminate_handler() {
	lua_crash_write("C++| ", "STD::TERMINATE вызван (abort/необработанное C++ исключение).");
	abort();
}
// atexit — нормальный выход exit(): позволяет отличить «чистый выход» от краша.
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
			if (err) writelog(err);
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
	cpp_tracef("pump_owner START L=%p (thread=%lu)", (void*)L, (unsigned long)GetCurrentThreadId());
	int nres = 0;// ВАЖНО (Lua >=5.4): lua_resume пишет *nresults БЕЗУСЛОВНО (ldo.c:889) — NULL 4-го арг = краш при ЛЮБОМ возврате resume (yield/ошибка/OK). Всегда валидный указатель.
	if (first_resume) lua_resume(L, NULL, args, &nres);
	while (LUA_OK != lua_status(L) && !lua_state_obsolete(L)) {
		// скрипт умер с ОШИБКОЙ (напр. "SCRIPT_STOPPED_BY_CPP" от StopHook или обычная lua-ошибка):
		// статус не YIELD и не OK — resume мёртвой корутины крутил бы цикл вечно. Выходим сразу.
		if (LUA_YIELD != lua_status(L) && LUA_OK != lua_status(L)) break;
		this_thread::sleep_for(chrono::milliseconds(1)); // задержка.
		if (L1 != NULL && LUA_TFUNCTION == lua_type(L1, -1) && LUA_YIELD == lua_status(L) && star_coroutine::get()) {
			for (int i = 1; i <= args; i++) { lua_pushvalue(L1, i); }// расстановка аргументов для вызова функции.
			lua_resume(L1, L, args, &nres);
		}
		if (L1 != NULL && LUA_YIELD == lua_status(L1)) {// если второй на паузе.
			lua_resume(L, L1, 0, &nres);// возобновить основной поток.
		}
		if (L1 != NULL && LUA_OK == lua_status(L1)) {// если второй поток завершен.
			lua_resume(L, NULL, 0, &nres);// основной поток продолжает без второй корутины.
		}
		// ---- КОНЕЦ АКТИВНОЙ ЗОНЫ ----
		if (LUA_YIELD == lua_status(L) || (L1 != NULL && LUA_YIELD == lua_status(L1) && !star_coroutine::get())) {
			goto pump_end;// скрипт на паузе/завершился: выходим из цикла (теardown-разборка, не пауза-in-place).
		}
	}
pump_end:
	script_stopped(L);
	cpp_tracef("pump_owner END L=%p (thread=%lu)", (void*)L, (unsigned long)GetCurrentThreadId());
	if (lua_state_obsolete(L)) { owner_selfclose(L); }
	else { drop_owner(L); }
}
// Единая разборка — Ctrl = ПОЛНЫЙ ПЕРЕЗАПУСК: остановить все Lua-потоки в безопасной точке,
// дождаться, что НИ ОДНОГО живого pump-потока не осталось (active_pumps==0), уничтожить
// состояния. Сами скрипты teardown НЕ запускает (restart=true только поднимает флаг
// g_scripts_teared_down — их перезапустит gameProcessEvent на игровом потоке; см. выше).
// Никаких фикс. задержек как «гарантии» завершения.
// Шаги: teardown_active рвёт блокирующие C-API, эпоха+1 заставляет wait() отдать ход,
// pump-потоки сами закрывают СВОИ состояния (owner_selfclose), затем здесь закрываем сирот.
static void teardown_all(bool restart) {
	if (reload_busy.exchange(true)) { cpp_trace("teardown: другая разборка уже идёт — пропуск"); return; }
	cpp_tracef("teardown: вход (restart=%d), pump-потоков: %d, состояний: %u", (int)restart, active_pumps.load(), (unsigned)luastate.size());
	// ---- 1. ОСТАНОВКА ВСЕХ СКРИПТОВ (без форс): блокирующие API рвут циклы, эпоха+1 = все устарели ----
	teardown_active.store(true);
	star_coroutine::set(false);// запретить новые вторые потоки в lua скриптах.
	teardown_mission_flag_pending.store(true);// игровой поток обнулит флаг миссии (запись только там).
	stop_mission_watch();
	g_lua_epoch.fetch_add(1);// ВСЕ существующие состояния устарели — мгновенно; wait() отдаст ход.
	// ---- 2. ЖДЁМ ПОЛНОГО ЗАВЕРШЕНИЯ: все владельцы закрывают СВОИ состояния сами ----
	// pump_owner при устаревшей эпохе выходит из цикла и зовёт owner_selfclose() -> destroy() ->
	// active_pumps--. Ждём, пока счётчик НЕ станет нулём — ГАРАНТИЯ, что старые потоки мертвы,
	// прежде чем создавать новые (иначе параллельное существование старых/новых = порча памяти).
	cpp_tracef("teardown: ожидание завершения всех pump-потоков (pumps=%d)...", (int)active_pumps.load());
	unsigned waited = 0;
	while (active_pumps.load() > 0 && waited < 5000) {// страховка от вечного ожидания (блокирующий C-API).
		this_thread::sleep_for(chrono::milliseconds(10));
		waited += 10;
	}
	if (active_pumps.load() > 0) { cpp_tracef("teardown: TIMEOUT — осталось живо pump-потоков: %d (форс закрываю сирот)", (int)active_pumps.load()); }
	else { cpp_tracef("teardown: все pump-потоки завершились (waited=%ums)", waited); }
	// ---- 3. Свип сирот: состояния без живого владельца закрываем сейчас ----
	std::vector<lua_State*> snapshot;
	{
		std::lock_guard<std::mutex> lk(luastate_mx);
		for (auto L : luastate) snapshot.push_back(L);
	}
	for (auto L : snapshot) {
		bool owned = false;
		{ std::lock_guard<std::mutex> pk(pumpLive_mx); owned = pumpLive.count(L) != 0; }
		if (owned) { continue; }// живой владелец ещё не закрыл — не трогаем (докончит сам).
		// Сирота: владелец-поток уже завершён, состояние до сих пор висит. Закрываем сейчас (безопасно).
		cpp_tracef("teardown: сирота %p — закрываю", (void*)L);
		lua_getglobal(L, "onScriptTerminate");
		if (lua_type(L, -1) == LUA_TFUNCTION) {
			if (lua_pcall(L, 0, 0, 0) != LUA_OK) { const char* err = lua_tostring(L, -1); if (err) writelog(err); lua_pop(L, 1); }
		} else { lua_pop(L, 1); }
		destroy(L);
		std::lock_guard<std::mutex> lk(luastate_mx); luastate.remove(L);
		lua_state_unstamp(L);
	}
	reload_busy.store(false);// разблокируем для новых перезагрузок.
	teardown_active.store(false);
	if (restart) {// «soft restart»: сама разборка НЕ запускает скрипты (мы в reload-потоке,
		g_scripts_teared_down.store(true);// старт не с игрового потока = data race). Только выставляем
		cpp_trace("teardown: рестарт — выставлен флаг, gameProcessEvent запустит search_scripts()");// флаг — его обработает gameProcessEvent на игровом потоке.
	}
	cpp_trace("teardown: разборка завершена");
}
int startscipt(string res, char* luafile, list<lua_State*>& luastate) {// запуска скрипта.

	state Lua; lua_State* L = Lua.get(); lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
	script_started(L);// пометить состояние: поток скрипта жив.
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

lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 1000);// StopHook ВСЕГДА активен (count=1000): при поднятом флаге бросает luaL_error; без флага — пустой возврат (дёшево). Ошибка перехватывается защищённым lua_resume (валидный nres) / lua_pcall.
			//Command<COMMAND_SCRIPT_NAME>(x);
				//Command<COMMAND_TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME>(luafile);
			lua_pcall(L, 0, 0, 0);// запуск файла.
			lua_getglobal(L, "main");	//writelog3("star scpipt");
			if (LUA_TFUNCTION == lua_type(L, -1)) {
				publish_state(L);// владелец+список регистрируются ДО видимости состояния (teardown-по-владельцу).

			    int nres = 0; lua_resume(L, NULL, 0, &nres);	// запуск файла.
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

	install_crash_filter();// SetUnhandledExceptionFilter: при вылете пишем код/адрес/тип в trace.txt напрямую.

	Events::gameProcessEvent += [] {//обработчик событий игры.
		// ---- ИГРОВЫЕ ОПЕРАЦИИ ПО ЗАПРОСУ ЧУЖИХ ПОТОКОВ ----
		// ЕДИНСТВЕННОЕ место, где плагин пишет ИГРОВУЮ память (ScriptSpace/AddMessageJumpQ) —
		// игровой поток. reload/teardown/final-потоки ставят только флаги-запросы выше;
		// иначе data race с игровым потоком = мгновенный вылет (наблюдался после «ПАУЗА: вход»).
		// Валидация: пишем только если индекс флага корректен (< размера ScriptSpace, массив 260512 байт).
		if (teardown_mission_flag_pending.exchange(false)) {// teardown просил обнулить флаг миссии.
			unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;// получить флаг миссии.
			if (OnAMissionFlag < 260512) { CTheScripts::ScriptSpace[OnAMissionFlag] = false; }
			cpp_tracef("teardown: флаг миссии обнулён в игровом потоке (thread=%lu)", (unsigned long)GetCurrentThreadId());
		}
		// ---- SOFT RESTART: teardown (Ctrl) выставил флаг, скриптов больше нет ----
		// Перезапуск делает ТОЛЬКО игровой поток (это единственное место, которому можно
		// дёргать Command<>/ScriptSpace/поток скриптов): reload-поток не имеет права
		// трогать игровое состояние. Флаг — это «мягкий» запрос: «скрипты убиты, запусти снова».
		// ВАЖНО: зовём search_scripts(), а НЕ start_lualoder()! start_lualoder после запуска
		// скриптов уходит в while(true) ожидания страницы меню 8/10 (загрузка/новая игра) —
		// в игре страница 32, цикл НИКОГДА не завершается, и каждый Ctrl оставляет ВИСЯЧИЙ
		// поток. При последующей загрузке сейвы все они разом звали final_scripts() → каскад
		// teardown'ов → вылет. search_scripts() просто перезапускает скрипты и возвращается.
		if (g_scripts_teared_down.exchange(false)) {
			cpp_trace("gameProcessEvent: флаг перезапуска — старт search_scripts() с игрового потока");
			star_thread::set(s);// скрипты снова разрешены (star_thread был погашен в teardown).
			// сообщение игроку: скрипты перезагружены (кириллица шрифтом VC не отображается — транслит).
			// Используем ровно тот же механизм, что и printmessage() в funcs.cpp (AddMessageJumpQ) —
			// AddBigMessage (центр экрана) не показывал текст; здесь lua_State нет, зовём C-API напрямую.
			wchar_t* msg = getwchat("Scripts reloaded!");
			CMessages::AddMessageJumpQ(msg, 3000, 1);// 1 — стиль как в printmessage (проверено в скриптах).
			std::thread(search_scripts).detach();
		}
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
	teardown_all(false);// единая разборка по владельцам (без рестарта: загрузка сейва / выход в меню).
	return 0;
};

// clrfl: ПОЛНАЯ ОСТАНОВКА ВСЕХ ЗАПУЩЕННЫХ Lua-СКРИПТОВ (Clear/Flush). По шагам:
//  1) g_shouldStopAllScripts=true — StopHook (count=1000) бросает luaL_error("SCRIPT_STOPPED_BY_CPP")
//     ЛЮБОМУ выполняющемуся скрипту в пределах 1000 инструкций (даже в busy-петле без wait());
//     долгие C-циклы (wait/getcord/play_voice/load_model_before_avalible) тоже проверяют флаг
//     и выходят. Ошибка перехватывается защищённым lua_resume (валидный nres) — корутина НЕ рвётся.
//  2) teardown_all(false): g_lua_epoch+1 -> все состояния устарели, pump-владельцы закрывают СВОИ
//     (owner_selfclose), свип добивает сирот, инфраструктура ЖДЁТ active_pumps==0 (это и есть join).
//  3) сбрасываем флаг — новые скрипты (новая игра/загрузка) стартуют чисто.
//  Никаких sleep_for и взаимоисключений в пути остановки: остановку делает сам флаг + хук
//  (мгновенно), а teardown лишь ждёт фактического завершения потоков. g_luaMutex тут НЕ нужен.
static void HandleClearFlush() {
	cpp_tracef("clrfl: ПОЛНАЯ ОСТАНОВКА ВСЕХ СКРИПТОВ (pumps до=%d)", (int)active_pumps.load());
	g_shouldStopAllScripts.store(true);   // хук+флаг: рвать каждый скрипт, включая busy-петли.
	teardown_all(false);                  // join: ждём active_pumps==0, свипуем сирот.
	g_shouldStopAllScripts.store(false);  // новые скрипты стартуют чисто.
	cpp_tracef("clrfl: остановка завершена (pumps после=%d)", (int)active_pumps.load());
};

// Запуск clrfl на ОТДЕЛЬНОМ потоке: не блокировать поток чтения клавиш на время teardown (до 5с).
static void TriggerClearFlush() {
	std::thread([]() { HandleClearFlush(); }).detach();
};

int reload() {// Ctrl: ПОЛНЫЙ ПЕРЕЗАПУСК всех Lua-скриптов (стоп -> дождаться -> уничтожить -> флаг).
	// Клавиша Ctrl = команда «перезапустить скрипты». Она НЕ трогает Lua напрямую: только вызывает
	// teardown_all(true), которая: 1) останавливает все pump-потоки в безопасной точке (wait()->yield,
	// по устаревшей эпохе), 2) ЖДЁТ active_pumps==0 — гарантия, что старых Stream-потоков нет,
	// 3) уничтожает состояния, 4) поднимает флаг g_scripts_teared_down — скрипты перезапустит
	// gameProcessEvent на игровом потоке (не из reload-потока — иначе data race).
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		bool ctrl = KeyPressed(VK_CONTROL);
		if (ctrl) {
			// перезапускаем в отдельном потоке, чтобы не блокировать фильтр клавиш на время разборки.
			std::thread([]() { teardown_all(true); }).detach();
			// ждём отпускания Ctrl, чтобы одно нажатие = один перезапуск.
			while (true) {
				this_thread::sleep_for(chrono::milliseconds(1));
				if (!KeyPressed(VK_CONTROL)) break;
			}
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
			// clrfl — чит-команда ПОЛНОЙ ОСТАНОВКИ всех Lua-скриптов (Safe Points + мьютекс).
			size_t f = cheatstr.find("clrfl");
			if (f != string::npos) {
				cheatstr.clear();// сброс ввода, чтобы не сработало повторно от "clrfl..." в буфере.
				TriggerClearFlush();
			}
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
			lua_sethook(L1, (lua_Hook)hookFunc, LUA_MASKCOUNT, 1000);// StopHook активен и на новом состоянии (clrfl должен рвать этот скрипт тоже).

			int stacksize = lua_gettop(L);	stacksize++;
			for (int i = 1; i < stacksize; i++) {lua_pushvalue(L, i);// копировать на вершину стека.
				lua_xmove(L, L1, 1);// Снимает с L1 элементов передает L.
			};

			int args = lua_gettop(L1);
			if (0 == luaL_loadfile(L1, luaname)) {// Текущий lua файл.
				lua_pcall(L1, 0, 0, 0);// запуск файла.
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
