// ma_tasks.h — кооперативный планировщик задач на игровом потоке.
//
// Проблема: detached std::thread'ы в funcs.cpp вызывают ma_cmdq_defer, который
// блокирует поток через cv.wait_for. В Wine это вызывает RtlpWaitForCriticalSection
// deadlock. Решение: все длительные задачи (getcord, expectations, turncar,
// turncar_with_delay) превращаются в state machines, которые крутит игровой поток
// через ma_tasks_frame() из Events::gameProcessEvent.
//
// Архитектура:
//   - ma_task_base — базовая state machine с полем wake (время пробуждения, мс)
//   - ma_task_registry — вектор активных задач + мьютекс
//   - ma_tasks_frame() — вызывается каждый игровой кадр: прокачивает проснувшиеся
//     задачи по бюджету (одна итерация за кадр, без sleep_for)
//
// Все задачи работают НА ИГРОВОМ ПОТОКЕ — data races нет по определению.

#ifndef LUALOADER_MA_TASKS_H
#define LUALOADER_MA_TASKS_H

#include <atomic>
#include <chrono>
#include <cstring>
#include <mutex>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>

// Forward declarations (полные определения — в funcs.h / SDK).
class CVehicle;
struct CVector;

// Объявления функций из funcs.cpp / plugin.cpp.
CVehicle* findcarinpool(const void* p);
CVector* get_car_angles_on_axes(CVehicle* car);
// (23.10) Даёт доступ к CTimer::m_snTimeInMilliseconds (игровое время в мс).
// Была объявлена, но НЕ определена — активация в build.sh падала на линковке.
unsigned long long ma_game_ms();

// Атомарные флаги (определения — в plugin.cpp, объявления — в funcs.h).
extern std::atomic<bool> teardown_active;
extern std::atomic<bool> scripts_paused;
// Псевдо-«стоп всех скриптов» (модель по директиве юзера 18.10). Было extern
// g_shouldStopAllScripts, но НИКТО его не определял → undefined symbol при линковке.
// Теперь function-local static в ma_tasks.cpp + доступ через эту ссылку
// (правило: никакого глобального объекта с конструктором в этом TU).
std::atomic<bool>& ma_should_stop_all();

// Зеркало star_thread::get() (function-local static, см. ma_tasks.cpp): разрешён ли сейчас
// опрос клавиш. Старые detached-потоки getkeyenvent крутили цикл только пока star_thread=true,
// теперь то же правило живёт в ma_task_keyboard (чутьё через это атомарное зеркало,
// т.к. struct star_thread определён в plugin.cpp и из ma_tasks.cpp невидим).
std::atomic<bool>& ma_star_mirror();

// Бюджет на одну задачу в кадре (мс реального времени).
static constexpr unsigned int MA_TASK_SLICE_MS = 5;

// Максимум задач в кадре (анти-starvation).
static constexpr size_t MA_TASKS_PER_FRAME = 8;

// Базовая state machine: все длительные задачи наследуют от этого.
// Виртуальный step() — одна итерация работы (без sleep_for). Возвращает:
//   false = задача завершена, можно удалить из реестра
//   true  = задача ещё активна, зови на следующем кадре (или по wake)
struct ma_task_base {
    unsigned long long wake = 0;
    void* owner = nullptr;
    const char* name = "task";

    virtual ~ma_task_base() {}
    virtual bool step() = 0;
};

// Реестр активных задач.
class ma_task_registry {
public:
    void add(ma_task_base* t) {
        std::lock_guard<std::mutex> lk(mx_);
        tasks_.push_back(t);
    }
    void remove(ma_task_base* t) {
        std::lock_guard<std::mutex> lk(mx_);
        auto it = std::find(tasks_.begin(), tasks_.end(), t);
        if (it != tasks_.end()) tasks_.erase(it);
    }
    std::vector<ma_task_base*> snapshot() {
        std::lock_guard<std::mutex> lk(mx_);
        return tasks_;
    }
    // То же, но БЕЗ аллокации: out переиспользует свой буфер (operator= копирует
    // в уже выделенную память). Для вызовов на каждом кадре — обязательно:
    // snapshot() возвращает вектор по значению, то есть malloc+free каждый кадр.
    void snapshot_into(std::vector<ma_task_base*>& out) {
        std::lock_guard<std::mutex> lk(mx_);
        out = tasks_;
    }
    // Найти задачу по имени (nullptr если нет).
    // 23.10: сравниваем СОДЕРЖИМОЕ строки, а не указатели. Литералы "mission_watch"
    // в разных TU могут иметь разные адреса — указательное сравнение молча не
    // находило задачу (и планировщик не мог дождаться снятия сторожа).
    ma_task_base* find_task(const char* task_name) {
        std::lock_guard<std::mutex> lk(mx_);
        for (auto* t : tasks_) {
            if (t->name != nullptr && std::strcmp(t->name, task_name) == 0) return t;
        }
        return nullptr;
    }
    size_t size() {
        std::lock_guard<std::mutex> lk(mx_);
        return tasks_.size();
    }
    void clear() {
        std::lock_guard<std::mutex> lk(mx_);
        tasks_.clear();
    }
private:
    std::vector<ma_task_base*> tasks_;
    std::mutex mx_;
};

// Доступ к реестру задач. НЕ глобальный объект, а function-local static: см. комментарий
// в ma_tasks.cpp — глобал с конструктором в этом TU ломал статическую инициализацию.
ma_task_registry& ma_tasks();

// Вызывается из Events::gameProcessEvent (игровой поток), после ma_sched_frame().
void ma_tasks_frame();

// Сброс всех задач (teardown / clrfl / reload).
void ma_tasks_teardown();

// 5) keyboard — ПЕРСОНАЛЬНАЯ задача (singleton, добавляется вручную из plugin.cpp):
//    опрос клавиатуры КАЖДЫЙ кадр на игровом потоке. Полностью заменяет два detached
//    std::thread (reload + getkeyenvent) из старого кода — в Wine они дёргали
//    RtlpWaitForCriticalSection и вели к deadlock'у.
//
//    Что делает step() (только игровой поток):
//      * Ctrl: «был отжат -> нажат» -> ctrl_req=true (фаза wait_for_release, пока
//        Ctrl не отпустят — одно нажатие = один перезапуск, аналог reload()).
//        Действие (teardown_all(true)) выполняет plugin.cpp по флагу ctrl_req,
//        т.к. teardown_all — static в plugin.cpp (только атомарные флаги).
//      * при star_thread::get() — опрос клавиш 8..190 через getkey() (GetAsyncKeyState,
//        thread-safe), накопление в cheatstr через обёртки под g_cheat_mx; при длине >19
//        — сброс; при "clrfl" — clrfl_req=true.
//      * clrfl_req: действие (HandleClearFlush) выполняет plugin.cpp — только игровая
//        память, и только с игрового потока.
//
//    step() ВСЕГДА возвращает false: задача живёт весь сеанс (как старый detached-поток),
//    из реестра её убирает только ma_tasks_teardown().
//
//    Внимание: должен быть ОДИН экземпляр — дубль дал бы двойное teardown_all.
struct ma_task_keyboard : ma_task_base {
    bool ctrl_req = false;   // Ctrl-нажатие поймано -> plugin.cpp вызовет teardown_all(true).
    bool ctrl_phase = false; // ждём отпускания Ctrl (фаза wait_for_release).
    bool release_pending = false; // (23.10) Ctrl ОТПУЩЕН в фазе ctrl_phase -> plugin.cpp
                                  // делает reload РОВНО как старый reload-поток (на отпускании).
    bool clrfl_req = false;  // "clrfl" собран -> plugin.cpp вызовет полную остановку скриптов.
    string last_char[191];   // (23.10) край нажатия по клавише (9..190): без него символы
                             // удержания вливаются каждый кадр — checkcheat ломался.
    ma_task_keyboard() { name = "keyboard"; }
    bool step() override;
};

// Фабрика singleton'а: добавляет keyboard-задачу ОДИН раз (повторный вызов — no-op).
// Безопасна из любого потока (lock registry).
void ma_task_start_keyboard();

// Магазинные задачи: создаются в funcs.cpp, крутятся в ma_tasks_frame().

// 1) getcord — ведёт авто по очереди координат.
//    (23.10) Исправлен класс бага «точка маршрута теряется»: step() вытаскивал
//    точку из очереди pop-ом, а при «не доехали» уходил в wake=+16мс — точка
//    терялась навсегда. Теперь точка держится (cur_*/has_cur) до касания сферы
//    (семантика старого тела потока getcord: крутиться по точке, пока не доехал).
struct ma_task_getcord : ma_task_base {
    std::queue<float> q;
    const void* p = nullptr;
    float cur_x = 0, cur_y = 0, cur_z = 0;   // (23.10) текущая точка маршрута (не вынутое из q).
    bool has_cur = false;                    // (23.10) точка выбрана и ждёт достижения.
    ma_task_getcord(std::queue<float> q_, const void* p_) : q(std::move(q_)), p(p_) {
        name = "getcord";
        owner = const_cast<void*>(p_);
    }
    bool step() override;
};

// 2) expectations — ждёт, пока модель станет видимой, потом освобождает её.
struct ma_task_expectations : ma_task_base {
    int model = 0;
    CVehicle* car = nullptr;
    int attempts = 0;
    static constexpr int kMaxAttempts = 600;
    ma_task_expectations(int model_, CVehicle* car_) : model(model_), car(car_) {
        name = "expectations";
        owner = car;
    }
    bool step() override;
};

// 3) turncar — вращает авто по оси (switc: 1=x, 2=y, 3=z) со скоростью.
//    (23.10) Хранит СЫРОЙ ключ (lightuserdata) из Lua, а не CVehicle*: авто может быть
//    удалено ПОКА задача крутится (старый detached-поток писал бы в освобождённую
//    память — класс use-after-free). step() решает findcarinpool(p) на игровом
//    потоке; авто нет → задача завершена.
struct ma_task_turncar : ma_task_base {
    const void* p = nullptr;
    float angle = 0.0f;
    float speed = 0.0f;
    int switc = 1;
    ma_task_turncar(const void* p_, float angle_, float speed_, int switc_)
        : p(p_), angle(angle_), speed(speed_ / 360.0f), switc(switc_) {
        name = "turncar";
        owner = const_cast<void*>(p_);
    }
    bool step() override;
};

// 4) turncar_with_delay — то же, но с задержкой между итерациями.
//    (23.10) p — сырой ключ (как в turncar); first_iter задерживается на time_ms,
//    чтобы первый оборот был ПОСЛЕ паузы, как в старом теле turncar_with_delay.
struct ma_task_turncar_delay : ma_task_base {
    const void* p = nullptr;
    float angle = 0.0f;
    int time_ms = 0;
    int switc = 1;
    unsigned long long next_iter = 0;
    ma_task_turncar_delay(const void* p_, float angle_, int time_ms_, int switc_)
        : p(p_), angle(angle_), time_ms(time_ms_), switc(switc_) {
        name = "turncar_delay";
        owner = const_cast<void*>(p_);
    }
    bool step() override;
};

// 6) mission_watch — кооперативная замена std::thread(mission_watch_proc):
//    сторож миссии крутится на игровом потоке раз в 100 мс (wake),
//    проверяет здоровье/арест игрока и флаг миссии. Все вызовы к игре
//    (FindPlayerPed, CMessages::AddBigMessage, destroy) выполняются НА
//    игровом потоке напрямую — без ma_cmdq_defer, без блокировок.
//
//    Флаги stop/running — атомарные (заменяют внешние atomic из funcs.cpp):
//    stop  — true = выйти из цикла (destroy/reload/новый запуск)
//    running — true = сторож активен
//
//    step() возвращает true (завершить) когда:
//      * игрок погиб/арестован (после надписи + сброса флага)
//      * сам по себе сменился flag миссии (mission_flag_get() == false)
//      * stop == true (внешний запрос остановки)
//
//    step() возвращает false (продолжить) при wake = now+100.
struct ma_task_mission_watch : ma_task_base {
    lua_State* L = nullptr;       // Lua-состояние для destroy()
    std::atomic<bool>* stop = nullptr;   // extern в funcs.cpp
    std::atomic<bool>* running = nullptr; // extern в funcs.cpp
    bool done = false;            // true = задача завершилась

    ma_task_mission_watch(lua_State* L_, std::atomic<bool>* stop_, std::atomic<bool>* running_)
        : L(L_), stop(stop_), running(running_) {
        name = "mission_watch";
        owner = L;
    }
    bool step() override;
};

// Фабрика: РОВНО ОДИН экземпляр (23.10 — устраняет двойной-free из док 02):
//   * существующий сторож жив → только обновляет L/running (reuse, как mission_watch_start);
//   * нет — создаёт и регистрирует. Двойное создание = двойное удаление из реестра.
ma_task_mission_watch* ma_task_create_mission_watch(
    lua_State* L, std::atomic<bool>* stop, std::atomic<bool>* running);

// O(1)-доступ к клавиатурной задаче без мьютекса/strcmp (для покадрового пути).
ma_task_keyboard* ma_keyboard();

// Фабрики (создают задачу и регистрируют в реестре). Безопасны из любого потока.
// (23.10) turncar/turncar_delay принимают СЫРОЙ ключ из Lua (не CVehicle*) —
// stale-указатель невозможен: резолв findcarinpool в step() на игровом потоке.
ma_task_getcord* ma_task_create_getcord(std::queue<float> q, const void* p);
ma_task_expectations* ma_task_create_expectations(int model, CVehicle* car);
ma_task_turncar* ma_task_create_turncar(const void* p, float angle, float speed, int switc);
ma_task_turncar_delay* ma_task_create_turncar_delay(const void* p, float angle, int time_ms, int switc);

// Объявления для ma_tasks.cpp (определения — в funcs.cpp).
CVector* get_car_angles_on_axes(CVehicle* car);
int destroy(lua_State* L);

#endif // LUALOADER_MA_TASKS_H
