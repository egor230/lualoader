#include "funcs.h"

int funs(lua_State* L) {// список функций.

	//set_path_to_module(L); // уст путь к модулю.
	lua_register(L, "findplayer", findplayer); // 1 найти томми.
	lua_register(L, "setpedhealth", setpedhealth); // 2 установить здоровье педу.
	lua_register(L, "setarmour", setarmour); // 3 установить броню педу.
	lua_register(L, "wait", wait); // 4 задержка.
	lua_register(L, "getpedhealth", getpedhealth); // 5 получить здоровье педа.
	lua_register(L, "getpedangle", getpedangle); // 6 получить угол педа.
	lua_register(L, "worldcoord", worldcoord); // 7 Перевод в мировые координаты.
	lua_register(L, "getpedcoordinates_on_x", getpedcoordinates_on_x); // 8 Получить мировую координату по x для педа.
	lua_register(L, "getpedcoordinates_on_y", getpedcoordinates_on_y); // 9 Получить мировую координату по y для педа.
	lua_register(L, "setarmour", setarmour); // 10 получить броню.
	lua_register(L, "givemoney", givemoney); // 11 дать денег педу.
	lua_register(L, "keypress", keypress); // 12 проверка на нажатие клавиш.
	lua_register(L, "printmessage", printmessage); // 13 вывод сообщение.
	lua_register(L, "getpedcoordes", getpedcoordes); // 14 получить координаты педа.
	lua_register(L, "randomfindped", randomfindped); // 15 получить рандомного педа.
	lua_register(L, "incar", incar); // 16 проверка пед в авто?.
	lua_register(L, "loadmodel", loadmodel); // 17 загрузить модель.
	lua_register(L, "availablemodel", availablemodel); // 18 проверка на загруженность модели.
	lua_register(L, "releasemodel", releasemodel); // 19 удалить модель из памяти.
	lua_register(L, "createcar", createcar); // 20 создать авто на координатах на координатах.
	lua_register(L, "createped", createped); // 21 создать педа на координатах.
	lua_register(L, "load_requested_models", load_requested_models); // 22 поставить модель на загрузить вне очереди.
	lua_register(L, "giveweaponped", giveweaponped); // 23 дать педу оружие.
	lua_register(L, "ped_sprint_to_point", ped_sprint_to_point); // 24 пед делает спринт к точке.
	lua_register(L, "ped_walk_to_point", ped_walk_to_point); // 25 Пед идет к точке.
	lua_register(L, "kill_ped_on_foot", kill_ped_on_foot); // 26 убить педа пешком.
	lua_register(L, "kill_char_any_means", kill_char_any_means); // 27 убить педа любыми средствами.
	lua_register(L, "ped_aim_at_ped", ped_aim_at_ped); // 28 пед целиться в педе.
 	lua_register(L, "is_current_weapon_ped", is_current_weapon_ped); // 29 проверить текущее оружие.
 	lua_register(L, "set_current_weapon_ped", set_current_weapon_ped); // 30 установить текущее оружие педа (01B9).
 	lua_register(L, "set_ped_stay_when_attacked", set_ped_stay_when_attacked); // 31 пед держит позицию при атаке (0350).
	lua_register(L, "set_ped_wait_state", set_ped_wait_state); // 32 установить состояние ожидания педа (0372).
	lua_register(L, "set_ped_personality", set_ped_personality); // 33 установить личность педа (0243).
	lua_register(L, "set_ped_only_damaged_by_player", set_ped_only_damaged_by_player); // 34 пед ранен только игроком (02A9).
	lua_register(L, "is_ped_health_greater", is_ped_health_greater); // 35 здоровье педа больше? (0184).
	lua_register(L, "make_obj_targettable", make_obj_targettable); // 36 в объект можно целиться (035D).
	lua_register(L, "get_dead_char_pickup_coords", get_dead_char_pickup_coords); // 37 координаты трупного пикапа (04A5).
	lua_register(L, "set_ped_mood", set_ped_mood); // 38 настроение педа (04E3).
	lua_register(L, "request_animation", request_animation); // 39 запросить анимацию (04ED).
	lua_register(L, "remove_animation", remove_animation); // 40 удалить анимацию (04EF).
	lua_register(L, "create_marker_actor", create_marker_actor); // 41 создать маркер над педом.
	lua_register(L, "removemarker", removemarker); // 42 удалить маркер.
	lua_register(L, "setpedcoordes", setpedcoordes); // 43 установить координаты для педа.
	lua_register(L, "remove_car", remove_car); // 44 удалить авто.
	lua_register(L, "car_in_water", car_in_water); // 45 проверка авто в воде.
	lua_register(L, "set_wanted", set_wanted); // 46 уcтановить уровень розыска.
	lua_register(L, "ped_in_point_in_radius", ped_in_point_in_radius); // 47 проверить находится пед в координатах с радиусом.
	lua_register(L, "create_sphere", create_sphere); // 48 создать сферу.
	lua_register(L, "clear_wanted", clear_wanted); // 49 убрать уровень розыска.
	lua_register(L, "getcarhealth", getcarhealth); // 50 получить кол-во здоровья авто.
	lua_register(L, "setcarhealth", setcarhealth); // 51 установить здоровье авто.
	lua_register(L, "remove_sphere", remove_sphere); // 52 удалить сферу.
	lua_register(L, "remove_ped", remove_ped); // 53 удалить педа.
	lua_register(L, "kill_ped", kill_ped); // 54 убить педа.
	lua_register(L, "getflagmission", getflagmission); // 55 проверка флага миссии.
	lua_register(L, "setflagmission", setflagmission); // 56 уcтановить флага миссии.
	lua_register(L, "Getflagmission", Getflagmission); // 57 большая: C++ + сторож миссии.
	lua_register(L, "Setflagmission", Setflagmission); // 58 большая: C++ + сторож миссии.
	lua_register(L, "showtext", showtext); // 59 Вывод особого текста на экран.
	lua_register(L, "remove_blip", remove_blip); // 60 удалить метку с карты.
	lua_register(L, "createblip", createblip); // 61 создать метку карте.
	lua_register(L, "play_sound", play_sound); // 62 проиграть мелодию.
	lua_register(L, "isped", isped); // 63 проверка это пед?
	lua_register(L, "isvehicle", isvehicle); // 64 проверка это транспорт?.
	lua_register(L, "cardrive", cardrive); // 65 авто едет в точку.
	lua_register(L, "setcarspeed", setcarspeed); // 66 установить скорость авто.
	lua_register(L, "opendoorcar", opendoorcar); // 67 открыть дверь авто.
	lua_register(L, "randomfindcar", randomfindcar); // 68 Найти случайное авто.
	lua_register(L, "getcarcoordes", getcarcoordes); // 69 получить координаты авто.
	lua_register(L, "create_money_pickup", create_money_pickup); // 70 создать пачку денег.
	lua_register(L, "getcarcoordinates_on_x", getcarcoordinates_on_x); // 71 Получить мировую координату по x для авто.
	lua_register(L, "getcarcoordinates_on_y", getcarcoordinates_on_y); // 72 Получить мировую координату по y для авто.
	lua_register(L, "car_in_point_in_radius", car_in_point_in_radius); // 73 проверить находится авто в координатах с радиусом.
	lua_register(L, "setdrivingstyle", setdrivingstyle); // 74 установить стиль езды авто.
	lua_register(L, "findped", findped); // 75 найти педа в пуле.
	lua_register(L, "create_weapon_pickup", create_weapon_pickup); // 76 создать пикап оружие.
	lua_register(L, "create_pickup", create_pickup); // 77 создать пикап.
	lua_register(L, "remove_pickup", remove_pickup); // 78 удалить пикап.
	lua_register(L, "picked_up", picked_up); // 79 проверка пикап подобран.
	lua_register(L, "play_voice", play_voice); // 80 Проиграть голос.
	lua_register(L, "fade", fade); // 81 затенение, просветления.
	lua_register(L, "draw_corona", draw_corona); // 82 создать корону(чекпойнт);.
	lua_register(L, "sound_coordinate", sound_coordinate); // 83 Проиграть звук в координатах
	lua_register(L, "show_text_styled", show_text_styled); // 84 Вывести игровой текст.
	lua_register(L, "setcarangle", setcarangle); // 85 установить угол авто.
	lua_register(L, "createmarker", createmarker); // 86 создать маркер на карте.
	lua_register(L, "setsizemarker", setsizemarker); // 87 установить размер маркера.
	lua_register(L, "cheat", checkcheat); // 88 чит код введен.
	lua_register(L, "destroy", destroy); // 89 удаления объектов из памяти при перезагрузки скрипта.
	lua_register(L, "yield", my_yield); // 90 приостановить выполнение скрипта.
	lua_register(L, "setcardrive", setcardrive); // 91 установить водителя для авто.
	lua_register(L, "setcarpassenger", setcarpassenger); // 92 установить пассажира для авто.
	lua_register(L, "setcarfirstcolor", setcarfirstcolor); // 93 установить первый цвет авто.
	lua_register(L, "setcarseconscolor", setcarseconscolor); // 94 установить второй цвет авто.
	lua_register(L, "set_traffic", set_traffic); // 95 установить плотномть трафика транспорта.
	lua_register(L, "create_marker_car", create_marker_car); // 96 создать маркер над авто.
	lua_register(L, "car_explode", car_explode); // 97 взрывать авто.
	lua_register(L, "is_car_stopped", is_car_stopped); // 98 авто остановилось.
	lua_register(L, "create_explosion", create_explosion); // 99 Создать взрыв на координатах.
	lua_register(L, "set_status_engine", set_status_engine); // 100 установить состояние двигателя авто.
	lua_register(L, "player_defined", player_defined); // 101 пед существует.
	lua_register(L, "setclock", setclock); // 102 задать время.
	lua_register(L, "arrested", arrested); // 103 пед арестован?
	lua_register(L, "lockstatus", lockstatus); // 104 статус двери авто.
	lua_register(L, "create_marker_pickup", create_marker_pickup); // 105 создать маркер над пикапом.
	lua_register(L, "create_obj", createobj); // 106 создать объект.
	lua_register(L, "remove_obj", remove_obj); // 107 удалить объект.
	lua_register(L, "setobjоcoordes", setobjоcoordes); // 108 установить координаты для объект.
	lua_register(L, "getobjcoordes", getobjcoordes); // 109 получить координаты объекта.
	lua_register(L, "create_marker_obj", create_marker_obj); // 110 создать маркер над объектом.
	lua_register(L, "isobject", isobject); // 111 проверка это объект?.
	lua_register(L, "setpedangle", setpedangle); // 112 установить угол педа.
	lua_register(L, "setcaraction", setcaraction); // 113 установить поведение авто.
	lua_register(L, "move_obj", move_obj); // 114 двигать объект.
	lua_register(L, "move_rotate", move_rotate); // 115 вращать объект.
	lua_register(L, "getobjangle", getobjangle); // 116 получить угол объекта.
	lua_register(L, "findcar", findcar); // 117 Найти авто.
	lua_register(L, "setcartask", setcartask); // 118 установить задачу авто.
	lua_register(L, "setcarcoordes", setcarcoordes); // 119 установить координаты авто.
	lua_register(L, "is_car_stuck", is_car_stuck); // 120 03CE: car 12@ stuck если машина застряла.
	lua_register(L, "is_car_upsidedown", is_car_upsidedown); // 121 01F4: car 12@ flipped если машина перевернута.
	lua_register(L, "is_car_upright", is_car_upright); // 122 020D: car 12@ flipped если указанный автомобиль стоит на колесах.
	lua_register(L, "find_road_for_car", find_road_for_car); // 123 найти дорогу.
	lua_register(L, "setcarstrong", setcarstrong); // 124 сделать авто устойчивым.
	lua_register(L, "putincar", putincar); // 125 переместить педа в авто.
	lua_register(L, "print_front", game_font_print); // 126 вывести особенный игровой текст.
	lua_register(L, "star_timer", star_timer); // 127 включить таймер.
	lua_register(L, "stop_timer", stop_timer); // 128 остановить таймер.
	lua_register(L, "timer_donw", timer_donw); // 129 таймер на уменьшение.
	lua_register(L, "ped_attack_car", ped_attack_car); // 130 пед атакует авто.
	lua_register(L, "ped_frozen", ped_frozen); // 131 заморозить игpока.
	lua_register(L, "hold_cellphone", hold_cellphone); // 132 поднять телефон.
	lua_register(L, "car_lastweapondamage", car_lastweapondamage); // 133 номер оружие, которое нанесло урон авто.
	lua_register(L, "car_currentgear", car_currentgear); // 134 текущая передача авто.
	lua_register(L, "getcar_model", getcar_model); // 135 получить модель авто.
	lua_register(L, "setcarsiren", setcarsiren); // 136 установить сирену для авто.
	lua_register(L, "ped_car_as_driver", ped_car_as_driver); // 137 пед садится в авто как водитель.
	lua_register(L, "ped_car_as_passenger", ped_car_as_passenger); // 138 пед садится в авто как пассажир.
	lua_register(L, "ped_atack", ped_atack); // 139 пед бьет.
	lua_register(L, "show_text_gtx", show_text_gtx); // 140 вывести игровой текст.
	lua_register(L, "camera_at_point", camera_at_point); // 141 переместить камеру в координатах.
	lua_register(L, "restore_camera", restore_camera); // 142 восстановить камеру.
	lua_register(L, "is_wanted_level", is_wanted_level); // 143 проверить уровень розыска.
	lua_register(L, "set_camera_position", set_camera_position); // 144 установить камеру в координатах.
	lua_register(L, "flash_hud", flash_hud); // 145 Мигание элементов HUD.
	lua_register(L, "set_radio", set_radio); // 146 установить радио.
	lua_register(L, "set_car_tires", set_car_tires); // 147 проколоть шину.
	lua_register(L, "create_spec_ped", create_spec_ped); // 148 создать спец педа.
	lua_register(L, "set_wheel_status", set_wheel_status); // 149 установить состояния шин авто.
	lua_register(L, "set_skin", set_skin); // 150 установить скин педа.
	lua_register(L, "remove_spec_ped", remove_spec_ped); // 151 удалить спец педа.
	lua_register(L, "go_to_route", go_to_route); // 152 установить маршрут авто.
	lua_register(L, "add_stuck_car_check", add_stuck_car_check); // 153 условия для того, чтобы авто считалась застрявшей.
	lua_register(L, "load_scene", load_scene); // 154 загрузить модели на координатах заранее.
	lua_register(L, "ped_anim", ped_anim); // 155 анимация.
	lua_register(L, "del_anim", del_anim); // 156 удалить анимацию.
	lua_register(L, "get_current_name_luascript", get_current_name_luascript); // 157 получить имя текущего lua файла.
	lua_register(L, "star_mission_marker", star_mission_marker); // 158 создать маркер для миссии.
	lua_register(L, "getobjcoordinates_on_x", getobjcoordinates_on_x); // 159 Получить мировую координату по x для объекта.
	lua_register(L, "getobjcoordinates_on_y", getobjcoordinates_on_y); // 160 Получить мировую координату по y для объекта.
	lua_register(L, "set_widescreen", set_widescreen); // вк // 161 вкл/выкл широкий экран.
	lua_register(L, "follow_the_leader", follow_the_leader); //149 //01DE // 162 DE / 01DF следовать за лидером.
	lua_register(L, "getcarspeed", getcarspeed); // 163 получить скорость авто.
	lua_register(L, "newthread", newthread); // 164 запуск функции в новом потоке.
	lua_register(L, "Getcameracoordes", Getcameracoordes); // 165 получить координаты камеры.

	lua_register(L, "remove_all_weapons_ped", remove_all_weapons_ped); // 166 удалить все оружия педа.
	lua_register(L, "Getweaponslot", Getweaponslot); // 167 получить номер слота по типу оружия.
	lua_register(L, "get_damage_weapon_ped", get_damage_weapon_ped); // 168 получить последний урон педа от оружия.
	lua_register(L, "get_aimgun_ped", get_aimgun_ped); // 169 получить показатель цели педа.
	lua_register(L, "get_ammo_weapon_ped", get_ammo_weapon_ped); // 170 получить кол-во патроны текущего оружие педа.
	lua_register(L, "createfireonped", createfireonped); // 171 создать огонь на педе.
	lua_register(L, "createfireoncar", createfireoncar); // 172 создать огонь на авто.
	lua_register(L, "createfireoncords", createfireoncords); // 173 создать огонь на координатах.
	lua_register(L, "remove_fire", remove_fire); // 174 удалить огонь.
	lua_register(L, "ped_shutdown", ped_shutdown); // 175
	lua_register(L, "is_ped_damage_from_ped", is_ped_damage_from_ped); // 176 получил ли пед урон от педа.
	lua_register(L, "is_targetting_in_ped", is_targetting_in_ped); // 177 игрок целиться в педа.
	lua_register(L, "Remove_weapon_model", Remove_weapon_model); // 178 удалить оружие у педа.
	lua_register(L, "Createped", Createped); // 179 макрос создать педа.
	lua_register(L, "Createcar", Createcar); // 180 макрос создать авто на координатах.
	lua_register(L, "Giveweaponped", Giveweaponped); // 181 макрос дать педу оружие и патроны.
	lua_register(L, "Opendoorcar", Opendoorcar); // 182 Макрос открыть все двери авто.
	lua_register(L, "Create_weapon_pickup", Create_weapon_pickup); // 183 макрос создать пикап оружие.
	lua_register(L, "Create_pickup", Create_pickup); // 184 макрос создать пикап.
	lua_register(L, "Get_model_and_type_current_weapon_ped", Get_model_and_type_current_weapon_ped); // 185 макрос получить модель и тип текущего оружие педа.
	lua_register(L, "is_ped_in_car", is_ped_in_car); // 186 игрок в авто?
	lua_register(L, "ped_car", ped_car); // 187 авто педа.
	lua_register(L, "wanted_level", wanted_level); // 188 получить уровень розыска.
	lua_register(L, "get_model_current_weapon_ped", get_model_current_weapon_ped); // 189 макрос получить модель текущего оружие педа.
	lua_register(L, "get_type_current_weapon_ped", get_type_current_weapon_ped); // 190 макрос получить тип текущего оружие педа.
	lua_register(L, "set_camera_and_point", set_camera_and_point); // 191 установить и переместить камеру в координатах.
	lua_register(L, "get_damage_current_weapon", get_damage_current_weapon); // 192 получить уровень урона текущего оружие.
	lua_register(L, "set_damage_current_weapon", set_damage_current_weapon); // 193 установить уровень урона текущего оружие педа.
	lua_register(L, "set_clip_current_weapon", set_clip_current_weapon); // 194 установить объём обоймы текущего оружие педа.
	lua_register(L, "get_ped_state", get_ped_state); // 195 получить состояние педа.
	lua_register(L, "get_ped_last_state", get_ped_last_state); // 196 получить прошлое состояние педа.
	lua_register(L, "set_ped_state", set_ped_state); // 197 установить состояние педа.
	lua_register(L, "get_ped_heading", get_ped_heading); // 198 получить текущий угол поворота педа.
	lua_register(L, "set_ped_heading_goal", set_ped_heading_goal); // 199 установить желаемый угол поворота педа.
	lua_register(L, "get_ped_heading_goal", get_ped_heading_goal); // 200 получить желаемый угол поворота педа.
	lua_register(L, "set_ped_heading_rate", set_ped_heading_rate); // 201 установить скорость поворота педа.
	lua_register(L, "get_ped_heading_rate", get_ped_heading_rate); // 202 получить скорость поворота педа.
	lua_register(L, "get_ped_enter_type", get_ped_enter_type); // 203 получить тип посадки педа в авто.
	lua_register(L, "get_ped_from_vehicle", get_ped_from_vehicle); // 204 получить авто из которого вышел пед.
	lua_register(L, "ped_can_be_deleted", ped_can_be_deleted); // 205 можно ли удалить педа из мира.
	lua_register(L, "ped_is_gang_member", ped_is_gang_member); // 206 пед член банды.
	lua_register(L, "ped_is_player", ped_is_player); // 207 пед это игрок.
	lua_register(L, "ped_is_in_control", ped_is_in_control); // 208 пед управляем.
	lua_register(L, "ped_is_shootable", ped_is_shootable); // 209 в педа можно стрелять.
	lua_register(L, "ped_is_pointer_valid", ped_is_pointer_valid); // 210 указатель на педа валиден.
	lua_register(L, "ped_set_dead", ped_set_dead); // 211 пометить педа мертвым.
	lua_register(L, "ped_dress", ped_dress); // 212 пед надевает одежду.
	lua_register(L, "ped_calculate_new_orientation", ped_calculate_new_orientation); // 213 пересчитать ориентацию педа.
	lua_register(L, "ped_set_move_anim", ped_set_move_anim); // 214 задать педу анимацию движения.
	lua_register(L, "ped_set_get_up", ped_set_get_up); // 215 пед встает с земли.
	lua_register(L, "ped_set_jump", ped_set_jump); // 216 пед прыгает.
	lua_register(L, "ped_set_landing", ped_set_landing); // 217 пед приземляется.
	lua_register(L, "ped_set_idle", ped_set_idle); // 218 пед переходит в состояние покоя.
	lua_register(L, "ped_fall", ped_fall); // 219 пед падает.
	lua_register(L, "ped_turn_body", ped_turn_body); // 220 пед поворачивает корпус к цели.
	lua_register(L, "ped_update_position", ped_update_position); // 221 пед обновить позицию по анимации.
	lua_register(L, "ped_restore_heading_rate", ped_restore_heading_rate); // 222 вернуть скорость поворота педа.
	lua_register(L, "ped_restore_head_position", ped_restore_head_position); // 223 вернуть голове положение.
	lua_register(L, "ped_restore_previous_state", ped_restore_previous_state); // 224 вернуть педу прошлое состояние.
	lua_register(L, "ped_restore_previous_objective", ped_restore_previous_objective); // 225 вернуть педу прошлую цель.
	lua_register(L, "ped_stop_non_partial_anims", ped_stop_non_partial_anims); // 226 остановить базовые анимации педа.
	lua_register(L, "ped_restart_non_partial_anims", ped_restart_non_partial_anims); // 227 возобновить базовые анимации педа.
	lua_register(L, "ped_clear_aim_flag", ped_clear_aim_flag); // 228 сбросить флаг прицеливания.
	lua_register(L, "ped_clear_look_flag", ped_clear_look_flag); // 229 сбросить флаг взгляда.
	lua_register(L, "ped_clear_chat", ped_clear_chat); // 230 пед прекращает болтать.
	lua_register(L, "ped_clear_attack", ped_clear_attack); // 231 сбросить у педы атаку.
	lua_register(L, "ped_clear_attack_anim", ped_clear_attack_anim); // 232 сбросить атаку с удалением анимации.
	lua_register(L, "ped_clear_seek", ped_clear_seek); // 233 сбросить у педа поиск цели.
	lua_register(L, "ped_clear_follow_path", ped_clear_follow_path); // 234 сбросить следование по маршруту.
	lua_register(L, "ped_clear_wait_state", ped_clear_wait_state); // 235 сбросить состояние ожидания.
	lua_register(L, "ped_clear_point_gun_at", ped_clear_point_gun_at); // 236 опустить наведенное оружие.
	lua_register(L, "ped_answer_mobile", ped_answer_mobile); // 237 пед отвечает на телефон.
	lua_register(L, "ped_clear_answer_mobile", ped_clear_answer_mobile); // 238 сбросить разговор по телефону.
	lua_register(L, "ped_face_phone", ped_face_phone); // 239 пед поворачивается к телефону.
	lua_register(L, "ped_make_phonecall", ped_make_phonecall); // 240 пед звонит по телефону.
	lua_register(L, "ped_pause", ped_pause); // 241 пед замирает.
	lua_register(L, "ped_mug", ped_mug); // 242 пед грабит ближайшего педа.
	lua_register(L, "ped_solicit", ped_solicit); // 243 пед предлагает услуги.
	lua_register(L, "ped_buy_ice_cream", ped_buy_ice_cream); // 244 пед покупает мороженое.
	lua_register(L, "ped_look_for_sexy_cars", ped_look_for_sexy_cars); // 245 пед ищет красивые авто.
	lua_register(L, "ped_look_for_sexy_peds", ped_look_for_sexy_peds); // 246 пед ищет красивых педов.
	lua_register(L, "get_car_driver", get_car_driver); // 247 получить водителя авто.
	lua_register(L, "get_car_passenger", get_car_passenger); // 248 получить пассажира авто по месту.
	lua_register(L, "get_car_num_passengers", get_car_num_passengers); // 249 получить кол-во пассажиров авто.
	lua_register(L, "get_car_max_passengers", get_car_max_passengers); // 250 получить макс кол-во пассажиров авто.
	lua_register(L, "get_car_steer_angle", get_car_steer_angle); // 251 получить угол руля авто.
	lua_register(L, "set_car_steer_angle", set_car_steer_angle); // 252 установить угол руля авто.
	lua_register(L, "get_car_gas_pedal", get_car_gas_pedal); // 253 получить педаль газа авто.
	lua_register(L, "get_car_brake_pedal", get_car_brake_pedal); // 254 получить педаль тормоза авто.
	lua_register(L, "get_car_engine_on", get_car_engine_on); // 255 работает ли двигатель авто.
	lua_register(L, "set_car_engine_on", set_car_engine_on); // 256 вкл/выкл двигатель авто.
	lua_register(L, "get_car_lights_on", get_car_lights_on); // 257 включены ли фары авто.
	lua_register(L, "set_car_lights_on", set_car_lights_on); // 258 вкл/выкл фары авто.
	lua_register(L, "get_car_handbrake_on", get_car_handbrake_on); // 259 включен ли ручник авто.
	lua_register(L, "set_car_handbrake_on", set_car_handbrake_on); // 260 вкл/выкл ручник авто.
	lua_register(L, "get_car_locked", get_car_locked); // 261 заблокировано ли авто.
	lua_register(L, "set_car_locked", set_car_locked); // 262 заблокировать/разблокировать авто.
	lua_register(L, "get_car_is_van", get_car_is_van); // 263 авто фургон.
	lua_register(L, "get_car_is_bus", get_car_is_bus); // 264 авто автобус.
	lua_register(L, "get_car_is_big", get_car_is_big); // 265 авто большая машина.
	lua_register(L, "set_car_comedy_controls", set_car_comedy_controls); // 266 шуточное управление авто.
	lua_register(L, "set_car_hide_occupants", set_car_hide_occupants); // 267 скрыть седоков авто.
	lua_register(L, "get_car_tires_invulnerable", get_car_tires_invulnerable); // 268 шины авто неуязвимы.
	lua_register(L, "set_car_tires_invulnerable", set_car_tires_invulnerable); // 269 шины авто неуязвимы вкл/выкл.
	lua_register(L, "get_car_door_lock", get_car_door_lock); // 270 получить блокировку дверей.
	lua_register(L, "set_car_door_lock", set_car_door_lock); // 271 установить блокировку дверей.
	lua_register(L, "get_car_last_damager", get_car_last_damager); // 272 получить последний урон авто.
	lua_register(L, "get_car_time_of_death", get_car_time_of_death); // 273 получить время смерти авто.
	lua_register(L, "get_car_weapon_damage", get_car_weapon_damage); // 274 тип оружия последнего урона авто.
	lua_register(L, "get_car_vehicle_class", get_car_vehicle_class); // 275 получить класс авто.
	lua_register(L, "car_play_horn", car_play_horn); // 276 авто сигналит.
	lua_register(L, "car_extinguish_fire", car_extinguish_fire); // 277 потушить пожар на авто.
	lua_register(L, "car_uses_siren", car_uses_siren); // 278 есть ли сирена у авто.
	lua_register(L, "car_is_law_enforcement", car_is_law_enforcement); // 279 авто правопорядка.
	lua_register(L, "car_has_roof", car_has_roof); // 280 у авто есть крыша.
	lua_register(L, "car_is_open_top", car_is_open_top); // 281 авто кабриолет.
	lua_register(L, "car_is_upside_down", car_is_upside_down); // 282 авто перевернуто.
	lua_register(L, "car_is_on_its_side", car_is_on_its_side); // 283 авто на боку.
	lua_register(L, "car_is_vehicle_normal", car_is_vehicle_normal); // 284 авто в норме.
	lua_register(L, "car_can_be_deleted", car_can_be_deleted); // 285 можно удалить авто.
	lua_register(L, "car_can_ped_enter", car_can_ped_enter); // 286 можно сесть в авто.
	lua_register(L, "car_can_doors_be_damaged", car_can_doors_be_damaged); // 287 двери могут быть повреждены.
	lua_register(L, "car_activate_bomb", car_activate_bomb); // 288 активировать бомбу.
	lua_register(L, "car_activate_bomb_when_entered", car_activate_bomb_when_entered); // 289 бомба при посадке.
	lua_register(L, "car_get_height_above_road", car_get_height_above_road); // 290 высота авто над дорогой.
	lua_register(L, "car_is_clear_to_drive_away", car_is_clear_to_drive_away); // 291 можно свободно уехать.
	lua_register(L, "car_is_door_ready", car_is_door_ready); // 292 дверь готова открыться.
	lua_register(L, "car_is_door_fully_open", car_is_door_fully_open); // 293 дверь полностью открыта.
	lua_register(L, "car_is_door_closed", car_is_door_closed); // 294 дверь закрыта.
	lua_register(L, "car_is_door_missing", car_is_door_missing); // 295 дверь отсутствует.
	lua_register(L, "car_shuffle_passengers", car_shuffle_passengers); // 296 пересадить пассажиров.
	lua_register(L, "car_update_passenger_list", car_update_passenger_list); // 297 обновить список пассажиров.
	lua_register(L, "get_car_radio_station", get_car_radio_station); // 298 получить радиостанцию авто.
	lua_register(L, "set_car_radio_station", set_car_radio_station); // 299 установить радиостанцию авто.
	lua_register(L, "get_car_created_by", get_car_created_by); // 300 кем создано авто.
	lua_register(L, "get_car_wanted_stars_on_enter", get_car_wanted_stars_on_enter); // 301 звёзды при входе в авто.
	lua_register(L, "set_car_wanted_stars_on_enter", set_car_wanted_stars_on_enter); // 302 установить звёзды при входе.
	lua_register(L, "get_weapon_range", get_weapon_range); // 303 получить дальность оружия.
	lua_register(L, "set_weapon_range", set_weapon_range); // 304 установить дальность оружия.
	lua_register(L, "get_weapon_firing_rate", get_weapon_firing_rate); // 305 получить скорость стрельбы.
	lua_register(L, "set_weapon_firing_rate", set_weapon_firing_rate); // 306 установить скорость стрельбы.
	lua_register(L, "get_weapon_reload", get_weapon_reload); // 307 получить время перезарядки.
	lua_register(L, "set_weapon_reload", set_weapon_reload); // 308 установить время перезарядки.
	lua_register(L, "get_weapon_speed", get_weapon_speed); // 309 получить скорость пули.
	lua_register(L, "set_weapon_speed", set_weapon_speed); // 310 установить скорость пули.
	lua_register(L, "get_weapon_spread", get_weapon_spread); // 311 получить разброс оружия.
	lua_register(L, "set_weapon_spread", set_weapon_spread); // 312 установить разброс оружия.
	lua_register(L, "get_weapon_radius", get_weapon_radius); // 313 получить радиус поражения.
	lua_register(L, "set_weapon_radius", set_weapon_radius); // 314 установить радиус поражения.
	lua_register(L, "get_weapon_lifespan", get_weapon_lifespan); // 315 получить время жизни снаряда.
	lua_register(L, "set_weapon_lifespan", set_weapon_lifespan); // 316 установить время жизни снаряда.
	lua_register(L, "get_weapon_slot_info", get_weapon_slot_info); // 317 получить слот оружия.
	lua_register(L, "get_ammo_in_clip_ped", get_ammo_in_clip_ped); // 318 получить патроны в обойме.
	lua_register(L, "set_ammo_in_clip_ped", set_ammo_in_clip_ped); // 319 установить патроны в обойме.
	lua_register(L, "get_weapon_state", get_weapon_state); // 320 получить состояние оружия.
	lua_register(L, "weapon_reload_ped", weapon_reload_ped); // 321 перезарядить оружие педа.
	lua_register(L, "weapon_is_melee", weapon_is_melee); // 322 оружие рукопашное.
	lua_register(L, "weapon_is_two_handed", weapon_is_two_handed); // 323 оружие двуручное.
	lua_register(L, "weapon_has_ammo_to_be_used", weapon_has_ammo_to_be_used); // 324 можно использовать оружие.
	lua_register(L, "get_player_money_info", get_player_money_info); // 325 получить деньги игрока.
	lua_register(L, "set_player_money_info", set_player_money_info); // 326 установить деньги игроку.
	lua_register(L, "get_player_display_money", get_player_display_money); // 327 получить отображаемые деньги.
	lua_register(L, "set_player_drive_by", set_player_drive_by); // 328 стрельба на ходу вкл/выкл.
	lua_register(L, "get_wanted_chaos_level", get_wanted_chaos_level); // 329 получить очки хаоса.
	lua_register(L, "set_wanted_chaos_level", set_wanted_chaos_level); // 330 установить очки хаоса.
	lua_register(L, "get_wanted_multiplier", get_wanted_multiplier); // 331 получить множитель розыска.
	lua_register(L, "set_wanted_multiplier", set_wanted_multiplier); // 332 установить множитель розыска.
	lua_register(L, "get_cops_in_pursuit", get_cops_in_pursuit); // 333 получить копов в погоне.
	lua_register(L, "get_max_cops_in_pursuit", get_max_cops_in_pursuit); // 334 получить макс копов в погоне.
	lua_register(L, "set_max_cops_in_pursuit", set_max_cops_in_pursuit); // 335 установить макс копов.
	lua_register(L, "get_max_cop_cars_in_pursuit", get_max_cop_cars_in_pursuit); // 336 получить макс полицейских авто.
	lua_register(L, "set_max_cop_cars_in_pursuit", set_max_cop_cars_in_pursuit); // 337 установить макс полицейских авто.
	lua_register(L, "get_chance_on_roadblock", get_chance_on_roadblock); // 338 получить шанс блокпоста.
	lua_register(L, "set_chance_on_roadblock", set_chance_on_roadblock); // 339 установить шанс блокпоста.
	lua_register(L, "wanted_are_swat_required", wanted_are_swat_required); // 340 нужны ли SWAT.
	lua_register(L, "wanted_are_fbi_required", wanted_are_fbi_required); // 341 нужны ли FBI.
	lua_register(L, "wanted_are_army_required", wanted_are_army_required); // 342 нужна ли армия.
	lua_register(L, "wanted_num_helis_required", wanted_num_helis_required); // 343 нужны ли вертолеты.
	lua_register(L, "wanted_clear_crimes", wanted_clear_crimes); // 344 очистить очередь преступлений.
	lua_register(L, "wanted_reset_police_pursuit", wanted_reset_police_pursuit); // 345 сбросить полицейскую погоню.
	lua_register(L, "wanted_update_wanted_level", wanted_update_wanted_level); // 346 обновить уровень розыска.
	lua_register(L, "wanted_set_no_drop", wanted_set_no_drop); // 347 уровень розыска без понижения.
	lua_register(L, "wanted_cheat_level", wanted_cheat_level); // 348 читерский уровень розыска.
	lua_register(L, "get_wanted_level_before_parole", get_wanted_level_before_parole); // 349 уровень розыска до пароля.
	lua_register(L, "get_wanted_last_decreased_time", get_wanted_last_decreased_time); // 350 время последнего понижения.
	lua_register(L, "force_weather", force_weather); // 351 плавно сменить погоду.
	lua_register(L, "force_weather_now", force_weather_now); // 352 мгновенно сменить погоду.
	lua_register(L, "release_weather", release_weather); // 353 отпустить погоду.
	lua_register(L, "get_old_weather_type", get_old_weather_type); // 354 получить прошлую погоду.
	lua_register(L, "get_new_weather_type", get_new_weather_type); // 355 получить текущую погоду.
	lua_register(L, "set_weather_type", set_weather_type); // 356 установить тип погоды.
	lua_register(L, "is_lightning_flash", is_lightning_flash); // 357 вспышка молнии.
	lua_register(L, "force_rain", force_rain); // 358 установить силу дождя.
	lua_register(L, "get_game_hours", get_game_hours); // 359 получить часы.
	lua_register(L, "get_game_minutes", get_game_minutes); // 360 получить минуты.
	lua_register(L, "get_game_seconds", get_game_seconds); // 361 получить секунды.
	lua_register(L, "set_game_hours", set_game_hours); // 362 установить часы.
	lua_register(L, "set_milliseconds_per_game_minute", set_milliseconds_per_game_minute); // 363 скорость хода времени.
	lua_register(L, "get_milliseconds_per_game_minute", get_milliseconds_per_game_minute); // 364 получить скорость времени.
	lua_register(L, "clock_store", clock_store); // 365 запомнить время.
	lua_register(L, "clock_restore", clock_restore); // 366 восстановить время.
	lua_register(L, "get_time_until", get_time_until); // 367 минуты до указанного времени.
	lua_register(L, "is_time_in_range", is_time_in_range); // 368 время в диапазоне.
	lua_register(L, "get_fov", get_fov); // 369 получить угол обзора.
	lua_register(L, "set_fov", set_fov); // 370 установить угол обзора.
	lua_register(L, "get_far_clip", get_far_clip); // 371 получить дальность прорисовки.
	lua_register(L, "set_far_clip", set_far_clip); // 372 установить дальность прорисовки.
	lua_register(L, "get_near_clip", get_near_clip); // 373 получить ближнюю границу.
	lua_register(L, "set_near_clip", set_near_clip); // 374 установить ближнюю границу.
	lua_register(L, "get_lod_distance", get_lod_distance); // 375 получить дистанцию LOD.
	lua_register(L, "set_lod_distance", set_lod_distance); // 376 установить дистанцию LOD.
	lua_register(L, "get_fade_color", get_fade_color); // 377 получить цвет затемнения.
	lua_register(L, "set_fade_color", set_fade_color); // 378 установить цвет затемнения.
	lua_register(L, "camera_shake", camera_shake); // 379 тряхнуть камеру.
	lua_register(L, "camera_draw_borders_widescreen", camera_draw_borders_widescreen); // 380 границы широкоэкранного.
	lua_register(L, "camera_clear_player_weapon_mode", camera_clear_player_weapon_mode); // 381 сбросить режим оружия.
	lua_register(L, "camera_calculate_derived_values", camera_calculate_derived_values); // 382 пересчитать камеру.
	lua_register(L, "camera_dont_process_obbe_cinema", camera_dont_process_obbe_cinema); // 383 отключить кино-камеру.
	lua_register(L, "get_mouse_accel_vertical", get_mouse_accel_vertical); // 384 вертикальное ускорение мыши.
	lua_register(L, "set_mouse_accel_vertical", set_mouse_accel_vertical); // 385 установить ускорение мыши.
	lua_register(L, "get_mouse_accel_horizontal", get_mouse_accel_horizontal); // 386 горизонтальное ускорение мыши.
	lua_register(L, "set_mouse_accel_horizontal", set_mouse_accel_horizontal); // 387 установить ускорение мыши.
	lua_register(L, "get_use_mouse_3rd_person", get_use_mouse_3rd_person); // 388 мышь в третьем лице.
	lua_register(L, "get_sun_blocked_by_clouds", get_sun_blocked_by_clouds); // 389 солнце за облаками.
	lua_register(L, "weather_add_rain", weather_add_rain); // 390 добавить дождь.
	lua_register(L, "weather_add_stream_after_rain", weather_add_stream_after_rain); // 391 пар после дождя.
	lua_register(L, "weather_render_rain_streaks", weather_render_rain_streaks); // 392 отрисовать полосы дождя.
	lua_register(L, "get_obj_type", get_obj_type); // 393 получить тип объекта.
	lua_register(L, "get_obj_bonus_value", get_obj_bonus_value); // 394 получить бонус объекта.
	lua_register(L, "set_obj_bonus_value", set_obj_bonus_value); // 395 установить бонус объекта.
	lua_register(L, "get_obj_cost_value", get_obj_cost_value); // 396 получить стоимость объекта.
	lua_register(L, "set_obj_cost_value", set_obj_cost_value); // 397 установить стоимость объекта.
	lua_register(L, "get_obj_damage_multiplier", get_obj_damage_multiplier); // 398 получить множитель урона.
	lua_register(L, "set_obj_damage_multiplier", set_obj_damage_multiplier); // 399 установить множитель урона.
	lua_register(L, "get_obj_collision_damage_type", get_obj_collision_damage_type); // 400 тип урона от столкновений.
	lua_register(L, "set_obj_collision_damage_type", set_obj_collision_damage_type); // 401 установить тип урона.
	lua_register(L, "get_obj_camera_avoids", get_obj_camera_avoids); // 402 камера обходит объект.
	lua_register(L, "set_obj_camera_avoids", set_obj_camera_avoids); // 403 установить обход камерой.
	lua_register(L, "get_obj_timer", get_obj_timer); // 404 получить таймер объекта.
	lua_register(L, "set_obj_timer", set_obj_timer); // 405 установить таймер объекта.
	lua_register(L, "get_obj_ref_model_id", get_obj_ref_model_id); // 406 модель на которую ссылается объект.
	lua_register(L, "obj_can_be_deleted", obj_can_be_deleted); // 407 можно удалить объект.
	lua_register(L, "obj_damage", obj_damage); // 408 нанести урон объекту.
	lua_register(L, "obj_delete_all_mission", obj_delete_all_mission); // 409 удалить миссионные объекты.
	lua_register(L, "obj_delete_all_temp", obj_delete_all_temp); // 410 удалить временные объекты.
	lua_register(L, "obj_delete_all_temp_in_area", obj_delete_all_temp_in_area); // 411 удалить временные в радиусе.
	lua_register(L, "get_obj_attach_force", get_obj_attach_force); // 412 сила прикрепления объекта.
	lua_register(L, "set_obj_attach_force", set_obj_attach_force); // 413 установить силу прикрепления.
	lua_register(L, "fire_find_nearest", fire_find_nearest); // 414 найти ближайший пожар.
	lua_register(L, "fire_start_on_point", fire_start_on_point); // 415 поджечь точку.
	lua_register(L, "get_fire_time_to_burn", get_fire_time_to_burn); // 416 время горения пожара.
	lua_register(L, "set_fire_time_to_burn", set_fire_time_to_burn); // 417 установить время горения.
	lua_register(L, "get_fire_target", get_fire_target); // 418 цель пожара.
	lua_register(L, "fire_is_active", fire_is_active); // 419 пожар активен.
	lua_register(L, "get_fire_position", get_fire_position); // 420 координаты пожара.
	lua_register(L, "ped_play_anim", ped_play_anim); // 421 запустить анимацию педа.
	lua_register(L, "ped_blend_anim", ped_blend_anim); // 422 запустить анимацию со смешиванием.
	lua_register(L, "get_anim_block_index", get_anim_block_index); // 423 индекс блока анимаций.
	lua_register(L, "anim_set_group_ref", anim_set_group_ref); // 424 задать группу анимаций.
	lua_register(L, "get_ped_stat_kills", get_ped_stat_kills); // 425 законные убийства игрока.
	lua_register(L, "get_ped_stat_cheated", get_ped_stat_cheated); // 426 число читов игрока.
	lua_register(L, "get_stat_property_destroyed", get_stat_property_destroyed); // 427 разрушенное имущество.
	lua_register(L, "get_stat_longest_wheelie_dist", get_stat_longest_wheelie_dist); // 428 дистанция вилли.
	lua_register(L, "get_stat_longest_stoppie_dist", get_stat_longest_stoppie_dist); // 429 дистанция стоппи.
	lua_register(L, "get_stat_longest_2wheel_dist", get_stat_longest_2wheel_dist); // 430 дистанция на двух колесах.
	lua_register(L, "get_stat_longest_wheelie_time", get_stat_longest_wheelie_time); // 431 время вилли.
	lua_register(L, "get_stat_seagulls_killed", get_stat_seagulls_killed); // 432 убитые чайки.
	lua_register(L, "get_stat_times_drowned", get_stat_times_drowned); // 433 кол-во утоплений.
	lua_register(L, "get_stat_flight_time", get_stat_flight_time); // 434 время полетов.
	lua_register(L, "get_stat_missions_total", get_stat_missions_total); // 435 общее число миссий.
	lua_register(L, "get_stat_frenzies_total", get_stat_frenzies_total); // 436 общее число безумий.
	lua_register(L, "get_stat_frenzies_passed", get_stat_frenzies_passed); // 437 пройденные безумия.
	lua_register(L, "get_stat_ice_cream_sold", get_stat_ice_cream_sold); // 438 проданное мороженое.
	lua_register(L, "get_stat_pizzas_delivered", get_stat_pizzas_delivered); // 439 доставленные пиццы.
	lua_register(L, "get_stat_assassinations", get_stat_assassinations); // 440 заказные убийства.
	lua_register(L, "get_stat_movie_stunts", get_stat_movie_stunts); // 441 кинотрюки.
	lua_register(L, "get_stat_stores_knocked_off", get_stat_stores_knocked_off); // 442 ограбленные магазины.
	lua_register(L, "get_stat_loan_sharks", get_stat_loan_sharks); // 443 сделки с ростовщиками.
	lua_register(L, "get_stat_garbage_pickups", get_stat_garbage_pickups); // 444 собранный мусор.
	lua_register(L, "get_stat_weapon_budget", get_stat_weapon_budget); // 445 бюджет на оружие.
	lua_register(L, "get_stat_fashion_budget", get_stat_fashion_budget); // 446 бюджет на одежду.
	lua_register(L, "get_stat_property_budget", get_stat_property_budget); // 447 бюджет на собственность.
	lua_register(L, "get_stat_auto_painting_budget", get_stat_auto_painting_budget); // 448 бюджет на покраску.
	lua_register(L, "get_stat_bloodring_kills", get_stat_bloodring_kills); // 449 убийства на Bloodring.
	lua_register(L, "get_stat_bloodring_time", get_stat_bloodring_time); // 450 время на Bloodring.
	lua_register(L, "get_stat_shooting_rank", get_stat_shooting_rank); // 451 ранг стрельбы.
	lua_register(L, "get_stat_top_shooting_score", get_stat_top_shooting_score); // 452 лучший счет в тире.
	lua_register(L, "get_stat_num_property_owned", get_stat_num_property_owned); // 453 кол-во собственности.
	lua_register(L, "radar_set_coord_blip", radar_set_coord_blip); // 454 создать метку на карте.
	lua_register(L, "radar_set_short_range_blip", radar_set_short_range_blip); // 455 короткая метка.
	lua_register(L, "radar_set_entity_blip", radar_set_entity_blip); // 456 метка на сущности.
	lua_register(L, "radar_clear_blip", radar_clear_blip); // 457 удалить метку.
	lua_register(L, "radar_change_blip_colour", radar_change_blip_colour); // 458 цвет метки.
	lua_register(L, "radar_change_blip_brightness", radar_change_blip_brightness); // 459 яркость метки.
	lua_register(L, "radar_change_blip_scale", radar_change_blip_scale); // 460 размер метки.
	lua_register(L, "radar_change_blip_display", radar_change_blip_display); // 461 показ метки.
	lua_register(L, "radar_set_blip_sprite", radar_set_blip_sprite); // 462 иконка метки.
	lua_register(L, "radar_show_marker", radar_show_marker); // 463 показать маркер в мире.
	lua_register(L, "radar_set_marker_state", radar_set_marker_state); // 464 состояние маркера миссии.
	lua_register(L, "radar_draw_radar_sprite", radar_draw_radar_sprite); // 465 отрисовать спрайт радара.
	lua_register(L, "radar_get_radar_range", radar_get_radar_range); // 466 дальность радара.
	lua_register(L, "radar_set_radar_range", radar_set_radar_range); // 467 установить дальность радара.
	lua_register(L, "radar_clear_blip_for_entity", radar_clear_blip_for_entity); // 468 удалить метку сущности.
	lua_register(L, "play_one_off_sound", play_one_off_sound); // 469 одноразовый звук.
	lua_register(L, "play_continuous_sound", play_continuous_sound); // 470 непрерывный звук.
	lua_register(L, "remove_sound_by_id", remove_sound_by_id); // 471 удалить звук.
	lua_register(L, "set_radio_channel", set_radio_channel); // 472 радиостанция игроку.
	lua_register(L, "get_radio_channel", get_radio_channel); // 473 текущая радиостанция.
	lua_register(L, "set_music_does_fade", set_music_does_fade); // 474 затухание музыки.
	lua_register(L, "police_radio_message", police_radio_message); // 475 полицейское радио.
	lua_register(L, "create_explosion_no_sound", create_explosion_no_sound); // 476 взрыв без звука.
	lua_register(L, "get_ped_density_multiplier", get_ped_density_multiplier); // 477 плотность пешеходов.
	lua_register(L, "set_ped_density_multiplier", set_ped_density_multiplier); // 478 установить плотность пешеходов.
	lua_register(L, "get_all_random_peds_type", get_all_random_peds_type); // 479 тип случайных педов.
	lua_register(L, "set_all_random_peds_type", set_all_random_peds_type); // 480 установить тип случайных педов.
	lua_register(L, "population_manage", population_manage); // 481 обработать популяцию.
	lua_register(L, "population_remove_peds_full_pool", population_remove_peds_full_pool); // 482 удалить педов при переполнении.
	lua_register(L, "population_convert_all_objects_to_dummy", population_convert_all_objects_to_dummy); // 483 конвертировать объекты в dummy.
	lua_register(L, "population_is_female", population_is_female); // 484 модель педа женская.
	lua_register(L, "population_is_male", population_is_male); // 485 модель педа мужская.
	lua_register(L, "population_is_sunbather", population_is_sunbather); // 486 модель педа загорающий.
	lua_register(L, "world_find_ground_z_for_coord", world_find_ground_z_for_coord); // 487 высота земли под точкой.
	lua_register(L, "world_find_ground_z_for_3d_coord", world_find_ground_z_for_3d_coord); // 488 высота земли под 3d точкой.
	lua_register(L, "world_find_roof_z_for_3d_coord", world_find_roof_z_for_3d_coord); // 489 высота крыши под точкой.
	lua_register(L, "world_get_is_line_of_sight_clear", world_get_is_line_of_sight_clear); // 490 прямая видимость между точками.
	lua_register(L, "world_clear_peds_from_area", world_clear_peds_from_area); // 491 удалить педов из области.
	lua_register(L, "world_clear_cars_from_area", world_clear_cars_from_area); // 492 удалить авто из области.
	lua_register(L, "world_extinguish_all_car_fires_in_area", world_extinguish_all_car_fires_in_area); // 493 потушить пожары в области.
	lua_register(L, "world_clear_exciting_stuff_from_area", world_clear_exciting_stuff_from_area); // 494 очистить область от эффектов.
	lua_register(L, "world_call_off_chase_for_area", world_call_off_chase_for_area); // 495 отменить погоню в области.
	lua_register(L, "world_add_entity", world_add_entity); // 496 добавить сущность в мир.
	lua_register(L, "world_clear_scan_codes", world_clear_scan_codes); // 497 сбросить коды сканирования.
	lua_register(L, "general_atan_of_xy", general_atan_of_xy); // 498 арктангенс от x,y.
	lua_register(L, "general_angle_between_points", general_angle_between_points); // 499 угол между точками.
	lua_register(L, "general_radian_angle_between_points", general_radian_angle_between_points); // 500 угол между точками в радианах.
	lua_register(L, "general_limit_angle", general_limit_angle); // 501 нормализовать угол.
	lua_register(L, "general_limit_radian_angle", general_limit_radian_angle); // 502 нормализовать угол в радианах.
	lua_register(L, "general_node_heading_from_vector", general_node_heading_from_vector); // 503 направление из вектора.
	lua_register(L, "get_timer_ms", get_timer_ms); // 504 игровое время в мс.
	lua_register(L, "get_timer_ms_pause_mode", get_timer_ms_pause_mode); // 505 время с учетом паузы.
	lua_register(L, "get_frame_counter", get_frame_counter); // 506 номер кадра.
	lua_register(L, "get_time_step", get_time_step); // 507 шаг времени кадра.
	lua_register(L, "is_user_pause", is_user_pause); // 508 игра на паузе.
	lua_register(L, "gangs_set_weapons", gangs_set_weapons); // 509 оружие банды.
	lua_register(L, "gangs_set_vehicle_model", gangs_set_vehicle_model); // 510 авто банды.
	lua_register(L, "gangs_set_ped_models", gangs_set_ped_models); // 511 модели педов банды.
	lua_register(L, "gangs_choose_ped_model", gangs_choose_ped_model); // 512 модель педа банды.
	lua_register(L, "gangs_set_attack_with_cops", gangs_set_attack_with_cops); // 513 банда атакует с копами.
	lua_register(L, "gangs_get_attack_with_cops", gangs_get_attack_with_cops); // 514 банда атакует с копами.
	lua_register(L, "gangs_initialise", gangs_initialise); // 515 переинициализация банд.
	lua_register(L, "gamelogic_pass_time", gamelogic_pass_time); // 516 пропустить время.
	lua_register(L, "gamelogic_update", gamelogic_update); // 517 обновить логику.
	lua_register(L, "gamelogic_after_death_arrest_shortcut", gamelogic_after_death_arrest_shortcut); // 518 такси-шорткат после смерти.
	lua_register(L, "message_add_big", message_add_big); // 519 большое сообщение.
	lua_register(L, "message_add_big_queue", message_add_big_queue); // 520 большое сообщение в очередь.
	lua_register(L, "message_add", message_add); // 521 маленькое сообщение.
	lua_register(L, "message_add_jump_queue", message_add_jump_queue); // 522 сообщение вне очереди.
	lua_register(L, "message_add_with_number", message_add_with_number); // 523 сообщение с числом.
	lua_register(L, "message_clear_all", message_clear_all); // 524 очистить все сообщения.
	lua_register(L, "message_clear_small_only", message_clear_small_only); // 525 очистить маленькие сообщения.
	lua_register(L, "message_clear_this_big_print", message_clear_this_big_print); // 526 удалить большое сообщение.
	lua_register(L, "message_clear_this_print", message_clear_this_print); // 527 удалить маленькое сообщение.
	lua_register(L, "font_set_style", font_set_style); // 528 стиль шрифта.
	lua_register(L, "font_set_drop_shadow", font_set_drop_shadow); // 529 тень шрифта.
	lua_register(L, "font_set_alpha_fade", font_set_alpha_fade); // 530 прозрачность шрифта.
	lua_register(L, "font_set_centre_size", font_set_centre_size); // 531 размер центр. текста.
	lua_register(L, "font_set_wrapx", font_set_wrapx); // 532 ширина переноса шрифта.
	lua_register(L, "font_set_prop_on", font_set_prop_on); // 533 пропорц. шрифт вкл.
	lua_register(L, "font_set_prop_off", font_set_prop_off); // 534 пропорц. шрифт выкл.
	lua_register(L, "font_set_background_on", font_set_background_on); // 535 фон шрифта вкл.
	lua_register(L, "font_set_background_off", font_set_background_off); // 536 фон шрифта выкл.
	lua_register(L, "font_set_right_justify_on", font_set_right_justify_on); // 537 выравнивание вправо вкл.
	lua_register(L, "font_set_right_justify_off", font_set_right_justify_off); // 538 выравнивание вправо выкл.
	lua_register(L, "heli_set_orientation", heli_set_orientation); // 539 установить ориентацию вертолета
	lua_register(L, "heli_clear_orientation", heli_clear_orientation); // 540 очистить ориентацию вертолета
	lua_register(L, "heli_set_stabiliser", heli_set_stabiliser); // 541 установить стабилизатор вертолета
	lua_register(L, "heli_make_crashing_down", heli_make_crashing_down); // 542 вертолет падает
	lua_register(L, "heli_is_player_in_any_heli", heli_is_player_in_any_heli); // 543 игрок в любом вертолете
	lua_register(L, "heli_is_char_in_any_heli", heli_is_char_in_any_heli); // 544 пед в любом вертолете
	lua_register(L, "heli_switch", heli_switch); // 545 переключить вертолет
	lua_register(L, "catalina_heli_start", catalina_heli_start); // 546 начать вертолет Catalina
	lua_register(L, "catalina_heli_take_off", catalina_heli_take_off); // 547 Catalina взлетает
	lua_register(L, "catalina_heli_remove", catalina_heli_remove); // 548 удалить Catalina
	lua_register(L, "catalina_heli_grab", catalina_heli_grab); // 549 сесть в Catalina
	lua_register(L, "catalina_heli_fly_away", catalina_heli_fly_away); // 550 Catalina улетает
	lua_register(L, "catalina_heli_has_shot_down", catalina_heli_has_shot_down); // 551 Catalina сбит
	lua_register(L, "plane_goto_coords", plane_goto_coords); // 552 самолет летит на координаты
	lua_register(L, "plane_is_player_in_any_plane", plane_is_player_in_any_plane); // 553 игрок в любом самолете
	lua_register(L, "plane_is_char_in_any_plane", plane_is_char_in_any_plane); // 554 пед в любом самолете
	lua_register(L, "find_drug_plane_coords", find_drug_plane_coords); // 555 координаты самолета с наркотиками
	lua_register(L, "find_drop_off_plane_coords", find_drop_off_plane_coords); // 556 координаты самолета-доставщика
	lua_register(L, "has_drug_plane_shot_down", has_drug_plane_shot_down); // 557 самолет с наркотиками сбит
	lua_register(L, "has_drop_off_plane_shot_down", has_drop_off_plane_shot_down); // 558 самолет-доставщик сбит
	lua_register(L, "boat_goto_coords", boat_goto_coords); // 559 катер плывет на координаты
	lua_register(L, "boat_set_cruise_speed", boat_set_cruise_speed); // 560 крейсерская скорость катера
	lua_register(L, "boat_anchor", boat_anchor); // 561 якорить катер
	lua_register(L, "boat_start_foam_animation", boat_start_foam_animation); // 562 включить пену катера
	lua_register(L, "boat_update_foam_animation", boat_update_foam_animation); // 563 обновить пену катера
	lua_register(L, "boat_is_player_in_any_boat", boat_is_player_in_any_boat); // 564 игрок в любом катере
	lua_register(L, "boat_is_char_in_any_boat", boat_is_char_in_any_boat); // 565 пед в любом катере
	lua_register(L, "is_vehicle_boat", is_vehicle_boat); // 566 транспорт это катер
	lua_register(L, "train_catch_char_obj", train_catch_char_obj); // 567 пед ловит поезд
	lua_register(L, "train_catch_coll_obj", train_catch_coll_obj); // 568 объект ловит поезд
	lua_register(L, "rc_give_remote_controlled_car_to_player", rc_give_remote_controlled_car_to_player); // 569 выдать RC-машину
	lua_register(L, "rc_give_remote_controlled_model_to_player", rc_give_remote_controlled_model_to_player); // 570 выдать RC-модель
	lua_register(L, "rc_get_remote_controlled_car", rc_get_remote_controlled_car); // 571 получить RC-машину
	lua_register(L, "rc_buggy_blow_up", rc_buggy_blow_up); // 572 взорвать RC-багги
	lua_register(L, "rc_buggy_remove", rc_buggy_remove); // 573 удалить RC-багги
	lua_register(L, "rc_detonate_enable", rc_detonate_enable); // 574 включить детонацию RC
	lua_register(L, "rc_detonate_on_contact_enable", rc_detonate_on_contact_enable); // 575 детонация RC при контакте
	lua_register(L, "player_is_in_remote_mode", player_is_in_remote_mode); // 576 игрок в режиме пульта
	lua_register(L, "set_fade_jumpcut_after_rc_explosion", set_fade_jumpcut_after_rc_explosion); // 577 затухание после взрыва RC
	lua_register(L, "bike_is_player_on_any_bike", bike_is_player_on_any_bike); // 578 игрок на любом мотоцикле
	lua_register(L, "bike_is_char_on_any_bike", bike_is_char_on_any_bike); // 579 пед на любом мотоцикле
	lua_register(L, "is_char_in_flying_vehicle", is_char_in_flying_vehicle); // 580 пед в летающем транспорте
	lua_register(L, "is_player_in_flying_vehicle", is_player_in_flying_vehicle); // 581 игрок в летающем транспорте
	lua_register(L, "crane_activate", crane_activate); // 582 активировать кран
	lua_register(L, "crane_is_carrying_car", crane_is_carrying_car); // 583 кран несет авто
	lua_register(L, "crane_is_targetting_car", crane_is_targetting_car); // 584 кран целится в авто
	lua_register(L, "escalators_count", escalators_count); // 585 количество эскалаторов
	lua_register(L, "zone_find_index", zone_find_index); // 586 найти индекс зоны по имени
	lua_register(L, "zone_get_level_from_position", zone_get_level_from_position); // 587 уровень зоны по позиции
	lua_register(L, "crusher_crane_activate", crusher_crane_activate); // 588 активировать пресс-кран
	lua_register(L, "military_crane_activate", military_crane_activate); // 589 активировать военный кран
	lua_register(L, "crane_deactivate", crane_deactivate); // 590 деактивировать кран
	lua_register(L, "crane_military_collected_all", crane_military_collected_all); // 591 военный кран собрал все авто
	lua_register(L, "crane_is_lifting_car", crane_is_lifting_car); // 592 кран поднимает авто
	lua_register(L, "boat_stop", boat_stop); // 593 остановить катер
	lua_register(L, "heli_goto_coords", heli_goto_coords); // 594 вертолет на координаты
	lua_register(L, "heli_create_dust_effect", heli_create_dust_effect); // 595 пыль катсценного вертолета
	lua_register(L, "car_goto_accurate", car_goto_accurate); // 596 точная езда на координаты
	lua_register(L, "car_set_idle", car_set_idle); // 597 авто в ожидание
	lua_register(L, "car_wander_randomly", car_wander_randomly); // 598 авто едет случайно
	lua_register(L, "car_face_coord", car_face_coord); // 599 повернуть к координатам
	lua_register(L, "freeze_car_position", freeze_car_position); // 600 заморозить позицию
	lua_register(L, "freeze_car_no_collide", freeze_car_no_collide); // 601 заморозить без коллизий
	lua_register(L, "set_car_status", set_car_status); // 602 статус авто
	lua_register(L, "set_car_mission", set_car_mission); // 603 миссия авто
	lua_register(L, "car_block_car", car_block_car); // 604 блокировать авто
	lua_register(L, "car_ram_car", car_ram_car); // 605 таранить
	lua_register(L, "car_change_lane", car_change_lane); // 606 смена полосы
	lua_register(L, "car_drive_straight", car_drive_straight); // 607 ехать прямо
	lua_register(L, "set_car_forward_speed", set_car_forward_speed); // 608 скорость вперед
	lua_register(L, "car_wait", car_wait); // 609 приказать ждать ms
	lua_register(L, "car_route_seed", car_route_seed); // 610 seed маршрута
	lua_register(L, "car_straight_dist", car_straight_dist); // 611 дистанция прямой линии
	lua_register(L, "car_convoy", car_convoy); // 612 конвойное авто
	lua_register(L, "car_make_stronger", car_make_stronger); // 613 усилить авто Крейга
	lua_register(L, "car_to_player", car_to_player); // 614 авто к игроку
	lua_register(L, "car_fade_in", car_fade_in); // 615 затухание
	lua_register(L, "car_on_trailer", car_on_trailer); // 616 на прицеп
	lua_register(L, "set_car_visible", set_car_visible); // 617 видимость
	lua_register(L, "set_car_watertight", set_car_watertight); // 618 водонепроницаемость
	lua_register(L, "set_car_big_wheels", set_car_big_wheels); // 619 большие колеса
	lua_register(L, "set_car_bproof", set_car_bproof); // 620 пуленепробиваемость
	lua_register(L, "set_car_fproof", set_car_fproof); // 621 огнеупорность
	lua_register(L, "set_car_rproof", set_car_rproof); // 622 ракетоупорность
	lua_register(L, "set_car_no_collide", set_car_no_collide); // 623 без коллизий
	lua_register(L, "set_car_burst_tires", set_car_burst_tires); // 624 пробиваемость шин
	lua_register(L, "set_car_dmg_player_only", set_car_dmg_player_only); // 625 урон только от игрока
	lua_register(L, "car_load_collision", car_load_collision); // 626 загрузка коллизий
	lua_register(L, "car_upside_safe", car_upside_safe); // 627 перевернутое не ломается
	lua_register(L, "car_change_color", car_change_color); // 628 сменить цвет
	lua_register(L, "car_set_comp", car_set_comp); // 629 компоненты модели
	lua_register(L, "set_car_health", set_car_health); // 630 здоровье авто
	lua_register(L, "get_car_health", get_car_health); // 631 получить здоровье
	lua_register(L, "get_car_heading", get_car_heading); // 632 получить угол
	lua_register(L, "get_car_forward_x", get_car_forward_x); // 633 вектор X
	lua_register(L, "get_car_forward_y", get_car_forward_y); // 634 вектор Y
	lua_register(L, "is_car_in_air", is_car_in_air); // 635 в воздухе?
	lua_register(L, "is_car_in_air_proper", is_car_in_air_proper); // 636 в воздухе точно?
	lua_register(L, "car_health_greater", car_health_greater); // 637 здоровье больше?
	lua_register(L, "is_car_crushed", is_car_crushed); // 638 прессовано?
	lua_register(L, "is_car_remap", is_car_remap); // 639 перекрашено?
	lua_register(L, "is_car_stuck_roof", is_car_stuck_roof); // 640 на крыше?
	lua_register(L, "car_is_tire_burst", car_is_tire_burst); // 641 шина пробита?
	lua_register(L, "is_car_visibly_damaged", is_car_visibly_damaged); // 642 видимо повреждено?
	lua_register(L, "car_waiting_collision", car_waiting_collision); // 643 ждет коллизии?
	lua_register(L, "is_car_on_trailer", is_car_on_trailer); // 644 на прицепе?
	lua_register(L, "car_damaged_by_car", car_damaged_by_car); // 645 повреждено авто?
	lua_register(L, "car_damaged_by_ped", car_damaged_by_ped); // 646 повреждено педом?
	lua_register(L, "car_just_sunk", car_just_sunk); // 647 утонуло?
	lua_register(L, "car_drowning", car_drowning); // 648 тонет?
	lua_register(L, "car_has_weapon", car_has_weapon); // 649 оружие на борту?
	lua_register(L, "car_any_bomb", car_any_bomb); // 650 есть бомба?
	lua_register(L, "car_has_bomb", car_has_bomb); // 651 тип бомбы?
	lua_register(L, "is_car_in_water", is_car_in_water); // 652 в воде?
	lua_register(L, "fire_button_pressed", fire_button_pressed); // 653 кнопка огня?
	lua_register(L, "is_car_alive", is_car_alive); // 654 живо?
	lua_register(L, "does_vehicle_exist", does_vehicle_exist); // 655 существует?
	lua_register(L, "is_car_in_garage", is_car_in_garage); // 656 в гараже?
	lua_register(L, "was_ever_police", was_ever_police); // 657 был полицейским?
	lua_register(L, "clear_car_dmg", clear_car_dmg); // 658 очистить урон
	lua_register(L, "clear_car_wep_dmg", clear_car_wep_dmg); // 659 очистить урон оружием
	lua_register(L, "remove_stuck_check", remove_stuck_check); // 660 убрать проверку застревания
	lua_register(L, "remove_upside_check", remove_upside_check); // 661 убрать проверку переворота
	lua_register(L, "add_upside_check", add_upside_check); // 662 добавить проверку переворота
	lua_register(L, "unlock_doors_area", unlock_doors_area); // 663 открыть двери
	lua_register(L, "close_all_doors", close_all_doors); // 664 закрыть все двери
	lua_register(L, "close_car_boot", close_car_boot); // 665 закрыть багажник
	lua_register(L, "pop_car_boot", pop_car_boot); // 666 открыть багажник
	lua_register(L, "pop_car_boot_physics", pop_car_boot_physics); // 667 открыть физикой
	lua_register(L, "lock_doors", lock_doors); // 668 запереть двери
	lua_register(L, "change_car_lock", change_car_lock); // 669 сменить замок
	lua_register(L, "get_car_driver_ref", get_car_driver_ref); // 670 водитель
	lua_register(L, "get_char_in_seat", get_char_in_seat); // 671 пассажир
	lua_register(L, "order_driver_out", order_driver_out); // 672 выгнать водителя
	lua_register(L, "set_ped_max_drive_speed", set_ped_max_drive_speed); // 673 макс скорость
	lua_register(L, "set_ped_drive_aggression", set_ped_drive_aggression); // 674 агрессия
	lua_register(L, "shuffle_to_driver", shuffle_to_driver); // 675 пересесть за руль
	lua_register(L, "stay_when_jacked", stay_when_jacked); // 676 не покидать при угоне
	lua_register(L, "is_ped_in_police_car", is_ped_in_police_car); // 677 в полицейском?
	lua_register(L, "is_ped_touching_vehicle", is_ped_touching_vehicle); // 678 касается?
	lua_register(L, "is_ped_leaving_to_die", is_ped_leaving_to_die); // 679 умирает из авто?
	lua_register(L, "is_ped_drowning", is_ped_drowning); // 680 тонет?
	lua_register(L, "is_ped_in_water", is_ped_in_water); // 681 в воде?
	lua_register(L, "is_ped_in_zone", is_ped_in_zone); // 682 в зоне?
	lua_register(L, "is_player_in_zone", is_player_in_zone); // 683 игрок в зоне?
	lua_register(L, "warp_ped_from_car", warp_ped_from_car); // 684 телепорт из авто
	lua_register(L, "warp_ped_passenger", warp_ped_passenger); // 685 в пассажиры
	lua_register(L, "warp_player_from_car", warp_player_from_car); // 686 телепорт игрока
	lua_register(L, "set_car_ammo", set_car_ammo); // 687 боезапас
	lua_register(L, "switch_car_weapons", switch_car_weapons); // 688 переключить оружие
	lua_register(L, "set_car_weapon", set_car_weapon); // 689 текущее оружие
	lua_register(L, "fire_hunter_gun", fire_hunter_gun); // 690 выстрел Hunter
	lua_register(L, "set_tank_detonate", set_tank_detonate); // 691 танк взрывает авто
	lua_register(L, "ensure_drive_by", ensure_drive_by); // 692 дать drive-by
	lua_register(L, "arm_car_bomb", arm_car_bomb); // 693 установить бомбу
	lua_register(L, "disarm_car_bomb", disarm_car_bomb); // 694 обезвредить
	lua_register(L, "give_car_alarm", give_car_alarm); // 695 сигнализация
	lua_register(L, "switch_car_siren", switch_car_siren); // 696 сирена
	lua_register(L, "set_driving_style_op", set_driving_style_op); // 697 стиль вождения (опкод)
	lua_register(L, "set_char_obj_wait", set_char_obj_wait); // 698 нет задач (ждёт на ногах)
	lua_register(L, "set_char_obj_flee_coord", set_char_obj_flee_coord); // 699 убежать от точки пешком до безопасности
	lua_register(L, "set_char_obj_guard_spot", set_char_obj_guard_spot); // 700 охранять точку
	lua_register(L, "set_char_obj_guard_area", set_char_obj_guard_area); // 701 охранять зону (радиус)
	lua_register(L, "set_char_obj_wait_in_car", set_char_obj_wait_in_car); // 702 ждать в машине
	lua_register(L, "set_char_obj_leave_car", set_char_obj_leave_car); // 703 выйти из машины
	lua_register(L, "set_char_obj_enter_car_passenger", set_char_obj_enter_car_passenger); // 704 сесть в авто как пассажир
	lua_register(L, "set_char_obj_enter_car_driver", set_char_obj_enter_car_driver); // 705 сесть в авто как водитель
	lua_register(L, "set_char_obj_follow_car_in_car", set_char_obj_follow_car_in_car); // 706 следовать за авто из авто
	lua_register(L, "set_char_obj_destroy_object", set_char_obj_destroy_object); // 707 уничтожить объект
	lua_register(L, "set_char_obj_goto_area_in_car", set_char_obj_goto_area_in_car); // 708 ехать к зоне
	lua_register(L, "set_char_obj_follow_car_foot_offset", set_char_obj_follow_car_foot_offset); // 709 следовать за авто пешком смещение
	lua_register(L, "set_char_obj_guard_attack", set_char_obj_guard_attack); // 710 охрана — атаковать противника
	lua_register(L, "set_char_obj_goto_coord_on_foot", set_char_obj_goto_coord_on_foot); // 711 идти к координатам пешком
	lua_register(L, "set_char_obj_goto_coord_in_car", set_char_obj_goto_coord_in_car); // 712 ехать к координатам
	lua_register(L, "set_char_obj_run_to_coord", set_char_obj_run_to_coord); // 713 бежать к координатам
	lua_register(L, "set_char_obj_goto_area_any_means", set_char_obj_goto_area_any_means); // 714 добираться любыми средствами
	lua_register(L, "set_char_obj_follow_formation", set_char_obj_follow_formation); // 715 следовать в строю за лидером
	lua_register(L, "set_char_obj_hail_taxi", set_char_obj_hail_taxi); // 716 вызвать такси
	lua_register(L, "set_char_obj_steal_any_car", set_char_obj_steal_any_car); // 717 украсть любую машину
	lua_register(L, "set_char_obj_leave_any_car", set_char_obj_leave_any_car); // 718 выйти из любого авто
	lua_register(L, "set_char_obj_flee_car", set_char_obj_flee_car); // 719 убежать от машины
	lua_register(L, "set_char_obj_walk_to_char", set_char_obj_walk_to_char); // 720 идти к другому педу
	lua_register(L, "set_char_obj_steal_any_car_mission", set_char_obj_steal_any_car_mission); // 721 украсть любую миссионную машину
	lua_register(L, "set_char_obj_sprint_to_coord", set_char_obj_sprint_to_coord); // 722 спринт к координатам
	lua_register(L, "set_char_obj_goto_car_on_foot", set_char_obj_goto_car_on_foot); // 723 идти к машине пешком
	lua_register(L, "get_projectile_count", get_projectile_count); // 724 число активных снарядов
	lua_register(L, "get_projectile_weapon", get_projectile_weapon); // 725 тип оружия снаряда
	lua_register(L, "get_projectile_source", get_projectile_source); // 726 источник снаряда (указатель)
	lua_register(L, "get_projectile_pos", get_projectile_pos); // 727 позиция снаряда x,y,z
	lua_register(L, "set_projectile_pos", set_projectile_pos); // 728 уст позицию снаряда (направление ракеты)
	lua_register(L, "is_projectile_in_use", is_projectile_in_use); // 729 слот снаряда занят
	lua_register(L, "remove_all_projectiles", remove_all_projectiles); // 730 удалить все снаряды
	lua_register(L, "is_projectile_in_area", is_projectile_in_area); // 731 снаряд в зоне (6 координат)
	lua_register(L, "destroy_projectiles_in_area", destroy_projectiles_in_area); // 732 уничтожить снаряды в зоне
	lua_register(L, "add_projectile", add_projectile); // 733 создать снаряд (источник, оружие, x,y,z, сила)
	lua_register(L, "find_car_by_model", find_car_by_model); // 734 найти первое авто по модели
	lua_register(L, "find_ped_by_model", find_ped_by_model); // 735 найти первого педа по модели
	lua_register(L, "find_object_by_model", find_object_by_model); // 736 найти объект по модели
	lua_register(L, "get_vehicle_pool_size", get_vehicle_pool_size); // 737 размер пула транспорта
	lua_register(L, "get_car_pool_index", get_car_pool_index); // 738 индекс авто в пуле
	lua_register(L, "get_car_by_pool_index", get_car_by_pool_index); // 739 авто по индексу в пуле
	lua_register(L, "get_car_velocity", get_car_velocity); // 740 скорость авто m_vecMoveSpeed (x,y,z)
	lua_register(L, "set_car_velocity", set_car_velocity); // 741 уст скорость авто m_vecMoveSpeed (x,y,z)
	lua_register(L, "remove_weapon_from_char", remove_weapon_from_char); // 742 удалить оружие у педа (0x555)
	lua_register(L, "get_forced_weather_type", get_forced_weather_type); // 743 принудительный тип погоды
	lua_register(L, "is_lightning_burst", is_lightning_burst); // 744 вспышка молнии (CWeather::LightningBurst)
	lua_register(L, "set_char_obj_kill_char_on_foot", set_char_obj_kill_char_on_foot); // 745 убить педа пешком (ближний бой)
	lua_register(L, "set_char_obj_kill_char_any_means", set_char_obj_kill_char_any_means); // 746 убить педа любыми средствами
	lua_register(L, "set_char_obj_flee_char_on_foot_till_safe", set_char_obj_flee_char_on_foot_till_safe); // 747 убежать от педа до безопасного места
	lua_register(L, "set_char_obj_flee_char_on_foot_always", set_char_obj_flee_char_on_foot_always); // 748 всегда убегать от педа
	lua_register(L, "set_char_obj_goto_char_on_foot", set_char_obj_goto_char_on_foot); // 749 идти к педу пешком
	lua_register(L, "set_char_obj_aim_gun_at_char", set_char_obj_aim_gun_at_char); // 750 целиться из оружия в педа
	lua_register(L, "set_char_obj_kill_player_on_foot", set_char_obj_kill_player_on_foot); // 751 убить игрока пешком
	lua_register(L, "set_char_obj_kill_player_any_means", set_char_obj_kill_player_any_means); // 752 убить игрока любыми средствами
	lua_register(L, "set_char_obj_flee_player_on_foot_till_safe", set_char_obj_flee_player_on_foot_till_safe); // 753 убежать от игрока до безопасного места
	lua_register(L, "set_char_obj_flee_player_on_foot_always", set_char_obj_flee_player_on_foot_always); // 754 всегда убегать от игрока
	lua_register(L, "set_char_obj_goto_player_on_foot", set_char_obj_goto_player_on_foot); // 755 идти к игроку пешком
	lua_register(L, "set_char_obj_no_obj", set_char_obj_no_obj); // 756 снять задачу с педа
	lua_register(L, "set_char_obj_catch_train", set_char_obj_catch_train); // 757 сесть на поезд
	lua_register(L, "set_char_obj_buy_ice_cream", set_char_obj_buy_ice_cream); // 758 купить мороженое
	lua_register(L, "set_char_obj_goto_area_on_foot", set_char_obj_goto_area_on_foot); // 759 идти к зоне пешком
	lua_register(L, "set_char_obj_destroy_car", set_char_obj_destroy_car); // 760 уничтожить машину
	lua_register(L, "set_char_obj_fire_at_object_from_vehicle", set_char_obj_fire_at_object_from_vehicle); // 761 стрелять по объекту из авто
	lua_register(L, "set_char_obj_follow_route", set_char_obj_follow_route); // 762 следовать по маршруту
	lua_register(L, "get_heli_forced_z", get_heli_forced_z); // 763 принудительная высота вертолёта (зависание)
	lua_register(L, "set_heli_forced_z", set_heli_forced_z); // 764 уст. принудительная высота вертолёта (зависание)
	lua_register(L, "get_heli_searchlight_x", get_heli_searchlight_x); // 765 координата X прожектора вертолёта
	lua_register(L, "set_heli_searchlight_x", set_heli_searchlight_x); // 766 уст. координата X прожектора вертолёта
	lua_register(L, "get_heli_searchlight_y", get_heli_searchlight_y); // 767 координата Y прожектора вертолёта
	lua_register(L, "set_heli_searchlight_y", set_heli_searchlight_y); // 768 уст. координата Y прожектора вертолёта
	lua_register(L, "get_heli_searchlight_intensity", get_heli_searchlight_intensity); // 769 яркость прожектора вертолёта (0..1)
	lua_register(L, "set_heli_searchlight_intensity", set_heli_searchlight_intensity); // 770 уст. яркость прожектора вертолёта (0..1)
	lua_register(L, "get_heli_rotor_angle", get_heli_rotor_angle); // 771 угол вращения винта вертолёта
	lua_register(L, "set_heli_rotor_angle", set_heli_rotor_angle); // 772 уст. угол вращения винта вертолёта
	lua_register(L, "heli_send_down_swat", heli_send_down_swat); // 773 спецназ спускается с вертолёта
	lua_register(L, "heli_get_num_random", heli_get_num_random); // 774 число случайных вертолётов
	lua_register(L, "heli_set_num_random", heli_set_num_random); // 775 уст. число случайных вертолётов
	lua_register(L, "heli_is_catalina_on", heli_is_catalina_on); // 776 вертолёт Катарины активен
	lua_register(L, "heli_set_catalina_on", heli_set_catalina_on); // 777 вкл/выкл вертолёт Катарины
	lua_register(L, "get_ped_velocity", get_ped_velocity); // 778 скорость педа m_vecMoveSpeed
	lua_register(L, "set_ped_velocity", set_ped_velocity); // 779 уст. скорость педа m_vecMoveSpeed
	lua_register(L, "get_object_velocity", get_object_velocity); // 780 скорость объекта m_vecMoveSpeed
	lua_register(L, "set_object_velocity", set_object_velocity); // 781 уст. скорость объекта m_vecMoveSpeed
	lua_register(L, "get_car_speed", get_car_speed); // 782 скалярная скорость авто (м/с)
	lua_register(L, "get_ped_speed", get_ped_speed); // 783 скалярная скорость педа (м/с)
	lua_register(L, "get_object_speed", get_object_speed); // 784 скалярная скорость объекта (м/с)
	lua_register(L, "add_ammo_to_char", add_ammo_to_char); // 785 добавить патронов педу в оружие
	lua_register(L, "set_char_ammo", set_char_ammo); // 786 установить патроны педу в оружие
	lua_register(L, "get_ammo_in_char_weapon", get_ammo_in_char_weapon); // 787 патроны в оружии педа
	lua_register(L, "get_char_weapon_in_slot", get_char_weapon_in_slot); // 788 оружие в слоте педа
	lua_register(L, "set_player_fast_reload", set_player_fast_reload); // 789 быстрая перезарядка игроку
	lua_register(L, "is_char_shooting_in_area", is_char_shooting_in_area); // 790 пед стреляет в зоне
	lua_register(L, "set_char_stop_shoot_dont_seek_entity", set_char_stop_shoot_dont_seek_entity); // 791 пед перестаёт стрелять и не ищет цель
	lua_register(L, "add_bullet_trace_radius", add_bullet_trace_radius); // 792 след пули: от точки к точке (радиус, время, прозрачность)
	lua_register(L, "add_bullet_trace_weapon", add_bullet_trace_weapon); // 793 след пули: оружием и сущностью
	lua_register(L, "get_bullet_trace_count", get_bullet_trace_count); // 794 число активных следов пуль
	lua_register(L, "get_bullet_trace_origin", get_bullet_trace_origin); // 795 точка начала следа пули
	lua_register(L, "get_bullet_trace_target", get_bullet_trace_target); // 796 точка конца следа пули
	lua_register(L, "clear_bullet_traces", clear_bullet_traces); // 797 удалить все следы пуль
	lua_register(L, "place_3d_marker", place_3d_marker); // 798 3D-маркер (стрелка/цилиндр)
	lua_register(L, "place_3d_marker_set", place_3d_marker_set); // 799 набор 3D-маркеров
	lua_register(L, "get_num_active_3d_markers", get_num_active_3d_markers); // 800 число активных 3D-маркеров
	lua_register(L, "get_pedstat_m_fFleeDistance", get_pedstat_m_fFleeDistance); // 801 дистанция бегства педа
	lua_register(L, "set_pedstat_m_fFleeDistance", set_pedstat_m_fFleeDistance); // 802 уст. дистанция бегства педа
	lua_register(L, "get_pedstat_m_fHeadingChangeRate", get_pedstat_m_fHeadingChangeRate); // 803 скорость поворота педа
	lua_register(L, "set_pedstat_m_fHeadingChangeRate", set_pedstat_m_fHeadingChangeRate); // 804 уст. скорость поворота педа
	lua_register(L, "get_pedstat_m_fAttackStrength", get_pedstat_m_fAttackStrength); // 805 сила атаки педа
	lua_register(L, "set_pedstat_m_fAttackStrength", set_pedstat_m_fAttackStrength); // 806 уст. сила атаки педа
	lua_register(L, "get_pedstat_m_fDefendWeakness", get_pedstat_m_fDefendWeakness); // 807 слабость защиты педа
	lua_register(L, "set_pedstat_m_fDefendWeakness", set_pedstat_m_fDefendWeakness); // 808 уст. слабость защиты педа
	lua_register(L, "get_car_door_status", get_car_door_status); // 809 состояние двери авто
	lua_register(L, "set_car_door_status", set_car_door_status); // 810 уст. состояние двери авто
	lua_register(L, "get_car_panel_status", get_car_panel_status); // 811 состояние панели авто
	lua_register(L, "get_car_light_status", get_car_light_status); // 812 состояние фары авто
	lua_register(L, "set_car_light_status", set_car_light_status); // 813 уст. состояние фары авто
	lua_register(L, "get_car_wheel_status", get_car_wheel_status); // 814 состояние колеса авто
	lua_register(L, "set_car_wheel_status", set_car_wheel_status); // 815 уст. состояние колеса авто
	lua_register(L, "get_car_engine_status", get_car_engine_status); // 816 состояние двигателя авто
	lua_register(L, "set_car_engine_status", set_car_engine_status); // 817 уст. состояние двигателя авто
	lua_register(L, "apply_car_damage", apply_car_damage); // 818 повредить компонент авто
	lua_register(L, "fuck_car_completely", fuck_car_completely); // 819 уничтожить авто визуально
	lua_register(L, "reset_car_damage", reset_car_damage); // 820 сбросить повреждения авто
	lua_register(L, "add_AutoPaintingBudget", add_AutoPaintingBudget); // 821 бюджет на покраску авто
	lua_register(L, "add_PropertyBudget", add_PropertyBudget); // 822 бюджет на недвижимость
	lua_register(L, "add_FashionBudget", add_FashionBudget); // 823 бюджет на одежду
	lua_register(L, "add_WeaponBudget", add_WeaponBudget); // 824 бюджет на оружие
	lua_register(L, "add_BloodRingKills", add_BloodRingKills); // 825 убийства в Bloodring
	lua_register(L, "add_PizzasDelivered", add_PizzasDelivered); // 826 доставленные пиццы
	lua_register(L, "add_Assassinations", add_Assassinations); // 827 заказные убийства
	lua_register(L, "add_MovieStunts", add_MovieStunts); // 828 киноскунты
	lua_register(L, "add_LoanSharks", add_LoanSharks); // 829 визиты к ростовщикам
	lua_register(L, "clear_all_char_anims", clear_all_char_anims); // 830 очистить все анимации педа
	lua_register(L, "clear_char_follow_path", clear_char_follow_path); // 831 очистить маршрут педа
	lua_register(L, "clear_char_last_damage_entity", clear_char_last_damage_entity); // 832 забыть последнюю повредившую сущность
	lua_register(L, "clear_char_last_weapon_damage", clear_char_last_weapon_damage); // 833 забыть последнее оружие урона
	lua_register(L, "char_set_idle", char_set_idle); // 834 пед в режим ожидания
	lua_register(L, "add_armour_to_char", add_armour_to_char); // 835 добавить брони педу
	lua_register(L, "add_armour_to_player", add_armour_to_player); // 836 добавить брони игроку
	lua_register(L, "add_ammo_to_player", add_ammo_to_player); // 837 добавить патронов игроку
	lua_register(L, "clear_area_of_cars", clear_area_of_cars); // 838 очистить зону от машин
	lua_register(L, "clear_area_of_chars", clear_area_of_chars); // 839 очистить зону от педов
	lua_register(L, "get_population_total_peds", get_population_total_peds); // 840 всего педов в мире
	lua_register(L, "get_population_civ_peds", get_population_civ_peds); // 841 мирных педов
	lua_register(L, "get_population_gang_peds", get_population_gang_peds); // 842 бандитских педов
	lua_register(L, "get_population_car_passengers", get_population_car_passengers); // 843 пассажиров в авто
	lua_register(L, "get_population_mission_peds", get_population_mission_peds); // 844 миссионных педов
	lua_register(L, "get_population_civ_male", get_population_civ_male); // 845 мужчин-мирных
	lua_register(L, "get_population_civ_female", get_population_civ_female); // 846 женщин-мирных
	lua_register(L, "get_population_max_in_use", get_population_max_in_use); // 847 макс. число используемых педов
	lua_register(L, "get_population_countdown", get_population_countdown); // 848 обратный отсчёт до спавна педов
	lua_register(L, "get_population_all_random_type", get_population_all_random_type); // 849 тип всех случайных педов
	lua_register(L, "set_population_all_random_type", set_population_all_random_type); // 850 уст. тип всех случайных педов
	lua_register(L, "get_hud_m_WantedTimer", get_hud_m_WantedTimer); // 851 таймер розыска на HUD
	lua_register(L, "set_hud_m_WantedTimer", set_hud_m_WantedTimer); // 852 таймер розыска на HUD
	lua_register(L, "get_hud_m_WeaponFadeTimer", get_hud_m_WeaponFadeTimer); // 853 таймер затухания оружия на HUD
	lua_register(L, "set_hud_m_WeaponFadeTimer", set_hud_m_WeaponFadeTimer); // 854 таймер затухания оружия на HUD
	lua_register(L, "get_hud_m_EnergyLostFadeTimer", get_hud_m_EnergyLostFadeTimer); // 855 таймер затухания потери энергии
	lua_register(L, "set_hud_m_EnergyLostFadeTimer", set_hud_m_EnergyLostFadeTimer); // 856 таймер затухания потери энергии
	lua_register(L, "get_hud_m_DisplayScoreFadeTimer", get_hud_m_DisplayScoreFadeTimer); // 857 таймер затухания счёта
	lua_register(L, "set_hud_m_DisplayScoreFadeTimer", set_hud_m_DisplayScoreFadeTimer); // 858 таймер затухания счёта
	lua_register(L, "get_hud_wanted_state", get_hud_wanted_state); // 859 состояние розыска на HUD
	lua_register(L, "get_hud_help_message_state", get_hud_help_message_state); // 860 состояние подсказки на HUD
	lua_register(L, "get_player_taxi_timer", get_player_taxi_timer); // 861 таймер такси игрока
	lua_register(L, "get_player_death_fade_timer", get_player_death_fade_timer); // 862 таймер затухания при смерти
	lua_register(L, "get_player_2wheels_time", get_player_2wheels_time); // 863 время на двух колёсах (мс)
	lua_register(L, "get_player_2wheels_distance", get_player_2wheels_distance); // 864 дистанция на двух колёсах
	lua_register(L, "get_player_road_density", get_player_road_density); // 865 плотность дорог вокруг игрока
	lua_register(L, "set_player_road_density", set_player_road_density); // 866 уст. плотность дорог вокруг игрока
	lua_register(L, "get_pedik_m_fYaw", get_pedik_m_fYaw); // 867 угол наклона головы педа
	lua_register(L, "set_pedik_m_fYaw", set_pedik_m_fYaw); // 868 уст. угол наклона головы педа
	lua_register(L, "get_pedik_m_fPitch", get_pedik_m_fPitch); // 869 наклон головы педа по тангажу
	lua_register(L, "set_pedik_m_fPitch", set_pedik_m_fPitch); // 870 уст. наклон головы педа по тангажу
	lua_register(L, "get_boat_m_fPropRotation", get_boat_m_fPropRotation); // 871 вращение винта лодки
	lua_register(L, "set_boat_m_fPropRotation", set_boat_m_fPropRotation); // 872 уст. вращение винта лодки
	lua_register(L, "get_boat_m_fPropSpeed", get_boat_m_fPropSpeed); // 873 скорость винта лодки
	lua_register(L, "set_boat_m_fPropSpeed", set_boat_m_fPropSpeed); // 874 уст. скорость винта лодки
	lua_register(L, "get_boat_m_fForcedZRotation", get_boat_m_fForcedZRotation); // 875 принудительный поворот лодки по Z
	lua_register(L, "set_boat_m_fForcedZRotation", set_boat_m_fForcedZRotation); // 876 уст. принудительный поворот лодки по Z
	lua_register(L, "get_boat_m_fBoatGasPedal", get_boat_m_fBoatGasPedal); // 877 газ лодки
	lua_register(L, "set_boat_m_fBoatGasPedal", set_boat_m_fBoatGasPedal); // 878 уст. газ лодки
	lua_register(L, "get_boat_m_fBoatSteeringLeftRight", get_boat_m_fBoatSteeringLeftRight); // 879 руль лодки
	lua_register(L, "set_boat_m_fBoatSteeringLeftRight", set_boat_m_fBoatSteeringLeftRight); // 880 уст. руль лодки
	lua_register(L, "get_bike_anim_group", get_bike_anim_group); // 881 группа анимаций мотоцикла
	lua_register(L, "set_bike_anim_group", set_bike_anim_group); // 882 уст. группу анимаций мотоцикла
	lua_register(L, "get_bike_damage_flags", get_bike_damage_flags); // 883 флаги повреждений мотоцикла
	lua_register(L, "set_bike_damage_flags", set_bike_damage_flags); // 884 уст. флаги повреждений мотоцикла
	lua_register(L, "get_time_step_non_clipped", get_time_step_non_clipped); // 885 шаг времени без клиппинга
	lua_register(L, "get_previous_time_ms", get_previous_time_ms); // 886 предыдущее время в мс
	lua_register(L, "get_time_ms_non_clipped", get_time_ms_non_clipped); // 887 время в мс без клиппинга
	lua_register(L, "is_slow_motion_active", is_slow_motion_active); // 888 активен ли слоумоушен
	lua_register(L, "get_cycles_per_millisecond", get_cycles_per_millisecond); // 889 циклы на миллисекунду
	lua_register(L, "get_clock_hours", get_clock_hours); // 890 часы игрового таймера
	lua_register(L, "get_clock_minutes", get_clock_minutes); // 891 минуты игрового таймера
	lua_register(L, "get_clock_seconds", get_clock_seconds); // 892 секунды игрового таймера
	lua_register(L, "get_clock_stored_hours", get_clock_stored_hours); // 893 сохранённые часы
	lua_register(L, "get_clock_stored_minutes", get_clock_stored_minutes); // 894 сохранённые минуты
	lua_register(L, "get_clock_minutes_until", get_clock_minutes_until); // 895 минут до заданного времени
	lua_register(L, "restore_clock", restore_clock); // 896 восстановить сохранённое время
	lua_register(L, "get_streaming_disabled", get_streaming_disabled); // 897 стриминг отключён
	lua_register(L, "set_streaming_disabled", set_streaming_disabled); // 898 отключить стриминг моделей
	lua_register(L, "get_streaming_models_requested", get_streaming_models_requested); // 899 запрошено моделей
	lua_register(L, "get_streaming_priority_requests", get_streaming_priority_requests); // 900 приоритетных запросов
	lua_register(L, "get_streaming_image_size", get_streaming_image_size); // 901 размер образа стриминга
	lua_register(L, "get_streaming_buffer_size", get_streaming_buffer_size); // 902 размер буфера стриминга
	lua_register(L, "get_streaming_current_ped_grp", get_streaming_current_ped_grp); // 903 текущая группа педов
	lua_register(L, "get_streaming_loading_big_model", get_streaming_loading_big_model); // 904 загружается большая модель
	lua_register(L, "get_streaming_peds_loaded", get_streaming_peds_loaded); // 905 загружено педов
	lua_register(L, "get_streaming_channel_error", get_streaming_channel_error); // 906 ошибка канала стриминга
	lua_register(L, "get_streaming_last_image_read", get_streaming_last_image_read); // 907 последний прочитанный образ
	lua_register(L, "get_streaming_old_sector_x", get_streaming_old_sector_x); // 908 старый сектор X
	lua_register(L, "get_streaming_old_sector_y", get_streaming_old_sector_y); // 909 старый сектор Y
	lua_register(L, "get_zone_level", get_zone_level); // 910 остров по координатам
	lua_register(L, "find_information_zone", find_information_zone); // 911 индекс информационной зоны
	lua_register(L, "get_navigation_zone", get_navigation_zone); // 912 навигационная зона по индексу
	lua_register(L, "init_zones", init_zones); // 913 инициализировать зоны
	lua_register(L, "traffic_lights_for_peds", traffic_lights_for_peds); // 914 светофор для пешеходов
	lua_register(L, "traffic_scan_for_lights", traffic_scan_for_lights); // 915 сканировать светофоры
	lua_register(L, "traffic_should_car_stop_for_bridge", traffic_should_car_stop_for_bridge); // 916 машина должна остановиться у моста
	lua_register(L, "should_car_stop_for_light", should_car_stop_for_light); // 917 должна ли машина остановиться на свет
	lua_register(L, "add_gun_flash_big", add_gun_flash_big); // 918 вспышка выстрела крупного оружия
	lua_register(L, "initialise_weapons", initialise_weapons); // 919 инициализировать оружие
	lua_register(L, "shutdown_weapons", shutdown_weapons); // 920 завершить работу оружия
	lua_register(L, "update_weapons", update_weapons); // 921 обновить оружие
	lua_register(L, "do_doom_aiming", do_doom_aiming); // 922 аим-траверс для оружия
	lua_register(L, "create_some_money", create_some_money); // 923 создать деньги на координатах
	lua_register(L, "give_player_goodies", give_player_goodies); // 924 выдать игроку бонусы по модели
	lua_register(L, "do_pickup_effects", do_pickup_effects); // 925 эффект поднятия пикапа
	lua_register(L, "do_money_effects", do_money_effects); // 926 эффект денег
	lua_register(L, "do_mine_effects", do_mine_effects); // 927 эффект мины
	lua_register(L, "get_obj_m_fAttachForce", get_obj_m_fAttachForce); // 928 сила привязки объекта
	lua_register(L, "set_obj_m_fAttachForce", set_obj_m_fAttachForce); // 929 уст. сила привязки объекта
	lua_register(L, "get_obj_m_CollisionDamageType", get_obj_m_CollisionDamageType); // 930 тип урона от столкновений
	lua_register(L, "set_obj_m_CollisionDamageType", set_obj_m_CollisionDamageType); // 931 уст. тип урона от столкновений
	lua_register(L, "get_obj_m_nSpecialCollisionType", get_obj_m_nSpecialCollisionType); // 932 специальный тип столкновений
	lua_register(L, "set_obj_m_nSpecialCollisionType", set_obj_m_nSpecialCollisionType); // 933 уст. специальный тип столкновений
	lua_register(L, "get_obj_m_nBounceScore", get_obj_m_nBounceScore); // 934 счёт отскока объекта
	lua_register(L, "set_obj_m_nBounceScore", set_obj_m_nBounceScore); // 935 уст. счёт отскока объекта
	lua_register(L, "get_obj_m_dwObjectTimer", get_obj_m_dwObjectTimer); // 936 таймер объекта
	lua_register(L, "set_obj_m_dwObjectTimer", set_obj_m_dwObjectTimer); // 937 уст. таймер объекта
	lua_register(L, "get_radar_range", get_radar_range); // 938 дальность радара
	lua_register(L, "set_radar_range", set_radar_range); // 939 уст. дальность радара
	lua_register(L, "set_sun_blocked_by_clouds", set_sun_blocked_by_clouds); // 940 уст. солнце закрыто облаками
	lua_register(L, "update_corona_coors", update_corona_coors); // 941 обновить координаты короны
	lua_register(L, "is_cutscene_running", is_cutscene_running); // 942 катсцена идёт
	lua_register(L, "get_num_cutscene_objs", get_num_cutscene_objs); // 943 число объектов катсцены
	lua_register(L, "get_cutscene_time_ms", get_cutscene_time_ms); // 944 время катсцены в мс
	lua_register(L, "choose_police_car_model", choose_police_car_model); // 945 выбрать модель полицейской машины
	lua_register(L, "generate_emergency_services", generate_emergency_services); // 946 вызвать экстренные службы
	lua_register(L, "clear_interesting_vehicles", clear_interesting_vehicles); // 947 очистить список интересных машин
	lua_register(L, "drag_car_to_point", drag_car_to_point); // 948 тащить машину к точке
	lua_register(L, "find_max_speed_in_traffic", find_max_speed_in_traffic); // 949 макс. скорость машины в трафике
	lua_register(L, "get_num_bright_lights", get_num_bright_lights); // 950 число ярких огней
	lua_register(L, "get_car_m_nPrimaryColor", get_car_m_nPrimaryColor); // 951 m_nPrimaryColor (CVehicle)
	lua_register(L, "set_car_m_nPrimaryColor", set_car_m_nPrimaryColor); // 952 уст. m_nPrimaryColor (CVehicle)
	lua_register(L, "get_car_m_nSecondaryColor", get_car_m_nSecondaryColor); // 953 m_nSecondaryColor (CVehicle)
	lua_register(L, "set_car_m_nSecondaryColor", set_car_m_nSecondaryColor); // 954 уст. m_nSecondaryColor (CVehicle)
	lua_register(L, "get_car_m_wWantedStarsOnEnter", get_car_m_wWantedStarsOnEnter); // 955 m_wWantedStarsOnEnter (CVehicle)
	lua_register(L, "set_car_m_wWantedStarsOnEnter", set_car_m_wWantedStarsOnEnter); // 956 уст. m_wWantedStarsOnEnter (CVehicle)
	lua_register(L, "get_car_m_wMissionValue", get_car_m_wMissionValue); // 957 m_wMissionValue (CVehicle)
	lua_register(L, "set_car_m_wMissionValue", set_car_m_wMissionValue); // 958 уст. m_wMissionValue (CVehicle)
	lua_register(L, "get_car_m_nNumPassengers", get_car_m_nNumPassengers); // 959 m_nNumPassengers (CVehicle)
	lua_register(L, "set_car_m_nNumPassengers", set_car_m_nNumPassengers); // 960 уст. m_nNumPassengers (CVehicle)
	lua_register(L, "get_car_m_nNumGettingIn", get_car_m_nNumGettingIn); // 961 m_nNumGettingIn (CVehicle)
	lua_register(L, "set_car_m_nNumGettingIn", set_car_m_nNumGettingIn); // 962 уст. m_nNumGettingIn (CVehicle)
	lua_register(L, "get_car_m_nGettingInFlags", get_car_m_nGettingInFlags); // 963 m_nGettingInFlags (CVehicle)
	lua_register(L, "set_car_m_nGettingInFlags", set_car_m_nGettingInFlags); // 964 уст. m_nGettingInFlags (CVehicle)
	lua_register(L, "get_car_m_nGettingOutFlags", get_car_m_nGettingOutFlags); // 965 m_nGettingOutFlags (CVehicle)
	lua_register(L, "set_car_m_nGettingOutFlags", set_car_m_nGettingOutFlags); // 966 уст. m_nGettingOutFlags (CVehicle)
	lua_register(L, "get_car_m_nMaxPassengers", get_car_m_nMaxPassengers); // 967 m_nMaxPassengers (CVehicle)
	lua_register(L, "set_car_m_nMaxPassengers", set_car_m_nMaxPassengers); // 968 уст. m_nMaxPassengers (CVehicle)
	lua_register(L, "get_car_m_fSteerAngle", get_car_m_fSteerAngle); // 969 m_fSteerAngle (CVehicle)
	lua_register(L, "set_car_m_fSteerAngle", set_car_m_fSteerAngle); // 970 уст. m_fSteerAngle (CVehicle)
	lua_register(L, "get_car_m_fGasPedal", get_car_m_fGasPedal); // 971 m_fGasPedal (CVehicle)
	lua_register(L, "set_car_m_fGasPedal", set_car_m_fGasPedal); // 972 уст. m_fGasPedal (CVehicle)
	lua_register(L, "get_car_m_fBreakPedal", get_car_m_fBreakPedal); // 973 m_fBreakPedal (CVehicle)
	lua_register(L, "set_car_m_fBreakPedal", set_car_m_fBreakPedal); // 974 уст. m_fBreakPedal (CVehicle)
	lua_register(L, "get_car_m_nCreatedBy", get_car_m_nCreatedBy); // 975 m_nCreatedBy (CVehicle)
	lua_register(L, "set_car_m_nCreatedBy", set_car_m_nCreatedBy); // 976 уст. m_nCreatedBy (CVehicle)
	lua_register(L, "get_car_m_nAmmoInClip", get_car_m_nAmmoInClip); // 977 m_nAmmoInClip (CVehicle)
	lua_register(L, "set_car_m_nAmmoInClip", set_car_m_nAmmoInClip); // 978 уст. m_nAmmoInClip (CVehicle)
	lua_register(L, "get_car_m_fHealth", get_car_m_fHealth); // 979 m_fHealth (CVehicle)
	lua_register(L, "set_car_m_fHealth", set_car_m_fHealth); // 980 уст. m_fHealth (CVehicle)
	lua_register(L, "get_car_m_nCurrentGear", get_car_m_nCurrentGear); // 981 m_nCurrentGear (CVehicle)
	lua_register(L, "set_car_m_nCurrentGear", set_car_m_nCurrentGear); // 982 уст. m_nCurrentGear (CVehicle)
	lua_register(L, "get_car_m_nTimeTillWeNeedThisCar", get_car_m_nTimeTillWeNeedThisCar); // 983 m_nTimeTillWeNeedThisCar (CVehicle)
	lua_register(L, "set_car_m_nTimeTillWeNeedThisCar", set_car_m_nTimeTillWeNeedThisCar); // 984 уст. m_nTimeTillWeNeedThisCar (CVehicle)
	lua_register(L, "get_car_m_nTimeOfDeath", get_car_m_nTimeOfDeath); // 985 m_nTimeOfDeath (CVehicle)
	lua_register(L, "set_car_m_nTimeOfDeath", set_car_m_nTimeOfDeath); // 986 уст. m_nTimeOfDeath (CVehicle)
	lua_register(L, "get_car_m_wBombTimer", get_car_m_wBombTimer); // 987 m_wBombTimer (CVehicle)
	lua_register(L, "set_car_m_wBombTimer", set_car_m_wBombTimer); // 988 уст. m_wBombTimer (CVehicle)
	lua_register(L, "get_car_m_nLastWeaponDamage", get_car_m_nLastWeaponDamage); // 989 m_nLastWeaponDamage (CVehicle)
	lua_register(L, "set_car_m_nLastWeaponDamage", set_car_m_nLastWeaponDamage); // 990 уст. m_nLastWeaponDamage (CVehicle)
	lua_register(L, "get_car_m_nRadioStation", get_car_m_nRadioStation); // 991 m_nRadioStation (CVehicle)
	lua_register(L, "set_car_m_nRadioStation", set_car_m_nRadioStation); // 992 уст. m_nRadioStation (CVehicle)
	lua_register(L, "get_car_m_bHornEnabled", get_car_m_bHornEnabled); // 993 m_bHornEnabled (CVehicle)
	lua_register(L, "set_car_m_bHornEnabled", set_car_m_bHornEnabled); // 994 уст. m_bHornEnabled (CVehicle)
	lua_register(L, "get_car_m_nSirenOrAlarm", get_car_m_nSirenOrAlarm); // 995 m_nSirenOrAlarm (CVehicle)
	lua_register(L, "set_car_m_nSirenOrAlarm", set_car_m_nSirenOrAlarm); // 996 уст. m_nSirenOrAlarm (CVehicle)
	lua_register(L, "get_car_m_nSirenExtra", get_car_m_nSirenExtra); // 997 m_nSirenExtra (CVehicle)
	lua_register(L, "set_car_m_nSirenExtra", set_car_m_nSirenExtra); // 998 уст. m_nSirenExtra (CVehicle)
	lua_register(L, "get_car_m_fSteerRatio", get_car_m_fSteerRatio); // 999 m_fSteerRatio (CVehicle)
	lua_register(L, "set_car_m_fSteerRatio", set_car_m_fSteerRatio); // 1000 уст. m_fSteerRatio (CVehicle)
	lua_register(L, "get_car_m_nVehicleClass", get_car_m_nVehicleClass); // 1001 m_nVehicleClass (CVehicle)
	lua_register(L, "set_car_m_nVehicleClass", set_car_m_nVehicleClass); // 1002 уст. m_nVehicleClass (CVehicle)
	lua_register(L, "get_ped_m_fCollisionSpeed", get_ped_m_fCollisionSpeed); // 1003 m_fCollisionSpeed (CPed)
	lua_register(L, "set_ped_m_fCollisionSpeed", set_ped_m_fCollisionSpeed); // 1004 уст. m_fCollisionSpeed (CPed)
	lua_register(L, "get_ped_m_nGangFlags", get_ped_m_nGangFlags); // 1005 m_nGangFlags (CPed)
	lua_register(L, "set_ped_m_nGangFlags", set_ped_m_nGangFlags); // 1006 уст. m_nGangFlags (CPed)
	lua_register(L, "get_ped_m_nPedStatus", get_ped_m_nPedStatus); // 1007 m_nPedStatus (CPed)
	lua_register(L, "set_ped_m_nPedStatus", set_ped_m_nPedStatus); // 1008 уст. m_nPedStatus (CPed)
	lua_register(L, "get_ped_m_vecObjective", get_ped_m_vecObjective); // 1009 m_vecObjective (CPed)
	lua_register(L, "set_ped_m_vecObjective", set_ped_m_vecObjective); // 1010 уст. m_vecObjective (CPed)
	lua_register(L, "get_ped_m_fObjectiveAngle", get_ped_m_fObjectiveAngle); // 1011 m_fObjectiveAngle (CPed)
	lua_register(L, "set_ped_m_fObjectiveAngle", set_ped_m_fObjectiveAngle); // 1012 уст. m_fObjectiveAngle (CPed)
	lua_register(L, "get_ped_m_nPedFormation", get_ped_m_nPedFormation); // 1013 m_nPedFormation (CPed)
	lua_register(L, "set_ped_m_nPedFormation", set_ped_m_nPedFormation); // 1014 уст. m_nPedFormation (CPed)
	lua_register(L, "get_ped_m_nFearFlags", get_ped_m_nFearFlags); // 1015 m_nFearFlags (CPed)
	lua_register(L, "set_ped_m_nFearFlags", set_ped_m_nFearFlags); // 1016 уст. m_nFearFlags (CPed)
	lua_register(L, "get_ped_m_nEventType", get_ped_m_nEventType); // 1017 m_nEventType (CPed)
	lua_register(L, "set_ped_m_nEventType", set_ped_m_nEventType); // 1018 уст. m_nEventType (CPed)
	lua_register(L, "get_ped_m_fAngleToEvent", get_ped_m_fAngleToEvent); // 1019 m_fAngleToEvent (CPed)
	lua_register(L, "set_ped_m_fAngleToEvent", set_ped_m_fAngleToEvent); // 1020 уст. m_fAngleToEvent (CPed)
	lua_register(L, "get_ped_m_nAnimGroupId", get_ped_m_nAnimGroupId); // 1021 m_nAnimGroupId (CPed)
	lua_register(L, "set_ped_m_nAnimGroupId", set_ped_m_nAnimGroupId); // 1022 уст. m_nAnimGroupId (CPed)
	lua_register(L, "get_ped_m_vecOffsetSeek", get_ped_m_vecOffsetSeek); // 1023 m_vecOffsetSeek (CPed)
	lua_register(L, "set_ped_m_vecOffsetSeek", set_ped_m_vecOffsetSeek); // 1024 уст. m_vecOffsetSeek (CPed)
	lua_register(L, "get_ped_m_nActionTimer", get_ped_m_nActionTimer); // 1025 m_nActionTimer (CPed)
	lua_register(L, "set_ped_m_nActionTimer", set_ped_m_nActionTimer); // 1026 уст. m_nActionTimer (CPed)
	lua_register(L, "get_ped_m_nWaitState", get_ped_m_nWaitState); // 1027 m_nWaitState (CPed)
	lua_register(L, "set_ped_m_nWaitState", set_ped_m_nWaitState); // 1028 уст. m_nWaitState (CPed)
	lua_register(L, "get_ped_m_nWaitTimer", get_ped_m_nWaitTimer); // 1029 m_nWaitTimer (CPed)
	lua_register(L, "set_ped_m_nWaitTimer", set_ped_m_nWaitTimer); // 1030 уст. m_nWaitTimer (CPed)
	lua_register(L, "get_ped_m_wPathNodes", get_ped_m_wPathNodes); // 1031 m_wPathNodes (CPed)
	lua_register(L, "set_ped_m_wPathNodes", set_ped_m_wPathNodes); // 1032 уст. m_wPathNodes (CPed)
	lua_register(L, "get_ped_m_wCurPathNode", get_ped_m_wCurPathNode); // 1033 m_wCurPathNode (CPed)
	lua_register(L, "set_ped_m_wCurPathNode", set_ped_m_wCurPathNode); // 1034 уст. m_wCurPathNode (CPed)
	lua_register(L, "get_ped_m_nPathNodeTimer", get_ped_m_nPathNodeTimer); // 1035 m_nPathNodeTimer (CPed)
	lua_register(L, "set_ped_m_nPathNodeTimer", set_ped_m_nPathNodeTimer); // 1036 уст. m_nPathNodeTimer (CPed)
	lua_register(L, "get_ped_m_vecPathNextNode", get_ped_m_vecPathNextNode); // 1037 m_vecPathNextNode (CPed)
	lua_register(L, "set_ped_m_vecPathNextNode", set_ped_m_vecPathNextNode); // 1038 уст. m_vecPathNextNode (CPed)
	lua_register(L, "get_ped_m_fPathNextNodeDir", get_ped_m_fPathNextNodeDir); // 1039 m_fPathNextNodeDir (CPed)
	lua_register(L, "set_ped_m_fPathNextNodeDir", set_ped_m_fPathNextNodeDir); // 1040 уст. m_fPathNextNodeDir (CPed)
	lua_register(L, "get_ped_m_nPathNodeType", get_ped_m_nPathNodeType); // 1041 m_nPathNodeType (CPed)
	lua_register(L, "set_ped_m_nPathNodeType", set_ped_m_nPathNodeType); // 1042 уст. m_nPathNodeType (CPed)
	lua_register(L, "get_ped_m_fHealth", get_ped_m_fHealth); // 1043 m_fHealth (CPed)
	lua_register(L, "set_ped_m_fHealth", set_ped_m_fHealth); // 1044 уст. m_fHealth (CPed)
	lua_register(L, "get_ped_m_fArmour", get_ped_m_fArmour); // 1045 m_fArmour (CPed)
	lua_register(L, "set_ped_m_fArmour", set_ped_m_fArmour); // 1046 уст. m_fArmour (CPed)
	lua_register(L, "get_ped_m_nShadowUpdateTimer", get_ped_m_nShadowUpdateTimer); // 1047 m_nShadowUpdateTimer (CPed)
	lua_register(L, "set_ped_m_nShadowUpdateTimer", set_ped_m_nShadowUpdateTimer); // 1048 уст. m_nShadowUpdateTimer (CPed)
	lua_register(L, "get_ped_wRouteLastPoint", get_ped_wRouteLastPoint); // 1049 wRouteLastPoint (CPed)
	lua_register(L, "set_ped_wRouteLastPoint", set_ped_wRouteLastPoint); // 1050 уст. wRouteLastPoint (CPed)
	lua_register(L, "get_ped_wRoutePoints", get_ped_wRoutePoints); // 1051 wRoutePoints (CPed)
	lua_register(L, "set_ped_wRoutePoints", set_ped_wRoutePoints); // 1052 уст. wRoutePoints (CPed)
	lua_register(L, "get_ped_wRoutePos", get_ped_wRoutePos); // 1053 wRoutePos (CPed)
	lua_register(L, "set_ped_wRoutePos", set_ped_wRoutePos); // 1054 уст. wRoutePos (CPed)
	lua_register(L, "get_ped_wRouteType", get_ped_wRouteType); // 1055 wRouteType (CPed)
	lua_register(L, "set_ped_wRouteType", set_ped_wRouteType); // 1056 уст. wRouteType (CPed)
	lua_register(L, "get_ped_wRouteCurDir", get_ped_wRouteCurDir); // 1057 wRouteCurDir (CPed)
	lua_register(L, "set_ped_wRouteCurDir", set_ped_wRouteCurDir); // 1058 уст. wRouteCurDir (CPed)
	lua_register(L, "get_ped_m_fHeadingCurrent", get_ped_m_fHeadingCurrent); // 1059 m_fHeadingCurrent (CPed)
	lua_register(L, "set_ped_m_fHeadingCurrent", set_ped_m_fHeadingCurrent); // 1060 уст. m_fHeadingCurrent (CPed)
	lua_register(L, "get_ped_m_fHeadingGoal", get_ped_m_fHeadingGoal); // 1061 m_fHeadingGoal (CPed)
	lua_register(L, "set_ped_m_fHeadingGoal", set_ped_m_fHeadingGoal); // 1062 уст. m_fHeadingGoal (CPed)
	lua_register(L, "get_ped_m_fHeadingChangeRate", get_ped_m_fHeadingChangeRate); // 1063 m_fHeadingChangeRate (CPed)
	lua_register(L, "set_ped_m_fHeadingChangeRate", set_ped_m_fHeadingChangeRate); // 1064 уст. m_fHeadingChangeRate (CPed)
	lua_register(L, "get_ped_m_nEnterType", get_ped_m_nEnterType); // 1065 m_nEnterType (CPed)
	lua_register(L, "set_ped_m_nEnterType", set_ped_m_nEnterType); // 1066 уст. m_nEnterType (CPed)
	lua_register(L, "get_ped_m_nWalkAroundType", get_ped_m_nWalkAroundType); // 1067 m_nWalkAroundType (CPed)
	lua_register(L, "set_ped_m_nWalkAroundType", set_ped_m_nWalkAroundType); // 1068 уст. m_nWalkAroundType (CPed)
	lua_register(L, "get_ped_m_vecOffsetFromPhysSurface", get_ped_m_vecOffsetFromPhysSurface); // 1069 m_vecOffsetFromPhysSurface (CPed)
	lua_register(L, "set_ped_m_vecOffsetFromPhysSurface", set_ped_m_vecOffsetFromPhysSurface); // 1070 уст. m_vecOffsetFromPhysSurface (CPed)
	lua_register(L, "get_ped_vecSeekVehicle", get_ped_vecSeekVehicle); // 1071 vecSeekVehicle (CPed)
	lua_register(L, "set_ped_vecSeekVehicle", set_ped_vecSeekVehicle); // 1072 уст. vecSeekVehicle (CPed)
	lua_register(L, "get_ped_m_bInVehicle", get_ped_m_bInVehicle); // 1073 m_bInVehicle (CPed)
	lua_register(L, "set_ped_m_bInVehicle", set_ped_m_bInVehicle); // 1074 уст. m_bInVehicle (CPed)
	lua_register(L, "get_ped_m_fSeatPrecisionX", get_ped_m_fSeatPrecisionX); // 1075 m_fSeatPrecisionX (CPed)
	lua_register(L, "set_ped_m_fSeatPrecisionX", set_ped_m_fSeatPrecisionX); // 1076 уст. m_fSeatPrecisionX (CPed)
	lua_register(L, "get_ped_m_fSeatPrecisionY", get_ped_m_fSeatPrecisionY); // 1077 m_fSeatPrecisionY (CPed)
	lua_register(L, "set_ped_m_fSeatPrecisionY", set_ped_m_fSeatPrecisionY); // 1078 уст. m_fSeatPrecisionY (CPed)
	lua_register(L, "get_ped_m_nSeatType", get_ped_m_nSeatType); // 1079 m_nSeatType (CPed)
	lua_register(L, "set_ped_m_nSeatType", set_ped_m_nSeatType); // 1080 уст. m_nSeatType (CPed)
	lua_register(L, "get_ped_m_bHasPhone", get_ped_m_bHasPhone); // 1081 m_bHasPhone (CPed)
	lua_register(L, "set_ped_m_bHasPhone", set_ped_m_bHasPhone); // 1082 уст. m_bHasPhone (CPed)
	lua_register(L, "get_ped_m_wPhoneId", get_ped_m_wPhoneId); // 1083 m_wPhoneId (CPed)
	lua_register(L, "set_ped_m_wPhoneId", set_ped_m_wPhoneId); // 1084 уст. m_wPhoneId (CPed)
	lua_register(L, "get_ped_m_nLookingForPhone", get_ped_m_nLookingForPhone); // 1085 m_nLookingForPhone (CPed)
	lua_register(L, "set_ped_m_nLookingForPhone", set_ped_m_nLookingForPhone); // 1086 уст. m_nLookingForPhone (CPed)
	lua_register(L, "get_ped_m_nPhoneTalkTimer", get_ped_m_nPhoneTalkTimer); // 1087 m_nPhoneTalkTimer (CPed)
	lua_register(L, "set_ped_m_nPhoneTalkTimer", set_ped_m_nPhoneTalkTimer); // 1088 уст. m_nPhoneTalkTimer (CPed)
	lua_register(L, "get_ped_m_fFleeFromPosX", get_ped_m_fFleeFromPosX); // 1089 m_fFleeFromPosX (CPed)
	lua_register(L, "set_ped_m_fFleeFromPosX", set_ped_m_fFleeFromPosX); // 1090 уст. m_fFleeFromPosX (CPed)
	lua_register(L, "get_ped_m_fFleeFromPosY", get_ped_m_fFleeFromPosY); // 1091 m_fFleeFromPosY (CPed)
	lua_register(L, "set_ped_m_fFleeFromPosY", set_ped_m_fFleeFromPosY); // 1092 уст. m_fFleeFromPosY (CPed)
	lua_register(L, "get_ped_m_nFleeTimer", get_ped_m_nFleeTimer); // 1093 m_nFleeTimer (CPed)
	lua_register(L, "set_ped_m_nFleeTimer", set_ped_m_nFleeTimer); // 1094 уст. m_nFleeTimer (CPed)
	lua_register(L, "get_ped_m_nLastThreatTimer", get_ped_m_nLastThreatTimer); // 1095 m_nLastThreatTimer (CPed)
	lua_register(L, "set_ped_m_nLastThreatTimer", set_ped_m_nLastThreatTimer); // 1096 уст. m_nLastThreatTimer (CPed)
	lua_register(L, "get_ped_m_nStateUnused", get_ped_m_nStateUnused); // 1097 m_nStateUnused (CPed)
	lua_register(L, "set_ped_m_nStateUnused", set_ped_m_nStateUnused); // 1098 уст. m_nStateUnused (CPed)
	lua_register(L, "get_ped_m_nTimerUnused", get_ped_m_nTimerUnused); // 1099 m_nTimerUnused (CPed)
	lua_register(L, "set_ped_m_nTimerUnused", set_ped_m_nTimerUnused); // 1100 уст. m_nTimerUnused (CPed)
	lua_register(L, "get_ped_m_nAtchStoredWep", get_ped_m_nAtchStoredWep); // 1101 m_nAtchStoredWep (CPed)
	lua_register(L, "set_ped_m_nAtchStoredWep", set_ped_m_nAtchStoredWep); // 1102 уст. m_nAtchStoredWep (CPed)
	lua_register(L, "get_ped_m_nStoredGiveWep", get_ped_m_nStoredGiveWep); // 1103 m_nStoredGiveWep (CPed)
	lua_register(L, "set_ped_m_nStoredGiveWep", set_ped_m_nStoredGiveWep); // 1104 уст. m_nStoredGiveWep (CPed)
	lua_register(L, "get_ped_m_nStoredGiveAmmo", get_ped_m_nStoredGiveAmmo); // 1105 m_nStoredGiveAmmo (CPed)
	lua_register(L, "set_ped_m_nStoredGiveAmmo", set_ped_m_nStoredGiveAmmo); // 1106 уст. m_nStoredGiveAmmo (CPed)
	lua_register(L, "get_ped_m_nCurrentWeapon", get_ped_m_nCurrentWeapon); // 1107 m_nCurrentWeapon (CPed)
	lua_register(L, "set_ped_m_nCurrentWeapon", set_ped_m_nCurrentWeapon); // 1108 уст. m_nCurrentWeapon (CPed)
	lua_register(L, "get_ped_m_nWepSkills", get_ped_m_nWepSkills); // 1109 m_nWepSkills (CPed)
	lua_register(L, "set_ped_m_nWepSkills", set_ped_m_nWepSkills); // 1110 уст. m_nWepSkills (CPed)
	lua_register(L, "get_ped_m_nWeaponAccuracy", get_ped_m_nWeaponAccuracy); // 1111 m_nWeaponAccuracy (CPed)
	lua_register(L, "set_ped_m_nWeaponAccuracy", set_ped_m_nWeaponAccuracy); // 1112 уст. m_nWeaponAccuracy (CPed)
	lua_register(L, "get_ped_m_nBodyPart", get_ped_m_nBodyPart); // 1113 m_nBodyPart (CPed)
	lua_register(L, "set_ped_m_nBodyPart", set_ped_m_nBodyPart); // 1114 уст. m_nBodyPart (CPed)
	lua_register(L, "get_ped_m_vecHitLastPos", get_ped_m_vecHitLastPos); // 1115 m_vecHitLastPos (CPed)
	lua_register(L, "set_ped_m_vecHitLastPos", set_ped_m_vecHitLastPos); // 1116 уст. m_vecHitLastPos (CPed)
	lua_register(L, "get_ped_m_nHitCounter", get_ped_m_nHitCounter); // 1117 m_nHitCounter (CPed)
	lua_register(L, "set_ped_m_nHitCounter", set_ped_m_nHitCounter); // 1118 уст. m_nHitCounter (CPed)
	lua_register(L, "get_ped_m_nLastHitState", get_ped_m_nLastHitState); // 1119 m_nLastHitState (CPed)
	lua_register(L, "set_ped_m_nLastHitState", set_ped_m_nLastHitState); // 1120 уст. m_nLastHitState (CPed)
	lua_register(L, "get_ped_m_nFightFlags1", get_ped_m_nFightFlags1); // 1121 m_nFightFlags1 (CPed)
	lua_register(L, "set_ped_m_nFightFlags1", set_ped_m_nFightFlags1); // 1122 уст. m_nFightFlags1 (CPed)
	lua_register(L, "get_ped_m_nFightFlags2", get_ped_m_nFightFlags2); // 1123 m_nFightFlags2 (CPed)
	lua_register(L, "set_ped_m_nFightFlags2", set_ped_m_nFightFlags2); // 1124 уст. m_nFightFlags2 (CPed)
	lua_register(L, "get_ped_m_nFightFlags3", get_ped_m_nFightFlags3); // 1125 m_nFightFlags3 (CPed)
	lua_register(L, "set_ped_m_nFightFlags3", set_ped_m_nFightFlags3); // 1126 уст. m_nFightFlags3 (CPed)
	lua_register(L, "get_ped_m_nBleedCounter", get_ped_m_nBleedCounter); // 1127 m_nBleedCounter (CPed)
	lua_register(L, "set_ped_m_nBleedCounter", set_ped_m_nBleedCounter); // 1128 уст. m_nBleedCounter (CPed)
	lua_register(L, "get_ped_m_fLookDirection", get_ped_m_fLookDirection); // 1129 m_fLookDirection (CPed)
	lua_register(L, "set_ped_m_fLookDirection", set_ped_m_fLookDirection); // 1130 уст. m_fLookDirection (CPed)
	lua_register(L, "get_ped_m_nWepModelID", get_ped_m_nWepModelID); // 1131 m_nWepModelID (CPed)
	lua_register(L, "set_ped_m_nWepModelID", set_ped_m_nWepModelID); // 1132 уст. m_nWepModelID (CPed)
	lua_register(L, "get_ped_m_nLeaveCarTimer", get_ped_m_nLeaveCarTimer); // 1133 m_nLeaveCarTimer (CPed)
	lua_register(L, "set_ped_m_nLeaveCarTimer", set_ped_m_nLeaveCarTimer); // 1134 уст. m_nLeaveCarTimer (CPed)
	lua_register(L, "get_ped_m_nGetUpTimer", get_ped_m_nGetUpTimer); // 1135 m_nGetUpTimer (CPed)
	lua_register(L, "set_ped_m_nGetUpTimer", set_ped_m_nGetUpTimer); // 1136 уст. m_nGetUpTimer (CPed)
	lua_register(L, "get_ped_m_nLookTimer", get_ped_m_nLookTimer); // 1137 m_nLookTimer (CPed)
	lua_register(L, "set_ped_m_nLookTimer", set_ped_m_nLookTimer); // 1138 уст. m_nLookTimer (CPed)
	lua_register(L, "get_ped_m_nStandardTimer", get_ped_m_nStandardTimer); // 1139 m_nStandardTimer (CPed)
	lua_register(L, "set_ped_m_nStandardTimer", set_ped_m_nStandardTimer); // 1140 уст. m_nStandardTimer (CPed)
	lua_register(L, "get_ped_m_nAttackTimer", get_ped_m_nAttackTimer); // 1141 m_nAttackTimer (CPed)
	lua_register(L, "set_ped_m_nAttackTimer", set_ped_m_nAttackTimer); // 1142 уст. m_nAttackTimer (CPed)
	lua_register(L, "get_ped_m_nLastHitTime", get_ped_m_nLastHitTime); // 1143 m_nLastHitTime (CPed)
	lua_register(L, "set_ped_m_nLastHitTime", set_ped_m_nLastHitTime); // 1144 уст. m_nLastHitTime (CPed)
	lua_register(L, "get_ped_m_nHitRecoverTimer", get_ped_m_nHitRecoverTimer); // 1145 m_nHitRecoverTimer (CPed)
	lua_register(L, "set_ped_m_nHitRecoverTimer", set_ped_m_nHitRecoverTimer); // 1146 уст. m_nHitRecoverTimer (CPed)
	lua_register(L, "get_ped_m_nObjectiveTimer", get_ped_m_nObjectiveTimer); // 1147 m_nObjectiveTimer (CPed)
	lua_register(L, "set_ped_m_nObjectiveTimer", set_ped_m_nObjectiveTimer); // 1148 уст. m_nObjectiveTimer (CPed)
	lua_register(L, "get_ped_m_nDuckTimer", get_ped_m_nDuckTimer); // 1149 m_nDuckTimer (CPed)
	lua_register(L, "set_ped_m_nDuckTimer", set_ped_m_nDuckTimer); // 1150 уст. m_nDuckTimer (CPed)
	lua_register(L, "get_ped_m_nDuckAndCoverTimer", get_ped_m_nDuckAndCoverTimer); // 1151 m_nDuckAndCoverTimer (CPed)
	lua_register(L, "set_ped_m_nDuckAndCoverTimer", set_ped_m_nDuckAndCoverTimer); // 1152 уст. m_nDuckAndCoverTimer (CPed)
	lua_register(L, "get_ped_m_nBloodyTimer", get_ped_m_nBloodyTimer); // 1153 m_nBloodyTimer (CPed)
	lua_register(L, "set_ped_m_nBloodyTimer", set_ped_m_nBloodyTimer); // 1154 уст. m_nBloodyTimer (CPed)
	lua_register(L, "get_ped_m_nShotTime", get_ped_m_nShotTime); // 1155 m_nShotTime (CPed)
	lua_register(L, "set_ped_m_nShotTime", set_ped_m_nShotTime); // 1156 уст. m_nShotTime (CPed)
	lua_register(L, "get_ped_m_nShotTimeAdd", get_ped_m_nShotTimeAdd); // 1157 m_nShotTimeAdd (CPed)
	lua_register(L, "set_ped_m_nShotTimeAdd", set_ped_m_nShotTimeAdd); // 1158 уст. m_nShotTimeAdd (CPed)
	lua_register(L, "get_ped_m_nPanicCounter", get_ped_m_nPanicCounter); // 1159 m_nPanicCounter (CPed)
	lua_register(L, "set_ped_m_nPanicCounter", set_ped_m_nPanicCounter); // 1160 уст. m_nPanicCounter (CPed)
	lua_register(L, "get_ped_m_nDeadBleeding", get_ped_m_nDeadBleeding); // 1161 m_nDeadBleeding (CPed)
	lua_register(L, "set_ped_m_nDeadBleeding", set_ped_m_nDeadBleeding); // 1162 уст. m_nDeadBleeding (CPed)
	lua_register(L, "get_ped_m_nBodyPartBleeding", get_ped_m_nBodyPartBleeding); // 1163 m_nBodyPartBleeding (CPed)
	lua_register(L, "set_ped_m_nBodyPartBleeding", set_ped_m_nBodyPartBleeding); // 1164 уст. m_nBodyPartBleeding (CPed)
	lua_register(L, "get_ped_m_nNumNearPeds", get_ped_m_nNumNearPeds); // 1165 m_nNumNearPeds (CPed)
	lua_register(L, "set_ped_m_nNumNearPeds", set_ped_m_nNumNearPeds); // 1166 уст. m_nNumNearPeds (CPed)
	lua_register(L, "get_ped_m_nPedMoney", get_ped_m_nPedMoney); // 1167 m_nPedMoney (CPed)
	lua_register(L, "set_ped_m_nPedMoney", set_ped_m_nPedMoney); // 1168 уст. m_nPedMoney (CPed)
	lua_register(L, "get_ped_m_nLastDamWep", get_ped_m_nLastDamWep); // 1169 m_nLastDamWep (CPed)
	lua_register(L, "set_ped_m_nLastDamWep", set_ped_m_nLastDamWep); // 1170 уст. m_nLastDamWep (CPed)
	lua_register(L, "get_ped_m_vecAttachOffset", get_ped_m_vecAttachOffset); // 1171 m_vecAttachOffset (CPed)
	lua_register(L, "set_ped_m_vecAttachOffset", set_ped_m_vecAttachOffset); // 1172 уст. m_vecAttachOffset (CPed)
	lua_register(L, "get_ped_m_nAttachType", get_ped_m_nAttachType); // 1173 m_nAttachType (CPed)
	lua_register(L, "set_ped_m_nAttachType", set_ped_m_nAttachType); // 1174 уст. m_nAttachType (CPed)
	lua_register(L, "get_ped_m_fAttachRot", get_ped_m_fAttachRot); // 1175 m_fAttachRot (CPed)
	lua_register(L, "set_ped_m_fAttachRot", set_ped_m_fAttachRot); // 1176 уст. m_fAttachRot (CPed)
	lua_register(L, "get_ped_m_nAttachWepAmmo", get_ped_m_nAttachWepAmmo); // 1177 m_nAttachWepAmmo (CPed)
	lua_register(L, "set_ped_m_nAttachWepAmmo", set_ped_m_nAttachWepAmmo); // 1178 уст. m_nAttachWepAmmo (CPed)
	lua_register(L, "get_ped_m_nThreatFlags", get_ped_m_nThreatFlags); // 1179 m_nThreatFlags (CPed)
	lua_register(L, "set_ped_m_nThreatFlags", set_ped_m_nThreatFlags); // 1180 уст. m_nThreatFlags (CPed)
	lua_register(L, "get_ped_m_nThreatCheck", get_ped_m_nThreatCheck); // 1181 m_nThreatCheck (CPed)
	lua_register(L, "set_ped_m_nThreatCheck", set_ped_m_nThreatCheck); // 1182 уст. m_nThreatCheck (CPed)
	lua_register(L, "get_ped_m_nLastThreatCheck", get_ped_m_nLastThreatCheck); // 1183 m_nLastThreatCheck (CPed)
	lua_register(L, "set_ped_m_nLastThreatCheck", set_ped_m_nLastThreatCheck); // 1184 уст. m_nLastThreatCheck (CPed)
	lua_register(L, "get_ped_m_nSayType", get_ped_m_nSayType); // 1185 m_nSayType (CPed)
	lua_register(L, "set_ped_m_nSayType", set_ped_m_nSayType); // 1186 уст. m_nSayType (CPed)
	lua_register(L, "get_ped_m_nSayTimer", get_ped_m_nSayTimer); // 1187 m_nSayTimer (CPed)
	lua_register(L, "set_ped_m_nSayTimer", set_ped_m_nSayTimer); // 1188 уст. m_nSayTimer (CPed)
	lua_register(L, "get_ped_m_nTalkTimerLast", get_ped_m_nTalkTimerLast); // 1189 m_nTalkTimerLast (CPed)
	lua_register(L, "set_ped_m_nTalkTimerLast", set_ped_m_nTalkTimerLast); // 1190 уст. m_nTalkTimerLast (CPed)
	lua_register(L, "get_ped_m_nTalkTimer", get_ped_m_nTalkTimer); // 1191 m_nTalkTimer (CPed)
	lua_register(L, "set_ped_m_nTalkTimer", set_ped_m_nTalkTimer); // 1192 уст. m_nTalkTimer (CPed)
	lua_register(L, "get_ped_m_wTalkTypeLast", get_ped_m_wTalkTypeLast); // 1193 m_wTalkTypeLast (CPed)
	lua_register(L, "set_ped_m_wTalkTypeLast", set_ped_m_wTalkTypeLast); // 1194 уст. m_wTalkTypeLast (CPed)
	lua_register(L, "get_ped_m_wTalkType", get_ped_m_wTalkType); // 1195 m_wTalkType (CPed)
	lua_register(L, "set_ped_m_wTalkType", set_ped_m_wTalkType); // 1196 уст. m_wTalkType (CPed)
	lua_register(L, "get_ped_m_bCanPedTalk", get_ped_m_bCanPedTalk); // 1197 m_bCanPedTalk (CPed)
	lua_register(L, "set_ped_m_bCanPedTalk", set_ped_m_bCanPedTalk); // 1198 уст. m_bCanPedTalk (CPed)
	lua_register(L, "get_ped_m_nPedLastComment", get_ped_m_nPedLastComment); // 1199 m_nPedLastComment (CPed)
	lua_register(L, "set_ped_m_nPedLastComment", set_ped_m_nPedLastComment); // 1200 уст. m_nPedLastComment (CPed)
	lua_register(L, "get_ped_m_vecSeekPosEx", get_ped_m_vecSeekPosEx); // 1201 m_vecSeekPosEx (CPed)
	lua_register(L, "set_ped_m_vecSeekPosEx", set_ped_m_vecSeekPosEx); // 1202 уст. m_vecSeekPosEx (CPed)
	lua_register(L, "get_ped_m_fSeekExAngle", get_ped_m_fSeekExAngle); // 1203 m_fSeekExAngle (CPed)
	lua_register(L, "set_ped_m_fSeekExAngle", set_ped_m_fSeekExAngle); // 1204 уст. m_fSeekExAngle (CPed)
	lua_register(L, "get_obj_m_nObjectType", get_obj_m_nObjectType); // 1205 m_nObjectType (CObject)
	lua_register(L, "set_obj_m_nObjectType", set_obj_m_nObjectType); // 1206 уст. m_nObjectType (CObject)
	lua_register(L, "get_obj_m_nBonusValue", get_obj_m_nBonusValue); // 1207 m_nBonusValue (CObject)
	lua_register(L, "set_obj_m_nBonusValue", set_obj_m_nBonusValue); // 1208 уст. m_nBonusValue (CObject)
	lua_register(L, "get_obj_m_wCostValue", get_obj_m_wCostValue); // 1209 m_wCostValue (CObject)
	lua_register(L, "set_obj_m_wCostValue", set_obj_m_wCostValue); // 1210 уст. m_wCostValue (CObject)
	lua_register(L, "get_obj_m_fDamageMultiplier", get_obj_m_fDamageMultiplier); // 1211 m_fDamageMultiplier (CObject)
	lua_register(L, "set_obj_m_fDamageMultiplier", set_obj_m_fDamageMultiplier); // 1212 уст. m_fDamageMultiplier (CObject)
	lua_register(L, "get_obj_m_bCameraAvoids", get_obj_m_bCameraAvoids); // 1213 m_bCameraAvoids (CObject)
	lua_register(L, "set_obj_m_bCameraAvoids", set_obj_m_bCameraAvoids); // 1214 уст. m_bCameraAvoids (CObject)
	lua_register(L, "get_obj_m_wRefModelId", get_obj_m_wRefModelId); // 1215 m_wRefModelId (CObject)
	lua_register(L, "set_obj_m_wRefModelId", set_obj_m_wRefModelId); // 1216 уст. m_wRefModelId (CObject)
	lua_register(L, "get_car_m_nAudioEntityId", get_car_m_nAudioEntityId); // 1217 m_nAudioEntityId (CPhysical)
	lua_register(L, "set_car_m_nAudioEntityId", set_car_m_nAudioEntityId); // 1218 уст. m_nAudioEntityId (CPhysical)
	lua_register(L, "get_car_m_vecMoveSpeed", get_car_m_vecMoveSpeed); // 1219 m_vecMoveSpeed (CPhysical)
	lua_register(L, "set_car_m_vecMoveSpeed", set_car_m_vecMoveSpeed); // 1220 уст. m_vecMoveSpeed (CPhysical)
	lua_register(L, "get_car_m_vecTurnSpeed", get_car_m_vecTurnSpeed); // 1221 m_vecTurnSpeed (CPhysical)
	lua_register(L, "set_car_m_vecTurnSpeed", set_car_m_vecTurnSpeed); // 1222 уст. m_vecTurnSpeed (CPhysical)
	lua_register(L, "get_car_m_vecFrictionMoveForce", get_car_m_vecFrictionMoveForce); // 1223 m_vecFrictionMoveForce (CPhysical)
	lua_register(L, "set_car_m_vecFrictionMoveForce", set_car_m_vecFrictionMoveForce); // 1224 уст. m_vecFrictionMoveForce (CPhysical)
	lua_register(L, "get_car_m_vecFrictionTurnForce", get_car_m_vecFrictionTurnForce); // 1225 m_vecFrictionTurnForce (CPhysical)
	lua_register(L, "set_car_m_vecFrictionTurnForce", set_car_m_vecFrictionTurnForce); // 1226 уст. m_vecFrictionTurnForce (CPhysical)
	lua_register(L, "get_car_m_vecForce", get_car_m_vecForce); // 1227 m_vecForce (CPhysical)
	lua_register(L, "set_car_m_vecForce", set_car_m_vecForce); // 1228 уст. m_vecForce (CPhysical)
	lua_register(L, "get_car_m_vecTorque", get_car_m_vecTorque); // 1229 m_vecTorque (CPhysical)
	lua_register(L, "set_car_m_vecTorque", set_car_m_vecTorque); // 1230 уст. m_vecTorque (CPhysical)
	lua_register(L, "get_car_m_fMass", get_car_m_fMass); // 1231 m_fMass (CPhysical)
	lua_register(L, "set_car_m_fMass", set_car_m_fMass); // 1232 уст. m_fMass (CPhysical)
	lua_register(L, "get_car_m_fTurnMass", get_car_m_fTurnMass); // 1233 m_fTurnMass (CPhysical)
	lua_register(L, "set_car_m_fTurnMass", set_car_m_fTurnMass); // 1234 уст. m_fTurnMass (CPhysical)
	lua_register(L, "get_car_m_fVelocityFrequency", get_car_m_fVelocityFrequency); // 1235 m_fVelocityFrequency (CPhysical)
	lua_register(L, "set_car_m_fVelocityFrequency", set_car_m_fVelocityFrequency); // 1236 уст. m_fVelocityFrequency (CPhysical)
	lua_register(L, "get_car_m_fAirResistance", get_car_m_fAirResistance); // 1237 m_fAirResistance (CPhysical)
	lua_register(L, "set_car_m_fAirResistance", set_car_m_fAirResistance); // 1238 уст. m_fAirResistance (CPhysical)
	lua_register(L, "get_car_m_fElasticity", get_car_m_fElasticity); // 1239 m_fElasticity (CPhysical)
	lua_register(L, "set_car_m_fElasticity", set_car_m_fElasticity); // 1240 уст. m_fElasticity (CPhysical)
	lua_register(L, "get_car_m_fBuoyancyConstant", get_car_m_fBuoyancyConstant); // 1241 m_fBuoyancyConstant (CPhysical)
	lua_register(L, "set_car_m_fBuoyancyConstant", set_car_m_fBuoyancyConstant); // 1242 уст. m_fBuoyancyConstant (CPhysical)
	lua_register(L, "get_car_m_vecCentreOfMass", get_car_m_vecCentreOfMass); // 1243 m_vecCentreOfMass (CPhysical)
	lua_register(L, "set_car_m_vecCentreOfMass", set_car_m_vecCentreOfMass); // 1244 уст. m_vecCentreOfMass (CPhysical)
	lua_register(L, "get_car_m_nNumCollisionRecords", get_car_m_nNumCollisionRecords); // 1245 m_nNumCollisionRecords (CPhysical)
	lua_register(L, "set_car_m_nNumCollisionRecords", set_car_m_nNumCollisionRecords); // 1246 уст. m_nNumCollisionRecords (CPhysical)
	lua_register(L, "get_car_m_fTotSpeed", get_car_m_fTotSpeed); // 1247 m_fTotSpeed (CPhysical)
	lua_register(L, "set_car_m_fTotSpeed", set_car_m_fTotSpeed); // 1248 уст. m_fTotSpeed (CPhysical)
	lua_register(L, "get_car_m_fCollisionPower", get_car_m_fCollisionPower); // 1249 m_fCollisionPower (CPhysical)
	lua_register(L, "set_car_m_fCollisionPower", set_car_m_fCollisionPower); // 1250 уст. m_fCollisionPower (CPhysical)
	lua_register(L, "get_car_m_vecCollisionPower", get_car_m_vecCollisionPower); // 1251 m_vecCollisionPower (CPhysical)
	lua_register(L, "set_car_m_vecCollisionPower", set_car_m_vecCollisionPower); // 1252 уст. m_vecCollisionPower (CPhysical)
	lua_register(L, "get_car_m_wComponentCol", get_car_m_wComponentCol); // 1253 m_wComponentCol (CPhysical)
	lua_register(L, "set_car_m_wComponentCol", set_car_m_wComponentCol); // 1254 уст. m_wComponentCol (CPhysical)
	lua_register(L, "get_car_m_nMoveFlags", get_car_m_nMoveFlags); // 1255 m_nMoveFlags (CPhysical)
	lua_register(L, "set_car_m_nMoveFlags", set_car_m_nMoveFlags); // 1256 уст. m_nMoveFlags (CPhysical)
	lua_register(L, "get_car_m_nCollFlags", get_car_m_nCollFlags); // 1257 m_nCollFlags (CPhysical)
	lua_register(L, "set_car_m_nCollFlags", set_car_m_nCollFlags); // 1258 уст. m_nCollFlags (CPhysical)
	lua_register(L, "get_car_m_nLastCollType", get_car_m_nLastCollType); // 1259 m_nLastCollType (CPhysical)
	lua_register(L, "set_car_m_nLastCollType", set_car_m_nLastCollType); // 1260 уст. m_nLastCollType (CPhysical)
	lua_register(L, "get_car_m_nZoneLevel", get_car_m_nZoneLevel); // 1261 m_nZoneLevel (CPhysical)
	lua_register(L, "set_car_m_nZoneLevel", set_car_m_nZoneLevel); // 1262 уст. m_nZoneLevel (CPhysical)
	lua_register(L, "get_car_fNegSpeed", get_car_fNegSpeed); // 1263 fNegSpeed (CAutomobile)
	lua_register(L, "set_car_fNegSpeed", set_car_fNegSpeed); // 1264 уст. fNegSpeed (CAutomobile)
	lua_register(L, "get_car_fAIGripMultiplier", get_car_fAIGripMultiplier); // 1265 fAIGripMultiplier (CAutomobile)
	lua_register(L, "set_car_fAIGripMultiplier", set_car_fAIGripMultiplier); // 1266 уст. fAIGripMultiplier (CAutomobile)
	lua_register(L, "get_car_fSpecialWepRotH", get_car_fSpecialWepRotH); // 1267 fSpecialWepRotH (CAutomobile)
	lua_register(L, "set_car_fSpecialWepRotH", set_car_fSpecialWepRotH); // 1268 уст. fSpecialWepRotH (CAutomobile)
	lua_register(L, "get_car_fSpecialWepRotV", get_car_fSpecialWepRotV); // 1269 fSpecialWepRotV (CAutomobile)
	lua_register(L, "set_car_fSpecialWepRotV", set_car_fSpecialWepRotV); // 1270 уст. fSpecialWepRotV (CAutomobile)
	lua_register(L, "get_car_fSpecialSteering", get_car_fSpecialSteering); // 1271 fSpecialSteering (CAutomobile)
	lua_register(L, "set_car_fSpecialSteering", set_car_fSpecialSteering); // 1272 уст. fSpecialSteering (CAutomobile)
	lua_register(L, "get_car_fSpecialMoveState", get_car_fSpecialMoveState); // 1273 fSpecialMoveState (CAutomobile)
	lua_register(L, "set_car_fSpecialMoveState", set_car_fSpecialMoveState); // 1274 уст. fSpecialMoveState (CAutomobile)
	lua_register(L, "get_car_fSkidMarkDensity", get_car_fSkidMarkDensity); // 1275 fSkidMarkDensity (CAutomobile)
	lua_register(L, "set_car_fSkidMarkDensity", set_car_fSkidMarkDensity); // 1276 уст. fSkidMarkDensity (CAutomobile)
	lua_register(L, "get_car_m_fPropRotation", get_car_m_fPropRotation); // 1277 m_fPropRotation (CBoat)
	lua_register(L, "set_car_m_fPropRotation", set_car_m_fPropRotation); // 1278 уст. m_fPropRotation (CBoat)
	lua_register(L, "get_car_m_fPropSpeed", get_car_m_fPropSpeed); // 1279 m_fPropSpeed (CBoat)
	lua_register(L, "set_car_m_fPropSpeed", set_car_m_fPropSpeed); // 1280 уст. m_fPropSpeed (CBoat)
	lua_register(L, "get_car_m_fForcedZRotation", get_car_m_fForcedZRotation); // 1281 m_fForcedZRotation (CBoat)
	lua_register(L, "set_car_m_fForcedZRotation", set_car_m_fForcedZRotation); // 1282 уст. m_fForcedZRotation (CBoat)
	lua_register(L, "get_car_m_nAttackPlayerTime", get_car_m_nAttackPlayerTime); // 1283 m_nAttackPlayerTime (CBoat)
	lua_register(L, "set_car_m_nAttackPlayerTime", set_car_m_nAttackPlayerTime); // 1284 уст. m_nAttackPlayerTime (CBoat)
	lua_register(L, "get_car_m_fBurningTimer", get_car_m_fBurningTimer); // 1285 m_fBurningTimer (CBoat)
	lua_register(L, "set_car_m_fBurningTimer", set_car_m_fBurningTimer); // 1286 уст. m_fBurningTimer (CBoat)
	lua_register(L, "get_car_m_fBoatGasPedal", get_car_m_fBoatGasPedal); // 1287 m_fBoatGasPedal (CBoat)
	lua_register(L, "set_car_m_fBoatGasPedal", set_car_m_fBoatGasPedal); // 1288 уст. m_fBoatGasPedal (CBoat)
	lua_register(L, "get_car_m_fBoatBrakePedal", get_car_m_fBoatBrakePedal); // 1289 m_fBoatBrakePedal (CBoat)
	lua_register(L, "set_car_m_fBoatBrakePedal", set_car_m_fBoatBrakePedal); // 1290 уст. m_fBoatBrakePedal (CBoat)
	lua_register(L, "get_car_m_fBoatSteeringLeftRight", get_car_m_fBoatSteeringLeftRight); // 1291 m_fBoatSteeringLeftRight (CBoat)
	lua_register(L, "set_car_m_fBoatSteeringLeftRight", set_car_m_fBoatSteeringLeftRight); // 1292 уст. m_fBoatSteeringLeftRight (CBoat)
	lua_register(L, "get_car_m_nPadNumber", get_car_m_nPadNumber); // 1293 m_nPadNumber (CBoat)
	lua_register(L, "set_car_m_nPadNumber", set_car_m_nPadNumber); // 1294 уст. m_nPadNumber (CBoat)
	lua_register(L, "get_car_m_nNumWaterTrailPoints", get_car_m_nNumWaterTrailPoints); // 1295 m_nNumWaterTrailPoints (CBoat)
	lua_register(L, "set_car_m_nNumWaterTrailPoints", set_car_m_nNumWaterTrailPoints); // 1296 уст. m_nNumWaterTrailPoints (CBoat)
	lua_register(L, "get_car_nBikeAnimGroup", get_car_nBikeAnimGroup); // 1297 nBikeAnimGroup (CBike)
	lua_register(L, "set_car_nBikeAnimGroup", set_car_nBikeAnimGroup); // 1298 уст. nBikeAnimGroup (CBike)
	lua_register(L, "get_car_m_nDamageFlags", get_car_m_nDamageFlags); // 1299 m_nDamageFlags (CBike)
	lua_register(L, "set_car_m_nDamageFlags", set_car_m_nDamageFlags); // 1300 уст. m_nDamageFlags (CBike)
	lua_register(L, "get_car_m_fSearchLightX", get_car_m_fSearchLightX); // 1301 m_fSearchLightX (CHeli)
	lua_register(L, "set_car_m_fSearchLightX", set_car_m_fSearchLightX); // 1302 уст. m_fSearchLightX (CHeli)
	lua_register(L, "get_car_m_fSearchLightY", get_car_m_fSearchLightY); // 1303 m_fSearchLightY (CHeli)
	lua_register(L, "set_car_m_fSearchLightY", set_car_m_fSearchLightY); // 1304 уст. m_fSearchLightY (CHeli)
	lua_register(L, "get_car_m_nExplosionTime", get_car_m_nExplosionTime); // 1305 m_nExplosionTime (CHeli)
	lua_register(L, "set_car_m_nExplosionTime", set_car_m_nExplosionTime); // 1306 уст. m_nExplosionTime (CHeli)
	lua_register(L, "get_car_m_fRotationX", get_car_m_fRotationX); // 1307 m_fRotationX (CHeli)
	lua_register(L, "set_car_m_fRotationX", set_car_m_fRotationX); // 1308 уст. m_fRotationX (CHeli)
	lua_register(L, "get_car_m_fHeliForcedZ", get_car_m_fHeliForcedZ); // 1309 m_fHeliForcedZ (CHeli)
	lua_register(L, "set_car_m_fHeliForcedZ", set_car_m_fHeliForcedZ); // 1310 уст. m_fHeliForcedZ (CHeli)
	lua_register(L, "get_car_m_fSearchLightIntensity", get_car_m_fSearchLightIntensity); // 1311 m_fSearchLightIntensity (CHeli)
	lua_register(L, "set_car_m_fSearchLightIntensity", set_car_m_fSearchLightIntensity); // 1312 уст. m_fSearchLightIntensity (CHeli)
	lua_register(L, "get_car_m_nDamagePoints", get_car_m_nDamagePoints); // 1313 m_nDamagePoints (CHeli)
	lua_register(L, "set_car_m_nDamagePoints", set_car_m_nDamagePoints); // 1314 уст. m_nDamagePoints (CHeli)
	lua_register(L, "get_car_m_fRotorAngle", get_car_m_fRotorAngle); // 1315 m_fRotorAngle (CHeli)
	lua_register(L, "set_car_m_fRotorAngle", set_car_m_fRotorAngle); // 1316 уст. m_fRotorAngle (CHeli)
	lua_register(L, "get_wanted_m_nLastTimeWantedDecreased", get_wanted_m_nLastTimeWantedDecreased); // 1317 последнее снижение розыска
	lua_register(L, "get_wanted_m_nLastTimeWantedLevelChanged", get_wanted_m_nLastTimeWantedLevelChanged); // 1318 последняя смена уровня розыска
	lua_register(L, "get_wanted_m_dwTimeOfParole", get_wanted_m_dwTimeOfParole); // 1319 таймер условного освобождения
	lua_register(L, "set_wanted_m_dwTimeOfParole", set_wanted_m_dwTimeOfParole); // 1320 уст. таймер условного освобождения
	lua_register(L, "get_wanted_m_fMultiplier", get_wanted_m_fMultiplier); // 1321 множитель розыска
	lua_register(L, "set_wanted_m_fMultiplier", set_wanted_m_fMultiplier); // 1322 уст. множитель розыска
	lua_register(L, "get_wanted_m_nCopsInPursuit", get_wanted_m_nCopsInPursuit); // 1323 копов в погоне
	lua_register(L, "get_wanted_m_nMaxCopsInPursuit", get_wanted_m_nMaxCopsInPursuit); // 1324 макс. копов в погоне
	lua_register(L, "set_wanted_m_nMaxCopsInPursuit", set_wanted_m_nMaxCopsInPursuit); // 1325 уст. макс. копов в погоне
	lua_register(L, "get_wanted_m_nMaxCopCarsInPursuit", get_wanted_m_nMaxCopCarsInPursuit); // 1326 макс. машин копов в погоне
	lua_register(L, "set_wanted_m_nMaxCopCarsInPursuit", set_wanted_m_nMaxCopCarsInPursuit); // 1327 уст. макс. машин копов в погоне
	lua_register(L, "get_wanted_m_nCopsBeatingSuspect", get_wanted_m_nCopsBeatingSuspect); // 1328 копов избивают подозреваемого
	lua_register(L, "choose_gang_ped_model", choose_gang_ped_model); // 1329 выбрать модель педа банды
	lua_register(L, "set_gang_attack_player_with_cops", set_gang_attack_player_with_cops); // 1330 банда атакует игрока с копами
	lua_register(L, "init_gangs", init_gangs); // 1331 инициализировать банды
	lua_register(L, "register_motion_streak", register_motion_streak); // 1332 размытие-полоса (id, левая/правая точки, rgb)
	lua_register(L, "init_trains", init_trains); // 1333 инициализировать поезда
	lua_register(L, "update_trains", update_trains); // 1334 обновить поезда
	lua_register(L, "shutdown_trains", shutdown_trains); // 1335 завершить поезда
	lua_register(L, "ped_in_targetting", ped_in_targetting); // 1336 пед на прицеле.
	lua_register(L, "Ped_in_targetting", Ped_in_targetting); // 1337 макрос пед на прицеле.
	lua_register(L, "Get_model_and_type_ped", Get_model_and_type_ped); // 1338 макрос получить модель и тип педа.
	lua_register(L, "get_model_ped", get_model_ped); // 1339 получить модель педа.
	lua_register(L, "get_type_ped", get_type_ped); // 1340 получить тип педа.
	lua_register(L, "ped_clear_investigate_event", ped_clear_investigate_event); // 1341 пед уходит, опустить педа.
	lua_register(L, "ped_clear_objective", ped_clear_objective); // 1342 снять задачи с педа.
	lua_register(L, "setbomb_in_car", setbomb_in_car); // 1343 установить бомбу в авто.
	lua_register(L, "getbomb_in_car", getbomb_in_car); // 1344 получить тип бомбы в авто.
	lua_register(L, "activetbomb", activetbomb); // 1345 активировать бомбу в авто.
	lua_register(L, "is_ped_in_this_car", is_ped_in_this_car); // 1346 игрок в этом авто?
	lua_register(L, "ishorncar", ishorncar); // 1347 проверить гудит ли авто.
	lua_register(L, "getcarfirstcolor", getcarfirstcolor); // 1348 получить первый цвет авто.
	lua_register(L, "getcarseconscolor", getcarseconscolor); // 1349 получить второй цвет авто.
	lua_register(L, "ped_char_spotted_player", ped_char_spotted_player); // 1350 если пед увидел игрока.
	lua_register(L, "set_timer_bomb_car", set_timer_bomb_car); // 1351 установить таймер на взрыв бомбы в авто.
	lua_register(L, "is_ped_objective", is_ped_objective); // 1352 если пед выполнил задачу.
	lua_register(L, "set_time_scale", set_time_scale); // 1353 установить ход времени в игре.
	lua_register(L, "is_ped_in_air", is_ped_in_air); // 1354 если пед в воздухе.
	lua_register(L, "kill_peds_in_car", kill_peds_in_car); // 1355 убить педов в авто.
	lua_register(L, "getstamina_ped", getstamina_ped); // 1356 получить выносливость педа.
	lua_register(L, "setstamina_ped", setstamina_ped); // 1357 установить выносливость педа.
	lua_register(L, "get_radio_car", get_radio_car); // 1358 получить текущую радиостанцию авто.
	lua_register(L, "is_ped_passenger_car", is_ped_passenger_car); // 1359 пед пассажир авто.
	lua_register(L, "is_ped_driver_car", is_ped_driver_car); // 1360 пед пассажир авто.
	lua_register(L, "ped_in_turret_on_car", ped_in_turret_on_car); // 1361 переместить педа в турель авто.
	lua_register(L, "ped_from_turret_on_car", ped_from_turret_on_car); // 1362 убрать педа из турели авто.
	lua_register(L, "is_button_pressed", is_button_pressed); // 1363 если спец клавиша нажата.
	lua_register(L, "getmoney_ped", getmoney_ped); // 1364 получить кол-во деньги педа.
	lua_register(L, "setmoney_ped", setmoney_ped); // 1365 установить кол-во деньги педа.
	lua_register(L, "addmoney_ped", addmoney_ped); // 1366 добавить кол-во деньги педа.
	lua_register(L, "getgravity", getgravity); // 1367 получить значения гравитации.
	lua_register(L, "setgravity", setgravity); // 1368 установить значения гравитации.
	lua_register(L, "gravity_default", gravity_default); // 1369 вернуть значения по умолчанию гравитации.
	lua_register(L, "create_shots_on_ped", create_shots_on_ped); // 1370 создать выстрелы.
	lua_register(L, "get_wheel_status", get_wheel_status); // 1371 получить состояния шины авто.
	lua_register(L, "create_shots_on_car", create_shots_on_car); // 1372 создать выстрелы.
	lua_register(L, "ped_save_pos_attack", ped_save_pos_attack); // 1373 пед сохраняет ли свою позицию при атаке.
	lua_register(L, "ped_rebuff", ped_rebuff); // 1374 пед атакует любого, кто нападает на него.
	lua_register(L, "ped_search_threat", ped_search_threat); // 1375 пед ищет угрозу.
	lua_register(L, "ped_clean_threat", ped_clean_threat); // 1376 очистить угрозы для педа.
	lua_register(L, "save_car_color", save_car_color); // 1377 авто сохраняет цвет при перекраске.
	lua_register(L, "is_car_passenger_free", is_car_passenger_free); // 1378 Проверяет, свободно ли место пассажира в авто.
	lua_register(L, "set_ped_bleeding", set_ped_bleeding); // 1379 вкл\выкл крованные у педа.
	lua_register(L, "all_exit_from_car", all_exit_from_car); // 1380 все педы выходят из авто.
	lua_register(L, "is_ped_duck", is_ped_duck); // 1381 Проверяет, пед присел.
	lua_register(L, "set_car_tank", set_car_tank); // 1382 установить способность танка детонировать авто при контакте.
	lua_register(L, "set_display_radar", set_display_radar); // 1383 показать скрывать радар.
	lua_register(L, "set_ped_in_car", set_ped_in_car); // 1384 установить педа в авто как водителя или пассажира.
	lua_register(L, "set_ped_traffic", set_ped_traffic); // 1385 установить плотноcть трафика педов.
	lua_register(L, "heli_to_fly", heli_to_fly); // 1386 вертолет летит на координаты.
	lua_register(L, "set_ped_immunities", set_ped_immunities); // 1387 уст иммунитеты педу.
	lua_register(L, "set_car_immunities", set_car_immunities); // 1388 уст иммунитеты авто.
	lua_register(L, "setcolourmarker", setcolourmarker); // 1389 установить цвет маркера.
	lua_register(L, "set_camera_on_car", set_camera_on_car); // 1390 уст камеру на авто.
	lua_register(L, "ped_run_to_point", ped_run_to_point); // 1391 пед бежит к точке пешком.
	lua_register(L, "select_interiour", select_interiour); // 1392 уcтановить интерьер.
	lua_register(L, "set_ped_stats_to", set_ped_stats_to); // 1393 уст поведения педа.
	lua_register(L, "ped_maintain_position_when_attacked", ped_maintain_position_when_attacked); // 1394 пед удерживает позицию при атаке.
	lua_register(L, "set_timer_ped_attack", set_timer_ped_attack); // 1395 установить таймер атаки педа.
	lua_register(L, "set_cops_ignored", set_cops_ignored); // 1396 установить игнор копов.
 	lua_register(L, "set_camera_near_clip", set_camera_near_clip); // 1397 установить обрезку камеры.
	lua_register(L, "setpedcrouch", setpedcrouch); // 1398 пед сел.
	lua_register(L, "is_ped_incar", is_ped_incar); // 1399 пед в авто или нет?
	lua_register(L, "delete_entity", delete_entity); // 1400 удалить сущность сразу.
	lua_register(L, "clean_leader", clean_leader); // 1401 перестать следовать за лидером.
	lua_register(L, "show_save_menu", show_save_menu); // 1402 показать меню сохранения.
	lua_register(L, "is_ped_shooting", is_ped_shooting); // 1403 пед стреляет?.
	lua_register(L, "set_ped_visible", set_ped_visible); // 1404 уст невидимость педу.
	lua_register(L, "set_player_mod", set_player_mod); // 1405 уст настроения игрока.
	lua_register(L, "setped_damaged_player", setped_damaged_player); // 1406 педу может нанести ущерб только игрок.
	lua_register(L, "set_ped_reaction_threat", set_ped_reaction_threat); // 1407 уст реакцию педа на ответную угрозу.
	lua_register(L, "obj_target", obj_target); // 1408 в объект можно целиться.
	lua_register(L, "clean_ped_wait", clean_ped_wait); // 1409 пед больше не ждет.
	lua_register(L, "set_ped_accuracy", set_ped_accuracy); // 1410 уст меткость педу.
	lua_register(L, "textbox", textbox); // 1411 вывести сообщение в textbox.
	lua_register(L, "remove_textbox", remove_textbox); // 1412 удалить сообщение на textbox.
	lua_register(L, "get_controller", get_controller); // 1413 получить контролер от игрока.
	lua_register(L, "create_pickup_clothes", create_pickup_clothes); // 1414 создать пикап одежды.
	lua_register(L, "set_riot_intensity", set_riot_intensity); // 1415 уст уровень шума беспорядков.
	lua_register(L, "set_ped_walk_style", set_ped_walk_style); // 1416 установить стиль ходьбы педа.
	lua_register(L, "check_car_resray", check_car_resray); // 1417 проверить авто игрока было перекрашена в гараже.
	lua_register(L, "set_car_range", set_car_range); // 1418 установить множитель диапазона на угрозу для автомобиля.
	lua_register(L, "set_ped_range", set_ped_range); // 1419 установить множитель диапазона на угрозу для педа.
	lua_register(L, "getcarangle", getcarangle); // 1420 получить угол авто.
	lua_register(L, "create_newthread", create_newthread); // 1421 создания нового потока.
	lua_register(L, "cleanarea", cleanarea); // 1422 очистить арену.
	lua_register(L, "set_brakes_car", set_brakes_car); // 1423 уст тормоза авто игрока.
	lua_register(L, "setmarker_brightness", setmarker_brightness); // 1424 уст яркость маркера.
	lua_register(L, "Createobj", Createobj); // 1425 макрос создать объект.
	lua_register(L, "setpednode_seek", setpednode_seek); // 1426 пед игнорирорует пути педов.
	lua_register(L, "ispedscreen", ispedscreen); // 1427 пед виден.
	lua_register(L, "iscarscreen", iscarscreen); // 1428 авто видно.
	lua_register(L, "isobjscreen", isobjscreen); // 1429 объект виден.
	lua_register(L, "ispeddead", ispeddead); // 1430 пед мертв.
	lua_register(L, "iscardead", iscardead); // 1431 авто уничтожено.
	lua_register(L, "ped_follow_ped", ped_follow_ped); // 1432 пед следует за педом.
	lua_register(L, "set_cars_damaged", set_cars_damaged); // 1433 Все авто повреждены.
	lua_register(L, "set_ped_targetted", set_ped_targetted); // 1434 запрет целиться в педа.
	lua_register(L, "set_ped_friend", set_ped_friend); // 1435 уст дружественное отношения педа.
	lua_register(L, "set_ped_running", set_ped_running); // 1436 пед может бежать.
	lua_register(L, "set_ped_damaged_gang", set_ped_damaged_gang); // 1437 уст педа уязвимым для членов банды.
	lua_register(L, "is_ped_damaged_weapon", is_ped_damaged_weapon); // 1438 пед получает от определенного вида оружие.
	lua_register(L, "is_car_damaged_weapon", is_car_damaged_weapon); // 1439 авто получает от определенного вида оружие.
	lua_register(L, "isped_in_air", isped_in_air); // 1440 пед в воздухе.
	lua_register(L, "set_threat_for_ped_type", set_threat_for_ped_type); // 1441 уст враждебность типа педа к другим типам педа.
	lua_register(L, "clean_threat_for_ped_type", clean_threat_for_ped_type); // 1442 Убрать враждебность типа педа к другим типам педа.
	lua_register(L, "create_phone", create_phone); // 1443 создать телефон на координатах.
	lua_register(L, "on_phone", on_phone); // 1444 вкл телефон.
	lua_register(L, "off_phone", off_phone); // 1445 выкл телефон.
	lua_register(L, "read_memory", read_memory); // 1446 читать адрес памяти.
	lua_register(L, "write_memory", write_memory); // 1447 записать адрес памяти.
	lua_register(L, "is_sphere_onscreen", is_sphere_onscreen); // 1448 видна точка на радаре.
	lua_register(L, "set_ped_can_shot_in_car", set_ped_can_shot_in_car); // 1449 водитель авто может получить пули.
	lua_register(L, "iscarfire", iscarfire); // 1450 авто горит?
	lua_register(L, "is_car_damage_by_car", is_car_damage_by_car); // 1451 Если авто получило урон от другого авто.
	lua_register(L, "show_text_gtx_table", show_text_gtx_table); // 1452 вывести текст из таблицы.
	lua_register(L, "ped_road_off", ped_road_off); // 1453 выкл путь педов.
	lua_register(L, "ped_road_on", ped_road_on); // 1454 вкл путь педов.
	lua_register(L, "car_road_off", car_road_off); // 1455 выкл путь авто.
	lua_register(L, "car_road_on", car_road_on); // 1456 вкл путь авто.
	lua_register(L, "getcardamage", getcardamage); // 1457 получить кол-во урона авто.
	lua_register(L, "randomfindobj", randomfindobj); // 1458 найти случайный объект в радиусе.
	lua_register(L, "findped_in_cord", findped_in_cord); // 1459 найти педа в радиусе с координатами.
	lua_register(L, "findcar_in_cord", findcar_in_cord); // 1460 найти авто в радиусе с координатами.
	lua_register(L, "findobj_in_cord", findobj_in_cord); // 1461 найти объект в радиусе с координатами.
	lua_register(L, "findped_nearest_in_cord", findped_nearest_in_cord); // 1462 найти ближайшего педа в радиусе с координатами.
	lua_register(L, "findcar_nearest_in_cord", findcar_nearest_in_cord); // 1463 найти ближайшее авто в радиусе с координатами.
	lua_register(L, "findobj_nearest_in_cord", findobj_nearest_in_cord); // 1464 найти ближайший объект в радиусе с координатами.
	lua_register(L, "getmodelindex", getmodelindex); // 1465 получить id модели.
	lua_register(L, "check_ped_in_cord", check_ped_in_cord); // 1466 есть ли педа в радиусе с координатами.
	lua_register(L, "get_ped_in_cord", get_ped_in_cord); // 1467 получить педа в радиусе с координатами.
	lua_register(L, "check_car_in_cord", check_car_in_cord); // 1468 есть ли авто в радиусе с координатами.
	lua_register(L, "get_car_in_cord", get_car_in_cord); // 1469 найти авто в радиусе с координатами.
	lua_register(L, "check_obj_in_cord", check_obj_in_cord); // 1470 есть ли объект в радиусе с координатами.
	lua_register(L, "get_obj_in_cord", get_obj_in_cord); // 1471 получить объект в радиусе с координатам.
	lua_register(L, "setobjangle", setobjangle); // 1472 уст угол объекта.
	lua_register(L, "getcardimension", getcardimension); // 1473 получить размер авто.
	lua_register(L, "carmoveforce", carmoveforce); // 1474 двигать авто с силой.
	lua_register(L, "carturnforce", carturnforce); // 1475 повернуть авто с силой.
	lua_register(L, "car_turn_speed", car_turn_speed); // 1476 повернуть авто с применением скорости.
	lua_register(L, "carturnfrictionforce", carturnfrictionforce); // 1477 повернуть авто с силой трения.
	lua_register(L, "car_move_speed", car_move_speed); // 1478 двигать авто с применением скорости.

	lua_register(L, "car_turnforce_on_x", car_turnforce_on_x); // 1479 Повернуть авто по оси x (вверх).
	lua_register(L, "car_turnforce_on_y", car_turnforce_on_y); // 1480 Повернуть авто по оси y (вбок).
	lua_register(L, "car_turnforce_on_z", car_turnforce_on_z); // 1481 Повернуть авто по оси z (вокруг оси).

	lua_register(L, "car_moveforce_on_x", car_moveforce_on_x); // 1482 Двигать авто по оси x.
	lua_register(L, "car_moveforce_on_y", car_moveforce_on_y); // 1483 Двигать авто по оси y.
	lua_register(L, "car_moveforce_on_z", car_moveforce_on_z); // 1484 Двигать авто по оси z.

	lua_register(L, "setcarrotate", setcarrotate); // 1485 уст вращения авто.

	lua_register(L, "getcar_turn_on_x", getcar_turn_on_x); // 1486 Получить угол поворота авто по оси x.
	lua_register(L, "getcar_turn_on_y", getcar_turn_on_y); // 1487 Получить угол поворота авто по оси y.
	lua_register(L, "getcar_turn_on_z", getcar_turn_on_z); // 1488 Получить угол поворота авто по оси z.

	lua_register(L, "carmovespeed_on_x", carmovespeed_on_x); // 1489 двигать авто с скоростью по x.
	lua_register(L, "carmovespeed_on_y", carmovespeed_on_y); // 1490 двигать авто с скоростью по y.
	lua_register(L, "carmovespeed_on_z", carmovespeed_on_z); // 1491 двигать авто с скоростью по z.

	lua_register(L, "car_rotate_on_x", car_rotate_on_x); // 1492 двигать авто по оси x.
	lua_register(L, "car_rotate_on_y", car_rotate_on_y); // 1493 двигать авто по оси y.
	lua_register(L, "car_rotate_on_z", car_rotate_on_z); // 1494 двигать угла авто по оси z.
	lua_register(L, "car_setrotate_on_x", car_setrotate_on_x); // 1495 уст авто по оси x.

	lua_register(L, "car_setrotate_on_y", car_setrotate_on_y); // 1496 уст авто по оси y.
	lua_register(L, "car_setrotate_on_z", car_setrotate_on_z); // 1497 уст авто по оси z.
	lua_register(L, "getcar_resistance", getcar_resistance); // 1498 Получить сопротивления авто трению.
	lua_register(L, "car_rotate_turn_on_x", car_rotate_turn_on_x); // 1499 уст угол поворота авто по оси x.

	lua_register(L, "car_rotate_turn_on_y", car_rotate_turn_on_y); // 1500 уст угол поворота авто по оси y.
	lua_register(L, "car_rotate_turn_on_z", car_rotate_turn_on_z); // 1501 уст угол поворота авто по оси z.

	lua_register(L, "setcar_resistance", setcar_resistance); // 1502 Уст силу сопротивления авто трению.
	lua_register(L, "getcar_turns_on_axes", getcar_turns_on_axes); // 1503 получить углы авто поворота по осям.
	lua_register(L, "setcar_at_on_x", setcar_on_x); // 1504 уст авто по оси x.
	lua_register(L, "setcar_at_on_y", setcar_on_y); // 1505 уст авто по оси y.

	lua_register(L, "setcar_at_on_z", setcar_on_z); // 1506 уст авто по оси z.
	lua_register(L, "car_turn_on_x_with_speed", car_turn_on_x_with_speed); // 1507 повeрнуть авто по оси x на угол со скорость.
	lua_register(L, "car_turn_on_y_with_speed", car_turn_on_y_with_speed); // 1508 повeрнуть авто по оси y на угол со скорость.
	lua_register(L, "car_turn_on_z_with_speed", car_turn_on_z_with_speed); // 1509 повeрнуть авто по оси z на угол со скорость.

	lua_register(L, "car_turn_on_x_with_delay", car_turn_on_x_with_delay); // 1510 повeрнуть авто по оси x на угол со задержкой.
	lua_register(L, "car_turn_on_y_with_delay", car_turn_on_y_with_delay); // 1511 повeрнуть авто по оси y на угол со задержкой.
	lua_register(L, "car_turn_on_z_with_delay", car_turn_on_z_with_delay); // 1512 повeрнуть авто по оси z на угол со задержкой.
	lua_register(L, "set_car_mass", set_car_mass); // 1513 установить массу авто на значение.
	lua_register(L, "set_car_acceleration", set_car_acceleration); // 1514 установить ускорение авто на значение.
	lua_register(L, "set_car_max_speed", set_car_max_speed); // 1515 установить максимальную скорость авто на значение.
	lua_register(L, "set_car_traction", set_car_traction); // 1516 установить сцепление авто на значение.

	lua_register(L, "create_rope_on_cords_with_swat", create_rope_on_cords_with_swat); // 1517 создать веревку на координатах с бойцом спецназа.
	lua_register(L, "create_rope_on_cords", create_rope_on_cords); // 1518 создать веревку на координатах.
	lua_register(L, "heli_change_height", heli_change_height); // 1519 изменить высоту вертолета.

	lua_register(L, "exitcar", exitcar); // 1520 выйти из авто.

	return 0;
};
