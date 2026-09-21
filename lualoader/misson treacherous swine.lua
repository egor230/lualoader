require("lualoader/mod")

-- ============================================================================
-- МИССИЯ "Treacherous Swine" ("Подлая свинья") — перенос ОРИГИНАЛА (97 миссий,
-- поток GENERL1, смещение 0x49063, 9568 байт, 1093 инструкций, 195 меток).
--
-- СЮЖЕТ: полковник Кортес даёт Томми бензопилу и просит убить предателя
-- ("подлую свинью"), который прячется на пляже под охраной двух телохранителей.
-- Томми преследует его по крышам особняка; свинья спрыгивает и убегает на
-- другой конец пляжа. Томми догоняет и убивает его.
--
-- ЛОГИКА ОРИГИНАЛА (важно — не инвертировать!):
-- - Свинья УБИТА → ADD_ONE_OFF_SOUND + GEN1_19 + ALTER_WANTED_LEVEL_NO_DROP 2
--   → игрок должен добежать до мойки (SET_FREE_RESPRAYS 1, блайп 327.2 429.9
--   10.3) в АВТО → HAS_RESPRAY_HAPPENED → M_PASS 250 (УСПЕХ).
-- - Свинья СБЕЖАЛА (достигла 398.3 -469.8 11.9, радиус 15) → катсцена
--   побега → M_FAIL (ПРОВАЛ).
-- - Провал M_FAIL = Свинья убежала (или арест/смерть игрока).
--
-- ЗАМЕНЫ (нет опкодов в API):
-- - Катсцена COL_1 (~42 сек, 12 субтитров GEN1_A..J, модели CSPLAY/CSCOLO/
--   CGONA/CSERVRB + LOBTRAY/LOBSTER/COKNIFE/COLTRAY) → fade + текстовые
--   реплики (как в Riot).
-- - Аудиодиалоги COL1_1..8 (LOAD_MISSION_AUDIO/PLAY_MISSION_AUDIO) → пропущены
--   (короткие текстовые реплики вместо них).
-- - HAS_RESPRAY_HAPPENED ($684/$685 — неизвестные индексы гаражей) →
--   детектор через get_player_wanted_level(): после установки no-drop 2
--   игрок въезжает в мойку → движок обнуляет розыск → 2→0 = респрей.
-- - SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE (18 объектов -95..-112 вокруг
--   особняка, прячутся на время катсцены) → пропущено (катсцены нет).
--
-- Ловушки:
-- - ped_in_point_in_radius = findpedinpool (1 мс) + sleep(10 мс) — В ЦИКЛЕ
--   НЕ ЗВАТЬ (зависание кадра); дистанции считать через getpedcoordes
--   (только findpedinpool 1 мс) и вручную.
-- - getpedcoordes тоже зовёт findpedinpool (1 мс) — не более ~5 вызовов/кадр.
-- - findpedinpool(NULL) → segfault: перед вызовом проверять isped/ispeddead.
-- - 4 объекта крыши (-116..-119) создаются DONT_REMOVE_OBJECT — движок не
--   удаляет; снимаются только в очистке.
-- - Свинья = спец-актор IGGONZ (модель 109), HP 150, ONLY_DAMAGED_BY_PLAYER,
--   anim group 50; охрана = CLA (модель 89).
-- - CTRL-reload перечитывает только Lua; новые C++-функции (set_free_resprays,
--   get_player_wanted_level и др.) требуют пересборки .asi и РЕСТАРТА игры.
-- ============================================================================

-- distance2d: квадрат дистанции между двумя точками (без sqrt — для сравнений).
local function distance2d_sq(x1, y1, x2, y2)
 return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)
end

-- ped_dist2d_sq: квадрат 2D-дистанции между педом и точкой (или nil, если пед
-- невалиден). Один вызов getpedcoordes = один findpedinpool (1 мс).
local function ped_dist2d_sq(ped, x, y)
 if not isped(ped) then return nil end
 local px, py = getpedcoordes(ped)
 if px == nil then return nil end
 return distance2d_sq(px, py, x, y)
end

function main()
while true do wait()  local player = findplayer()

 if Star_mission_marker(9, -250.5, -1361.1, 8.1)
 then
  ped_frozen(1)  -- разморозить: обёртка SMM погасила экран
  fade(1, 1200)  -- проявление после Star_mission_marker
  wait(1200)

  local step = 1               -- 1 = катсцена→пляж, 2 = ждать подхода, 3 = конфронтация (одноразовый блок),
                               -- 4 = преследование, 5 = респрай, 6 = успех, 7 = провал-побег, nil = конец
  local pig                    -- Свинья (IGGONZ / 109) — главная цель
  local guard1, guard2         -- 2 охранника (CLA / 89)
  local beach1, beach2, beach3 -- 3 случайных педа на пляжу (декор)
  local roof1, roof2, roof3, roof4  -- 4 объекта крыши (-116..-119)
  local blip_site              -- блайп места встречи (018A)
  local blip_pig               -- блайп на Свинью (0187 → create_marker_actor)
  local blip_respray           -- блайп мойки (при провале-убийстве)
  local pig_phase = 0          -- фаза маршрута Свиньи ($2264: 0..4 = точки крыши, 5..8 = диалоги)
  local pig_escaped = 0        -- 1 = Свинья спрыгнула с крыши и бежит к точке побега
  local timer_t = 0            -- get_game_timer() последнего рефреша ИИ охраны
  local timer_u = 0            -- то же для второй охраны
  local beach_removed = 0      -- 1 = пляжные педы уже сняты
  local respray_msg = 0        -- 1 = GEN1_20 ("нужно авто") уже показано
  set_wanted(0)
  showtext(" Treacherous Swine", 500, 1)
  ma_trace("swine: СТАРТ у особняка Кортеса (-250.5, -1361.1, 8.1)")

  -- === шаг 1: катсцена → текст + fade + телепорт на пляж (LOAD_CUTSCENE в API нет) ===
  printmessage("~w~Colonel Cortez: One of my people has been talking to the cops.", 6000, 1)  -- GEN1_A
  wait(3000)
  printmessage("~w~Cortez: This... person... is a treacherous swine!", 6000, 1)  -- GEN1_B
  wait(3000)
  printmessage("~w~Cortez: He's hiding at the beach with his bodyguards.", 6000, 1)  -- GEN1_C
  wait(3000)
  printmessage("~w~Cortez: Take this chainsaw. Show him my displeasure.", 6000, 1)  -- GEN1_D
  wait(3000)
  ma_trace("swine: катсцена COL_1 заменена на текст (опкодов LOAD_CUTSCENE нет)")

  fade(0, 1500)  -- 016A DO_FADE 1500 0 — затемнение
  wait(1500)

  -- подготовка пляжа (@L49737): коллизия, очистка, телепорт
  request_collision(-244.28, -1360.67)  -- 04E4 REQUEST_COLLISION
  cleanarea(-244.28, -1360.67, 7.0786, 1, 1)  -- 0395 CLEAR_AREA
  setcord(player, -244.28, -1360.67, 7.0786)  -- 0055 SET_PLAYER_COORDINATES
  setangle(player, 279.136)  -- 0171 SET_PLAYER_HEADING
  set_camera_behind_player()  -- 0373 SET_CAMERA_BEHIND_PLAYER
  set_car_density_multiplier(1)  -- 01EB SET_CAR_DENSITY_MULTIPLIER 1
  set_ped_density_multiplier(1)  -- 03DE SET_PED_DENSITY_MULTIPLIER 1
  set_fade_color(0, 0, 0)  -- 0169 SET_FADING_COLOUR 0 0 0
  wait(500)
  set_camera_behind_player()  -- 0373 (оригинал ставит дважды)
  set_ped_mood(1, 60000)  -- 04E3 SET_PLAYER_MOOD 1 60000
  force_weather_now(4)  -- 01B6 FORCE_WEATHER_NOW 4
  loadmodel(269)  -- 0247 REQUEST_MODEL 269 (chnsaw — бензопила)
  loadmodel(89)  -- 0247 REQUEST_MODEL 89 (CLA — охрана)
  load_requested_models()  -- 038B LOAD_ALL_MODELS_NOW

  -- сцена пляжа (@L499fd): дороги, оружие, блайп, педы, объекты крыши
  ped_road_off()  -- 022B SWITCH_PED_ROADS_OFF
  Giveweaponped(player, 1, "chnsaw")  -- 01B1 GIVE_WEAPON_TO_PLAYER 11 1 — БЕНЗОПИЛА
  blip_site = radar_add_blip_for_coord(476.8, 30.4, 11.0)  -- 018A ADD_BLIP_FOR_COORD
  beach1 = create_random_char(-242.8, -1342.5, 7.1)  -- 0376 CREATE_RANDOM_CHAR
  if beach1 ~= nil then setangle(beach1, 204.5) end  -- 0173 SET_CHAR_HEADING
  beach2 = create_random_char(-240.8, -1344.1, 7.1)
  if beach2 ~= nil then setangle(beach2, 89.6) end
  beach3 = create_random_char(-242.3, -1344.7, 7.1)
  if beach3 ~= nil then setangle(beach3, 349.2) end
  if beach1 ~= nil and beach2 ~= nil then set_chars_chatting(beach1, beach2, 86400000) end  -- 03F9 SET_CHARS_CHATTING
  if beach3 ~= nil and beach2 ~= nil then set_chars_chatting(beach3, beach2, 86400000) end
  roof1 = Createobj(-116, 464.663, 30.336, 23.881)  -- 029B CREATE_OBJECT_NO_OFFSET
  if roof1 ~= nil then dont_remove_object(roof1) end  -- 01C7 DONT_REMOVE_OBJECT
  roof2 = Createobj(-117, 461.961, 31.436, 31.24)
  if roof2 ~= nil then dont_remove_object(roof2) end
  roof3 = Createobj(-118, 454.321, 31.436, 35.198)
  if roof3 ~= nil then dont_remove_object(roof3) end
  roof4 = Createobj(-119, 470.68, 30.336, 30.19)
  if roof4 ~= nil then dont_remove_object(roof4) end
  fade(1, 1500)  -- 016A DO_FADE 1500 1 — проявление
  wait(1500)
  printmessage("~g~Find the swine before he leaves the beach!", 5000, 1)  -- GEN1_05
  step = 2
  ma_trace("swine: сцена пляжа готова (3 рандом-педа, 4 объекта крыши, блайп)")

  while true == Getflagmission() do wait()
   local pp = findplayer()
   if pp == nil then pp = player end

   -- === шаг 2: ждать подхода игрока к месту встречи (@L49b48) ===
   if step == 2 then
    if beach_removed == 0 and is_player_in_zone("BEACH1") == 0 then  -- 8121 NOT IS_PLAYER_IN_ZONE 'BEACH1' → снять педов
     mark_char_as_no_longer_needed(beach1)  -- 01C2 MARK_CHAR_AS_NO_LONGER_NEEDED
     mark_char_as_no_longer_needed(beach2)
     mark_char_as_no_longer_needed(beach3)
     beach_removed = 1
    end
    if beach_removed == 0 and is_ped_shooting(pp) then  -- 02DF IS_PLAYER_SHOOTING → снять педов
     mark_char_as_no_longer_needed(beach1)
     mark_char_as_no_longer_needed(beach2)
     mark_char_as_no_longer_needed(beach3)
     beach_removed = 1
    end
    -- зона 3D 476.5 27.3 11.28 → 474.8 33 14.3 (центр 475.65 30.15 12.79, пол-размеры 0.85 2.85 1.51)
    local d = ped_dist2d_sq(pp, 475.65, 30.15)  -- 8057 NOT IS_PLAYER_IN_AREA_3D — грубо по 2D (z отсечётся почти всегда)
    if d ~= nil and d < 9.0 then  -- радиус ~3 по X+Y (0.85²+2.85² ≈ 8.9)
     step = 3
     ma_trace("swine: игрок пришёл на место встречи — конфронтация")
    end
   end

   -- === шаг 3: конфронтация (@L49c48) — одноразовый блок ===
   if step == 3 then
    step = 31  -- маркер одноразового выполнения (31 = конфронтация идёт)
    message_clear_all()  -- 00BE CLEAR_PRINTS
    if blip_site ~= nil then remove_blip(blip_site) blip_site = nil end  -- 0164 REMOVE_BLIP
    ped_frozen(0)  -- 01B4 SET_PLAYER_CONTROL 0 — заморозить
    set_widescreen(true)  -- 02A3 SWITCH_WIDESCREEN 1
    Giveweaponped(player, 1, "chnsaw")  -- 01B1 (повтор, как в оригинале @L49c48)
    set_current_weapon_ped(player, 11)  -- 01B8 SET_CURRENT_PLAYER_WEAPON 11 — бензопила в руки
    pig = create_spec_ped("IGGONZ", 466.0, 38.2, 32.0)  -- 009A CREATE_CHAR 4 109 — Свинья
    if pig ~= nil then
     ped_clean_threat(pig)  -- 01ED CLEAR_CHAR_THREAT_SEARCH
     setpedhealth(pig, 150)  -- 0223 SET_CHAR_HEALTH 150
     set_ped_only_damaged_by_player(pig, true)  -- 02A9 SET_CHAR_ONLY_DAMAGED_BY_PLAYER 1
     set_ped_m_nAnimGroupId(pig, 50)  -- 0245 SET_ANIM_GROUP_FOR_CHAR 50
    end
    guard1 = create_spec_ped("CLA", 466.1, 40.1, 32.0)  -- 009A CREATE_CHAR 4 89 — охрана 1
    if guard1 ~= nil then
     ped_clean_threat(guard1)  -- 01ED
     setangle(guard1, 180)  -- 0173 SET_CHAR_HEADING
    end
    guard2 = create_spec_ped("CLA", 464.3, 43.7, 32.0)  -- 009A CREATE_CHAR 4 89 — охрана 2
    if guard2 ~= nil then
     ped_clean_threat(guard2)  -- 01ED
    end
    if pig ~= nil and guard1 ~= nil then set_chars_chatting(pig, guard1, 9999999) end  -- 03F9
    set_char_obj_run_to_coord(pp, 475.7, 30.3, 11.0)  -- 0239 — игрок бежит к месту
    -- ждать 5000 мс или прохождения цели (оригинал @L49d04)
    local t0 = get_game_timer()  -- 01BD GET_GAME_TIMER
    while step == 31 do
     wait()
     local p2 = findplayer()
     if p2 ~= nil and is_ped_objective(p2) then step = 32 end  -- 0126 IS_CHAR_OBJECTIVE_PASSED
     if pig ~= nil and ispeddead(pig) then step = 32 end
     if get_game_timer() - t0 > 5000 then step = 32 end  -- таймаут
    end
    set_char_obj_no_obj(pp)  -- 011C SET_CHAR_OBJ_NO_OBJ
    set_fade_color(0, 0, 1)  -- 0169 SET_FADING_COLOUR 0 0 1
    fade(0, 500)  -- 016A DO_FADE 500 0 — затемнение
    while get_fading_status() do wait() end  -- 016B GET_FADING_STATUS
    setcord(pp, 454.4, 31.3, 33.86)  -- 0055 — телепорт на крышу
    setangle(pp, 270)  -- 0171 SET_PLAYER_HEADING
    set_camera_position(468.1, 41.0, 33.5, 0.0, 0.0, 0.0)  -- 015F SET_FIXED_CAMERA_POSITION
    camera_at_point(461.0, 32.8, 33.0, 2)  -- 0160 POINT_CAMERA_AT_POINT
    fade(1, 500)  -- 016A DO_FADE 500 1 — проявление
    if pig ~= nil and not ispeddead(pig) then
     set_char_obj_goto_coord_on_foot(pig, 460.7, 27.5, 33.0)  -- 0211 — Свинья идёт к углу
    end
    -- ждать подхода игрока к 460.7 27.5 (радиус 0.5) или 5000 мс (оригинал @L49dff)
    local t1 = get_game_timer()
    while step == 32 do
     wait()
     local p3 = findplayer()
     if p3 ~= nil then
      local d2 = ped_dist2d_sq(p3, 460.7, 27.5)
      if d2 ~= nil and d2 < 0.25 then step = 33 end  -- 0.5² = 0.25
     end
     if pig ~= nil and ispeddead(pig) then step = 33 end
     if get_game_timer() - t1 > 5000 then step = 33 end  -- таймаут
    end
    if pig ~= nil and not ispeddead(pig) then
     set_char_obj_no_obj(pig)  -- 011C
     turn_player_to_face_char(pig)  -- 0210 TURN_PLAYER_TO_FACE_CHAR
    end
    set_ped_wait_state(pp, 19, 10000)  -- 0372 SET_CHAR_WAIT_STATE 19 10000
    -- аудиодиалоги COL1_5/COL1_1/COL1_6 — в API нет → текст
    printmessage("~w~Pig: So, Cortez sent you. I'm not afraid of a chainsaw.", 4000, 1)  -- GEN1_10
    wait(4000)
    printmessage("~w~Pig: You're making a big mistake, my friend.", 4000, 1)  -- GEN1_06
    wait(4000)
    printmessage("~r~Pig: Boys, kill him!", 3000, 1)  -- GEN1_11
    wait(3000)
    set_ped_wait_state(pp, 0, 100)  -- 0372 — снять wait
    if pig ~= nil and not ispeddead(pig) then
     set_ped_wait_state(pig, 0, 100)
     turn_char_to_face_player(pig)  -- 020F TURN_CHAR_TO_FACE_PLAYER
     set_ped_wait_state(pig, 0, 100)
    end
    if guard1 ~= nil and not ispeddead(guard1) then set_char_obj_no_obj(guard1) end  -- 011C
    -- охрана атакует игрока (01CA SET_CHAR_OBJ_KILL_PLAYER_ON_FOOT)
    if guard1 ~= nil and not ispeddead(guard1) then set_char_obj_kill_player_on_foot(guard1, player) end
    if guard2 ~= nil and not ispeddead(guard2) then set_char_obj_kill_player_on_foot(guard2, player) end
    -- разблокировка игрока
    restore_camera()  -- 02EB RESTORE_CAMERA_JUMPCUT
    ped_frozen(1)  -- 01B4 SET_PLAYER_CONTROL 1 — разморозить
    set_widescreen(false)  -- 02A3 SWITCH_WIDESCREEN 0
    timer_t = get_game_timer()  -- 01BD
    timer_u = get_game_timer()
    pig_phase = 0
    step = 4
    ma_trace("swine: конфронтация — охрана атакует, Свинья бежит по крыше")
   end

   -- === шаг 4: преследование (@L4a290 + @L4a92b) ===
   if step == 4 then
    if pig == nil or ispeddead(pig) then
     -- Свинья УБИТА → провал-эскалация → респрай → M_PASS (@L4a57c/@L4ac69)
     play_one_off_sound(0.0, 0.0, 0.0, 1)  -- 018C ADD_ONE_OFF_SOUND
     printmessage("~r~The swine is dead! The cops are all over you!", 5000, 1)  -- GEN1_19
     wanted_set_no_drop(2)  -- 010E ALTER_WANTED_LEVEL_NO_DROP 2
     set_free_resprays(1)  -- 0335 SET_FREE_RESPRAYS 1
     blip_respray = radar_add_blip_for_coord(327.2, 429.9, 10.3)  -- 018A — блайп мойки
     if blip_pig ~= nil then remove_blip(blip_pig) blip_pig = nil end
     printmessage("~g~Get to the Pay 'n' Spray and respray the car!", 6000, 1)  -- GEN1_21
     step = 5
     ma_trace("swine: Свинья убита — респрай (wanted no-drop 2, блайп мойки)")
    else
     -- блайп на Свинью (один раз)
     if blip_pig == nil and isped(pig) then
      blip_pig = create_marker_actor(pig)  -- 0187 ADD_BLIP_FOR_CHAR
     end
     -- ИИ охраны 1: каждые 500 мс — бежать к игроку / атаковать (оригинал @L4a9a7)
     if get_game_timer() - timer_t > 500 then
      timer_t = get_game_timer()  -- 01BD
      if isped(guard1) and not ispeddead(guard1) then
       local gx, gy = getpedcoordes(pp)  -- 0054 GET_PLAYER_COORDINATES
       if gx ~= nil then
        set_char_obj_run_to_coord(guard1, gx, gy, 11.0)  -- 0239 — бежать к игроку
        set_char_obj_kill_player_on_foot(guard1, player)  -- 01CA — атаковать
       end
      end
     end
     -- ИИ охраны 2 (оригинал @L4aa44)
     if get_game_timer() - timer_u > 500 then
      timer_u = get_game_timer()
      if isped(guard2) and not ispeddead(guard2) then
       local gx2, gy2 = getpedcoordes(pp)
       if gx2 ~= nil then
        set_char_obj_run_to_coord(guard2, gx2, gy2, 11.0)
        set_char_obj_kill_player_on_foot(guard2, player)
       end
      end
     end
     -- state-машина маршрута Свиньи по крыше ($2264: 0→1→2→3→4)
     if isped(pig) and not ispeddead(pig) then
      if pig_phase == 0 then
       set_char_obj_run_to_coord(pig, 466.1, 42.7, 32.0)  -- 0239
       pig_phase = 1
      elseif pig_phase == 1 then
       local d = ped_dist2d_sq(pig, 466.1, 42.7)
       if d ~= nil and d < 1.0 then set_char_obj_run_to_coord(pig, 472.3, 42.8, 32.0) pig_phase = 2 end
      elseif pig_phase == 2 then
       local d = ped_dist2d_sq(pig, 472.3, 42.8)
       if d ~= nil and d < 1.0 then set_char_obj_run_to_coord(pig, 472.5, 35.9, 32.0) pig_phase = 3 end
      elseif pig_phase == 3 then
       local d = ped_dist2d_sq(pig, 472.5, 35.9)
       if d ~= nil and d < 1.0 then set_char_obj_run_to_coord(pig, 464.0, 35.6, 32.0) pig_phase = 4 end
      elseif pig_phase == 4 then
       local d = ped_dist2d_sq(pig, 464.0, 35.6)
       if d ~= nil and d < 1.0 then
        set_char_obj_run_to_coord(pig, 463.0, 31.6, 32.0)
        pig_phase = 5
       end
      elseif pig_phase == 5 then
       local d = ped_dist2d_sq(pig, 463.0, 31.6)
       if d ~= nil and d < 1.0 then
        set_char_obj_no_obj(pig)  -- 011C
        set_ped_stay_when_attacked(pig, true)  -- 0350 SET_CHAR_STAY_IN_SAME_PLACE 1
        pig_phase = 6
        ma_trace("swine: Свинья на углу крыши (463, 31.6) — спрыгнет")
       end
      elseif pig_phase == 6 then
       -- катсцена побега (@L4a5cf): widescreen, камера, Свинья спрыгивает
       pig_phase = 7  -- одноразовая катсцена побега
       ped_frozen(0)  -- 01B4 SET_PLAYER_CONTROL 0
       set_widescreen(true)  -- 02A3
       set_camera_position(470.2, 37.1, 34.2, 0.0, 0.0, 0.0)  -- 015F
       camera_at_point(460.1, 31.0, 32.0, 2)  -- 0160
       set_char_obj_run_to_coord(pig, 457.8, 31.4, 32.0)  -- 0239 — к краю крыши
       local t2 = get_game_timer()
       while true do  -- ждать 3000 мс или достижения края
        wait()
        if not isped(pig) or ispeddead(pig) then break end
        local d2 = ped_dist2d_sq(pig, 457.8, 31.4)
        if d2 ~= nil and d2 < 1.0 then break end
        if get_game_timer() - t2 > 3000 then break end  -- таймаут
       end
       if isped(pig) and not ispeddead(pig) then
        set_fade_color(0, 0, 1)  -- 0169
        fade(0, 500)  -- 016A DO_FADE 500 0
        while get_fading_status() do wait() end  -- 016B
        setpedcoordes(pig, 474.7, 29.9, 11.07)  -- 00A1 SET_CHAR_COORDINATES — спрыгнул
        setangle(pig, 92.5)  -- 0173 SET_CHAR_HEADING
        fade(1, 500)  -- 016A DO_FADE 500 1
       end
       set_camera_position(484.2, 37.0, 12.8, 0.0, 0.0, 0.0)  -- 015F
       camera_at_point(479.7, 27.5, 11.5, 2)  -- 0160
       if isped(pig) and not ispeddead(pig) then
        set_ped_stay_when_attacked(pig, false)  -- 0350 SET_CHAR_STAY_IN_SAME_PLACE 0
        set_char_obj_run_to_coord(pig, 482.8, 30.9, 11.0)  -- 0239 — бежит по пляжу
       end
       printmessage("~w~Pig: I'm getting out of here! You'll never take me alive!", 4000, 1)  -- GEN1_08 (вместо COL1_3)
       wait(4000)
       if isped(pig) and not ispeddead(pig) then
        set_char_obj_run_to_coord(pig, 402.1, -462.2, 10.1)  -- 0239 — побег на другой конец пляжа
       end
       -- телепорт игрока вниз, если он ещё на крыше (оригинал @L4a7fb)
       local pz = nil
       if isped(pp) then local _,_,pz2 = getpedcoordes(pp) pz = pz2 end
       if pz ~= nil and pz > 20.0 then  -- игрок на крыше (z > 20)
        setpedcoordes(pp, 474.7, 29.9, 11.07)  -- 00A1 SET_CHAR_COORDINATES
        setangle(pp, 92.5)  -- 0173
       end
       set_ped_stay_when_attacked(pp, false)  -- 0350
       printmessage("~g~Chase the swine! Don't let him get away!", 4000, 1)  -- GEN1_12 (вместо COL1_7)
       wait(2000)
       restore_camera()  -- 02EB RESTORE_CAMERA_JUMPCUT
       ped_frozen(1)  -- 01B4 SET_PLAYER_CONTROL 1
       set_widescreen(false)  -- 02A3
       pig_escaped = 1
       timer_t = get_game_timer()
       timer_u = get_game_timer()
       ma_trace("swine: Свинья спрыгнула и бежит к точке побега (402.1, -462.2)")
      elseif pig_phase == 7 then
       -- фаза побега: Свинья бежит к (402.1, -462.2), охрана догоняет
       if get_game_timer() - timer_t > 500 then  -- рефреш ИИ свиньи каждые 500 мс
        timer_t = get_game_timer()
        if isped(pig) and not ispeddead(pig) then
         set_char_obj_run_to_coord(pig, 402.1, -462.2, 10.1)  -- 0239 — бежит к точке побега
        end
       end
       if get_game_timer() - timer_u > 500 then  -- рефреш ИИ охраны
        timer_u = get_game_timer()
        if isped(guard1) and not ispeddead(guard1) then
         local gx, gy = getpedcoordes(pp)
         if gx ~= nil then set_char_obj_run_to_coord(guard1, gx, gy, 11.0) set_char_obj_kill_player_on_foot(guard1, player) end
        end
        if isped(guard2) and not ispeddead(guard2) then
         local gx2, gy2 = getpedcoordes(pp)
         if gx2 ~= nil then set_char_obj_run_to_coord(guard2, gx2, gy2, 11.0) set_char_obj_kill_player_on_foot(guard2, player) end
        end
       end
       -- проверка побега: LOCATE_CHAR_ANY_MEANS_3D 398.3 -469.8 11.9 15 15 15
       if isped(pig) and not ispeddead(pig) then
        local d = ped_dist2d_sq(pig, 398.3, -469.8)
        if d ~= nil and d < 225.0 then  -- 15² = 225
         step = 7  -- ПРОВАЛ: Свинья убежала
         ma_trace("swine: Свинья достигла точки побега (398.3, -469.8) — ПРОВАЛ")
        end
       end
      end
     end
    end
   end

   -- === шаг 5: респрай — игрок в авто должен доехать до мойки (@L4b200) ===
   if step == 5 then
    if is_ped_in_car(pp) then  -- 00E0 IS_PLAYER_IN_ANY_CAR
     if respray_msg == 1 then respray_msg = 0 end
     if get_player_wanted_level() == 0 then  -- респрай сработал (wanted 2→0)
      if blip_respray ~= nil then remove_blip(blip_respray) blip_respray = nil end
      set_free_resprays(0)  -- 0335 SET_FREE_RESPRAYS 0
      wait(4000)  -- оригинал: WAIT 4000 после HAS_RESPRAY_HAPPENED
      step = 6  -- M_PASS
      ma_trace("swine: респрай сработал (wanted 2→0) — M_PASS")
     end
    else
     if respray_msg == 0 then
      printmessage("~g~You need a car to lose the cops! Steal one!", 5000, 1)  -- GEN1_20
      respray_msg = 1
     end
    end
   end

   -- === шаг 6: УСПЕХ — Свинья убита и респрей пройден (@L4b318) ===
   if step == 6 then
    if blip_pig ~= nil then remove_blip(blip_pig) blip_pig = nil end
    if blip_respray ~= nil then remove_blip(blip_respray) blip_respray = nil end
    if blip_site ~= nil then remove_blip(blip_site) blip_site = nil end
    if isped(pig) then remove_ped(pig) pig = nil end  -- 009B DELETE_CHAR
    if isped(guard1) then remove_ped(guard1) guard1 = nil end
    if isped(guard2) then remove_ped(guard2) guard2 = nil end
    ped_road_on(479.9, -1.4, 11.0, 450.3, 59.5, 40.0)  -- 022A SWITCH_PED_ROADS_ON
    set_free_resprays(0)  -- 0335 (снять)
    release_weather()  -- 01B7 RELEASE_WEATHER
    ma_trace("swine: МИССИЯ ВЫПОЛНЕНА (Свинья убита + респрей)")
    miss(250)  -- M_PASS 250 + ADD_SCORE 250 + REGISTER_MISSION_PASSED + CLEAR_WANTED_LEVEL
    -- одежду создавать ПОСЛЕ miss(): destroy() внутри end_mission() удаляет
    -- все пикапы скрипта (pickupsids) — награда исчезла бы сразу (055B)
    create_pickup_clothes(8, 414.3, 1042.0, 25.4)  -- 055B CREATE_CLOTHES_PICKUP 8 (значок-костюм)
    step = nil
    break
   end

   -- === шаг 7: ПРОВАЛ — Свинья убежала (@L4b307) ===
   if step == 7 then
    if blip_pig ~= nil then remove_blip(blip_pig) blip_pig = nil end
    if blip_respray ~= nil then remove_blip(blip_respray) blip_respray = nil end
    if blip_site ~= nil then remove_blip(blip_site) blip_site = nil end
    if isped(pig) then remove_ped(pig) pig = nil end
    if isped(guard1) then remove_ped(guard1) guard1 = nil end
    if isped(guard2) then remove_ped(guard2) guard2 = nil end
    ped_road_on(479.9, -1.4, 11.0, 450.3, 59.5, 40.0)  -- 022A
    set_free_resprays(0)  -- 0335
    release_weather()  -- 01B7
    ma_trace("swine: МИССИЯ ПРОВАЛЕНА — Свинья сбежала (M_FAIL)")
    end_mission("mission failed!")  -- M_FAIL
    step = nil
    break
   end

  end

  -- пост-цикловая очистка (смерть/арест)
  if blip_site ~= nil then remove_blip(blip_site) blip_site = nil end
  if blip_pig ~= nil then remove_blip(blip_pig) blip_pig = nil end
  if blip_respray ~= nil then remove_blip(blip_respray) blip_respray = nil end
  if isped(pig) then remove_ped(pig) pig = nil end
  if isped(guard1) then remove_ped(guard1) guard1 = nil end
  if isped(guard2) then remove_ped(guard2) guard2 = nil end
  if roof1 ~= nil then remove_obj(roof1) roof1 = nil end
  if roof2 ~= nil then remove_obj(roof2) roof2 = nil end
  if roof3 ~= nil then remove_obj(roof3) roof3 = nil end
  if roof4 ~= nil then remove_obj(roof4) roof4 = nil end
  set_free_resprays(0)
  release_weather()
  step = nil
 end
end
end
