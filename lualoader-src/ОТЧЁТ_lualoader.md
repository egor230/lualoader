# Отчёт по проекту lualoader

**Репозиторий:** https://github.com/egor230/lualoader («lua плагин для gta vice», 37 коммитов)
**Установка в игре:** `/mnt/807EB5FA7EB5E954/games/old GTA Vice City/` (`lualoader.asi` + папка `lualoader/`)
**Дата отчёта:** 24.08.2026 (обновление 14.09.2026; дополнение 15.09.2026 — массовое
расширение API до 710 функций, см. раздел 3; дополнение 18.09.2026 — починены
системные баги: ABI `CVector`, гонка потоков `Createcar`, UB в удалениях,
появились C++-сторож миссии и трассировка — см. разделы 3 и 10; итог
**1519 функций**).

---

## 1. Что это такое

**Lualoader** — ASI-плагин для GTA Vice City (PC), который встраивает интерпретатор **Lua** в игру и позволяет писать моды и миссии на Lua вместо CLEO/main.scm. Это собственный аналог известных MoonLoader (SA) / GTALua, написанный поверх **plugin-sdk**.

Состав репозитория:

| Файл | Размер | Назначение |
|------|--------|-----------|
| `plugin.cpp` | ~1690 строк | Точка входа ASI, менеджер скриптов, регистрация Lua-API |
| `funcs.h` | 14095 строк, ~710 регистраций | Реализация всех Lua→C++ функций-биндингов |
| `lualoader/` | 30 .lua | Примеры и готовые миссии автора |
| `plugin.VC.asi`, `lualoader.zip/rar` | — | Собранный плагин |
| `Lua автозавершения.txt/.rar` | — | Автодополнение API для Notepad++ (частично устарело) |

## 2. Архитектура

```
gta-vc.exe
 └── lualoader.asi (plugin-sdk)
      ├── Events::gameProcessEvent  ← главный цикл
      ├── Менеджер скриптов (plugin.cpp)
      │    ├── поиск *.lua в папке lualoader/
      │    ├── на каждый файл: свой std::thread + свой lua_State
      │    ├── main() скрипта = корутина Lua 5.4 (lua_yieldk/lua_resume)
      │    └── перезагрузка скриптов по Ctrl (reload())
      └── funcs.h — API: 710 функций, зарегистрированных через lua_register
```

### Ключевые механизмы

**Запуск скриптов.** `search()` рекурсивно обходит папку `lualoader/`, для каждого `.lua`
создаётся отдельный поток и отдельное Lua-состояние (`state`). Скрипт обязан определить
глобальную функцию `main()` — она вызывается как сопрограмма. `wait()` из Lua реализован
через механизм хуков: `lua_sethook(LUA_MASKCOUNT)` → `lua_yieldk` → корутина
приостанавливается, а её возобновлением управляет цикл в C++ (`startscipt`).

**Второй поток внутри скрипта.** `newthread()` создаёт вложенную `lua_newthread`,
что позволяет запускать параллельные задачи прямо из Lua (флаги-переключатели
`star_thread`, `star_coroutine`).

**Жизненный цикл.** `start_lualoder()` следит за страницами меню (`CMenuManager::m_nCurrentPage`
по адресу 0x869630): различает «новая игра» (7/10), «загрузка» (8/10) и «в игре» (32),
чтобы вовремя запустить/убить скрипты. `final_scripts()` корректно завершает все
Lua-состояния. `reload()` — горячая перезагрузка всех скриптов по Ctrl с очисткой
созданных объектов через `destroy(L)` и сборкой мусора.

**Лог.** Ошибки компиляции каждого скрипта пишутся в `log.txt` с датой
(`writelog`). В установленной копии лог показывает успешную загрузку трёх скриптов
24-08-2026: `test .lua`, `write_coordinate.lua`, `mod.lua`.

## 3. Lua-API (1519 зарегистрированных функций)

Из них **366 — оригинальные функции автора** (август–сентябрь 2026) и
**+1144 добавлены массовым переводом игрового API plugin-sdk**
(все новые — в стиле автора: try/catch + writelog, русские комментарии,
проверка типов аргументов через `lua_type`). Полный список новых функций
по этапам см. в git-истории plugin.cpp (регистрации с номерами 181б и далее).

**Дополнения 17–18.09.2026 (поверх этапов):** `Getflagmission` /
`Setflagmission` (C++-сторож миссии: при вызове поднимает поток, отслеживающий
гибель/арест игрока → «Миссия провалена» + очистка; поток всегда один —
старый уничтожается через `join`), `set_current_weapon_ped` (01B9 — положить
оружие педу в руки), `set_ped_stay_when_attacked` (0350 — пед держит позицию
при атаке). Исторический дубль `setarmour` оставлен как было.

**Дополнение 18.09.2026 (перенос Riot): +9 функций для переноса поведений
толпы, объектов-целей и выпадения оружия** (регистрации plugin.cpp 363-371,
реализации funcs.h 2810-2932):

| Функция | Опкод | Сигнатура | Назначение |
|---|---|---|---|
| `set_ped_wait_state` | 0372 | `(ped, state, timer)` | поставить педа в «ожидание» (толпа Riot: 29, 99999999) |
| `set_ped_personality` | 0243 | `(ped, personality)` | тип личности (16=горожанин, 30=агрессор) |
| `set_ped_only_damaged_by_player` | 02A9 | `(ped, flag)` | ранить может только игрок |
| `is_ped_health_greater` | 0184 | `(ped, threshold) -> bool` | здоровье строго больше порога (`not greater(99)` = «укушен») |
| `make_obj_targettable` | 035D | `(obj, flag)` | в объект можно прицеливаться/бросать |
| `get_dead_char_pickup_coords` | 04A5 | `(ped) -> x, y, z` | координаты пикапа, выпавшего из трупа |
| `set_ped_mood` | 04E3 | `(mood, timer)` | настроение ИГРОКА (как `SET_PLAYER_MOOD $2 1 60000`) |
| `request_animation` | 04ED | `(name)` | загрузить анимацию (RIOT) |
| `remove_animation` | 04EF | `(name)` | выгрузить анимацию (RIOT в очистке) |

Исторические дубли-аналоги (оставлены как есть): `set_ped_stats_to` ≈ 0243,
`setped_damaged_player` ≈ 02A9 (баг: всегда флаг 0), `obj_target` ≈ 035D,
`set_player_mod` ≈ 04E3 — новые функции исправляют/расширяют.

**Полный справочник «как вызывать»** (категории, параметры по порядку,
возврат, номера, примеры вызова) — см. `LUA_API_СПРАВОЧНИК.md` в этой папке,
сгенерирован автоматически из `plugin.cpp` + `funcs.h` (генератор
`/tmp/opencode/gen_doc.py`).

### 3.1 Оригинальные категории (366 функций, были до 15.09.2026)

| Категория | Примеры функций |
|-----------|-----------------|
| Игрок/педы (≈60) | `findplayer`, `setpedhealth`, `getpedhealth`, `setarmour`, `givemoney`, `createped`, `Createped` (макрос), `create_spec_ped`, `set_skin`, `ped_sprint_to_point`, `ped_walk_to_point`, `kill_ped_on_foot`, `kill_char_any_means`, `ped_aim_at_ped`, `ped_atack`, `ped_frozen`, `hold_cellphone`, `ped_anim`, `follow_the_leader` |
| Транспорт (≈50) | `createcar`/`Createcar`, `remove_car`, `cardrive`, `setcarspeed`, `getcarspeed`, `setdrivingstyle`, `car_explode`, `is_car_stopped/stuck/upsidedown/upright`, `setcarcoordes`, `setcarangle`, `opendoorcar`/`Opendoorcar`, `setcaraction`, `setcartask`, `go_to_route`, `find_road_for_car`, `set_car_tires`, `setbomb_in_car`, `activetbomb`, `set_timer_bomb_car`, `putincar`, `ped_car_as_driver/passenger`, `kill_peds_in_car` |
| Пикапы/объекты (≈15) | `create_pickup`, `create_weapon_pickup`, `create_money_pickup`, `picked_up`, `create_obj`, `move_obj`, `move_rotate` |
| Маркеры/HUD (≈20) | `createmarker`, `setsizemarker`, `create_marker_actor/car/pickup/obj`, `star_mission_marker`, `remove_blip`, `printmessage`, `showtext`, `show_text_styled`, `show_text_gtx`, `print_front`, `flash_hud`, `set_widescreen`, `draw_corona`, `create_sphere` |
| Камера (≈6) | `camera_at_point`, `set_camera_position`, `set_camera_and_point`, `restore_camera`, `Getcameracoordes` |
| Розыск/полиция (≈5) | `set_wanted`, `clear_wanted`, `is_wanted_level`, `arrested` |
| Мир/эффекты (≈15) | `create_explosion`, `createfireonped/car/cords`, `remove_fire`, `fade`, `load_scene`, `set_traffic`, `setclock`, `set_time_scale`, `set_radio` |
| Звук (≈3) | `play_sound`, `play_voice`, `sound_coordinate` |
| Система/прочее (≈25) | `wait`, `yield`, `keypress`/`Keypress`, `cheat`, `loadmodel`/`availablemodel`/`releasemodel`/`load_requested_models`, `newthread`, `destroy`, `star_timer`/`stop_timer`/`timer_donw`, `getflagmission`/`setflagmission`, `get_current_name_luascript` |
| Оружие/урон (≈20) | `giveweaponped`/`Giveweaponped`, `remove_all_weapons_ped`, `Getweaponslot`, `get_damage_weapon_ped`, `get_aimgun_ped`, `get_ammo_weapon_ped`, `is_ped_damage_from_ped`, `is_targetting_in_ped`, `car_lastweapondamage` |
| Память (≈5) | `read_memory`, `write_memory`, `getgravity`/`setgravity`/`gravity_default` |
| Поиск в мире (≈20) | `findcar`/`findped`/`findobj`, `randomfindcar/ped/obj`, `find*_nearest_in_cord`, `get_*_in_cord`, `check_*_in_cord`, `isped/vehicle/object` |

### 3.2 Новые категории (344 функции, добавлены 15.09.2026, этапы 1–10)

Все построены на прямых вызовах классов plugin-sdk (поля структур и методы,
без опкодов где возможно). Регистрация в plugin.cpp с номерами 181б…375т.

**Этап 1 — Педы: состояния и действия (+51)**
`get_ped_state`, `get_ped_last_state`, `set_ped_state` (ePedState),
`get_ped_heading`, `set_ped_heading_goal`, `get_ped_heading_goal`,
`set_ped_heading_rate`, `get_ped_heading_rate`, `get_ped_enter_type`,
`get_ped_from_vehicle`, `ped_can_be_deleted`, `ped_is_gang_member`,
`ped_is_player`, `ped_is_in_control`, `ped_is_shootable`, `ped_is_pointer_valid`,
`ped_set_dead`, `ped_dress`, `ped_calculate_new_orientation`, `ped_set_move_anim`,
`ped_set_get_up`, `ped_set_jump`, `ped_set_landing`, `ped_set_idle`, `ped_fall`,
`ped_turn_body`, `ped_update_position`, `ped_restore_heading_rate`,
`ped_restore_head_position`, `ped_restore_previous_state`,
`ped_restore_previous_objective`, `ped_stop_non_partial_anims`,
`ped_restart_non_partial_anims`, `ped_clear_aim_flag`, `ped_clear_look_flag`,
`ped_clear_chat`, `ped_clear_attack`, `ped_clear_attack_anim`, `ped_clear_seek`,
`ped_clear_follow_path`, `ped_clear_wait_state`, `ped_clear_point_gun_at`,
`ped_answer_mobile`, `ped_clear_answer_mobile`, `ped_face_phone`,
`ped_make_phonecall`, `ped_pause`, `ped_mug`, `ped_solicit`, `ped_buy_ice_cream`,
`ped_look_for_sexy_cars`, `ped_look_for_sexy_peds`.

**Этап 2 — Машины: салон, флаги, двери (+55)**
`get_car_driver`, `get_car_passenger(car,seat)`, `get_car_num_passengers`,
`get_car_max_passengers`, `get/set_car_steer_angle`, `get_car_gas_pedal`,
`get_car_brake_pedal`, `get/set_car_engine_on`, `get/set_car_lights_on`,
`get/set_car_handbrake_on`, `get/set_car_locked`, `get_car_is_van/bus/big`,
`set_car_comedy_controls`, `set_car_hide_occupants`,
`get/set_car_tires_invulnerable`, `get/set_car_door_lock`,
`get_car_last_damager`, `get_car_time_of_death`, `get_car_weapon_damage`,
`get_car_vehicle_class`, `car_play_horn`, `car_extinguish_fire`, `car_uses_siren`,
`car_is_law_enforcement`, `car_has_roof`, `car_is_open_top`, `car_is_upside_down`,
`car_is_on_its_side`, `car_is_vehicle_normal`, `car_can_be_deleted`,
`car_can_ped_enter`, `car_can_doors_be_damaged`, `car_activate_bomb`,
`car_activate_bomb_when_entered`, `car_get_height_above_road`,
`car_is_clear_to_drive_away`, `car_is_door_ready/fully_open/closed/missing(car,door)`
(двери: 0-капот, 1-багажник, 2-5), `car_shuffle_passengers`,
`car_update_passenger_list`, `get/set_car_radio_station`, `get_car_created_by`,
`get/set_car_wanted_stars_on_enter`.

**Этап 3 — Оружие + розыск (+48)**
Характеристики текущего оружия (get/set пары): `weapon_range`, `weapon_firing_rate`,
`weapon_reload`, `weapon_speed`, `weapon_spread`, `weapon_radius`,
`weapon_lifespan`; `get_weapon_slot_info`, `get/set_ammo_in_clip_ped`,
`get_weapon_state`, `weapon_reload_ped`, `weapon_is_melee`, `weapon_is_two_handed`,
`weapon_has_ammo_to_be_used`. Игрок: `get/set_player_money_info`,
`get_player_display_money`, `set_player_drive_by`. Розыск (через
`CPlayerPed::GetWanted()`): `get/set_wanted_chaos_level`, `get/set_wanted_multiplier`,
`get_cops_in_pursuit`, `get/set_max_cops_in_pursuit`,
`get/set_max_cop_cars_in_pursuit`, `get/set_chance_on_roadblock`,
`wanted_are_swat/fbi/army_required`, `wanted_num_helis_required`,
`wanted_clear_crimes`, `wanted_reset_police_pursuit`, `wanted_update_wanted_level`,
`wanted_set_no_drop`, `wanted_cheat_level`, `get_wanted_level_before_parole`,
`get_wanted_last_decreased_time`. Плюс ранние `set_damage_current_weapon`,
`set_clip_current_weapon` (обойма, проверка WEAPON_FIRE_INSTANT_HIT).

**Этап 4 — Погода/время/камера/графика (+43)**
`force_weather`, `force_weather_now`, `release_weather`, `get_old/new_weather_type`,
`set_weather_type`, `is_lightning_flash`, `force_rain`, `weather_add_rain`,
`weather_add_stream_after_rain`, `weather_render_rain_streaks`,
`get_sun_blocked_by_clouds`. Время: `get_game_hours/minutes/seconds`,
`set_game_hours`, `get/set_milliseconds_per_game_minute`, `clock_store`,
`clock_restore`, `get_time_until(h,m)`, `is_time_in_range(a,b)`. Графика:
`get/set_fov`, `get/set_far_clip`, `get/set_near_clip`, `get/set_lod_distance`,
`get/set_fade_color(r,g,b)`. Камера: `camera_shake(strength,x,y,z)`,
`camera_draw_borders_widescreen`, `camera_clear_player_weapon_mode`,
`camera_calculate_derived_values`, `camera_dont_process_obbe_cinema`,
`get/set_mouse_accel_vertical`, `get/set_mouse_accel_horizontal`,
`get_use_mouse_3rd_person`.

**Этап 5 — Объекты + пожары (+29)**
`get_obj_type`, `get/set_obj_bonus_value`, `get/set_obj_cost_value`,
`get/set_obj_damage_multiplier`, `get/set_obj_collision_damage_type`,
`get/set_obj_camera_avoids`, `get/set_obj_timer`, `get_obj_ref_model_id`,
`get/set_obj_attach_force`, `obj_can_be_deleted`, `obj_damage(obj,damage)`,
`obj_delete_all_mission`, `obj_delete_all_temp`, `obj_delete_all_temp_in_area(x,y,z,r)`,
`fire_find_nearest(x,y,z)` (→ fire + дистанция), `fire_start_on_point(x,y,z,size,moves)`,
`get/set_fire_time_to_burn`, `get_fire_target`, `fire_is_active`, `get_fire_position`.

**Этап 6 — Анимации + статистика (+34)**
`ped_play_anim(ped,group,id)`, `ped_blend_anim(ped,group,id,time)`,
`get_anim_block_index(name)`, `anim_set_group_ref`. Статистика (CStats):
`get_ped_stat_kills`, `get_ped_stat_cheated`, `get_stat_property_destroyed`,
`get_stat_longest_wheelie/stoppie/2wheel_dist`, `get_stat_longest_wheelie_time`,
`get_stat_seagulls_killed`, `get_stat_times_drowned`, `get_stat_flight_time`,
`get_stat_missions_total`, `get_stat_frenzies_total/passed`,
`get_stat_ice_cream_sold`, `get_stat_pizzas_delivered`, `get_stat_assassinations`,
`get_stat_movie_stunts`, `get_stat_stores_knocked_off`, `get_stat_loan_sharks`,
`get_stat_garbage_pickups`, `get_stat_weapon/fashion/property/auto_painting_budget`,
`get_stat_bloodring_kills/time`, `get_stat_shooting_rank`,
`get_stat_top_shooting_score`, `get_stat_num_property_owned`.

**Этап 7 — Радар/метки + звук (+24)**
`radar_set_coord_blip(type,x,y,z,colour,display)` (→ id),
`radar_set_short_range_blip(...)`, `radar_set_entity_blip(type,entity,colour,display)`,
`radar_clear_blip(id)`, `radar_change_blip_colour/brightness/scale/display`,
`radar_set_blip_sprite`, `radar_show_marker`, `radar_set_marker_state`,
`radar_draw_radar_sprite`, `get/set_radar_range`,
`radar_clear_blip_for_entity(type,entity)`. Звук: `play_one_off_sound(x,y,z,sound)`,
`play_continuous_sound(...)` (→ id), `remove_sound_by_id(id)`,
`set/get_radio_channel`, `set_music_does_fade`, `police_radio_message`,
`create_explosion_no_sound`.

**Этап 8 — Мир + популяция (+22)**
`get/set_ped_density_multiplier`, `get/set_all_random_peds_type`,
`population_manage`, `population_remove_peds_full_pool`,
`population_convert_all_objects_to_dummy`, `population_is_female/male/sunbather`.
Мир: `world_find_ground_z_for_coord(x,y)`, `world_find_ground_z_for_3d_coord`,
`world_find_roof_z_for_3d_coord`, `world_get_is_line_of_sight_clear(6 чисел)`,
`world_clear_peds/cars_from_area(6 чисел)`, `world_extinguish_all_car_fires_in_area`,
`world_clear_exciting_stuff_from_area`, `world_call_off_chase_for_area`,
`world_add_entity`, `world_clear_scan_codes`.

**Этап 9 — Математика/таймер/банды (+22)**
`general_atan_of_xy`, `general_angle_between_points`,
`general_radian_angle_between_points`, `general_limit_angle`,
`general_limit_radian_angle`, `general_node_heading_from_vector`. Таймер:
`get_timer_ms`, `get_timer_ms_pause_mode`, `get_frame_counter`, `get_time_step`,
`is_user_pause`. Банды: `gangs_set_weapons(gang,w1,w2)`,
`gangs_set_vehicle_model`, `gangs_set_ped_models`, `gangs_choose_ped_model`,
`gangs_set/get_attack_with_cops`, `gangs_initialise`. Логика:
`gamelogic_pass_time(ms)`, `gamelogic_update`, `gamelogic_after_death_arrest_shortcut`.

**Этап 10 — Сообщения + шрифт (+21)**
`message_add_big(text,time,style)`, `message_add_big_queue`, `message_add(text,time,flag)`,
`message_add_jump_queue`, `message_add_with_number`, `message_clear_all`,
`message_clear_small_only`, `message_clear_this_big_print(text)`,
`message_clear_this_print(text)`. Шрифт (CFont): `font_set_style`,
`font_set_drop_shadow`, `font_set_alpha_fade`, `font_set_centre_size`,
`font_set_wrapx`, `font_set_prop_on/off`, `font_set_background_on/off`,
`font_set_right_justify_on/off`.

**Этап 11 — Транспорт: вертолёты, лодки, поезда, краны, RC (+49)**
Вертолёты: `heli_orientate_body`, `heli_orientate_body_time`, `heli_pre_rotor`,
`heli_engine_running`, `heli_particle`, `heli_rotor_speed` (get/set),
`heli_draw_rotor`, `heli_slow_blade`, `heli_switch`, `heli_flame`,
`heli_is_rotor_at_full_speed`, `heli_has_any_ped`. Катсценный вертолёт
(Catalina): `catalina_heli_*` (seek, follow, stop, create_marker,
search_start/stop, is_in_air). Самолёты: `plane_*`, `find_drug_plane_coords`,
`has_drug_plane_been_shot_down`, `has_bomb_plane_been_shot_down`,
`plane_locked_on_target`. Лодки: `is_boat`, `is_boat_anchored`,
`boat_apply_water_force`, `boat_selected_water`. Поезда: `train_*` (catch,
unhook, stop, leave). RC: `rc_car_drive_*`, `rc_chopper_gun`,
`rc_rocket_leaves_vehicle`, `rc_mine_explode`, `give_rc_car`,
`remote_control_car_on/off`, `rc_detonators_on`. Мото: `is_bike`,
`is_flying_vehicle`, `bike_is_in_air`. Краны/зоны: `crane_*`, `escalator`,
`prevent_escalator_sideways_movement`, `zone_find_index`,
`zone_get_level_from_position`.

**Этап 12 — Транспорт: остальное (управление, свойства, проверки, педы) (+110)**
Краны: `crusher_crane_activate`, `military_crane_activate`, `crane_deactivate`,
`crane_military_collected_all`, `crane_is_lifting_car`. Катер: `boat_stop`.
Вертолёты: `heli_goto_coords`, `heli_create_dust_effect`. Управление авто
(опкоды CCarAI): `car_goto_accurate`, `car_set_idle`, `car_wander_randomly`,
`car_face_coord`, `freeze_car_position`, `freeze_car_no_collide`,
`set_car_status`, `set_car_mission`, `car_block_car`, `car_ram_car`,
`car_change_lane`, `car_drive_straight`, `set_car_forward_speed`, `car_wait`,
`car_route_seed`, `car_straight_dist`, `car_convoy`, `car_make_stronger`,
`car_to_player`, `car_fade_in`, `car_on_trailer`. Свойства:
`set_car_visible`, `set_car_watertight`, `set_car_big_wheels`,
`set_car_bproof`, `set_car_fproof`, `set_car_rproof`, `set_car_no_collide`,
`set_car_burst_tires`, `set_car_dmg_player_only`, `car_load_collision`,
`car_upside_safe`, `car_change_color`, `car_set_comp`. Здоровье/вектора:
`set/get_car_health`, `get_car_heading`, `get_car_forward_x/y`.
Проверки (boolean): `is_car_in_air(_proper)`, `car_health_greater`,
`is_car_crushed`, `is_car_remap`, `is_car_stuck_roof`, `car_is_tire_burst`,
`is_car_visibly_damaged`, `car_waiting_collision`, `is_car_on_trailer`,
`car_damaged_by_car/ped`, `car_just_sunk`, `car_drowning`, `car_has_weapon`,
`car_any_bomb`, `car_has_bomb`, `is_car_in_water`, `fire_button_pressed`,
`is_car_alive`, `does_vehicle_exist`, `is_car_in_garage`, `was_ever_police`.
Очистка: `clear_car_dmg`, `clear_car_wep_dmg`, `remove_stuck_check`,
`remove/add_upside_check`, `unlock_doors_area`. Двери/багажник:
`close_all_doors`, `close_car_boot`, `pop_car_boot(_physics)`, `lock_doors`,
`change_car_lock`. Пед/водитель: `get_car_driver_ref`, `get_char_in_seat`,
`order_driver_out`, `set_ped_max_drive_speed`, `set_ped_drive_aggression`,
`shuffle_to_driver`, `stay_when_jacked`, `is_ped_in_police_car`,
`is_ped_touching_vehicle`, `is_ped_leaving_to_die`, `is_ped_drowning`,
`is_ped_in_water`, `is_ped_in_zone`, `is_player_in_zone`. Телепортация:
`warp_ped_from_car`, `warp_ped_passenger`, `warp_player_from_car`.
Оружие транспорта: `set_car_ammo`, `switch_car_weapons`, `set_car_weapon`,
`fire_hunter_gun`, `set_tank_detonate`, `ensure_drive_by`. Бомбы/сигнализация:
`arm/disarm_car_bomb`, `give_car_alarm`. Прочее: `switch_car_siren`,
`set_driving_style_op`.

**Этап 13 — Поведение педов (задачи), снаряды/РПГ, пул, физика (+47)**
Задачи педа (SetObjective / SET_CHAR_OBJ): `set_char_obj_wait`,
`set_char_obj_flee_coord`, `set_char_obj_guard_spot`, `set_char_obj_guard_area`,
`set_char_obj_wait_in_car`, `set_char_obj_leave_car`,
`set_char_obj_enter_car_passenger/driver`, `set_char_obj_follow_car_in_car`,
`set_char_obj_destroy_object`, `set_char_obj_goto_area_in_car`,
`set_char_obj_follow_car_foot_offset`, `set_char_obj_guard_attack`,
`set_char_obj_goto_coord_on_foot/in_car`, `set_char_obj_run_to_coord`,
`set_char_obj_goto_area_any_means`, `set_char_obj_follow_formation`,
`set_char_obj_hail_taxi`, `set_char_obj_steal_any_car`,
`set_char_obj_leave_any_car`, `set_char_obj_flee_car`,
`set_char_obj_walk_to_char`, `set_char_obj_steal_any_car_mission`,
`set_char_obj_sprint_to_coord`, `set_char_obj_goto_car_on_foot`.
Снаряды (CProjectileInfo, `ms_apProjectile` 0x7DB888, 32 слота):
`get_projectile_count`, `get_projectile_weapon`, `get_projectile_source`,
`get_projectile_pos`/`set_projectile_pos` (направление ракеты в полёте),
`is_projectile_in_use`, `remove_all_projectiles`, `is_projectile_in_area`,
`destroy_projectiles_in_area`, `add_projectile`. Пул/поиск:
`find_car_by_model`, `find_ped_by_model`, `find_object_by_model`,
`get_vehicle_pool_size`, `get_car_pool_index`, `get_car_by_pool_index`.
Физика: `get/set_car_velocity` (m_vecMoveSpeed 0x70). Оружие:
`remove_weapon_from_char`. Погода: `get_forced_weather_type`,
`is_lightning_burst`.

**Этап 14 — Из переписки: турель/маньяк, вертолёты, физика, патроны (+47)**
Добивка задач педа (`COMMAND_SET_CHAR_OBJ_*` / `SetObjective`):
`set_char_obj_kill_char_on_foot`, `_kill_player_on_foot`,
`_kill_char_any_means`, `_kill_player_any_means`,
`_flee_char_on_foot_till_safe`, `_flee_player_on_foot_till_safe`,
`_flee_char_on_foot_always`, `_flee_player_on_foot_always`,
`_goto_char_on_foot`, `_goto_player_on_foot`, `_aim_gun_at_char`,
`_no_obj`, `_catch_train`, `_buy_ice_cream`, `_goto_area_on_foot`,
`_destroy_car`, `_fire_at_object_from_vehicle`, `_follow_route`
(режим «турели» — `_aim_gun_at_char`/`_destroy_car`/`_fire_at_object`).
Вертолёты (`CHeli`, проверка `m_nVehicleClass == VEHICLE_HELI`):
`heli_send_down_swat` (спецназ на верёвке), `get/set_heli_forced_z`
(зависание), `get/set_heli_searchlight_x/y/intensity`, `get/set_heli_rotor_angle`,
`heli_get/set_num_random`, `heli_is/set_catalina_on`. Физика:
`get/set_ped_velocity`, `get/set_object_velocity` (m_vecMoveSpeed 0x70 —
«лифт из двери гаража»), `get_car/ped/object_speed` (скаляр, тема
«Рекламный тур»). Патроны: `add_ammo_to_char`, `set_char_ammo`,
`get_ammo_in_char_weapon`, `get_char_weapon_in_slot`,
`set_player_fast_reload`, `is_char_shooting_in_area`. Прочее:
`set_char_stop_shoot_dont_seek_entity`.

**Этап 15 — Следы пуль, 3D-маркеры, пед-статы, повреждения, статистика (+48)**
Следы пуль (`CBulletTraces::aTraces[16]`): `add_bullet_trace_radius`,
`add_bullet_trace_weapon`, `get_bullet_trace_count`, `get_bullet_trace_origin/target`,
`clear_bullet_traces`. 3D-маркеры (`C3dMarkers`): `place_3d_marker`,
`place_3d_marker_set`, `get_num_active_3d_markers`. Пед-статы
(`CPed::m_pPedStats`): `get/set_pedstat_m_fFleeDistance`,
`_m_fHeadingChangeRate`, `_m_fAttackStrength`, `_m_fDefendWeakness`.
Повреждения (`CAutomobile::m_carDamage` — `CDamageManager`, проверка
`VEHICLE_AUTOMOBILE`): `get/set_car_door_status`, `_panel_status`,
`_light_status`, `_wheel_status`, `_engine_status`, `apply_car_damage`,
`fuck_car_completely`, `reset_car_damage`. Статистика (прямая запись в
`CStats::*`): `add_auto_painting/property/fashion/weapon_budget`,
`add_blood_ring_kills`, `add_pizzas_delivered`, `add_assassinations`,
`add_movie_stunts`, `add_loan_sharks`. Очистка/пед (опкоды):
`clear_all_char_anims`, `clear_char_follow_path`,
`clear_char_last_damage_entity`, `clear_char_last_weapon_damage`,
`char_set_idle`, `add_armour_to_char/player`, `add_ammo_to_player`,
`clear_area_of_cars/chars`.

**Этап 16 — CPopulation/CHud/CPlayerInfo/CPedIK/CBoat/CBike/CTimer/CClock/CStreaming/
CTheZones/CTrafficLights/CWeapon/CPickups/CObject/CRadar/CCoronas/CCutsceneMgr/
CCarCtrl/CBrightLights/CTrain (+114)**
Счётчики педов (`CPopulation::ms_nTotal*`), таймеры HUD (`CHud::m_*FadeTimer`),
поля игрока (`CWorld::Players[...]`), наклон головы (`CPed::m_PedIK.m_sHead`),
поля лодки/мотоцикла (с проверкой `VEHICLE_BOAT/BIKE`), шаг времени
(`CTimer`), игровой таймер (`CClock`), стриминг (`CStreaming::ms_*`),
зоны (`CTheZones`), светофоры, эффекты оружия/пикапов, поля объекта,
управление блипами (`CRadar::ChangeBlip*`), короны, катсцены, трафик.

**Этап 17 — Массовые get/set для всех полей классов (+366)**
Автоматический майнер полей `CVehicle`/`CPed`/`CObject`/`CPhysical`/
`CAutomobile`/`CBoat`/`CBike`/`CHeli` из заголовков SDK: для каждого
оставшегося поля сгенерированы `get_<префикс>_<поле>` и
`set_<префикс>_<поле>` (CVector-поля — через x,y,z). Касты к подклассам
— с проверкой `m_nVehicleClass` и скобками: `((CAutomobile*)car)->`.

**Финал — CWanted/CGangs/CMotionBlurStreaks (+16)**
Поля розыска (`FindPlayerPed()->GetWanted()->m_fMultiplier`,
`m_nMaxCopsInPursuit`…), `CGangs::ChooseGangPedModel`,
`register_motion_streak`.

*Примечание:* `car_is_door_closed` уже существовал (этап 2) — дубль из этапа 12
не регистрировался. Сознательно не переводились на Lua (низкоуровневые/
миссионные): CCarCtrl (спавн/трафик), CProjectile/CBulletTrace,
CCrane-привязки к миссиям, CPedAttractor, CCutsceneMgr, CReplay,
CMotionBlurStreaks.

### 3.3 Технические заметки по новым функциям

- **Глобальность**: CWeaponInfo (урон/обойма/характеристики оружия) и
  tHandlingData (ускорение/макс.скорость/сцепление) — глобальные таблицы на
  тип оружия/модель авто: правка влияет на ВСЕ объекты этого типа.
- **Обойма**: `CWeaponInfo::m_nAmountofAmmunition` (0x10) — объём магазина;
  `Reload()` читает его каждый раз; дополнительно меняется `m_nAmmoInClip` у педа.
- **CWanted** доступен только через `CPlayerPed::GetWanted()` (m_pWanted = 0x5F4),
  НЕ через CPlayerInfo. **Пожары** — глобальный `gFireManager`.
- **SDK-обёртки**: перед использованием SDK-метода проверять, что он реально
  обёрнут в `game_vc/*.cpp` (пример провала: `GetAnimGroupName` — не обёрнута).
- **Анимации** запускаются через `ped->m_pRwClump` (AddAnimation/BlendAnimation);
  группа педа — `m_nAnimGroupId` (0x1F4).

### Пример скрипта из репозитория (test.lua)

```lua
require("lualoader/mod")
function main()
  while true do wait()
    player = findplayer()
    if Keypress("VK_H") then
      x,y,z = getcoordinates_on_y(player, 5)
      car = Createcar("MODEL_INFERNUS",x,y,z)
      setcarangle(car,180)
      Giveweaponped(player,600,"uzi")
      setarmour(player,150) wait(3100) remove_car(car)
    end
  end
end
```

По нажатию H спавнит Infernalus позади игрока, выдаёт Uzi и броню, через 3 секунды удаляет машину.

## 4. Готовый контент (папка lualoader/)

Помимо примеров в репозитории лежат **авторские миссии**: серия `mission 1–9`,
`mission guardianship`, `Dirty Laundry`, `races`, `races 2`, `hijack a taxi`,
скрипты мостов (`open and close first/second bridge*.lua` — плавное открытие дверей
моста через покадровую анимацию `DoorsExample` + `VehicleExtendedData`), `autoaim`,
`drive and shoot`. Это фактически мини-CLEO-библиотека миссий для VC.

## 5. Сильные стороны

1. Полноценный кооперативный планировщик: у каждого скрипта свой поток и своё состояние,
   `main()`+`wait()` работают как в Sanny Builder/MoonLoader.
2. Большое покрытие API (**1519 функций**) — почти весь игровой функционал SCM и
   plugin-sdk (педы, авто, оружие, розыск, погода, радар, статистика, сообщения)
   доступен из Lua.
3. Горячая перезагрузка скриптов (Ctrl) с очисткой объектов — быстрая разработка.
4. Логирование ошибок компиляции в log.txt с датами.
5. Есть автодополнение для Notepad++, собранный .asi и богатые примеры-миссии.

## 6. Найденные проблемы и риски (статус на 18.09.2026)

### Починено в этой серии работ (16–18.09.2026)

- **UB в удалениях** (главный источник вылетов): erase-в-итерации во всех циклах
  `destroy()`, висячие записи в map-ах после `remove_*`, вложенные циклы —
  переписано на `it = map.erase(it)`; каждая `remove_*` стирает свою запись;
  добавлены проверки висячих указателей через `find*inpool`. Мёртвый дубль
  `all_destroy()` удалён — функция удаления одна (`destroy`).
- **Удаление авто с сидящими** (раньше — вылет): `warp_out_car_occupants` /
  `warp_ped_out_of_car` / `safe_remove_car` высаживают игрока, водителя и 8
  пассажиров перед удалением; порядок очистки — сначала авто, потом педов.
- **⛔ ABI `CVector`** (критичный, регресс миграции SDK): пользовательский
  copy-ctor → clang/MinGW передавал класс по скрытой ссылке, а игра ждёт 12
  байт по значению → вылет на ~15 местах `SetObjective(OBJ, pos)`. Фикс:
  `= default` + удаление определения; либа пересобрана. **Не откатывать**
  (детали — `КАК_НАПИСАТЬ_МИССИИ.md` §5.10).
- **Гонка потоков `Createcar`**: detached-поток `expectations` дёргал `Command<>`
  из фонового потока (общий `static CRunningScript`) — нестабильное зависание.
  Поток убран; release модели — сразу после create на главном потоке.
- **Диагностика**: `cpp_trace`/`cpp_tracef` пишут в `trace.txt` (префикс `C++|`,
  flush) — segfault в лог не пишется, трейс единственный способ поймать место.
- **`DoorsExample::EnableDoorEvent`**: неинициализированный `CDamageManager* p`
  → `&automobile->m_carDamage` (фикс миграции SDK 15.09).

### Осталось (сознательно не трогается — «работает — не трогай»)

**Баги (UB):**
1. `spite::active()` присваивает значения **локальным** переменным (`bool switc = switc1;`)
   вместо статических членов — функция ничего не делает.
2. `reload()`: `for (auto L : luastate) { luastate.pop_front(); }` — изменение контейнера
   во время итерации, неопределённое поведение (и состояния не закрываются `lua_close`).
3. `state::~state()` закомментирован — Lua-состояния никогда не освобождаются,
   плюс `lua_gc(LUA_GCSTOP)` отключает сборщик мусора → рост памяти при перезагрузках.

**Гонки и потокобезопасность:**
4. Скриптовые потоки (`std::thread(...).detach()`) и общий `list<lua_State*>`
   без mutex (mutex используется только трейсером). Работает, пока
   каждая миссия пишет только в свои структуры; менять модель — опасно.
   **C++-сторож миссии — исключение**: он единственный (join старого) и
   трогает только чтение состояния игрока + флаг.
5. Вызовы игровых функций из посторонних потоков небезопасны — игра
   однопоточная. Надёжнее: накапливать задачи в очередь и исполнять их в
   `gameProcessEvent` (правило зафиксировано в `КАК_НАПИСАТЬ_МИССИИ.md` §12).

**Совместимость:**
6. Жёсткие адреса `0x869630`, `0x9B5F08`, `0x974B2C`, `0x978748` — работает только
   на gta-vc.exe 1.0 (EN). На других версиях — краш. Лучше использовать
   `patch::get`/сигнатуры или поля классов SDK.
7. `std::experimental::filesystem` — устаревший API (C++14); в новом стандарте `<filesystem>`.
8. Кириллическое имя `setobjоcoordes` содержит русскую «о» внутри латиницы —
   невидимая ловушка при наборе имени функции в Lua.

## 7. Рекомендации

**Выполнено** (можно не поднимать): «очередь команд» — правило «игровые вызовы
только из главного потока» закреплено в `КАК_НАПИСАТЬ_МИССИИ.md` §12 и
соблюдается (detached-поток `expectations` удалён); сырые адреса заменены на
символы SDK при миграции 15.09; API задокументировано —
`LUA_API_СПРАВОЧНИК.md` (1506 функций; устарел на 13 — см. раздел 9.2
`ОПИСАНИЕ_проекта_для_ИИ.md`).

**Осталось актуальным** (сознательно откладывается — «работает — не трогай»):

1. Исправить `spite::active()` (присвоение локальным вместо статических
   членов) и `reload()` (erase-в-итерации + нет `lua_close`) — теоретические
   источники крашей (раздел 6, «Осталось»).
2. Завести один глобальный `std::mutex` на `luastate`/`listfile` — пока
   работает без него (mutex используется только трейсером).
3. Добавить `lua_close()` в деструктор `state` и вернуть включённым GC между
   перезагрузками — рост памяти при частых reload (Ctrl).
4. При новом пополнении API — перегенерировать `LUA_API_СПРАВОЧНИК.md`
   (старый генератор удалён, написать новый парсер plugin.cpp + funcs.h).

## 8. Состояние установки в игре

Проверено 18.09.2026: `lualoader.asi` (6 390 263 Б) лежит в корне игры, папка
`lualoader/` содержит 5 скриптов: `mod.lua` (обёртки и миссионный API),
`autoaim.lua` (внутри тестовая миссия по **VK_J**), `write_coordinate.lua`,
`open and close first bridge.lua`, **`misson riot.lua`** (перенесённая
оригинальная миссия «Мятеж», см. `КАК_ПЕРЕНОСИТЬ_МИССИИ.md`).
`log.txt` от 18.09 03:26 подтверждает загрузку всех пяти без ошибок;
`trace.txt` крутится на этапе подъезда к стройплощадке — вылетов нет.

## 9. Обновление plugin-sdk и пересборка (14.09.2026)

**SDK:** перекачан и сверен с origin/master — локальный клон совпадает с HEAD
репозитория DK22Pac/plugin-sdk: коммит `15f15b60` ("SA fix renderware", 08.09.2026).
С момента предыдущей сборки (24.08.2026, коммит `5d7c5611`) в SDK пришло **13 новых
коммитов** (31.08–08.09.2026): маршрут workflow, `CAERadioTrackManager`,
`CBaseModelInfo`, `CPathNode`, `CTheZones`/`CCullZones`, `RenderWare`, `Types.h`/`PluginBase.h`,
`injector.hpp`, premake5.

### Как именно поменялся SDK (дифф `5d7c5611..15f15b60`)

Дифф — **37 файлов, +267/−229 строк**, и major'a затронута только ветка **SA**:

| Файл | Что изменилось | Влияние на lualoader (VC) |
|---|---|---|
| `plugin_sa/game_sa/*` (CAERadioTrackManager, CBaseModelInfo, CPathNode, CTheZones, CCullZones, RenderWare и др., ~33 файла) | SA-классы/поля, C++23-совместимость, VALIDATE_OFFSET | нет — `plugin_vc` не тронут |
| `shared/Types.h` (**новый**, +22) | типы `UInt8..UInt64`, `Int8..Int64`, `Bool8/16` | только через `PluginBase.h`; конфликтов нет |
| `shared/PluginBase.h` (+1) | `#include "Types.h"` | безвреден |
| `injector/injector.hpp` (+2/−2) | **фикс бага** `auto_pointer::is_null()`: было `p != nullptr`, стало `p == nullptr` (и `operator bool` теперь корректен) | lualoader не использует `injector::auto_pointer` (grep по funcs.h/plugin.cpp пуст) |
| `tools/premake/*`, `.github/workflows/*` | premake5 (.lua + .exe), CI-джобы | нет |

Итог: **plugin_vc и shared-зависимости lualoader не менялись** → правки исходников
не потребовались, компиляция прошла без ошибок с первой попытки.

### Пересборка (аккуратно, в целевой папке)

1. SDK-библиотека пересобрана заново (после `rm -rf output`):
   `premake5 gmake` (env `PLUGIN_SDK_DIR=/tmp/opencode/plugin-sdk`, симлинк из-за пробелов
   в пути) → `make config=release Plugin_VC` → **`Plugin-SDK/output/lib/libPlugin_VC.a`
   (5 551 116 Б, 14.09.2026 09:18)**. Ошибок нет, пара штатных предупреждений
   (`-Wdeprecated-enum-enum-conversion`, `NOMINMAX macro redefined`).
2. lualoader пересобран без изменения кода (`./build.sh`, clang `--target=i686-w64-mingw32`):
   **`../lualoader.asi` (5 307 124 Б, 09:19) — PE32 x86 DLL**, зависимости KERNEL32/msvcrt
   (предупреждения «duplicate section … different size» — штатные для clang↔libstdc++).
3. Версия Lua в исходниках — **5.4.9** (встроена статически, миграция от 13.09.2026,
   см. `lualoader-src/ОТЧЁТ_миграция.md`); движок проекта не менялся.

Проверка в игре не выполнялась (нужен ручной запуск `gta-vc.sh`): после первого запуска
в `lualoader/log.txt` появится свежая дата загрузки скриптов.


---

## 10. Сводка серии работ 16–18.09.2026

| Что | Где | Статус |
|---|---|---|
| Спец-акторы в `Createped` (пол → базовый пед → скин, без загрузки спец-модели) | funcs.h, `КАК_НАПИСАТЬ_МИССИИ.md` §5.1 | ✅ проверено в игре |
| Тотальная регистрация созданных объектов (8 списков, `destroy()` чистит всё) | funcs.h/plugin.cpp | ✅ |
| Единый `destroy()` + безопасная высадка сидящих (auto → ped → obj) | funcs.h, §5.9 | ✅ |
| C++-трассировка `cpp_trace`/`cpp_tracef` → `trace.txt` | funcs.h | ✅ |
| **ABI `CVector`** — корневой фикс, `= default` copy-ctor | Plugin-SDK + пересборка либы | ✅ **не откатывать** |
| Гонка `Createcar` (detached-поток убран, канон SCM) | funcs.h | ✅ |
| C++-сторож миссии `Getflagmission`/`Setflagmission` (один поток) | funcs.h:3434/3441, plugin.cpp:379-380 | ✅ |
| **+9 функций переноса поведений** (толпа/wait_state, только-игрок, объекты-цели, выпадение оружия, анимации, настроение) | funcs.h:2810-2932, plugin.cpp:363-371 | ✅ |
| Перенос миссии «Riot» (v3, все 9 функций + луа-файл) | `lualoader/misson riot.lua` | ⏳ полный прогон не подтверждён |
| Документы: `КАК_ПЕРЕНОСИТЬ_МИССИИ.md` (новый), `ЗАМЕТКА_О_МИССИЯХ.md` (флаг/сторож) | lualoader-src | ✅ |

---
*Первоначальный отчёт составлен opencode 24.08.2026 по исходникам plugin.cpp (1688 строк),
funcs.h (9303 строки) и скриптам репозитория egor230/lualoader. Дополнен 14.09.2026
по итогам обновления plugin-sdk и пересборки; 15–18.09.2026 — массовое расширение
API (1519 функций), системные фиксы (ABI, потоки, удаления, сторож) и перенос
миссии Riot. funcs.h — 25133 строки, plugin.cpp — 2686 строк.*

