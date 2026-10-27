// Диагностика вылетов lualoader. Подробности — в diag.h.
// Собирается отдельной единицей трансляции, подключается в build.sh (LUALOADER_DIAG).

#include "diag.h"

#include <windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <malloc.h>

#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>
#include <deque>
#include <condition_variable>
#include <functional>
#include "Events.h"
// Игровой контекст в отчёте (18.10.2026): состояние игры, время, игрок, пулы.
// CPools.h тянет за собой типы элементов пулов (CPed/CPlayerPed/CVehicle/
// CAutomobile/CObject/CCutsceneObject) — для разбора адреса падения по пулам.
#include "CGame.h"
#include "CTimer.h"
// Сторож стиля шрифта (18.10.2026): CFont::Details.m_FontStyle / RenderState.FontStyle.
#include "CFont.h"
#include "CWorld.h"
#include "CPlayerInfo.h"
#include "CPools.h"

#pragma comment(lib, "dbghelp.lib")   // для MSVC; при clang линкуем явно в build.sh

// ---------------------------------------------------------------------------
// Хлебные крошки: кольцевой буфер в памяти. Без мьютексов намеренно.
// Если крашится поток, державший глобальный мьютекс (как mtx в writelog),
// обработчик не должен ждать этот мьютекс — иначе он сам зависнет и мы
// потеряем отчёт. Запись — атомарный индекс + memcpy структуры целиком.
// ---------------------------------------------------------------------------
namespace {

const int kCrumbs = 64;
// По требованию: ВСЁ пишем в один файл. Других текстовых файлов не создаём.
const char* kLogPath = "lualoader\\log.txt";

struct Crumb {
    unsigned long long tick;     // GetTickCount64() на момент записи
    unsigned int       cmdId;    // id Command<> либо 0
    unsigned int       threadId; // кто писал
    int                nargs;
    char               text[168];
};

Crumb           g_crumbs[kCrumbs];
std::atomic<int> g_crumbNext{ 0 };

// Текущая (ещё не завершённая) команда — для отчёта «во что именно влетел».
std::atomic<unsigned int> g_inflightCmd{ 0 };
std::atomic<unsigned int> g_inflightThread{ 0 };
std::atomic<unsigned long long> g_inflightSince{ 0 };
// Контекст последней команды (см. ma_diag::cmd_ctx): адрес thread_local CRunningScript,
// её m_nIp и база скрипт-пространства игры. Нужны в отчёте о вылете.
std::atomic<unsigned int> g_scriptAddr{ 0 };
std::atomic<int>          g_scriptIp{ 0 };
std::atomic<unsigned int> g_scriptBase{ 0 };

// Ожидание модели (для watchdog «модель грузится вечно»).
std::atomic<int>  g_modelId{ -1 };
std::atomic<unsigned long long> g_modelSince{ 0 };

// Счётчики по потокам — сколько Command<> исполнил каждый поток-скрипт.
const int kMaxThreads = 64;
std::atomic<unsigned long long> g_cmdCount[kMaxThreads];
std::atomic<unsigned int>       g_cmdTick[kMaxThreads];
char                            g_threadName[kMaxThreads][32];

// --- Lua-контекст по потокам (18.10.2026) ---
// Пишется обёрткой lua_register на КАЖДЫЙ вход из Lua в C-функцию (см. funcs.cpp,
// ma_lua_api_wrap). Отвечает на вопрос «какой скрипт/строка/API выполнялись» в
// момент падения. Только память — никаких файлов на горячем пути.
const int kLuaFileMax = 96;
char g_luaFile[kMaxThreads][kLuaFileMax];   // short_src последнего вызова
int  g_luaLine[kMaxThreads];                // строка в нём (-1 = нет данных)
char g_luaApi[kMaxThreads][40];             // имя Lua-функции (findplayer, ...)
char g_luaErr[256];                         // последняя ошибка Lua (любой поток)

// Одноразовый запуск watchdog-потока.
std::atomic<bool> g_watchdogStarted{ false };
std::atomic<bool> g_quit{ false };
HANDLE           g_diagFile = INVALID_HANDLE_VALUE;
CRITICAL_SECTION g_fileLock;      // только для журнала; отчёт о краше пишем без него
volatile LONG    g_inHandler = 0;
std::atomic<unsigned int>       g_gameThread{ 0 };      // id игрового потока
std::atomic<unsigned int>       g_lockOwner{ 0 };      // кто держит мьютекс Command<>
std::atomic<unsigned long long> g_lockSince{ 0 };
std::atomic<unsigned long long> g_skipped{ 0 };       // сколько раз игровой поток пропустил команду

// Диагностика «процесс умер молча» + контроль вложенности Command<> (07.10.2026).
// Шаблон Command<> в SDK держит ОДИН общий `static CRunningScript script`; если
// команда вложится в команду, внутренний memset затрёт m_nIp внешнего. Считаем
// глубину, чтобы такой случай был виден в журнале.
std::atomic<int>  g_cmdDepth{ 0 };
std::atomic<int>  g_cmdMaxDepth{ 0 };
std::atomic<int>  g_reentrancyHits{ 0 };

// --- ОЧЕРЕДЬ КОМАНД (LUALOADER_CMDQ) ---
// Worker-потоки скриптов кладут сюда замыкания с Command<>() и блокируются.
// Разбирает очередь ma_cmdq_drain(), который вызывается из Events::gameProcessEvent,
// т.е. НА ИГРОВОМ ПОТОКЕ каждое кадро. Так скрипты больше не пишут в игровую память.
struct Req {
    std::function<bool()> fn;
    std::mutex  mx;
    std::condition_variable cv;
    bool done = false;
    bool cancelled = false;   // worker не дождался — команду выполнять нельзя (out-параметр мог умереть)
    bool waiter = false;      // true = Req ждёт worker (удалит сам); false = post (удалит drain)
    bool result = false;
    unsigned long long queuedAt = 0;
};
// ⛔ ОЧЕРЕДЬ КОМАНД — ЛЕНИВАЯ ИНИЦИАЛИЗАЦИЯ (2026-10-07).
//
// БЫЛО: std::mutex g_qMx; std::deque g_q; на уровне пространства имён. У них есть
// конструкторы ⇒ clang генерирует _GLOBAL__sub_I_diag.cpp, который выполняется
// ПЕРВЫМ (порядок динамической инициализации в PE/COFF обратный порядку линковки,
// а diag.o линкуется последним). Его динамик тянет за собой динамик libstdc++
// (pthread_once, локальные буферы errno) — и уже в этот момент другие инициализаторы
// могут лочить секции, которые ещё не подняты. Именно это ловилось как
// «RtlpWaitForCriticalSection ... blocked by 0000» и не давало запустить игру.
// Проверено машинно: после удаления конструктора из этого TU и переноса install()
// в plugin.cpp число инициализаторов должно совпасть с рабочей сборкой 24.09 (30).
//
// СТАЛО: function-local static, создаётся при ПЕРВОМ обращении, то есть уже из
// работающего плагина, когда весь SDK инициализирован. Ровно тот приём, который
// применён в compat/ma_tasks.cpp.
struct CmdQueue {
    std::mutex              mx;
    std::deque<Req*>        q;
    std::mutex              cmdMx;      // мьютекс на все Command<> (g_cmd_mx в SDK)
};
CmdQueue& cmdq() {
    static CmdQueue s;
    return s;
}
std::atomic<bool>       g_hookInstalled{ false };
std::atomic<int>        g_qDepth{ 0 };
std::atomic<unsigned long long> g_qMaxWait{ 0 };
std::atomic<unsigned long long> g_qTotal{ 0 };
std::atomic<unsigned long long> g_qTimeout{ 0 };
const unsigned long long kCmdTimeoutMs = 10000;   // 10 с — иначе скрипт висит вечно

volatile LONG    g_reported = 0;

int slot_of(unsigned tid) {
    int s = (int)(tid % (unsigned)kMaxThreads);
    return s;
}

void append_raw(const char* path, const char* text, DWORD len) {
    HANDLE h = CreateFileA(path, FILE_APPEND_DATA,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                           OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD w = 0;
    WriteFile(h, text, len, &w, nullptr);
    CloseHandle(h);
}

// ---------------------------------------------------------------------------
// БУФЕРИЗОВАННАЯ ЗАПИСЬ ЖУРНАЛА (2026-10-07) — ГЛАВНЫЙ ФИКС МИКРОФРИЗОВ.
//
// БЫЛО: diag_line() на КАЖДУЮ строку делал CreateFileA + WriteFile + CloseHandle.
// Это 3 syscall'а и открытие/закрытие объекта файла на каждой записи. А писать
// приходилось на КАЖДУЮ Command<> (cmd_begin/cmd_end) — а скрипты в кадре
// выполняют сотни команд. Итог: тысячи open/close в секунду = микрофризы.
// Сам комментарий в CRASH_ANALYSIS.md §7.3/§7.8 это уже называл, но правился
// только ma_boot, а diag_line остался.
//
// СТАЛО: один HANDLE держим открытым (режим FILE_APPEND_DATA — ОС сама
// разруливает позицию записи между процессами, поэтому параллельная запись
// ma_boot не портит файл), пишем через буфер в памяти и сбрасываем его
// порциями. Сброс принудительный только там, где без него нельзя:
//   * перед записью отчёта о вылете (flush) — иначе потеряем хвост журнала;
//   * по таймеру (каждые 250 мс) из watchdog — иначе при зависании потеряем;
//   * при выходе процесса.
// Если файл открыть не удалось (нет папки lualoader и т.п.) — тихо работаем
// в буфере, запись не теряется и уйдёт при следующей попытке.
// ---------------------------------------------------------------------------
const int          kLogBufMax = 16 * 1024;
char               g_logBuf[kLogBufMax];
int                g_logLen = 0;
unsigned long long g_lastFlush = 0;
std::atomic<int>   g_logReady{ 0 };

bool log_handle(HANDLE& h) {
    if (h != INVALID_HANDLE_VALUE) return true;
    h = CreateFileA(kLogPath, FILE_APPEND_DATA,
                    FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    g_logReady.store(h != INVALID_HANDLE_VALUE ? 1 : 0);
    return h != INVALID_HANDLE_VALUE;
}

// Вызывать ТОЛЬКО под g_fileLock.
void log_flush_locked() {
    HANDLE h = INVALID_HANDLE_VALUE;
    if (!log_handle(h) || g_logLen <= 0) { g_logLen = 0; return; }
    DWORD w = 0;
    WriteFile(h, g_logBuf, (DWORD)g_logLen, &w, nullptr);
    g_logLen = 0;
    g_lastFlush = GetTickCount64();
}

void diag_line(const char* text) {
    char buf[512];
    int n = _snprintf_s(buf, sizeof(buf), _TRUNCATE, "%s\r\n", text);
    if (n <= 0) return;
    EnterCriticalSection(&g_fileLock);
    // Хвост, который не влез, дописываем после сброса — строка не режется.
    if (g_logLen + n > kLogBufMax) { log_flush_locked(); }
    if (n > kLogBufMax) {
        // Аномально длинная строка — пишем напрямую, в буфер она не влезает.
        HANDLE h = INVALID_HANDLE_VALUE;
        if (log_handle(h)) { DWORD w = 0; WriteFile(h, buf, (DWORD)n, &w, nullptr); }
        LeaveCriticalSection(&g_fileLock);
        return;
    }
    memcpy(g_logBuf + g_logLen, buf, (size_t)n);
    g_logLen += n;
    // Периодический сброс по времени: иначе при ЖЁСТКОМ зависании (поток в
    // игровом цикле, watchdog не дошёл) хвост журнала остался бы в памяти.
    unsigned long long now = GetTickCount64();
    if (!g_lastFlush || now - g_lastFlush >= 250) { log_flush_locked(); }
    LeaveCriticalSection(&g_fileLock);
}

// Принудительный сброс журнала. Зовём из отчёта о вылете (иначе в файле не
// будет последних строк перед падением) и из watchdog по таймеру.
void diag_flush() {
    EnterCriticalSection(&g_fileLock);
    log_flush_locked();
    LeaveCriticalSection(&g_fileLock);
}

void push_crumb(unsigned int cmdId, int nargs, const char* text) {
    Crumb c;
    c.tick = GetTickCount64();
    c.cmdId = cmdId;
    c.nargs = nargs;
    c.threadId = GetCurrentThreadId();
    _snprintf_s(c.text, sizeof(c.text), _TRUNCATE, "%s", text ? text : "");
    g_crumbs[g_crumbNext.fetch_add(1) % kCrumbs] = c;
}

const char* phase_name(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:      return "ACCESS_VIOLATION";
    case EXCEPTION_STACK_OVERFLOW:        return "STACK_OVERFLOW";
    case EXCEPTION_ILLEGAL_INSTRUCTION:  return "ILLEGAL_INSTRUCTION";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "INT_DIVIDE_BY_ZERO";
    case EXCEPTION_DATATYPE_MISALIGNMENT:return "DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:    return "FLT_DIVIDE_BY_ZERO";
    case 0xE06D7363:                      return "C++ EH (throw)";
    case 0x40010006:                      return "DBG_PRINT (Wine, не краш)";
    case EXCEPTION_BREAKPOINT:            return "BREAKPOINT";
    default:                              return "прочее";
    }
}

void write_module_info(char* out, size_t cap, void* addr) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(addr, &mbi, sizeof(mbi)) && mbi.AllocationBase) {
        char mod[MAX_PATH] = "?";
        HMODULE h = (HMODULE)mbi.AllocationBase;
        if (GetModuleFileNameA(h, mod, MAX_PATH)) {
            _snprintf_s(out, cap, _TRUNCATE, "%s +0x%llX", mod,
                        (unsigned long long)((char*)addr - (char*)mbi.AllocationBase));
            return;
        }
    }
    _snprintf_s(out, cap, _TRUNCATE, "модуль неизвестен (%p)", addr);
}

// Принадлежность адреса пулу сущностей — для отчёта «дикая запись в пул»:
// педы (CPool<CPed,CPlayerPed>), авто (CPool<CVehicle,CAutomobile>),
// объекты (CPool<CObject,CCutsceneObject>). Шаг элемента = sizeof(B).
template <typename A, typename B>
bool pool_hit(CPool<A, B>* p, void* addr, int* idx) {
    if (!p || !p->m_pObjects || p->m_nSize <= 0) return false;
    char* lo = (char*)p->m_pObjects;
    char* hi = lo + (size_t)p->m_nSize * sizeof(B);
    char* c = (char*)addr;
    if (c < lo || c + 4 > hi) return false;
    if (idx) *idx = (int)((c - lo) / sizeof(B));
    return true;
}

// Ключевые глобалы шрифта (адреса проверены дизасмом exe — полная карта цепочки):
//   Details.m_FontStyle (0x97F858) — «запрошенный» стиль; слово пишут ТОЛЬКО
//     0x54FFF5 (imm=1), 0x550007 (SetFontStyle, ловушка C) и 0x552437 (imm=0);
//   шаблонный буфер кадра 0x70935C..0x70975C (0x400 байт; шапка записи 0x30 байт:
//     dword[+0], стиль word на +0x2C, текст wchar с +0x30); указатель записи —
//     ОН ЖЕ конец буфера — по адресу 0x70975C;
//   CFont::RenderState (0x94B8F8) — то, что читает PrintChar (+0x2C = FontStyle).
// ВАЖНО: определения — здесь (охватывающий namespace), чтобы ими пользовались
// И report() (ниже), И блок ловушек/DR-точек (вложенный namespace ниже).
const DWORD kTplBase    = 0x70935C;
const DWORD kTplEnd     = 0x70975C;
const DWORD kStyleWord  = 0x94B924;   // RenderState.FontStyle
const DWORD kDetailsSty = 0x97F858;   // Details.m_FontStyle
// 20.10: head сортированного списка CRadar (контейнер 0x704858 — данные CRadar
// сразу за cachedSin/cachedCos 0x70483C/0x704840; голова [+0xC], пусто = sentinel
// 0x704868). AV 0x4CA523 = голова == 0 (писать 0 в head код списка не умеет) —
// ловим аппаратной точкой DR2 писателя нуля.
const DWORD kRadarHead  = 0x704864;

// Обход шаблонного буфера кадра (0x70935C..0x70975C): записи по цепочке
// «шапка 0x30 + текст wchar + терминатор + выравнивание до 4». Печатает стиль
// КАЖДОЙ записи — если мусор сидит в конкретной записи, это видно сразу.
int ft_tpl_walk(char* out, size_t cap) {
    if (!out || cap < 128) return 0;
    out[0] = '\0';
    DWORD dumpPtr = *(volatile const DWORD*)(uintptr_t)kTplEnd;
    int m = _snprintf_s(out, cap, _TRUNCATE,
        "  TPL-цепочка [0x%X..0x%X] указатель записи=0x%X:", kTplBase, kTplEnd, dumpPtr);
    if (m < 0) m = 0;
    if (dumpPtr <= kTplBase) {
        return m + _snprintf_s(out + m, cap - m, _TRUNCATE, " пусто/сброшен");
    }
    if (dumpPtr > kTplEnd) dumpPtr = kTplEnd;
    DWORD p = kTplBase;
    int k = 0;
    while (p + 0x30 <= kTplEnd && p < dumpPtr && k < 24 && m < (int)cap - 400) {
        const unsigned char* e = (const unsigned char*)(uintptr_t)p;
        unsigned h0 = *(volatile const unsigned int*)e;
        unsigned st = *(volatile const unsigned short*)(e + 0x2C);
        const unsigned short* t = (const unsigned short*)(e + 0x30);
        size_t maxw = (size_t)((kTplEnd - p - 0x30) / 2);
        if (maxw > 80) maxw = 80;
        char txt[128]; txt[0] = 0; int tn = 0;
        size_t wl = 0;
        for (; wl < maxw; ++wl) {
            unsigned short wc = t[wl];
            if (!wc) break;
            if (tn > (int)sizeof(txt) - 12) break;
            if (wc >= 32 && wc <= 0x7E)
                tn += _snprintf_s(txt + tn, sizeof(txt) - tn, _TRUNCATE, "%c", (char)wc);
            else
                tn += _snprintf_s(txt + tn, sizeof(txt) - tn, _TRUNCATE, "\\u%04X", (unsigned)wc);
        }
        int w = _snprintf_s(out + m, cap - m, _TRUNCATE,
            "\r\n    #%d @0x%X h0=%08X style=0x%04X text(len=%u)='%s'",
            k, p, h0, st, (unsigned)wl, txt);
        if (w < 0) break;
        m += w;
        if (st > 5) {
            w = _snprintf_s(out + m, cap - m, _TRUNCATE, " <-- МУСОРНЫЙ СТИЛЬ!");
            if (w > 0) m += w;
        }
        DWORD next = p + 0x30 + 2 * ((DWORD)wl + 1);
        next = (next + 3u) & ~3u;
        if (next <= p) break;
        p = next;
        ++k;
    }
    if (k == 0) {
        int w = _snprintf_s(out + m, cap - m, _TRUNCATE, " (записей нет)");
        if (w > 0) m += w;
    }
    return m;
}

void report(EXCEPTION_POINTERS* ep, const char* how) {
    // Пишем всегда, даже если уже что-то писали: второй отчёт не хуже первого,
    // а thread_local реентерабельность защищает от рекурсии.
    if (InterlockedExchange(&g_inHandler, 1) != 0) return;
    if (!ep || !ep->ExceptionRecord) { InterlockedExchange(&g_inHandler, 0); return; }

    // (27.10.2026) ИСХОДНЫЙ контекст упавшего потока. StackWalk64 ниже МЕНЯЕТ
    // ctx (Esp/Ebp/Eip двигаются по кадрам), поэтому запоминаем значения ДО
    // обхода. Раньше дамп стека брал ctx.Esp ПОСЛЕ StackWalk64 — на Wine там
    // оказывался мусор (наблюдали Esp=0xFFFFFFE8), и report() падал на чтении
    // [0xFFFFFFE8]. Именно эти падения и были видны в журнале как «AV в msvcrt»
    // (0x772E1C00/0x772F1AB0 — на самом деле report()+0xF50/+0xE00): первичное
    // исключение до журнала не доходило.
    const DWORD origEip = ep->ContextRecord ? (DWORD)ep->ContextRecord->Eip : 0;
    const DWORD origEsp = ep->ContextRecord ? (DWORD)ep->ContextRecord->Esp : 0;
    const DWORD origEbp = ep->ContextRecord ? (DWORD)ep->ContextRecord->Ebp : 0;

    // Буфер большой: в отчёт теперь попадают регистры, стек до 40 кадров, список
    // модулей и список потоков. Раньше было 4096 — не хватало, отчёт обрезался.
    char buf[32768];
    int n = 0;
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    void* addr = ep->ExceptionRecord->ExceptionAddress;

    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\n================ LUALOADER CRASH (%s) ================\r\n"
        "момент        : %llu\r\n"
        "поток         : %lu\r\n"
        "код           : 0x%08lX (%s)\r\n"
        "адрес         : %p\r\n",
        how, (unsigned long long)GetTickCount64(), GetCurrentThreadId(),
        (unsigned long)code, phase_name(code), addr);

    if (code == EXCEPTION_ACCESS_VIOLATION) {
        // ВАЖНО (19.10.2026): раньше тут стояло `NumberParameters ? "запись" : "чтение"` —
        // для AV NumberParameters ВСЕГДА 2, поэтому каждая ошибка печаталась как «запись»
        // (ложная метка у читающего movsx 0x551E0D вводила в заблуждение). Истинная
        // операция — ExceptionInformation[0]: 0 = чтение, 1 = запись, 8 = исполнение.
        const ULONG_PTR op = ep->ExceptionRecord->NumberParameters > 0
            ? ep->ExceptionRecord->ExceptionInformation[0] : (ULONG_PTR)-1;
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "операция      : %s, адрес данных %p\r\n",
            op == 0 ? "чтение" : op == 1 ? "запись" : op == 8 ? "исполнение" : "? (нет данных)",
            ep->ExceptionRecord->NumberParameters > 1
                ? (void*)ep->ExceptionRecord->ExceptionInformation[1] : nullptr);

        // ---- РАЗБОР АДРЕСА ДАННЫХ (что это за память) -------------------
        // Отвечает на «почему именно сюда нельзя было писать/читать»: свободная
        // страница (дикий указатель/порча кучи), RESERVE (нулевые страницы), или
        // адрес внутри ПУЛА СУЩНОСТЕЙ (обращение к удалённому педу/авто/объекту).
        if (ep->ExceptionRecord->NumberParameters > 1) {
            void* fa = (void*)ep->ExceptionRecord->ExceptionInformation[1];
            MEMORY_BASIC_INFORMATION fq;
            if (VirtualQuery(fa, &fq, sizeof(fq))) {
                const char* st = fq.State == MEM_COMMIT ? "ЗАНЯТА (COMMIT)"
                               : fq.State == MEM_RESERVE ? "РЕЗЕРВ (нулевые страницы)"
                               : "СВОБОДНА (FREE!)";
                const char* pr = (fq.Protect & PAGE_NOACCESS) ? "NOACCESS"
                               : (fq.Protect & PAGE_EXECUTE) ? "EXECUTE..."
                               : (fq.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE)) ? "R/W"
                               : (fq.Protect & (PAGE_READONLY | PAGE_EXECUTE_READ)) ? "R/O"
                               : "?";
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "адрес данных  : %p — %s, protect=0x%lX (%s)\r\n",
                    fa, st, (unsigned long)fq.Protect, pr);
            } else {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "адрес данных  : %p — ВНЕ адресного пространства\r\n", fa);
            }
            // Пулы: индекс в пуле — прямой признак «обращение к удалённой сущности».
            char pool_name[64];
            pool_name[0] = '\0';
            int pidx = -1;
            if (pool_hit(CPools::ms_pPedPool, fa, &pidx))
                _snprintf_s(pool_name, sizeof(pool_name), _TRUNCATE, "ПУЛ ПЕДОВ (индекс %d)", pidx);
            else if (pool_hit(CPools::ms_pVehiclePool, fa, &pidx))
                _snprintf_s(pool_name, sizeof(pool_name), _TRUNCATE, "ПУЛ АВТО (индекс %d)", pidx);
            else if (pool_hit(CPools::ms_pObjectPool, fa, &pidx))
                _snprintf_s(pool_name, sizeof(pool_name), _TRUNCATE, "ПУЛ ОБЪЕКТОВ (индекс %d)", pidx);
            if (pool_name[0])
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "              : адрес ВНУТРИ %s — типичный признак обращения\r\n"
                    "                к УДАЛЁННОЙ сущности (use-after-free в пуле)\r\n", pool_name);
        }
    }
    char mod[300];
    write_module_info(mod, sizeof(mod), addr);
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "в модуле      : %s\r\n", mod);

    // ---- РЕГИСТРЫ УПАВШЕГО ПОТОКА ------------------------------------
    // Без них половина диагностики невозможна: по одним коду и адресу нельзя
    // понять, что именно лежало в eax/ecx (например, указатель на модель/состояние).
    if (ep->ContextRecord) {
        const CONTEXT& c = *ep->ContextRecord;
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nРЕГИСТРЫ: eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX\r\n"
            "            esi=%08lX edi=%08lX ebp=%08lX esp=%08lX\r\n"
            "            EIP=%08lX  EFLAGS=%08lX  сегмент ошибки=%04lX\r\n",
            (unsigned long)c.Eax, (unsigned long)c.Ebx, (unsigned long)c.Ecx, (unsigned long)c.Edx,
            (unsigned long)c.Esi, (unsigned long)c.Edi, (unsigned long)c.Ebp, (unsigned long)c.Esp,
            (unsigned long)c.Eip, (unsigned long)c.EFlags,
            (unsigned long)(ep->ExceptionRecord->ExceptionInformation[1] == 1 ? c.SegSs : c.SegDs));
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  байты по EIP: %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[0] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[1] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[2] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[3] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[4] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[5] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[6] : 0,
            ep->ContextRecord && ep->ContextRecord->Eip < 0x7FFFFFFFu
                ? (unsigned)((const unsigned char*)ep->ContextRecord->Eip)[7] : 0);
    }

    unsigned long long since = g_inflightSince.load();
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\nВЛОЖЕННОСТЬ Command<>: глубина=%d, макс=%d, случаев=%d\r\n"
        "  (макс>1 = команда вложилась в команду; общий static CRunningScript затирается)\r\n"
        "\r\nОЧЕРЕДЬ КОМАНД: глубина=%d, всего=%llu, макс.ожидание=%llu мс, таймаутов=%llu\r\n",
        g_cmdDepth.load(), g_cmdMaxDepth.load(), g_reentrancyHits.load(),
        g_qDepth.load(), g_qTotal.load(), g_qMaxWait.load(), g_qTimeout.load());
    {
        unsigned lo = g_lockOwner.load();
        unsigned long long ls = g_lockSince.load();
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nМЬЮТЕКС Command<>: %s\r\n  игровой поток   : %lu\r\n  пропущено им   : %llu\r\n",
            lo ? "ЗАНЯТ" : "свободен", g_gameThread.load(), g_skipped.load());
        if (lo) n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  держит поток   : %lu (уже %llu мс)\r\n", lo,
            ls ? GetTickCount64() - ls : 0);
    }
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\nПОСЛЕДНЯЯ КОМАНДА (если в момент краша была в работе):\r\n"
        "  id           : 0x%X\r\n  поток        : %lu\r\n  висит (мс)   : %llu\r\n",
        g_inflightCmd.load(), g_inflightThread.load(),
        since ? GetTickCount64() - since : 0);

    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\nСОСТОЯНИЕ МОДЕЛИ:\r\n  id           : %d\r\n  ждёт (мс)    : %llu\r\n",
        g_modelId.load(), g_modelSince.load() ? GetTickCount64() - g_modelSince.load() : 0);

    // ---- СОСТОЯНИЕ ШРИФТА (CFont) ----------------------------------------
    // Цепочка вылета 0x551E0D: Details.m_FontStyle (0x97F858) → шаблонный буфер
    // кадра (0x70935C..0x70975C; шапка записи 0x30 байт, стиль на +0x2C, текст
    // wchar с +0x30) → CFont::RenderState.FontStyle (0x94B924), который читает
    // PrintChar. Здесь — снимок всей цепочки на момент краха: сразу видно, ГДЕ
    // сидит мусор (доказанный 0x9CDC) и какой текст при этом рисовался.
    {
        const unsigned char* rs = (const unsigned char*)(uintptr_t)0x94B8F8;
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nСОСТОЯНИЕ ШРИФТА (CFont):\r\n"
            "  want = Details.m_FontStyle   [0x97F858] = %d (0x%04X)\r\n"
            "  used = RenderState.FontStyle [0x94B924] = %d (0x%04X)\r\n"
            "  tplPtr[0x70975C]=0x%X  tpl0+0x2C[0x709388]=%d (0x%04X)  0x97F860=%u 0x97F823=0x%02X\r\n",
            (int)*(const short*)(uintptr_t)0x97F858, (unsigned)*(const unsigned short*)(uintptr_t)0x97F858,
            (int)*(const short*)(uintptr_t)0x94B924, (unsigned)*(const unsigned short*)(uintptr_t)0x94B924,
            (unsigned)*(const unsigned long*)(uintptr_t)0x70975C,
            (int)*(const short*)(uintptr_t)0x709388, (unsigned)*(const unsigned short*)(uintptr_t)0x709388,
            (unsigned)*(const unsigned short*)(uintptr_t)0x97F860,
            (unsigned)*(const unsigned char*)(uintptr_t)0x97F823);
        if (n < (int)sizeof(buf) - 300) {
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "  RenderState hex (0x30): ");
            for (int i = 0; i < 0x30 && n < (int)sizeof(buf) - 8; i++)
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "%02X", rs[i]);
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "\r\n");
        }
        if (n < (int)sizeof(buf) - 6600) {
            char tpl[6400];
            ft_tpl_walk(tpl, sizeof(tpl));
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "%s\r\n", tpl);
        }
        if (ep->ContextRecord) {
            DWORD esi = ep->ContextRecord->Esi;
            DWORD ebp = ep->ContextRecord->Ebp;
            if ((esi >= 0x70935C && esi < 0x70975C) || (ebp >= 0x70935C && ebp < 0x70975C))
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  ESI=0x%X EBP=0x%X — ВНУТРИ шаблонного буфера (рисуемый текст/шапка)\r\n",
                    esi, ebp);
        }
    }

    // ---- LUA-КОНТЕКСТ (КАКОЙ скрипт и строка выполнялись) ----------------
    // Заполняется обёрткой lua_register (ma_lua_api_wrap в funcs.cpp): последний
    // вход из Lua в C-функцию этого потока. Главный ответ на «что делал скрипт».
    {
        int s = slot_of(GetCurrentThreadId());
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nLUA-КОНТЕКСТ УПАВШЕГО ПОТОКА:\r\n"
            "  скрипт        : %s\r\n"
            "  строка        : %d\r\n"
            "  последний API : %s\r\n"
            "  последняя ошибка Lua (любой поток): %s\r\n",
            g_luaFile[s][0] ? g_luaFile[s] : "(нет данных — поток не вызывал Lua-API)",
            g_luaLine[s], g_luaApi[s][0] ? g_luaApi[s] : "-",
            g_luaErr[0] ? g_luaErr : "-");
    }

    // ---- ИГРОВОЙ КОНТЕКСТ -------------------------------------------------
    // Где был игрок и в каком состоянии игра — половина «почему» без него не видна.
    {
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nИГРОВОЙ КОНТЕКСТ:\r\n"
            "  gGameState    : %d (9 = в игре)\r\n"
            "  игровое время : %d мс\r\n",
            gGameState, CTimer::m_snTimeInMilliseconds);
        CPlayerPed* pl = CWorld::Players ? CWorld::Players[CWorld::PlayerInFocus].m_pPed : nullptr;
        if (pl) {
            MEMORY_BASIC_INFORMATION pmi;
            if (VirtualQuery((void*)pl, &pmi, sizeof(pmi)) && pmi.State == MEM_COMMIT) {
                CVector& pp = pl->GetPosition();
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  игрок         : %p модель=%d поз=(%.1f %.1f %.1f) в авто=%d\r\n",
                    (void*)pl, (int)pl->m_nModelIndex, pp.x, pp.y, pp.z, (int)pl->m_bInVehicle);
            } else {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  игрок         : %p (страница не COMMIT — не читаем)\r\n", (void*)pl);
            }
        } else {
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "  игрок         : (нет)\r\n");
        }
    }

    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\nКОНТЕКСТ КОМАНДЫ (адрес thread_local CRunningScript):\r\n"
        "  script       : %p\r\n  m_nIp        : %d (0x%X)\r\n"
        "  база скрипта: %p\r\n  итого адрес  : %p\r\n",
        (void*)g_scriptAddr.load(), g_scriptIp.load(), (unsigned)g_scriptIp.load(),
        (void*)g_scriptBase.load(),
        (void*)(unsigned long)(g_scriptBase.load() + (unsigned)g_scriptIp.load()));

    // ---- BACKTRACE УПАВШЕГО ПОТОКА ------------------------------------
    // ВАЖНО: CaptureStackBackTrace() здесь даёт стек САМОГО ОБРАБОТЧИКА (мы стоим
    // внутри report()), а не стек упавшего потока — такой вывод бесполезен.
    // Поэтому идём по ContextRecord из EXCEPTION_POINTERS через StackWalk64:
    // это единственный способ увидеть настоящую цепочку вызовов.
    {
        static LONG symReady = 0;
        if (!InterlockedCompareExchange(&symReady, 1, 0)) {
            SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
            SymInitialize(GetCurrentProcess(), NULL, TRUE);
        }
        CONTEXT ctx; ZeroMemory(&ctx, sizeof(ctx));
        if (ep->ContextRecord) ctx = *ep->ContextRecord; // копия: StackWalk64 портит контекст
        STACKFRAME64 sf; ZeroMemory(&sf, sizeof(sf));
        sf.AddrPC.Offset   = ctx.Eip;
        sf.AddrPC.Mode     = AddrModeFlat;
        sf.AddrStack.Offset = ctx.Esp;
        sf.AddrStack.Mode  = AddrModeFlat;
        sf.AddrFrame.Offset = ctx.Ebp;
        sf.AddrFrame.Mode  = AddrModeFlat;
        DWORD64 disp = 0;
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nСТЕК УПАВШЕГО ПОТОКА (EIP=%08lX ESP=%08lX):\r\n",
            (unsigned long)ctx.Eip, (unsigned long)ctx.Esp);
        int fi = 0;
        for (; fi < 40 && n < (int)sizeof(buf) - 400; fi++) {
            if (!StackWalk64(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(),
                             GetCurrentThread(), &sf, &ctx, NULL,
                             SymFunctionTableAccess64, SymGetModuleBase64, NULL))
                break;
            if (!sf.AddrPC.Offset) break;
            char mi[220];
            write_module_info(mi, sizeof(mi), (void*)sf.AddrPC.Offset);
            // Имя функции + ФАЙЛ:СТРОКА из PDB (если символы доступны). Так кадры
            // стека сразу читаются как «файл:строка», без ручного разбора по карте.
            // ВАЖНО: буфер символа выделяем ВМЕСТЕ с именем (классический контракт
            // dbghelp: Name лежит сразу за структурой), иначе SymFromAddr пишет
            // имя за пределами SYMBOL_INFO — раньше тут был переполняющий буфер.
            char sym[260] = "";
            DWORD64 disp = 0;
            char symbuf[sizeof(SYMBOL_INFO) + 200];
            SYMBOL_INFO* psi = (SYMBOL_INFO*)symbuf;
            ZeroMemory(psi, sizeof(SYMBOL_INFO));
            psi->SizeOfStruct = sizeof(SYMBOL_INFO);
            psi->MaxNameLen = 199;
            const bool haveSym = SymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &disp, psi) != FALSE;
            IMAGEHLP_LINE64 ln; ZeroMemory(&ln, sizeof(ln)); ln.SizeOfStruct = sizeof(ln);
            DWORD lineDisp = 0;
            const bool haveLine = SymGetLineFromAddr64(GetCurrentProcess(), sf.AddrPC.Offset, &lineDisp, &ln) != FALSE;
            if (haveSym && haveLine)
                _snprintf_s(sym, sizeof(sym), _TRUNCATE, "%s (%s:%lu)", psi->Name, ln.FileName, (unsigned long)ln.LineNumber);
            else if (haveSym)
                _snprintf_s(sym, sizeof(sym), _TRUNCATE, "%s", psi->Name);
            else if (haveLine)
                _snprintf_s(sym, sizeof(sym), _TRUNCATE, "(без символа) %s:%lu", ln.FileName, (unsigned long)ln.LineNumber);
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "  #%-2d %08llX  %-58s %s\r\n", fi,
                (unsigned long long)sf.AddrPC.Offset, mi, sym);
        }
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  (кадров разобрано StackWalk64: %d)\r\n", fi);

        // ---- РУЧНОЙ РАЗБОР СТЕКА (2026-10-07) --------------------------------
        // StackWalk64 на 32-битном x86 в Wine обрывается через 2-4 кадра: для
        // frames без FPO он не может восстановить EBP предыдущего кадра и
        // возвращает FALSE. Практически это значит «вылета нет нигде» — вывод
        // бесполезный. Поэтому ниже ДВА независимых прохода по сырой памяти стека:
        //   1) цепочка EBP: EBP -> [EBP+4] = адрес возврата (классика -O0/-O1);
        //   2) сканирование: ищем в стеке DWORD'ы, попадающие в загруженный модуль.
        // Второй проход даёт порядок кадров даже без символов — этого достаточно,
        // чтобы отличить «упало внутри игры» от «упало внутри плагина/Lua».
        {
            // Снимок окна стека упавшего потока прямо из его CONTEXT.
            // БАЗА = ESP (2026-10-19): раньше окно бралось из VirtualQuery, но у
            // Wine VQ на стеке умирающих потоков время от времени возвращает
            // мусор (окно 0xFFFFFFE8..0x00001FE8 → «найдено 0» всегда). Теперь
            // окно НАДЕЖНО = [Esp-0x40, Esp+0x1000]; VQ-окно применяем только
            // если оно реально накрывает ESP-окно.
            DWORD stackLo = origEsp, stackHi = origEsp + 0x1000;
            bool stackOk = false;
            MEMORY_BASIC_INFORMATION smbi;
            if (origEsp >= 0x40 && VirtualQuery((void*)(ULONG_PTR)origEsp, &smbi, sizeof(smbi))
                && smbi.State == MEM_COMMIT) {
                const DWORD b = (DWORD)smbi.BaseAddress;
                const DWORD e = b + (DWORD)smbi.RegionSize;
                if (b <= origEsp) {
                    stackLo = b;
                    if (e < stackHi) stackHi = e;   // не выходим за committed-регион
                    stackOk = true;
                }
            }
            // Сырой дамп стека — первые 4 строки по 12 dword (48 всего): по ним
            // вручную всегда видны адреса возврата индивидно, даже если
            // эвристика их отфильтрует.
            if (stackOk && stackHi - origEsp >= 0xC0) {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "\r\n  СЫРОЙ СТЕК [Esp..Esp+0xC0] (4 строки по 12 dword):\r\n");
                const DWORD* sp = (const DWORD*)(ULONG_PTR)origEsp;
                for (int row = 0; row < 4; row++, sp += 12) {
                    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "    +0x%02X:", row * 0x30);
                    for (int i = 0; i < 12; i++)
                        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, " %08X", sp[i]);
                    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "\r\n");
                }
            } else {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "\r\n  СЫРОЙ СТЕК: пропущен (Esp=%08X не в COMMIT)\r\n", origEsp);
            }
            // --- проход 1: цепочка EBP ---
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "\r\n  цепочка EBP (ручной разбор):\r\n");
            DWORD ebp = origEbp;
            int guard = 0;
            for (int k = 0; k < 24 && stackOk && ebp >= stackLo && ebp + 8 <= stackHi; k++) {
                const DWORD* f = (const DWORD*)(ULONG_PTR)ebp;
                DWORD ret = f[1];
                DWORD nextEbp = f[0];
                if (ret < 0x10000 || ret == 0xFFFFFFFF) break;   // мусор — цепочка кончилась
                char mi[220];
                write_module_info(mi, sizeof(mi), (void*)(ULONG_PTR)ret);
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    EBP+0x%02X: ret=%08X  %s\r\n", k * 4, ret, mi);
                if (nextEbp <= ebp || nextEbp >= stackHi) break;  // защита от цикла/выхода
                ebp = nextEbp;
                if (++guard > 32) break;
            }
            // --- проход 2: сканирование стека на адреса кода ---
            // Печатаем только адреса, попавшие в ЗАГРУЖЕННЫЙ модуль, и не подряд
            // (иначе будет тысяча одинаковых строк от локальных переменных).
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "\r\n  сканирование стека на адреса кода (heuristic, порядок сверху вниз):\r\n");
            int printed = 0;
            const DWORD* base = (const DWORD*)stackLo;
            const DWORD* end = (const DWORD*)stackHi;
            for (const DWORD* p = base; stackOk && p + 1 <= end && printed < 24 && n < (int)sizeof(buf) - 300; p++) {
                DWORD v = *p;
                if (v < 0x401000 || v > 0xA00000) continue;         // не в диапазоне .text игры
                // Перед адресом возврата в _thiscall-кадре лежит this — проверяем,
                // что он похож на указатель. Отсекаем мусорные «случайные» значения.
                DWORD prev = (p > base) ? p[-1] : 0;
                if (!(prev == 0 || (prev >= 0x400000 && prev <= 0xA00000))) continue;
                // не печатаем тот же адрес дважды подряд
                static DWORD lastRet = 0;
                if (v == lastRet) continue;
                lastRet = v;
                char mi[220];
                write_module_info(mi, sizeof(mi), (void*)(ULONG_PTR)v);
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    %08X  %s\r\n", v, mi);
                printed++;
            }
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "    (найдено адресов кода: %d, окно стека 0x%08X..0x%08X)\r\n",
                printed, stackLo, stackHi);
        }
    }

    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "\r\nПОСЛЕДНИЕ КРОШКИ (новые сверху):\r\n");
    int start = g_crumbNext.load();
    for (int i = 1; i <= kCrumbs; i++) {
        const Crumb& c = g_crumbs[(start + i) % kCrumbs];
        if (!c.text[0]) continue;
        unsigned long long ago = GetTickCount64() - c.tick;
        char tn[33] = "?";
        _snprintf_s(tn, sizeof(tn), _TRUNCATE, "%s", g_threadName[slot_of(c.threadId)]);
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  +%-8llu tid=%-6lu cmd=0x%-6X %-12s %s\r\n",
            ago, c.threadId, c.cmdId, tn, c.text);
    }

    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "\r\nКОМАНД ПО ПОТОКАМ:\r\n");
    for (int i = 0; i < kMaxThreads; i++) {
        unsigned long long cnt = g_cmdCount[i].load();
        if (!cnt) continue;
        char tn[33] = "?";
        _snprintf_s(tn, sizeof(tn), _TRUNCATE, "%s", g_threadName[i]);
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  slot=%-3d %-14s команд=%llu\r\n", i, tn, cnt);
    }

    // ---- СПИСОК ЗАГРУЖЕННЫХ МОДУЛЕЙ ------------------------------------
    // Нужен, чтобы потом вручную превратить «lualoader.asi +0xD1F32» в имя функции:
    // без базы и размера модуля смещение не разрешить. Раньше в отчёте модуль был
    // только у адреса падения, а у остальных кадров стека — нет.
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
        "\r\nЗАГРУЖЕННЫЕ МОДУЛИ (сначала lualoader.asi):\r\n");
    {
        HMODULE mods[512];
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
        if (snap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 me; ZeroMemory(&me, sizeof(me));
            me.dwSize = sizeof(me);
            if (Module32First(snap, &me)) {
                size_t used = 0;
                do {
                    // наш модуль печатаем первым, остальные — если ещё влезем
                    int mine = (_stricmp(me.szModule, "lualoader.asi") == 0);
                    if ((mine || used < sizeof(mods)/sizeof(mods[0])) && n < (int)sizeof(buf) - 400) {
                        if (!mine) mods[used++] = me.hModule;
                        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                            "  %-28s base=%08lX size=%08lX  %s\r\n",
                            me.szModule, (unsigned long)me.modBaseAddr, me.modBaseSize, me.szExePath);
                    }
                } while (Module32Next(snap, &me));
            }
            CloseHandle(snap);
        }
    }

    // ---- ПОТОКИ ПРОЦЕССА -----------------------------------------------
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "\r\nПОТОКИ ПРОЦЕССА:\r\n");
    {
        HANDLE tsnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (tsnap != INVALID_HANDLE_VALUE) {
            THREADENTRY32 te; ZeroMemory(&te, sizeof(te));
            te.dwSize = sizeof(te);
            if (Thread32First(tsnap, &te)) {
                do {
                    if (te.th32OwnerProcessID == GetCurrentProcessId()
                        && n < (int)sizeof(buf) - 200) {
                        char tn[33] = "?";
                        _snprintf_s(tn, sizeof(tn), _TRUNCATE, "%s", g_threadName[slot_of(te.th32ThreadID)]);
                        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                            "  tid=%-8lu %-16s%s\r\n", (unsigned long)te.th32ThreadID, tn,
                            te.th32ThreadID == GetCurrentThreadId() ? "  <-- УПАВШИЙ" : "");
                    }
                } while (Thread32Next(tsnap, &te));
            }
            CloseHandle(tsnap);
        }
    }
    n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE, "=======================================================\r\n");

    // ---- ВОЗМОЖНАЯ ПРИЧИНА (авто-заключение) --------------------------------
    // Ниже — интерпретация собранного выше. Человек должен видеть ВЕРДИКТ,
    // а не только сырые данные. Логика: смотрим на поток, последнюю команду,
    // модель, глубину очереди и сам код исключения.
    {
        unsigned long long inflight_ms = 0;
        unsigned lo = g_lockOwner.load();
        unsigned long long ls = g_lockSince.load();
        if (g_inflightSince.load()) inflight_ms = GetTickCount64() - g_inflightSince.load();
        unsigned lo2 = lo;
        bool on_game = (GetCurrentThreadId() == g_gameThread.load());
        bool stuck = inflight_ms > 3000;
        bool model_stuck = g_modelSince.load() && (GetTickCount64() - g_modelSince.load()) > 5000;

        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "\r\nВОЗМОЖНАЯ ПРИЧИНА (авто-заключение, НЕ факт — по данным выше):\r\n");

        if (code == EXCEPTION_ACCESS_VIOLATION) {
            // Разберём по вариантам, от конкретного к общему.
            if (model_stuck) {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  Вероятно: модель id=%d не загрузилась / грузилась очень долго.\r\n",
                    g_modelId.load());
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    Если это произошло в create_car/create_ped/create_pickup/give_weapon —\r\n");
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    скорее всего игра попыталась создать объект из НЕСУЩЕСТВУЮЩЕЙ модели.\r\n");
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    Проверка: id модели должен быть 0..7950 и реально существовать в игре.\r\n");
            } else if (stuck && g_inflightCmd.load()) {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  Вероятно: команда 0x%X зависла на %llu мс (поток %lu), игра упала из-за\r\n",
                    g_inflightCmd.load(), inflight_ms, g_inflightThread.load());
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    блокировки игрового потока (недопустимая длительная блокировка / deadlock).\r\n");
            } else if (!on_game) {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  Вероятно: вылет в НЕИГРОВОМ потоке (%lu). Все игровые операции\r\n",
                    GetCurrentThreadId());
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    должны выполняться через ma_cmdq_defer на игровом потоке.\r\n");
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    Прямой вызов Command<> из постороннего потока — запрещён.\r\n");
            } else {
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "  Вероятно: игровые структуры в момент вылета были повреждены/некорректны.\r\n");
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    Посмотрите, какая команда 0x%X последняя была в работе, и что она\r\n",
                    g_inflightCmd.load());
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                    "    читала/писала (адрес данных в «операция» выше).\r\n");
            }
        } else if (code == EXCEPTION_STACK_OVERFLOW) {
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "  Переполнение стека: рекурсивный вызов или очень глубокая цепочка.\r\n");
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "    Смотрите стек выше — где начинается глубокое вложение.\r\n");
        } else if (code == 0xE06D7363) {
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "  C++ throw (исключение): скорее всего ошибка Lua-скрипта, не пойманная\r\n");
                n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "    в плагине. Посмотрите крошки (последние команды) и lua_log.\r\n");
        } else {
            n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
                "  Код 0x%08lX — см. phase_name. Подробности — в стеке выше.\r\n",
                (unsigned long)code);
        }
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  (Это авто-заключение по данным, собранным выше; не доказательство. Точную\r\n");
        n += _snprintf_s(buf + n, sizeof(buf) - n, _TRUNCATE,
            "  причину ищите по стеку и Lua-контексту выше — .dmp-дампы больше не создаются).\r\n");
    }

    // Сброс буфера журнала: пишем БУФЕР напрямую через append_raw БЕЗ g_fileLock.
    // Дедлок (13.10): diag_flush() брал g_fileLock, а если крашился поток, который
    // уже держал g_fileLock в diag_line() — EnterCriticalSection зависал вечно,
    // отчёт не дописывался. append_raw открывается
    // отдельно (FILE_APPEND_DATA) — не входит в буфер/гаснет при дедлоке.
    if (g_logLen > 0) {
        HANDLE h = INVALID_HANDLE_VALUE;
        if (log_handle(h)) {
            DWORD w = 0;
            WriteFile(h, g_logBuf, (DWORD)g_logLen, &w, nullptr);
            g_logLen = 0;
            g_lastFlush = GetTickCount64();
        }
    }
    // В файл БЕЗ мьютексов и БЕЗ fstream: если крашится поток, держащий
    // g_fileLock или std::fstream, обычный логгер сам зависнет.
    append_raw(kLogPath, buf, (DWORD)n);
    InterlockedExchange(&g_inHandler, 0);
}

// ============================================================================
// ЛОВУШКИ СТИЛЯ ШРИФТА (INT3) — 18.10.2026 (вечер)
// ============================================================================
// ЗАДАЧА: поймать момент, когда в путь «запись сообщения → CFont::SetFontStyle
// → Details(0x97F858) → очередь → RenderState(0x94B924) → глифы» попадает
// мусорный стиль (напр. 0x9CDC), валящий игру на 0x551E0D. Сторож
// font_style_guard() ловит только НА ГРАНИЦЕ КАДРА — если запись и крах
// происходят в одном кадре, он молчит. Ловушки срабатывают РОВНО в месте чтения
// поля стиля — с полным контекстом: регистры, адрес и СОДЕРЖИМОЕ записи
// (0x7F0EA0 + n*0xF4, стиль — dword +0x20, текст — wchar с +0x2C).
//
// Сайты (смещения от базы exe; exe грузится по 0x400000, но берём из модуля):
//   A  +0x156881  (0x556881)  push eax перед call SetFontStyle: AX = [rec+0x20],
//                             EBX = номер записи, EBP = номер*0xF4, ESI = 0x7F0EA0+номер*0xF4
//   B  +0x15ACB1  (0x55ACB1)  push eax (второй путь отрисовки сообщений):
//                             ESI = номер*0xF4, EDI = 0x7F0EA0+номер*0xF4
//   C  +0x150007  (0x550007)  mov [0x97F858], cx внутри SetFontStyle (ЛЮБОЙ вызов):
//                             CX = стиль, [esp] = адрес возврата
//   D  +0x151B41  (0x551B41)  mov [0x94B924], ax — копия шаблон[0]+0x2C (0x709388)
//                             в RenderState при рендере кадра (флаш буфера); AX = значение
//   E  +0x151C42  (0x551C42)  mov [0x94B924], ax — копия шапки записи [esi+0x2C]
//                             (esi — шапка в буфере 0x70935C..0x70975C); AX = значение
//   F  +0x1516EB  (0x5516EB)  mov [0x94B924], ax — Details(0x97F858)->RenderState
//                             при смене стиля посреди печати; AX = значение
//   G  +0x15193B  (0x55193B)  mov [eax+0x2C], bp — постройка шапки записи: BP = Details
//                             (0x97F858), EAX = указатель записи [0x70975C]
//   I  +0x15188F  (0x55188F)  страж-кламп построителя записи: ESI=начало строки,
//                             EBX=конец; len>0x3C0 → EBX=ESI+0x3C0 — ЕДИНСТВЕННАЯ
//                             write-side проверка переполнения буфера (см. блок
//                             комментария перед ft_build_guard; бывшая ловушка H
//                             0x550EBF = середина мега-функции, убрана 19.10)
// Механика: INT3 → VEH: Eip к INT3, вернуть исходный байт, фильтр (стиль вне 0..5 —
// дамп), TF. #DB → вернуть INT3. Фильтр «0..5» — легитимные стили игры.
namespace {
const DWORD kRecBase   = 0x7F0EA0;   // массив записей сообщений (48 шт.)
const DWORD kRecStride = 0xF4;
const DWORD kRecCount  = 0x30;

struct FontTrap {
    DWORD off;               // RVA от базы exe
    DWORD addr;              // абсолютный адрес (после install)
    unsigned char orig;      // исходный байт
    volatile LONG armed;     // 1 — INT3 установлен
    const char* name;
    int valreg;              // чей регистр держит значение: 0=AX(EAX), 1=CX(ECX), 2=BP(EBP)
};
FontTrap g_ft[8];
volatile LONG g_ftEvents = 0;        // сколько «плохих» событий уже записали (общий кап)
volatile LONG g_ftFirst[8] = {0,0,0,0,0,0,0,0};
// ── СТРАЖ ПОСТРОЙКИ ШРИФТОВОЙ ЗАПИСИ (ловушка 8, 0x55188F) ──────────────────
// 19.10 (вечер): «вход PrintString» не найден (0x550EBF = середина мега-функции,
// старый страж H никогда не срабатывал). ЗАТО найдена ЕДИНСТВЕННАЯ write-side
// точка проверки переполнения шаблонного буфера: скан всего exe по ref'ам к
// 0x70975C (16 шт., 0x551888..0x5522C4) — все чтения/записи:
//   0x55188F  проверка ПЕРЕД достройкой записи (mov edx,ebx; sub edx,esi;...
//             call 0x551A30 при переполнении): ESI=начало строки, EBX=конец,
//             размер записи = (len/2+0x1A)*2 = len+0x34;
//   0x551A30  флуш (рендер накопленного + сброс курсора в 0x70935C на 0x551E54);
//   0x5522B0  полный сброс (mov [0x70975C],0x70935C на 0x5522C2);
//   0x551B66/0x551BA9/0x551E41 — чтения в РЕНДЕР-проходе (jae 0x551E4F = выход).
// Крах-механика: флуш срабатывает один раз и НИЧЕГО не проверяет после —
// дикая запись (len≈37748 wchar, 19.10) достраивается за курсор (tplPtr
// 0x71BA74), мусорный стиль 0x9CDC → AV 0x551E0D.
// Страж: len(байт) > 0x3C0 → EBX = ESI + 0x3C0 (кламп до 480 wchar) —
// гарантированно влезает в пустой буфер (0x3F4 ≤ размеру лимита + шапка 0x34),
// ЛЕЧИТ КЛАСС при ЛЮБОМ вызывающем (не нужно знать точный вход печати).
// Стек (20 dword) и пара start/end пишем в лог для поиска источника.
const DWORD kBuildCheckOffs = 0x15188F;          // RVA: mov edx,ebx (ESI=start, EBX=end)
const DWORD kPrintClampBytes = 0x3C0;            // 480 wchar — безопасный предел
volatile LONG g_printHits = 0;                   // всего вызовов PrintString
volatile LONG g_printClamps = 0;                 // сколько раз кламп сработал

// Адреса глобалов шрифта (kTplBase/kTplEnd/kStyleWord/kDetailsSty) определены
// ВЫШЕ, перед report() — в охватывающем namespace, чтобы обе части файла
// пользовались ОДНИМИ константами (иначе вложенный namespace видел бы копии).

void ft_dump_record(DWORD cand, int style) {
    if (cand < kRecBase || cand > kRecBase + kRecStride * kRecCount) return;
    if (((cand - kRecBase) % kRecStride) != 0) return;
    const unsigned char* r = (const unsigned char*)(uintptr_t)cand;
    char b[600];
    DWORD d20 = *(const DWORD*)(r + 0x20);
    DWORD t8  = *(const DWORD*)(r + 0x08);
    DWORD f24 = *(const DWORD*)(r + 0x24);
    DWORD f28 = *(const DWORD*)(r + 0x28);
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "FONT-TRAP запись[%d] @0x%X: dword+0x20=0x%08X (word=0x%04X, старшее слово=0x%04X) "
        "time+0x8=%u f+0x24=0x%08X f+0x28=0x%08X b+0x1D=%u b+0x1E=%u",
        (int)((cand - kRecBase) / kRecStride), cand, d20, (unsigned)(d20 & 0xFFFF),
        (unsigned)(d20 >> 16), t8, f24, f28, (unsigned)r[0x1D], (unsigned)r[0x1E]);
    diag_line(b);
    for (int off = 0; off < 0x40; off += 16) {
        char line[128]; int m = 0;
        m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE, "FONT-TRAP rec %02X:", off);
        for (int k = 0; k < 16; ++k)
            m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE, " %02X", r[off + k]);
        diag_line(line);
    }
    {
        const unsigned short* t = (const unsigned short*)(r + 0x2C);
        char txt[420]; int m = 0;
        for (int k = 0; k < 100 && t[k] && m < (int)sizeof(txt) - 12; ++k) {
            unsigned short wc = t[k];
            if (wc >= 32 && wc <= 0x7E)
                m += _snprintf_s(txt + m, sizeof(txt) - m, _TRUNCATE, "%c", (char)wc);
            else
                m += _snprintf_s(txt + m, sizeof(txt) - m, _TRUNCATE, "\\u%04X", (unsigned)wc);
        }
        char line[520];
        _snprintf_s(line, sizeof(line), _TRUNCATE, "FONT-TRAP текст+0x2C: %s", txt);
        diag_line(line);
    }
}

// (ft_tpl_walk определён ВЫШЕ, перед report(), — здесь не дублировать)

// Печать одной записи буфера (шапка hex + текст) — для сайтов D/E/G.
void ft_dump_tpl_entry(DWORD p, const char* tag) {
    if (p < kTplBase || p + 0x30 > kTplEnd) {
        char b[160];
        _snprintf_s(b, sizeof(b), _TRUNCATE, "  FONT-TRAP %s: 0x%X ВНЕ шаблонного буфера", tag, p);
        diag_line(b);
        return;
    }
    const unsigned char* e = (const unsigned char*)(uintptr_t)p;
    {
        char line[180]; int m = 0;
        m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE, "  FONT-TRAP %s @0x%X:", tag, p);
        for (int i = 0; i < 0x30 && m < (int)sizeof(line) - 12; i += 8)
            m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE, " %08X", *(const unsigned int*)(e + i));
        diag_line(line);
    }
    const unsigned short* t = (const unsigned short*)(e + 0x30);
    size_t maxw = (size_t)((kTplEnd - p - 0x30) / 2);
    if (maxw > 90) maxw = 90;
    char txt[220]; txt[0] = 0; int m = 0;
    size_t wl = 0;
    for (; wl < maxw; ++wl) {
        unsigned short wc = t[wl];
        if (!wc) break;
        if (m > (int)sizeof(txt) - 12) break;
        if (wc >= 32 && wc <= 0x7E)
            m += _snprintf_s(txt + m, sizeof(txt) - m, _TRUNCATE, "%c", (char)wc);
        else
            m += _snprintf_s(txt + m, sizeof(txt) - m, _TRUNCATE, "\\u%04X", (unsigned)wc);
    }
    char line[320];
    _snprintf_s(line, sizeof(line), _TRUNCATE, "  FONT-TRAP %s текст(len=%u): %s", tag, (unsigned)wl, txt);
    diag_line(line);
}

// Короткий снимок стека (адреса возврата) — для сайтов D/E/F/G.
void ft_dump_stack(CONTEXT* c, const char* tag) {
    const unsigned int* st = (const unsigned int*)(uintptr_t)c->Esp;
    char b[280];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "  FONT-TRAP %s стек: %08X %08X %08X %08X %08X %08X",
        tag, st[0], st[1], st[2], st[3], st[4], st[5]);
    diag_line(b);
}

// Полный снимок глобалов шрифта + цепочки буфера (для «плохих» срабатываний).
void ft_dump_font_snapshot(const char* tag) {
    char b[400];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "  FONT-GLOB [%s]: want[0x97F858]=%d (0x%04X) used[0x94B924]=%d (0x%04X) "
        "tplPtr[0x70975C]=0x%X tpl0+2C[0x709388]=%d (0x%04X) 0x97F860=%u",
        tag,
        (int)*(const short*)(uintptr_t)kDetailsSty, (unsigned)*(const unsigned short*)(uintptr_t)kDetailsSty,
        (int)*(const short*)(uintptr_t)kStyleWord, (unsigned)*(const unsigned short*)(uintptr_t)kStyleWord,
        (unsigned)*(const unsigned long*)(uintptr_t)kTplEnd,
        (int)*(const short*)(uintptr_t)(kTplBase + 0x2C), (unsigned)*(const unsigned short*)(uintptr_t)(kTplBase + 0x2C),
        (unsigned)*(const unsigned short*)(uintptr_t)0x97F860);
    diag_line(b);
    char tpl[6400];
    ft_tpl_walk(tpl, sizeof(tpl));
    diag_line(tpl);
}

// Страж постройки записи (INT3 на 0x55188F, до вычисления длины):
// ESI = начало строки (wchar_t*), EBX = конец. Легитимные записи (<0x3C0 байт)
// проходят без изменений; дикая запись обрезается по EBX. Стек НЕ трогаем в
// стиле старого стража ([esp] здесь это середина мега-функции, не возврат) —
// вместо этого логируем 20 dword стека и ищем возврат в exe-код (0x0046xxxx..).
void ft_build_guard(CONTEXT* c) {
    const DWORD start = c->Esi;
    const DWORD end = c->Ebx;
    const long long len = (long long)end - (long long)start;
    const LONG n = InterlockedIncrement(&g_printHits);
    if (len <= (long long)kPrintClampBytes && len >= -8) {
        // Валидация: первые легитимные постройки подтверждают семантику пары.
        if (n <= 2) {
            char b[260];
            MEMORY_BASIC_INFORMATION q;
            const bool ok = start >= 0x10000u && start < 0x7FFF0000u
                && VirtualQuery((void*)(uintptr_t)start, &q, sizeof(q)) && q.State == MEM_COMMIT;
            const bool txt = ok && *(const unsigned short*)(uintptr_t)start != 0;
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "FONT-GUARD: постройка #%ld: start=0x%X end=0x%X len=%lld (валидация: %s)",
                (long)n, start, end, len, txt ? "строка читаема" : "start не текст/не читаем?");
            diag_line(b);
        }
        return;
    }
    // Подозрительная пара: не зажимаем (смещения у пары иные или это не текст).
    MEMORY_BASIC_INFORMATION qs;
    const bool startOk = start >= 0x10000u && start < 0x7FFF0000u
        && VirtualQuery((void*)(uintptr_t)start, &qs, sizeof(qs)) && qs.State == MEM_COMMIT;
    const bool plausible = startOk && end > start && end < 0x80000000u;
    if (!plausible) {
        if (InterlockedIncrement(&g_printClamps) <= 8) {
            char b[300];
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "FONT-GUARD: подозрительная пара esi=0x%X ebx=0x%X len=%lld — НЕ указатели, "
                "кламп НЕ применён (пара достроится как у игры)",
                start, end, len);
            diag_line(b);
        }
        return;
    }
    const LONG k = InterlockedIncrement(&g_printClamps);
    // Курсор при срабатывании: если уже убежал за буфер — вернуть на базу.
    const DWORD cur = *(volatile const DWORD*)(uintptr_t)kTplEnd;
    DWORD newEnd = start + kPrintClampBytes;
    {
        MEMORY_BASIC_INFORMATION qn;
        if (VirtualQuery((void*)(uintptr_t)start, &qn, sizeof(qn)) && qn.State == MEM_COMMIT) {
            const unsigned short* t = (const unsigned short*)(uintptr_t)start;
            const DWORD maxw = (DWORD)((end - start) / 2);
            const DWORD capw = kPrintClampBytes / 2;
            DWORD w = 0;
            while (w < maxw && w < capw && t[w] != 0) ++w;
            if (w < maxw && w < capw)
                newEnd = start + (w + 1) * 2;
        }
    }
    if (k <= 8) {
        char b[760];
        const DWORD* st = (const DWORD*)(uintptr_t)c->Esp;
        char stk[440]; int m2 = 0;
        stk[0] = 0;
        for (int i = 0; i < 48; ++i)
            m2 += _snprintf_s(stk + m2, sizeof(stk) - m2, _TRUNCATE, " %08X", st[i]);
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "FONT-GUARD: ДИКАЯ ЗАПИСЬ #%ld: len=%lld байт (%lld wchar) start=0x%X end=0x%X "
            "cursor=0x%X (норма 0x%X..0x%X) eax=0x%X ecx=0x%X edx=0x%X edipos=0x%X — КЛАМП ebx до 0x%X (NUL+2; предел 0x%X)",
            (long)k, len, len / 2, start, end, cur, kTplBase, kTplEnd,
            (unsigned)c->Eax, (unsigned)c->Ecx, (unsigned)c->Edx, (unsigned)c->Eip,
            (unsigned)newEnd, (unsigned)(start + kPrintClampBytes));
        diag_line(b);
        diag_line(stk);
        // Что за текст пришёл: 80 wchar по start, если страница читаема.
        MEMORY_BASIC_INFORMATION q;
        if (VirtualQuery((void*)(uintptr_t)start, &q, sizeof(q)) && q.State == MEM_COMMIT) {
            const unsigned short* t = (const unsigned short*)(uintptr_t)start;
            char hx[520]; int m = 0;
            m += _snprintf_s(hx + m, sizeof(hx) - m, _TRUNCATE, "  FONT-GUARD источник[80]:");
            for (int i = 0; i < 80 && m < (int)sizeof(hx) - 8; ++i)
                m += _snprintf_s(hx + m, sizeof(hx) - m, _TRUNCATE, " %04X", (unsigned)t[i]);
            diag_line(hx);
        } else {
            diag_line("  FONT-GUARD источник: страница start НЕ читаема");
        }
    }
    if (cur < kTplBase || cur > kTplEnd) {
        *(volatile DWORD*)(uintptr_t)kTplEnd = kTplBase;   // курсор убежал — вернуть на базу
        char b[220];
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "FONT-GUARD: КУРСОР ВНЕ БУФЕРА 0x%X — СБРОШЕН на 0x%X (страж #%ld)",
            cur, kTplBase, (long)k);
        diag_line(b);
    }
    c->Ebx = newEnd;       // обрезать конец по NUL (в паре esi/ebx)
}

void ft_log(int idx, int style, EXCEPTION_POINTERS* ep) {
    CONTEXT* c = ep->ContextRecord;
    bool first = false;
    if (!g_ftFirst[idx]) { g_ftFirst[idx] = 1; first = true; }
    const bool bad = (style < 0 || style > 5);
    if (!bad && !first) return;
    LONG ev = InterlockedIncrement(&g_ftEvents);
    if (ev > 18) return;   // кап: не заливать журнал
    char b[400];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "FONT-TRAP %s: %s стиль=%d (0x%04X) eip=0x%X eax=0x%X ebx=0x%X ecx=0x%X edx=0x%X "
        "esi=0x%X edi=0x%X ebp=0x%X esp=0x%X",
        g_ft[idx].name, bad ? "МУСОР!" : "первое срабатывание,", style, style & 0xFFFF,
        (unsigned)c->Eip, c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp);
    diag_line(b);
    if (!bad) return;
    ft_dump_font_snapshot(g_ft[idx].name);   // снимок глобалов + цепочки буфера
    if (idx == 0) {          // сайт A
        ft_dump_record(c->Esi, style);
        ft_dump_record(kRecBase + c->Ebp, style);
    } else if (idx == 1) {   // сайт B
        ft_dump_record(kRecBase + c->Esi, style);
        ft_dump_record(c->Edi, style);
    } else if (idx == 2) {   // сайт C: кто вызвал
        const DWORD* st = (const DWORD*)(uintptr_t)c->Esp;
        DWORD caller = st[0];
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "FONT-TRAP C caller=0x%X стек: %08X %08X %08X %08X %08X %08X",
            caller, st[0], st[1], st[2], st[3], st[4], st[5]);
        diag_line(b);
        if (caller == 0x556887) { ft_dump_record(c->Esi, style); ft_dump_record(kRecBase + c->Ebp, style); }
        else if (caller == 0x55ACB7) { ft_dump_record(kRecBase + c->Esi, style); ft_dump_record(c->Edi, style); }
    } else if (idx == 3) {   // сайт D: 0x551B41 — шаблон[0]+0x2C (0x709388) → RenderState
        ft_dump_tpl_entry(kTplBase, "шаблон[0]");
        ft_dump_stack(c, "D");
    } else if (idx == 4) {   // сайт E: 0x551C42 — шапка записи [esi+0x2C] → RenderState
        ft_dump_tpl_entry(c->Esi, "запись@esi→RS");
        ft_dump_stack(c, "E");
    } else if (idx == 5) {   // сайт F: 0x5516EB — Details→RenderState (смена стиля в печати)
        ft_dump_stack(c, "F");
    } else if (idx == 6) {   // сайт G: 0x55193B — Details→шапка [eax+0x2C] (постройка записи)
        ft_dump_tpl_entry(c->Eax, "новая шапка@eax");
        ft_dump_stack(c, "G");
    }
}

// Возврат 1 = исключение наше, продолжать (EXCEPTION_CONTINUE_EXECUTION).
int ft_exception(EXCEPTION_POINTERS* ep) {
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    CONTEXT* c = ep->ContextRecord;
    if (code == EXCEPTION_BREAKPOINT) {
        DWORD ea = (DWORD)(uintptr_t)ep->ExceptionRecord->ExceptionAddress;
        DWORD byEip = c->Eip - 1;
        for (int i = 0; i < 8; ++i) {
            if (!g_ft[i].armed) continue;
            if (ea != g_ft[i].addr && byEip != g_ft[i].addr) continue;
            DWORD a = g_ft[i].addr;
            c->Eip = a;                          // назад на INT3
            *(volatile unsigned char*)(uintptr_t)a = g_ft[i].orig;  // вернуть байт
            if (g_ft[i].valreg == 3) {
                ft_build_guard(c);               // страж постройки записи 0x55188F
            } else {
                int style;
                switch (g_ft[i].valreg) {        // чей регистр держит значение
                    case 1:  style = (short)(c->Ecx & 0xFFFF); break;
                    case 2:  style = (short)(c->Ebp & 0xFFFF); break;
                    default: style = (short)(c->Eax & 0xFFFF); break;
                }
                ft_log(i, style, ep);
            }
            c->EFlags |= 0x100;                  // TF: один шаг и #DB — вернём INT3
            return 1;
        }
        return 0;
    }
    if (code == EXCEPTION_SINGLE_STEP) {
        int ours = 0;
        for (int i = 0; i < 8; ++i) {
            if (!g_ft[i].armed) continue;
            DWORD a = g_ft[i].addr;
            if (*(volatile unsigned char*)(uintptr_t)a != 0xCC) {
                *(volatile unsigned char*)(uintptr_t)a = 0xCC;
                ours = 1;
            }
        }
        if (ours) { c->EFlags &= ~0x100u; return 1; }
        return 0;
    }
    return 0;
}

void font_traps_install() {
    DWORD base = (DWORD)(uintptr_t)GetModuleHandleA("gta-vc.exe");
    if (!base) base = (DWORD)(uintptr_t)GetModuleHandleA(nullptr);
    if (!base) return;
    const DWORD offs[8] = { 0x156881, 0x15ACB1, 0x150007, 0x151B41, 0x151C42, 0x1516EB, 0x15193B, kBuildCheckOffs };
    const char* names[8] = {
        "A(rec push)", "B(rec push alt)", "C(SetFontStyle store)",
        "D(0x551B41 tpl0->RS)", "E(0x551C42 entry->RS)", "F(0x5516EB details->RS)", "G(0x55193B details->tpl)",
        "I(0x55188F buildclamp)"
    };
    const unsigned char expect[8] = { 0x50, 0x50, 0x66, 0x66, 0x66, 0x66, 0x66, 0x89 };
    const int valreg[8] = { 0, 0, 1, 0, 0, 0, 2, 3 };
    int ok = 0;
    for (int i = 0; i < 8; ++i) {
        g_ft[i].off  = offs[i];
        g_ft[i].addr = base + offs[i];
        g_ft[i].name = names[i];
        g_ft[i].valreg = valreg[i];
        DWORD p = g_ft[i].addr;
        DWORD old = 0;
        if (!VirtualProtect((LPVOID)(uintptr_t)p, 1, PAGE_EXECUTE_READWRITE, &old)) continue;
        unsigned char orig = *(volatile unsigned char*)(uintptr_t)p;
        g_ft[i].orig = orig;
        if (orig != expect[i]) {
            // Не тот байт — значит база/версия exe не совпала. НЕ трогаем код!
            VirtualProtect((LPVOID)(uintptr_t)p, 1, old, &old);
            char b[200];
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "FONT-TRAP: сайт %s @0x%X ОТКЛЮЧЁН: байт 0x%02X вместо 0x%02X",
                names[i], p, (unsigned)orig, (unsigned)expect[i]);
            diag_line(b);
            continue;
        }
        *(volatile unsigned char*)(uintptr_t)p = 0xCC;
        VirtualProtect((LPVOID)(uintptr_t)p, 1, old, &old);
        InterlockedExchange(&g_ft[i].armed, 1);
        ++ok;
        char b[160];
        _snprintf_s(b, sizeof(b), _TRUNCATE, "FONT-TRAP: сайт %s @0x%X установлен (байт был 0x%02X)",
                    names[i], p, (unsigned)orig);
        diag_line(b);
    }
    char b[160];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "FONT-TRAP: установлено %d из 8 ловушек (база exe 0x%X)", ok, base);
    diag_line(b);
}
// ============================================================================
// PRINT-CLAMP (27.10.2026) — ТИХИЙ предохранитель построителя сообщений.
//
// Вылет 27.10 01:26: EIP=0x551E0D, чтение по адресу 0xFFC6C66A. В шаблоне
// RenderState.FontStyle=0x9CDC (мусор), указатель записи [0x70975C]=0x71BA74
// (далеко за концом буфера 0x70975C). Механика: одна запись длиннее буфера
// достраивается ЗА его конец и затирает сам курсор [0x70975C]; следующий
// рендер идёт по мусорным «записям» и читает стиль 0x9CDC → AV.
//
// Единственная write-side проверка переполнения — 0x55188F: ESI=начало строки,
// EBX=конец, длина = EBX-ESI. Она флушит буфер, но НЕ ограничивает длину одной
// записи. Здесь ставим постоянный (не INT3, без спама в журнал) детур:
//   len = EBX-ESI; if (len > 0x3C0) { EDX = EBX = ESI+0x3C0; }
// 0x3C0 байт текста (480 wchar) + шапка 0x34 = 0x3F4 ≤ 0x400 — запись
// гарантированно влезает в пустой буфер. Работает для ЛЮБОГО вызывающего.
// Детур ставится один раз при install(); если байты exe не совпали — не трогаем.
namespace {
void print_clamp_install() {
    DWORD base = (DWORD)(uintptr_t)GetModuleHandleA("gta-vc.exe");
    if (!base) base = (DWORD)(uintptr_t)GetModuleHandleA(nullptr);
    if (!base) return;
    const DWORD target = base + 0x15188F;
    const unsigned char expect[7] = { 0x89, 0xDA, 0xB8, 0x5C, 0x97, 0x70, 0x00 };
    const unsigned char* p = (const unsigned char*)(uintptr_t)target;
    if (p[0] == 0xE9) {
        diag_line("PRINT-CLAMP: уже установлен (0x55188F начинается с E9)");
        return;
    }
    if (memcmp(p, expect, 7) != 0) {
        char b[200];
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "PRINT-CLAMP: ОТКЛЮЧЁН — байты 0x55188F не совпали "
            "(%02X %02X %02X %02X %02X %02X %02X)",
            p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
        diag_line(b);
        return;
    }
    unsigned char* cave = (unsigned char*)VirtualAlloc(nullptr, 64,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!cave) {
        diag_line("PRINT-CLAMP: VirtualAlloc не удался — страж не установлен");
        return;
    }
    int n = 0;
    cave[n++] = 0x89; cave[n++] = 0xDA;                  // mov edx, ebx
    cave[n++] = 0x29; cave[n++] = 0xF2;                  // sub edx, esi
    cave[n++] = 0x81; cave[n++] = 0xFA;                  // cmp edx, 0x3C0
    *(DWORD*)(cave + n) = kPrintClampBytes; n += 4;
    cave[n++] = 0x76; cave[n++] = 0x0B;                  // jbe +11 (пропустить кламп)
    cave[n++] = 0xBA;                                    // mov edx, 0x3C0
    *(DWORD*)(cave + n) = kPrintClampBytes; n += 4;
    cave[n++] = 0x8D; cave[n++] = 0x9E;                  // lea ebx, [esi+0x3C0]
    *(DWORD*)(cave + n) = kPrintClampBytes; n += 4;
    cave[n++] = 0xB8;                                    // mov eax, 0x70975C
    *(DWORD*)(cave + n) = kTplEnd; n += 4;
    cave[n++] = 0xE9;                                    // jmp 0x551898 (после sub edx,esi)
    {
        DWORD rel = (base + 0x151898) - ((DWORD)(uintptr_t)(cave + n) + 4);
        *(DWORD*)(cave + n) = rel; n += 4;
    }
    DWORD oldProt = 0;
    if (!VirtualProtect((LPVOID)(uintptr_t)target, 7, PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(cave, 0, MEM_RELEASE);
        diag_line("PRINT-CLAMP: VirtualProtect не удался — страж не установлен");
        return;
    }
    unsigned char patch[7];
    patch[0] = 0xE9;
    *(DWORD*)(patch + 1) = (DWORD)(uintptr_t)cave - (target + 5);
    patch[5] = 0x90; patch[6] = 0x90;
    memcpy((void*)(uintptr_t)target, patch, 7);
    DWORD tmp = 0;
    VirtualProtect((LPVOID)(uintptr_t)target, 7, oldProt, &tmp);
    FlushInstructionCache(GetCurrentProcess(), (LPCVOID)(uintptr_t)target, 7);
    FlushInstructionCache(GetCurrentProcess(), cave, (SIZE_T)n);
    char b[200];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "PRINT-CLAMP: установлен на 0x55188F (cave=0x%X, предел записи 0x%X байт)",
        (unsigned)(uintptr_t)cave, (unsigned)kPrintClampBytes);
    diag_line(b);
}
} // namespace (print clamp)

} // namespace (ловушки стиля)

// ============================================================================
// АППАРАТНЫЕ ТОЧКИ НА ЗАПИСЬ (DR0/DR1 — шрифт, DR2 — head списка CRadar) — 19.10/20.10.2026
// ============================================================================
// Скан exe доказал: слово 0x94B924 пишут РОВНО 4 инструкции (0x5516EB/0x551B41/
// 0x551C42 + `or word[0x94B924],-1` в сбросе 0x5522B0), слово 0x97F858 — РОВНО 3
// (0x54FFF5 imm=1, 0x550007 SetFontStyle, 0x552437 imm=0). Ловушка C на 0x550007
// чиста, значит 0x9CDC в кадре краха попадает в глобалы ДИКОЙ записью извне.
// DR0/DR1 ловят САМУ запись: для data-trap EIP указывает на СЛЕДУЮЩУЮ инструкцию —
// писатель виден в байтах ПЕРЕД EIP (в логе печатаем окно EIP-16..EIP+8).
// Вооружение — один раз из font_style_guard (игровой поток; рендер в VC однопоточный).
namespace {
volatile LONG g_wpState = 0;   // 0 = не вооружён, 1 = вооружён, -1 = не удалось
volatile LONG g_wpHits = 0;    // легитимные попадания — доказательство доставки
volatile LONG g_wpBad = 0;     // попадания с мусорным значением
volatile LONG g_wpRadarOK = 0;    // DR2: легитимные записи head
volatile LONG g_wpRadarZero = 0;  // DR2: записи head == 0 (виновник AV 0x4CA523)

void wp_log_radar_zero(CONTEXT* c) {
    char b[900];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "WATCH-RADAR!!! ПИСАТЕЛЬ НУЛЯ В HEAD 0x704864: eip=0x%X dr6=0x%X eax=%08X ebx=%08X ecx=%08X edx=%08X "
        "esi=%08X edi=%08X ebp=%08X esp=%08X",
        (unsigned)c->Eip, (unsigned)c->Dr6, (unsigned)c->Eax, (unsigned)c->Ebx, (unsigned)c->Ecx,
        (unsigned)c->Edx, (unsigned)c->Esi, (unsigned)c->Edi, (unsigned)c->Ebp, (unsigned)c->Esp);
    diag_line(b);
    // Байты инструкции-писателя: для data-trap EIP = СЛЕДУЮЩАЯ инструкция,
    // поэтому байты записи заканчиваются ровно на EIP.
    if (c->Eip > 0x10000 && c->Eip < 0x7FFFFFF0u) {
        const unsigned char* p = (const unsigned char*)(uintptr_t)(c->Eip - 16);
        char l[160]; int m = 0;
        m += _snprintf_s(l + m, sizeof(l) - m, _TRUNCATE,
            "  WATCH-RADAR байты EIP-16..EIP+8 (писатель кончается на EIP):");
        for (int i = 0; i < 24 && m < (int)sizeof(l) - 4; ++i)
            m += _snprintf_s(l + m, sizeof(l) - m, _TRUNCATE, " %02X", p[i]);
        diag_line(l);
    }
    // Контейнер CRadar 0x704858 сразу ПОСЛЕ нулевой записи.
    const volatile DWORD* ct = (const volatile DWORD*)(uintptr_t)0x704858;
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "  RADAR-контейнер 0x704858: d0=0x%X d4=0x%X head=0x%X sent=0x%X +14=0x%X +18=0x%X +1C=0x%X "
        "bhead=0x%X bsent=0x%X +38=0x%X +3C=0x%X nodes=0x%X",
        (unsigned)ct[0], (unsigned)ct[1], (unsigned)ct[3], (unsigned)ct[4], (unsigned)ct[5],
        (unsigned)ct[6], (unsigned)ct[7], (unsigned)ct[11], (unsigned)ct[12], (unsigned)ct[14],
        (unsigned)ct[15], (unsigned)ct[16]);
    diag_line(b);
    ft_dump_stack(c, "WATCH-RADAR");
}

void wp_log_bad(CONTEXT* c) {
    char b[700];
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "WATCH!!! ДИКАЯ ЗАПИСЬ В СТИЛЬ: eip=0x%X dr6=0x%X eax=%08X ebx=%08X ecx=%08X edx=%08X "
        "esi=%08X edi=%08X ebp=%08X esp=%08X",
        (unsigned)c->Eip, (unsigned)c->Dr6, (unsigned)c->Eax, (unsigned)c->Ebx, (unsigned)c->Ecx,
        (unsigned)c->Edx, (unsigned)c->Esi, (unsigned)c->Edi, (unsigned)c->Ebp, (unsigned)c->Esp);
    diag_line(b);
    // Байты инструкции-писателя: для data-trap EIP = СЛЕДУЮЩАЯ инструкция,
    // поэтому последние байты записи заканчиваются ровно на EIP.
    if (c->Eip > 0x10000 && c->Eip < 0x7FFFFFF0u) {
        const unsigned char* p = (const unsigned char*)(uintptr_t)(c->Eip - 16);
        char line[160]; int m = 0;
        m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE,
            "  WATCH байты EIP-16..EIP+8 (писатель кончается на EIP):");
        for (int i = 0; i < 24 && m < (int)sizeof(line) - 4; ++i)
            m += _snprintf_s(line + m, sizeof(line) - m, _TRUNCATE, " %02X", p[i]);
        diag_line(line);
    }
    // Известные писатели (для ручной сверки с окном байтов выше):
    //   0x94B924: 0x5516EB / 0x551B41 / 0x551C42 (`66 A3 24 B9 94 00`), 0x5522B0 (`or`)
    //   0x97F858: 0x54FFF5 / 0x550007 / 0x552437
    ft_dump_stack(c, "WATCH");
    ft_dump_font_snapshot("WATCH");
}

// Возврат 1 = исключение наше (data-trap DR0/DR1/DR2).
int wp_exception(EXCEPTION_POINTERS* ep) {
    if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_SINGLE_STEP) return 0;
    if (g_wpState != 1) return 0;
    CONTEXT* c = ep->ContextRecord;
    if (!(c->Dr6 & 0x7)) return 0;      // не наши слоты 0/1/2 (BS/BT от TF-шага, чужие DR)
    if (c->Dr6 & 0x4) {
        // DR2 — head списка радара 0x704864. Для data-trap EIP = следующая
        // инструкция, запись уже выполнена: читаем head ПОСЛЕ записи.
        c->Dr6 = 0;                          // снять причину, иначе повторный trap
        const unsigned int hv = *(volatile const unsigned int*)(uintptr_t)kRadarHead;
        if (hv == 0) {
            // КУРЕНИЕ: писатель записал головой 0 → AV 0x4CA523 в сливе — точный EIP здесь.
            if (InterlockedIncrement(&g_wpRadarZero) <= 3) wp_log_radar_zero(c);
            return 1;
        }
        if (InterlockedIncrement(&g_wpRadarOK) <= 2) {
            char b[240];
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "WATCH-RADAR: легитимная запись head=0x%X eip=0x%X (доставка DR2 работает)",
                hv, (unsigned)c->Eip);
            diag_line(b);
        }
        return 1;
    }
    const unsigned short sv = *(volatile const unsigned short*)(uintptr_t)kStyleWord;
    const unsigned short dv = *(volatile const unsigned short*)(uintptr_t)kDetailsSty;
    const bool bad = (sv > 5) || (dv > 5);
    c->Dr6 = 0;                          // снять причину, иначе повторный trap
    if (!bad) {
        LONG n = InterlockedIncrement(&g_wpHits);
        if (n <= 6) {
            char b[240];
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "WATCH: запись стиля #%ld eip=0x%X (доставка работает) style=0x%04X details=0x%04X",
                (long)n, (unsigned)c->Eip, (unsigned)sv, (unsigned)dv);
            diag_line(b);
        }
        return 1;
    }
    if (InterlockedIncrement(&g_wpBad) <= 8) wp_log_bad(c);
    return 1;
}

void wp_arm() {
    if (InterlockedCompareExchange(&g_wpState, -2, 0) != 0) return;
    char b[340];
    MEMORY_BASIC_INFORMATION q;
    if (VirtualQuery((void*)(uintptr_t)kStyleWord, &q, sizeof(q))) {
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "WATCH-ARM: VirtualQuery(0x%X) state=0x%X protect=0x%X type=0x%X",
            (unsigned)kStyleWord, (unsigned)q.State, (unsigned)q.Protect, (unsigned)q.Type);
        diag_line(b);
    } else {
        _snprintf_s(b, sizeof(b), _TRUNCATE, "WATCH-ARM: VirtualQuery не удался (err=%lu)", GetLastError());
        diag_line(b);
    }
    // Wine может отдавать нестандартный PROTECT для .data — НЕ блокируем вооружение
    // по состоянию страницы: DR-точки не зависят от PROTECT, а проверка уже дважды
    // ложно отменяла их (19.10). Вооружаемся всегда; работоспособность покажут попадания.
    HANDLE h = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, FALSE, GetCurrentThreadId());
    if (!h) {
        _snprintf_s(b, sizeof(b), _TRUNCATE, "WATCH-ARM: OpenThread не удался (err=%lu)", GetLastError());
        diag_line(b);
        g_wpState = -1;
        return;
    }
    CONTEXT ctx; ZeroMemory(&ctx, sizeof(ctx));
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(h, &ctx)) {
        _snprintf_s(b, sizeof(b), _TRUNCATE, "WATCH-ARM: GetThreadContext не удался (err=%lu)", GetLastError());
        diag_line(b);
        CloseHandle(h);
        g_wpState = -1;
        return;
    }
    _snprintf_s(b, sizeof(b), _TRUNCATE, "WATCH-ARM: было dr0=0x%X dr1=0x%X dr2=0x%X dr6=0x%X dr7=0x%X",
                (unsigned)ctx.Dr0, (unsigned)ctx.Dr1, (unsigned)ctx.Dr2, (unsigned)ctx.Dr6, (unsigned)ctx.Dr7);
    diag_line(b);
    ctx.Dr0 = kStyleWord;      // CFont::RenderState.FontStyle, 2 байта, запись
    ctx.Dr1 = kDetailsSty;     // CFont::Details.m_FontStyle, 2 байта, запись
    ctx.Dr2 = kRadarHead;      // CRadar head списка, 4 байта, запись (писатель нуля)
    ctx.Dr6 = 0;
    // L0|RW0=01|LEN0=2B + L1|RW1=01|LEN1=2B = 0x00550005;
    // DR2: L2 + RW2=01 + LEN2=4B (пишем dword) = 0x0D000030 (чужие биты DR7 сохраняем)
    ctx.Dr7 = (ctx.Dr7 & ~0x00000030u) | (0x00550005u | 0x0D000030u);
    if (!SetThreadContext(h, &ctx)) {
        _snprintf_s(b, sizeof(b), _TRUNCATE,
            "WATCH-ARM: SetThreadContext не удался (err=%lu) — Wine не даёт DR?", GetLastError());
        diag_line(b);
        CloseHandle(h);
        g_wpState = -1;
        return;
    }
    CONTEXT v; ZeroMemory(&v, sizeof(v)); v.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    const BOOL rb = GetThreadContext(h, &v);
    CloseHandle(h);
    _snprintf_s(b, sizeof(b), _TRUNCATE,
        "WATCH-ARM: вооружено tid=%lu dr0=0x%X dr1=0x%X dr2=0x%X dr7=0x%X | readback: dr0=0x%X dr2=0x%X dr7=0x%X %s",
        GetCurrentThreadId(), (unsigned)kStyleWord, (unsigned)kDetailsSty, (unsigned)0x00D50035u,
        rb ? (unsigned)v.Dr0 : 0, rb ? (unsigned)v.Dr2 : 0, rb ? (unsigned)v.Dr7 : 0,
        (rb && v.Dr0 == kStyleWord && v.Dr2 == kRadarHead) ? "(подтверждено)" : "(readback не совпал — доставку проверим по попаданиям)");
    diag_line(b);
    g_wpState = 1;
}
} // namespace (watchpoints)

// (27.10.2026) ПЕРВИЧНОЕ исключение — в журнал ДО тяжёлого report().
// Причина: первым в цепочке VEH стоит этот файл (AddVectoredExceptionHandler
// вызван последним), и если report() упадёт, plugin.cpp-фильтр увидит уже
// ВТОРИЧНОЕ исключение внутри отчёта. Так и терялся настоящий адрес: в журнале
// были только «VECTORED» с EIP внутри lualoader.asi (report+0xE00..0x1100).
// Строка пишется через append_raw — без мьютексов, без кучи, без dbghelp.
void log_firstchance(EXCEPTION_POINTERS* ep, const char* how) {
    if (!ep || !ep->ExceptionRecord) return;
    const EXCEPTION_RECORD* r = ep->ExceptionRecord;
    const CONTEXT* c = ep->ContextRecord;
    static char b[320];
    int n = _snprintf_s(b, sizeof(b), _TRUNCATE,
        "C++| %s: code=0x%08X addr=%p EIP=%08X ESP=%08X EBP=%08X tid=%lu\r\n",
        how, (unsigned)r->ExceptionCode, r->ExceptionAddress,
        c ? (unsigned)c->Eip : 0u, c ? (unsigned)c->Esp : 0u,
        c ? (unsigned)c->Ebp : 0u, (unsigned long)GetCurrentThreadId());
    if (n > 0) append_raw(kLogPath, b, (DWORD)n);
}

LONG WINAPI veh(EXCEPTION_POINTERS* ep) {
    // 64 КБ гарантированных последних страниц стека: у VEH-потока может быть
    // исчерпан стек (RECURSION), и без guarantee отчёт сам упадёт на WriteFile.
    ULONG guarantee = 65536; SetThreadStackGuarantee(&guarantee);
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    // Наши ловушки стиля (INT3/#DB в коде игры) и DR-точки на запись стиля —
    // до общего пропуска точек останова. Обработчики вызываются ОБА: TF-шаг
    // ловушки может одновременно попасть в DR-точку (сайты D/E/F сами пишут
    // 0x94B924) — ft снимает TF и перевооружает INT3, wp логирует и чистит DR6.
    if (code == EXCEPTION_BREAKPOINT || code == EXCEPTION_SINGLE_STEP) {
        int handled = 0;
        if (ft_exception(ep)) handled = 1;
        if (wp_exception(ep)) handled = 1;
        if (handled)
            return EXCEPTION_CONTINUE_EXECUTION;
    }
    // Отладочные/виновые исключения Wine (DBG_PRINT) и точки останова — не краш.
    if (code == 0x40010006 || code == 0x80000003 || code == 0x80000004)
        return EXCEPTION_CONTINUE_SEARCH;
    // 0xE06D7363 — обычный C++ throw. В Wine их тысячи (в т.ч. наши throw "bad argument"),
    // на first chance их ловить нельзя. Реальный необработанный C++ EH поймает UEF.
    if (code == 0xE06D7363)
        return EXCEPTION_CONTINUE_SEARCH;
    log_firstchance(ep, "FIRSTCHANCE");
    report(ep, "VEH (first chance)");
    return EXCEPTION_CONTINUE_SEARCH;   // игру не swallows — пусть упадёт как обычно
}

LONG WINAPI uef(EXCEPTION_POINTERS* ep) {
    // 64 КБ гарантированных последних страниц стека: у UEF-потока может быть
    // исчерпан стек (RECURSION), и без guarantee отчёт сам упадёт на WriteFile.
    ULONG guarantee = 65536; SetThreadStackGuarantee(&guarantee);
    log_firstchance(ep, "UNHANDLED");
    if (!InterlockedCompareExchange(&g_reported, 1, 0)) {
        report(ep, "UnhandledExceptionFilter (последний шанс)");
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void watchdog() {
    while (!g_quit.load()) {
        Sleep(500);
        unsigned long long since = g_inflightSince.load();
        if (since && g_inflightCmd.load()) {
            unsigned long long held = GetTickCount64() - since;
            if (held > 3000) {
                char b[256];
                _snprintf_s(b, sizeof(b), _TRUNCATE,
                    "WATCHDOG: Command<0x%X> висит %llu мс в потоке %lu (поток застрял или игра сдохла)",
                    g_inflightCmd.load(), held, g_inflightThread.load());
                diag_line(b);
            }
        }
        unsigned long long msince = g_modelSince.load();
        if (msince && g_modelId.load() >= 0) {
            unsigned long long held = GetTickCount64() - msince;
            if (held > 5000) {
                char b[256];
                _snprintf_s(b, sizeof(b), _TRUNCATE,
                    "WATCHDOG: модель id=%d грузится %llu мс (поток %lu) — стрим не идёт",
                    g_modelId.load(), held, g_inflightThread.load());
                diag_line(b);
            }
        }
        // Периодический сброс буфера журнала. Нужен для СИЛЬНОГО зависания:
        // если игровой поток стоит колом и до отчёта о вылете дело не доходит,
        // последние строки иначе остались бы в памяти и потерялись. Раз в
        // 500 мс сброс стоит одну запись — на игру не влияет.
        diag_flush();
    }
}

DWORD WINAPI watchdog_thread(LPVOID) { watchdog(); return 0; }

// ⛔ (2026-10-07) ЗДЕСЬ БЫЛ ГЛОБАЛЬНЫЙ КОНСТРУКТОР `AutoInstall g_auto_install`,
// который звал ma_diag::install() ПРИ ЗАГРУЗКЕ DLL — раньше всех прочих TU.
// Это единственный «лишний» инициализатор по сравнению с рабочей сборкой 24.09
// (проверено: nm даёт 31 против 30, добавился ровно _GLOBAL__sub_I_diag.cpp).
//
// Чем это было плохо: install() делает InitializeCriticalSection(&g_fileLock) и
// СРАЗУ поднимает watchdog-поток, который начинает писать в журнал. Инициализаторы
// других TU (cmdq_hooks.o, ma_tasks.o) в это время лочат ещё не инициализированные
// критические секции — в Wine это «RtlpWaitForCriticalSection ... blocked by 0000»,
// то есть игра НЕ СТАРТОВАЛА. Плюс watchdog-поток начинал работать до того, как
// готов сам плагин.
//
// СТАЛО: установщиков обработчиков в этом TU нет вообще. Установку зовёт
// Message в plugin.cpp — там же, где регистрируются хуки Events, и по построению
// после инициализации SDK. Итог: число инициализаторов снова 30, как в рабочей сборке.

} // namespace

// ---------------------------------------------------------------------------
// Публичный интерфейс
// ---------------------------------------------------------------------------
namespace ma_diag {

void install() {
    // ВАЖНО (17.10.2026): install() зовётся из Message (plugin.cpp) при ЗАГРУЗКЕ DLL.
    // Поток загрузки НЕ обязан быть игровым (в этом билде asi грузится отдельным потоком
    // загрузчика — см. комментарий про thread_local в ScriptCommands.h, boot.txt: tid≠pid).
    // Поэтому id игрового потока здесь НЕ фиксируем: его авторитетно выставляет
    // ma_cmdq_drain() при первом разборе очереди (gameProcessEvent = игровой поток).
    // Ручное переопределение — переменной окружения LUALOADER_GAME_THREAD.
    if (const char* e = getenv("LUALOADER_GAME_THREAD")) {
        unsigned v = 0; sscanf(e, "%u", &v); if (v) g_gameThread.store(v);
    }
    CreateDirectoryA("lualoader", nullptr);   // каталог для log.txt
    InitializeCriticalSection(&g_fileLock);
    AddVectoredExceptionHandler(1, veh);      // FIRST: ловим сразу, до разрушения контекста
    // (25.10, директива юзера) INT3-ловушки шрифта и DR-точки ВЫКЛЮЧЕНЫ: они
    // срабатывали на каждую обычную запись стиля шрифта и ЗАЛИВАЛИ log.txt
    // строками FONT-TRAP/WATCH («ерунда»). Журнал — только ошибки и вылеты.
    // font_traps_install();
    // (27.10) Вместо INT3-ловушек — ОДИН тихий детур на write-side проверку:
    // запись длиннее 0x3C0 байт обрезается до безопасной (класс AV 0x551E0D).
    print_clamp_install();
    SetUnhandledExceptionFilter(uef);
    if (!g_watchdogStarted.exchange(true)) {
        HANDLE t = CreateThread(nullptr, 0, &watchdog_thread, nullptr, 0, nullptr);
        if (t) CloseHandle(t);
    }
}

// (25.10) Полный отчёт о вылете: определение — глобальный ::report (выше, строка ~371,
// не в namespace — исторически), объявление в diag.h — в namespace ma_diag.
// Обёртка связывает: вызов ma_diag::report() из обработчиков plugin.cpp.
void report(EXCEPTION_POINTERS* ep, const char* how) {
    ::report(ep, how);
}

void mark(const char* fmt, ...) {
    char buf[200];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = '\0';
    push_crumb(0, 0, buf);
    diag_line(buf);
}

void cmd_begin(unsigned int commandId, int nargs) {
    const unsigned tid = GetCurrentThreadId();
    int s = slot_of(tid);
    g_cmdCount[s].fetch_add(1);
    g_cmdTick[s].store(GetTickCount());
    g_inflightCmd.store(commandId);
    g_inflightThread.store(tid);
    g_inflightSince.store(GetTickCount64());
    char b[120];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "Command<0x%X> начало (арг=%d)", commandId, nargs);
    push_crumb(commandId, nargs, b);
}

void cmd_ctx(unsigned int commandId, const void* script, int ip, unsigned int base) {
    (void)commandId;
    g_scriptAddr.store((unsigned int)(unsigned long)script);
    g_scriptIp.store(ip);
    g_scriptBase.store(base);
}

void cmd_end(const char* res) {
    unsigned int id = g_inflightCmd.exchange(0);
    g_inflightSince.store(0);
    if (!res || !res[0]) return;
    char b[160];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "Command<0x%X> out: %s", id, res);
    push_crumb(id, 0, b);
    // В ФАЙЛ пишем только подозрительные результаты. Команды теперь выполняются на
    // игровом потоке, а diag_line() открывает/закрывает файл — на каждой команде
    // с out-параметром это был бы микрофриз кадра. Мусорный указатель (0, -1,
    // значение меньше 0x10000) — признак того, что VM отдала не то.
    if (strstr(res, "=0x0 ") || strstr(res, "=0x0") || strstr(res, "=0xFFFFFFFF")
        || strstr(res, "=0x1)") || strstr(res, "=0x2)"))
        diag_line(b);
}

// ============================================================================
// ДИАГНОСТИКА «ПРОЦЕСС УМЕР МОЛЧА» (07.10.2026)
// ============================================================================
// ПРОБЛЕМА: 07.10 при подъезде к забастовкам процесс исчез, и в log.txt НЕ БЫЛО
// ни блока LUALOADER CRASH, ни минидампа. То есть обработчик не сработал вообще.
// Причина не в «сложном краше»: VEH ловит и ACCESS_VIOLATION, и
// EXCEPTION_STACK_OVERFLOW (0xC00000FD) — пропускаются только
// breakpoint'ы и C++ throw. Значит исключения не было вовсе: процесс убили
// снаружи, он завис и был закрыт, либо упал ниже уровня SEH.
//
// СЕРДЦЕБИЕНИЕ УБРАНО (24.10.2026): раньше watchdog раз в 2 с писал строку
// HEARTBEAT в журнал. Теперь log.txt содержит только ошибки, поэтому
// периодической записи нет — остаются отчёт о вылете и записи о проблемах
// (зависшая команда, застрявшая модель, ошибка Lua).
//
// Считаем ГЛУБИНУ ВЛОЖЕННОСТИ Command<>. Шаблон Command<> в SDK
// использует ОДИН общий `static CRunningScript script` на весь модуль. Если
// команда вложится в другую команду (реентрантность), внутренний вызов делает
// memset этого объекта и затирает m_nIp внешнего — после чего движок читает
// команды по мусорному указателю. Это тихий порча состояния без исключения.
// Счётчик глубины делает такой случай видимым в журнале.
// ============================================================================

void set_thread_name(const char* name) {
    _snprintf_s(g_threadName[slot_of(GetCurrentThreadId())], 32, _TRUNCATE, "%s", name ? name : "?");
}

void model_event(int model, const char* phase, int elapsedMs) {
    if (phase && strcmp(phase, "wait_start") == 0) {
        g_modelId.store(model);
        g_modelSince.store(GetTickCount64());
    } else {
        g_modelId.store(-1);
        g_modelSince.store(0);
    }
    char b[160];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "МОДЕЛЬ id=%d: %s (%d мс)", model, phase ? phase : "?", elapsedMs);
    push_crumb(0, 0, b);
    diag_line(b);
}

// --- Lua-контекст (18.10.2026) ---------------------------------------------
// Горячий путь: вызывается на КАЖДЫЙ вход из Lua в C-функцию. Никаких файлов,
// никаких мьютексов — только запись в per-thread слот и кольцевую крошку.
void lua_note(const char* file, int line, const char* api) {
    int s = slot_of(GetCurrentThreadId());
    _snprintf_s(g_luaFile[s], kLuaFileMax, _TRUNCATE, "%s", file ? file : "?");
    g_luaLine[s] = line;
    _snprintf_s(g_luaApi[s], 40, _TRUNCATE, "%s", api ? api : "?");
    char b[220];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "LUA: %s:%d -> %s()", file ? file : "?", line, api ? api : "?");
    push_crumb(0, -1, b);   // без diag_line: горячий путь, файл не трогаем
}

void lua_error_note(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(g_luaErr, sizeof(g_luaErr), _TRUNCATE, fmt, ap);
    va_end(ap);
    g_luaErr[sizeof(g_luaErr) - 1] = '\0';
    char b[280];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "LUA ERROR: %s", g_luaErr);
    push_crumb(0, -1, b);
    diag_line(b);   // ошибки редки — файл не жалко
}

void lua_context(char* out, size_t cap) {
    if (!out || !cap) return;
    out[0] = '\0';
    int s = slot_of(GetCurrentThreadId());
    if (!g_luaFile[s][0]) return;
    _snprintf_s(out, cap, _TRUNCATE, "lua=%s:%d api=%s()",
        g_luaFile[s], g_luaLine[s], g_luaApi[s][0] ? g_luaApi[s] : "?");
}

unsigned long worst_stalled_cmd_ms() {
    unsigned long long since = g_inflightSince.load();
    return since ? (unsigned long)(GetTickCount64() - since) : 0;
}

bool looks_like_pointer(const void* p) {
    uintptr_t v = (uintptr_t)p;
    // Мусор из VM: 0, 0xFFFFFFFF (-1), значения меньше 0x10000, либо «1»-подобные.
    return v != 0 && v != 0xFFFFFFFFu && v > 0x10000;
}

} // namespace ma_diag
// Свободная обёртка для SDK-хедера (там namespace ma_diag не объявлен).
void ma_diag_cmd_ctx(unsigned int id, const void* script, int ip, unsigned int base) {
    ma_diag::cmd_ctx(id, script, ip, base);
}

// Снимок ЗНАЧЕНИЙ аргументов команды (зовётся из ScriptCommands.h). Только крошка
// в кольцевом буфере — БЕЗ файла: это горячий путь каждой Command<>.
void ma_diag_cmd_args(unsigned int id, const char* text) {
    if (!text || !text[0]) return;
    char b[220];
    _snprintf_s(b, sizeof(b), _TRUNCATE, "ARGS<0x%X>:%s", id, text);
    push_crumb(id, -1, b);
}


// ---------------------------------------------------------------------------
// Хуки для SDK (ScriptCommands.h)
// ---------------------------------------------------------------------------
// ⛔ (2026-10-07) Раньше здесь был `static std::mutex g_cmd_mx;` — глобальный объект
// с конструктором, т.е. ещё один источник ранней динамической инициализации.
// Теперь он живёт внутри ленивой CmdQueue (см. cmdq() выше): создаётся при первом
// обращении, когда SDK уже инициализирован. Мьютекс остаётся ровно один — на все
// Command<>, как и было.
static std::mutex& cmd_mutex() { return cmdq().cmdMx; }

unsigned long ma_diag_game_thread() { return g_gameThread.load(); }

// --------------------------- ОЧЕРЕДЬ КОМАНД ---------------------------
#ifdef LUALOADER_CMDQ

bool ma_cmdq_on_game_thread() {
    return GetCurrentThreadId() == g_gameThread.load();
}

void ma_cmdq_drain();   // разбор очереди; регистрируется в cmdq_hooks.cpp

// SEH-обёртки здесь НЕТ намеренно: на i686-mingw clang не собирает __try/__except
// с непрямым вызовом внутри (битые assembler-метки __ehtable), а обойти это можно
// только обёрткой на ассемблере. Обёртка не нужна: если команда роняет игру, то
// падает процесс, а отчёт о падении пишет VEH (в нём уже есть id последней команды).

// ВАЖНО: хук НЕ регистрируется отсюда. Список хуков SDK — std::list, а Add()
// делает emplace_back(); вызов += изнутри обхода того же списка портит heap
// (был вылет в msvcrt). Регистрация живёт в cmdq_hooks.cpp, конструктор которого
// выполняется при загрузке DLL, до старта игрового цикла.

void ma_cmdq_drain() {
    // Выполняется на игровом потоке. Обрабатываем всё, что накопилось, за один
    // вызов; лимит нужен, чтобы один скрипт не monopolизировал кадр.
    // ЕДИНСТВЕННАЯ авторитетная точка определения игрового потока: сюда попадаем
    // ТОЛЬКО из Events::gameProcessEvent, т.е. всегда на игровом потоке. Поток
    // загрузки DLL игровым не является, поэтому install() его не фиксирует.
    g_gameThread.store(GetCurrentThreadId());
    int budget = 64;
    for (;;) {
        Req* r = nullptr;
        {
            std::lock_guard<std::mutex> lk(cmdq().mx);
            if (cmdq().q.empty()) break;
            r = cmdq().q.front();
            cmdq().q.pop_front();
        }
        g_qDepth.store((int)cmdq().q.size());

        bool cancelled = false;
        bool waiter = false;
        {
            std::lock_guard<std::mutex> lk(r->mx);
            cancelled = r->cancelled;
            waiter = r->waiter;
        }
        if (cancelled) {           // worker не дождался — out-параметр уже невалиден
            g_qTimeout.fetch_add(1);
            {
                std::lock_guard<std::mutex> lk(r->mx);
                r->done = true;
            }
            r->cv.notify_one();
            delete r;              // ждущий вернулся по таймауту сам — владелец здесь drain
            continue;
        }
        unsigned long long waited = GetTickCount64() - r->queuedAt;
        if (waited > 200) {
            char b[128];
            _snprintf_s(b, sizeof(b), _TRUNCATE, "CMDQ: команда ждала игровой поток %llu мс", waited);
            ma_diag::mark("%s", b);
        }
        const bool ok = r->fn();
        {
            std::lock_guard<std::mutex> lk(r->mx);
            r->result = ok;
            r->done = true;
        }
        r->cv.notify_one();
        // ВЛАДЕЛЕЦ Req: при defer удаляет сам worker (после чтения result), при post —
        // drain. Раньше удаляли ОБА (drain+waiter) → double-free/UAF на каждой команде.
        if (!waiter) delete r;
        if (--budget <= 0) break;
    }
}

int ma_cmdq_pending() {
    std::lock_guard<std::mutex> lk(cmdq().mx);
    return (int)cmdq().q.size();
}

bool ma_cmdq_defer(std::function<bool()> f) {
    if (ma_cmdq_on_game_thread()) {
        // plugin.cpp:246 — игра сама зовёт Command<> из своего же хука. Тут же и
        // выполняем: ставить в очередь бессмысленно (её же сейчас и разбираем).
        return f();
    }

    Req* r = new Req();
    r->fn = std::move(f);
    r->waiter = true;          // Req удалит сам ожидающий (drain не трогает)
    r->queuedAt = GetTickCount64();
    {
        std::lock_guard<std::mutex> lk(cmdq().mx);
        cmdq().q.push_back(r);
        g_qDepth.store((int)cmdq().q.size());
    }
    g_qTotal.fetch_add(1);

    bool done = false;
    {
        std::unique_lock<std::mutex> lk(r->mx);
        done = r->cv.wait_for(lk, std::chrono::milliseconds(kCmdTimeoutMs), [&] { return r->done; });
    }
    if (!done) {
        // Таймаут: очередь не разбирается (игра на паузе/в меню). Помечаем запрос
        // отменённым — выполнять его нельзя, out-параметр уже вне области видимости.
        std::lock_guard<std::mutex> lk(r->mx);
        r->cancelled = true;
        r->done = true;
        g_qTimeout.fetch_add(1);
        unsigned long long waited = GetTickCount64() - r->queuedAt;
        unsigned long long prev = g_qMaxWait.load();
        while (waited > prev && !g_qMaxWait.compare_exchange_weak(prev, waited)) {}
        ma_diag::mark("CMDQ: ТАЙМАУТ %llu мс — команда не выполнена (очередь не разбирается?)", waited);
        return false;
    }
    unsigned long long waited = GetTickCount64() - r->queuedAt;
    unsigned long long prev = g_qMaxWait.load();
    while (waited > prev && !g_qMaxWait.compare_exchange_weak(prev, waited)) {}
    bool res;
    {
        std::lock_guard<std::mutex> lk(r->mx);
        res = r->result;
    }
    delete r;
    return res;
}

void ma_cmdq_post(std::function<void()> f) {
    // Fire-and-forget: выполнить на ИГРОВОМ потоке, не ожидая. Для функций-сеттеров
    // без возврата (сообщения/шрифт и т.п.). Лямбда ОБЯЗАНА владеть копиями данных
    // (строк и т.д.) — она живёт до разбора очереди. Порядок — FIFO.
    if (ma_cmdq_on_game_thread()) {
        f();                   // уже игровой поток — выполняем сразу
        return;
    }
    Req* r = new Req();
    std::function<void()> fncopy = std::move(f);
    r->fn = [fncopy]() -> bool { fncopy(); return true; };
    r->queuedAt = GetTickCount64();
    {
        std::lock_guard<std::mutex> lk(cmdq().mx);
        cmdq().q.push_back(r);
        g_qDepth.store((int)cmdq().q.size());
    }
    g_qTotal.fetch_add(1);
}

#endif // LUALOADER_CMDQ

bool ma_diag_enter(unsigned int id, int nargs) {
    const unsigned tid = GetCurrentThreadId();
    if (tid == g_gameThread.load()) {
        // Игровой поток НЕ блокируется: иначе deadlock (скрипт держит мьютекс и ждёт
        // игру, а игра ждёт мьютекс). Пропускаем команду в этом кадре.
        if (!cmd_mutex().try_lock()) {
            g_skipped.fetch_add(1);
            char b[128];
            _snprintf_s(b, sizeof(b), _TRUNCATE,
                "Command<0x%X> ПРОПУЩЕНА игровым потоком (мьютекс держит %lu)",
                id, g_lockOwner.load());
            push_crumb(id, nargs, b);
            return false;
        }
    } else {
        cmd_mutex().lock();
    }
    // ГЛУБИНА ВЛОЖЕННОСТИ (07.10.2026). Шаблон Command<> в SDK держит ОДИН общий
    // `static CRunningScript script`. Вложенный вызов команды в команду делает
    // memset этого объекта и затирает m_nIp внешнего — движок потом читает команды
    // по мусорному указателю (тихая порча, без исключения). Считаем глубину, чтобы
    // такой случай был виден в журнале, а не проявлялся через 40 секунд как «миссия
    // упала сама».
    const int depth = g_cmdDepth.fetch_add(1) + 1;
    if (depth > g_cmdMaxDepth.load()) g_cmdMaxDepth.store(depth);
    if (depth > 1) {
        // Реентерентность: общий static CRunningScript в SDK затрётся внутренним memset.
        // Отменяем инкремент, освобождаем мьютекс и пропускаем команду — иначе движок
        // пойдёт по мусорному m_nIp после возврата из вложенного вызова.
        g_cmdDepth.fetch_sub(1);
        g_lockOwner.store(0);
        g_lockSince.store(0);
        cmd_mutex().unlock();
        g_reentrancyHits.fetch_add(1);
        char rb[128];
        _snprintf_s(rb, sizeof(rb), _TRUNCATE,
            "REENTRANCY: Command<0x%X> вложена в другую команду (глубина=%d) — ПРОПУЩЕНА", id, depth);
        push_crumb(id, nargs, rb);
        return false;
    }
    g_lockOwner.store(tid);
    g_lockSince.store(GetTickCount64());
    ma_diag::cmd_begin(id, nargs);
    return true;
}

void ma_diag_exit(unsigned int id, const char* res) {
    int depth = g_cmdDepth.load() - 1;
    g_cmdDepth.store(depth < 0 ? 0 : depth);
    ma_diag::cmd_end(res);
    // Разблокировать мьютекс только если этот поток его залочил (enter вернул true).
    // При try_lock-отказе (игровой поток пропустил команду) мьютекс не был залочен —
    // unlock чужого мьютекса — это undefined behavior.
    if (g_lockOwner.load() == GetCurrentThreadId()) {
        g_lockOwner.store(0);
        g_lockSince.store(0);
        cmd_mutex().unlock();
    }
}

// ---------------------------------------------------------------------------
// СТОРОЖ СТИЛЯ ШРИФТА + ВРЕМЯ ЖИЗНИ ТЕКСТА СООБЩЕНИЙ (18.10.2026)
//
// Доказанная цепочка вылета 0x551E0D (дамп crash_280_*, арифметика бит-в-бит):
//   CFont::PrintChar -> movsx eax, word[edx*4+0x696D7A], где edx = FontStyle*105
//   в дампе edx = 0xFFD7563C = 105 * (-25380)  =>  FontStyle = 0x9CDC
//   адрес данных 0xFFD7563C*4 + 0x696D7A = 0xFFC6C66A — совпадает с EXCEPTION_RECORD.
// Details.m_FontStyle (0x97F858) пишет ТОЛЬКО CFont::SetFontStyle (0x54FFE0):
// значения 0..2 → 1 (флаговое поле 0x97F85A=1), >2 → как есть.
// (25.10, дизасм gta-vc.exe): RenderState.FontStyle (0x94B924) game пишет САМА —
// CFont::InitPerFrame (0x5522B0) каждый кадр `or word [0x94B924], -1` (сентинель
// «стиль не установлен»), дальше значение приходит копированием состояния.
// Поэтому used == -1 — штатно, guard его НЕ трогает (см. font_style_guard).
// Сторож ниже ловит мусор (значения вне 0..5, кроме -1) и обнуляет его ДО
// отрисовки кадра: вылет превращается в строку журнала FONT: с значениями
// и контекстом Lua. Игру больше ничем не меняет.
// ---------------------------------------------------------------------------
namespace ma_diag {

namespace {

// Сколько буферов текста держим одновременно. Сообщений на экране единицы;
// 96 — с большим запасом (очередь больших сообщений + «previous brief»).
const int kMsgKeepSlots = 96;
// Запас сверх time: сообщение может ждать в очереди (AddBigMessageQ) и
// перерисовываться в следующем кадре после истечения time.
const unsigned long long kMsgKeepExtraMs = 15000;

struct MsgKeepSlot {
    wchar_t*           text;
    unsigned long long expire;   // GetTickCount64(), после — free
};

MsgKeepSlot g_msgKeep[kMsgKeepSlots];

// Стили, которые игра реально использует: 0..2 — маленькие/большие сообщения,
// 3..5 — слоты m_BigMessage[6]; всё вне 0..5 — уже мусор (в таблице глифов
// 3 стиля, в сообщениях 6 слотов).
const short kFontStyleMax = 5;

short g_lastBadWant = -1;
short g_lastBadUsed = -1;

} // namespace

void font_style_guard() {
    // (25.10, директива юзера) DR-точки НЕ вооружаем: их обработчик писал в log.txt
    // поток строк WATCH/FONT-TRAP на каждую обычную запись стиля («ерунда»).
    // Сам guard ниже остаётся: клампит мусорный стиль и пишет ОДНУ строку только
    // при настоящей аномалии.
    // if (g_wpState == 0) wp_arm();
    // Адреса через SDK: CFont::Details = 0x97F820 (+0x38 = 0x97F858),
    // CFont::RenderState = 0x94B8F8 (+0x2C = 0x94B924). Проверены дизасмом.
    short& want = CFont::Details.m_FontStyle;     // «запрошенный» стиль
    short& used = CFont::RenderState.FontStyle;   // «применённый» стиль (его читает PrintChar)

    // (25.10, дизасм gta-vc.exe) CFont::InitPerFrame (0x5522B0) КАЖДЫЙ КАДР делает
    // `or word [0x94B924], -1` — used == -1 это штатный сентинель «стиль ещё не
    // установлен в этом кадре», а НЕ мусор. Раньше guard считал -1 мусором:
    // логил «FONT: МУСОРНЫЙ СТИЛЬ ... used=-1» и обнулял поле — то есть перебивал
    // собственное состояние шрифта игры каждый кадр (ветка рисованного текста
    // `if (style==-1)` в CFont шла не так). Теперь -1 пропускается молча;
    // клампится только настоящий мусор (другие значения вне 0..5 — класс 0x551E0D).
    const bool badWant = (want < 0 || want > kFontStyleMax);
    const bool badUsed = (used != -1) && (used < 0 || used > kFontStyleMax);
    if (!badWant && !badUsed) {
        g_lastBadWant = -1;
        g_lastBadUsed = -1;
        return;
    }

    // Пишем только при СМЕНЕ мусорного значения, иначе горячий путь (каждый кадр)
    // зальёт журнал.
    if (want != g_lastBadWant || used != g_lastBadUsed) {
        g_lastBadWant = want;
        g_lastBadUsed = used;
        char ctx[160];
        lua_context(ctx, sizeof(ctx));
        ma_diag::mark("FONT: МУСОРНЫЙ СТИЛЬ want=%d used=%d (t=%u мс, tid=%lu) %s",
                      (int)want, (int)used, CTimer::m_snTimeInMilliseconds,
                      (unsigned long)GetCurrentThreadId(), ctx);
    }
    if (badWant) want = 0;
    if (badUsed) used = 0;
}

void radar_list_guard() {
    const DWORD kList = 0x704858;
    const DWORD* base = (const DWORD*)(uintptr_t)kList;
    if (base[0x40 / 4] == 0) return;          // список ещё не сконструирован
    volatile DWORD* head = (volatile DWORD*)(uintptr_t)(kList + 0xC);
    if (*head != 0) return;
    *head = kList + 0x10;                     // пустой список: head = sentinel
    static bool logged = false;
    if (!logged) {
        logged = true;
        char ctx[160];
        lua_context(ctx, sizeof(ctx));
        ma_diag::mark("RADAR-LIST: head=NULL -> sentinel 0x704868 (t=%u мс, tid=%lu) %s",
                      CTimer::m_snTimeInMilliseconds, (unsigned long)GetCurrentThreadId(), ctx);
    }
}

void intro_text_guard() {
    // CTheScripts::IntroTextLines (v1.0) — адрес из SDK, размеры tScriptText (SDK
    // VALIDATE_OFFSET): font int32 на +0x20 (движок читает младшее слово), флаги
    // 0x0C..0x0F и 0x1C..0x1E, xPosition +0x24, yPosition +0x28, wchar text[100] +0x2C.
    unsigned char* const kBase = (unsigned char*)0x7F0EA0;
    static short g_lastBadFont[48];
    static bool  g_seenActive[48];
    static bool  g_init = false;
    if (!g_init) {
        for (int i = 0; i < 48; i++) { g_lastBadFont[i] = -1; g_seenActive[i] = false; }
        g_init = true;
    }
    for (int i = 0; i < 48; i++) {
        unsigned char* rec = kBase + (size_t)i * 0xF4;
        wchar_t* text = (wchar_t*)(rec + 0x2C);
        if (text[0] == 0) continue;          // строка не активна — движок её не рисует
        if (!g_seenActive[i]) {
            // В этой миссии intro-строк быть НЕ ДОЛЖНО: любая активация — событие.
            g_seenActive[i] = true;
            char ctx[160];
            lua_context(ctx, sizeof(ctx));
            ma_diag::mark("INTRO: СТРОКА АКТИВИРОВАНА i=%d font=%d (t=%u мс, tid=%lu) %s",
                          i, (int)*(short*)(rec + 0x20), CTimer::m_snTimeInMilliseconds,
                          (unsigned long)GetCurrentThreadId(), ctx);
        }
        short font = *(short*)(rec + 0x20);
        if (font >= 0 && font <= kFontStyleMax) continue;   // штатный стиль
        if (g_lastBadFont[i] == font) continue;             // то же мусорное значение уже писали
        g_lastBadFont[i] = font;
        char hex[80];
        int n = 0;
        for (int k = 0; k < 10 && text[k] && n < (int)sizeof(hex) - 8; k++)
            n += sprintf(hex + n, "%04X ", (unsigned)text[k]);
        hex[n] = 0;
        char ctx[160];
        lua_context(ctx, sizeof(ctx));
        ma_diag::mark("INTRO: МУСОРНЫЙ ШРИФТ i=%d font=%d флаги=%d,%d,%d,%d,%d,%d,%d x=0x%08X y=0x%08X текст: %s (t=%u мс, tid=%lu) %s",
                      i, (int)font,
                      rec[0x0C], rec[0x0D], rec[0x0E], rec[0x0F], rec[0x1C], rec[0x1D], rec[0x1E],
                      *(unsigned int*)(rec + 0x24), *(unsigned int*)(rec + 0x28), hex,
                      CTimer::m_snTimeInMilliseconds, (unsigned long)GetCurrentThreadId(), ctx);
        *(int*)(rec + 0x20) = 1;   // карантин: FONT_STANDARD — движок отрисует без AV
    }
}

void msg_keepalive(wchar_t* text, unsigned int timeMs) {
    if (!text) return;
    const unsigned long long now = GetTickCount64();
    const unsigned long long expire = now + (unsigned long long)timeMs + kMsgKeepExtraMs;

    int slot = -1;
    for (int i = 0; i < kMsgKeepSlots; i++) {
        if (!g_msgKeep[i].text) { slot = i; break; }
    }
    if (slot < 0) {
        // Все слоты заняты — освобождаем самый старый. Без этого «вечные» сообщения
        // (time=0) копили бы память; 96 слотов при обычной игре не достигаются.
        slot = 0;
        for (int i = 1; i < kMsgKeepSlots; i++) {
            if (g_msgKeep[i].expire < g_msgKeep[slot].expire) slot = i;
        }
        delete[] g_msgKeep[slot].text;
    }
    g_msgKeep[slot].text = text;
    g_msgKeep[slot].expire = expire;
}

void msg_keepalive_purge() {
    const unsigned long long now = GetTickCount64();
    for (int i = 0; i < kMsgKeepSlots; i++) {
        if (g_msgKeep[i].text && g_msgKeep[i].expire <= now) {
            delete[] g_msgKeep[i].text;
            g_msgKeep[i].text = nullptr;
        }
    }
}

// ===========================================================================
// (27.10.2026) ДИАГНОСТИКА ПОРЧИ КУЧИ И СТЕК БЕЗ ВЫДЕЛЕНИЯ ПАМЯТИ
// ===========================================================================
// Зачем: все свежие вылеты — AV ВНУТРИ msvcrt (0x772F1AB0, 0x772E1C00,
// 0x772A9E60), т.е. порча кучи случилась раньше, а Lua-строка в журнале лишь
// называет последний вход в API. report() в VEH сам ходит в Toolhelp/dbghelp
// (выделяет память) и на испорченной куче обрывается — в log.txt оставалась
// одна строка VECTORED. Здесь всё наоборот: только статический буфер,
// _snprintf и WriteFile.

namespace {

int addf(char* b, int n, size_t cap, const char* fmt, ...) {
    if (n < 0) n = 0;
    if ((size_t)n >= cap - 1) return (int)cap - 2;
    va_list ap; va_start(ap, fmt);
    int r = _vsnprintf(b + n, cap - (size_t)n - 1, fmt, ap);
    va_end(ap);
    if (r < 0) return n;
    if ((size_t)(n + r) >= cap - 1) return (int)cap - 2;
    return n + r;
}

// «msvcrt.dll+0x1C00» без единого выделения памяти.
void module_of(void* addr, char* out, size_t cap) {
    if (!out || cap < 8) return;
    out[0] = '\0';
    HMODULE h = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           (LPCSTR)addr, &h) && h) {
        char path[MAX_PATH]; path[0] = '\0';
        DWORD n = GetModuleFileNameA(h, path, MAX_PATH);
        const char* base = path;
        if (n) {
            for (DWORD i = 0; i < n; i++)
                if (path[i] == '\\' || path[i] == '/') base = path + i + 1;
        }
        _snprintf(out, cap, "%s+0x%X", n ? base : "?", (unsigned)((char*)addr - (char*)h));
        out[cap - 1] = '\0';
    } else {
        _snprintf(out, cap, "%p", addr);
        out[cap - 1] = '\0';
    }
}

// Код ли это (для «сырого» скана стека): страница исполняемая.
bool looks_code(void* addr) {
    MEMORY_BASIC_INFORMATION mi;
    if (!VirtualQuery(addr, &mi, sizeof(mi))) return false;
    if (mi.State != MEM_COMMIT) return false;
    DWORD p = mi.Protect & 0xFF;
    return p == PAGE_EXECUTE || p == PAGE_EXECUTE_READ ||
           p == PAGE_EXECUTE_READWRITE || p == PAGE_EXECUTE_WRITECOPY;
}

} // namespace

bool heap_check(const char* tag) {
    static ULONGLONG last = 0;
    ULONGLONG now = GetTickCount64();
    if (tag == NULL) {
        if (now - last < 1000) return true;   // периодическая — не чаще раза в секунду
    }
    last = now;

    // Крошка ДО проверки: если _heapchk сам упадёт на испорченной куче,
    // в отчёте/журнале останется имя контрольной точки.
    char crumb[120];
    _snprintf(crumb, sizeof(crumb), "HEAPCHECK-BEGIN %s", tag ? tag : "(периодически)");
    crumb[sizeof(crumb) - 1] = '\0';
    push_crumb(0, 0, crumb);

    if (tag != NULL) {
        char line[160];
        int n = _snprintf(line, sizeof(line), "C++| HEAPCHECK-BEGIN %s\n", tag);
        if (n > 0) append_raw(kLogPath, line, (DWORD)n);
    }

    int r = _heapchk();
    if (r == _HEAPOK || r == _HEAPEMPTY) {
        if (tag != NULL) {
            char line[160];
            int n = _snprintf(line, sizeof(line), "C++| HEAPCHECK-OK %s\n", tag);
            if (n > 0) append_raw(kLogPath, line, (DWORD)n);
        }
        return true;
    }

    // Порча найдена ВПЕРВЫЕ (или повторно) — пишем всегда, это главный факт.
    char line[192];
    int n = _snprintf(line, sizeof(line),
        "C++| HEAPCHECK-BAD code=%d (%s) после: %s\n", r,
        r == _HEAPBADBEGIN ? "BADBEGIN" : r == _HEAPBADNODE ? "BADNODE" :
        r == _HEAPEND ? "BADEND" : r == _HEAPEMPTY ? "EMPTY" : "?",
        tag ? tag : "(периодически)");
    if (n > 0) append_raw(kLogPath, line, (DWORD)n);
    return false;
}

void dump_stack_noheap(EXCEPTION_POINTERS* ep, const char* how) {
    if (!ep || !ep->ExceptionRecord || !ep->ContextRecord) return;
    EXCEPTION_RECORD* rec = ep->ExceptionRecord;
    CONTEXT* c = ep->ContextRecord;

    static char out[4096];
    int n = 0;
    char mod[160];

    unsigned acc = 0, bad = 0;
    if (rec->NumberParameters >= 2) {
        acc = (unsigned)rec->ExceptionInformation[0];
        bad = (unsigned)rec->ExceptionInformation[1];
    }
    module_of((void*)c->Eip, mod, sizeof(mod));
    n = addf(out, n, sizeof(out),
        "C++| %s: fault EIP=%08X (%s) %s addr=%08X\r\n",
        how ? how : "STACK", (unsigned)c->Eip, mod,
        acc == 1 ? "WRITE" : acc == 0 ? "READ" : "EXEC", bad);
    n = addf(out, n, sizeof(out),
        "  regs eax=%08X ebx=%08X ecx=%08X edx=%08X esi=%08X edi=%08X ebp=%08X esp=%08X\r\n",
        (unsigned)c->Eax, (unsigned)c->Ebx, (unsigned)c->Ecx, (unsigned)c->Edx,
        (unsigned)c->Esi, (unsigned)c->Edi, (unsigned)c->Ebp, (unsigned)c->Esp);

    // Байты по EIP — distinguishes «вызов через битый указатель» от «чтения данных».
    n = addf(out, n, sizeof(out), "  code@eip:");
    for (int i = 0; i < 16; i++) n = addf(out, n, sizeof(out), " %02X", (unsigned)((unsigned char*)c->Eip)[i]);

    // Цепочка EBP (msvcrt/игровые модули собраны с frame pointer).
    n = addf(out, n, sizeof(out), "\r\n  ebp-chain:\r\n");
    DWORD ebp = (DWORD)c->Ebp;
    for (int i = 0; i < 16 && ebp; i++) {
        MEMORY_BASIC_INFORMATION mi;
        if (!VirtualQuery((void*)ebp, &mi, sizeof(mi)) || mi.State != MEM_COMMIT) break;
        if ((DWORD)((char*)mi.BaseAddress + mi.RegionSize - (char*)ebp) < 8) break;
        DWORD prev = ((DWORD*)ebp)[0];
        DWORD ret  = ((DWORD*)ebp)[1];
        if (!ret) break;
        module_of((void*)ret, mod, sizeof(mod));
        n = addf(out, n, sizeof(out), "    #%d ret=%08X %s (frame %08X)\r\n", i, ret, mod, ebp);
        if (prev <= ebp || prev - ebp > 0x20000) break;   // стек растёт вниз
        ebp = prev;
    }

    // «Сырой» скан стека: адреса возврата на месте даже без frame pointer.
    // Печатаем только то, что лежит в исполняемой странице.
    n = addf(out, n, sizeof(out), "  stack-scan (code words):\r\n");
    MEMORY_BASIC_INFORMATION mi;
    if (VirtualQuery((void*)c->Esp, &mi, sizeof(mi)) && mi.State == MEM_COMMIT) {
        DWORD avail = (DWORD)((char*)mi.BaseAddress + mi.RegionSize - (char*)c->Esp);
        int words = (int)(avail / 4);
        if (words > 160) words = 160;
        for (int i = 0; i < words; i++) {
            DWORD v = ((DWORD*)c->Esp)[i];
            if (v < 0x10000) continue;
            if (!looks_code((void*)v)) continue;
            module_of((void*)v, mod, sizeof(mod));
            n = addf(out, n, sizeof(out), "    [esp+%03X] %08X %s\r\n", i * 4, v, mod);
            if ((size_t)n > sizeof(out) - 200) break;
        }
    }

    if (n > 0) append_raw(kLogPath, out, (DWORD)n);
}

void dump_modules_once() {
    static bool done = false;
    if (done) return;
    done = true;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (snap == INVALID_HANDLE_VALUE) return;
    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    static char buf[1024];
    int n = 0;
    if (Module32First(snap, &me)) {
        do {
            if (n > (int)sizeof(buf) - 200) {
                append_raw(kLogPath, buf, (DWORD)n);
                n = 0;
            }
            n = addf(buf, n, sizeof(buf), "MOD %08X %08X %s\r\n",
                     (unsigned)(DWORD_PTR)me.modBaseAddr, (unsigned)me.modBaseSize, me.szModule);
        } while (Module32Next(snap, &me));
    }
    CloseHandle(snap);
    if (n > 0) append_raw(kLogPath, buf, (DWORD)n);
}

} // namespace ma_diag
