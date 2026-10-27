// Диагностика вылетов lualoader — ТОЛЬКО чтение/логирование, игру не меняет.
//
// Модуль ловит вылет и отвечает на вопрос «почему упало»:
//   lualoader\log.txt — ЕДИНЫЙ файл. Туда же пишет writelog() из funcs.cpp.
//   В нём: отчёт о вылете (код исключения, адрес, модуль, поток, состояние мьютекса,
//   ПОСЛЕДНИЕ 64 крошки, значения out-параметров команды) и журнал (кто какой
//   Command<> исполнял, сколько ждали модели, какой .lua на каком потоке, зависания).
//
// Подключается отдельной единицей трансляции (diag.cpp) + хуками в
// Plugin-SDK/shared/extensions/ScriptCommands.h под -DLUALOADER_DIAG.
// plugin.cpp не изменяется.

#ifndef LUALOADER_DIAG_H
#define LUALOADER_DIAG_H

#include <string>
#include <windows.h>

namespace ma_diag {

    // Ставит VEH + UnhandledExceptionFilter + журнал + сторожевой поток.
    // Вызывается из Message (plugin.cpp) при загрузке DLL — НЕ из глобального
    // конструктора (иначе игра не стартует: RtlpWaitForCriticalSection).
    void install();

    // Полный отчёт о вылете (определён в diag.cpp): регистры, байты по EIP,
    // крошки Command<>, очередь команд, состояние шрифта/моделей, Lua-контекст,
    // игровой контекст, StackWalk64 + ручной разбор стека, модули, потоки,
    // авто-заключение. Пишет в lualoader\log.txt напрямую (без общего буфера),
    // реентерабелен: отчёт — ОДИН на процесс (g_inHandler). Зовётся из VEH-
    // обработчика (стек ещё здоров) и из unhandled-filter.
    void report(EXCEPTION_POINTERS* ep, const char* how);

// Хлебная крошка: кольцевой буфер в памяти (БЕЗ мьютексов — на пути краша взаимоблокировка
// невозможна) + строка в lualoader\log.txt.
void mark(const char* fmt, ...);

// Вызывается из SDK на входе/выходе КАЖДОЙ Command<>() — покрывает все 497 вызовов
// без правки funcs.cpp. res — строка значений out-параметров ("" если нет).
void cmd_begin(unsigned int commandId, int nargs);
void cmd_end(const char* res);

// Контекст исполняемой команды: адрес thread_local CRunningScript, её m_nIp и база
// скрипт-пространства игры. Только запись в глобальные переменные (без файлов и
// мьютексов) — вызывается из SDK на КАЖДОЙ команде, поэтому обязана быть дешёвой.
// Нужен, чтобы в отчёте о вылете отличить «битый адрес TLS-объекта» от
// «битая база скрипт-пространства» — по обоим вариантам падает memset/ProcessOneCommand.
void cmd_ctx(unsigned int commandId, const void* script, int ip, unsigned int base);

// Имя .lua-потока (для отчёта о краше).
void set_thread_name(const char* name);

// Событие загрузки модели: phase = "wait_start" | "wait_done" | "timeout".
void model_event(int model, const char* phase, int elapsedMs);

// --- Lua-контекст (18.10.2026) ---------------------------------------------
// Отвечает на вопрос «КАКОЙ скрипт и какая строка выполнялись в момент вылета».
// lua_note зовётся обёрткой lua_register (см. funcs.h/register.cpp) на КАЖДЫЙ
// вход из Lua в C-функцию: per-thread «последний API + файл:строка» + крошка.
// Файл НЕ трогается (горячий путь) — только память.
void lua_note(const char* file, int line, const char* api);

// Ошибка lua_pcall (в т.ч. ошибка загрузки/выполнения скрипта). Пишет текст в
// отчёт о вылете (последняя ошибка Lua) + крошку + строку журнала.
void lua_error_note(const char* fmt, ...);

// Короткая строка «lua=файл:строка api=имя» для журнала (lualoader\log.txt; 19.10
// отдельный trace.txt запрещён) ("" если данных нет).
// Без мьютексов и файлового ввода-вывода.
void lua_context(char* out, size_t cap);

// Поток, который сейчас висит в Command<> дольше порога (для watchdog), 0 = нет.
unsigned long worst_stalled_cmd_ms();

// Проверка указателя, полученного от CREATE_*: nullptr и «не похож на адрес» —
// самые частые признаки того, что VM игры отдала мусор.
bool looks_like_pointer(const void* p);

// --- Сторож стиля шрифта (18.10.2026) ---------------------------------------
// Вылет 0x551E0D: PrintChar берёт FontStyle как ИНДЕКС таблицы глифов
// (movsx eax, word[edx*4+0x696D7A], edx = FontStyle*105). Мусорный стиль -> AV.
// Зовётся РАЗ В КАДР с игрового потока (Events::gameProcessEvent, ДО отрисовки).
// Если запрошенный (CFont::Details.m_FontStyle, 0x97F858) или применённый
// (CFont::RenderState.FontStyle, 0x94B924) стиль вне 0..5 — пишет строку FONT:
// в журнал (значения, время, поток, контекст Lua) и обнуляет стиль, чтобы кадр
// отрисовался, а игра не упала. Игру больше ничем не меняет.
void font_style_guard();

// --- Сторож списка радара (19.10.2026) --------------------------------------
// CRadar::DrawRadar (0x4C9F40) в начале кадра двигает глобальный CList
// 0x704858; обнулённый head ([+0xC]) даёт AV 0x4CA523 (mov eax,[ebx+8]).
// Если список уже сконструирован ([+0x40]!=0), а head==0 — вернуть sentinel
// 0x704868 и записать RADAR-LIST в журнал. Зовётся раз в кадр с игрового потока.
void radar_list_guard();

// --- Сторож интро-строк катсцен (18.10.2026, итерация 4 — источник 0x551E0D) --
// CTheScripts::IntroTextLines = 0x7F0EA0, 48 записей tScriptText по 0xF4
// (SDK: struct tScriptText; font — int32 на +0x20, движок читает младшее слово).
// CHud::Draw/DrawAfterFade каждый кадр рисуют строку с text[0]!=0:
// SetFontStyle(font) -> CFont::Details.style -> записи буфера шрифта ->
// RenderFontBuffer -> Size[style][c] -> AV 0x551E0D при стиле вне таблицы.
// Во ВСЕХ вылетах 0x9CDC = младшее слово 0x7F0EC0 (font строки i=0/...).
// Зовётся РАЗ В КАДР с игрового потока (Events::gameProcessEvent, ДО отрисовки):
// первое появление текста в строке и мусорный font пишет в журнал (i, font,
// флаги, координаты, начало текста, контекст Lua), мусорный font заменяет на 1
// (FONT_STANDARD) — кадр отрисуется, игра не упадёт, а журнал покажет первый
// кадр повреждения (по нему вычислим, кто испортил запись).
void intro_text_guard();

// --- Время жизни текста сообщения (18.10.2026) ------------------------------
// CMessages::Add*Message* НЕ копируют строку: игра хранит УКАЗАТЕЛЬ (m_pText) и
// читает его кадры спустя. Раньше буфер getwchat() удалялся сразу после вызова ->
// use-after-free: игра рисовала освобождённую кучу (мусорные символы, «убежавшая»
// строка без нуля -> переполнение буфера состояний шрифта -> мусорный FontStyle
// -> AV 0x551E0D). Теперь буфер живёт time + запас и освобождается на игровом
// потоке в ma_diag::msg_keepalive_purge().
void msg_keepalive(wchar_t* text, unsigned int timeMs);
void msg_keepalive_purge();

// --- Целостность кучи CRT (27.10.2026) --------------------------------------
// _heapchk() обходит кучу msvcrt — ту самую, в которой падает игра
// (heap AV 0x772Fxxxx). Зовётся в контрольных точках (set_skin, destroy) и
// раз в секунду из кадра (tag == nullptr — с троттлингом). Перед проверкой
// кладёт крошку «HEAPCHECK-BEGIN <tag>»: если _heapchk упадёт сам, в отчёте
// будет видно, какая операция проверялась. Ничего не выделяет из кучи.
bool heap_check(const char* tag);

// --- Снимок стека БЕЗ выделения памяти (27.10.2026) -------------------------
// Причина: report() в VEH делает Toolhelp/dbghelp-вызовы, которые сами
// выделяют память; при уже испорченной куче отчёт не дописывался и в журнале
// оставалась одна строка VECTORED без модуля, стека и доступа. Эта функция
// пишет ТОЛЬКО в статический буфер через _snprintf + WriteFile: код и адрес
// исключения, тип доступа (READ/WRITE/EXEC), регистры, байты по EIP, слова
// стека и цепочку EBP с модулем+смещением для каждого адреса.
void dump_stack_noheap(EXCEPTION_POINTERS* ep, const char* how);

// --- Карта модулей (27.10.2026) ---------------------------------------------
// Один раз при старте пишет в журнал «MOD <base> <size> <имя>»: по ней
// абсолютный адрес падения (например 0x772E1C00) переводится в модуль+смещение
// даже если полный отчёт не записался.
void dump_modules_once();

} // namespace ma_diag

// Хуки для Plugin-SDK/shared/extensions/ScriptCommands.h (только при LUALOADER_DIAG).
//
// ВАЖНО: plugin.cpp:246 вызывает Command<COMMAND_CAN_PLAYER_START_MISSION> из
// Events::gameProcessEvent, т.е. НА ИГРОВОМ ПОТОКЕ, каждый кадр. Поэтому игровой
// поток никогда не блокируется на мьютексе: если мьютекс занят worker-потоком,
// команда просто пропускается в этом кадре (ma_diag_enter вернёт false) и будет
// повторена в следующем. Блокировка игрового потока = дедлок и вылет.
bool ma_diag_enter(unsigned int id, int nargs);   // false = команда пропущена
void ma_diag_exit(unsigned int id, const char* res);
unsigned long ma_diag_game_thread();
#endif
#ifdef LUALOADER_CMDQ
#include <functional>
// Очередь команд: вызовы из worker-потоков скриптов выполняются на ИГРОВОМ потоке
// в Events::gameProcessEvent. Определено в compat/diag.cpp.
bool ma_cmdq_on_game_thread();
bool ma_cmdq_defer(std::function<bool()> f);
void ma_cmdq_post(std::function<void()> f);  // fire-and-forget: на игровом потоке, БЕЗ ожидания
void ma_cmdq_drain();      // вызывается из Events::gameProcessEvent (игровой поток)
int  ma_cmdq_pending();    // глубина очереди (для замера; лок уже отпущен)

// Регистрация хука разбора очереди. Зовётся ИЗ plugin.cpp (Message), НЕ из
// глобального конструктора: конструктор в этом TU инициализировался раньше
// plugin.o и лез в неготовый список Events — игра не стартовала
// («blocked by 0000» в RtlpWaitForCriticalSection). Идемпотентна.
void ma_cmdq_install_hook();

#endif // LUALOADER_DIAG_H
