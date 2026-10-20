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
// Блокирует ТОЛЬКО worker-поток; выход событийный (без опроса через очередь).
bool ma_stream_wait(int model, int timeout_ms);

// Спец-слот (LOAD_SPECIAL_CHARACTER): slot 1-based (1..8 => модель 109..116),
// name — строка как в SCM (SPANDXA, IGMERC...), регистр НЕ меняем.
void ma_stream_want_special(int slot, const char* name);

// Кадровый обработчик: шлёт RequestModel/RequestSpecialChar по всем «хотелкам»
// и обновляет зеркало загруженности. Вызывается ТОЛЬКО на игровом потоке.
void ma_stream_frame();

// Регистрация кадрового хука (Events::gameProcessEvent). Зовётся ИЗ plugin.cpp
// (Message), ПОСЛЕ ma_cmdq_install_hook() — порядок важен: drain очереди первым
// в кадре выставляет id игрового потока. Идемпотентна.
void ma_stream_install_hook();

#endif
