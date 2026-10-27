// Менеджер потоковой загрузки моделей (2026-10-18). Подробности — ma_streaming.cpp.
//
// Заменяет костыль «REQUEST + LOAD_ALL_MODELS_NOW + опрос HAS_MODEL_LOADED через
// очередь команд»: каждая Command<> с worker-потока — это круг до игрового кадра
// (≥16 мс), и создание сущности с уже загруженной моделью стоило ~6 таких кругов.
//
// Теперь: worker-поток только регистрирует «хочу модель» и ждёт на condition_variable;
// REQUEST-команды шлёт кадровый обработчик ma_stream_frame() на ИГРОВОМ потоке.
// Загрузку ведёт сам движок (CStreaming::Update каждый кадр) — LoadAll не нужен.
//
// Все функции потокобезопасны. В этом TU нет глобальных конструкторов
// (function-local statics) — иначе игра не стартует (см. cmdq_hooks.cpp).

#ifndef LUALOADER_STREAMING_H
#define LUALOADER_STREAMING_H

// Зарегистрировать интерес к модели (+1 refcount). Ноль очереди команд.
// model >= 0 (обычные модели ms_modelInfoPtrs). Отрицательные id (UsedObjectArray)
// менеджер не умеет — их вызывает старый путь Command<>.
void ma_stream_want(int model);

// Снять интерес (-1 refcount). 0 refs => запись удаляется, движку ничего не шлём
// (снятие брони скрипта — отдельно, COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED).
void ma_stream_release(int model);

// Дождаться загрузки. true — модель в памяти; false — невозможно (нет в CD-образе),
// teardown, запись снята или таймаут (только как предохранитель от мёртвого стриминга).
//
// КОНТЕКСТ-ЗАВИСИМОЕ ПОВЕДЕНИЕ (26.10):
//  * НА игровом потоке — СИНХРОННАЯ догрузка: RequestModel/RequestSpecialChar +
//    LoadAllRequestedModels(false) (как SCM-опкод 038B, с Stop/Update таймера).
//    ВАЖНО: lua_yield здесь больше НЕ вызывается. Lua 5.4 завершает C-функцию
//    при возобновлении, если yield был без lua_yieldk: продолжение после yield
//    в C++ не исполняется. Из-за этого Createcar/Createped теряли сущности
//    (лог: 21 wait_start, 14 wait_done) и оставляли незакрытый lock_guard.
//    Вызывающая C-функция теперь гарантированно продолжается после возврата.
//  * С НЕИГРОВОГО потока (legacy-пути) — событийное ожидание на cv.
bool ma_stream_wait(int model, int timeout_ms);

// «Модель объявлена невозможной» (нет в CD-образе, см. ma_stream_frame):
// ожидающие прекращают ждать раньше таймаута. false, если интерес не заведён.
bool ma_stream_impossible(int model);

// Сколько моделей сейчас в реестре «хочу» (heartbeat-диагностика).
unsigned ma_stream_wants();

// Спец-слот (LOAD_SPECIAL_CHARACTER): slot 1-based (1..8 => модель 109..116),
// name — строка как в SCM (SPANDXA, IGMERC...), регистр НЕ меняем.
void ma_stream_want_special(int slot, const char* name);

// --- ДЕРЖАНИЕ МОДЕЛИ СУЩНОСТЬЮ (26.10, директива юзера) ---------------------
// «Модель, под которую создана сущность, остаётся в памяти, пока жива сущность».
// hold — отдельный счётчик (не refs): интерес держится до let_go/let_go_all,
// кадровый обработчик продолжает просить модель, если движок её выгрузил.
// MARK (0249) по такой модели НЕ шлётся — иначе движок получает право выгрузить
// её, пока на ней ещё висят педы/авто/объекты (общий класс heap-AV в msvcrt).
void ma_stream_hold(void* owner, int model);
void ma_stream_hold_special(void* owner, int slot, const char* name);

// Снять ОДНО держание владельца (releasemodel: явная выгрузка скриптом).
void ma_stream_let_go(void* owner, int model);

// Снять ВСЕ держания владельца (destroy(lua_State*) — скрипт сворачивается).
void ma_stream_let_go_all(void* owner);

// Снять ВСЕ держания всех владельцев (cleanstl/полная перезагрузка скриптов).
void ma_stream_let_go_everything();

// Кадровый обработчик: шлёт RequestModel/RequestSpecialChar по всем «хотелкам»
// и обновляет зеркало загруженности. Вызывается ТОЛЬКО на игровом потоке.
void ma_stream_frame();

// Регистрация кадрового хука (Events::gameProcessEvent). Зовётся ИЗ plugin.cpp
// (Message), ПОСЛЕ ma_cmdq_install_hook() — порядок важен: drain очереди первым
// в кадре выставляет id игрового потока. Идемпотентна.
void ma_stream_install_hook();

#endif
