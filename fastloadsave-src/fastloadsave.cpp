/*
    fastloadsave.asi — автоматическая загрузка указанного слота сохранения
    в GTA Vice City 1.0 EN через ШТАТНЫЙ обработчик меню.

    НЕ эмулирует клавиши, НЕ зависит от LuaLoader.

    ───────────────────────────────────────────────────────────────────────────
    ПОЧЕМУ НЕ Events::gameProcessEvent:
      lualoader.asi загружается позже (по алфавиту) и перезаписывает хук SDK
      на 0x48E49B, поэтому наш обработчик кадра не вызывался.
      Поэтому используется собственный поток-наблюдатель.

    ШТАТНЫЙ ПУТЬ ЗАГРУЗКИ (дизассембл gta-vc.exe, функция 0x497148):
      Это thiscall-обработчик пунктов меню (ecx = this = 0x869630).
      При m_nCurrentMenuPage == 12 он переходит на 0x497306:
        0x497306  push [ebp+0x100]        ; m_nCurrentSaveSlot
        0x49730C  call 0x61A6C0           ; CheckSlotDataValid(слот)
                                          ; (и запись имени файла в 0xA0CE9C)
        0x497346  mov  byte [ebp+0x11],1
        0x49734C  mov  byte [ebp+0x39],1  ; m_bWantToRestart = 1
        0x497350..0x497374  звук/сброс массива 0x862460
        0x497378  call 0x498E5F           ; SwitchMenuOnAndOff() — ЗАКРЫТЬ МЕНЮ
        0x497388  call 0x5F98D0
        0x49738D  mov  byte [ebp+0x3C],1  ; m_bWantToLoad = 1

      Именно вызов SwitchMenuOnAndOff() отсутствовал в первой версии:
      меню оставалось активным (m_bMenuActive=1) с затемнением fade=255,
      поэтому после загрузки был чёрный экран.

    ИМЯ ФАЙЛА: CheckSlotDataValid @0x61A6C0 строит
      sprintf(buf, "%s%i%s", префикс@0x97509C, слот+1, ".b")
      и копирует его в 0xA0CE9C, откуда игра грузит состояние (0x61A700).
      С portablegta.asi префикс = "userfiles\\GTAVCsf" → userfiles/GTAVCsf1.b.

    Сборка: ./build.sh  →  ../fastloadsave.asi
*/
#define _CRT_SECURE_NO_WARNINGS
#include <plugin.h>
#include <CMenuManager.h>
#include <GenericGameStorage.h>
#include <C_PcSave.h>
#include <CGame.h>

#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

using namespace plugin;

static const char* const FLS_LOG = "fastloadsave.log";
static const char* const FLS_INI = "fastloadsave.ini";

// Точный адрес обработчика пунктов меню (1.0 EN), см. разбор выше.
static const uintptr_t MENU_ENTRY_HANDLER = 0x497148;
// Страница, на которой меню выполняет загрузку слота (ветка 0x497306).
static const int MENU_PAGE_LOAD = 12;

// ----------------------- лог через Win32 API -------------------------------
static CRITICAL_SECTION g_log_cs;
static bool g_log_cs_ready = false;

static void fls_win32_log(const char* s) {
    HANDLE h = CreateFileA(FLS_LOG, FILE_APPEND_DATA,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                           OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    SetFilePointer(h, 0, NULL, FILE_END);
    DWORD n = 0;
    WriteFile(h, s, (DWORD)std::strlen(s), &n, NULL);
    CloseHandle(h);
}

static void fls_logf(const char* fmt, ...) {
    char buf[768];
    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (len <= 0) return;
    char line[1024], stamp[32];
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    if (t) strftime(stamp, sizeof(stamp), "%H:%M:%S", t);
    else std::strcpy(stamp, "??:??:??");
    _snprintf_s(line, sizeof(line), _TRUNCATE, "[%s] %s\n", stamp, buf);
    if (g_log_cs_ready) EnterCriticalSection(&g_log_cs);
    fls_win32_log(line);
    if (g_log_cs_ready) LeaveCriticalSection(&g_log_cs);
}

static inline int fls_sprites()    { return *reinterpret_cast<int*>(0x86971C); }
static inline int fls_game_state() { return *reinterpret_cast<int*>(0x9B5F08); }
static inline int fls_menu_active(){ return *reinterpret_cast<unsigned char*>(0x869668); }
static inline int fls_err()        { return *reinterpret_cast<int*>(0x9753C4); }

static DWORD WINAPI fls_watchdog(LPVOID) {
    Sleep(1500);   // дать CRT/SDK/игре завершить инициализацию
    fls_logf("=== fastloadsave.asi запущен (сборка %s %s) ===", __DATE__, __TIME__);

    int slot = GetPrivateProfileIntA("FastLoadSave", "Slot", 1, FLS_INI);
    int on   = GetPrivateProfileIntA("FastLoadSave", "Enabled", 1, FLS_INI);
    if (!on) { fls_logf("Отказ: Enabled=0."); return 0; }
    if (slot < 1 || slot > 8) {
        fls_logf("Отказ: Slot=%d вне диапазона 1..8.", slot);
        return 0;
    }
    int index = slot - 1;   // игра индексирует слоты с нуля
    fls_logf("Конфиг: Slot=%d (индекс %d). Ожидаю готовности меню...", slot, index);

    int stable = 0, ticks = 0;
    for (;;) {
        Sleep(100);
        ++ticks;
        if (fls_sprites() == 0) {
            if (ticks % 40 == 0)
                fls_logf("  ожидание: sprites=0, gGameState=%d", fls_game_state());
            continue;
        }
        if (++stable < 5) continue;

        fls_logf("Меню готово: gGameState=%d, m_bMenuActive=%d.",
                 fls_game_state(), fls_menu_active());

        // ВАЖНО: вызывать игровые функции (0x497148, SwitchMenuOnAndOff)
        // из фонового потока НЕЛЬЗЯ — игра падает. Поэтому делаем только
        // безопасные записи в память, полностью повторяющие состояние,
        // которое оставляет нативный обработчик загрузки:
        //   0x497306  CheckSlotDataValid(slot)   -> имя файла в 0xA0CE9C
        //   0x497346  m_bShutDownFrontEndRequested = 1
        //   0x49734C  m_bWantToRestart = 1
        //   0x49738D  m_bWantToLoad = 1
        bool valid = CheckSlotDataValid(index);   // проверено: работает из потока
        fls_logf("CheckSlotDataValid(%d) -> %d, err=%d", index, valid ? 1 : 0, fls_err());
        if (!valid) {
            fls_logf("Слот %d пуст/повреждён — загрузку не производим.", slot);
            return 0;
        }

        // Закрываем фронт-энд «мягко» через родной флаг запроса,
        // а не вызовом функции: игра сама обработает его в своём потоке.
        *reinterpret_cast<unsigned char*>(0x869641) = 1; // m_bShutDownFrontEndRequested
        *reinterpret_cast<unsigned char*>(0x869669) = 1; // m_bWantToRestart
        *reinterpret_cast<unsigned char*>(0x86966C) = 1; // m_bWantToLoad
        fls_logf("Флаги загрузки выставлены (ShutDownFrontEnd, "
                 "WantToRestart, WantToLoad).");

        // Ждём, пока игра съест флаги.
        for (int i = 0; i < 80 && FrontEndMenuManager.m_bWantToLoad; ++i)
            Sleep(50);
        Sleep(500);
        fls_logf("После загрузки: gGameState=%d, m_bMenuActive=%d, "
                 "m_bGameNotLoaded=%d, fade=%d/%d, err=%d",
                 fls_game_state(), fls_menu_active(),
                 *reinterpret_cast<unsigned char*>(0x86969C) ? 1 : 0,
                 *reinterpret_cast<int*>(0x869738),
                 *reinterpret_cast<int*>(0x8696B0), fls_err());

        // Снимаем чёрное затемнение, оставшееся от меню, чтобы был виден мир.
        *reinterpret_cast<int*>(0x869738) = 0;   // m_nMenuFadeAlpha
        *reinterpret_cast<int*>(0x8696B0) = 0;   // m_nMenuFadeAlpha2
        *reinterpret_cast<unsigned char*>(0x869740) = 0; // m_bMenuChangeOngoing
        fls_logf("Итог: gGameState=%d, m_bMenuActive=%d, fade=%d/%d, err=%d",
                 fls_game_state(), fls_menu_active(),
                 *reinterpret_cast<int*>(0x869738),
                 *reinterpret_cast<int*>(0x8696B0), fls_err());
        return 0;   // только один раз за запуск
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        HANDLE h = CreateFileA(FLS_LOG, GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h != INVALID_HANDLE_VALUE) {
            static const char m[] = "=== fastloadsave.asi: DllMain ===\n";
            DWORD n = 0;
            WriteFile(h, m, (DWORD)sizeof(m) - 1, &n, NULL);
            CloseHandle(h);
        }
        DisableThreadLibraryCalls(hModule);
        InitializeCriticalSection(&g_log_cs);
        g_log_cs_ready = true;
        CreateThread(NULL, 0, fls_watchdog, NULL, 0, NULL);
    } else if (reason == DLL_PROCESS_DETACH) {
        if (g_log_cs_ready) { DeleteCriticalSection(&g_log_cs); g_log_cs_ready = false; }
    }
    return TRUE;
}
