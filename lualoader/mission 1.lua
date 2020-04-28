require("lualoader/mod")

function main()
while true do wait() player = findplayer()-- получить игрока
 if Star_mission_marker(8, 526.6, 370.1, 10.9) -- создать маркер миссии на карте.
 then showtext("kill the man in the red shirt.", 500,1)-- вывод названия миссии. 
  Giveweaponped(player,300, "uzi")-- дать педу оружие. 
  fade(1,5500) --просветления, 600 время.
  wait(3600)
  p1, m1 = Create_ped_and_give_weapon("BMYBB", "uzi", 548.1, 365.5, 16.8)-- создать врага.
  setangle(p1, 76)
  set_camera_and_point(526.6, 370.1, 16.8, 548.1, 365.5, 16.9 )-- направить камеру.
  wait(2500)
  restore_camera()-- восстановить камеру.
  kill_ped_on_foot(p1, player)-- пед хочет убить игрока.
  printmessage("~r~kill him!.",3000,1)
  while true == Getflagmission() do wait(10) -- цикл пока статус миссии не изменится.  
    if not player_defined(p1) then miss(100) --break 
	end
	 end
	 end   
	 end
	 end 
 
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
