#!/bin/bash
# Сборка отдельного плагина fastloadsave.asi (не зависит от LuaLoader).
# Результат кладётся в корень игры: ../fastloadsave.asi
# Запуск: ./build.sh   |   ./build.sh clean

set -e
cd "$(dirname "$0")"

GAME="/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
SDK="$GAME/Plugin-SDK"
NAME="fastloadsave"
OUT="../${NAME}.asi"
BUILD="obj"

if [ "$1" = "clean" ]; then rm -rf "$BUILD"; echo "очищено"; exit 0; fi

[ -f "$SDK/output/lib/libPlugin_VC.a" ] || { echo "нет libPlugin_VC.a — сначала собери SDK"; exit 1; }

mkdir -p "$BUILD"
CC=clang
CXX=clang++

COMMON=(--target=i686-w64-mingw32 -m32 -O1 -fpermissive -fcommon -fms-extensions
        -Wno-invalid-offsetof -Wno-microsoft-include -Wno-builtin-macro-redefined
        -D__cpp_concepts=202202L -DGTAVC -DPLUGIN_SGV_10EN -DRW)
INCLUDES=(-I"$PWD"
          -I"$SDK/plugin_vc" -I"$SDK/plugin_vc/game_vc" -I"$SDK/plugin_vc/game_vc/enums"
          -I"$SDK/plugin_vc/game_vc/rw" -I"$SDK/shared" -I"$SDK/shared/game")

SRC="fastloadsave.cpp"
OBJ="$BUILD/fastloadsave.o"
echo "CXX  $SRC"
$CXX "${COMMON[@]}" -std=c++23 -w "${INCLUDES[@]}" -c "$SRC" -o "$OBJ"

echo "LINK ${NAME}.asi"
$CXX --target=i686-w64-mingw32 -m32 "$OBJ" -shared \
    -static-libgcc -static-libstdc++ \
    -L"$SDK/output/lib" -lPlugin_VC \
    -ldbghelp -lwinmm \
    -o "$OUT"

# Информация: тот же __emutls из libstdc++, что и в рабочем lualoader.asi
# (там .tls=0x24). Наш плагин использует только статические C++ строки.
TLS_HEX=$(objdump -h "$OUT" 2>/dev/null | awk '$2==".tls"{print $3; exit}')
TLS=$((16#${TLS_HEX:-00}))
SUB_I=$(nm -C "$OUT" 2>/dev/null | grep -c '_GLOBAL__sub_I' || true)
echo "Инфо: .tls=$TLS, _GLOBAL__sub_I=$SUB_I (CGame.cpp, C_PcSave.cpp, наш TU, libstdc++)"

echo ""
echo "ГОТОВО: $OUT ($(du -h "$OUT" | cut -f1))"
