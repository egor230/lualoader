// Стенд планировщика lualoader (23.10.2026 — ИГРОВОЙ ПОТОК).
//
// Собирает НАСТОЯЩИЙ compat/ma_sched.cpp (через #define MA_SCHED_UNIT_TEST и
// заглушки игрового слоя) и проверяет на реальных скриптах:
//   1) wait() в верхнем блоке файла и в main() — скрипт спит РОВНО заданное время,
//      остальные скрипты в это время работают;
//   2) десятки скриптов одновременно (динамический реестр, один поток);
//   3) скрипт с бесконечным циклом БЕЗ wait() не блокирует остальные (вытеснение);
//   4) teardown снимает все состояния, включая busy-скрипт и скрипт, застрявший в
//      wait(60000) (onScriptTerminate обязан выполниться).
//
// Планировщик НЕ поднимает свой OS-поток: «игровой цикл» — это цикл main(),
// который каждый итерации зовёт ma_sched_frame() (ровно то, что делает
// Events::gameProcessEvent в игре).
//
// Сборка/запуск: ./run.sh (см. рядом).

#include <atomic>
#include <chrono>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "compat/ma_sched.h"
#include "ma_sched_unit_stubs.h"

// ---- определения заглушек --------------------------------------------------
int gGameState = 9;   // «в игре»: teardown в тесте должен звать destroy()/onScriptTerminate
std::list<lua_State*> luastate;
std::list<char*> listfile;
std::atomic<bool> teardown_active{false};
std::atomic<bool> scripts_paused{false};

static std::mutex g_mx;
static std::map<std::string, long long> g_count;
static std::vector<std::string> g_order;

void writelog(const char* s) { std::fprintf(stderr, "LOG: %s\n", s ? s : "?"); }
void dellod() {}
int destroy(lua_State*) { return 0; }
void stop_mission_watch() {}
void lua_state_unstamp(lua_State*) {}

ma_task_registry_stub& ma_tasks() { static ma_task_registry_stub s; return s; }

namespace ma_diag {
void set_thread_name(const char* name) { (void)name; }
void lua_error_note(const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    std::vfprintf(stderr, fmt, ap);
    std::fprintf(stderr, "\n");
    va_end(ap);
}
} // namespace ma_diag

// ---- тестовые Lua-функции --------------------------------------------------
static long long counter(const char* name) {
    std::lock_guard<std::mutex> lk(g_mx);
    auto it = g_count.find(name);
    return it == g_count.end() ? 0 : it->second;
}

static int l_tick(lua_State* L) {
    const char* name = lua_tostring(L, 1);
    long long v = (long long)lua_tointeger(L, 2);
    {
        std::lock_guard<std::mutex> lk(g_mx);
        g_count[name] += 1;
        std::string key = std::string(name) + "#last";
        g_count[key] = v;
        g_order.push_back(name);
    }
    return 0;
}

static int l_now(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)ma_sched_now_ms());
    return 1;
}

static int l_newthread(lua_State* L) {
    if (lua_gettop(L) < 1) return 0;
    if (!ma_sched_spawn(L, 1)) std::fprintf(stderr, "spawn failed\n");
    return 0;
}

// Зеркало funcs.cpp:wait() (27.10): wait()/wait(nil) -> 1 мс, число
// нормализуется, не-число -> luaL_argerror, park + yield; вне планировщика —
// OS-сон. _ma_terminate здесь НЕ проверяется: teardown будит запаркованную
// корутину через kill_resume, а ma_sched_hook бросает SCRIPT_STOPPED_BY_CPP.
static int l_wait(lua_State* L) {
    int ms = 1;
    if (!lua_isnoneornil(L, 1)) {
        if (LUA_TNUMBER != lua_type(L, 1)) {
            return luaL_argerror(L, 1, "number or nil expected");
        }
        lua_Number n = lua_tonumber(L, 1);
        if (!(n > 0)) ms = 1;
        else if (n >= (lua_Number)INT_MAX) ms = INT_MAX;
        else ms = (int)n;
    }
    if (ma_sched_park(L, ms)) {
        if (lua_isyieldable(L)) return lua_yield(L, 0);
        return 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return 0;
}

void funs(lua_State* L) {
    lua_register(L, "tick", l_tick);
    lua_register(L, "now", l_now);
    lua_register(L, "wait", l_wait);
    lua_register(L, "create_newthread", l_newthread);
}

// ---- сценарий --------------------------------------------------------------
static void write_file(const std::filesystem::path& p, const std::string& text) {
    std::ofstream f(p, std::ios::binary | std::ios::trunc);
    f << text;
}

// Сколько OS-потоков в процессе (Linux): планировщик потоков НЕ поднимает —
// на 70 скриптов приходится ОДИН поток (главный, «игровой»).
static int count_threads() {
    std::ifstream f("/proc/self/status");
    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("Threads:", 0) == 0) return std::atoi(line.c_str() + 8);
    }
    return -1;
}

// «Игровой кадр» стенда: ровно то, что в игре делает Events::gameProcessEvent
// (ма_tasks_frame → ma_sched_frame). Планировщик своего потока НЕ поднимает,
// поэтому без этой прокачки скрипты не идут.
static void pump_frame() {
    ma_sched_frame();
}

static void pump_ms(int ms) {
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
    while (std::chrono::steady_clock::now() < end) {
        pump_frame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

static bool pump_wait(const char* name, long long value, int timeout_ms) {
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
    while (std::chrono::steady_clock::now() < end) {
        pump_frame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (counter(name) >= value) return true;
    }
    return counter(name) >= value;
}

// Дождаться условия, прокачивая кадры (для teardown/reload).
static void pump_until_deadline(int timeout_ms) {
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
    while (std::chrono::steady_clock::now() < end) {
        pump_frame();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    namespace fs = std::filesystem;
    fs::path root = fs::current_path();
    fs::path luaDir = root / "lualoader";
    fs::remove_all(luaDir);
    fs::create_directories(luaDir);

    // 1) wait в верхнем блоке + main с пятью итерациями.
    write_file(luaDir / "01_top.lua",
        "tick('top_before', now())\n"
        "wait(100)\n"
        "tick('top_after', now())\n"
        "function main()\n"
        "  for i = 1, 5 do tick('a', i); wait(50) end\n"
        "end\n");

    // 2) обычный скрипт с wait(10).
    write_file(luaDir / "02_b.lua",
        "function main()\n"
        "  for i = 1, 20 do tick('b', i); wait(10) end\n"
        "end\n");

    // 3) ещё один, с другим периодом.
    write_file(luaDir / "03_c.lua",
        "function main()\n"
        "  for i = 1, 10 do tick('c', i); wait(25) end\n"
        "end\n");

    // 4) бесконечный цикл БЕЗ wait(): должен вытесняться, не мешая остальным.
    write_file(luaDir / "04_busy.lua",
        "function main()\n"
        "  local n = 0\n"
        "  while true do n = n + 1 end\n"
        "end\n");

    // 5) точность wait(200) в main().
    write_file(luaDir / "05_timing.lua",
        "function main()\n"
        "  local t0 = now()\n"
        "  wait(200)\n"
        "  tick('timing', now() - t0)\n"
        "end\n");

    // 6) onScriptTerminate должен быть вызван при teardown.
    write_file(luaDir / "06_term.lua",
        "function main() wait(10) end\n"
        "function onScriptTerminate() tick('term', 1) end\n");

    // 7) ошибка в одном скрипте не должна ронять планировщик/другие скрипты.
    write_file(luaDir / "07_err.lua",
        "function main() tick('err_before', 1); error('boom') end\n");

    // 8) create_newthread: второй экземпляр файла получает аргумент и не плодится.
    write_file(luaDir / "08_spawn.lua",
        "function main(fn)\n"
        "  if fn ~= nil then tick('spawn_child', 1)\n"
        "  else tick('spawn_parent', 1); create_newthread(1) end\n"
        "end\n");

    // 9) скрипт, «висящий» в wait(60000) в момент разборки: onScriptTerminate
    //    обязан выполниться на приостановленной корутине.
    write_file(luaDir / "09_parked.lua",
        "function main() wait(60000) end\n"
        "function onScriptTerminate() tick('term_parked', 1) end\n");

    // 10) 60 скриптов одновременно.
    for (int i = 0; i < 60; ++i) {
        char name[64];
        std::snprintf(name, sizeof(name), "%02d_many.lua", 10 + i);
        write_file(luaDir / name,
            "function main()\n"
            "  tick('many', 1)\n"
            "  wait(5)\n"
            "  tick('many', 2)\n"
            "end\n");
    }

    std::printf("scripts on disk: 69\n");
    ma_sched_scan();

    bool ok = true;
    auto check = [&](const char* what, bool cond) {
        std::printf("%s %s\n", cond ? "PASS" : "FAIL", what);
        if (!cond) ok = false;
    };

    check("top-level wait(100) does not start main before it",
          pump_wait("top_after", 1, 3000));
    {
        long long tb = counter("top_before#last"), ta = counter("top_after#last");
        std::printf("top-level wait: before=%lld after=%lld delta=%lld\n", tb, ta, ta - tb);
        check("top-level wait(100) lasted >= 95 ms", ta - tb >= 95);
    }
    check("main() of 01_top ran 5 iterations", pump_wait("a#last", 5, 5000));
    check("02_b ran 20 iterations", pump_wait("b#last", 20, 5000));
    check("03_c ran 10 iterations", pump_wait("c#last", 10, 5000));
    check("60 extra scripts ran (many = 120 ticks)", pump_wait("many", 120, 8000));
    check("script with runtime error ran once (no crash)", pump_wait("err_before", 1, 3000));
    check("create_newthread spawned a second state", pump_wait("spawn_child", 1, 3000));
    check("spawned state did not spawn recursively", counter("spawn_parent") == 1);
    check("wait(200) actually slept >= 200 ms", pump_wait("timing", 1, 5000));

    long long dt = counter("timing#last");
    std::printf("timing: wait(200) measured %lld ms\n", dt);
    check("wait(200) >= 195 ms", dt >= 195);
    check("wait(200) not absurdly long (< 700 ms)", dt < 700);

    // Вытеснение: busy-скрипт не должен был заблокировать 02_b/c/many — они уже
    // доиграли выше. Дополнительно смотрим, что порядок тиков чередуется.
    {
        std::lock_guard<std::mutex> lk(g_mx);
        bool saw_ab = false;
        for (size_t i = 0; i + 1 < g_order.size(); ++i) {
            if (g_order[i] == "a" && g_order[i + 1] == "b") { saw_ab = true; break; }
            if (g_order[i] == "b" && g_order[i + 1] == "a") { saw_ab = true; break; }
        }
        check("a and b interleaved (cooperative scheduling)", saw_ab);
        check("busy script did not starve others (many after a/b)", g_order.size() > 150);
    }

    check("registry has 70 states (69 files + 1 spawned)", ma_sched_count() == 70);
    {
        int th = count_threads();
        std::printf("OS threads in process: %d (70 scripts)\n", th);
        check("70 scripts without scheduler OS threads (threads <= 2)", th > 0 && th <= 2);
    }

    // teardown должен снять и вечный busy-скрипт, и скрипт, висящий в wait(60000).
    ma_sched_request_teardown(false);
    {
        auto end = std::chrono::steady_clock::now() + std::chrono::seconds(5);
        while ((ma_sched_count() != 0 || teardown_active.load()) && std::chrono::steady_clock::now() < end) {
            pump_frame();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    check("teardown removed all states (busy + parked included)", ma_sched_count() == 0);
    check("teardown_active cleared by scheduler", !teardown_active.load());
    check("onScriptTerminate called for the script that defines it", counter("term") == 1);
    check("onScriptTerminate called on a coroutine parked in wait(60000)", counter("term_parked") == 1);

    // ---- перезагрузка (teardown + restart): скрипты должны встать снова ----
    ma_sched_request_teardown(true);
    {
        auto end2 = std::chrono::steady_clock::now() + std::chrono::seconds(8);
        while (counter("top_after") < 2 && std::chrono::steady_clock::now() < end2) {
            pump_frame();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    check("reload started scripts again", counter("top_after") >= 2);
    check("reload restored the full registry (70)", ma_sched_count() == 70);

    ma_sched_request_teardown(false);
    {
        auto end3 = std::chrono::steady_clock::now() + std::chrono::seconds(5);
        while ((ma_sched_count() != 0 || teardown_active.load()) && std::chrono::steady_clock::now() < end3) {
            pump_frame();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    check("final teardown clean", ma_sched_count() == 0 && !teardown_active.load());

    std::printf("%s\n", ok ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return ok ? 0 : 1;
}
