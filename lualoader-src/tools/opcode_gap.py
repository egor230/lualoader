#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Сверка опкодов SCM с реализованными в lualoader (отчёт «чего нет для переноса миссий»).

ЧТО ДЕЛАЕТ
  1. Берёт ВСЕ опкоды игры из SDK-перечисления
     Plugin-SDK/plugin_vc/game_vc/enums/eScriptCommands.h (1475 имён COMMAND_* с кодами).
  2. Ищет, какие из них лоадер реально исполняет — по вызовам Command<COMMAND_*>
     в funcs.cpp / funcs.h / register.cpp / plugin.cpp / compat/*.cpp|h.
  3. Разбирает файлы оригинальных миссий «Source missions/Оригинал игры (97 миссий)/*.txt»
     (декод опкодов от reVC; строки вида «  00A5: CREATE_CAR 175 83.5 -851 10 0@»)
     и собирает, какие опкоды и в скольких миссиях используются.
  4. Печатает ТОП нереализованных опкодов. Опкоды «уровня языка» (GOTO/IF/WAIT,
     арифметика переменных, GOSUB/CREATE_THREAD/TERMINATE_THIS_SCRIPT) в отчёт
     не попадают: в Lua их заменяют сам язык и каркас миссии (см. документ 03).

ЗАПУСК (из любой папки)
    python3 tools/opcode_gap.py            # топ-80 нереализованных
    python3 tools/opcode_gap.py 200        # топ-200
    python3 tools/opcode_gap.py 0          # только сводка

ВАЖНО ПРО ТРАКТОВКУ
  «Нет в Command<>» != «функции нет в Lua-API». Часть функций лоадера реализована
  прямыми вызовами движка (setpedhealth, getcarcoordes, is_ped_in_car, set_wanted…),
  поэтому перед реализацией сверяйте имя с register.cpp и документом 04.
  Семантику опкода брать из reVC: «gta vc база/re3-miami.zip» ->
  src/control/Script.cpp, Script2.cpp, Script3.cpp, Script4.cpp (case COMMAND_*).
"""
import collections
import glob
import os
import re
import sys

GAME = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
ENUM = os.path.join(GAME, "Plugin-SDK/plugin_vc/game_vc/enums/eScriptCommands.h")
SRC = os.path.join(GAME, "lualoader-src")
MISS = os.path.join(GAME, "Source missions/Оригинал игры (97 миссий)")

# Опкоды уровня языка/каркаса: в Lua не нужны — их заменяют сам язык и каркас миссии.
#   * любое имя с _VAR — переменные и арифметика/сравнения опкодами;
#   * переходы/потоки/ожидание/завершение — каркас `while ... wait()` и корутины.
LANG_NAMES = {
    "COMMAND_NOP", "COMMAND_WAIT", "COMMAND_GOTO", "COMMAND_GOTO_IF_FALSE",
    "COMMAND_GOSUB", "COMMAND_RETURN", "COMMAND_IF", "COMMAND_IF_NOT",
    "COMMAND_ANDOR", "COMMAND_LAUNCH_MISSION", "COMMAND_SCRIPT_NAME",
    "COMMAND_TERMINATE_THIS_SCRIPT", "COMMAND_START_NEW_SCRIPT",
    "COMMAND_CREATE_THREAD", "COMMAND_MISSION_HAS_FINISHED",
    "COMMAND_PLAYER_MADE_PROGRESS", "COMMAND_SET_THREAD", "COMMAND_IS_SCRIPT_RUNNING",
}


def is_lang(name):
    return ("_VAR" in name) or ("LVAR" in name) or (name in LANG_NAMES)


def load_opcode_names():
    """COMMAND_XXX -> код опкода."""
    name2code = {}
    with open(ENUM, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = re.match(r"\s*(COMMAND_[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+)", line)
            if m:
                name2code[m.group(1)] = int(m.group(2), 16)
    return name2code


def load_implemented():
    """Опкоды, которые лоадер исполняет через Command<COMMAND_*>."""
    files = [os.path.join(SRC, n) for n in ("funcs.cpp", "funcs.h", "register.cpp", "plugin.cpp")]
    files += sorted(glob.glob(os.path.join(SRC, "compat", "*.cpp")))
    files += sorted(glob.glob(os.path.join(SRC, "compat", "*.h")))
    names = set()
    for path in files:
        if os.path.exists(path):
            with open(path, encoding="utf-8", errors="replace") as f:
                names |= set(re.findall(r"Command<\s*(COMMAND_[A-Z0-9_]+)", f.read()))
    return names


def load_missions():
    used = collections.Counter()
    in_mission = collections.defaultdict(set)
    example = {}
    line_re = re.compile(r"^\s*([0-9A-Fa-f]{4}):\s*(.*)$")
    for path in sorted(glob.glob(os.path.join(MISS, "*.txt"))):
        base = os.path.basename(path)
        with open(path, encoding="utf-8", errors="replace") as f:
            for line in f:
                m = line_re.match(line)
                if not m:
                    continue
                op = int(m.group(1), 16) & 0x7FFF          # снять бит NOT (ветка 8xxx)
                used[op] += 1
                in_mission[op].add(base)
                example.setdefault(op, m.group(2).strip()[:64])
    return used, in_mission, example


def main():
    top = int(sys.argv[1]) if len(sys.argv) > 1 else 80
    name2code = load_opcode_names()
    code2name = {v: k for k, v in name2code.items()}
    used_names = load_implemented()
    supported = {name2code[n] for n in used_names if n in name2code}
    used, in_mission, example = load_missions()

    def lang(op):
        name = code2name.get(op, "")
        return is_lang(name)

    missing = [op for op in used if op not in supported]
    candidates = [op for op in missing if not lang(op)]
    candidates.sort(key=lambda o: (-len(in_mission[o]), -used[o]))

    print("Опкодов в SDK: %d | исполняет лоадер (Command<>): %d" % (len(name2code), len(supported)))
    print("ВНИМАНИЕ: часть функций лоадера реализована прямыми вызовами движка, без Command<>")
    print("          (setpedhealth, getcarcoordes, is_ped_in_car, get_game_timer…). Перед")
    print("          реализацией сверяйте имя с register.cpp и документом 04.")
    print("Опкодов в 97 миссиях: %d | из них не реализовано: %d | «не языковых» не реализовано: %d"
          % (len(used), len(missing), len(candidates)))
    if top <= 0:
        return
    print("\n%-7s %9s %8s %9s  %-40s %s" % ("опкод", "вхожд.", "миссий", "число", "имя опкода (SDK)", "пример из миссии"))
    for op in candidates[:top]:
        print("0x%04X %9d %8d %9d  %-40s %s"
              % (op, used[op], len(in_mission[op]), op,
                 (code2name.get(op, "?").replace("COMMAND_", "")), example[op]))


if __name__ == "__main__":
    main()
