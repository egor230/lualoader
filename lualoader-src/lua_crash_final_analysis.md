# Ð“Ð»ÑƒÐ±Ð¾ÐºÐ¸Ð¹ Ð°Ð½Ð°Ð»Ð¸Ð· Ð½ÐµÐ²Ð¸Ð´Ð¸Ð¼Ð¾Ð³Ð¾ ÐºÑ€Ð°ÑˆÐ° Ð¿Ñ€Ð¸ Ð¿Ð°ÑƒÐ·Ðµ Lua-ÑÐºÑ€Ð¸Ð¿Ñ‚Ð¾Ð² (reVC + Wine)

## ÐšÐ Ð˜Ð¢Ð˜Ð§Ð•Ð¡ÐšÐ˜Ð™ ÐšÐžÐÐ¢Ð•ÐšÐ¡Ð¢

### ÐÑ€Ñ…Ð¸Ñ‚ÐµÐºÑ‚ÑƒÑ€Ð°
- **5 Lua-ÑÐºÑ€Ð¸Ð¿Ñ‚Ð¾Ð²**, ÐºÐ°Ð¶Ð´Ñ‹Ð¹ Ð² Ð¾Ñ‚Ð´ÐµÐ»ÑŒÐ½Ð¾Ð¼ `std::thread` (pump_owner)
**504 Ð²Ñ‹Ð·Ð¾Ð²Ð° `Command<COMMAND_*>`** Ð¸Ð· worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð² (Ð¿Ð°Ñ€Ð°Ð»Ð»ÐµÐ»ÑŒÐ½Ð¾ Ð¸Ð³Ñ€Ð¾Ð²Ð¾Ð¼Ñƒ Ð¿Ð¾Ñ‚Ð¾ÐºÑƒ)
**reVC** (reverse-engineered GTA VC) Ð¿Ð¾Ð´ **Wine 9+** Ð½Ð° Linux
**Lua 5.4.9** ÑÑ‚Ð°Ñ‚Ð¸Ñ‡ÐµÑÐºÐ°Ñ ÑÐ±Ð¾Ñ€ÐºÐ°

Ð§Ñ‚Ð¾ Ð¸ÑÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¾ (2-Ñ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ñ)
âœ… Reload-Ð¿Ð¾Ñ‚Ð¾Ðº ÐÐ• Ð¿Ð¸ÑˆÐµÑ‚ Ð¸Ð³Ñ€Ð¾Ð²ÑƒÑŽ Ð¿Ð°Ð¼ÑÑ‚ÑŒ Ð½Ð°Ð¿Ñ€ÑÐ¼ÑƒÑŽ
Ð’Ð²ÐµÐ´ÐµÐ½Ñ‹ Ñ„Ð»Ð°Ð³Ð¸-Ð·Ð°Ð¿Ñ€Ð¾ÑÑ‹ (`pause_requested`, `resume_requested`)
Ð—Ð°Ð¿Ð¸ÑÑŒ `ScriptSpace[OnAMissionFlag]` Ñ‚Ð¾Ð»ÑŒÐºÐ¾ `gameProcessEvent` (Ð¸Ð³Ñ€Ð¾Ð²Ð¾Ð¹ Ð¿Ð¾Ñ‚Ð¾Ðº)
Bounds-guard `< 260512` `OnAMissionFlag`
`SetUnhandledExceptionFilter` ÑƒÑÑ‚Ð°Ð½Ð¾Ð²Ð»ÐµÐ½
`load_model_before_avalible` ÐºÐ¾Ð¾Ð¿ÐµÑ€Ð°Ñ‚Ð¸Ð²Ð½Ñ‹Ð¹ wait Ð¿Ð°ÑƒÐ·Ðµ

ÐÐ¾Ð²Ñ‹Ð¹ Ñ‚Ñ€ÐµÐ¹Ñ (Ð¿Ð¾ÑÐ»Ðµ Ð¸ÑÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¸Ð¹)
printmessage
C++| load_model_before_avalible: Ð—ÐÐŸÐ ÐžÐ¡ Ð¼Ð¾Ð´ÐµÐ»Ð¸ id=280
Ð¼Ð¾Ð´ÐµÐ»ÑŒ id=280 Ð·Ð°Ð³Ñ€ÑƒÐ¶ÐµÐ½Ð°
ÐŸÐÐ£Ð—Ð: Ð·Ð°Ð¿Ñ€Ð¾Ñ (thread=468), ÑÐºÑ€Ð¸Ð¿Ñ‚Ñ‹ Ð·Ð°Ð¼Ð¾Ñ€Ð°Ð¶Ð¸Ð²Ð°ÑŽÑ‚ÑÑ (yield-Ð¿Ð°ÑƒÐ·Ð° Ð±ÐµÐ· teardown)
Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÐµÐ½Ð° Ð¸Ð³Ñ€Ð¾Ð²Ð¾Ð¼ Ð¿Ð¾Ñ‚Ð¾ÐºÐµ (thread=280), Ð·Ð°Ð¼Ð¾Ñ€Ð¾Ð¶ÐµÐ½Ñ‹
[ÐžÐ‘Ð Ð«Ð’ â€” Ð¿Ñ€Ð¾Ñ†ÐµÑÑ Ð·Ð°Ð²ÐµÑ€ÑˆÐ¸Ð»ÑÑ]
```

Ð¤ÐÐšÐ¢
**Crash handler Ð¡Ð ÐÐ‘ÐžÐ¢ÐÐ›** `grep -c CRASH lualoader/trace.txt` = **0**.

ÐÐ¸ Ð¾Ð´Ð½Ð¾Ð¹ ÑÑ‚Ñ€Ð¾ÐºÐ¸ Ð²Ð¸Ð´Ð° `C++| CRASH: ÐºÐ¾Ð´ 0x...` Ñ‚Ñ€ÐµÐ¹ÑÐµ.

---

Q1. ÐŸÐ¾Ñ‡ÐµÐ¼Ñƒ SetUnhandledExceptionFilter ÑÑ€Ð°Ð±Ð°Ñ‚Ñ‹Ð²Ð°ÐµÑ‚?

ÐŸÐ¾Ð»Ð½Ñ‹Ð¹ ÑÐ¿Ð¸ÑÐ¾Ðº Ð¿Ñ€Ð¸Ñ‡Ð¸Ð½ (Windows/Wine/reVC)

#### ÐŸÑ€Ð¸Ñ‡Ð¸Ð½Ð° 1: reVC ÑƒÑÑ‚Ð°Ð½Ð°Ð²Ð»Ð¸Ð²Ð°ÐµÑ‚ Ð¡Ð’ÐžÐ™ Ñ„Ð¸Ð»ÑŒÑ‚Ñ€ ÐŸÐžÐ¡Ð›Ð• Ð½Ð°Ñ

**ÐœÐµÑ…Ð°Ð½Ð¸ÐºÐ°:**
```cpp
// ÐÐ°Ñˆ Ð¿Ð»Ð°Ð³Ð¸Ð½ (DllMain):
SetUnhandledExceptionFilter(our_crash_handler);

main() (Ð¿Ð¾Ð·Ð¶Ðµ):
SetUnhandledExceptionFilter(reVC_crash_handler);  â† ÐŸÐ•Ð Ð•Ð—ÐÐŸÐ˜Ð¡Ð«Ð’ÐÐ•Ð¢ ÐÐÐ¨!
Ð²Ð¾Ð·Ð²Ñ€Ð°Ñ‰Ð°ÐµÑ‚ **ÐŸÐ Ð•Ð”Ð«Ð”Ð£Ð©Ð˜Ð™** Ñ„Ð¸Ð»ÑŒÑ‚Ñ€, Ð½Ð¾ ÑÐ¾Ð·Ð´Ð°Ñ‘Ñ‚ Ñ†ÐµÐ¿Ð¾Ñ‡ÐºÑƒ. ÐŸÐ¾ÑÐ»ÐµÐ´Ð½Ð¸Ð¹ Ð²Ñ‹Ð·Ð¾Ð² Ð¿Ð¾Ð±ÐµÐ¶Ð´Ð°ÐµÑ‚.

**ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ°:**
LPTOP_LEVEL_EXCEPTION_FILTER our_handler SetUnhandledExceptionFilter(crash_handler);
cpp_tracef("ÐŸÑ€ÐµÐ´Ñ‹Ð´ÑƒÑ‰Ð¸Ð¹ Ñ„Ð¸Ð»ÑŒÑ‚Ñ€: %p", our_handler);

Ð§ÐµÑ€ÐµÐ· 1 ÑÐµÐºÑƒÐ½Ð´Ñƒ Ð¿Ñ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼, Ð½Ðµ Ð¿ÐµÑ€ÐµÐ·Ð°Ð¿Ð¸ÑÐ°Ð»Ð¸ Ð»Ð¸ Ð½Ð°Ñ:
Sleep(1000);
current if (current != crash_handler) {
    cpp_tracef("Ð’ÐÐ˜ÐœÐÐÐ˜Ð•: Ð±Ñ‹Ð» Ð¿ÐµÑ€ÐµÐ·Ð°Ð¿Ð¸ÑÐ°Ð½ %p!", current);
}
**Ð ÐµÑˆÐµÐ½Ð¸Ðµ:** Ð˜ÑÐ¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÑŒ `AddVectoredExceptionHandler` (Ð²Ñ‹ÑˆÐµ Ñ†ÐµÐ¿Ð¾Ñ‡ÐºÐµ):

LONG WINAPI vectored_crash_handler(EXCEPTION_POINTERS* ex_info) FILE* f fopen("lualoader/trace.txt", "a");
    (f) {
        fprintf(f, "C++| VECTORED 0x%08X, Ð°Ð´Ñ€ÐµÑ Ð¿Ð¾Ñ‚Ð¾Ðº %lu\n",
                ex_info->ExceptionRecord->ExceptionCode,
                (DWORD)ex_info->ExceptionRecord->ExceptionAddress,
                GetCurrentThreadId());
        fflush(f);
        fclose(f);
    }
    
    // EXCEPTION_CONTINUE_SEARCH Ð¿ÐµÑ€ÐµÐ´Ð°Ñ‚ÑŒ ÑÐ»ÐµÐ´ÑƒÑŽÑ‰ÐµÐ¼Ñƒ Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ñ‡Ð¸ÐºÑƒ
    return EXCEPTION_CONTINUE_SEARCH;
}

void install_crash_handler() FIRST=1 Ð½Ð°Ñˆ Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ñ‡Ð¸Ðº ÐŸÐ•Ð Ð’Ð«Ðœ Ñ†ÐµÐ¿Ð¾Ñ‡ÐºÐµ
    AddVectoredExceptionHandler(1, vectored_crash_handler);
    cpp_tracef("Vectored exception ÑƒÑÑ‚Ð°Ð½Ð¾Ð²Ð»ÐµÐ½");
2: Ð˜ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ðµ Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ð°Ð½Ð¾ __try/__except Ð’ÐÐ£Ð¢Ð Ð˜ reVC

**ÐœÐµÑ…Ð°Ð½Ð¸ÐºÐ°:**

Ð¼Ð¾Ð¶ÐµÑ‚ Ð¾Ð±Ñ‘Ñ€Ñ‚Ñ‹Ð²Ð°Ñ‚ÑŒ Ð¸Ð³Ñ€Ð¾Ð²Ð¾Ð¹ Ñ†Ð¸ÐºÐ» SEH:
main loop (Ð¿ÑÐµÐ²Ð´Ð¾ÐºÐ¾Ð´):
while (running) __try game_loop();  Ð’Ð½ÑƒÑ‚Ñ€Ð¸ Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ÑÑ Events::gameProcessEvent
    __except (EXCEPTION_EXECUTE_HANDLER) log("Game crashed, exiting");
        ExitProcess(1);  Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ UnhandledExceptionFilter!
    Ð•ÑÐ»Ð¸ Ð¿Ð°Ð´Ð°ÐµÑ‚ **Ð’ÐÐ£Ð¢Ð Ð˜** `__try` Ð±Ð»Ð¾ÐºÐ° â†’ `__except` Ð»Ð¾Ð²Ð¸Ñ‚ `ExitProcess` Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ÑÑ.

**ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ°:**

Ð”Ð¾Ð±Ð°Ð²Ð¸Ñ‚ÑŒ Ð»Ð¾Ð³Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð¸Ðµ **Ð”Ðž** Ð¸ **ÐŸÐžÐ¡Ð›Ð•** ÐºÐ°Ð¶Ð´Ð¾Ð¹ ÐºÑ€Ð¸Ñ‚Ð¸Ñ‡ÐµÑÐºÐ¾Ð¹ Ð¾Ð¿ÐµÑ€Ð°Ñ†Ð¸Ð¸:

Events::gameProcessEvent += [] cpp_tracef("gameProcessEvent: Ð’Ð¥ÐžÐ” (thread=%lu)", GetCurrentThreadId());
    
    (pause_requested.exchange(false)) pause_requested TRUE, Ð½Ð°Ñ‡Ð°Ð»Ð¾ Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚ÐºÐ¸");
        
        unsigned int& OnAMissionFlag *(unsigned int*)0x978748;
        %u", OnAMissionFlag);
        
        (OnAMissionFlag < 260512) {
            ÐŸÐ•Ð Ð•Ð” Ð·Ð°Ð¿Ð¸ÑÑŒÑŽ ScriptSpace[%u]", OnAMissionFlag);
            CTheScripts::ScriptSpace[OnAMissionFlag] false;
            Ð·Ð°Ð¿Ð¸ÑÐ¸ ScriptSpace");
        }
        
        scripts_paused.store(true);
        scripts_paused ÑƒÑÑ‚Ð°Ð½Ð¾Ð²Ð»ÐµÐ½");
        
        cpp_tracef("gameProcessEvent: CMessages::AddMessageJumpQ");
        CMessages::AddMessageJumpQ(L"Scripts disabled", 2000, 1);
        CMessages::AddMessageJumpQ");
        
        Ð¿Ð°ÑƒÐ·Ð° Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÐµÐ½Ð°");
    
    cpp_tracef("gameProcessEvent: Ð’Ð«Ð¥ÐžÐ”");
};
**Ð•ÑÐ»Ð¸ ÐºÑ€Ð°Ñˆ Ð¼ÐµÐ¶Ð´Ñƒ `ÐŸÐ•Ð Ð•Ð”` `ÐŸÐžÐ¡Ð›Ð•`** ÑÑ‚Ð¾ SEH.

Vectored SEH (`__try/__except`).

3: Wine Ñ‚Ñ€Ð°Ð½ÑÑ„Ð¾Ñ€Ð¼Ð¸Ñ€ÑƒÐµÑ‚ Windows-Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ðµ POSIX-ÑÐ¸Ð³Ð½Ð°Ð»

ÑÐ¼ÑƒÐ»Ð¸Ñ€ÑƒÐµÑ‚ Windows Ñ‡ÐµÑ€ÐµÐ· POSIX signals:
Access Violation (0xC0000005) `SIGSEGV`
Stack Overflow (0xC00000FD) `SIGSEGV`

Ð¸Ð»Ð¸ ÑÑ‚Ð°Ð²Ð¸Ñ‚ ÑÐ²Ð¾Ð¹ `signal(SIGSEGV, handler)` Windows-Ñ„Ð¸Ð»ÑŒÑ‚Ñ€ **ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ° (Linux-ÑÐ¿ÐµÑ†Ð¸Ñ„Ð¸Ñ‡Ð½Ð¾):**

#include <signal.h>
<execinfo.h>

posix_crash_handler(int sig, siginfo_t* info, void* ctx) SIGNAL ÑÐ¸Ð³Ð½Ð°Ð» %d, %p\n", info->si_addr);
        
        // Backtrace (Linux)
        buffer[128];
        int count backtrace(buffer, 128);
        char** symbols backtrace_symbols(buffer, count);
        
        for (int i 0; count; i++) "  %s\n", symbols[i]);
        free(symbols);
        Ð—Ð°Ð²ÐµÑ€ÑˆÐ¸Ñ‚ÑŒ Ð¿Ñ€Ð¾Ñ†ÐµÑÑ
    _exit(1);
install_posix_crash_handler() struct sigaction sa;
    sa.sa_sigaction posix_crash_handler;
    sa.sa_flags SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, sigaction(SIGFPE, nullptr);
    
    cpp_tracef("POSIX signal handlers ÑƒÑÑ‚Ð°Ð½Ð¾Ð²Ð»ÐµÐ½Ñ‹");
Ð’Ñ‹Ð·Ñ‹Ð²Ð°Ñ‚ÑŒ Ð˜Ð— DllMain/plugin_init:
install_posix_crash_handler();
install_crash_handler();  vectored
4: ÐŸÑ€Ð¾Ñ†ÐµÑÑ Ð·Ð°Ð²ÐµÑ€ÑˆÑ‘Ð½ ExitProcess/std::terminate Ð‘Ð•Ð— Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ñ

Ð³Ð´Ðµ-Ñ‚Ð¾ ÐºÐ¾Ð´Ðµ (reVC, Wine, Ð¿Ð»Ð°Ð³Ð¸Ð½) Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ÑÑ:
`ExitProcess(code)`
`std::terminate()` `abort()` `SIGABRT`
`_exit(code)` (POSIX)

Ð·Ð°Ð²ÐµÑ€ÑˆÐ°ÐµÑ‚ÑÑ **Ð‘Ð•Ð—** Ð²Ð¾Ð·Ð±ÑƒÐ¶Ð´ÐµÐ½Ð¸Ñ exception.

ÐŸÐµÑ€ÐµÑ…Ð²Ð°Ñ‚Ð¸Ñ‚ÑŒ `std::terminate`:

custom_terminate_handler() STD::TERMINATE Ð’Ð«Ð—Ð’ÐÐ (thread=%lu)\n", }
    
    ÐŸÐ¾Ð¿Ñ‹Ñ‚Ð°Ñ‚ÑŒÑÑ Ð¿Ð¾Ð»ÑƒÑ‡Ð¸Ñ‚ÑŒ Ñ‚ÐµÐºÑƒÑ‰ÐµÐµ Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ðµ
    try (std::current_exception()) std::rethrow_exception(std::current_exception());
        } catch (const std::exception& e) f2 "a");
        (f2) fprintf(f2, Ð˜Ð¡ÐšÐ›Ð®Ð§Ð•ÐÐ˜Ð•: e.what());
            fclose(f2);
        (...) ÐÐµÐ¸Ð·Ð²ÐµÑÑ‚Ð½Ð¾Ðµ 
    abort();  Ð¡Ñ‚Ð°Ð½Ð´Ð°Ñ€Ñ‚Ð½Ð¾Ðµ Ð¿Ð¾Ð²ÐµÐ´ÐµÐ½Ð¸Ðµ
Ð’ plugin_init:
std::set_terminate(custom_terminate_handler);
5: Fast Fail (0xC0000409) Ñ‚Ð¸Ñ…Ð¾Ðµ Ð·Ð°Ð²ÐµÑ€ÑˆÐµÐ½Ð¸Ðµ

Ð·Ð°Ð²ÐµÑ€ÑˆÐ¸Ñ‚ÑŒ `__fastfail()` (security mitigation):
ÐšÐ¾Ð´ Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ñ: `0xC0000409` (STATUS_STACK_BUFFER_OVERRUN)
Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ñ‡Ð¸ÐºÐ¾Ð²

(Ð»Ð¾Ð³Ð¸ Wine):**

```bash
WINEDEBUG=+seh,+relay wine reVC.exe 2>&1 | tee wine.log
Ð˜ÑÐºÐ°Ñ‚ÑŒ `wine.log`:
err:seh:dispatch_exception EXCEPTION_STACK_BUFFER_OVERRUN
Ð¢Ð¾Ð»ÑŒÐºÐ¾ vectored Ð¿Ð¾Ð¹Ð¼Ð°Ñ‚ÑŒ (Ð½Ð¾ Ð²ÑÐµÐ³Ð´Ð°).

6: ÑƒÐ±Ð¸Ñ‚ Ð¸Ð·Ð²Ð½Ðµ (OOM killer, SIGKILL)

(Linux):**

dmesg tail -50  "Out of memory: Kill process"
journalctl -xe grep reVC
ÐœÐ¾Ð½Ð¸Ñ‚Ð¾Ñ€Ð¸Ð½Ð³ Ð¿Ð°Ð¼ÑÑ‚Ð¸, ÑƒÐ²ÐµÐ»Ð¸Ñ‡ÐµÐ½Ð¸Ðµ Ð»Ð¸Ð¼Ð¸Ñ‚Ð¾Ð².

Ð­ÐœÐŸÐ˜Ð Ð˜Ð§Ð•Ð¡ÐšÐÐ¯ ÐŸÐ ÐžÐ’Ð•Ð ÐšÐ Ð’Ð¡Ð•Ð¥ ÐŸÐ Ð˜Ð§Ð˜Ð

**ÐšÐ¾Ð´ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ð³Ð¾ crash handler:**

<windows.h>
<execinfo.h>
<exception>

1. Exception Handler (ÐŸÐ•Ð Ð’Ð«Ð™ Ñ†ÐµÐ¿Ð¾Ñ‡ÐºÐµ)
ex) fopen("lualoader/crash_vectored.txt", "VECTORED: ÐºÐ¾Ð´=0x%08X Ð°Ð´Ñ€ÐµÑ=0x%08X Ð¿Ð¾Ñ‚Ð¾Ðº=%lu\n",
                ex->ExceptionRecord->ExceptionCode,
                (DWORD)ex->ExceptionRecord->ExceptionAddress,
                2. Unhandled Filter (ÐŸÐžÐ¡Ð›Ð•Ð”ÐÐ˜Ð™ unhandled_crash_handler(EXCEPTION_POINTERS* fopen("lualoader/crash_unhandled.txt", "UNHANDLED: EXCEPTION_EXECUTE_HANDLER;
3. Signal (Wine/Linux)
fopen("lualoader/crash_posix.txt", "POSIX: ÑÐ¸Ð³Ð½Ð°Ð»=%d Ð°Ð´Ñ€ÐµÑ=%p\n", 
        void* backtrace_symbols_fd(buffer, count, fileno(f));
        
        4. std::terminate Handler
terminate_handler() fopen("lualoader/crash_terminate.txt", "STD::TERMINATE Ð¿Ð¾Ñ‚Ð¾Ðº=%lu\n", abort();
5. atexit (Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ÑÑ Ð½Ð¾Ñ€Ð¼Ð°Ð»ÑŒÐ½Ð¾Ð¼ Ð²Ñ‹Ñ…Ð¾Ð´Ðµ)
atexit_handler() fopen("lualoader/crash_atexit.txt", "ATEXIT Ð²Ñ‹Ð·Ð²Ð°Ð½ (Ð½Ð¾Ñ€Ð¼Ð°Ð»ÑŒÐ½Ñ‹Ð¹ Ð²Ñ‹Ñ…Ð¾Ð´ exit())\n");
        install_all_crash_handlers() (FIRST=1)
    vectored_crash_handler);
    
    unhandled
    SetUnhandledExceptionFilter(unhandled_crash_handler);
    
    signals
    sigaction(SIGSEGV, sigaction(SIGILL, C++ terminate
    std::set_terminate(terminate_handler);
    atexit
    atexit(atexit_handler);
    
    cpp_tracef("Ð’Ð¡Ð• **ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ°:** Ð—Ð°Ð¿ÑƒÑÑ‚Ð¸Ñ‚ÑŒ Ð¸Ð³Ñ€Ñƒ Ð¿Ñ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ, ÐºÐ°ÐºÐ¾Ð¹ Ñ„Ð°Ð¹Ð» Ð¿Ð¾ÑÐ²Ð¸Ð»ÑÑ:
`crash_vectored.txt` Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ðµ Ð¿Ð¾Ð¹Ð¼Ð°Ð½Ð¾, ÐµÑÑ‚ÑŒ Ð´Ñ€ÑƒÐ³Ð¾Ð¹ Ð²Ñ‹ÑˆÐµ
`crash_unhandled.txt` Ð½ÐµÐ¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ð°Ð½Ð¾ (Ð¸Ð´ÐµÐ°Ð»ÑŒÐ½Ð¾)
`crash_posix.txt` Ñ‚Ñ€Ð°Ð½ÑÑ„Ð¾Ñ€Ð¼Ð¸Ñ€Ð¾Ð²Ð°Ð» signal
`crash_terminate.txt` `std::terminate()`
`crash_atexit.txt` Ð½Ð¾Ñ€Ð¼Ð°Ð»ÑŒÐ½Ð¾ `exit()`
**ÐÐ˜ ÐžÐ”ÐÐžÐ“Ðž Ñ„Ð°Ð¹Ð»Ð°** (`SIGKILL`, OOM killer) fast fail

Q2. ÐŸÑƒÑ‚Ð¸ Ð¿Ð°Ð´ÐµÐ½Ð¸Ñ Ñ„Ð¸Ð»ÑŒÑ‚Ñ€Ð°

Ð¡Ð¿Ð¸ÑÐ¾Ðº Ð¿ÑƒÑ‚ÐµÐ¹

ÐŸÑƒÑ‚ÑŒ ExitProcess(code) SEH

Ñ„Ð¸Ð»ÑŒÑ‚Ñ€Ñ‹
**ÐŸÑ€Ð¸Ð·Ð½Ð°Ðº:** ÑÐ¾Ð·Ð´Ð°Ð½, Ð¾ÑÑ‚Ð°Ð»ÑŒÐ½Ñ‹Ñ… Ð½ÐµÑ‚.

**ÐšÐ°Ðº Ð¾Ñ‚Ð»Ð¸Ñ‡Ð¸Ñ‚ÑŒ:** Ð¾Ð¿ÐµÑ€Ð°Ñ†Ð¸Ð¸. ÑÑ‚Ñ€Ð¾ÐºÐ° ExitProcess, ÐµÑÐ»Ð¸ Ð½ÐµÑ‚ Ð²Ð¾Ð·Ð¼Ð¾Ð¶Ð½Ð¾ ExitProcess.

abort SIGABRT

Ð“Ð´Ðµ-Ñ‚Ð¾ ÐºÐ¾Ð´Ðµ:
throw std::runtime_error("error");  ÐÐµÐ¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚Ð°Ð½Ð½Ð¾Ðµ Ð¸ÑÐºÐ»ÑŽÑ‡ÐµÐ½Ð¸Ðµ
std::terminate() abort() SIGABRT
ÑÐ¾Ð·Ð´Ð°Ð½.

Ð»Ð¾Ð³Ð°:**

"terminate called" SEH, Ð²Ð½ÑƒÑ‚Ñ€Ð¸

gameProcessEvent callback
    CTheScripts::ScriptSpace[bad_index] 0;  AV
log("Caught AV");
    ExitProcess(1);
ÑÐ¾Ð·Ð´Ð°Ð½ (vectored Ð”Ðž SEH), ÐÐ•Ð¢.

(0xC0000409)

Heap/stack corruption Ð¼Ð³Ð½Ð¾Ð²ÐµÐ½Ð½Ð¾Ðµ Ð·Ð°Ð²ÐµÑ€ÑˆÐµÐ½Ð¸Ðµ.

Ð’Ð¾Ð·Ð¼Ð¾Ð¶Ð½Ð¾ Ñ ÐºÐ¾Ð´Ð¾Ð¼ `0xC0000409`, ÐÐ˜ Ñ„Ð°Ð¹Ð»Ð°.

Ð»Ð¾Ð³:**

"0xc0000409" Ð—Ð°Ð²Ð¸ÑÐ°Ð½Ð¸Ðµ (hang), ÐºÑ€Ð°Ñˆ

Ð¶Ð¸Ð², Ð¾Ñ‚Ð²ÐµÑ‡Ð°ÐµÑ‚. ÐŸÐ¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŒ Ð·Ð°ÐºÑ€Ñ‹Ð²Ð°ÐµÑ‚ Ð¾ÐºÐ½Ð¾ `SIGTERM`/`SIGKILL`.

(Ð¿Ñ€Ð¸ `SIGTERM`), Ñ„Ð°Ð¹Ð»Ð° `SIGKILL`).

Ð’Ð¾ Ð²Ñ€ÐµÐ¼Ñ "ÐºÑ€Ð°ÑˆÐ°" Ð¿Ñ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ Ð¿Ñ€Ð¾Ñ†ÐµÑÑ:
ps aux Ð·Ð°Ð²Ð¸ÑÐ°Ð½Ð¸Ðµ, Ð¡Ñ‚ÐµÐº Ð²ÑÐµÑ… Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð² (Linux):
gdb -p $(pidof reVC.exe) -batch -ex "thread apply all bt"
Killer (Linux)

Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÑ‚ ÑÐ»Ð¸ÑˆÐºÐ¾Ð¼ Ð¼Ð½Ð¾Ð³Ð¾ Ð¿Ð°Ð¼ÑÑ‚Ð¸ ÑÐ´Ñ€Ð¾ ÑƒÐ±Ð¸Ð²Ð°ÐµÑ‚ ÐµÐ³Ð¾.

crash-Ñ„Ð°Ð¹Ð»Ð°.

-i "killed ÐšÐ°Ðº Ð¾Ñ‚Ð»Ð¸Ñ‡Ð¸Ñ‚ÑŒ Ð¿Ð¾ Ñ‚Ñ€ÐµÐ¹ÑÑƒ/Ð»Ð¾Ð³Ð°Ð¼ Wine

**ÐŸÐ¾ÑÐ»ÐµÐ´Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŒÐ½Ð¾ÑÑ‚ÑŒ Ð´Ð¸Ð°Ð³Ð½Ð¾ÑÑ‚Ð¸ÐºÐ¸:**

**ÐŸÑ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ crash-Ñ„Ð°Ð¹Ð»Ñ‹:**
   Ð•ÑÑ‚ÑŒ Ð±Ñ‹Ð»Ð¾
   (Ð½Ð°Ñˆ ÑÑ€Ð°Ð±Ð¾Ñ‚Ð°Ð»)
   Wine/POSIX signal
   `std::terminate()`
   Ð½Ð¾Ñ€Ð¼Ð°Ð»ÑŒÐ½Ñ‹Ð¹ `exit()`
   **ÐÐµÑ‚ Ð½Ð¸ Ð¾Ð´Ð½Ð¾Ð³Ð¾** `SIGKILL`, OOM, Ð»Ð¾Ð³:**
   ```bash
   wine.log
   ```
   
   Ð˜ÑÐºÐ°Ñ‚ÑŒ:
   `err:seh:dispatch_exception` Wine
   `terminate called` `std::terminate`
   `0xc0000409` fail
   `EXCEPTION_ACCESS_VIOLATION` AV

dmesg/journalctl:**
   -T -50
   --since "5 minutes ago" reVC
   lualoader:**
   
   ÐŸÐ¾ÑÐ»ÐµÐ´Ð½ÑÑ Ð¿ÐµÑ€ÐµÐ´ Ð¾Ð±Ñ€Ñ‹Ð²Ð¾Ð¼ Ð¿Ð¾ÐºÐ°Ð·Ñ‹Ð²Ð°ÐµÑ‚, Ð³Ð´Ðµ Ð¾ÑÑ‚Ð°Ð½Ð¾Ð²Ð¸Ð»Ð¸ÑÑŒ:
   `ÐŸÐ•Ð Ð•Ð” ScriptSpace` Ð·Ð°Ð¿Ð¸ÑÐ¸
   `ÐŸÐžÐ¡Ð›Ð• ScriptSpace`, CMessages` `CMessages::AddMessageJumpQ`
   Ð¿Ð¾ÑÐ»Ðµ Ð¿Ð°ÑƒÐ·Ñ‹ (Ð² pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ°Ñ…?)

Q3. ÐŸÑ€Ð°Ð²Ð¸Ð»ÑŒÐ½Ð¾ÑÑ‚ÑŒ Ð´Ð¸Ð·Ð°Ð¹Ð½Ð° Ð¿Ð°ÑƒÐ·Ñ‹

Ð¢ÐµÐºÑƒÑ‰Ð¸Ð¹ Ð´Ð¸Ð·Ð°Ð¹Ð½

**Ð¡Ñ…ÐµÐ¼Ð°:**
`pause_requested true`
Ð˜Ð³Ñ€Ð¾Ð²Ð¾Ð¹ `gameProcessEvent`:
   ÐŸÐ¸ÑˆÐµÑ‚ `ScriptSpace[OnAMissionFlag] false`
   Ð¡Ñ‚Ð°Ð²Ð¸Ñ‚ `scripts_paused true`
   Ð’Ñ‹Ð·Ñ‹Ð²Ð°ÐµÑ‚ `CMessages::AddMessageJumpQ`
Pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¸ Ð¿Ñ€Ð¾Ð²ÐµÑ€ÑÑŽÑ‚ `scripts_paused` Ð´ÐµÐ»Ð°ÑŽÑ‚ `continue` (Ð¿Ð°Ñ€ÐºÑƒÑŽÑ‚ÑÑ)

ÐŸÑ€Ð¾Ð±Ð»ÐµÐ¼Ð°: ScriptSpace Ð¿Ð¸ÑˆÐµÑ‚ÑÑ Ð¿Ð°Ñ€ÐºÐ¾Ð²ÐºÐ¸ Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²

**ÐšÑ€Ð¸Ñ‚Ð¸Ñ‡ÐµÑÐºÐ°Ñ Ð³Ð¾Ð½ÐºÐ°:**

T=0:  ScriptSpace[OnAMissionFlag] false
Pump-Ð¿Ð¾Ñ‚Ð¾Ðº #1 Command<COMMAND_GIVE_WEAPON_TO_CHAR>(ped, ...)
                    â””â”€> ÐžÐ±Ñ€Ð°Ñ‰Ð°ÐµÑ‚ÑÑ Ðº Ð¸Ð³Ñ€Ð¾Ð²Ñ‹Ð¼ ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ð°Ð¼ (CPed::GiveWeapon)
T=1:  Ð¿Ñ€Ð¾Ð´Ð¾Ð»Ð¶Ð°ÐµÑ‚ Ñ€Ð°Ð±Ð¾Ñ‚Ñƒ (Ñ€ÐµÐ½Ð´ÐµÑ€, Ñ„Ð¸Ð·Ð¸ÐºÐ°)
                ÐžÐ±Ñ€Ð°Ð±Ð°Ñ‚Ñ‹Ð²Ð°ÐµÑ‚ false
                         â””â”€> Ð¾ÑÐ²Ð¾Ð±Ð¾Ð¶Ð´Ð°ÐµÑ‚ Ñ€ÐµÑÑƒÑ€ÑÑ‹ Ð¼Ð¸ÑÑÐ¸Ð¸
Command Ð¾Ð±Ñ€Ð°Ñ‰Ð°ÐµÑ‚ÑÑ Ð¾ÑÐ²Ð¾Ð±Ð¾Ð¶Ð´Ñ‘Ð½Ð½Ð¾Ð¼Ñƒ CPed ÐšÐ ÐÐ¨
**Ð’Ñ‹Ð²Ð¾Ð´:** false` pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð² data race Ð¸Ñ… Ð¸Ð³Ñ€Ð¾Ð²Ñ‹Ð¼Ð¸ Ð²Ñ‹Ð·Ð¾Ð²Ð°Ð¼Ð¸.

ÐŸÐ ÐÐ’Ð˜Ð›Ð¬ÐÐÐ¯ Ð¡Ð¥Ð•ÐœÐ: Ð”Ð²ÑƒÑ…Ñ„Ð°Ð·Ð½Ð°Ñ Ð±Ð°Ñ€ÑŒÐµÑ€Ð¾Ð¼

**Ð¤Ð°Ð·Ð° ÐŸÐ°Ñ€ÐºÐ¾Ð²ÐºÐ° pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²**

**Ð–Ð”ÐÐ¢**, Ð¿Ð¾ÐºÐ° Ð²ÑÐµ pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¸ Ð´Ð¾ÑÑ‚Ð¸Ð³Ð½ÑƒÑ‚ Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾Ð¹ Ñ‚Ð¾Ñ‡ÐºÐ¸ (wait/yield)
ÑÑ‚Ð¾Ð³Ð¾ true`

Ð˜Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸Ðµ Ð¿Ð°Ð¼ÑÑ‚Ð¸**

`ScriptSpace`, `CMessages`

Ð ÐµÐ°Ð»Ð¸Ð·Ð°Ñ†Ð¸Ñ Ð±Ð°Ñ€ÑŒÐµÑ€Ð°

Ð“Ð»Ð¾Ð±Ð°Ð»ÑŒÐ½Ñ‹Ðµ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ñ‹Ðµ
std::atomic<int> active_pump_threads{0};       Ð¡Ñ‡Ñ‘Ñ‚Ñ‡Ð¸Ðº Ð°ÐºÑ‚Ð¸Ð²Ð½Ñ‹Ñ… Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²
std::atomic<bool> scripts_paused{false};       Ð¤Ð»Ð°Ð³ Ð¿Ð°ÑƒÐ·Ñ‹
pause_requested{false};      Ð—Ð°Ð¿Ñ€Ð¾Ñ Ð¸Ð³Ñ€Ð¾Ð²Ñ‹Ðµ Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸Ñ
std::mutex pause_mutex;
std::condition_variable pause_cv;

pump_owner Ð½Ð°Ñ‡Ð°Ð»Ðµ ÐšÐÐ–Ð”ÐžÐ™ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ð¸):
pump_owner(lua_State* L, ...) active_pump_threads.fetch_add(1);  Ð ÐµÐ³Ð¸ÑÑ‚Ñ€Ð°Ñ†Ð¸Ñ
    
    ÐÐÐ§ÐÐ›Ðž Ð˜Ð¢Ð•Ð ÐÐ¦Ð˜Ð˜: Ð¿Ð¾Ð¼ÐµÑ‡Ð°ÐµÐ¼, Ñ‡Ñ‚Ð¾ Ð¼Ñ‹ "Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾Ð¹ Ð·Ð¾Ð½Ðµ"
        active_pump_threads.fetch_sub(1);  Ð’Ñ‹ÑˆÐ»Ð¸ Ð°ÐºÑ‚Ð¸Ð²Ð½Ð¾Ð¹ Ñ€Ð°Ð±Ð¾Ñ‚Ñ‹
        
        this_thread::sleep_for(chrono::milliseconds(1));
        
        (scripts_paused.load()) ÐŸÐ°Ñ€ÐºÐ¾Ð²ÐºÐ°: Ñ€ÐµÑÑƒÐ¼ÐµÐ¸Ð¼, Ð¶Ð´Ñ‘Ð¼
            pause_cv.notify_all();  Ð¡Ð¾Ð¾Ð±Ñ‰Ð¸Ñ‚ÑŒ reload-Ð¿Ð¾Ñ‚Ð¾ÐºÑƒ
            continue;
        ÐŸÐµÑ€ÐµÐ´ resume: Ð²Ñ…Ð¾Ð´Ð¸Ð¼ Ð°ÐºÑ‚Ð¸Ð²Ð½ÑƒÑŽ Ñ€Ð°Ð±Ð¾Ñ‚Ñƒ
        active_pump_threads.fetch_add(1);
        
        Resume (Ð¼Ð¾Ð¶ÐµÑ‚ Ð²Ñ‹Ð·Ð²Ð°Ñ‚ÑŒ Command<>)
        lua_resume(L, ...);
        ÐŸÐ¾ÑÐ»Ðµ ÑÐ½Ð¾Ð²Ð° Ð·Ð¾Ð½Ðµ (ÐµÑÐ»Ð¸ yield)
    
    active_pump_threads.fetch_sub(1);  Ð Ð°Ð·Ñ€ÐµÐ³Ð¸ÑÑ‚Ñ€Ð°Ñ†Ð¸Ñ
reload-Ð¿Ð¾Ñ‚Ð¾ÐºÐµ:
request_pause() cpp_tracef("request_pause: ÑƒÑÑ‚Ð°Ð½Ð¾Ð²ÐºÐ° scripts_paused");
    scripts_paused.store(true);
    Ð–Ð”ÐÐœ, Ð¿Ð°Ñ€ÐºÑƒÑŽÑ‚ÑÑ
    Ð¾Ð¶Ð¸Ð´Ð°Ð½Ð¸Ðµ pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²");
    
    auto deadline std::chrono::steady_clock::now() std::chrono::seconds(1);
    std::unique_lock<std::mutex> lock(pause_mutex);
    
    (active_pump_threads.load() > 0) (pause_cv.wait_until(lock, deadline) == std::cv_status::timeout) TIMEOUT! ÐÐºÑ‚Ð¸Ð²Ð½Ñ‹Ñ… Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²: %d", 
                       active_pump_threads.load());
            break;  ÐŸÑ€Ð¾Ð´Ð¾Ð»Ð¶Ð¸Ñ‚ÑŒ Ð´Ð°Ð¶Ðµ timeout
        
    cpp_tracef("request_pause: Ð¿Ð¾Ñ‚Ð¾ÐºÐ¸ Ð¿Ñ€Ð¸Ð¿Ð°Ñ€ÐºÐ¾Ð²Ð°Ð½Ñ‹, Ð°ÐºÑ‚Ð¸Ð²Ð½Ñ‹Ñ…: 
               active_pump_threads.load());
    Ð¢ÐµÐ¿ÐµÑ€ÑŒ Ð‘Ð•Ð—ÐžÐŸÐÐ¡ÐÐž Ð·Ð°Ð¿Ñ€Ð¾ÑÐ¸Ñ‚ÑŒ Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸Ðµ Ð¿Ð°Ð¼ÑÑ‚Ð¸
    pause_requested.store(true);
ÐÑƒÐ¶Ð½Ð¾ Ð²Ð¾Ð¾Ð±Ñ‰Ðµ Ñ‚Ñ€Ð¾Ð³Ð°Ñ‚ÑŒ Ð¿Ð°ÑƒÐ·Ðµ?

**ÐÐ½Ð°Ð»Ð¸Ð·:**

`OnAMissionFlag` Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÑ‚ÑÑ Ð¸Ð³Ñ€Ð¾Ð¹ Ð´Ð»Ñ:
ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ¸ Ð°ÐºÑ‚Ð¸Ð²Ð½Ð¾ÑÑ‚Ð¸ Ð‘Ð»Ð¾ÐºÐ¸Ñ€Ð¾Ð²ÐºÐ¸ Ð½ÐµÐºÐ¾Ñ‚Ð¾Ñ€Ñ‹Ñ… Ð¸Ð³Ñ€Ð¾Ð²Ñ‹Ñ… Ð´ÐµÐ¹ÑÑ‚Ð²Ð¸Ð¹ (Ð½Ð°Ð¿Ñ€Ð¸Ð¼ÐµÑ€, Ð½ÐµÐ»ÑŒÐ·Ñ ÑÐ¾Ñ…Ñ€Ð°Ð½Ð¸Ñ‚ÑŒÑÑ)

**ÐŸÑ€Ð¸ Ð½Ð°ÑˆÐ¸Ñ… ÑÐºÑ€Ð¸Ð¿Ñ‚Ð¾Ð²:**
Ð¡ÐºÑ€Ð¸Ð¿Ñ‚Ñ‹ Ð·Ð°Ð¼Ð¾Ñ€Ð¾Ð¶ÐµÐ½Ñ‹ (Ð½Ðµ Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÑÑŽÑ‚ÑÑ)
Ð˜Ð³Ñ€Ð¾Ð²Ñ‹Ðµ Ð¼Ð¸ÑÑÐ¸Ð¸ VC ÐµÑÑ‚ÑŒ) Ð¿Ñ€Ð¾Ð´Ð¾Ð»Ð¶Ð°ÑŽÑ‚ Ñ€Ð°Ð±Ð¾Ñ‚Ñƒ

**Ð’Ð¾Ð¿Ñ€Ð¾Ñ:** ÑÐ±Ñ€Ð°ÑÑ‹Ð²Ð°Ñ‚ÑŒ `OnAMissionFlag`?

**ÐžÑ‚Ð²ÐµÑ‚:** **ÐÐ•Ð¢**, ÐµÑÐ»Ð¸:
ÐœÑ‹ Ñ…Ð¾Ñ‚Ð¸Ð¼ Ð²Ð»Ð¸ÑÑ‚ÑŒ ÐÐ°ÑˆÐ¸ ÑƒÐ¿Ñ€Ð°Ð²Ð»ÑÑŽÑ‚ÑÑ `scripts_paused`

**Ð ÐµÐºÐ¾Ð¼ÐµÐ½Ð´Ð°Ñ†Ð¸Ñ:** **Ð£Ð‘Ð ÐÐ¢Ð¬** Ð·Ð°Ð¿Ð¸ÑÑŒ Ð¿Ð°ÑƒÐ·Ñ‹:

Ð£Ð‘Ð ÐÐ¢Ð¬ Ð­Ð¢Ð˜ Ð¡Ð¢Ð ÐžÐšÐ˜:
        unsigned //     false;
        }
        
        scripts_paused.store(true);
        
        CMessages::AddMessageJumpQ(L"Scripts paused", 
        cpp_tracef("ÐŸÐÐ£Ð—Ð: (Ð‘Ð•Ð— ScriptSpace)");
    **ÐŸÑ€ÐµÐ¸Ð¼ÑƒÑ‰ÐµÑÑ‚Ð²Ð°:**
ÐÐµÑ‚ Ð³Ð¾Ð½ÐºÐ¸ `ScriptSpace`
Ð·Ð°Ñ‚Ñ€Ð°Ð³Ð¸Ð²Ð°ÑŽÑ‚ÑÑ
Ð¿Ð°Ñ€ÐºÑƒÑŽÑ‚ÑÑ `scripts_paused`, Ð½ÐµÐ·Ð°Ð²Ð¸ÑÐ¸Ð¼Ð¾ Ð¾Ñ‚ `OnAMissionFlag`

Q4. ÐŸÑ€Ð¾Ñ‚Ð¾ÐºÐ¾Ð» Ð¼Ð½Ð¾Ð³Ð¾Ñ„Ð°Ð·Ð½Ð¾Ð¹ ÐŸÐ¾Ð»Ð½Ð°Ñ ÑÑ…ÐµÐ¼Ð°

Ð¤Ð°Ð·Ð° 0: Ð˜Ð½Ð¸Ñ†Ð¸Ð°Ð»Ð¸Ð·Ð°Ñ†Ð¸Ñ (DllMain/plugin_init)

active_pump_threads{0};
scripts_paused{false};
pause_requested{false};
pause_cv;
(reload-Ð¿Ð¾Ñ‚Ð¾Ðº)

Ð£ÑÑ‚Ð°Ð½Ð¾Ð²Ð¸Ñ‚ÑŒ Ñ„Ð»Ð°Ð³ Ð¿Ð°ÑƒÐ·Ñ‹
    true");
    Ð–Ð´Ð°Ñ‚ÑŒ (timeout ÑÐµÐº)
    auto lock(pause_mutex);
    
    int iteration 0;
    active_pump_threads.load();
        Ð¿Ð°Ñ€ÐºÐ¾Ð²ÐºÐ¸, %d (iter %d)", 
                   ++iteration);
        
        count);
            
            // ÐŸÐ¾Ð¿Ñ‹Ñ‚ÐºÐ° Ð¿Ð¾Ð´Ð¾Ð¶Ð´Ð°Ñ‚ÑŒ ÐµÑ‰Ñ‘ 500Ð¼Ñ
            std::chrono::milliseconds(500);
            {
                TIMEOUT #2! ÐŸÑ€Ð¸Ð½ÑƒÐ´Ð¸Ñ‚ÐµÐ»ÑŒÐ½Ð¾Ðµ Ð¿Ñ€Ð¾Ð´Ð¾Ð»Ð¶ÐµÐ½Ð¸Ðµ");
                break;
            Ð’Ð¡Ð• 
               active_pump_threads.load());
    
    Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾ Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸Ñ
    pause_requested.store(true);
    true, Ð—ÐÐ’Ð•Ð Ð¨Ð•ÐÐ˜Ð•");
ÐŸÑ€Ð¸Ð¼ÐµÐ½ÐµÐ½Ð¸Ðµ Ð¿Ð¾Ñ‚Ð¾Ðº)

ÐŸÐÐ£Ð—Ð Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÑÐµÑ‚ÑÑ GetCurrentThreadId());
        
        Ð¢Ð ÐžÐ“ÐÐ•Ðœ (ÑÐ¼. Q3)
        ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ Ð¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŽ
        ÐžÑÑ‚Ð°Ð»ÑŒÐ½Ð°Ñ Ð»Ð¾Ð³Ð¸ÐºÐ° gameProcessEvent
pump-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²

lua_State* L1, args, bool first_resume) register_script(L);  Ð³Ð»Ð¾Ð±Ð°Ð»ÑŒÐ½Ñ‹Ð¹ ÑÐ¿Ð¸ÑÐ¾Ðº
    
    cpp_tracef("pump_owner START L=%p (void*)L, (first_resume) NULL, NULL);
    (LUA_OK lua_status(L) && !lua_state_obsolete(L) !teardown_active.load()) Ð·Ð¾Ð½Ðµ" Ð²Ð½ÑƒÑ‚Ñ€Ð¸ Ð”ÐµÐºÑ€ÐµÐ¼ÐµÐ½Ñ‚ Ð±Ñ‹Ñ‚ÑŒ Ð¾Ñ‚Ñ€Ð¸Ñ†Ð°Ñ‚ÐµÐ»ÑŒÐ½Ñ‹Ð¼ Ð¿ÐµÑ€Ð²Ð¾Ð¹ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ð¸ Ð¸ÑÐ¿Ñ€Ð°Ð²Ð¸Ð¼)
        
        cpp_tracef("pump_owner: ÐŸÐÐ£Ð—Ð, Ð¿Ð°Ñ€ÐºÐ¾Ð²ÐºÐ° L=%p", (void*)L);
            continue;  Ñ€ÐµÑÑƒÐ¼ÐµÐ¸Ð¼
        Command<>, Ð¾Ð±Ñ€Ð°Ñ‚Ð¸Ñ‚ÑŒÑÑ Ð¿Ð°Ð¼ÑÑ‚Ð¸)
        (L1 NULL lua_type(L1, -1) LUA_TFUNCTION && 
            LUA_YIELD star_coroutine::get()) 
            for 1; <= args; lua_pushvalue(L1, i);
            lua_resume(L1, NULL);
        lua_status(L1) LUA_YIELD) 0, 
        if LUA_OK) ÐºÐ¾Ñ€ÑƒÑ‚Ð¸Ð½Ð° yield)
        active_pump_threads Ð´ÐµÐºÑ€ÐµÐ¼ÐµÐ½Ñ‚Ð¸Ñ€ÑƒÐµÑ‚ÑÑ ÑÐ»ÐµÐ´ÑƒÑŽÑ‰ÐµÐ¹ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ð¸
        (lua_status(L) || 
            (L1 !star_coroutine::get())) break;
        Ð¤Ð¸Ð½Ð°Ð»ÑŒÐ½Ñ‹Ð¹ Ð´ÐµÐºÑ€ÐµÐ¼ÐµÐ½Ñ‚
    
    cpp_tracef("pump_owner END GetCurrentThreadId());
    
    if (teardown_active.load() lua_state_obsolete(L)) lua_close(L);
    else drop_owner(L);
    
    unregister_script(L);
**ÐŸÐ ÐžÐ‘Ð›Ð•ÐœÐ:** `active_pump_threads.fetch_sub(1)` Ð´Ð°Ñ‚ÑŒ **Ð¾Ñ‚Ñ€Ð¸Ñ†Ð°Ñ‚ÐµÐ»ÑŒÐ½Ð¾Ðµ** Ð·Ð½Ð°Ñ‡ÐµÐ½Ð¸Ðµ Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ð¸.

**Ð˜Ð¡ÐŸÐ ÐÐ’Ð›Ð•ÐÐ˜Ð•:**

register_script(L);
    
    ÐÐÐ§ÐÐ›Ð¬ÐÐÐ¯ Ð Ð•Ð“Ð˜Ð¡Ð¢Ð ÐÐ¦Ð˜Ð¯
    
    L=%p, 
               (void*)L, active_pump_threads.load());
    
    
    while Command<>
        active_pump_threads.fetch_sub(1);
        
        pause_cv.notify_all();
            Resume
        ...
        
        ÑÑ‡Ñ‘Ñ‚Ñ‡Ð¸Ðº ÑÐ»ÐµÐ´. Ð¸Ñ‚ÐµÑ€Ð°Ñ†Ð¸Ð¸
    Ð½ÑƒÐ¶ÐµÐ½ Ñ„Ð¸Ð½Ð°Ð»ÑŒÐ½Ñ‹Ð¹ fetch_sub Ð¿Ð¾ÑÐ»ÐµÐ´Ð½Ð¸Ð¹ (void*)L);
    ...
    Ð’Ð¾Ð·Ð¾Ð±Ð½Ð¾Ð²Ð»ÐµÐ½Ð¸Ðµ

request_resume() cpp_tracef("request_resume: ÐÐÐ§ÐÐ›Ðž");
    
    scripts_paused.store(false);
    Ð±Ð°Ñ€ÑŒÐµÑ€ Ð²Ð¾Ð·Ð¾Ð±Ð½Ð¾Ð²Ð»ÐµÐ½Ð¸Ð¸ ÑÐ°Ð¼Ð¸ Ð²Ð¾Ð·Ð¾Ð±Ð½Ð¾Ð²ÑÑ‚ÑÑ ÐžÐ¿Ñ†Ð¸Ð¾Ð½Ð°Ð»ÑŒÐ½Ð¾: Ð¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŽ Ð¿Ð¾Ñ‚Ð¾Ðº
    resume_requested.store(true);
    
    cpp_tracef("request_resume: gameProcessEvent:
(resume_requested.exchange(false)) resumed", 1);
    Ð’ÐžÐ—ÐžÐ‘ÐÐžÐ’Ð›Ð•ÐÐ˜Ð• Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÐµÐ½Ð¾");
Q5. Ð’Ð»Ð¸ÑÐ½Ð¸Ðµ load_model_before_avalible(280)

ÐÐ½Ð°Ð»Ð¸Ð· Ñ‚Ñ€ÐµÐ¹ÑÐ°

Ð·Ð°Ð³Ñ€ÑƒÐ¶ÐµÐ½Ð°  Ð£Ð¡ÐŸÐ•Ð¨ÐÐž Ð—ÐÐ’Ð•Ð Ð¨Ð˜Ð›Ð¡Ð¯
Ð·Ð°Ð¼Ð¾Ñ€Ð°Ð¶Ð¸Ð²Ð°ÑŽÑ‚ÑÑ
[ÐšÐ ÐÐ¨]
**Ð¤Ð°ÐºÑ‚Ñ‹:**
`load_model_before_avalible(280)` **Ð—ÐÐ’Ð•Ð Ð¨Ð˜Ð›Ð¡Ð¯** Ð´Ð¾ ÐœÐ¾Ð´ÐµÐ»ÑŒ **Ð—ÐÐ“Ð Ð£Ð–Ð•ÐÐ** (ÑÑ‚Ñ€Ð¾ÐºÐ° "Ð¼Ð¾Ð´ÐµÐ»ÑŒ Ð·Ð°Ð³Ñ€ÑƒÐ¶ÐµÐ½Ð°" Ð¿Ð¾ÑÐ²Ð¸Ð»Ð°ÑÑŒ)
ÐšÑ€Ð°Ñˆ Ð¿Ñ€Ð¾Ð¸Ð·Ð¾ÑˆÑ‘Ð» Ð¿Ð°ÑƒÐ·Ñ‹, Ð° Ð²Ð¾ Ð·Ð°Ð³Ñ€ÑƒÐ·ÐºÐ¸

Ñ‚Ð°ÐºÐ¾Ðµ 280?

**GTA Ð¼Ð¾Ð´ÐµÐ»Ð¸:**
0-285: Peds (Ð¿ÐµÑ€ÑÐ¾Ð½Ð°Ð¶Ð¸, NPC)
280 Ð¾Ð±Ñ‹Ñ‡Ð½Ð¾: Ð¾Ð´Ð¸Ð½ ped-Ð¼Ð¾Ð´ÐµÐ»ÐµÐ¹ "SWAT")

**Ð§Ñ‚Ð¾ Ð´ÐµÐ»Ð°ÐµÑ‚ `load_model_before_avalible`:**

load_model_before_avalible(int model_id) REQUEST_MODEL(model_id);  Ð·Ð°Ð³Ñ€ÑƒÐ·ÐºÐ¸
    
    (!HAS_MODEL_LOADED(model_id)) sleep(10ms);
            Ð–Ð´Ñ‘Ð¼ Ð¿Ð°ÑƒÐ·Ñ‹
        (teardown_active.load()) break;
        
        sleep(1ms);
    cpp_tracef("load_model_before_avalible: id=%d Ð·Ð°Ð³Ñ€ÑƒÐ¶ÐµÐ½Ð°", model_id);
**ÐŸÑ€Ð¾Ð±Ð»ÐµÐ¼Ð°:** Ð·Ð°Ð³Ñ€ÑƒÐ·ÐºÐ¸ ÑÐºÑ€Ð¸Ð¿Ñ‚ **ÐŸÐ ÐžÐ”ÐžÐ›Ð–ÐÐ•Ð¢ Ð’Ð«ÐŸÐžÐ›ÐÐ•ÐÐ˜Ð•** ÑÐ»ÐµÐ´ÑƒÑŽÑ‰Ð°Ñ Ð¸Ð½ÑÑ‚Ñ€ÑƒÐºÑ†Ð¸Ñ Lua `create_ped(280)` API.

Ð’Ð¾Ð·Ð¼Ð¾Ð¶Ð½Ñ‹Ð¹ ÑÑ†ÐµÐ½Ð°Ñ€Ð¸Ð¹ ÐºÑ€Ð°ÑˆÐ°

**Lua-ÑÐºÑ€Ð¸Ð¿Ñ‚:**
```lua
function main()
    load_model_before_avalible(280)  -- Ð—Ð°Ð³Ñ€ÑƒÐ·ÐºÐ° SWAT
    local ped create_ped(280, x, y, z)  Ð¡Ð¾Ð·Ð´Ð°Ð½Ð¸Ðµ ped
    ...
end
**ÐŸÐ¾ÑÐ»ÐµÐ´Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŒÐ½Ð¾ÑÑ‚ÑŒ:**

(Ð¼Ð¾Ð´ÐµÐ»ÑŒ Ð·Ð°Ð³Ñ€ÑƒÐ¶ÐµÐ½Ð°)
Ð’Ð¾Ð·Ð²Ñ€Ð°Ñ‚ **ÑÐ»ÐµÐ´ÑƒÑŽÑ‰Ð°Ñ Ð¸Ð½ÑÑ‚Ñ€ÑƒÐºÑ†Ð¸Ñ** `create_ped`)
**ÐœÐ•Ð–Ð”Ð£** `load_model` `create_ped` Ð¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŒ Ð½Ð°Ð¶Ð¸Ð¼Ð°ÐµÑ‚ Ctrl
(ÑÑ‚Ð°Ñ€Ð°Ñ Ð²ÐµÑ€ÑÐ¸Ñ ÐºÐ¾Ð´Ð°)
6. **Ð•Ð©Ð Ð”ÐžÐ¨ÐÐ›** Ð¿Ñ€Ð¾Ð²ÐµÑ€ÐºÐ¸ `Command<COMMAND_CREATE_CHAR>` ÑÐ±Ñ€Ð¾ÑÐ° 7. Ð´Ð²Ð¸Ð¶Ð¾Ðº Ð¾Ð±Ñ€Ð°Ð±Ð°Ñ‚Ñ‹Ð²Ð°ÐµÑ‚ `OnAMissionFlag 8. Ð¿Ñ‹Ñ‚Ð°ÐµÑ‚ÑÑ ÑÐ¾Ð·Ð´Ð°Ñ‚ÑŒ Ð¾ÑÐ²Ð¾Ð±Ð¾Ð¶Ð´Ñ‘Ð½Ð½Ñ‹Ð¼ **ÐšÐ ÐÐ¨**

Ð ÐµÑˆÐµÐ½Ð¸Ðµ

**Ð˜ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÑ Ð±Ð°Ñ€ÑŒÐµÑ€Ð½ÑƒÑŽ Ð¿Ð°ÑƒÐ·Ñƒ (Q4):**
Ð¶Ð´Ñ‘Ñ‚, Ð¿Ñ€Ð¸Ð¿Ð°Ñ€ÐºÑƒÑŽÑ‚ÑÑ (Ð°ÐºÑ‚Ð¸Ð²Ð½Ñ‹Ñ… 0)
Ð¸Ð·Ð¼ÐµÐ½ÑÐµÑ‚ Ð¿Ð°Ð¼ÑÑ‚ÑŒ

**+ Ð£Ð±Ñ€Ð°Ñ‚ÑŒ `ScriptSpace` (Q3):**
ÐÐµ Ñ€ÐµÑÑƒÑ€ÑÑ‹

Q6. Ð‘ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾ÑÑ‚ÑŒ 504 Command<> worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²

Ð¾Ð±Ñ‰ÐµÐ¹ Ð°Ñ€Ñ…Ð¸Ñ‚ÐµÐºÑ‚ÑƒÑ€Ñ‹

**ÐŸÑ€Ð¾Ð±Ð»ÐµÐ¼Ð°:**
`Command<COMMAND_*>` worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð²
**ÐžÐ”ÐÐžÐ’Ð Ð•ÐœÐ•ÐÐÐž** Ð¼Ð¸Ñ€ (Ñ„Ð¸Ð·Ð¸ÐºÐ°, Ñ€ÐµÐ½Ð´ÐµÑ€, ÑÐºÑ€Ð¸Ð¿Ñ‚Ñ‹)
**ÐÐ•Ð¢ Ð¡Ð˜ÐÐ¥Ð ÐžÐÐ˜Ð—ÐÐ¦Ð˜Ð˜** worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ°Ð¼Ð¸ Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð¼

**Ð­Ñ‚Ð¾ Ð¿Ñ€Ð¸Ð½Ñ†Ð¸Ð¿Ðµ ÐÐ•Ð‘Ð•Ð—ÐžÐŸÐÐ¡ÐÐž.**

Ñ€Ð°Ð±Ð¾Ñ‚Ð°ÐµÑ‚ (Ð¾Ð±Ñ‹Ñ‡Ð½Ð¾)

**reVC/GTA VC:**
ÐœÐ½Ð¾Ð³Ð¸Ðµ `Command<>` Ñ„ÑƒÐ½ÐºÑ†Ð¸Ð¸ Ð°Ñ‚Ð¾Ð¼Ð°Ñ€Ð½Ñ‹ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÑŽÑ‚ Ð²Ð½ÑƒÑ‚Ñ€ÐµÐ½Ð½ÑŽÑŽ ÑÐ¸Ð½Ñ…Ñ€Ð¾Ð½Ð¸Ð·Ð°Ñ†Ð¸ÑŽ
ÐŸÑƒÐ»Ñ‹ Ð¾Ð±ÑŠÐµÐºÑ‚Ð¾Ð² (CPools) Ð¼Ð¾Ð³ÑƒÑ‚ Ð¸Ð¼ÐµÑ‚ÑŒ thread-safe Ð°Ð»Ð»Ð¾ÐºÐ°Ñ†Ð¸ÑŽ Ñ„Ð°ÐºÑ‚)
ÐŸÑ€Ð¾ÑÑ‚Ñ‹Ðµ Ð¾Ð¿ÐµÑ€Ð°Ñ†Ð¸Ð¸ (ÑƒÑÑ‚Ð°Ð½Ð¾Ð²ÐºÐ° ÐºÐ¾Ð¾Ñ€Ð´Ð¸Ð½Ð°Ñ‚, Ð·Ð´Ð¾Ñ€Ð¾Ð²ÑŒÑ) Ð°Ñ‚Ð¾Ð¼Ð°Ñ€Ð½Ñ‹Ðµ 4 Ð±Ð°Ð¹Ñ‚ (Ð¾Ð±Ñ‹Ñ‡Ð½Ð¾ x86)

**ÐÐž:** Ð³Ð°Ñ€Ð°Ð½Ñ‚Ð¸Ð¹. Ð¡Ð»Ð¾Ð¶Ð½Ñ‹Ðµ (ÑÐ¾Ð·Ð´Ð°Ð½Ð¸Ðµ ped, ÑƒÐ´Ð°Ð»ÐµÐ½Ð¸Ðµ vehicle Ñ‚.Ð´.) Ð¼Ð¾Ð³ÑƒÑ‚:
Ð˜Ð·Ð¼ÐµÐ½ÑÑ‚ÑŒ Ð³Ð»Ð¾Ð±Ð°Ð»ÑŒÐ½Ñ‹Ðµ ÑÐ¿Ð¸ÑÐºÐ¸
ÐÐ»Ð»Ð¾Ñ†Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ/Ð´ÐµÐ°Ð»Ð»Ð¾Ñ†Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ Ð¿Ð°Ð¼ÑÑ‚ÑŒ
Ñ€ÐµÐ½Ð´ÐµÑ€Ð°

Data Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð¼ ÐºÑ€Ð°Ñˆ.

Ð¡Ñ‚Ñ€Ð°Ñ‚ÐµÐ³Ð¸Ñ ÑÑ‚Ð°Ð±Ð¸Ð»Ð¸Ð·Ð°Ñ†Ð¸Ð¸ Ð¿ÐµÑ€ÐµÐ¿Ð¸ÑÑ‹Ð²Ð°Ð½Ð¸Ñ Ð²Ñ‹Ð·Ð¾Ð²Ð¾Ð²

Ð‘Ð°Ñ€ÑŒÐµÑ€Ð½Ð°Ñ (ÑƒÐ¶Ðµ Ñ€ÐµÐ°Ð»Ð¸Ð·Ð¾Ð²Ð°Ð½Ð° Q4)

**Ð˜Ð´ÐµÑ:**
ÐŸÑ€Ð¸ Ð¶Ð´Ñ‘Ð¼, Ð²Ñ‹Ð¹Ð´ÑƒÑ‚ (Ð¿Ñ€Ð¸Ð¿Ð°Ñ€ÐºÑƒÑŽÑ‚ÑÑ)
Ð¼ÐµÐ½ÑÐµÐ¼ **ÐŸÑ€ÐµÐ¸Ð¼ÑƒÑ‰ÐµÑÑ‚Ð²Ð¾:** ÐœÐ¸Ð½Ð¸Ð¼Ð°Ð»ÑŒÐ½Ñ‹Ðµ Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸Ñ ÐºÐ¾Ð´Ð°.

**ÐÐµÐ´Ð¾ÑÑ‚Ð°Ñ‚Ð¾Ðº:** Ð·Ð°Ñ‰Ð¸Ñ‰Ð°ÐµÑ‚ Ð³Ð¾Ð½Ð¾Ðº **Ð’Ðž Ð’Ð Ð•ÐœÐ¯ Ð ÐÐ‘ÐžÐ¢Ð«** (Ñ‚Ð¾Ð»ÑŒÐºÐ¾ Ð¿Ð°ÑƒÐ·Ñ‹).

Queue (Ð¾Ñ‚Ð»Ð¾Ð¶ÐµÐ½Ð½Ð¾Ðµ Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÐµÐ½Ð¸Ðµ)

Worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ¸ Ð²Ñ‹Ð·Ñ‹Ð²Ð°ÑŽÑ‚ Ð¡ÐºÐ»Ð°Ð´Ñ‹Ð²Ð°ÑŽÑ‚ Ð·Ð°Ð¿Ñ€Ð¾ÑÑ‹ **Ð¾Ñ‡ÐµÑ€ÐµÐ´ÑŒ**
**ÐºÐ°Ð¶Ð´Ñ‹Ð¹ ÐºÐ°Ð´Ñ€** Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÑÐµÑ‚ ÐºÐ¾Ð¼Ð°Ð½Ð´Ñ‹ Ð¾Ñ‡ÐµÑ€ÐµÐ´Ð¸

(ÐºÐ¾Ð½Ñ†ÐµÐ¿Ñ†Ð¸Ñ):**

CommandRequest command_id;
    std::vector<int> args;
};

command_queue_mutex;
std::queue<CommandRequest> command_queue;

funcs.cpp (Ð²Ð¼ÐµÑÑ‚Ð¾ Ð¿Ñ€ÑÐ¼Ð¾Ð³Ð¾ Command<>):
create_ped(lua_State* L) lua_yield(L, 0);
    model luaL_checkinteger(L, float x luaL_checknumber(L, 2);
    Ð’ÐœÐ•Ð¡Ð¢Ðž:
    CPed* Command<COMMAND_CREATE_CHAR>(...);
    Ð”Ð¾Ð±Ð°Ð²Ð»ÑÐµÐ¼ Ð¾Ñ‡ÐµÑ€ÐµÐ´ÑŒ:
    req;
    req.command_id COMMAND_CREATE_CHAR;
    req.args {model, (int)x, (int)y, (int)z};
    
    {
        std::lock_guard<std::mutex> lock(command_queue_mutex);
        command_queue.push(req);
    Ð–Ð”ÐÐœ Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÐµÐ½Ð¸Ñ (Ð¾Ð¿Ñ€Ð¾Ñ ÐºÐ°Ð¶Ð´Ñ‹Ðµ 1Ð¼Ñ)
    (true) ÐŸÑ€Ð¾Ð²ÐµÑ€ÑÐµÐ¼, Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÐµÐ½Ð° ÐºÐ¾Ð¼Ð°Ð½Ð´Ð° (Ð½ÑƒÐ¶ÐµÐ½ Ð¼ÐµÑ…Ð°Ð½Ð¸Ð·Ð¼ ÑƒÐ²ÐµÐ´Ð¾Ð¼Ð»ÐµÐ½Ð¸Ð¹)
        ...
        0);
        lock(command_queue_mutex);
    
    (!command_queue.empty()) req command_queue.front();
        command_queue.pop();
        
        Ð’Ñ‹Ð¿Ð¾Ð»Ð½ÑÐµÐ¼ Ð˜Ð“Ð ÐžÐ’ÐžÐœ ÐŸÐžÐ¢ÐžÐšÐ•
        switch (req.command_id) case COMMAND_CREATE_CHAR:
                Command<COMMAND_CREATE_CHAR>(...);
                Ð¡Ð¾Ñ…Ñ€Ð°Ð½Ð¸Ñ‚ÑŒ Ñ€ÐµÐ·ÑƒÐ»ÑŒÑ‚Ð°Ñ‚ Ð´Ð»Ñ worker-Ð¿Ð¾Ñ‚Ð¾ÐºÐ°
                Ð²Ñ‹Ð·Ð¾Ð²Ñ‹ Ð¾Ð´Ð½Ð¾Ð³Ð¾ Ð¿Ð¾Ñ‚Ð¾ÐºÐ° Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾.

Ð¢Ñ€ÐµÐ±ÑƒÐµÑ‚ Ð²Ñ‹Ð·Ð¾Ð²Ð¾Ð² `Command<>`.

Critical Section Command<>

Ð“Ð»Ð¾Ð±Ð°Ð»ÑŒÐ½Ñ‹Ð¹ `std::mutex game_api_mutex`
ÐšÐ°Ð¶Ð´Ñ‹Ð¹ Ð·Ð°Ñ…Ð²Ð°Ñ‚Ñ‹Ð²Ð°ÐµÑ‚ Ð¼ÑŒÑŽÑ‚ÐµÐºÑ

**ÐšÐ¾Ð´:**

game_api_mutex;

0);
    
    ...
    
    lock(game_api_mutex);
    Command<COMMAND_CREATE_CHAR>(...);
    
    lua_pushlightuserdata(L, ped);
    1;
ÐŸÑ€Ð¾ÑÑ‚Ð°Ñ Ð·Ð°Ñ‰Ð¸Ñ‚Ð° Ð¾Ð´Ð½Ð¾Ð²Ñ€ÐµÐ¼ÐµÐ½Ð½Ñ‹Ñ… Ñ€Ð°Ð·Ð½Ñ‹Ñ… Ð¿Ð¾Ñ‚Ð¾ÐºÐ¾Ð².


- Ð¡ÐµÑ€Ð¸Ð°Ð»Ð¸Ð·Ð°Ñ†Ð¸Ñ (Ð¿Ñ€Ð¾Ð¸Ð·Ð²Ð¾Ð´Ð¸Ñ‚ÐµÐ»ÑŒÐ½Ð¾ÑÑ‚ÑŒ)
Ð¼ÑŒÑŽÑ‚ÐµÐºÑ)

Ð Ð•ÐšÐžÐœÐ•ÐÐ”ÐÐ¦Ð˜Ð¯ ÑÑ‚Ð°Ð±Ð¸Ð»ÑŒÐ½Ð¾Ð¹ **ÐœÐ¸Ð½Ð¸Ð¼Ð°Ð»ÑŒÐ½Ñ‹Ðµ (Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÑÐµÐ¼ Ð½ÐµÐ¼ÐµÐ´Ð»ÐµÐ½Ð½Ð¾):**

**Ð‘Ð°Ñ€ÑŒÐµÑ€Ð½Ð°Ñ Ð¿Ð°ÑƒÐ·Ð°** (Q4) Ð¶Ð´Ñ‘Ð¼ **Ð£Ð±Ñ€Ð°Ñ‚ÑŒ `ScriptSpace`** (Q3) **Ð”Ð¾Ð±Ð°Ð²Ð¸Ñ‚ÑŒ Ð¿Ñ€Ð¾Ð²ÐµÑ€ÐºÑƒ `scripts_paused`** C-Ñ„ÑƒÐ½ÐºÑ†Ð¸Ð¸ API Ð½Ð°Ñ‡Ð°Ð»Ð¾)

**Ð”Ð¾Ð»Ð³Ð¾ÑÑ€Ð¾Ñ‡Ð½Ð¾ (Ð´Ð»Ñ Ð¿Ð¾Ð»Ð½Ð¾Ð¹ Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð¾ÑÑ‚Ð¸):**

**Command Queue** Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÑÑ‚ÑŒ (Ð¿ÐµÑ€ÐµÐ¿Ð¸ÑÐ°Ñ‚ÑŒ Ð²Ñ‹Ð·Ð¾Ð²Ð°)

Q7. Ð°Ð´Ñ€ÐµÑÐ° 0x978748 ÑÐµÐ¼Ð°Ð½Ñ‚Ð¸ÐºÐ¸ OnAMissionFlag

Ð·Ð° Ð°Ð´Ñ€ÐµÑ?

Vice City (Ð¾Ñ€Ð¸Ð³Ð¸Ð½Ð°Ð», Windows):**

`0x978748` **Ð“Ð›ÐžÐ‘ÐÐ›Ð¬ÐÐÐ¯ ÐŸÐ•Ð Ð•ÐœÐ•ÐÐÐÐ¯** `CTheScripts::OnAMissionFlag`.

**Ð¢Ð¸Ð¿:** `unsigned int` (4 Ð±Ð°Ð¹Ñ‚Ð°).

**Ð¡ÐµÐ¼Ð°Ð½Ñ‚Ð¸ÐºÐ°:** **Ð˜ÐÐ”Ð•ÐšÐ¡** Ð¼Ð°ÑÑÐ¸Ð²Ðµ `CTheScripts::ScriptSpace`.

Ð¡Ñ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ð° CTheScripts

class CTheScripts {
public:
    static char ScriptSpace[260512];  0xFF Ð±Ð°Ð¹Ñ‚
    OnAMissionFlag;        Ð˜Ð½Ð´ÐµÐºÑ ScriptSpace
    
    Ð˜Ð³Ñ€Ð° Ñ‚Ð°Ðº:
    (ScriptSpace[OnAMissionFlag]) ÐœÐ¸ÑÑÐ¸Ñ Ð°ÐºÑ‚Ð¸Ð²Ð½Ð°
    ÐÐ´Ñ€ÐµÑÐ° (GTA 1.0 Windows):
ScriptSpace:     0x00A0D8A0
OnAMissionFlag:  0x00978748  ÐÐÐ¨ ÐÐ”Ð Ð•Ð¡
Ð¸Ð³Ñ€Ð° ÑÑ‚Ð°Ñ€Ñ‚Ðµ Ð¼Ð¸ÑÑÐ¸Ð¸:**
SCM-ÑÐºÑ€Ð¸Ð¿Ñ‚ (Ð¾Ð¿ÐºÐ¾Ð´ 03A4):
allocate_script_variable();  Ð’Ñ‹Ð´ÐµÐ»Ð¸Ñ‚ÑŒ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½ÑƒÑŽ ScriptSpace
1;  "Ð¼Ð¸ÑÑÐ¸Ñ Ð°ÐºÑ‚Ð¸Ð²Ð½Ð°"
**Ð’Ð¾ Ð¿Ñ€Ð¾Ð²ÐµÑ€ÑÐµÑ‚:
Ð°ÐºÑ‚Ð¸Ð²Ð½Ð° Ð±Ð»Ð¾ÐºÐ¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ ÑÐ¾Ñ…Ñ€Ð°Ð½ÐµÐ½Ð¸Ñ, Ð¿Ð¾ÐºÐ°Ð·Ñ‹Ð²Ð°Ñ‚ÑŒ Ñ‚Ð°Ð¹Ð¼ÐµÑ€ Ñ‚.Ð´.
Ð·Ð°Ð²ÐµÑ€ÑˆÐµÐ½Ð¸Ð¸ Ð¡Ð±Ñ€Ð¾ÑÐ¸Ñ‚ÑŒ Ñ„Ð»Ð°Ð³
ÐŸÑ€Ð¾Ð±Ð»ÐµÐ¼Ð° Ð½Ð°ÑˆÐ¸Ð¼ ÐºÐ¾Ð´Ð¾Ð¼

**ÐœÑ‹ Ð´ÐµÐ»Ð°ÐµÐ¼:**
int*)0x978748;  Ð§Ð¸Ñ‚Ð°ÐµÐ¼ Ð˜ÐÐ”Ð•ÐšÐ¡
false;  ÐŸÐ¸ÑˆÐµÐ¼ Ð˜ÐÐ”Ð•ÐšÐ¡Ð£
ÐŸÐ ÐÐ’Ð˜Ð›Ð¬ÐÐž**, Ð•Ð¡Ð›Ð˜:
ÑÐ¾Ð´ÐµÑ€Ð¶Ð¸Ñ‚ Ð²Ð°Ð»Ð¸Ð´Ð½Ñ‹Ð¹ Ð¸Ð½Ð´ÐµÐºÑ 1000)
`ScriptSpace[1000]` Ð¼Ð¸ÑÑÐ¸Ð¸

**ÐÐž:**

0`** Ð¸Ð½Ð¸Ñ†Ð¸Ð°Ð»Ð¸Ð·Ð¸Ñ€Ð¾Ð²Ð°Ð½) Ð¿Ð¸ÑˆÐµÐ¼ `ScriptSpace[0]` Ð·Ð°Ñ‚ÐµÑ€ÐµÑ‚ÑŒ Ð´Ñ€ÑƒÐ³ÑƒÑŽ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½ÑƒÑŽ
Ð¼Ð¸ÑÑÐ¸Ñ Ð·Ð°Ð¿ÑƒÑ‰ÐµÐ½Ð°** ÑÐ¾Ð´ÐµÑ€Ð¶Ð°Ñ‚ÑŒ Ð¼ÑƒÑÐ¾Ñ€ 0
**reVC Ñ…Ñ€Ð°Ð½Ð¸Ñ‚ÑŒ Ð¿Ð¾-Ð´Ñ€ÑƒÐ³Ð¾Ð¼Ñƒ** (Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ð°Ñ Ð¿ÐµÑ€ÐµÐ¼ÐµÑ‰ÐµÐ½Ð°, Ð¸Ð·Ð¼ÐµÐ½Ñ‘Ð½ Ñ„Ð¾Ñ€Ð¼Ð°Ñ‚)

ÐŸÑ€Ð¾Ð²ÐµÑ€ÐºÐ° Ð²Ð°Ð»Ð¸Ð´Ð½Ð¾ÑÑ‚Ð¸ VC):**
ÐœÐ¾Ð¶ÐµÑ‚ Ð¸Ð·Ð¼ÐµÐ½Ð¸Ñ‚ÑŒ Ñ€Ð°ÑÐ¿Ð¾Ð»Ð¾Ð¶ÐµÐ½Ð¸Ðµ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ñ‹Ñ…
Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÑŒ ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ñƒ

Ð¿Ñ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ:**

int*)0x978748;

cpp_tracef("OnAMissionFlag: Ð°Ð´Ñ€ÐµÑ=0x%08X, Ð·Ð½Ð°Ñ‡ÐµÐ½Ð¸Ðµ=%u", 
           (DWORD)&OnAMissionFlag, OnAMissionFlag);

Ð°Ð´Ñ€ÐµÑÐ°
(IsBadReadPtr((void*)0x978748, sizeof(unsigned int))) cpp_tracef("ÐžÐ¨Ð˜Ð‘ÐšÐ: Ð½ÐµÐ²Ð°Ð»Ð¸Ð´ÐµÐ½!");
cpp_tracef("ÐÐ´Ñ€ÐµÑ Ð²Ð°Ð»Ð¸Ð´ÐµÐ½");
Ð´Ð¸Ð°Ð¿Ð°Ð·Ð¾Ð½Ð° Ð¸Ð½Ð´ÐµÐºÑÐ°
>= OnAMissionFlag=%u Ð²Ð½Ðµ ScriptSpace!", OnAMissionFlag);
(IsBadWritePtr(&CTheScripts::ScriptSpace[0], 260512)) **Ð—Ð°Ð¿ÑƒÑÑ‚Ð¸Ñ‚ÑŒ ÑÐ¼Ð¾Ñ‚Ñ€ÐµÑ‚ÑŒ Ñ‚Ñ€ÐµÐ¹Ñ:**
Ð½ÐµÐ²Ð°Ð»Ð¸Ð´ÐµÐ½ Ð¿ÐµÑ€ÐµÐ¼ÐµÑÑ‚Ð¸Ð» 260512 Ð¼ÑƒÑÐ¾Ñ€Ð½Ð¾Ðµ Ð·Ð½Ð°Ñ‡ÐµÐ½Ð¸Ðµ
Ð²ÑÑ‘ OK Ð¿Ñ€Ð°Ð²Ð¸Ð»ÑŒÐ½Ñ‹Ð¹

Ð Ð•ÐšÐžÐœÐ•ÐÐ”ÐÐ¦Ð˜Ð¯

**ÐÐ• Ð¢Ð ÐžÐ“ÐÐ¢Ð¬ ÐŸÐ Ð˜ ÐŸÐÐ£Ð—Ð•** Q3):

Ð’Ð¡Ð Ð¿Ñ€Ð¾ OnAMissionFlag:
        ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ðµ
        (ScriptSpace Ñ‚Ñ€Ð¾Ð³Ð°ÐµÐ¼)");
    **ÐŸÑ€Ð¸Ñ‡Ð¸Ð½Ñ‹:**
Ð²Ð»Ð¸ÑÐµÐ¼ Ð¤Ð˜ÐÐÐ›Ð¬ÐÐ«Ð™ ÐŸÐ›ÐÐ Ð˜Ð¡ÐŸÐ ÐÐ’Ð›Ð•ÐÐ˜Ð™

Ð¨Ð°Ð³ Ð”Ð˜ÐÐ“ÐÐžÐ¡Ð¢Ð˜ÐšÐ Ð“Ð°Ñ€Ð°Ð½Ñ‚Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð½Ñ‹Ð¹ Ð»Ð¾Ð²ÐµÑ† **Ð¦ÐµÐ»ÑŒ:** ÐŸÐ¾Ð½ÑÑ‚ÑŒ, ÐŸÐžÐ§Ð•ÐœÐ£ Ð»Ð¾Ð²Ð¸Ñ‚ÑÑ.

(plugin.cpp, DllMain):**

(ÐŸÐ•Ð Ð’Ð«Ð™)
vectored_handler(EXCEPTION_POINTERS* HANDLE CreateFileA("lualoader/crash_vectored.txt", GENERIC_WRITE, NULL, 
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (f INVALID_HANDLE_VALUE) buf[256];
        len snprintf(buf, sizeof(buf), 
                          "VECTORED: Ð¿Ð¾Ñ‚Ð¾Ðº=%lu\n",
                          ex->ExceptionRecord->ExceptionCode,
                                    GetCurrentThreadId());
        DWORD written;
        WriteFile(f, buf, len, &written, CloseHandle(f);
    Filter
unhandled_handler(EXCEPTION_POINTERS* CreateFileA("lualoader/crash_unhandled.txt", 
                           CREATE_ALWAYS, 
                          "UNHANDLED: posix_handler(int "w");
    fileno(f));
        
        fclose(f);
    std::terminate
atexit
Ð²Ñ‹Ñ…Ð¾Ð´)\n");
        install_all_handlers() vectored_handler);
    SetUnhandledExceptionFilter(unhandled_handler);
    
    struct posix_handler;
    
    std::set_terminate(terminate_handler);
    DllMain:
BOOL APIENTRY DllMain(HMODULE hModule, reason, LPVOID reserved) (reason DLL_PROCESS_ATTACH) install_all_handlers();
    TRUE;
**Ð—Ð°Ð¿ÑƒÑÐº Ð¿Ñ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ:**
ÐšÐ°ÐºÐ¾Ð¹ Ð¿Ð¾Ð½Ð¸Ð¼Ð°ÐµÐ¼, Ð»Ð¾Ð²Ð¸Ñ‚ÑÑ
Ð”Ð•Ð¢ÐÐ›Ð¬ÐÐžÐ• Ð›ÐžÐ“Ð˜Ð ÐžÐ’ÐÐÐ˜Ð• gameProcessEvent

Ð¢Ð¾Ñ‡Ð½Ð¾ Ð¾Ð¿Ñ€ÐµÐ´ÐµÐ»Ð¸Ñ‚ÑŒ, (plugin.cpp):**

pause_requested=true, Ð¾Ð±Ñ€Ð°Ð±Ð¾Ñ‚ÐºÐ¸");
        
        Ð£Ð‘Ð˜Ð ÐÐ•Ðœ Q3):
        OnAMissionFlag);
        scripts_paused=true");
        ÐŸÐ¾ÑÐ»ÐµÐ´Ð½ÑÑ ÐºÑ€Ð°ÑˆÐµÐ¼ Ð¿Ð¾ÐºÐ°Ð¶ÐµÑ‚ Ð¼ÐµÑÑ‚Ð¾.

Ð‘ÐÐ Ð¬Ð•Ð ÐÐÐ¯ Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸ÑÐ¼Ð¸

Ð“Ð°Ñ€Ð°Ð½Ñ‚Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ, Ð¿Ð°ÑƒÐ·Ðµ.

Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ñ‹Ðµ

reload-Ð¿Ð¾Ñ‚Ð¾Ðº

scripts_paused=true");
    ÑÐµÐº retry 500Ð¼Ñ)
    count);
        #1, 
            deadline #2, Ð¿Ñ€Ð¸Ð½ÑƒÐ´Ð¸Ñ‚ÐµÐ»ÑŒÐ½Ð¾Ðµ Ð—Ð°Ð¿Ñ€Ð¾ÑÐ¸Ñ‚ÑŒ Ð¿Ð¾Ñ‚Ð¾ÐºÐµ
    pump_owner

Ð Ð•Ð“Ð˜Ð¡Ð¢Ð ÐÐ¦Ð˜Ð¯
    active_pump_threads.fetch_add(1);
    (thread=%lu), %d", 
               GetCurrentThreadId(), Ð±ÐµÐ·Ð¾Ð¿Ð°ÑÐ½Ð°Ñ Ð·Ð¾Ð½Ð° 
                       (void*)L, active_pump_threads.load());
            Ð·Ð¾Ð½Ñƒ Ð²Ð°Ñˆ Ñ‚ÐµÐºÑƒÑ‰Ð¸Ð¹ ÐºÐ¾Ð´)
        Ð¿Ð°Ð¼ÑÑ‚ÑŒ, ÐºÐ¾Ñ‚Ð¾Ñ€Ð°Ñ ÐºÐ¾Ð½Ñ„Ð»Ð¸ÐºÑ‚Ð¾Ð²Ð°Ñ‚ÑŒ.

ScriptSpace:
        1);
        
        Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÑÐµÑ‚ÑÑ");
        
        Ð¿Ñ€Ð¸Ð¼ÐµÐ½ÐµÐ½Ð¾");
    Ð»Ð¾Ð³Ð¸ÐºÐ°
(Ð¾Ð¿Ñ†Ð¸Ð¾Ð½Ð°Ð»ÑŒÐ½Ð¾)

Ñ€Ð°Ð· ÑÑ‚Ð°Ñ€Ñ‚Ðµ):**

validate_game_addresses() cpp_tracef("validate_game_addresses: OnAMissionFlag
    (OnAMissionFlag) ÐÐ•Ð’ÐÐ›Ð˜Ð”Ð•Ð!");
    Ð°Ð´Ñ€ÐµÑ=0x978748, cpp_tracef("ÐŸÐ Ð•Ð”Ð£ÐŸÐ Ð•Ð–Ð”Ð•ÐÐ˜Ð•: (Ð²Ð½Ðµ Ð´Ð¸Ð°Ð¿Ð°Ð·Ð¾Ð½Ð°)", ScriptSpace
    (IsBadWritePtr((void*)CTheScripts::ScriptSpace, CTheScripts::ScriptSpace cpp_tracef("CTheScripts::ScriptSpace: Ð°Ð´Ñ€ÐµÑ=%p, Ñ€Ð°Ð·Ð¼ÐµÑ€=260512", 
                   (void*)CTheScripts::ScriptSpace);
    
    cpp_tracef("validate_game_addresses: Ð’Ñ‹Ð·Ð²Ð°Ñ‚ÑŒ plugin_init DllMain
Ð’ÐžÐ—ÐžÐ‘ÐÐžÐ’Ð›Ð•ÐÐ˜Ð•

(reload-Ð¿Ð¾Ñ‚Ð¾Ðº):**

Ð²Ð¾Ð·Ð¾Ð±Ð½Ð¾Ð²ÑÑ‚ÑÑ
    
    Ð˜Ð¢ÐžÐ“ÐžÐ’Ð«Ð™ Ð§Ð•ÐšÐ›Ð˜Ð¡Ð¢

Ð”Ð¸Ð°Ð³Ð½Ð¾ÑÑ‚Ð¸ÐºÐ° (Ð¿Ñ€Ð¸Ð¾Ñ€Ð¸Ñ‚ÐµÑ‚ 1)

[ ] **Ð²ÑÐµ handlers** (vectored, unhandled, POSIX, terminate, atexit)
**Ð´ÐµÑ‚Ð°Ð»ÑŒÐ½Ð¾Ðµ Ð»Ð¾Ð³Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð¸Ðµ** (Ð”Ðž Ð¾Ð¿ÐµÑ€Ð°Ñ†Ð¸Ð¸)
**crash_*.txt** Ñ„Ð°Ð¹Ð»Ñ‹
ÐŸÑ€Ð¾Ð²ÐµÑ€Ð¸Ñ‚ÑŒ Ð»Ð¾Ð³** (`WINEDEBUG=+seh reVC.exe`)
**dmesg/journalctl** killer)

Ð˜ÑÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¸Ðµ 2)

Ð ÐµÐ°Ð»Ð¸Ð·Ð¾Ð²Ð°Ñ‚ÑŒ **Ð±Ð°Ñ€ÑŒÐµÑ€Ð½ÑƒÑŽ Ð¿Ð°ÑƒÐ·Ñƒ** 0 Ð¸Ð·Ð¼ÐµÐ½ÐµÐ½Ð¸ÑÐ¼Ð¸)
**Ð’Ð¡Ð• API** (wait, load_model, create_ped Ñ‚.Ð´.)
ÐŸÑ€Ð¾Ñ‚ÐµÑÑ‚Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ Ð¿Ð°ÑƒÐ·Ñƒ/Ð²Ð¾Ð·Ð¾Ð±Ð½Ð¾Ð²Ð»ÐµÐ½Ð¸Ðµ **Ð±ÐµÐ·** Ð¿Ð°Ð¼ÑÑ‚Ð¸

Ð’Ð°Ð»Ð¸Ð´Ð°Ñ†Ð¸Ñ 3)

`IsBadReadPtr`
Ð›Ð¾Ð³Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ ÑÑ‚Ð°Ñ€Ñ‚Ðµ
Ð£Ð±ÐµÐ´Ð¸Ñ‚ÑŒÑÑ, Ð²Ð°Ð»Ð¸Ð´ÐµÐ½

Ð”Ð¾Ð»Ð³Ð¾ÑÑ€Ð¾Ñ‡Ð½Ð¾ Ð Ð°ÑÑÐ¼Ð¾Ñ‚Ñ€ÐµÑ‚ÑŒ (Ð²Ñ‹Ð¿Ð¾Ð»Ð½ÐµÐ½Ð¸Ðµ Ð¿Ð¾Ñ‚Ð¾ÐºÐµ)
`std::mutex` ÐºÑ€Ð¸Ñ‚Ð¸Ñ‡ÐµÑÐºÐ¸Ðµ (Ð·Ð°Ñ‰Ð¸Ñ‚Ð° Ð²Ñ‹Ð·Ð¾Ð²Ð¾Ð²)

Ð—ÐÐšÐ›Ð®Ð§Ð•ÐÐ˜Ð•

**ÐÐ°Ð¸Ð±Ð¾Ð»ÐµÐµ Ð²ÐµÑ€Ð¾ÑÑ‚Ð½Ð°Ñ Ð¿Ñ€Ð¸Ñ‡Ð¸Ð½Ð° ÐºÑ€Ð°ÑˆÐ°:**

ÑÑ€Ð°Ð±Ð°Ñ‚Ñ‹Ð²Ð°ÐµÑ‚** Ð¸Ð·-Ð·Ð° Ñ‚Ñ€Ð°Ð½ÑÑ„Ð¾Ñ€Ð¼Ð°Ñ†Ð¸Ð¸ handlers
**Data race** ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ð°Ñ…, ÐºÐ¾Ð³Ð´Ð° Ð°ÐºÑ‚Ð¸Ð²Ð½Ñ‹ Ð±Ð°Ñ€ÑŒÐµÑ€Ð½Ð°Ñ Ð¿Ð°ÑƒÐ·Ð°
**CMessages::AddMessageJumpQ** ÐºÐ¾Ð½Ñ„Ð»Ð¸ÐºÑ‚Ð¾Ð²Ð°Ñ‚ÑŒ Ñ†Ð¸ÐºÐ»Ð¾Ð¼ Ð´ÐµÑ‚Ð°Ð»ÑŒÐ½Ñ‹Ð¼ Ð»Ð¾Ð³Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð¸ÐµÐ¼

**Ð¤Ð¸Ð½Ð°Ð»ÑŒÐ½Ð°Ñ ÑÑ…ÐµÐ¼Ð°:**

**Ð’Ð¡Ð•** Ð¿Ð°ÑƒÐ·Ð°:** Ð¶Ð´Ð°Ñ‚ÑŒ Ñ‚Ñ€Ð¾Ð³Ð°Ñ‚ÑŒ** Ð¿Ð°ÑƒÐ·Ðµ
`CMessages`
Ð”ÐµÑ‚Ð°Ð»ÑŒÐ½Ð¾Ðµ ÐºÐ°Ð¶Ð´Ð¾Ð¼ ÑˆÐ°Ð³Ðµ

ÑÑ‚Ð¸Ñ… Ð¸ÑÐ¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð¸Ð¹ Ð´Ð¾Ð»Ð¶ÐµÐ½ Ð»Ð¸Ð±Ð¾ **Ð¸ÑÑ‡ÐµÐ·Ð½ÑƒÑ‚ÑŒ**, **Ð¿Ð¾Ð¹Ð¼Ð°Ð½** Ð¾Ð´Ð½Ð¸Ð¼ Ñ‚Ð¾Ñ‡Ð½Ñ‹Ð¼ ÑƒÐºÐ°Ð·Ð°Ð½Ð¸ÐµÐ¼ Ð¼ÐµÑÑ‚Ð°.