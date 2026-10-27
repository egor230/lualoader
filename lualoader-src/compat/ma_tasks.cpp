// ma_tasks.cpp — кооперативный планировщик задач на игровом потоке.
//
// Все задачи крутятся только с игрового потока (ma_tasks_frame из Events::gameProcessEvent).
// Никаких detached std::thread'ов — Wine не будет дёргать RtlpWaitForCriticalSection.

#include "funcs.h" // полные определения CVehicle/CVector/Command<> и т.д.
#include "ma_tasks.h"
#include "diag.h"

// (23.10) ma_game_ms: была только ОБЪЯВЛЕНА (ma_tasks.h) — линковка упала бы
// "undefined". Игровое время из SDK (funcs.h подключает CTimer.h).
unsigned long long ma_game_ms() {
    return (unsigned long long)CTimer::m_snTimeInMilliseconds;
}

// (23.10) Псевдо-«стоп всех скриптов»: было extern ma_should_stop_all(), но
// ОПРЕДЕЛЕНИЯ не было нигде. Здесь — function-local static (в этом TU глобальный
// объект с конструктором запрещён: см. комментарий ниже, из-за него глохла Wine).
std::atomic<bool>& ma_should_stop_all() {
    static std::atomic<bool> s{false};
    return s;
}

// (23.10) Пул ПОСТОЯННЫХ строк сообщений — определён в funcs.cpp (внешняя связь),
// объявление здесь. Add*Message* хранит НАШ указатель (дизасм 20.10), getwchat()
// возвращает временный буфер → посылать сообщение можно только из ma_msgpool.
namespace ma_msgpool { wchar_t* put(const char* c); }

// ВАЖНО (05.10): здесь НЕТ ни одного глобального объекта с нетривиальным конструктором.
// Реестр и зеркало — function-local static. Причина: порядок динамической инициализации
// в PE/COFF следует порядку объектных файлов, и глобал с конструктором в этом TU уезжал
// вперёд динамики libstdc++/SDK. В Wine это давало висяк RtlpWaitForCriticalSection
// («blocked by 0000») в самом начале загрузки DLL — игра не стартовала вообще.
ma_task_registry& ma_tasks() {
    static ma_task_registry s;   // создаётся при первом обращении, не при загрузке DLL
    return s;
}

// Атомарное зеркало star_thread::get() — тоже без глобального конструктора
// (std::atomic<bool> тривиален, но держим единый стиль: ноль глобалов в этом TU).
// Кэш клавиатурной задачи (только игровой поток): см. ma_keyboard().
static ma_task_keyboard* g_keyboard = nullptr;

std::atomic<bool>& ma_star_mirror() {
    static std::atomic<bool> s{false};
    return s;
}

// ============================================================
// ma_tasks_frame — один кадр: прокачать проснувшиеся задачи.
// Вызывается только с игрового потока.
// ============================================================
void ma_tasks_frame() {
    if (teardown_active.load() || ma_should_stop_all().load()) {
        ma_tasks_teardown();
        return;
    }

    unsigned long long now = ma_game_ms();
    // Персистентный буфер: ma_tasks().snapshot() возвращает вектор ПО ЗНАЧЕНИЮ,
    // то есть malloc+free на КАЖДОМ кадре. Крутим только игровой поток, поэтому
    // один статический буфер безопасен.
    static std::vector<ma_task_base*> snap;
    ma_tasks().snapshot_into(snap);

    size_t budget = 0;
    for (size_t i = 0; i < snap.size() && budget < MA_TASKS_PER_FRAME; i++) {
        ma_task_base* t = snap[i];
        if (now < t->wake) continue; // спит до своего времени

        bool done = t->step();

        if (done) {
            ma_tasks().remove(t);
            delete t;
        }
        budget++;
    }
}

void ma_tasks_teardown() {
    auto snap = ma_tasks().snapshot();
    for (auto* t : snap) {
        ma_tasks().remove(t);
        if (t == g_keyboard) g_keyboard = nullptr;   // не оставляем висячий указатель
        delete t;
    }
    g_keyboard = nullptr;
}

// ============================================================
// ma_task_getcord::step — одна итерация маршрута.
// ============================================================
bool ma_task_getcord::step() {
    if (teardown_active.load() || ma_should_stop_all().load()) return true;

    if (scripts_paused.load()) {
        wake = ma_game_ms() + 10;
        return false;
    }

    CVehicle* car = findcarinpool(p);
    if (!car) {
        return true;
    }

    // (23.10) Точка маршрута ТОЛЬКО задаётся ЦЕЛЬЮ в CAR_GOTO. Точка удерживается
    // до её касания (bugfix: pop-и-забыли терял недоеденные точки маршрута в wake).
    if (!has_cur) {
        if (q.empty()) {
            return true;
        }
        cur_x = q.front(); q.pop();
        cur_y = q.front(); q.pop();
        cur_z = q.front(); q.pop();
        has_cur = true;
    }

    Command<COMMAND_CAR_GOTO_COORDINATES>(car, cur_x, cur_y, cur_z);

    if (!car->IsSphereTouchingVehicle(cur_x, cur_y, cur_z, 3.0f)) {
        wake = ma_game_ms() + 16; // не доехали — повторный прогон (point НЕ выпадает)
        return false;
    }
    has_cur = false;
    wake = ma_game_ms();
    return false;
}

// ============================================================
// ma_task_expectations::step — ждём видимости модели.
// ============================================================
bool ma_task_expectations::step() {
    if (teardown_active.load() || ma_should_stop_all().load()) return true;

    if (attempts >= kMaxAttempts) {
        return true;
    }
    attempts++;

    CVehicle* car = findcarinpool(owner);
    if (!car) return true;

    if (!car->IsVisible()) {
        wake = ma_game_ms() + 100;
        return false;
    }

    Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
    return true;
}

// ============================================================
// ma_task_turncar::step — вращение авто по оси.
// ============================================================
bool ma_task_turncar::step() {
    if (teardown_active.load() || ma_should_stop_all().load()) return true;

    if (scripts_paused.load()) {
        wake = ma_game_ms() + 10;
        return false;
    }

    CVehicle* car = findcarinpool(p);
    if (!car) {
        return true;
    }

    CVector* ang = get_car_angles_on_axes(car);
    if (!ang) return true;

    bool done = false;
    if (switc == 1) {
        car->m_vecFrictionTurnForce.x = speed;
        car->m_vecTurnSpeed.x = speed;
        done = ang->x >= angle;
    } else if (switc == 2) {
        car->m_vecFrictionTurnForce.y = speed;
        car->m_vecTurnSpeed.y = speed;
        done = ang->y >= angle;
    } else {
        car->m_vecFrictionTurnForce.z = speed;
        car->m_vecTurnSpeed.z = speed;
        done = ang->z >= angle;
    }

    if (done) {
        return true;
    }

    wake = ma_game_ms() + 1;
    return false;
}

// ============================================================
// ma_task_turncar_delay::step — вращение с задержкой.
// ============================================================
bool ma_task_turncar_delay::step() {
    if (teardown_active.load() || ma_should_stop_all().load()) return true;

    if (scripts_paused.load()) {
        wake = ma_game_ms() + 10;
        return false;
    }

    CVehicle* car = findcarinpool(p);
    if (!car) {
        return true;
    }

    if (next_iter > ma_game_ms()) {
        wake = next_iter;
        return false;
    }

    next_iter = ma_game_ms() + time_ms;

    CVector* ang = get_car_angles_on_axes(car);
    if (!ang) return true;

    bool done = false;
    if (switc == 1) {
        car->m_vecFrictionTurnForce.x = 0.001f;
        car->m_vecTurnSpeed.x = 0.001f;
        done = ang->x >= angle;
    } else if (switc == 2) {
        car->m_vecFrictionTurnForce.y = 0.001f;
        car->m_vecTurnSpeed.y = 0.001f;
        done = ang->y >= angle;
    } else {
        car->m_vecFrictionTurnForce.z = 0.001f;
        car->m_vecTurnSpeed.z = 0.001f;
        done = ang->z >= angle;
    }

    if (done) {
        return true;
    }

    wake = next_iter; // (23.10) явное ожидание до следующей итерации
    return false;
}

// ============================================================
// Фабрики.
// ============================================================
ma_task_getcord* ma_task_create_getcord(std::queue<float> q, const void* p) {
    auto* t = new ma_task_getcord(std::move(q), p);
    ma_tasks().add(t);
    return t;
}

ma_task_expectations* ma_task_create_expectations(int model, CVehicle* car) {
    auto* t = new ma_task_expectations(model, car);
    ma_tasks().add(t);
    return t;
}

// (23.10) Фабрики turncar* принимают СЫРОЙ ключ из Lua: авто может быть удалено,
// пока задача крутится — резолв живёт в step() на игровом потоке.
ma_task_turncar* ma_task_create_turncar(const void* p, float angle, float speed, int switc) {
    auto* t = new ma_task_turncar(p, angle, speed, switc);
    ma_tasks().add(t);
    return t;
}

ma_task_turncar_delay* ma_task_create_turncar_delay(const void* p, float angle, int time_ms, int switc) {
    auto* t = new ma_task_turncar_delay(p, angle, time_ms, switc);
    t->next_iter = ma_game_ms() + time_ms; // (23.10) первый оборот — ПОСЛЕ паузы (как старое тело)
    ma_tasks().add(t);
    return t;
}

// ============================================================
// ma_task_mission_watch — кооперативная замена std::thread(mission_watch_proc).
// Крутится на игровом потоке раз в 100 мс. Все игровые вызовы — напрямую
// (без ma_cmdq_defer): мы уже на игровом потоке.
//
// Логика (пошаговый аналог старого while-цикла):
//   1. Если stop == true → завершить (наружный запрос остановки).
//   2. Если !running → завершить (задача устарела).
//   3. Читаем здоровье/арест игрока (FindPlayerPed — игровая память, мы на игровом потоке).
//   4. Если dead || arest:
//        - показываем "Mission failed!" через CMessages::AddBigMessage
//        - сбрасываем flag миссии (CTheScripts::ScriptSpace[flag] = false)
//        - destroy(L) — очистка Lua-состояния
//        - завершаем задачу (done=true → step() вернёт true)
//   5. Если mission_flag_get() == false (флаг сменился сам) → завершаем.
//   6. Иначе: wake = now+100, возвращаем false (продолжить).
// ============================================================
bool ma_task_mission_watch::step() {
    if (teardown_active.load() || ma_should_stop_all().load()) {
        // teardown: завершаем без destroy (реестр уже чистит задачи)
        if (running) running->store(false);
        return true;
    }

    if (stop && stop->load()) {
        if (running) running->store(false);
        return true;
    }
    if (running && !running->load()) {
        // running=false → задача остановлена извне
        return true;
    }

    // Читаем состояние игрока — на игровом потоке, без блокировок.
    CPed* pl = FindPlayerPed();
    bool dead = (pl == NULL) || (pl->m_fHealth <= 1.0f);
    bool arest = (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED);

    // Читаем flag миссии (только на игровом потоке).
    unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
    bool flag_valid = (OnAMissionFlag < 260512);
    bool flag_active = flag_valid ? (bool)CTheScripts::ScriptSpace[OnAMissionFlag] : false;

    if (dead || arest) {
        // (23.10) Буфер AddBigMessage обязан жить вечно (движок хранит УКАЗАТЕЛЬ,
        // дизасм 20.10): та же правка, что у printmessage — пул ma_msgpool.
        // getwchat() давал висячий wchar на временной памяти.
        wchar_t* str = ma_msgpool::put("Mission failed!");
        CMessages::AddBigMessage(str, 5000, 0);
        if (flag_valid) {
            CTheScripts::ScriptSpace[OnAMissionFlag] = false;
        }
        if (L != nullptr) {
            destroy(L);
        }
        if (running) running->store(false);
        return true; // завершено
    }

    if (!flag_active) {
        // Флаг сменился сам (или повреждён) → просто выходим.
        if (running) running->store(false);
        return true;
    }

    wake = ma_game_ms() + 100; // следующий тик через 100 мс
    return false; // продолжаем
}

ma_task_mission_watch* ma_task_create_mission_watch(
    lua_State* L, std::atomic<bool>* stop, std::atomic<bool>* running) {
    // (23.10) РОВНО ОДИН сторож: живой — обновляем Lua-состояние (как старый
    // mission_watch_start уничтожал старый поток и поднимал новый). Двойной new
    // давал бы двойное удаление из реестра = класс double-free (док 02:3203).
    auto snap = ma_tasks().snapshot();
    for (auto* t : snap) {
        if (t->name != nullptr && std::strcmp(t->name, "mission_watch") == 0) {
            auto* mw = static_cast<ma_task_mission_watch*>(t);
            mw->L = L;
            if (running) running->store(true);
            return mw;
        }
    }
    auto* t = new ma_task_mission_watch(L, stop, running);
    ma_tasks().add(t);
    return t;
}

// ============================================================
// ma_task_keyboard — персональная задача (singleton): опрос клавиатуры
// каждый игровой кадр. Полностью заменяет два detached std::thread
// (reload + getkeyenvent) из старого кода plugin.cpp.
// Крутится ТОЛЬКО на игровом потоке.
//
// Сам step() НЕ трогает игровую память и НЕ вызывает static-функции plugin.cpp
// (teardown_all / HandleClearFlush — static в plugin.cpp, из ma_tasks.cpp невидимы):
//   * Ctrl-нажатие -> флаги ctrl_req/ctrl_phase (читает только GetKeyState);
//   * "clrfl" в cheatstr -> флаг clrfl_req (cheatstr_* обёртки — под g_cheat_mx).
// Действия по флагам выполняет Events::gameProcessEvent в plugin.cpp — на игровом
// потоке, где уже живут teardown_all/HandleClearFlush. step() ВСЕГДА возвращает
// false: задача живёт весь сеанс (как старый detached-поток); из реестра её
// убирает только ma_tasks_teardown().
// ============================================================
bool ma_task_keyboard::step() {
    // (26.10, P1) ЕДИНЫЙ СНИМОК КЛАВИАТУРЫ: единственный Win32-опрос клавиш
    // (GetAsyncKeyState) во всём лоадере — здесь, раз в кадр, НА ИГРОВОМ
    // ПОТОКЕ, до скриптов (задача идёт перед планировщиком). getkey/
    // keypress/wait_key читают только снимок ma_keys() — без syscall'ов
    // в пути Lua (прежние ~183 GetAsyncKeyState/кадр на чек-стринг сняты).
    ma_keys_poll();

    // ---- Ctrl-фаза: аналогия reload() ----
    // Фазовая машина: «был отжат -> нажат» = одно нажатие = один teardown_all(true)
    // (вызовет plugin.cpp по флагу ctrl_req). Пока Ctrl удерживаем, ждём отпускания.
    bool ctrl = plugin::KeyPressed(VK_CONTROL);
    if (!ctrl_phase) {
        if (ctrl) {
            ctrl_req = true;   // игровому потоку: teardown_all(true)
            ctrl_phase = true; // дальше только ждём отпускания
        }
    } else if (!ctrl) {
        // (23.10) Старый reload-поток делал teardown НА ОТПУСКАНИИ Ctrl.
        // Plugin.cpp ждёт release_pending, а не ctrl_req — семантика сохранена.
        if (ctrl_req) release_pending = true;
        ctrl_phase = false;
    }

    // ---- Опрос клавиш (аналог getkeyenvent из plugin.cpp): только когда star_thread разрешён ----
    // (23.10) ОРИГИНАЛЬНАЯ версия ma_task_keyboard не существовала в сборке и звала
    // несуществующие обёртки cheatstr_add/... — заменено на работу с ЕДИНЫМ глобалом
    // cheatstr (funcs.cpp:28), который читает Lua-функция checkcheat.
    // Детектор нажатия по КРАЮ (last_char): как старый getkeyenvent, удержание
    // клавиши не вливает символ в cheatstr заново — иначе cheatstr заполнялся бы
    // повторами каждый кадр и checkcheat() ломался.
    if (ma_star_mirror().load()) {
        for (int q = 8; q <= 190; q++) {
            string s = getkey(q); // читает единый снимок (ma_keys), без Win32-вызова
            if (s.empty()) { last_char[q].clear(); continue; } // клавиша отпущена → готовы к повтору
            if (last_char[q] == s) continue; // клавиша ещё зажата — уже записано
            last_char[q] = s;
            cheatstr = cheatstr + s;
            int size = (int)cheatstr.size(); // длина больше 20 — чистим (как getkeyenvent)
            if (size > 19) { cheatstr.clear(); }
            // "clrfl" — чит-команда ПОЛНОЙ ОСТАНОВКИ всех Lua-скриптов.
            if (cheatstr.find("clrfl") != string::npos) {
                cheatstr.clear(); // сброс, чтобы не сработало повторно
                clrfl_req = true; // игровому потоку: полная остановка (plugin.cpp)
            }
        }
    }

    wake = ma_game_ms(); // готов к следующему кадру
    return false;        // задача живёт весь сеанс
}

// ============================================================
// Singleton: добавляем keyboard-задачу ОДИН раз на сессию.
// ============================================================
void ma_task_start_keyboard() {
    if (g_keyboard) return;              // уже есть — не плодим дублей (O(1), без лока)
    auto snap = ma_tasks().snapshot();
    for (auto* t : snap) {
        if (t->name != nullptr && std::strcmp(t->name, "keyboard") == 0) { g_keyboard = static_cast<ma_task_keyboard*>(t); return; }
    }
    g_keyboard = new ma_task_keyboard();
    ma_tasks().add(g_keyboard);
}

// O(1)-доступ к клавиатурной задаче для покадрового обработчика в plugin.cpp.
// Раньше там стояло find_task("keyboard") — мьютекс + strcmp на КАЖДОМ кадре.
// Указатель живёт ровно между add() в ma_task_start_keyboard() и удалением
// в ma_tasks_teardown(), где мы его гасим, чтобы не остался висячим.
ma_task_keyboard* ma_keyboard() { return g_keyboard; }
