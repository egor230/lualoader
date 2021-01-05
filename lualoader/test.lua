require("lualoader/mod")
function f(n)
givemoney(n)
printmessage("~r~end script 1", 3400,1 )
end
function main()
while true do wait()-- задержка.
  player = findplayer()-- получить игрока.
 if Keypress("VK_H") -- если клавиша H.
  then  
  x,y,z = getcoordinates_on_y(player, 5)Giveweaponped(player,600,"uzi")-- дать педу оружие.
	car = Createcar("MODEL_PCJ600",x,y,z) 
wait(800)	set_radio(9,1)-- выключить радио.
	 setcarangle(car, getpedangle(player)  )-- уст угол авто
	 -- remove_car(car)
-- 
 end
	if Keypress("VK_S") -- если клавиша H.
     then  --show_save_menu()
	 givemoney(33)
	 car = ped_car(player)
	 dam = getmodelindex(car)
	 printmessage("damage = "..tostring(string.format("%.d", dam)), 3300,1 )
		 wait(4000)
	--setcord(player, 325.5, 1199.1, 27.6)
  
   end 
  end
 
end
 	-- while true do wait() dam = getcardamage(car)
	     -- if dam ~= 0
       -- then printmessage("damage = "..tostring(string.format("%.f", dam)), 3300,1 )
		-- wait(4000)
	-- end
	-- end
  -- x,y,z = getcoordinates_on_y(player, 5)
   
  -- set_traffic(0)
	-- car = Createcar("MODEL_SENTXS",x,y,z) --set_camera_on_car(car, 18,2)
	-- while true do wait()
	-- x,y,z = getcarcoordinates_on_y(car,30)
	-- x,y,z = find_road_for_car(x,y,z)
	-- sp = create_sphere(x,y,z, 3)
	-- blip = createmarker(1,2,x,y,z)
	-- cardrive(car,x,y,z)
	
	-- while not  car_in_point_in_radius(car, x,y,z, 3.0) do wait(100) end
	-- remove_sphere(sp)
	-- remove_blip(blip)
	-- wait(1000)
	-- end
	-- printmessage("~r~end script 1", 3400,1 )
  -- ped = Createped("HMYST", x,y,z)
-- ped_rebuff(ped, 1)-- отвечать на атаки.
-- set_ped_stats_to(ped,16)-- уст враждебность педа.
-- ped_search_threat(ped,1)-- пед ищет угрозу.  
	-- set_radio(9,1)-- выключить радио.
  -- x,y,z = getcoordinates_on_y(player, 10)
	-- car = Createcar("MODEL_PCJ600",x,y,z) 
	-- wait(1000)
 -- ped_attack_car(ped, car)
	-- remove_car(car)
	-- Giveweaponped(player,600,"uzi")-- дать педу оружие.
      -- create_newthread(f,1000)
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
