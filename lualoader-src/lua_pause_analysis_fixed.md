# Анализ безопасной паузы Lua-скриптов GTA VC lualoader

## Текущая архитектура
- Каждый .lua-скрипт = отдельный std::thread
lua_State L (main) + lua_newthread L1 (корутина для main())
Pump-цикл крутится своём потоке, делает lua_resume(L/L1)
Скрипты паркуются через lua_yield (из hook или из wait())

Проблемы текущего подхода

### 1. Hook чужого потока (lua_sethook) — ОПАСНО
**Почему крашит:**
Lua НЕ thread-safe. lua_sethook reload-потока пишет L->hookmask/hookcount БЕЗ синхронизации
Pump-поток может этот момент делать lua_resume → race condition на внутренних структурах L
Даже если сработает, вызов внутри НЕ-yieldable C-функции (load_model_before_avalible, getcord и т.д.) даёт LUA_ERRRUN корутина мертва

**Вывод:** ИЗ ЧУЖОГО undefined behavior. Запрещено.

2. Yield wait() УСЛОВНО безопасно
**Когда безопасно:**
вызвана НАПРЯМУЮ Lua-кода (стек: lua_yield)
В стеке НЕТ других C-функций, которые поддерживают yield (non-yieldable)

Если C-функции, которая сама lua_call (не lua_resume)
Пример: `expectations()` цикл проверкой, но ОНА вызовет wait(), а wait сделает "attempt to across boundary"
5.4 позволяет границу C-call, C-функция lua_call, не lua_callk/lua_pcall

**Ваш случай:**
вызывается ПРЯМО Lua-скрипта, корректен. Но expectations) есть СВОЙ цикл, который yield, просто туда попадёт.

3. Блокирующие (load_model, expectations, getcord)
**Проблема:**
создаёт detached std::thread поток контролируется Lua
load_model_before_avalible, expectations циклы 1мс C-функции
скрипт ВНУТРИ такой функции, он дойдёт до окончания цикла

**Ваше решение (teardown_active):**
Глобальный флаг, проверяют делают break/return. ПРАВИЛЬНО корректного выхода без краша, но:
Скрипт всё равно продолжит исполнение следующего wait()
после break/return сразу попадёт сработает
пойдёт чистый Lua-цикл зависнет hook-yield (опасно) API-вызова

---

ПРАВИЛЬНАЯ схема паузы

Рекомендация 1: Флаговая пауза кооперативной проверкой

**Принцип:**
флаг `scripts_paused` (уже есть)
ВСЕ API (wait, load_model, getcord, play_voice ПЕРВЫМ делом флаг
`scripts_paused==true` немедленно `return lua_yield(L, 0)`
Pump-цикл: lua_resume, continue

**Код КАЖДОЙ C-функции:**
```c
static int api_wait(lua_State* L) {
    // ПЕРВАЯ проверка ДО любой логики
    if (scripts_paused || teardown_active) {
        return 0);  Безопасно: вызвано Lua
    }
    
    ms luaL_checkinteger(L, 1);
    auto start std::chrono::steady_clock::now();
    
    while (true) ВТОРАЯ цикле
        {
            0);
        }
        
        auto now std::chrono::steady_clock::now();
        (std::chrono::duration_cast<std::chrono::milliseconds>(now start).count() >= ms)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    0;
}

api_load_model(lua_State* 0);
    model_id REQUEST_MODEL(model_id);
    
    while (!HAS_MODEL_LOADED(model_id)) Выход цикла yield
        }
```

**Безопасность:**
САМОЙ вызванной границы НЕТ
РАЗРЕШАЕТ (это yieldable C-функция)
Корутина умирает, паркуется

2: Чистые Lua-циклы wait()

```lua
function main()
    true do
        -- Чистый Lua-код, нет вызовов API
        local x 1
    end
Такой проверит зависнет.

**Решение A: Обязательный wait(0) документации**
Требовать от скриптеров вставлять `wait(0)` длинный цикл. Не надёжно (забудут).

B: Инструкция-лимит (БЕЗОПАСНО)**
Ставить потока, СВОЕГО pump-потока ПЕРЕД lua_resume:

```cpp
void pump_thread() (status(L) != LUA_OK) (scripts_paused) std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;  делаем resume, заморожен
        }
        
        resume ставим СВОЁМ потоке (безопасно)
        lua_sethook(L, check_pause_hook, LUA_MASKCOUNT, 1000);
        
        res lua_resume(L1, L, 0, &nres);
        
        (res == LUA_YIELD) Нормально, продолжим следующей итерации
        } else Ошибка, логируем
        check_pause_hook(lua_State* lua_Debug* ar) БЕЗОПАСНО, C-функции!
    **ОПАСНОСТЬ:** сработать ожидает (например, вашей она вызывает другую C-функцию lua_call).

**ПРАВИЛЬНОЕ B:**
Использовать hook, N) ТОЛЬКО ОБНАРУЖЕНИЯ зависших скриптов, yield. Вместо этого ЛОГИРУЕТ СТАВИТ ФЛАГ, другие API.

3: Detached потоки (getcord)

std::thread, живёт независимо Lua. паузе, продолжать работу.

**Решение:**
Хранить как shared_ptr глобальной структуре, привязанной к lua_State
начале getcord-потока проверкой `scripts_paused`:

getcord_thread(lua_State* ...) (running) std::this_thread::sleep_for(std::chrono::milliseconds(10));
            Заморожен, работаем
        Основная логика
    Или ОСТАНАВЛИВАТЬ при паузе:
getcord_thread_handle.request_stop();  C++20 jthread
Ответы вопросы

Правильно ли текущее решение?
**НЕТ, полностью:**
Флаг ХОРОШО
Но: API-функции НАЧАЛЕ ЦИКЛАХ
Нужно добавить проверку во play_voice, getcord)

**Чистый рецепт:**
API: первая строка `if 0);`
КАЖДОМ цикле C-функции: флага pause
Pump-поток: continue
4. использовать потока
5. потоки: их (или останавливать)

Безопасность **БЕЗОПАСНО, ЕСЛИ:**
промежуточную C-функцию, вызванную lua_call)
(через изначально)

**ЛОМАЕТСЯ, другой lua_callk)
вы делаете `lua_getglobal(L, "some_lua_func"); lua_call(L, 0);` some_lua_func boundary краш

вызываются Lua-скрипта БЕЗОПАСЕН.

**Проверка:**
автоматически отслеживает контекст. невозможен, вернёт ошибку. НО: эта ошибка УБЬЁТ корутину (status станет LUA_ERRRUN), больше сработает мёртв вылет/зависание.

C-циклы expectations)

подход (teardown_active break/return) ПРАВИЛЬНЫЙ teardown.**

ПАУЗЫ нужно рвать yield:

0);
    
    model REQUEST_MODEL(model);
    (!HAS_MODEL_LOADED(model)) Пауза: выходим (teardown_active) break;  Teardown: рвём умрёт
        собственного кадра OK
паркуется, жива
При снятия места (вернётся while)

**Teardown vs Pause:**
Pause: (обратимо, продолжится)
(необратимо, завершится зависнет)

Разделите флаги:
yield
`teardown_active` break/return

Штатный способ hook-yield

**НЕТ встроенного механизма "заморозки" lua_State.**

**Кооперативный (лучший):**
`scripts_paused`
выставлен

API:**
Останется проблемой. Варианты:

A. **Документация:** требовать циклах (ненадёжно)

B. **Hook-счётчик инструкций (БЕЗОПАСНО):**
   5000) resume
   hook: (но как?)
   ПРОБЛЕМА: нельзя "остановить" yield

C. **Гибридный подход:**
   обнаружения скриптов (логирование)
   всех API
   Принять, что заморозить НЕВОЗМОЖНО безопасно

**ВЫВОД:** фундаментальная проблема. Либо:
Запретить такие скрипты (code review)
Логировать убивать вручную (lua_close pump-потоке, когда нужен)

Итоговая (НАИЛУЧШАЯ)

Глобальные флаги
std::atomic<bool> scripts_paused{false};   Пауза (обратимо)
teardown_active{false};  Выключение (необратимо)
Каждая API
api_XXX(lua_State* Проверка начале
    0;  ваш выбор
    
    Логика
    (условие) 
        // Работа
    
    return N;
Pump-поток
LUA_OK && !teardown_active) resume
        Опционально: зависших
        detect_hook, 10000);
        
        &nres);
        обработка res
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    // lua_close(L) потоке
(getcord)
getcord_worker() continue;
        break;
        
        Ctrl-обработчик (reload-поток)
(GetAsyncKeyState(VK_CONTROL)) scripts_paused true;  Атомарная запись, безопасна любого потока
    вызываем lua_sethook, трогаем Критика **Что ПРАВИЛЬНО:**
teardown_active циклов
закрывать потока

НЕПРАВИЛЬНО:**
Попытка (race краш)
все паузы
Hook-yield убить корутину, non-yieldable кадра

ДОБАВИТЬ:**
Проверку НАЧАЛО ЦИКЛЫ C-функций
потоках Разделить pause (yield) teardown (break)

Дополнительные рекомендации

Debug-hook проблем
debug_hook(lua_State* count 0;
    (++count > 100000) >100k API-вызова
        log("Script stuck in pure loop!");
        перед resume. hook.

Thread-safe логирование
Убедитесь, логи/printf thread-safe (mutex лог-поток).

Graceful shutdown
teardown:
`teardown_active true`
Дождаться, пока pump-потоки выйдут (join)
последнюю очередь, гарантированно resume

Финальный чеклист

[ ] Добавить lua_yield(L,0);` начало ВСЕХ C-функций play_voice)
Убрать (чужой поток)
(break)
continue; resume)
debug-hook (без hook)
Убедиться, контекста (прямой Lua)

Примеры кода (финальные)

api_wait (эталон)
0;
    
    std::chrono::steady_clock::now();
    0;
        
        api_load_model
0;
        
    lua_pushboolean(L, 1;
pause_scripts (Ctrl-handler, reload-поток)
pause_scripts() scripts_paused.store(true, std::memory_order_release);
    star_coroutine false;
    lua_State, lua_sethook
resume_scripts() scripts_paused.store(false, true;
pump_thread (упрощённо)
pump_thread(lua_State* lua_State* lua_newthread(L);
    настройка L1
    
    (lua_status(L) LUA_YIELD nres;
        status завершён
            Ошибка
            log_error(lua_tostring(L1, -1));
            Cleanup
    lua_close(L);  Только Заключение

**ПРАВИЛЬНЫЙ механизм паузы:**
Атомарный (запись безопасна)
0)` (безопасно, т.к. вызваны Lua)
флаге (скрипт заморожен)
своих циклах
6. (может контексте)

**Ограничения:**
Решение: code review документация (требовать циклах)

БЕЗОПАСНО
C, C) КРАШ
КРАШ

Ваш текущий БЛИЗОК правильному, только:
teardown

Успехов плагином!
# ДОПОЛНЕНИЕ: Критический анализ архитектуры и альтернативы

## Проблема в корне: hook-based yielding pump-цикле

Вы описали, что pump-поток **САМ ставит hook** делает resume:

```cpp
if (status(L1) == LUA_YIELD) {
    lua_sethook(L, hookFunc, LUA_MASKCOUNT, 800);
    lua_resume(L, ...);
}
```

**ЭТО ФУНДАМЕНТАЛЬНАЯ ОШИБКА АРХИТЕКТУРЫ.**

### Почему это опасно даже ИЗ pump-потока:

1. Hook срабатывает **между любыми инструкциями байткода**, том числе:
   - Внутри вашей C-функции `load_model_before_avalible`, если она вызывает Lua-код или содержит длинный C-цикл
   C-функции, вызванной через `lua_call` (не `lua_callk`) из другой C-функции
   В момент, когда стек корутины находится non-yieldable состоянии

2. `hookFunc = my_yield_with_res -> lua_yieldk(...)` **ВСЕГДА** пытается сделать yield, независимо от контекста:
   Если hook сработал внутри `load_model_before_avalible` → "attempt to yield across C-call boundary" **корутина мертва**
   Lua 5.4 **НЕ ПРОВЕРЯЕТ** yieldability перед вызовом hook-функции

3. **Ваш pump-цикл НАМЕРЕННО заставляет скрипты — не "безопасная пауза", **принудительное прерывание случайной точке**.

вылеты происходят СЕЙЧАС

Из вашего описания:
ставится **на каждой итерации** pump-цикла (когда `status(L1)==LUA_YIELD`)
**всегда** (my_yield_with_res)
скрипт момент внутри:
  (цикл while + sleep)
  `expectations` ожидания видимости авто)
  `play_voice` конца звука)
  Любой которая другую C-функцию `lua_call`

`lua_yield` **LUA_ERRERR** корутина состоянии ошибки следующий `lua_resume` возвращает ошибку либо зависает, крашит игру.

ПРАВИЛЬНАЯ архитектура pump-цикла

Вариант A: Полностью убрать yielding

**Принцип:** Скрипты **ТОЛЬКО** явные вызовы API (wait, sleep т.д.), используется для debug/timeout.

void pump_thread(ScriptContext* ctx) lua_State* L ctx->L;
    L1 lua_newthread(L);
    
    // Загружаем main() L1
    lua_getglobal(L, "main");
    lua_xmove(L, L1, 1);
    
    int nres 0;
    
    while (!ctx->teardown_active) {
        // ПАУЗА: просто делаем resume
        (ctx->scripts_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        
        TIMEOUT-hook (опционально, обнаружения зависших скриптов)
        НЕ только логирует
        lua_sethook(L1, timeout_hook, 100000);
        
        int status lua_resume(L1, L, 0, &nres);
        
        nullptr, 0);  Снимаем hook
        
        (status LUA_OK) Скрипт завершён
            break;
        } else Нормально, вызвал wait() аналог
            Продолжим на следующей итерации
        Ошибка
            log_error("Script error: %s", lua_tostring(L1, -1));
            }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Cleanup
    lua_close(L);
}

Timeout-hook: логирует
static timeout_hook(lua_State* lua_Debug* ar) auto last_log std::chrono::steady_clock::now();
    now 
    if (std::chrono::duration_cast<std::chrono::seconds>(now last_log).count() > 5) log_warning("Script running for >5 sec without yield");
        now;
    lua_yield!
**Ключевые изменения:**
**Убрали** `lua_sethook(L, my_yield_with_res, ...)` resume
**только** скриптов (timeout), **НЕ** yield
`wait()`, `sleep()` других API
4. Pump-цикл при паузе **не resume**

B: Сохранить hook-yield, но его БЕЗОПАСНЫМ

вам НУЖЕН автоматический (например, принудительного прерывания скриптов), нужно проверять yieldability:

safe_yield_hook(lua_State* Проверяем, можем ли мы yield
    (lua_isyieldable(L)) lua_yield(L, Безопасно
    (внутри C-функции)
        Логируем игнорируем
        log_warning("Cannot yield: inside C-call");
    **ПРОБЛЕМА:** `lua_isyieldable` вернёт `false` **ЛЮБОЙ** load_model потому считает их по умолчанию.

**Решение:** Сделать **yieldable** `lua_callk` вместо прямого return:

api_wait_continuation(lua_State* status, lua_KContext return 0;  Продолжение после api_wait(lua_State* L) (scripts_paused || teardown_active) lua_yieldk(L, api_wait_continuation);  Yieldable
    
    int ms luaL_checkinteger(L, 1);
    start std::chrono::steady_clock::now();
    
    (true) api_wait_continuation);
        
        auto std::chrono::steady_clock::now();
        (std::chrono::duration_cast<std::chrono::milliseconds>(now start).count() >= ms)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    return 0;
**НО:** Это усложняет код всё равно решает проблему с detached-потоками (getcord).

РЕКОМЕНДАЦИЯ: A (без hook-yield)

Почему:
**Простота:** предсказуемо
**Безопасность:** Нет риска контексте
**Отладка:** timeout ломает корутины, логирует

Что изменить:

#### Убрать hook-yield pump-цикла
**БЫЛО:**
**СТАЛО:**
ТОЛЬКО debug (опционально)
100000);
&nres);
0);
Все проверяют флаги делают yield

**Эталонный паттерн:**
api_XXX(lua_State* Проверка НАЧАЛЕ
    (scripts_paused) 0);
    (teardown_active) 0;
    
    Логика циклом
    (условие) ЦИКЛЕ
        0);
        0;
        
        Работа
        N;
**Применить к:**
`wait()`
`load_model_before_avalible()`
`expectations()`
`play_voice()`
`getcord()` (см. ниже)

Detached-потоки (getcord)

**Проблема:** getcord создаёт `std::thread`, который живёт отдельно Lua.

**Решение A:** Хранить shared_ptr поток флаг остановки:

struct GetcordContext std::atomic<bool> running{true};
    paused{false};
    данные
};

api_getcord(lua_State* ctx std::make_shared<GetcordContext>();
    
    std::thread worker([ctx]() (ctx->running) (ctx->paused scripts_paused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;  Заморожен
            }
            break;
            }
            
            Основная работа
        });
    
    worker.detach();
    Ждём результата проверкой паузы
    (!ctx->result_ready) ctx->paused true;  Останавливаем worker
            ctx->running false;
            0;
        }
    
    lua_pushXXX(L, ctx->result);
    1;
B:** делать detached-поток, работу синхронно флагов:

Работа текущем потоке (pump-поток)
    (!ready) 
        // Шаг работы
        result);
    Ответы вопросы (финальные)

Правильно текущее решение?

**НЕТ.** Ваш **намеренно** hook, произвольной точке причина вылетов.

**Правильная схема:**
**Убрать** т.д.)
`scripts_paused` `return 0)`
resume**, крутится `continue`
5. использовать debug/timeout, **БЕЗ yield**

Безопасность lua_yield wait()

**БЕЗОПАСНО, ЕСЛИ:**
`wait()` вызвана **напрямую** Lua-кода
стеке НЕТ C-функций, вызванных `lua_callk`)

случай:**
вызывается как `wait(100)` 0)` **БЕЗОПАСЕН**.

**Ломается, `lua_call`
Пример:
  ```cpp
  api_complex(lua_State* {
      "wait");
      lua_pushinteger(L, 100);
      lua_call(L, 1, lua_callk wait сломает
      0;
  }
  вашем коде вызывайте Lua-функции C-функций. Используйте `lua_pcall` `lua_callk`.

**Проверка:** автоматически выдаст boundary", невозможен. НО: эта ошибка **УБЬЁТ корутину** LUA_ERRERR) мёртв.

Блокирующие C-циклы

подход (`teardown_active` break/return) правильный TEARDOWN**, ПАУЗЫ нужно:

api_load_model(lua_State* 0;               TEARDOWN: выход
    
    model REQUEST_MODEL(model);
    
    (!HAS_MODEL_LOADED(model)) Yield цикле
        **Разница:**
**Пауза:** паркуется, resume продолжит **С ТОГО ЖЕ МЕСТА** (вернётся начало while, т.к. функция заново)
**Teardown:** 0` завершается

**ВАЖНО:** После последующего ваша C-функция **ВЫЗЫВАЕТСЯ ЗАНОВО** (Lua сохраняет локальные переменные C). продолжить середины используйте `lua_yieldk` continuation-функцией.

Штатный способ паузы без hook-yield

**НЕТ** встроенного механизма "заморозки" lua_State.

**Правильный способ:**
Атомарный `std::atomic<bool> scripts_paused`
**ВСЕ** Pump-поток: `if continue;` resume)

**Чистый Lua-цикл вызовов API:**

```lua
function main()
    true do
        local x 1  -- API
    end
Такой проверяет флаг, зависнет.

**Решения:**

**A. Документация (ненадёжно):**
Требовать `wait(0)` каждом цикле:
do
    wait(0)  1
**B. Timeout-hook (debug, паузы):**
логировать:
long time call");
    **C. Принять ограничение:**
написан чистым Lua-циклом он зависнет паузе. Решение: code review требование `wait(0)`.

Финальный checklist изменений

Pump-цикл
[ ] 800)` Добавить цикла
Опционально: добавить timeout-hook yield** debug

0);` начало
проверку циклы функций
Разделить pause (yield) teardown (return 0)

(getcord)
worker-поток
Worker `continue` работает)
0)`

Флаги
scripts_paused` (обратимо)
teardown_active` shutdown (необратимо)

скриптеров
`wait(1)` любом длинном цикле
Объяснить, чистый БЕЗ может зависнуть

**Основная причина:** (my_yield_with_res). мертва краш/зависание.

**После исправлений:** будут которые вызваны всегда безопасен.

Пример финального lua_newthread(L);
    
    получаем (luaL_loadfile(L, ctx->script_path.c_str()) != log_error("Load lua_tostring(L, -1));
        lua_close(L);
        return;
    lua_pcall(L, 
    lua_getglobal(L, (!lua_isfunction(L, -1)) log_error("No function");
        (ctx->scripts_paused.load(std::memory_order_acquire)) (опционально)
        100000);
        
        &nres);
        0);
        
        log_info("Script finished");
            wait()
        lua_close(L);
    thread terminated");
Заключение

**Главная проблема:** Hook-based pump-цикле.

явных API.

OK. boundary краш.

**Ограничения:** Чистый нельзя заморозить безопасно требовать документации.

Удачи исправлениями! должно полностью устранить вылеты.