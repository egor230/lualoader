require("lualoader/mod")

-- ============================================================================
-- МИССИЯ "Riot" ("Мятеж") — ИДЕАЛЬНЫЙ перенос ОРИГИНАЛА (97 миссий, LAWYER4).
--
-- ПОТОК ОРИГИНАЛА:
-- 1. SMM у офиса Кена Розенберга (95.4, -1135.5, 9.4) → катсцена LAW_4
-- 2. После катсцены: SET_PLAYER_MOOD 1 60000 (=set_ped_mood(1, 60000));
--    ДВА глобальных блайпа: офис Кена (спрайт 28) и стройплощадка (299, -314.6)
-- 3. Игрок у офиса пешком (радиус 1.2, GOSUB @L48df8) → меняет скин на рабочего
-- 4. Едет на стройплощадку → при приближении 70м (L46c6c) — сцена бунта:
--    21 пикетчик (SPANDXA/SPANDXB) + 3 GDA + 3 SPAND-грузовика + 2 объекта -114
-- 5. БОЙ (L4725d): здоровье пикетчика ≤99 → он в бой (CLEAR_WAIT_STATE +
--    KILL_PLAYER_ON_FOOT); умер пикетчик → SET_RIOT_INTENSITY -6;
--    укушено>3 или умерло>2 → все в бой (GOSUB L48970: personality 30,
--    THREAT 16+1); зачинщик умер → из трупа падает колт (04A5 + 032B).
-- 6. Победа: все 3 грузовика мертвы → $1000.
--
-- V3 (18.09.2026): подключены 9 новых C++-функций (раздел 2.2
-- КАК_ПЕРЕНОСИТЬ_МИССИИ.md): set_ped_wait_state, set_ped_personality,
-- set_ped_only_damaged_by_player, is_ped_health_greater, make_obj_targettable,
-- get_dead_char_pickup_coords, set_ped_mood, request_animation, remove_animation.
--
-- Каркас: step 0(старт)→1(пикап/поездка)→2(подъезд→сцена)→3(бой)→4(победа)
-- ============================================================================

function main()
while true do wait() local player = findplayer()

 -- ============================================================
 -- ЭТАП 0: СТАРТ У ОФИСА КЕНА (95.4, -1135.5, 9.4)
 -- ============================================================
 if Star_mission_marker(9, 95.4, -1135.5, 9.4)
 then
  local step
  local ken_blip, site_blip
  local clothes_pickup
  local truck1, truck2, truck3
  local mt1, mt2, mt3
  local obj1, obj2
  local agit1, agit2, leader
  local rioters = {}
  local bite_flag = {}     -- пикетчик уже «укушен» игроком (флаг N@ в оригинале)
  local dead_flag = {}     -- пикетчик уже посчитан мёртвым ($2225-$2245)
  local agit1_drop, agit2_drop -- колт из трупа зачинщика уже создан ($2246/$2247)
  local bitten, killed     -- счётчики $2212 и $2198
  local escalated          -- взбешена вся толпа ($2205)
  local dressed            -- игрок переоделся у Кена ($2213)
  local riot_intensity
  step = nil
  dressed = 0
  escalated = 0
  bitten = 0
  killed = 0
  agit1_drop = 0
  agit2_drop = 0
  riot_intensity = 127
  set_wanted(0)
  showtext(" Riot", 500, 1)
  ma_trace("riot: СТАРТ у офиса Кена")

  -- Катсцена LAW_4 (Кен + Эйвери) — текст вместо видео
  printmessage("~w~Ken: Tommy, the workers on Avery's site are rioting!", 5000, 1)
  wait(2000)
  printmessage("~w~Avery: They're destroying everything! You gotta stop 'em!", 5000, 1)
  wait(2000)
  printmessage("~w~Ken: We'll get you some clothes there so they don't spot you.", 6000, 1)

  -- 04E3 SET_PLAYER_MOOD 1 60000 (оригинал после катсцены) — настроение игрока
  set_ped_mood(1, 60000)

  -- 04ED REQUEST_ANIMATION 'RIOT' — толпа использует анимации бунта
  request_animation("RIOT")

  -- ПИКАП РАБОЧЕЙ ОДЕЖДЫ у офиса Кена (модель 2 = рабочий костюм)
  clothes_pickup = create_pickup_clothes(2, 97.5, -1133.6, 10.4)

  -- БЛАЙП 1: Офис Кена (спрайт 28 = миссия/контакт) — виден глобально
  ken_blip = radar_set_coord_blip(0, 95.4, -1135.5, 9.4, 2, 2)
  if ken_blip ~= nil then radar_set_blip_sprite(ken_blip, 28) end

  -- БЛАЙП 2: Стройплощадка (спрайт 19 = цель/квадрат) — виден глобально
  site_blip = radar_set_coord_blip(0, 299.0, -314.6, 11.4, 2, 2)
  if site_blip ~= nil then radar_set_blip_sprite(site_blip, 19) end

  printmessage("~g~Get the worker outfit at Ken's, then go to the construction site.", 6000, 1)
  ma_trace("riot: блайпы созданы, ждём пикап/подъезд")
  step = 1

  while true == Getflagmission() do wait()

   -- ---- STEP 1: параллельно — пикап одежды ИЛИ подъезд к стройке ----
   local at_ken = ped_in_point_in_radius(player, 97.5, -1133.6, 10.4, 3.0, 3.0, 2.0)
   local at_site = ped_in_point_in_radius(player, 299.0, -314.6, 11.4, 70.0, 70.0, 30.0)

   -- 1А: подобрали пикап одежды у Кена (оригинал LOCATE у офиса 1.2)
   if step == 1 and at_ken and clothes_pickup ~= nil then
    remove_pickup(clothes_pickup)
    clothes_pickup = nil
    if ken_blip ~= nil then radar_clear_blip(ken_blip) ken_blip = nil end
    -- Оригинал: UNDRESS_CHAR 'PLAYER3' + DRESS_CHAR → мена скина на рабочего
    local player_ped = findplayer()
    if player_ped ~= nil then set_skin(player_ped, "SPANDXA") end
    dressed = 1
    printmessage("~g~Outfit changed! ~y~Now drive to the construction site.", 5000, 1)
    ma_trace("riot: рабочая одежда надета ($2213=1)")
   end

   -- 1Б: подъехали к стройплощадке (любым способом, радиус 70м как в оригинале)
   if step == 1 and at_site then
    if site_blip ~= nil then radar_clear_blip(site_blip) site_blip = nil end
    if ken_blip ~= nil then radar_clear_blip(ken_blip) ken_blip = nil end
    -- Сцена создаётся в темноте (SMM уже затемнил экран)
    Giveweaponped(player, 6, "molotov")       -- коктейли Молотова
    set_current_weapon_ped(player, 15)         -- достать молотов (тип 15)

    -- 0552 SET_RIOT_INTENSITY 127 — уровень беспорядков (как в оригинале)
    set_riot_intensity(riot_intensity)

    -- 3 грузовика рабочих (MODEL_SPAND = 213), углы как в оригинале
    truck1 = Createcar("MODEL_SPAND", 286.7, -302.1, 11.9) if truck1 ~= nil then setangle(truck1, 270) end
    truck2 = Createcar("MODEL_SPAND", 312.2, -304.7, 11.9) if truck2 ~= nil then setangle(truck2, 90) end
    truck3 = Createcar("MODEL_SPAND", 312.2, -300.7, 11.9) if truck3 ~= nil then setangle(truck3, 90) end
    if truck1 ~= nil then mt1 = create_marker(truck1) end
    if truck2 ~= nil then mt2 = create_marker(truck2) end
    if truck3 ~= nil then mt3 = create_marker(truck3) end

    -- 2 объекта -114 (20..22) + 035D TARGETTABLE (как в оригинале)
    obj1 = create_obj(-114, 309.3, -298.6, 10.9) if obj1 ~= nil then make_obj_targettable(obj1, true) end
    obj2 = create_obj(-114, 284.3, -312.4, 10.9) if obj2 ~= nil then make_obj_targettable(obj2, true) end

    -- 21 ВЗБУНТОВАВШИЙСЯ РАБОЧИЙ — СПЕЦ-СКИНЫ SPANDXA/SPANDXB (как в оригинале,
    -- CREATE_CHAR 4 112/113). КАЖДОМУ (точь-в-точь оригинал L46c6c):
    --   01ED CLEAR_CHAR_THREAT_SEARCH → ped_clean_threat
    --   0243 SET_CHAR_PERSONALITY 16   → set_ped_personality(16)
    --   02A9 SET_CHAR_ONLY_DAMAGED_BY_PLAYER 1 → set_ped_only_damaged_by_player(true)
    --   0372 SET_CHAR_WAIT_STATE 29 99999999 → set_ped_wait_state(29, 99999999)
    local riot_pos = {
     {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
     {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8},
     {296.2,-325.3},{299.3,-329.3},{303.8,-329.7},{308.5,-328.4},
     {310.2,-324.2},{305.6,-322.2},{300.0,-317.5},{294.9,-317.6},
     {291.0,-320.0},{293.0,-324.8},{291.6,-317.2},{301.1,-324.0},
     {297.6,-327.649}  -- 21-й (в оригинале $2197, модель 112)
    }
    for i, p in ipairs(riot_pos) do
     local skin = (i % 2 == 1) and "SPANDXA" or "SPANDXB"  -- чередуем 112/113
     local r = Createped(skin, p[1], p[2], 10.9)
     if r ~= nil then
      ped_clean_threat(r)
      set_ped_personality(r, 16)
      set_ped_only_damaged_by_player(r, true)
      set_ped_wait_state(r, 29, 99999999)
      table.insert(rioters, r)
      bite_flag[#rioters] = 0
      dead_flag[#rioters] = 0
     end
    end

    -- 3 зачинщика GDA (CREATE_CHAR 5 91), углы 180 как в оригинале
    agit1 = Createped("GDA", 301.2, -311.5, 10.9) if agit1 ~= nil then setangle(agit1, 180) end
    agit2 = Createped("GDA", 303.2, -311.5, 10.9) if agit2 ~= nil then setangle(agit2, 180) end
    leader = Createped("GDA", 304.0, -298.7, 10.9) if leader ~= nil then setangle(leader, 180) end
    -- 01B2 GIVE_WEAPON_TO_CHAR 4 0 — тип 4 nitestick (дубинка), как в оригинале
    if agit1 ~= nil then Giveweaponped(agit1, 0, "nitestick") set_current_weapon_ped(agit1, 4) end
    if agit2 ~= nil then Giveweaponped(agit2, 0, "nitestick") set_current_weapon_ped(agit2, 4) end
    -- 0223 SET_CHAR_HEALTH 70 + 01B2 GIVE_WEAPON 17 300 (colt45) + 0350 STAY
    if leader ~= nil then
     sethealth(leader, 70)
     Giveweaponped(leader, 300, "colt45")
     set_current_weapon_ped(leader, 17)
     set_ped_stay_when_attacked(leader, true)
    end
    -- зачинщиков тоже можно ранить только игроком (в оригинале 02A9 у всех 3)
    if agit1 ~= nil then set_ped_only_damaged_by_player(agit1, true) end
    if agit2 ~= nil then set_ped_only_damaged_by_player(agit2, true) end
    if leader ~= nil then set_ped_only_damaged_by_player(leader, true) end

    fade(1, 1200) wait(1200)                    -- засветка
    printmessage("~g~Destroy the workers' trucks to end the strike!", 6000, 1)
    printmessage("~r~Workers: This is our site! Get out! We won't back down!", 5000, 1)
    printmessage("~r~Workers: Burn it all! Strike forever!", 5000, 1)
    ma_trace("riot: сцена создана (3 SPAND, 2 obj -114, 21 рабочий, 3 GDA); бунт начался")
    step = 3
   end

   -- ---- STEP 3: БОЙ (главный цикл оригинала @L4725d) ----
   if step == 3 then
    -- 21 пикетчик: жив? здоровье ≤99 (получил урон) и не «укушен» и толпа ещё
    -- не взбешена → пикетчик идёт убивать игрока (053D CLEAR_WAIT + 01CA ON_FOOT)
    for i, r in ipairs(rioters) do
     if isped(r) then
      if not ispeddead(r) then
       if bite_flag[i] == 0 and escalated == 0
        and (not is_ped_health_greater(r, 99)) then
        bite_flag[i] = 1
        bitten = bitten + 1
        clean_ped_wait(r)
        set_char_obj_kill_player_on_foot(r, player)
       end
      else
       -- пикетчик умер? считаем один раз → SET_RIOT_INTENSITY -= 6 (GOSUB L49043)
       if dead_flag[i] == 0 then
        dead_flag[i] = 1
        killed = killed + 1
        riot_intensity = riot_intensity - 6
        if riot_intensity < 0 then riot_intensity = 0 end
        set_riot_intensity(riot_intensity)
       end
      end
     end
    end

    -- Взбесить всю толпу (L47c68 → GOSUB L48970): укушено>3 или убито>2
    if escalated == 0 and (bitten > 3 or killed > 2) then
     for _, r in ipairs(rioters) do
      if isped(r) then
       clean_ped_wait(r)                     -- 053D
       set_ped_personality(r, 30)            -- 0243
       ped_search_threat(r, 16)              -- 011A (граждане)
       ped_search_threat(r, 1)               -- 011A (игрок)
      end
     end
     escalated = 1
     printmessage("~r~They've seen you! Burn the trucks now!", 4000, 1)
     ma_trace("riot: вся толпа взбешена ($2205=1)")
    end

    -- Климат бунта: убито >14 → SET_RIOT_INTENSITY 0 (оригинал L47cac)
    if killed > 14 then
     set_riot_intensity(0)
    end

    -- Зачинщик 1 умер и ещё не падал колт → 04A5 GET_DEAD_CHAR_PICKUP_COORDS
    -- + $2250 += 0.5 → 032B CREATE_PICKUP_WITH_AMMO 274 3 17
    if agit1_drop == 0 and agit1 ~= nil and isped(agit1) and ispeddead(agit1) then
     local dx, dy, dz = get_dead_char_pickup_coords(agit1)
     if dx ~= nil then
      create_weapon_pickup(274, 3, 17, dx, dy, dz + 0.5)
      agit1_drop = 1
     end
    end
    if agit2_drop == 0 and agit2 ~= nil and isped(agit2) and ispeddead(agit2) then
     local dx, dy, dz = get_dead_char_pickup_coords(agit2)
     if dx ~= nil then
      create_weapon_pickup(274, 3, 17, dx, dy, dz + 0.5)
      agit2_drop = 1
     end
    end

    -- ---- STEP 3 → 4: все 3 грузовика уничтожены -> ПОБЕДА $1000 ----
    if (truck1 == nil or iscardead(truck1))
    and (truck2 == nil or iscardead(truck2))
    and (truck3 == nil or iscardead(truck3)) then
     if mt1 ~= nil then remove_blip(mt1) mt1 = nil end
     if mt2 ~= nil then remove_blip(mt2) mt2 = nil end
     if mt3 ~= nil then remove_blip(mt3) mt3 = nil end
     -- 04EF REMOVE_ANIMATION 'RIOT' + SET_RIOT_INTENSITY 0 (очистка L4864f)
     remove_animation("RIOT")
     set_riot_intensity(0)
     printmessage("~g~The strike is broken! Avery's site is secure.", 4000, 1) wait(4000)
     for _, r in ipairs(rioters) do if isped(r) then remove_ped(r) end end
     if agit1 ~= nil and isped(agit1) then remove_ped(agit1) end
     if agit2 ~= nil and isped(agit2) then remove_ped(agit2) end
     if leader ~= nil and isped(leader) then remove_ped(leader) end
     if truck1 ~= nil and isvehicle(truck1) then remove_car(truck1) end
     if truck2 ~= nil and isvehicle(truck2) then remove_car(truck2) end
     if truck3 ~= nil and isvehicle(truck3) then remove_car(truck3) end
     if obj1 ~= nil then remove_obj(obj1) end
     if obj2 ~= nil then remove_obj(obj2) end
     -- 0109 ADD_SCORE + 1000 (M_PASS), 0318 REGISTER, 030C PLAYER_MADE_PROGRESS
     ma_trace("riot: МИССИЯ ВЫПОЛНЕНА (3 грузовика уничтожены)")
     miss(1000)
     step = nil
     break
    end
   end

  end

  step = nil
  -- Смерть/арест — C++-сторож Getflagmission вызовет destroy()
 end
end
end