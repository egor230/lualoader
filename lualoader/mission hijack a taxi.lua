require("lualoader/mod")

function foel_for_car(model, car, weapon, x,y,z, angle)
 local ped = Createped(model, x,y,z)-- создать педа.
 setpedangle(ped, angle)-- уст угол педа.
 Giveweaponped(ped,600, weapon)-- дать оружия педу.
 local m  = create_marker(ped)-- создать маркер над педом.
 set_ped_accuracy(ped, 100)-- уст меткость педа.
  x,y,z = getcoordinates_on_y(ped, 25)
  ped_sprint_to_point(ped, x,y,z)-- заставить педа бежать к точке.
  wait(3200) ped_attack_car(ped, car)-- пед атакует авто.
while true == getflagmission() do wait(10) -- цикл пока статус миссии не изменится. 
   if getpedhealth(ped) == 0 -- если пед мертв.
  then 
  removemarker(m) remove_ped(ped)  break end     
  end 
  wait(3000)
  destroy()
 end
function mine(model, x1,y1,z1)-- создать мину.
player = findplayer() obj = Createobj(model, x1,y1,z1)-- создать бочку(объект).
while true == getflagmission() do  wait(5) -- цикл пока статус миссии не изменится. 
   if ped_in_radius_mod(player, x1,y1,z1, 2.0, 2.0, 2.0)
  then create_explosion(6,x1,y1,z1)-- создать взрыв.
  remove_obj(model) -- удалить объект.
   break end     
  end
end

function main() while true do wait() player = findplayer()-- получить игрока
 if Star_mission_marker(22, 325.5, 1199.1, 27.6) -- создать маркер миссии на карте. 
 then showtext(" hijack a taxi", 700,1)-- вывод названия миссии. 
  fade(1,1400) --просветления, 600 время. 
  car = Createcar("MODEL_ZEBRA", 328.8, 1157.3, 27.6) -- создать авто.
  setcarangle(car,6)-- уст угол авто.	
  set_radio(9,1)-- выключить радио.
  create_newthread(foel_for_car, "HNA", car, "m4", 352.7, 1242.2, 25.2, 96 )-- создать врага.
  wait(300)
  create_newthread(foel_for_car, "HNA", car, "m4", 353.1, 1244.8, 25.2, 96 )  wait(300)
  create_newthread(foel_for_car, "HNA", car, "m4", 353.1, 1247.7, 25.2, 110)  wait(300)
  create_newthread(mine, 350, 351.6, 1173.0, 24.3) -- создать мину.
  wait(300)
  create_newthread(mine, 350, 351.6, 1175.0, 24.3)  wait(300)
  create_newthread(mine, 350, 351.6, 1178.0, 24.3) wait(300) 
  -- putincar(player, car)
  printmessage("~r~take a taxi to the Parking lot",4000,1)
  blip, sp = create_marker_and_sphere(561.5, 1061.4, 18.2, 3,3.0)-- создать метку на карте с размером и сферу с радиусом.
while true == Getflagmission() do  wait(10) -- цикл пока статус миссии не изменится. 
 if ped_in_radius_mod(player, 561.5, 1061.4, 18.2, 2.0, 2.0, 2.0)-- если игрок находится в точке.
  then miss(5000)-- миссия выполнена.
  break end    
 if iscardead(car)-- если авто уничтожено.
  then end_mission("mission failed!")
  break end    
 end

 end
 end
 end
 
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
-- pickup =Create_weapon_pickup("buddyshot",3, 100,x,y,z)-- создать пикап оружие
 -- [[ 1 - за деньги, регенерируется почти сразу
-- 2 - бесплатно, регенерация за 30 минут игрового времени, надо отойти недалеко
-- 3 - бесплатно, одноразовый 9,11 - бомба
-- 15 - эти регенерируется за 6 часов игрового времени или за 6 минут по-нормальному--]] 
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
