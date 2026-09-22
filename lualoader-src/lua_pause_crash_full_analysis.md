# СВОДНЫЙ АНАЛИЗ: ПАУЗА И ВЫЛЕТ LUA-СКРИПТОВ (GTA VC lualoader, reVC/Wine)

> **Свёрстано из 7 документов** (5 уникальных; `*_fixed.md` — безвозвратно повреждённые
> копии своих оригиналов с `?` на месте cp1252-неопределяемых байтов):
>
> | Документ | Тема | Итерация |
> |----------|------|----------|
> | `lua_freeze_debug.md` | Диагностика ЗАВИСАНИЯ игры при паузе | до крашей |
> | `lua_pause_analysis.md` / `lua_pause_analysis_fixed.md` | Безопасная пауза: правила Lua-корутин, запрет hook-yield | до крашей |
> | `lua_crash_analysis.md` / `lua_crash_analysis_fixed.md` | Детальный анализ 1-го МГНОВЕННОГО вылета (data race) | итерация 1 |
> | `lua_crash_final_analysis.md` | «Невидимый» краш: почему не сработал handler; барьерная пауза | итерация 2 |
> | `lua_pause_crash_problem.md` | ХРОНИКА всех итераций (авторитетный таймлайн) | итерация 3 |
>
> **Состояние на 2026-09-22 09:32.** Плагин `lualoader.asi` (Lua 5.4.9 статика),
> GTA Vice City (reVC), Wine 9+ на Linux. Билд `aed0d44c…` (6567927 байт, 09:31).

---

## 1. РЕЗЮМЕ ТЕКУЩЕГО СОСТОЯНИЯ

**Симптом (неизменён во всех итерациях):** при нажатии **Ctrl** (пауза/отключение всех
Lua-скриптов) игра **мгновенно вылетает**. В `lualoader/trace.txt` последняя запись —
строка запроса/применения паузы, далее обрыв.

**Цель (не поменялась):** Ctrl = поставить ВСЕ Lua-скрипты на паузу (заморозить)
без вылета. Пауза обратимая, без закрытия и переинициализации lua_State.

**Что найдено и устранено:**
1. Блокирующий цикл `while (GetAsyncKeyState(VK_CONTROL)) Sleep(1);` в игровом потоке (зависание).
2. Хук-yield (`lua_sethook(L, hookFunc, LUA_MASKCOUNT, 800)` + принудительный
   `lua_yieldk` из хука) — UB в Lua 5.4, «attempt to yield across a C-call boundary».
   Убран как архитектурное решение.
3. Data race: reload-поток напрямую писал игровую память
   (`CTheScripts::ScriptSpace[OnAMissionFlag] = false`, `CMessages::AddMessageJumpQ`).
   Перенесено в игровой поток через флаги-запросы.
4. Запись `ScriptSpace[OnAMissionFlag]` **в момент паузы убрана полностью**
   (гонка с активными `Command<>` + ломает логику самой игры).
5. Crash-ловушка: `SetUnhandledExceptionFilter` молчал → добавлен
   `AddVectoredExceptionHandler(1, …)` (FIRST=1) — **краш теперь ПОЙМАН**.

**Что поймано (новое, точное):**
```
C++| VECTORED: code=0x40010006 addr=0x7B569767 thread=280        (xN — норм. debug-исключение Wine)
C++| ПАУЗА: барьер — жду выход из активной зоны (активных=0)
C++| ПАУЗА: барьер — все pump-потоки припаркованы (активных=0)
C++| VECTORED: code=0xC0000005 addr=0x7725EFDD thread=448
C++| ПАУЗА: запрос (thread=468), скрипты замораживаются (yield-пауза без teardown)
C++| ПАУЗА: применена в игровом потоке (thread=280), скрипты заморожены
C++| VECTORED: code=0xC0000005 addr=0x7725EFDD thread=460
C++| (множественные VECTORED: code=0xC0000005 addr=0x7725EFDD thread=448/460/464 …)
```

**Ключевые факты:**
1. **Барьер паузы РАБОТАЕТ**: `активных=0`, все pump-потоки припаркованы ДО применения паузы.
2. **Краш — Access Violation 0xC0000005 по ОДНОМУ и тому же адресу 0x7725EFDD**
   на нескольких потоках (448, 460, 464) — pump-потоки/worker'ы.
3. `0x7725EFDD` — область системного модуля Wine (ntdll/kernel32-образная). Либо сюда
   приходят паразитные/псевдоуказатели (общий строковый/аллокационный код Wine),
   либо повреждение кучи/стека системных структур произошло **РАНЬШЕ**, а проявилось
   в момент паузы (симптом — следствие, а не причина).
4. `code=0x40010006` (DBG_PRINTEXCEPTION_C) — нормальное отладочное исключение Wine,
   не краш (игнорируется через `EXCEPTION_CONTINUE_SEARCH`).

**НЕ РЕШЕНО (см. раздел 10):**
- Причина AV `0x7725EFDD` не установлена (нужен модуль/символ/параметры вызова).
- Остаточная гонка `Command<>` из worker-потоков ВНЕ момента паузы (504+ вызова —
  по построению небезопасно; барьер защищает только «во время паузы»).
- Гипотеза «порча до паузы, проявление в момент паузы» не подтверждена/опровергнута.

---

## 2. АРХИТЕКТУРА СИСТЕМЫ

### 2.1 Компоненты
- **Игровой поток (main thread GTA VC/reVC)** — игровой цикл (рендер, физика, скрипты).
  Хук `Message::gameProcessEvent` вызывается каждый кадр; здесь же безопасно менять
  игровую память. При `star_thread` запускает `std::thread(start_lualoder).detach()`
  на каждый скрипт.
- **5 Lua-скриптов**, каждый в отдельном `std::thread` — **pump_owner** (detached worker).
  Внутри: `lua_State L` (main) + `L1 = lua_newthread(L)` (корутина, в неё перемещён
  `main()` через `lua_xmove`). Pump-цикл делает `lua_resume(L1, …)`; скрипт паркуется
  через `lua_yield(L, 0)` из своего `wait()`.
- **Reload-поток** (detached, в трейсах thread=468/472) — ловит edge нажатия/отпускания
  Ctrl (`GetAsyncKeyState(VK_CONTROL) & 0x8000`), ставит атомарные флаги.
- **504+ прямых вызова `Command<COMMAND_*>`** и обращений к игровым структурам
  (CPed/CVehicle) из worker-потоков **параллельно с игровым потоком** — фундаментальный
  риск всей архитектуры.

### 2.2 Окружение
- **reVC** (reverse-engineered GTA VC) под **Wine 9+** на Linux.
- **Lua 5.4.9** статическая сборка.
- Исходники: `lualoader-src/` (`plugin.cpp`, `funcs.cpp`, `funcs.h`), сборка `build.sh`.

### 2.3 Критические игровые адреса (GTA VC 1.0 Windows)
| Сущность | Адрес | Размер |
|----------|-------|--------|
| `CTheScripts::ScriptSpace` | `0x00A0D8A0` | 260512 байт (0xFF …) |
| `CTheScripts::OnAMissionFlag` | `0x00978748` | `unsigned int` — **ИНДЕКС** в ScriptSpace |

Семантика `OnAMissionFlag` (Q7): игра выделяет его SCM-опкодом (03A4, «выделить
переменную ScriptSpace») на старте миссии, проверяет `if (ScriptSpace[OnAMissionFlag])`
для блокировки сохранений/таймера, сбрасывает при завершении. РИСКИ: может содержать 0
(→ запись в `ScriptSpace[0]` затирает чужую переменную) или мусор (вне диапазона);
в reVC расположение/формат могут отличаться — **валидировать при старте**
(`IsBadReadPtr` + проверка `< 260512`). **Итоговое решение: при паузе не трогать.**

### 2.4 Флаги состояния (текущий билд)
```cpp
std::atomic<bool> scripts_paused{false};         // пауза (обратимо) — парковка pump
std::atomic<bool> scripts_disabled{false};       // отключение спавна новых скриптов
std::atomic<bool> teardown_active{false};        // shutdown (необратимо) — break/return
std::atomic<bool> pause_requested{false};        // запрос игровому потоку (применить паузу)
std::atomic<bool> resume_requested{false};       // запрос игровому потоку (сообщить resume)
std::atomic<int>  active_pump_threads{0};        // барьер: в «активной зоне» (lua_resume)
std::mutex         pause_mutex;
std::condition_variable pause_cv;
```

---

## 3. ХРОНОЛОГИЯ ИТЕРАЦИЙ

### Итерация 1 — исходный баг (краш в reload-потоке)
- **Непосредственно** в потоке перезагрузки (thread=472, reload-поток) вызывались:
  ```cpp
  CMessages::AddMessageJumpQ(L"Scripts disabled", 2000, 1);
  CTheScripts::ScriptSpace[OnAMissionFlag] = false;
  ```
- Трейс: `ПАУЗА: вход (thread=472), скрипты замораживаются (yield-пауза без teardown)` → **ОБРЫВ**.
- **Диагноз:** data race — reload-поток писал игровую память (`ScriptSpace`,
  `AddMessageJumpQ`) без синхронизации с игровым потоком.
- Детальный разбор механики — раздел 6.

### Итерация 2 — первый фикс (перенос записей игровой памяти в игровой поток)
Внедрено:
1. **Флаги-запросы** `std::atomic<bool> pause_requested / resume_requested /
   teardown_mission_flag_pending`. Reload/teardown-потоки **перестали** писать игровую
   память — только ставят флаги.
2. Реальные записи (`ScriptSpace[OnAMissionFlag]=false`, `AddMessageJumpQ`) перенесены
   в `Events::gameProcessEvent` — исполняются на игровом потоке каждый кадр.
3. `mission_flag_get()/mission_flag_set_safe()` со **bounds-guard** `OnAMissionFlag < 260512`
   (размер `CTheScripts::ScriptSpace`), продублировано в `setflagmission`,
   `setstatusmission`, `getstatusmission`, `getflagmission`.
4. `load_model_before_avalible` и `expectations` — **кооперативный wait** по
   `scripts_paused` (`if (scripts_paused.load()) { sleep(1ms); continue; }`).
5. Crash handler `SetUnhandledExceptionFilter` — пишет `trace.txt` прямым WinAPI
   (CreateFileA/WriteFile, без CRT-буферов): код/адрес/тип исключения.

**Результат:** трейс стал `ПАУЗА: запрос (thread=468)` → `ПАУЗА: применена в игровом
потоке (thread=280), скрипты заморожены` → **ОБРЫВ**. Вылет остался, а crash handler
**НЕ оставил ни одной строки** `CRASH:` (`grep -c CRASH trace.txt` == 0) — «невидимый»
краш. Глубокий разбор — раздел 7.

### Итерация 3 — текущий фикс (по детальному плану от 2026-09-22 09:26)
Внедрено:
1. **УБРАН источник гонки** — при паузе плагин больше **НЕ пишет**
   `ScriptSpace[OnAMissionFlag]=false`. Обоснование (Q3/Q7):
   - запись в момент активных `Command<>` = data race → мгновенный краш;
   - скрипты останавливаются через `scripts_paused` (yield в `wait()`), флаг миссии им не нужен;
   - сброс «миссии» ломает логику самой ИГРЫ.
2. **Барьерная пауза** — счётчик `active_pump_threads`:
   - pump-потоки `fetch_add(1)` перед активной зоной (`lua_resume`, внутри которой
     вызываются `Command<>`), `fetch_sub(1)` + `pause_cv.notify_all()` после
     (скрипт отдал ход / yield);
   - при `scripts_paused` pump паркуется в цикл (`continue`), счётчик остаётся 0;
   - `pause_scripts()`: `scripts_paused=true` → ждёт через `pause_cv.wait_for(1с)`
     пока `active_pump_threads==0` (лог: `барьер — все припаркованы` / `TIMEOUT`);
   - только **после барьера** игровой поток применяет `AddMessageJumpQ`.
3. **Усиленный ловец краша:**
   - `AddVectoredExceptionHandler(1, lua_vectored_filter)` — FIRST=1, видит исключение
     РАНЬШЕ любых SEH reVC (в т.ч. если reVC перезаписал наш UnhandledExceptionFilter);
   - `std::set_terminate(lua_terminate_handler)` — ловит abort/необработанное C++;
   - `atexit(lua_atexit_handler)` — отличает нормальный `exit()` от краша.

**Результат:** краш ПОЙМАН (см. трейс в разделе 1), барьер работает, но AV жив —
раздел 8.

---

## 4. ЭТАП «ЗАВИСАНИЕ»: диагностика (до крашей)

До перехода к мгновенным вылетам основная проблема была ЗАВИСАНИЕ игры при паузе.
Пять кандидатов по убыванию вероятности:

### 4.1 Причина №1 — `reload()` блокирует игровой поток (95%)
Если `reload()` выполняется в игровом потоке (из обработчика ввода/хука) и содержит
`while (GetAsyncKeyState(VK_CONTROL)) Sleep(1);` — **игровой поток заблокирован циклом**,
игра висит.

**Решение:** неблокирующая edge-проверка каждый кадр ИЗ игрового цикла:
```cpp
static bool ctrl_was_pressed = false;

void check_pause_key() {  // вызывать КАЖДЫЙ КАДР из gameProcessEvent
    bool ctrl_now = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
    if (!ctrl_was_pressed && ctrl_now)      request_pause();   // нажали
    if (ctrl_was_pressed && !ctrl_now)      request_resume();  // отпустили
    ctrl_was_pressed = ctrl_now;
}
```

### 4.2 Причина №2 — скрипт resume'ится синхронно в игровом потоке (70%)
Если `gameProcessEvent` вызывает `lua_resume(L_main, …)` напрямую, а скрипт делает
`wait(1000)` → `this_thread::sleep_for(1000ms)` **в игровом потоке** → игра зависает
на секунду (и дольше).

**Решение:** ВСЕ скрипты выполняются только в отдельных detached-потоках;
`gameProcessEvent` **никогда** не вызывает `lua_resume`. Проверка: логировать
`GetCurrentThreadId()` в `wait()` и сравнить с `g_game_thread_id` (узнать при старте).

### 4.3 Причина №3 — путаница L/L1 в hookFunc (50%)
Было: `lua_sethook(L, hookFunc, LUA_MASKCOUNT, 800)` (хук на **L**) при
`lua_resume(L, …)` (resume **L1**). Проверка `lua_isyieldable(L)` проверяет НЕ тот
state, для которого делается resume; yield из non-yieldable контекста →
LUA_ERRERR → корутина мертва → pump крутится, пытаясь resume мёртвую корутину → зависание.

**Нормальная схема:** L — главный state; `L1 = lua_newthread(L)`; resume `L1`;
хук (если вообще нужен) ставится на `L1` — и только **из своего pump-потока**.

### 4.4 Причина №4 — не все lua_State паузятся (30%)
Несколько скриптов, каждый со своим L, общий флаг `scripts_paused`. Если хотя бы один
скрипт находится в чистом Lua-цикле (без `wait()`) — он НЕ паузится и продолжает
выполняться. Логирование `pump_owner START/END` по L и thread-id показывает, кто жив.

### 4.5 Причина №5 — race на `lua_sethook` (10%)
`lua_sethook` НЕ потокобезопасен: один поток (pump) делает `lua_resume(L)`,
другой (reload) ставит hook → race на внутренних структурах L → краш/зависание.
**Решение:** хуки ставятся только из своего потока, до `lua_resume`.

### 4.6 Правила `lua_close` (критично для teardown)
- `lua_close(L)` — **НЕЛЬЗЯ** вызывать из другого потока, чем тот, где выполнялся
  `lua_resume(L)`.
- **МОЖНО** только ТОГО же потока-владельца (pump_owner), после выхода из цикла.
- Закрывать в состоянии YIELD — **МОЖНО**: `lua_close` освобождает ресурсы независимо
  от статуса корутины. Нужно лишь гарантировать, что НИКТО больше не вызовет
  `lua_resume(L, …)`.

### 4.7 Teardown (необратимый) — эталон
```cpp
std::mutex scripts_mutex;
std::condition_variable scripts_cv;
std::set<lua_State*> active_scripts;

void register_script(lua_State* L)   { std::lock_guard<std::mutex> l(scripts_mutex); active_scripts.insert(L); }
void unregister_script(lua_State* L) { std::lock_guard<std::mutex> l(scripts_mutex); active_scripts.erase(L); scripts_cv.notify_all(); }

// внутри pump_owner: регистрация в начале, разрегистрация в конце;
// lua_close(L) — ТОЛЬКО здесь, своим потоком

void teardown_all_scripts() {
    teardown_active.store(true);
    std::unique_lock<std::mutex> lock(scripts_mutex);
    while (!active_scripts.empty())
        scripts_cv.wait_for(lock, std::chrono::seconds(1));
    // все pump-потоки завершены, lua_State закрыты ими же
}
```

---

## 5. ПРАВИЛА LUA-КОРУТИН И БЕЗОПАСНАЯ ПАУЗА

### 5.1 `lua_sethook` из чужого потока — ЗАПРЕЩЁН (UB)
Lua НЕ thread-safe. `lua_sethook` из reload-потока пишет `L->hookmask/hookcount`
без синхронизации, пока pump-поток делает `lua_resume` → race condition на внутренних
структурах L → краш. **Вывод: хуки ставить только из своего pump-потока; из чужого —
undefined behavior, запрещено.**

### 5.2 Когда `lua_yield` в `wait()` безопасен
**БЕЗОПАСНО, ЕСЛИ:**
- `wait()` вызвана **напрямую** из Lua-кода (стек: `Lua → lua_yield`),
- в стеке **НЕТ** C-функций, не поддерживающих yield (non-yieldable).

**ЛОМАЕТСЯ**, если выше в стеке есть C-функция, вызвавшая `wait()` через `lua_call`
(а не `lua_callk`): yield через C-call boundary → «attempt to yield across a C-call
boundary» → LUA_ERRERR → **корутина мертва**. Lua 5.4 отдаёт эту ошибку, но ошибка
УБИВАЕТ корутину (status = LUA_ERRRUN), дальше — мёртвый resume, вылет/зависание.
Поэтому из C-функций Lua-функции вызывать только `lua_pcall`/`lua_callk`.

**Пример корректного использования:** `wait(100)` прямо из `main()` — безопасно.
Корутина паркуется (LUA_YIELD), стек и байткодовая позиция сохраняются; при следующем
resume продолжится **с того же места** (сразу после `wait()`).

### 5.3 Hook-yield — ФУНДАМЕНТАЛЬНАЯ ОШИБКА АРХИТЕКТУРЫ (устранён)
Было: pump-поток САМ ставил hook перед каждым resume:
```cpp
if (lua_status(L1) == LUA_YIELD) {
    lua_sethook(L, hookFunc, LUA_MASKCOUNT, 800);  // hookFunc → my_yield_with_res → lua_yieldk
    lua_resume(L, ...);
}
```
Почему это опасно **даже из своего pump-потока**:
1. Hook срабатывает **между любыми инструкциями байткода**, в т.ч. внутри C-функций
   с длинными C-циклами (`load_model_before_avalible`, `expectations`, `play_voice`)
   и в C-функциях, вызванных через `lua_call` из другой C-функции.
2. Hook-функция **ВСЕГДА** пыталась сделать `lua_yieldk`, независимо от контекста.
   Lua 5.4 **НЕ ПРОВЕРЯЕТ** yieldability перед вызовом hook-функции.
3. Это **принудительное прерывание в случайной точке**, а не «безопасная пауза».

Итог: скрипт в C-функции → hook → yield across C-call boundary → LUA_ERRERR →
корутина в состоянии ошибки → следующий resume возвращает ошибку или крашит игру.
**Решение: hook-yield УБРАН полностью** (архитектурное решение, см. реестр, п. 9).

### 5.4 Рекомендуемая архитектура pump-цикла (вариант A — принят)
Принцип: скрипты ТОЛЬКО сами делают явные `wait()`; hook — только для
**от обнаружения** зависших (логирование, без yield).

```cpp
void pump_thread(ScriptContext* ctx) {
    lua_State* L  = ctx->L;
    lua_State* L1 = lua_newthread(L);
    lua_getglobal(L, "main");
    lua_xmove(L, L1, 1);

    int nres = 0;
    while (!teardown_active.load() && LUA_OK == lua_status(L)) {
        if (ctx->scripts_paused.load()) {          // ПАУЗА: не resume, просто спим
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        // опционально: timeout-hook ТОЛЬКО для обнаружения зависших (не yield!)
        lua_sethook(L1, timeout_hook, LUA_MASKCOUNT, 100000);
        int status = lua_resume(L1, L, 0, &nres);
        lua_sethook(L1, NULL, 0, 0);               // снять

        if (status == LUA_OK)      break;          // скрипт завершён
        if (status != LUA_YIELD) {  // ошибка
            log_error("Script error: %s", lua_tostring(L1, -1));
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    lua_close(L);   // ТОЛЬКО своим потоком!
}

// timeout-hook: логирует, НЕ делает yield
static void timeout_hook(lua_State* L, lua_Debug* ar) {
    static auto last_log = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - last_log).count() > 5) {
        log_warning("Script running for >5 sec without yield");
        last_log = now;
    }
}
```

Альтернатива (вариант B, НЕ принята): сохранить hook-yield, но безопасным —
проверка `lua_isyieldable(L)` в хуке + сделать C-API yieldable через `lua_yieldk`
с continuation. Проблемы: `lua_isyieldable` вернёт false в любой не-yieldable
C-функции; усложнение кода; не решает проблему detached-потоков (getcord).

### 5.5 Блокирующие C-функции: кооперативный wait
Подход `teardown_active → break/return` в C-циклах — правильный **для TEARDOWN**.
Для **ПАУЗЫ** цикл должен yield'иться, чтобы корутина парковалась и продолжилась:

- **Пауза:** `if (scripts_paused) return lua_yield(L, 0);` — корутина паркуется,
  после resume функция **вызывается заново** (C-локальные переменные НЕ сохраняются),
  вернётся в начало while.
- **Teardown:** `if (teardown_active) return 0;` — функция завершается, скрипт умрёт.
- Если нужно продолжить **с середины** C-функции после yield — только `lua_yieldk`
  с continuation-функцией.

**Эталонный паттерн ВСЕХ C-API** (reконструирован по источникам):
```c
static int api_wait(lua_State* L) {
    // ПЕРВАЯ проверка ДО любой логики
    if (scripts_paused.load() || teardown_active.load())
        return lua_yield(L, 0);          // безопасно: вызвано прямо из Lua-кода

    int ms = luaL_checkinteger(L, 1);
    auto start = std::chrono::steady_clock::now();
    while (true) {
        if (scripts_paused.load())       return lua_yield(L, 0);  // ВТОРАЯ проверка в цикле
        if (teardown_active.load())      return 0;
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= ms)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}

static int api_load_model(lua_State* L) {
    if (scripts_paused.load())           return lua_yield(L, 0);
    if (teardown_active.load())          return 0;

    int model_id = luaL_checkinteger(L, 1);
    REQUEST_MODEL(model_id);
    while (!HAS_MODEL_LOADED(model_id)) {
        if (scripts_paused.load())       return lua_yield(L, 0);
        if (teardown_active.load())      return 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
```

Применить к: `wait`, `load_model_before_avalible`, `expectations`, `play_voice`,
`getcord`, `remove_ped` (проверка в начале) и т.д.

**Важно:** после возврата из блокирующей C-функции (например, модель загружена)
скрипт **ПРОДОЛЖАЕТ ВЫПОЛНЕНИЕ** (следующая инструкция, напр. `create_ped`), которая
ТОЖДЕ должна проверять `scripts_paused`. Окно между `load_model` и `create_ped`
— классическая точка гонок при паузе (см. 7.5).

### 5.6 Чистые Lua-циклы — фундаментальное ограничение
Встроенного механизма «заморозки» lua_State **НЕТ**. Чистый Lua-цикл
(`while true do ... end` без API) проверять `scripts_paused` не будет и зависнет
на паузе. Варианты:
- **A. Документация** (ненадёжно): требовать `wait(0)`/`wait(1)` в любом цикле;
- **B. Timeout-hook** (debug): логировать скрипты >N сек без yield;
- **C. Принять ограничение**: code review + запрет чистых циклов; при обнаружении —
  `lua_close` вручную из pump-потока.

### 5.7 Detached-потоки API (getcord)
`getcord` создаёт `std::thread`, живущий независимо от Lua: при паузе продолжает
работать. Решение: context с атомарными флагами, worker проверяет `scripts_paused`
(паркуется) и `running` (останавливается):
```cpp
struct GetcordContext {
    std::atomic<bool> running{true};
    std::atomic<bool> result_ready{false};
    // данные...
};

static int api_getcord(lua_State* L) {
    auto ctx = std::make_shared<GetcordContext>();
    std::thread worker([ctx]() {
        while (ctx->running.load()) {
            if (scripts_paused.load()) { sleep_for(10ms); continue; } // заморожен
            // основная логика...
            ctx->result_ready.store(true);
            break;
        }
    });
    worker.detach();

    while (!ctx->result_ready.load()) {
        if (teardown_active.load()) {
            ctx->running.store(false);
            ctx->result_ready.store(true);
            return 0;
        }
        if (scripts_paused.load()) lua_yield(L, 0);
        sleep_for(1ms);
    }
    // lua_pushXXX(L, ctx->result);
    return 1;
}
```
Альтернатива: делать работу синхронно в pump-потоке (без detached), шагом с
проверкой флагов.

### 5.8 Lightuserdata и use-after-free (список риска)
C-функции передают Lua сырые указатели (`lua_pushlightuserdata(L, (void*)ped)`).
При паузе/выходе игра может удалить объект (миссия завершена, игрок вышел), а
корутина продолжит держать указатель на **удалённый** объект → при возобновлении
`remove_ped(старый_ped)` — **USE-AFTER-FREE → КРАШ**.

Список подозрительных функций:
- **Критические (работают по указателям):** `create_ped`, `create_spec_ped`,
  `remove_ped`, `spawner` (CVehicle*/CObject*), `autoaim` (цели), `pipir`.
- **Средний риск (читают память):** `getcord`, `mission_watch`, `draw`, `corona`.
- **Низкий риск (не работают с указателями напрямую):** `wait`, `printmessage`,
  `load_model_before_avalible` — но добавить проверку `scripts_paused` для консистентности.

**Правильная защита:**
1. Валидация указателя перед использованием:
   ```cpp
   bool is_valid_ped(CPed* ped) { return CPools::GetPedPool()->IsValidPtr(ped); }
   ```
2. Глобальный `game_shutting_down` — при завершении игры не трогать игровые объекты.
3. (Опционально, рекомендуется) переход на **full userdata** с метатаблицей и
   `__gc`: `PedWrapper { CPed* ped; bool valid; }`, `luaL_setmetatable(L, "PedMeta")`,
   `__gc` делает автоудаление/инвалидацию, `remove_ped` проверяет `wrapper->valid`.

---

## 6. АНАЛИЗ 1-ГО КРАША (итерация 1): data race в reload-потоке

### 6.1 Факты из трейса
```
printmessage
C++| load_model_before_avalible: ЗАПРОС модели id=280
модель id=280 загружена
ПАУЗА: вход (thread=472), скрипты замораживаются (yield-пауза без teardown)
[ОБРЫВ — игра вылетела]
```
1. Скрипты выполнялись (printmessage, load_model успешно завершён).
2. `pump_owner START` НЕТ в трейсе → либо не успел записаться, либо дошёл до этой строки.
3. ПАУЗА записана из **thread=472 (reload-поток)**.
4. После ПАУЗЫ **МГНОВЕННЫЙ** вылет (не зависание, без продолжения).

**Вывод:** вылет происходит **ВНУТРИ** `pause_scripts()` или **СРАЗУ ПОСЛЕ** —
в reload-потоке, в ответ на действия reload-потока.

### 6.2 Последовательность (вероятная)
- **T=0:** скрипт вызывает `load_model_before_avalible(280)` — блокирующий цикл
  `while (!HAS_MODEL_LOADED(280)) { if (teardown_active) break; sleep(1ms); }`.
- **T=100мс:** пользователь жмёт Ctrl; reload-поток ждёт отпускания.
- **T=500мс:** отпускание → reload-поток:
  ```cpp
  scripts_disabled.store(true);          // атомарно, безопасно
  star_thread::set(false);               // (см. 6.4)
  CMessages::AddMessageJumpQ(L"Scripts disabled", 2000, 1);   // ← ОПАСНО (6.3)
  // pause_scripts():
  unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
  CTheScripts::ScriptSpace[OnAMissionFlag] = false;           // ← ОПАСНО (6.2)
  scripts_paused.store(true);
  cpp_tracef("ПАУЗА: вход (thread=%lu) ...");   // ПОСЛЕДНЯЯ СТРОКА ТРЕЙСА
  ```

**КРИТИЧЕСКИЙ МОМЕНТ:** запись `CTheScripts::ScriptSpace[OnAMissionFlag]` из
reload-потока **одновременно** с чтением/записью игрового потока — DATA RACE,
undefined behavior → краш.

### 6.3 Почему `CMessages::AddMessageJumpQ` из чужого потока — опасно
GTA-функции SDK **НЕ потокобезопасны**. `AddMessageJumpQ` меняет внутренние структуры
`CMessages` (массивы, счётчики очереди сообщений), которые игровой поток читает
каждый кадр при рендере. Вызов из reload-потока → race → heap corruption / КРАШ.

### 6.4 Механика гонок на `ScriptSpace`
Игровой поток в этот момент может: читать `ScriptSpace[OnAMissionFlag]` для проверки
активности миссии; записывать соседние ячейки; обрабатывать скриптовые переменные.
Reload-поток БЕЗ СИНХРОНИЗАЦИИ пишет `false`. Возможные исходы:
- **Access Violation** (наиболее вероятно): CPU-кэши рассинхронизированы
  (каждый поток в своём L1), чтение частично записанного значения (torn read);
  движок использует битое значение как индекс → доступ к невалидному адресу.
- **SEH-исключение** в игровом потоке без обработчика → завершение процесса.
- **Heap corruption**, если `ScriptSpace` рядом с heap-структурами: следующий
  `malloc/free` падает позже (поэтому crash может «опоздать»).

Worst case: `OnAMissionFlag` используется как **индекс** другого массива:
`MissionData* data = &missions[ScriptSpace[OnAMissionFlag]]` → битый индекс → AV.

**Почему вылет МГНОВЕННЫЙ, а не зависание:** это нарушение памяти, а не ожидание —
процесс убивается сразу; буфер трейса не успевает дописать строки (буферизация).
Для диагностики — unbuffered I/O: `setvbuf(f, NULL, _IONBF, 0)` или прямой WinAPI.

### 6.5 Pump-потоки в состоянии YIELD во время паузы — НОРМАЛЬНО
Pump-цикл при паузе:
```cpp
while (LUA_OK == lua_status(L) && !lua_state_obsolete(L)) {
    this_thread::sleep_for(chrono::milliseconds(1));
    if (scripts_paused.load()) continue;   // ЗАВИСАЕТ ЗДЕСЬ — это парковка
    ... lua_resume ...
}
```
Корутина в LUA_YIELD, цикл крутится в пустую с `continue`, корутина НЕ resume'ится.
**Это правильно.** Безопасность при этом: не трогать игровую память из чужих потоков;
`lua_State` остаётся валидным (не закрыт/удалён). Риск: принудительное убийство
потоков ОС при выходе игры **внутри** `lua_resume` (например, в C-функции) — краш;
поэтому нужен корректный teardown (раздел 4.7).

### 6.6 Правильная схема связи потоков (результат итерации 1)
**Reload-поток:** только атомарные флаги-запросы, НИКАКОЙ игровой памяти.
**Игровой поток (`gameProcessEvent`):** каждый кадр проверяет флаги и **САМ**
изменяет игровую память.
**Pump-потоки:** паркуются по `scripts_paused`.

```cpp
// reload-поток: только флаги
void request_pause()  { pause_requested.store(true);  cpp_tracef("request_pause: thread=%lu", GetCurrentThreadId()); }
void request_resume() { resume_requested.store(true); cpp_tracef("request_resume: thread=%lu", GetCurrentThreadId()); }

// gameProcessEvent: применение на игровом потоке
if (pause_requested.exchange(false)) {
    // (в итерации 1 здесь была запись ScriptSpace — в итерации 3 УБРАНА, см. 7.3)
    scripts_paused.store(true);
    CMessages::AddMessageJumpQ(L"Scripts paused", 2000, 1);
    cpp_tracef("ПАУЗА: применена (thread=%lu)", GetCurrentThreadId());
}
if (resume_requested.exchange(false)) {
    scripts_paused.store(false);
    CMessages::AddMessageJumpQ(L"Scripts resumed", 2000, 1);
    cpp_tracef("ВОЗОБНОВЛЕНИЕ: применено (thread=%lu)", GetCurrentThreadId());
}
```

### 6.7 Связь с завершением миссии
Наблюдение: краш повторялся после `[миссия swine выполнена] / givemoney +250`.
Гипотеза: после `mission_destroy` (удаление peds/vehicles) корутины продолжают
держать lightuserdata на удалённые объекты → возобновление/следующий вызов API
→ use-after-free. Инвалидация при удалении — через `valid`-флаг (см. 5.8).

### 6.8 Диагностика вылета: что предлагалось на этом этапе
- `SetUnhandledExceptionFilter` с кодом/адресом/тейдом исключения + дамп регистров
  (EAX/EBX/ECX/EDX/ESI/EDI, EIP/ESP/EBP) в `crash_dump.txt`.
- MiniDump: `MiniDumpWriteDump` → `crash.dmp` (dbghelp.lib).
- Маркеры трейса **ДО и ПОСЛЕ** каждой опасной операции (`pause_scripts: ВХОД`,
  `ПЕРЕД записью ScriptSpace`, `ПОСЛЕ ScriptSpace`, `scripts_paused ВЫХОД`) —
  последняя строка покажет точку краша.
- Валидация адресов: `IsBadReadPtr(0x978748, 4)`, проверка индекса
  `OnAMissionFlag < 0x10000/260512`, `IsBadWritePtr(&ScriptSpace[i], sizeof)`.

**Ограничение, обнаруженное позже:** `SetUnhandledExceptionFilter` НЕ сработал
(«невидимый» краш) — полный разбор в разделе 7.

---

## 7. «НЕВИДИМЫЙ» КРАШ (итерация 2): глубокий анализ

### 7.0 Факты
Новый трейс после итерации 2:
```
printmessage
C++| load_model_before_avalible: ЗАПРОС модели id=280
модель id=280 загружена
ПАУЗА: запрос (thread=468), скрипты замораживаются (yield-пауза без teardown)
ПАУЗА: применена в игровом потоке (thread=280), скрипты заморожены
[ОБРЫВ — процесс завершился]
```
Crash handler `SetUnhandledExceptionFilter` **НЕ сработал**:
`grep -c CRASH lualoader/trace.txt` = 0. Ни одной строки `C++| CRASH: код 0x…`.

### 7.1 Q1. Почему `SetUnhandledExceptionFilter` не срабатывает — 6 причин
**Причина 1: reVC устанавливает СВОЙ фильтр ПОСЛЕ нас.**
`SetUnhandledExceptionFilter` возвращает предыдущий фильтр, но цепочки не строит —
**последний вызов побеждает**. Проверка: сохранить наш фильтр, через 1 сек
перечитать текущий, если != наш — логировать «перезаписан».
**Решение:** `AddVectoredExceptionHandler(1, …)` (FIRST=1) — вьекторедный обработчик
встаёт **ПЕРВЫМ** в цепочке и видит исключение раньше любого SEH.

**Причина 2: исключение обработано `__try/__except` ВНУТРИ reVC.**
reVC может обёртывать игровой цикл SEH: `__except (EXCEPTION_EXECUTE_HANDLER)
{ log; ExitProcess(1); }` — падение внутри `__try` перехватывается, наш unhandled
фильтр не вызывается. Проверка: логирование ДО/ПОСЛЕ каждой критической операции в
`gameProcessEvent` — краш между `ПЕРЕД` и `ПОСЛЕ` укажет на SEH reVC.

**Причина 3: Wine трансформирует Windows-исключение в POSIX-сигнал.**
Wine эмулирует SEH через POSIX signals: AV (0xC0000005) → SIGSEGV, stack overflow
(0xC00000FD) → SIGSEGV. Если Wine ставит свой `signal(SIGSEGV, handler)`,
Windows-фильтр может не увидеть. **Решение (Linux-специфично):** собственный
`sigaction(SIGSEGV/SIGABRT/SIGFPE)` с `SA_SIGINFO` + `backtrace_symbols_fd`.

**Причина 4: процесс завершён БЕЗ исключения** — `ExitProcess(code)`,
`std::terminate()`/`abort()` (SIGABRT), `_exit()`. **Решение:**
`std::set_terminate(custom_terminate_handler)` (ловит `std::current_exception`,
пишет, затем `abort()`).

**Причина 5: Fast Fail (0xC0000409, STATUS_STACK_BUFFER_OVERRUN)** — мгновенное
завершение security-митигацией, почти без обработчиков. Лог Wine:
`WINEDEBUG=+seh,+relay` → искать `err:seh:dispatch_exception … BUFFER_OVERRUN`.
Поймать может только vectored (но не всегда).

**Причина 6: убит извне** — OOM killer / SIGKILL. Проверка:
`dmesg | tail -50 | grep -i "killed process"`, `journalctl --since "5 minutes ago" | grep reVC`.

**Комплексное решение (принято):** установить ВСЕ ловцы сразу, каждый пишет свой
файл — по факту появления файла определяется путь:
| Файл | Значит |
|------|--------|
| `crash_vectored.txt` | исключение есть, наш vectored сработал первым |
| `crash_unhandled.txt` | необработанное исключение дошло до фильтра |
| `crash_posix.txt` | Wine: сигнал + backtrace |
| `crash_terminate.txt` | `std::terminate()` |
| `crash_atexit.txt` | нормальный `exit()` |
| НИ ОДНОГО | SIGKILL/OOM/fast fail |

### 7.2 Q2. Пути «падения» фильтра и как отличить
| Путь | Признак |
|------|---------|
| `ExitProcess(code)` | `crash_atexit.txt` — нет, остальных нет; последняя строка трейса — нормальная |
| `abort`/SIGABRT | `crash_posix.txt` (signal=6); в логe Wine `terminate called` |
| SEH внутри reVC | AV внутри `__try` → `ExitProcess(1)`: vectored ЕСТЬ (он ДО SEH), unhandled НЕТ |
| Fast Fail 0xC0000409 | heap/stack corruption; в логе Wine `0xc0000409`; файлов может не быть |
| Hang (зависание) | процесс жив; `gdb -p $(pidof reVC.exe) -batch -ex "thread apply all bt"` |
| OOM killer | `dmesg: Killed process`; crash-файла нет |

Последовательность диагностики: (1) crash-файлы; (2) `wine.log`
(`err:seh:dispatch_exception`, `terminate called`, `0xc0000409`,
`EXCEPTION_ACCESS_VIOLATION`); (3) `dmesg`/`journalctl`; (4) последняя строка
`trace.txt` (где остановились: `ПЕРЕД ScriptSpace` / `ПОСЛЕ ScriptSpace` /
`CMessages` / в pump-потоках).

### 7.3 Q3. Критика дизайна паузы: запись ScriptSpace ДО парковки потоков
Даже на игровом потоке порядок был неверен:
```
T=0: (игровой поток)  ScriptSpace[OnAMissionFlag] = false;
Pump-поток #1:        Command<COMMAND_GIVE_WEAPON_TO_CHAR>(ped, ...) — активен!
T=1: (игровой поток)  движок видит false → освобождает ресурсы миссии
Pump-поток #1:        Command обращается к освобождённому CPed → КРАШ
```
**Вывод:** запись `false` должна происходить **ПОСЛЕ парковки** pump-потоков
(двухфазная схема с барьером, см. 7.4) либо **вообще убрана** — и она убрана:
скрипты останавливаются через `scripts_paused`, флаг миссии им не нужен,
а сброс «миссии» ломает логику самой игры.

### 7.4 Q4. Двухфазная (барьерная) пауза — полная схема
**Фаза 1 — парковка:** `scripts_paused=true`, reload-поток ЖДЁТ (CV, timeout 1с +
retry 500мс), пока все pump-потоки не достигнут безопасной точки (`active_pump_threads==0`).
**Фаза 2 — изменение памяти:** только потом `pause_requested=true` → игровой поток
применяет (`AddMessageJumpQ` и пр.).

```cpp
// Глобальные
std::atomic<int>  active_pump_threads{0};
std::atomic<bool> scripts_paused{false};
std::atomic<bool> pause_requested{false};
std::mutex        pause_mutex;
std::condition_variable pause_cv;

// reload-поток
void request_pause() {
    cpp_tracef("request_pause: установка scripts_paused");
    scripts_paused.store(true);

    // ЖДЁМ, пока все pump-потоки припаркуются
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    std::unique_lock<std::mutex> lock(pause_mutex);
    int iteration = 0;
    while (active_pump_threads.load() > 0) {
        if (pause_cv.wait_until(lock, deadline) == std::cv_status::timeout) {
            cpp_tracef("TIMEOUT #1! Активных потоков: %d, повтор 500мс",
                       active_pump_threads.load());
            deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
            if (++iteration > 1) { cpp_tracef("TIMEOUT #2! Принудительное продолжение"); break; }
        }
    }
    cpp_tracef("request_pause: потоки припаркованы, активных: %d", active_pump_threads.load());

    // Теперь БЕЗОПАСНО запросить изменение игровой памяти
    pause_requested.store(true);
    cpp_tracef("request_pause: ЗАВЕРШЕНИЕ");
}

void request_resume() {
    cpp_tracef("request_resume: НАЧАЛО");
    scripts_paused.store(false);   // pump-потоки сами возобновятся
    resume_requested.store(true);  // игровой поток шлёт сообщение
}

// gameProcessEvent
if (pause_requested.exchange(false)) {
    cpp_tracef("ПАУЗА: применена в игровом потоке (thread=%lu)", GetCurrentThreadId());
    // (запись ScriptSpace УБРАНА — см. Q3)
    CMessages::AddMessageJumpQ(L"Scripts paused", 2000, 1);
}
if (resume_requested.exchange(false)) {
    CMessages::AddMessageJumpQ(L"Scripts resumed", 2000, 1);
}
```

**pump_owner с барьером** (реконструкция по источникам):
```cpp
void pump_owner(lua_State* L, lua_State* L1, int args, bool first_resume) {
    register_script(L);
    active_pump_threads.fetch_add(1);                    // НАЧАЛЬНАЯ РЕГИСТРАЦИЯ
    cpp_tracef("pump_owner START L=%p, активных=%d (thread=%lu)",
               (void*)L, active_pump_threads.load(), GetCurrentThreadId());

    if (first_resume) {
        int status = lua_resume(L, NULL, 0, NULL);      // первый (синхронный) resume
        cpp_tracef("pump_owner: первый resume status=%d", status);
    }

    while (LUA_OK == lua_status(L) && !lua_state_obsolete(L) && !teardown_active.load()) {
        active_pump_threads.fetch_sub(1);               // выход из «активной зоны»

        this_thread::sleep_for(chrono::milliseconds(1));

        if (scripts_paused.load()) {
            pause_cv.notify_all();                      // сообщить ожидающему
            continue;                                   // ПАУЗКА: не resume
        }

        active_pump_threads.fetch_add(1);               // вход в активную зону

        if (L1 != NULL && lua_type(L1, -1) == LUA_TFUNCTION &&
            LUA_YIELD == lua_status(L1) && star_coroutine::get()) {
            for (int i = 1; i <= args; i++) lua_pushvalue(L1, i);
            int nres, status = lua_resume(L1, NULL, 0, &nres);
            if (status != LUA_OK && status != LUA_YIELD) {
                cpp_tracef("pump: ошибка resume status=%d: %s", status, lua_tostring(L1, -1));
                break;
            }
            if (LUA_OK == lua_status(L1) || (L1 && !star_coroutine::get())) break;
            // скрипт в yield — «безопасная зона», декремент на след. итерации
        }
    }

    active_pump_threads.fetch_sub(1);                   // ФИНАЛЬНАЯ РАЗРЕГИСТРАЦИЯ
    script_stopped(L);
    cpp_tracef("pump_owner END L=%p (thread=%lu)", (void*)L, GetCurrentThreadId());

    if (teardown_active.load() || lua_state_obsolete(L)) lua_close(L);  // своим потоком!
    else drop_owner(L);

    unregister_script(L);
}
```
**Подводка из источника:** при неправильном порядке `fetch_sub` счётчик мог уйти
в **отрицательные** (декремент в начале итерации до начального инкремента) —
исправлено начальным `fetch_add` при регистрации.

### 7.5 Q5. Роль `load_model_before_avalible(280)`
Факты: модель 280 (ped, «SWAT») **УСПЕШНО ЗАГРУЖЕНА** (строка в трейсе), краш —
**в момент паузы**, а не во время загрузки. Сценарий:
```lua
load_model_before_avalible(280)  -- завершена
local ped = create_ped(280, x, y, z)   -- следующая инструкция
```
Между `load_model` и `create_ped` пользователь нажимает Ctrl: (в старом коде)
reload-поток пишет `OnAMissionFlag=false`/CMessages, движок обрабатывает сброс
«миссии» и освобождает ресурсы, а pump-поток **ещё дошёл** до
`Command<COMMAND_CREATE_CHAR>` — обращение к освобождённым структурам → КРАШ.
**Решение:** барьерная пауза (7.4) + убранная запись ScriptSpace (7.3): память не
меняется, пока `active_pump_threads != 0`.

### 7.6 Q6. Безопасность 504 вызовов `Command<>` из worker-потоков
**Проблема:** `Command<COMMAND_*>` из worker-потоков одновременно с игровым
потоком (физика, рендер, скрипты) — **НЕТ СИНХРОНИЗАЦИИ. По построению
НЕБЕЗОПАСНО.**

Почему «обычно работает»: многие `Command<>` — простые атомарные операции
(установка 4 байт на x86); пулы CPools могут иметь thread-safe аллокацию.
НО: сложные операции (создание ped, удаление vehicle) изменяют глобальные списки,
аллоцируют память, структуры рендера → data race с игровым потоком → краш.
Барьерная пауза защищает **только «во время паузы»**, не «во время обычной работы».

Стратегии:
1. **Барьерная пауза** (принята) — минимум изменений; не решает гонки в штатном режиме.
2. **Command Queue** (долгосрочно) — worker-потоки складывают запросы в очередь,
   игровой поток каждый кадр выполняет их:
   ```cpp
   struct CommandRequest { CommandId id; std::vector<int> args; };
   std::mutex command_queue_mutex;
   std::queue<CommandRequest> command_queue;

   // funcs.cpp: вместо прямого Command<>
   { std::lock_guard<std::mutex> l(command_queue_mutex);
     command_queue.push(req); }
   // ждём выполнения (lua_yield + опрос уведомления)

   // gameProcessEvent:
   while (!command_queue.empty()) {
       req = command_queue.front(); command_queue.pop();
       // Command<req.id>(...) — ВЫПОЛНЯЕМ ИГРОВОМ ПОТОКЕ
   }
   ```
   Цена: переписать ВСЕ вызовы `Command<>` (504).
3. **Глобальный `std::mutex game_api_mutex`** вокруг каждого `Command<>` +
   в `gameProcessEvent` — простая сериализация, цена: производительность + риск
   дедлока (игровой поток не должен ждать worker'ов).

**Рекомендация:** минимум — барьерная пауза + убрать ScriptSpace + проверки
`scripts_paused` в C-API (СДЕЛАНО); долгосрочно — Command Queue.

### 7.7 Q7. Адрес 0x978748 и семантика `OnAMissionFlag`
- `0x978748` — **глобальная переменная** `CTheScripts::OnAMissionFlag`, тип
  `unsigned int`, семантика: **ИНДЕКС** в `ScriptSpace` (не булево значение!).
- Старт миссии: SCM (опкод 03A4) выделяет переменную и пишет 1.
- **Риски нашего кода:** `OnAMissionFlag==0` (миссии нет) → запись в
  `ScriptSpace[0]` затирает чужую переменную; мусорное значение → вне диапазона;
  reVC может хранить иначе.
- **Валидация при старте** (разово):
  ```cpp
  void validate_game_addresses() {
      unsigned int* OnAMissionFlag = (unsigned int*)0x978748;
      if (IsBadReadPtr((void*)0x978748, sizeof(unsigned int)))
          cpp_tracef("ОШИБКА: 0x978748 НЕВАЛИДЕН!");
      else
          cpp_tracef("OnAMissionFlag: адрес=0x%08X, значение=%u",
                     (DWORD)OnAMissionFlag, *OnAMissionFlag);
      if (*OnAMissionFlag >= 260512)
          cpp_tracef("ПРЕДУПРЕЖДЕНИЕ: %u вне ScriptSpace!", *OnAMissionFlag);
      if (IsBadWritePtr((void*)CTheScripts::ScriptSpace, 260512))
          cpp_tracef("ОШИБКА: ScriptSpace невалиден!");
  }
  ```
- **ИТОГОВАЯ РЕКОМЕНДАЦИЯ: при паузе НЕ ТРОГАТЬ** `OnAMissionFlag`/`ScriptSpace`
  (внедрено в итерации 3).

### 7.8 Итоговый план (по документу итерации 2)
1. **Диагностика (приоритет 1):** все handlers сразу (vectored FIRST=1, unhandled,
   POSIX, terminate, atexit) + детальное логирование ДО/ПОСЛЕ операций +
   `WINEDEBUG=+seh` + dmesg/journalctl.
2. **Исправление (приоритет 2):** барьерная пауза; `scripts_paused`-проверки во
   ВСЕХ C-API; пауза БЕЗ записи в игровую память.
3. **Валидация (приоритет 3):** `IsBadReadPtr`/`IsBadWritePtr`, логирование адресов
   при старте.
4. **Долгосрочно:** Command Queue (выполнение в игровом потоке); mutex для
   критических `Command<>`.

**Ожидаемый итог:** краш либо исчезнет, либо будет ПОЙМАН с точным указанием места.

---

## 8. НАБЛЮДЕНИЯ ПОСЛЕ ИТЕРАЦИИ 3 (сессия 09:32)

Реальный трейс нового билда (маркеры `VECTORED:`, `ПАУЗА: запрос/применена/барьер`):
```
C++| VECTORED: code=0x40010006 addr=0x7B569767 thread=280        (xN раз)
C++| ПАУЗА: барьер — жду выход из активной зоны (активных=0)
C++| ПАУЗА: барьер — все pump-потоки припаркованы (активных=0)
C++| VECTORED: code=0xC0000005 addr=0x7725EFDD thread=448
C++| ПАУЗА: запрос (thread=468), скрипты замораживаются (yield-пауза без teardown)
C++| ПАУЗА: применена в игровом потоке (thread=280), скрипты заморожены
C++| VECTORED: code=0xC0000005 addr=0x7725EFDD thread=460
C++| VECTORED: code=0xC0000005 addr=0x7725EFDD thread=464 ...
```

1. **Vectored-обработчик работает** — в отличие от `SetUnhandledExceptionFilter`,
   краш пойман. Подтверждена гипотеза 7.1: reVC/Wine не давали нашему
   UnhandledExceptionFilter управление (перезаписанный фильтр / `__try/__except`
   в reVC / трансформация в POSIX-сигнал).
2. **Барьер РАБОТАЕТ**: `активных=0`, все pump-потоки припаркованы ДО применения паузы.
3. **Краш — AV 0xC0000005 по одному адресу 0x7725EFDD** на нескольких потоках
   (448, 460, 464).
4. `0x40010006` (DBG_PRINTEXCEPTION_C) в thread=280 — **нормальное** debug-исключение
   Wine, не краш.

**Интерпретация:** 0x7725EFDD — системный модуль Wine (ntdll/kernel32-образная
область), вероятно общий код (строковые/аллокационные паттерны, стековый вызов
из DLL-процедуры), на который приходят паразитные/псевдоуказатели. Проверка
`OnAMissionFlag < 260512` и убранная запись ScriptSpace устранили «классическую»
гонку — но краш жив. Значит: либо (а) осталась гонка игровых данных от `Command<>`
в другом контексте, либо (б) повреждение кучи/стека системных структур случилось
**РАНЬШЕ** и проявилось в момент паузы (точечный симптом в 0x7725EFDD — следствие,
а не причина).

---

## 9. РЕЕСТР ВСЕХ ПРИМЕНЁННЫХ РЕШЕНИЙ (актуальный билд 09:31, sha aed0d44c…)

| # | Решение | Файл/функция | Статус |
|---|---------|--------------|--------|
| 1 | Перенос записи игровой памяти в игровой поток (флаги-запросы `pause_requested`/`resume_requested`) | plugin.cpp `Message`/`pause_scripts`/`reload`/`teardown_all` | внедрено, краш остался |
| 2 | Bounds-guard `OnAMissionFlag < 260512` на все записи/чтения ScriptSpace | funcs.cpp `mission_flag_*`, `setflagmission`, `setstatusmission`, `getstatusmission`, `getflagmission` | внедрено |
| 3 | Убран `ScriptSpace[OnAMissionFlag]=false` ПРИ ПАУЗЕ | plugin.cpp gameProcessEvent (pause-блок) | внедрено (итерация 3) |
| 4 | Барьерная пауза: `active_pump_threads` + `pause_cv`, ожидание активных==0 (1с + retry 500мс) | plugin.cpp `pump_owner`, `pause_scripts` | внедрено; барьер отработал (активных=0), краш остался |
| 5 | `AddMessageJumpQ` — только из игрового потока | plugin.cpp gameProcessEvent | внедрено |
| 6 | Кооперативный wait по `scripts_paused` в блокирующих C-API | funcs.cpp `load_model_before_avalible`, `expectations` | внедрено |
| 7 | `AddVectoredExceptionHandler(1, lua_vectored_filter)` + `SetUnhandledExceptionFilter` + `set_terminate` + `atexit` | plugin.cpp `lua_vectored_filter`, `install_crash_filter` | внедрено; **КРАШ ПОЙМАН** (0xC0000005 @ 0x7725EFDD) |
| 8 | Crash-write прямым WinAPI (CreateFileA/WriteFile) без CRT-буферов | plugin.cpp `lua_crash_write` | внедрено |
| 9 | Запрет hook-yield (UB в Lua 5.4, «attempt to yield across a C-call boundary») | — | архитектурное решение; скрипты паркуются через свой `wait()` |

---

## 10. НЕ РЕШЕНО / ОТКРЫТЫЕ ВОПРОСЫ

1. **Причина AV 0x7725EFDD на потоках 448/460/464 — НЕ установлена.**
   Нужно: под каким модулем адрес (Wine/ntdll?), что за код (сигнатура/символ),
   какой «виновник» передавал параметр в этот системный вызов.
2. **Остаточная гонка самих `Command<>`** между pump-потоками и игровым циклом
   **ВНЕ** момента паузы (существует по построению: 504+ вызова из worker-потоков).
   Барьер защищает только «во время паузы», не «во время обычной работы».
3. **Повреждение до паузы?** Может ли порча памяти происходить ДО паузы и лишь
   проявляться в момент паузы (симптом-следствие)? Для ответа — трейс с
   `pump_owner START/END` и/или проверка кучи (в Wine сложно).
4. **Привязка 0x7725EFDD к модулю** через `WINEDEBUG=+seh,+relay` и/или
   gdb-backtrace всех потоков на момент краша.

---

## 11. СЛЕДУЮЩИЕ ШАГИ (черновик)

1. Определить модуль/функцию по адресу 0x7725EFDD:
   `gdb -p $(pidof reVC.exe) -batch -ex "x/i 0x7725EFDD"` на живом процессе,
   либо лог `WINEDEBUG=+seh`.
2. Понять, ЧТО передаёт каждый поток в 0x7725EFDD (общий буфер/строка/аллокация)
   — это укажет на источник порчи.
3. Подтвердить/опровергнуть «порча до паузы»: контрольная маркировка состояния
   Lua/педов перед паузой (схема «сертификат здоровья»).
4. Если причина — системный вызов при массовой припарковке потоков: парковка через
   1мс-sleep мутирует N потоков слишком близко; альтернатива — полноценный сон
   через `WaitOnAddress` / `std::condition_variable` вместо busy-sleep цикла.
5. (Долгосрочно) Command Queue: выполнить ВСЕ `Command<>` в игровом потоке (7.6).

---

## 12. СПРАВОЧНИК КОДА

### 12.1 Полный набор crash-ловушек (reконструкция по итерации 2/3)
```cpp
#include <windows.h>
#include <dbghelp.h>
#include <execinfo.h>
#include <exception>

// 1) Vectored — ПЕРВЫЙ в цепочке (FIRST=1): видит исключение раньше SEH reVC
LONG WINAPI lua_vectored_filter(EXCEPTION_POINTERS* ex) {
    // запись прямым WinAPI, без CRT-буферов (crash_write)
    HANDLE f = CreateFileA("lualoader/crash_vectored.txt", GENERIC_WRITE, NULL,
                           NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f != INVALID_HANDLE_VALUE) {
        char buf[256];
        int len = snprintf(buf, sizeof(buf),
            "VECTORED: code=0x%08X addr=0x%08X thread=%lu\n",
            ex->ExceptionRecord->ExceptionCode,
            (DWORD)ex->ExceptionRecord->ExceptionAddress,
            GetCurrentThreadId());
        DWORD written; WriteFile(f, buf, len, &written, NULL);
        CloseHandle(f);
    }
    return EXCEPTION_CONTINUE_SEARCH;   // передать следующему обработчику
}

// 2) Unhandled filter — последняя линия (может быть перезаписан reVC!)
LONG WINAPI lua_unhandled_filter(EXCEPTION_POINTERS* ex) {
    // crash_write: "UNHANDLED: code=0x%08X addr=0x%08X thread=%lu\n" ...
    return EXCEPTION_EXECUTE_HANDLER;
}

// 3) POSIX-сигналы (Wine: AV → SIGSEGV) + backtrace
static void lua_posix_handler(int sig, siginfo_t* info, void* uctx) {
    FILE* f = fopen("lualoader/crash_posix.txt", "w");
    if (f) {
        fprintf(f, "POSIX: signal=%d addr=%p\n", sig, info->si_addr);
        void* buffer[128];
        int count = backtrace(buffer, 128);
        backtrace_symbols_fd(buffer, count, fileno(f));
        fclose(f);
    }
    _exit(1);
}

// 4) std::terminate (abort / необработанное C++-исключение)
static void lua_terminate_handler() {
    FILE* f = fopen("lualoader/crash_terminate.txt", "w");
    if (f) { fprintf(f, "STD::TERMINATE thread=%lu\n", GetCurrentThreadId()); fclose(f); }
    try { std::rethrow_exception(std::current_exception()); } catch (...) {}
    abort();
}

// 5) atexit — отличает нормальный exit() от краша
static void lua_atexit_handler() {
    FILE* f = fopen("lualoader/crash_atexit.txt", "w");
    if (f) { fprintf(f, "ATEXIT вызван (нормальный выход)\n"); fclose(f); }
}

void install_crash_filter() {
    AddVectoredExceptionHandler(1, lua_vectored_filter);      // FIRST=1
    SetUnhandledExceptionFilter(lua_unhandled_filter);
    struct sigaction sa{};
    sa.sa_sigaction = lua_posix_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGFPE,  &sa, nullptr);
    std::set_terminate(lua_terminate_handler);
    atexit(lua_atexit_handler);
    cpp_tracef("ВСЕ crash handlers установлены");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        install_crash_filter();
        validate_game_addresses();        // см. 7.7
    }
    return TRUE;
}
```

### 12.2 Барьерная пауза
См. 7.4: `request_pause()` / `request_resume()` / обработка флагов в
`gameProcessEvent` / `pump_owner` с `active_pump_threads`.

### 12.3 Кооперативный C-API
Эталон — 5.5 (`api_wait`, `api_load_model`). Обязательные проверки:
- ПЕРВАЯ строка функции: `if (scripts_paused) return lua_yield(L, 0);`
  `if (teardown_active) return 0;`
- В КАЖДОМ C-цикле: те же проверки перед каждой итерацией.

### 12.4 Teardown
См. 4.7: `register_script`/`unregister_script` + `teardown_all_scripts`
(`teardown_active=true` → ждём `active_scripts.empty()` через CV, timeout 1с);
`lua_close` — только в своём pump-потоке.

### 12.5 Non-blocking Ctrl
См. 4.1 (`check_pause_key` из `gameProcessEvent` каждый кадр, edge-trigger).

### 12.6 Валидация адресов
См. 7.7 (`validate_game_addresses`: IsBadReadPtr 0x978748, индекс < 260512,
IsBadWritePtr ScriptSpace).

### 12.7 Long-term: Command Queue
См. 7.6, стратегия 2.

---

## 13. ФАЙЛЫ-АРТЕФАКТЫ

- `lualoader/trace.txt` — трассировка (накапливается между сессиями; новый билд
  пишет маркеры `VECTORED:`, `ПАУЗА: запрос`, `ПАУЗА: применена`, `ПАУЗА: барьер`).
- Исходники: `lualoader-src/` (`plugin.cpp`, `funcs.cpp`, `funcs.h`), сборка `build.sh`.
- Текущий билд: `../lualoader.asi`, sha256
  `aed0d44ce1e52cc787a4759c94a2228e3b015f6cdbc1f4b454ad8569a710ced8`
  (6567927 байт, 2026-09-22 09:31).
- Исходные 7 документов анализа (оставлены для истории):
  `lua_freeze_debug.md`, `lua_pause_analysis.md`, `lua_pause_analysis_fixed.md`,
  `lua_crash_analysis.md`, `lua_crash_analysis_fixed.md`, `lua_crash_final_analysis.md`,
  `lua_pause_crash_problem.md`.
  **Примечание о кодировке:** оригиналы `lua_crash_analysis.md`,
  `lua_crash_final_analysis.md`, `lua_freeze_debug.md`, `lua_pause_analysis.md`
  повреждены double-encoding (UTF-8 → cp1252 → UTF-8, часть байтов заменена
  C1-контрольными символами); текст полностью восстанавливаем обратным
  преобразованием. Файлы `*_fixed.md` — частичная (безвозвратная, с `?`) попытка
  такого восстановления; их содержимое дублирует оригиналы.
