require("lualoader/mod")

local function spawn_trucks()

 local m1, m2, m3
 if t1 ~= nil then m1 = create_marker(t1) end  -- маркер над грузовиком (mod.lua:709 → create_marker_car → ADD_BLIP_FOR_CAR)
 if t2 ~= nil then m2 = create_marker(t2) end
 if t3 ~= nil then m3 = create_marker(t3) end
 ma_trace("riot: 3 грузовика SPAND созданы, маркеры навешены (главный поток)")
 local step = 1          -- этап миссии: 1 = переодевание/подъезд к стройке, 3 = бой, nil = конец
 local bitten = 0        -- сколько пикетчиков игрок уже «укусил» ($2212)
 local killed = 0        -- сколько пикетчиков умерло ($2198)
 local escalated = 0     -- толпа взбешена ($2205)
 local riot_intensity = 127  -- уровень беспорядков (0552), убывает по убийствам пикетчиков
 local gates_open = 0    -- ворота стройки открыты ($2206); 25.10: счётчик кадров убран — ворота ставятся мгновенно (setobjcoordes)
 local law4_13_shown = 0  -- LAW4_13 печатается один раз при подъезде близко
 local frame = 0          -- счётчик кадров боя (step==3): для низкочастотной диагностики — боевой цикл не оставляет трейсов, иначе краш/ошибку не локализовать
 local agit1_drop = 0    -- колт уже выпал из трупа агитатора 1
 local agit2_drop = 0    -- колт уже выпал из трупа агитатора 2

 local rioters = {}      -- 21 пикетчик (SPANDXA/SPANDXB)
 local bite_flag = {}    -- bite_flag[i] — «укушен» ли пикетчик i
 local dead_flag = {}    -- dead_flag[i] — умер ли пикетчик i
 local hp_prev = {}      -- hp_prev[i] — HP пикетчика i в прошлом кадре (float): любой урон = падение HP
  local truck1, truck2, truck3  -- 3 грузовика рабочих (MODEL_SPAND=213) — главная цель миссии
 local mt1, mt2, mt3  -- маркеры грузовиков; снимаются главным циклом В КАДР уничтожения (потоки-наблюдатели убраны — см. spawn_trucks)
 local obj1, obj2        -- 2 объекта -114 (приоритетные цели, 035D)
 local agit1, agit2, leader  -- 3 зачинщика GDA: 2 с nitestick + бригадир с colt45
 local gate
 for i, p in ipairs({
  {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
  {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8},
  {296.2,-325.3},{299.3,-329.3},{303.8,-329.7},{308.5,-328.4},
  {310.2,-324.2},{305.6,-322.2},{300.0,-317.5},{294.9,-317.6},
  {291.0,-320.0},{293.0,-324.8},{291.6,-317.2},{301.1,-324.0},
  {297.6,-327.649}
 }) do  -- 21 ПИКЕТЧИК (CREATE_CHAR 4 112/113), каждому — как оригинал L46c6c
  local skin = (i % 2 == 1) and "SPANDXA" or "SPANDXB"  -- оригинал: чередование 112/113
  local r = Createped(skin, p[1], p[2], 10.9)  -- пед на координате пикетчика
  if r ~= nil then
   ped_clean_threat(r)  -- 01ED CLEAR_CHAR_THREAT_SEARCH
   set_ped_personality(r, 16)  -- (оригинал 0243 SET_CHAR_PERSONALITY 16: обычный горожанин)
   set_ped_only_damaged_by_player(r, true)  -- (оригинал 02A9 SET_CHAR_ONLY_DAMAGED_BY_PLAYER: ранить может только игрок)
   set_ped_wait_state(r, 29, 99999999)  -- (оригинал 0372 SET_CHAR_WAIT_STATE 29: стоит, пока игрок не ранит)
   table.insert(rioters, r)
   bite_flag[#rioters] = 0
   dead_flag[#rioters] = 0
   hp_prev[#rioters] = getpedhealthf(r)
  end
 end
end
 -- return t1, t2, t3, m1, m2, m3
-- end

-- start_riot(pl, rioters, agit1, agit2, leader, guard_target): ЭСКАЛАЦИЯ мятежа —
-- вызывается ОДИН раз при первом уроне/убийстве рабочего. Одежду определяет
-- dressed_as_worker() (по скину, не по флагу). guard_target — таблица
-- «охранник → цель-мятежник», мутируется тут и читается главным циклом для
-- перебинда, если цель умрёт; escalated = 1 выставляет ВЫЗЫВАЮЩИЙ.
-- Ветвление: БЕЗ одежды — толпа И охранники валят ИГРОКА (01CA); В ОЖЕДЕ рабочего —
-- рабочие дерутся друг с другом (0216 по «соседу»), а охранники GDA бегут и бьют
-- мятежников: 0350-флаг бригадира («держать позицию») снимается, personality 30 +
-- threat 16 (прохожие) и ЯВНАЯ цель 0216 — без явной цели ped_search_threat не
-- атакует (урок 6-й ревизии).
local function start_riot(pl, rioters, agit1, agit2, leader, guard_target)
 local guards = {}  -- список охранников БЕЗ дыр (ipairs остановился бы на nil-дыре и пропустил бы бригадира)
 if agit1  ~= nil then guards[#guards + 1] = agit1  end
 if agit2  ~= nil then guards[#guards + 1] = agit2  end
 if leader ~= nil then guards[#guards + 1] = leader end

 if not dressed_as_worker() then  -- (БЕЗ одежды: рабочие поняли, что игрок — чужак → ВСЯ толпа валит НА ИГРОКА)
  for _, r in ipairs(rioters) do
   if isped(r) then
    clean_ped_wait(r)                        -- 053D: снять wait-state 29 (иначе педы стоят и не дерются)
    set_ped_personality(r, 30)               -- 0243
    ped_search_threat(r, 16)                 -- 011A (прохожие)
    ped_search_threat(r, 1)                  -- 011A (игрок — главная цель)
    set_char_obj_kill_player_on_foot(r, pl)  -- 01CA: ЯВНАЯ цель — игрок
   end
  end
  for _, g in ipairs(guards) do  -- охранники БЕЗ одежды открывают огонь ПО ИГРОКУ, мятежников не трогают
   if isped(g) then
    set_ped_personality(g, 30)
    ped_search_threat(g, 1)
    if g == leader then set_ped_accuracy(leader, 10) end  -- стрелок с colt45
    set_char_obj_kill_player_on_foot(g, pl)  -- 01CA: ЯВНАЯ цель — игрок
   end
  end
  --printmessage("~r~They've seen you! Get out!", 4000, 1)
 else  -- (В одежде рабочего: рабочие дерутся ДРУГ С ДРУГОМ; охранники добивают мятежников)
  local n = #rioters
  for i, r in ipairs(rioters) do
   if isped(r) then
    clean_ped_wait(r)                         -- 053D
    set_ped_only_damaged_by_player(r, false)  -- 02A9: снять «только игрок ранит» — иначе урон между рабочими не проходит
    set_ped_personality(r, 30)                -- 0243
    ped_search_threat(r, 16)                  -- 011A (прохожие — бьют друг друга)
    local t = rioters[(i % n) + 1]            -- цель-«сосед»: каждый рабочий СРАЗУ целится в соседа
    if t ~= nil and isped(t) and t ~= r then
     set_char_obj_kill_char_on_foot(r, t)     -- 0216: гарантирует реальное нападение на другого рабочего
    end
   end
  end
  for _, g in ipairs(guards) do  -- охранники В ОДЕЖДЕ бегут к мятежниками и бьют их (агитаторы — с nitestick, бригадир — с colt45)
   if isped(g) then
    set_ped_stay_when_attacked(g, false)      -- 0350: СНЯТЬ «держать позицию» (бригадир сто́ял с 0350=true — не бежал на цель)
    set_ped_personality(g, 30)                -- 0243
    ped_search_threat(g, 16)                  -- 011A (прохожие = мятежники в рабочей одежде)
    if g == leader then set_ped_accuracy(leader, 10) end
    local gt = nil                            -- первый живой мятежник — цель для охранника
    for _, r in ipairs(rioters) do if isped(r) and not ispeddead(r) then gt = r break end end
    if gt ~= nil then
     set_char_obj_kill_char_on_foot(g, gt)    -- 0216: ЯВНАЯ цель — мятежник (без неё threat_search не атакует)
     guard_target[g] = gt                     -- запомнить для перебинда в главном цикле, если цель умрёт
    end
   end
  end
  --printmessage("~r~The workers have turned on each other! Get the trucks!", 4000, 1)
 end
 ma_trace("riot: вся толпа взбешена ($2205=1), dressed=" .. tostring(dressed_as_worker()))
end

-- drop_agitator_colt(agit, dropped): зачинщик GDA умер → выпадает его колт из
-- трупа (оригинал 04A5 GET_DEAD_CHAR_PICKUP_COORDS + 032B CREATE_PICKUP_WITH_AMMO
-- 274 3 17). dropped = «колт уже выпал» (1 = да). Возвращает обновлённый флаг:
-- 1, если колт выпал сейчас (или уже был), иначе старое значение.
local function drop_agitator_colt(agit, dropped)
 if dropped == 0 and agit ~= nil and isped(agit) and ispeddead(agit) then
  local dx, dy, dz = get_dead_char_pickup_coords(agit)
  if dx ~= nil then
   create_weapon_pickup(274, 3, 17, dx, dy, dz + 0.5)
   return 1
  end
 end
 return dropped
end
function main()
while true do wait(10)-- задержка.
  player = findplayer()-- получить игрока.
 if Keypress("VK_G") -- если клавиша H.
  then   --Giveweaponped(player,600,"uzi")-- дать педу оружие.
setpedcoordes(player,118.0, -825.9, 10.5)
  --set_skin(player, "PLAYER3") -- Переодевание
  -- spawn_trucks()
 end
 --mod_autoaim("m4", "on")  printmessage("~r~on autoaim", 3400,1 )
 --

 if Keypress("VK_H") -- если клавиша H.
  then  --mod_autoaim("M40", "off") printmessage("~r~off autoaim", 3400,1 )
  -- setpedcoordes(player,286.7, -303.1, 11.9)
  setpedcoordes(player,95.4, -1135.5, 9.4)
 end
 if Keypress("VK_S") -- если клавиша H.
then  show_save_menu()
  end
  if Keypress("VK_Z") -- если клавиша H.
    then    x,y,z = getcoordinates_on_y(player, 5)
     car = Createcar("MODEL_PCJ600",x,y,z)
     Giveweaponped(player,600,"m4")-- дать педу оружие.
    end
  if Keypress("VK_N") -- если клавиша H.
    then    x,y,z = getcoordinates_on_y(player, 5)
    -- car = Createcar("MODEL_PCJ600",x,y,z)
      setpedcoordes(player,286.7, -303.1, 11.9)
    end
  end

end

	-- if Keypress("VK_J") -- если клавиша H.
  --    then Giveweaponped(player,600,"M40")-- дать педу оружие.
  --      x,y,z=getpedcoordinates_on_y(player, 10)
  --      setpedcoordes(player,  -349.0, -427.0, 10.9)
  --   end
-- ped1 = Createped("IGMERC", x,y,z)-- создать педа на координатах.
--   x,y,z=getpedcoordinates_on_y(player, 20)
--   ped_walk_to_point(ped1,x,y,z) wait(4000)
  --Giveweaponped(player,600,"sniper")-- дать педу оружие.
   -- wait(1000)

	-- if Keypress("VK_J") -- если клавиша H.
     -- then -- show_save_menu()
	-- setcord(player, 325.5, 1199.1, 27.6)

   -- end
 -- printmessage("value = "..tostring(string.format("%.d", res)), 3400,1 )
  -- file = io.open('autoaim.txt', 'a') -- открыть файл для записи
  -- file:write(tostring(string.format("%.d", res)))
  -- file:close()
		  -- if Keypress("VK_J") -- если клавиша H.
   -- then  setcord(player, 495.7, -1734.3, 13.7)

  -- end
  -- x,y,z = getcoordinates_on_y(player, 5)
	-- -- car = Createcar("MODEL_SECURICA",x,y,z)
	-- -- setcarangle(car, getpedangle(player))

	-- set_radio(9,1)-- выключить радио.
	-- car = Createcar("MODEL_PCJ600",x,y,z)
-- --	set_skin(player, "IGMIKE")
	 -- -- wait(3100)
	-- -- delete_entity(car)
	-- remove_car(car)
  -- -- set_skin(player, "PLAYER")-- восстановить скин.

	--
	-- set_skin("IGKEN")
-- printmessage("jhgfc",2000,3)
	--ped1 = create_spec_ped("SAM",x,y,z)
      --p =	  Createped("HMYST", x,y,z)

	  -- ped_from_turret_on_car(ped)--убрать педа из турели авто.
      -- destroy()-- удалить все объекты, которые были созданы скриптом.
	  -- printmessage("~r~end script 1", 3400,1 )
-- local ped1 = set_ped_in_car(car, "HFORI",1) -- уст пассажира авто.
	-- car = Createcar("MODEL_PCJ600",x,y,z)
	--
-- local
-- m2 = create_marker(car)
  -- Giveweaponped(player,600,"uzi")-- дать педу оружие.
   -- wait(300)

-- if isvehicle(car)
-- then  m = create_marker_car(car)
-- end

 -- -- pickup = Create_pickup(335,3,606.5, -255.9, 13.8) -- создать пикап

 -- restore_camera()
 -- -- remove_pickup(pickup)
  -- end
-- --  setpedhealth(player, 300)
  -- -- set_camera_on_car(car, 18, 2)
  -- -- set_camera_on_car(car, 18, 2)
   -- -- set_cops_ignored(1)
 --setcord(player, 529.6, 370.1, 10.9)

-- if is_button_pressed(17)
     -- --wait(300)
   -- ped1 = set_ped_in_car(car, "HFORI") -- уст водителя авто.
  -- ped2 = set_ped_in_car(car, "HFORI",0) -- уст пассажира авто.
  -- ped3 = set_ped_in_car(car, "HFORI",1) -- уст пассажира авто.
  -- ped4 = set_ped_in_car(car, "HFORI",2) -- уст пассажира авто.
  --ped = Createped("BMYBB", x,y,z)
--  p1, m1 = Create_ped_and_give_weapon("BMYBB", "uzi", x,y,z)-- создать врага.
--   remove_car(car)
  -- if Keypress("VK_J") -- если клавиша H.
  -- then x,y,z = getcoordinates_on_y(player, 15)
  -- -- Giveweaponped(ped,600,"m4")-- дать педу оружие.
  -- Giveweaponped(player,600,"uzi")-- дать педу оружие.
  -- wait(200)
  -- --addmoney_ped(ped,100)
 -- --  create_shots_on_car(car,1,100 )
 -- --create_shots(ped, x, y, z)
 -- end
	-- x,y,z = getcoordinates_on_y(player, 10)
	--car = Createcar("MODEL_PCJ600",x,y,z)
	--remove_car(car)
	--p1, m1 =
	--set_car_tank(car, 0)

 --ped = Createped("HMYST",x,y,z) --setmoney_ped(ped,100)
-- Giveweaponped(ped,600,"uzi")-- дать педу оружие.
--setmoney_ped(ped, 10)
--wait(300)
-- addmoney_ped(ped,100)
-- wait(200)
-- s = getmoney_ped(p)
 -- printmessage("money = "..tostring(string.format("%.d", s)), 3400,1 )
 -- wait(4500)
-- kill_ped(ped)
--setpedhealth(ped,0)
--setmoney_ped(player, 10)
   -- ped1 = set_ped_in_car(car, "HFORI") -- уст водителя авто.
  -- ped2 = set_ped_in_car(car, "HFORI",0) -- уст пассажира авто.
  -- ped3 = set_ped_in_car(car, "HFORI",1) -- уст пассажира авто.
  -- ped4 = set_ped_in_car(car, "HFORI",2) -- уст пассажира авто.
  -- wait(2200)
  -- all_exit_from_car(car, 4)
    -- while true do wait(100)
  -- local b, p = Ped_in_targetting()
  -- if b
  -- then
  -- --m = create_marker_actor(p)
 -- s = getmoney_ped(p)
 -- printmessage("stam = "..tostring(string.format("%.d", s)), 3400,1 )
 -- wait(4500)
 -- break
 -- end
 -- end
    -- printmessage("yes", 3000,1)
	-- car = Createcar("MODEL_INFERNUS",x,y,z)
   --ped_in_turret_on_car(ped, car, 0.424, -0.067, 1.13, 0, 360, 26)

	-- printmessage("stam = "..tostring(string.format("%.d", stam)), 3400,1 )
 -- if Keypress("VK_J") -- если клавиша H.
   -- then x,y,z = getcoordinates_on_y(player, 5)
   -- gravity_default()
   --    setgravity(0.002)
   -- end
 	-- car = ped_car(player)
	-- f = get_radio_car(car)
	-- givemoney(f)
	-- setstamina_ped(player, 900)

  -- car = Createcar("MODEL_PCJ600",x,y,z)

  -- Giveweaponped(player,600,"colt45","chromegun","uzi")-- дать педу оружие.
 	 -- while true do wait(1010)
	 -- stam = getstamina_ped(player)
     -- printmessage("stam = "..tostring(string.format("%.f", stam)), 1400,1 )
-- end
-- while true do wait(1010)
-- if not is_ped_in_car(player)
 -- then break
 -- else
-- car = ped_car(player)
-- horn = gethorncar(car)
-- printmessage("horn = "..tostring(string.format("%.d", horn)), 1400,1 )


  -- -- setbomb_in_car(car, 1)
  -- -- t = getbomb_in_car(car)
  -- -- givemoney(t)
  -- -- wait(1500)
  -- -- activetbomb(car)
 -- --  ped = Createped("BMOST",x,y,z)

 -- end
-- end	--
-- camera -67.383, -851.841, 11.123  угол 90
-- cord -67.4, -848.5, 10.5 угол 270
-- function f(player)

-- local x1,y1,z1 =  getcameracoordes()--getcoordinates_on_y(player, 10)
-- x1 = tonumber(x1)
-- y1 = tonumber(y1)
-- z1 = tonumber(z1)
-- for i=0, 360,30 do
-- x2, y2 = findpoints(x1,y1, 8, i)
-- camera_at_point(x1, y1, z1, 1)
-- set_camera_position(x2,y2,z1, 0.0, 0.0, 0.0)
-- pickup =Create_weapon_pickup("sniper",3, 30, x2,y2,z1)-- создать пикап оружие.
-- printmessage("x = "..tostring(string.format("%.3f", x2))..", y = "..tostring(string.format("%.3f", y2))..", angle = "..tostring(string.format("%.d", i)), 400,1 )
-- wait(2100)
-- end
-- end
  --set_camera_and_point(-248.9, -1304.0, 11.1, -234.6, -1301.8, 9.1 )

   -- x,y,z = getcoordinates_on_y(player, 10)
   -- ped1 = Createped("HMYST",x,y,z)
   -- ped_aim_at_ped(ped, ped1)

 -- if Keypress(VK_G) -- если клавиша G.
 -- then printmessage("fire bullets included", 3000,1)
 -- while true do wait(100)
 -- local b, p = Ped_in_targetting()
 -- if b
 -- then m = create_marker_actor(p)

 -- while not ped_damage_from_ped(p, player) do wait(200) end
 -- f = createfireonped(p)
 -- wait(6600)
 -- removemarker(m)
 -- remove_fire(f)
-- -- break
 -- end
 -- end
 -- end
 --printmessage("throw current weapon", 3000,1)
 -- if ped_targetting_in_ped(player, ped1)
 -- then
 -- Giveweaponped(ped,600,"uzi","python","chromegun")-- дать педу оружие.
  -- restore_camera()
	 -- set_wanted(6)
	 -- --throw_current_weapon(player)

 -- end
 -- if Keypress(VK_N) -- если клавиша N.
 -- then printmessage("remove current weapon", 3000,1)
 -- remove_current_weapon_ped(player)

 -- end   --
 -- Giveweaponped(player,600,"uzi","python","chromegun")-- дать педу оружие.
  -- if Keypress(VK_G) -- если клавиша G.
 -- then printmessage("throw current weapon", 3000,1)
     -- throw_current_weapon(player)
-- end
 -- if Keypress(VK_N) -- если клавиша N.
 -- then remove_current_weapon_ped(player)
      -- printmessage("remove current weapon", 3000,1)
 -- end
 --remove_current_weapon_ped(player)
   -- x,y,z = getcoordinates_on_y(player, 5)
    -- ped1 = Createped("HMORI",x,y,z)
  -- x,y,z = getcoordinates_on_y(player, 10)
  -- ped = Createped("HMYST",x,y,z)
  -- kill_ped_on_foot(ped, player)
  -- kill_char_any_means(ped1, player)
  -- while true do wait(10)
    -- if ped_damage_from_ped(player, ped1)
	-- then
    -- printmessage("star talking", 3000,1)
	-- break
	-- end
  --ped_shutdown(player)
  -- Giveweaponped(player,600,"uzi")
  -- Giveweaponped(ped1,600,"uzi")
-- Kill_ped_on_foot(ped1, player)
  -- ped_shutdown(ped1)
    --createfireoncords(x,y,z)
  --g =createfireonped(ped1)
  -- wait(600)-- задержка.
-- g = get_ammo_weapon_ped(ped1)
-- g = tonumber(g)
-- printmessage("x = "..tostring(string.format("%.d", g)), 1400,1 )
  -- wait(3600)-- задержка.
  -- -- end
    -- if Keypress(VK_H) -- если клавиша H.
 -- then   Giveweaponped(player,600,"uzi","python","chromegun")-- дать педу оружие.
-- end	--
 -- wait(300)
 -- g = getweaponslot(player,"uzi")
  -- printmessage("x = "..tostring(string.format("%.d", g)), 3400,1 )
  -- x,y,z = getcoordinates_on_y(player, 10)
  -- ped = Createped("HMYST",x,y,z)
  -- Giveweaponped(ped,600,"uzi")
  -- ped_aim_at_ped(ped1, ped)
-- while lualoader == nil do
-- -- g = get_damage_weapon_ped(ped)
-- g =greatefireonped(ped1)
   -- if Keypress(VK_H) -- если клавиша H.
 -- then restore_camera()--
 -- remove_sphere(sp)
-- end	--
 --printmessage("camera", 2000,1)--
 -- setcord(player, -180.1, -1453.9, 8.9)
 -- x,y,z = getcoordinates_on_y(player, 10)
 -- sp = create_sphere(x,y,z,2,0)
 -- for i =0 ,360,45 do
 -- set_angle_camera(x,y,z, 10,i)
 -- wait(2900)
-- end
 --setangle(player,90)

   --  printmessage("angle = ".. tostring( getpedangle(player)), 500, 1)
-- for i=0, 360,30 do
 --wait(1000)

 -- set_camera_position(-67.4, -848.5, 10.5, 0.0, 0.0, 0.0)
 -- camera_at_point(-67.4, -848.5, 10.5, 1) --переместить камеру в координатах.
 -- setcord(player, -260.678, -1214.5, 8.773)
-- set_camera_position(-250.123, -1218.243, 8.773, 0.0, 0.0, 0.0)
-- camera_at_point(-248.406, -1228.094, 8.773, 1)
-- camera_at_point(x1, y1, z, 1)
-- set_camera_position(x,y,z, 0.0, 0.0, 0.0)
 -- cord = "set_camera_position("..tostring(string.format("%.3f", x))..", "..tostring(string.format("%.3f", y))..", " .. tostring(string.format("%.3f", z))..", 0.0, 0.0, 0.0)\n".."camera_at_point("..tostring(string.format("%.3f", x1))..", "..tostring(string.format("%.3f", y1))..", " .. tostring(string.format("%.3f", z))..", 1)"
  -- file = io.open('camera.txt', 'w') -- открыть файл для записи
  -- file:write(cord)
  -- file:close()
  -- printmessage("write cordinat", 3000, 1)
 -- wait(600)

 -- x,y,z = getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.--
 -- printmessage("car", 2000,1)--
-- x,y,z = getcameracoordes()
-- a1="cord "..x..", "..y..", "..z
-- printmessage(a1, 1000,1)
  -- set_camera_position(91.813, -1131.116, 17.251, 0.0, 0.0, 0.0) --уст камеру в координатах.
  -- camera_at_point(92.39, -1131.74, 16.724, 1) --переместить камеру в координатах.
 -- for i=1, 6 do
 -- set_wanted(i)
 -- wait(2500)
 -- end
-- mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.
 --Giveweaponped(player,600,"uzi")-- дать педу оружие.
   -- set_widescreen(true)--
 -- setcord(player,0.0,0.0,0.0)
-- givemoney(3000) setcarcoordes(mycar,0.0,0.0,0.0)
 -- while lualoader == nil do
 -- wait(100)
 -- speed =getcarspeed(mycar)
-- s = tostring(speed)
 -- printmessage(s, 2000,1)--
 --
 --
-- mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.
-- go_to_route(mycar, road)
  -- setpedhealth(player, 300)
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
  -- ped1 = create_spec_ped("SAM", x,y,z) --  --
   -- -- if isped(ped)
-- -- then
-- -- printmessage("ped", 2000,1)
-- -- end
 -- printmessage("star talking", 2000,1)
 -- wait(3000)-- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.--
  -- setpedhealth(ped1, 0)
 -- -- wait(300)
 -- -- ped = Createped("HMYST",x,y,z)
 -- -- setpedhealth(ped, 0)
    -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.--
	-- mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.
 -- ped = Createped("HMYST",x,y,z)
