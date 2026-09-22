# Перенос миссии Treacherous Swine («Подлая свинья»): правила и подходы

> Документ-двойник `ПЕРЕНОС_МИССИИ_RIOT.md`, но для миссии 07 — Treacherous
> Swine (поток GENERL1, смещение 0x49063, 9568 байт, 1093 инструкций, 195 меток).
> Все правила §0–§8 из RIOT-документа действуют и здесь (читать его обязательно);
> ниже — только специфика Swine и **5 багов первой тестовой сессии**.

---

## 0. Состояние миссии

- Файл: `lualoader/misson treacherous swine.lua` (541 строка, BOM+CRLF).
- Оригинал: `Source missions/Оригинал игры (97 миссий)/07 - Treacherous Swine.txt`.
- Запуск: маркер `Star_mission_marker(9, -250.5, -1361.1, 8.1)` (особняк Кортеса).
- **Ревизия 2 (22.09.2026, после первой игры пользователя):** исправлены
  5 критических багов (см. §2). C++ не трогался — все функции уже были в
  текущем `lualoader.asi` (6,4 МБ от 22.09 03:07), нужен только Ctrl-reload.

---

## 1. Логика оригинала (НЕ ИНВЕРТИРОВАТЬ!)

Главная ловушка — направление win/lose кажется обратным здравому смыслу:

- **Свинья УБИТА** → `ADD_ONE_OFF_SOUND` + GEN1_19 +
  `ALTER_WANTED_LEVEL_NO_DROP 2` → игрок едет в мойку (`SET_FREE_RESPRAYS 1`,
  блайп 327.2 429.9 10.3) → `HAS_RESPRAY_HAPPENED` → **M_PASS 250** (УСПЕХ).
- **Свинья СБЕЖАЛА** (достигла 398.3 -469.8 11.9, радиус 15) → катсцена
  побега (`LOAD_SCENE 393.5 -476.4 12.3`, Свинью удаляют) → **M_FAIL**.

То есть убийство цели — это **успех**, а не провал. Первый черновик был
написан с инвертированной логикой — исправлено в ревизии 1.

### Порядок выдачи бензопилы

Оригинал @L499fd даёт `GIVE_WEAPON_TO_PLAYER 11 1` **на пляже, ДО
конфронтации** — игрок сразу может резать. Не переносите выдачу в шаг 3.

---

## 2. ПЯТЬ БАГОВ первой тестовой сессии (22.09.2026)

Пользователь тестировал миссию в игре и нашёл 5 проблем. Корни и фиксы:

### 2.1 Вылет игры (crash)

**Корень 1:** `set_chars_chatting(pig, guard1, 9999999)` — оригинал ставит
бесконечный чат, но в нашем окружении это **блокирует ИИ охраны** и вешает
кадр. **Фикс:** конечный таймер `8000` — к концу диалогов (COL1_5+COL1_1 ≈ 8с)
охрана уже атакует, чат снимается сам.

**Корень 2:** `getpedcoordes(pp)` в ИИ охраны (шаг 4) без `isped(pp)` —
`pp = findplayer()` мог вернуть nil → `findpedinpool(NULL)` → segfault.
**Фикс:** везде `isped(pp) and not ispeddead(guard) and isped(pp)` перед
`getpedcoordes`. См. §3.

### 2.2 Бензопилу надо получить СРАЗУ

**Симптом:** игрок прибегает на пляж, а бензопилы нет — она выдавалась
только в шаге 3 (конфронтация).

**Фикс:** перенос в самое начало `main()`:
```lua
loadmodel(269)                -- 0247 REQUEST_MODEL 269 (chnsaw)
loadmodel(89)                 -- 0247 REQUEST_MODEL 89 (CLA — охрана)
load_requested_models()       -- 038B LOAD_ALL_MODELS_NOW
Giveweaponped(player, 1, "chnsaw")   -- 01B1 GIVE_WEAPON_TO_PLAYER 11 1
set_current_weapon_ped(player, 11)   -- 01B8 — бензопила в руки
```

### 2.3 Двери пентхауса закрыты / нет сферы

**Симптом:** игрок не может зайти в пентхаус; на месте встречи нет сферы.

**Корень:** оригинал @L4a1d2 при переходе к крыше **удаляет объект -118**
(закрытая дверь) и **ставит -76** (открытая дверь). Черновик это пропустил.
Также @L49c2b рисует `DRAW_SPHERE 475.5 30.3 11 3` каждый кадр — без сферы
непонятно, куда идти.

**Фикс:**
```lua
-- дверь пентхауса (оригинал @L4a1d2)
if roof3 ~= nil then remove_obj(roof3) roof3 = nil end
local door_open = Createobj(-76, 465.375, 30.336, 33.181)
if door_open ~= nil then dont_remove_object(door_open) end

-- сфера места встречи (оригинал @L49c2b)
sphere_site = create_sphere(475.5, 30.3, 11.0, 3.0)
```
Сфера снимается при входе в шаг 3 и во всех очистках (шаг 6, 7, пост-цикл).

### 2.4 Нет аудиофразы в пентхаусе

**Корень:** я заменил диалоги на текст, а `play_voice` **уже есть в API**
(`funcs.cpp:2578`: `LOAD_MISSION_AUDIO` + `PLAY_MISSION_AUDIO` + ожидание
`HAS_MISSION_AUDIO_FINISHED` внутри C++, с guard `teardown_active` — не
вешается при reload).

**Фикс:** хелпер + использование реальных имён аудио:
```lua
local function play_voice_line(name, text, text_ms)
	printmessage(text, text_ms, 1)
	play_voice(name)
end
play_voice_line("COL1_5", "...", 4000)   -- GEN1_10
play_voice_line("COL1_1", "...", 4000)   -- GEN1_06
play_voice_line("COL1_3", "...", 4000)   -- GEN1_08 (побег)
play_voice_line("COL1_7", "...", 4000)   -- GEN1_12 (погоня)
```

### 2.5 Два бесконечных цикла

**Корень:** `while get_fading_status() do wait() end` ×2 (шаг 3 и шаг 4).
Если fade по какой-то причине не стартует (например, при reload скриптов),
цикл **никогда** не выйдет — зависание кадра.

**Фикс:** хелпер с конечным ожиданием:
```lua
local function fade_and_wait(fade_dir, dur)
	fade(fade_dir, dur)
	local t = get_game_timer()
	while true do
		wait()
		if not get_fading_status() then break end           -- fade завершился
		if get_game_timer() - t > dur + 2000 then break end  -- страховка +2с
	end
end
```

---

## 3. Правило: isped ПЕРЕД getpedcoordes (главная причина вылетов)

`getpedcoordes` (`funcs.cpp:964`) вызывает `findpedinpool(p)` и **разыменовывает
результат без NULL-проверки**:
```cpp
CPed* ped = findpedinpool(p);
lua_pushnumber(L, ped->GetPosition().x);   // ped == NULL → SEGFAULT
```
`findpedinpool(NULL)` возвращает NULL (не крашит), но любая невалидная
передача далее — вылет. **Всегда**:
```lua
if isped(pp) then local x, y = getpedcoordes(pp) ... end
```
Особенно в **кадровых** местах (ИИ охраны каждые 500 мс) — там `findplayer()`
может вернуть nil в любой момент.

---

## 4. Замены опкодов (нет в API → решение)

| Опкод | Замена | Почему |
|-------|--------|--------|
| `LOAD_CUTSCENE COL_1` | fade + текстовые реплики | опкодов катсцен нет; паттерн из Riot |
| `LOAD_MISSION_AUDIO/PLAY/HAS_*` | `play_voice(name)` | есть в API, внутри C++ делает всю цепочку |
| `HAS_RESPRAY_HAPPENED $684/$685` | `get_player_wanted_level() == 0` | индексы гаражей неизвестны; детектор: no-drop 2 → мойка обнуляет → 2→0 |
| `SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE -95..-112` | пропущено | 18 объектов прячутся только на время катсцены (которой нет) |
| `DRAW_SPHERE` (каждый кадр) | `create_sphere` один раз | постоянная сфера, снимается через `remove_sphere` |

---

## 5. Сводная таблица: опкоды Swine → Lua-функции

| Опкод | Lua | Примечание |
|-------|-----|-----------|
| 029B CREATE_OBJECT_NO_OFFSET | `Createobj(model, x, y, z)` | крыша -116..-119, дверь -76 |
| 01C7 DONT_REMOVE_OBJECT | `dont_remove_object(obj)` | новая (ревизия 1) |
| 0108 DELETE_OBJECT | `remove_obj(obj)` | дверь -118 перед -76 |
| 03A1 DRAW_SPHERE | `create_sphere` / `remove_sphere` | сфера места встречи |
| 03F9 SET_CHARS_CHATTING | `set_chars_chatting(p1, p2, мс)` | **КОНЕЧНЫЙ** таймер! |
| 03CF/03D1/03D2/040D | `play_voice(name)` | целиком внутри C++ |
| 0239 SET_CHAR_OBJ_RUN_TO_COORD | `set_char_obj_run_to_coord` | маршрут Свиньи по крыше |
| 0211 SET_CHAR_OBJ_GOTO_COORD_ON_FOOT | `set_char_obj_goto_coord_on_foot` | |
| 01CA SET_CHAR_OBJ_KILL_PLAYER_ON_FOOT | `set_char_obj_kill_player_on_foot` | |
| 0126 IS_CHAR_OBJECTIVE_PASSED | `is_ped_objective(ped)` | |
| 01BD GET_GAME_TIMER | `get_game_timer()` | новая; таймауты циклов |
| 016B GET_FADING_STATUS | `get_fading_status()` | новая; только внутри `fade_and_wait` |
| 0335 SET_FREE_RESPRAYS | `set_free_resprays(флаг)` | новая |
| 010E ALTER_WANTED_LEVEL_NO_DROP | `wanted_set_no_drop(ур)` | |
| 01B1 GIVE_WEAPON_TO_PLAYER | `Giveweaponped(ped, патр, "имя")` | бензопила = "chnsaw" (11) |
| 01B8 SET_CURRENT_PLAYER_WEAPON | `set_current_weapon_ped(ped, 11)` | |
| 0376 CREATE_RANDOM_CHAR | `create_random_char(x, y, z)` | новая; 3 педа на пляже |
| 01C2 MARK_CHAR_AS_NO_LONGER_NEEDED | `mark_char_as_no_longer_needed(ped)` | новая |
| 0372 SET_CHAR_WAIT_STATE | `set_ped_wait_state(ped, ст, мс)` | |
| 0210/020F TURN_*_TO_FACE_* | `turn_player_to_face_char` / `turn_char_to_face_player` | новые |
| 015F/0160 SET_FIXED_CAMERA_POSITION / POINT_CAMERA_AT_POINT | `set_camera_position` / `camera_at_point` | |
| 02EB RESTORE_CAMERA_JUMPCUT | `restore_camera()` | |
| 022B/022A SWITCH_PED_ROADS_OFF/ON | `ped_road_off` / `ped_road_on` | |
| 055B CREATE_CLOTHES_PICKUP | `create_pickup_clothes(8, 414.3, 1042.0, 25.4)` | **ПОСЛЕ** `miss()`! |

### Одежда ПОСЛЕ miss() — ловушка pickupsids

`create_pickup_clothes` добавляет пикап в map `pickupsids`, а `destroy()`
внутри `miss()` → `end_mission()` удаляет **все** пикапы скрипта. Если
создать одежду до `miss(250)`, награда исчезнет в тот же кадр.

---

## 6. Производительность (кадровый цикл)

- `ped_in_point_in_radius` = `findpedinpool` (1 мс) + `sleep(10 мс)` — **В
  КАДРОВОМ ЦИКЛЕ НЕ ЗВАТЬ** (зависание кадра).
- `getpedcoordes` = `findpedinpool` (1 мс) — не более ~5 вызовов/кадр.
- ИИ охраны рефрешится **раз в 500 мс** через `get_game_timer()`, не каждый
  кадр.
- Дистанции — через локальный хелпер `ped_dist2d_sq` (без sqrt, квадраты).

---

## 7. Чек-лист верификации (как RIOT §8)

1. `luajit -bl "misson treacherous swine.lua"` — синтаксис.
2. `grep -c local` — все переменные local.
3. Кодировка: BOM + CRLF, 0 bare LF:
   ```python
   b=open(p,'rb').read(); b[:3]==b'\xef\xbb\xbf' and b.count(b'\n')==b.count(b'\r\n')
   ```
4. `grep "while get_fading_status"` — должен быть только внутри
   `fade_and_wait` (с таймаутом).
5. Все `getpedcoordes` — после `isped`.
6. `set_chars_chatting` — конечный таймер, не 9999999.
7. Тест в игре: `log.txt` (нет `bad argument` / runtime) + `trace.txt`
   (дойти до `miss(250)`).

---

## 8. Что ещё не сделано (известные ограничения)

- **Катсцена COL_1** — только текст. Опкодов `LOAD_CUTSCENE` в API нет;
  полноценная катсцена требует ручной режиссуры моделей CSPLAY/CSCOLO/
  CGONA/CSERVRB и предметов LOBTRAY/LOBSTER/COKNIFE/COLTRAY.
- **COL1_6 (GEN1_11)** — третья фраза конфронтации — оставлена текстом
  (фраза короткая, «Boys, kill him!»); при желании добавляется
  `play_voice("COL1_6", ...)`.
- **Коллизия дверей:** объект -76 ставится как «открытая дверь», но
  коллизия оригинального -118 снимается только удалением самого объекта.
  Если игрок застрянет в дверном проёме — проверить координаты 465.375
  30.336 33.181.
- **Тест в игре** — ревизия 2 не проверена вживую. Ждём `log.txt` /
  `trace.txt` после Ctrl-reload.
