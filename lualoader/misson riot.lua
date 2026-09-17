require("lualoader/mod")

-- ============================================================================
-- МИССИЯ "Riot" ("Мятеж") — перенос ОРИГИНАЛЬНОЙ миссии GTA Vice City,
-- Source missions/Оригинал игры (97 миссий)/06 - Riot.txt (поток LAWYER4).
--
-- Сюжет: рабочие на стройплощадке взбунтовались и мешают строительству.
-- Кен просит сорвать забастовку — сжечь грузовики рабочих. Игрок подъезжает
-- к стройплощадке, берёт коктейли Молотова и уничтожает 3 грузовика SPAND,
-- отбиваясь от толпы зачинщиков (двое с дубинами, «бригадир» с кольтом).
--
-- Каркас (см. КАК_ПЕРЕНОСИТЬ_МИССИИ.md): значок -> подъезд НА МАШИНЕ
-- (Star_mission_marker высадит игрока и поднимет флаг) -> сцена -> автомат step.
-- ============================================================================

function main()
while true do wait() local player = findplayer()

 -- СТАРТ: игрок подъехал к стройплощадке на машине. Экран ещё тёмный (SMM).
 if Star_mission_marker(9, 299.0, -314.6, 11.4)
 then
  local step
  local truck1, truck2, truck3
  local mt1, mt2, mt3
  local agit1, agit2, leader
  local rioters = {}
  step = nil
  set_wanted(0)                                        -- копы не мешают
  showtext(" Riot", 500, 1)
  ma_trace("riot: МИССИЯ СТАРТ")

  -- коктейли Молотова — основное оружие миссии (в оригинале их уже носил игрок)
  Giveweaponped(player, 6, "molotov")
  set_current_weapon_ped(player, 15)

  -- три грузовика рабочих (в оригинале модель 213 SPAND) — ЦЕЛЬ миссии
  truck1 = Createcar("MODEL_SPAND", 286.7, -302.1, 11.9) if truck1 ~= nil then setangle(truck1, 270) end
  truck2 = Createcar("MODEL_SPAND", 312.2, -304.7, 11.9) if truck2 ~= nil then setangle(truck2, 90) end
  truck3 = Createcar("MODEL_SPAND", 312.2, -300.7, 11.9) if truck3 ~= nil then setangle(truck3, 90) end
  if truck1 ~= nil then mt1 = create_marker(truck1) end
  if truck2 ~= nil then mt2 = create_marker(truck2) end
  if truck3 ~= nil then mt3 = create_marker(truck3) end

  -- взбунтовавшиеся рабочие. В оригинале их 21 на спец-скинах SPANDXA/SPANDXB
  -- (модели 112/113, спец-слоты); в lualoader их не грузим — берём 8 обычных
  -- гражданских мужчин-рабочих.
  for _, p in ipairs({ {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
                       {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8} })
  do
   local r = Createped("HMYST", p[1], p[2], 10.9)
   if r ~= nil then table.insert(rioters, r) end
  end

  -- двое зачинщиков с дубинами (в оригинале weapon 4) у входа на площадку
  agit1 = Createped("GDA", 301.2, -311.5, 10.9)
  agit2 = Createped("GDA", 303.2, -311.5, 10.9)
  if agit1 ~= nil then Giveweaponped(agit1, 1, "golfclub") set_current_weapon_ped(agit1, 3) end
  if agit2 ~= nil then Giveweaponped(agit2, 1, "golfclub") set_current_weapon_ped(agit2, 3) end

  -- «бригадир» с кольтом (colt45, 300 патронов, HP 70), не сходит с места
  leader = Createped("GDA", 304.0, -298.7, 10.9)
  if leader ~= nil then
   sethealth(leader, 70)
   Giveweaponped(leader, 300, "colt45")
   set_current_weapon_ped(leader, 17)
   set_ped_stay_when_attacked(leader, true)
  end

  fade(1, 1200) wait(1200)                             -- засветка: бунт на виду
  printmessage("~g~Destroy the workers' trucks to end the strike!", 6000, 1)
  ma_trace("riot: сцена создана (3 грузовика, 11 рабочих); ждём подхода игрока")
  step = 1

  -- ======================== ОСНОВНОЙ ЦИКЛ ========================
  while true == Getflagmission() do wait()

   -- ЭТАП 1 -> 2: игрок подошёл к технике (или валит с дистанции) — толпа
   -- бросается в атаку.
   if step == 1 and (
       (truck1 ~= nil and iscardead(truck1))
    or (truck2 ~= nil and iscardead(truck2))
    or (truck3 ~= nil and iscardead(truck3))
    or (truck1 ~= nil and ped_in_point_in_radius(player, 286.7, -302.1, 11.9, 14.0, 14.0, 6.0))
    or (truck2 ~= nil and ped_in_point_in_radius(player, 312.2, -304.7, 11.9, 14.0, 14.0, 6.0))
    or (truck3 ~= nil and ped_in_point_in_radius(player, 312.2, -300.7, 11.9, 14.0, 14.0, 6.0))
    or (leader ~= nil and ped_in_point_in_radius(player, 304.0, -298.7, 10.9, 8.0, 8.0, 4.0)))
   then
    for _, r in ipairs(rioters) do
     if isped(r) then set_char_obj_kill_player_any_means(r, player) end
    end
    if agit1 ~= nil and isped(agit1) then set_char_obj_kill_player_any_means(agit1, player) end
    if agit2 ~= nil and isped(agit2) then set_char_obj_kill_player_any_means(agit2, player) end
    if leader ~= nil and isped(leader) then set_char_obj_kill_player_any_means(leader, player) end
    printmessage("~r~They've seen you! Burn the trucks!", 4000, 1)
    ma_trace("riot: step 1 -> 2 (бунт начался, все бросились на игрока)")
    step = 2
   end

   -- ЭТАП 2 -> 3: ВСЕ грузовики уничтожены — УСПЕХ $1000 (как в оригинале).
   if step == 2 and (
       (truck1 == nil or iscardead(truck1))
   and (truck2 == nil or iscardead(truck2))
   and (truck3 == nil or iscardead(truck3)))
   then
    if mt1 ~= nil then remove_blip(mt1) mt1 = nil end
    if mt2 ~= nil then remove_blip(mt2) mt2 = nil end
    if mt3 ~= nil then remove_blip(mt3) mt3 = nil end
    printmessage("~g~The strike is broken!", 4000, 1) wait(4000)
    for _, r in ipairs(rioters) do if isped(r) then remove_ped(r) end end
    if agit1 ~= nil and isped(agit1) then remove_ped(agit1) end
    if agit2 ~= nil and isped(agit2) then remove_ped(agit2) end
    if leader ~= nil and isped(leader) then remove_ped(leader) end
    if truck1 ~= nil and isvehicle(truck1) then remove_car(truck1) end
    if truck2 ~= nil and isvehicle(truck2) then remove_car(truck2) end
    if truck3 ~= nil and isvehicle(truck3) then remove_car(truck3) end
    set_wanted(6)                                        -- вернуть розыск в норму
    ma_trace("riot: МИССИЯ ВЫПОЛНЕНА")
    miss(1000)
    step = nil
    break
   end

  end                                                    -- конец основного цикла

  step = nil
  -- Смерть/арест игрока обрабатывает C++-сторож Getflagmission (destroy()).
 end
end
end
