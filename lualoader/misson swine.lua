-- ============================================================================
-- МИССИЯ "Treacherous Swine" ("Предатель-свинья") — перенос ОРИГИНАЛА
-- (97 миссий, поток GENERL1, файл "07 - Treacherous Swine.txt", 0x49063).
--
-- СЮЖЕТ (по скрипту оригинала): Кортез зовет Томми на яхту. Его правая рука
-- ГОНСАЛЕС оказался предателем («treacherous swine»). В оригинале цель
-- создается как CREATE_CHAR 4 109 — модель 109 = СПЕЦ-СЛОТ 1, перед этим
-- загруженный 'IGGONZ' (LOAD_SPECIAL_CHARACTER 1 'IGGONZ'), то есть цель —
-- именно Гонсалес. Порт: Createped("IGGONZ", ...). Два официанта (CLA,
-- модель 89) нападают. Гонсалес сбегает; если добежит до (398.3,-469.8)±15 —
-- сцена побега и ПРОВАЛ; если УБИТ — розыск 2 (ALTER_WANTED_LEVEL_NO_DROP),
-- уйти от копов: бесплатная перекраска (SET_FREE_RESPRAYS 1, блайп
-- 327.2,429.9) или розыск упал до 0 — ПОБЕДА $250 + пикап одежды.
--
-- СЦЕНА: пентхаус в Вашингтон-Бич (терраса z≈32-36, вход 475.5,30.3,11).
-- Игрок заходит в двери → фейд → телепорт на террасу (454.4,31.3,33.86) →
-- диалоги (play_voice COL1_5/COL1_1/COL1_6) → погоня.
--
-- ПОРТ (правила §4 + уроки §3.2):
--  - катсцена COL_1 и аудио COL1_x → printmessage + play_voice (блокирующая
--    цепочка 03CF→03D1→03D2, проверено первой сессией Swine);
--  - бензопила выдается В НАЧАЛЕ: Giveweaponped(player, 1, "chnsaw") +
--    set_current_weapon_ped(player, 11) (урок §3.2: «нет бензопилы на пляже»);
--  - штатные объекты пентхауса (-76/-77/-78) снимаются ПО КООРДИНАТАМ
--    (find_object_by_model запрещен — модель встречается по всей карте,
--    как ворота Riot), ставятся «вечериночные» -116/-117/-118/-119;
--    при выходе из миссии мир восстанавливается (L4b388 оригинала);
--  - болтатели у пирса: CREATE_RANDOM_CHAR + SET_CHARS_CHATTING с КОНЕЧНЫМ
--    таймером 8000 мс (урок §3.2: 9999999 блокировал ИИ);
--  - погоня: ИИ обновляется раз в 500 мс по get_game_timer(); путь занят →
--    char_follow_path, свободен → run_to_coord (порядок КАК В ОРИГИНАЛЕ);
--  - победа: розыск стал 0 — единый детектор: перекраска сама сбрасывает
--    розыск, как и отсиживание (has_respray_happened требует номер гаража);
--  - ВЫХОД ИЗ ЦИКЛА Getflagmission: C++ теперь сам удаляет ВСЕ созданное
--    скриптом (purge_script_entities, 06.11) — педы, авто, объекты, пикапы,
--    блайпы, сферы; здесь остается только восстановление мира.
--
-- ВАЖНО ПРО АРХИТЕКТУРУ: вспомогательные функции живут ВЫШЕ main(), состояние
-- миссии передается в них параметрами (как в Riot; в глобалы не выносить).
-- ============================================================================

require("lualoader/mod")  -- подключаем модуль загрузчика игры

-- fade_and_wait(dir, dur): затемнение/проявление + ожидание конца фейда.
-- Урок §3.2: `while get_fading_status() do wait() end` без таймаута зависал —
-- предохранитель dur + 2000 мс обязателен.
local function fade_and_wait(dir, dur)  -- функция фейда с ожиданием
 fade(dir, dur)  -- запускаем затемнение/проявление
 local t0 = get_game_timer()  -- точка отсчета предохранителя
 while get_fading_status() do  -- пока экран гаснет/проявляется
  wait(10)  -- ждем короткими шагами
  if get_game_timer() - t0 > dur + 2000 then break end  -- предохранитель
 end  -- конец ожидания фейда
end  -- конец функции fade_and_wait

-- dialog(text, voice): реплика сцены — печать + блокирующая озвучка play_voice
-- (внутри C++ вся цепочка LOAD_MISSION_AUDIO → PLAY → HAS_FINISHED).
local function dialog(text, voice)  -- функция реплики
 if text ~= nil then printmessage(text, 4000, 1) end  -- печать реплики
 if voice ~= nil then play_voice(voice) end  -- озвучка (блокирует до конца фразы)
end  -- конец функции dialog

-- spawn_chatters(): трое болтателей у пирса (0376 CREATE_RANDOM_CHAR +
-- 03F9 SET_CHARS_CHATTING). Таймер чата КОНЕЧНЫЙ (урок §3.2).
local function spawn_chatters()  -- функция болтателей
 local c1 = create_random_char(-242.8, -1342.5, 7.1) if c1 ~= nil then setangle(c1, 204.5) end  -- болтатель 1
 local c2 = create_random_char(-240.8, -1344.1, 7.1) if c2 ~= nil then setangle(c2, 89.6) end   -- болтатель 2
 local c3 = create_random_char(-242.3, -1344.7, 7.1) if c3 ~= nil then setangle(c3, 349.2) end  -- болтатель 3
 if c1 ~= nil and c2 ~= nil then set_chars_chatting(c1, c2, 8000) end  -- конечный таймер чата!
 if c3 ~= nil and c2 ~= nil then set_chars_chatting(c3, c2, 8000) end  -- конечный таймер чата!
 return c1, c2, c3  -- возвращаем болтателей
end  -- конец функции spawn_chatters

-- open_penthouse_party(): сцена вечеринки в пентхаусе (029B оригинала).
-- Штатные объекты (-76/-77/-78 на $1814/$1815/$1816) снимаем ПО КООРДИНАТАМ,
-- ставим «вечериночные» -116/-117/-118/-119 + dont_remove_object.
local function open_penthouse_party()  -- функция превращения пентхауса в сцену
 local o1 = get_obj_in_cord(465.375, 30.336, 33.181, 2.0) if o1 ~= nil then remove_obj(o1) end  -- штатный -76
 local o2 = get_obj_in_cord(461.961, 31.436, 31.24, 2.0) if o2 ~= nil then remove_obj(o2) end   -- штатный -77
 local o3 = get_obj_in_cord(464.663, 30.336, 23.881, 2.0) if o3 ~= nil then remove_obj(o3) end  -- штатный -78
 local n1 = create_obj(-116, 464.663, 30.336, 23.881) if n1 ~= nil then dont_remove_object(n1) end  -- сцена 1
 local n2 = create_obj(-117, 461.961, 31.436, 31.24) if n2 ~= nil then dont_remove_object(n2) end   -- сцена 2
 local n3 = create_obj(-118, 454.321, 31.436, 35.198) if n3 ~= nil then dont_remove_object(n3) end  -- сцена 3 (открытые двери)
 local n4 = create_obj(-119, 470.68, 30.336, 30.19) if n4 ~= nil then dont_remove_object(n4) end    -- сцена 4
 return n1, n2, n3, n4  -- возвращаем созданное (destroy удалит при выходе)
end  -- конец функции open_penthouse_party

-- restore_penthouse_objects(): вернуть пентхаус в штатный вид (L4b388).
-- Вызывается ПОСЛЕ miss()/destroy — созданное здесь остается в мире до
-- следующего запуска миссии (тогда open_penthouse_party снимет его заново).
local function restore_penthouse_objects()  -- функция восстановления мира
 Createobj(-76, 465.375, 30.336, 33.181)  -- штатный объект 1
 Createobj(-77, 465.375, 30.336, 33.181)  -- штатный объект 2
 Createobj(-78, 464.663, 30.336, 23.881)  -- штатный объект 3
end  -- конец функции restore_penthouse_objects

-- spawn_penthouse_scene(): Гонсалес + 2 официанта на террасе (009A оригинала).
local function spawn_penthouse_scene()  -- функция сцены на террасе
 local gonzo = Createped("IGGONZ", 466.0, 38.2, 32.0)  -- Гонсалес (модель 109 = спец-слот 1 оригинала)
 if gonzo ~= nil then  -- если создался
  ped_clean_threat(gonzo)  -- 01ED: чистим поиск угроз
  setpedhealth(gonzo, 150)  -- 0223: SET_CHAR_HEALTH 150
  set_ped_only_damaged_by_player(gonzo, true)  -- 02A9: урон только от игрока
 end  -- конец настройки Гонсалеса
 local cook1 = Createped("CLA", 466.1, 40.1, 32.0)  -- официант 1 (модель 89)
 local cook2 = Createped("CLA", 464.3, 43.7, 32.0)  -- официант 2 (модель 89)
 if cook1 ~= nil and cook2 ~= nil then set_chars_chatting(cook1, cook2, 8000) end  -- болтают, таймер конечный
 return gonzo, cook1, cook2  -- возвращаем троих
end  -- конец функции spawn_penthouse_scene

-- gonzo_run(gonzo, x, y, z): обновить цель побега Гонсалеса (кадр ИИ 500 мс).
-- Порядок КАК В ОРИГИНАЛЕ (L4a92b): путь СВОБОДЕН → прямой бег,
-- путь ЗАНЯТ → CHAR_FOLLOW_PATH (обход по узлам).
local function gonzo_run(gonzo, x, y, z)  -- функция побега Гонсалеса
 if gonzo == nil or not isped(gonzo) or ispeddead(gonzo) then return end  -- цели нет/мертва
 if is_char_wander_path_clear(gonzo, x, y, z, 1) then  -- путь свободен?
  set_char_obj_run_to_coord(gonzo, x, y, z)  -- прямой бег к точке
 else  -- путь занят
  char_follow_path(gonzo, x, y, z, 1, 1)  -- обход по узлам: радиус 1, СКОРОСТЬ 1 = БЕГ (0 = шаг)
 end  -- конец выбора способа
end  -- конец функции gonzo_run

-- cook_chase(cook, pl, px, py, pz): ИИ официанта (GOSUB L4b461 оригинала):
-- игрок дальше 7 м (00FB ANY_MEANS) — догоняет; ближе — убить игрока.
local function cook_chase(cook, pl, px, py, pz)  -- функция ИИ официанта
 if cook == nil or not isped(cook) or ispeddead(cook) then return end  -- официанта нет/мертв
 if locate_player_any_means_char_3d(cook, 7.0, 7.0, 7.0, 0) then  -- игрок рядом (даже в авто)
  set_char_obj_kill_player_on_foot(cook, pl)  -- убить игрока
 else  -- игрок далеко
  if is_char_wander_path_clear(cook, px, py, pz, 1) then  -- путь к игроку свободен
   set_char_obj_run_to_coord(cook, px, py, pz)  -- прямой бег к игроку
  else  -- путь занят
   char_follow_path(cook, px, py, pz, 1, 1)  -- обход по узлам: радиус 1, СКОРОСТЬ 1 = БЕГ
  end  -- конец выбора способа
 end  -- конец проверки дистанции
end  -- конец функции cook_chase

function main()  -- главная функция миссии
while true do wait() local player = findplayer()  -- бесконечный цикл

 if Star_mission_marker(9, -244.28, -1360.67, 8.0)  -- старт миссии у пирса Кортеза
 then

  local step = 1           -- 1: идем в пентхаус; 2: погоня по террасе; 3: спуск; 4: погоня по городу; 5: побег от копов
  local stage = 0          -- индекс waypoint'а Гонсалеса на террасе ($2264 оригинала)
  local gonzo              -- Гонсалес (цель)
  local cook1, cook2       -- официанты
  local ch1, ch2, ch3      -- болтатели у пирса
  local n1, n2, n3, n4     -- объекты вечеринки
  local blip_site          -- блайп входа пентхауса
  local sphere_site        -- сфера входа пентхауса
  local blip_gonzo         -- блайп Гонсалеса
  local blip_spray         -- блайп перекраски
  local ch_released = 0    -- болтатели отпущены ($2269)
  local gonzo_t = 0        -- таймер ИИ Гонсалеса (16@)
  local cook1_t = 0        -- таймер ИИ официанта 1 (17@)
  local cook2_t = 0        -- таймер ИИ официанта 2
  local cooks_street = 0   -- официанты сброшены на улицу ($2265)
  local said1 = 0          -- первая реплика при близком контакте ($2264=5)
  local said2 = 0          -- вторая реплика при близком контакте ($2264=7)
  local said1_t = 0        -- таймер перезарядки реплик
  local pushed_out = 0     -- игрок вытолкнут из пентхауса
  local car_msg_t = 0      -- таймер сообщений «сядь в машину»
  local spray_car = nil    -- машина игрока (контекст перекраски)

  showtext(" Treacherous Swine", 500, 1)  -- название миссии
  set_ped_mood(1, 60000)  -- 04E3: настроение (оригинал)
  force_weather_now(4)    -- 01B6: FORCE_WEATHER_NOW 4 (оригинал; отпускается в after-loop)
  printmessage("~w~Cortez: Tommy! Welcome aboard! Glad you could come to my little party.", 5000, 1)  -- интро (катсцена COL_1 → текст)
  wait(20)
  printmessage("~w~Cortez: A treacherous swine has insulted me for the last time!", 5000, 1)  -- интро
  wait(20)
  printmessage("~w~Cortez: He is hiding at the penthouse party up the coast. Silence the pig!", 6000, 1)  -- интро
  wait(20)

  -- бензопила — СРАЗУ (урок §3.2: выдача в начале main, weapon 11 = chnsaw)
  Giveweaponped(player, 1, "chnsaw")  -- 01B1: GIVE_WEAPON_TO_PLAYER 11 1
  set_current_weapon_ped(player, 11)  -- 01B8: бензопила в руки

  ch1, ch2, ch3 = spawn_chatters()  -- болтатели у пирса
  n1, n2, n3, n4 = open_penthouse_party()  -- объекты вечеринки
  ped_road_off(479.9, -1.4, 11.0, 450.3, 59.5, 40.0)  -- 022B: пешеходов у пентхауса нет

  blip_site = radar_add_blip_for_coord(476.8, 30.4, 11.0)  -- 018A: блайп входа
  sphere_site = create_sphere(475.5, 30.3, 11.0, 3.0)  -- 03A1: сфера входа (урок §3.2)
  printmessage("~g~Get to the penthouse party!", 5000, 1)  -- GEN1_05

  while true == Getflagmission() do wait()  -- цикл миссии
   local pp = findplayer()  -- игрок
   if pp == nil then pp = player end  -- страховка

   -- болтатели: игрок покинул BEACH1 → отпустить (01C2, $2269 оригинала)
   if ch_released == 0 and is_ped_in_zone(pp, "BEACH1") == 0 then  -- функция возвращает 1/0, а не булево!
    if ch1 ~= nil and isped(ch1) then mark_char_as_no_longer_needed(ch1) end  -- отпускаем 1
    if ch2 ~= nil and isped(ch2) then mark_char_as_no_longer_needed(ch2) end  -- отпускаем 2
    if ch3 ~= nil and isped(ch3) then mark_char_as_no_longer_needed(ch3) end  -- отпускаем 3
    ch_released = 1  -- помечаем
   end  -- конец отпускания болтателей

   -- ШАГ 1: вход в пентхаус (0057 оригинала, углы 476.5,27.3,11.28 - 474.8,33,14.3)
   if step == 1 and is_player_in_area_3d(476.5, 27.3, 11.28, 474.8, 33.0, 14.3, 0) then
    if blip_site ~= nil then remove_blip(blip_site) blip_site = nil end  -- блайп снят
    if sphere_site ~= nil then remove_sphere(sphere_site) sphere_site = nil end  -- сфера снята (урок §3.2)
    ped_frozen(0)  -- 01B4: SET_PLAYER_CONTROL 0 — управление убрано
    set_widescreen(true)  -- 02A3: широкие полосы
    gonzo, cook1, cook2 = spawn_penthouse_scene()  -- сцена на террасе
    set_char_obj_run_to_coord(pp, 475.7, 30.3, 11.0)  -- 0239: игрок бежит к дверям
    wait(300)  -- даем подойти
    fade_and_wait(0, 500)  -- затемнение (оригинал: синий DO_FADE 500 0)
    setcord(pp, 454.4, 31.3, 33.86)  -- 0055: телепорт на террасу
    setangle(pp, 270.0)  -- 0171: heading 270
    set_camera_position(468.1, 41.0, 33.5, 0.0, 0.0, 0.0)  -- 015F: камера сцены
    camera_at_point(461.0, 32.8, 33.0, 2)  -- 0160: взгляд сцены
    fade_and_wait(1, 500)  -- проявление
    set_char_obj_goto_coord_on_foot(pp, 460.7, 27.5, 33.0)  -- 0211: игрок идет к Гонсалесу
    wait(250)  -- даем подойти (оригинал: до 5 с)
    if gonzo ~= nil and isped(gonzo) then turn_player_to_face_char(pp, gonzo) end  -- 0210: лицом к цели

    dialog("~r~Gonzalez: Tommy?! What are you doing here with a chainsaw?!", "COL1_5")  -- GEN1_10
    dialog("~r~Gonzalez: Cortez sent you, hasn't he? That old fool! I am done with him!", "COL1_1")  -- GEN1_06
    if cook1 ~= nil and isped(cook1) then set_char_obj_kill_player_on_foot(cook1, pp) end  -- официант 1 атакует
    if cook2 ~= nil and isped(cook2) then set_char_obj_kill_player_on_foot(cook2, pp) end  -- официант 2 атакует
    if gonzo ~= nil and isped(gonzo) then set_char_obj_run_to_coord(gonzo, 466.1, 42.7, 32.0) end  -- цель побежала
    dialog("~w~Cortez: Don't let that traitor get away, Tommy! Kill the swine!", "COL1_6")  -- GEN1_11

    if gonzo ~= nil and isped(gonzo) then blip_gonzo = create_marker(gonzo) end  -- 0187: блайп цели
    restore_camera()  -- 02EB: камера обратно
    ped_frozen(1)  -- 01B4: SET_PLAYER_CONTROL 1 — управление игроку
    set_widescreen(false)  -- 02A3: полосы убраны
    stage = 1  -- погоня по террасе началась
    step = 2  -- шаг 2
   end  -- конец шага 1

   -- ШАГ 2: погоня по террасе (цепочка waypoint'ов $2264=0..4 оригинала)
   if step == 2 and gonzo ~= nil and isped(gonzo) and not ispeddead(gonzo) then
    if stage == 1 then  -- waypoint 1
     if locate_char_any_means_2d(gonzo, 466.1, 42.7, 1.0, 1.0, 0) then  -- 00EC: достиг точку
      set_char_obj_run_to_coord(gonzo, 472.3, 42.8, 32.0)  -- к следующей
      stage = 2  -- waypoint 2
     end  -- конец waypoint 1
    elseif stage == 2 then  -- waypoint 2
     if locate_char_any_means_2d(gonzo, 472.3, 42.8, 1.0, 1.0, 0) then
      set_char_obj_run_to_coord(gonzo, 472.5, 35.9, 32.0)
      stage = 3  -- waypoint 3
     end  -- конец waypoint 2
    elseif stage == 3 then  -- waypoint 3
     if locate_char_any_means_2d(gonzo, 472.5, 35.9, 1.0, 1.0, 0) then
      set_char_obj_run_to_coord(gonzo, 464.0, 35.6, 32.0)
      stage = 4  -- waypoint 4
     end  -- конец waypoint 3
    elseif stage == 4 then  -- waypoint 4
     if locate_char_any_means_2d(gonzo, 464.0, 35.6, 1.0, 1.0, 0) then
      set_char_obj_run_to_coord(gonzo, 463.0, 31.6, 32.0)
      stage = 5  -- waypoint 5 (последний)
     end  -- конец waypoint 4
    elseif stage == 5 then  -- последний waypoint: замер на месте (0350)
     if locate_char_any_means_2d(gonzo, 463.0, 31.6, 1.0, 1.0, 0) then
      set_char_obj_no_obj(gonzo)  -- 011C: задачи нет
      set_ped_stay_when_attacked(gonzo, true)  -- 0350: SET_CHAR_STAY_IN_SAME_PLACE
      step = 3  -- сцена спуска
     end  -- конец waypoint 5
    end  -- конец цепочки waypoint'ов
   end  -- конец шага 2

   -- ШАГ 3: сцена спуска на улицу (L4a5cf-L4a8bc оригинала)
   if step == 3 then
    ped_frozen(0)  -- управление убрано
    set_widescreen(true)  -- широкие полосы
    set_everyone_ignore_player(1)  -- 03BF: все игнорируют игрока (сцена)
    if gonzo ~= nil and isped(gonzo) and not ispeddead(gonzo) then
     set_char_obj_run_to_coord(gonzo, 457.8, 31.4, 32.0)  -- цель идет к краю
     wait(200)  -- оригинал: до 3 с
     fade_and_wait(0, 500)  -- затемнение
     setcord(gonzo, 474.7, 29.9, 11.07)  -- 00A1: цель спускается на улицу
     setangle(gonzo, 92.5)  -- 0173: heading 92.5
    end  -- конец телепорта цели
    if is_player_in_area_3d(434.7, 51.5, 45.6, 480.4, 12.0, 24.0, 0) then  -- игрок еще в пентхаусе
     fade_and_wait(0, 500)  -- затемнение (телепорт только за черным экраном)
     setcord(pp, 474.7, 29.9, 11.07)  -- игрок тоже вниз
     setangle(pp, 92.5)  -- heading
    end  -- конец телепорта игрока
    set_camera_position(484.2, 37.0, 12.8, 0.0, 0.0, 0.0)  -- камера улицы
    camera_at_point(479.7, 27.5, 11.5, 2)  -- взгляд
    fade_and_wait(1, 500)  -- проявление
    if gonzo ~= nil and isped(gonzo) and not ispeddead(gonzo) then
     set_ped_stay_when_attacked(gonzo, false)  -- 0350: отпускаем цель
     set_char_obj_run_to_coord(gonzo, 482.8, 30.9, 11.0)  -- бежит по улице
    end  -- конец запуска цели
    dialog("~w~Cortez: He is running for the docks! Cut him off, Tommy!", "COL1_3")  -- GEN1_08
    wait(100)  -- оригинал: WAIT 1000
    gonzo_run(gonzo, 402.1, -462.2, 10.1)  -- дальняя цель побега
    dialog("~r~Gonzalez: Stay away from me, you psycho!", "COL1_7")  -- GEN1_12
    wait(150)  -- оригинал: WAIT 1500
    if n1 ~= nil then remove_obj(n1) n1 = nil end  -- сцена: -116 снимаем
    local n78 = Createobj(-78, 464.663, 30.336, 23.881)  -- и ставим -78 (оригинал L4a8bc)
    if n78 ~= nil then dont_remove_object(n78) end  -- не выгружать
    restore_camera()  -- 02EB: камера
    ped_frozen(1)  -- управление игроку
    set_everyone_ignore_player(0)  -- мир снова реагирует
    set_widescreen(false)  -- полосы убраны
    printmessage("~g~Don't let him reach the docks!", 5000, 1)  -- GEN1_05
    gonzo_t = get_game_timer()  -- таймеры ИИ стартуют
    cook1_t = gonzo_t  -- официант 1
    cook2_t = gonzo_t  -- официант 2
    said1_t = gonzo_t  -- перезарядка реплик
    step = 4  -- шаг 4
   end  -- конец шага 3

   -- ШАГ 4: погоня по городу (L4a92b оригинала)
   if step == 4 then
    local now = get_game_timer()  -- текущее время (мс)
    if now - gonzo_t > 500 then  -- ИИ цели раз в 500 мс
     gonzo_t = now  -- запоминаем время
     gonzo_run(gonzo, 402.1, -462.2, 10.1)  -- обновляем побег
    end  -- конец ИИ цели
    if isped(pp) then  -- игрок существует (урок §3.2: isped перед getpedcoordes)
     local px, py, pz = getpedcoordes(pp)  -- координаты игрока
     if px ~= nil then  -- координаты получены
      if now - cook1_t > 500 then  -- ИИ официанта 1 раз в 500 мс
       cook1_t = now  -- запоминаем время
       cook_chase(cook1, pp, px, py, pz)  -- обновляем погоню
      end  -- конец официанта 1
      if now - cook2_t > 500 then  -- ИИ официанта 2 раз в 500 мс
       cook2_t = now  -- запоминаем время
       cook_chase(cook2, pp, px, py, pz)  -- обновляем погоню
      end  -- конец официанта 2
     end  -- конец проверки координат
    end  -- конец ИИ официантов
    -- игрок в пентхаусе → официанты на улицу ($2265 оригинала)
    if cooks_street == 0 and is_player_in_area_3d(434.7, 51.5, 45.6, 480.4, 12.0, 24.0, 0) then
     if cook1 ~= nil and isped(cook1) and not ispeddead(cook1) then  -- официант 1 жив
      setcord(cook1, 477.0, 28.7, 11.0)  -- вниз на улицу
      setangle(cook1, 272.4)  -- heading
     end  -- конец официанта 1
     if cook2 ~= nil and isped(cook2) and not ispeddead(cook2) then  -- официант 2 жив
      setcord(cook2, 477.0, 32.7, 11.0)  -- вниз на улицу
      setangle(cook2, 272.4)  -- heading
     end  -- конец официанта 2
     cooks_street = 1  -- помечаем
    end  -- конец сброса официантов
    -- реплики при близком контакте (00E9 5x5, $2264=5/7 оригинала)
    if now - said1_t > 20000 then  -- перезарядка 20 с
     if said1 == 0 and locate_player_any_means_char_2d(gonzo, 5.0, 5.0, 0) then
      dialog("~r~Gonzalez: Help! Somebody stop this madman!", "COL1_4")  -- GEN1_09
      said1 = 1  -- первая сказана
      said1_t = now  -- перезарядка
     elseif said1 == 1 and locate_player_any_means_char_2d(gonzo, 5.0, 5.0, 0) then
      dialog("~w~Cortez: Finish him, Tommy! Make it quick!", "COL1_8")  -- GEN1_13
      said2 = 1  -- вторая сказана
      said1_t = now  -- перезарядка
     end  -- конец реплик
    end  -- конец перезарядки
    -- ЦЕЛЬ ДОБЕЖАЛА → сцена побега → ПРОВАЛ (L4adf6 оригинала)
    if gonzo ~= nil and isped(gonzo) and not ispeddead(gonzo)
     and locate_char_any_means_3d(gonzo, 398.3, -469.8, 11.9, 15.0, 15.0, 15.0, 0) then
     ped_frozen(0)  -- управление убрано
     set_everyone_ignore_player(1)  -- все игнорируют
     set_widescreen(true)  -- полосы
     set_camera_position(403.96, -463.42, 15.2, 0.0, 0.0, 0.0)  -- камера побега
     camera_at_point(396.33, -472.82, 11.8, 2)  -- взгляд
     if ped_in_point_in_radius(pp, 398.3, -469.8, 11.9, 15.0, 15.0, 15.0) then  -- 00F5: ИГРОК у сцены
      -- игрок рядом со сценой — ставим его наблюдателем (00F5/0055 оригинала)
      fade_and_wait(0, 500)  -- черным экраном
      setcord(pp, 401.618, -464.7, 9.6664)  -- точка наблюдения
     end  -- конец телепорта игрока
     set_char_obj_no_obj(gonzo)  -- задачи нет
     setcord(gonzo, 405.5, -473.2, 9.0)  -- цель у сцены
     setangle(gonzo, 49.0)  -- heading
     wait(100)  -- оригинал: WAIT 1000
     gonzo_run(gonzo, 393.5, -476.4, 12.3)  -- цель уходит к воде
     wait(100)  -- оригинал: WAIT 3000
     restore_camera()  -- камера
     ped_frozen(1)  -- управление игроку
     set_everyone_ignore_player(0)  -- мир реагирует
     set_widescreen(false)  -- полосы убраны
     remove_char_elegantly(gonzo)  -- 034F: DELETE_CHAR — цель скрылась
     printmessage("~r~Gonzalez got away! Cortez is furious.", 5000, 1)  -- GEN1_18
     end_mission("mission failed!")  -- 00BA: M_FAIL (+ destroy всего созданного)
     step = nil  -- сброс шага
     break  -- выход из цикла миссии
    end  -- конец провала
   end  -- конец шага 4

   -- ЦЕЛЬ МЕРТВА (или не создалась) в ЛЮБОЙ фазе погони → побег от копов
   -- (L4a57c оригинала: смерть $2251 ловится в общем цикле погони L4a290,
   -- на террасе или на улице — неважно; nil = спавн не удался, не зацикливаемся)
   if step == 2 or step == 4 then
    if gonzo == nil or (isped(gonzo) and ispeddead(gonzo)) then
     play_one_off_sound(0.0, 0.0, 0.0, 1)  -- 018C: звук (оригинал)
     printmessage("~g~The swine is dead! Now lose the cops!", 5000, 1)  -- GEN1_19
     if blip_gonzo ~= nil then remove_blip(blip_gonzo) blip_gonzo = nil end  -- блайп цели снят
     alter_wanted_level_no_drop(2)  -- 010E: розыск 2 без авто-спада
     car_msg_t = get_game_timer()  -- таймер сообщений
     step = 5  -- шаг 5
    end  -- конец убийства цели
   end  -- конец общего детектора смерти

   -- ШАГ 5: побег от копов (L4afd1 оригинала)
   if step == 5 then
    -- вытолкнуть игрока из пентхауса (однократно, за черным экраном)
    if pushed_out == 0 and is_player_in_area_3d(434.7, 51.5, 45.6, 480.4, 12.0, 24.0, 0) then
     fade_and_wait(0, 500)  -- затемнение
     setcord(pp, 479.5, 30.11, 10.07)  -- 0055: на улицу
     setangle(pp, 274.5)  -- 0171: heading
     fade_and_wait(1, 500)  -- проявление
     pushed_out = 1  -- помечаем
    end  -- конец выталкивания
    local inc, car = incar(pp)  -- игрок в машине?
    if inc and car ~= nil then  -- в машине
     if spray_car == nil then  -- машина еще не зафиксирована
      spray_car = car  -- запоминаем
      printmessage("~w~Take the car to the respray - it's free!", 5000, 1)  -- GEN1_21
      set_free_resprays(1)  -- 0335: перекраска бесплатна
      blip_spray = radar_add_blip_for_coord(327.2, 429.9, 10.3)  -- 018A: блайп перекраски
     end  -- конец фиксации машины
    else  -- пешком
     if get_game_timer() - car_msg_t > 6000 then  -- не спамим: раз в 6 с
      car_msg_t = get_game_timer()  -- запоминаем время
      printmessage("~w~Get a car!", 3000, 1)  -- GEN1_20
     end  -- конец анти-спама
    end  -- конец проверки машины
    -- ПОБЕДА: розыск упал до 0 (перекраска сама его сбрасывает, как и укрытие)
    if not is_wanted_level_greater(0) then  -- 010F: розыск НЕ выше 0
     if blip_spray ~= nil then remove_blip(blip_spray) blip_spray = nil end  -- блайп снят
     register_mission_passed("COL_1")  -- 0318: REGISTER_MISSION_PASSED
     player_made_progress(1)  -- 030C: прогресс игры
     alter_wanted_level(0)  -- 0110: CLEAR_WANTED_LEVEL (страховка)
     miss(250)  -- победа: звук, флаг, destroy, $250 (порядок как в Riot)
     create_pickup_clothes(414.3, 1042, 25.4, 8)  -- 055B: пикап одежды — ПОСЛЕ miss() (урок §3.2)
     step = nil  -- сброс шага
     break  -- выход из цикла миссии
    end  -- конец победы
   end  -- конец шага 5

  end  -- конец цикла Getflagmission

  -- AFTER-LOOP: восстановление мира (L4b388 оригинала). Созданные сущности
  -- (цель, официанты, болтатели, объекты вечеринки, блайпы) уже удалены
  -- purge_script_entities (06.11: Getflagmission удаляет все при выходе;
  -- miss()/end_mission зовет destroy() — то же самое).
  ped_frozen(1)  -- 01B4: контроль игроку
  set_everyone_ignore_player(0)  -- 03BF: мир снова реагирует
  set_widescreen(false)  -- полосы убраны
  set_free_resprays(0)  -- 0335: перекраска снова платная
  release_weather()  -- 01B7: погоду отпустить
  ped_road_on(479.9, -1.4, 11.0, 450.3, 59.5, 40.0)  -- 022A: пешеходов вернуть
  restore_penthouse_objects()  -- штатные объекты пентхауса на место
  if ch1 ~= nil and isped(ch1) then mark_char_as_no_longer_needed(ch1) end  -- болтатель 1
  if ch2 ~= nil and isped(ch2) then mark_char_as_no_longer_needed(ch2) end  -- болтатель 2
  if ch3 ~= nil and isped(ch3) then mark_char_as_no_longer_needed(ch3) end  -- болтатель 3
  step = nil  -- сброс шага
 end  -- конец маркера миссии
end  -- конец главного цикла
end  -- конец функции main
