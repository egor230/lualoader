// ma_sched.cpp — кооперативный планировщик Lua-скриптов (23.10.2026, ИГРОВОЙ ПОТОК).
//
// ВЕСЬ Lua крутится на игровом потоке (ma_sched_frame из Events::gameProcessEvent):
//   * каждый скрипт — корутина; wait(ms) = ma_sched_park() + lua_yield;
//   * busy-циклы вытесняются count-хуком (слайс 2 мс на скрипт, проход кадра 12 мс);
//   * teardown двухфазный: phase1 помечает _ma_terminate, phase2 каждый кадр
//     «убивает» живые (ошибка SCRIPT_STOPPED_BY_CPP из wait()/хука), затем
//     onScriptTerminate + destroy + снятие из реестров.
//
// Правила файла (как и во всех compat/*): НИ ОДНОГО глобального объекта с
// нетривиальным конструктором и НИ ОДНОГО std::thread / thread_local — иначе
// в Wine игра не стартует (RtlpWaitForCriticalSection "... blocked by 0000"),
// а BUILD-GATE в build.sh ловит регресс (30 _GLOBAL__sub_I / .tls = 0x24).
// Всё состояние — function-local static / POD-глобалы.

#include "compat/ma_sched.h"

#ifdef MA_SCHED_UNIT_TEST
// Стенд планировщика (sched_test/): заглушки игрового слоя вместо SDK.
#include "ma_sched_unit_stubs.h"
#else
#include "funcs.h"
#include "compat/diag.h"
#include "compat/ma_tasks.h"
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstring>
#include <deque>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

extern void dellod();                 // funcs.cpp: очистить lualoader\log.txt
extern int destroy(lua_State* L);     // funcs.cpp: снять сущности скрипта
extern void stop_mission_watch();     // funcs.cpp: приказ сторожу миссии выйти

// Обёртка destroy(): C++-исключение при разборке не должно убивать игровой поток
// (SEH-форму __try/__except clang для i686-mingw не собирает — только C++ try).
static bool safe_destroy(lua_State* L) {
    try { destroy(L); return true; }
    catch (...) { return false; }
}

namespace {

// ---- настройки -------------------------------------------------------------
// Сколько реального времени один скрипт может непрерывно занимать за один
// проход. Скрипт без wait() вытесняется и продолжит в следующем проходе.
// (23.10, вечер: срез 2→1 мс и проход 12→6 мс — горячий цикл вида
// `while Getflagmission() do ... end` (misson riot.lua:319) сожирал до 12 мс
// каждого кадра и «сильно тормозил» игру; паркуемые скрипты от этого не
// страдают, busy-цикл обесценен до ≤6 мс/кадр.)
constexpr unsigned int MA_SCHED_SLICE_MS = 1;
// Сколько один кадр (проход) может длиться суммарно (анти-залипание при 100+).
constexpr unsigned int MA_SCHED_PASS_MS = 6;
// Инструкций Lua между вызовами count-хука (проверка бюджета).
constexpr int MA_SCHED_HOOK_COUNT = 20000;
// Сколько файлов загружаем за один кадр (загрузка = openlibs + парсинг).
constexpr int MA_SCHED_LOADS_PER_PASS = 2;
// Сколько миллисекунд мир должен «постоять» после входа в игру, прежде чем
// скрипты начнут трогать пулы и создавать сущности. Сразу после загрузки сейва
// движок ещё достраивает мир (стриминг, кат-сцена, fade) — опкоды в этот момент
// бьют по пересоздаваемым структурам.
#ifdef MA_SCHED_UNIT_TEST
constexpr uint64_t MA_SCHED_WORLD_SETTLE_MS = 0;
#else
constexpr uint64_t MA_SCHED_WORLD_SETTLE_MS = 1500;
#endif

struct PendingArg {
    int type = LUA_TNIL;
    double num = 0.0;
    const void* ptr = nullptr;
    std::string str;
};

struct Script {
    lua_State* L = nullptr;   // мастер-состояние (владеет T)
    lua_State* T = nullptr;   // корутина, в которой исполняется скрипт
    std::string file;
    int stage = 0;            // 1 = верхний блок файла, 2 = main(), 3 = завершён
    bool dead = false;        // ошибка загрузки/исполнения (или убийство при teardown)
    bool terminating = false; // teardown: скрипт должен умереть при ближайшем resume
    bool sleeping = false;    // запаркован (wait) или вытеснен
    uint64_t wake = 0;        // время пробуждения (мс планировщика)
    uint64_t deadline = 0;    // конец текущего слайса (0 = не в резюме)
    unsigned noyield_streak = 0; // сколько слайсов подряд вытеснен БЕЗ добровольного wait()
    int nargs = 0;            // аргументы для ближайшего lua_resume (main)
    std::vector<PendingArg> args;
};

struct Sched {
    std::atomic<bool> scan_req{false};
    std::atomic<bool> teardown_req{false};
    std::atomic<bool> teardown_restart{false};
    std::atomic<bool> pending{false};   // teardown в progress (phase 1 сделан)
    std::atomic<int>  count{0};
};

Sched& S() {
    static Sched s;   // без глобального конструктора (POD + атомики)
    return s;
}

// Всё ниже — ТОЛЬКО игровой поток (единственный pump — ma_sched_frame).
// НЕТРИВИАЛЬНЫЕ контейнеры — в function-local static (guarded init): глобальный
// std::deque/unordered_map эмитит _GLOBAL__sub_I + atexit → игра в Wine не стартует.
bool g_inResume = false;
Script* g_current = nullptr;
size_t g_rr = 0;
uint64_t g_inGameSince = 0;   // 0 = игра не в мире (меню/загрузка сейва)
bool g_clean_world = false;   // teardown: чистить мир через destroy() (Ctrl-reload в мире)
bool g_restart_flag = false;  // teardown: повторный скан после разборки
std::atomic<unsigned> g_pass_ms_last(0); // сколько реального времени съел последний проход (для heartbeat)

std::vector<Script*>& scripts() {
    static std::vector<Script*> v;   // guarded init, без глобального конструктора
    return v;
}
std::unordered_map<lua_State*, Script*>& by_state() {
    static std::unordered_map<lua_State*, Script*> m;
    return m;
}
std::deque<std::string>& pending_files() {
    static std::deque<std::string> q;
    return q;
}

uint64_t now_ms() {
    using namespace std::chrono;
    static const steady_clock::time_point t0 = steady_clock::now();
    return (uint64_t)duration_cast<milliseconds>(steady_clock::now() - t0).count();
}

// ---- аргументы create_newthread (простые типы; таблицы/функции не переносим) --
PendingArg copy_arg(lua_State* L, int idx) {
    PendingArg a;
    a.type = lua_type(L, idx);
    switch (a.type) {
        case LUA_TBOOLEAN: a.num = lua_toboolean(L, idx) ? 1.0 : 0.0; break;
        case LUA_TNUMBER:  a.num = lua_tonumber(L, idx); break;
        case LUA_TSTRING:  a.str = lua_tostring(L, idx); break;
        case LUA_TLIGHTUSERDATA: a.ptr = lua_topointer(L, idx); break;
        default: a.type = LUA_TNIL; break;
    }
    return a;
}

void push_arg(lua_State* L, const PendingArg& a) {
    switch (a.type) {
        case LUA_TBOOLEAN: lua_pushboolean(L, a.num != 0.0); break;
        case LUA_TNUMBER:  lua_pushnumber(L, a.num); break;
        case LUA_TSTRING:  lua_pushstring(L, a.str.c_str()); break;
        case LUA_TLIGHTUSERDATA: lua_pushlightuserdata(L, const_cast<void*>(a.ptr)); break;
        default: lua_pushnil(L); break;
    }
}

// ---- создание состояния ----------------------------------------------------
Script* create_script(const std::string& file, std::vector<PendingArg> args) {
    if (S().pending.load()) return nullptr;   // teardown в progress — новые экземпляры не создаём
    Script* sc = new Script();
    sc->file = file;
    sc->args = std::move(args);

    lua_State* L = luaL_newstate();
    if (L == nullptr) {
        delete sc;
        return nullptr;
    }
    luaL_openlibs(L);
    sc->L = L;

    // package.path — как в старом startscipt: <cwd>\?.lua
    std::string pp = (std::filesystem::current_path() / "?.lua").string();
    lua_pushstring(L, pp.c_str());
    lua_setglobal(L, "fullpath");
    luaL_dostring(L, "package.path = fullpath");

    funs(L);   // ~1583 Lua-API функции

    lua_State* T = lua_newthread(L);   // остаётся на стеке L (индекс 1) — GC-якорь
    sc->T = T;

    // Имя файла для create_newthread(): ключ — сама корутина.
    lua_pushlightuserdata(T, T);
    lua_pushstring(T, file.c_str());
    lua_settable(T, LUA_REGISTRYINDEX);

    lua_sethook(T, ma_sched_hook, LUA_MASKCOUNT, MA_SCHED_HOOK_COUNT);

    int st = luaL_loadfile(T, file.c_str());
    if (st != LUA_OK) {
        const char* err = lua_tostring(T, -1);
        if (err) { writelog(err); ma_diag::lua_error_note("скрипт %s: %s", file.c_str(), err); }
        lua_settop(T, 0);
        sc->dead = true;
        sc->stage = 3;
    } else {
        sc->stage = 1;
        std::string loaded = "loaded " + file;
        writelog(loaded.c_str());   // совместимость с прежним журналом startscipt
    }

    scripts().push_back(sc);
    by_state()[T] = sc;
    S().count.store((int)scripts().size());

    luastate.push_back(T);   // для диагностики/совместимости (игровой поток — единственный владелец)

    return sc;
}

void report_runtime_error(Script* sc) {
    const char* err = lua_tostring(sc->T, -1);
    if (err) { writelog(err); ma_diag::lua_error_note("скрипт %s: %s", sc->file.c_str(), err); }
    lua_settop(sc->T, 0);
    sc->dead = true;
    sc->stage = 3;
}

// Один резюм/продвижение стадии (нормальный путь). Возвращает управление на любом yield.
void resume_script(Script* sc) {
    for (int guard = 0; guard < 4 && !sc->dead && sc->stage < 3; ++guard) {
        sc->deadline = now_ms() + MA_SCHED_SLICE_MS;
        sc->sleeping = false;

        g_current = sc;
        g_inResume = true;
        int nres = 0;
        int status = lua_resume(sc->T, nullptr, sc->nargs, &nres);
        sc->nargs = 0;
        g_inResume = false;
        g_current = nullptr;

        if (status == LUA_YIELD) {
            sc->deadline = 0;
            return;                       // wait() или вытеснение: ход отдан
        }
        sc->deadline = 0;
        if (status != LUA_OK) {
            report_runtime_error(sc);
            return;
        }

        if (sc->stage == 1) {
            // Верхний блок файла доиграл. Запускаем глобальную main(), если она есть.
            lua_settop(sc->T, 0);
            lua_getglobal(sc->T, "main");
            if (lua_type(sc->T, -1) == LUA_TFUNCTION) {
                int n = 0;
                for (const PendingArg& a : sc->args) { push_arg(sc->T, a); ++n; }
                sc->args.clear();
                sc->nargs = n;
                sc->stage = 2;
                continue;                 // сразу резюмим main в этом же слайсе
            }
            lua_pop(sc->T, 1);
            sc->stage = 3;
            return;
        }

        if (sc->stage == 2) {
            sc->stage = 3;
            return;
        }
    }
}

// ---- onScriptTerminate / финализация ---------------------------------------
// Состояние МОЁТ быть приостановленным (script был запаркован) ИЛИ мёртвым
// (main завершился/упал). В обоих случаях на нём можно запустить НОВЫЙ чанк:
// lua_pcall на состоянии, где предыдущий чанк закончился, исполняет новую
// функцию, а yield-стек корутины при этом не трогает. Приостановленный main()
// при этом ЗАБРАШЕН — планировщик его больше не возобновляет (скрипт «мертв»).
void call_on_script_terminate(Script* sc) {
    lua_getglobal(sc->T, "onScriptTerminate");
    if (lua_type(sc->T, -1) == LUA_TFUNCTION) {
        if (lua_pcall(sc->T, 0, 0, 0) != LUA_OK) {
            const char* err = lua_tostring(sc->T, -1);
            lua_settop(sc->T, 0);
        }
    } else {
        lua_pop(sc->T, 1);
    }
}

void finalize_script(Script* sc) {
    if (!sc->dead) sc->dead = true;
    call_on_script_terminate(sc);
    // destroy() шлёт игровые опкоды. Мы НА игровом потоке — выполняем напрямую
    // (очередь не нужна). Мир чистим только при явной перезагрузке (Ctrl) и
    // только когда игра уже в мире: при загрузке сейва мир сбрасывает сама игра.
    if (g_clean_world) {
        safe_destroy(sc->T);
    }
    lua_state_unstamp(sc->T);
    luastate.remove(sc->T);
    auto it = by_state().find(sc->T);
    if (it != by_state().end()) by_state().erase(it);
    S().count.store((int)scripts().size());
    delete sc;
}

// ---- убийство (teardown, phase 2) ------------------------------------------
// Скрипт помечен terminating: _ma_terminate стоит, wait() шлёт ошибку,
// count-хук то же. Один резюм продвигает скрипт на слайс; если он всё ещё
// жив (yield) — попробуем в следующем кадре. Как только main() умрёт — finalize.
void kill_resume(Script* sc) {
    sc->deadline = now_ms() + MA_SCHED_SLICE_MS;
    sc->sleeping = false;
    g_current = sc;
    g_inResume = true;
    int nres = 0;
    int status = lua_resume(sc->T, nullptr, 0, &nres);
    sc->deadline = 0;
    g_inResume = false;
    g_current = nullptr;

    if (status == LUA_YIELD) return;   // ещё жив — следующий кадр

    sc->dead = true;
    sc->stage = 3;
    sc->terminating = false;
    lua_settop(sc->T, 0);
    // НЕ финализируем здесь: все финализации делает ОДИН цикл в
    // end_teardown_check (иначе — двойное удаление через висячие указатели).
}

// ---- teardown ---------------------------------------------------------------
// Phase 1 (из ma_sched_frame по флагу): быстрые атомарные шаги + пометки.
void begin_teardown(bool restart) {
    if (S().pending.load()) return;
    teardown_active.store(true);
    stop_mission_watch();
    g_clean_world = restart && (gGameState == 9);
    g_restart_flag = restart;
    for (Script* sc : scripts()) {
        if (sc == nullptr || sc->dead || sc->stage >= 3) continue;
        sc->terminating = true;
        // Состояние приостановлено (frame-boundary) — стопку трогать можно.
        lua_pushboolean(sc->T, 1);
        lua_setglobal(sc->T, "_ma_terminate");
    }
    S().pending.store(true);
}

// Phase 2 (каждый кадр, пока pending): дождаться снятия сторожа, убить живые
// (одна попытка на кадр), и когда ВСЕ мёртвы — единый цикл финализации.
void end_teardown_check() {
    // Сторож миссии дёргает игровые структуры: ждём, пока игровой поток
    // (ma_tasks_frame, раньше нас в этом же кадре) снимет его из реестра.
    if (ma_tasks().find_task("mission_watch") != nullptr) return;

    // Пасс убийства: живые помеченные скрипты продвигаем на один слайс.
    for (Script* sc : scripts()) {
        if (sc == nullptr || sc->dead || sc->stage >= 3) continue;
        if (sc->terminating) kill_resume(sc);
    }

    bool all_dead = true;
    for (Script* sc : scripts()) {
        if (sc != nullptr && !sc->dead && sc->stage < 3) { all_dead = false; break; }
    }
    if (!all_dead) return;

    // ЕДИНЫЙ цикл финализации (каждое состояние — ровно один раз):
    // onScriptTerminate + destroy + снятие из реестров + удаление.
    for (Script* sc : scripts()) {
        if (sc == nullptr) continue;
        finalize_script(sc);
    }
    scripts().clear();
    by_state().clear();
    pending_files().clear();
    g_rr = 0;
    g_inGameSince = 0;        // после reload снова ждём «успокоения» мира
    listfile.clear();

    teardown_active.store(false);
    g_clean_world = false;
    S().pending.store(false);
    S().count.store(0);
    if (g_restart_flag) S().scan_req.store(true);
    g_restart_flag = false;
}

// ---- скан файлов -----------------------------------------------------------
void do_scan() {
    if (!scripts().empty()) {
        return;
    }
    std::vector<std::string> files;
    std::error_code ec;
    auto dir = std::filesystem::current_path() / "lualoader";
    for (auto it = std::filesystem::recursive_directory_iterator(dir, ec);
         !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        std::string ext = it->path().extension().string();
        if (ext == ".lua" || ext == ".LUA") files.push_back(it->path().string());
    }
    std::sort(files.begin(), files.end());

    dellod();                 // чистим журнал один раз на загрузку (как старый search_scripts)
    pending_files().clear();
    for (auto& f : files) pending_files().push_back(f);
}

} // namespace

// ============================================================================
// Публичный API
// ============================================================================

// Один кадр планировщика. Вызывается из Events::gameProcessEvent (игровой поток),
// после ma_tasks_frame(). Порядок внутри кадра:
//   1. teardown (запросы/продолжение),
//   2. скан (первый кадр после запроса/teardown-restart),
//   3. пауза/не-в-мире/успокоение мира — выход без работы,
//   4. загрузка файлов порциями (пока есть pendingFiles),
//   5. round-robin-проход по живым скриптам в пределах бюджета кадра,
//   6. мягкий инкрементальный GC.
void ma_sched_frame() {
    if (S().teardown_req.exchange(false)) {
        begin_teardown(S().teardown_restart.exchange(false));
    }
    if (S().pending.load()) {
        end_teardown_check();
        return;
    }
    if (S().scan_req.exchange(false)) do_scan();
    if (scripts_paused.load()) return;

    // Игровые вызовы из скриптов допустимы только когда мир уже собран.
    // В меню и во время загрузки сейва gGameState != 9, а пулы/стриминг
    // пересоздаются: любой опкод оттуда — запись в освобождённую память.
    if (gGameState != 9) {
        g_inGameSince = 0;
        return;
    }
    uint64_t now = now_ms();
    if (g_inGameSince == 0) g_inGameSince = now;
    if (now - g_inGameSince < MA_SCHED_WORLD_SETTLE_MS) return;

    // Загрузка файлов порциями: не держим кадр на 100+ скриптах (file I/O + парсинг).
    if (!pending_files().empty()) {
        for (int i = 0; i < MA_SCHED_LOADS_PER_PASS && !pending_files().empty(); ++i) {
            std::string f = pending_files().front();
            pending_files().pop_front();
            create_script(f, {});
            if (S().teardown_req.load()) break;
        }
        return;
    }

    // Round-robin-проход в пределах бюджета кадра.
    uint64_t passStart = now;
    bool ran = false;
    size_t n = scripts().size();
    for (size_t k = 0; k < n && (now_ms() - passStart) < MA_SCHED_PASS_MS; ++k) {
        if (S().teardown_req.load() || S().scan_req.load()) break;
        size_t idx = (g_rr + k) % n;
        Script* sc = scripts()[idx];
        if (sc == nullptr || sc->dead || sc->stage >= 3) continue;

        if (sc->terminating) { kill_resume(sc); ran = true; continue; }
        if (sc->sleeping && sc->wake > now_ms()) continue;

        sc->sleeping = false;
        resume_script(sc);
        ran = true;
        g_rr = (idx + 1) % n;
    }
    (void)ran;
    g_pass_ms_last.store((unsigned)(now_ms() - passStart)); // метрика для heartbeat

    // Мягкий инкрементальный GC: раньше он был вообще остановлен, и память
    // росла всё время игры. Здесь — по чуть-чуть, когда скрипт не работает.
    if (!scripts().empty()) {
        Script* sc = scripts()[g_rr % scripts().size()];
        if (sc && sc->L) lua_gc(sc->L, LUA_GCSTEP, 1);
    }
}

void ma_sched_scan() {
    S().scan_req.store(true);
}

uint64_t ma_sched_now_ms() {
    return now_ms();
}

int ma_sched_count() {
    return S().count.load();
}

bool ma_sched_park(lua_State* L, int ms) {
    auto it = by_state().find(L);
    if (it == by_state().end()) return false;
    Script* sc = it->second;
    if (sc->dead) return false;
    if (ms <= 0) ms = 1;
    sc->sleeping = true;
    sc->wake = now_ms() + (uint64_t)ms;
    // ДОБРОВОЛЬНОЕ уступление (wait()): бэк-офф спин-циклов к таким скриптам не
    // применяется — сбрасываем счётчик.
    sc->noyield_streak = 0;
    return true;
}

void ma_sched_hook(lua_State* L, lua_Debug* ar) {
    (void)ar;
    if (!g_inResume || g_current == nullptr || g_current->T != L) return;
    Script* sc = g_current;

    // TEARDOWN: убить скрипт — любое дальнейшее исполнение кода должно
    // закончиться ошибкой (longjmp в границу lua_resume). Хук вызывается
    // между байткодами; lua_error из него — та же механика, что из C-функции.
    if (sc->terminating) {
        luaL_error(L, "SCRIPT_STOPPED_BY_CPP");
    }

    // Пауза (reload/стоп): скрипт замирает ровно на своём кадре.
    if (scripts_paused.load()) {
        sc->deadline = 0;
        sc->sleeping = true;
        sc->wake = now_ms() + 1;
        if (lua_isyieldable(L)) lua_yieldk(L, 0, 0, nullptr);
        return;
    }

    uint64_t now = now_ms();
    if (sc->deadline != 0 && now >= sc->deadline) {
        // Бюджет слайса исчерпан. Вытесняем ТОЛЬКО в Lua-коде: вне yieldable
        // (require/dofile/парсинг) yield запрещён — просто отложим пробуждение.
        sc->deadline = 0;
        sc->sleeping = true;
        // (23.10, вечер) Экспоненциальный бэк-офф для «спин-циклов»: скрипт,
        // который НЕ делает wait() (например `while Getflagmission() do ... end`
        // без паузы — misson riot.lua:319), вытесняется всё реже: 2,4,8,...,250 мс
        // между слайсами. Такой цикл «обесценивается» до ~4 вызовов/сек и НЕ ЕСТ
        // игровой кадр, а добровольный wait() (ma_sched_park) счётчик СБРАСЫВАЕТ —
        // нормальные скрипты не замедляются. Без бэк-оффа горячий цикл жрал до
        // PASS_MS каждого кадра и ронял FPS игры до 10.
        sc->noyield_streak++;
        unsigned backoff = 2u * sc->noyield_streak;
        if (backoff > 250u) backoff = 250u;
        sc->wake = now + backoff;
        if (lua_isyieldable(L)) {
            lua_yieldk(L, 0, 0, nullptr);
        }
    }
}

void ma_sched_request_teardown(bool restart) {
    S().teardown_restart.store(restart);
    S().teardown_req.store(true);
}

void ma_sched_request_pause(bool paused) {
    scripts_paused.store(paused);
}

lua_State* ma_sched_current_L() {
    return g_inResume ? g_current->T : nullptr;
}

unsigned ma_sched_last_pass_ms() {
    return g_pass_ms_last.load();
}

bool ma_sched_spawn(lua_State* parent, int firstArg) {
    if (teardown_active.load() || S().pending.load()) {
        return false;
    }
    // Имя файла лежит в реестре под ключом самой корутины.
    lua_pushlightuserdata(parent, parent);
    lua_gettable(parent, LUA_REGISTRYINDEX);
    const char* file = lua_type(parent, -1) == LUA_TSTRING ? lua_tostring(parent, -1) : nullptr;
    lua_pop(parent, 1);
    if (file == nullptr) {
        return false;
    }

    std::vector<PendingArg> args;
    int top = lua_gettop(parent);
    for (int i = firstArg; i <= top; ++i) {
        PendingArg a = copy_arg(parent, i);
        args.push_back(a);
    }
    if (create_script(file, std::move(args)) == nullptr) return false;
    return true;
}
