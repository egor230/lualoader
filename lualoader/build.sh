#!/bin/bash
# Сборка плагина lualoader (Lua-моддинг GTA Vice City) против НОВОГО plugin-sdk.
# Готовый lualoader.asi кладётся НА УРОВЕНЬ ВЫШЕ этой папки (в корень игры).
#
# Запуск:            ./build.sh
#                    ./build.sh clean
# Lua 5.3.5 встраивается статически (исходники в ./lua), внешняя lua53.dll не нужна.

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
        -D__cpp_concepts=202202L -DGTAVC -DPLUGIN_SGV_10EN -DRW)

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

# 2. Плагин
o="$BUILD/plugin.o"
echo "CXX plugin.cpp"
$CXX "${COMMON[@]}" -std=c++23 -w "${INCLUDES[@]}" -c plugin.cpp -o "$o"
OBJS+=("$o")

# 3. Линковка
echo "LINK ${NAME}.asi"
$CXX --target=i686-w64-mingw32 -m32 "${OBJS[@]}" -shared \
    -static-libgcc -static-libstdc++ \
    -L"$SDK/output/lib" -lPlugin_VC \
    -o "$OUT"

echo ""
echo "ГОТОВО: $OUT ($(du -h "$OUT" | cut -f1))"
