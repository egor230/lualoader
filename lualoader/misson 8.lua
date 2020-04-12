require("lualoader/mod")

function main()
while lualoader == nil do wait() player = findplayer()-- получить игрока
 if Star_mission_marker(5,-73.5, -1538.0, 10.5) -- создать маркер миссии на карте. 
 then showtext("kill a vacationer", 700,1)-- вывод названия миссии. 
  setcord(player, -74.4, -1532.8, 12.8 ) setangle(player, 209)
  fade(1,5500) --просветления, 600 время.
  wait(3300)
  blip, sp = create_marker_and_sphere(-209.1, -1204.3, 10.5, 3,3.0)-- создать метку на карте с размером и сферу с радиусом.
  printmessage("~r~go to the location, wait for Cortez to leave the hotel.",3000,1)
  step = 0 
  end--
while true == Getflagmission() do wait(10) -- цикл пока статус миссии не изменится.  
 if step == 0 and ped_in_point_in_radius(player, -209.1, -1204.3, 10.5, 2.0,2.0,2.0 )
  then remove_blip(blip) remove_sphere(sp) 
  ped_frozen(0)
 
  load_scene(-142.9, -1145.5, 13.5)
  set_camera_and_point(-176.5, -1118.4, 13.6, -165.7, -1127.5, 13.5 )
  p =  create_spec_ped("IGCOLON", -142.9, -1145.5, 13.5)-- создать спец педа.
  mp = create_marker(p)
  setangle(p, 48.0) -- установить угол для педа.
  set_camera_and_point(-176.5, -1118.4, 13.6, -165.7, -1127.5, 13.5 )
  load_scene(-175.8, -1118.9, 9.9) car = Createcar("MODEL_SENTXS", -175.8, -1118.9, 9.9 ) setangle(car, 309)
  ped1 = set_ped_in_car(car, "HFORI") -- уст водителя авто.
  ped_walk_to_point(p, -171.8, -1121.8, 10.5)
  printmessage("~r~he gets in the car and drives off to the Malibu club, kill him before he gets to the club, he has 2 security guards, they want to kill you!",
  8000,1)
  setcord(p,-159.8, -1134.1, 13.5)
  wait(12000) 
 
  p1 = foel1("HMOBE", player,"m4", -154.8, -1121.5, 13.5)-- создать врага.
  p2 = foel1("HFOST", player,"m4", -170.1, -1132.0, 13.5)-- создать врага.
  ped_sprint_to_point(p1,-182.3, -1129.3, 10.3 )
  ped_sprint_to_point(p2,-182.3, -1129.3, 10.3 )
  wait(3000)
  kill_ped_on_foot(p1, player)
  kill_ped_on_foot(p2, player)
  ped_frozen(1)
  restore_camera()
  step = 1
 end
 if step > 0 -- проверка кортес убит?
 then if not player_defined(p)
 then miss(3000) step= -1
 break
 end
 end
 if step == 1
 then if ped_in_point_in_radius(p, -171.8, -1121.8, 10.5, 2.0,2.0,2.0 )
 then ped_car_as_passenger(p, car)-- кортес садится как пассажир.
 step = 2
 end
 end
 if step == 2 
  then if is_ped_in_car(p)-- если он в авто.
   then setcarspeed(car, 50) setdrivingstyle(car, 3) setcarstrong(car, 1) cardrive(car, 489.1, -93.9, 10.0)-- ехать в Малибу.
   step = 3
   end
 end 
 if step == 3
  then if car_in_point_in_radius(car, 489.1, -93.9, 10.0, 4.0 )-- кортес приехал в Малибу. 
   then end_mission("mission failed!") step = -1
  end
  end
 end   
 end
 end 

 --++pickup = Create_pickup(335,3,x,y,z) -- создать пикап 
 --m = create_marker_pickup(pickup) -- создать маркер над пикапом.
-- car1 = Createcar(MODEL_DELUXO, -244.0, -1221.4, 8.1) setangle(car1, 184.0) -- установить угол для авто.
 -- function Create_foes_in_car()
 -- player = findplayer()-- получить игрока
 -- local car = Createcar(MODEL_FBICAR, 370.6, -1609.7, 16.7)
 -- local m = create_marker(car)

 -- local ped1 = set_ped_in_car(car, "HFORI") -- уст водителя авто.
  -- Giveweaponped(ped1,1300,"m4")-- дать педу оружие.
 -- setpedhealth(ped1, 200)-- уст здоровье педу.
 -- Kill_char_any_means(ped1, player)-- пед хочет убить игрока.

 -- local ped2 = set_ped_in_car(car, "HFORI",0) -- уст пассажира авто.
 -- setpedhealth(ped2, 200)-- уст здоровье педу.
 -- Giveweaponped(ped2,1300,"m4")-- дать педу оружие.
 -- Kill_char_any_means(ped2, player)-- пед хочет убить игрока.
 
 -- local  ped3 = set_ped_in_car(car, "HFORI",1) -- уст пассажира авто.
 -- setpedhealth(ped3, 200)-- уст здоровье педу.
 -- Giveweaponped(ped3,1300,"m4")-- дать педу оружие.
 -- Kill_char_any_means(ped3, player)-- пед хочет убить игрока.

 -- local  ped4 = set_ped_in_car(car, "HFORI",2) -- уст пассажира авто.
 -- setpedhealth(ped4, 200)-- уст здоровье педу.
 -- Giveweaponped(ped4,1300,"m4")-- дать педу оружие.
 -- Kill_char_any_means(ped4, player)-- пед хочет убить игрока.
 -- return car
-- end 
 
   -- if picked_up(pickup) and step == nil
   -- then removemarker(m)  remove_pickup(pickup)  remove_sphere(sp) 
    -- car2 = Create_foes_in_car()--[[создать авто с врагами]]  setangle(car2, 223.0) -- установить угол для авто.
    -- set_wanted(6) printmessage("~r~deliver to the hotel owner",3000,1) 
    -- blip = createmarker(1,2, 606.5, -255.9, 13.8) -- создать маркер на карте. Принимает тип, размер, координаты, id маркера.
    -- sp = create_sphere( 606.5, -255.9, 13.8, 2.0)-- создать сферу.   
    -- setsizemarker(blip,3)--уст размер маркера. Принимает маркер, значение его размера
    -- step = 1
  -- end
  -- if step == 1 and ped_in_point_in_radius(player, 606.5, -255.9, 13.8, 2.0,2.0,2.0) 
   -- then clear_wanted() miss(3000)--миссия выполнена
   -- step = nil   
 -- end
 --end
 
 -- while true == Getflagmission() do  wait() -- цикл пока статус миссии не изменится.--  
  -- if ped_in_point_in_radius(player,95.4, -1136.2, 10.5, 2.0,2.0,2.0) and not is_ped_in_car(player) and step == nil
   -- then 
   
   -- step = 1
   -- end
   -- if step == 1 and ped_in_point_in_radius(player,-251.2, -1360.8, 8.1, 2.0,2.0,2.0) and not is_ped_in_car(player)
   -- then step = 2 
	-- end	    
  -- if step == 2 
   -- then 
   
 -- end
 -- end
 
 
 
 
 -- while true == getflagmission() do  wait()
  -- local mer, car = incar(ped)
  -- if mer == false
 -- then
 -- break
 -- end
 -- end
  -- local car1 = Createcar(MODEL_CHEETAH, -236.6, -1374.6, 10.0)
 -- setangle(car1,  280.3) -- установить угол для авто.
  -- wait(10) 
-- car2 = Create(MODEL_INFERNUS, -240.8, -1355.3, 10.0)
 -- setangle(car2, 99.8) -- установить угол для авто.
  -- wait(10) 
-- car3 = Create(MODEL_STRETCH, -244.2, -1339.2, 10.0)
 -- setangle(car3,  103.8) -- установить угол для авто.
-- car4 = Create(MODEL_BANSHEE, -233.3, -1386.0, 10.0)
 -- setangle(car4,  96.8) -- установить угол для авто.
-- break
 -- if 
 -- then
 -- break
 -- end
  -- if not player_defined(ped) -- если враг убит.
   -- then  miss(3000)-- миссия выполнена сколько денег получено за выполнение.
   -- break   end  end-- 
  -- destroy()-- удалить все объекты, которые были созданы скриптом.	 
  -- ped = create_spec_ped("IGDIAZ", 487.2, -98.8, 10.6 )-- создать педа на координатах.
  -- Giveweaponped(player,600,"uzi")-- дать педу оружие.
  -- Giveweaponped(ped,600,"m4")-- дать педу оружие.
  -- m = create_marker(ped)-- создать маркер над педом.
  -- Kill_ped_on_foot(ped, player)-- пед хочет убить игрока.

--ped_anim(ped, 14)

 -- fade(1,3100) --просветления, 600 время.
   -- ped = Createped("HFOST", 513.7, -51.8, 10.8)-- 
 -- while true == getflagmission() do  wait(100) 
  -- -- if not player_defined(ped) -- если враги убиты.
 -- -- then miss(3000)--миссия выполнена
 -- -- break
 -- -- end 
 -- end
-- destroy()-- удалить все объекты	 





-- local road = {480.0, -198.1, 10.3 --1

   -- ,446.3, -372.3, 9.6 --2
  -- ,363.4, -450.0, 9.4 --3
 
 -- ,117.8, -893.3, 10.2 -- 4
-- ,219.5, -957.8, 9.9 -- 5
-- , 97.9, -1480.6, 9.9 -- 6
-- , 1.6, -1477.8, 9.9, --7
 -- -18.1, -1568.4, 9.9,  --8
 -- -191.7, -1285.7, 9.9, --9
 -- -64.0, -1065.4, 9.9 --10
 -- ,-90.5, -891.5, 9.9,--11
 -- 155.4, -719.2, 9.9, --12
 -- 192.5, -393.5, 10.5, --13
 -- 334.6, -14.1, 10.9,--14
 -- 473.9, 94.4, 10.9,--15
 -- 489.7, 83.2, 10.8, --16
 -- 516.7, -72.5, 10.2,--17
 -- 478.2, -117.4, 10.1--18
 -- } 


-- function main()
-- while lualoader == nil do
 -- wait()
 -- player = findplayer()-- получить игрока
 -- if star_mission(player, "cb") --Keypress(VK_H) 
-- then set_traffic(0) mycar = Createcar(MODEL_STINGER,485.5, -111.2, 10.1) 
 -- putincar(player, mycar) setcarcoordes(mycar, 485.5, -111.2, 10.1)-- установить координаты авто.
 -- setcarangle(mycar, 180.0)-- уст угол авто. 
 
-- car = Createcar(MODEL_SENTXS,478.2, -117.4, 10.1) -- создать авто на 5 впереди. 
  -- mar1 = create_marker_car(car) -- создать маркер над авто
-- ped = create_spec_ped("IGHLARY", 0.0,0.0,0.0) --
 -- putincar(ped, car)--  поместить педа в авто.
 
 -- setcarangle(car, 180.0)-- уст угол авто. 
  -- setcarspeed(car, 25) setdrivingstyle(car, 3) setcarstrong(car, 1) -- wait(3300) -- printmessage("race",2000,1)
 -- restore_camera()
 -- t = races() 
   -- go_to_route(car, road)	
    -- while lualoader == nil do  wait()
    -- b1 = follow_route_for_corona_for_playercar(mycar, road) 
   -- if b1 == true
	   -- then printmessage("you win",2000,1)  miss(money)
       -- break
       -- end 
   -- if finish_road(car, road)
	   -- then printmessage("you lose",2000,1)  end_mission("mission failed!")
       -- break
       -- end 
  -- end  
  -- remove_blip(blip)
  -- stop_timer(t)-- удалить таймер.
  -- destroy()
  -- set_traffic(1) local t = {false, 0, 0, 0, 0, 0, 0, 0,0,0}
  -- draw_corona(t) 
 -- end 
 -- end
 -- end

--setcarcoordes(car, 478.2, -117.4, 10.1)------// установить координаты авто.


-- road = {-141.4, -1454.2, 10.3--,
-- -- -159.2, -1458.0, 10.6,
-- -- -212.4, -1433.0, 8.1,
-- -- -211.6, -1413.8, 8.1
-- }
-- function main()
-- while lualoader == nil do
 -- wait()-- задержка.
  -- player = findplayer()-- получить игрока
  -- if Keypress(VK_H) -- если клавиша H.
 -- then  x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.-- 
	-- mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.
-- go_to_route(mycar, road)
  -- end  
 -- end
-- end
  -- -- setpedhealth(player, 300)
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
 -- printmessage("ok", 2000,1)
 -- set_skin(player, "IGDIAZ")
	--m = create_marker_actor(ped1)
	-- m1 =create_marker(mycar)

 -- wait(900)
 -- Kill_ped_on_foot(ped, player)
 -- for i=1, 1000000 do
 -- ped_atack(ped)
-- end
 -- if isped(ped)
-- then 
-- printmessage("star talking", 2000,1)
-- end 
 
-- Giveweaponped(player,600,"tec9")-- дать педу оружие. 
-- mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.

  -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.
 -- ped = Createped("HMYST",x,y,z) 
-- ped_car_as_driver(ped, mycar)-- пед садится в авто как водитель.
 -- wait(20)-- задержка.
  -- x,y,z =getcoordinates_on_y(player,15)-- получить координаты на 5 м впереди.
 -- ped1 = Createped("HFOBE",x,y,z) 
 
-- ped_car_as_passenger(ped1, mycar)-- пед садится в авто как пассажир.
  -- Hold_cellphone(player, 1)-- понять телефон.
  -- wait(3200)
  -- printmessage("star talking", 2000,1)
  -- Hold_cellphone(player, 0)-- положить телефон.
   -- mycar = Createcar(MODEL_SENTXS,x,y,z) -- создать авто на 5 впереди.
   -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 10 м впереди. 
   -- ped = Createped("HMYST",x,y,z) 
   -- Giveweaponped(ped,600,"buddyshot")-- дать педу оружие. 
   -- ped_attack_car(ped, mycar)--  пед атакует  авто.
 --putincar(ped, mycar)--  поместить педа в авто.
 -- while lualoader == nil do wait()
-- x =car_lastweapondamage(mycar)
-- x = tonumber(string.format("%d", x))
 -- a1="cord "..x
 -- printmessage(a1, 3000,1)
-- end 
 -- while lualoader == nil do wait()
-- b, mycar = incar(player)
-- if b == true
-- then
-- x =getcar_model(mycar)
-- x = tonumber(string.format("%d", x))
 -- a1="cord "..x
 -- printmessage(a1, 3000,1)
-- end 
-- end
--cheat("zx")
 -- if Keypress(VK_T) -- если клавиша T.
 -- then
 -- timer_donw(60,"R_TIME",1)-- Таймер на уменьшения
 -- end
 
 -- if Keypress(VK_U) -- если клавиша U.
 -- then
 -- stop_timer("R_TIME")-- удалить таймер.
 -- end
 -- if Keypress(VK_H) -- если клавиша H.
 -- then x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 10 м впереди.
  -- z = z-1
  -- obj = Create_obj(350, x,y,z) -- создать объект(бочка).
  -- printmessage("the bomb is planted", 2000,1)
 -- while countdown(8) do wait()-- таймер на 8 секунд. 
      
-- end
 -- create_explosion(3,x,y,z ) --Создать взрыв на координатах.
-- remove_obj(obj)
-- end

 -- if Keypress(VK_G) -- если клавиша G.
-- then printmessage("star timer", 2000,1)
-- t =turn_default_timer(true,"TIMER:")
-- end 
 -- if Keypress(VK_N) -- если клавиша N.
-- then printmessage("stop timer", 2000,1)
-- turn_default_timer(false,t)

 -- for i=1,5 do
 -- setcarsiren(mycar, i)
 -- wait(2600)
 -- end

 -- setcarsiren(mycar, 0)


 -- if Keypress(VK_G) -- если клавиша H.
-- then printmessage("star timer", 2000,1)
-- t =turn_default_timer(true,"TIMER:")
-- end 
 -- if Keypress(VK_N) -- если клавиша H.
-- then printmessage("stop timer", 2000,1)
-- turn_default_timer(false,t)

-- end 
-- end end
-- end

--open_doors = coroutine.wrap(function(car)
-- while true == getflagmission() do wait() 
  -- coroutine.yield()
  -- local health = getcarhealth(car)
 -- if health < 800 and status == nil
 -- then printmessage("open doors car", 3500,1)
 -- opendoorcar(car, DRL) opendoorcar(car, DRR)
 -- status = 1
 -- for i = 1, 30 do x,y,z = getcarcoordinates_on_y(car,-3)
  -- wait(600)  z = z -0.9 create_money_pickup(2000, x,y,z) 
  -- coroutine.yield()
 -- end 
 -- end
 -- end
 -- end
 -- ) 
-- exit_foel = coroutine.wrap(function(car,ped)
-- while true == getflagmission() do wait()  coroutine.yield()  
  -- local health = getcarhealth(car)-- получить здоровье авто.
 -- if health < 500 
 -- then printmessage("~r~i kill you", 2500,1)
   -- Giveweaponped(ped,600,"buddyshot")-- дать педу оружие. 
 -- exitcar(ped)-- выход педа из авто.
 -- Kill_ped_on_foot(ped, player)-- пед хочет убить игрока.
 -- break
 -- end
 -- end
 -- end
 -- )
 
-- function main()
-- while lualoader == nil do wait()
 -- player = findplayer()-- получить игрока.
 -- if  star_mission(player,"nm")-- чит-код для активации миссии.
 -- then fade(0,200) -- затенение, 200 время.
 -- showtext("collection car", 500,1)-- вывод названия миссии.
   -- Giveweaponped(player,600,"uzi")-- дать педу оружие. 
   -- setpedcoordes(player, -212.4, -1433.0, 8.1)-- переместить игрока в координаты.
   -- setpedangle(player, 220.0)-- уст угол педа.
 -- mycar = Createcar(MODEL_PCJ600, 97.1, -1520.8, 10.0) -- создать авто. 
  -- setcarangle(mycar, 169)-- уст угол авто игрока.
  -- putincar(player, mycar)--  поместить педа в авто.
  
 -- car = Createcar(MODEL_SECURICA, 86.4, -1590.9, 10.0, 265) -- создать авто. 
  -- setcarangle(car, 270) -- уст угол авто педа. 
  -- m = create_marker_car(car)-- создать маркер над авто.
  -- ped = set_ped_in_car(car, "WMYSK" )-- уст педа водителя авто.
 -- setcarspeed(car, 75) setdrivingstyle(car, 2) setcarstrong(car, 1)

   -- wait(1900)-- задержка.
  -- fade(1,2600) --просветления, 2600 время.
  -- printmessage("~r~kill driver and car!",4000,1)
  -- newthread(checkmission, player) -- в новом потоке, постоянная проверка жив ли игрок?
 -- while true == getflagmission() do wait() 
  -- exit_foe(car, ped) -- потока выхода педа из авто. 
  -- open_doors(car) -- выпадения денег.
 -- if follow_route(car, road) -- ехать по маршруту.
 -- then  end_mission("mission failed!")--миссия провалена.
 -- break
 -- end
 -- if 0 == getcarhealth(car) and 0 == getpedhealth(ped)-- если враг убит и машина уничтожена.
 -- then miss(100) -- миссия выполнена дать 100$
 -- break
 -- end 
 -- end
-- destroy()-- удалить все объекты
-- end end
-- end
-- while lualoader == nil do wait()
 -- player = findplayer()-- получить игрока.
 -- if  star_mission(player,"mn")-- чит-код для активации миссии.
 -- then fade(0,600) -- затенение, 600 время.
 -- showtext("collection car", 500,1)-- вывод названия миссии.
   -- Giveweaponped(player,600,"uzi")-- дать педу оружие. 
 -- mycar = Createcar(MODEL_PCJ600, 97.1, -1520.8, 10.0) -- создать авто 
  -- setcarangle(mycar, 169)
  -- putincar(player, mycar)
  
 -- car = Createcar(MODEL_SECURICA, 86.4, -1590.9, 10.0, 265) -- создать авто 
 -- m = create_marker_car(car)
  -- setcarangle(car, 270)
 -- ped = set_ped_in_car(car, "WMYSK" )
-- setcarspeed(car, 15) setdrivingstyle(car, 2) setcarstrong(car, 1)
    -- wait(1900)-- задержка.  
 -- fade(1,2600) --просветления, 600 время.

 -- newthread(checkmission, player) -- в новом потоке, постоянная жив ли игрок?
 -- --printmessage("main", 6500,1)
 -- while  true == getflagmission() do  wait()
   -- if follow_route(car, road) 
    -- then end_mission("mission failed!")
    -- break
	-- end
	-- if 0 == getcarhealth(car) or 0 == getpedhealth(ped)
	-- then miss(100) -- дать 100$
	-- break
     -- end 
 -- end 
 -- removemarker(m)
 -- remove_ped(ped)
 -- remove_car(car)
 -- end
 -- end

   -- setpedcoordes(player, -212.4, -1433.0, 8.1)-- переместить игрока в координаты.
   -- setpedangle(player, 220.0)-- уст угол педа.
-- printmessage("star timer", 2000,1)
-- t = star_timer("R_TIME",1,1)

-- wait(4100) 
-- printmessage("yes stop_timer", 3000,1)
-- stop_timer(t)
-- end
 
 
 -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди от игрока.
  -- mycar = Createcar(MODEL_SENTXS,x,y,z) -- создать авто на 5 впереди.
-- putincar(player, mycar)-- переместить педа в авто
-- wait(600)
   -- while lualoader == nil do  wait()
   -- if follow_route_for_corona_for_playercar(mycar, road) 
    -- then miss(100) -- дать 100$
    -- break
     -- end 

 -- end 
 
 -- function main()
-- while lualoader == nil do
 -- wait()-- задержка.
 -- player = findplayer()-- получить игрока
 -- if Keypress(VK_H) -- если клавиша H.

-- end 
-- end
-- end

-- end 
-- then  t={"yes"}
-- while lualoader == nil do
 -- wait()-- задержка.
-- print_front(t)
 -- Giveweaponped(player,300,"ingramsl")-- дать педу оружие.   
 -- setpedhealth(player,200)-- уст 200 здоровье педу
 -- setarmour(player,200)-- уст 200 броню педу
 -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди от игрока.
 -- mycar = Createcar(MODEL_SENTXS,x,y,z) -- создать авто на 5 впереди.
--cheat("zx")
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- ped = Createped("HMYST",x,y,z) 
 -- if isped(ped)
 -- then
 -- printmessage("yes", 3000,1)
 -- end
 -- if isped(mycar)
 -- then
 -- printmessage("yes", 3000,1)
 -- setpedhealth(mycar,10)
 -- else 
 -- printmessage("not", 3000,1)
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
-- ped = Createped("HMYST",x,y,z)
-- m = create_marker_actor(ped)
  -- Kill_ped_on_foot(ped, player)
 -- end
 -- wait()
  -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.
 -- ped1 = Createped("HFOBE",x,y,z)
 -- m1 = create_marker_actor(ped1)
 -- Kill_char_any_means(ped1, player)
   -- printmessage("yes", 3000,1)
 --set_traffic(0) -- end
-- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.
-- mycar = Createcar(MODEL_SENTXS,x,y,z) -- создать авто на 5 впереди.

-- setcarspeed(mycar, 25)-- уст скорость авто.
-- setdrivingstyle(mycar, 2)-- уст стиль вождения.
-- while true  do wait() 
-- if follow_route(mycar, road) -- ехать по маршруту.
 -- then 
 -- break
 -- end 
-- end
 -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.
-- mycar = Createcar(MODEL_SENTXS,x,y,z) -- создать авто на 5 впереди.
  -- ped1 = set_ped_in_car(mycar, "HMOST") -- уст водителя авто.
  -- end
 -- if Keypress(VK_G) --cheat("zx")
-- then 
 -- wait(200)
 -- setcartask(mycar, 5)
  -- --setcaraction(mycar, 9,2000)
-- end
-- mycar = findcar(mycar)
-- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.
-- obj = Create_obj(577, x,y,z) -- создать объект.
 -- marobj = create_marker_obj(obj)-- создать маркер над объектом
--x,y,z =getcoordinates_on_y(player,20)-- получить координаты на 5 м впереди.
-- for i=1,360 do--while lualoader == nil do 
-- wait(9)
-- angle = getobjangle(obj)
-- printmessage(string.format("%.1f", angle), 3000,1)
  -- rotate_obj(obj)
-- end
--set_wanted(3)
 -- move_obj(obj, x,y,z,0.0,0.0,0.01,0)
-- z=z+10
 -- x,y,z=getobjcoordes(obj)-- получить координаты объекта.
 -- x = tonumber(string.format("%.1f", x))
 -- y = tonumber(string.format("%.1f", y))
 -- z = tonumber(string.format("%.1f", z))
 -- a1="cord "..x.." "..y.." "..z
 -- printmessage(a1, 3000,1)
 -- ped = Createped("HFOST",x,y,z)-- 
-- mycar = Createcar(MODEL_ENFORCER,x,y,z) -- создать авто на 5 впереди.

  -- ped1 = set_ped_in_car(mycar, "HMOST") -- уст водителя авто.
-- setcaraction(mycar, 2, 20000)  
-- Giveweaponped(player,300,"tec9")-- дать педу оружие. 
--  mar1 = create_marker_actor(ped) -- создать маркер педа.
-- if isped(ped)
-- then printmessage("yes", 3000,1)
-- else  printmessage("dead", 3000,1)
-- end
--end
--destroy() 
 -- if  not player_defined(player)-- 
 -- then 
 -- printmessage("dead", 3000,1)

-- end
-- x,y = getworldcoordped(player,1.0,7)
--mycar = Createcar(MODEL_ENFORCER,x,y,z) -- создать авто на 5 впереди.  
 -- ped = Createped("HFOST",x,y,z)
 -- Giveweaponped(ped,tec9,100)
-- tw = Get_type_weapon_ped(ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000,1)
--kill_ped_on_foot(ped2, ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000)
 -- x,y = getworldcoordped(player,2.0,8)  
 -- ped2 = Createped("HFYBU",x,y,z)
--is_current_weapon_ped(ped,22)-- проверить пед держит в руках это оружие?
 -- if keypress(VK_H)-- клавиша h.
 -- then

 -- if keypress(VK_H)-- клавиша h.
 -- then
-- Giveweaponped(player,tec9,100)
-- while keypress(VK_H) do wait(1) end
  -- x,y,z =getcoordes(player)
 -- x,y = getworldcoordped(player,1.0,6)  
 -- ped = Createped("HFOST",x,y,z)
-- end 
-- Giveweaponped(ped,buddyshot,100)
-- Giveweaponped(ped,tec9,100)

--model = uzi

--   h2=tostring(t)
-- loadmodel(model)
-- ped = Createped("HFOST",x,y,z)
 --while keypress(VK_H) do wait(1) 
--releasemodel(model)
--while not availablemodel(model) do  wait(1) end
-- x,y,z =getcoordes(player)
-- x,y = getworldcoordped(player,1.0,13)  
-- h1=tostring(x)
  -- h2=tostring(y)
 -- h3 = h1.." "..h2
 -- printmessage(h3, 3000)
 -- car = createcar(model, x,y,z)
-- angle = getangleped(player)
--x,y= 
  -- h1=tostring(x)
 -- printmessage(h1, 3000)

--printmessage("create car ", 1000)
		-- create_marker_actor()
	-- removemarker()

-- car = createcar(model, x,y,z)
 --if keypress(key("M"))-- клавиша m.
--  then 
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -766.3, -1119.3, 11.0)
 
  --res1 =in_point_car_in_radius(car,  -766.3, -1119.3, 11.0, 4, 3,2) 

-- if res1 == true 
 -- then
-- end
--end
--end
 --res, car = incar(player)--в авто?   
 -- if res == true 
  -- then
 -- -- if res1 == true
 -- -- then
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -787.8, -1143.1, 11.05)
 -- end
-- end

 ---drivecar(mycar, -159.2, -1458.0, 10.6)
-- printmessage(" mission ",3000,1)
-- mycar = Createcar(MODEL_RANCHER, 227.6, -707.6, 10.3) -- создать авто  

-- setcarangle(mycar, 163.0)-- уст угол авто.
-- mar = create_marker_car(mycar)-- создать маркер над авто.
-- ped = set_ped_in_car(mycar, "SWAT") -- уст водителя авто.
-- setcarspeed(mycar, 10)-- уст скорость авто.
-- setdrivingstyle(mycar, 2)-- уст стиль вождения.


-- Keypress(VK_G) --cheat("zx")
-- then 
 -- x,y,z =getcoordinates_on_y(player,15)-- получить координаты на 5 м впереди.
 -- ped = Createped("HFOST", x,y,z)-- 
-- mycar = Createcar(MODEL_REGINA, x,y,z) -- создать авто  
-- -- wait(1000)
-- if isped(mycar)
-- then printmessage(" this is ped",3000,1)

-- else givemoney(100000000)
-- end
 -- end
-- --isobject(obj)
--obj = Create_obj(588, x,y,z) -- создать объект.
 --marobj = create_marker_obj(obj)-- создать маркер над объектом
 -- if Keypress(VK_H) --cheat("zx")
-- then 
-- remove_blip(marobj)
-- remove_obj(obj)
-- Giveweaponped(player,300,"grenade")-- дать педу оружие.
-- end
--newthread(foo, player)

-- local x = 0
-- while true do

-- x = x+1  
-- printmessage(tostring(x),1000,1)
-- wait(1200)

-- function Player_defined(player)
-- if player_defined(player) --or Arrested()
-- then
-- return true
-- end
-- else 
 -- while not player_defined(player) do wait()
 -- end
-- return true
-- end
-- if not player_defined(player) --or Arrested()
-- then 
-- givemoney(1000)
-- printmessage("arrested", 3000,1)
 -- while not player_defined(player) do wait()
 -- end 

-- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
-- Giveweaponped(player,1300,"buddyshot")-- дать педу оружие.
-- Giveweaponped(player,1300,"flame")-- дать педу оружие.
-- -- mycar = Createcar(MODEL_REGINA, x,y,z) -- создать авто  
-- -- lockstatus(mycar, 2)
-- -- setcarfirstcolor(mycar, 0)

 -- x,y,z =getcoordinates_on_y(player,15)-- получить координаты на 5 м впереди.
 -- pickup =Create_weapon_pickup("sniper",3, 30, x,y,z)-- создать пикап оружие.
-- create_marker_pickup(pickup)
-- end
 -- if Keypress(VK_H) --cheat("zx")
-- then destroy()
-- --set_wanted(3)
-- --local car1 = Createcar(MODEL_REGINA, 154.8, -837.2, 10.3 ) -- создать авто на 5 впереди.  
-- -- Giveweaponped(player,300,"grenade")-- дать педу оружие.
-- --drivecar(car1, 76.9, -1055.7, 10.3)
-- end
 --Giveweaponped(player,300,"grenade")-- дать педу оружие.
--drivecar(car1, 76.9, -1055.7, 10.3)



 -- blip = createblip(8, x,y,z)-- создать метку на карте.
-- mar = create_marker_car(mycar)-- создать маркер над авто.
 -- x,y,z =getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.

-- man = Createped("BMYBB",x,y,z) 
 -- mar1 = create_marker_actor(man) -- создать маркер педа.
 
 -- x,y,z =getcoordinates_on_y(player,15)-- получить координаты на 5 м впереди.
 -- sp = create_sphere(x,y,z, 6.5)-- создать сферу, последний параметр радиус. 
 
 -- x,y,z =getcoordinates_on_y(player,25)-- получить координаты на 5 м впереди.
 -- adrealin = Create_pickup(367, 15, x,y,z )-- создать пикап адреалина.
  
 -- x,y,z =getcoordinates_on_y(player,35)-- получить координаты на 5 м впереди.
 -- pickup =Create_weapon_pickup("sniper",3, 30, x,y,z)-- создать пикап оружие.

 -- x,y,z =getcoordinates_on_y(player,45)-- получить координаты на 5 м впереди.
-- p= create_money_pickup(2000, x,y,z)

 -- printmessage("key 1",1200,1)
-- --wait(2000)
-- while lualoader == nil  do
 -- wait() 
-- if  Car_in_radius(mycar, 64.4, -1060.2, 10.5) 
 -- then break
 -- end 
 -- end
 
-- while lualoader == nil  do
 -- wait() 
-- if  Car_in_radius(mycar, 56.2, -1059.0, 10.5) 
 -- then break
 -- end 
 -- end
-- while lualoader == nil  do
 -- wait() 
-- if  Car_in_radius(mycar, 55.1, -1078.3, 10.5) 
 -- then break
 -- end 
 -- end

 --printmessage("key h",1200,1)
-- end 
-- end
 -- if Keypress(VK_B) 
 -- then 
--set_traffic(0)
 
--setcarhealth(mycar,150)--уст здоровье авто.
 -- then local car1 = Createcar(MODEL_REGINA, 154.8, -837.2, 10.3 ) -- создать авто на 5 впереди.  
 
-- drivecar(car1, 76.9, -1055.7, 10.3)
-- end
-- end
 -- end
 
 
-- while lualoader == nil  do
-- wait() 
-- i =f(mycar, 73.3, -1025.1, 10.3, 4.0,4.0,2,0) 
-- if i == true

-- then printmessage("key h",1500,1)
 -- setcoordesped(player, 77.169, -1054.604, 9.332 )
 -- adrealin = Create_pickup(367, 15, 46.478, -1065.627, 16.024 )
 -- pickup =Create_weapon_pickup("sniper",3, 30, 46.478, -1063.627, 16.024)-- создать пикап оружие
 -- setcoordesped(player, 41.5, -1058.013, 15.424 )
 -- end
  -- if Keypress(VK_H)
  -- then remove_pickup(adrealin)
-- --printmessage("key h",1500,1) Giveweaponped(player,300,"ingramsl")-- дать педу оружие.
 -- end end
 
 
 -- setcarangle(mycar, 145.0)
-- ped = set_ped_in_car(mycar,"SWAT")-- уст педа в авто, без третного параметра то водитель.
-- setcarspeed(mycar,30)
 -- setdrivingstyle(mycar, 2 )
--mycar = Createcar(MODEL_CHEETAH,x,y,z) -- создать авто  -- if Keypress(VK_G)
-- then 
-- mycar = Createcar(MODEL_REGINA, 79.0, -965.8, 10.3 ) -- создать авто на 5 впереди.  

 -- end 
 -- if Keypress(VK_B) 
 -- then printmessage("key h",1500,1) 
-- x,y,z =getcoordinates_on_y(player,35)
-- setcarspeed(mycar, 35)
 -- drivecar(mycar,x,y,z)
  -- end
-- end 
 -- setcarangle(mycar, 153.0)
 --x,y,z =getcoordinates_on_y(player,5)
-- function foo()
-- local x = 0
-- while true do

-- x = x+1  
-- printmessage(tostring(x),1000,1)
-- wait(1200)
-- --givemoney(m)
-- end end


-- f = coroutine.wrap(
 -- function(car, x1, y1, z1, rx, ry, rz)
-- while lualoader == nil  do wait() 
-- b = Car_in_radius(car, x1, y1, z1, rx, ry, rz)
-- coroutine.yield(b) 
  -- end
 -- end
 -- )
--setcarspeed(mycar,30)
-- while lualoader == nil  do
-- wait() 
-- i =f(mycar, 73.3, -1025.1, 10.3, 4.0,4.0,2,0) 
-- if i == true

-- then printmessage("key h",1500,1)
 -- adrealin = Create_pickup(367, 15, 46.478, -1065.627, 16.024 )
 -- pickup =Create_weapon_pickup("sniper",3, 30, 46.478, -1063.627, 16.024)-- создать пикап оружие
 -- setcoordesped(player, 41.5, -1058.013, 15.424 )
 -- end
  -- if Keypress(VK_H)
  -- then remove_pickup(adrealin)
 -- setcoordesped(player, 77.169, -1054.604, 9.332 )
-- --printmessage("key h",1500,1) Giveweaponped(player,300,"ingramsl")-- дать педу оружие.
 -- end end

-- pickup =Create_weapon_pickup("buddyshot",3, 100,x,y,z)-- создать пикап оружие
 -- [[ 1 - за деньги, регенерируется почти сразу
-- 2 - бесплатно, регенерация за 30 минут игрового времени, надо отойти недалеко
-- 3 - бесплатно, одноразовый 9,11 - бомба
-- 15 - эти регенерируется за 6 часов игрового времени или за 6 минут по-нормальному--]] 
 
  
 
 
 
 --showtext("p", 800,0)
 --newthread(foo)
--yield()
  -- if cheat("go")-- чит введен
  -- then printmessage("cheat action",2000,1)
  -- Giveweaponped(player,300,"ingramsl")-- дать педу оружие.
 
 --x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- man = Createped("BMYBB",x,y,z) 
 -- mar = create_marker_actor(man) -- создать маркер педа.
  -- --printmessage("h1",1500,1) 
-- sound_coordinate(7, 0.0,0.0,0.0)
-- show_text_styled("RACE4", 1500, 4)-- 1 для гонки
-- m = createmarker(1,3,x,y,z)
-- setsizemarker(m,3)
-- end
  -- if Keypress(VK_H)
 -- then 
-- remove_blip(m)
 -- -- while lualoader == nil do
 -- -- wait()
-- -- Draw_corona(4.5, 6, 0, 255, 0, 0, x,y,z) --5.5, 6, 0,255, 0, 0, x,y,z
 -- -- --Newthread(player)
 
 
 
 
-- function funs(x)	 -- 
   -- peds = findped() -- все педы -- end
   -- for i1, v1 in pairs(peds) do
-- sethealth(v1,10)
      -- v2 = tostring(v1) 
	  -- if x == v2 
	   -- then 
-- printmessage("h1",1500,1)
	   -- break
 -- end
-- end
-- end
-- function Newthread( p)-- преобразовать указатели в строки
    -- if ("userdata" == type(p))-- если указател, то преобразовать в строку
	 -- then 
	 -- x =tostring(p)
 -- funs(x)
 -- end
-- newthread(funs, func,x)--указатели строки
-- end

-- function f(p)
 -- sethealth(p,30)
-- end

--require('effil')
  -- for i,v in pairs(t) do
 -- if ("string" == type(v))
   -- then    -- sethealth(i1,310)
	-- -- printmessage("h1",1500,1)
    -- -- for i1, v1 in pairs(peds) do
 -- -- end end  
 -- end  
 -- end 
 -- t={...}--таблица
  -- t1={}
 -- for i,v in pairs(t) do
     -- --table.insert(t1, x)
	-- else y= t[v]
	-- table.insert(t1, y) --  end
 -- end func,
 -- peds= findped()
 -- printmessage("h1",1500,1) 
 -- for i, v in pairs(peds) do
    -- sethealth(i,10)
-- end
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- p = Create_pickup(335,3,x,y,z)-- создать пикап
 -- end 
 -- if picked_up(p)
 -- then printmessage("h1",1500,1) 
-- remove_pickup(p)
 -- end
  -- if Keypress(VK_H)
 -- then fade(1,600) --просветления, 600 время.
 -- play_voice("FIN_12a")-- проиграть голос.
 -- end
 --fade(0,600)--затенение 0, 600 время.
 --x,y,z =getcoordinates_on_y(player,8)-- получить координаты на 5 м впереди.
-- givemoney(3000)
-- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
 --printmessage("h1",1500,1) 
  -- showtext("h", 2500,0)-- вывод статуса миссии.
  -- z1=tostring(player)
  -- b, h =findped(z1)
  -- showtext(z1, 3000,0)
-- if b == true
-- then Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие.
-- sethealth(h, 52)
-- end  
 -- --newthread(funs, "BMYBB")
-- thr = effil.thread(foo)()
-- thr:wait()
--play_voice("FIN_12a")
 -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- givemoney(3000)
 -- pickup = Create_pickup(335,3,x,y,z)
-- end 

-- if picked_up(pickup)
 -- then remove_pickup(pickup)
 -- printmessage("ok",2000,1)
 
 --pickup =Create_weapon_pickup("buddyshot",1, 100,x,y,z)
 -- [[ 1 - за деньги, регенерируется почти сразу
-- 2 - бесплатно, регенерация за 30 минут игрового времени, надо отойти недалеко
-- 3 - бесплатно, одноразовый 9,11 - бомба
-- 15 - эти регенерируется за 6 часов игрового времени или за 6 минут по-нормальному--]] 
-- mycar = Createcar(MODEL_CHEETAH,x,y,z) -- создать авто   
-- x,y,z =getcoordinates_on_y(player,45)-- получить координаты на 5 м впереди.
  -- setdrivingstyle(mycar, 1) setcarspeed(mycar, 55) 
 -- cardrive(mycar, x,y,z) 
 -- f = coroutine.wrap(function ()
-- while true do  wait()  coroutine.yield()
 -- if  Car_in_point_in_radius(car, x,y,z, 4.0,4.0,2.0)
 -- then printmessage("ok",2000,1)
-- end end end)
 -- end 
 -- f(mycar, x,y,z)
 -- end  
 -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
-- function funs(ped)
 -- sethealth(ped,30)
-- end
 --m = tostring(player)
 --showtext(m, 2500,0)-- вывод статуса миссии.
 --newthread(player)
--g = tostring(ped)

--showtext(g, 2500,0)-- вывод статуса миссии.
--printmessage(g, 3500,1)
-- b, p = findped(ped)
-- sethealth(ped,30)
-- if b == true
-- then 
-- -- while true do 
 -- -- if 0 == gethealth(man)
 -- -- then  
 -- -- printmessage(m, 1500,1)
 -- -- remove_ped(man)
 -- -- removemarker(mar)-- удалить маркер над педом.
 -- -- break
 -- -- end
-- else
 -- printmessage("no",3500,1)
 -- end
 --man = Createped("BMYBB",x,y,z) 
-- mar = create_marker_actor(man) -- создать маркер педа.



-- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
--  removeped(man)-- удалить пед.
  -- wait(100)
  -- man = Createped("BMYBB",x,y,z)
  -- printmessage("greate ped", 1500,1)
 -- if Keypress(VK_L)
 -- then m ,t =Get_type_weapon_ped(player)
 -- Giveweaponped(man,300,m)-- дать педу оружие.
  -- printmessage("Give weapon ped", 1500,1)
 -- end
 -- if Keypress(VK_H)
 -- then 
  -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
 -- end 
--x,y,z =getcoordinates_on_y(player,6)
  -- printmessage("run forward", 1500,1)
 -- ped_sprint_to_point(man, x,y,z)
 -- if Keypress(VK_J)
 -- then res, ped = randomfindped(player,20)
 -- if res ==true and man~= ped and false == incar(ped)
 -- then  printmessage("find ped", 1500,1)
-- mar = create_marker_actor(ped) -- создать маркер педа.
  -- end 
  -- end
  -- if Keypress(VK_K)
 -- then Kill_ped_on_foot(man, ped)-- пед хочет убить другого педа.
  -- printmessage("kill ped", 1500,1)
 -- end 

 -- if Keypress(VK_Y) or 0 == gethealth(man)
 -- then printmessage("remove ped", 1500,1)
  -- remove_ped(ped)
 -- end
 
 -- if 0 == gethealth(ped)
 -- then  removemarker(mar)-- удалить маркер над педом.
 -- end


 -- if Keypress(VK_G)
 -- then
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
  -- man = Createped("WFYPR",x,y,z)
  -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие.
-- end
 -- if Keypress(VK_L)
 -- then m ,t =Get_type_weapon_ped(player)
 -- Giveweaponped(man,300,m)-- дать педу оружие.
 -- end
 -- if Keypress(VK_H)
 -- then  x,y,z =getcoordinates_on_y(player,6)
 -- ped_sprint_to_point(man, x,y,z)
 -- end 
-- end




--  Giveweaponped(man,300,ruger,uzi)-- дать педу оружие.
-- printmessage("sphere", 1500,1)
-- sp = create_sphere(x,y,z, 1.5)-- создать сферу, последний параметр радиус. 
-- end
-- if Keypress(VK_J)
  -- -- if Ped_in_point_in_radius(player, 241.6, -1283.0, 10.9, 2.0,2.0,2.0)
 -- then 
-- -- printmessage("on place", 1500,1)
  -- remove_sphere(sp) -- удалить сферу.
--  setcoordesped(player, 241.6, -1283.0, 10.9)
 -- x1=tostring(x) y1=tostring(y) z1=tostring(z)
-- a1="cord "..x1.." "..y1.." "..z1
-- printmessage(a1, 1000,1)
-- x1=tostring(x) y1=tostring(y) z1=tostring(z)
-- a1="cord "..x1.." "..y1.." "..z1
-- printmessage(a1, 1000,1)
--end
-- if Ped_in_point_in_radius(player, 241.6, -1283.0, 10.9, 2.0,2.0,2.0)
-- then
-- printmessage("ped in point", 5000,1)
-- end

--x,y,z = getworldcoordped(player,1.0,3)
 -- woman = Createped("HFYPR",x,y,z)
 -- x,y,z = getworldcoordped(woman,0.1,-4)
 -- ped_sprint_to_point(man, x,y,z)
 --wait(3000)
 --ped_aim_at_ped(man, woman)-- пед целиться в педа.
--printmessage("created by the killer and victim", 2500, 3)
 -- ped_aim_at_ped(man, woman)-- пед целиться в педа.
-- end--Kill_ped_on_foot(man, woman)-- пед хочет убить другого педа.
-- removemarker(mar)-- удалить маркер над педом.

-- if Keypress(VK_H)
-- then   
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
 -- Giveweaponped(player,uzi,300)-- дать педу оружие.
 -- x,y,z = getworldcoordped(player,1.0,4)
-- man = Createped("WMYPI",x,y,z)
-- end
-- if Keypress(VK_M)
-- then   
-- remove_ped(man)
-- kill_ped(man)
-- printmessage("remove ped", 2500, 3)
 -- for i=1, 100 do
 -- h2=tostring(i)
 -- h3 = "ped № "..h2
-- printmessage(h3, 1500, 3)
-- wait(2000)
 -- end
 -- Giveweaponped(man,m4,100)-- дать педу оружие.
 -- mar = create_marker_actor(man) -- создать маркер педа.
-- removemarker(mar)-- удалить маркер над педом.
 -- x,y,z = getworldcoordped(man,0.1,30)
 -- woman = Createped("HFYPR",x,y,z)
 -- sp = create_sphere(x,y,z, 1.5)-- создать сферу, последний параметр радиус.
 -- x,y,z = getworldcoordped(woman,0.1,-4)
 -- ped_sprint_to_point(man, x,y,z)
 --wait(3000)
 --ped_aim_at_ped(man, woman)-- пед целиться в педа.
--printmessage("created by the killer and victim", 2500, 3)
 -- ped_aim_at_ped(man, woman)-- пед целиться в педа.
-- end
 -- if Keypress(VK_K)
 -- then Kill_ped_on_foot(man, woman)-- пед хочет убить другого педа.
-- removemarker(mar)-- удалить маркер над педом.
-- remove_sphere(sp) -- удалить сферу.
 -- end 
-- while not in_point_actor_in_radius(man, x, y, z, 2.0,2.0,2.0) do 
-- ped_sprint_to_point(man, x,y,z) wait(10) end
--end
 -- man = Createped("HFOST",x,y,z)
 -- woman = Createped("HFYMD",x,y,z)
 --Kill_char_any_means(man, woman)
 -- Giveweaponped(player,tec9,10)
-- -- printmessage("lkjm???", 1500,1)
 -- x,y = getworldcoordped(player,1.0,7)
 -- printmessage("lkjm???", 3500,1)
-- kill_ped_on_foot(ped, ped2)
 --Giveweaponped(ped,brassknuckle,100)
 --wait(200)
 -- m = getangleped(player)
 -- h1=tostring(m)
 --Giveweaponped(ped2,m4,100)
-- x,y = getworldcoordped(player,1.0,7)
--mycar = Createcar(MODEL_ENFORCER,x,y,z) -- создать авто на 5 впереди.  
 -- ped = Createped("HFOST",x,y,z)
 -- Giveweaponped(ped,tec9,100)
-- tw = Get_type_weapon_ped(ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000,1)
--kill_ped_on_foot(ped2, ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000)
 -- x,y = getworldcoordped(player,2.0,8)  
 -- ped2 = Createped("HFYBU",x,y,z)
--is_current_weapon_ped(ped,22)-- проверить пед держит в руках это оружие?
 -- if keypress(VK_H)-- клавиша h.
 -- then

 -- if keypress(VK_H)-- клавиша h.
 -- then
-- Giveweaponped(player,tec9,100)
-- while keypress(VK_H) do wait(1) end
  -- x,y,z =getcoordes(player)
 -- x,y = getworldcoordped(player,1.0,6)  
 -- ped = Createped("HFOST",x,y,z)
-- end 
-- Giveweaponped(ped,buddyshot,100)
-- Giveweaponped(ped,tec9,100)

--model = uzi

--   h2=tostring(t)
-- loadmodel(model)
-- ped = Createped("HFOST",x,y,z)
 --while keypress(VK_H) do wait(1) 
--releasemodel(model)
--while not availablemodel(model) do  wait(1) end
-- x,y,z =getcoordes(player)
-- x,y = getworldcoordped(player,1.0,13)  
-- h1=tostring(x)
  -- h2=tostring(y)
 -- h3 = h1.." "..h2
 -- printmessage(h3, 3000)
 -- car = createcar(model, x,y,z)
-- angle = getangleped(player)
--x,y= 
  -- h1=tostring(x)
 -- printmessage(h1, 3000)

--printmessage("create car ", 1000)
		-- create_marker_actor()
	-- removemarker()

-- car = createcar(model, x,y,z)
 --if keypress(key("M"))-- клавиша m.
--  then 
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -766.3, -1119.3, 11.0)
 
  --res1 =in_point_car_in_radius(car,  -766.3, -1119.3, 11.0, 4, 3,2) 

-- if res1 == true 
 -- then
-- end
--end
--end
 --res, car = incar(player)--в авто?   
 -- if res == true 
  -- then
 -- -- if res1 == true
 -- -- then
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -787.8, -1143.1, 11.05)
 -- end
-- end
 -- while true do 
 -- res1 =in_point_car_in_radius(car, -787.8, -1143.1, 11.05,  2,2,2) 
 -- if res1 == true 
 -- then
 -- break
 -- end
 -- end
-- printmessage("car in point", 1000)
--else givemoney(1)
--end
--// x,y,z=getcoordes(player)
 -- a1="cord "..x.." "..y.." "..z
 -- printmessage(a1, 1000)
 -- -- explodecar(car)
-- --wait(10)
 -- h=getcarhealth(car)
 -- h1=tostring(h)
 -- a1="health "..h1
 -- printmessage(a1, 1000)
 -- then res1, car = incar(p)--в авто?
 -- if res1 == true
 -- then exitcar(p)
-- sethealth(p, 0) -- 0 хп.
-- end

-- else
 -- printmessage("not find car", 1000)
--printmessage("yes in car", 1000)

-- x,y,z=cargetcoordes(car)-- получить координаты авто.
 -- a1="cord "..x.." "..y.." "..z
 -- printmessage(a1, 1000)
 --setcarhealth(car,100)
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
--res,p = randomfindped(player, 10.0)-- найти педа
--end
-- res,p = randomfindped(player, 10.0)-- найти педа
 -- if res == true
 -- then
 
 --else printmessage("not in car", 1000)
-- givemoney(1)-- дать денег.
 -- sethealth(p, 0) -- 150 хп.
 -- end
--sethealth(p, 0) -- 150 хп.
----.x,y,z=getcoordes(player)--.pri()b= 
--sethealth(p, 0) -- 150 хп.
--end
 -- if keypress(key("M"))-- клавиша T.
 -- then 	

--.x,y,z=getcoordes(player)--.pri()b= 
--p = randomfindped(player, 20)--// название функции в lua и c
--printmessage("yes", 1000)


-- x,y,z=getcoordes(player)
-- a1="cord "--..m.x--.." "..y.." "..z
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
-- printmessage(a1, 1000)
-- end 

-- end
--
--function main()
 -- health = gethealth(player)-- получить здоровье игрока.
-- if health<70 then
 -- setarmour(player, 0) -- уст броню
-- sethealth(player, 171) -- 150 хп.
-- end

--end
-- sleep(10) -- миллисекунды в скобках










--setcarhealth(mycar,150)--уст здоровье авто.
 -- woman = Createped("HFYPR",x,y,z)
 -- if isped(woman) --это пед?
 -- then printmessage("this is ped", 3000, 1)
 -- else printmessage("this is NOT ped", 3000, 1)
 -- end
-- mycar = Createcar(MODEL_ENFORCER,x,y,z) -- создать авто 
 -- if isped(mycar) --это транспорт?
 -- then printmessage("this is vehicle", 3000, 1)
 -- else printmessage("this is NOT vehicle", 3000, 1)
 -- end

 -- ax = string.format("a =% s", player)
 -- cord = tostring(ax)
  -- printmessage(cord, 3000, 1)
 -- if Keypress(VK_L) 
 -- then x,y,z = getcoordes(player)
 -- cord = tostring(x)..", "..tostring(y)..", " .. tostring(z).."\n"
  -- file = io.open('cordinat.txt', 'a') -- открыть файл для записи 
  -- file:write(cord)    
  -- file:close()
  -- printmessage("write cordinat", 3000, 1)
 -- end  

 --newthread(func)--func,
 --woman = Createped("HFYPR",x,y,z)
 --newthread(
-- x,y,z =getcoordinates_on_y(player,30)-- получить координаты на 5 м впереди.
 -- if Ped_in_point_in_radius(player, 541.96, 367.94, 15,4, 1.0, 1.0, 2.0)
 -- then
-- remove_blip(blip)-- удалить метку с карты.
-- play_sound(1)
-- showtext("on", 2500,0)
 -- else setflagmission(1)
-- end
 -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
 -- newthread(funs, 6000, "dead", "BMYBB")--, "remove ped")

 -- x,y,z =getcoordes(player)
 -- x1=tostring(x) y1=tostring(y) z1=tostring(z)
-- a1="cord "..x1.." "..y1.." "..z1
-- printmessage(a1, 1000,1)


-- sethealth(player, 10)
 -- player = findplayer()-- получить игрока
 -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- man = Createped(ped,x,y,z) -- создать педа. 





-- function func()
-- printmessage("func", 3500,1)
-- end
-- function main()
-- while lualoader == nil do
 -- wait()
 -- player = findplayer()-- получить игрока
 -- if Keypress(VK_G)
 -- then  newthread(func)
 
 -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. function func()
-- end
 -- end 
 -- end  
--sethealth(player, 0)
--sethealth(player, 0)
  --  foo() -- expected to print "Hello world", by 
--/local lanes = require "lanes".configure()
-- local launcher = lanes.gen("*", func)
-- thread = launcher()
-- thread:join()-- newthread(funs,"BMYBB","remove ped")-- функция, модель пед, текст надписи.
 --newthread(funs,"WFYPR","remove ped2")-- функция, модель пед, текст надписи.
-- end
          -- calling below C function,but not happening
 -- player = findplayer()-- получить игрока
 -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
 -- man = Createped(ped,x,y,z) -- создать педа.
 -- mar = create_marker_actor(man) -- создать маркер педа.
-- h = tostring(man)
-- while true do 
 -- if 0 == gethealth(man)-- проверить его здоровье.
 -- then  
-- -- printmessage(text, 1500,1)
 -- remove_ped(man)-- удалить педа.
 -- removemarker(mar)-- удалить маркер над педом.
 -- break
 -- end
-- end
 -- if Keypress(VK_T)
 -- then 
  -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
 -- end 


-- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
--  removeped(man)-- удалить пед.
  -- wait(100)
  -- man = Createped("BMYBB",x,y,z)
  -- printmessage("greate ped", 1500,1)
 -- if Keypress(VK_L)
 -- then m ,t =Get_type_weapon_ped(player)
 -- Giveweaponped(man,300,m)-- дать педу оружие.
  -- printmessage("Give weapon ped", 1500,1)
 -- end
 -- if Keypress(VK_H)
 -- then 
  -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие. 
 -- end 
--x,y,z =getcoordinates_on_y(player,6)
  -- printmessage("run forward", 1500,1)
 -- ped_sprint_to_point(man, x,y,z)
 -- if Keypress(VK_J)
 -- then res, ped = randomfindped(player,20)
 -- if res ==true and man~= ped and false == incar(ped)
 -- then  printmessage("find ped", 1500,1)
-- mar = create_marker_actor(ped) -- создать маркер педа.
  -- end 
  -- end
  -- if Keypress(VK_K)
 -- then Kill_ped_on_foot(man, ped)-- пед хочет убить другого педа.
  -- printmessage("kill ped", 1500,1)
 -- end 

 -- if Keypress(VK_Y) or 0 == gethealth(man)
 -- then printmessage("remove ped", 1500,1)
  -- remove_ped(ped)
 -- end
 
 -- if 0 == gethealth(ped)
 -- then  removemarker(mar)-- удалить маркер над педом.
 -- end


 -- if Keypress(VK_G)
 -- then
  -- x,y,z =getcoordinates_on_y(player,5)-- получить координаты на 5 м впереди.
  -- man = Createped("WFYPR",x,y,z)
  -- Giveweaponped(player,300,"colt45","ruger","uzi")-- дать педу оружие.
-- end
 -- if Keypress(VK_L)
 -- then m ,t =Get_type_weapon_ped(player)
 -- Giveweaponped(man,300,m)-- дать педу оружие.
 -- end
 -- if Keypress(VK_H)
 -- then  x,y,z =getcoordinates_on_y(player,6)
 -- ped_sprint_to_point(man, x,y,z)
 -- end 
-- end




--  Giveweaponped(man,300,ruger,uzi)-- дать педу оружие.
-- printmessage("sphere", 1500,1)
-- sp = create_sphere(x,y,z, 1.5)-- создать сферу, последний параметр радиус. 
-- end
-- if Keypress(VK_J)
  -- -- if Ped_in_point_in_radius(player, 241.6, -1283.0, 10.9, 2.0,2.0,2.0)
 -- then 
-- -- printmessage("on place", 1500,1)
  -- remove_sphere(sp) -- удалить сферу.
--  setcoordesped(player, 241.6, -1283.0, 10.9)
 -- x1=tostring(x) y1=tostring(y) z1=tostring(z)
-- a1="cord "..x1.." "..y1.." "..z1
-- printmessage(a1, 1000,1)
-- x1=tostring(x) y1=tostring(y) z1=tostring(z)
-- a1="cord "..x1.." "..y1.." "..z1
-- printmessage(a1, 1000,1)
--end
-- if Ped_in_point_in_radius(player, 241.6, -1283.0, 10.9, 2.0,2.0,2.0)
-- then
-- printmessage("ped in point", 5000,1)
-- end

--x,y,z = getworldcoordped(player,1.0,3)
 -- woman = Createped("HFYPR",x,y,z)
 -- x,y,z = getworldcoordped(woman,0.1,-4)
 -- ped_sprint_to_point(man, x,y,z)
 --wait(3000)
 --ped_aim_at_ped(man, woman)-- пед целиться в педа.
--printmessage("created by the killer and victim", 2500, 3)
 -- ped_aim_at_ped(man, woman)-- пед целиться в педа.
-- end--Kill_ped_on_foot(man, woman)-- пед хочет убить другого педа.
-- removemarker(mar)-- удалить маркер над педом.

-- if Keypress(VK_H)
-- then   
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
 -- Giveweaponped(player,uzi,300)-- дать педу оружие.
 -- x,y,z = getworldcoordped(player,1.0,4)
-- man = Createped("WMYPI",x,y,z)
-- end
-- if Keypress(VK_M)
-- then   
-- remove_ped(man)
-- kill_ped(man)
-- printmessage("remove ped", 2500, 3)
 -- for i=1, 100 do
 -- h2=tostring(i)
 -- h3 = "ped № "..h2
-- printmessage(h3, 1500, 3)
-- wait(2000)
 -- end
 -- Giveweaponped(man,m4,100)-- дать педу оружие.
 -- mar = create_marker_actor(man) -- создать маркер педа.
-- removemarker(mar)-- удалить маркер над педом.
 -- x,y,z = getworldcoordped(man,0.1,30)
 -- woman = Createped("HFYPR",x,y,z)
 -- sp = create_sphere(x,y,z, 1.5)-- создать сферу, последний параметр радиус.
 -- x,y,z = getworldcoordped(woman,0.1,-4)
 -- ped_sprint_to_point(man, x,y,z)
 --wait(3000)
 --ped_aim_at_ped(man, woman)-- пед целиться в педа.
--printmessage("created by the killer and victim", 2500, 3)
 -- ped_aim_at_ped(man, woman)-- пед целиться в педа.
-- end
 -- if Keypress(VK_K)
 -- then Kill_ped_on_foot(man, woman)-- пед хочет убить другого педа.
-- removemarker(mar)-- удалить маркер над педом.
-- remove_sphere(sp) -- удалить сферу.
 -- end 
-- while not in_point_actor_in_radius(man, x, y, z, 2.0,2.0,2.0) do 
-- ped_sprint_to_point(man, x,y,z) wait(10) end
--end
 -- man = Createped("HFOST",x,y,z)
 -- woman = Createped("HFYMD",x,y,z)
 --Kill_char_any_means(man, woman)
 -- Giveweaponped(player,tec9,10)
-- -- printmessage("lkjm???", 1500,1)
 -- x,y = getworldcoordped(player,1.0,7)
 -- printmessage("lkjm???", 3500,1)
-- kill_ped_on_foot(ped, ped2)
 --Giveweaponped(ped,brassknuckle,100)
 --wait(200)
 -- m = getangleped(player)
 -- h1=tostring(m)
 --Giveweaponped(ped2,m4,100)
-- x,y = getworldcoordped(player,1.0,7)
--mycar = Createcar(MODEL_ENFORCER,x,y,z) -- создать авто на 5 впереди.  
 -- ped = Createped("HFOST",x,y,z)
 -- Giveweaponped(ped,tec9,100)
-- tw = Get_type_weapon_ped(ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000,1)
--kill_ped_on_foot(ped2, ped)
-- h2=tostring(tw)
 -- h3 = "type weapon "..h2
 -- printmessage(h3, 3000)
 -- x,y = getworldcoordped(player,2.0,8)  
 -- ped2 = Createped("HFYBU",x,y,z)
--is_current_weapon_ped(ped,22)-- проверить пед держит в руках это оружие?
 -- if keypress(VK_H)-- клавиша h.
 -- then

 -- if keypress(VK_H)-- клавиша h.
 -- then
-- Giveweaponped(player,tec9,100)
-- while keypress(VK_H) do wait(1) end
  -- x,y,z =getcoordes(player)
 -- x,y = getworldcoordped(player,1.0,6)  
 -- ped = Createped("HFOST",x,y,z)
-- end 
-- Giveweaponped(ped,buddyshot,100)
-- Giveweaponped(ped,tec9,100)

--model = uzi

--   h2=tostring(t)
-- loadmodel(model)
-- ped = Createped("HFOST",x,y,z)
 --while keypress(VK_H) do wait(1) 
--releasemodel(model)
--while not availablemodel(model) do  wait(1) end
-- x,y,z =getcoordes(player)
-- x,y = getworldcoordped(player,1.0,13)  
-- h1=tostring(x)
  -- h2=tostring(y)
 -- h3 = h1.." "..h2
 -- printmessage(h3, 3000)
 -- car = createcar(model, x,y,z)
-- angle = getangleped(player)
--x,y= 
  -- h1=tostring(x)
 -- printmessage(h1, 3000)

--printmessage("create car ", 1000)
		-- create_marker_actor()
	-- removemarker()

-- car = createcar(model, x,y,z)
 --if keypress(key("M"))-- клавиша m.
--  then 
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -766.3, -1119.3, 11.0)
 
  --res1 =in_point_car_in_radius(car,  -766.3, -1119.3, 11.0, 4, 3,2) 

-- if res1 == true 
 -- then
-- end
--end
--end
 --res, car = incar(player)--в авто?   
 -- if res == true 
  -- then
 -- -- if res1 == true
 -- -- then
 -- res, car = randomfindcar(player, 30.0)-- найти авто
 -- if res == true 
 -- then drivecar(car, -787.8, -1143.1, 11.05)
 -- end
-- end
 -- while true do 
 -- res1 =in_point_car_in_radius(car, -787.8, -1143.1, 11.05,  2,2,2) 
 -- if res1 == true 
 -- then
 -- break
 -- end
 -- end
-- printmessage("car in point", 1000)
--else givemoney(1)
--end
--// x,y,z=getcoordes(player)
 -- a1="cord "..x.." "..y.." "..z
 -- printmessage(a1, 1000)
 -- -- explodecar(car)
-- --wait(10)
 -- h=getcarhealth(car)
 -- h1=tostring(h)
 -- a1="health "..h1
 -- printmessage(a1, 1000)
 -- then res1, car = incar(p)--в авто?
 -- if res1 == true
 -- then exitcar(p)
-- sethealth(p, 0) -- 0 хп.
-- end

-- else
 -- printmessage("not find car", 1000)
--printmessage("yes in car", 1000)

-- x,y,z=cargetcoordes(car)-- получить координаты авто.
 -- a1="cord "..x.." "..y.." "..z
 -- printmessage(a1, 1000)
 --setcarhealth(car,100)
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
--res,p = randomfindped(player, 10.0)-- найти педа
--end
-- res,p = randomfindped(player, 10.0)-- найти педа
 -- if res == true
 -- then
 
 --else printmessage("not in car", 1000)
-- givemoney(1)-- дать денег.
 -- sethealth(p, 0) -- 150 хп.
 -- end
--sethealth(p, 0) -- 150 хп.
----.x,y,z=getcoordes(player)--.pri()b= 
--sethealth(p, 0) -- 150 хп.
--end
 -- if keypress(key("M"))-- клавиша T.
 -- then 	

--.x,y,z=getcoordes(player)--.pri()b= 
--p = randomfindped(player, 20)--// название функции в lua и c
--printmessage("yes", 1000)


-- x,y,z=getcoordes(player)
-- a1="cord "--..m.x--.." "..y.." "..z
-- sethealth(player, 200) -- 150 хп.
-- setarmour(player, 250) -- уст броню
-- givemoney(1000)-- дать денег.
-- printmessage(a1, 1000)
-- end 

-- end
--
--function main()
 -- health = gethealth(player)-- получить здоровье игрока.
-- if health<70 then
 -- setarmour(player, 0) -- уст броню
-- sethealth(player, 171) -- 150 хп.
-- end

--end
-- sleep(10) -- миллисекунды в скобках
