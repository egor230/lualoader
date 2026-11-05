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
using namespace plugin;
using namespace std;

// ============================================================================
// ПЛАНИРОВЩИК (2021, директива юзера): ОДИН OS-ПОТОК НА КАЖДЫЙ .lua.
//
// Каждый скрипт живёт в своём lua_State и своём std::thread:
//   * wait(ms) спит ЭТИМ потоком (как в последнем коммите 2021);
//   * create_newthread() поднимает ещё один поток/состояние;
//   * игровой поток не блокируется, остальные скрипты продолжают работать.
//
// Разборка (Ctrl-reload / выход из игры) — фоновый поток: ставит
// teardown_active, ждёт завершения скриптовых потоков (wait() и count-хук
// бросают SCRIPT_STOPPED_BY_CPP), затем destroy(L) + cleanstl() через очередь
// команд (Command<> исполняется на игровом потоке). lua_close не зовём:
// состояние могло остаться у зависшего потока — утечка лучше UAF (как в 2021).
// ============================================================================

static std::atomic<bool> reload_busy(false);
static std::atomic<bool> scripts_disabled(false);
std::atomic<bool> scripts_paused(false);
std::atomic<bool> teardown_active(false);
static std::atomic<bool> scripts_started(false);
static std::atomic<bool> rescan_requested(false);
static std::atomic<bool> start_pending(false);

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

// ==== Реестр скриптовых потоков =============================================
// Аргументы create_newthread переносятся между независимыми lua_State как
// простые значения (lua_xmove между разными global state — UB).
struct ScriptArg {
    int type = LUA_TNIL;
    double num = 0;
    bool b = false;
    std::string str;
    const void* ptr = nullptr;
};

struct ScriptRec {
    lua_State* L = nullptr;   // главное состояние (GC-якорь; не закрываем)
    lua_State* T = nullptr;   // корутина, на которой исполняется скрипт
    std::atomic<bool> done{ false };
    std::string file;
};

struct ScriptRegistry {
    std::mutex mx;
    std::vector<std::shared_ptr<ScriptRec>> active;
    std::vector<std::shared_ptr<ScriptRec>> zombies;
};

static ScriptRegistry& script_registry() {
    static ScriptRegistry r;
    return r;
}

// count-хук: разборка должна уметь прервать даже busy-цикл без wait().
static void script_stop_hook(lua_State* L, lua_Debug* ar) {
    (void)ar;
    if (teardown_active.load()) {
        luaL_error(L, "SCRIPT_STOPPED_BY_CPP");
    }
}

static void push_script_arg(lua_State* L, const ScriptArg& a) {
    switch (a.type) {
        case LUA_TBOOLEAN: lua_pushboolean(L, a.b ? 1 : 0); break;
        case LUA_TNUMBER:  lua_pushnumber(L, (lua_Number)a.num); break;
        case LUA_TSTRING:  lua_pushlstring(L, a.str.data(), a.str.size()); break;
        case LUA_TLIGHTUSERDATA: lua_pushlightuserdata(L, const_cast<void*>(a.ptr)); break;
        default: lua_pushnil(L); break;
    }
}

static void log_lua_error(lua_State* L, const char* where) {
    const char* e = lua_tostring(L, -1);
    if (!e) e = "(ошибка без текста)";
    writelog(e);
    ma_diag::lua_error_note("%s: %s", where ? where : "?", e);
    lua_pop(L, 1);
}

static void script_thread_main(std::shared_ptr<ScriptRec> rec, std::string dumped, std::vector<ScriptArg> args) {
    lua_State* L = luaL_newstate();
    if (!L) { rec->done.store(true); return; }
    luaL_openlibs(L);
    lua_State* T = lua_newthread(L);   // остаётся на стеке L — GC-якорь
    rec->L = L;
    rec->T = T;
    lua_state_stamp(T);

    // Имя файла для wait_script_name()/create_newthread(): ключ — сама корутина.
    lua_pushlightuserdata(T, T);
    lua_pushstring(T, rec->file.c_str());
    lua_settable(T, LUA_REGISTRYINDEX);

    std::error_code cec;
    std::string pp = (std::filesystem::current_path(cec) / "?.lua").string();
    if (cec) pp = "?.lua";
    lua_pushstring(T, pp.c_str());
    lua_setglobal(T, "fullpath");
    luaL_dostring(T, "package.path = fullpath");

    funs(T);   // ~1583 Lua-API функции
    lua_sethook(T, script_stop_hook, LUA_MASKCOUNT, 2000);
    ma_diag::set_thread_name(rec->file.c_str());

    int st = luaL_loadfile(T, rec->file.c_str());
    if (st != LUA_OK) {
        log_lua_error(T, "luaL_loadfile");
        rec->done.store(true);
        return;
    }
    int pst = lua_pcall(T, 0, 0, 0);
    if (pst != LUA_OK) {
        log_lua_error(T, "верхний блок скрипта");
        rec->done.store(true);
        return;
    }
    if (teardown_active.load()) { rec->done.store(true); return; }

    int nargs = 0;
    if (dumped.empty()) {
        // Обычный скан: запускаем глобальную main().
        lua_getglobal(T, "main");
        if (lua_type(T, -1) != LUA_TFUNCTION) {
            lua_pop(T, 1);
            rec->done.store(true);
            return;
        }
    }
    else {
        // create_newthread: файл уже исполнен (глобалы на месте), грузим байткод
        // переданной функции и её аргументы.
        if (luaL_loadbuffer(T, dumped.data(), dumped.size(), "=create_newthread") != LUA_OK) {
            log_lua_error(T, "create_newthread: loadbuffer");
            rec->done.store(true);
            return;
        }
        for (const ScriptArg& a : args) push_script_arg(T, a);
        nargs = (int)args.size();
    }

    int nres = 0;
    int status = lua_resume(T, nullptr, nargs, &nres);
    while (status == LUA_YIELD) {
        if (teardown_active.load()) break;
        status = lua_resume(T, nullptr, 0, &nres);
    }
    if (status != LUA_OK && status != LUA_YIELD) {
        log_lua_error(T, dumped.empty() ? "main" : "create_newthread");
    }
    rec->done.store(true);
}

static bool spawn_script(const std::string& file, std::string dumped, std::vector<ScriptArg> args) {
    if (scripts_disabled.load() || teardown_active.load()) return false;
    std::shared_ptr<ScriptRec> rec = std::make_shared<ScriptRec>();
    rec->file = file;
    {
        ScriptRegistry& reg = script_registry();
        std::lock_guard<std::mutex> lk(reg.mx);
        reg.active.push_back(rec);
    }
    try {
        std::thread([rec, dumped = std::move(dumped), args = std::move(args)]() mutable {
            script_thread_main(rec, std::move(dumped), std::move(args));
        }).detach();
    }
    catch (...) {
        rec->done.store(true);
        writelog("не удалось создать поток скрипта");
        return false;
    }
    return true;
}

static void scan_and_start_scripts() {
    if (scripts_disabled.load() || teardown_active.load()) return;
    dellod();   // новый журнал на новую загрузку (как старый search)

    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path root = fs::current_path(ec) / "lualoader";
    if (ec) return;
    fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end;
    for (; !ec && it != end; it.increment(ec)) {
        std::error_code fec;
        if (!it->is_regular_file(fec)) continue;
        std::string ext = it->path().extension().string();
        if (ext != ".lua" && ext != ".LUA") continue;
        std::string res = it->path().string();
        char* luafile = strdup(res.c_str());
        if (luafile) listfile.push_back(luafile);
        spawn_script(res, std::string(), std::vector<ScriptArg>());
    }
}

// ==== Разборка (фон, чтобы не блокировать игровой поток) ====================
static void teardown_worker(std::vector<std::shared_ptr<ScriptRec>> snap, bool restart) {
    const unsigned long long deadline = GetTickCount64() + 15000;
    for (auto& r : snap) {
        while (!r->done.load() && GetTickCount64() < deadline) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    bool all_done = true;
    for (auto& r : snap) {
        if (!r->done.load()) { all_done = false; continue; }
        if (r->T) {
            try { destroy(r->T); } catch (...) {}
            lua_state_unstamp(r->T);
        }
    }
    cleanstl();

    if (!all_done) {
        ScriptRegistry& reg = script_registry();
        std::lock_guard<std::mutex> lk(reg.mx);
        for (auto& r : snap) {
            if (!r->done.load()) reg.zombies.push_back(r);
        }
        reload_busy.store(false);
        writelog("teardown: часть скриптов не завершилась за 15 с — повтор при следующей разборке");
        return;   // teardown_active остаётся true: зависшие увидят флаг на wait()/хуке
    }

    teardown_active.store(false);
    reload_busy.store(false);
    if (restart || start_pending.exchange(false)) rescan_requested.store(true);
}

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

    std::vector<std::shared_ptr<ScriptRec>> snap;
    {
        ScriptRegistry& reg = script_registry();
        std::lock_guard<std::mutex> lk(reg.mx);
        snap.swap(reg.active);
        for (auto& z : reg.zombies) snap.push_back(z);
        reg.zombies.clear();
    }

    if (snap.empty()) {
        cleanstl();
        teardown_active.store(false);
        reload_busy.store(false);
        if (restart || start_pending.exchange(false)) rescan_requested.store(true);
        return;
    }

    std::thread([snap = std::move(snap), restart]() mutable {
        teardown_worker(std::move(snap), restart);
    }).detach();
}

// ==== Per-frame: старт при входе в игру, разборка при выходе ====
static void lifecycle_tick() {
    const bool in_game = (gGameState == 9);

    // Отложенный скан после фоновой разборки (Ctrl-reload).
    if (rescan_requested.exchange(false)) {
        if (in_game && !scripts_disabled.load()) {
            scan_and_start_scripts();
            scripts_started.store(true);
        }
    }

    // Вход в игру: один раз на сессию — найти и запустить все .lua.
    if (in_game && !scripts_started.load()) {
        if (scripts_disabled.load()) return;
        if (teardown_active.load() || reload_busy.load()) { start_pending.store(true); return; }
        int gtg = CTimer::m_snTimeInMilliseconds;
        bool can_start = (gtg < 1000)
            ? (Command<COMMAND_CAN_PLAYER_START_MISSION>(CWorld::PlayerInFocus) != 0)
            : true;
        if (can_start) {
            star_thread::set(true);
            scan_and_start_scripts();
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
    scan_and_start_scripts();
    scripts_started.store(true);
}

int start_lualoder() {
    if (scripts_disabled.load()) { star_thread::set(false); return 0; }
    star_thread::set(true);
    if (!scripts_started.load()) {
        scan_and_start_scripts();
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

struct DumpBuf { std::string s; };

static int dump_writer(lua_State* L, const void* p, size_t sz, void* ud) {
    (void)L;
    static_cast<DumpBuf*>(ud)->s.append(static_cast<const char*>(p), sz);
    return 0;
}

int create_newthread(lua_State* L) {// создания нового потока (2021: свой OS-поток).
    try {
        if (LUA_TFUNCTION != lua_type(L, 1)) { throw "bad argument in function create_newthread"; }

        // Имя файла — из реестра (ключ = сам L), как в 2021.
        lua_pushlightuserdata(L, L);
        lua_gettable(L, LUA_REGISTRYINDEX);
        const char* luaname = lua_tostring(L, -1);
        std::string file = luaname ? luaname : "";
        lua_pop(L, 1);
        if (file.empty()) {
            writelog("create_newthread: не найдено имя lua-файла");
            return 0;
        }

        // Переносим БАЙТКОД функции, а не Lua-значения между состояниями
        // (lua_xmove между разными global state — UB, как было в 2021).
        lua_pushvalue(L, 1);
        DumpBuf db;
        int dr = lua_dump(L, dump_writer, &db, 0);
        lua_pop(L, 1);
        if (dr != 0 || db.s.empty()) {
            writelog("create_newthread: lua_dump failed");
            return 0;
        }

        // Аргументы: nil/bool/number/string/lightuserdata; остальное — nil.
        std::vector<ScriptArg> args;
        int n = lua_gettop(L);
        for (int i = 2; i <= n; i++) {
            ScriptArg a;
            a.type = lua_type(L, i);
            switch (a.type) {
                case LUA_TBOOLEAN: a.b = lua_toboolean(L, i) != 0; break;
                case LUA_TNUMBER:  a.num = (double)lua_tonumber(L, i); break;
                case LUA_TSTRING: {
                    size_t len = 0;
                    const char* s = lua_tolstring(L, i, &len);
                    if (s) a.str.assign(s, len);
                    break;
                }
                case LUA_TLIGHTUSERDATA: a.ptr = lua_topointer(L, i); break;
                case LUA_TNIL: break;
                default:
                    writelog("create_newthread: аргумент неподдерживаемого типа заменён на nil");
                    a.type = LUA_TNIL;
                    break;
            }
            args.push_back(std::move(a));
        }

        if (!spawn_script(file, std::move(db.s), std::move(args))) {
            writelog("create_newthread: не удалось запустить поток");
        }
        return 0;
    }
    catch (const char* x) { writelog(x); }
    return 0;
};
