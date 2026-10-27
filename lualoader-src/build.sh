#!/bin/bash
# Сборка плагина lualoader (Lua-моддинг GTA Vice City) против НОВОГО plugin-sdk.
# Готовый lualoader.asi кладётся НА УРОВЕНЬ ВЫШЕ этой папки (в корень игры).
#
# Запуск:            ./build.sh
#                    ./build.sh clean
# Lua 5.4.9 встраивается статически (исходники в ./lua), внешняя DLL не нужна.

set -e
cd "$(dirname "$0")"

GAME="/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
SDK="$GAME/Plugin-SDK"
NAME="lualoader"
OUT="../${NAME}.asi"
BUILD="obj"

if [ "$1" = "clean" ]; then rm -rf "$BUILD"; echo "очищено"; exit 0; fi

[ -f "$SDK/output/lib/libPlugin_VC.a" ] || { echo "нет libPlugin_VC.a — сначала собери SDK (build-linux.sh в Plugin-SDK)"; exit 1; }

mkdir -p "$BUILD"
CC=clang
CXX=clang++

COMMON=(--target=i686-w64-mingw32 -m32 -O1 -fpermissive -fcommon -fms-extensions
        -Wno-invalid-offsetof -Wno-microsoft-include -Wno-builtin-macro-redefined
        -D__cpp_concepts=202202L -DGTAVC -DPLUGIN_SGV_10EN -DRW
        -DLUALOADER_CMDQ -DLUALOADER_DIAG)

INCLUDES=(-I"$PWD" -I"$PWD/lua"
          -I"$SDK/plugin_vc" -I"$SDK/plugin_vc/game_vc" -I"$SDK/plugin_vc/game_vc/enums"
          -I"$SDK/plugin_vc/game_vc/rw" -I"$SDK/shared" -I"$SDK/shared/game")

OBJS=()

# 1. Lua 5.3.5 (статически)
for c in lua/*.c; do
    o="$BUILD/lua_$(basename "${c%.c}").o"
    [ "$c" -nt "$o" ] && { echo "CC  $c"; $CC "${COMMON[@]}" -std=c11 -O2 -w "${INCLUDES[@]}" -c "$c" -o "$o"; }
    OBJS+=("$o")
done

# 2. Плагин (3 единицы трансляции; funcs.h — общий заголовок для всех трёх)
#    funcs.cpp    — реализации всех Lua-функций и общие глобалы
#    register.cpp — таблица регистрации lua_register (funs)
#    plugin.cpp   — точка входа ASI, загрузка/остановка скриптов, потоки
for c in funcs.cpp register.cpp plugin.cpp; do
    o="$BUILD/${c%.cpp}.o"
    if [ "$c" -nt "$o" ] || [ "funcs.h" -nt "$o" ]; then
        echo "CXX  $c"
        $CXX "${COMMON[@]}" -std=c++23 -w "${INCLUDES[@]}" -c "$c" -o "$o"
    fi
    OBJS+=("$o")
done

# 2b. compat: диагностика + ОЧЕРЕДЬ КОМАНД (LUALOADER_CMDQ) + МЕНЕДЖЕР СТРИМИНГА.
#     Command<> из потоков скриптов больше НЕ пишут в игровую память — они
#     откладываются и исполняются на игровом потоке в Events::gameProcessEvent
#     (см. compat/diag.cpp, cmdq_hooks.cpp). Загрузка моделей идёт через
#     compat/ma_streaming.cpp (REQUEST шлёт кадровый обработчик, без кругов очереди).
#     Глобальных конструкторов в этих TU нет — иначе игра не стартует
#     (RtlpWaitForCriticalSection "... blocked by 0000"). Регистрация хуков — из
#     Message в plugin.cpp, ПОСЛЕ инициализации SDK.
#     + compat/ma_tasks.cpp — кооперативные задачи игрового потока (keyboard,
#       mission_watch, getcord/turncar): раньше файл не входил в сборку, хотя
#       plugin.cpp уже вызывал ma_tasks_frame/ma_task_start_keyboard.
#     + compat/ma_sched.cpp — ПЛАНИРОВЩИК LUA-СКРИПТОВ (23.10.2026): один поток
#       на все .lua, wait() = парковка корутины (lua_yield), вытеснение по
#       бюджету count-хуком. Ноль OS-потоков на скрипт.
for c in compat/diag.cpp compat/cmdq_hooks.cpp compat/ma_streaming.cpp compat/ma_tasks.cpp compat/ma_sched.cpp; do
    o="$BUILD/$(basename "${c%.cpp}").o"
    if [ "$c" -nt "$o" ] || [ "compat/diag.h" -nt "$o" ] || [ "compat/ma_streaming.h" -nt "$o" ] || [ "compat/ma_tasks.h" -nt "$o" ] || [ "compat/ma_sched.h" -nt "$o" ]; then
        echo "CXX  $c"
        $CXX "${COMMON[@]}" -std=c++23 -w "${INCLUDES[@]}" -c "$c" -o "$o"
    fi
    OBJS+=("$o")
done

# 3. Линковка
echo "LINK ${NAME}.asi"
$CXX --target=i686-w64-mingw32 -m32 "${OBJS[@]}" -shared \
    -static-libgcc -static-libstdc++ \
    -L"$SDK/output/lib" -lPlugin_VC \
    -ldbghelp \
    -o "$OUT"

# 4. BUILD-GATE (правило 13.10: compat/* не добавляют глобальных конструкторов
# и TLS — std::thread / thread_local / нетривиальные глобалы = в Wine игра не
# стартует, RtlpWaitForCriticalSection "... blocked by 0000"). Плохой .asi удаляется.
if [ -z "$LUALOADER_SKIP_GATE" ]; then
    GATE_SUB_I=$(nm -C "$OUT" 2>/dev/null | grep -c "_GLOBAL__sub_I" || true)
    GATE_TLS_HEX=$(objdump -h "$OUT" 2>/dev/null | awk '$2==".tls"{print $3; exit}')
    GATE_TLS=$((16#${GATE_TLS_HEX:-00}))
    if [ "${GATE_SUB_I:-0}" -ne 30 ] || [ "$GATE_TLS" -ne 36 ]; then
        echo "BUILD-GATE FAIL: _GLOBAL__sub_I=$GATE_SUB_I (ожидается 30), .tls=$GATE_TLS (ожидается 36 = 0x24)"
        echo "Плохой .asi удалён. Аварийный обход: LUALOADER_SKIP_GATE=1 ./build.sh"
        rm -f "$OUT"
        exit 1
    fi
    echo "BUILD-GATE OK: _GLOBAL__sub_I=30, .tls=0x24"
fi

echo ""
echo "ГОТОВО: $OUT ($(du -h "$OUT" | cut -f1))"
