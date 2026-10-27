#define _CRT_SECURE_NO_WARNINGS
#include "funcs.h"
#include <map>
#include <mutex>
#include <set>
#include <vector>
#include <thread>
#include <atomic>
#include <exception>
#include "compat/diag.h"
#include "compat/ma_tasks.h"
#include "compat/ma_sched.h"
using namespace plugin;
using namespace std;

// ============================================================================
// ПЛАНИРОВЩИК (25.10, финал-7, директива юзера): ВСЕ скрипты — на ИГРОВОМ потоке.
// До входа в игру: ma_sched_scan() находит и проверяет все .lua (порциями).
// Каждый кадр (Events::gameProcessEvent): удостоверяемся, что игрок в игре
// (gGameState == 9), и крутим ЦИКЛ FOR ПО СКРИПТАМ — ma_sched_frame(): каждое
// состояние получает resume до ближайшего yield; wait(ms)/yield() паркуют
// корутину до следующего кадра. Ни одного OS-потока на скрипты → гонок нет
// по построению. (Поточная машина «Комета»/pump удалена — она и давала
// вылеты при 2-3 скриптах: параллельные потоки лезли в общее состояние.)
// ============================================================================

static std::atomic<bool> reload_busy(false);
static std::atomic<bool> scripts_disabled(false);
std::atomic<bool> scripts_paused(false);
std::atomic<bool> teardown_active(false);
static std::atomic<bool> scripts_started(false);

struct star_thread {
    static bool star_second_thread;
    static void set(bool v) {
        star_second_thread = v;
        ma_star_mirror().store(v);
    }
    static bool get() { return star_second_thread; }
};
bool star_thread::star_second_thread;

struct star_coroutine {
    static bool coroutine;
    static void set(bool v) { coroutine = v; }
    static bool get() { return coroutine; }
};
bool star_coroutine::coroutine;

// Предварительные объявления (шимы для funcs.h).
int start_lualoder();
int final_scripts();
int reload();
void search_scripts();
static void teardown_all(bool restart);
static void lifecycle_tick();

void writelog3(const char x[]) {
    string path = "lualoader\\log.txt";
    fstream f1; { f1.open(path, fstream::in | fstream::out | fstream::app);
        f1 << x; time_t rawtime; struct tm* timeinfo;
        char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);
        string er2(buffer); f1 << er2 << "\n"; }
    f1.close();
};
void writelog3(int x) {
    string path = "lualoader\\log.txt";
    fstream f1; { f1.open(path, fstream::in | fstream::out | fstream::app);
        f1 << x; time_t rawtime; struct tm* timeinfo;
        char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);
        string er2(buffer); f1 << er2 << "\n"; }
    f1.close();
};

// ==== Разборка (реальная работа — в ma_sched, двухфазная) ====
static void teardown_all(bool restart) {
    if (reload_busy.exchange(true)) { return; }
    teardown_active.store(true);
    star_coroutine::set(false);

    // ScriptSpace — игровая память; зовём только с игрового потока.
    auto clear_on_mission = [] {
        unsigned int& f = *(unsigned int*)0x978748;
        CTheScripts::ScriptSpace[f] = false;
    };
    if (ma_cmdq_on_game_thread()) clear_on_mission();
    else ma_cmdq_post(clear_on_mission);

    stop_mission_watch();
    g_lua_epoch.fetch_add(1);
    ma_sched_request_teardown(restart);
    reload_busy.store(false);
}

// ==== Per-frame: старт при входе в игру, разборка при выходе ====
static void lifecycle_tick() {
    const bool in_game = (gGameState == 9);

    // Вход в игру: один раз на сессию — найти и проверить все .lua.
    if (in_game && !scripts_started.load()) {
        if (scripts_disabled.load()) return;
        int gtg = CTimer::m_snTimeInMilliseconds;
        bool can_start = (gtg < 1000)
            ? (Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus) != 0)
            : true;
        if (can_start) {
            star_thread::set(true);
            ma_sched_scan();// скан и проверка всех скриптов (грузит порциями по 2 файла/кадр)
            scripts_started.store(true);
        }
        return;
    }

    // Выход из игры: разобрать Lua.
    if (!in_game && scripts_started.load()) {
        teardown_all(false);
        scripts_started.store(false);
    }
}

// ==== Шимы жизненного цикла (объявлены в funcs.h) ====
void search_scripts() {
    if (scripts_disabled.load()) return;
    if (scripts_started.load()) return;
    ma_sched_scan();
    scripts_started.store(true);
}

int start_lualoder() {
    if (scripts_disabled.load()) { star_thread::set(false); return 0; }
    star_thread::set(true);
    if (!scripts_started.load()) {
        ma_sched_scan();
        scripts_started.store(true);
    }
    return 0;
}

int timerstar() { return 0; }

int final_scripts() {
    teardown_all(false);
    scripts_started.store(false);
    return 0;
}

int reload() {
    teardown_all(true);
    scripts_started.store(true);
    return 0;
}

int pause_scripts() {
    star_coroutine::set(false);
    auto off = [] {
        unsigned int& f = *(unsigned int*)0x978748;
        CTheScripts::ScriptSpace[f] = false;
    };
    if (ma_cmdq_on_game_thread()) off();
    else ma_cmdq_post(off);
    ma_sched_request_pause(true);
    return 0;
};

void getkeyenvent() { ma_task_start_keyboard(); }

// ==== Crash handlers ====
// (27.10.2026) Отметка сборки funcs.cpp — см. первый кадр ниже.
extern "C" const char* ma_funcs_build_stamp(void);

static void lua_crash_write(const char* tag, const char* line) {
    HANDLE h = CreateFileA("lualoader\\log.txt", FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
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
    char buf[512]; int n = 0;
    n += _snprintf(buf + n, sizeof(buf) - (size_t)n,
        "C++| CRASH: code=0x%08X addr=0x%p thread=%lu EXCEPTION.",
        rec ? (unsigned)rec->ExceptionCode : 0u,
        rec ? rec->ExceptionAddress : (void*)0, tid);
    if (rec) {
        switch ((unsigned)rec->ExceptionCode) {
            case EXCEPTION_ACCESS_VIOLATION: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ACCESS_VIOLATION (0xC0000005)"); break;
            case EXCEPTION_ILLEGAL_INSTRUCTION: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ILLEGAL_INSTRUCTION"); break;
            case EXCEPTION_STACK_OVERFLOW: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " STACK_OVERFLOW"); break;
            case EXCEPTION_INT_DIVIDE_BY_ZERO: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " INT_DIVIDE_BY_ZERO"); break;
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " ARRAY_BOUNDS_EXCEEDED"); break;
            default: break;
        }
        if ((unsigned)rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2) {
            n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " %s @0x%p",
                rec->ExceptionInformation[0] == 1 ? "WRITE" :
                rec->ExceptionInformation[0] == 0 ? "READ" : "EXEC",
                (void*)rec->ExceptionInformation[1]);
        }
    }
    { char lc[192]; ma_diag::lua_context(lc, sizeof(lc));
      if (lc[0]) n += _snprintf(buf + n, sizeof(buf) - (size_t)n, " %s", lc); }
    if (n < 0) n = 0; if (n > (int)sizeof(buf) - 2) n = (int)sizeof(buf) - 2;
    buf[n] = '\n'; buf[n + 1] = '\0'; n++;
    lua_crash_write(NULL, buf);
    // (27.10) Снимок стека БЕЗ выделения памяти — ДО report(): отчёт сам
    // выделяет память (Toolhelp/dbghelp) и на уже испорченной куче обрывается,
    // оставляя в журнале только строку выше. Снимок даёт EIP+модуль, доступ,
    // регистры и адреса возврата даже в этом случае.
    ma_diag::dump_stack_noheap(ep, "UNHANDLED");
    // Полный отчёт (крошки Command<>, регистры, стек, модули, авто-заключение):
    // один на процесс (report() дедуплицируется).
    ma_diag::report(ep, "UNHANDLED");
    return EXCEPTION_CONTINUE_SEARCH;
}

static LONG WINAPI lua_vectored_filter(EXCEPTION_POINTERS* ep) {
    EXCEPTION_RECORD* rec = ep ? ep->ExceptionRecord : NULL;
    unsigned long tid = GetCurrentThreadId();
    unsigned code = rec ? (unsigned)rec->ExceptionCode : 0u;
    if (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_ILLEGAL_INSTRUCTION ||
        code == EXCEPTION_STACK_OVERFLOW || code == EXCEPTION_INT_DIVIDE_BY_ZERO ||
        code == EXCEPTION_ARRAY_BOUNDS_EXCEEDED || code == EXCEPTION_DATATYPE_MISALIGNMENT) {
        char lc[192]; ma_diag::lua_context(lc, sizeof(lc));
        char buf[512];
        int n = _snprintf(buf, sizeof(buf), "C++| VECTORED: code=0x%08X addr=0x%p thread=%lu%s%s",
            code, rec ? rec->ExceptionAddress : (void*)0,
            (unsigned long)GetCurrentThreadId(), lc[0] ? " " : "", lc);
        if (n < 0) n = 0; if (n > (int)sizeof(buf) - 2) n = (int)sizeof(buf) - 2;
        buf[n] = '\0';
        lua_crash_write("C++| ", buf);
        // (27.10) Сначала снимок БЕЗ выделения памяти (см. dump_stack_noheap),
        // затем тяжёлый report(): он может не дожить до конца на битой куче.
        ma_diag::dump_stack_noheap(ep, "VECTORED");
        // (25.10) Полный отчёт ПРЯМО СЮДА: в VEH-момент стек ещё здоров, а раньше
        // report() никуда не был подключён — в log.txt оставалась только эта строка
        // (heap AV 0x772Fxxxxx после set_skin так и остался неразобранным: крошки,
        // модуль/смещение, стек и авто-заключение не писались). report() дедуплицирует
        // сам: полный отчёт — один на процесс, повторные VECTORED только дописывают
        // строки выше.
        ma_diag::report(ep, "VECTORED");
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void lua_terminate_handler() {
    lua_crash_write("C++| ", "STD::TERMINATE");
    if (std::current_exception()) {
        try { std::rethrow_exception(std::current_exception()); }
        catch (const std::exception& e) { lua_crash_write("C++| what(): ", e.what()); }
        catch (...) { lua_crash_write("C++| ", "не-std::exception"); }
    }
    abort();
}

static void lua_atexit_handler() { lua_crash_write("C++| ", "ATEXIT"); }

static void install_crash_filter() {
    AddVectoredExceptionHandler(1, lua_vectored_filter);
    SetUnhandledExceptionFilter(lua_crash_filter);
    std::set_terminate(lua_terminate_handler);
    atexit(lua_atexit_handler);
};

// ==== Точка входа ASI ====
class Message {
public: Message() {
    // (25.10, директива юзера) Каждый запуск игры — НОВЫЙ журнал.
    DeleteFileA("lualoader\\log.txt");
    // ВАЖНО (26.10): НИКАКОГО writelog/fstream-IO в этом конструкторе!
    // Глобальный конструктор гонится при LOАД DLL (под loader-lock, CRT ещё не
    // готова). writelog ходит в std::fstream (системная CRT) и ДЕДЛОЧИТСЯ на
    // критсете CRT под Wine (RtlpWaitForCriticalSection ... blocked by 0000),
    // игра не стартует, лог стёрт DeleteFileA не создаётся заново.
    // Build-stamp пишется на ПЕРВОМ кадре (см. обработчик ниже) — в безопасном
    // контексте игрового потока.
    install_crash_filter();
    // Порядок критичен: diag → cmdq → stream
    ma_diag::install();
    ma_cmdq_install_hook();
    ma_stream_install_hook();

    // --- ГЛАВНЫЙ ОБРАБОТЧИК КАДРА ---
    Events::gameProcessEvent += [] {
        // 0.0) (26.10, P1) BUILD-STAMP — первый кадр, игрового потока,
        //      CRT/SDK готовы: видно, какой бинарник реально на диске
        //      (сценарий «исходник и бинарник разошлись»). Одна запись.
        static bool s_stamp_done = false;
        if (!s_stamp_done) {
            s_stamp_done = true;
            writelog("lualoader.asi BUILD-STAMP: plugin " __DATE__ " " __TIME__);
            // (27.10) Отметка funcs.cpp: plugin.cpp пересобирается не всегда, и
            // «BUILD-STAMP: Oct 26 ...» в журнале не доказывал, что новый funcs.cpp
            // в бинарнике (именно так уже терялся один прогон).
            ma_diag::mark("BUILD-STAMP: funcs.cpp %s", ma_funcs_build_stamp());
            // Карта модулей: абсолютный адрес падения (0x772E1C00) переводится в
            // «модуль+смещение» без Toolhelp в момент краха.
            ma_diag::dump_modules_once();
        }
        // 0) Сторожа кадра: пишут в журнал ТОЛЬКО при аномалии.
        ma_diag::msg_keepalive_purge();
        ma_diag::font_style_guard();
        ma_diag::radar_list_guard();
        ma_diag::intro_text_guard();
        // (27.10) Контроль целостности кучи msvcrt (троттлинг 1 с внутри).
        // Если порча уже есть — HEAPCHECK-BAD попадёт в журнал ДО падения, и
        // будет видно, какая контрольная точка её обнаружила.
        ma_diag::heap_check(NULL);

        // 1) Клавиатура (задача на игровом потоке: Ctrl-рестарт, clrfl, cheatstr).
        ma_task_start_keyboard();
        ma_tasks_frame();

        // 2) Флаги клавиатуры.
        if (ma_task_keyboard* kb = ma_keyboard()) {
            if (kb->release_pending) {         // Ctrl отпущен → полная перезагрузка.
                kb->release_pending = false;
                if (!scripts_disabled.load()) {
                    CMessages::AddMessageJumpQ(L"Script reloaded", 2000, 1);
                    teardown_all(true);
                    scripts_started.store(true);
                }
            }
            if (kb->clrfl_req) {               // clrfl → полная остановка.
                kb->clrfl_req = false;
                scripts_disabled.store(true);
                star_thread::set(false);
                teardown_all(false);
                scripts_started.store(false);
            }
        }

        // 3) Игрок в игре? старт при входе / разборка при выходе.
        lifecycle_tick();

        // 4) ЦИКЛ FOR ПО СКРИПТАМ: каждому состоянию — resume до ближайшего
        //    yield (wait()/yield() паркуют корутину до следующего кадра).
        ma_sched_frame();
    };

    Events::gameProcessEvent += spite::draw;
    Events::gameProcessEvent += corona::draw;
    Events::vehicleRenderEvent += DoorsExample::ProcessDoors;

    Events::gameProcessEvent += [] {
        if (iters > 4294967200) { iters = 300; }
        iters++;
    };
}
    ~Message() { }
} message;

int create_newthread(lua_State* L) {// создания нового потока (корутина планировщика).
	std::lock_guard<std::recursive_mutex> lua_fn_guard_(g_lua_api_mx);// общий мьютекс всех API.
	try {
		if (LUA_TFUNCTION == lua_type(L, 1)) {
			if (!ma_sched_spawn(L, 1))
				writelog("create_newthread: планировщик недоступен");
			return 0;
		}
		else { throw "bad argument in create_newthread"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
