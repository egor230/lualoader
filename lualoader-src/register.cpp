#include "funcs.h"

int funs(lua_State* L) {// список функций.

	//set_path_to_module(L); // уст путь к модулю.
	lua_register(L, "findplayer", findplayer); // 1 найти томми.
	lua_register(L, "setpedhealth", setpedhealth); // 2 установить здоровье педу.
	lua_register(L, "setarmour", setarmour); // 3 установить броню педу.
	lua_register(L, "wait", wait); // 4 задержка.
	lua_register(L, "wait_key", wait_key); // 1587 (26.10) обработчик нажатия клавиш как гражданин планировщика: wait_key("VK_G", 5000) — ждёт нажатие и отпускание, паркуя корутину (остальные скрипты переключаются, кадр не блокируется).
	lua_register(L, "getpedhealth", getpedhealth); // 5 получить здоровье педа.
	lua_register(L, "getpedhealthf", getpedhealthf); // получить здоровье педа (float).
	lua_register(L, "getpedangle", getpedangle); // 6 получить угол педа.
	lua_register(L, "worldcoord", worldcoord); // 7 Перевод в мировые координаты.
	lua_register(L, "getpedcoordinates_on_x", getpedcoordinates_on_x); // 8 Получить мировую координату по x для педа.
	lua_register(L, "getpedcoordinates_on_y", getpedcoordinates_on_y); // 9 Получить мировую координату по y для педа.
	lua_register(L, "setarmour", setarmour); // 10 получить броню.
	lua_register(L, "stop_car_radio", stop_car_radio);   // выключить играющее радио
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
	lua_register(L, "iscar_in_water", iscar_in_water); // 45 проверка авто в воде.
	lua_register(L, "car_in_water", iscar_in_water); // алиас для совместимости со старыми скриптами.
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
	lua_register(L, "setobjоcoordes", setobjocoordes); // 108 установить координаты для объект.
	lua_register(L, "setobjocoordes", setobjocoordes);
	lua_register(L, "setobjcoordes", setobjocoordes);
	lua_register(L, "getobjcoordes", getobjcoordes); // 109 получить координаты объекта.
	lua_register(L, "create_marker_obj", create_marker_obj); // 110 создать маркер над объектом.
	lua_register(L, "isobject", isobject); // 111 проверка это объект?.
	lua_register(L, "setpedangle", setpedangle); // 112 установить угол педа.
	lua_register(L, "setcaraction", setcaraction); // 113 установить поведение авто.
	lua_register(L, "move_obj", move_obj); // 114 двигать объект.
	lua_register(L, "move_obj_to", move_obj_to); // двигать объект в заданные координаты с заданной скоростью ед/сек (свой поток, скрипт не ждёт).
	lua_register(L, "move_rotate", move_rotate); // 115 вращать объект.
	lua_register(L, "getobjangle", getobjangle); // 116 получить угол объекта.
	lua_register(L, "findcar", findcar); // 117 Найти авто.
	lua_register(L, "setcartask", setcartask); // 118 установить задачу авто.
	lua_register(L, "setcarcoordes", setcarcoordes); // 119 установить координаты авто.
	lua_register(L, "is_car_stuck", is_car_stuck); // 120 03CE: car 12@ stuck если машина застряла.
	lua_register(L, "is_car_upsidedown", is_car_upsidedown); // 121 01F4: car 12@ flipped если машина перевернута.
	lua_register(L, "is_car_upright", is_car_upright); // 122 020D: car 12@ flipped если указанный автомобиль стоит на колесах.
	lua_register(L, "find_road_for_car", find_road_for_car); // 123 найти дорогу.
	lua_register(L, "setcarstrong", setcarstrong);
	lua_register(L, "set_car_only_damaged_by_player", set_car_only_damaged_by_player);
	lua_register(L, "open_diaz_gates", open_diaz_gates);
	lua_register(L, "open_west_diaz_gates", open_west_diaz_gates);
	lua_register(L, "open_east_diaz_gates", open_east_diaz_gates); // открыть ворота острова Диаса // 02AA // 124 сделать авто устойчивым.
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
	lua_register(L, "Createped", Createped);
	lua_register(L, "CreateSpecialActor", CreateSpecialActor); // 23.11: спец-актёр: сам грузит/создаёт/выгружает
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
	lua_register(L, "radar_add_sprite_blip_for_coord", radar_add_sprite_blip_for_coord); // 462а спрайт-блайп на карте (02A8).
	lua_register(L, "radar_add_blip_for_coord", radar_add_blip_for_coord); // 462б координатный блайп на карте (018A).
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
	lua_register(L, "angle_entity_to_entity", angle_entity_to_entity); // 23.11: угол 2-й сущности к 1-й
	lua_register(L, "angle_to_entity", angle_to_entity);
	lua_register(L, "get_angle_to_target", get_angle_to_target);
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
	lua_register(L, "show_save_menu", show_save_menu); // 1402 меню сохранения (жив, не в миссии, не в машине).
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
	lua_register(L, "show_text_gtx_table", show_text_gtx_table);
	lua_register(L, "show_text_key", show_text_key); // 23.11: авто-таблица по GXT-ключу // 1452 вывести текст из таблицы.
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

	lua_register(L, "get_ped_model_index", get_ped_model_index); // 1521 получить ID базовой модели педа.
	lua_register(L, "get_current_skin_name", get_current_skin_name); // 1522 получить имя последнего скина игрока.
	lua_register(L, "get_player_skin", get_player_skin); // 1523 получить НАСТОЯЩЕЕ имя текущей модели игрока (CModelInfo).

	// ---- НОВЫЕ ФУНКЦИИ (добавлены 2026-09-21 из gta vc база / reVC / IDA-отчёта) ----
	lua_register(L, "ped_set_look", ped_set_look); // 1524 пед смотрит на сущность.
	lua_register(L, "ped_set_look_flag", ped_set_look_flag); // 1525 флаг взгляда педа.
	lua_register(L, "ped_can_see_entity", ped_can_see_entity); // 1526 видит ли пед сущность.
	lua_register(L, "ped_set_move_state", ped_set_move_state); // 1527 скорость передвижения педа.
	lua_register(L, "ped_set_wander_path", ped_set_wander_path); // 1528 блуждание педа по пути.
	lua_register(L, "ped_set_follow_path", ped_set_follow_path); // 1529 пед следует по пути к точке.
	lua_register(L, "ped_attach_to_entity", ped_attach_to_entity); // 1530 прикрепить педа к сущности.
	lua_register(L, "ped_point_gun_at", ped_point_gun_at); // 1531 пед целится оружием в сущность.
	lua_register(L, "car_join_road_system", car_join_road_system); // 1532 встроить авто в систему дорог.
	lua_register(L, "car_join_road_goto", car_join_road_goto); // 1533 авто в дороги с целью ехать к точке.
	lua_register(L, "car_switch_real_physics", car_switch_real_physics); // 1534 вернуть авто настоящую физику.
	lua_register(L, "car_update_on_rails", car_update_on_rails); // 1535 движение авто по рельсам.
	lua_register(L, "car_set_component_rotation", car_set_component_rotation); // 1536 повернуть компонент авто.
	lua_register(L, "car_open_door", car_open_door); // 1537 открыть/закрыть дверь авто.
	lua_register(L, "car_goto_coors", car_goto_coors); // 1538 авто едет к точке (ИИ).
	lua_register(L, "car_park_at_coors", car_park_at_coors); // 1539 авто паркуется у точки (ИИ).
	lua_register(L, "player_nearby_peds_interact", player_nearby_peds_interact); // 1540 педы рядом взаимодействуют с игроком.
	lua_register(L, "clear_adrenaline", clear_adrenaline); // 1541 снять адреналин игрока.
	lua_register(L, "annoy_player_ped", annoy_player_ped); // 1542 разозлить педа на игрока.
	lua_register(L, "set_max_wanted_level", set_max_wanted_level); // 1543 задать максимум уровня розыска.
	lua_register(L, "register_crime", register_crime); // 1544 зарегистрировать преступление игрока.
	lua_register(L, "register_crime_immediately", register_crime_immediately); // 1545 мгновенно зарегистрировать преступление.
	lua_register(L, "pad_shake", pad_shake); // 1546 тряска геймпада игрока.
	lua_register(L, "stop_pad_shaking", stop_pad_shaking); // 1547 прекратить тряску геймпадов.
	lua_register(L, "extinguish_point", extinguish_point); // 1548 потушить огонь в точке.
	lua_register(L, "extinguish_point_with_water", extinguish_point_with_water); // 1549 потушить огонь водой.
	lua_register(L, "is_script_fire_extinguished", is_script_fire_extinguished); // 1550 потушен ли скриптовый огонь.
	lua_register(L, "remove_script_fire", remove_script_fire); // 1551 удалить скриптовый огонь по индексу.
	lua_register(L, "remove_all_script_fires", remove_all_script_fires); // 1552 удалить все скриптовые огни.
	lua_register(L, "start_script_fire", start_script_fire); // 1553 запустить скриптовый огонь в точке.
	lua_register(L, "set_help_message", set_help_message); // 1554 показать сообщение помощи.
	lua_register(L, "get_percentage_progress", get_percentage_progress); // 1555 процент прохождения игры.
	lua_register(L, "get_criminal_rating", get_criminal_rating); // 1556 уровень преступника.
	lua_register(L, "point_light_add", point_light_add); // 1557 точечный источник света.
	lua_register(L, "shot_add_tracer", shot_add_tracer); // 1558 трассер пули.
	lua_register(L, "add_jet_explosion", add_jet_explosion); // 1559 взрыв-струя (частица).
	lua_register(L, "add_particle", add_particle); // 1560 добавить частицу.

	lua_register(L, "set_free_resprays", set_free_resprays); // 1561 0335 вкл/выкл бесплатные респреи (провал Swine → респрай → PASS).
	lua_register(L, "has_respray_happened", has_respray_happened); // 1562 0329 был ли респрай в гараже.
	lua_register(L, "set_camera_behind_player", set_camera_behind_player); // 1563 0373 камера за игроком.
	lua_register(L, "get_fading_status", get_fading_status); // 1564 016B идёт ли затемнение/проявление.
	lua_register(L, "get_game_timer", get_game_timer); // 1565 01BD игровое время (мс).
	lua_register(L, "set_chars_chatting", set_chars_chatting); // 1566 03F9 два педа болтают.
	lua_register(L, "create_random_char", create_random_char); // 1567 0376 создать случайного педа.
	lua_register(L, "dont_remove_object", dont_remove_object); // 1568 01C7 не удалять объект движком.
	lua_register(L, "char_follow_path", char_follow_path); // 1569 009E пед идёт по пути к точке.
	lua_register(L, "is_char_wander_path_clear", is_char_wander_path_clear); // 1570 0510 свободен ли путь педа.
	lua_register(L, "turn_player_to_face_char", turn_player_to_face_char); // 1571 0210 игрок лицом к педу.
	lua_register(L, "turn_char_to_face_player", turn_char_to_face_player); // 1572 020F пед лицом к игроку.
	lua_register(L, "turn_char_to_face_coord", turn_char_to_face_coord); // 1573 01BE пед лицом к координате.
	lua_register(L, "set_car_density_multiplier", set_car_density_multiplier); // 1574 01EB плотность трафика.
	lua_register(L, "request_collision", request_collision); // 1575 04E4 загрузить коллизию в точке.
	lua_register(L, "switch_rubbish", switch_rubbish); // 1576 03AD вкл/выкл мусор.
	lua_register(L, "switch_streaming", switch_streaming); // 1577 03AF вкл/выкл стриминг.
	lua_register(L, "mark_char_as_no_longer_needed", mark_char_as_no_longer_needed); // 1578 01C2 пометить педа как ненужного.
	lua_register(L, "set_visibility_of_closest_object_of_type", set_visibility_of_closest_object_of_type); // 1579 0363 видимость ближайшего объекта типа.

	lua_register(L, "get_player_wanted_level", get_player_wanted_level); // 1580 уровень розыска игрока (детектор респрея Swine).


	lua_register(L, "register_mission_given", register_mission_given); // 1588 0317 отметить миссию выданной (CStats::MissionsGiven).
	lua_register(L, "register_mission_passed", register_mission_passed); // 1589 0318 отметить миссию пройденной (CStats::MissionsPassed).
	lua_register(L, "set_police_ignore_player", set_police_ignore_player); // 1590 01F7 копы игнорируют игрока.
	lua_register(L, "set_object_collision", set_object_collision); // 1591 0382 объект сталкивается с миром.
	lua_register(L, "set_object_dynamic", set_object_dynamic); // 1592 0392 объект динамический (падает/двигается).

	// ---- (06.11) Новые функции и вариации (миссия 07 - Treacherous Swine / main.scm) ----
	lua_register(L, "load_mission_audio", load_mission_audio); // 1593 03CF загрузить миссийную аудиофразу (слот, имя).
	lua_register(L, "has_mission_audio_loaded", has_mission_audio_loaded); // 1594 03D0 аудиофраза слота загружена?
	lua_register(L, "play_mission_audio", play_mission_audio); // 1595 03D1 проиграть загруженную фразу слота.
	lua_register(L, "has_mission_audio_finished", has_mission_audio_finished); // 1596 03D2 фраза слота доиграла?
	lua_register(L, "clear_mission_audio", clear_mission_audio); // 1597 040D выгрузить фразу слота.
	lua_register(L, "set_everyone_ignore_player", set_everyone_ignore_player); // 1598 03BF все игнорируют игрока (сцены).
	lua_register(L, "alter_wanted_level", alter_wanted_level); // 1599 010D поставить уровень розыска (с авто-спадом).
	lua_register(L, "alter_wanted_level_no_drop", alter_wanted_level_no_drop); // 1600 010E уровень розыска без авто-спада.
	lua_register(L, "is_wanted_level_greater", is_wanted_level_greater); // 1601 010F розыск выше N?
	lua_register(L, "remove_char_elegantly", remove_char_elegantly); // 1602 034F мягкое удаление педа (MISSION→RANDOM+fade).
	lua_register(L, "locate_char_any_means_2d", locate_char_any_means_2d); // 1603 00EC пед (пешком/в авто) в круге.
	lua_register(L, "locate_char_any_means_3d", locate_char_any_means_3d); // 1604 00FE пед (пешком/в авто) в сфере.
	lua_register(L, "locate_player_any_means_char_2d", locate_player_any_means_char_2d); // 1605 00E9 игрок рядом с педом (2D).
	lua_register(L, "locate_player_any_means_char_3d", locate_player_any_means_char_3d); // 1606 00FB игрок рядом с педом (3D).
	lua_register(L, "is_player_in_area_3d", is_player_in_area_3d); // 1607 0057 игрок в 3D-зоне по двум углам.
	lua_register(L, "is_char_in_area_3d", is_char_in_area_3d); // 1608 0154 пед в 3D-зоне по двум углам.
	lua_register(L, "player_made_progress", player_made_progress); // 1609 030C процент прохождения игры.
	lua_register(L, "save_game_slot", save_game_slot); // 1610 0x61D8B0 сохранить игру в слот 1..8 (игровой поток).
	lua_register(L, "save_to_slot", save_game_slot);   // 1611 псевдоним save_game_slot.
	lua_register(L, "load_game_slot", load_game_slot); // 1612 0x61A6C0 загрузить игру из слота 1..8 (игровой поток).
	lua_register(L, "load_from_slot", load_game_slot); // 1613 псевдоним load_game_slot.


	// (доп.) Обёртки по значениям: стиль вождения, временные действия и миссии авто
	lua_register(L, "set_car_driving_style_stop_for_cars", set_car_driving_style_stop_for_cars); // 1614 стиль вождения авто: stop for cars
	lua_register(L, "set_car_driving_style_slow_down_for_cars", set_car_driving_style_slow_down_for_cars); // 1615 стиль вождения авто: slow down for cars
	lua_register(L, "set_car_driving_style_avoid_cars", set_car_driving_style_avoid_cars); // 1616 стиль вождения авто: avoid cars
	lua_register(L, "set_car_driving_style_plough_through", set_car_driving_style_plough_through); // 1617 стиль вождения авто: plough through
	lua_register(L, "set_car_driving_style_stop_for_cars_ignore_lights", set_car_driving_style_stop_for_cars_ignore_lights); // 1618 стиль вождения авто: stop for cars ignore lights
	lua_register(L, "car_temp_action_none", car_temp_action_none); // 1619 временное действие авто: none
	lua_register(L, "car_temp_action_wait", car_temp_action_wait); // 1620 временное действие авто: wait
	lua_register(L, "car_temp_action_reverse", car_temp_action_reverse); // 1621 временное действие авто: reverse
	lua_register(L, "car_temp_action_handbrake_turn_left", car_temp_action_handbrake_turn_left); // 1622 временное действие авто: handbrake turn left
	lua_register(L, "car_temp_action_handbrake_turn_right", car_temp_action_handbrake_turn_right); // 1623 временное действие авто: handbrake turn right
	lua_register(L, "car_temp_action_handbrake_straight", car_temp_action_handbrake_straight); // 1624 временное действие авто: handbrake straight
	lua_register(L, "car_temp_action_turn_left", car_temp_action_turn_left); // 1625 временное действие авто: turn left
	lua_register(L, "car_temp_action_turn_right", car_temp_action_turn_right); // 1626 временное действие авто: turn right
	lua_register(L, "car_temp_action_go_forward", car_temp_action_go_forward); // 1627 временное действие авто: go forward
	lua_register(L, "car_temp_action_swerve_left", car_temp_action_swerve_left); // 1628 временное действие авто: swerve left
	lua_register(L, "car_temp_action_swerve_right", car_temp_action_swerve_right); // 1629 временное действие авто: swerve right
	lua_register(L, "car_mission_none", car_mission_none); // 1630 поведение авто в миссии: none
	lua_register(L, "car_mission_cruise", car_mission_cruise); // 1631 поведение авто в миссии: cruise
	lua_register(L, "car_mission_ram_player_faraway", car_mission_ram_player_faraway); // 1632 поведение авто в миссии: ram player faraway
	lua_register(L, "car_mission_ram_player_close", car_mission_ram_player_close); // 1633 поведение авто в миссии: ram player close
	lua_register(L, "car_mission_block_player_faraway", car_mission_block_player_faraway); // 1634 поведение авто в миссии: block player faraway
	lua_register(L, "car_mission_block_player_close", car_mission_block_player_close); // 1635 поведение авто в миссии: block player close
	lua_register(L, "car_mission_block_player_handbrake_stop", car_mission_block_player_handbrake_stop); // 1636 поведение авто в миссии: block player handbrake stop
	lua_register(L, "car_mission_wait_for_deletion", car_mission_wait_for_deletion); // 1637 поведение авто в миссии: wait for deletion
	lua_register(L, "car_mission_goto_coords", car_mission_goto_coords); // 1638 поведение авто в миссии: goto coords
	lua_register(L, "car_mission_goto_coords_straight", car_mission_goto_coords_straight); // 1639 поведение авто в миссии: goto coords straight
	lua_register(L, "car_mission_emergency_vehicle_stop", car_mission_emergency_vehicle_stop); // 1640 поведение авто в миссии: emergency vehicle stop
	lua_register(L, "car_mission_stop_forever", car_mission_stop_forever); // 1641 поведение авто в миссии: stop forever
	lua_register(L, "car_mission_goto_coords_accurate", car_mission_goto_coords_accurate); // 1642 поведение авто в миссии: goto coords accurate
	lua_register(L, "car_mission_goto_coords_straight_accurate", car_mission_goto_coords_straight_accurate); // 1643 поведение авто в миссии: goto coords straight accurate
	lua_register(L, "car_mission_goto_coords_as_the_crow_swims", car_mission_goto_coords_as_the_crow_swims); // 1644 поведение авто в миссии: goto coords as the crow swims
	lua_register(L, "car_mission_ram_car_faraway", car_mission_ram_car_faraway); // 1645 поведение авто в миссии: ram car faraway
	lua_register(L, "car_mission_ram_car_close", car_mission_ram_car_close); // 1646 поведение авто в миссии: ram car close
	lua_register(L, "car_mission_block_car_faraway", car_mission_block_car_faraway); // 1647 поведение авто в миссии: block car faraway
	lua_register(L, "car_mission_block_car_close", car_mission_block_car_close); // 1648 поведение авто в миссии: block car close
	lua_register(L, "car_mission_block_car_handbrake_stop", car_mission_block_car_handbrake_stop); // 1649 поведение авто в миссии: block car handbrake stop
	lua_register(L, "car_mission_heli_fly_to_coords", car_mission_heli_fly_to_coords); // 1650 поведение авто в миссии: heli fly to coords
	lua_register(L, "car_mission_attack_player", car_mission_attack_player); // 1651 поведение авто в миссии: attack player
	lua_register(L, "car_mission_plane_fly_to_coords", car_mission_plane_fly_to_coords); // 1652 поведение авто в миссии: plane fly to coords
	lua_register(L, "car_mission_heli_land", car_mission_heli_land); // 1653 поведение авто в миссии: heli land
	lua_register(L, "car_mission_slowly_drive_towards_player_1", car_mission_slowly_drive_towards_player_1); // 1654 поведение авто в миссии: slowly drive towards player 1
	lua_register(L, "car_mission_slowly_drive_towards_player_2", car_mission_slowly_drive_towards_player_2); // 1655 поведение авто в миссии: slowly drive towards player 2
	lua_register(L, "car_mission_block_player_forward_and_back", car_mission_block_player_forward_and_back); // 1656 поведение авто в миссии: block player forward and back


	// (доп.) Обёртки: скорость передвижения педа, pedstat, банды
	lua_register(L, "ped_move_state_none", ped_move_state_none); // 1657 состояние движения педа: none
	lua_register(L, "ped_move_state_still", ped_move_state_still); // 1658 состояние движения педа: still
	lua_register(L, "ped_move_state_walk", ped_move_state_walk); // 1659 состояние движения педа: walk
	lua_register(L, "ped_move_state_run", ped_move_state_run); // 1660 состояние движения педа: run
	lua_register(L, "ped_move_state_sprint", ped_move_state_sprint); // 1661 состояние движения педа: sprint
	lua_register(L, "set_ped_stat_player", set_ped_stat_player); // 1662 назначить pedstat: player
	lua_register(L, "set_ped_stat_cop", set_ped_stat_cop); // 1663 назначить pedstat: cop
	lua_register(L, "set_ped_stat_medic", set_ped_stat_medic); // 1664 назначить pedstat: medic
	lua_register(L, "set_ped_stat_fire", set_ped_stat_fire); // 1665 назначить pedstat: fire
	lua_register(L, "set_ped_stat_gang1", set_ped_stat_gang1); // 1666 назначить pedstat: gang1
	lua_register(L, "set_ped_stat_gang2", set_ped_stat_gang2); // 1667 назначить pedstat: gang2
	lua_register(L, "set_ped_stat_gang3", set_ped_stat_gang3); // 1668 назначить pedstat: gang3
	lua_register(L, "set_ped_stat_gang4", set_ped_stat_gang4); // 1669 назначить pedstat: gang4
	lua_register(L, "set_ped_stat_gang5", set_ped_stat_gang5); // 1670 назначить pedstat: gang5
	lua_register(L, "set_ped_stat_gang6", set_ped_stat_gang6); // 1671 назначить pedstat: gang6
	lua_register(L, "set_ped_stat_gang7", set_ped_stat_gang7); // 1672 назначить pedstat: gang7
	lua_register(L, "set_ped_stat_street_guy", set_ped_stat_street_guy); // 1673 назначить pedstat: street guy
	lua_register(L, "set_ped_stat_suit_guy", set_ped_stat_suit_guy); // 1674 назначить pedstat: suit guy
	lua_register(L, "set_ped_stat_sensible_guy", set_ped_stat_sensible_guy); // 1675 назначить pedstat: sensible guy
	lua_register(L, "set_ped_stat_geek_guy", set_ped_stat_geek_guy); // 1676 назначить pedstat: geek guy
	lua_register(L, "set_ped_stat_old_guy", set_ped_stat_old_guy); // 1677 назначить pedstat: old guy
	lua_register(L, "set_ped_stat_tough_guy", set_ped_stat_tough_guy); // 1678 назначить pedstat: tough guy
	lua_register(L, "set_ped_stat_street_girl", set_ped_stat_street_girl); // 1679 назначить pedstat: street girl
	lua_register(L, "set_ped_stat_suit_girl", set_ped_stat_suit_girl); // 1680 назначить pedstat: suit girl
	lua_register(L, "set_ped_stat_sensible_girl", set_ped_stat_sensible_girl); // 1681 назначить pedstat: sensible girl
	lua_register(L, "set_ped_stat_geek_girl", set_ped_stat_geek_girl); // 1682 назначить pedstat: geek girl
	lua_register(L, "set_ped_stat_old_girl", set_ped_stat_old_girl); // 1683 назначить pedstat: old girl
	lua_register(L, "set_ped_stat_tough_girl", set_ped_stat_tough_girl); // 1684 назначить pedstat: tough girl
	lua_register(L, "set_ped_stat_tramp_male", set_ped_stat_tramp_male); // 1685 назначить pedstat: tramp male
	lua_register(L, "set_ped_stat_tramp_female", set_ped_stat_tramp_female); // 1686 назначить pedstat: tramp female
	lua_register(L, "set_ped_stat_tourist", set_ped_stat_tourist); // 1687 назначить pedstat: tourist
	lua_register(L, "set_ped_stat_prostitute", set_ped_stat_prostitute); // 1688 назначить pedstat: prostitute
	lua_register(L, "set_ped_stat_criminal", set_ped_stat_criminal); // 1689 назначить pedstat: criminal
	lua_register(L, "set_ped_stat_busker", set_ped_stat_busker); // 1690 назначить pedstat: busker
	lua_register(L, "set_ped_stat_taxidriver", set_ped_stat_taxidriver); // 1691 назначить pedstat: taxidriver
	lua_register(L, "set_ped_stat_psycho", set_ped_stat_psycho); // 1692 назначить pedstat: psycho
	lua_register(L, "set_ped_stat_steward", set_ped_stat_steward); // 1693 назначить pedstat: steward
	lua_register(L, "set_ped_stat_sportsfan", set_ped_stat_sportsfan); // 1694 назначить pedstat: sportsfan
	lua_register(L, "set_ped_stat_shopper", set_ped_stat_shopper); // 1695 назначить pedstat: shopper
	lua_register(L, "set_ped_stat_oldshopper", set_ped_stat_oldshopper); // 1696 назначить pedstat: oldshopper
	lua_register(L, "set_ped_stat_beach_guy", set_ped_stat_beach_guy); // 1697 назначить pedstat: beach guy
	lua_register(L, "set_ped_stat_beach_girl", set_ped_stat_beach_girl); // 1698 назначить pedstat: beach girl
	lua_register(L, "set_ped_stat_skater", set_ped_stat_skater); // 1699 назначить pedstat: skater
	lua_register(L, "set_ped_stat_std_mission", set_ped_stat_std_mission); // 1700 назначить pedstat: std mission
	lua_register(L, "set_ped_stat_coward", set_ped_stat_coward); // 1701 назначить pedstat: coward
	lua_register(L, "gang_enable_attack_player_with_cops", gang_enable_attack_player_with_cops); // 1702 поведение банды: enable attack player with cops
	lua_register(L, "gang_disable_attack_player_with_cops", gang_disable_attack_player_with_cops); // 1703 поведение банды: disable attack player with cops
	lua_register(L, "reset_gangs", reset_gangs); // 1704 обёртка: reset gangs

	// (20.11) Реализации, которые существовали в funcs.cpp, но НЕ были зарегистрированы.
	// Параметры сверены с reVC/телом функции; неполные create_rope2..11 НЕ регистрируются.
	lua_register(L, "carmovespeed", carmovespeed); // 1705 авто, vx, vy, vz — прямая запись m_vecMoveSpeed.
	lua_register(L, "get_clip_current_weapon", get_clip_current_weapon); // 1706 пед → патроны в обойме.
	lua_register(L, "getpedarmour", getpedarmour); // 1707 пед → броня.
	lua_register(L, "is_car_model", is_car_model); // 1708 имя модели, авто → bool (0137).
	lua_register(L, "is_ped_sitting_in_car", is_ped_sitting_in_car); // 1709 пед, авто → bool (0448).
	lua_register(L, "load_gtx_table", load_gtx_table); // 1710 имя GXT-таблицы (LOAD_MISSION_TEXT).
	lua_register(L, "set_path_to_module", set_path_to_module); // 1711 выставить package.path (без аргументов).
	lua_register(L, "set_player_control", set_player_control); // 1712 0/1 — управление игроком (01B4).

	
	
	// === AUTO-ADDED OPCODE WRAPPERS (20.11) ===
	lua_register(L, "print_with_number_big", print_with_number_big); // 1713 0x01E3 COMMAND_PRINT_WITH_NUMBER_BIG (reVC Script2.cpp:1389)
	lua_register(L, "add_score", add_score); // 1714 0x0109 COMMAND_ADD_SCORE (reVC Script.cpp:4512)
	lua_register(L, "has_deatharrest_been_executed", has_deatharrest_been_executed); // 1715 0x0112 COMMAND_HAS_DEATHARREST_BEEN_EXECUTED (reVC Script.cpp:4556)
	lua_register(L, "clear_prints", clear_prints); // 1716 0x00BE COMMAND_CLEAR_PRINTS (reVC Script.cpp:4162)
	lua_register(L, "set_fading_colour", set_fading_colour); // 1717 0x0169 COMMAND_SET_FADING_COLOUR (reVC Script2.cpp:384)
	lua_register(L, "load_special_character", load_special_character); // 1718 0x023C COMMAND_LOAD_SPECIAL_CHARACTER (reVC Script3.cpp:662)
	lua_register(L, "has_special_character_loaded", has_special_character_loaded); // 1719 0x023D COMMAND_HAS_SPECIAL_CHARACTER_LOADED (reVC Script3.cpp:673)
	lua_register(L, "set_cutscene_offset", set_cutscene_offset); // 1720 0x0244 COMMAND_SET_CUTSCENE_OFFSET (reVC Script3.cpp:729)
	lua_register(L, "load_cutscene", load_cutscene); // 1721 0x02E4 COMMAND_LOAD_CUTSCENE (reVC Script3.cpp:1950)
	lua_register(L, "create_cutscene_object", create_cutscene_object); // 1722 0x02E5 COMMAND_CREATE_CUTSCENE_OBJECT (reVC Script3.cpp:1958)
	lua_register(L, "set_cutscene_anim", set_cutscene_anim); // 1723 0x02E6 COMMAND_SET_CUTSCENE_ANIM (reVC Script3.cpp:1966)
	lua_register(L, "start_cutscene", start_cutscene); // 1724 0x02E7 COMMAND_START_CUTSCENE (reVC Script3.cpp:1977)
	lua_register(L, "get_cutscene_time", get_cutscene_time); // 1725 0x02E8 COMMAND_GET_CUTSCENE_TIME (reVC Script3.cpp:1980)
	lua_register(L, "clear_cutscene", clear_cutscene); // 1726 0x02EA COMMAND_CLEAR_CUTSCENE (reVC Script3.cpp:1987)
	lua_register(L, "set_shortcut_dropoff_point_for_mission", set_shortcut_dropoff_point_for_mission); // 1727 0x058E COMMAND_SET_SHORTCUT_DROPOFF_POINT_FOR_MISSION (reVC Script8.cpp:234)
	lua_register(L, "clear_this_print", clear_this_print); // 1728 0x03D5 COMMAND_CLEAR_THIS_PRINT (reVC Script4.cpp:2079)
	lua_register(L, "is_player_in_any_car", is_player_in_any_car); // 1729 0x00E0 COMMAND_IS_PLAYER_IN_ANY_CAR (reVC Script.cpp:4400)
	lua_register(L, "is_player_in_car", is_player_in_car); // 1730 0x00DC COMMAND_IS_PLAYER_IN_CAR (reVC Script.cpp:4370)
	lua_register(L, "get_player_coordinates", get_player_coordinates); // 1731 0x0054 COMMAND_GET_PLAYER_COORDINATES (reVC Script.cpp:3183)
	lua_register(L, "load_special_model", load_special_model); // 1732 0x02F3 COMMAND_LOAD_SPECIAL_MODEL (reVC Script3.cpp:2086)
	lua_register(L, "is_char_in_car", is_char_in_car); // 1733 0x00DB COMMAND_IS_CHAR_IN_CAR (reVC Script.cpp:4361)
	lua_register(L, "is_char_in_any_car", is_char_in_any_car); // 1734 0x00DF COMMAND_IS_CHAR_IN_ANY_CAR (reVC Script.cpp:4393)
	lua_register(L, "locate_player_any_means_3d", locate_player_any_means_3d); // 1735 0x00F5 COMMAND_LOCATE_PLAYER_ANY_MEANS_3D (reVC Script.cpp:4448)
	lua_register(L, "get_car_coordinates", get_car_coordinates); // 1736 0x00AA COMMAND_GET_CAR_COORDINATES (reVC Script.cpp:3982)
	lua_register(L, "store_car_player_is_in", store_car_player_is_in); // 1737 0x00DA COMMAND_STORE_CAR_PLAYER_IS_IN (reVC Script.cpp:4310)
	lua_register(L, "clear_onscreen_counter", clear_onscreen_counter); // 1738 0x0151 COMMAND_CLEAR_ONSCREEN_COUNTER (reVC Script2.cpp:194)
	lua_register(L, "clear_extra_colours", clear_extra_colours); // 1739 0x04FA COMMAND_CLEAR_EXTRA_COLOURS (reVC Script7.cpp:567)
	lua_register(L, "register_oddjob_mission_passed", register_oddjob_mission_passed); // 1740 0x0595 COMMAND_REGISTER_ODDJOB_MISSION_PASSED (reVC Script8.cpp:330)
	lua_register(L, "locate_char_on_foot_2d", locate_char_on_foot_2d); // 1741 0x00ED COMMAND_LOCATE_CHAR_ON_FOOT_2D (reVC Script.cpp:4436)
	lua_register(L, "locate_car_2d", locate_car_2d); // 1742 0x01AD COMMAND_LOCATE_CAR_2D (reVC Script2.cpp:881)
	lua_register(L, "display_onscreen_counter_with_string", display_onscreen_counter_with_string); // 1743 0x03C4 COMMAND_DISPLAY_ONSCREEN_COUNTER_WITH_STRING (reVC Script4.cpp:1797)
	lua_register(L, "set_extra_colours", set_extra_colours); // 1744 0x04F9 COMMAND_SET_EXTRA_COLOURS (reVC Script7.cpp:561)
	lua_register(L, "set_char_obj_enter_car_as_driver", set_char_obj_enter_car_as_driver); // 1745 0x01D5 COMMAND_SET_CHAR_OBJ_ENTER_CAR_AS_DRIVER (reVC Script2.cpp:1285)
	lua_register(L, "create_object_no_offset", create_object_no_offset); // 1746 0x029B COMMAND_CREATE_OBJECT_NO_OFFSET (reVC Script3.cpp:1260)
	lua_register(L, "change_blip_display", change_blip_display); // 1747 0x018B COMMAND_CHANGE_BLIP_DISPLAY (reVC Script2.cpp:683)
	lua_register(L, "set_char_health", set_char_health); // 1748 0x0223 COMMAND_SET_CHAR_HEALTH (reVC Script3.cpp:393)
	lua_register(L, "locate_player_on_foot_3d", locate_player_on_foot_3d); // 1749 0x00F6 COMMAND_LOCATE_PLAYER_ON_FOOT_3D (reVC Script.cpp:4449)
	lua_register(L, "get_char_coordinates", get_char_coordinates); // 1750 0x00A0 COMMAND_GET_CHAR_COORDINATES (reVC Script.cpp:3759)
	lua_register(L, "register_fastest_time", register_fastest_time); // 1751 0x042E COMMAND_REGISTER_FASTEST_TIME (reVC Script6.cpp:635)
	lua_register(L, "print", print); // 1752 0x00BB COMMAND_PRINT (reVC Script.cpp:4139)
	lua_register(L, "display_onscreen_timer", display_onscreen_timer); // 1753 0x014E COMMAND_DISPLAY_ONSCREEN_TIMER (reVC Script2.cpp:169)
	lua_register(L, "locate_player_in_car_3d", locate_player_in_car_3d); // 1754 0x00F7 COMMAND_LOCATE_PLAYER_IN_CAR_3D (reVC Script.cpp:4450)
	lua_register(L, "locate_player_any_means_car_2d", locate_player_any_means_car_2d); // 1755 0x01FC COMMAND_LOCATE_PLAYER_ANY_MEANS_CAR_2D (reVC Script3.cpp:108)
	lua_register(L, "print_with_2_numbers_now", print_with_2_numbers_now); // 1756 0x02FD COMMAND_PRINT_WITH_2_NUMBERS_NOW (reVC Script3.cpp:2161)
	lua_register(L, "clear_small_prints", clear_small_prints); // 1757 0x03EB COMMAND_CLEAR_SMALL_PRINTS (reVC Script6.cpp:99)
	lua_register(L, "is_player_in_model", is_player_in_model); // 1758 0x00DE COMMAND_IS_PLAYER_IN_MODEL (reVC Script.cpp:4386)
	lua_register(L, "switch_car_generator", switch_car_generator); // 1759 0x014C COMMAND_SWITCH_CAR_GENERATOR (reVC Script2.cpp:146)
	lua_register(L, "turn_char_to_face_char", turn_char_to_face_char); // 1760 0x020E COMMAND_TURN_CHAR_TO_FACE_CHAR (reVC Script3.cpp:163)
	lua_register(L, "is_player_playing", is_player_playing); // 1761 0x0256 COMMAND_IS_PLAYER_PLAYING (reVC Script3.cpp:854)
	lua_register(L, "is_point_obscured_by_a_mission_entity", is_point_obscured_by_a_mission_entity); // 1762 0x038A COMMAND_IS_POINT_OBSCURED_BY_A_MISSION_ENTITY (reVC Script4.cpp:1262)
	lua_register(L, "sqrt", sqrt); // 1763 0x01FB COMMAND_SQRT (reVC Script3.cpp:101)
	lua_register(L, "set_camera_in_front_of_player", set_camera_in_front_of_player); // 1764 0x03C8 COMMAND_SET_CAMERA_IN_FRONT_OF_PLAYER (reVC Script4.cpp:1983)
	lua_register(L, "locate_char_on_foot_3d", locate_char_on_foot_3d); // 1765 0x00FF COMMAND_LOCATE_CHAR_ON_FOOT_3D (reVC Script.cpp:4462)
	lua_register(L, "print_with_number_now", print_with_number_now); // 1766 0x01E5 COMMAND_PRINT_WITH_NUMBER_NOW (reVC Script2.cpp:1403)
	lua_register(L, "was_cutscene_skipped", was_cutscene_skipped); // 1767 0x056A COMMAND_WAS_CUTSCENE_SKIPPED (reVC Script7.cpp:1305)
	lua_register(L, "set_zone_ped_info", set_zone_ped_info); // 1768 0x015C COMMAND_SET_ZONE_PED_INFO (reVC Script2.cpp:275)
	lua_register(L, "add_money_spent_on_property", add_money_spent_on_property); // 1769 0x0529 COMMAND_ADD_MONEY_SPENT_ON_PROPERTY (reVC Script7.cpp:878)
	lua_register(L, "set_property_as_owned", set_property_as_owned); // 1770 0x0542 COMMAND_SET_PROPERTY_AS_OWNED (reVC Script7.cpp:1003)
	lua_register(L, "get_object_coordinates", get_object_coordinates); // 1771 0x01BB COMMAND_GET_OBJECT_COORDINATES (reVC Script2.cpp:951)
	lua_register(L, "generate_random_int_in_range", generate_random_int_in_range); // 1772 0x0209 COMMAND_GENERATE_RANDOM_INT_IN_RANGE (reVC Script3.cpp:129)
	lua_register(L, "set_char_cant_be_dragged_out", set_char_cant_be_dragged_out); // 1773 0x039E COMMAND_SET_CHAR_CANT_BE_DRAGGED_OUT (reVC Script4.cpp:1485)
	lua_register(L, "set_cutscene_anim_to_loop", set_cutscene_anim_to_loop); // 1774 0x04BC COMMAND_SET_CUTSCENE_ANIM_TO_LOOP (reVC Script7.cpp:139)
	lua_register(L, "is_player_in_area_2d", is_player_in_area_2d); // 1775 0x0056 COMMAND_IS_PLAYER_IN_AREA_2D (reVC Script.cpp:3248)
	lua_register(L, "set_time_of_day", set_time_of_day); // 1776 0x00C0 COMMAND_SET_TIME_OF_DAY (reVC Script.cpp:4170)
	lua_register(L, "locate_stopped_player_in_car_3d", locate_stopped_player_in_car_3d); // 1777 0x00FA COMMAND_LOCATE_STOPPED_PLAYER_IN_CAR_3D (reVC Script.cpp:4453)
	lua_register(L, "set_char_obj_enter_car_as_passenger", set_char_obj_enter_car_as_passenger); // 1778 0x01D4 COMMAND_SET_CHAR_OBJ_ENTER_CAR_AS_PASSENGER (reVC Script2.cpp:1275)
	lua_register(L, "set_char_suffers_critical_hits", set_char_suffers_critical_hits); // 1779 0x0446 COMMAND_SET_CHAR_SUFFERS_CRITICAL_HITS (reVC Script6.cpp:801)
	lua_register(L, "store_car_char_is_in", store_car_char_is_in); // 1780 0x00D9 COMMAND_STORE_CAR_CHAR_IS_IN (reVC Script.cpp:4258)
	lua_register(L, "locate_player_any_means_2d", locate_player_any_means_2d); // 1781 0x00E3 COMMAND_LOCATE_PLAYER_ANY_MEANS_2D (reVC Script.cpp:4422)
	lua_register(L, "locate_stopped_player_on_foot_3d", locate_stopped_player_on_foot_3d); // 1782 0x00F9 COMMAND_LOCATE_STOPPED_PLAYER_ON_FOOT_3D (reVC Script.cpp:4452)
	lua_register(L, "set_player_as_leader", set_player_as_leader); // 1783 0x01DF COMMAND_SET_PLAYER_AS_LEADER (reVC Script2.cpp:1357)
	lua_register(L, "add_short_range_sprite_blip_for_coord", add_short_range_sprite_blip_for_coord); // 1784 0x04CE COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_COORD (reVC Script7.cpp:244)
	lua_register(L, "stop_char_looking", stop_char_looking); // 1785 0x022F COMMAND_STOP_CHAR_LOOKING (reVC Script3.cpp:548)
	lua_register(L, "has_cutscene_finished", has_cutscene_finished); // 1786 0x02E9 COMMAND_HAS_CUTSCENE_FINISHED (reVC Script3.cpp:1984)
	lua_register(L, "shut_player_up", shut_player_up); // 1787 0x04E2 COMMAND_SHUT_PLAYER_UP (reVC Script7.cpp:390)
	lua_register(L, "is_char_health_greater", is_char_health_greater); // 1788 0x0184 COMMAND_IS_CHAR_HEALTH_GREATER (reVC Script2.cpp:605)
	lua_register(L, "store_wanted_level", store_wanted_level); // 1789 0x01C0 COMMAND_STORE_WANTED_LEVEL (reVC Script2.cpp:1028)
	lua_register(L, "is_char_in_players_group", is_char_in_players_group); // 1790 0x0320 COMMAND_IS_CHAR_IN_PLAYERS_GROUP (reVC Script4.cpp:65)
	lua_register(L, "make_player_safe_for_cutscene", make_player_safe_for_cutscene); // 1791 0x03EF COMMAND_MAKE_PLAYER_SAFE_FOR_CUTSCENE (reVC Script6.cpp:125)
	lua_register(L, "create_protection_pickup", create_protection_pickup); // 1792 0x04A6 COMMAND_CREATE_PROTECTION_PICKUP (reVC Script6.cpp:1719)
	lua_register(L, "locate_stopped_player_any_means_3d", locate_stopped_player_any_means_3d); // 1793 0x00F8 COMMAND_LOCATE_STOPPED_PLAYER_ANY_MEANS_3D (reVC Script.cpp:4451)
	lua_register(L, "char_look_at_player_always", char_look_at_player_always); // 1794 0x022D COMMAND_CHAR_LOOK_AT_PLAYER_ALWAYS (reVC Script3.cpp:526)
	lua_register(L, "stop_player_looking", stop_player_looking); // 1795 0x0230 COMMAND_STOP_PLAYER_LOOKING (reVC Script3.cpp:559)
	lua_register(L, "store_car_player_is_in_no_save", store_car_player_is_in_no_save); // 1796 0x03C1 COMMAND_STORE_CAR_PLAYER_IS_IN_NO_SAVE (reVC Script4.cpp:1770)
	lua_register(L, "get_offset_from_car_in_world_coords", get_offset_from_car_in_world_coords); // 1797 0x0407 COMMAND_GET_OFFSET_FROM_CAR_IN_WORLD_COORDS (reVC Script6.cpp:286)
	lua_register(L, "shut_char_up", shut_char_up); // 1798 0x0489 COMMAND_SHUT_CHAR_UP (reVC Script6.cpp:1524)
	lua_register(L, "draw_weaponshop_corona", draw_weaponshop_corona); // 1799 0x04D5 COMMAND_DRAW_WEAPONSHOP_CORONA (reVC Script7.cpp:302)
	lua_register(L, "does_char_exist", does_char_exist); // 1800 0x056D COMMAND_DOES_CHAR_EXIST (reVC Script7.cpp:1328)
	lua_register(L, "point_camera_at_player", point_camera_at_player); // 1801 0x0157 COMMAND_POINT_CAMERA_AT_PLAYER (reVC Script2.cpp:244)
	lua_register(L, "mark_object_as_no_longer_needed", mark_object_as_no_longer_needed); // 1802 0x01C4 COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED (reVC Script2.cpp:1063)
	lua_register(L, "get_player_char", get_player_char); // 1803 0x01F5 COMMAND_GET_PLAYER_CHAR (reVC Script3.cpp:50)
	lua_register(L, "get_ground_z_for_3d_coord", get_ground_z_for_3d_coord); // 1804 0x02CE COMMAND_GET_GROUND_Z_FOR_3D_COORD (reVC Script3.cpp:1654)
	lua_register(L, "place_object_relative_to_car", place_object_relative_to_car); // 1805 0x035C COMMAND_PLACE_OBJECT_RELATIVE_TO_CAR (reVC Script4.cpp:726)
	lua_register(L, "add_moving_particle_effect", add_moving_particle_effect); // 1806 0x039D COMMAND_ADD_MOVING_PARTICLE_EFFECT (reVC Script4.cpp:1464)
	lua_register(L, "change_garage_type", change_garage_type); // 1807 0x02FA COMMAND_CHANGE_GARAGE_TYPE (reVC Script3.cpp:2127)
	lua_register(L, "has_object_been_damaged", has_object_been_damaged); // 1808 0x0366 COMMAND_HAS_OBJECT_BEEN_DAMAGED (reVC Script4.cpp:899)
	lua_register(L, "is_player_sitting_in_car", is_player_sitting_in_car); // 1809 0x0442 COMMAND_IS_PLAYER_SITTING_IN_CAR (reVC Script6.cpp:775)
	lua_register(L, "is_char_on_foot", is_char_on_foot); // 1810 0x044B COMMAND_IS_CHAR_ON_FOOT (reVC Script6.cpp:846)
	lua_register(L, "is_car_in_area_2d", is_car_in_area_2d); // 1811 0x00B0 COMMAND_IS_CAR_IN_AREA_2D (reVC Script.cpp:4084)
	lua_register(L, "locate_player_on_foot_2d", locate_player_on_foot_2d); // 1812 0x00E4 COMMAND_LOCATE_PLAYER_ON_FOOT_2D (reVC Script.cpp:4423)
	lua_register(L, "locate_stopped_char_any_means_3d", locate_stopped_char_any_means_3d); // 1813 0x0101 COMMAND_LOCATE_STOPPED_CHAR_ANY_MEANS_3D (reVC Script.cpp:4464)
	lua_register(L, "set_char_as_leader", set_char_as_leader); // 1814 0x01DE COMMAND_SET_CHAR_AS_LEADER (reVC Script2.cpp:1348)
	lua_register(L, "locate_char_any_means_car_2d", locate_char_any_means_car_2d); // 1815 0x0202 COMMAND_LOCATE_CHAR_ANY_MEANS_CAR_2D (reVC Script3.cpp:116)
	lua_register(L, "generate_random_float_in_range", generate_random_float_in_range); // 1816 0x0208 COMMAND_GENERATE_RANDOM_FLOAT_IN_RANGE (reVC Script3.cpp:124)
	lua_register(L, "get_char_health", get_char_health); // 1817 0x0226 COMMAND_GET_CHAR_HEALTH (reVC Script3.cpp:428)
	lua_register(L, "char_look_at_char_always", char_look_at_char_always); // 1818 0x022C COMMAND_CHAR_LOOK_AT_CHAR_ALWAYS (reVC Script3.cpp:515)
	lua_register(L, "player_look_at_char_always", player_look_at_char_always); // 1819 0x022E COMMAND_PLAYER_LOOK_AT_CHAR_ALWAYS (reVC Script3.cpp:537)
	lua_register(L, "is_player_in_angled_area_3d", is_player_in_angled_area_3d); // 1820 0x02B3 COMMAND_IS_PLAYER_IN_ANGLED_AREA_3D (reVC Script3.cpp:1494)
	lua_register(L, "get_closest_char_node", get_closest_char_node); // 1821 0x02C0 COMMAND_GET_CLOSEST_CHAR_NODE (reVC Script3.cpp:1547)
	lua_register(L, "add_to_object_velocity", add_to_object_velocity); // 1822 0x038C COMMAND_ADD_TO_OBJECT_VELOCITY (reVC Script4.cpp:1304)
	lua_register(L, "is_player_sitting_in_any_car", is_player_sitting_in_any_car); // 1823 0x0443 COMMAND_IS_PLAYER_SITTING_IN_ANY_CAR (reVC Script6.cpp:784)
	lua_register(L, "is_char_sitting_in_any_car", is_char_sitting_in_any_car); // 1824 0x0449 COMMAND_IS_CHAR_SITTING_IN_ANY_CAR (reVC Script6.cpp:829)
	lua_register(L, "disable_cutscene_shadows", disable_cutscene_shadows); // 1825 0x0522 COMMAND_DISABLE_CUTSCENE_SHADOWS (reVC Script7.cpp:830)
	lua_register(L, "add_number_of_assassinations", add_number_of_assassinations); // 1826 0x0533 COMMAND_ADD_NUMBER_OF_ASSASSINATIONS (reVC Script7.cpp:912)
	lua_register(L, "load_uncompressed_anim", load_uncompressed_anim); // 1827 0x0569 COMMAND_LOAD_UNCOMPRESSED_ANIM (reVC Script7.cpp:1297)
	lua_register(L, "locate_stopped_char_on_foot_2d", locate_stopped_char_on_foot_2d); // 1828 0x00F0 COMMAND_LOCATE_STOPPED_CHAR_ON_FOOT_2D (reVC Script.cpp:4439)
	lua_register(L, "locate_char_any_means_char_2d", locate_char_any_means_char_2d); // 1829 0x00F2 COMMAND_LOCATE_CHAR_ANY_MEANS_CHAR_2D (reVC Script.cpp:4443)
	lua_register(L, "is_player_dead", is_player_dead); // 1830 0x0117 COMMAND_IS_PLAYER_DEAD (reVC Script.cpp:4577)
	lua_register(L, "set_char_obj_wait_on_foot", set_char_obj_wait_on_foot); // 1831 0x0192 COMMAND_SET_CHAR_OBJ_WAIT_ON_FOOT (reVC Script2.cpp:786)
	lua_register(L, "add_route_point", add_route_point); // 1832 0x01E2 COMMAND_ADD_ROUTE_POINT (reVC Script2.cpp:1383)
	lua_register(L, "get_maximum_number_of_passengers", get_maximum_number_of_passengers); // 1833 0x01EA COMMAND_GET_MAXIMUM_NUMBER_OF_PASSENGERS (reVC Script2.cpp:1468)
	lua_register(L, "get_random_car_of_type_in_area", get_random_car_of_type_in_area); // 1834 0x0327 COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA (reVC Script4.cpp:134)
	lua_register(L, "remove_route", remove_route); // 1835 0x03AC COMMAND_REMOVE_ROUTE (reVC Script4.cpp:1583)
	lua_register(L, "set_interpolation_parameters", set_interpolation_parameters); // 1836 0x0460 COMMAND_SET_INTERPOLATION_PARAMETERS (reVC Script6.cpp:1149)
	lua_register(L, "has_animation_loaded", has_animation_loaded); // 1837 0x04EE COMMAND_HAS_ANIMATION_LOADED (reVC Script7.cpp:476)
	lua_register(L, "display_nth_onscreen_counter_with_string", display_nth_onscreen_counter_with_string); // 1838 0x04F7 COMMAND_DISPLAY_NTH_ONSCREEN_COUNTER_WITH_STRING (reVC Script7.cpp:540)
	lua_register(L, "is_char_obj_no_obj", is_char_obj_no_obj); // 1839 0x04FF COMMAND_IS_CHAR_OBJ_NO_OBJ (reVC Script7.cpp:610)
	lua_register(L, "get_distance_between_coords_2d", get_distance_between_coords_2d); // 1840 0x0509 COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D (reVC Script7.cpp:679)
	lua_register(L, "shake_cam", shake_cam); // 1841 0x0003 COMMAND_SHAKE_CAM (reVC Script.cpp:2629)
	lua_register(L, "is_char_in_area_2d", is_char_in_area_2d); // 1842 0x00A3 COMMAND_IS_CHAR_IN_AREA_2D (reVC Script.cpp:3821)
	lua_register(L, "set_zone_car_info", set_zone_car_info); // 1843 0x0152 COMMAND_SET_ZONE_CAR_INFO (reVC Script2.cpp:201)
	lua_register(L, "add_blip_for_car_old", add_blip_for_car_old); // 1844 0x0161 COMMAND_ADD_BLIP_FOR_CAR_OLD (reVC Script2.cpp:325)
	lua_register(L, "add_blip_for_char_old", add_blip_for_char_old); // 1845 0x0162 COMMAND_ADD_BLIP_FOR_CHAR_OLD (reVC Script2.cpp:335)
	lua_register(L, "set_char_obj_flee_on_foot_till_safe", set_char_obj_flee_on_foot_till_safe); // 1846 0x0193 COMMAND_SET_CHAR_OBJ_FLEE_ON_FOOT_TILL_SAFE (reVC Script2.cpp:795)
	lua_register(L, "locate_stopped_car_3d", locate_stopped_car_3d); // 1847 0x01B0 COMMAND_LOCATE_STOPPED_CAR_3D (reVC Script2.cpp:884)
	lua_register(L, "print_with_number", print_with_number); // 1848 0x01E4 COMMAND_PRINT_WITH_NUMBER (reVC Script2.cpp:1396)
	lua_register(L, "get_number_of_passengers", get_number_of_passengers); // 1849 0x01E9 COMMAND_GET_NUMBER_OF_PASSENGERS (reVC Script2.cpp:1459)
	lua_register(L, "set_player_health", set_player_health); // 1850 0x0222 COMMAND_SET_PLAYER_HEALTH (reVC Script3.cpp:385)
	lua_register(L, "get_player_health", get_player_health); // 1851 0x0225 COMMAND_GET_PLAYER_HEALTH (reVC Script3.cpp:419)
	lua_register(L, "sin", sin); // 1852 0x02F6 COMMAND_SIN (reVC Script3.cpp:2099)
	lua_register(L, "cos", cos); // 1853 0x02F7 COMMAND_COS (reVC Script3.cpp:2104)
	lua_register(L, "explode_player_head", explode_player_head); // 1854 0x0322 COMMAND_EXPLODE_PLAYER_HEAD (reVC Script4.cpp:83)
	lua_register(L, "is_area_occupied", is_area_occupied); // 1855 0x0339 COMMAND_IS_AREA_OCCUPIED (reVC Script4.cpp:340)
	lua_register(L, "swap_nearest_building_model", swap_nearest_building_model); // 1856 0x03B6 COMMAND_SWAP_NEAREST_BUILDING_MODEL (reVC Script4.cpp:1642)
	lua_register(L, "store_car_char_is_in_no_save", store_car_char_is_in_no_save); // 1857 0x03C0 COMMAND_STORE_CAR_CHAR_IS_IN_NO_SAVE (reVC Script4.cpp:1760)
	lua_register(L, "get_closest_car_node_with_heading", get_closest_car_node_with_heading); // 1858 0x03D3 COMMAND_GET_CLOSEST_CAR_NODE_WITH_HEADING (reVC Script4.cpp:2061)
	lua_register(L, "set_mission_audio_position", set_mission_audio_position); // 1859 0x03D7 COMMAND_SET_MISSION_AUDIO_POSITION (reVC Script4.cpp:2091)
	lua_register(L, "get_ammo_in_player_weapon", get_ammo_in_player_weapon); // 1860 0x0419 COMMAND_GET_AMMO_IN_PLAYER_WEAPON (reVC Script6.cpp:413)
	lua_register(L, "freeze_char_position", freeze_char_position); // 1861 0x04D7 COMMAND_FREEZE_CHAR_POSITION (reVC Script7.cpp:318)
	lua_register(L, "add_big_gun_flash", add_big_gun_flash); // 1862 0x058A COMMAND_ADD_BIG_GUN_FLASH (reVC Script8.cpp:210)
	lua_register(L, "is_player_pressing_horn", is_player_pressing_horn); // 1863 0x0122 COMMAND_IS_PLAYER_PRESSING_HORN (reVC Script.cpp:4631)
	lua_register(L, "point_camera_at_char", point_camera_at_char); // 1864 0x0159 COMMAND_POINT_CAMERA_AT_CHAR (reVC Script2.cpp:259)
	lua_register(L, "set_player_ammo", set_player_ammo); // 1865 0x017A COMMAND_SET_PLAYER_AMMO (reVC Script2.cpp:573)
	lua_register(L, "is_player_health_greater", is_player_health_greater); // 1866 0x0183 COMMAND_IS_PLAYER_HEALTH_GREATER (reVC Script2.cpp:598)
	lua_register(L, "locate_player_any_means_car_3d", locate_player_any_means_car_3d); // 1867 0x01FF COMMAND_LOCATE_PLAYER_ANY_MEANS_CAR_3D (reVC Script3.cpp:111)
	lua_register(L, "set_gang_weapons", set_gang_weapons); // 1868 0x0237 COMMAND_SET_GANG_WEAPONS (reVC Script3.cpp:587)
	lua_register(L, "is_player_stopped", is_player_stopped); // 1869 0x029F COMMAND_IS_PLAYER_STOPPED (reVC Script3.cpp:1345)
	lua_register(L, "is_char_in_chars_group", is_char_in_chars_group); // 1870 0x031F COMMAND_IS_CHAR_IN_CHARS_GROUP (reVC Script3.cpp:2398)
	lua_register(L, "set_camera_zoom", set_camera_zoom); // 1871 0x032A COMMAND_SET_CAMERA_ZOOM (reVC Script4.cpp:208)
	lua_register(L, "set_text_scale", set_text_scale); // 1872 0x033F COMMAND_SET_TEXT_SCALE (reVC Script4.cpp:396)
	lua_register(L, "set_text_colour", set_text_colour); // 1873 0x0340 COMMAND_SET_TEXT_COLOUR (reVC Script4.cpp:403)
	lua_register(L, "set_text_justify", set_text_justify); // 1874 0x0341 COMMAND_SET_TEXT_JUSTIFY (reVC Script4.cpp:410)
	lua_register(L, "set_text_centre", set_text_centre); // 1875 0x0342 COMMAND_SET_TEXT_CENTRE (reVC Script4.cpp:416)
	lua_register(L, "set_text_wrapx", set_text_wrapx); // 1876 0x0343 COMMAND_SET_TEXT_WRAPX (reVC Script4.cpp:422)
	lua_register(L, "set_text_background", set_text_background); // 1877 0x0345 COMMAND_SET_TEXT_BACKGROUND (reVC Script4.cpp:436)
	lua_register(L, "set_text_proportional", set_text_proportional); // 1878 0x0348 COMMAND_SET_TEXT_PROPORTIONAL (reVC Script4.cpp:457)
	lua_register(L, "freeze_onscreen_timer", freeze_onscreen_timer); // 1879 0x0396 COMMAND_FREEZE_ONSCREEN_TIMER (reVC Script4.cpp:1413)
	lua_register(L, "draw_sphere", draw_sphere); // 1880 0x03A1 COMMAND_DRAW_SPHERE (reVC Script4.cpp:1514)
	lua_register(L, "set_text_right_justify", set_text_right_justify); // 1881 0x03E4 COMMAND_SET_TEXT_RIGHT_JUSTIFY (reVC Script4.cpp:2179)
	lua_register(L, "set_generate_cars_around_camera", set_generate_cars_around_camera); // 1882 0x03EA COMMAND_SET_GENERATE_CARS_AROUND_CAMERA (reVC Script6.cpp:95)
	lua_register(L, "use_text_commands", use_text_commands); // 1883 0x03F0 COMMAND_USE_TEXT_COMMANDS (reVC Script6.cpp:138)
	lua_register(L, "set_car_can_be_damaged", set_car_can_be_damaged); // 1884 0x03F5 COMMAND_SET_CAR_CAN_BE_DAMAGED (reVC Script6.cpp:166)
	lua_register(L, "get_offset_from_object_in_world_coords", get_offset_from_object_in_world_coords); // 1885 0x0400 COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS (reVC Script6.cpp:253)
	lua_register(L, "register_highest_score", register_highest_score); // 1886 0x042F COMMAND_REGISTER_HIGHEST_SCORE (reVC Script6.cpp:639)
	lua_register(L, "create_single_particle", create_single_particle); // 1887 0x0437 COMMAND_CREATE_SINGLE_PARTICLE (reVC Script6.cpp:682)
	lua_register(L, "is_player_on_foot", is_player_on_foot); // 1888 0x044A COMMAND_IS_PLAYER_ON_FOOT (reVC Script6.cpp:837)
	lua_register(L, "set_object_rotation", set_object_rotation); // 1889 0x0453 COMMAND_SET_OBJECT_ROTATION (reVC Script6.cpp:951)
	lua_register(L, "display_text_with_number", display_text_with_number); // 1890 0x045A COMMAND_DISPLAY_TEXT_WITH_NUMBER (reVC Script6.cpp:1054)
	lua_register(L, "get_random_cop_in_area", get_random_cop_in_area); // 1891 0x0469 COMMAND_GET_RANDOM_COP_IN_AREA (reVC Script6.cpp:1233)
	lua_register(L, "get_offset_from_char_in_world_coords", get_offset_from_char_in_world_coords); // 1892 0x04C4 COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS (reVC Script7.cpp:191)
	lua_register(L, "switch_security_camera", switch_security_camera); // 1893 0x04C7 COMMAND_SWITCH_SECURITY_CAMERA (reVC Script7.cpp:225)
	lua_register(L, "set_zone_civilian_car_info", set_zone_civilian_car_info); // 1894 0x04EC COMMAND_SET_ZONE_CIVILIAN_CAR_INFO (reVC Script7.cpp:443)
	lua_register(L, "is_char_waiting_for_world_collision", is_char_waiting_for_world_collision); // 1895 0x04F0 COMMAND_IS_CHAR_WAITING_FOR_WORLD_COLLISION (reVC Script7.cpp:492)
	lua_register(L, "attach_char_to_object", attach_char_to_object); // 1896 0x04F4 COMMAND_ATTACH_CHAR_TO_OBJECT (reVC Script7.cpp:523)
	lua_register(L, "is_player_wearing", is_player_wearing); // 1897 0x0500 COMMAND_IS_PLAYER_WEARING (reVC Script7.cpp:618)
	lua_register(L, "attach_cutscene_object_to_bone", attach_cutscene_object_to_bone); // 1898 0x0524 COMMAND_ATTACH_CUTSCENE_OBJECT_TO_BONE (reVC Script7.cpp:846)
	lua_register(L, "set_player_is_in_stadium", set_player_is_in_stadium); // 1899 0x057E COMMAND_SET_PLAYER_IS_IN_STADIUM (reVC Script8.cpp:83)
	lua_register(L, "char_wander_dir", char_wander_dir); // 1900 0x009C COMMAND_CHAR_WANDER_DIR (reVC Script.cpp:3709)
	lua_register(L, "get_time_of_day", get_time_of_day); // 1901 0x00BF COMMAND_GET_TIME_OF_DAY (reVC Script.cpp:4165)
	lua_register(L, "locate_player_in_car_2d", locate_player_in_car_2d); // 1902 0x00E5 COMMAND_LOCATE_PLAYER_IN_CAR_2D (reVC Script.cpp:4424)
	lua_register(L, "locate_stopped_player_on_foot_2d", locate_stopped_player_on_foot_2d); // 1903 0x00E7 COMMAND_LOCATE_STOPPED_PLAYER_ON_FOOT_2D (reVC Script.cpp:4426)
	lua_register(L, "locate_player_on_foot_char_2d", locate_player_on_foot_char_2d); // 1904 0x00EA COMMAND_LOCATE_PLAYER_ON_FOOT_CHAR_2D (reVC Script.cpp:4431)
	lua_register(L, "locate_player_in_car_char_2d", locate_player_in_car_char_2d); // 1905 0x00EB COMMAND_LOCATE_PLAYER_IN_CAR_CHAR_2D (reVC Script.cpp:4432)
	lua_register(L, "locate_char_in_car_2d", locate_char_in_car_2d); // 1906 0x00EE COMMAND_LOCATE_CHAR_IN_CAR_2D (reVC Script.cpp:4437)
	lua_register(L, "locate_player_on_foot_char_3d", locate_player_on_foot_char_3d); // 1907 0x00FC COMMAND_LOCATE_PLAYER_ON_FOOT_CHAR_3D (reVC Script.cpp:4457)
	lua_register(L, "locate_player_in_car_char_3d", locate_player_in_car_char_3d); // 1908 0x00FD COMMAND_LOCATE_PLAYER_IN_CAR_CHAR_3D (reVC Script.cpp:4458)
	lua_register(L, "is_score_greater", is_score_greater); // 1909 0x010A COMMAND_IS_SCORE_GREATER (reVC Script.cpp:4518)
	lua_register(L, "store_score", store_score); // 1910 0x010B COMMAND_STORE_SCORE (reVC Script.cpp:4522)
	lua_register(L, "set_deatharrest_state", set_deatharrest_state); // 1911 0x0111 COMMAND_SET_DEATHARREST_STATE (reVC Script.cpp:4552)
	lua_register(L, "create_car_generator", create_car_generator); // 1912 0x014B COMMAND_CREATE_CAR_GENERATOR (reVC Script2.cpp:133)
	lua_register(L, "add_hospital_restart", add_hospital_restart); // 1913 0x016C COMMAND_ADD_HOSPITAL_RESTART (reVC Script2.cpp:395)
	lua_register(L, "add_police_restart", add_police_restart); // 1914 0x016D COMMAND_ADD_POLICE_RESTART (reVC Script2.cpp:405)
	lua_register(L, "add_blip_for_contact_point", add_blip_for_contact_point); // 1915 0x0189 COMMAND_ADD_BLIP_FOR_CONTACT_POINT (reVC Script2.cpp:657)
	lua_register(L, "locate_player_in_car_car_2d", locate_player_in_car_car_2d); // 1916 0x01FE COMMAND_LOCATE_PLAYER_IN_CAR_CAR_2D (reVC Script3.cpp:110)
	lua_register(L, "locate_char_on_foot_car_2d", locate_char_on_foot_car_2d); // 1917 0x0203 COMMAND_LOCATE_CHAR_ON_FOOT_CAR_2D (reVC Script3.cpp:117)
	lua_register(L, "locate_char_any_means_car_3d", locate_char_any_means_car_3d); // 1918 0x0205 COMMAND_LOCATE_CHAR_ANY_MEANS_CAR_3D (reVC Script3.cpp:119)
	lua_register(L, "set_gang_ped_models", set_gang_ped_models); // 1919 0x0235 COMMAND_SET_GANG_PED_MODELS (reVC Script3.cpp:579)
	lua_register(L, "is_player_in_angled_area_2d", is_player_in_angled_area_2d); // 1920 0x02AD COMMAND_IS_PLAYER_IN_ANGLED_AREA_2D (reVC Script3.cpp:1488)
	lua_register(L, "set_zone_group", set_zone_group); // 1921 0x0324 COMMAND_SET_ZONE_GROUP (reVC Script4.cpp:99)
	lua_register(L, "display_text", display_text); // 1922 0x033E COMMAND_DISPLAY_TEXT (reVC Script4.cpp:382)
	lua_register(L, "open_garage", open_garage); // 1923 0x0360 COMMAND_OPEN_GARAGE (reVC Script4.cpp:763)
	lua_register(L, "is_sniper_bullet_in_area", is_sniper_bullet_in_area); // 1924 0x037E COMMAND_IS_SNIPER_BULLET_IN_AREA (reVC Script4.cpp:1131)
	lua_register(L, "print_string_in_string_now", print_string_in_string_now); // 1925 0x0384 COMMAND_PRINT_STRING_IN_STRING_NOW (reVC Script4.cpp:1205)
	lua_register(L, "remove_particle_effects_in_area", remove_particle_effects_in_area); // 1926 0x03AE COMMAND_REMOVE_PARTICLE_EFFECTS_IN_AREA (reVC Script4.cpp:1591)
	lua_register(L, "is_garage_closed", is_garage_closed); // 1927 0x03B1 COMMAND_IS_GARAGE_CLOSED (reVC Script4.cpp:1624)
	lua_register(L, "does_object_exist", does_object_exist); // 1928 0x03CA COMMAND_DOES_OBJECT_EXIST (reVC Script4.cpp:1996)
	lua_register(L, "clear_this_big_print", clear_this_big_print); // 1929 0x03D6 COMMAND_CLEAR_THIS_BIG_PRINT (reVC Script4.cpp:2085)
	lua_register(L, "no_special_camera_for_this_garage", no_special_camera_for_this_garage); // 1930 0x03DA COMMAND_NO_SPECIAL_CAMERA_FOR_THIS_GARAGE (reVC Script4.cpp:2109)
	lua_register(L, "set_car_avoid_level_transitions", set_car_avoid_level_transitions); // 1931 0x0428 COMMAND_SET_CAR_AVOID_LEVEL_TRANSITIONS (reVC Script6.cpp:580)
	lua_register(L, "get_car_model", get_car_model); // 1932 0x0441 COMMAND_GET_CAR_MODEL (reVC Script6.cpp:766)
	lua_register(L, "display_text_with_2_numbers", display_text_with_2_numbers); // 1933 0x045B COMMAND_DISPLAY_TEXT_WITH_2_NUMBERS (reVC Script6.cpp:1065)
	lua_register(L, "get_number_of_followers", get_number_of_followers); // 1934 0x046D COMMAND_GET_NUMBER_OF_FOLLOWERS (reVC Script6.cpp:1354)
	lua_register(L, "get_current_char_weapon", get_current_char_weapon); // 1935 0x0470 COMMAND_GET_CURRENT_CHAR_WEAPON (reVC Script6.cpp:1390)
	lua_register(L, "remove_all_char_weapons", remove_all_char_weapons); // 1936 0x048F COMMAND_REMOVE_ALL_CHAR_WEAPONS (reVC Script6.cpp:1549)
	lua_register(L, "has_player_got_weapon", has_player_got_weapon); // 1937 0x0490 COMMAND_HAS_PLAYER_GOT_WEAPON (reVC Script6.cpp:1557)
	lua_register(L, "get_position_of_analogue_sticks", get_position_of_analogue_sticks); // 1938 0x0494 COMMAND_GET_POSITION_OF_ANALOGUE_STICKS (reVC Script6.cpp:1582)
	lua_register(L, "set_object_path_speed", set_object_path_speed); // 1939 0x049E COMMAND_SET_OBJECT_PATH_SPEED (reVC Script6.cpp:1668)
	lua_register(L, "get_closest_straight_road", get_closest_straight_road); // 1940 0x04B9 COMMAND_GET_CLOSEST_STRAIGHT_ROAD (reVC Script7.cpp:104)
	lua_register(L, "get_nth_closest_car_node", get_nth_closest_car_node); // 1941 0x04D3 COMMAND_GET_NTH_CLOSEST_CAR_NODE (reVC Script7.cpp:291)
	lua_register(L, "set_object_records_collisions", set_object_records_collisions); // 1942 0x04D9 COMMAND_SET_OBJECT_RECORDS_COLLISIONS (reVC Script7.cpp:334)
	lua_register(L, "has_object_collided_with_anything", has_object_collided_with_anything); // 1943 0x04DA COMMAND_HAS_OBJECT_COLLIDED_WITH_ANYTHING (reVC Script7.cpp:342)
	lua_register(L, "set_heli_stabiliser", set_heli_stabiliser); // 1944 0x04DF COMMAND_SET_HELI_STABILISER (reVC Script7.cpp:366)
	lua_register(L, "is_object_in_water", is_object_in_water); // 1945 0x04E7 COMMAND_IS_OBJECT_IN_WATER (reVC Script7.cpp:415)
	lua_register(L, "set_player_can_do_drive_by", set_player_can_do_drive_by); // 1946 0x0501 COMMAND_SET_PLAYER_CAN_DO_DRIVE_BY (reVC Script7.cpp:631)
	lua_register(L, "sort_out_object_collision_with_car", sort_out_object_collision_with_car); // 1947 0x050E COMMAND_SORT_OUT_OBJECT_COLLISION_WITH_CAR (reVC Script7.cpp:710)
	lua_register(L, "print_help_forever", print_help_forever); // 1948 0x0512 COMMAND_PRINT_HELP_FOREVER (reVC Script7.cpp:728)
	lua_register(L, "create_forsale_property_pickup", create_forsale_property_pickup); // 1949 0x0518 COMMAND_CREATE_FORSALE_PROPERTY_PICKUP (reVC Script7.cpp:775)
	lua_register(L, "increase_player_max_health", increase_player_max_health); // 1950 0x055E COMMAND_INCREASE_PLAYER_MAX_HEALTH (reVC Script7.cpp:1162)
	lua_register(L, "increase_player_max_armour", increase_player_max_armour); // 1951 0x055F COMMAND_INCREASE_PLAYER_MAX_ARMOUR (reVC Script7.cpp:1170)
	lua_register(L, "create_random_char_as_passenger", create_random_char_as_passenger); // 1952 0x0561 COMMAND_CREATE_RANDOM_CHAR_AS_PASSENGER (reVC Script7.cpp:1209)
	lua_register(L, "set_object_area_visible", set_object_area_visible); // 1953 0x0566 COMMAND_SET_OBJECT_AREA_VISIBLE (reVC Script7.cpp:1280)
	lua_register(L, "set_char_crouch_when_threatened", set_char_crouch_when_threatened); // 1954 0x056B COMMAND_SET_CHAR_CROUCH_WHEN_THREATENED (reVC Script7.cpp:1310)
	lua_register(L, "set_allow_hurricanes", set_allow_hurricanes); // 1955 0x057C COMMAND_SET_ALLOW_HURRICANES (reVC Script8.cpp:73)
	lua_register(L, "play_announcement", play_announcement); // 1956 0x057D COMMAND_PLAY_ANNOUNCEMENT (reVC Script8.cpp:77)
	lua_register(L, "create_player", create_player); // 1957 0x0053 COMMAND_CREATE_PLAYER (reVC Script.cpp:3159)
	lua_register(L, "generate_random_int", generate_random_int); // 1958 0x0099 COMMAND_GENERATE_RANDOM_INT (reVC Script.cpp:3637)
	lua_register(L, "is_car_in_area_3d", is_car_in_area_3d); // 1959 0x00B1 COMMAND_IS_CAR_IN_AREA_3D (reVC Script.cpp:4100)
	lua_register(L, "get_minutes_to_time_of_day", get_minutes_to_time_of_day); // 1960 0x00C1 COMMAND_GET_MINUTES_TO_TIME_OF_DAY (reVC Script.cpp:4174)
	lua_register(L, "is_char_in_model", is_char_in_model); // 1961 0x00DD COMMAND_IS_CHAR_IN_MODEL (reVC Script.cpp:4378)
	lua_register(L, "locate_char_on_foot_char_2d", locate_char_on_foot_char_2d); // 1962 0x00F3 COMMAND_LOCATE_CHAR_ON_FOOT_CHAR_2D (reVC Script.cpp:4444)
	lua_register(L, "locate_char_in_car_3d", locate_char_in_car_3d); // 1963 0x0100 COMMAND_LOCATE_CHAR_IN_CAR_3D (reVC Script.cpp:4463)
	lua_register(L, "locate_stopped_char_on_foot_3d", locate_stopped_char_on_foot_3d); // 1964 0x0102 COMMAND_LOCATE_STOPPED_CHAR_ON_FOOT_3D (reVC Script.cpp:4465)
	lua_register(L, "locate_char_any_means_char_3d", locate_char_any_means_char_3d); // 1965 0x0104 COMMAND_LOCATE_CHAR_ANY_MEANS_CHAR_3D (reVC Script.cpp:4469)
	lua_register(L, "override_next_restart", override_next_restart); // 1966 0x016E COMMAND_OVERRIDE_NEXT_RESTART (reVC Script2.cpp:415)
	lua_register(L, "declare_mission_flag", declare_mission_flag); // 1967 0x0180 COMMAND_DECLARE_MISSION_FLAG (reVC Script2.cpp:592)
	lua_register(L, "dont_remove_char", dont_remove_char); // 1968 0x01C5 COMMAND_DONT_REMOVE_CHAR (reVC Script2.cpp:1072)
	lua_register(L, "cancel_override_restart", cancel_override_restart); // 1969 0x01F6 COMMAND_CANCEL_OVERRIDE_RESTART (reVC Script3.cpp:59)
	lua_register(L, "start_kill_frenzy", start_kill_frenzy); // 1970 0x01F9 COMMAND_START_KILL_FRENZY (reVC Script3.cpp:86)
	lua_register(L, "read_kill_frenzy_status", read_kill_frenzy_status); // 1971 0x01FA COMMAND_READ_KILL_FRENZY_STATUS (reVC Script3.cpp:95)
	lua_register(L, "set_taxi_lights", set_taxi_lights); // 1972 0x0216 COMMAND_SET_TAXI_LIGHTS (reVC Script3.cpp:253)
	lua_register(L, "print_big_q", print_big_q); // 1973 0x0217 COMMAND_PRINT_BIG_Q (reVC Script3.cpp:262)
	lua_register(L, "set_garage", set_garage); // 1974 0x0219 COMMAND_SET_GARAGE (reVC Script3.cpp:279)
	lua_register(L, "set_target_car_for_mission_garage", set_target_car_for_mission_garage); // 1975 0x021B COMMAND_SET_TARGET_CAR_FOR_MISSION_GARAGE (reVC Script3.cpp:311)
	lua_register(L, "set_gang_car_model", set_gang_car_model); // 1976 0x0236 COMMAND_SET_GANG_CAR_MODEL (reVC Script3.cpp:583)
	lua_register(L, "store_clock", store_clock); // 1977 0x0253 COMMAND_STORE_CLOCK (reVC Script3.cpp:834)
	lua_register(L, "reset_num_of_models_killed_by_player", reset_num_of_models_killed_by_player); // 1978 0x0297 COMMAND_RESET_NUM_OF_MODELS_KILLED_BY_PLAYER (reVC Script3.cpp:1235)
	lua_register(L, "get_num_of_models_killed_by_player", get_num_of_models_killed_by_player); // 1979 0x0298 COMMAND_GET_NUM_OF_MODELS_KILLED_BY_PLAYER (reVC Script3.cpp:1238)
	lua_register(L, "is_player_in_angled_area_on_foot_3d", is_player_in_angled_area_on_foot_3d); // 1980 0x02B4 COMMAND_IS_PLAYER_IN_ANGLED_AREA_ON_FOOT_3D (reVC Script3.cpp:1495)
	lua_register(L, "is_player_in_angled_area_in_car_3d", is_player_in_angled_area_in_car_3d); // 1981 0x02B5 COMMAND_IS_PLAYER_IN_ANGLED_AREA_IN_CAR_3D (reVC Script3.cpp:1496)
	lua_register(L, "is_player_shooting_in_area", is_player_shooting_in_area); // 1982 0x02D5 COMMAND_IS_PLAYER_SHOOTING_IN_AREA (reVC Script3.cpp:1722)
	lua_register(L, "get_random_char_in_zone", get_random_char_in_zone); // 1983 0x02DD COMMAND_GET_RANDOM_CHAR_IN_ZONE (reVC Script3.cpp:1844)
	lua_register(L, "is_player_in_taxi", is_player_in_taxi); // 1984 0x02DE COMMAND_IS_PLAYER_IN_TAXI (reVC Script3.cpp:1898)
	lua_register(L, "create_collectable1", create_collectable1); // 1985 0x02EC COMMAND_CREATE_COLLECTABLE1 (reVC Script3.cpp:1993)
	lua_register(L, "set_collectable1_total", set_collectable1_total); // 1986 0x02ED COMMAND_SET_COLLECTABLE1_TOTAL (reVC Script3.cpp:2002)
	lua_register(L, "is_char_model", is_char_model); // 1987 0x02F2 COMMAND_IS_CHAR_MODEL (reVC Script3.cpp:2078)
	lua_register(L, "print_with_3_numbers", print_with_3_numbers); // 1988 0x02FF COMMAND_PRINT_WITH_3_NUMBERS (reVC Script3.cpp:2177)
	lua_register(L, "print_with_4_numbers", print_with_4_numbers); // 1989 0x0302 COMMAND_PRINT_WITH_4_NUMBERS (reVC Script3.cpp:2200)
	lua_register(L, "print_with_6_numbers", print_with_6_numbers); // 1990 0x0308 COMMAND_PRINT_WITH_6_NUMBERS (reVC Script3.cpp:2244)
	lua_register(L, "set_progress_total", set_progress_total); // 1991 0x030D COMMAND_SET_PROGRESS_TOTAL (reVC Script3.cpp:2282)
	lua_register(L, "register_jump_distance", register_jump_distance); // 1992 0x030E COMMAND_REGISTER_JUMP_DISTANCE (reVC Script3.cpp:2288)
	lua_register(L, "register_jump_height", register_jump_height); // 1993 0x030F COMMAND_REGISTER_JUMP_HEIGHT (reVC Script3.cpp:2292)
	lua_register(L, "register_jump_flips", register_jump_flips); // 1994 0x0310 COMMAND_REGISTER_JUMP_FLIPS (reVC Script3.cpp:2296)
	lua_register(L, "register_jump_spins", register_jump_spins); // 1995 0x0311 COMMAND_REGISTER_JUMP_SPINS (reVC Script3.cpp:2300)
	lua_register(L, "register_jump_stunt", register_jump_stunt); // 1996 0x0312 COMMAND_REGISTER_JUMP_STUNT (reVC Script3.cpp:2304)
	lua_register(L, "register_unique_jump_found", register_unique_jump_found); // 1997 0x0313 COMMAND_REGISTER_UNIQUE_JUMP_FOUND (reVC Script3.cpp:2308)
	lua_register(L, "set_unique_jumps_total", set_unique_jumps_total); // 1998 0x0314 COMMAND_SET_UNIQUE_JUMPS_TOTAL (reVC Script3.cpp:2311)
	lua_register(L, "register_passenger_dropped_off_taxi", register_passenger_dropped_off_taxi); // 1999 0x0315 COMMAND_REGISTER_PASSENGER_DROPPED_OFF_TAXI (reVC Script3.cpp:2315)
	lua_register(L, "register_money_made_taxi", register_money_made_taxi); // 2000 0x0316 COMMAND_REGISTER_MONEY_MADE_TAXI (reVC Script3.cpp:2318)
	lua_register(L, "set_player_never_gets_tired", set_player_never_gets_tired); // 2001 0x0330 COMMAND_SET_PLAYER_NEVER_GETS_TIRED (reVC Script4.cpp:268)
	lua_register(L, "close_garage", close_garage); // 2002 0x0361 COMMAND_CLOSE_GARAGE (reVC Script4.cpp:769)
	lua_register(L, "print_with_2_numbers_big", print_with_2_numbers_big); // 2003 0x036D COMMAND_PRINT_WITH_2_NUMBERS_BIG (reVC Script4.cpp:963)
	lua_register(L, "is_icecream_jingle_on", is_icecream_jingle_on); // 2004 0x0383 COMMAND_IS_ICECREAM_JINGLE_ON (reVC Script4.cpp:1182)
	lua_register(L, "is_char_male", is_char_male); // 2005 0x03A3 COMMAND_IS_CHAR_MALE (reVC Script4.cpp:1533)
	lua_register(L, "is_garage_open", is_garage_open); // 2006 0x03B0 COMMAND_IS_GARAGE_OPEN (reVC Script4.cpp:1620)
	lua_register(L, "switch_world_processing", switch_world_processing); // 2007 0x03B7 COMMAND_SWITCH_WORLD_PROCESSING (reVC Script4.cpp:1676)
	lua_register(L, "remove_all_player_weapons", remove_all_player_weapons); // 2008 0x03B8 COMMAND_REMOVE_ALL_PLAYER_WEAPONS (reVC Script4.cpp:1680)
	lua_register(L, "set_rotating_garage_door", set_rotating_garage_door); // 2009 0x03BB COMMAND_SET_ROTATING_GARAGE_DOOR (reVC Script4.cpp:1721)
	lua_register(L, "create_random_car_for_car_park", create_random_car_for_car_park); // 2010 0x03C5 COMMAND_CREATE_RANDOM_CAR_FOR_CAR_PARK (reVC Script4.cpp:1808)
	lua_register(L, "has_import_garage_slot_been_filled", has_import_garage_slot_been_filled); // 2011 0x03D4 COMMAND_HAS_IMPORT_GARAGE_SLOT_BEEN_FILLED (reVC Script4.cpp:2073)
	lua_register(L, "has_save_game_finished", has_save_game_finished); // 2012 0x03D9 COMMAND_HAS_SAVE_GAME_FINISHED (reVC Script4.cpp:2106)
	lua_register(L, "force_random_ped_type", force_random_ped_type); // 2013 0x03DF COMMAND_FORCE_RANDOM_PED_TYPE (reVC Script4.cpp:2152)
	lua_register(L, "get_collectable1s_collected", get_collectable1s_collected); // 2014 0x03E1 COMMAND_GET_COLLECTABLE1S_COLLECTED (reVC Script4.cpp:2162)
	lua_register(L, "get_car_colours", get_car_colours); // 2015 0x03F3 COMMAND_GET_CAR_COLOURS (reVC Script6.cpp:150)
	lua_register(L, "set_drunk_input_delay", set_drunk_input_delay); // 2016 0x03FD COMMAND_SET_DRUNK_INPUT_DELAY (reVC Script6.cpp:236)
	lua_register(L, "register_life_saved", register_life_saved); // 2017 0x0401 COMMAND_REGISTER_LIFE_SAVED (reVC Script6.cpp:263)
	lua_register(L, "register_criminal_caught", register_criminal_caught); // 2018 0x0402 COMMAND_REGISTER_CRIMINAL_CAUGHT (reVC Script6.cpp:266)
	lua_register(L, "register_ambulance_level", register_ambulance_level); // 2019 0x0403 COMMAND_REGISTER_AMBULANCE_LEVEL (reVC Script6.cpp:269)
	lua_register(L, "register_fire_extinguished", register_fire_extinguished); // 2020 0x0404 COMMAND_REGISTER_FIRE_EXTINGUISHED (reVC Script6.cpp:273)
	lua_register(L, "set_total_number_of_kill_frenzies", set_total_number_of_kill_frenzies); // 2021 0x0408 COMMAND_SET_TOTAL_NUMBER_OF_KILL_FRENZIES (reVC Script6.cpp:296)
	lua_register(L, "set_free_health_care", set_free_health_care); // 2022 0x0414 COMMAND_SET_FREE_HEALTH_CARE (reVC Script6.cpp:357)
	lua_register(L, "load_and_launch_mission_internal", load_and_launch_mission_internal); // 2023 0x0417 COMMAND_LOAD_AND_LAUNCH_MISSION_INTERNAL (reVC Script6.cpp:373)
	lua_register(L, "set_object_draw_last", set_object_draw_last); // 2024 0x0418 COMMAND_SET_OBJECT_DRAW_LAST (reVC Script6.cpp:405)
	lua_register(L, "convert_metres_to_feet", convert_metres_to_feet); // 2025 0x0425 COMMAND_CONVERT_METRES_TO_FEET (reVC Script6.cpp:521)
	lua_register(L, "set_total_number_of_missions", set_total_number_of_missions); // 2026 0x042C COMMAND_SET_TOTAL_NUMBER_OF_MISSIONS (reVC Script6.cpp:626)
	lua_register(L, "convert_metres_to_feet_int", convert_metres_to_feet_int); // 2027 0x042D COMMAND_CONVERT_METRES_TO_FEET_INT (reVC Script6.cpp:630)
	lua_register(L, "set_char_is_chris_criminal", set_char_is_chris_criminal); // 2028 0x0433 COMMAND_SET_CHAR_IS_CHRIS_CRIMINAL (reVC Script6.cpp:665)
	lua_register(L, "start_credits", start_credits); // 2029 0x0434 COMMAND_START_CREDITS (reVC Script6.cpp:673)
	lua_register(L, "stop_credits", stop_credits); // 2030 0x0435 COMMAND_STOP_CREDITS (reVC Script6.cpp:676)
	lua_register(L, "are_credits_finished", are_credits_finished); // 2031 0x0436 COMMAND_ARE_CREDITS_FINISHED (reVC Script6.cpp:679)
	lua_register(L, "play_end_of_game_tune", play_end_of_game_tune); // 2032 0x043F COMMAND_PLAY_END_OF_GAME_TUNE (reVC Script6.cpp:759)
	lua_register(L, "stop_end_of_game_tune", stop_end_of_game_tune); // 2033 0x0440 COMMAND_STOP_END_OF_GAME_TUNE (reVC Script6.cpp:762)
	lua_register(L, "are_any_car_cheats_activated", are_any_car_cheats_activated); // 2034 0x0445 COMMAND_ARE_ANY_CAR_CHEATS_ACTIVATED (reVC Script6.cpp:798)
	lua_register(L, "load_splash_screen", load_splash_screen); // 2035 0x044D COMMAND_LOAD_SPLASH_SCREEN (reVC Script6.cpp:902)
	lua_register(L, "load_end_of_game_tune", load_end_of_game_tune); // 2036 0x0451 COMMAND_LOAD_END_OF_GAME_TUNE (reVC Script6.cpp:940)
	lua_register(L, "terminate_all_scripts_with_this_name", terminate_all_scripts_with_this_name); // 2037 0x0459 COMMAND_TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME (reVC Script6.cpp:1037)
	lua_register(L, "get_current_player_weapon", get_current_player_weapon); // 2038 0x046F COMMAND_GET_CURRENT_PLAYER_WEAPON (reVC Script6.cpp:1381)
	lua_register(L, "locate_char_any_means_object_2d", locate_char_any_means_object_2d); // 2039 0x0471 COMMAND_LOCATE_CHAR_ANY_MEANS_OBJECT_2D (reVC Script6.cpp:1399)
	lua_register(L, "locate_char_on_foot_object_2d", locate_char_on_foot_object_2d); // 2040 0x0472 COMMAND_LOCATE_CHAR_ON_FOOT_OBJECT_2D (reVC Script6.cpp:1400)
	lua_register(L, "can_char_see_dead_char", can_char_see_dead_char); // 2041 0x0480 COMMAND_CAN_CHAR_SEE_DEAD_CHAR (reVC Script6.cpp:1473)
	lua_register(L, "is_any_pickup_at_coords", is_any_pickup_at_coords); // 2042 0x048C COMMAND_IS_ANY_PICKUP_AT_COORDS (reVC Script6.cpp:1540)
	lua_register(L, "initialise_object_path", initialise_object_path); // 2043 0x049C COMMAND_INITIALISE_OBJECT_PATH (reVC Script6.cpp:1649)
	lua_register(L, "start_object_on_path", start_object_on_path); // 2044 0x049D COMMAND_START_OBJECT_ON_PATH (reVC Script6.cpp:1660)
	lua_register(L, "set_object_path_position", set_object_path_position); // 2045 0x049F COMMAND_SET_OBJECT_PATH_POSITION (reVC Script6.cpp:1674)
	lua_register(L, "clear_object_path", clear_object_path); // 2046 0x04A1 COMMAND_CLEAR_OBJECT_PATH (reVC Script6.cpp:1681)
	lua_register(L, "reset_havoc_caused_by_player", reset_havoc_caused_by_player); // 2047 0x04BE COMMAND_RESET_HAVOC_CAUSED_BY_PLAYER (reVC Script7.cpp:155)
	lua_register(L, "get_havoc_caused_by_player", get_havoc_caused_by_player); // 2048 0x04BF COMMAND_GET_HAVOC_CAUSED_BY_PLAYER (reVC Script7.cpp:161)
	lua_register(L, "create_script_roadblock", create_script_roadblock); // 2049 0x04C0 COMMAND_CREATE_SCRIPT_ROADBLOCK (reVC Script7.cpp:168)
	lua_register(L, "clear_all_script_roadblocks", clear_all_script_roadblocks); // 2050 0x04C1 COMMAND_CLEAR_ALL_SCRIPT_ROADBLOCKS (reVC Script7.cpp:174)
	lua_register(L, "has_char_been_photographed", has_char_been_photographed); // 2051 0x04C5 COMMAND_HAS_CHAR_BEEN_PHOTOGRAPHED (reVC Script7.cpp:201)
	lua_register(L, "add_money_spent_on_clothes", add_money_spent_on_clothes); // 2052 0x04CF COMMAND_ADD_MONEY_SPENT_ON_CLOTHES (reVC Script7.cpp:257)
	lua_register(L, "set_heli_orientation", set_heli_orientation); // 2053 0x04D0 COMMAND_SET_HELI_ORIENTATION (reVC Script7.cpp:262)
	lua_register(L, "clear_heli_orientation", clear_heli_orientation); // 2054 0x04D1 COMMAND_CLEAR_HELI_ORIENTATION (reVC Script7.cpp:275)
	lua_register(L, "set_char_drowns_in_water", set_char_drowns_in_water); // 2055 0x04D8 COMMAND_SET_CHAR_DROWNS_IN_WATER (reVC Script7.cpp:326)
	lua_register(L, "get_char_armour", get_char_armour); // 2056 0x04DD COMMAND_GET_CHAR_ARMOUR (reVC Script7.cpp:356)
	lua_register(L, "locate_object_2d", locate_object_2d); // 2057 0x04E5 COMMAND_LOCATE_OBJECT_2D (reVC Script7.cpp:411)
	lua_register(L, "locate_object_3d", locate_object_3d); // 2058 0x04E6 COMMAND_LOCATE_OBJECT_3D (reVC Script5.cpp:740)
	lua_register(L, "is_object_in_area_3d", is_object_in_area_3d); // 2059 0x04EA COMMAND_IS_OBJECT_IN_AREA_3D (reVC Script5.cpp:1295)
	lua_register(L, "add_set_piece", add_set_piece); // 2060 0x04F8 COMMAND_ADD_SET_PIECE (reVC Script7.cpp:552)
	lua_register(L, "get_wheelie_stats", get_wheelie_stats); // 2061 0x04FC COMMAND_GET_WHEELIE_STATS (reVC Script7.cpp:574)
	lua_register(L, "switch_lift_camera", switch_lift_camera); // 2062 0x0507 COMMAND_SWITCH_LIFT_CAMERA (reVC Script7.cpp:665)
	lua_register(L, "create_locked_property_pickup", create_locked_property_pickup); // 2063 0x0517 COMMAND_CREATE_LOCKED_PROPERTY_PICKUP (reVC Script7.cpp:760)
	lua_register(L, "has_glass_been_shattered_nearby", has_glass_been_shattered_nearby); // 2064 0x0523 COMMAND_HAS_GLASS_BEEN_SHATTERED_NEARBY (reVC Script7.cpp:835)
	lua_register(L, "attach_cutscene_object_to_component", attach_cutscene_object_to_component); // 2065 0x0525 COMMAND_ATTACH_CUTSCENE_OBJECT_TO_COMPONENT (reVC Script7.cpp:852)
	lua_register(L, "add_money_spent_on_weapons", add_money_spent_on_weapons); // 2066 0x0528 COMMAND_ADD_MONEY_SPENT_ON_WEAPONS (reVC Script7.cpp:874)
	lua_register(L, "set_player_drunkenness", set_player_drunkenness); // 2067 0x052C COMMAND_SET_PLAYER_DRUNKENNESS (reVC Script7.cpp:893)
	lua_register(L, "add_stores_knocked_off", add_stores_knocked_off); // 2068 0x0531 COMMAND_ADD_STORES_KNOCKED_OFF (reVC Script7.cpp:907)
	lua_register(L, "add_pizzas_delivered", add_pizzas_delivered); // 2069 0x0534 COMMAND_ADD_PIZZAS_DELIVERED (reVC Script7.cpp:916)
	lua_register(L, "add_ice_creams_sold", add_ice_creams_sold); // 2070 0x0536 COMMAND_ADD_ICE_CREAMS_SOLD (reVC Script7.cpp:921)
	lua_register(L, "set_char_in_players_group_can_fight", set_char_in_players_group_can_fight); // 2071 0x053C COMMAND_SET_CHAR_IN_PLAYERS_GROUP_CAN_FIGHT (reVC Script7.cpp:930)
	lua_register(L, "get_random_car_of_type_in_area_no_save", get_random_car_of_type_in_area_no_save); // 2072 0x053E COMMAND_GET_RANDOM_CAR_OF_TYPE_IN_AREA_NO_SAVE (reVC Script7.cpp:946)
	lua_register(L, "set_player_auto_aim", set_player_auto_aim); // 2073 0x0540 COMMAND_SET_PLAYER_AUTO_AIM (reVC Script7.cpp:981)
	lua_register(L, "add_blood_ring_kills", add_blood_ring_kills); // 2074 0x0543 COMMAND_ADD_BLOOD_RING_KILLS (reVC Script7.cpp:1007)
	lua_register(L, "set_longest_time_in_blood_ring", set_longest_time_in_blood_ring); // 2075 0x0544 COMMAND_SET_LONGEST_TIME_IN_BLOOD_RING (reVC Script7.cpp:1011)
	lua_register(L, "remove_everything_for_huge_cutscene", remove_everything_for_huge_cutscene); // 2076 0x0545 COMMAND_REMOVE_EVERYTHING_FOR_HUGE_CUTSCENE (reVC Script7.cpp:1015)
	lua_register(L, "is_player_touching_vehicle", is_player_touching_vehicle); // 2077 0x0546 COMMAND_IS_PLAYER_TOUCHING_VEHICLE (reVC Script7.cpp:1020)
	lua_register(L, "check_for_ped_model_around_player", check_for_ped_model_around_player); // 2078 0x0548 COMMAND_CHECK_FOR_PED_MODEL_AROUND_PLAYER (reVC Script7.cpp:1033)
	lua_register(L, "attach_cutscene_object_to_vehicle", attach_cutscene_object_to_vehicle); // 2079 0x054B COMMAND_ATTACH_CUTSCENE_OBJECT_TO_VEHICLE (reVC Script7.cpp:1071)
	lua_register(L, "set_tonights_event", set_tonights_event); // 2080 0x054D COMMAND_SET_TONIGHTS_EVENT (reVC Script7.cpp:1085)
	lua_register(L, "freeze_object_position", freeze_object_position); // 2081 0x0550 COMMAND_FREEZE_OBJECT_POSITION (reVC Script7.cpp:1102)
	lua_register(L, "set_player_has_met_debbie_harry", set_player_has_met_debbie_harry); // 2082 0x0551 COMMAND_SET_PLAYER_HAS_MET_DEBBIE_HARRY (reVC Script7.cpp:1111)
	lua_register(L, "set_up_taxi_shortcut", set_up_taxi_shortcut); // 2083 0x0556 COMMAND_SET_UP_TAXI_SHORTCUT (reVC Script7.cpp:1126)
	lua_register(L, "clear_taxi_shortcut", clear_taxi_shortcut); // 2084 0x0557 COMMAND_CLEAR_TAXI_SHORTCUT (reVC Script7.cpp:1134)
	lua_register(L, "add_porn_leaflet_to_rubbish", add_porn_leaflet_to_rubbish); // 2085 0x055A COMMAND_ADD_PORN_LEAFLET_TO_RUBBISH (reVC Script7.cpp:1139)
	lua_register(L, "make_player_fire_proof", make_player_fire_proof); // 2086 0x055D COMMAND_MAKE_PLAYER_FIRE_PROOF (reVC Script7.cpp:1155)
	lua_register(L, "create_random_char_as_driver", create_random_char_as_driver); // 2087 0x0560 COMMAND_CREATE_RANDOM_CHAR_AS_DRIVER (reVC Script7.cpp:1178)
	lua_register(L, "set_char_ignore_threats_behind_objects", set_char_ignore_threats_behind_objects); // 2088 0x0562 COMMAND_SET_CHAR_IGNORE_THREATS_BEHIND_OBJECTS (reVC Script7.cpp:1239)
	lua_register(L, "make_heli_come_crashing_down", make_heli_come_crashing_down); // 2089 0x0564 COMMAND_MAKE_HELI_COME_CRASHING_DOWN (reVC Script7.cpp:1266)
	lua_register(L, "is_char_stuck", is_char_stuck); // 2090 0x0571 COMMAND_IS_CHAR_STUCK (reVC Script7.cpp:1347)
	lua_register(L, "set_all_taxis_have_nitro", set_all_taxis_have_nitro); // 2091 0x0572 COMMAND_SET_ALL_TAXIS_HAVE_NITRO (reVC Script7.cpp:1355)
	lua_register(L, "register_vigilante_level", register_vigilante_level); // 2092 0x0578 COMMAND_REGISTER_VIGILANTE_LEVEL (reVC Script8.cpp:35)
	lua_register(L, "set_maximum_number_of_cars_in_garage", set_maximum_number_of_cars_in_garage); // 2093 0x057A COMMAND_SET_MAXIMUM_NUMBER_OF_CARS_IN_GARAGE (reVC Script8.cpp:63)
	lua_register(L, "wanted_stars_are_flashing", wanted_stars_are_flashing); // 2094 0x057B COMMAND_WANTED_STARS_ARE_FLASHING (reVC Script8.cpp:67)
	lua_register(L, "get_bus_fares_collected_by_player", get_bus_fares_collected_by_player); // 2095 0x057F COMMAND_GET_BUS_FARES_COLLECTED_BY_PLAYER (reVC Script8.cpp:89)
	lua_register(L, "register_best_position", register_best_position); // 2096 0x0582 COMMAND_REGISTER_BEST_POSITION (reVC Script8.cpp:126)
	lua_register(L, "is_player_in_info_zone", is_player_in_info_zone); // 2097 0x0583 COMMAND_IS_PLAYER_IN_INFO_ZONE (reVC Script8.cpp:130)
	lua_register(L, "has_char_attempted_attractor", has_char_attempted_attractor); // 2098 0x0586 COMMAND_HAS_CHAR_ATTEMPTED_ATTRACTOR (reVC Script8.cpp:155)
	lua_register(L, "set_load_collision_for_char_flag", set_load_collision_for_char_flag); // 2099 0x0588 COMMAND_SET_LOAD_COLLISION_FOR_CHAR_FLAG (reVC Script8.cpp:186)
	lua_register(L, "has_char_bought_ice_cream", has_char_bought_ice_cream); // 2100 0x058B COMMAND_HAS_CHAR_BOUGHT_ICE_CREAM (reVC Script8.cpp:216)
	lua_register(L, "get_progress_percentage", get_progress_percentage); // 2101 0x058C COMMAND_GET_PROGRESS_PERCENTAGE (reVC Script8.cpp:224)
	lua_register(L, "set_shortcut_pickup_point", set_shortcut_pickup_point); // 2102 0x058D COMMAND_SET_SHORTCUT_PICKUP_POINT (reVC Script8.cpp:228)
	lua_register(L, "get_random_ice_cream_customer_in_area", get_random_ice_cream_customer_in_area); // 2103 0x058F COMMAND_GET_RANDOM_ICE_CREAM_CUSTOMER_IN_AREA (reVC Script8.cpp:240)
	lua_register(L, "set_char_frightened_in_jacked_car", set_char_frightened_in_jacked_car); // 2104 0x0593 COMMAND_SET_CHAR_FRIGHTENED_IN_JACKED_CAR (reVC Script8.cpp:314)
	lua_register(L, "is_player_in_shortcut_taxi", is_player_in_shortcut_taxi); // 2105 0x0596 COMMAND_IS_PLAYER_IN_SHORTCUT_TAXI (reVC Script8.cpp:336)
	lua_register(L, "register_fire_level", register_fire_level); // 2106 0x0599 COMMAND_REGISTER_FIRE_LEVEL (reVC Script8.cpp:370)


	// === COMMUNITY ADDRESSES + HEADLIGHTS (20.11) ===
	lua_register(L, "patch_headlights", patch_headlights); // 2107 GTAForums 0x58CD4E
	lua_register(L, "set_headlights_on", set_headlights_on); // 2108 обёртка: set headlights on
	lua_register(L, "set_headlights_off", set_headlights_off); // 2109 обёртка: set headlights off
	lua_register(L, "is_headlights_on", is_headlights_on); // 2110 обёртка: is headlights on
	lua_register(L, "get_m_timepassingmeby_faster_time", get_m_timepassingmeby_faster_time); // 2111 0xA10B87 TIMEPASSINGMEBY faster time
	lua_register(L, "set_m_timepassingmeby_faster_time", set_m_timepassingmeby_faster_time); // 2112 обёртка: set m timepassingmeby faster time
	lua_register(L, "get_m_youwonttakemealive_wanted_level", get_m_youwonttakemealive_wanted_level); // 2113 0x4ABF4C YOUWONTTAKEMEALIVE set wanted level
	lua_register(L, "set_m_youwonttakemealive_wanted_level", set_m_youwonttakemealive_wanted_level); // 2114 обёртка: set m youwonttakemealive wanted level
	lua_register(L, "get_m_leavemealone_wanted_level", get_m_leavemealone_wanted_level); // 2115 0x4ABFC4 LEAVEMEALONE set wanted level
	lua_register(L, "set_m_leavemealone_wanted_level", set_m_leavemealone_wanted_level); // 2116 обёртка: set m leavemealone wanted level
	lua_register(L, "get_m_apleasantday_weather", get_m_apleasantday_weather); // 2117 0x4AC009 APLEASANTDAY set weather
	lua_register(L, "set_m_apleasantday_weather", set_m_apleasantday_weather); // 2118 обёртка: set m apleasantday weather
	lua_register(L, "get_m_alovelyday_weather", get_m_alovelyday_weather); // 2119 0x4AC04E ALOVELYDAY set weather
	lua_register(L, "set_m_alovelyday_weather", set_m_alovelyday_weather); // 2120 обёртка: set m alovelyday weather
	lua_register(L, "get_m_abitdrieg_weather", get_m_abitdrieg_weather); // 2121 0x4AC099 ABITDRIEG set weather
	lua_register(L, "set_m_abitdrieg_weather", set_m_abitdrieg_weather); // 2122 обёртка: set m abitdrieg weather
	lua_register(L, "get_m_catsanddogs_weather", get_m_catsanddogs_weather); // 2123 0x4AC0DE CATSANDDOGS set weather
	lua_register(L, "set_m_catsanddogs_weather", set_m_catsanddogs_weather); // 2124 обёртка: set m catsanddogs weather
	lua_register(L, "get_m_cantseeathing_weather", get_m_cantseeathing_weather); // 2125 0x4AC129 CANTSEEATHING set weather
	lua_register(L, "set_m_cantseeathing_weather", set_m_cantseeathing_weather); // 2126 обёртка: set m cantseeathing weather
	lua_register(L, "get_m_panzer_rhino_model", get_m_panzer_rhino_model); // 2127 0x4AC14B PANZER Rhino model
	lua_register(L, "set_m_panzer_rhino_model", set_m_panzer_rhino_model); // 2128 обёртка: set m panzer rhino model
	lua_register(L, "get_m_travelinstyle_bloodring_a_model", get_m_travelinstyle_bloodring_a_model); // 2129 0x4AC946 TRAVELINSTYLE Bloodring A model
	lua_register(L, "set_m_travelinstyle_bloodring_a_model", set_m_travelinstyle_bloodring_a_model); // 2130 обёртка: set m travelinstyle bloodring a model
	lua_register(L, "get_m_thelastride_romero_hearse_model", get_m_thelastride_romero_hearse_model); // 2131 0x4AC976 THELASTRIDE Romero's Hearse model
	lua_register(L, "set_m_thelastride_romero_hearse_model", set_m_thelastride_romero_hearse_model); // 2132 обёртка: set m thelastride romero hearse model
	lua_register(L, "get_m_rockandrollcar_love_fist_model", get_m_rockandrollcar_love_fist_model); // 2133 0x4AC9A6 ROCKANDROLLCAR Love Fist model
	lua_register(L, "set_m_rockandrollcar_love_fist_model", set_m_rockandrollcar_love_fist_model); // 2134 обёртка: set m rockandrollcar love fist model
	lua_register(L, "get_m_rubbishcar_trashmaster_model", get_m_rubbishcar_trashmaster_model); // 2135 0x4AC9D6 RUBBISHCAR Trashmaster model
	lua_register(L, "set_m_rubbishcar_trashmaster_model", set_m_rubbishcar_trashmaster_model); // 2136 обёртка: set m rubbishcar trashmaster model
	lua_register(L, "get_m_gettherefast_sabre_turbo_model", get_m_gettherefast_sabre_turbo_model); // 2137 0x4ACA06 GETTHEREFAST Sabre Turbo model
	lua_register(L, "set_m_gettherefast_sabre_turbo_model", set_m_gettherefast_sabre_turbo_model); // 2138 обёртка: set m gettherefast sabre turbo model
	lua_register(L, "get_m_betterthanwalking_caddy_model", get_m_betterthanwalking_caddy_model); // 2139 0x4ACA36 BETTERTHANWALKING Caddy model
	lua_register(L, "set_m_betterthanwalking_caddy_model", set_m_betterthanwalking_caddy_model); // 2140 обёртка: set m betterthanwalking caddy model
	lua_register(L, "get_m_gettherequickly_bloodring_b_model", get_m_gettherequickly_bloodring_b_model); // 2141 0x4ACDD6 GETTHEREQUICKLY Bloodring B model
	lua_register(L, "set_m_gettherequickly_bloodring_b_model", set_m_gettherequickly_bloodring_b_model); // 2142 обёртка: set m gettherequickly bloodring b model
	lua_register(L, "get_m_getthereveryfastindeed_hotring_a_model", get_m_getthereveryfastindeed_hotring_a_model); // 2143 0x4ACE06 GETTHEREVERYFASTINDEED Hotring A model
	lua_register(L, "set_m_getthereveryfastindeed_hotring_a_model", set_m_getthereveryfastindeed_hotring_a_model); // 2144 обёртка: set m getthereveryfastindeed hotring a model
	lua_register(L, "get_m_getthereamazinglyfast_hotring_b_model", get_m_getthereamazinglyfast_hotring_b_model); // 2145 0x4ACE36 GETTHEREAMAZINGLYFAST Hotring B model
	lua_register(L, "set_m_getthereamazinglyfast_hotring_b_model", set_m_getthereamazinglyfast_hotring_b_model); // 2146 обёртка: set m getthereamazinglyfast hotring b model
	lua_register(L, "get_m_nuttertools_chainsaw_model", get_m_nuttertools_chainsaw_model); // 2147 0x4AEAEF NUTTERTOOLS Chainsaw model
	lua_register(L, "set_m_nuttertools_chainsaw_model", set_m_nuttertools_chainsaw_model); // 2148 обёртка: set m nuttertools chainsaw model
	lua_register(L, "get_m_nuttertools_chainsaw_weapon_note", get_m_nuttertools_chainsaw_weapon_note); // 2149 0x4AEB77 NUTTERTOOLS Chainsaw weapon number (NOTE: Model needs to be
	lua_register(L, "set_m_nuttertools_chainsaw_weapon_note", set_m_nuttertools_chainsaw_weapon_note); // 2150 обёртка: set m nuttertools chainsaw weapon note
	lua_register(L, "get_m_nuttertools_m4_weapon_note", get_m_nuttertools_m4_weapon_note); // 2151 0x4AEBD4 NUTTERTOOLS M4 weapon number (NOTE: Model needs to be loaded
	lua_register(L, "set_m_nuttertools_m4_weapon_note", set_m_nuttertools_m4_weapon_note); // 2152 обёртка: set m nuttertools m4 weapon note
	lua_register(L, "get_m_nuttertools_sniper_weapon_note", get_m_nuttertools_sniper_weapon_note); // 2153 0x4AEBE6 NUTTERTOOLS .308 Sniper weapon number (NOTE: Model needs to
	lua_register(L, "set_m_nuttertools_sniper_weapon_note", set_m_nuttertools_sniper_weapon_note); // 2154 обёртка: set m nuttertools sniper weapon note
	lua_register(L, "get_m_nuttertools_minigun_weapon_note", get_m_nuttertools_minigun_weapon_note); // 2155 0x4AEBFB NUTTERTOOLS Minigun weapon number (NOTE: Model needs to be l
	lua_register(L, "set_m_nuttertools_minigun_weapon_note", set_m_nuttertools_minigun_weapon_note); // 2156 обёртка: set m nuttertools minigun weapon note
	lua_register(L, "get_m_professionaltools_katana_model", get_m_professionaltools_katana_model); // 2157 0x4AEC8F PROFESSIONALTOOLS Katana model
	lua_register(L, "set_m_professionaltools_katana_model", set_m_professionaltools_katana_model); // 2158 обёртка: set m professionaltools katana model
	lua_register(L, "get_m_professionaltools_katana_weapon_note", get_m_professionaltools_katana_weapon_note); // 2159 0x4AED17 PROFESSIONALTOOLS Katana weapon number (NOTE: Model needs to
	lua_register(L, "set_m_professionaltools_katana_weapon_note", set_m_professionaltools_katana_weapon_note); // 2160 обёртка: set m professionaltools katana weapon note
	lua_register(L, "get_m_professionaltools_m4_weapon_note", get_m_professionaltools_m4_weapon_note); // 2161 0x4AED74 PROFESSIONALTOOLS M4 weapon number (NOTE: Model needs to be
	lua_register(L, "set_m_professionaltools_m4_weapon_note", set_m_professionaltools_m4_weapon_note); // 2162 обёртка: set m professionaltools m4 weapon note
	lua_register(L, "get_m_professionaltools_sniper_weapon_note", get_m_professionaltools_sniper_weapon_note); // 2163 0x4AED86 PROFESSIONALTOOLS .308 Sniper weapon number (NOTE: Model nee
	lua_register(L, "set_m_professionaltools_sniper_weapon_note", set_m_professionaltools_sniper_weapon_note); // 2164 обёртка: set m professionaltools sniper weapon note
	lua_register(L, "get_m_professionaltools_rocket_launcher_weapon", get_m_professionaltools_rocket_launcher_weapon); // 2165 0x4AED98 PROFESSIONALTOOLS Rocket Launcher weapon number (NOTE: Model
	lua_register(L, "set_m_professionaltools_rocket_launcher_weapon", set_m_professionaltools_rocket_launcher_weapon); // 2166 обёртка: set m professionaltools rocket launcher weapon
	lua_register(L, "get_m_thugstools_brass_knuckles_model", get_m_thugstools_brass_knuckles_model); // 2167 0x4AEE1F THUGSTOOLS Brass Knuckles model
	lua_register(L, "set_m_thugstools_brass_knuckles_model", set_m_thugstools_brass_knuckles_model); // 2168 обёртка: set m thugstools brass knuckles model
	lua_register(L, "get_m_thugstools_brass_knuckles_weapon", get_m_thugstools_brass_knuckles_weapon); // 2169 0x4AEEA7 THUGSTOOLS Brass Knuckles weapon number (NOTE: Model needs t
	lua_register(L, "set_m_thugstools_brass_knuckles_weapon", set_m_thugstools_brass_knuckles_weapon); // 2170 обёртка: set m thugstools brass knuckles weapon
	lua_register(L, "get_m_thugstools_tec_weapon_note", get_m_thugstools_tec_weapon_note); // 2171 0x4AEF04 THUGSTOOLS Tec 9 weapon number (NOTE: Model needs to be load
	lua_register(L, "set_m_thugstools_tec_weapon_note", set_m_thugstools_tec_weapon_note); // 2172 обёртка: set m thugstools tec weapon note
	lua_register(L, "get_m_thugstools_kruger_weapon_note", get_m_thugstools_kruger_weapon_note); // 2173 0x4AEF16 THUGSTOOLS Kruger weapon number (NOTE: Model needs to be loa
	lua_register(L, "set_m_thugstools_kruger_weapon_note", set_m_thugstools_kruger_weapon_note); // 2174 обёртка: set m thugstools kruger weapon note
	lua_register(L, "get_m_thugstools_sniper_rifle_weapon", get_m_thugstools_sniper_rifle_weapon); // 2175 0x4AEF28 THUGSTOOLS Sniper Rifle weapon number (NOTE: Model needs to
	lua_register(L, "set_m_thugstools_sniper_rifle_weapon", set_m_thugstools_sniper_rifle_weapon); // 2176 обёртка: set m thugstools sniper rifle weapon
	lua_register(L, "get_m_thugstools_flame_thrower_weapon", get_m_thugstools_flame_thrower_weapon); // 2177 0x4AEF38 THUGSTOOLS Flame Thrower weapon number (NOTE: Model needs to
	lua_register(L, "set_m_thugstools_flame_thrower_weapon", set_m_thugstools_flame_thrower_weapon); // 2178 обёртка: set m thugstools flame thrower weapon
	lua_register(L, "get_m_deepfriedmarsbars_programmer_tommy_body", get_m_deepfriedmarsbars_programmer_tommy_body); // 2179 0x68E058 DEEPFRIEDMARSBARS/PROGRAMMER Tommy's body width
	lua_register(L, "set_m_deepfriedmarsbars_programmer_tommy_body", set_m_deepfriedmarsbars_programmer_tommy_body); // 2180 обёртка: set m deepfriedmarsbars programmer tommy body
	lua_register(L, "get_m_onspeed_game_speed", get_m_onspeed_game_speed); // 2181 0x68F1F0 ONSPEED set game speed
	lua_register(L, "set_m_onspeed_game_speed", set_m_onspeed_game_speed); // 2182 обёртка: set m onspeed game speed
	lua_register(L, "get_m_onspeed_game_speed_multiplier", get_m_onspeed_game_speed_multiplier); // 2183 0x68F1F4 ONSPEED game speed multiplier
	lua_register(L, "set_m_onspeed_game_speed_multiplier", set_m_onspeed_game_speed_multiplier); // 2184 обёртка: set m onspeed game speed multiplier
	lua_register(L, "get_m_booooooring_game_speed", get_m_booooooring_game_speed); // 2185 0x68F1F8 BOOOOOORING set game speed
	lua_register(L, "set_m_booooooring_game_speed", set_m_booooooring_game_speed); // 2186 обёртка: set m booooooring game speed
	lua_register(L, "get_m_booooooring_game_speed_multiplier", get_m_booooooring_game_speed_multiplier); // 2187 0x68F1FC BOOOOOORING game speed multiplier
	lua_register(L, "set_m_booooooring_game_speed_multiplier", set_m_booooooring_game_speed_multiplier); // 2188 обёртка: set m booooooring game speed multiplier
	lua_register(L, "get_m_chasestat", get_m_chasestat); // 2189 0x97F2C4 CHASESTAT cheat switch
	lua_register(L, "set_m_chasestat", set_m_chasestat); // 2190 обёртка: set m chasestat
	lua_register(L, "get_m_ourgodgivenrighttobeararms", get_m_ourgodgivenrighttobeararms); // 2191 0xA10AB3 OURGODGIVENRIGHTTOBEARARMS cheat switch
	lua_register(L, "set_m_ourgodgivenrighttobeararms", set_m_ourgodgivenrighttobeararms); // 2192 обёртка: set m ourgodgivenrighttobeararms
	lua_register(L, "get_m_greenlight", get_m_greenlight); // 2193 0xA10ADC GREENLIGHT cheat switch
	lua_register(L, "set_m_greenlight", set_m_greenlight); // 2194 обёртка: set m greenlight
	lua_register(L, "get_m_gripiseverything", get_m_gripiseverything); // 2195 0xA10B0F GRIPISEVERYTHING cheat switch
	lua_register(L, "set_m_gripiseverything", set_m_gripiseverything); // 2196 обёртка: set m gripiseverything
	lua_register(L, "get_m_airship", get_m_airship); // 2197 0xA10B11 AIRSHIP cheat switch
	lua_register(L, "set_m_airship", set_m_airship); // 2198 обёртка: set m airship
	lua_register(L, "get_m_certaindeath", get_m_certaindeath); // 2199 0xA10B23 CERTAINDEATH cheat switch
	lua_register(L, "set_m_certaindeath", set_m_certaindeath); // 2200 обёртка: set m certaindeath
	lua_register(L, "get_m_ahairdresserscar", get_m_ahairdresserscar); // 2201 0xA10B26 AHAIRDRESSERSCAR cheat switch
	lua_register(L, "set_m_ahairdresserscar", set_m_ahairdresserscar); // 2202 обёртка: set m ahairdresserscar
	lua_register(L, "get_m_comeflywithme", get_m_comeflywithme); // 2203 0xA10B28 COMEFLYWITHME cheat switch
	lua_register(L, "set_m_comeflywithme", set_m_comeflywithme); // 2204 обёртка: set m comeflywithme
	lua_register(L, "get_m_used_flag", get_m_used_flag); // 2205 0xA10B2E Cheat is used flag
	lua_register(L, "set_m_used_flag", set_m_used_flag); // 2206 обёртка: set m used flag
	lua_register(L, "get_m_wheelsareallineed", get_m_wheelsareallineed); // 2207 0xA10B30 WHEELSAREALLINEED cheat switch
	lua_register(L, "set_m_wheelsareallineed", set_m_wheelsareallineed); // 2208 обёртка: set m wheelsareallineed
	lua_register(L, "get_m_miamitraffic", get_m_miamitraffic); // 2209 0xA10B47 MIAMITRAFFIC cheat switch
	lua_register(L, "set_m_miamitraffic", set_m_miamitraffic); // 2210 обёртка: set m miamitraffic
	lua_register(L, "get_m_fannymagnet", get_m_fannymagnet); // 2211 0xA10B5F FANNYMAGNET cheat switch
	lua_register(L, "set_m_fannymagnet", set_m_fannymagnet); // 2212 обёртка: set m fannymagnet
	lua_register(L, "get_m_seaways", get_m_seaways); // 2213 0xA10B81 SEAWAYS cheat switch
	lua_register(L, "set_m_seaways", set_m_seaways); // 2214 обёртка: set m seaways
	lua_register(L, "get_m_iwantitpaintedblack", get_m_iwantitpaintedblack); // 2215 0xA10B82 IWANTITPAINTEDBLACK cheat switch
	lua_register(L, "set_m_iwantitpaintedblack", set_m_iwantitpaintedblack); // 2216 обёртка: set m iwantitpaintedblack
	lua_register(L, "get_m_lifeispassingmeby", get_m_lifeispassingmeby); // 2217 0xA10B87 LIFEISPASSINGMEBY cheat
	lua_register(L, "set_m_lifeispassingmeby", set_m_lifeispassingmeby); // 2218 обёртка: set m lifeispassingmeby
	lua_register(L, "get_m_opcode_de_check_if", get_m_opcode_de_check_if); // 2219 0x456046 Opcode 02DE check if player is in a Taxi
	lua_register(L, "set_m_opcode_de_check_if", set_m_opcode_de_check_if); // 2220 обёртка: set m opcode de check if
	lua_register(L, "get_m_cash_when_entering_a", get_m_cash_when_entering_a); // 2221 0x5B8AB6 Cash when entering a taxi (set to 12 by default)
	lua_register(L, "set_m_cash_when_entering_a", set_m_cash_when_entering_a); // 2222 обёртка: set m cash when entering a
	lua_register(L, "get_m_kaufman_cabs_radio_flag", get_m_kaufman_cabs_radio_flag); // 2223 0xA10B7F Kaufman Cabs' radio flag
	lua_register(L, "set_m_kaufman_cabs_radio_flag", set_m_kaufman_cabs_radio_flag); // 2224 обёртка: set m kaufman cabs radio flag
	lua_register(L, "get_m_kaufman_cabs_spawn_note", get_m_kaufman_cabs_spawn_note); // 2225 0x42AEAC Kaufman Cabs spawn (NOTE: Model needs to be loaded through 0
	lua_register(L, "set_m_kaufman_cabs_spawn_note", set_m_kaufman_cabs_spawn_note); // 2226 обёртка: set m kaufman cabs spawn note
	lua_register(L, "get_m_kaufman_cabs_model_load", get_m_kaufman_cabs_model_load); // 2227 0x42AFB3 Kaufman Cabs model load
	lua_register(L, "set_m_kaufman_cabs_model_load", set_m_kaufman_cabs_model_load); // 2228 обёртка: set m kaufman cabs model load
	lua_register(L, "get_m_blue_fade_in_color", get_m_blue_fade_in_color); // 2229 0x42B219 Blue fade in color
	lua_register(L, "set_m_blue_fade_in_color", set_m_blue_fade_in_color); // 2230 обёртка: set m blue fade in color
	lua_register(L, "get_m_taxi_text_duration", get_m_taxi_text_duration); // 2231 0x42B24E TAXI text duration
	lua_register(L, "set_m_taxi_text_duration", set_m_taxi_text_duration); // 2232 обёртка: set m taxi text duration
	lua_register(L, "get_m_x_coord_taxi_destination", get_m_x_coord_taxi_destination); // 2233 0x94DD94 X-coord taxi destination (updated by opcode 058E)
	lua_register(L, "set_m_x_coord_taxi_destination", set_m_x_coord_taxi_destination); // 2234 обёртка: set m x coord taxi destination
	lua_register(L, "get_m_taxi_spawn", get_m_taxi_spawn); // 2235 0x94EEB8 Set taxi to spawn
	lua_register(L, "set_m_taxi_spawn", set_m_taxi_spawn); // 2236 обёртка: set m taxi spawn
	lua_register(L, "get_m_angle_taxi_destination", get_m_angle_taxi_destination); // 2237 0xA0D378 Angle taxi destination
	lua_register(L, "set_m_angle_taxi_destination", set_m_angle_taxi_destination); // 2238 обёртка: set m angle taxi destination
	lua_register(L, "get_m_activate_mission_restart_taxi", get_m_activate_mission_restart_taxi); // 2239 0xA10B6D Activate mission restart taxi
	lua_register(L, "set_m_activate_mission_restart_taxi", set_m_activate_mission_restart_taxi); // 2240 обёртка: set m activate mission restart taxi
	lua_register(L, "get_m_camera_move_when_entering", get_m_camera_move_when_entering); // 2241 0x46B389 camera move when entering/exiting car
	lua_register(L, "set_m_camera_move_when_entering", set_m_camera_move_when_entering); // 2242 обёртка: set m camera move when entering
	lua_register(L, "get_m_same", get_m_same); // 2243 0x46B58F same [5]
	lua_register(L, "set_m_same", set_m_same); // 2244 обёртка: set m same
	lua_register(L, "get_m_camera_distance_from_only", get_m_camera_distance_from_only); // 2245 0x68AE2E camera distance from player (set only lower, higher bugs the
	lua_register(L, "set_m_camera_distance_from_only", set_m_camera_distance_from_only); // 2246 обёртка: set m camera distance from only
	lua_register(L, "get_m_cinema_top_border_height", get_m_cinema_top_border_height); // 2247 0x68AEE0 cinema top border height
	lua_register(L, "set_m_cinema_top_border_height", set_m_cinema_top_border_height); // 2248 обёртка: set m cinema top border height
	lua_register(L, "get_m_cinema_bottom_border_height", get_m_cinema_bottom_border_height); // 2249 0x68AEE4 cinema bottom border height
	lua_register(L, "set_m_cinema_bottom_border_height", set_m_cinema_bottom_border_height); // 2250 обёртка: set m cinema bottom border height
	lua_register(L, "get_m_controls_radar_map", get_m_controls_radar_map); // 2251 0x68FD02 controls radar map [6]
	lua_register(L, "set_m_controls_radar_map", set_m_controls_radar_map); // 2252 обёртка: set m controls radar map
	lua_register(L, "get_m_radar_width", get_m_radar_width); // 2253 0x68FD16 radar width
	lua_register(L, "set_m_radar_width", set_m_radar_width); // 2254 обёртка: set m radar width
	lua_register(L, "get_m_only_radar_ring_visible", get_m_only_radar_ring_visible); // 2255 0x68FD17 only radar ring visible [6]
	lua_register(L, "set_m_only_radar_ring_visible", set_m_only_radar_ring_visible); // 2256 обёртка: set m only radar ring visible
	lua_register(L, "get_m_radar_size_xy", get_m_radar_size_xy); // 2257 0x68FD28 Radar Size (XY)
	lua_register(L, "set_m_radar_size_xy", set_m_radar_size_xy); // 2258 обёртка: set m radar size xy
	lua_register(L, "get_m_radar_x_position", get_m_radar_x_position); // 2259 0x68FD2C Radar X Position
	lua_register(L, "set_m_radar_x_position", set_m_radar_x_position); // 2260 обёртка: set m radar x position
	lua_register(L, "get_m_radar_y_position", get_m_radar_y_position); // 2261 0x68FD34 Radar Y Position
	lua_register(L, "set_m_radar_y_position", set_m_radar_y_position); // 2262 обёртка: set m radar y position
	lua_register(L, "get_m_radar_shape", get_m_radar_shape); // 2263 0x68FD38 Radar Shape
	lua_register(L, "set_m_radar_shape", set_m_radar_shape); // 2264 обёртка: set m radar shape
	lua_register(L, "get_m_addr_68FD3C", get_m_addr_68FD3C); // 2265 0x68FD3C Radar Shape
	lua_register(L, "set_m_addr_68FD3C", set_m_addr_68FD3C); // 2266 обёртка: set m addr 68FD3C
	lua_register(L, "get_m_ambient_lighting", get_m_ambient_lighting); // 2267 0x698A8C ambient lighting
	lua_register(L, "set_m_ambient_lighting", set_m_ambient_lighting); // 2268 обёртка: set m ambient lighting
	lua_register(L, "get_m_camera_x_position", get_m_camera_x_position); // 2269 0x7E46B8 camera X position
	lua_register(L, "set_m_camera_x_position", set_m_camera_x_position); // 2270 обёртка: set m camera x position
	lua_register(L, "get_m_camera_y_position", get_m_camera_y_position); // 2271 0x7E46BC camera Y position
	lua_register(L, "set_m_camera_y_position", set_m_camera_y_position); // 2272 обёртка: set m camera y position
	lua_register(L, "get_m_camera_z_position", get_m_camera_z_position); // 2273 0x7E46C0 camera Z position
	lua_register(L, "set_m_camera_z_position", set_m_camera_z_position); // 2274 обёртка: set m camera z position
	lua_register(L, "get_m_wide_screen_borders", get_m_wide_screen_borders); // 2275 0x7E46F5 wide screen borders
	lua_register(L, "set_m_wide_screen_borders", set_m_wide_screen_borders); // 2276 обёртка: set m wide screen borders
	lua_register(L, "get_m_car_camera", get_m_car_camera); // 2277 0x7E4764 car camera
	lua_register(L, "set_m_car_camera", set_m_car_camera); // 2278 обёртка: set m car camera
	lua_register(L, "get_m_camera", get_m_camera); // 2279 0x7E47EC player camera
	lua_register(L, "set_m_camera", set_m_camera); // 2280 обёртка: set m camera
	lua_register(L, "get_m_frame_limiter", get_m_frame_limiter); // 2281 0x869655 frame limiter
	lua_register(L, "set_m_frame_limiter", set_m_frame_limiter); // 2282 обёртка: set m frame limiter
	lua_register(L, "get_m_subtitles", get_m_subtitles); // 2283 0x869650 subtitles
	lua_register(L, "set_m_subtitles", set_m_subtitles); // 2284 обёртка: set m subtitles
	lua_register(L, "get_m_widescreen", get_m_widescreen); // 2285 0x869652 widescreen
	lua_register(L, "set_m_widescreen", set_m_widescreen); // 2286 обёртка: set m widescreen
	lua_register(L, "get_m_hud_display", get_m_hud_display); // 2287 0x86963A hud display
	lua_register(L, "set_m_hud_display", set_m_hud_display); // 2288 обёртка: set m hud display
	lua_register(L, "get_m_replay_display", get_m_replay_display); // 2289 0xA10AF6 replay display on/off
	lua_register(L, "set_m_replay_display", set_m_replay_display); // 2290 обёртка: set m replay display
	lua_register(L, "get_m_white_scanlines", get_m_white_scanlines); // 2291 0xA10B68 white scanlines
	lua_register(L, "set_m_white_scanlines", set_m_white_scanlines); // 2292 обёртка: set m white scanlines
	lua_register(L, "get_m_green_scanlines", get_m_green_scanlines); // 2293 0xA10B69 green scanlines
	lua_register(L, "set_m_green_scanlines", set_m_green_scanlines); // 2294 обёртка: set m green scanlines
	lua_register(L, "get_m_resolution_width", get_m_resolution_width); // 2295 0xA0FD04 resolution Width
	lua_register(L, "set_m_resolution_width", set_m_resolution_width); // 2296 обёртка: set m resolution width
	lua_register(L, "get_m_resolution_height", get_m_resolution_height); // 2297 0xA0FD08 resolution height
	lua_register(L, "set_m_resolution_height", set_m_resolution_height); // 2298 обёртка: set m resolution height
	lua_register(L, "get_m_color_bit", get_m_color_bit); // 2299 0xA0FD00 Color bit.
	lua_register(L, "set_m_color_bit", set_m_color_bit); // 2300 обёртка: set m color bit
	lua_register(L, "get_m_mouse_x_movement", get_m_mouse_x_movement); // 2301 0x936910 Mouse X movement
	lua_register(L, "set_m_mouse_x_movement", set_m_mouse_x_movement); // 2302 обёртка: set m mouse x movement
	lua_register(L, "get_m_mouse_y_movement", get_m_mouse_y_movement); // 2303 0x936914 Mouse Y movement
	lua_register(L, "set_m_mouse_y_movement", set_m_mouse_y_movement); // 2304 обёртка: set m mouse y movement
	lua_register(L, "get_m_total_stores_knock", get_m_total_stores_knock); // 2305 0x4CC102 Total number of stores to knock off (default is 15)
	lua_register(L, "set_m_total_stores_knock", set_m_total_stores_knock); // 2306 обёртка: set m total stores knock
	lua_register(L, "get_m_total_assassination_contracts_complete", get_m_total_assassination_contracts_complete); // 2307 0x4CC1CD Total number of assassination contracts to complete (default
	lua_register(L, "set_m_total_assassination_contracts_complete", set_m_total_assassination_contracts_complete); // 2308 обёртка: set m total assassination contracts complete
	lua_register(L, "get_m_police_helicopter_state", get_m_police_helicopter_state); // 2309 0xA10ADB Police helicopter state
	lua_register(L, "set_m_police_helicopter_state", set_m_police_helicopter_state); // 2310 обёртка: set m police helicopter state
	lua_register(L, "get_m_size_police_em_lights", get_m_size_police_em_lights); // 2311 0x69A633 size of Police EM lights
	lua_register(L, "set_m_size_police_em_lights", set_m_size_police_em_lights); // 2312 обёртка: set m size police em lights
	lua_register(L, "get_m_first_police_car", get_m_first_police_car); // 2313 0x426A21 first police car
	lua_register(L, "set_m_first_police_car", set_m_first_police_car); // 2314 обёртка: set m first police car
	lua_register(L, "get_m_second_police_car", get_m_second_police_car); // 2315 0x426987 second police car
	lua_register(L, "set_m_second_police_car", set_m_second_police_car); // 2316 обёртка: set m second police car
	lua_register(L, "get_m_enforcer", get_m_enforcer); // 2317 0x42697E Enforcer
	lua_register(L, "set_m_enforcer", set_m_enforcer); // 2318 обёртка: set m enforcer
	lua_register(L, "get_m_vicechee", get_m_vicechee); // 2319 0x4268B8 Vicechee 1
	lua_register(L, "set_m_vicechee", set_m_vicechee); // 2320 обёртка: set m vicechee
	lua_register(L, "get_m_addr_4268D3", get_m_addr_4268D3); // 2321 0x4268D3 Vicechee 2
	lua_register(L, "set_m_addr_4268D3", set_m_addr_4268D3); // 2322 обёртка: set m addr 4268D3
	lua_register(L, "get_m_addr_4268EE", get_m_addr_4268EE); // 2323 0x4268EE Vicechee 3
	lua_register(L, "set_m_addr_4268EE", set_m_addr_4268EE); // 2324 обёртка: set m addr 4268EE
	lua_register(L, "get_m_addr_426909", get_m_addr_426909); // 2325 0x426909 Vicechee 4
	lua_register(L, "set_m_addr_426909", set_m_addr_426909); // 2326 обёртка: set m addr 426909
	lua_register(L, "get_m_fbi_rancher", get_m_fbi_rancher); // 2327 0x4269BA FBI Rancher
	lua_register(L, "set_m_fbi_rancher", set_m_fbi_rancher); // 2328 обёртка: set m fbi rancher
	lua_register(L, "get_m_barracks_ol", get_m_barracks_ol); // 2329 0x426A0A Barracks Ol
	lua_register(L, "set_m_barracks_ol", set_m_barracks_ol); // 2330 обёртка: set m barracks ol


	// === ENGINE / LUA INTROSPECTION (21.11) ===
	lua_register(L, "get_lua_version", get_lua_version); // 2331 21.11: из папки «движок»
	lua_register(L, "lua_memory_used", lua_memory_used); // 2332 21.11: из папки «движок»
	lua_register(L, "lua_gc_collect", lua_gc_collect); // 2333 21.11: из папки «движок»
	lua_register(L, "lua_gc_stop", lua_gc_stop); // 2334 21.11: из папки «движок»
	lua_register(L, "lua_gc_restart", lua_gc_restart); // 2335 21.11: из папки «движок»
	lua_register(L, "lua_gc_step", lua_gc_step); // 2336 21.11: из папки «движок»
	lua_register(L, "lua_is_yieldable", lua_is_yieldable); // 2337 21.11: из папки «движок»
	lua_register(L, "lua_stack_top", lua_stack_top); // 2338 21.11: из папки «движок»
	lua_register(L, "get_current_script_name", get_current_script_name); // 2339 21.11: из папки «движок»
	lua_register(L, "get_game_speed", get_game_speed); // 2340 21.11: из папки «движок»
	lua_register(L, "set_gamespeed", set_gamespeed); // 2341 21.11: из папки «движок»
	lua_register(L, "get_actor_health", get_actor_health); // 2342 21.11: из папки «движок»
	lua_register(L, "set_actor_health", set_actor_health); // 2343 21.11: из папки «движок»

	// === LOG LINE (21.11) ===
	lua_register(L, "log_line", log_line); // 2344 21.11: строка в lualoader/log.txt

	// === AIRSHIP FLIGHT FLAG (21.11) ===
	lua_register(L, "set_airship_cheat", set_airship_cheat); // 2345 21.11: 0xA10B11 флаг полёта AIRSHIP
	lua_register(L, "is_airship_cheat", is_airship_cheat); // 2346 21.11: 0xA10B11 флаг полёта AIRSHIP

	// === CAR GAS PEDAL (21.11) ===
	lua_register(L, "set_car_gas", set_car_gas); // 2347 21.11: m_fGasPedal +0x1F0 (тяга полёта)

	// === MISSION LAUNCH (21.11) ===
	lua_register(L, "start_mission", start_mission); // 2348 21.11: запуск миссии (0417)
	lua_register(L, "start_mission_orig", start_mission_orig); // 2349 21.11: запуск миссии (0417) с подготовкой

	// === ORIGINAL MISSION NAMES (21.11) ===
	lua_register(L, "get_original_mission_name", get_original_mission_name); // 2350 21.11: оригинальные миссии
	lua_register(L, "get_original_mission_script", get_original_mission_script); // 2351 21.11: оригинальные миссии
	lua_register(L, "show_original_mission", show_original_mission); // 2352 21.11: оригинальные миссии
	lua_register(L, "show_original_missions", show_original_missions); // 2353 21.11: оригинальные миссии
	lua_register(L, "hide_original_missions", hide_original_missions); // 2354 21.11: закрыть список миссий
	lua_register(L, "start_mission_original", start_mission_original); // 2355 21.11: оригинальные миссии
	lua_register(L, "get_model_id_by_name", get_model_id_by_name); // 2356 получить id модели по имени
	lua_register(L, "check_studio_gates", check_studio_gates); // 2357 диагностика ворот киностудии
	lua_register(L, "open_studio_gates", open_studio_gates); // 2358 убрать закрытые ворота и поставить открытые
	lua_register(L, "get_flag", get_flag); // 2359 обёртка: get flag
	lua_register(L, "set_flag", set_flag); // 2360 обёртка: set flag
	lua_register(L, "flag_addr", flag_addr); // 2361 обёртка: flag addr
	lua_register(L, "get_flag_airship", get_flag_airship); // 2362 обёртка: get flag airship
	lua_register(L, "set_flag_airship", set_flag_airship); // 2363 обёртка: set flag airship
	lua_register(L, "get_flag_rc_zone", get_flag_rc_zone); // 2364 обёртка: get flag rc zone
	lua_register(L, "set_flag_rc_zone", set_flag_rc_zone); // 2365 обёртка: set flag rc zone
	lua_register(L, "get_flag_rc_mode", get_flag_rc_mode); // 2366 обёртка: get flag rc mode
	lua_register(L, "set_flag_rc_mode", set_flag_rc_mode); // 2367 обёртка: set flag rc mode
	lua_register(L, "get_flag_airship2", get_flag_airship2); // 2368 обёртка: get flag airship2
	lua_register(L, "set_flag_airship2", set_flag_airship2); // 2369 обёртка: set flag airship2
	lua_register(L, "get_flag_cheat82", get_flag_cheat82); // 2370 обёртка: get flag cheat82
	lua_register(L, "set_flag_cheat82", set_flag_cheat82); // 2371 обёртка: set flag cheat82
	lua_register(L, "get_flag_cheat26", get_flag_cheat26); // 2372 обёртка: get flag cheat26
	lua_register(L, "set_flag_cheat26", set_flag_cheat26); // 2373 обёртка: set flag cheat26
	lua_register(L, "get_flag_cheatadc", get_flag_cheatadc); // 2374 обёртка: get flag cheatadc
	lua_register(L, "set_flag_cheatadc", set_flag_cheatadc); // 2375 обёртка: set flag cheatadc
	lua_register(L, "get_flag_cheat23", get_flag_cheat23); // 2376 обёртка: get flag cheat23
	lua_register(L, "set_flag_cheat23", set_flag_cheat23); // 2377 обёртка: set flag cheat23
	lua_register(L, "get_flag_cheat47", get_flag_cheat47); // 2378 обёртка: get flag cheat47
	lua_register(L, "set_flag_cheat47", set_flag_cheat47); // 2379 обёртка: set flag cheat47
	lua_register(L, "get_flag_cheat5f", get_flag_cheat5f); // 2380 обёртка: get flag cheat5f
	lua_register(L, "set_flag_cheat5f", set_flag_cheat5f); // 2381 обёртка: set flag cheat5f
	lua_register(L, "get_flag_cheatstate", get_flag_cheatstate); // 2382 обёртка: get flag cheatstate
	lua_register(L, "set_flag_cheatstate", set_flag_cheatstate); // 2383 обёртка: set flag cheatstate
	lua_register(L, "get_flag_named", get_flag_named); // 2384 обёртка: get flag named
	lua_register(L, "set_flag_named", set_flag_named); // 2385 обёртка: set flag named
	lua_register(L, "list_flags", list_flags); // 2386 обёртка: list flags
	lua_register(L, "keep_rc", keep_rc); // 2387 обёртка: keep rc
	lua_register(L, "get_mem_addr", get_mem_addr); // 2388 обёртка: get mem addr
	lua_register(L, "force_rc_autopilot", force_rc_autopilot); // 2389 обёртка: force rc autopilot
	lua_register(L, "press_fire", press_fire); // 2390 обёртка: press fire
	lua_register(L, "release_fire", release_fire); // 2391 обёртка: release fire
	lua_register(L, "set_script_int", set_script_int); // 2392 обёртка: set script int
	lua_register(L, "get_script_int", get_script_int); // 2393 обёртка: get script int
	lua_register(L, "set_all_timers_99", set_all_timers_99); // 2394 23.11: все убывающие таймеры -> 99 мин
	lua_register(L, "restore_all_timers", restore_all_timers); // 2395 23.11: вернуть исходные значения таймеров
	lua_register(L, "map_overlay_on", map_overlay_on); // 2396 23.11: показать карту меню
	lua_register(L, "map_overlay_off", map_overlay_off); // 2397 23.11: скрыть карту меню
	lua_register(L, "map_overlay_toggle", map_overlay_toggle); // 2398 23.11: переключить карту меню
	lua_register(L, "map_overlay_is_on", map_overlay_is_on); // 2399 23.11: статус карты
	lua_register(L, "map_crosshair_world", map_crosshair_world); // 2400 23.11: точка карты -> мир
	lua_register(L, "map_coords_diag", map_coords_diag); // 2401 23.11: диагностика полей карты
	lua_register(L, "map_cross_set", map_cross_set); // 2402 23.11: перекрестие (u,v)
	lua_register(L, "map_cross_get", map_cross_get); // 2403 23.11: перекрестие + мир
	lua_register(L, "play_wav_file", play_wav_file); // 2404 23.11: WAV из Audio/
	lua_register(L, "stop_wav_file", stop_wav_file); // 2405 23.11: остановить WAV
	lua_register(L, "set_pad_controls", set_pad_controls); // 2406 обёртка: set pad controls
	lua_register(L, "sys_key_down", sys_key_down); // 2407 обёртка: sys key down
	lua_register(L, "sys_key_up", sys_key_up); // 2408 обёртка: sys key up
	lua_register(L, "sys_mouse_down", sys_mouse_down); // 2409 обёртка: sys mouse down
	lua_register(L, "sys_mouse_up", sys_mouse_up); // 2410 обёртка: sys mouse up
	lua_register(L, "find_all_peds_by_model", find_all_peds_by_model); // 2411 обёртка: find all peds by model
	lua_register(L, "get_water_level", get_water_level); // 2412 0x5C2C80 уровень воды в точке (x,y) или nil
	lua_register(L, "create_door_object", create_door_object); // 2413 объект-дверь с удержанием модели/коллизии
return 0;
};

