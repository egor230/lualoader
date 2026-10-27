#!/bin/bash
# Стенд планировщика: собирает НАСТОЯЩИЕ lua/*.c и compat/ma_sched.cpp (с заглушками
# игрового слоя) и гоняет сценарий из sched_test.cpp. Linux/gcc; к игре не относится.
set -e
cd "$(dirname "$0")"
ROOT="$(cd .. && pwd)"
OUT="${TMPDIR:-/tmp}/lualoader_sched_test"
rm -rf "$OUT"
mkdir -p "$OUT/luaobj"

for c in "$ROOT"/lua/*.c; do
    base="$(basename "${c%.c}")"
    # lua.c/luac.c — интерпретатор/компилятор, стенду не нужны
    [ "$base" = "lua" ] && continue
    [ "$base" = "luac" ] && continue
    gcc -O2 -w -DLUA_USE_POSIX -I"$ROOT/lua" -c "$c" -o "$OUT/luaobj/$base.o"
done

g++ -std=c++17 -O1 -w -DMA_SCHED_UNIT_TEST \
    -I"$ROOT" -I"$ROOT/lua" -I"$PWD" \
    sched_test.cpp "$ROOT/compat/ma_sched.cpp" \
    "$OUT"/luaobj/*.o -lpthread -ldl -lm -o "$OUT/sched_test"

mkdir -p "$OUT/run"
cd "$OUT/run"
exec ../sched_test
