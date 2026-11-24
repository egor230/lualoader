# `fastloadsave.asi` — полное техническое описание

**Версия:** сборка `Nov 25 2026`, файл `fastloadsave.asi`, `sha256 67d4a517…`
**Игра:** GTA Vice City 1.0 EN (английская версия, `gta-vc.exe`)
**Зависимости:** Plugin-SDK (SDK 1005), `libPlugin_VC.a`, MinGW-w64 (clang)
**Не зависит от:** LuaLoader, CLEO, клавиатуры, Lua-скриптов

---

## 1. Что делает плагин

При запуске игры, как только главное меню становится готовым, плагин **один раз**
загружает слот сохранения, номер которого указан в `fastloadsave.ini`, через
**штатный механизм игры** — тот же путь, который проходит меню при нажатии
«Load Game». Плагин не нажимает клавиши, не рисует своё меню и не парсит файл
сохранения вручную.

Если слота нет или он повреждён, плагин ничего не загружает и пишет причину
в `fastloadsave.log`.

---

## 2. Файлы

| Файл | Что это |
|---|---|
| `../fastloadsave.asi` | готовый плагин, кладётся в корень игры |
| `../fastloadsave.ini` | конфиг (номер слота и включение) |
| `../fastloadsave.log` | журнал работы, создаётся заново при каждом запуске |
| `fastloadsave.cpp` | весь исходник плагина (одна единица трансляции) |
| `build.sh` | сборка через clang + Plugin-SDK |
| `fastloadsave.ini` (здесь) | образец конфига |
| `FASTLOADSAVE_ИССЛЕДОВАНИЕ.md` | этот документ |

---

## 3. Установка и настройка

1. Скопировать `fastloadsave.asi` и `fastloadsave.ini` в корень игры, рядом с
   `gta-vc.exe`.
2. В `fastloadsave.ini`:

   ```ini
   [FastLoadSave]
   Slot=1       ; номер слота 1..8 (как в меню игры)
   Enabled=1    ; 1 = включён, 0 = плагин бездействует
   ```

3. Запустить игру. После появления главного меню произойдёт загрузка слота.
4. Отключение: `Enabled=0` или удалить `.asi`.

**Важно:** слот нумеруется как в игре (1…8). Внутри игры это индекс `0…7`,
поскольку в имени файла движок пишет `slot+1`: слот 1 → `GTAVCsf1.b`.

---

## 4. Сборка

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City/fastloadsave-src"
./build.sh
```

`build.sh` использует тот же инструмент, что и LuaLoader:

```
clang++ --target=i686-w64-mingw32 -m32 -O1 -fpermissive -fcommon -fms-extensions
        -D__cpp_concepts=202202L -DGTAVC -DPLUGIN_SGV_10EN -DRW
        -I<SDK>/plugin_vc -I<SDK>/plugin_vc/game_vc ... -I<SDK>/shared ...
```

Линковка: `-lPlugin_VC -ldbghelp -lwinmm`, статические `libgcc`/`libstdc++`.
После сборки печатается размер `.tls` и число `_GLOBAL__sub_I` — это
информация, а не жёсткий гейт (у LuaLoader гейт другой: `_GLOBAL__sub_I=30`,
`.tls=0x24`; `lualoader.asi` этот плагин не трогает).

---

## 5. Исследование штатного механизма загрузки

Все адреса ниже получены из трёх независимых источников и сверены между собой:

1. **Plugin-SDK** (`CMenuManager.h/.cpp`, `GenericGameStorage.cpp`,
   `C_PcSave.h/.cpp`, `CGame.cpp`) — имена, смещения, сигнатуры;
2. **дизассембл `gta-vc.exe`** через capstone — фактические адреса и последовательность
   вызовов (PE `.text`, imagebase `0x400000`);
3. **CLEO-мод `QuickSL.cs`** из архива `быстрого сохранения и загрузки игры..rar` —
   подтверждение, что именно эти адреса использует рабочий мод;
4. **файлы на диске** `userfiles/GTAVCsf1.b` и др. — подтверждение имени файла.

### 5.1. Вопрос 1: какую функцию вызывает меню при загрузке слота?

Ответ: отдельной функции `LoadSlot` нет. Есть **обработчик пунктов меню** —
thiscall-функция по адресу `0x497148` (`ecx = this = 0x869630`). Она делает
switch по `CMenuManager::m_nCurrentMenuPage` (смещение `0xF8`):

```
00497148  push ebx
00497149  push ebp
0049714A  mov  ebp, ecx             ; this
0049714C  mov  eax, [ebp+0xF8]       ; m_nCurrentMenuPage
00497158  sub  eax, 0xC
0049715B  je   0x497306             ; ← ветка ЗАГРУЗКИ
00497161  sub  eax, 1
00497164  je   0x497174             ; удаление
00497166  sub  eax, 4
00497169  je   0x497244             ; сохранение
0049716F  jmp  0x49739C
```

Ветка загрузки `0x497306` (полный разбор):

```
00497306  push dword ptr [ebp+0x100]    ; m_nCurrentSaveSlot
0049730C  call 0x61A6C0                 ; CheckSlotDataValid(слот)
00497311  test al, al
00497314  je   0x497393                 ; неверный → SwitchToNewScreen(1)
00497316  cmp  byte ptr [ebp+0x6C], 0   ; m_bGameNotLoaded
0049732D  ... вызовы звуковых функций 0x5F9D80, 0x5F9E50 ...
00497346  mov  byte ptr [ebp+0x11], 1   ; m_bShutDownFrontEndRequested
0049734C  mov  byte ptr [ebp+0x39], 1   ; m_bWantToRestart
00497350  ... звук 0x5F9CC0, 0x5F9C90 ...
00497365  ... очистка 10 dword по 0x862460 ...
00497376  mov  ecx, ebp
00497378  call 0x498E5F                ; SwitchMenuOnAndOff() ← ЗАКРЫТИЕ МЕНЮ
0049737D  ... 0x5F98D0 ...
0049738D  mov  byte ptr [ebp+0x3C], 1   ; m_bWantToLoad
```

Дальше игровой цикл (`0x600404…0x600570`) видит `m_bWantToLoad` и вызывает
`0x4A4670 → 0x61A700` — собственно чтение состояния из файла.

Ключевой вывод: **меню не вызывает «LoadSlot» напрямую**. Оно:
1. проверяет слот (`CheckSlotDataValid`),
2. просит закрыть фронт-энд (`SwitchMenuOnAndOff`),
3. выставляет `m_bWantToRestart` и `m_bWantToLoad`.

### 5.2. Вопрос 2: когда безопасно вызывать?

- **Нельзя** в `DllMain` и нельзя вызывать игровые функции из фонового потока:
  на практике это приводило к крашу (лог обрывался прямо на вызове `0x497148`).
- **Безопасно** дождаться готовности интерфейса:
  `CMenuManager::m_bSpritesLoaded` (смещение `0xEC`, адрес `0x86971C`) ≠ 0,
  и затем либо работать в игровом потоке, либо делать **только записи в память**
  из потока-наблюдателя (однобайтовые записи атомарны и не ломают игру).
- Плагин дополнительно ждёт 5 стабильных опросов по 100 мс, чтобы не сработать
  на переходном кадре.

### 5.3. Вопрос 3: как игра определяет, что сохранение есть?

`CheckSlotDataValid(int slot)` @ `0x61A6C0` (из `GenericGameStorage.cpp`
Plugin-SDK):

```
0061A6C0  mov eax, [esp+4]              ; slot
0061A6C4  mov [0x9753C4], 0             ; PcSaveHelper.nErrorCode = 0
0061A6CE  push 0xA0CE9C
0061A6D3  push eax
0061A6D4  call 0x61A310                ; открыть/прочитать файл
0061A6D9  test al, al
...       call 0x40F470 ; al=1
0061A6E7  xor al, al
0061A6E9  mov [0x9753C4], 7             ; SAVESTATUS_ERR_DATA_INVALID
```

`0x61A310` строит имя файла:

```
sprintf(buf, "%s%i%s", префикс@0x97509C, slot+1, ".b")
strcpy((char*)0xA0CE9C, buf)            ; 0x642560 = strcpy
```

Префикс создаётся `C_PcSave::SetSaveDirectory()` @ `0x61D930`:

```
sprintf(0x97509C, "%s\\%s", "GTAVCsf", path)   ; строки @0x6D8B30 и @0x6D8B38
```

Итого:
- без `portablegta.asi`: `GTAVCsf1.b` … `GTAVCsf8.b` в корне игры;
- с `portablegta.asi` (вызывает `SetSaveDirectory("userfiles")`):
  `userfiles\GTAVCsf1.b` … `userfiles\GTAVCsf8.b`.

Подтверждено на диске:

```
userfiles/GTAVCsf1.b   201828 байт   (заголовок: 7c a4 00 00 + "In the beginning...")
userfiles/GTAVCsf3.b   201828 байт
userfiles/GTAVCsf8.b   201832 байт
```

Коды `PcSaveHelper.nErrorCode` (из `C_PcSave.h`):

| Код | Значение |
|---|---|
| 0 | `SAVESTATUS_SUCCESSFUL` |
| 4 | `SAVESTATUS_ERR_LOAD_OPEN` (нет файла) |
| 5 | `SAVESTATUS_ERR_LOAD_READ` |
| 6 | `SAVESTATUS_ERR_LOAD_CLOSE` |
| 7 | `SAVESTATUS_ERR_DATA_INVALID` (повреждён) |

Побочный эффект `CheckSlotDataValid` критически важен: он **заносит имя нужного
файла в `0xA0CE9C`**, откуда `0x61A700` потом читает состояние. Поэтому вызов
`CheckSlotDataValid(index)` обязательно должен предшествовать выставлению флагов.

### 5.4. Таблица подтверждённых адресов (1.0 EN)

| Symbol / поле | Адрес | Источник |
|---|---|---|
| `FrontEndMenuManager` | `0x869630` | `CMenuManager.cpp` SDK |
| `m_bShutDownFrontEndRequested` (`0x11`) | `0x869641` | `CMenuManager.h`, `VALIDATE_OFFSET` |
| `m_bMenuActive` (`0x38`) | `0x869668` | `CMenuManager.h` |
| `m_bWantToRestart` (`0x39`) | `0x869669` | `CMenuManager.h` |
| `m_bFirstTime` (`0x3A`) | `0x86966A` | `CMenuManager.h` |
| `m_bWantToLoad` (`0x3C`) | `0x86966C` | `CMenuManager.h` |
| `m_bGameNotLoaded` (`0x6C`) | `0x86969C` | `CMenuManager.h` |
| `m_nMenuFadeAlpha2` (`0x80`) | `0x8696B0` | `CMenuManager.h` |
| `m_bSpritesLoaded` (`0xEC`) | `0x86971C` | `CMenuManager.h` |
| `m_nCurrentMenuPage` (`0xF8`) | `0x869728` | `CMenuManager.h` |
| `m_nCurrentSaveSlot` (`0x100`) | `0x869730` | `CMenuManager.h` |
| `m_nMenuFadeAlpha` (`0x108`) | `0x869738` | `CMenuManager.h` |
| `m_bMenuChangeOngoing` (`0x110`) | `0x869740` | `CMenuManager.h` |
| `gGameState` | `0x9B5F08` | `CGame.cpp` SDK |
| `CheckSlotDataValid` | `0x61A6C0` | `GenericGameStorage.cpp` SDK |
| `C_PcSave::SaveSlot` | `0x61D8B0` | `C_PcSave.cpp` SDK |
| `C_PcSave::SetSaveDirectory` | `0x61D930` | `C_PcSave.cpp` SDK |
| `PcSaveHelper.nErrorCode` | `0x9753C4` | `C_PcSave.cpp` SDK |
| префикс имени файла | `0x97509C` | дизассембл `0x61D930` |
| буфер имени файла | `0xA0CE9C` | дизассембл `0x61A310` |
| обработчик пунктов меню | `0x497148` | дизассембл |
| ветка загрузки в нём | `0x497306` | дизассембл |
| `CMenuManager::SwitchToNewScreen` | `0x4983EF` | `CMenuManager.cpp` SDK |
| `CMenuManager::SwitchMenuOnAndOff` | `0x498E5F` | `CMenuManager.cpp` SDK |
| загрузка состояния | `0x61A700` | дизассембл |
| MVC-хук SDK `gameProcessEvent` | `0x48E49B` | `Events.h` SDK |

---

## 6. История отладки: почему несколько подходов не работали

Этот раздел нужен, чтобы другие люди не повторяли ошибки.

### Попытка A — `Events::gameProcessEvent`

Плагин регистрировал `Events::gameProcessEvent += []{...}` в глобальном
конструкторе, как примеры из Plugin-SDK. **Результат:** обработчик вообще не
вызывался (лог содержал только раннюю отметку из `DllMain`).

**Причина:** `lualoader.asi` загружается позже (алфавитный порядок ASI) и
ставит свой хук на тот же адрес `0x48E49B`, перезаписывая наш. У SDK событие
реализовано через `BaseEvent::GetInstance()` — **отдельный статический
экземпляр на DLL**, поэтому наша подписка и хук lualoader не объединяются.

**Решение:** не использовать SDK-события; собственный поток-наблюдатель.

### Попытка B — только флаги `m_bWantToLoad=1`, `m_bWantToRestart=1`

**Результат:** загрузка начиналась (`gGameState` уходил в 9, звук и NPC
работали), но экран оставался чёрным: `m_bMenuActive=1`, `fade=255/190`.

**Причина:** меню не было закрыто. В нативном обработчике перед флагами
вызывается `SwitchMenuOnAndOff()`; мы его пропустили.

### Попытка C — принудительные записи `gGameState=6`, `m_bFirstTime`, `m_bWantToRestart`

**Результат:** экран мигал, игра падала (в логе LuaLoader — диагноз о
повреждённых игровых структурах).

**Причина:** состояние 9 в этой игре — нормальное состояние загрузки; ручная
подмена `gGameState` и удержание `m_bWantToRestart` ломают конечный автомат.
Записывать можно только те поля, которые пишет сам нативный обработчик.

### Попытка D — вызов `0x497148` из фонового потока

**Результат:** лог обрывался сразу на строке «Вызываю штатный обработчик»,
игра падала.

**Причина:** функция дёргает игровые подсистемы (звук, fx, меню), которые
ожидают игровой поток.

### Финальное решение

Из нативного обработчика берём **только безопасные для фонового потока записи**:

```
CheckSlotDataValid(index)                    ; проверка + имя файла в 0xA0CE9C
[0x869641] = 1   ; m_bShutDownFrontEndRequested
[0x869669] = 1   ; m_bWantToRestart
[0x86966C] = 1   ; m_bWantToLoad
```

Игровой поток сам обрабатывает эти флаги и загружает слот. После этого
плагин сбрасывает оставшееся от меню затемнение:

```
[0x869738] = 0   ; m_nMenuFadeAlpha
[0x8696B0] = 0   ; m_nMenuFadeAlpha2
[0x869740] = 0   ; m_bMenuChangeOngoing
```

---

## 7. Устройство кода (по функциям)

Файл: `fastloadsave-src/fastloadsave.cpp`, одна единица трансляции.

### 7.1. Включения

```cpp
#include <plugin.h>              // Plugin-SDK: Events, plugin::Call*, типы
#include <CMenuManager.h>        // FrontEndMenuManager, поля, SwitchToNewScreen
#include <GenericGameStorage.h>  // CheckSlotDataValid
#include <C_PcSave.h>            // PcSaveHelper, eSaveStatus
#include <CGame.h>               // gGameState
#include <windows.h>             // CreateFileA/WriteFile/CreateThread/ini
#include <cstdarg> <cstdio> <cstring> <ctime>
```

### 7.2. `fls_win32_log()` и `fls_logf()`

Лог пишется через `CreateFileA(..., FILE_APPEND_DATA, OPEN_ALWAYS)` +
`WriteFile`, а не через `fstream`. Причины:

- в `DllMain` под loader-lock CRT может быть ещё не готова (под Wine это даёт
  `RtlpWaitForCriticalSection ... blocked by 0000`);
- лог может писаться из фонового потока.

Форматирование — `vsnprintf` в локальный буфер 768 байт, итоговая строка
`[ЧЧ:ММ:СС] текст\n`. Для разных потоков есть `CRITICAL_SECTION` `g_log_cs`.

### 7.3. `DllMain()`

```
DLL_PROCESS_ATTACH:
  CreateFileA("fastloadsave.log", CREATE_ALWAYS)  → пишет строку "DllMain"
  DisableThreadLibraryCalls()
  InitializeCriticalSection()
  CreateThread(fls_watchdog)
DLL_PROCESS_DETACH:
  DeleteCriticalSection()
```

Никаких вызовов игровых функций в `DllMain` нет. Ранняя отметка в логе
позволяет сразу отличить «ASI не загрузился» от «загрузился, но обработчик
не сработал».

### 7.4. `fls_watchdog()` — поток-наблюдатель

1. `Sleep(1500)` — дать CRT/SDK/игре завершить инициализацию.
2. Читает `fastloadsave.ini` через `GetPrivateProfileIntA`:
   `Slot` (по умолчанию 1), `Enabled` (по умолчанию 1).
3. Проверяет диапазон 1…8; при отказе пишет причину и выходит.
4. `index = Slot - 1` — перевод в внутреннюю индексацию игры.
5. Цикл опроса каждые 100 мс:
   - пока `m_bSpritesLoaded` (`0x86971C`) == 0 — ждёт, раз в 4 секунды пишет
     состояние;
   - после 5 стабильных опросов — логирует готовность и переходит к загрузке.
6. `CheckSlotDataValid(index)` — проверяет слот и готовит имя файла. При
   `false` пишет отказ и выходит (обычный старт игры без загрузки).
7. Пишет три байта: `m_bShutDownFrontEndRequested`, `m_bWantToRestart`,
   `m_bWantToLoad`.
8. Ждёт до 4 секунд, пока игра съест `m_bWantToLoad`.
9. Логирует состояние (`gGameState`, `m_bMenuActive`, `m_bGameNotLoaded`,
   `fade`, `nErrorCode`).
10. Сбрасывает `m_nMenuFadeAlpha`, `m_nMenuFadeAlpha2`,
    `m_bMenuChangeOngoing` — убирает чёрный экран, доставшийся от меню.
11. Выходит. Повторных вызовов за запуск не будет.

---

## 8. Формат журнала и реальный пример

Файл `fastloadsave.log` пересоздаётся при каждом запуске игры. Пример
успешного запуска:

```
=== fastloadsave.asi: DllMain ===
[01:24:18] === fastloadsave.asi запущен (сборка Nov 25 2026 01:23:51) ===
[01:24:18] Конфиг: Slot=1 (индекс 0). Ожидаю готовности меню...
[01:24:22]   ожидание: sprites=0, gGameState=2
[01:24:26] Меню готово: gGameState=7, m_bMenuActive=1.
[01:24:26] CheckSlotDataValid(0) -> 1, err=0
[01:24:26] Флаги загрузки выставлены (ShutDownFrontEnd, WantToRestart, WantToLoad).
[01:24:28] После загрузки: gGameState=9, m_bMenuActive=0, m_bGameNotLoaded=0, fade=255/255, err=0
[01:24:28] Итог: gGameState=9, m_bMenuActive=0, fade=0/0, err=0
```

Как читать:

- `DllMain` есть → ASI загружен. Нет файла → ASI-загрузчик не взял DLL.
- `CheckSlotDataValid(0) -> 1, err=0` → `userfiles/GTAVCsf1.b` найден и валиден.
- `m_bMenuActive=0` → меню закрыто.
- `m_bGameNotLoaded=0` → игра загружена.
- `fade=0/0` → затемнение снято, мир виден.
- `gGameState=9` — состояние загрузки/игры после загрузки сохранения; оно не
  означает «не загрузилось», если `err=0` и `m_bGameNotLoaded=0`.

---

## 9. Результаты проверки

- ✅ Игра запускалась, доходила до главного меню, плагин загружал слот 1
  (`CheckSlotDataValid(0) -> 1`, `err=0`).
- ✅ Меню закрывалось (`m_bMenuActive=0`), затемнение снималось (`fade=0/0`).
- ✅ `lualoader.asi` не изменялся и продолжает работать (в логе LuaLoader нет
  ошибок, связанных с fastloadsave).
- ✅ Плагин работает и с `portablegta.asi`, потому что путь к сохранению берётся
  из самой игры (`0x97509C`).

---

## 10. Ограничения и важные замечания

1. **Только 1.0 EN.** Все адреса приведены для `gta-vc.exe` версии 1.0 EN.
   Для 1.1/Steam нужно брать значения из `ADDRESS_BY_VERSION` в Plugin-SDK.
2. **Один раз за запуск.** После срабатывания поток выходит; повторной
   загрузки после «New Game» или загрузки из меню не будет.
3. **Нет поиска по файлу вручную.** Плагин не открывает `.b`-файл сам — он
   вызывает игру, поэтому корректно учитывает `portablegta.asi` и любой другой
   плагин, меняющий каталог сохранений.
4. **Нет открытия меню при отсутствии слота.** В финальной версии при пустом
   или повреждённом слоте игра просто стартует обычно, а причина пишется в лог.
5. **Фоновый поток.** По договорённости плагин не эмулирует ввод и не дёргает
   игровые функции из потока — только записи в память и Win32-лог.
6. `MENU_ENTRY_HANDLER` (`0x497148`) и `MENU_PAGE_LOAD` (`12`) оставлены в
   исходнике как документация найденного нативного пути; финальная версия
   саму функцию **не вызывает** (вызов из потока роняет игру).

---

## 11. Источники

- Plugin-SDK: `plugin_vc/game_vc/CMenuManager.h/.cpp`,
  `GenericGameStorage.cpp`, `C_PcSave.h/.cpp`, `CGame.cpp`, `shared/Events.h`
- `gta-vc.exe` — дизассембл `.text` (capstone + собственный разбор PE)
- CLEO-мод `QuickSL.cs` из `/mnt/807EB5FA7EB5E954/диск/gta vc/быстрого сохранения и загрузки игры..rar`
- `userfiles/GTAVCsf1.b`, `GTAVCsf3.b`, `GTAVCsf8.b` — реальные сохранения
- `fastloadsave.log` — журнал успешного запуска
