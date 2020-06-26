require("lualoader/mod")
function first_step()
  load_scene( -1721.964, -275.942, 13.887)
  ped_frozen(0) -- заморозить игрока
  set_traffic(0)
  set_ped_traffic(0) 
  wait(500)
  helicopter = Createcar("MODEL_HUNTER", -1721.964, -275.942, 13.887)
  setangle(helicopter, 270)
  ped1 = set_ped_in_car(helicopter,"ARMY" )
  
  tank = Createcar("MODEL_RHINO", -1722.805, -263.981, 13.887)
  putincar(player, tank)
  setangle(tank, 180)
  
  Giveweaponped(player, 600, "uzi")-- дать педу оружие.
  Giveweaponped(ped1, 600,"m4")-- дать педу оружие.
  ped2 = Createped("ARMY", 231.774, -1272.967, 18.29)
  Giveweaponped(ped2, 600,"m4")-- дать педу оружие.
  ped_in_turret_on_car(ped2, tank,0,-1,2,0,0,26)
  --set_ped_immunities(ped2, 1,1,1,1,1)
  
  fade(1,5500) --просветления, 600 время.
  wait(3600)
  heli_to_fly(helicopter,-1721.964, -275.942, 40.959, 40  )
  wait(2500)
  heli_to_fly(helicopter,  -1728.515, -238.666, 50.959, 50 )
  --set_ped_immunities(ped1, 1,1,1,1,1)

  blip, sp = create_marker_and_sphere(-875.235, 1159.456, 10.223, 3,3.0)-- создать метку на карте с размером и сферу с радиусом.
  setcolourmarker(blip, 4)-- уст цвет маркера.
  printmessage("We'll secure the studio. Make sure you get that tape running and then catch up with", 3000,1)
  
  set_camera_on_car(helicopter, 18, 2)
  wait(9000)  
  ped_frozen(1)
  restore_camera()
  printmessage("~y~Get to VROCK and plat the tape", 3000,1)
 step = 0 
end
function foes_on_bikes()
 bike1 = Createcar("MODEL_FAGGIO", -708.9, 1182.6, 10.9 ) 
  ped4 = set_ped_in_car(bike1,"BKA" )
  m1 = create_marker_actor(ped4)
  setangle(bike1, 88)
    
  Giveweaponped(ped4, 600, "buddyshot")-- дать педу оружие.
   kill_char_any_means(ped4, player)
  bike2 = Createcar("MODEL_FAGGIO", -753.4, 1180.4, 10.9)
  ped5 = set_ped_in_car(bike2,"BKB" )
  setangle(bike2, 93)
  m2 = create_marker_actor(ped5)
  Giveweaponped(ped5, 600, "buddyshot")-- дать педу оружие.
  kill_char_any_means(ped5, player)
end
function main()
while true do wait()-- задержка.
  player = findplayer()-- получить игрока.
 if Star_mission_marker(6, -238.5, -1229.4, 8.1) -- создать маркер миссии на карте.
 then showtext("Open microphone.", 500,1)-- вывод названия миссии. 
 first_step() --просветления, 600 время.
while true do Getflagmission() -- цикл пока статус миссии не изменится.  
 if step == 0 and not is_ped_in_car(player) and ped_in_point_in_radius(player, -875.235, 1159.456, 10.223, 2.0,2.0,2.0 )
  then remove_blip(blip) remove_sphere(sp) 
   
  ped_frozen(0) -- заморозить игрока
  ped_run_to_point(player,-876.8, 1160.3, 17.8 )
  wait(2000)
  fade(0,900)
  ped_frozen(1)
  ped_clear_objective(player)
  select_interiour (9)
  setcord(player, -876.9, 1160.5, 16.8)
  set_cops_ignored(1)
  setangle(player, 86.883 )
  fade(1,900)
  ped3 = Createped("ARMY",  -879.333, 1156.702, 16.831)
  
  Giveweaponped(ped3, 600, "uzi")-- дать педу оружие.
  set_ped_stats_to(ped3, 16)
  ped_maintain_position_when_attacked(ped3, 1)
  set_timer_ped_attack(ped3, 1500)
  setpedhealth(ped3, 300)
  --kill_ped_on_foot(ped3, player)
  wait(500)
   step= 1
 
  end
  if step ==1 and not player_defined(ped3)
  then   ped_frozen(0)
  fade(0,900)
  wait(2000)
  select_interiour (0)
  setcord(player,-864.2, 1151.1, 10.9  )
   fade(1,900)
   setangle(player, 336 )
  ped_frozen(1)
  wait(1500)
   foes_on_bikes()
  kill_ped(ped2)
  car_explode(tank)
 step = 2   
  end
  
  if step == 2 and not player_defined(ped4)
  then removemarker(m1)
  remove_ped(ped4)
  end
  if step == 2 and not player_defined(ped5)
  then removemarker(m2)
  remove_ped(ped5)
  end
  if step == 2 and not player_defined(ped4) and not player_defined(ped5)
  then  blip, sp = create_marker_and_sphere(-1353.6, 1153.2, 8.7, 3,3.0)-- создать метку на карте с размером и сферу с радиусом.

 heli_to_fly(helicopter,-1353.6, 1053.2, 13.7, 40  )
 m3 = create_marker(helicopter)  
  step =3
   end
if step == 3 and ped_in_point_in_radius(player, -1353.6, 1153.2, 8.7, 2.0,2.0,2.0 ) 
 then  remove_blip(blip) remove_sphere(sp) 
  heli_to_fly(helicopter,-1353.6, 1153.2, 10.7, 60  )
   setdrivingstyle(helicopter, 1) 
  set_camera_on_car(helicopter, 18, 2)
 wait(600)
 step =4
 end  
  if step == 4 and car_in_point_in_radius(helicopter, -1353.6, 1153.2, 13.7,2)
  then restore_camera()
   wait(200)
  setcarspeed(helicopter,0)
  set_status_engine(helicopter,0)
  exitcar(ped1)
  miss(1000) 
  step =5
  end
  
 end

 end

 end
 end 


-- fade 1 1000 
-- Player.CanMove($PLAYER_CHAR) = True
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 0 

-- :NONAME_20_1034
-- wait 50 
-- if 
-- 0123:   actor $1374 spotted_player $PLAYER_CHAR 
-- jf @NONAME_20_1034 
-- 04C6: actor $1374 aim_gun_at_actor $PLAYER_ACTOR 
-- 00BC: text_highpriority 'COK2_E' time 3500 1  // ?t? ?g?ot© - ?t? ?ag© xot¬t o?ec?ect?t? te?¬.
-- wait 800 
-- 01CA: actor $1374 kill_player $PLAYER_CHAR 

-- :NONAME_20_1093
-- wait 50 
-- if 
   -- Actor.Dead($1374)
-- jf @NONAME_20_1093 
-- 03BC: 4@ = create_sphere -880.188 1156.804 16.831 0.75 

-- :NONAME_20_1138
-- wait 25 
-- if 
-- 00F8:   player $PLAYER_CHAR stopped 1 -880.188 1156.804 16.831 radius 0.5 0.5 1.5 
-- jf @NONAME_20_1138 
-- 03BD: destroy_sphere 4@ 
-- Player.CanMove($PLAYER_CHAR) = False
-- 0372: set_actor $PLAYER_ACTOR anim 19 wait_state_time 1500 ms 
-- wait 1500 
-- Player.CanMove($PLAYER_CHAR) = True
-- 00BC: text_highpriority 'COK2_F' time 2500 1  // ?to ?a ?po??ema c ?t?m ge?om.
-- 057D: play_bridge_status_mp3 2 
-- 03BC: 5@ = create_sphere -876.407 1160.341 16.836 1.0 

-- :NONAME_20_1268
-- wait 25 
-- if 
-- 00F8:   player $PLAYER_CHAR stopped 1 -876.407 1160.341 16.836 radius 1.0 1.0 1.0 
-- jf @NONAME_20_1268 
-- fade 0 1000 
-- Player.CanMove($PLAYER_CHAR) = False
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 1 
-- wait 1000 
-- 020C: create_explosion 2 at -878.346 1159.822 10.252 
-- 020C: create_explosion 11 at -878.346 1159.822 10.252 
-- 020C: create_explosion 1 at -878.346 1159.822 10.252 
-- 020C: create_explosion 6 at -878.346 1159.822 10.252 
-- 039D: scatter_particles 4 5.0 0 0 0 11000 at -878.346 1159.822 10.252 16.0 0.0 0.1 
-- 039D: scatter_particles 4 5.0 0 0 0 11000 at -878.346 1159.822 10.252 16.0 0.0 0.1 
-- 039D: scatter_particles 4 5.0 0 0 0 11000 at -878.346 1159.822 10.252 16.0 0.0 0.1 
-- 039D: scatter_particles 4 5.0 0 0 0 11000 at -878.346 1159.822 10.252 16.0 0.0 0.1 
-- 039D: scatter_particles 4 5.0 0 0 0 11000 at -878.346 1159.822 10.252 16.0 0.0 0.1 
-- 041D: set_camera_near_clip 0.1 
-- wait 4000 
-- Car.PutAt($1378, -858.733, 1170.692, 10.02)
-- Car.Angle($1378) = 180.0
-- 0465: remove_actor $1375 from_turret_mode 
-- Actor.PutAt($1375, -870.627, 1159.935, 10.087)
-- 02AB: set_actor $1375 immunities BP 0 FP 0 EP 0 CP 0 MP 0 
-- Actor.Health($1375) = 58
-- Actor.LockInCurrentPosition($1375) = True
-- 03BD: destroy_sphere 5@ 
-- select_interior 0 
-- 0055: put_player $PLAYER_CHAR at -882.154 1159.136 11.252 
-- 0239: actor $PLAYER_ACTOR run_to -874.822 1159.451 
-- Camera.SetPosition(-882.154, 1159.136, 11.252, 0.0, 0.0, 0.0)
-- Camera.OnPed($PLAYER_ACTOR, 15, 2)
-- $1376 = Actor.Create(Gang2, #BKA, -868.776, 1163.003, 10.138)
-- $1377 = Actor.Create(Gang2, #BKB, -865.444, 1155.315, 9.89)
-- 01B2: give_actor $1376 weapon 19 ammo 9999 // Load the weapon model before using this 
-- 01B2: give_actor $1377 weapon 22 ammo 9999 // Load the weapon model before using this 
-- Actor.Angle($1376) = 98.0
-- Actor.Angle($1377) = 82.0
-- Actor.LockInCurrentPosition($1376) = True
-- Actor.LockInCurrentPosition($1377) = True
-- $1380 = Car.Create(#FREEWAY, -867.858, 1164.09, 10.156)
-- $1381 = Car.Create(#FREEWAY, -867.805, 1155.959, 9.917)
-- fade 1 500 
-- wait 1000 
-- 02C2: car $1378 drive_to_point -865.499 1102.246 9.934 
-- Car.SetMaxSpeed($1378, 15.0)

-- :NONAME_20_2033
-- wait 50 
-- if 
-- 00E7:   player $PLAYER_CHAR stopped 0 -874.822 1159.451 radius 0.5 0.5 
-- jf @NONAME_20_2033 
-- 01C9: actor $1376 kill_actor $1375 
-- 01C9: actor $1377 kill_actor $1375 
-- 020A: set_car $1378 door_status_to 7 
-- wait 2000 
-- Car.Destroy($1378)
-- Player.CanMove($PLAYER_CHAR) = True
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 0 
-- Camera.Restore_WithJumpCut
-- wait 1500 
-- 0243: set_actor $1376 ped_stats_to 16 
-- 011A: set_actor $1376 search_threat 16 
-- 0350: set_actor $1376 maintain_position_when_attacked 1 
-- 0483: set_actor $1376 cease_attack_timer 1500 
-- Actor.Health($1376) = 300
-- 0243: set_actor $1377 ped_stats_to 16 
-- 011A: set_actor $1377 search_threat 16 
-- 0350: set_actor $1377 maintain_position_when_attacked 1 
-- 0483: set_actor $1377 cease_attack_timer 1500 
-- Actor.Health($1377) = 300
-- 01CA: actor $1376 kill_player $PLAYER_CHAR 
-- 01CA: actor $1377 kill_player $PLAYER_CHAR 
-- 018A: $1384 = create_checkpoint_at 19.645 963.158 9.998 
-- 03BC: 6@ = create_sphere 19.645 963.158 9.998 5.0 
-- $9 = 0 // $ = int 
-- $10 = 0 // $ = int 
-- $28 = 1 // $ = int 

-- :NONAME_20_2285
-- wait 50 
-- if and
   -- Actor.Dead($1376)
   -- Actor.Dead($1377)
-- jf @NONAME_20_2540 
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 1 
-- Player.CanMove($PLAYER_CHAR) = False
-- fade 0 500 
-- wait 500 
-- 02A3: enable_widescreen 1 
-- Camera.SetPosition(-870.627, 1159.235, 30.07, 0.0, 0.0, 0.0)
-- 0055: put_player $PLAYER_CHAR at -870.627 1159.235 10.087 
-- 0171: set_player $PLAYER_CHAR z_angle_to 270.0 
-- Camera.OnPed($PLAYER_ACTOR, 15, 2)
-- fade 1 500 
-- wait 500 
-- 0460: set_camera_pointing_time 0.0 3500 
-- Camera.SetPosition(-870.627, 1159.235, 15.07, 0.0, 0.0, 0.0)
-- Camera.OnPed($PLAYER_ACTOR, 15, 1)
-- 00BC: text_highpriority 'COK2_G' time 5000 1  // ?to t© gymae¦?, ?to t© ge?ae¦??
-- wait 5000 
-- Camera.Restore
-- 022F: set_actor $PLAYER_ACTOR stop_looking 
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 0 
-- Player.CanMove($PLAYER_CHAR) = True
-- $9 = 0 // $ = int 
-- $10 = 0 // $ = int 
-- 02A3: enable_widescreen 0 
-- jump @NONAME_20_2667 

-- :NONAME_20_2540
-- wait 50 
-- if 
-- 00F5:   player $PLAYER_CHAR 0 19.645 963.158 9.998 radius 2.0 2.0 2.0 
-- jf @NONAME_20_2285 
-- 03BD: destroy_sphere 6@ 
-- $36 = 1 // $ = int 
-- 0394: play_music 1 
-- 00BA: text_styled 'M_PASSN' 5000 ms 1  // m?cc?¬ ?po?geha!
-- fade 0 1000 
-- 012A: put_player $PLAYER_CHAR at 19.645 963.158 9.998 and_remove_from_car 
-- wait 2000 
-- 0318: set_latest_mission_passed 'COK_1'  // ?o?oh¬
-- return 

-- :NONAME_20_2667
-- wait 50 
-- if 
-- 00F5:   player $PLAYER_CHAR 0 19.645 963.158 9.998 radius 2.0 2.0 2.0 
-- jf @NONAME_20_2667 
-- 03BD: destroy_sphere 6@ 
-- 0394: play_music 1 
-- 00BA: text_styled 'M_PASSN' 5000 ms 1  // m?cc?¬ ?po?geha!
-- $36 = 1 // $ = int 
-- fade 0 1000 
-- 012A: put_player $PLAYER_CHAR at 19.645 963.158 9.998 and_remove_from_car 
-- wait 2000 
-- 0318: set_latest_mission_passed 'COK_1'  // ?o?oh¬
-- return 

-- :NONAME_20_2794
-- 00BA: text_styled 'M_FAIL' 5000 ms 1  // m?cc?¬ ?poa?eha!
-- wait 2000 
-- return 

-- :NONAME_20_2816
-- 054C: use_GXT_table 'INTRO' 
-- 00BC: text_highpriority 'INT1_X' time 5000 1  // ? gaem emy hemho?o gehe? ha packpytky, ok?
-- wait 1400 
-- 0322: kill_player $PLAYER_CHAR 
-- jump @NONAME_20_2794 







-- 00F5:   player $PLAYER_CHAR 0 -875.235 1159.456 10.223 radius 2.0 2.0 2.0 
-- jf @NONAME_20_753 
-- $9 = 1 // $ = int 
-- $10 = 1 // $ = int 
-- 03BD: destroy_sphere $1382 
-- Marker.Disable($1383)
-- Player.CanMove($PLAYER_CHAR) = False
-- 03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 1 
-- 0239: actor $PLAYER_ACTOR run_to -885.376 1159.004 
-- wait 3000 
-- fade 0 1000 
-- wait 1000 
-- 011C: actor $PLAYER_ACTOR clear_objective 
-- select_interior 9 
-- 0055: put_player $PLAYER_CHAR at -876.407 1160.341 16.836 
-- 0171: set_player $PLAYER_CHAR z_angle_to 188.883 

 -- if Keypress("VK_J") -- если клавиша H.
  -- then x,y,z = getcoordinates_on_y(player, 15)
  -- Giveweaponped(ped,600,"m4")-- дать педу оружие.
  -- Giveweaponped(player,600,"uzi")-- дать педу оружие.
  -- wait(200)
  -- addmoney_ped(ped,100)
  -- create_shots_on_car(car,1,100 )
 -- create_shots(ped, x, y, z)    x,y,z = getcoordinates_on_y(player, 5)
	-- car = Createcar("MODEL_PCJ600",x,y,z)
     -- wait(300)	
   -- ped1 = set_ped_in_car(car, "HFORI") -- уст водителя авто.
  -- ped2 = set_ped_in_car(car, "HFORI",0) -- уст пассажира авто.
  -- ped3 = set_ped_in_car(car, "HFORI",1) -- уст пассажира авто.
  -- ped4 = set_ped_in_car(car, "HFORI",2) -- уст пассажира авто.
  -- ped = Createped("BMYBB", x,y,z)
 -- p1, m1 = Create_ped_and_give_weapon("BMYBB", "uzi", x,y,z)-- создать врага.
  -- remove_car(car)
   -- wait(300)
  -- givemoney(10)
-- if is_button_pressed(17)
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

 
 
 
 
 
 
 
 
 
 
-- $ONMISSION = 1 // $ = int 
-- 054C: use_GXT_table 'BARON2' 
-- 00BA: text_styled 'COK2_B' 4000 ms 2  // gypak! gypak! gypak! gypak!
-- select_interior 0 
-- 023C: load_special_actor 3 'IGWMYPI' 
-- Model.Load(#RHINO)
-- Model.Load(#ARMY)
-- Model.Load(#M4)
-- Model.Load(#BKA)
-- Model.Load(#BKB)
-- Model.Load(#HUNTER)
-- Model.Load(#BUDDYSHOT)
-- Model.Load(#FREEWAY)
-- Model.Load(#TEC9)
-- Model.Load(#CHROMEGUN)
-- 038B: load_requested_models 
-- wait 1000 
-- Player.CanMove($PLAYER_CHAR) = False
-- 01EB: set_traffic_density_multiplier_to 0.0 
-- 03DE: set_pedestrians_density_multiplier_to 0.0 

-- $1375 = Actor.Create(Gang2, #ARMY, 231.774, -1272.967, 18.29)
-- $1379 = Car.Create(#HUNTER, -1721.964, -275.942, 13.887)
-- Car.Angle($1379) = 270.0
-- 0129: 2@ = create_actor 4 #ARMY in_car $1379 driverseat 
-- 01C8: 3@ = create_actor_pedtype 4 model #ARMY in_car $1379 passenger_seat 0 
-- $1378 = Car.Create(#RHINO, -1722.805, -263.981, 13.887)
-- Car.Angle($1378) = 180.0

-- 01B1: give_player $PLAYER_CHAR weapon 26 ammo 90 // Load the weapon model before using this 
-- 01B2: give_actor $1375 weapon 26 ammo 9999 // Load the weapon model before using this 
-- 0369: put_player $PLAYER_CHAR in_car $1378 
-- Car.PutAt($1378, -1722.805, -263.981, 13.887)
-- Car.Angle($1378) = 180.0

-- 0464: put_actor $1375 into_turret_on_car $1378 at_car_offset 0.0 -1.0 2.0 position 0 angle 0.0 with_weapon 26 
-- 02AB: set_actor $1375 immunities BP 1 FP 1 EP 1 CP 1 MP 1 
-- 018A: $1383 = create_checkpoint_at -875.235 1159.456 10.223 
-- 03BC: $1382 = create_sphere -875.235 1159.456 10.223 2.0 
-- fade 1 1000 

-- 04A2: heli $1379 fly_to -1721.964 -275.942 40.959 speed 40 
-- wait 2500 
-- 04A2: heli $1379 fly_to -1728.515 -238.666 50.959 speed 50 

-- Camera.SetPosition(-1722.805, -254.007, 18.085, 0.0, 0.0, 0.0)
-- Camera.OnVehicle($1379, 15, 2)
-- 00BC: text_highpriority 'COK2_C' time 4000 1  // tomm?!
-- wait 3000 
-- 00BC: text_highpriority 'COK2_D' time 3500 1  // ?to, p?kapgo?
-- wait 1500 
-- Player.CanMove($PLAYER_CHAR) = True
-- Camera.Restore
-- $9 = 0 // $ = int 
-- $10 = 0 // $ = int 
-- $28 = 1 // $ = int 
-- wait 3000 
-- Actor.DestroyInstantly(2@)
-- Actor.DestroyInstantly(3@)
-- Car.Destroy($1379)

-- :NONAME_20_753
-- wait 50 
-- if 
   -- not Car.Wrecked($1378)
-- jf @NONAME_20_2816 
-- if 

-- $1374 = Actor.Create(Gang2, #SPECIAL03, -879.333, 1156.702, 16.831)
-- 0243: set_actor $1374 ped_stats_to 16 
-- 011A: set_actor $1374 search_threat 1 
-- 011A: set_actor $1374 search_threat 16 
-- 01B2: give_actor $1374 weapon 21 ammo 9999 // Load the weapon model before using this 
-- 0350: set_actor $1374 maintain_position_when_attacked 1 
-- 0483: set_actor $1374 cease_attack_timer 1500 
-- Actor.Health($1374) = 300
-- wait 500 


  --Giveweaponped(ped2, 600,"m4")-- дать педу оружие.
  
  --ped2 = set_ped_in_car(helicopter,"ARMY",0 )
  --setcord(player, 529.6, 370.1, 10.9)
  --ped2 = set_ped_in_car(tank,"ARMY",0 )
  -- setcord(player,-1722.805, -263.981, 13.887 )
  
  -- camera_at_point(-1722.805, -254.007, 18.085, 0.0, 0.0, 0.0)
  