// Регистрация разбора очереди команд на ИГРОВОМ потоке.
//
// ⛔ ГЛАВНОЕ (2026-10-07) — ЗДЕСЬ БОЛЬШЕ НЕТ ГЛОБАЛЬНОГО КОНСТРУКТОРА.
//
// БЫЛО: struct CmdQueueHookInstaller с глобальным объектом g_cmdqHookInstaller.
// Его конструктор выполнялся при загрузке DLL и писал в plugin::Events::gameProcessEvent
// (список std::list из SDK) — то есть РАНЬШЕ, чем инициализировался сам plugin.o.
// Порядок динамической ини��ализации в PE/COFF ОБРАТНЫЙ порядку объектных файлов
// в командной строке линковки, а линковка шла plugin.o, cmdq_hooks.o, ma_tasks.o,
// diag.o ⇒ cmdq_hooks.o инициализировался ПЕРВЫМ и лез в ещё не готовый список.
// В Wine это давало висяк:
//     RtlpWaitForCriticalSection section ... "?" wait timed out ... blocked by 0000
// и игра НЕ ЗАПУСКАЛАСЬ вообще. То же самое уже было описано в ma_tasks.cpp
// (там конструктор убрали по той же причине), но здесь правило не применили.
//
// СТАЛО: функция ma_cmdq_install_hook() без состояния. Её зовёт Message из
// plugin.cpp — тот же приём, что у всех остальных подписок, и он по построению
// выполняется после инициализации SDK (plugin.o линкуется предпоследним среди
// «ранних» TU, а Events инициализируется раньше нашей Message).
//
// Почему всё ещё отдельный файл, а не код внутри plugin.cpp:
// хуки SDK живут в std::list, и Add() делает hooks.emplace_back(). Вызывать +=
// изнутри обхода ТОГО ЖЕ списка (т.е. из самого Events::gameProcessEvent) список
// портит -> AV в msvcrt. Раньше это и ломало игру: plugin.cpp звал
// Command<COMMAND_CAN_PLAYER_START_MISSION> из своего же хука gameProcessEvent,
// и ленивая регистрация попадала ровно внутрь обхода. Отдельный обработчик —
// единственный способ этого избежать.

#include "diag.h"
#include "Events.h"

namespace {

// Флаг: хук уже зарегистрирован. Идемпотентно — повторный вызов безопасен.
bool g_installed = false;

} // namespace

void ma_cmdq_install_hook() {
    if (g_installed) return;   // повторный вызов ничего не делает
    g_installed = true;

    plugin::Events::gameProcessEvent += [] {
        // ЗАМЕР разбора очереди (06.10). Это ОТДЕЛЬНЫЙ обработчик gameProcessEvent,
        // поэтому в счётчик plugin.cpp он не попадает — а именно сюда уходит
        // основная работа: выполнение Command<>() скриптов. Раз в 600 кадров пишем
        // в log.txt (НЕ через ma_boot — тот флашит файл и сам создавал бы микрофриз).
        const unsigned long long t0 = GetTickCount64();
        const int before = ma_cmdq_pending();
        ma_cmdq_drain();
        const unsigned long long dt = GetTickCount64() - t0;
        static unsigned long long sum = 0, cnt = 0, mx = 0, did = 0;
        sum += dt; cnt++; did += (unsigned long long)(before > 0 ? before : 0);
        if (dt > mx) mx = dt;
        if (cnt >= 600) {
            ma_diag::mark("ЗАМЕР ОЧЕРЕДИ: вызовов=%llu  %.2f мс/вызов (сред)  %.2f мс (макс)  команд всего=%llu",
                (unsigned long long)cnt, (double)sum / (double)cnt, (double)mx,
                (unsigned long long)(did / 600));
            sum = 0; cnt = 0; mx = 0; did = 0;
        }
    };
    ma_diag::mark("CMDQ: разбор очереди команд включён (Events::gameProcessEvent, "
                  "регистрация из Message в plugin.cpp — порядок инициализации соблюдён)");
}