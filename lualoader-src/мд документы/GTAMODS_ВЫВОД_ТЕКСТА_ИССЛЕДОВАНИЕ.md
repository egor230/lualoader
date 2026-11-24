# GTAMods Wiki: вывод игрового текста в GTA Vice City

Источники (прочитаны 25.11.2026):
- `https://gtamods.com/wiki/Memory_Addresses_(VC)` — разделы 1–9 (Cheats, Taxi, Display,
  Mp3 player, Loading screen, Mouse, Menu, Stats, Controller), запрошенные «2–3 раздела выше #Controller»;
- `https://gtamods.com/wiki/Text` — типы текста и опкоды;
- `https://gtamods.com/wiki/Function_Memory_Addresses_(VC)` — функции CFont / CMessages / CText / CHud;
- `https://gtamods.com/wiki/GXT` — формат GXT и токены;
- локально: `Plugin-SDK/plugin_vc/game_vc/*.cpp`, `lualoader-src/funcs.cpp` — сверка адресов.

---

## 1. Разделы Memory Addresses (VC) выше #Controller

### Taxi (2)
| Адрес | Тип | Что |
|---|---|---|
| `0x687574` | string | текст `TAXI` |
| `0x42B24E` | 4 bytes | длительность показа текста `TAXI` |
| `0x42B219` | byte | цвет fade-in (синий), `+0x2` зелёный, `+0x2` красный |
| `0x5B8AB6` | byte | плата за посадку в такси (12 по умолчанию) |
| `0x94DD94` | float | X-координата точки назначения (обновляется 058E), `+0x4` Y, `+0x4` Z |
| `0xA0D378` | float | угол точки назначения |

### Display (3)
| Адрес | Тип | Что |
|---|---|---|
| `0x869650` | byte | субтитры (subtitles) |
| `0x86963A` | byte | HUD |
| `0x7E46F5` | byte | широкие чёрные поля (widescreen borders) |
| `0xA10AF6` | byte | replay вкл/выкл |
| `0x68AEE0/0x68AEE4` | float | верхняя/нижняя граница «кино»-полос |
| `0xA0FD00/0xA0FD04/0xA0FD08` | dword | цветовой режим / ширина / высота экрана |

### Loading screen (5)
| Адрес | Тип | Что |
|---|---|---|
| `0xA0CE94` | float | прогресс загрузки |
| `0x68E6FC` | float | шаг прогресса |
| `0x68E70C/0x68E710` | float | ширина/высота полосы |
| `0x68E704/0x68E708` | float | расстояние слева/снизу |
| `0x4A6C24/29/2E/33` | byte | цвет полосы A/B/G/R (движущейся) |
| `0x4A6B77/7C/7E/80` | byte | цвет полосы A/B/G/R |

**Важно для нас:** `0xA0CE94` (прогресс) находится всего на 8 байт раньше `0xA0CE9C` —
глобального буфера имени файла сохранения, который использует `CheckSlotDataValid`.
Это разные поля, но соседние.

### Menu (7)
| Адрес | Тип | Что |
|---|---|---|
| `0x703997` | byte | выбранный пункт меню (см. список экранов ниже) |
| `0x869728` | byte | текущий menu id (SDK: `m_nCurrentMenuPage`, int) |
| `0x869660` | byte | выбранный пункт (SDK: `m_nCurrentMenuEntry`, int) |
| `0x783F12` | string | каталог сохранений |
| **`0x97509C`** | string | **Save Game DIR** — префикс имени файла (тот самый, что мы используем) |
| `0x68BF58` | string | имя текстуры фона меню |
| `0x68BF68` | string | имя текстуры логотипа меню |
| `0x68BF7C` | string | имя текстуры мыши |
| `0x869694/0x869698` | float | позиция мыши X/Y |

Список экранов по `0x703997` (совпадает с `eMenuScreen` в SDK):
`00 Stats, 01 Start Game, 03 Audio, 04 Display, 05 Language, 06 Map, 07 InGame/New Game,
08 Load Game, 09 Delete Game, 10 «All unsaved progress…», 11 «Proceed with deleting…»,
12 → 01, 13 «Deleting Saved game. Please Wait…», 14 «Delete Successful», 15 Save Game List,
16 «Are you sure you want to save…», 17 «Saving current game…», 18 «Save Successful»,
19 пустой экран, 20 предупреждение о читах, 21 Skin, 26 Controller, 27 Options, 29 Main Menu,
30 Redefine Controls, 31 Mouse Settings, 32 Resume, 33/34 «Greetings From Vice City», 35+ пусто`.

### Stats (8)
`0x4CC102` — всего магазинов для ограбления (15); `0x4CC1CD` — всего контрактов на убийство (5).

### Controller (9)
`0x869630` — `FrontEndMenuManager (CMenuManager)`. Смещения по вики:

| + | Имя по вики | Имя в Plugin-SDK | Наш адрес |
|---|---|---|---|
| 0x0A | m_bHudMode | `m_bPrefsShowHud` | 0x86963A |
| 0x18 | m_PrefsBrightness | `m_nPrefsBrightness` | — |
| 0x20 | m_bSubtitles | `m_bSubtitles` | 0x869650 |
| 0x22 | m_bWidescreen | `m_bPrefsUseWideScreen` | 0x869652 |
| 0x25 | m_bFrameLimiter | `m_bFrameLimiter` | 0x869655 |
| 0x38 | m_bMenuVisible | **`m_bMenuActive`** | 0x869668 |
| 0x39 | m_bDemoQuit | **`m_bWantToRestart`** | 0x869669 |
| 0x3C | m_bExtraSplash | **`m_bWantToLoad`** | 0x86966C |
| 0x4C | m_nGameLanguage | `m_nPrefsLanguage` | 0x86967C |
| 0x5B | controller state | `m_nControlMethod` | — |
| 0x6C | m_bGameStarted | **`m_bGameNotLoaded`** | 0x86969C |
| 0x84 | m_apMenuSprites[26] | `m_aMenuSprites[26]` | 0x8696B4 |
| 0xF8 | m_nCurrentPage | `m_nCurrentMenuPage` | 0x869728 |

⚠️ **Расхождение имен.** Вики называет смещения 0x38/0x39/0x3C иначе, чем Plugin-SDK.
По фактическому поведению (проверено дизассемблом `gta-vc.exe` и рабочим CLEO-модом)
верна **терминология SDK**: 0x39 = `m_bWantToRestart`, 0x3C = `m_bWantToLoad`.
Вики-метки «m_bDemoQuit / m_bExtraSplash» — устаревшие/неточные. Адреса совпадают.

---

## 2. Массив Intro text — основа вывода текста (033E)

Адрес `0x7F0EA0`, размер элемента `0xF4`, количество `0x30` (48 слотов).

| Смещение | Тип | Initial | Что | Опкод |
|---|---|---|---|---|
| 0x00 | float[2] | 0.48, 1.12 | X/Y scale | 033F SET_TEXT_SCALE |
| 0x08 | byte[4] | 225,225,225,255 | RGBA цвет | 0340 SET_TEXT_COLOUR |
| 0x0C | byte | 0 | justify | 0341 |
| 0x0D | byte | 0 | centre | 0342 |
| 0x0E | byte | 0 | background | 0345 |
| 0x0F | byte | 0 | background only text | 0347 |
| 0x10 | float | 182.0 | wrap x | 0343 |
| 0x14 | float | 640.0 | centre size | 0344 |
| 0x18 | byte[4] | 128,128,128,128 | RGBA фона | 0346 |
| 0x1C | byte | 1 | proportional | 0348 |
| 0x1D | byte | 0 | draw before fade | 03E0 |
| 0x1E | byte | 0 | right justify | 03E4 |
| 0x20 | word | 1 | font | 0349 |
| 0x24 | float[2] | 0,0 | X/Y позиция | 033E DISPLAY_TEXT |
| 0x2C | wchar_t[100] | null | **сам текст** | 033E / 045A / 045B |

Опкод `03F0 USE_TEXT_COMMANDS` включает работу с этими слотами.

---

## 3. Типы текста (/wiki/Text)

1. **Message** — внизу экрана, белый по умолчанию (`~h~`). Очередь **до 8 сообщений**.
   `PRINT` (00BB) добавляет в конец очереди, `PRINT_NOW` (00BC) — в начало с перезаписью,
   `PRINT_SOON` — на вторую позицию. Последние уникальные сообщения попадают в раздел
   Brief меню паузы. С субтитрами связаны `~z~` и настройка 0x869650.
   Опкоды с числами: 01E4/01E5, 02FD, 02FF, 0302, 0308, 0384.
2. **Help message** — сверху слева, тёмный полупрозрачный фон, серый `~w~`.
   03E5 PRINT_HELP, 03E6 CLEAR_HELP, 0512 PRINT_HELP_FOREVER.
   Хардкод: CHEAT1..CHEAT7, GA_6/GA_7/GA_8, GA_21, PROP_1/PROP_2, PU_CF1/3/4.
   Нативная функция: **CHud::SetHelpMessage @0x55BFC0** (cmecb: `wchar_t*, bool, bool`).
3. **Big message** — крупный стилизованный текст по центру или справа внизу.
   00BA PRINT_BIG, 0217 PRINT_BIG_Q, 01E3, 036D, 03D6 CLEAR_THIS_BIG_PRINT.
   Приоритет: высокий (`AddBigMessage`) сразу перезаписывает, низкий (`AddBigMessageQ`) — в очередь.
   Хардкод: BUSTED (5000 мс, стиль 3), DEAD (4000, стиль 3), GOODBOY (5000, стиль 1),
   TAXI (4500, стиль 2).
4. **Intro text** — см. §2 (полная кастомизация, 48 слотов).
5. **Garage message** — крупный синий текст по центру, только хардкод:
   CO_ALL, CO_ONE, CR_1, GA_1..GA_20, PU_MONY и др.
   Нативная функция **CGarages::TriggerMessage @0x42EDE0** (GXT key, number, duration, number).

---

## 4. Функции вывода (/wiki/Function_Memory_Addresses_(VC) + сверка с SDK)

### CText (GXT-словарь)
| Адрес | Функция | Подтверждено SDK |
|---|---|---|
| `0x584F30` | `CText::Get(char const*)` | ✅ `CText.cpp:12` (`CallMethodAndReturn`) |
| `0x585150` | `CText::LoadMissionText(char const*)` | ✅ |
| `0x5850B0` | `CText::UnloadMissionText()` | ✅ |
| `0x5854F0` | `CText::Load()` | ✅ |
| `0x585A80` | `CKeyArray::BinarySearch` | ✅ |

### CMessages (очереди сообщений)
| Адрес | Функция | Подтверждено SDK |
|---|---|---|
| `0x584410` | `AddMessage` | ✅ `CMessages.cpp:28` |
| `0x584300` | `AddMessageJumpQ` | ✅ `CMessages.cpp:33` |
| `0x584050` | `AddBigMessage` | ✅ |
| `0x583F40` | `AddBigMessageQ` | ✅ |
| `0x583560` | `AddMessageWithNumber` | ✅ |
| `0x583350` | `AddBigMessageWithNumber` | ✅ |
| `0x583C80` | `InsertNumberInString` (токен `~1~`, до 6 чисел) | ✅ |
| `0x583AF0` | `InsertStringInString` | ✅ |
| `0x583DE0` | `AddToPreviousBriefArray` | ✅ |
| `0x582FA0` / `0x582CF0` | `ClearThisPrint` / `ClearThisBigPrint` | ✅ |
| `0x5841E0` / `0x582C70` | `ClearMessages` / `ClearAllMessagesDisplayedByGame` | ✅ |
| `0x5849C0` | `Init` | ✅ |

### CFont (низкоуровневый вывод)
| Адрес | Функция |
|---|---|
| `0x551040` | `CFont::PrintString(float x, float y, wchar_t*)` ✅ `CFont.cpp:178` |
| `0x551620` | `CFont::PrintSomething` (PrintStringFromBottom) ✅ |
| `0x550650` | `GetTextLength` |
| `0x550260` / `0x5502D0` / `0x550510` | `RemoveTokens` / `ParseToken` / `ParseToken(+CRGBA)` |
| `0x550170` | `SetColor(CRGBA&)` |
| `0x550230` | `SetScale(float,float)` |
| `0x5500D0` / `0x5500E0` | `SetBackgroundOff` / `SetBackgroundOn` |
| `0x550110` / `0x550120` | `SetCentreOff` / `SetCentreOn` |
| `0x550100` | `SetWrapx` |
| `0x550140` / `0x550150` | `SetJustifyOff` / `SetJustifyOn` |
| `0x550040` / `0x550060` | `SetRightJustifyOff` / `On` |
| `0x550020` / `0x550030` | `SetPropOn` / `SetPropOff` |
| `0x5500A0` | `SetBackgroundColor` |
| `0x550200` / `0x550210` | `SetRotation` / `SetRotationOffset` |

### CHud / прочее
| Адрес | Функция |
|---|---|
| `0x55BFC0` | `CHud::SetHelpMessage(wchar_t*, bool, bool)` ✅ `CHud.cpp:137` |
| `0x557320` | `CHud::Draw()` ✅ `CHud.cpp:74` |
| `0x42EDE0` | `CGarages::TriggerMessage` |
| `0x487200` | `CDebug::DebugDisplayTextBuffer` |
| `0x4A6190` | `Render2DStuff` |
| `0x4A6510` | `RenderEffects` |
| `0x4A69D0` | `LoadingScreen` |

---

## 5. GXT (формат storing)

VC (`GTA VC/LCS/VCS`): блоки **TABL → TKEY → TDAT**.
- `TABL`: [char8 name][dword absolute offset], отсортирован по алфавиту, кроме первого `MAIN`.
- `TKEY`: [char8 name][char4 "TKEY"][dword size], затем записи [dword offset][char8 key], отсортированы.
- `TDAT`: [char4 "TDAT"][dword size], строки **UTF-16 (wchar_t)**, null-terminated, НЕ отсортированы.

Токены VC: `~1~` число, `~b~` синий, `~g~` горячий розовый, `~h~` highlight/белый,
`~l~` цвет по умолчанию, `~n~` пустой символ (гашение субтитров), `~o~` розовый,
`~p~` фиолетовый, `~q~` сливовый, `~r~` горячий розовый, `~t~` зелёный,
`~w~` белый, `~x~` светло-голубой, `~y~` жёлтый, `~f~` мигание.
Одиночный `~` вызывает краш игры.

---

## 6. Как это используется в нашем LuaLoader

- `printmessage()` → `CMessages::AddMessageJumpQ` (const wchar_t*, time, style) — см. `funcs.cpp:1564`.
- `show_text_key()` → сначала `CText::Get(key)`; если ключ из `MAIN`, `LOAD_MISSION_TEXT` не нужен;
  если из миссионной таблицы — `Command<COMMAND_LOAD_MISSION_TEXT>(table)`.
- `ma_msgpool` — пул широких строк: `CMessages::Add*` **не копируют** `wchar_t*`,
  поэтому указатель должен жить до конца показа.
- Массив `0x7F0EA0` (Intro text) — готовый механизм для вывода произвольного текста
  в произвольной точке экрана с кастомизацией (48 слотов), если понадобится расширить API.

---

## 7. НАЙДЕННАЯ ОШИБКА `show_text_key()` И ЕЁ ИСПРАВЛЕНИЕ (25.11.2026)

### Симптом
В логе было:
```
GXT: show_text_key called key=COK1_6
GXT: source=MAIN — LOAD_MISSION_TEXT НЕ нужен
GXT: executing on game thread — PRINT_NOW
GXT: PRINT_NOW command executed
```
но **текст на экране не появлялся**.

### Причина (два независимых бага)

1. **Регистр ключа.** Дизассембл `CText::Get` (`0x584F30`) показывает, что он
   передаёт ключ прямо в `CKeyArray::BinarySearch` (`0x585A80`), а та вызывает
   функцию `0x642620` — обычный **case-sensitive `strcmp`** (цикл `mov al,[ecx]` /
   `cmp al,[edx]`, без приведения регистра). Ключи в GXT TKEY хранятся В ВЕРХНЕМ
   регистре (`COK1_6`). Помощник `ma_run_label_command2()` **переводил ключ в
   нижний регистр** (`'cok1_6'`), опираясь на неверный комментарий «reVC приводит
   имя к нижнему регистру». Реальный байткод `main.scm` подтверждает обратное:
   `bc 00 47 4f 41 57 41 59 31 00 ...` — `'GOAWAY1'` в верхнем регистре.
   Поэтому `CText::Get('cok1_6')` возвращал `NULL` и PRINT_NOW «выполнялся»,
   ноне находил строку — экран оставался пустым.

2. **Формат параметров.** В настоящем SCM `00BC PRINT_NOW` после 8 сырых байт
   ключа идут `05` (int16, 2 байта — время) и `04` (int8, 1 байт — стиль),
   например `bc 00 47 4f 41 57 41 59 31 00 05 88 13 04 01`. Помощник писал
   `01` + 4-байтовые int для обоих параметров — несовместимо с родным форматом.

### Исправление
`show_text_key()` больше **не подделывает `CRunningScript`** для PRINT_NOW.
На игровом потоке (`ma_cmdq_defer`) он:
1. вызывает `TheText.Get(key)` — исходный ключ в верхнем регистре, строка находится;
2. если ключ из миссионной таблицы — сначала `LOAD_MISSION_TEXT`, затем повторный `Get`;
3. копирует готовую UTF-16 строку в `ma_msgpool::putw()` (движок **не копирует** `wchar_t*`);
4. вызывает проверенный `CMessages::AddMessageJumpQ(w, time, style)` — тот же путь,
   что и в `printmessage()`.

Добавлена новая функция `ma_msgpool::putw(const wchar_t*)` — копирует уже готовую
UTF-16 строку без конверсии.

### Подтверждение
После пересборки (`BUILD-GATE OK: _GLOBAL__sub_I=30, .tls=0x24`) и запуска игры
в лог пишется:
```
GXT: show_text_key called key=COK1_6
GXT: source=MAIN — LOAD_MISSION_TEXT НЕ нужен
GXT: AddMessageJumpQ выполнен, time=3000
```
Реплика «I'm sick of these pricks.» появляется на экране (пользователь подтвердил).
Тот же путь работает и для миссионных ключей: `BNK4_3J` → `LOAD_MISSION_TEXT BANKJ4` →
`AddMessageJumpQ`.

### Остаточное замечание
`ma_run_label_command2()` (используется для LOAD_MISSION_AUDIO и др.) всё ещё
приводит ключ к нижнему регистру — это отдельный потенциальный баг, но на вывод
текста он больше не влияет. При возникновении проблем с озвучкой проверять в первую очередь его.
