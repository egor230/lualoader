require("lualoader/mod")

-- ============================================================================
-- МИССИЯ "Riot" ("Мятеж") — перенос ОРИГИНАЛА (97 миссий, поток LAWYER4).
--
-- ПОТОК ОРИГИНАЛА:
-- 1. Миссия начинается у ОФИСА КЕНА (118.0, -825.9, 10.5). Значок ставит
--    штатная обёртка Star_mission_marker (C++ short-range блайп, пересоздаётся
--    для повтора автоматически). Поверх него из Lua ставится дальнобойный
--    спрайт-блайп 9 (Lawyer), чтобы старт было видно на радаре издалека.
-- 2. SET_PLAYER_MOOD 1 60000, REQUEST_ANIMATION 'RIOT'. Два блайпа:
--    магазин Рафаэля — СПРАЙТ-БЛАЙП (radar_set_coord_blip + radar_set_blip_sprite
--    28, значок одежды; оригинал ADD_SPRITE_BLIP_FOR_COORD ... 28 $59);
--    стройплощадка — обычный розовый квадрат (ADD_BLIP_FOR_COORD 299 -314.6
--    11.4 $19). Никаких сфер-заливок: сфера только У ОДЕЖДЫ радиусом 3.
-- 3. Переодевание у Рафаэля (GOSUB L48df8): пешком у маркера → 0352 UNDRESS
--    'PLAYER3' + 0353 DRESS (= set_skin(player, "PLAYER3")), блайп снят.
-- 4. Подъезд к стройке (LOCATE_PLAYER_ANY_MEANS_3D 70/70/30) → сцена бунта:
--    21 пикетчик (SPANDXA/SPANDXB), 3 GDA, 3 грузовика SPAND, 2 объекта -114.
--    Один раз печатается LAW4_13 "Уничтожь грузовики".
-- 5. БОЙ (L4725d): здоровье ≤99 → в бой; умер → intensity -6; укушено>3 или
--    убито>2 → вся толпа (GOSUB L48970), $2205=1.
-- 6. ЭСКАЛАЦИЯ (L47dfc→L47f89): толпа взбешена → ОТКРЫТЬ ВОРОТА $-33
--    SLIDE_OBJECT 298,-313.6,11 → 304,-313.6,11 (ворота стройки), GDA-боссы
--    получают personality 30 + threat_search, LAW4_14 ($2206=1).
--    Ворота ищутся по координатам (get_obj_in_cord) — модель -33 встречается
--    по всей карте, find_object_by_model вернёт первый попавшийся!
-- 7. Победа — 3 грузовика уничтожены → miss(1000). Провал — M_FAIL ($1778
--    SET_OBJECT_COORDINATES обратно 298,-313.6,11 = закрыть ворота).
-- ============================================================================

function main()нет
local riot_start_blip = nil -- дальнобойная метка старта (спрайт 9), пока миссия не началась

while true do wait() local player = findplayer()

 -- C++ Star_mission_marker ставит короткобойную метку (видна только вблизи).
 -- Добавляем дальнобойный спрайт-блайп на те же координаты — логику старта
 -- (заморозку/фейд/проверку у маркера) не трогаем, только индикация на радаре.
 if riot_start_blip == nil then
  riot_start_blip = radar_set_coord_blip(0, 118.0, -825.9, 10.5, 2, 2)
  if riot_start_blip ~= nil then radar_set_blip_sprite(riot_start_blip, 9) end
 end

 if Star_mission_marker(9, 118.0, -825.9, 10.5)
 then
  if riot_start_blip ~= nil then remove_blip(riot_start_blip) riot_start_blip = nil end
  -- Обёртка (mod.lua:1410) гасит экран и размораживает игрока.
  -- ped_frozen(0)=ЗАМОРОЗИТЬ, ped_frozen(1)=РАЗМОРОЗИТЬ (races() мод.lua:401/414).
  ped_frozen(1)
  fade(1, 1200)
  wait(1200)

  local step = 1
  local dressed = 0
  local bitten = 0       -- сколько пикетчиков игрок уже «укусил» ($2212)
  local killed = 0       -- сколько пикетчиков умерло ($2198)
  local escalated = 0    -- толпа взбешена ($2205)
  local riot_intensity = 127
  local gates_open = 0   -- ворота стройки открыты ($2206)
  local law4_13_shown = 0 -- LAW4_13 печатается один раз при подъезде близко
  local agit1_drop = 0
  local agit2_drop = 0

  set_wanted(0)
  showtext(" Riot", 500, 1)
  ma_trace("riot: СТАРТ у офиса Кена (118.0, -825.9, 10.5)")

  -- Катсцена LAW_4 → короткий текст
  printmessage("~w~Ken: Tommy! The workers at Avery's site have gone on the rampage!", 5000, 1)
  wait(2000)
  printmessage("~w~Ken: Get some work clothes from Rafael's so they don't recognize you!", 6000, 1)
  wait(2000)

  -- 04E3 SET_PLAYER_MOOD 1 60000, 04ED REQUEST_ANIMATION 'RIOT'
  set_ped_mood(1, 60000)
  request_animation("RIOT")

  -- МАРКЕР у Рафаэля: СПРАЙТ-БЛАЙП 28 (значок одежды) + малая сфера 3.0
  local dress_blip = radar_set_coord_blip(0, 95.4, -1135.5, 9.4, 2, 2)
  if dress_blip ~= nil then radar_set_blip_sprite(dress_blip, 28) end
  local dress_sphere = create_sphere(95.4, -1136.2, 10.5, 3.0)

  -- МАРКЕР стройплощадки: обычный розовый квадрат, БЕЗ сферы (сферу на 70
  -- убрали — она «залила» весь квартал и была видна из офиса).
  local site_blip = radar_set_coord_blip(0, 299.0, -314.6, 11.4, 2, 2)

  printmessage("~g~Get the work outfit at Rafael's, then hit the construction site.", 6000, 1)
  ma_trace("riot: блайпы Рафаэля (спрайт 28) и стройки созданы, сферы не дикие")

  local rioters = {}
  local bite_flag = {}
  local dead_flag = {}
  local truck1, truck2, truck3
  local mt1, mt2, mt3
  local obj1, obj2
  local agit1, agit2, leader
  local gate

  while true == Getflagmission() do wait()
   local pp = findplayer()
   if pp == nil then pp = player end

   -- ---- ШАГ 1А: переодевание у Рафаэля (пешком, радиус 1.2) ----
   -- (оригинал GOSUB @L48df8: LOCATE_PLAYER_ON_FOOT 1.2 → $2213=1)
   if step == 1 and dressed == 0 and not is_ped_in_car(pp)
    and ped_in_point_in_radius(pp, 95.4, -1136.2, 10.5, 1.5, 1.5, 3.0) then
    set_skin(pp, "PLAYER3")
    dressed = 1
    if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
    if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
    printmessage("~g~You look like a worker. ~y~Now hit the construction site!", 5000, 1)
    ma_trace("riot: Томми переоделся в рабочего у Рафаэля ($2213=1)")
   end

   -- ---- ШАГ 1Б: подъезд к стройплощадке (любым транспортом, 70/70/30) ----
   -- (оригинал L46bee: LOCATE_PLAYER_ANY_MEANS_3D 70 70 30)
   if step == 1 and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 70.0, 70.0, 30.0) then
    if site_blip ~= nil then remove_blip(site_blipriot_intensity) site_blip = nil end
    if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
    if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end

    -- Коктейли Молотова — основное оружие
    Giveweaponped(player, 6, "molotov")
    set_current_weapon_ped(player, 15)

    -- 0552 SET_RIOT_INTENSITY 127 — уровень беспорядков
    set_riot_intensity(riot_intensity)

    -- 3 грузовика рабочих (MODEL_SPAND=213), углы как в оригинале
    truck1 = Createcar("MODEL_SPAND", 286.7, -302.1, 11.9) if truck1 ~= nil then setangle(truck1, 270) end
    truck2 = Createcar("MODEL_SPAND", 312.2, -304.7, 11.9) if truck2 ~= nil then setangle(truck2, 90) end
    truck3 = Createcar("MODEL_SPAND", 312.2, -300.7, 11.9) if truck3 ~= nil then setangle(truck3, 90) end
    if truck1 ~= nil then mt1 = create_marker(truck1) end
    if truck2 ~= nil then mt2 = create_marker(truck2) end
    if truck3 ~= nil then mt3 = create_marker(truck3) end

    -- 2 объекта -114 + 035D MAKE_OBJECT_TARGETTABLE (как в оригинале)
    obj1 = create_obj(-114, 309.3, -298.6, 10.9) if obj1 ~= nil then make_obj_targettable(obj1, true) end
    obj2 = create_obj(-114, 284.3, -312.4, 10.9) if obj2 ~= nil then make_obj_targettable(obj2, true) end

    -- ВОРОТА стройки: объект -33, создан глобально в 00 - Initial.txt:739 на
    -- (298,-313.6,11). Модель -33 есть в нескольких местах карты, поэтому
    -- ищем ПО КООРДИНАТАМ (get_obj_in_cord), а не по модели!
    gate = get_obj_in_cord(298.0, -313.6, 11.0, 8.0)
    ma_trace("riot: ворота -33 найдены по координатам")

    -- 21 ПИКЕТЧИК (CREATE_CHAR 4 112/113), КАЖДОМУ как оригинал L46c6c
    for i, p in ipairs({
     {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
     {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8},
     {296.2,-325.3},{299.3,-329.3},{303.8,-329.7},{308.5,-328.4},
     {310.2,-324.2},{305.6,-322.2},{300.0,-317.5},{294.9,-317.6},
     {291.0,-320.0},{293.0,-324.8},{291.6,-317.2},{301.1,-324.0},
     {297.6,-327.649}
    }) do
     local skin = (i % 2 == 1) and "SPANDXA" or "SPANDXB"  -- оригинал: черед 112/113
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

    -- 3 ЗАЧИНЩИКА GDA (CREATE_CHAR 5 91), углы 180
    agit1 = Createped("GDA", 301.2, -311.5, 10.9) if agit1 ~= nil then setangle(agit1, 180) end
    agit2 = Createped("GDA", 303.2, -311.5, 10.9) if agit2 ~= nil then setangle(agit2, 180) end
    leader = Createped("GDA", 304.0, -298.7, 10.9) if leader ~= nil then setangle(leader, 180) end
    -- 01B2 GIVE_WEAPON_TO_CHAR 4 0 → nitestick (тип 4)
    if agit1 ~= nil then Giveweaponped(agit1, 0, "nitestick") set_current_weapon_ped(agit1, 4) end
    if agit2 ~= nil then Giveweaponped(agit2, 0, "nitestick") set_current_weapon_ped(agit2, 4) end
    -- бригадир: 0223 SET_CHAR_HEALTH 70 + GIVE_WEAPON 17 300 (colt45) + 0350 STAY
    if leader ~= nil then
     sethealth(leader, 70)
     Giveweaponped(leader, 300, "colt45")
     set_current_weapon_ped(leader, 17)
     set_ped_stay_when_attacked(leader, true)
    end
    -- всех трёх можно ранить только игроком (02A9, как в оригинале)
    if agit1 ~= nil then set_ped_only_damaged_by_player(agit1, true) end
    if agit2 ~= nil then set_ped_only_damaged_by_player(agit2, true) end
    if leader ~= nil then set_ped_only_damaged_by_player(leader, true) end

    printmessage("~g~Destroy the workers' trucks to break the strike!", 6000, 1)
    printmessage("~r~Workers: This is our site! Get out!", 5000, 1)
    printmessage("~r~Workers: Burn everything! Strike forever!", 5000, 1)
    ma_trace("riot: сцена создана (3 SPAND, 2 obj -114, 21 пикетчик, 3 GDA); бунт")
    step = 3
   end

   -- ---- ШАГ 2: LAW4_13 — игрок БЛИЗКО к стройке (LOCATE 20 20 10) ----
   -- (оригинал @L4849f: LOCATE_PLAYER_ANY_MEANS_3D 299 -314.6 11.4 20 20 10)
   if step == 3 and law4_13_shown == 0
    and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 20.0, 20.0, 10.0) then
    law4_13_shown = 1
    printmessage("~r~Destroy the trucks! Now!", 8000, 1)
   end

   -- ---- ШАГ 3: БОЙ (главный цикл оригинала @L4725d) ----
   if step == 3 then
    -- жив? здоровье ≤99 и не «укушен» и толпа не взбешена → в бой
    -- (053D CLEAR_WAIT + 01CA KILL_PLAYER_ON_FOOT)
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
       -- умер пикетчик → SET_RIOT_INTENSITY -= 6 (оригинал L49043)
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
       ped_search_threat(r, 16)              -- 011A (прохожие)
       ped_search_threat(r, 1)               -- 011A (игрок)
      end
     end
     escalated = 1
     printmessage("~r~They've seen you! Burn the trucks now!", 4000, 1)
     ma_trace("riot: вся толпа взбешена ($2205=1)")
    end

    -- ЭСКАЛАЦИЯ (оригинал L47dfc-L47f89): толпа взбешена ($2205==1) →
    -- ОТКРЫТЬ ВОРОТА (SLIDE_OBJECT $1778 304 -313.6 11), GDA-боссы
    -- personality 30 + threat_search, LAW4_14, $2206=1. Один раз.
    if escalated == 1 and gates_open == 0 then
     gates_open = 1
     -- ворота: 298,-313.6,11 → 304,-313.6,11 со скоростью 0.1, коллизия выкл
     if gate ~= nil then
      move_obj(gate, 304.0, -313.6, 11.0, 0.1, 0.1, 0.0, 0)
      ma_trace("riot: ворота открываются (SLIDE_OBJECT 304 -313.6 11)")
     end
     if agit1 ~= nil and isped(agit1) then
      set_ped_personality(agit1, 30)
      ped_search_threat(agit1, 16)
      ped_search_threat(agit1, 1)
     end
     if agit2 ~= nil and isped(agit2) then
      set_ped_personality(agit2, 30)
      ped_search_threat(agit2, 16)
      ped_search_threat(agit2, 1)
     end
     if leader ~= nil and isped(leader) then
      ped_search_threat(leader, 1)
      set_ped_accuracy(leader, 10)
     end
     printmessage("~r~Lawyers and cops are on their way! Get the trucks!", 8000, 1)
    end

    -- Убито >14 → SET_RIOT_INTENSITY 0 (оригинал L47cac)
    if killed > 14 then
     set_riot_intensity(0)
    end

    -- Зачинщик умер → колт из трупа: 04A5 + 032B CREATE_PICKUP_WITH_AMMO 274 3 17
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

    -- Все 3 грузовика уничтожены → ПОБЕДА $1000
    if (truck1 == nil or iscardead(truck1))
    and (truck2 == nil or iscardead(truck2))
    and (truck3 == nil or iscardead(truck3)) then
     if mt1 ~= nil then remove_blip(mt1) mt1 = nil end
     if mt2 ~= nil then remove_blip(mt2) mt2 = nil end
     if mt3 ~= nil then remove_blip(mt3) mt3 = nil end
     -- 04EF REMOVE_ANIMATION 'RIOT' + SET_RIOT_INTENSITY 0 (очистка L4864f)
     remove_animation("RIOT")
     set_riot_intensity(0)
     printmessage("~g~The strike is broken! Avery's site is secure.", 4000, 1)
     wait(4000)
     for _, r in ipairs(rioters) do if isped(r) then remove_ped(r) end end
     if agit1 ~= nil and isped(agit1) then remove_ped(agit1) end
     if agit2 ~= nil and isped(agit2) then remove_ped(agit2) end
     if leader ~= nil and isped(leader) then remove_ped(leader) end
     if truck1 ~= nil and isvehicle(truck1) then remove_car(truck1) end
     if truck2 ~= nil and isvehicle(truck2) then remove_car(truck2) end
     if truck3 ~= nil and isvehicle(truck3) then remove_car(truck3) end
     if obj1 ~= nil then remove_obj(obj1) end
     if obj2 ~= nil then remove_obj(obj2) end
     ma_trace("riot: МИССИЯ ВЫПОЛНЕНА (3 грузовика уничтожены; ворота: " .. tostring(gates_open) .. ")")
     miss(1000)
     step = nil
     break
    end
   end

  end

  -- Пост-цикловая очистка: смерть/арест (destroy() снимает педов/авто) —
  -- остаются блайпы, сфера, уровень беспорядков и, если ворота открывались,
  -- сами ворота в открытом положении. Вернуть всё как было.
  if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
  if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
  if site_blip ~= nil then remove_blip(site_blip) site_blip = nil end
  if riot_start_blip ~= nil then remove_blip(riot_start_blip) riot_start_blip = nil end
  if step == 3 then set_riot_intensity(0) end
  -- M_FAIL (оригинал @L485a1): если ворота успели открыться — закрыть обратно
  -- (SLIDE_OBJECT назад в 298,-313.6,11). set-координат в API нет, используем
  -- move_obj — ворота приедут на место.
  if gates_open == 1 and gate ~= nil then
   move_obj(gate, 298.0, -313.6, 11.0, 0.15, 0.15, 0.0, 0)
   ma_trace("riot: ворота закрыты (M_FAIL)")
  end
  step = nil
 end
end
end
