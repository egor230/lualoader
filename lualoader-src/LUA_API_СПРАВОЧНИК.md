# lualoader — Справочник всех Lua-функций (как вызывать)

**1506 зарегистрированных функций.** Для каждой: параметры по порядку, что возвращает,
короткое объяснение и пример вызова. Указатели на пед/авто/объект/игрока получаются
функциями поиска: `findplayer()`, `findped...`, `findcar...`, `createped`, `createcar` и т.п.

> ⚠ **Устарел на 13 функций (18.09.2026):** в plugin.cpp уже **1519** уникальных
> регистраций. Не описаны:
> `Getflagmission` (флаг миссии + подъём C++-сторожа),
> `Setflagmission` (установка флага + сторож),
> `set_current_weapon_ped(ped, тип)` (01B9 — положить оружие в руки),
> `set_ped_stay_when_attacked(ped, flag)` (0350 — пед держит позицию при атаке),
> `set_ped_wait_state(ped, state, timer)` (0372 — «ожидание», напр. 29, 99999999),
> `set_ped_personality(ped, personality)` (0243 — личность: 16 горожанин, 30 агрессор),
> `set_ped_only_damaged_by_player(ped, flag)` (02A9 — ранить может только игрок),
> `is_ped_health_greater(ped, threshold)` → `bool` (0184 — здоровье строго больше),
> `make_obj_targettable(obj, flag)` (035D — в объект можно целиться/бросать),
> `get_dead_char_pickup_coords(ped)` → `x, y, z` (04A5 — пикап из трупа),
> `set_ped_mood(mood, timer)` (04E3 — настроение ИГРОКА),
> `request_animation(name)` (04ED — загрузить анимацию),
> `remove_animation(name)` (04EF — выгрузить анимацию).
> Подробности — `ОПИСАНИЕ_проекта_для_ИИ.md` разделы 4.4 и 9.2.


## Конвенции

- **Координаты**: всегда **три отдельных числа подряд — X, Y, Z**.
  Пример: `ped_sprint_to_point(ped, 100.0, 200.0, 30.0)`.
- **Возврат нескольких чисел** (например скорость) — три значения `x, y, z`.
- Типы параметров: **число** (float) / **целое** (int) / **флаг** (true/false) /
  **строка** (`"..."`); **пед/авто/объект/игрок** — указатель из функции поиска.
- «действие» вместо возврата = функция только выполняет действие, ничего не даёт.
- `...` в конце параметров = можно передать ещё аргументы того же типа.
- Номера — порядковые по порядку регистрации (1–1506), без буквенных суффиксов.
- Функции вида `get_*_m_*` / `set_*_m_*` — прямой доступ к полю игрового объекта;
  в описании расшифровано имя поля (например `m_fHeliForcedZ` = принудительная высота
  Z вертолёта) с типом поля.

## Пример (как обычно пишут скрипт)

```lua
local player = findplayer()            -- указатель на игрока
local car   = createcar(159, 100, 200, 30) -- создать авто (id, X, Y, Z)
set_car_mass(car, 3000)                -- действие: масса авто
local x, y, z = get_car_velocity(car)  -- возврат: 3 числа (X, Y, Z)
local ok, v   = incar(ped)             -- возврат: флаг + авто
```


## Транспорт: авто (334)

- **15.** `incar(пед)` → флаг, указ. — пед в авто?.  *Пример: `incar(ped)`*
- **19.** `createcar(число, число, число, число)` → указ. — создать авто на координатах на координатах.  *Пример: `createcar(x, y, z, 3)`*
- **51.** `cardrive(авто, число, число, число)` → действие — авто едет в точку.  *Пример: `cardrive(car, x, y, z)`*
- **59.** `car_in_point_in_radius(авто, число, число, число, число)` → флаг — проверить находится авто в координатах с радиусом.  *Пример: `car_in_point_in_radius(car, x, y, z, 3)`*
- **83.** `car_explode(авто)` → действие — взорвать авто.  *Пример: `car_explode(car)`*
- **84.** `is_car_stopped(авто)` → флаг — авто остановилось.  *Пример: `is_car_stopped(car)`*
- **106.** `is_car_stuck(авто)` → флаг — 03CE: car 12@ stuck если машина застряла.  *Пример: `is_car_stuck(car)`*
- **107.** `is_car_upsidedown(авто)` → флаг — 01F4: car 12@ flipped если машина перевернута.  *Пример: `is_car_upsidedown(car)`*
- **108.** `is_car_upright(авто)` → флаг — 020D: car 12@ flipped если указанный автомобиль перевернут.  *Пример: `is_car_upright(car)`*
- **119.** `car_lastweapondamage(авто)` → целое — номер оружие, которое нанесло урон авто.  *Пример: `car_lastweapondamage(car)`*
- **120.** `car_currentgear(авто)` → целое — текущая передача авто.  *Пример: `car_currentgear(car)`*
- **133.** `set_car_tires(авто, число)` → действие — проколоть шину авто.  *Пример: `set_car_tires(car, x)`*
- **166.** `Createcar(строка, число, число, число)` → указ. — макрос создать авто на координатах.  *Пример: `Createcar("текст", x, y, z)`*
- **233.** `get_car_driver(авто)` → указ. — получить водителя авто.  *Пример: `get_car_driver(car)`*
- **234.** `get_car_passenger(авто, число)` → указ. — получить пассажира авто по месту.  *Пример: `get_car_passenger(car, x)`*
- **235.** `get_car_num_passengers(авто)` → целое — получить кол-во пассажиров авто.  *Пример: `get_car_num_passengers(car)`*
- **236.** `get_car_max_passengers(авто)` → целое — получить макс кол-во пассажиров авто.  *Пример: `get_car_max_passengers(car)`*
- **237.** `get_car_steer_angle(авто)` → число — получить угол руля авто.  *Пример: `get_car_steer_angle(car)`*
- **238.** `set_car_steer_angle(авто, число)` → действие — установить угол руля авто.  *Пример: `set_car_steer_angle(car, x)`*
- **239.** `get_car_gas_pedal(авто)` → число — получить педаль газа авто.  *Пример: `get_car_gas_pedal(car)`*
- **240.** `get_car_brake_pedal(авто)` → число — получить педаль тормоза авто.  *Пример: `get_car_brake_pedal(car)`*
- **241.** `get_car_engine_on(авто)` → флаг — работает ли двигатель авто.  *Пример: `get_car_engine_on(car)`*
- **242.** `set_car_engine_on(авто, флаг)` → действие — вкл/выкл двигатель авто.  *Пример: `set_car_engine_on(car, true)`*
- **243.** `get_car_lights_on(авто)` → флаг — включены ли фары авто.  *Пример: `get_car_lights_on(car)`*
- **244.** `set_car_lights_on(авто, флаг)` → действие — вкл/выкл фары авто.  *Пример: `set_car_lights_on(car, true)`*
- **245.** `get_car_handbrake_on(авто)` → флаг — включен ли ручник авто.  *Пример: `get_car_handbrake_on(car)`*
- **246.** `set_car_handbrake_on(авто, флаг)` → действие — вкл/выкл ручник авто.  *Пример: `set_car_handbrake_on(car, true)`*
- **247.** `get_car_locked(авто)` → флаг — заблокировано ли авто.  *Пример: `get_car_locked(car)`*
- **248.** `set_car_locked(авто, флаг)` → действие — заблокировать/разблокировать авто.  *Пример: `set_car_locked(car, true)`*
- **249.** `get_car_is_van(авто)` → флаг — авто фургон.  *Пример: `get_car_is_van(car)`*
- **250.** `get_car_is_bus(авто)` → флаг — авто автобус.  *Пример: `get_car_is_bus(car)`*
- **251.** `get_car_is_big(авто)` → флаг — авто большая машина.  *Пример: `get_car_is_big(car)`*
- **252.** `set_car_comedy_controls(авто, флаг)` → действие — шуточное управление авто.  *Пример: `set_car_comedy_controls(car, true)`*
- **253.** `set_car_hide_occupants(авто, флаг)` → действие — скрыть седоков авто.  *Пример: `set_car_hide_occupants(car, true)`*
- **254.** `get_car_tires_invulnerable(авто)` → флаг — шины авто неуязвимы.  *Пример: `get_car_tires_invulnerable(car)`*
- **255.** `set_car_tires_invulnerable(авто, флаг)` → действие — шины авто неуязвимы вкл/выкл.  *Пример: `set_car_tires_invulnerable(car, true)`*
- **256.** `get_car_door_lock(авто)` → целое — получить блокировку дверей.  *Пример: `get_car_door_lock(car)`*
- **257.** `set_car_door_lock(авто, число)` → действие — установить блокировку дверей.  *Пример: `set_car_door_lock(car, x)`*
- **258.** `get_car_last_damager(авто)` → указ. — получить последний урон авто.  *Пример: `get_car_last_damager(car)`*
- **259.** `get_car_time_of_death(авто)` → целое — получить время смерти авто.  *Пример: `get_car_time_of_death(car)`*
- **260.** `get_car_weapon_damage(авто)` → целое — тип оружия последнего урона авто.  *Пример: `get_car_weapon_damage(car)`*
- **261.** `get_car_vehicle_class(авто)` → целое — получить класс авто.  *Пример: `get_car_vehicle_class(car)`*
- **262.** `car_play_horn(авто)` → действие — авто сигналит.  *Пример: `car_play_horn(car)`*
- **263.** `car_extinguish_fire(авто)` → действие — потушить пожар на авто.  *Пример: `car_extinguish_fire(car)`*
- **264.** `car_uses_siren(авто)` → флаг — есть ли сирена у авто.  *Пример: `car_uses_siren(car)`*
- **265.** `car_is_law_enforcement(авто)` → флаг — авто правопорядка.  *Пример: `car_is_law_enforcement(car)`*
- **266.** `car_has_roof(авто)` → флаг — у авто есть крыша.  *Пример: `car_has_roof(car)`*
- **267.** `car_is_open_top(авто)` → флаг — авто кабриолет.  *Пример: `car_is_open_top(car)`*
- **268.** `car_is_upside_down(авто)` → флаг — авто перевернуто.  *Пример: `car_is_upside_down(car)`*
- **269.** `car_is_on_its_side(авто)` → флаг — авто на боку.  *Пример: `car_is_on_its_side(car)`*
- **270.** `car_is_vehicle_normal(авто)` → флаг — авто в норме.  *Пример: `car_is_vehicle_normal(car)`*
- **271.** `car_can_be_deleted(авто)` → флаг — можно удалить авто.  *Пример: `car_can_be_deleted(car)`*
- **272.** `car_can_ped_enter(авто)` → флаг — можно сесть в авто.  *Пример: `car_can_ped_enter(car)`*
- **273.** `car_can_doors_be_damaged(авто)` → флаг — двери могут быть повреждены.  *Пример: `car_can_doors_be_damaged(car)`*
- **274.** `car_activate_bomb(авто)` → действие — активировать бомбу.  *Пример: `car_activate_bomb(car)`*
- **275.** `car_activate_bomb_when_entered(авто)` → действие — бомба при посадке.  *Пример: `car_activate_bomb_when_entered(car)`*
- **276.** `car_get_height_above_road(авто)` → число — высота авто над дорогой.  *Пример: `car_get_height_above_road(car)`*
- **277.** `car_is_clear_to_drive_away(авто)` → флаг — можно свободно уехать.  *Пример: `car_is_clear_to_drive_away(car)`*
- **278.** `car_is_door_ready(авто, число)` → флаг — дверь готова открыться.  *Пример: `car_is_door_ready(car, x)`*
- **279.** `car_is_door_fully_open(авто, число)` → флаг — дверь полностью открыта.  *Пример: `car_is_door_fully_open(car, x)`*
- **280.** `car_is_door_closed(авто, число)` → флаг — дверь закрыта.  *Пример: `car_is_door_closed(car, x)`*
- **281.** `car_is_door_missing(авто, число)` → флаг — дверь отсутствует.  *Пример: `car_is_door_missing(car, x)`*
- **282.** `car_shuffle_passengers(авто)` → флаг — пересадить пассажиров.  *Пример: `car_shuffle_passengers(car)`*
- **283.** `car_update_passenger_list(авто)` → действие — обновить список пассажиров.  *Пример: `car_update_passenger_list(car)`*
- **284.** `get_car_radio_station(авто)` → целое — получить радиостанцию авто.  *Пример: `get_car_radio_station(car)`*
- **285.** `set_car_radio_station(авто, число)` → действие — установить радиостанцию авто.  *Пример: `set_car_radio_station(car, x)`*
- **286.** `get_car_created_by(авто)` → целое — кем создано авто.  *Пример: `get_car_created_by(car)`*
- **287.** `get_car_wanted_stars_on_enter(авто)` → целое — звёзды при входе в авто.  *Пример: `get_car_wanted_stars_on_enter(car)`*
- **288.** `set_car_wanted_stars_on_enter(авто, число)` → действие — установить звёзды при входе.  *Пример: `set_car_wanted_stars_on_enter(car, x)`*
- **582.** `car_goto_accurate(авто, число, число, число)` → действие — точная езда на координаты.  *Пример: `car_goto_accurate(car, x, y, z)`*
- **583.** `car_set_idle(авто)` → действие — авто в ожидание.  *Пример: `car_set_idle(car)`*
- **584.** `car_wander_randomly(авто)` → действие — авто едет случайно.  *Пример: `car_wander_randomly(car)`*
- **585.** `car_face_coord(авто, число, число, число)` → действие — повернуть к координатам.  *Пример: `car_face_coord(car, x, y, z)`*
- **588.** `set_car_status(авто, число)` → действие — статус авто.  *Пример: `set_car_status(car, x)`*
- **589.** `set_car_mission(авто, число)` → действие — миссия авто.  *Пример: `set_car_mission(car, x)`*
- **590.** `car_block_car(авто, авто)` → действие — блокировать авто.  *Пример: `car_block_car(car, car)`*
- **591.** `car_ram_car(авто, авто)` → действие — таранить.  *Пример: `car_ram_car(car, car)`*
- **592.** `car_change_lane(авто, флаг)` → действие — смена полосы.  *Пример: `car_change_lane(car, true)`*
- **593.** `car_drive_straight(авто, флаг)` → действие — ехать прямо.  *Пример: `car_drive_straight(car, true)`*
- **594.** `set_car_forward_speed(авто, число)` → действие — скорость вперед.  *Пример: `set_car_forward_speed(car, x)`*
- **595.** `car_wait(авто, число)` → действие — приказать ждать ms.  *Пример: `car_wait(car, x)`*
- **596.** `car_route_seed(авто, число)` → действие — seed маршрута.  *Пример: `car_route_seed(car, x)`*
- **597.** `car_straight_dist(авто, число)` → действие — дистанция прямой линии.  *Пример: `car_straight_dist(car, x)`*
- **598.** `car_convoy(авто, флаг)` → действие — конвойное авто.  *Пример: `car_convoy(car, true)`*
- **599.** `car_make_stronger(авто)` → действие — усилить авто Крейга.  *Пример: `car_make_stronger(car)`*
- **600.** `car_to_player(авто)` → действие — авто к игроку.  *Пример: `car_to_player(car)`*
- **601.** `car_fade_in(авто, число)` → действие — затухание.  *Пример: `car_fade_in(car, x)`*
- **602.** `car_on_trailer(авто)` → действие — на прицеп.  *Пример: `car_on_trailer(car)`*
- **603.** `set_car_visible(авто, флаг)` → действие — видимость.  *Пример: `set_car_visible(car, true)`*
- **605.** `set_car_big_wheels(авто, флаг)` → действие — большие колеса.  *Пример: `set_car_big_wheels(car, true)`*
- **606.** `set_car_bproof(авто, флаг)` → действие — пуленепробиваемость.  *Пример: `set_car_bproof(car, true)`*
- **607.** `set_car_fproof(авто, флаг)` → действие — огнеупорность.  *Пример: `set_car_fproof(car, true)`*
- **608.** `set_car_rproof(авто, флаг)` → действие — ракетоупорность.  *Пример: `set_car_rproof(car, true)`*
- **609.** `set_car_no_collide(авто, флаг)` → действие — без коллизий.  *Пример: `set_car_no_collide(car, true)`*
- **610.** `set_car_burst_tires(авто, флаг)` → действие — пробиваемость шин.  *Пример: `set_car_burst_tires(car, true)`*
- **611.** `set_car_dmg_player_only(авто, флаг)` → действие — урон только от игрока.  *Пример: `set_car_dmg_player_only(car, true)`*
- **612.** `car_load_collision(авто, флаг)` → действие — загрузка коллизий.  *Пример: `car_load_collision(car, true)`*
- **613.** `car_upside_safe(авто, флаг)` → действие — перевернутое не ломается.  *Пример: `car_upside_safe(car, true)`*
- **614.** `car_change_color(авто, число, число)` → действие — сменить цвет.  *Пример: `car_change_color(car, x, y)`*
- **615.** `car_set_comp(авто, число, число)` → действие — компоненты модели.  *Пример: `car_set_comp(car, x, y)`*
- **616.** `set_car_health(авто, число)` → действие — здоровье авто.  *Пример: `set_car_health(car, x)`*
- **617.** `get_car_health(авто)` → число — получить здоровье.  *Пример: `get_car_health(car)`*
- **618.** `get_car_heading(авто)` → число — получить угол.  *Пример: `get_car_heading(car)`*
- **619.** `get_car_forward_x(авто)` → число — вектор X.  *Пример: `get_car_forward_x(car)`*
- **620.** `get_car_forward_y(авто)` → число — вектор Y.  *Пример: `get_car_forward_y(car)`*
- **621.** `is_car_in_air(авто)` → целое — в воздухе?.  *Пример: `is_car_in_air(car)`*
- **623.** `car_health_greater(авто, число)` → целое — здоровье больше?.  *Пример: `car_health_greater(car, x)`*
- **624.** `is_car_crushed(авто)` → целое — прессовано?.  *Пример: `is_car_crushed(car)`*
- **625.** `is_car_remap(авто)` → целое — перекрашено?.  *Пример: `is_car_remap(car)`*
- **626.** `is_car_stuck_roof(авто)` → целое — на крыше?.  *Пример: `is_car_stuck_roof(car)`*
- **627.** `car_is_tire_burst(авто)` → целое — шина пробита?.  *Пример: `car_is_tire_burst(car)`*
- **628.** `is_car_visibly_damaged(авто)` → целое — видимо повреждено?.  *Пример: `is_car_visibly_damaged(car)`*
- **629.** `car_waiting_collision(авто)` → целое — ждет коллизии?.  *Пример: `car_waiting_collision(car)`*
- **630.** `is_car_on_trailer(авто)` → целое — на прицепе?.  *Пример: `is_car_on_trailer(car)`*
- **631.** `car_damaged_by_car(авто, авто)` → целое — повреждено авто?.  *Пример: `car_damaged_by_car(car, car)`*
- **632.** `car_damaged_by_ped(авто, пед)` → целое — повреждено педом?.  *Пример: `car_damaged_by_ped(car, ped)`*
- **633.** `car_just_sunk(авто)` → целое — утонуло?.  *Пример: `car_just_sunk(car)`*
- **634.** `car_drowning(авто)` → целое — тонет?.  *Пример: `car_drowning(car)`*
- **635.** `car_has_weapon(авто, число)` → целое — оружие на борту?.  *Пример: `car_has_weapon(car, x)`*
- **636.** `car_any_bomb(авто)` → целое — есть бомба?.  *Пример: `car_any_bomb(car)`*
- **637.** `car_has_bomb(авто, число)` → целое — тип бомбы?.  *Пример: `car_has_bomb(car, x)`*
- **640.** `is_car_alive(авто)` → целое — живо?.  *Пример: `is_car_alive(car)`*
- **641.** `does_vehicle_exist(авто)` → целое — существует?.  *Пример: `does_vehicle_exist(car)`*
- **642.** `is_car_in_garage(авто)` → целое — в гараже?.  *Пример: `is_car_in_garage(car)`*
- **643.** `was_ever_police(авто)` → целое — был полицейским?.  *Пример: `was_ever_police(car)`*
- **656.** `get_car_driver_ref(авто)` → указ. — водитель.  *Пример: `get_car_driver_ref(car)`*
- **673.** `set_car_ammo(авто, число, число)` → действие — боезапас.  *Пример: `set_car_ammo(car, x, y)`*
- **675.** `set_car_weapon(авто, число)` → действие — текущее оружие.  *Пример: `set_car_weapon(car, x)`*
- **723.** `get_vehicle_pool_size()` → целое — размер пула транспорта.  *Пример: `get_vehicle_pool_size()`*
- **724.** `get_car_pool_index(авто)` → целое — индекс авто в пуле.  *Пример: `get_car_pool_index(car)`*
- **725.** `get_car_by_pool_index(число)` → указ. — авто по индексу в пуле.  *Пример: `get_car_by_pool_index(x)`*
- **726.** `get_car_velocity(авто)` → число, число, число (X, Y, Z) — скорость авто `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `get_car_velocity(car)`*
- **727.** `set_car_velocity(авто, число, число, число)` → действие — уст скорость авто `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `set_car_velocity(car, x, y, z)`*
- **768.** `get_car_speed(авто)` → число — скалярная скорость авто.  *Пример: `get_car_speed(car)`*
- **795.** `get_car_door_status(авто, число)` → целое — состояние двери авто.  *Пример: `get_car_door_status(car, x)`*
- **796.** `set_car_door_status(авто, число, число)` → действие — уст. состояние двери авто.  *Пример: `set_car_door_status(car, x, y)`*
- **797.** `get_car_panel_status(авто, число)` → целое — состояние панели авто.  *Пример: `get_car_panel_status(car, x)`*
- **798.** `get_car_light_status(авто, число)` → целое — состояние фары авто.  *Пример: `get_car_light_status(car, x)`*
- **799.** `set_car_light_status(авто, число, число)` → действие — уст. состояние фары авто.  *Пример: `set_car_light_status(car, x, y)`*
- **800.** `get_car_wheel_status(авто, число)` → целое — состояние колеса авто.  *Пример: `get_car_wheel_status(car, x)`*
- **801.** `set_car_wheel_status(авто, число, число)` → действие — уст. состояние колеса авто.  *Пример: `set_car_wheel_status(car, x, y)`*
- **802.** `get_car_engine_status(авто)` → целое — состояние двигателя авто.  *Пример: `get_car_engine_status(car)`*
- **803.** `set_car_engine_status(авто, число)` → действие — уст. состояние двигателя авто.  *Пример: `set_car_engine_status(car, x)`*
- **937.** `get_car_m_nPrimaryColor(авто)` → целое — `m_nPrimaryColor` (Основной цвет (целое)).  *Пример: `get_car_m_nPrimaryColor(car)`*
- **938.** `set_car_m_nPrimaryColor(авто, число)` → действие — уст. `m_nPrimaryColor` (Основной цвет (целое)).  *Пример: `set_car_m_nPrimaryColor(car, x)`*
- **939.** `get_car_m_nSecondaryColor(авто)` → целое — `m_nSecondaryColor` (Дополнительный цвет (целое)).  *Пример: `get_car_m_nSecondaryColor(car)`*
- **940.** `set_car_m_nSecondaryColor(авто, число)` → действие — уст. `m_nSecondaryColor` (Дополнительный цвет (целое)).  *Пример: `set_car_m_nSecondaryColor(car, x)`*
- **941.** `get_car_m_wWantedStarsOnEnter(авто)` → целое — `m_wWantedStarsOnEnter` (Розыск звёзды розыска On вход (целое)).  *Пример: `get_car_m_wWantedStarsOnEnter(car)`*
- **942.** `set_car_m_wWantedStarsOnEnter(авто, число)` → действие — уст. `m_wWantedStarsOnEnter` (Розыск звёзды розыска On вход (целое)).  *Пример: `set_car_m_wWantedStarsOnEnter(car, x)`*
- **943.** `get_car_m_wMissionValue(авто)` → целое — `m_wMissionValue` (Миссия значение (целое)).  *Пример: `get_car_m_wMissionValue(car)`*
- **944.** `set_car_m_wMissionValue(авто, число)` → действие — уст. `m_wMissionValue` (Миссия значение (целое)).  *Пример: `set_car_m_wMissionValue(car, x)`*
- **945.** `get_car_m_nNumPassengers(авто)` → целое — `m_nNumPassengers` (Кол-во пассажиры (целое)).  *Пример: `get_car_m_nNumPassengers(car)`*
- **946.** `set_car_m_nNumPassengers(авто, число)` → действие — уст. `m_nNumPassengers` (Кол-во пассажиры (целое)).  *Пример: `set_car_m_nNumPassengers(car, x)`*
- **947.** `get_car_m_nNumGettingIn(авто)` → целое — `m_nNumGettingIn` (Кол-во посадка In (целое)).  *Пример: `get_car_m_nNumGettingIn(car)`*
- **948.** `set_car_m_nNumGettingIn(авто, число)` → действие — уст. `m_nNumGettingIn` (Кол-во посадка In (целое)).  *Пример: `set_car_m_nNumGettingIn(car, x)`*
- **949.** `get_car_m_nGettingInFlags(авто)` → целое — `m_nGettingInFlags` (Посадка In флаги (целое)).  *Пример: `get_car_m_nGettingInFlags(car)`*
- **950.** `set_car_m_nGettingInFlags(авто, число)` → действие — уст. `m_nGettingInFlags` (Посадка In флаги (целое)).  *Пример: `set_car_m_nGettingInFlags(car, x)`*
- **951.** `get_car_m_nGettingOutFlags(авто)` → целое — `m_nGettingOutFlags` (Посадка Out флаги (целое)).  *Пример: `get_car_m_nGettingOutFlags(car)`*
- **952.** `set_car_m_nGettingOutFlags(авто, число)` → действие — уст. `m_nGettingOutFlags` (Посадка Out флаги (целое)).  *Пример: `set_car_m_nGettingOutFlags(car, x)`*
- **953.** `get_car_m_nMaxPassengers(авто)` → целое — `m_nMaxPassengers` (Максимум пассажиры (целое)).  *Пример: `get_car_m_nMaxPassengers(car)`*
- **954.** `set_car_m_nMaxPassengers(авто, число)` → действие — уст. `m_nMaxPassengers` (Максимум пассажиры (целое)).  *Пример: `set_car_m_nMaxPassengers(car, x)`*
- **955.** `get_car_m_fSteerAngle(авто)` → число — `m_fSteerAngle` (Руль угол (число)).  *Пример: `get_car_m_fSteerAngle(car)`*
- **956.** `set_car_m_fSteerAngle(авто, число)` → действие — уст. `m_fSteerAngle` (Руль угол (число)).  *Пример: `set_car_m_fSteerAngle(car, x)`*
- **957.** `get_car_m_fGasPedal(авто)` → число — `m_fGasPedal` (Газа педаль (число)).  *Пример: `get_car_m_fGasPedal(car)`*
- **958.** `set_car_m_fGasPedal(авто, число)` → действие — уст. `m_fGasPedal` (Газа педаль (число)).  *Пример: `set_car_m_fGasPedal(car, x)`*
- **959.** `get_car_m_fBreakPedal(авто)` → число — `m_fBreakPedal` (Тормоза педаль (число)).  *Пример: `get_car_m_fBreakPedal(car)`*
- **960.** `set_car_m_fBreakPedal(авто, число)` → действие — уст. `m_fBreakPedal` (Тормоза педаль (число)).  *Пример: `set_car_m_fBreakPedal(car, x)`*
- **961.** `get_car_m_nCreatedBy(авто)` → целое — `m_nCreatedBy` (Created By (целое)).  *Пример: `get_car_m_nCreatedBy(car)`*
- **962.** `set_car_m_nCreatedBy(авто, число)` → действие — уст. `m_nCreatedBy` (Created By (целое)).  *Пример: `set_car_m_nCreatedBy(car, x)`*
- **963.** `get_car_m_nAmmoInClip(авто)` → целое — `m_nAmmoInClip` (Патроны In магазин (целое)).  *Пример: `get_car_m_nAmmoInClip(car)`*
- **964.** `set_car_m_nAmmoInClip(авто, число)` → действие — уст. `m_nAmmoInClip` (Патроны In магазин (целое)).  *Пример: `set_car_m_nAmmoInClip(car, x)`*
- **965.** `get_car_m_fHealth(авто)` → число — `m_fHealth` (Здоровье (число)).  *Пример: `get_car_m_fHealth(car)`*
- **966.** `set_car_m_fHealth(авто, число)` → действие — уст. `m_fHealth` (Здоровье (число)).  *Пример: `set_car_m_fHealth(car, x)`*
- **967.** `get_car_m_nCurrentGear(авто)` → целое — `m_nCurrentGear` (Текущий передача (целое)).  *Пример: `get_car_m_nCurrentGear(car)`*
- **968.** `set_car_m_nCurrentGear(авто, число)` → действие — уст. `m_nCurrentGear` (Текущий передача (целое)).  *Пример: `set_car_m_nCurrentGear(car, x)`*
- **969.** `get_car_m_nTimeTillWeNeedThisCar(авто)` → целое — `m_nTimeTillWeNeedThisCar` (Время Till We Need этот авто (целое)).  *Пример: `get_car_m_nTimeTillWeNeedThisCar(car)`*
- **970.** `set_car_m_nTimeTillWeNeedThisCar(авто, число)` → действие — уст. `m_nTimeTillWeNeedThisCar` (Время Till We Need этот авто (целое)).  *Пример: `set_car_m_nTimeTillWeNeedThisCar(car, x)`*
- **971.** `get_car_m_nTimeOfDeath(авто)` → целое — `m_nTimeOfDeath` (Время Of смерть (целое)).  *Пример: `get_car_m_nTimeOfDeath(car)`*
- **972.** `set_car_m_nTimeOfDeath(авто, число)` → действие — уст. `m_nTimeOfDeath` (Время Of смерть (целое)).  *Пример: `set_car_m_nTimeOfDeath(car, x)`*
- **973.** `get_car_m_wBombTimer(авто)` → целое — `m_wBombTimer` (Bomb таймер (целое)).  *Пример: `get_car_m_wBombTimer(car)`*
- **974.** `set_car_m_wBombTimer(авто, число)` → действие — уст. `m_wBombTimer` (Bomb таймер (целое)).  *Пример: `set_car_m_wBombTimer(car, x)`*
- **975.** `get_car_m_nLastWeaponDamage(авто)` → целое — `m_nLastWeaponDamage` (Последний оружие урон (целое)).  *Пример: `get_car_m_nLastWeaponDamage(car)`*
- **976.** `set_car_m_nLastWeaponDamage(авто, число)` → действие — уст. `m_nLastWeaponDamage` (Последний оружие урон (целое)).  *Пример: `set_car_m_nLastWeaponDamage(car, x)`*
- **977.** `get_car_m_nRadioStation(авто)` → целое — `m_nRadioStation` (Радио Station (целое)).  *Пример: `get_car_m_nRadioStation(car)`*
- **978.** `set_car_m_nRadioStation(авто, число)` → действие — уст. `m_nRadioStation` (Радио Station (целое)).  *Пример: `set_car_m_nRadioStation(car, x)`*
- **979.** `get_car_m_bHornEnabled(авто)` → целое — `m_bHornEnabled` (Клаксон включён (флаг)).  *Пример: `get_car_m_bHornEnabled(car)`*
- **980.** `set_car_m_bHornEnabled(авто, число)` → действие — уст. `m_bHornEnabled` (Клаксон включён (флаг)).  *Пример: `set_car_m_bHornEnabled(car, x)`*
- **981.** `get_car_m_nSirenOrAlarm(авто)` → целое — `m_nSirenOrAlarm` (Сирена Or Alarm (целое)).  *Пример: `get_car_m_nSirenOrAlarm(car)`*
- **982.** `set_car_m_nSirenOrAlarm(авто, число)` → действие — уст. `m_nSirenOrAlarm` (Сирена Or Alarm (целое)).  *Пример: `set_car_m_nSirenOrAlarm(car, x)`*
- **983.** `get_car_m_nSirenExtra(авто)` → целое — `m_nSirenExtra` (Сирена дополнительный (целое)).  *Пример: `get_car_m_nSirenExtra(car)`*
- **984.** `set_car_m_nSirenExtra(авто, число)` → действие — уст. `m_nSirenExtra` (Сирена дополнительный (целое)).  *Пример: `set_car_m_nSirenExtra(car, x)`*
- **985.** `get_car_m_fSteerRatio(авто)` → число — `m_fSteerRatio` (Руль Ratio (число)).  *Пример: `get_car_m_fSteerRatio(car)`*
- **986.** `set_car_m_fSteerRatio(авто, число)` → действие — уст. `m_fSteerRatio` (Руль Ratio (число)).  *Пример: `set_car_m_fSteerRatio(car, x)`*
- **987.** `get_car_m_nVehicleClass(авто)` → целое — `m_nVehicleClass` (Транспорт класс (целое)).  *Пример: `get_car_m_nVehicleClass(car)`*
- **988.** `set_car_m_nVehicleClass(авто, число)` → действие — уст. `m_nVehicleClass` (Транспорт класс (целое)).  *Пример: `set_car_m_nVehicleClass(car, x)`*
- **1203.** `get_car_m_nAudioEntityId(авто)` → целое — `m_nAudioEntityId` (Звук Entity ID (целое)).  *Пример: `get_car_m_nAudioEntityId(car)`*
- **1204.** `set_car_m_nAudioEntityId(авто, число)` → действие — уст. `m_nAudioEntityId` (Звук Entity ID (целое)).  *Пример: `set_car_m_nAudioEntityId(car, x)`*
- **1205.** `get_car_m_vecMoveSpeed(авто)` → число, число, число (X, Y, Z) — `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `get_car_m_vecMoveSpeed(car)`*
- **1206.** `set_car_m_vecMoveSpeed(авто, число, число, число)` → действие — уст. `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `set_car_m_vecMoveSpeed(car, x, y, z)`*
- **1207.** `get_car_m_vecTurnSpeed(авто)` → число, число, число (X, Y, Z) — `m_vecTurnSpeed` (Поворот скорость (вектор (X, Y, Z))).  *Пример: `get_car_m_vecTurnSpeed(car)`*
- **1208.** `set_car_m_vecTurnSpeed(авто, число, число, число)` → действие — уст. `m_vecTurnSpeed` (Поворот скорость (вектор (X, Y, Z))).  *Пример: `set_car_m_vecTurnSpeed(car, x, y, z)`*
- **1209.** `get_car_m_vecFrictionMoveForce(авто)` → число, число, число (X, Y, Z) — `m_vecFrictionMoveForce` (Трение движение сила (вектор (X, Y, Z))).  *Пример: `get_car_m_vecFrictionMoveForce(car)`*
- **1210.** `set_car_m_vecFrictionMoveForce(авто, число, число, число)` → действие — уст. `m_vecFrictionMoveForce` (Трение движение сила (вектор (X, Y, Z))).  *Пример: `set_car_m_vecFrictionMoveForce(car, x, y, z)`*
- **1211.** `get_car_m_vecFrictionTurnForce(авто)` → число, число, число (X, Y, Z) — `m_vecFrictionTurnForce` (Трение поворот сила (вектор (X, Y, Z))).  *Пример: `get_car_m_vecFrictionTurnForce(car)`*
- **1212.** `set_car_m_vecFrictionTurnForce(авто, число, число, число)` → действие — уст. `m_vecFrictionTurnForce` (Трение поворот сила (вектор (X, Y, Z))).  *Пример: `set_car_m_vecFrictionTurnForce(car, x, y, z)`*
- **1213.** `get_car_m_vecForce(авто)` → число, число, число (X, Y, Z) — `m_vecForce` (Сила (вектор (X, Y, Z))).  *Пример: `get_car_m_vecForce(car)`*
- **1214.** `set_car_m_vecForce(авто, число, число, число)` → действие — уст. `m_vecForce` (Сила (вектор (X, Y, Z))).  *Пример: `set_car_m_vecForce(car, x, y, z)`*
- **1215.** `get_car_m_vecTorque(авто)` → число, число, число (X, Y, Z) — `m_vecTorque` (Крутящий момент (вектор (X, Y, Z))).  *Пример: `get_car_m_vecTorque(car)`*
- **1216.** `set_car_m_vecTorque(авто, число, число, число)` → действие — уст. `m_vecTorque` (Крутящий момент (вектор (X, Y, Z))).  *Пример: `set_car_m_vecTorque(car, x, y, z)`*
- **1217.** `get_car_m_fMass(авто)` → число — `m_fMass` (Масса (число)).  *Пример: `get_car_m_fMass(car)`*
- **1218.** `set_car_m_fMass(авто, число)` → действие — уст. `m_fMass` (Масса (число)).  *Пример: `set_car_m_fMass(car, x)`*
- **1219.** `get_car_m_fTurnMass(авто)` → число — `m_fTurnMass` (Поворот масса (число)).  *Пример: `get_car_m_fTurnMass(car)`*
- **1220.** `set_car_m_fTurnMass(авто, число)` → действие — уст. `m_fTurnMass` (Поворот масса (число)).  *Пример: `set_car_m_fTurnMass(car, x)`*
- **1221.** `get_car_m_fVelocityFrequency(авто)` → число — `m_fVelocityFrequency` (Скорость Frequency (число)).  *Пример: `get_car_m_fVelocityFrequency(car)`*
- **1222.** `set_car_m_fVelocityFrequency(авто, число)` → действие — уст. `m_fVelocityFrequency` (Скорость Frequency (число)).  *Пример: `set_car_m_fVelocityFrequency(car, x)`*
- **1223.** `get_car_m_fAirResistance(авто)` → число — `m_fAirResistance` (Air Resistance (число)).  *Пример: `get_car_m_fAirResistance(car)`*
- **1224.** `set_car_m_fAirResistance(авто, число)` → действие — уст. `m_fAirResistance` (Air Resistance (число)).  *Пример: `set_car_m_fAirResistance(car, x)`*
- **1225.** `get_car_m_fElasticity(авто)` → число — `m_fElasticity` (Elasticity (число)).  *Пример: `get_car_m_fElasticity(car)`*
- **1226.** `set_car_m_fElasticity(авто, число)` → действие — уст. `m_fElasticity` (Elasticity (число)).  *Пример: `set_car_m_fElasticity(car, x)`*
- **1227.** `get_car_m_fBuoyancyConstant(авто)` → число — `m_fBuoyancyConstant` (Buoyancy Constant (число)).  *Пример: `get_car_m_fBuoyancyConstant(car)`*
- **1228.** `set_car_m_fBuoyancyConstant(авто, число)` → действие — уст. `m_fBuoyancyConstant` (Buoyancy Constant (число)).  *Пример: `set_car_m_fBuoyancyConstant(car, x)`*
- **1229.** `get_car_m_vecCentreOfMass(авто)` → число, число, число (X, Y, Z) — `m_vecCentreOfMass` (Centre Of масса (вектор (X, Y, Z))).  *Пример: `get_car_m_vecCentreOfMass(car)`*
- **1230.** `set_car_m_vecCentreOfMass(авто, число, число, число)` → действие — уст. `m_vecCentreOfMass` (Centre Of масса (вектор (X, Y, Z))).  *Пример: `set_car_m_vecCentreOfMass(car, x, y, z)`*
- **1231.** `get_car_m_nNumCollisionRecords(авто)` → целое — `m_nNumCollisionRecords` (Кол-во столкновение Records (целое)).  *Пример: `get_car_m_nNumCollisionRecords(car)`*
- **1232.** `set_car_m_nNumCollisionRecords(авто, число)` → действие — уст. `m_nNumCollisionRecords` (Кол-во столкновение Records (целое)).  *Пример: `set_car_m_nNumCollisionRecords(car, x)`*
- **1233.** `get_car_m_fTotSpeed(авто)` → число — `m_fTotSpeed` (Общая скорость (число)).  *Пример: `get_car_m_fTotSpeed(car)`*
- **1234.** `set_car_m_fTotSpeed(авто, число)` → действие — уст. `m_fTotSpeed` (Общая скорость (число)).  *Пример: `set_car_m_fTotSpeed(car, x)`*
- **1235.** `get_car_m_fCollisionPower(авто)` → число — `m_fCollisionPower` (Столкновение Power (число)).  *Пример: `get_car_m_fCollisionPower(car)`*
- **1236.** `set_car_m_fCollisionPower(авто, число)` → действие — уст. `m_fCollisionPower` (Столкновение Power (число)).  *Пример: `set_car_m_fCollisionPower(car, x)`*
- **1237.** `get_car_m_vecCollisionPower(авто)` → число, число, число (X, Y, Z) — `m_vecCollisionPower` (Столкновение Power (вектор (X, Y, Z))).  *Пример: `get_car_m_vecCollisionPower(car)`*
- **1238.** `set_car_m_vecCollisionPower(авто, число, число, число)` → действие — уст. `m_vecCollisionPower` (Столкновение Power (вектор (X, Y, Z))).  *Пример: `set_car_m_vecCollisionPower(car, x, y, z)`*
- **1239.** `get_car_m_wComponentCol(авто)` → целое — `m_wComponentCol` (Компонент столкновение (целое)).  *Пример: `get_car_m_wComponentCol(car)`*
- **1240.** `set_car_m_wComponentCol(авто, число)` → действие — уст. `m_wComponentCol` (Компонент столкновение (целое)).  *Пример: `set_car_m_wComponentCol(car, x)`*
- **1241.** `get_car_m_nMoveFlags(авто)` → целое — `m_nMoveFlags` (Движение флаги (целое)).  *Пример: `get_car_m_nMoveFlags(car)`*
- **1242.** `set_car_m_nMoveFlags(авто, число)` → действие — уст. `m_nMoveFlags` (Движение флаги (целое)).  *Пример: `set_car_m_nMoveFlags(car, x)`*
- **1243.** `get_car_m_nCollFlags(авто)` → целое — `m_nCollFlags` (Столкновение флаги (целое)).  *Пример: `get_car_m_nCollFlags(car)`*
- **1244.** `set_car_m_nCollFlags(авто, число)` → действие — уст. `m_nCollFlags` (Столкновение флаги (целое)).  *Пример: `set_car_m_nCollFlags(car, x)`*
- **1245.** `get_car_m_nLastCollType(авто)` → целое — `m_nLastCollType` (Последний столкновение Type (целое)).  *Пример: `get_car_m_nLastCollType(car)`*
- **1246.** `set_car_m_nLastCollType(авто, число)` → действие — уст. `m_nLastCollType` (Последний столкновение Type (целое)).  *Пример: `set_car_m_nLastCollType(car, x)`*
- **1247.** `get_car_m_nZoneLevel(авто)` → целое — `m_nZoneLevel` (Зона Level (целое)).  *Пример: `get_car_m_nZoneLevel(car)`*
- **1248.** `set_car_m_nZoneLevel(авто, число)` → действие — уст. `m_nZoneLevel` (Зона Level (целое)).  *Пример: `set_car_m_nZoneLevel(car, x)`*
- **1249.** `get_car_fNegSpeed(авто)` → целое — fNegSpeed.  *Пример: `get_car_fNegSpeed(car)`*
- **1250.** `set_car_fNegSpeed(авто, число)` → действие — уст. fNegSpeed.  *Пример: `set_car_fNegSpeed(car, x)`*
- **1251.** `get_car_fAIGripMultiplier(авто)` → целое — fAIGripMultiplier.  *Пример: `get_car_fAIGripMultiplier(car)`*
- **1252.** `set_car_fAIGripMultiplier(авто, число)` → действие — уст. fAIGripMultiplier.  *Пример: `set_car_fAIGripMultiplier(car, x)`*
- **1253.** `get_car_fSpecialWepRotH(авто)` → целое — fSpecialWepRotH.  *Пример: `get_car_fSpecialWepRotH(car)`*
- **1254.** `set_car_fSpecialWepRotH(авто, число)` → действие — уст. fSpecialWepRotH.  *Пример: `set_car_fSpecialWepRotH(car, x)`*
- **1255.** `get_car_fSpecialWepRotV(авто)` → целое — fSpecialWepRotV.  *Пример: `get_car_fSpecialWepRotV(car)`*
- **1256.** `set_car_fSpecialWepRotV(авто, число)` → действие — уст. fSpecialWepRotV.  *Пример: `set_car_fSpecialWepRotV(car, x)`*
- **1257.** `get_car_fSpecialSteering(авто)` → целое — fSpecialSteering.  *Пример: `get_car_fSpecialSteering(car)`*
- **1258.** `set_car_fSpecialSteering(авто, число)` → действие — уст. fSpecialSteering.  *Пример: `set_car_fSpecialSteering(car, x)`*
- **1259.** `get_car_fSpecialMoveState(авто)` → целое — fSpecialMoveState.  *Пример: `get_car_fSpecialMoveState(car)`*
- **1260.** `set_car_fSpecialMoveState(авто, число)` → действие — уст. fSpecialMoveState.  *Пример: `set_car_fSpecialMoveState(car, x)`*
- **1261.** `get_car_fSkidMarkDensity(авто)` → целое — fSkidMarkDensity.  *Пример: `get_car_fSkidMarkDensity(car)`*
- **1262.** `set_car_fSkidMarkDensity(авто, число)` → действие — уст. fSkidMarkDensity.  *Пример: `set_car_fSkidMarkDensity(car, x)`*
- **1263.** `get_car_m_fPropRotation(авто)` → число — `m_fPropRotation` (Винт Rotation (число)).  *Пример: `get_car_m_fPropRotation(car)`*
- **1264.** `set_car_m_fPropRotation(авто, число)` → действие — уст. `m_fPropRotation` (Винт Rotation (число)).  *Пример: `set_car_m_fPropRotation(car, x)`*
- **1265.** `get_car_m_fPropSpeed(авто)` → число — `m_fPropSpeed` (Винт скорость (число)).  *Пример: `get_car_m_fPropSpeed(car)`*
- **1266.** `set_car_m_fPropSpeed(авто, число)` → действие — уст. `m_fPropSpeed` (Винт скорость (число)).  *Пример: `set_car_m_fPropSpeed(car, x)`*
- **1267.** `get_car_m_fForcedZRotation(авто)` → число — `m_fForcedZRotation` (Принудительный Z Rotation (число)).  *Пример: `get_car_m_fForcedZRotation(car)`*
- **1268.** `set_car_m_fForcedZRotation(авто, число)` → действие — уст. `m_fForcedZRotation` (Принудительный Z Rotation (число)).  *Пример: `set_car_m_fForcedZRotation(car, x)`*
- **1269.** `get_car_m_nAttackPlayerTime(авто)` → целое — `m_nAttackPlayerTime` (Attack игрок время (целое)).  *Пример: `get_car_m_nAttackPlayerTime(car)`*
- **1270.** `set_car_m_nAttackPlayerTime(авто, число)` → действие — уст. `m_nAttackPlayerTime` (Attack игрок время (целое)).  *Пример: `set_car_m_nAttackPlayerTime(car, x)`*
- **1271.** `get_car_m_fBurningTimer(авто)` → число — `m_fBurningTimer` (Burning таймер (число)).  *Пример: `get_car_m_fBurningTimer(car)`*
- **1272.** `set_car_m_fBurningTimer(авто, число)` → действие — уст. `m_fBurningTimer` (Burning таймер (число)).  *Пример: `set_car_m_fBurningTimer(car, x)`*
- **1273.** `get_car_m_fBoatGasPedal(авто)` → число — `m_fBoatGasPedal` (Лодка газа педаль (число)).  *Пример: `get_car_m_fBoatGasPedal(car)`*
- **1274.** `set_car_m_fBoatGasPedal(авто, число)` → действие — уст. `m_fBoatGasPedal` (Лодка газа педаль (число)).  *Пример: `set_car_m_fBoatGasPedal(car, x)`*
- **1275.** `get_car_m_fBoatBrakePedal(авто)` → число — `m_fBoatBrakePedal` (Лодка тормоза педаль (число)).  *Пример: `get_car_m_fBoatBrakePedal(car)`*
- **1276.** `set_car_m_fBoatBrakePedal(авто, число)` → действие — уст. `m_fBoatBrakePedal` (Лодка тормоза педаль (число)).  *Пример: `set_car_m_fBoatBrakePedal(car, x)`*
- **1277.** `get_car_m_fBoatSteeringLeftRight(авто)` → число — `m_fBoatSteeringLeftRight` (Лодка рулевого Left Right (число)).  *Пример: `get_car_m_fBoatSteeringLeftRight(car)`*
- **1278.** `set_car_m_fBoatSteeringLeftRight(авто, число)` → действие — уст. `m_fBoatSteeringLeftRight` (Лодка рулевого Left Right (число)).  *Пример: `set_car_m_fBoatSteeringLeftRight(car, x)`*
- **1279.** `get_car_m_nPadNumber(авто)` → целое — `m_nPadNumber` (Pad число (целое)).  *Пример: `get_car_m_nPadNumber(car)`*
- **1280.** `set_car_m_nPadNumber(авто, число)` → действие — уст. `m_nPadNumber` (Pad число (целое)).  *Пример: `set_car_m_nPadNumber(car, x)`*
- **1281.** `get_car_m_nNumWaterTrailPoints(авто)` → целое — `m_nNumWaterTrailPoints` (Кол-во Water Trail Points (целое)).  *Пример: `get_car_m_nNumWaterTrailPoints(car)`*
- **1282.** `set_car_m_nNumWaterTrailPoints(авто, число)` → действие — уст. `m_nNumWaterTrailPoints` (Кол-во Water Trail Points (целое)).  *Пример: `set_car_m_nNumWaterTrailPoints(car, x)`*
- **1283.** `get_car_nBikeAnimGroup(авто)` → целое — nBikeAnimGroup.  *Пример: `get_car_nBikeAnimGroup(car)`*
- **1284.** `set_car_nBikeAnimGroup(авто, число)` → действие — уст. nBikeAnimGroup.  *Пример: `set_car_nBikeAnimGroup(car, x)`*
- **1285.** `get_car_m_nDamageFlags(авто)` → целое — `m_nDamageFlags` (Урон флаги (целое)).  *Пример: `get_car_m_nDamageFlags(car)`*
- **1286.** `set_car_m_nDamageFlags(авто, число)` → действие — уст. `m_nDamageFlags` (Урон флаги (целое)).  *Пример: `set_car_m_nDamageFlags(car, x)`*
- **1287.** `get_car_m_fSearchLightX(авто)` → число — `m_fSearchLightX` (Прожектор свет X (число)).  *Пример: `get_car_m_fSearchLightX(car)`*
- **1288.** `set_car_m_fSearchLightX(авто, число)` → действие — уст. `m_fSearchLightX` (Прожектор свет X (число)).  *Пример: `set_car_m_fSearchLightX(car, x)`*
- **1289.** `get_car_m_fSearchLightY(авто)` → число — `m_fSearchLightY` (Прожектор свет Y (число)).  *Пример: `get_car_m_fSearchLightY(car)`*
- **1290.** `set_car_m_fSearchLightY(авто, число)` → действие — уст. `m_fSearchLightY` (Прожектор свет Y (число)).  *Пример: `set_car_m_fSearchLightY(car, x)`*
- **1291.** `get_car_m_nExplosionTime(авто)` → целое — `m_nExplosionTime` (Explosion время (целое)).  *Пример: `get_car_m_nExplosionTime(car)`*
- **1292.** `set_car_m_nExplosionTime(авто, число)` → действие — уст. `m_nExplosionTime` (Explosion время (целое)).  *Пример: `set_car_m_nExplosionTime(car, x)`*
- **1293.** `get_car_m_fRotationX(авто)` → число — `m_fRotationX` (Rotation X (число)).  *Пример: `get_car_m_fRotationX(car)`*
- **1294.** `set_car_m_fRotationX(авто, число)` → действие — уст. `m_fRotationX` (Rotation X (число)).  *Пример: `set_car_m_fRotationX(car, x)`*
- **1295.** `get_car_m_fHeliForcedZ(авто)` → число — `m_fHeliForcedZ` (Вертолёт принудительный Z (число)).  *Пример: `get_car_m_fHeliForcedZ(car)`*
- **1296.** `set_car_m_fHeliForcedZ(авто, число)` → действие — уст. `m_fHeliForcedZ` (Вертолёт принудительный Z (число)).  *Пример: `set_car_m_fHeliForcedZ(car, x)`*
- **1297.** `get_car_m_fSearchLightIntensity(авто)` → число — `m_fSearchLightIntensity` (Прожектор свет яркость (число)).  *Пример: `get_car_m_fSearchLightIntensity(car)`*
- **1298.** `set_car_m_fSearchLightIntensity(авто, число)` → действие — уст. `m_fSearchLightIntensity` (Прожектор свет яркость (число)).  *Пример: `set_car_m_fSearchLightIntensity(car, x)`*
- **1299.** `get_car_m_nDamagePoints(авто)` → целое — `m_nDamagePoints` (Урон Points (целое)).  *Пример: `get_car_m_nDamagePoints(car)`*
- **1300.** `set_car_m_nDamagePoints(авто, число)` → действие — уст. `m_nDamagePoints` (Урон Points (целое)).  *Пример: `set_car_m_nDamagePoints(car, x)`*
- **1301.** `get_car_m_fRotorAngle(авто)` → число — `m_fRotorAngle` (Ротор угол (число)).  *Пример: `get_car_m_fRotorAngle(car)`*
- **1302.** `set_car_m_fRotorAngle(авто, число)` → действие — уст. `m_fRotorAngle` (Ротор угол (число)).  *Пример: `set_car_m_fRotorAngle(car, x)`*
- **1363.** `save_car_color(авто, число)` → действие — авто сохраняет цвет при перекраске.  *Пример: `save_car_color(car, x)`*
- **1364.** `is_car_passenger_free(авто, число)` → флаг — Проверяет, свободно ли место пассажира в авто.  *Пример: `is_car_passenger_free(car, x)`*
- **1368.** `set_car_tank(авто, число)` → действие — установить способность танка детонировать авто при контакте.  *Пример: `set_car_tank(car, x)`*
- **1374.** `set_car_immunities(авто, число, число, число, число, число)` → действие — уст иммунитеты авто.  *Пример: `set_car_immunities(car, x, y, z, 3, 4)`*
- **1404.** `set_car_range(число)` → действие — установить множитель диапазона на угрозу для автомобиля.  *Пример: `set_car_range(x)`*
- **1409.** `set_brakes_car(флаг)` → действие — уст тормоза авто игрока.  *Пример: `set_brakes_car(true)`*
- **1425.** `is_car_damaged_weapon(авто, число)` → флаг — авто получает от определенного вида оружие.  *Пример: `is_car_damaged_weapon(car, x)`*
- **1437.** `is_car_damage_by_car(авто, авто)` → флаг — Если авто получило урон от другого авто.  *Пример: `is_car_damage_by_car(car, car)`*
- **1441.** `car_road_off(число, число, число, число, число, число)` → действие — выкл путь авто.  *Пример: `car_road_off(x, y, z, 3, 4, 5)`*
- **1442.** `car_road_on(число, число, число, число, число, число)` → действие — вкл путь авто.  *Пример: `car_road_on(x, y, z, 3, 4, 5)`*
- **1455.** `get_car_in_cord(число, число, число, число)` → указ. — найти авто в радиусе с координатами.  *Пример: `get_car_in_cord(x, y, z, 3)`*
- **1462.** `car_turn_speed(авто)` → действие — повернуть авто с применением скорости.  *Пример: `car_turn_speed(car)`*
- **1464.** `car_move_speed(авто)` → действие — двигать авто с применением скорости.  *Пример: `car_move_speed(car)`*
- **1465.** `car_turnforce_on_x(авто, число)` → действие — Повернуть авто по оси x.  *Пример: `car_turnforce_on_x(car, x)`*
- **1466.** `car_turnforce_on_y(авто, число)` → действие — Повернуть авто по оси y.  *Пример: `car_turnforce_on_y(car, x)`*
- **1467.** `car_turnforce_on_z(авто, число)` → действие — Повернуть авто по оси z.  *Пример: `car_turnforce_on_z(car, x)`*
- **1468.** `car_moveforce_on_x(авто, число)` → действие — Двигать авто по оси x.  *Пример: `car_moveforce_on_x(car, x)`*
- **1469.** `car_moveforce_on_y(авто, число)` → действие — Двигать авто по оси y.  *Пример: `car_moveforce_on_y(car, x)`*
- **1470.** `car_moveforce_on_z(авто, число)` → действие — Двигать авто по оси z.  *Пример: `car_moveforce_on_z(car, x)`*
- **1478.** `car_rotate_on_x(авто, число)` → действие — Поворот угла авто по оси x.  *Пример: `car_rotate_on_x(car, x)`*
- **1479.** `car_rotate_on_y(авто, число)` → действие — Поворот угла авто по оси y.  *Пример: `car_rotate_on_y(car, x)`*
- **1480.** `car_rotate_on_z(авто, число)` → действие — Поворот угла авто по оси z.  *Пример: `car_rotate_on_z(car, x)`*
- **1481.** `car_setrotate_on_x(авто, число)` → действие — уст авто по оси x.  *Пример: `car_setrotate_on_x(car, x)`*
- **1482.** `car_setrotate_on_y(авто, число)` → действие — уст авто по оси y.  *Пример: `car_setrotate_on_y(car, x)`*
- **1483.** `car_setrotate_on_z(авто, число)` → действие — уст авто по оси z.  *Пример: `car_setrotate_on_z(car, x)`*
- **1485.** `car_rotate_turn_on_x(авто, число, число)` → действие — уст угол поворота авто по оси x.  *Пример: `car_rotate_turn_on_x(car, x, y)`*
- **1486.** `car_rotate_turn_on_y(авто, число, число)` → действие — уст угол поворота авто по оси y.  *Пример: `car_rotate_turn_on_y(car, x, y)`*
- **1487.** `car_rotate_turn_on_z(авто, число, число)` → действие — уст угол поворота авто по оси z.  *Пример: `car_rotate_turn_on_z(car, x, y)`*
- **1493.** `car_turn_on_x_with_speed(авто, число, число)` → действие — повeрнуть авто по оси x на угол со скорость.  *Пример: `car_turn_on_x_with_speed(car, x, y)`*
- **1494.** `car_turn_on_y_with_speed(авто, число, число)` → действие — повeрнуть авто по оси y на угол со скорость.  *Пример: `car_turn_on_y_with_speed(car, x, y)`*
- **1495.** `car_turn_on_z_with_speed(авто, число, число)` → действие — повeрнуть авто по оси z на угол со скорость.  *Пример: `car_turn_on_z_with_speed(car, x, y)`*
- **1496.** `car_turn_on_x_with_delay(авто, число, число)` → действие — повeрнуть авто по оси x на угол со задержкой.  *Пример: `car_turn_on_x_with_delay(car, x, y)`*
- **1497.** `car_turn_on_y_with_delay(авто, число, число)` → действие — повeрнуть авто по оси y на угол со задержкой.  *Пример: `car_turn_on_y_with_delay(car, x, y)`*
- **1498.** `car_turn_on_z_with_delay(авто, число, число)` → действие — повeрнуть авто по оси z на угол со задержкой.  *Пример: `car_turn_on_z_with_delay(car, x, y)`*
- **1499.** `set_car_mass(авто, число)` → действие — увеличить массу авто.  *Пример: `set_car_mass(car, 3000)`*
- **1500.** `set_car_acceleration(авто, число)` → действие — установить ускорение авто.  *Пример: `set_car_acceleration(car, 0.2)`*
- **1501.** `set_car_max_speed(авто, число)` → действие — установить макс. скорость авто.  *Пример: `set_car_max_speed(car, 100)`*
- **1502.** `set_car_traction(авто, число)` → действие — установить сцепление авто.  *Пример: `set_car_traction(car, 0.9)`*
- **1506.** `exitcar(пед)` → действие — выйти из авто.  *Пример: `exitcar(ped)`*


## Педы и игрок (303)

- **7.** `worldcoord(указ., число, число)` → число, число (X, Y) — Перевод в мировые координаты.  *Пример: `worldcoord(entity, x, y)`*
- **24.** `ped_walk_to_point(пед, число, число, число)` → действие — пед идет к точке.  *Пример: `ped_walk_to_point(ped, x, y, z)`*
- **27.** `ped_aim_at_ped(пед, пед)` → действие — пед целиться в педе.  *Пример: `ped_aim_at_ped(ped, ped)`*
- **35.** `ped_in_point_in_radius(пед, число, число, число, число, число, число)` → флаг — проверить находится пед в координатах с радиусом.  *Пример: `ped_in_point_in_radius(ped, x, y, z, 3, 4, 5)`*
- **114.** `stop_timer(строка)` → действие — остановить таймер.  *Пример: `stop_timer("текст")`*
- **116.** `ped_attack_car(пед, авто)` → действие — пед атакует авто.  *Пример: `ped_attack_car(ped, car)`*
- **117.** `ped_frozen(число)` → действие — заморозить игpока.  *Пример: `ped_frozen(x)`*
- **123.** `ped_car_as_driver(пед, авто)` → действие — пед садится в авто как водитель.  *Пример: `ped_car_as_driver(ped, car)`*
- **124.** `ped_car_as_passenger(пед, авто)` → действие — пед садится в авто как пассажир.  *Пример: `ped_car_as_passenger(ped, car)`*
- **125.** `ped_atack(пед)` → действие — пед бьет.  *Пример: `ped_atack(ped)`*
- **141.** `ped_anim()` → действие — анимация.  *Пример: `ped_anim()`*
- **156.** `get_ammo_weapon_ped(пед)` → целое — получить кол-во патроны текущего оружие педа.  *Пример: `get_ammo_weapon_ped(ped)`*
- **161.** `ped_shutdown(пед)` → действие — отключение текущего оружие педа.  *Пример: `ped_shutdown(ped)`*
- **165.** `Createped(строка, число, число, число)` → указ. — макрос создать педа.  *Пример: `Createped("текст", x, y, z)`*
- **171.** `Get_model_and_type_current_weapon_ped(пед)` → целое — макрос получить модель и тип текущего оружие педа.  *Пример: `Get_model_and_type_current_weapon_ped(ped)`*
- **173.** `ped_car(пед)` → указ. — авто педа.  *Пример: `ped_car(ped)`*
- **175.** `get_model_current_weapon_ped(пед)` → целое — макрос получить модель текущего оружие педа.  *Пример: `get_model_current_weapon_ped(ped)`*
- **176.** `get_type_current_weapon_ped(пед)` → целое — макрос получить тип текущего оружие педа.  *Пример: `get_type_current_weapon_ped(ped)`*
- **184.** `get_ped_heading(пед)` → число — получить текущий угол поворота педа.  *Пример: `get_ped_heading(ped)`*
- **185.** `set_ped_heading_goal(пед, число)` → действие — установить желаемый угол поворота педа.  *Пример: `set_ped_heading_goal(ped, x)`*
- **186.** `get_ped_heading_goal(пед)` → число — получить желаемый угол поворота педа.  *Пример: `get_ped_heading_goal(ped)`*
- **187.** `set_ped_heading_rate(пед, число)` → действие — установить скорость поворота педа.  *Пример: `set_ped_heading_rate(ped, x)`*
- **188.** `get_ped_heading_rate(пед)` → число — получить скорость поворота педа.  *Пример: `get_ped_heading_rate(ped)`*
- **189.** `get_ped_enter_type(пед)` → целое — получить тип посадки педа в авто.  *Пример: `get_ped_enter_type(ped)`*
- **190.** `get_ped_from_vehicle(пед)` → указ. — получить авто из которого вышел пед.  *Пример: `get_ped_from_vehicle(ped)`*
- **191.** `ped_can_be_deleted(пед)` → флаг — можно ли удалить педа из мира.  *Пример: `ped_can_be_deleted(ped)`*
- **193.** `ped_is_player(пед)` → флаг — пед это игрок.  *Пример: `ped_is_player(ped)`*
- **194.** `ped_is_in_control(пед)` → флаг — пед управляем.  *Пример: `ped_is_in_control(ped)`*
- **195.** `ped_is_shootable(пед)` → флаг — в педа можно стрелять.  *Пример: `ped_is_shootable(ped)`*
- **196.** `ped_is_pointer_valid(пед)` → флаг — указатель на педа валиден.  *Пример: `ped_is_pointer_valid(ped)`*
- **197.** `ped_set_dead(пед)` → действие — пометить педа мертвым.  *Пример: `ped_set_dead(ped)`*
- **198.** `ped_dress(пед)` → действие — пед надевает одежду.  *Пример: `ped_dress(ped)`*
- **199.** `ped_calculate_new_orientation(пед)` → действие — пересчитать ориентацию педа.  *Пример: `ped_calculate_new_orientation(ped)`*
- **200.** `ped_set_move_anim(пед)` → действие — задать педу анимацию движения.  *Пример: `ped_set_move_anim(ped)`*
- **201.** `ped_set_get_up(пед)` → действие — пед встает с земли.  *Пример: `ped_set_get_up(ped)`*
- **202.** `ped_set_jump(пед)` → действие — пед прыгает.  *Пример: `ped_set_jump(ped)`*
- **203.** `ped_set_landing(пед)` → действие — пед приземляется.  *Пример: `ped_set_landing(ped)`*
- **204.** `ped_set_idle(пед)` → действие — пед переходит в состояние покоя.  *Пример: `ped_set_idle(ped)`*
- **205.** `ped_fall(пед)` → действие — пед падает.  *Пример: `ped_fall(ped)`*
- **206.** `ped_turn_body(пед)` → действие — пед поворачивает корпус к цели.  *Пример: `ped_turn_body(ped)`*
- **207.** `ped_update_position(пед)` → действие — пед обновить позицию по анимации.  *Пример: `ped_update_position(ped)`*
- **208.** `ped_restore_heading_rate(пед)` → действие — вернуть скорость поворота педа.  *Пример: `ped_restore_heading_rate(ped)`*
- **209.** `ped_restore_head_position(пед)` → действие — вернуть голове положение.  *Пример: `ped_restore_head_position(ped)`*
- **212.** `ped_stop_non_partial_anims(пед)` → действие — остановить базовые анимации педа.  *Пример: `ped_stop_non_partial_anims(ped)`*
- **213.** `ped_restart_non_partial_anims(пед)` → действие — возобновить базовые анимации педа.  *Пример: `ped_restart_non_partial_anims(ped)`*
- **214.** `ped_clear_aim_flag(пед)` → действие — сбросить флаг прицеливания.  *Пример: `ped_clear_aim_flag(ped)`*
- **215.** `ped_clear_look_flag(пед)` → действие — сбросить флаг взгляда.  *Пример: `ped_clear_look_flag(ped)`*
- **216.** `ped_clear_chat(пед)` → действие — пед прекращает болтать.  *Пример: `ped_clear_chat(ped)`*
- **217.** `ped_clear_attack(пед)` → действие — сбросить у педы атаку.  *Пример: `ped_clear_attack(ped)`*
- **218.** `ped_clear_attack_anim(пед)` → действие — сбросить атаку с удалением анимации.  *Пример: `ped_clear_attack_anim(ped)`*
- **219.** `ped_clear_seek(пед)` → действие — сбросить у педа поиск цели.  *Пример: `ped_clear_seek(ped)`*
- **220.** `ped_clear_follow_path(пед)` → действие — сбросить следование по маршруту.  *Пример: `ped_clear_follow_path(ped)`*
- **223.** `ped_answer_mobile(пед)` → действие — пед отвечает на телефон.  *Пример: `ped_answer_mobile(ped)`*
- **224.** `ped_clear_answer_mobile(пед)` → действие — сбросить разговор по телефону.  *Пример: `ped_clear_answer_mobile(ped)`*
- **225.** `ped_face_phone(пед)` → действие — пед поворачивается к телефону.  *Пример: `ped_face_phone(ped)`*
- **226.** `ped_make_phonecall(пед)` → действие — пед звонит по телефону.  *Пример: `ped_make_phonecall(ped)`*
- **227.** `ped_pause(пед)` → действие — пед замирает.  *Пример: `ped_pause(ped)`*
- **228.** `ped_mug(пед)` → действие — пед грабит ближайшего педа.  *Пример: `ped_mug(ped)`*
- **229.** `ped_solicit(пед)` → действие — пед предлагает услуги.  *Пример: `ped_solicit(ped)`*
- **230.** `ped_buy_ice_cream(пед)` → действие — пед покупает мороженое.  *Пример: `ped_buy_ice_cream(ped)`*
- **231.** `ped_look_for_sexy_cars(пед)` → действие — пед ищет красивые авто.  *Пример: `ped_look_for_sexy_cars(ped)`*
- **232.** `ped_look_for_sexy_peds(пед)` → действие — пед ищет красивых педов.  *Пример: `ped_look_for_sexy_peds(ped)`*
- **407.** `ped_play_anim(пед, число, число)` → действие — запустить анимацию педа.  *Пример: `ped_play_anim(ped, x, y)`*
- **408.** `ped_blend_anim(пед, число, число, число)` → действие — запустить анимацию со смешиванием.  *Пример: `ped_blend_anim(ped, x, y, z)`*
- **463.** `get_ped_density_multiplier()` → число — плотность пешеходов.  *Пример: `get_ped_density_multiplier()`*
- **464.** `set_ped_density_multiplier(число)` → действие — установить плотность пешеходов.  *Пример: `set_ped_density_multiplier(x)`*
- **657.** `get_char_in_seat(авто, число)` → указ. — пассажир.  *Пример: `get_char_in_seat(car, x)`*
- **658.** `order_driver_out(пед)` → действие — выгнать водителя.  *Пример: `order_driver_out(ped)`*
- **660.** `set_ped_drive_aggression(пед, число)` → действие — агрессия.  *Пример: `set_ped_drive_aggression(ped, x)`*
- **670.** `warp_ped_from_car(пед, число, число, число)` → действие — телепорт из авто.  *Пример: `warp_ped_from_car(ped, x, y, z)`*
- **671.** `warp_ped_passenger(пед, авто, число)` → действие — в пассажиры.  *Пример: `warp_ped_passenger(ped, car, x)`*
- **672.** `warp_player_from_car(число, число, число)` → действие — телепорт игрока.  *Пример: `warp_player_from_car(x, y, z)`*
- **989.** `get_ped_m_fCollisionSpeed(пед)` → число — `m_fCollisionSpeed` (Столкновение скорость (число)).  *Пример: `get_ped_m_fCollisionSpeed(ped)`*
- **990.** `set_ped_m_fCollisionSpeed(пед, число)` → действие — уст. `m_fCollisionSpeed` (Столкновение скорость (число)).  *Пример: `set_ped_m_fCollisionSpeed(ped, x)`*
- **991.** `get_ped_m_nGangFlags(пед)` → целое — `m_nGangFlags` (Банда флаги (целое)).  *Пример: `get_ped_m_nGangFlags(ped)`*
- **992.** `set_ped_m_nGangFlags(пед, число)` → действие — уст. `m_nGangFlags` (Банда флаги (целое)).  *Пример: `set_ped_m_nGangFlags(ped, x)`*
- **993.** `get_ped_m_nPedStatus(пед)` → целое — `m_nPedStatus` (Пед статус (целое)).  *Пример: `get_ped_m_nPedStatus(ped)`*
- **994.** `set_ped_m_nPedStatus(пед, число)` → действие — уст. `m_nPedStatus` (Пед статус (целое)).  *Пример: `set_ped_m_nPedStatus(ped, x)`*
- **995.** `get_ped_m_vecObjective(пед)` → число, число, число (X, Y, Z) — `m_vecObjective` (Цель (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecObjective(ped)`*
- **996.** `set_ped_m_vecObjective(пед, число, число, число)` → действие — уст. `m_vecObjective` (Цель (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecObjective(ped, x, y, z)`*
- **997.** `get_ped_m_fObjectiveAngle(пед)` → число — `m_fObjectiveAngle` (Цель угол (число)).  *Пример: `get_ped_m_fObjectiveAngle(ped)`*
- **998.** `set_ped_m_fObjectiveAngle(пед, число)` → действие — уст. `m_fObjectiveAngle` (Цель угол (число)).  *Пример: `set_ped_m_fObjectiveAngle(ped, x)`*
- **999.** `get_ped_m_nPedFormation(пед)` → целое — `m_nPedFormation` (Пед Formation (целое)).  *Пример: `get_ped_m_nPedFormation(ped)`*
- **1000.** `set_ped_m_nPedFormation(пед, число)` → действие — уст. `m_nPedFormation` (Пед Formation (целое)).  *Пример: `set_ped_m_nPedFormation(ped, x)`*
- **1001.** `get_ped_m_nFearFlags(пед)` → целое — `m_nFearFlags` (Fear флаги (целое)).  *Пример: `get_ped_m_nFearFlags(ped)`*
- **1002.** `set_ped_m_nFearFlags(пед, число)` → действие — уст. `m_nFearFlags` (Fear флаги (целое)).  *Пример: `set_ped_m_nFearFlags(ped, x)`*
- **1003.** `get_ped_m_nEventType(пед)` → целое — `m_nEventType` (Event Type (целое)).  *Пример: `get_ped_m_nEventType(ped)`*
- **1004.** `set_ped_m_nEventType(пед, число)` → действие — уст. `m_nEventType` (Event Type (целое)).  *Пример: `set_ped_m_nEventType(ped, x)`*
- **1005.** `get_ped_m_fAngleToEvent(пед)` → число — `m_fAngleToEvent` (Угол To Event (число)).  *Пример: `get_ped_m_fAngleToEvent(ped)`*
- **1006.** `set_ped_m_fAngleToEvent(пед, число)` → действие — уст. `m_fAngleToEvent` (Угол To Event (число)).  *Пример: `set_ped_m_fAngleToEvent(ped, x)`*
- **1007.** `get_ped_m_nAnimGroupId(пед)` → целое — `m_nAnimGroupId` (Анимация Group ID (целое)).  *Пример: `get_ped_m_nAnimGroupId(ped)`*
- **1008.** `set_ped_m_nAnimGroupId(пед, число)` → действие — уст. `m_nAnimGroupId` (Анимация Group ID (целое)).  *Пример: `set_ped_m_nAnimGroupId(ped, x)`*
- **1009.** `get_ped_m_vecOffsetSeek(пед)` → число, число, число (X, Y, Z) — `m_vecOffsetSeek` (Offset поиск (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecOffsetSeek(ped)`*
- **1010.** `set_ped_m_vecOffsetSeek(пед, число, число, число)` → действие — уст. `m_vecOffsetSeek` (Offset поиск (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecOffsetSeek(ped, x, y, z)`*
- **1011.** `get_ped_m_nActionTimer(пед)` → целое — `m_nActionTimer` (Действие таймер (целое)).  *Пример: `get_ped_m_nActionTimer(ped)`*
- **1012.** `set_ped_m_nActionTimer(пед, число)` → действие — уст. `m_nActionTimer` (Действие таймер (целое)).  *Пример: `set_ped_m_nActionTimer(ped, x)`*
- **1013.** `get_ped_m_nWaitState(пед)` → целое — `m_nWaitState` (Wait состояние (целое)).  *Пример: `get_ped_m_nWaitState(ped)`*
- **1014.** `set_ped_m_nWaitState(пед, число)` → действие — уст. `m_nWaitState` (Wait состояние (целое)).  *Пример: `set_ped_m_nWaitState(ped, x)`*
- **1015.** `get_ped_m_nWaitTimer(пед)` → целое — `m_nWaitTimer` (Wait таймер (целое)).  *Пример: `get_ped_m_nWaitTimer(ped)`*
- **1016.** `set_ped_m_nWaitTimer(пед, число)` → действие — уст. `m_nWaitTimer` (Wait таймер (целое)).  *Пример: `set_ped_m_nWaitTimer(ped, x)`*
- **1017.** `get_ped_m_wPathNodes(пед)` → целое — `m_wPathNodes` (Путь узлы (целое)).  *Пример: `get_ped_m_wPathNodes(ped)`*
- **1018.** `set_ped_m_wPathNodes(пед, число)` → действие — уст. `m_wPathNodes` (Путь узлы (целое)).  *Пример: `set_ped_m_wPathNodes(ped, x)`*
- **1019.** `get_ped_m_wCurPathNode(пед)` → целое — `m_wCurPathNode` (Текущий путь узел (целое)).  *Пример: `get_ped_m_wCurPathNode(ped)`*
- **1020.** `set_ped_m_wCurPathNode(пед, число)` → действие — уст. `m_wCurPathNode` (Текущий путь узел (целое)).  *Пример: `set_ped_m_wCurPathNode(ped, x)`*
- **1021.** `get_ped_m_nPathNodeTimer(пед)` → целое — `m_nPathNodeTimer` (Путь узел таймер (целое)).  *Пример: `get_ped_m_nPathNodeTimer(ped)`*
- **1022.** `set_ped_m_nPathNodeTimer(пед, число)` → действие — уст. `m_nPathNodeTimer` (Путь узел таймер (целое)).  *Пример: `set_ped_m_nPathNodeTimer(ped, x)`*
- **1023.** `get_ped_m_vecPathNextNode(пед)` → число, число, число (X, Y, Z) — `m_vecPathNextNode` (Путь следующий узел (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecPathNextNode(ped)`*
- **1024.** `set_ped_m_vecPathNextNode(пед, число, число, число)` → действие — уст. `m_vecPathNextNode` (Путь следующий узел (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecPathNextNode(ped, x, y, z)`*
- **1025.** `get_ped_m_fPathNextNodeDir(пед)` → число — `m_fPathNextNodeDir` (Путь следующий узел Dir (число)).  *Пример: `get_ped_m_fPathNextNodeDir(ped)`*
- **1026.** `set_ped_m_fPathNextNodeDir(пед, число)` → действие — уст. `m_fPathNextNodeDir` (Путь следующий узел Dir (число)).  *Пример: `set_ped_m_fPathNextNodeDir(ped, x)`*
- **1027.** `get_ped_m_nPathNodeType(пед)` → целое — `m_nPathNodeType` (Путь узел Type (целое)).  *Пример: `get_ped_m_nPathNodeType(ped)`*
- **1028.** `set_ped_m_nPathNodeType(пед, число)` → действие — уст. `m_nPathNodeType` (Путь узел Type (целое)).  *Пример: `set_ped_m_nPathNodeType(ped, x)`*
- **1029.** `get_ped_m_fHealth(пед)` → число — `m_fHealth` (Здоровье (число)).  *Пример: `get_ped_m_fHealth(ped)`*
- **1030.** `set_ped_m_fHealth(пед, число)` → действие — уст. `m_fHealth` (Здоровье (число)).  *Пример: `set_ped_m_fHealth(ped, x)`*
- **1031.** `get_ped_m_fArmour(пед)` → число — `m_fArmour` (Броня (число)).  *Пример: `get_ped_m_fArmour(ped)`*
- **1032.** `set_ped_m_fArmour(пед, число)` → действие — уст. `m_fArmour` (Броня (число)).  *Пример: `set_ped_m_fArmour(ped, x)`*
- **1033.** `get_ped_m_nShadowUpdateTimer(пед)` → целое — `m_nShadowUpdateTimer` (Shadow Update таймер (целое)).  *Пример: `get_ped_m_nShadowUpdateTimer(ped)`*
- **1034.** `set_ped_m_nShadowUpdateTimer(пед, число)` → действие — уст. `m_nShadowUpdateTimer` (Shadow Update таймер (целое)).  *Пример: `set_ped_m_nShadowUpdateTimer(ped, x)`*
- **1035.** `get_ped_wRouteLastPoint(пед)` → целое — wRouteLastPoint.  *Пример: `get_ped_wRouteLastPoint(ped)`*
- **1036.** `set_ped_wRouteLastPoint(пед, число)` → действие — уст. wRouteLastPoint.  *Пример: `set_ped_wRouteLastPoint(ped, x)`*
- **1037.** `get_ped_wRoutePoints(пед)` → целое — wRoutePoints.  *Пример: `get_ped_wRoutePoints(ped)`*
- **1038.** `set_ped_wRoutePoints(пед, число)` → действие — уст. wRoutePoints.  *Пример: `set_ped_wRoutePoints(ped, x)`*
- **1039.** `get_ped_wRoutePos(пед)` → целое — wRoutePos.  *Пример: `get_ped_wRoutePos(ped)`*
- **1040.** `set_ped_wRoutePos(пед, число)` → действие — уст. wRoutePos.  *Пример: `set_ped_wRoutePos(ped, x)`*
- **1041.** `get_ped_wRouteType(пед)` → целое — wRouteType.  *Пример: `get_ped_wRouteType(ped)`*
- **1042.** `set_ped_wRouteType(пед, число)` → действие — уст. wRouteType.  *Пример: `set_ped_wRouteType(ped, x)`*
- **1043.** `get_ped_wRouteCurDir(пед)` → целое — wRouteCurDir.  *Пример: `get_ped_wRouteCurDir(ped)`*
- **1044.** `set_ped_wRouteCurDir(пед, число)` → действие — уст. wRouteCurDir.  *Пример: `set_ped_wRouteCurDir(ped, x)`*
- **1045.** `get_ped_m_fHeadingCurrent(пед)` → число — `m_fHeadingCurrent` (Курс текущий (число)).  *Пример: `get_ped_m_fHeadingCurrent(ped)`*
- **1046.** `set_ped_m_fHeadingCurrent(пед, число)` → действие — уст. `m_fHeadingCurrent` (Курс текущий (число)).  *Пример: `set_ped_m_fHeadingCurrent(ped, x)`*
- **1047.** `get_ped_m_fHeadingGoal(пед)` → число — `m_fHeadingGoal` (Курс Goal (число)).  *Пример: `get_ped_m_fHeadingGoal(ped)`*
- **1048.** `set_ped_m_fHeadingGoal(пед, число)` → действие — уст. `m_fHeadingGoal` (Курс Goal (число)).  *Пример: `set_ped_m_fHeadingGoal(ped, x)`*
- **1049.** `get_ped_m_fHeadingChangeRate(пед)` → число — `m_fHeadingChangeRate` (Курс Change Rate (число)).  *Пример: `get_ped_m_fHeadingChangeRate(ped)`*
- **1050.** `set_ped_m_fHeadingChangeRate(пед, число)` → действие — уст. `m_fHeadingChangeRate` (Курс Change Rate (число)).  *Пример: `set_ped_m_fHeadingChangeRate(ped, x)`*
- **1051.** `get_ped_m_nEnterType(пед)` → целое — `m_nEnterType` (Вход Type (целое)).  *Пример: `get_ped_m_nEnterType(ped)`*
- **1052.** `set_ped_m_nEnterType(пед, число)` → действие — уст. `m_nEnterType` (Вход Type (целое)).  *Пример: `set_ped_m_nEnterType(ped, x)`*
- **1053.** `get_ped_m_nWalkAroundType(пед)` → целое — `m_nWalkAroundType` (Walk Around Type (целое)).  *Пример: `get_ped_m_nWalkAroundType(ped)`*
- **1054.** `set_ped_m_nWalkAroundType(пед, число)` → действие — уст. `m_nWalkAroundType` (Walk Around Type (целое)).  *Пример: `set_ped_m_nWalkAroundType(ped, x)`*
- **1055.** `get_ped_m_vecOffsetFromPhysSurface(пед)` → число, число, число (X, Y, Z) — `m_vecOffsetFromPhysSurface` (Offset From Phys Surface (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecOffsetFromPhysSurface(ped)`*
- **1056.** `set_ped_m_vecOffsetFromPhysSurface(пед, число, число, число)` → действие — уст. `m_vecOffsetFromPhysSurface` (Offset From Phys Surface (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecOffsetFromPhysSurface(ped, x, y, z)`*
- **1057.** `get_ped_vecSeekVehicle(пед)` → число, число, число (X, Y, Z) — vecSeekVehicle.  *Пример: `get_ped_vecSeekVehicle(ped)`*
- **1058.** `set_ped_vecSeekVehicle(пед, число, число, число)` → действие — уст. vecSeekVehicle.  *Пример: `set_ped_vecSeekVehicle(ped, x, y, z)`*
- **1059.** `get_ped_m_bInVehicle(пед)` → флаг — `m_bInVehicle` (In транспорт (флаг)).  *Пример: `get_ped_m_bInVehicle(ped)`*
- **1060.** `set_ped_m_bInVehicle(пед, флаг)` → действие — уст. `m_bInVehicle` (In транспорт (флаг)).  *Пример: `set_ped_m_bInVehicle(ped, true)`*
- **1061.** `get_ped_m_fSeatPrecisionX(пед)` → число — `m_fSeatPrecisionX` (Сиденье Precision X (число)).  *Пример: `get_ped_m_fSeatPrecisionX(ped)`*
- **1062.** `set_ped_m_fSeatPrecisionX(пед, число)` → действие — уст. `m_fSeatPrecisionX` (Сиденье Precision X (число)).  *Пример: `set_ped_m_fSeatPrecisionX(ped, x)`*
- **1063.** `get_ped_m_fSeatPrecisionY(пед)` → число — `m_fSeatPrecisionY` (Сиденье Precision Y (число)).  *Пример: `get_ped_m_fSeatPrecisionY(ped)`*
- **1064.** `set_ped_m_fSeatPrecisionY(пед, число)` → действие — уст. `m_fSeatPrecisionY` (Сиденье Precision Y (число)).  *Пример: `set_ped_m_fSeatPrecisionY(ped, x)`*
- **1065.** `get_ped_m_nSeatType(пед)` → целое — `m_nSeatType` (Сиденье Type (целое)).  *Пример: `get_ped_m_nSeatType(ped)`*
- **1066.** `set_ped_m_nSeatType(пед, число)` → действие — уст. `m_nSeatType` (Сиденье Type (целое)).  *Пример: `set_ped_m_nSeatType(ped, x)`*
- **1067.** `get_ped_m_bHasPhone(пед)` → флаг — `m_bHasPhone` (Has Phone (флаг)).  *Пример: `get_ped_m_bHasPhone(ped)`*
- **1068.** `set_ped_m_bHasPhone(пед, флаг)` → действие — уст. `m_bHasPhone` (Has Phone (флаг)).  *Пример: `set_ped_m_bHasPhone(ped, true)`*
- **1069.** `get_ped_m_wPhoneId(пед)` → целое — `m_wPhoneId` (Phone ID (целое)).  *Пример: `get_ped_m_wPhoneId(ped)`*
- **1070.** `set_ped_m_wPhoneId(пед, число)` → действие — уст. `m_wPhoneId` (Phone ID (целое)).  *Пример: `set_ped_m_wPhoneId(ped, x)`*
- **1071.** `get_ped_m_nLookingForPhone(пед)` → целое — `m_nLookingForPhone` (Looking для Phone (целое)).  *Пример: `get_ped_m_nLookingForPhone(ped)`*
- **1072.** `set_ped_m_nLookingForPhone(пед, число)` → действие — уст. `m_nLookingForPhone` (Looking для Phone (целое)).  *Пример: `set_ped_m_nLookingForPhone(ped, x)`*
- **1073.** `get_ped_m_nPhoneTalkTimer(пед)` → целое — `m_nPhoneTalkTimer` (Phone Talk таймер (целое)).  *Пример: `get_ped_m_nPhoneTalkTimer(ped)`*
- **1074.** `set_ped_m_nPhoneTalkTimer(пед, число)` → действие — уст. `m_nPhoneTalkTimer` (Phone Talk таймер (целое)).  *Пример: `set_ped_m_nPhoneTalkTimer(ped, x)`*
- **1075.** `get_ped_m_fFleeFromPosX(пед)` → число — `m_fFleeFromPosX` (Flee From позиция X (число)).  *Пример: `get_ped_m_fFleeFromPosX(ped)`*
- **1076.** `set_ped_m_fFleeFromPosX(пед, число)` → действие — уст. `m_fFleeFromPosX` (Flee From позиция X (число)).  *Пример: `set_ped_m_fFleeFromPosX(ped, x)`*
- **1077.** `get_ped_m_fFleeFromPosY(пед)` → число — `m_fFleeFromPosY` (Flee From позиция Y (число)).  *Пример: `get_ped_m_fFleeFromPosY(ped)`*
- **1078.** `set_ped_m_fFleeFromPosY(пед, число)` → действие — уст. `m_fFleeFromPosY` (Flee From позиция Y (число)).  *Пример: `set_ped_m_fFleeFromPosY(ped, x)`*
- **1079.** `get_ped_m_nFleeTimer(пед)` → целое — `m_nFleeTimer` (Flee таймер (целое)).  *Пример: `get_ped_m_nFleeTimer(ped)`*
- **1080.** `set_ped_m_nFleeTimer(пед, число)` → действие — уст. `m_nFleeTimer` (Flee таймер (целое)).  *Пример: `set_ped_m_nFleeTimer(ped, x)`*
- **1081.** `get_ped_m_nLastThreatTimer(пед)` → целое — `m_nLastThreatTimer` (Последний Threat таймер (целое)).  *Пример: `get_ped_m_nLastThreatTimer(ped)`*
- **1082.** `set_ped_m_nLastThreatTimer(пед, число)` → действие — уст. `m_nLastThreatTimer` (Последний Threat таймер (целое)).  *Пример: `set_ped_m_nLastThreatTimer(ped, x)`*
- **1083.** `get_ped_m_nStateUnused(пед)` → целое — `m_nStateUnused` (Состояние Unused (целое)).  *Пример: `get_ped_m_nStateUnused(ped)`*
- **1084.** `set_ped_m_nStateUnused(пед, число)` → действие — уст. `m_nStateUnused` (Состояние Unused (целое)).  *Пример: `set_ped_m_nStateUnused(ped, x)`*
- **1085.** `get_ped_m_nTimerUnused(пед)` → целое — `m_nTimerUnused` (Таймер Unused (целое)).  *Пример: `get_ped_m_nTimerUnused(ped)`*
- **1086.** `set_ped_m_nTimerUnused(пед, число)` → действие — уст. `m_nTimerUnused` (Таймер Unused (целое)).  *Пример: `set_ped_m_nTimerUnused(ped, x)`*
- **1087.** `get_ped_m_nAtchStoredWep(пед)` → целое — `m_nAtchStoredWep` (Atch Stored оружие (целое)).  *Пример: `get_ped_m_nAtchStoredWep(ped)`*
- **1088.** `set_ped_m_nAtchStoredWep(пед, число)` → действие — уст. `m_nAtchStoredWep` (Atch Stored оружие (целое)).  *Пример: `set_ped_m_nAtchStoredWep(ped, x)`*
- **1089.** `get_ped_m_nStoredGiveWep(пед)` → целое — `m_nStoredGiveWep` (Stored Give оружие (целое)).  *Пример: `get_ped_m_nStoredGiveWep(ped)`*
- **1090.** `set_ped_m_nStoredGiveWep(пед, число)` → действие — уст. `m_nStoredGiveWep` (Stored Give оружие (целое)).  *Пример: `set_ped_m_nStoredGiveWep(ped, x)`*
- **1091.** `get_ped_m_nStoredGiveAmmo(пед)` → целое — `m_nStoredGiveAmmo` (Stored Give патроны (целое)).  *Пример: `get_ped_m_nStoredGiveAmmo(ped)`*
- **1092.** `set_ped_m_nStoredGiveAmmo(пед, число)` → действие — уст. `m_nStoredGiveAmmo` (Stored Give патроны (целое)).  *Пример: `set_ped_m_nStoredGiveAmmo(ped, x)`*
- **1093.** `get_ped_m_nCurrentWeapon(пед)` → целое — `m_nCurrentWeapon` (Текущий оружие (целое)).  *Пример: `get_ped_m_nCurrentWeapon(ped)`*
- **1094.** `set_ped_m_nCurrentWeapon(пед, число)` → действие — уст. `m_nCurrentWeapon` (Текущий оружие (целое)).  *Пример: `set_ped_m_nCurrentWeapon(ped, x)`*
- **1095.** `get_ped_m_nWepSkills(пед)` → целое — `m_nWepSkills` (Оружие Skills (целое)).  *Пример: `get_ped_m_nWepSkills(ped)`*
- **1096.** `set_ped_m_nWepSkills(пед, число)` → действие — уст. `m_nWepSkills` (Оружие Skills (целое)).  *Пример: `set_ped_m_nWepSkills(ped, x)`*
- **1097.** `get_ped_m_nWeaponAccuracy(пед)` → целое — `m_nWeaponAccuracy` (Оружие Accuracy (целое)).  *Пример: `get_ped_m_nWeaponAccuracy(ped)`*
- **1098.** `set_ped_m_nWeaponAccuracy(пед, число)` → действие — уст. `m_nWeaponAccuracy` (Оружие Accuracy (целое)).  *Пример: `set_ped_m_nWeaponAccuracy(ped, x)`*
- **1099.** `get_ped_m_nBodyPart(пед)` → целое — `m_nBodyPart` (Кузов часть (целое)).  *Пример: `get_ped_m_nBodyPart(ped)`*
- **1100.** `set_ped_m_nBodyPart(пед, число)` → действие — уст. `m_nBodyPart` (Кузов часть (целое)).  *Пример: `set_ped_m_nBodyPart(ped, x)`*
- **1101.** `get_ped_m_vecHitLastPos(пед)` → число, число, число (X, Y, Z) — `m_vecHitLastPos` (Hit последний позиция (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecHitLastPos(ped)`*
- **1102.** `set_ped_m_vecHitLastPos(пед, число, число, число)` → действие — уст. `m_vecHitLastPos` (Hit последний позиция (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecHitLastPos(ped, x, y, z)`*
- **1103.** `get_ped_m_nHitCounter(пед)` → целое — `m_nHitCounter` (Hit Counter (целое)).  *Пример: `get_ped_m_nHitCounter(ped)`*
- **1104.** `set_ped_m_nHitCounter(пед, число)` → действие — уст. `m_nHitCounter` (Hit Counter (целое)).  *Пример: `set_ped_m_nHitCounter(ped, x)`*
- **1105.** `get_ped_m_nLastHitState(пед)` → целое — `m_nLastHitState` (Последний Hit состояние (целое)).  *Пример: `get_ped_m_nLastHitState(ped)`*
- **1106.** `set_ped_m_nLastHitState(пед, число)` → действие — уст. `m_nLastHitState` (Последний Hit состояние (целое)).  *Пример: `set_ped_m_nLastHitState(ped, x)`*
- **1107.** `get_ped_m_nFightFlags1(пед)` → целое — `m_nFightFlags1` (Fight Flags1 (целое)).  *Пример: `get_ped_m_nFightFlags1(ped)`*
- **1108.** `set_ped_m_nFightFlags1(пед, число)` → действие — уст. `m_nFightFlags1` (Fight Flags1 (целое)).  *Пример: `set_ped_m_nFightFlags1(ped, x)`*
- **1109.** `get_ped_m_nFightFlags2(пед)` → целое — `m_nFightFlags2` (Fight Flags2 (целое)).  *Пример: `get_ped_m_nFightFlags2(ped)`*
- **1110.** `set_ped_m_nFightFlags2(пед, число)` → действие — уст. `m_nFightFlags2` (Fight Flags2 (целое)).  *Пример: `set_ped_m_nFightFlags2(ped, x)`*
- **1111.** `get_ped_m_nFightFlags3(пед)` → целое — `m_nFightFlags3` (Fight Flags3 (целое)).  *Пример: `get_ped_m_nFightFlags3(ped)`*
- **1112.** `set_ped_m_nFightFlags3(пед, число)` → действие — уст. `m_nFightFlags3` (Fight Flags3 (целое)).  *Пример: `set_ped_m_nFightFlags3(ped, x)`*
- **1113.** `get_ped_m_nBleedCounter(пед)` → целое — `m_nBleedCounter` (Bleed Counter (целое)).  *Пример: `get_ped_m_nBleedCounter(ped)`*
- **1114.** `set_ped_m_nBleedCounter(пед, число)` → действие — уст. `m_nBleedCounter` (Bleed Counter (целое)).  *Пример: `set_ped_m_nBleedCounter(ped, x)`*
- **1115.** `get_ped_m_fLookDirection(пед)` → число — `m_fLookDirection` (Look Direction (число)).  *Пример: `get_ped_m_fLookDirection(ped)`*
- **1116.** `set_ped_m_fLookDirection(пед, число)` → действие — уст. `m_fLookDirection` (Look Direction (число)).  *Пример: `set_ped_m_fLookDirection(ped, x)`*
- **1117.** `get_ped_m_nWepModelID(пед)` → целое — `m_nWepModelID` (Оружие модель I D (целое)).  *Пример: `get_ped_m_nWepModelID(ped)`*
- **1118.** `set_ped_m_nWepModelID(пед, число)` → действие — уст. `m_nWepModelID` (Оружие модель I D (целое)).  *Пример: `set_ped_m_nWepModelID(ped, x)`*
- **1119.** `get_ped_m_nLeaveCarTimer(пед)` → целое — `m_nLeaveCarTimer` (Leave авто таймер (целое)).  *Пример: `get_ped_m_nLeaveCarTimer(ped)`*
- **1120.** `set_ped_m_nLeaveCarTimer(пед, число)` → действие — уст. `m_nLeaveCarTimer` (Leave авто таймер (целое)).  *Пример: `set_ped_m_nLeaveCarTimer(ped, x)`*
- **1121.** `get_ped_m_nGetUpTimer(пед)` → целое — `m_nGetUpTimer` (Get Up таймер (целое)).  *Пример: `get_ped_m_nGetUpTimer(ped)`*
- **1122.** `set_ped_m_nGetUpTimer(пед, число)` → действие — уст. `m_nGetUpTimer` (Get Up таймер (целое)).  *Пример: `set_ped_m_nGetUpTimer(ped, x)`*
- **1123.** `get_ped_m_nLookTimer(пед)` → целое — `m_nLookTimer` (Look таймер (целое)).  *Пример: `get_ped_m_nLookTimer(ped)`*
- **1124.** `set_ped_m_nLookTimer(пед, число)` → действие — уст. `m_nLookTimer` (Look таймер (целое)).  *Пример: `set_ped_m_nLookTimer(ped, x)`*
- **1125.** `get_ped_m_nStandardTimer(пед)` → целое — `m_nStandardTimer` (Standard таймер (целое)).  *Пример: `get_ped_m_nStandardTimer(ped)`*
- **1126.** `set_ped_m_nStandardTimer(пед, число)` → действие — уст. `m_nStandardTimer` (Standard таймер (целое)).  *Пример: `set_ped_m_nStandardTimer(ped, x)`*
- **1127.** `get_ped_m_nAttackTimer(пед)` → целое — `m_nAttackTimer` (Attack таймер (целое)).  *Пример: `get_ped_m_nAttackTimer(ped)`*
- **1128.** `set_ped_m_nAttackTimer(пед, число)` → действие — уст. `m_nAttackTimer` (Attack таймер (целое)).  *Пример: `set_ped_m_nAttackTimer(ped, x)`*
- **1129.** `get_ped_m_nLastHitTime(пед)` → целое — `m_nLastHitTime` (Последний Hit время (целое)).  *Пример: `get_ped_m_nLastHitTime(ped)`*
- **1130.** `set_ped_m_nLastHitTime(пед, число)` → действие — уст. `m_nLastHitTime` (Последний Hit время (целое)).  *Пример: `set_ped_m_nLastHitTime(ped, x)`*
- **1131.** `get_ped_m_nHitRecoverTimer(пед)` → целое — `m_nHitRecoverTimer` (Hit Recover таймер (целое)).  *Пример: `get_ped_m_nHitRecoverTimer(ped)`*
- **1132.** `set_ped_m_nHitRecoverTimer(пед, число)` → действие — уст. `m_nHitRecoverTimer` (Hit Recover таймер (целое)).  *Пример: `set_ped_m_nHitRecoverTimer(ped, x)`*
- **1133.** `get_ped_m_nObjectiveTimer(пед)` → целое — `m_nObjectiveTimer` (Цель таймер (целое)).  *Пример: `get_ped_m_nObjectiveTimer(ped)`*
- **1134.** `set_ped_m_nObjectiveTimer(пед, число)` → действие — уст. `m_nObjectiveTimer` (Цель таймер (целое)).  *Пример: `set_ped_m_nObjectiveTimer(ped, x)`*
- **1135.** `get_ped_m_nDuckTimer(пед)` → целое — `m_nDuckTimer` (Duck таймер (целое)).  *Пример: `get_ped_m_nDuckTimer(ped)`*
- **1136.** `set_ped_m_nDuckTimer(пед, число)` → действие — уст. `m_nDuckTimer` (Duck таймер (целое)).  *Пример: `set_ped_m_nDuckTimer(ped, x)`*
- **1137.** `get_ped_m_nDuckAndCoverTimer(пед)` → целое — `m_nDuckAndCoverTimer` (Duck And Cover таймер (целое)).  *Пример: `get_ped_m_nDuckAndCoverTimer(ped)`*
- **1138.** `set_ped_m_nDuckAndCoverTimer(пед, число)` → действие — уст. `m_nDuckAndCoverTimer` (Duck And Cover таймер (целое)).  *Пример: `set_ped_m_nDuckAndCoverTimer(ped, x)`*
- **1139.** `get_ped_m_nBloodyTimer(пед)` → целое — `m_nBloodyTimer` (Bloody таймер (целое)).  *Пример: `get_ped_m_nBloodyTimer(ped)`*
- **1140.** `set_ped_m_nBloodyTimer(пед, число)` → действие — уст. `m_nBloodyTimer` (Bloody таймер (целое)).  *Пример: `set_ped_m_nBloodyTimer(ped, x)`*
- **1141.** `get_ped_m_nShotTime(пед)` → целое — `m_nShotTime` (Shot время (целое)).  *Пример: `get_ped_m_nShotTime(ped)`*
- **1142.** `set_ped_m_nShotTime(пед, число)` → действие — уст. `m_nShotTime` (Shot время (целое)).  *Пример: `set_ped_m_nShotTime(ped, x)`*
- **1143.** `get_ped_m_nShotTimeAdd(пед)` → целое — `m_nShotTimeAdd` (Shot время Add (целое)).  *Пример: `get_ped_m_nShotTimeAdd(ped)`*
- **1144.** `set_ped_m_nShotTimeAdd(пед, число)` → действие — уст. `m_nShotTimeAdd` (Shot время Add (целое)).  *Пример: `set_ped_m_nShotTimeAdd(ped, x)`*
- **1145.** `get_ped_m_nPanicCounter(пед)` → целое — `m_nPanicCounter` (Panic Counter (целое)).  *Пример: `get_ped_m_nPanicCounter(ped)`*
- **1146.** `set_ped_m_nPanicCounter(пед, число)` → действие — уст. `m_nPanicCounter` (Panic Counter (целое)).  *Пример: `set_ped_m_nPanicCounter(ped, x)`*
- **1147.** `get_ped_m_nDeadBleeding(пед)` → целое — `m_nDeadBleeding` (Dead Bleeding (целое)).  *Пример: `get_ped_m_nDeadBleeding(ped)`*
- **1148.** `set_ped_m_nDeadBleeding(пед, число)` → действие — уст. `m_nDeadBleeding` (Dead Bleeding (целое)).  *Пример: `set_ped_m_nDeadBleeding(ped, x)`*
- **1149.** `get_ped_m_nBodyPartBleeding(пед)` → целое — `m_nBodyPartBleeding` (Кузов часть Bleeding (целое)).  *Пример: `get_ped_m_nBodyPartBleeding(ped)`*
- **1150.** `set_ped_m_nBodyPartBleeding(пед, число)` → действие — уст. `m_nBodyPartBleeding` (Кузов часть Bleeding (целое)).  *Пример: `set_ped_m_nBodyPartBleeding(ped, x)`*
- **1151.** `get_ped_m_nNumNearPeds(пед)` → целое — `m_nNumNearPeds` (Кол-во Near Peds (целое)).  *Пример: `get_ped_m_nNumNearPeds(ped)`*
- **1152.** `set_ped_m_nNumNearPeds(пед, число)` → действие — уст. `m_nNumNearPeds` (Кол-во Near Peds (целое)).  *Пример: `set_ped_m_nNumNearPeds(ped, x)`*
- **1153.** `get_ped_m_nPedMoney(пед)` → целое — `m_nPedMoney` (Пед деньги (целое)).  *Пример: `get_ped_m_nPedMoney(ped)`*
- **1154.** `set_ped_m_nPedMoney(пед, число)` → действие — уст. `m_nPedMoney` (Пед деньги (целое)).  *Пример: `set_ped_m_nPedMoney(ped, x)`*
- **1155.** `get_ped_m_nLastDamWep(пед)` → целое — `m_nLastDamWep` (Последний урон оружие (целое)).  *Пример: `get_ped_m_nLastDamWep(ped)`*
- **1156.** `set_ped_m_nLastDamWep(пед, число)` → действие — уст. `m_nLastDamWep` (Последний урон оружие (целое)).  *Пример: `set_ped_m_nLastDamWep(ped, x)`*
- **1157.** `get_ped_m_vecAttachOffset(пед)` → число, число, число (X, Y, Z) — `m_vecAttachOffset` (Attach Offset (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecAttachOffset(ped)`*
- **1158.** `set_ped_m_vecAttachOffset(пед, число, число, число)` → действие — уст. `m_vecAttachOffset` (Attach Offset (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecAttachOffset(ped, x, y, z)`*
- **1159.** `get_ped_m_nAttachType(пед)` → целое — `m_nAttachType` (Attach Type (целое)).  *Пример: `get_ped_m_nAttachType(ped)`*
- **1160.** `set_ped_m_nAttachType(пед, число)` → действие — уст. `m_nAttachType` (Attach Type (целое)).  *Пример: `set_ped_m_nAttachType(ped, x)`*
- **1161.** `get_ped_m_fAttachRot(пед)` → число — `m_fAttachRot` (Attach Rot (число)).  *Пример: `get_ped_m_fAttachRot(ped)`*
- **1162.** `set_ped_m_fAttachRot(пед, число)` → действие — уст. `m_fAttachRot` (Attach Rot (число)).  *Пример: `set_ped_m_fAttachRot(ped, x)`*
- **1163.** `get_ped_m_nAttachWepAmmo(пед)` → целое — `m_nAttachWepAmmo` (Attach оружие патроны (целое)).  *Пример: `get_ped_m_nAttachWepAmmo(ped)`*
- **1164.** `set_ped_m_nAttachWepAmmo(пед, число)` → действие — уст. `m_nAttachWepAmmo` (Attach оружие патроны (целое)).  *Пример: `set_ped_m_nAttachWepAmmo(ped, x)`*
- **1165.** `get_ped_m_nThreatFlags(пед)` → целое — `m_nThreatFlags` (Threat флаги (целое)).  *Пример: `get_ped_m_nThreatFlags(ped)`*
- **1166.** `set_ped_m_nThreatFlags(пед, число)` → действие — уст. `m_nThreatFlags` (Threat флаги (целое)).  *Пример: `set_ped_m_nThreatFlags(ped, x)`*
- **1167.** `get_ped_m_nThreatCheck(пед)` → целое — `m_nThreatCheck` (Threat Check (целое)).  *Пример: `get_ped_m_nThreatCheck(ped)`*
- **1168.** `set_ped_m_nThreatCheck(пед, число)` → действие — уст. `m_nThreatCheck` (Threat Check (целое)).  *Пример: `set_ped_m_nThreatCheck(ped, x)`*
- **1169.** `get_ped_m_nLastThreatCheck(пед)` → целое — `m_nLastThreatCheck` (Последний Threat Check (целое)).  *Пример: `get_ped_m_nLastThreatCheck(ped)`*
- **1170.** `set_ped_m_nLastThreatCheck(пед, число)` → действие — уст. `m_nLastThreatCheck` (Последний Threat Check (целое)).  *Пример: `set_ped_m_nLastThreatCheck(ped, x)`*
- **1171.** `get_ped_m_nSayType(пед)` → целое — `m_nSayType` (Say Type (целое)).  *Пример: `get_ped_m_nSayType(ped)`*
- **1172.** `set_ped_m_nSayType(пед, число)` → действие — уст. `m_nSayType` (Say Type (целое)).  *Пример: `set_ped_m_nSayType(ped, x)`*
- **1173.** `get_ped_m_nSayTimer(пед)` → целое — `m_nSayTimer` (Say таймер (целое)).  *Пример: `get_ped_m_nSayTimer(ped)`*
- **1174.** `set_ped_m_nSayTimer(пед, число)` → действие — уст. `m_nSayTimer` (Say таймер (целое)).  *Пример: `set_ped_m_nSayTimer(ped, x)`*
- **1175.** `get_ped_m_nTalkTimerLast(пед)` → целое — `m_nTalkTimerLast` (Talk таймер последний (целое)).  *Пример: `get_ped_m_nTalkTimerLast(ped)`*
- **1176.** `set_ped_m_nTalkTimerLast(пед, число)` → действие — уст. `m_nTalkTimerLast` (Talk таймер последний (целое)).  *Пример: `set_ped_m_nTalkTimerLast(ped, x)`*
- **1177.** `get_ped_m_nTalkTimer(пед)` → целое — `m_nTalkTimer` (Talk таймер (целое)).  *Пример: `get_ped_m_nTalkTimer(ped)`*
- **1178.** `set_ped_m_nTalkTimer(пед, число)` → действие — уст. `m_nTalkTimer` (Talk таймер (целое)).  *Пример: `set_ped_m_nTalkTimer(ped, x)`*
- **1179.** `get_ped_m_wTalkTypeLast(пед)` → целое — `m_wTalkTypeLast` (Talk Type последний (целое)).  *Пример: `get_ped_m_wTalkTypeLast(ped)`*
- **1180.** `set_ped_m_wTalkTypeLast(пед, число)` → действие — уст. `m_wTalkTypeLast` (Talk Type последний (целое)).  *Пример: `set_ped_m_wTalkTypeLast(ped, x)`*
- **1181.** `get_ped_m_wTalkType(пед)` → целое — `m_wTalkType` (Talk Type (целое)).  *Пример: `get_ped_m_wTalkType(ped)`*
- **1182.** `set_ped_m_wTalkType(пед, число)` → действие — уст. `m_wTalkType` (Talk Type (целое)).  *Пример: `set_ped_m_wTalkType(ped, x)`*
- **1183.** `get_ped_m_bCanPedTalk(пед)` → флаг — `m_bCanPedTalk` (Can пед Talk (флаг)).  *Пример: `get_ped_m_bCanPedTalk(ped)`*
- **1184.** `set_ped_m_bCanPedTalk(пед, флаг)` → действие — уст. `m_bCanPedTalk` (Can пед Talk (флаг)).  *Пример: `set_ped_m_bCanPedTalk(ped, true)`*
- **1185.** `get_ped_m_nPedLastComment(пед)` → целое — `m_nPedLastComment` (Пед последний Comment (целое)).  *Пример: `get_ped_m_nPedLastComment(ped)`*
- **1186.** `set_ped_m_nPedLastComment(пед, число)` → действие — уст. `m_nPedLastComment` (Пед последний Comment (целое)).  *Пример: `set_ped_m_nPedLastComment(ped, x)`*
- **1187.** `get_ped_m_vecSeekPosEx(пед)` → число, число, число (X, Y, Z) — `m_vecSeekPosEx` (Поиск позиция Ex (вектор (X, Y, Z))).  *Пример: `get_ped_m_vecSeekPosEx(ped)`*
- **1188.** `set_ped_m_vecSeekPosEx(пед, число, число, число)` → действие — уст. `m_vecSeekPosEx` (Поиск позиция Ex (вектор (X, Y, Z))).  *Пример: `set_ped_m_vecSeekPosEx(ped, x, y, z)`*
- **1189.** `get_ped_m_fSeekExAngle(пед)` → число — `m_fSeekExAngle` (Поиск Ex угол (число)).  *Пример: `get_ped_m_fSeekExAngle(ped)`*
- **1190.** `set_ped_m_fSeekExAngle(пед, число)` → действие — уст. `m_fSeekExAngle` (Поиск Ex угол (число)).  *Пример: `set_ped_m_fSeekExAngle(ped, x)`*
- **1322.** `ped_in_targetting()` → указ. — пед на прицеле.  *Пример: `ped_in_targetting()`*
- **1324.** `Get_model_and_type_ped(строка)` → целое — макрос получить модель и тип педа.  *Пример: `Get_model_and_type_ped("текст")`*
- **1325.** `get_model_ped(строка)` → целое — получить модель педа.  *Пример: `get_model_ped("текст")`*
- **1326.** `get_type_ped(строка)` → целое — получить тип педа.  *Пример: `get_type_ped("текст")`*
- **1327.** `ped_clear_investigate_event(пед)` → действие — пед уходит, опустить педа.  *Пример: `ped_clear_investigate_event(ped)`*
- **1336.** `ped_char_spotted_player(пед)` → флаг — если пед увидел игрока.  *Пример: `ped_char_spotted_player(ped)`*
- **1347.** `ped_in_turret_on_car(пед, авто, число, число, число, число, число, число)` → действие — переместить педа в турель авто.  *Пример: `ped_in_turret_on_car(ped, car, x, y, z, 3, 4, 5)`*
- **1348.** `ped_from_turret_on_car(пед)` → действие — убрать педа из турели авто.  *Пример: `ped_from_turret_on_car(ped)`*
- **1359.** `ped_save_pos_attack(пед, число)` → действие — пед сохраняет ли свою позицию при атаке.  *Пример: `ped_save_pos_attack(ped, x)`*
- **1360.** `ped_rebuff(пед, число)` → действие — пед атакует любого, кто нападает на него.  *Пример: `ped_rebuff(ped, x)`*
- **1361.** `ped_search_threat(пед, число)` → действие — пед ищет угрозу.  *Пример: `ped_search_threat(ped, x)`*
- **1362.** `ped_clean_threat(пед)` → действие — очистить угрозы для педа.  *Пример: `ped_clean_threat(ped)`*
- **1365.** `set_ped_bleeding(пед, число)` → действие — вкл\выкл крованные у педа.  *Пример: `set_ped_bleeding(ped, x)`*
- **1366.** `all_exit_from_car(авто, число)` → действие — все педы выходят из авто.  *Пример: `all_exit_from_car(car, x)`*
- **1373.** `set_ped_immunities(пед, число, число, число, число, число)` → действие — уст иммунитеты педу.  *Пример: `set_ped_immunities(ped, x, y, z, 3, 4)`*
- **1377.** `ped_run_to_point(пед, число, число, число)` → действие — пед бежит к точке пешком.  *Пример: `ped_run_to_point(ped, x, y, z)`*
- **1380.** `ped_maintain_position_when_attacked(пед, число)` → действие — пед удерживает позицию при атаке.  *Пример: `ped_maintain_position_when_attacked(ped, x)`*
- **1390.** `set_ped_visible(пед, число)` → действие — уст невидимость педу.  *Пример: `set_ped_visible(ped, x)`*
- **1393.** `set_ped_reaction_threat(пед, флаг)` → действие — уст реакцию педа на ответную угрозу.  *Пример: `set_ped_reaction_threat(ped, true)`*
- **1396.** `set_ped_accuracy(пед, число)` → действие — уст меткость педу.  *Пример: `set_ped_accuracy(ped, x)`*
- **1402.** `set_ped_walk_style(пед, число)` → действие — установить стиль ходьбы педа.  *Пример: `set_ped_walk_style(ped, x)`*
- **1405.** `set_ped_range(число)` → действие — установить множитель диапазона на угрозу для педа.  *Пример: `set_ped_range(x)`*
- **1418.** `ped_follow_ped(пед, указ.)` → флаг — пед следует за педом.  *Пример: `ped_follow_ped(ped, entity)`*
- **1420.** `set_ped_targetted(пед, число)` → действие — запрет целиться в педа.  *Пример: `set_ped_targetted(ped, x)`*
- **1421.** `set_ped_friend(пед, число)` → действие — уст дружественное отношения педа.  *Пример: `set_ped_friend(ped, x)`*
- **1422.** `set_ped_running(пед, число)` → действие — пед может бежать.  *Пример: `set_ped_running(ped, x)`*
- **1439.** `ped_road_off()` → действие — выкл путь педов.  *Пример: `ped_road_off()`*
- **1440.** `ped_road_on(число, число, число, число, число, число)` → действие — вкл путь педов.  *Пример: `ped_road_on(x, y, z, 3, 4, 5)`*
- **1453.** `get_ped_in_cord(число, число, число, число)` → флаг, указ. — получить педа в радиусе с координатами.  *Пример: `get_ped_in_cord(x, y, z, 3)`*


## Объекты (58)

- **56.** `create_money_pickup(число, число, число, число)` → целое — создать пачку денег.  *Пример: `create_money_pickup(x, y, z, 3)`*
- **62.** `create_weapon_pickup(число, число, число, число, число, число)` → целое — создать пикап оружие.  *Пример: `create_weapon_pickup(x, y, z, 3, 4, 5)`*
- **63.** `create_pickup(число, число, число, число, число)` → целое — создать пикап.  *Пример: `create_pickup(x, y, z, 3, 4)`*
- **64.** `remove_pickup(число)` → действие — удалить пикап.  *Пример: `remove_pickup(x)`*
- **91.** `create_marker_pickup(число)` → целое — создать маркер над пикапом.  *Пример: `create_marker_pickup(x)`*
- **100.** `move_obj(объект, число, число, число, число, число, число, число)` → действие — двигать объект.  *Пример: `move_obj(obj, x, y, z, 3, 4, 5, 6)`*
- **169.** `Create_weapon_pickup(число, число, число, число, число, число)` → целое — макрос создать пикап оружие.  *Пример: `Create_weapon_pickup(x, y, z, 3, 4, 5)`*
- **170.** `Create_pickup(число, число, число, число, число)` → целое — макрос создать пикап.  *Пример: `Create_pickup(x, y, z, 3, 4)`*
- **379.** `get_obj_type(объект)` → целое — получить тип объекта.  *Пример: `get_obj_type(obj)`*
- **380.** `get_obj_bonus_value(объект)` → целое — получить бонус объекта.  *Пример: `get_obj_bonus_value(obj)`*
- **381.** `set_obj_bonus_value(объект, число)` → действие — установить бонус объекта.  *Пример: `set_obj_bonus_value(obj, x)`*
- **382.** `get_obj_cost_value(объект)` → целое — получить стоимость объекта.  *Пример: `get_obj_cost_value(obj)`*
- **383.** `set_obj_cost_value(объект, число)` → действие — установить стоимость объекта.  *Пример: `set_obj_cost_value(obj, x)`*
- **384.** `get_obj_damage_multiplier(объект)` → число — получить множитель урона.  *Пример: `get_obj_damage_multiplier(obj)`*
- **385.** `set_obj_damage_multiplier(объект, число)` → действие — установить множитель урона.  *Пример: `set_obj_damage_multiplier(obj, x)`*
- **386.** `get_obj_collision_damage_type(объект)` → целое — тип урона от столкновений.  *Пример: `get_obj_collision_damage_type(obj)`*
- **387.** `set_obj_collision_damage_type(объект, число)` → действие — установить тип урона.  *Пример: `set_obj_collision_damage_type(obj, x)`*
- **388.** `get_obj_camera_avoids(объект)` → флаг — камера обходит объект.  *Пример: `get_obj_camera_avoids(obj)`*
- **389.** `set_obj_camera_avoids(объект, флаг)` → действие — установить обход камерой.  *Пример: `set_obj_camera_avoids(obj, true)`*
- **390.** `get_obj_timer(объект)` → целое — получить таймер объекта.  *Пример: `get_obj_timer(obj)`*
- **391.** `set_obj_timer(объект, число)` → действие — установить таймер объекта.  *Пример: `set_obj_timer(obj, x)`*
- **392.** `get_obj_ref_model_id(объект)` → целое — модель на которую ссылается объект.  *Пример: `get_obj_ref_model_id(obj)`*
- **393.** `obj_can_be_deleted(объект)` → флаг — можно удалить объект.  *Пример: `obj_can_be_deleted(obj)`*
- **394.** `obj_damage(объект, число)` → действие — нанести урон объекту.  *Пример: `obj_damage(obj, x)`*
- **395.** `obj_delete_all_mission()` → действие — удалить миссионные объекты.  *Пример: `obj_delete_all_mission()`*
- **396.** `obj_delete_all_temp()` → действие — удалить временные объекты.  *Пример: `obj_delete_all_temp()`*
- **397.** `obj_delete_all_temp_in_area(число, число, число, число)` → действие — удалить временные в радиусе.  *Пример: `obj_delete_all_temp_in_area(x, y, z, 3)`*
- **398.** `get_obj_attach_force(объект)` → число — сила прикрепления объекта.  *Пример: `get_obj_attach_force(obj)`*
- **399.** `set_obj_attach_force(объект, число)` → действие — установить силу прикрепления.  *Пример: `set_obj_attach_force(obj, x)`*
- **430.** `get_stat_garbage_pickups()` → число — собранный мусор.  *Пример: `get_stat_garbage_pickups()`*
- **911.** `do_pickup_effects(указ.)` → действие — эффект поднятия пикапа.  *Пример: `do_pickup_effects(entity)`*
- **914.** `get_obj_m_fAttachForce(объект)` → число — сила привязки объекта.  *Пример: `get_obj_m_fAttachForce(obj)`*
- **915.** `set_obj_m_fAttachForce(объект, число)` → действие — уст. сила привязки объекта.  *Пример: `set_obj_m_fAttachForce(obj, x)`*
- **916.** `get_obj_m_CollisionDamageType(объект)` → число — тип урона от столкновений.  *Пример: `get_obj_m_CollisionDamageType(obj)`*
- **917.** `set_obj_m_CollisionDamageType(объект, число)` → действие — уст. тип урона от столкновений.  *Пример: `set_obj_m_CollisionDamageType(obj, x)`*
- **918.** `get_obj_m_nSpecialCollisionType(объект)` → число — специальный тип столкновений.  *Пример: `get_obj_m_nSpecialCollisionType(obj)`*
- **919.** `set_obj_m_nSpecialCollisionType(объект, число)` → действие — уст. специальный тип столкновений.  *Пример: `set_obj_m_nSpecialCollisionType(obj, x)`*
- **920.** `get_obj_m_nBounceScore(объект)` → число — счёт отскока объекта.  *Пример: `get_obj_m_nBounceScore(obj)`*
- **921.** `set_obj_m_nBounceScore(объект, число)` → действие — уст. счёт отскока объекта.  *Пример: `set_obj_m_nBounceScore(obj, x)`*
- **922.** `get_obj_m_dwObjectTimer(объект)` → число — таймер объекта.  *Пример: `get_obj_m_dwObjectTimer(obj)`*
- **923.** `set_obj_m_dwObjectTimer(объект, число)` → действие — уст. таймер объекта.  *Пример: `set_obj_m_dwObjectTimer(obj, x)`*
- **1191.** `get_obj_m_nObjectType(объект)` → целое — `m_nObjectType` (Object Type (целое)).  *Пример: `get_obj_m_nObjectType(obj)`*
- **1192.** `set_obj_m_nObjectType(объект, число)` → действие — уст. `m_nObjectType` (Object Type (целое)).  *Пример: `set_obj_m_nObjectType(obj, x)`*
- **1193.** `get_obj_m_nBonusValue(объект)` → целое — `m_nBonusValue` (Bonus значение (целое)).  *Пример: `get_obj_m_nBonusValue(obj)`*
- **1194.** `set_obj_m_nBonusValue(объект, число)` → действие — уст. `m_nBonusValue` (Bonus значение (целое)).  *Пример: `set_obj_m_nBonusValue(obj, x)`*
- **1195.** `get_obj_m_wCostValue(объект)` → целое — `m_wCostValue` (Cost значение (целое)).  *Пример: `get_obj_m_wCostValue(obj)`*
- **1196.** `set_obj_m_wCostValue(объект, число)` → действие — уст. `m_wCostValue` (Cost значение (целое)).  *Пример: `set_obj_m_wCostValue(obj, x)`*
- **1197.** `get_obj_m_fDamageMultiplier(объект)` → число — `m_fDamageMultiplier` (Урон множитель (число)).  *Пример: `get_obj_m_fDamageMultiplier(obj)`*
- **1198.** `set_obj_m_fDamageMultiplier(объект, число)` → действие — уст. `m_fDamageMultiplier` (Урон множитель (число)).  *Пример: `set_obj_m_fDamageMultiplier(obj, x)`*
- **1199.** `get_obj_m_bCameraAvoids(объект)` → флаг — `m_bCameraAvoids` (Camera Avoids (флаг)).  *Пример: `get_obj_m_bCameraAvoids(obj)`*
- **1200.** `set_obj_m_bCameraAvoids(объект, флаг)` → действие — уст. `m_bCameraAvoids` (Camera Avoids (флаг)).  *Пример: `set_obj_m_bCameraAvoids(obj, true)`*
- **1201.** `get_obj_m_wRefModelId(объект)` → целое — `m_wRefModelId` (Ref модель ID (целое)).  *Пример: `get_obj_m_wRefModelId(obj)`*
- **1202.** `set_obj_m_wRefModelId(объект, число)` → действие — уст. `m_wRefModelId` (Ref модель ID (целое)).  *Пример: `set_obj_m_wRefModelId(obj, x)`*
- **1394.** `obj_target(объект, флаг)` → действие — в объект можно целиться.  *Пример: `obj_target(obj, true)`*
- **1400.** `create_pickup_clothes(число, число, число, число)` → целое — создать пикап одежды.  *Пример: `create_pickup_clothes(x, y, z, 3)`*
- **1411.** `Createobj(число, число, число, число)` → указ. — макрос создать объект.  *Пример: `Createobj(x, y, z, 3)`*
- **1444.** `randomfindobj(пед, число)` → флаг, указ. — найти случайный объект в радиусе.  *Пример: `randomfindobj(ped, x)`*
- **1457.** `get_obj_in_cord(число, число, число, число)` → указ. — получить объект в радиусе с координатам.  *Пример: `get_obj_in_cord(x, y, z, 3)`*


## Вертолёты и самолёты (52)

- **326.** `wanted_are_swat_required()` → флаг — нужны ли SWAT.  *Пример: `wanted_are_swat_required()`*
- **329.** `wanted_num_helis_required()` → флаг — нужны ли вертолеты.  *Пример: `wanted_num_helis_required()`*
- **413.** `get_stat_property_destroyed()` → целое — разрушенное имущество.  *Пример: `get_stat_property_destroyed()`*
- **433.** `get_stat_property_budget()` → число — бюджет на собственность.  *Пример: `get_stat_property_budget()`*
- **439.** `get_stat_num_property_owned()` → целое — кол-во собственности.  *Пример: `get_stat_num_property_owned()`*
- **525.** `heli_set_orientation(авто, число)` → действие — транспорт/краны/зоны.  *Пример: `heli_set_orientation(car, x)`*
- **526.** `heli_clear_orientation(авто)` → действие — транспорт/краны/зоны.  *Пример: `heli_clear_orientation(car)`*
- **527.** `heli_set_stabiliser(авто, число)` → действие — транспорт/краны/зоны.  *Пример: `heli_set_stabiliser(car, x)`*
- **528.** `heli_make_crashing_down(авто)` → действие — транспорт/краны/зоны.  *Пример: `heli_make_crashing_down(car)`*
- **529.** `heli_is_player_in_any_heli()` → целое — транспорт/краны/зоны.  *Пример: `heli_is_player_in_any_heli()`*
- **530.** `heli_is_char_in_any_heli(пед)` → целое — транспорт/краны/зоны.  *Пример: `heli_is_char_in_any_heli(ped)`*
- **531.** `heli_switch(авто)` → действие — транспорт/краны/зоны.  *Пример: `heli_switch(car)`*
- **532.** `catalina_heli_start()` → действие — транспорт/краны/зоны.  *Пример: `catalina_heli_start()`*
- **533.** `catalina_heli_take_off()` → действие — транспорт/краны/зоны.  *Пример: `catalina_heli_take_off()`*
- **534.** `catalina_heli_remove()` → действие — транспорт/краны/зоны.  *Пример: `catalina_heli_remove()`*
- **535.** `catalina_heli_grab()` → действие — транспорт/краны/зоны.  *Пример: `catalina_heli_grab()`*
- **536.** `catalina_heli_fly_away()` → действие — транспорт/краны/зоны.  *Пример: `catalina_heli_fly_away()`*
- **537.** `catalina_heli_has_shot_down()` → целое — транспорт/краны/зоны.  *Пример: `catalina_heli_has_shot_down()`*
- **538.** `plane_goto_coords(авто, число, число, число)` → действие — транспорт/краны/зоны.  *Пример: `plane_goto_coords(car, x, y, z)`*
- **539.** `plane_is_player_in_any_plane()` → целое — транспорт/краны/зоны.  *Пример: `plane_is_player_in_any_plane()`*
- **540.** `plane_is_char_in_any_plane(пед)` → целое — транспорт/краны/зоны.  *Пример: `plane_is_char_in_any_plane(ped)`*
- **555.** `rc_give_remote_controlled_car_to_player()` → целое — транспорт/краны/зоны.  *Пример: `rc_give_remote_controlled_car_to_player()`*
- **556.** `rc_give_remote_controlled_model_to_player()` → целое — транспорт/краны/зоны.  *Пример: `rc_give_remote_controlled_model_to_player()`*
- **557.** `rc_get_remote_controlled_car()` → целое — транспорт/краны/зоны.  *Пример: `rc_get_remote_controlled_car()`*
- **558.** `rc_buggy_blow_up(авто)` → действие — транспорт/краны/зоны.  *Пример: `rc_buggy_blow_up(car)`*
- **559.** `rc_buggy_remove(авто)` → действие — транспорт/краны/зоны.  *Пример: `rc_buggy_remove(car)`*
- **560.** `rc_detonate_enable(авто, флаг)` → действие — транспорт/краны/зоны.  *Пример: `rc_detonate_enable(car, true)`*
- **561.** `rc_detonate_on_contact_enable(авто, флаг)` → действие — транспорт/краны/зоны.  *Пример: `rc_detonate_on_contact_enable(car, true)`*
- **562.** `player_is_in_remote_mode()` → целое — транспорт/краны/зоны.  *Пример: `player_is_in_remote_mode()`*
- **580.** `heli_goto_coords(авто, число, число, число)` → действие — вертолет на координаты.  *Пример: `heli_goto_coords(car, x, y, z)`*
- **581.** `heli_create_dust_effect(авто)` → действие — пыль катсценного вертолета.  *Пример: `heli_create_dust_effect(car)`*
- **622.** `is_car_in_air_proper(авто)` → целое — в воздухе точно?.  *Пример: `is_car_in_air_proper(car)`*
- **749.** `get_heli_forced_z(авто)` → число — принудительная высота вертолёта.  *Пример: `get_heli_forced_z(car)`*
- **750.** `set_heli_forced_z(авто, число)` → действие — уст. принудительная высота вертолёта.  *Пример: `set_heli_forced_z(car, x)`*
- **751.** `get_heli_searchlight_x(авто)` → число — координата X прожектора вертолёта.  *Пример: `get_heli_searchlight_x(car)`*
- **752.** `set_heli_searchlight_x(авто, число)` → действие — уст. координата X прожектора вертолёта.  *Пример: `set_heli_searchlight_x(car, x)`*
- **753.** `get_heli_searchlight_y(авто)` → число — координата Y прожектора вертолёта.  *Пример: `get_heli_searchlight_y(car)`*
- **754.** `set_heli_searchlight_y(авто, число)` → действие — уст. координата Y прожектора вертолёта.  *Пример: `set_heli_searchlight_y(car, x)`*
- **755.** `get_heli_searchlight_intensity(авто)` → число — яркость прожектора вертолёта.  *Пример: `get_heli_searchlight_intensity(car)`*
- **756.** `set_heli_searchlight_intensity(авто, число)` → действие — уст. яркость прожектора вертолёта.  *Пример: `set_heli_searchlight_intensity(car, x)`*
- **757.** `get_heli_rotor_angle(авто)` → число — угол вращения винта вертолёта.  *Пример: `get_heli_rotor_angle(car)`*
- **758.** `set_heli_rotor_angle(авто, число)` → действие — уст. угол вращения винта вертолёта.  *Пример: `set_heli_rotor_angle(car, x)`*
- **759.** `heli_send_down_swat(авто)` → действие — спецназ спускается с вертолёта на верёвке.  *Пример: `heli_send_down_swat(car)`*
- **760.** `heli_get_num_random()` → целое — число случайных вертолётов.  *Пример: `heli_get_num_random()`*
- **761.** `heli_set_num_random(число)` → действие — уст. число случайных вертолётов.  *Пример: `heli_set_num_random(x)`*
- **762.** `heli_is_catalina_on()` → флаг — вертолёт Катарины активен.  *Пример: `heli_is_catalina_on()`*
- **763.** `heli_set_catalina_on(флаг)` → действие — вкл/выкл вертолёт Катарины.  *Пример: `heli_set_catalina_on(true)`*
- **808.** `add_PropertyBudget(число)` → действие — бюджет на недвижимость.  *Пример: `add_PropertyBudget(x)`*
- **1372.** `heli_to_fly(авто, число, число, число, число)` → действие — вертолет летит на координаты.  *Пример: `heli_to_fly(car, x, y, z, 3)`*
- **1503.** `create_rope_on_cords_with_swat(число, число, число)` → действие — создать веревку на координатах с бойцом спецназа.  *Пример: `create_rope_on_cords_with_swat(x, y, z)`*
- **1504.** `create_rope_on_cords(число, число, число)` → 1 значения — создать веревку на координатах.  *Пример: `create_rope_on_cords(x, y, z)`*
- **1505.** `heli_change_height(авто, число, число)` → действие — изменить высоту вертолета.  *Пример: `heli_change_height(car, x, y)`*


## Мир: трафик, стриминг, зоны (50)

- **81.** `set_traffic(число)` → действие — установить плотность трафика транспорта.  *Пример: `set_traffic(x)`*
- **467.** `population_manage()` → действие — обработать популяцию.  *Пример: `population_manage()`*
- **468.** `population_remove_peds_full_pool()` → действие — удалить педов при переполнении.  *Пример: `population_remove_peds_full_pool()`*
- **469.** `population_convert_all_objects_to_dummy()` → действие — конвертировать объекты в dummy.  *Пример: `population_convert_all_objects_to_dummy()`*
- **470.** `population_is_female(число)` → флаг — модель педа женская.  *Пример: `population_is_female(x)`*
- **471.** `population_is_male(число)` → флаг — модель педа мужская.  *Пример: `population_is_male(x)`*
- **568.** `crane_activate(авто)` → действие — транспорт/краны/зоны.  *Пример: `crane_activate(car)`*
- **569.** `crane_is_carrying_car(авто)` → целое — транспорт/краны/зоны.  *Пример: `crane_is_carrying_car(car)`*
- **570.** `crane_is_targetting_car(авто)` → целое — транспорт/краны/зоны.  *Пример: `crane_is_targetting_car(car)`*
- **571.** `escalators_count()` → целое — транспорт/краны/зоны.  *Пример: `escalators_count()`*
- **572.** `zone_find_index(строка)` → целое — транспорт/краны/зоны.  *Пример: `zone_find_index("текст")`*
- **573.** `zone_get_level_from_position(число, число, число)` → целое — транспорт/краны/зоны.  *Пример: `zone_get_level_from_position(x, y, z)`*
- **574.** `crusher_crane_activate()` → действие — активировать пресс-кран.  *Пример: `crusher_crane_activate()`*
- **575.** `military_crane_activate()` → действие — активировать военный кран.  *Пример: `military_crane_activate()`*
- **576.** `crane_deactivate(авто)` → действие — деактивировать кран.  *Пример: `crane_deactivate(car)`*
- **577.** `crane_military_collected_all()` → целое — военный кран собрал все авто.  *Пример: `crane_military_collected_all()`*
- **578.** `crane_is_lifting_car(авто)` → целое — кран поднимает авто.  *Пример: `crane_is_lifting_car(car)`*
- **669.** `is_player_in_zone(строка)` → целое — игрок в зоне?.  *Пример: `is_player_in_zone("текст")`*
- **826.** `get_population_total_peds()` → число — всего педов в мире.  *Пример: `get_population_total_peds()`*
- **827.** `get_population_civ_peds()` → число — мирных педов.  *Пример: `get_population_civ_peds()`*
- **829.** `get_population_car_passengers()` → число — пассажиров в авто.  *Пример: `get_population_car_passengers()`*
- **830.** `get_population_mission_peds()` → число — миссионных педов.  *Пример: `get_population_mission_peds()`*
- **831.** `get_population_civ_male()` → число — мужчин-мирных.  *Пример: `get_population_civ_male()`*
- **832.** `get_population_civ_female()` → число — женщин-мирных.  *Пример: `get_population_civ_female()`*
- **833.** `get_population_max_in_use()` → число — макс. число используемых педов.  *Пример: `get_population_max_in_use()`*
- **834.** `get_population_countdown()` → число — обратный отсчёт до спавна педов.  *Пример: `get_population_countdown()`*
- **835.** `get_population_all_random_type()` → целое — тип всех случайных педов.  *Пример: `get_population_all_random_type()`*
- **836.** `set_population_all_random_type(число)` → действие — уст. тип всех случайных педов.  *Пример: `set_population_all_random_type(x)`*
- **883.** `get_streaming_disabled()` → флаг — стриминг отключён.  *Пример: `get_streaming_disabled()`*
- **884.** `set_streaming_disabled(флаг)` → действие — отключить стриминг моделей.  *Пример: `set_streaming_disabled(true)`*
- **885.** `get_streaming_models_requested()` → число — запрошено моделей.  *Пример: `get_streaming_models_requested()`*
- **886.** `get_streaming_priority_requests()` → число — приоритетных запросов.  *Пример: `get_streaming_priority_requests()`*
- **887.** `get_streaming_image_size()` → число — размер образа стриминга.  *Пример: `get_streaming_image_size()`*
- **888.** `get_streaming_buffer_size()` → число — размер буфера стриминга.  *Пример: `get_streaming_buffer_size()`*
- **889.** `get_streaming_current_ped_grp()` → число — текущая группа педов.  *Пример: `get_streaming_current_ped_grp()`*
- **890.** `get_streaming_loading_big_model()` → число — загружается большая модель.  *Пример: `get_streaming_loading_big_model()`*
- **891.** `get_streaming_peds_loaded()` → число — загружено педов.  *Пример: `get_streaming_peds_loaded()`*
- **892.** `get_streaming_channel_error()` → число — ошибка канала стриминга.  *Пример: `get_streaming_channel_error()`*
- **893.** `get_streaming_last_image_read()` → число — последний прочитанный образ.  *Пример: `get_streaming_last_image_read()`*
- **894.** `get_streaming_old_sector_x()` → число — старый сектор X.  *Пример: `get_streaming_old_sector_x()`*
- **895.** `get_streaming_old_sector_y()` → число — старый сектор Y.  *Пример: `get_streaming_old_sector_y()`*
- **896.** `get_zone_level(число, число, число)` → целое — остров по координатам.  *Пример: `get_zone_level(x, y, z)`*
- **897.** `find_information_zone(число, число, число)` → целое — индекс информационной зоны.  *Пример: `find_information_zone(x, y, z)`*
- **898.** `get_navigation_zone(число)` → целое — навигационная зона по индексу.  *Пример: `get_navigation_zone(x)`*
- **899.** `init_zones()` → действие — инициализировать зоны.  *Пример: `init_zones()`*
- **900.** `traffic_lights_for_peds()` → действие — светофор для пешеходов.  *Пример: `traffic_lights_for_peds()`*
- **901.** `traffic_scan_for_lights()` → действие — сканировать светофоры.  *Пример: `traffic_scan_for_lights()`*
- **902.** `traffic_should_car_stop_for_bridge()` → действие — машина должна остановиться у моста.  *Пример: `traffic_should_car_stop_for_bridge()`*
- **932.** `generate_emergency_services()` → действие — вызвать экстренные службы.  *Пример: `generate_emergency_services()`*
- **1371.** `set_ped_traffic(число)` → действие — установить плотноcть трафика педов.  *Пример: `set_ped_traffic(x)`*


## Задачи педа (47)

- **148.** `follow_the_leader(пед, указ.)` → действие — 01DE / 01DF	следовать за лидером.  *Пример: `follow_the_leader(ped, entity)`*
- **211.** `ped_restore_previous_objective(пед)` → действие — вернуть педу прошлую цель.  *Пример: `ped_restore_previous_objective(ped)`*
- **684.** `set_char_obj_wait(пед)` → действие — нет задач.  *Пример: `set_char_obj_wait(ped)`*
- **685.** `set_char_obj_flee_coord(пед, число, число, число)` → действие — убежать от точки пешком до безопасности.  *Пример: `set_char_obj_flee_coord(ped, x, y, z)`*
- **686.** `set_char_obj_guard_spot(пед, число, число, число)` → действие — охранять точку.  *Пример: `set_char_obj_guard_spot(ped, x, y, z)`*
- **687.** `set_char_obj_guard_area(пед, число, число, число, число)` → действие — охранять зону.  *Пример: `set_char_obj_guard_area(ped, x, y, z, 3)`*
- **688.** `set_char_obj_wait_in_car(пед, указ.)` → действие — ждать в машине.  *Пример: `set_char_obj_wait_in_car(ped, entity)`*
- **689.** `set_char_obj_leave_car(пед, указ.)` → действие — выйти из машины.  *Пример: `set_char_obj_leave_car(ped, entity)`*
- **690.** `set_char_obj_enter_car_passenger(пед, указ.)` → действие — сесть в авто как пассажир.  *Пример: `set_char_obj_enter_car_passenger(ped, entity)`*
- **691.** `set_char_obj_enter_car_driver(пед, указ.)` → действие — сесть в авто как водитель.  *Пример: `set_char_obj_enter_car_driver(ped, entity)`*
- **692.** `set_char_obj_follow_car_in_car(пед, указ.)` → действие — следовать за авто из авто.  *Пример: `set_char_obj_follow_car_in_car(ped, entity)`*
- **693.** `set_char_obj_destroy_object(пед, указ.)` → действие — уничтожить объект.  *Пример: `set_char_obj_destroy_object(ped, entity)`*
- **694.** `set_char_obj_goto_area_in_car(пед, число, число, число)` → действие — ехать к зоне.  *Пример: `set_char_obj_goto_area_in_car(ped, x, y, z)`*
- **695.** `set_char_obj_follow_car_foot_offset(пед, авто, число, число, число)` → действие — следовать за авто пешком смещение.  *Пример: `set_char_obj_follow_car_foot_offset(ped, car, x, y, z)`*
- **696.** `set_char_obj_guard_attack(пед, указ.)` → действие — охрана — атаковать противника.  *Пример: `set_char_obj_guard_attack(ped, entity)`*
- **697.** `set_char_obj_goto_coord_on_foot(пед, число, число, число)` → действие — идти к координатам пешком.  *Пример: `set_char_obj_goto_coord_on_foot(ped, x, y, z)`*
- **698.** `set_char_obj_goto_coord_in_car(пед, число, число, число)` → действие — ехать к координатам.  *Пример: `set_char_obj_goto_coord_in_car(ped, x, y, z)`*
- **699.** `set_char_obj_run_to_coord(пед, число, число, число)` → действие — бежать к координатам.  *Пример: `set_char_obj_run_to_coord(ped, x, y, z)`*
- **700.** `set_char_obj_goto_area_any_means(пед, число, число, число)` → действие — добираться любыми средствами.  *Пример: `set_char_obj_goto_area_any_means(ped, x, y, z)`*
- **701.** `set_char_obj_follow_formation(пед, указ.)` → действие — следовать в строю за лидером.  *Пример: `set_char_obj_follow_formation(ped, entity)`*
- **702.** `set_char_obj_hail_taxi(пед)` → действие — вызвать такси.  *Пример: `set_char_obj_hail_taxi(ped)`*
- **703.** `set_char_obj_steal_any_car(пед)` → действие — украсть любую машину.  *Пример: `set_char_obj_steal_any_car(ped)`*
- **704.** `set_char_obj_leave_any_car(пед)` → действие — выйти из любого авто.  *Пример: `set_char_obj_leave_any_car(ped)`*
- **705.** `set_char_obj_flee_car(пед, указ.)` → действие — убежать от машины.  *Пример: `set_char_obj_flee_car(ped, entity)`*
- **706.** `set_char_obj_walk_to_char(пед, пед)` → действие — идти к другому педу.  *Пример: `set_char_obj_walk_to_char(ped, ped)`*
- **707.** `set_char_obj_steal_any_car_mission(пед)` → действие — украсть любую миссионную машину.  *Пример: `set_char_obj_steal_any_car_mission(ped)`*
- **709.** `set_char_obj_goto_car_on_foot(пед, авто)` → действие — идти к машине пешком.  *Пример: `set_char_obj_goto_car_on_foot(ped, car)`*
- **731.** `set_char_obj_kill_char_on_foot(пед, пед)` → действие — убить педа пешком.  *Пример: `set_char_obj_kill_char_on_foot(ped, ped)`*
- **732.** `set_char_obj_kill_char_any_means(пед, пед)` → действие — убить педа любыми средствами.  *Пример: `set_char_obj_kill_char_any_means(ped, ped)`*
- **733.** `set_char_obj_flee_char_on_foot_till_safe(пед, пед)` → действие — убежать от педа до безопасного места.  *Пример: `set_char_obj_flee_char_on_foot_till_safe(ped, ped)`*
- **734.** `set_char_obj_flee_char_on_foot_always(пед, пед)` → действие — всегда убегать от педа.  *Пример: `set_char_obj_flee_char_on_foot_always(ped, ped)`*
- **735.** `set_char_obj_goto_char_on_foot(пед, пед)` → действие — идти к педу пешком.  *Пример: `set_char_obj_goto_char_on_foot(ped, ped)`*
- **737.** `set_char_obj_kill_player_on_foot(пед, указ.)` → действие — убить игрока пешком.  *Пример: `set_char_obj_kill_player_on_foot(ped, entity)`*
- **738.** `set_char_obj_kill_player_any_means(пед, указ.)` → действие — убить игрока любыми средствами.  *Пример: `set_char_obj_kill_player_any_means(ped, entity)`*
- **739.** `set_char_obj_flee_player_on_foot_till_safe(пед, указ.)` → действие — убежать от игрока до безопасного места.  *Пример: `set_char_obj_flee_player_on_foot_till_safe(ped, entity)`*
- **740.** `set_char_obj_flee_player_on_foot_always(пед, указ.)` → действие — всегда убегать от игрока.  *Пример: `set_char_obj_flee_player_on_foot_always(ped, entity)`*
- **741.** `set_char_obj_goto_player_on_foot(пед, указ.)` → действие — идти к игроку пешком.  *Пример: `set_char_obj_goto_player_on_foot(ped, entity)`*
- **742.** `set_char_obj_no_obj(пед)` → действие — снять задачу с педа.  *Пример: `set_char_obj_no_obj(ped)`*
- **744.** `set_char_obj_buy_ice_cream(пед)` → действие — купить мороженое.  *Пример: `set_char_obj_buy_ice_cream(ped)`*
- **745.** `set_char_obj_goto_area_on_foot(пед, число, число, число)` → действие — идти к зоне пешком.  *Пример: `set_char_obj_goto_area_on_foot(ped, x, y, z)`*
- **746.** `set_char_obj_destroy_car(пед, авто)` → действие — уничтожить машину.  *Пример: `set_char_obj_destroy_car(ped, car)`*
- **748.** `set_char_obj_follow_route(пед, число, число)` → действие — следовать по маршруту.  *Пример: `set_char_obj_follow_route(ped, x, y)`*
- **817.** `clear_char_follow_path(пед)` → действие — очистить маршрут педа.  *Пример: `clear_char_follow_path(ped)`*
- **820.** `char_set_idle(пед)` → действие — пед в режим ожидания.  *Пример: `char_set_idle(ped)`*
- **1328.** `ped_clear_objective(пед)` → действие — снять задачи с педа.  *Пример: `ped_clear_objective(ped)`*
- **1370.** `set_ped_in_car(указ., строка, число)` → указ. — установить педа в авто как водителя или пассажира.  *Пример: `set_ped_in_car(entity, "текст", x)`*
- **1435.** `set_ped_can_shot_in_car(пед, число)` → действие — водитель авто может получить пули.  *Пример: `set_ped_can_shot_in_car(ped, x)`*


## Статистика и деньги (45)

- **10.** `givemoney(число)` → действие — дать денег.  *Пример: `givemoney(x)`*
- **90.** `lockstatus(авто, число)` → действие — статус двери авто.  *Пример: `lockstatus(car, x)`*
- **135.** `set_wheel_status(авто, число, число)` → действие — установить состояния шин авто.  *Пример: `set_wheel_status(car, x, y)`*
- **181.** `get_ped_state(пед)` → целое — получить состояние педа.  *Пример: `get_ped_state(ped)`*
- **182.** `get_ped_last_state(пед)` → целое — получить прошлое состояние педа.  *Пример: `get_ped_last_state(ped)`*
- **183.** `set_ped_state(пед, число)` → действие — установить состояние педа.  *Пример: `set_ped_state(ped, x)`*
- **210.** `ped_restore_previous_state(пед)` → действие — вернуть педу прошлое состояние.  *Пример: `ped_restore_previous_state(ped)`*
- **221.** `ped_clear_wait_state(пед)` → действие — сбросить состояние ожидания.  *Пример: `ped_clear_wait_state(ped)`*
- **411.** `get_ped_stat_kills()` → целое — законные убийства игрока.  *Пример: `get_ped_stat_kills()`*
- **412.** `get_ped_stat_cheated()` → целое — число читов игрока.  *Пример: `get_ped_stat_cheated()`*
- **414.** `get_stat_longest_wheelie_dist()` → число — дистанция вилли.  *Пример: `get_stat_longest_wheelie_dist()`*
- **415.** `get_stat_longest_stoppie_dist()` → число — дистанция стоппи.  *Пример: `get_stat_longest_stoppie_dist()`*
- **416.** `get_stat_longest_2wheel_dist()` → число — дистанция на двух колесах.  *Пример: `get_stat_longest_2wheel_dist()`*
- **417.** `get_stat_longest_wheelie_time()` → целое — время вилли.  *Пример: `get_stat_longest_wheelie_time()`*
- **418.** `get_stat_seagulls_killed()` → целое — убитые чайки.  *Пример: `get_stat_seagulls_killed()`*
- **419.** `get_stat_times_drowned()` → целое — кол-во утоплений.  *Пример: `get_stat_times_drowned()`*
- **420.** `get_stat_flight_time()` → целое — время полетов.  *Пример: `get_stat_flight_time()`*
- **421.** `get_stat_missions_total()` → целое — общее число миссий.  *Пример: `get_stat_missions_total()`*
- **422.** `get_stat_frenzies_total()` → целое — общее число безумий.  *Пример: `get_stat_frenzies_total()`*
- **423.** `get_stat_frenzies_passed()` → целое — пройденные безумия.  *Пример: `get_stat_frenzies_passed()`*
- **424.** `get_stat_ice_cream_sold()` → число — проданное мороженое.  *Пример: `get_stat_ice_cream_sold()`*
- **425.** `get_stat_pizzas_delivered()` → число — доставленные пиццы.  *Пример: `get_stat_pizzas_delivered()`*
- **426.** `get_stat_assassinations()` → число — заказные убийства.  *Пример: `get_stat_assassinations()`*
- **427.** `get_stat_movie_stunts()` → число — кинотрюки.  *Пример: `get_stat_movie_stunts()`*
- **428.** `get_stat_stores_knocked_off()` → число — ограбленные магазины.  *Пример: `get_stat_stores_knocked_off()`*
- **429.** `get_stat_loan_sharks()` → число — сделки с ростовщиками.  *Пример: `get_stat_loan_sharks()`*
- **431.** `get_stat_weapon_budget()` → число — бюджет на оружие.  *Пример: `get_stat_weapon_budget()`*
- **432.** `get_stat_fashion_budget()` → число — бюджет на одежду.  *Пример: `get_stat_fashion_budget()`*
- **435.** `get_stat_bloodring_kills()` → целое — убийства на Bloodring.  *Пример: `get_stat_bloodring_kills()`*
- **436.** `get_stat_bloodring_time()` → целое — время на Bloodring.  *Пример: `get_stat_bloodring_time()`*
- **437.** `get_stat_shooting_rank()` → число — ранг стрельбы.  *Пример: `get_stat_shooting_rank()`*
- **438.** `get_stat_top_shooting_score()` → число — лучший счет в тире.  *Пример: `get_stat_top_shooting_score()`*
- **787.** `get_pedstat_m_fFleeDistance(пед)` → число — дистанция бегства педа.  *Пример: `get_pedstat_m_fFleeDistance(ped)`*
- **788.** `set_pedstat_m_fFleeDistance(пед, число)` → действие — уст. дистанция бегства педа.  *Пример: `set_pedstat_m_fFleeDistance(ped, x)`*
- **789.** `get_pedstat_m_fHeadingChangeRate(пед)` → число — скорость поворота педа.  *Пример: `get_pedstat_m_fHeadingChangeRate(ped)`*
- **790.** `set_pedstat_m_fHeadingChangeRate(пед, число)` → действие — уст. скорость поворота педа.  *Пример: `set_pedstat_m_fHeadingChangeRate(ped, x)`*
- **791.** `get_pedstat_m_fAttackStrength(пед)` → число — сила атаки педа.  *Пример: `get_pedstat_m_fAttackStrength(ped)`*
- **792.** `set_pedstat_m_fAttackStrength(пед, число)` → действие — уст. сила атаки педа.  *Пример: `set_pedstat_m_fAttackStrength(ped, x)`*
- **793.** `get_pedstat_m_fDefendWeakness(пед)` → число — слабость защиты педа.  *Пример: `get_pedstat_m_fDefendWeakness(ped)`*
- **794.** `set_pedstat_m_fDefendWeakness(пед, число)` → действие — уст. слабость защиты педа.  *Пример: `set_pedstat_m_fDefendWeakness(ped, x)`*
- **822.** `add_armour_to_player(число)` → действие — добавить брони игроку.  *Пример: `add_armour_to_player(x)`*
- **823.** `add_ammo_to_player(число, число)` → действие — добавить патронов игроку.  *Пример: `add_ammo_to_player(x, y)`*
- **910.** `give_player_goodies(число, число)` → флаг — выдать игроку бонусы по модели.  *Пример: `give_player_goodies(x, y)`*
- **1357.** `get_wheel_status(авто, число)` → 1 значения — получить состояния шины авто.  *Пример: `get_wheel_status(car, x)`*
- **1379.** `set_ped_stats_to(пед, число)` → действие — уст поведения педа.  *Пример: `set_ped_stats_to(ped, x)`*


## Оружие и патроны (40)

- **22.** `giveweaponped(пед, число, число, число)` → действие — дать педу оружие.  *Пример: `giveweaponped(ped, x, y, z)`*
- **153.** `Getweaponslot(пед, число)` → целое — получить номер слота по типу оружия.  *Пример: `Getweaponslot(ped, x)`*
- **155.** `get_aimgun_ped(пед)` → целое — получить показатель цели педа.  *Пример: `get_aimgun_ped(ped)`*
- **164.** `Remove_weapon_model(пед, число)` → действие — удалить оружие у педа.  *Пример: `Remove_weapon_model(ped, x)`*
- **180.** `set_clip_current_weapon(пед, число)` → действие — установить объём обоймы текущего оружие педа.  *Пример: `set_clip_current_weapon(ped, x)`*
- **222.** `ped_clear_point_gun_at(пед)` → действие — опустить наведенное оружие.  *Пример: `ped_clear_point_gun_at(ped)`*
- **289.** `get_weapon_range(пед)` → число — получить дальность оружия.  *Пример: `get_weapon_range(ped)`*
- **290.** `set_weapon_range(пед, число)` → действие — установить дальность оружия.  *Пример: `set_weapon_range(ped, x)`*
- **291.** `get_weapon_firing_rate(пед)` → целое — получить скорость стрельбы.  *Пример: `get_weapon_firing_rate(ped)`*
- **292.** `set_weapon_firing_rate(пед, число)` → действие — установить скорость стрельбы.  *Пример: `set_weapon_firing_rate(ped, x)`*
- **293.** `get_weapon_reload(пед)` → целое — получить время перезарядки.  *Пример: `get_weapon_reload(ped)`*
- **294.** `set_weapon_reload(пед, число)` → действие — установить время перезарядки.  *Пример: `set_weapon_reload(ped, x)`*
- **295.** `get_weapon_speed(пед)` → число — получить скорость пули.  *Пример: `get_weapon_speed(ped)`*
- **296.** `set_weapon_speed(пед, число)` → действие — установить скорость пули.  *Пример: `set_weapon_speed(ped, x)`*
- **297.** `get_weapon_spread(пед)` → число — получить разброс оружия.  *Пример: `get_weapon_spread(ped)`*
- **298.** `set_weapon_spread(пед, число)` → действие — установить разброс оружия.  *Пример: `set_weapon_spread(ped, x)`*
- **299.** `get_weapon_radius(пед)` → число — получить радиус поражения.  *Пример: `get_weapon_radius(ped)`*
- **300.** `set_weapon_radius(пед, число)` → действие — установить радиус поражения.  *Пример: `set_weapon_radius(ped, x)`*
- **301.** `get_weapon_lifespan(пед)` → число — получить время жизни снаряда.  *Пример: `get_weapon_lifespan(ped)`*
- **302.** `set_weapon_lifespan(пед, число)` → действие — установить время жизни снаряда.  *Пример: `set_weapon_lifespan(ped, x)`*
- **303.** `get_weapon_slot_info(пед)` → целое — получить слот оружия.  *Пример: `get_weapon_slot_info(ped)`*
- **304.** `get_ammo_in_clip_ped(пед)` → целое — получить патроны в обойме.  *Пример: `get_ammo_in_clip_ped(ped)`*
- **305.** `set_ammo_in_clip_ped(пед, число)` → действие — установить патроны в обойме.  *Пример: `set_ammo_in_clip_ped(ped, x)`*
- **306.** `get_weapon_state(пед)` → целое — получить состояние оружия.  *Пример: `get_weapon_state(ped)`*
- **307.** `weapon_reload_ped(пед)` → действие — перезарядить оружие педа.  *Пример: `weapon_reload_ped(ped)`*
- **308.** `weapon_is_melee(пед)` → флаг — оружие рукопашное.  *Пример: `weapon_is_melee(ped)`*
- **309.** `weapon_is_two_handed(пед)` → флаг — оружие двуручное.  *Пример: `weapon_is_two_handed(ped)`*
- **310.** `weapon_has_ammo_to_be_used(пед)` → флаг — можно использовать оружие.  *Пример: `weapon_has_ammo_to_be_used(ped)`*
- **357.** `get_far_clip()` → число — получить дальность прорисовки.  *Пример: `get_far_clip()`*
- **358.** `set_far_clip(число)` → действие — установить дальность прорисовки.  *Пример: `set_far_clip(x)`*
- **359.** `get_near_clip()` → число — получить ближнюю границу.  *Пример: `get_near_clip()`*
- **360.** `set_near_clip(число)` → действие — установить ближнюю границу.  *Пример: `set_near_clip(x)`*
- **676.** `fire_hunter_gun(авто)` → действие — выстрел Hunter.  *Пример: `fire_hunter_gun(car)`*
- **736.** `set_char_obj_aim_gun_at_char(пед, пед)` → действие — целиться из оружия в педа.  *Пример: `set_char_obj_aim_gun_at_char(ped, ped)`*
- **871.** `get_time_step_non_clipped()` → число — шаг времени без клиппинга.  *Пример: `get_time_step_non_clipped()`*
- **873.** `get_time_ms_non_clipped()` → целое — время в мс без клиппинга.  *Пример: `get_time_ms_non_clipped()`*
- **904.** `add_gun_flash_big(число, число, число, число, число, число)` → действие — вспышка выстрела крупного оружия.  *Пример: `add_gun_flash_big(x, y, z, 3, 4, 5)`*
- **1356.** `create_shots_on_ped(пед)` → действие — создать выстрелы.  *Пример: `create_shots_on_ped(ped)`*
- **1358.** `create_shots_on_car(авто, число, число)` → действие — создать выстрелы.  *Пример: `create_shots_on_car(car, x, y)`*
- **1383.** `set_camera_near_clip(число)` → действие — установить обрезку камеры.  *Пример: `set_camera_near_clip(x)`*


## Сообщения, текст, HUD (39)

- **12.** `printmessage(строка, число, число)` → действие — вывести сообщение на экран.  *Пример: `printmessage("текст", x, y)`*
- **23.** `ped_sprint_to_point(пед, число, число, число)` → действие — пед делает спринт к точке.  *Пример: `ped_sprint_to_point(ped, x, y, z)`*
- **70.** `show_text_styled(строка, число, число)` → действие — вывести игровой текст.  *Пример: `show_text_styled("текст", x, y)`*
- **112.** `print_front()` → действие — вывести особенный игровой текст.  *Пример: `print_front()`*
- **126.** `show_text_gtx(строка, число, число)` → действие — вывести игровой текст.  *Пример: `show_text_gtx("текст", x, y)`*
- **131.** `flash_hud(число)` → действие — Мигание элементов HUD.  *Пример: `flash_hud(x)`*
- **461.** `police_radio_message(число)` → действие — полицейское радио.  *Пример: `police_radio_message(x)`*
- **505.** `message_add_big(строка, число, число)` → действие — большое сообщение.  *Пример: `message_add_big("текст", x, y)`*
- **506.** `message_add_big_queue(строка, число, число)` → действие — большое сообщение в очередь.  *Пример: `message_add_big_queue("текст", x, y)`*
- **507.** `message_add(строка, число, число)` → действие — маленькое сообщение.  *Пример: `message_add("текст", x, y)`*
- **508.** `message_add_jump_queue(строка, число, число)` → действие — сообщение вне очереди.  *Пример: `message_add_jump_queue("текст", x, y)`*
- **509.** `message_add_with_number(строка, число, число, число)` → действие — сообщение с числом.  *Пример: `message_add_with_number("текст", x, y, z)`*
- **510.** `message_clear_all()` → действие — очистить все сообщения.  *Пример: `message_clear_all()`*
- **511.** `message_clear_small_only()` → действие — очистить маленькие сообщения.  *Пример: `message_clear_small_only()`*
- **512.** `message_clear_this_big_print(строка)` → действие — удалить большое сообщение.  *Пример: `message_clear_this_big_print("текст")`*
- **513.** `message_clear_this_print(строка)` → действие — удалить маленькое сообщение.  *Пример: `message_clear_this_print("текст")`*
- **514.** `font_set_style(число)` → действие — стиль шрифта.  *Пример: `font_set_style(x)`*
- **515.** `font_set_drop_shadow(число)` → действие — тень шрифта.  *Пример: `font_set_drop_shadow(x)`*
- **517.** `font_set_centre_size(число)` → действие — размер центр. текста.  *Пример: `font_set_centre_size(x)`*
- **518.** `font_set_wrapx(число)` → действие — ширина переноса шрифта.  *Пример: `font_set_wrapx(x)`*
- **519.** `font_set_prop_on()` → действие — пропорц. шрифт вкл.  *Пример: `font_set_prop_on()`*
- **520.** `font_set_prop_off()` → действие — пропорц. шрифт выкл.  *Пример: `font_set_prop_off()`*
- **521.** `font_set_background_on()` → действие — фон шрифта вкл.  *Пример: `font_set_background_on()`*
- **522.** `font_set_background_off()` → действие — фон шрифта выкл.  *Пример: `font_set_background_off()`*
- **523.** `font_set_right_justify_on()` → действие — выравнивание вправо вкл.  *Пример: `font_set_right_justify_on()`*
- **524.** `font_set_right_justify_off()` → действие — выравнивание вправо выкл.  *Пример: `font_set_right_justify_off()`*
- **708.** `set_char_obj_sprint_to_coord(пед, число, число, число)` → действие — спринт к координатам.  *Пример: `set_char_obj_sprint_to_coord(ped, x, y, z)`*
- **837.** `get_hud_m_WantedTimer()` → число — таймер розыска на HUD.  *Пример: `get_hud_m_WantedTimer()`*
- **838.** `set_hud_m_WantedTimer(число)` → действие — таймер розыска на HUD.  *Пример: `set_hud_m_WantedTimer(x)`*
- **839.** `get_hud_m_WeaponFadeTimer()` → число — таймер затухания оружия на HUD.  *Пример: `get_hud_m_WeaponFadeTimer()`*
- **840.** `set_hud_m_WeaponFadeTimer(число)` → действие — таймер затухания оружия на HUD.  *Пример: `set_hud_m_WeaponFadeTimer(x)`*
- **841.** `get_hud_m_EnergyLostFadeTimer()` → число — таймер затухания потери энергии.  *Пример: `get_hud_m_EnergyLostFadeTimer()`*
- **842.** `set_hud_m_EnergyLostFadeTimer(число)` → действие — таймер затухания потери энергии.  *Пример: `set_hud_m_EnergyLostFadeTimer(x)`*
- **843.** `get_hud_m_DisplayScoreFadeTimer()` → число — таймер затухания счёта.  *Пример: `get_hud_m_DisplayScoreFadeTimer()`*
- **844.** `set_hud_m_DisplayScoreFadeTimer(число)` → действие — таймер затухания счёта.  *Пример: `set_hud_m_DisplayScoreFadeTimer(x)`*
- **845.** `get_hud_wanted_state()` → целое — состояние розыска на HUD.  *Пример: `get_hud_wanted_state()`*
- **846.** `get_hud_help_message_state()` → целое — состояние подсказки на HUD.  *Пример: `get_hud_help_message_state()`*
- **1397.** `textbox(строка)` → действие — вывести сообщение в textbox.  *Пример: `textbox("текст")`*
- **1438.** `show_text_gtx_table(строка, строка, число, число)` → действие — вывести текст из таблицы.  *Пример: `show_text_gtx_table("текст", "текст", x, y)`*


## Розыск (Wanted) (38)

- **34.** `set_wanted(число)` → действие — уcтановить уровень розыска.  *Пример: `set_wanted(x)`*
- **37.** `clear_wanted()` → действие — убрать уровень розыска.  *Пример: `clear_wanted()`*
- **129.** `is_wanted_level(число)` → флаг — проверить уровень розыска.  *Пример: `is_wanted_level(x)`*
- **174.** `wanted_level()` → целое — получить уровень розыска.  *Пример: `wanted_level()`*
- **315.** `get_wanted_chaos_level()` → целое — получить очки хаоса.  *Пример: `get_wanted_chaos_level()`*
- **316.** `set_wanted_chaos_level(число)` → действие — установить очки хаоса.  *Пример: `set_wanted_chaos_level(x)`*
- **317.** `get_wanted_multiplier()` → число — получить множитель розыска.  *Пример: `get_wanted_multiplier()`*
- **318.** `set_wanted_multiplier(число)` → действие — установить множитель розыска.  *Пример: `set_wanted_multiplier(x)`*
- **319.** `get_cops_in_pursuit()` → целое — получить копов в погоне.  *Пример: `get_cops_in_pursuit()`*
- **320.** `get_max_cops_in_pursuit()` → целое — получить макс копов в погоне.  *Пример: `get_max_cops_in_pursuit()`*
- **321.** `set_max_cops_in_pursuit(число)` → действие — установить макс копов.  *Пример: `set_max_cops_in_pursuit(x)`*
- **324.** `get_chance_on_roadblock()` → целое — получить шанс блокпоста.  *Пример: `get_chance_on_roadblock()`*
- **325.** `set_chance_on_roadblock(число)` → действие — установить шанс блокпоста.  *Пример: `set_chance_on_roadblock(x)`*
- **327.** `wanted_are_fbi_required()` → флаг — нужны ли FBI.  *Пример: `wanted_are_fbi_required()`*
- **328.** `wanted_are_army_required()` → флаг — нужна ли армия.  *Пример: `wanted_are_army_required()`*
- **330.** `wanted_clear_crimes()` → действие — очистить очередь преступлений.  *Пример: `wanted_clear_crimes()`*
- **331.** `wanted_reset_police_pursuit()` → действие — сбросить полицейскую погоню.  *Пример: `wanted_reset_police_pursuit()`*
- **332.** `wanted_update_wanted_level()` → действие — обновить уровень розыска.  *Пример: `wanted_update_wanted_level()`*
- **333.** `wanted_set_no_drop(число)` → действие — уровень розыска без понижения.  *Пример: `wanted_set_no_drop(x)`*
- **334.** `wanted_cheat_level(число)` → действие — читерский уровень розыска.  *Пример: `wanted_cheat_level(x)`*
- **335.** `get_wanted_level_before_parole()` → целое — уровень розыска до пароля.  *Пример: `get_wanted_level_before_parole()`*
- **336.** `get_wanted_last_decreased_time()` → целое — время последнего понижения.  *Пример: `get_wanted_last_decreased_time()`*
- **499.** `gangs_set_attack_with_cops(число, флаг)` → действие — банда атакует с копами.  *Пример: `gangs_set_attack_with_cops(x, true)`*
- **500.** `gangs_get_attack_with_cops(число)` → флаг — банда атакует с копами.  *Пример: `gangs_get_attack_with_cops(x)`*
- **1303.** `get_wanted_m_nLastTimeWantedDecreased()` → целое — последнее снижение розыска.  *Пример: `get_wanted_m_nLastTimeWantedDecreased()`*
- **1304.** `get_wanted_m_nLastTimeWantedLevelChanged()` → целое — последняя смена уровня розыска.  *Пример: `get_wanted_m_nLastTimeWantedLevelChanged()`*
- **1305.** `get_wanted_m_dwTimeOfParole()` → целое — таймер условного освобождения.  *Пример: `get_wanted_m_dwTimeOfParole()`*
- **1306.** `set_wanted_m_dwTimeOfParole(число)` → действие — уст. таймер условного освобождения.  *Пример: `set_wanted_m_dwTimeOfParole(x)`*
- **1307.** `get_wanted_m_fMultiplier()` → число — множитель розыска.  *Пример: `get_wanted_m_fMultiplier()`*
- **1308.** `set_wanted_m_fMultiplier(число)` → действие — уст. множитель розыска.  *Пример: `set_wanted_m_fMultiplier(x)`*
- **1309.** `get_wanted_m_nCopsInPursuit()` → целое — копов в погоне.  *Пример: `get_wanted_m_nCopsInPursuit()`*
- **1310.** `get_wanted_m_nMaxCopsInPursuit()` → целое — макс. копов в погоне.  *Пример: `get_wanted_m_nMaxCopsInPursuit()`*
- **1311.** `set_wanted_m_nMaxCopsInPursuit(число)` → действие — уст. макс. копов в погоне.  *Пример: `set_wanted_m_nMaxCopsInPursuit(x)`*
- **1312.** `get_wanted_m_nMaxCopCarsInPursuit()` → целое — макс. машин копов в погоне.  *Пример: `get_wanted_m_nMaxCopCarsInPursuit()`*
- **1313.** `set_wanted_m_nMaxCopCarsInPursuit(число)` → действие — уст. макс. машин копов в погоне.  *Пример: `set_wanted_m_nMaxCopCarsInPursuit(x)`*
- **1314.** `get_wanted_m_nCopsBeatingSuspect()` → целое — копов избивают подозреваемого.  *Пример: `get_wanted_m_nCopsBeatingSuspect()`*
- **1316.** `set_gang_attack_player_with_cops(число, флаг)` → действие — банда атакует игрока с копами.  *Пример: `set_gang_attack_player_with_cops(x, true)`*
- **1382.** `set_cops_ignored(число)` → действие — установить игнор копов.  *Пример: `set_cops_ignored(x)`*


## Радар, маркеры, блипы (35)

- **29.** `create_marker_actor(пед)` → целое — создать маркер над педа.  *Пример: `create_marker_actor(ped)`*
- **30.** `removemarker(число)` → действие — удалить маркер.  *Пример: `removemarker(x)`*
- **46.** `remove_blip(число)` → действие — удалить метку с карты.  *Пример: `remove_blip(x)`*
- **47.** `createblip(число, число, число, число)` → целое — создать метку на карте.  *Пример: `createblip(x, y, z, 3)`*
- **68.** `draw_corona()` → действие — создать корону(гоночный чекпойнт).  *Пример: `draw_corona()`*
- **72.** `createmarker(число, число, число, число, число)` → целое — создать маркер на карте.  *Пример: `createmarker(x, y, z, 3, 4)`*
- **73.** `setsizemarker(число, число)` → действие — установить размер отображение маркера на карте.  *Пример: `setsizemarker(x, y)`*
- **82.** `create_marker_car(авто)` → целое — создать маркер над авто.  *Пример: `create_marker_car(car)`*
- **96.** `create_marker_obj(объект)` → целое — создать маркер над объектом.  *Пример: `create_marker_obj(obj)`*
- **144.** `star_mission_marker()` → действие — создать маркер для миссии.  *Пример: `star_mission_marker()`*
- **440.** `radar_set_coord_blip(число, число, число, число, число, число)` → целое — создать метку на карте.  *Пример: `radar_set_coord_blip(x, y, z, 3, 4, 5)`*
- **441.** `radar_set_short_range_blip(число, число, число, число, число, число)` → целое — короткая метка.  *Пример: `radar_set_short_range_blip(x, y, z, 3, 4, 5)`*
- **442.** `radar_set_entity_blip(число, пед, число, число)` → действие — метка на сущности.  *Пример: `radar_set_entity_blip(x, ped, y, z)`*
- **443.** `radar_clear_blip(число)` → действие — удалить метку.  *Пример: `radar_clear_blip(x)`*
- **444.** `radar_change_blip_colour(число, число)` → действие — цвет метки.  *Пример: `radar_change_blip_colour(x, y)`*
- **445.** `radar_change_blip_brightness(число, число)` → действие — яркость метки.  *Пример: `radar_change_blip_brightness(x, y)`*
- **446.** `radar_change_blip_scale(число, число)` → действие — размер метки.  *Пример: `radar_change_blip_scale(x, y)`*
- **447.** `radar_change_blip_display(число, число)` → действие — показ метки.  *Пример: `radar_change_blip_display(x, y)`*
- **448.** `radar_set_blip_sprite(число, число)` → действие — иконка метки.  *Пример: `radar_set_blip_sprite(x, y)`*
- **449.** `radar_show_marker(число, число, число, число, число)` → действие — показать маркер в мире.  *Пример: `radar_show_marker(x, y, z, 3, 4)`*
- **450.** `radar_set_marker_state(число, число)` → действие — состояние маркера миссии.  *Пример: `radar_set_marker_state(x, y)`*
- **451.** `radar_draw_radar_sprite(число, число, число, число)` → действие — отрисовать спрайт радара.  *Пример: `radar_draw_radar_sprite(x, y, z, 3)`*
- **452.** `radar_get_radar_range()` → число — дальность радара.  *Пример: `radar_get_radar_range()`*
- **453.** `radar_set_radar_range(число)` → действие — установить дальность радара.  *Пример: `radar_set_radar_range(x)`*
- **454.** `radar_clear_blip_for_entity(число, пед)` → действие — удалить метку сущности.  *Пример: `radar_clear_blip_for_entity(x, ped)`*
- **784.** `place_3d_marker(число, число, число, число, число, число, число, число, число, число, число, число, число)` → действие — 3D-маркер.  *Пример: `place_3d_marker(x, y, z, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)`*
- **785.** `place_3d_marker_set(число, число, число, число, число, число, число, число, число, число, число, число, число)` → действие — набор 3D-маркеров.  *Пример: `place_3d_marker_set(x, y, z, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)`*
- **786.** `get_num_active_3d_markers()` → целое — число активных 3D-маркеров.  *Пример: `get_num_active_3d_markers()`*
- **924.** `get_radar_range()` → число — дальность радара.  *Пример: `get_radar_range()`*
- **925.** `set_radar_range(число)` → действие — уст. дальность радара.  *Пример: `set_radar_range(x)`*
- **927.** `update_corona_coors(число, число, число, число, число, число)` → действие — обновить координаты короны.  *Пример: `update_corona_coors(x, y, z, 3, 4, 5)`*
- **936.** `get_num_bright_lights()` → целое — число ярких огней.  *Пример: `get_num_bright_lights()`*
- **1369.** `set_display_radar(число)` → действие — показать скрывать радар.  *Пример: `set_display_radar(x)`*
- **1375.** `setcolourmarker(число, число)` → действие — установить цвет маркера.  *Пример: `setcolourmarker(x, y)`*
- **1410.** `setmarker_brightness(число, флаг)` → действие — уст яркость маркера.  *Пример: `setmarker_brightness(x, true)`*


## Поиск и проверки (32)

- **1.** `findplayer()` → действие — найти педа.  *Пример: `findplayer()`*
- **28.** `is_current_weapon_ped(пед, число)` → флаг — проверить текущее оружие.  *Пример: `is_current_weapon_ped(ped, x)`*
- **61.** `findped()` → указ. — найти педа в пуле.  *Пример: `findped()`*
- **103.** `findcar(авто)` → указ. — Найти авто.  *Пример: `findcar(car)`*
- **109.** `find_road_for_car(число, число, число)` → число, число, число (X, Y, Z) — найти дорогу.  *Пример: `find_road_for_car(x, y, z)`*
- **163.** `is_targetting_in_ped(пед)` → флаг — игрок целиться в педа.  *Пример: `is_targetting_in_ped(ped)`*
- **343.** `is_lightning_flash()` → флаг — вспышка молнии.  *Пример: `is_lightning_flash()`*
- **354.** `is_time_in_range(число, число)` → флаг — время в диапазоне.  *Пример: `is_time_in_range(x, y)`*
- **494.** `is_user_pause()` → флаг — игра на паузе.  *Пример: `is_user_pause()`*
- **541.** `find_drug_plane_coords()` → число, число, число (X, Y, Z) — транспорт/краны/зоны.  *Пример: `find_drug_plane_coords()`*
- **542.** `find_drop_off_plane_coords()` → число, число, число (X, Y, Z) — транспорт/краны/зоны.  *Пример: `find_drop_off_plane_coords()`*
- **543.** `has_drug_plane_shot_down()` → целое — транспорт/краны/зоны.  *Пример: `has_drug_plane_shot_down()`*
- **544.** `has_drop_off_plane_shot_down()` → целое — транспорт/краны/зоны.  *Пример: `has_drop_off_plane_shot_down()`*
- **566.** `is_char_in_flying_vehicle(пед)` → целое — транспорт/краны/зоны.  *Пример: `is_char_in_flying_vehicle(ped)`*
- **567.** `is_player_in_flying_vehicle()` → целое — транспорт/краны/зоны.  *Пример: `is_player_in_flying_vehicle()`*
- **720.** `find_car_by_model(число)` → указ. — найти первое авто по модели.  *Пример: `find_car_by_model(x)`*
- **721.** `find_ped_by_model(число)` → указ. — найти первого педа по модели.  *Пример: `find_ped_by_model(x)`*
- **722.** `find_object_by_model(число)` → указ. — найти объект по модели.  *Пример: `find_object_by_model(x)`*
- **730.** `is_lightning_burst()` → флаг — вспышка молнии.  *Пример: `is_lightning_burst()`*
- **776.** `is_char_shooting_in_area(пед, число, число, число, число, флаг)` → флаг — пед стреляет в зоне.  *Пример: `is_char_shooting_in_area(ped, x, y, z, 3, true)`*
- **874.** `is_slow_motion_active()` → флаг — активен ли слоумоушен.  *Пример: `is_slow_motion_active()`*
- **1349.** `is_button_pressed(число)` → флаг — если спец клавиша нажата.  *Пример: `is_button_pressed(x)`*
- **1403.** `check_car_resray(число)` → флаг — проверить авто игрока было перекрашена в гараже.  *Пример: `check_car_resray(x)`*
- **1445.** `findped_in_cord(число, число, число, число)` → флаг, указ. — найти педа в радиусе с координатами.  *Пример: `findped_in_cord(x, y, z, 3)`*
- **1446.** `findcar_in_cord(число, число, число, число)` → флаг, указ. — найти авто в радиусе с координатами.  *Пример: `findcar_in_cord(x, y, z, 3)`*
- **1447.** `findobj_in_cord(число, число, число, число)` → флаг, указ. — найти объект в радиусе с координатами.  *Пример: `findobj_in_cord(x, y, z, 3)`*
- **1448.** `findped_nearest_in_cord(число, число, число)` → флаг, указ. — найти ближайшего педа в радиусе с координатами.  *Пример: `findped_nearest_in_cord(x, y, z)`*
- **1449.** `findcar_nearest_in_cord(число, число, число, число)` → флаг, указ. — найти ближайшее авто в радиусе с координатами.  *Пример: `findcar_nearest_in_cord(x, y, z, 3)`*
- **1450.** `findobj_nearest_in_cord(число, число, число)` → флаг, указ. — найти ближайший объект в радиусе с координатами.  *Пример: `findobj_nearest_in_cord(x, y, z)`*
- **1452.** `check_ped_in_cord(число, число, число, число)` → флаг — есть ли педа в радиусе с координатами.  *Пример: `check_ped_in_cord(x, y, z, 3)`*
- **1454.** `check_car_in_cord(число, число, число, число)` → флаг — есть ли авто в радиусе с координатами.  *Пример: `check_car_in_cord(x, y, z, 3)`*
- **1456.** `check_obj_in_cord(число, число, число, число)` → флаг — есть ли объект в радиусе с координатами.  *Пример: `check_obj_in_cord(x, y, z, 3)`*


## Транспорт: авто (старые) (31)

- **38.** `getcarhealth(авто)` → целое — получить кол-во здоровья авто.  *Пример: `getcarhealth(car)`*
- **39.** `setcarhealth(авто, число)` → действие — установить здоровье авто.  *Пример: `setcarhealth(car, x)`*
- **53.** `opendoorcar()` → действие — открыть дверь авто.  *Пример: `opendoorcar()`*
- **55.** `getcarcoordes(авто)` → число, число, число (X, Y, Z) — получить координаты авто.  *Пример: `getcarcoordes(car)`*
- **57.** `getcarcoordinates_on_x(авто, число)` → число, число, число (X, Y, Z) — Получить мировую координату по x для авто.  *Пример: `getcarcoordinates_on_x(car, x)`*
- **58.** `getcarcoordinates_on_y(авто, число)` → число, число, число (X, Y, Z) — Получить мировую координату по y для авто.  *Пример: `getcarcoordinates_on_y(car, x)`*
- **60.** `setdrivingstyle(авто, число)` → действие — установить стиль езды авто.  *Пример: `setdrivingstyle(car, x)`*
- **77.** `setcardrive(авто, число, число)` → указ. — установить водителя для авто.  *Пример: `setcardrive(car, x, y)`*
- **78.** `setcarpassenger(авто, число, число, целое)` → указ. — установить пассажира для авто.  *Пример: `setcarpassenger(car, x, y, 1)`*
- **79.** `setcarfirstcolor(авто, число)` → действие — установить первый цвет авто.  *Пример: `setcarfirstcolor(car, x)`*
- **80.** `setcarseconscolor(авто, число)` → действие — установить второй цвет авто.  *Пример: `setcarseconscolor(car, x)`*
- **99.** `setcaraction(авто, число, число)` → действие — установить поведение авто.  *Пример: `setcaraction(car, x, y)`*
- **104.** `setcartask(авто, число)` → действие — установить задачу авто.  *Пример: `setcartask(car, x)`*
- **105.** `setcarcoordes(авто, число, число, число)` → действие — установить координаты авто.  *Пример: `setcarcoordes(car, x, y, z)`*
- **110.** `setcarstrong(авто, число)` → действие — сделать авто устойчивым.  *Пример: `setcarstrong(car, x)`*
- **121.** `getcar_model(указ.)` → целое — получить модель авто.  *Пример: `getcar_model(entity)`*
- **122.** `setcarsiren(указ., число)` → действие — установить сирену для авто.  *Пример: `setcarsiren(entity, x)`*
- **168.** `Opendoorcar()` → действие — Макрос открыть все двери авто.  *Пример: `Opendoorcar()`*
- **1334.** `getcarfirstcolor(авто)` → целое — получить первый цвет авто.  *Пример: `getcarfirstcolor(car)`*
- **1335.** `getcarseconscolor(авто)` → целое — получить второй цвет авто.  *Пример: `getcarseconscolor(car)`*
- **1459.** `getcardimension(авто)` → число, число, число (X, Y, Z) — получить размер авто.  *Пример: `getcardimension(car)`*
- **1471.** `setcarrotate(авто, число, число, число)` → действие — уст вращения авто.  *Пример: `setcarrotate(car, x, y, z)`*
- **1472.** `getcar_turn_on_x(авто)` → число — Получить угол поворота авто по оси x.  *Пример: `getcar_turn_on_x(car)`*
- **1473.** `getcar_turn_on_y(авто)` → число — Получить угол поворота авто по оси y.  *Пример: `getcar_turn_on_y(car)`*
- **1474.** `getcar_turn_on_z(авто)` → число — Получить угол поворота авто по оси z.  *Пример: `getcar_turn_on_z(car)`*
- **1484.** `getcar_resistance(авто)` → число — Получить сопротивления авто трению.  *Пример: `getcar_resistance(car)`*
- **1488.** `setcar_resistance(авто, число)` → действие — Уст силу сопротивления авто трению.  *Пример: `setcar_resistance(car, x)`*
- **1489.** `getcar_turns_on_axes(авто)` → число, число, число (X, Y, Z) — получить углы авто поворота по осям.  *Пример: `getcar_turns_on_axes(car)`*
- **1490.** `setcar_at_on_x()` → действие — уст авто по оси x.  *Пример: `setcar_at_on_x()`*
- **1491.** `setcar_at_on_y()` → действие — уст авто по оси y.  *Пример: `setcar_at_on_y()`*
- **1492.** `setcar_at_on_z()` → действие — уст авто по оси z.  *Пример: `setcar_at_on_z()`*


## Камера и экран (31)

- **67.** `fade(число, число)` → действие — затенение, просветления.  *Пример: `fade(x, y)`*
- **127.** `camera_at_point(число, число, число, целое)` → действие — переместить камеру в координатах.  *Пример: `camera_at_point(x, y, z, 1)`*
- **128.** `restore_camera()` → действие — восстановить камеру.  *Пример: `restore_camera()`*
- **130.** `set_camera_position(число, число, число, число, число, число)` → действие — установить камеру в координатах.  *Пример: `set_camera_position(x, y, z, 3, 4, 5)`*
- **147.** `set_widescreen(флаг)` → действие — вкл/выкл широкий экран.  *Пример: `set_widescreen(true)`*
- **151.** `Getcameracoordes()` → число, число, число (X, Y, Z) — получить координаты камеры.  *Пример: `Getcameracoordes()`*
- **177.** `set_camera_and_point(число, число, число, число, число, число)` → действие — установить и переместить камеру в координатах.  *Пример: `set_camera_and_point(x, y, z, 3, 4, 5)`*
- **313.** `get_player_display_money()` → целое — получить отображаемые деньги.  *Пример: `get_player_display_money()`*
- **355.** `get_fov()` → число — получить угол обзора.  *Пример: `get_fov()`*
- **356.** `set_fov(число)` → действие — установить угол обзора.  *Пример: `set_fov(x)`*
- **361.** `get_lod_distance()` → число — получить дистанцию LOD.  *Пример: `get_lod_distance()`*
- **362.** `set_lod_distance(число)` → действие — установить дистанцию LOD.  *Пример: `set_lod_distance(x)`*
- **363.** `get_fade_color()` → целое — получить цвет затемнения.  *Пример: `get_fade_color()`*
- **364.** `set_fade_color(число, число, число)` → действие — установить цвет затемнения.  *Пример: `set_fade_color(x, y, z)`*
- **365.** `camera_shake(число, число, число, число)` → действие — тряхнуть камеру.  *Пример: `camera_shake(x, y, z, 3)`*
- **366.** `camera_draw_borders_widescreen()` → действие — границы широкоэкранного.  *Пример: `camera_draw_borders_widescreen()`*
- **367.** `camera_clear_player_weapon_mode()` → действие — сбросить режим оружия.  *Пример: `camera_clear_player_weapon_mode()`*
- **368.** `camera_calculate_derived_values()` → действие — пересчитать камеру.  *Пример: `camera_calculate_derived_values()`*
- **369.** `camera_dont_process_obbe_cinema()` → действие — отключить кино-камеру.  *Пример: `camera_dont_process_obbe_cinema()`*
- **370.** `get_mouse_accel_vertical()` → число — вертикальное ускорение мыши.  *Пример: `get_mouse_accel_vertical()`*
- **371.** `set_mouse_accel_vertical(число)` → действие — установить ускорение мыши.  *Пример: `set_mouse_accel_vertical(x)`*
- **372.** `get_mouse_accel_horizontal()` → число — горизонтальное ускорение мыши.  *Пример: `get_mouse_accel_horizontal()`*
- **373.** `set_mouse_accel_horizontal(число)` → действие — установить ускорение мыши.  *Пример: `set_mouse_accel_horizontal(x)`*
- **460.** `set_music_does_fade(флаг)` → действие — затухание музыки.  *Пример: `set_music_does_fade(true)`*
- **516.** `font_set_alpha_fade(число)` → действие — прозрачность шрифта.  *Пример: `font_set_alpha_fade(x)`*
- **848.** `get_player_death_fade_timer()` → целое — таймер затухания при смерти.  *Пример: `get_player_death_fade_timer()`*
- **1376.** `set_camera_on_car(авто, число, число)` → действие — уст камеру на авто.  *Пример: `set_camera_on_car(car, x, y)`*
- **1413.** `ispedscreen(пед)` → флаг — пед виден.  *Пример: `ispedscreen(ped)`*
- **1414.** `iscarscreen(авто)` → флаг — авто видно.  *Пример: `iscarscreen(car)`*
- **1415.** `isobjscreen(объект)` → флаг — объект виден.  *Пример: `isobjscreen(obj)`*
- **1434.** `is_sphere_onscreen(число, число, число, число)` → флаг — видна точка на радаре.  *Пример: `is_sphere_onscreen(x, y, z, 3)`*


## Погода, время, гравитация (31)

- **88.** `setclock(число, число, число, число)` → действие — задать время.  *Пример: `setclock(x, y, z, 3)`*
- **339.** `release_weather()` → действие — отпустить погоду.  *Пример: `release_weather()`*
- **340.** `get_old_weather_type()` → целое — получить прошлую погоду.  *Пример: `get_old_weather_type()`*
- **341.** `get_new_weather_type()` → целое — получить текущую погоду.  *Пример: `get_new_weather_type()`*
- **342.** `set_weather_type(число)` → действие — установить тип погоды.  *Пример: `set_weather_type(x)`*
- **345.** `get_game_hours()` → целое — получить часы.  *Пример: `get_game_hours()`*
- **346.** `get_game_minutes()` → целое — получить минуты.  *Пример: `get_game_minutes()`*
- **348.** `set_game_hours(число)` → действие — установить часы.  *Пример: `set_game_hours(x)`*
- **349.** `set_milliseconds_per_game_minute(число)` → действие — скорость хода времени.  *Пример: `set_milliseconds_per_game_minute(x)`*
- **350.** `get_milliseconds_per_game_minute()` → целое — получить скорость времени.  *Пример: `get_milliseconds_per_game_minute()`*
- **351.** `clock_store()` → действие — запомнить время.  *Пример: `clock_store()`*
- **352.** `clock_restore()` → действие — восстановить время.  *Пример: `clock_restore()`*
- **375.** `get_sun_blocked_by_clouds()` → флаг — солнце за облаками.  *Пример: `get_sun_blocked_by_clouds()`*
- **376.** `weather_add_rain()` → действие — добавить дождь.  *Пример: `weather_add_rain()`*
- **377.** `weather_add_stream_after_rain()` → действие — пар после дождя.  *Пример: `weather_add_stream_after_rain()`*
- **378.** `weather_render_rain_streaks()` → действие — отрисовать полосы дождя.  *Пример: `weather_render_rain_streaks()`*
- **472.** `population_is_sunbather(число)` → флаг — модель педа загорающий.  *Пример: `population_is_sunbather(x)`*
- **553.** `train_catch_char_obj(пед, флаг)` → действие — транспорт/краны/зоны.  *Пример: `train_catch_char_obj(ped, true)`*
- **554.** `train_catch_coll_obj(объект, флаг)` → действие — транспорт/краны/зоны.  *Пример: `train_catch_coll_obj(obj, true)`*
- **743.** `set_char_obj_catch_train(пед)` → действие — сесть на поезд.  *Пример: `set_char_obj_catch_train(ped)`*
- **876.** `get_clock_hours()` → целое — часы игрового таймера.  *Пример: `get_clock_hours()`*
- **877.** `get_clock_minutes()` → целое — минуты игрового таймера.  *Пример: `get_clock_minutes()`*
- **878.** `get_clock_seconds()` → целое — секунды игрового таймера.  *Пример: `get_clock_seconds()`*
- **879.** `get_clock_stored_hours()` → целое — сохранённые часы.  *Пример: `get_clock_stored_hours()`*
- **880.** `get_clock_stored_minutes()` → целое — сохранённые минуты.  *Пример: `get_clock_stored_minutes()`*
- **881.** `get_clock_minutes_until(число, число)` → целое — минут до заданного времени.  *Пример: `get_clock_minutes_until(x, y)`*
- **882.** `restore_clock()` → действие — восстановить сохранённое время.  *Пример: `restore_clock()`*
- **926.** `set_sun_blocked_by_clouds(флаг)` → действие — уст. солнце закрыто облаками.  *Пример: `set_sun_blocked_by_clouds(true)`*
- **1319.** `init_trains()` → действие — инициализировать поезда.  *Пример: `init_trains()`*
- **1320.** `update_trains()` → действие — обновить поезда.  *Пример: `update_trains()`*
- **1321.** `shutdown_trains()` → действие — завершить поезда.  *Пример: `shutdown_trains()`*


## Физика: скорость, гравитация, масса (29)

- **52.** `setcarspeed(авто, число)` → действие — установить скорость авто.  *Пример: `setcarspeed(car, x)`*
- **149.** `getcarspeed(авто)` → число — получить скорость авто.  *Пример: `getcarspeed(car)`*
- **337.** `force_weather(число)` → действие — плавно сменить погоду.  *Пример: `force_weather(x)`*
- **338.** `force_weather_now(число)` → действие — мгновенно сменить погоду.  *Пример: `force_weather_now(x)`*
- **344.** `force_rain(число)` → действие — установить силу дождя.  *Пример: `force_rain(x)`*
- **586.** `freeze_car_position(авто, флаг)` → действие — заморозить позицию.  *Пример: `freeze_car_position(car, true)`*
- **587.** `freeze_car_no_collide(авто, флаг)` → действие — заморозить без коллизий.  *Пример: `freeze_car_no_collide(car, true)`*
- **644.** `clear_car_dmg(авто)` → действие — очистить урон.  *Пример: `clear_car_dmg(car)`*
- **645.** `clear_car_wep_dmg(авто)` → действие — очистить урон оружием.  *Пример: `clear_car_wep_dmg(car)`*
- **648.** `add_upside_check(авто)` → действие — добавить проверку переворота.  *Пример: `add_upside_check(car)`*
- **659.** `set_ped_max_drive_speed(пед, число)` → действие — макс скорость.  *Пример: `set_ped_max_drive_speed(ped, x)`*
- **729.** `get_forced_weather_type()` → целое — принудительный тип погоды.  *Пример: `get_forced_weather_type()`*
- **764.** `get_ped_velocity(пед)` → число, число, число (X, Y, Z) — скорость педа `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `get_ped_velocity(ped)`*
- **765.** `set_ped_velocity(пед, число, число, число)` → действие — уст. скорость педа `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `set_ped_velocity(ped, x, y, z)`*
- **766.** `get_object_velocity(объект)` → число, число, число (X, Y, Z) — скорость объекта `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `get_object_velocity(obj)`*
- **767.** `set_object_velocity(объект, число, число, число)` → действие — уст. скорость объекта `m_vecMoveSpeed` (Движение скорость (вектор (X, Y, Z))).  *Пример: `set_object_velocity(obj, x, y, z)`*
- **769.** `get_ped_speed(пед)` → число — скалярная скорость педа.  *Пример: `get_ped_speed(ped)`*
- **770.** `get_object_speed(объект)` → число — скалярная скорость объекта.  *Пример: `get_object_speed(obj)`*
- **934.** `drag_car_to_point(авто, число, число, число)` → действие — тащить машину к точке.  *Пример: `drag_car_to_point(car, x, y, z)`*
- **935.** `find_max_speed_in_traffic(авто)` → число — макс. скорость машины в трафике.  *Пример: `find_max_speed_in_traffic(car)`*
- **1353.** `getgravity()` → число — получить значения гравитации.  *Пример: `getgravity()`*
- **1354.** `setgravity(число)` → действие — установить значения гравитации.  *Пример: `setgravity(x)`*
- **1355.** `gravity_default()` → действие — вернуть значения по умолчанию гравитации.  *Пример: `gravity_default()`*
- **1460.** `carmoveforce(авто, число, число, число)` → действие — двигать авто с силой.  *Пример: `carmoveforce(car, x, y, z)`*
- **1461.** `carturnforce(авто, число, число, число, число, число, число)` → действие — повернуть авто с силой.  *Пример: `carturnforce(car, x, y, z, 3, 4, 5)`*
- **1463.** `carturnfrictionforce(авто, число, число, число, число, число, число)` → действие — повернуть авто с силой трения.  *Пример: `carturnfrictionforce(car, x, y, z, 3, 4, 5)`*
- **1475.** `carmovespeed_on_x(авто, число)` → действие — двигать авто с скоростью по x.  *Пример: `carmovespeed_on_x(car, x)`*
- **1476.** `carmovespeed_on_y(авто, число)` → действие — двигать авто с скоростью по y.  *Пример: `carmovespeed_on_y(car, x)`*
- **1477.** `carmovespeed_on_z(авто, число)` → действие — двигать авто с скоростью по z.  *Пример: `carmovespeed_on_z(car, x)`*


## Поиск и проверки (старые) (25)

- **14.** `randomfindped(пед, число)` → флаг, указ. — найти случайнного педа.  *Пример: `randomfindped(ped, x)`*
- **17.** `availablemodel(число)` → флаг — проверка на загруженность модели.  *Пример: `availablemodel(x)`*
- **49.** `isped(пед)` → флаг — проверка это пед?.  *Пример: `isped(ped)`*
- **50.** `isvehicle(авто)` → флаг — проверка это транспорт?.  *Пример: `isvehicle(car)`*
- **54.** `randomfindcar(пед, число)` → флаг, указ. — Найти случайное авто.  *Пример: `randomfindcar(ped, x)`*
- **65.** `picked_up(число)` → флаг — проверка пикап подобран.  *Пример: `picked_up(x)`*
- **97.** `isobject(объект)` → флаг — проверка это объект?.  *Пример: `isobject(obj)`*
- **172.** `is_ped_in_car(пед)` → флаг — игрок в авто?.  *Пример: `is_ped_in_car(ped)`*
- **663.** `is_ped_in_police_car(пед)` → целое — в полицейском?.  *Пример: `is_ped_in_police_car(ped)`*
- **664.** `is_ped_touching_vehicle(пед)` → целое — касается?.  *Пример: `is_ped_touching_vehicle(ped)`*
- **665.** `is_ped_leaving_to_die(пед)` → целое — умирает из авто?.  *Пример: `is_ped_leaving_to_die(ped)`*
- **666.** `is_ped_drowning(пед)` → целое — тонет?.  *Пример: `is_ped_drowning(ped)`*
- **668.** `is_ped_in_zone(пед, строка)` → целое — в зоне?.  *Пример: `is_ped_in_zone(ped, "текст")`*
- **1323.** `Ped_in_targetting()` → флаг, указ. — Макрос пед на прицеле.  *Пример: `Ped_in_targetting()`*
- **1332.** `is_ped_in_this_car(пед, авто)` → флаг — игрок в этом авто?.  *Пример: `is_ped_in_this_car(ped, car)`*
- **1338.** `is_ped_objective(пед)` → флаг — если пед выполнил задачу.  *Пример: `is_ped_objective(ped)`*
- **1340.** `is_ped_in_air(пед)` → флаг — если пед в воздухе.  *Пример: `is_ped_in_air(ped)`*
- **1345.** `is_ped_passenger_car(пед, авто)` → флаг — пед пассажир авто.  *Пример: `is_ped_passenger_car(ped, car)`*
- **1346.** `is_ped_driver_car(пед, авто)` → флаг — пед пассажир авто.  *Пример: `is_ped_driver_car(ped, car)`*
- **1367.** `is_ped_duck(пед)` → флаг — Проверяет, пед присел.  *Пример: `is_ped_duck(ped)`*
- **1385.** `is_ped_incar(пед)` → флаг — пед в авто или нет?.  *Пример: `is_ped_incar(ped)`*
- **1389.** `is_ped_shooting(пед)` → флаг — пед стреляет?.  *Пример: `is_ped_shooting(ped)`*
- **1416.** `ispeddead(пед)` → флаг — пед мертв.  *Пример: `ispeddead(ped)`*
- **1417.** `iscardead(авто)` → флаг — авто уничтожено.  *Пример: `iscardead(car)`*
- **1426.** `isped_in_air(пед)` → флаг — пед в воздухе.  *Пример: `isped_in_air(ped)`*


## Водный транспорт (23)

- **33.** `car_in_water(авто)` → флаг — проверка авто в воде?.  *Пример: `car_in_water(car)`*
- **545.** `boat_goto_coords(авто, число, число, число)` → действие — транспорт/краны/зоны.  *Пример: `boat_goto_coords(car, x, y, z)`*
- **546.** `boat_set_cruise_speed(авто, число)` → действие — транспорт/краны/зоны.  *Пример: `boat_set_cruise_speed(car, x)`*
- **547.** `boat_anchor(авто, флаг)` → действие — транспорт/краны/зоны.  *Пример: `boat_anchor(car, true)`*
- **548.** `boat_start_foam_animation(авто)` → действие — транспорт/краны/зоны.  *Пример: `boat_start_foam_animation(car)`*
- **549.** `boat_update_foam_animation(авто)` → действие — транспорт/краны/зоны.  *Пример: `boat_update_foam_animation(car)`*
- **550.** `boat_is_player_in_any_boat()` → целое — транспорт/краны/зоны.  *Пример: `boat_is_player_in_any_boat()`*
- **551.** `boat_is_char_in_any_boat(пед)` → целое — транспорт/краны/зоны.  *Пример: `boat_is_char_in_any_boat(ped)`*
- **552.** `is_vehicle_boat(авто)` → целое — транспорт/краны/зоны.  *Пример: `is_vehicle_boat(car)`*
- **579.** `boat_stop(авто)` → действие — остановить катер.  *Пример: `boat_stop(car)`*
- **604.** `set_car_watertight(авто, флаг)` → действие — водонепроницаемость.  *Пример: `set_car_watertight(car, true)`*
- **638.** `is_car_in_water(авто)` → целое — в воде?.  *Пример: `is_car_in_water(car)`*
- **667.** `is_ped_in_water(пед)` → целое — в воде?.  *Пример: `is_ped_in_water(ped)`*
- **857.** `get_boat_m_fPropRotation(авто)` → число — вращение винта лодки.  *Пример: `get_boat_m_fPropRotation(car)`*
- **858.** `set_boat_m_fPropRotation(авто, число)` → действие — уст. вращение винта лодки.  *Пример: `set_boat_m_fPropRotation(car, x)`*
- **859.** `get_boat_m_fPropSpeed(авто)` → число — скорость винта лодки.  *Пример: `get_boat_m_fPropSpeed(car)`*
- **860.** `set_boat_m_fPropSpeed(авто, число)` → действие — уст. скорость винта лодки.  *Пример: `set_boat_m_fPropSpeed(car, x)`*
- **861.** `get_boat_m_fForcedZRotation(авто)` → число — принудительный поворот лодки по Z.  *Пример: `get_boat_m_fForcedZRotation(car)`*
- **862.** `set_boat_m_fForcedZRotation(авто, число)` → действие — уст. принудительный поворот лодки по Z.  *Пример: `set_boat_m_fForcedZRotation(car, x)`*
- **863.** `get_boat_m_fBoatGasPedal(авто)` → число — газ лодки.  *Пример: `get_boat_m_fBoatGasPedal(car)`*
- **864.** `set_boat_m_fBoatGasPedal(авто, число)` → действие — уст. газ лодки.  *Пример: `set_boat_m_fBoatGasPedal(car, x)`*
- **865.** `get_boat_m_fBoatSteeringLeftRight(авто)` → число — руль лодки.  *Пример: `get_boat_m_fBoatSteeringLeftRight(car)`*
- **866.** `set_boat_m_fBoatSteeringLeftRight(авто, число)` → действие — уст. руль лодки.  *Пример: `set_boat_m_fBoatSteeringLeftRight(car, x)`*


## Создание и удаление (22)

- **16.** `loadmodel(число)` → действие — загрузить модель.  *Пример: `loadmodel(x)`*
- **18.** `releasemodel(число)` → действие — удалить модель из памяти.  *Пример: `releasemodel(x)`*
- **20.** `createped(число, число, число, число, число)` → указ. — создать педа.  *Пример: `createped(x, y, z, 3, 4)`*
- **21.** `load_requested_models()` → действие — загрузить модель вне очереди.  *Пример: `load_requested_models()`*
- **32.** `remove_car(авто)` → действие — удалить авто.  *Пример: `remove_car(car)`*
- **36.** `create_sphere(число, число, число, число)` → целое — создать сферу.  *Пример: `create_sphere(x, y, z, 3)`*
- **40.** `remove_sphere(число)` → действие — удалить сферу.  *Пример: `remove_sphere(x)`*
- **41.** `remove_ped(пед)` → действие — удалить педа.  *Пример: `remove_ped(ped)`*
- **75.** `destroy()` → действие — удаления объектов из памяти при перезагрузки скрипта.  *Пример: `destroy()`*
- **92.** `create_obj()` → действие — создать объект.  *Пример: `create_obj()`*
- **93.** `remove_obj(объект)` → действие — удалить объект.  *Пример: `remove_obj(obj)`*
- **134.** `create_spec_ped(строка, число, число, число)` → указ. — создать спец педа.  *Пример: `create_spec_ped("текст", x, y, z)`*
- **137.** `remove_spec_ped(число)` → действие — удалить спец педа.  *Пример: `remove_spec_ped(x)`*
- **140.** `load_scene(число, число, число)` → действие — загрузить модели на координатах заранее.  *Пример: `load_scene(x, y, z)`*
- **152.** `remove_all_weapons_ped()` → действие — удалить все оружия педа.  *Пример: `remove_all_weapons_ped()`*
- **646.** `remove_stuck_check(авто)` → действие — убрать проверку застревания.  *Пример: `remove_stuck_check(car)`*
- **647.** `remove_upside_check(авто)` → действие — убрать проверку переворота.  *Пример: `remove_upside_check(car)`*
- **728.** `remove_weapon_from_char(пед, число)` → действие — удалить оружие у педа.  *Пример: `remove_weapon_from_char(ped, x)`*
- **1386.** `delete_entity(указ.)` → действие — удалить сущность сразу.  *Пример: `delete_entity(entity)`*
- **1398.** `remove_textbox()` → действие — удалить сообщение на textbox.  *Пример: `remove_textbox()`*
- **1407.** `create_newthread()` → действие — создания нового потока.  *Пример: `create_newthread()`*
- **1429.** `create_phone(число, число)` → целое — создать телефон на координатах.  *Пример: `create_phone(x, y)`*


## Повреждения и урон (19)

- **154.** `get_damage_weapon_ped(пед)` → целое — получить последний урон педа от оружия.  *Пример: `get_damage_weapon_ped(ped)`*
- **162.** `is_ped_damage_from_ped(пед, пед)` → флаг — получил ли пед урон от педа.  *Пример: `is_ped_damage_from_ped(ped, ped)`*
- **178.** `get_damage_current_weapon(пед)` → целое — получить уровень урона текущего оружие.  *Пример: `get_damage_current_weapon(ped)`*
- **179.** `set_damage_current_weapon(пед, число)` → действие — установить уровень урона текущего оружие педа.  *Пример: `set_damage_current_weapon(ped, x)`*
- **434.** `get_stat_auto_painting_budget()` → число — бюджет на покраску.  *Пример: `get_stat_auto_painting_budget()`*
- **677.** `set_tank_detonate(авто, флаг)` → действие — танк взрывает авто.  *Пример: `set_tank_detonate(car, true)`*
- **804.** `apply_car_damage(авто, число, число)` → действие — повредить компонент авто.  *Пример: `apply_car_damage(car, x, y)`*
- **805.** `fuck_car_completely(авто)` → действие — уничтожить авто визуально.  *Пример: `fuck_car_completely(car)`*
- **806.** `reset_car_damage(авто)` → действие — сбросить повреждения авто.  *Пример: `reset_car_damage(car)`*
- **818.** `clear_char_last_damage_entity(пед)` → действие — забыть последнюю повредившую сущность.  *Пример: `clear_char_last_damage_entity(ped)`*
- **819.** `clear_char_last_weapon_damage(пед)` → действие — забыть последнее оружие урона.  *Пример: `clear_char_last_weapon_damage(ped)`*
- **1329.** `setbomb_in_car(авто, число)` → действие — установить бомбу в авто.  *Пример: `setbomb_in_car(car, x)`*
- **1330.** `getbomb_in_car(авто)` → целое — получить тип бомбы в авто.  *Пример: `getbomb_in_car(car)`*
- **1331.** `activetbomb(авто)` → действие — активировать бомбу в авто.  *Пример: `activetbomb(car)`*
- **1392.** `setped_damaged_player(пед, флаг)` → действие — педу может нанести ущерб только игрок.  *Пример: `setped_damaged_player(ped, true)`*
- **1419.** `set_cars_damaged(число)` → действие — Все авто повреждены.  *Пример: `set_cars_damaged(x)`*
- **1423.** `set_ped_damaged_gang(пед, число, число)` → действие — уст педа уязвимым для членов банды.  *Пример: `set_ped_damaged_gang(ped, x, y)`*
- **1424.** `is_ped_damaged_weapon(пед, число)` → флаг — пед получает от определенного вида оружие.  *Пример: `is_ped_damaged_weapon(ped, x)`*
- **1443.** `getcardamage(авто)` → число — получить кол-во урона авто.  *Пример: `getcardamage(car)`*


## Огонь и взрывы (18)

- **85.** `create_explosion()` → действие — Создать взрыв на координатах.  *Пример: `create_explosion()`*
- **157.** `createfireonped(пед)` → целое — создать огонь на педе.  *Пример: `createfireonped(ped)`*
- **158.** `createfireoncar(авто)` → целое — создать огонь на авто.  *Пример: `createfireoncar(car)`*
- **159.** `createfireoncords(число, число, число)` → целое — создать огонь на координатах.  *Пример: `createfireoncords(x, y, z)`*
- **160.** `remove_fire(число)` → действие — удалить огонь.  *Пример: `remove_fire(x)`*
- **400.** `fire_find_nearest(число, число, число)` → указ., число — найти ближайший пожар.  *Пример: `fire_find_nearest(x, y, z)`*
- **401.** `fire_start_on_point(число, число, число, число, флаг)` → действие — поджечь точку.  *Пример: `fire_start_on_point(x, y, z, 3, true)`*
- **402.** `get_fire_time_to_burn(указ.)` → целое — время горения пожара.  *Пример: `get_fire_time_to_burn(entity)`*
- **403.** `set_fire_time_to_burn(указ., число)` → действие — установить время горения.  *Пример: `set_fire_time_to_burn(entity, x)`*
- **404.** `get_fire_target(указ.)` → указ. — цель пожара.  *Пример: `get_fire_target(entity)`*
- **405.** `fire_is_active(указ.)` → флаг — пожар активен.  *Пример: `fire_is_active(entity)`*
- **406.** `get_fire_position(указ.)` → число, число, число (X, Y, Z) — координаты пожара.  *Пример: `get_fire_position(entity)`*
- **462.** `create_explosion_no_sound(число, число, число, число)` → действие — взрыв без звука.  *Пример: `create_explosion_no_sound(x, y, z, 3)`*
- **479.** `world_extinguish_all_car_fires_in_area(число, число, число, число)` → действие — потушить пожары в области.  *Пример: `world_extinguish_all_car_fires_in_area(x, y, z, 3)`*
- **563.** `set_fade_jumpcut_after_rc_explosion(флаг)` → действие — транспорт/краны/зоны.  *Пример: `set_fade_jumpcut_after_rc_explosion(true)`*
- **639.** `fire_button_pressed()` → целое — кнопка огня?.  *Пример: `fire_button_pressed()`*
- **747.** `set_char_obj_fire_at_object_from_vehicle(пед, объект)` → действие — стрелять по объекту из авто.  *Пример: `set_char_obj_fire_at_object_from_vehicle(ped, obj)`*
- **1436.** `iscarfire(авто)` → флаг — авто горит?.  *Пример: `iscarfire(car)`*


## Педы (старые) (17)

- **2.** `setpedhealth(пед, число)` → действие — установить здоровье педа.  *Пример: `setpedhealth(ped, x)`*
- **3.** `setarmour(пед, число)` → действие — установить броню педа.  *Пример: `setarmour(ped, x)`*
- **5.** `getpedhealth(пед)` → целое — получить здоровье педа.  *Пример: `getpedhealth(ped)`*
- **8.** `getpedcoordinates_on_x(пед, число)` → число, число, число (X, Y, Z) — Получить мировую координату по x для педа.  *Пример: `getpedcoordinates_on_x(ped, x)`*
- **9.** `getpedcoordinates_on_y(пед, число)` → число, число, число (X, Y, Z) — Получить мировую координату по y для педа.  *Пример: `getpedcoordinates_on_y(ped, x)`*
- **13.** `getpedcoordes(пед)` → число, число, число (X, Y, Z) — получить координаты.  *Пример: `getpedcoordes(ped)`*
- **25.** `kill_ped_on_foot(пед, пед)` → действие — убить педа пешком.  *Пример: `kill_ped_on_foot(ped, ped)`*
- **26.** `kill_char_any_means(пед, пед)` → действие — убить педа любыми средствами.  *Пример: `kill_char_any_means(ped, ped)`*
- **31.** `setpedcoordes(пед, число, число, число)` → действие — установить координаты для педа.  *Пример: `setpedcoordes(ped, x, y, z)`*
- **42.** `kill_ped(пед)` → действие — убить педа.  *Пример: `kill_ped(ped)`*
- **87.** `player_defined(пед)` → флаг — пед существует?.  *Пример: `player_defined(ped)`*
- **89.** `arrested()` → флаг — пед арестован?.  *Пример: `arrested()`*
- **1341.** `kill_peds_in_car(авто)` → действие — убить педов в авто.  *Пример: `kill_peds_in_car(car)`*
- **1342.** `getstamina_ped(пед)` → число — получить выносливость педа.  *Пример: `getstamina_ped(ped)`*
- **1343.** `setstamina_ped(пед, число)` → действие — установить выносливость педа.  *Пример: `setstamina_ped(ped, x)`*
- **1384.** `setpedcrouch(пед, флаг, число)` → действие — пед сел.  *Пример: `setpedcrouch(ped, true, x)`*
- **1412.** `setpednode_seek(пед, флаг)` → действие — пед игнорирорует пути педов.  *Пример: `setpednode_seek(ped, true)`*


## Снаряды и следы пуль (16)

- **710.** `get_projectile_count()` → целое — число активных снарядов.  *Пример: `get_projectile_count()`*
- **711.** `get_projectile_weapon(число)` → целое — тип оружия снаряда.  *Пример: `get_projectile_weapon(x)`*
- **712.** `get_projectile_source(число)` → указ. — источник снаряда.  *Пример: `get_projectile_source(x)`*
- **713.** `get_projectile_pos(число)` → число, число, число (X, Y, Z) — позиция снаряда x,y,z.  *Пример: `get_projectile_pos(x)`*
- **714.** `set_projectile_pos(число, число, число, число)` → действие — уст позицию снаряда.  *Пример: `set_projectile_pos(x, y, z, 3)`*
- **715.** `is_projectile_in_use(число)` → флаг — слот снаряда занят.  *Пример: `is_projectile_in_use(x)`*
- **716.** `remove_all_projectiles()` → действие — удалить все снаряды.  *Пример: `remove_all_projectiles()`*
- **717.** `is_projectile_in_area(число, число, число, число, число, число)` → флаг — снаряд в зоне.  *Пример: `is_projectile_in_area(x, y, z, 3, 4, 5)`*
- **718.** `destroy_projectiles_in_area(число, число, число, число, число, число)` → действие — уничтожить снаряды в зоне.  *Пример: `destroy_projectiles_in_area(x, y, z, 3, 4, 5)`*
- **719.** `add_projectile(указ., число, число, число, число, число)` → флаг — создать снаряд.  *Пример: `add_projectile(entity, x, y, z, 3, 4)`*
- **778.** `add_bullet_trace_radius(число, число, число, число, число, число, число, число, число)` → действие — след пули: от точки к точке.  *Пример: `add_bullet_trace_radius(x, y, z, 3, 4, 5, 6, 7, 8)`*
- **779.** `add_bullet_trace_weapon(число, число, число, число, число, число, число, указ.)` → действие — след пули: оружием и сущностью.  *Пример: `add_bullet_trace_weapon(x, y, z, 3, 4, 5, 6, entity)`*
- **780.** `get_bullet_trace_count()` → целое — число активных следов пуль.  *Пример: `get_bullet_trace_count()`*
- **781.** `get_bullet_trace_origin(число)` → число, число, число (X, Y, Z) — точка начала следа пули.  *Пример: `get_bullet_trace_origin(x)`*
- **782.** `get_bullet_trace_target(число)` → число, число, число (X, Y, Z) — точка конца следа пули.  *Пример: `get_bullet_trace_target(x)`*
- **783.** `clear_bullet_traces()` → действие — удалить все следы пуль.  *Пример: `clear_bullet_traces()`*


## Двери, замки, сигнализация (14)

- **649.** `unlock_doors_area(авто)` → действие — открыть двери.  *Пример: `unlock_doors_area(car)`*
- **650.** `close_all_doors(авто)` → действие — закрыть все двери.  *Пример: `close_all_doors(car)`*
- **651.** `close_car_boot(авто)` → действие — закрыть багажник.  *Пример: `close_car_boot(car)`*
- **652.** `pop_car_boot(авто)` → действие — открыть багажник.  *Пример: `pop_car_boot(car)`*
- **653.** `pop_car_boot_physics(авто)` → действие — открыть физикой.  *Пример: `pop_car_boot_physics(car)`*
- **654.** `lock_doors(авто, флаг)` → действие — запереть двери.  *Пример: `lock_doors(car, true)`*
- **655.** `change_car_lock(авто, число)` → действие — сменить замок.  *Пример: `change_car_lock(car, x)`*
- **674.** `switch_car_weapons(авто, флаг)` → действие — переключить оружие.  *Пример: `switch_car_weapons(car, true)`*
- **679.** `arm_car_bomb(авто, число)` → действие — установить бомбу.  *Пример: `arm_car_bomb(car, x)`*
- **680.** `disarm_car_bomb(авто)` → действие — обезвредить.  *Пример: `disarm_car_bomb(car)`*
- **681.** `give_car_alarm(авто)` → действие — сигнализация.  *Пример: `give_car_alarm(car)`*
- **682.** `switch_car_siren(авто, флаг)` → действие — сирена.  *Пример: `switch_car_siren(car, true)`*
- **683.** `set_driving_style_op(авто, число)` → действие — стиль вождения.  *Пример: `set_driving_style_op(car, x)`*
- **903.** `should_car_stop_for_light(авто, флаг)` → действие — должна ли машина остановиться на свет.  *Пример: `should_car_stop_for_light(car, true)`*


## Общее и системное (13)

- **101.** `move_rotate(объект, число, число, целое)` → действие — вращать объект.  *Пример: `move_rotate(obj, x, y, 1)`*
- **111.** `putincar(пед, авто)` → действие — переместить педа в авто.  *Пример: `putincar(ped, car)`*
- **136.** `set_skin(пед, строка)` → действие — установить скин педа.  *Пример: `set_skin(ped, "текст")`*
- **138.** `go_to_route(указ., таблица, ...)` → действие — установить маршрут авто.  *Пример: `go_to_route(entity, val, ...)`*
- **139.** `add_stuck_car_check(авто, число, число)` → действие — условия для того, чтобы авто считалась застрявшей.  *Пример: `add_stuck_car_check(car, x, y)`*
- **143.** `get_current_name_luascript()` → указ. — получить имя текущего lua файла.  *Пример: `get_current_name_luascript()`*
- **150.** `newthread(LUA_TFUNCTION)` → действие — запуск функции в новом потоке.  *Пример: `newthread(val)`*
- **1378.** `select_interiour(число)` → действие — уcтановить интерьер.  *Пример: `select_interiour(x)`*
- **1387.** `clean_leader(пед)` → действие — перестать следовать за лидером.  *Пример: `clean_leader(ped)`*
- **1388.** `show_save_menu()` → действие — показать меню сохранения.  *Пример: `show_save_menu()`*
- **1395.** `clean_ped_wait(пед)` → действие — пед больше не ждет.  *Пример: `clean_ped_wait(ped)`*
- **1401.** `set_riot_intensity(число)` → действие — уст уровень шума беспорядков.  *Пример: `set_riot_intensity(x)`*
- **1428.** `clean_threat_for_ped_type(число, число)` → действие — Убрать враждебность типа педа к другим типам педа.  *Пример: `clean_threat_for_ped_type(x, y)`*


## Математика (12)

- **6.** `getpedangle(пед)` → целое — получить угол педа.  *Пример: `getpedangle(ped)`*
- **71.** `setcarangle(авто, число)` → действие — установить угол авто.  *Пример: `setcarangle(car, x)`*
- **98.** `setpedangle(пед, число)` → действие — установить угол педа.  *Пример: `setpedangle(ped, x)`*
- **102.** `getobjangle(объект)` → число — получить угол объекта.  *Пример: `getobjangle(obj)`*
- **484.** `general_atan_of_xy(число, число)` → число — арктангенс от x,y.  *Пример: `general_atan_of_xy(x, y)`*
- **485.** `general_angle_between_points(число, число, число, число)` → число — угол между точками.  *Пример: `general_angle_between_points(x, y, z, 3)`*
- **486.** `general_radian_angle_between_points(число, число, число, число)` → число — угол между точками в радианах.  *Пример: `general_radian_angle_between_points(x, y, z, 3)`*
- **487.** `general_limit_angle(число)` → число — нормализовать угол.  *Пример: `general_limit_angle(x)`*
- **488.** `general_limit_radian_angle(число)` → число — нормализовать угол в радианах.  *Пример: `general_limit_radian_angle(x)`*
- **489.** `general_node_heading_from_vector(число, число)` → целое — направление из вектора.  *Пример: `general_node_heading_from_vector(x, y)`*
- **1406.** `getcarangle(авто)` → целое — получить угол авто.  *Пример: `getcarangle(car)`*
- **1458.** `setobjangle(объект, число)` → действие — уст угол объекта.  *Пример: `setobjangle(obj, x)`*


## Время и таймер (12)

- **113.** `star_timer()` → действие — включить таймер.  *Пример: `star_timer()`*
- **115.** `timer_donw(число, строка, число)` → действие — таймер на уменьшение.  *Пример: `timer_donw(x, "текст", y)`*
- **347.** `get_game_seconds()` → целое — получить секунды.  *Пример: `get_game_seconds()`*
- **353.** `get_time_until(число, число)` → целое — минуты до указанного времени.  *Пример: `get_time_until(x, y)`*
- **490.** `get_timer_ms()` → целое — игровое время в мс.  *Пример: `get_timer_ms()`*
- **491.** `get_timer_ms_pause_mode()` → целое — время с учетом паузы.  *Пример: `get_timer_ms_pause_mode()`*
- **493.** `get_time_step()` → число — шаг времени кадра.  *Пример: `get_time_step()`*
- **872.** `get_previous_time_ms()` → целое — предыдущее время в мс.  *Пример: `get_previous_time_ms()`*
- **875.** `get_cycles_per_millisecond()` → целое — циклы на миллисекунду.  *Пример: `get_cycles_per_millisecond()`*
- **1337.** `set_timer_bomb_car(авто, число)` → действие — установить таймер на взрыв бомбы в авто.  *Пример: `set_timer_bomb_car(car, x)`*
- **1339.** `set_time_scale(число)` → действие — установить ход времени в игре.  *Пример: `set_time_scale(x)`*
- **1381.** `set_timer_ped_attack(пед, число)` → действие — установить таймер атаки педа.  *Пример: `set_timer_ped_attack(ped, x)`*


## Звук (11)

- **48.** `play_sound(число)` → действие — проиграть мелодию.  *Пример: `play_sound(x)`*
- **69.** `sound_coordinate(число, число, число, число)` → действие — Проиграть звук в координатах.  *Пример: `sound_coordinate(x, y, z, 3)`*
- **86.** `set_status_engine(авто, число)` → действие — установить состояние двигателя авто.  *Пример: `set_status_engine(car, x)`*
- **132.** `set_radio(число, число)` → действие — установить радио.  *Пример: `set_radio(x, y)`*
- **455.** `play_one_off_sound(число, число, число, число)` → действие — одноразовый звук.  *Пример: `play_one_off_sound(x, y, z, 3)`*
- **456.** `play_continuous_sound(число, число, число, число)` → целое — непрерывный звук.  *Пример: `play_continuous_sound(x, y, z, 3)`*
- **457.** `remove_sound_by_id(число)` → действие — удалить звук.  *Пример: `remove_sound_by_id(x)`*
- **458.** `set_radio_channel(число)` → действие — радиостанция игроку.  *Пример: `set_radio_channel(x)`*
- **459.** `get_radio_channel()` → целое — текущая радиостанция.  *Пример: `get_radio_channel()`*
- **1333.** `ishorncar(авто)` → флаг — проверить гудит ли авто.  *Пример: `ishorncar(car)`*
- **1344.** `get_radio_car(авто)` → целое — получить текущую радиостанцию авто.  *Пример: `get_radio_car(car)`*


## Игрок: состояние и свойства (11)

- **314.** `set_player_drive_by(флаг)` → действие — стрельба на ходу вкл/выкл.  *Пример: `set_player_drive_by(true)`*
- **465.** `get_all_random_peds_type()` → целое — тип случайных педов.  *Пример: `get_all_random_peds_type()`*
- **466.** `set_all_random_peds_type(число)` → действие — установить тип случайных педов.  *Пример: `set_all_random_peds_type(x)`*
- **775.** `set_player_fast_reload(флаг)` → действие — быстрая перезарядка игроку.  *Пример: `set_player_fast_reload(true)`*
- **847.** `get_player_taxi_timer()` → целое — таймер такси игрока.  *Пример: `get_player_taxi_timer()`*
- **849.** `get_player_2wheels_time()` → целое — время на двух колёсах.  *Пример: `get_player_2wheels_time()`*
- **850.** `get_player_2wheels_distance()` → число — дистанция на двух колёсах.  *Пример: `get_player_2wheels_distance()`*
- **851.** `get_player_road_density()` → число — плотность дорог вокруг игрока.  *Пример: `get_player_road_density()`*
- **852.** `set_player_road_density(число)` → действие — уст. плотность дорог вокруг игрока.  *Пример: `set_player_road_density(x)`*
- **1391.** `set_player_mod(число, число)` → действие — уст настроения игрока.  *Пример: `set_player_mod(x, y)`*
- **1427.** `set_threat_for_ped_type(число, число)` → действие — уст враждебность типа педа к другим типам педа.  *Пример: `set_threat_for_ped_type(x, y)`*


## Системное и потоки (11)

- **374.** `get_use_mouse_3rd_person()` → флаг — мышь в третьем лице.  *Пример: `get_use_mouse_3rd_person()`*
- **824.** `clear_area_of_cars(число, число, число, число)` → действие — очистить зону от машин.  *Пример: `clear_area_of_cars(x, y, z, 3)`*
- **825.** `clear_area_of_chars(число, число, число, число)` → действие — очистить зону от педов.  *Пример: `clear_area_of_chars(x, y, z, 3)`*
- **905.** `initialise_weapons()` → действие — инициализировать оружие.  *Пример: `initialise_weapons()`*
- **906.** `shutdown_weapons()` → действие — завершить работу оружия.  *Пример: `shutdown_weapons()`*
- **907.** `update_weapons()` → действие — обновить оружие.  *Пример: `update_weapons()`*
- **908.** `do_doom_aiming(указ., число, число, число, число, число, число)` → действие — аим-траверс для оружия.  *Пример: `do_doom_aiming(entity, x, y, z, 3, 4, 5)`*
- **913.** `do_mine_effects(указ.)` → действие — эффект мины.  *Пример: `do_mine_effects(entity)`*
- **931.** `choose_police_car_model()` → целое — выбрать модель полицейской машины.  *Пример: `choose_police_car_model()`*
- **933.** `clear_interesting_vehicles()` → действие — очистить список интересных машин.  *Пример: `clear_interesting_vehicles()`*
- **1318.** `register_motion_streak(число, число, число, число, число, число, число, число, число, число)` → действие — размытие-полоса.  *Пример: `register_motion_streak(x, y, z, 3, 4, 5, 6, 7, 8, 9)`*


## Мир: геометрия и очистка (11)

- **473.** `world_find_ground_z_for_coord(число, число)` → число — высота земли под точкой.  *Пример: `world_find_ground_z_for_coord(x, y)`*
- **474.** `world_find_ground_z_for_3d_coord(число, число, число)` → число, флаг — высота земли под 3d точкой.  *Пример: `world_find_ground_z_for_3d_coord(x, y, z)`*
- **475.** `world_find_roof_z_for_3d_coord(число, число, число)` → число, флаг — высота крыши под точкой.  *Пример: `world_find_roof_z_for_3d_coord(x, y, z)`*
- **476.** `world_get_is_line_of_sight_clear(число, число, число, число, число, число)` → флаг — прямая видимость между точками.  *Пример: `world_get_is_line_of_sight_clear(x, y, z, 3, 4, 5)`*
- **477.** `world_clear_peds_from_area(число, число, число, число, число, число)` → действие — удалить педов из области.  *Пример: `world_clear_peds_from_area(x, y, z, 3, 4, 5)`*
- **478.** `world_clear_cars_from_area(число, число, число, число, число, число)` → действие — удалить авто из области.  *Пример: `world_clear_cars_from_area(x, y, z, 3, 4, 5)`*
- **480.** `world_clear_exciting_stuff_from_area(число, число, число, число)` → действие — очистить область от эффектов.  *Пример: `world_clear_exciting_stuff_from_area(x, y, z, 3)`*
- **481.** `world_call_off_chase_for_area(число, число, число, число)` → действие — отменить погоню в области.  *Пример: `world_call_off_chase_for_area(x, y, z, 3)`*
- **482.** `world_add_entity(указ.)` → действие — добавить сущность в мир.  *Пример: `world_add_entity(entity)`*
- **483.** `world_clear_scan_codes()` → действие — сбросить коды сканирования.  *Пример: `world_clear_scan_codes()`*
- **1408.** `cleanarea(число, число, число, число, число)` → действие — очистить арену.  *Пример: `cleanarea(x, y, z, 3, 4)`*


## Педы: посадка в авто (10)

- **167.** `Giveweaponped(пед, число, строка, ...)` → действие — макрос дать педу оружие и патроны.  *Пример: `Giveweaponped(ped, x, "текст", ...)`*
- **661.** `shuffle_to_driver(пед)` → действие — пересесть за руль.  *Пример: `shuffle_to_driver(ped)`*
- **662.** `stay_when_jacked(пед, флаг)` → действие — не покидать при угоне.  *Пример: `stay_when_jacked(ped, true)`*
- **678.** `ensure_drive_by()` → действие — дать drive-by.  *Пример: `ensure_drive_by()`*
- **771.** `add_ammo_to_char(пед, число, число)` → действие — добавить патронов педу в оружие.  *Пример: `add_ammo_to_char(ped, x, y)`*
- **772.** `set_char_ammo(пед, число, число)` → действие — установить патроны педу в оружие.  *Пример: `set_char_ammo(ped, x, y)`*
- **773.** `get_ammo_in_char_weapon(пед, число)` → целое — патроны в оружии педа.  *Пример: `get_ammo_in_char_weapon(ped, x)`*
- **774.** `get_char_weapon_in_slot(пед, число)` → целое — оружие в слоте педа.  *Пример: `get_char_weapon_in_slot(ped, x)`*
- **777.** `set_char_stop_shoot_dont_seek_entity(пед, флаг)` → действие — пед перестаёт стрелять и не ищет цель.  *Пример: `set_char_stop_shoot_dont_seek_entity(ped, true)`*
- **821.** `add_armour_to_char(пед, число)` → действие — добавить брони педу.  *Пример: `add_armour_to_char(ped, x)`*


## Банды (9)

- **192.** `ped_is_gang_member(пед)` → флаг — пед член банды.  *Пример: `ped_is_gang_member(ped)`*
- **495.** `gangs_set_weapons(число, число, число)` → действие — оружие банды.  *Пример: `gangs_set_weapons(x, y, z)`*
- **496.** `gangs_set_vehicle_model(число, число)` → действие — авто банды.  *Пример: `gangs_set_vehicle_model(x, y)`*
- **497.** `gangs_set_ped_models(число, число, число)` → действие — модели педов банды.  *Пример: `gangs_set_ped_models(x, y, z)`*
- **498.** `gangs_choose_ped_model(число)` → целое — модель педа банды.  *Пример: `gangs_choose_ped_model(x)`*
- **501.** `gangs_initialise()` → действие — переинициализация банд.  *Пример: `gangs_initialise()`*
- **828.** `get_population_gang_peds()` → число — бандитских педов.  *Пример: `get_population_gang_peds()`*
- **1315.** `choose_gang_ped_model(число)` → целое — выбрать модель педа банды.  *Пример: `choose_gang_ped_model(x)`*
- **1317.** `init_gangs()` → действие — инициализировать банды.  *Пример: `init_gangs()`*


## Игровая логика и сценарий (8)

- **43.** `getflagmission()` → флаг — проверка флага миссии.  *Пример: `getflagmission()`*
- **44.** `setflagmission(флаг)` → действие — уcтановить флага миссии.  *Пример: `setflagmission(true)`*
- **74.** `cheat()` → действие — чит код введен.  *Пример: `cheat()`*
- **76.** `yield()` → действие — приостановить выполнение скрипта.  *Пример: `yield()`*
- **492.** `get_frame_counter()` → целое — номер кадра.  *Пример: `get_frame_counter()`*
- **502.** `gamelogic_pass_time(число)` → действие — пропустить время.  *Пример: `gamelogic_pass_time(x)`*
- **503.** `gamelogic_update()` → действие — обновить логику.  *Пример: `gamelogic_update()`*
- **504.** `gamelogic_after_death_arrest_shortcut()` → действие — такси-шорткат после смерти.  *Пример: `gamelogic_after_death_arrest_shortcut()`*


## Статистика (бюджеты) (8)

- **807.** `add_AutoPaintingBudget(число)` → действие — бюджет на покраску авто.  *Пример: `add_AutoPaintingBudget(x)`*
- **809.** `add_FashionBudget(число)` → действие — бюджет на одежду.  *Пример: `add_FashionBudget(x)`*
- **810.** `add_WeaponBudget(число)` → действие — бюджет на оружие.  *Пример: `add_WeaponBudget(x)`*
- **811.** `add_BloodRingKills(число)` → действие — убийства в Bloodring.  *Пример: `add_BloodRingKills(x)`*
- **812.** `add_PizzasDelivered(число)` → действие — доставленные пиццы.  *Пример: `add_PizzasDelivered(x)`*
- **813.** `add_Assassinations(число)` → действие — заказные убийства.  *Пример: `add_Assassinations(x)`*
- **814.** `add_MovieStunts(число)` → действие — киноскунты.  *Пример: `add_MovieStunts(x)`*
- **815.** `add_LoanSharks(число)` → действие — визиты к ростовщикам.  *Пример: `add_LoanSharks(x)`*


## Деньги и статистика (старые) (7)

- **311.** `get_player_money_info()` → целое — получить деньги игрока.  *Пример: `get_player_money_info()`*
- **312.** `set_player_money_info(число)` → действие — установить деньги игроку.  *Пример: `set_player_money_info(x)`*
- **909.** `create_some_money(число, число, число, число)` → действие — создать деньги на координатах.  *Пример: `create_some_money(x, y, z, 3)`*
- **912.** `do_money_effects(указ.)` → действие — эффект денег.  *Пример: `do_money_effects(entity)`*
- **1350.** `getmoney_ped(пед)` → целое — получить кол-во деньги педа.  *Пример: `getmoney_ped(ped)`*
- **1351.** `setmoney_ped(пед, число)` → действие — установить кол-во деньги педа.  *Пример: `setmoney_ped(ped, x)`*
- **1352.** `addmoney_ped(пед, число)` → действие — добавить кол-во деньги педа.  *Пример: `addmoney_ped(ped, x)`*


## Мотоциклы (6)

- **564.** `bike_is_player_on_any_bike()` → целое — транспорт/краны/зоны.  *Пример: `bike_is_player_on_any_bike()`*
- **565.** `bike_is_char_on_any_bike(пед)` → целое — транспорт/краны/зоны.  *Пример: `bike_is_char_on_any_bike(ped)`*
- **867.** `get_bike_anim_group(авто)` → целое — группа анимаций мотоцикла.  *Пример: `get_bike_anim_group(car)`*
- **868.** `set_bike_anim_group(авто, число)` → действие — уст. группу анимаций мотоцикла.  *Пример: `set_bike_anim_group(car, x)`*
- **869.** `get_bike_damage_flags(авто)` → целое — флаги повреждений мотоцикла.  *Пример: `get_bike_damage_flags(car)`*
- **870.** `set_bike_damage_flags(авто, число)` → действие — уст. флаги повреждений мотоцикла.  *Пример: `set_bike_damage_flags(car, x)`*


## Объекты (старые) (4)

- **94.** `setobjоcoordes()` → действие — установить координаты для объект.  *Пример: `setobjоcoordes()`*
- **95.** `getobjcoordes(объект)` → число, число, число (X, Y, Z) — получить координаты объекта.  *Пример: `getobjcoordes(obj)`*
- **145.** `getobjcoordinates_on_x(объект, число)` → число, число, число (X, Y, Z) — Получить мировую координату по x для объекта.  *Пример: `getobjcoordinates_on_x(obj, x)`*
- **146.** `getobjcoordinates_on_y(объект, число)` → число, число, число (X, Y, Z) — Получить мировую координату по y для объекта.  *Пример: `getobjcoordinates_on_y(obj, x)`*


## Анимации (4)

- **142.** `del_anim(пед)` → действие — удалить анимацию.  *Пример: `del_anim(ped)`*
- **409.** `get_anim_block_index(строка)` → целое — индекс блока анимаций.  *Пример: `get_anim_block_index("текст")`*
- **410.** `anim_set_group_ref(пед, число)` → действие — задать группу анимаций.  *Пример: `anim_set_group_ref(ped, x)`*
- **816.** `clear_all_char_anims(пед)` → действие — очистить все анимации педа.  *Пример: `clear_all_char_anims(ped)`*


## Педы: IK и голова (4)

- **853.** `get_pedik_m_fYaw(пед)` → число — угол наклона головы педа.  *Пример: `get_pedik_m_fYaw(ped)`*
- **854.** `set_pedik_m_fYaw(пед, число)` → действие — уст. угол наклона головы педа.  *Пример: `set_pedik_m_fYaw(ped, x)`*
- **855.** `get_pedik_m_fPitch(пед)` → число — наклон головы педа по тангажу.  *Пример: `get_pedik_m_fPitch(ped)`*
- **856.** `set_pedik_m_fPitch(пед, число)` → действие — уст. наклон головы педа по тангажу.  *Пример: `set_pedik_m_fPitch(ped, x)`*


## Управление игроком (3)

- **4.** `wait(флаг)` → действие — задержка.  *Пример: `wait(true)`*
- **11.** `keypress(строка)` → флаг — проверка на нажатия клавиши.  *Пример: `keypress("текст")`*
- **1399.** `get_controller()` → целое — получить контролер от игрока.  *Пример: `get_controller()`*


## Телефон (3)

- **118.** `hold_cellphone(пед, число)` → действие — поднять телефон.  *Пример: `hold_cellphone(ped, x)`*
- **1430.** `on_phone(число)` → действие — вкл телефон.  *Пример: `on_phone(x)`*
- **1431.** `off_phone(число)` → действие — выкл телефон.  *Пример: `off_phone(x)`*


## Катсцены (3)

- **928.** `is_cutscene_running()` → флаг — катсцена идёт.  *Пример: `is_cutscene_running()`*
- **929.** `get_num_cutscene_objs()` → целое — число объектов катсцены.  *Пример: `get_num_cutscene_objs()`*
- **930.** `get_cutscene_time_ms()` → целое — время катсцены в мс.  *Пример: `get_cutscene_time_ms()`*


## Память и низкий уровень (3)

- **1432.** `read_memory(число, число)` → целое — читать адрес памяти.  *Пример: `read_memory(x, y)`*
- **1433.** `write_memory(число, число, число)` → действие — записать адрес памяти.  *Пример: `write_memory(x, y, z)`*
- **1451.** `getmodelindex(указ.)` → целое — получить id модели.  *Пример: `getmodelindex(entity)`*


## Сообщения (старые) (2)

- **45.** `showtext(строка, число, число)` → действие — Вывод особого текста на экран.  *Пример: `showtext("текст", x, y)`*
- **66.** `play_voice(строка)` → действие — Проиграть голос(реплику).  *Пример: `play_voice("текст")`*


## Розыск (старые) (2)

- **322.** `get_max_cop_cars_in_pursuit()` → целое — получить макс полицейских авто.  *Пример: `get_max_cop_cars_in_pursuit()`*
- **323.** `set_max_cop_cars_in_pursuit(число)` → действие — установить макс полицейских авто.  *Пример: `set_max_cop_cars_in_pursuit(x)`*

---

## Итого по категориям

- **Транспорт: авто** — 334
- **Педы и игрок** — 303
- **Объекты** — 58
- **Вертолёты и самолёты** — 52
- **Мир: трафик, стриминг, зоны** — 50
- **Задачи педа** — 47
- **Статистика и деньги** — 45
- **Оружие и патроны** — 40
- **Сообщения, текст, HUD** — 39
- **Розыск (Wanted)** — 38
- **Радар, маркеры, блипы** — 35
- **Поиск и проверки** — 32
- **Транспорт: авто (старые)** — 31
- **Камера и экран** — 31
- **Погода, время, гравитация** — 31
- **Физика: скорость, гравитация, масса** — 29
- **Поиск и проверки (старые)** — 25
- **Водный транспорт** — 23
- **Создание и удаление** — 22
- **Повреждения и урон** — 19
- **Огонь и взрывы** — 18
- **Педы (старые)** — 17
- **Снаряды и следы пуль** — 16
- **Двери, замки, сигнализация** — 14
- **Общее и системное** — 13
- **Математика** — 12
- **Время и таймер** — 12
- **Звук** — 11
- **Игрок: состояние и свойства** — 11
- **Системное и потоки** — 11
- **Мир: геометрия и очистка** — 11
- **Педы: посадка в авто** — 10
- **Банды** — 9
- **Игровая логика и сценарий** — 8
- **Статистика (бюджеты)** — 8
- **Деньги и статистика (старые)** — 7
- **Мотоциклы** — 6
- **Объекты (старые)** — 4
- **Анимации** — 4
- **Педы: IK и голова** — 4
- **Управление игроком** — 3
- **Телефон** — 3
- **Катсцены** — 3
- **Память и низкий уровень** — 3
- **Сообщения (старые)** — 2
- **Розыск (старые)** — 2

Всего: **1506** функций.
(+ `Getflagmission`, `Setflagmission`, `set_current_weapon_ped`,
`set_ped_stay_when_attacked`, `set_ped_wait_state`, `set_ped_personality`,
`set_ped_only_damaged_by_player`, `is_ped_health_greater`,
`make_obj_targettable`, `get_dead_char_pickup_coords`, `set_ped_mood`,
`request_animation`, `remove_animation` — добавлены позже, см.
предупреждение вверху.)
