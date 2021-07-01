require("lualoader/mod")

function foel_for_ped(model, ped1, weapon, x,y,z, angle)
 local ped = Createped(model, x,y,z)-- создать педа.
 setpedangle(ped, angle)-- уст угол педа.
 Giveweaponped(ped,600, weapon)-- дать оружия педу.
 local m  = create_marker(ped)-- создать маркер над педом.
 set_ped_accuracy(ped, 60)-- уст меткость педа.
 kill_ped_on_foot(ped,ped1)
while true == getflagmission() do wait(10) -- цикл пока статус миссии не изменится. 
   if getpedhealth(ped) == 0 -- если пед мертв.
  then removemarker(m) remove_ped(ped)  break end     
  end 
  wait(3000)
  destroy()
 end
function teleport(car)
wait(7100)
 fade(0,900) 
  wait(1200) 
setcarcoordes(car,-1041.3, -20.8, 63.9)  wait(3000) 
--load_scene(-1041.3, -20.8, 13.9) wait(3000)  --
heli_to_fly(car,-1170.5, -144.5, 28.2, 7) 
fade(1,700) 
end

function main() while true do wait() player = findplayer()-- получить игрока

  if Star_mission_marker(30, 492.3, -1236.1, 11.1 ) -- создать маркер миссии на карте.
  then showtext(" Dirty Laundry", 1700,1)-- вывод названия миссии. 
  fade(1,6900) --просветления, 600 время. 
  car, pilot = create_car_with_driver_back_car("MODEL_MAVERICK","ARMY", 493.7, -1257.3, 11.1, 90)-- создать вертолет с пилотом.  
  m = create_marker_car(car)
  printmessage("~r~get in the helicopter.",3600,1) ped_frozen(0) wait(1000) 
  ped_car_as_passenger(player, car) step = 1 
while true == Getflagmission() do wait(350) -- цикл пока статус миссии не изменится. 
   if is_ped_in_this_car(player, car) and step == 1
   then step = 2  removemarker(m) heli_to_fly(car,-1170.5, -144.5, 48.2,10) 
   printmessage("~r~fly to the \"Little Havana\".",3800,1)  teleport(car)  
   end   
 
   if car_in_radius_mod(car, -1170.5, -144.5, 28.2, 7,8, 5) and step == 2 
   then  step=3 printmessage("~r~I'm dropping you off on the roof of this building.",3100,1)  heli_change_height(car,14.9, 1)-- снижения вертолета.
   end    
   
   if car_in_radius_mod(car, -1170.5, -144.5, 14.9, 6,5,1.0) and step == 3 
   then exitcar(player) wait(2600) ped_frozen(1)  heli_change_height(car,99,15)  
   printmessage("~r~get out of the helicopter, get your briefcase, and I'll be waiting for you on the roof.", 3500,1)
   p1 = Create_pickup(335, 3, -1197.1, -324.3, 10.9) m = create_marker_pickup(p1) wait(3600) 
   printmessage("~y~pick up the product.", 3500,1) step=4
   end    
   
   if step == 4 then
   if picked_up(p1) 
   then printmessage("~r~I'm here, come here!", 4000,1) removemarker(m) remove_pickup(p1) 
   create_newthread(foel_for_ped,"BMYCR", player,"m4", -1201.1, -360.5, 10.9, 5 )-- создать врага.
   create_newthread(foel_for_ped, "SGB", player,"m4", -1201.1, -363.5, 10.9,  5 )-- создать врага.
   step=5  m,sp= create_marker_and_sphere(-890.9, 102.2, 16.0, 3,3.0)
   end  end

   if ped_in_point_in_radius(player, -890.9, 102.2, 16.0, 2.0,2.0,2.0) and step == 5    
   then remove_blip(m) remove_sphere(sp) m1= create_marker_car(car) step = 6 
   printmessage("~r~come on, I'm already on the approach, I'll land soon", 4000,1)
   heli_to_fly(car, -890.9, 102.2, 36.0, 20)
   end
   
   if car_in_radius_mod(car, -890.9, 102.2, 36.0, 4,4, 1) and step == 6 
   then removemarker(m1)  heli_change_height(car,15.9, 1)-- снижения вертолета.
   step = 7 end
   
   if car_in_radius_mod(car, -890.9, 102.2, 15.9, 3,3, 1) and step == 7    
   then ped_car_as_passenger(player, car) step =8
    end
	
   if is_ped_in_this_car(player, car) and step==8
   then heli_to_fly(car, -890.9, 102.2, 105.9,70) wait(3100)
   heli_to_fly(car, 503.1, -1253.9, 109.1, 70) m1,sp1= create_marker_and_sphere(503.1, -1253.9, 11.3, 3,8.0) step = 9 
   
   printmessage("~r~now we are flying back to the beach, I will try to fly quickly.", 4000,1)
   end
   
      if car_in_radius_mod(car, 503.1, -1253.9, 109.1, 4,4, 1) and step == 9 
   then heli_to_fly(car, 503.1, -1253.9, 11.3, 10)  step = 10 end
   
   if car_in_radius_mod(car, 503.1, -1253.9, 11.3, 5,5, 3) and step == 10 
   then removemarker(m1) remove_sphere(sp1) wait(1500)  exitcar(player) 
   exitcar(pilot)   miss(5000)   break 
   end
 if 0 == getcarhealth(car)  
  then printmessage("~r~You have tightened the car", 4000,1)
  end_mission("mission failed!")  break end 
   
  end

  end
 end
 end
 --[[
 492.1, -1238.1, 11.1 -- стар мисси
493.7, -1247.3, 11.1 -- место вертолета
-1041.3, -20.8, 53.9  переместить вертолет.

-1201.1, -360.5, 10.9 angle = 5 -- 1
-1198.4, -360.2, 10.9 angle = 31 -- 2

 -1170.5, -144.5, 14.2-- зона посадки
 -1197.1, -324.3, 10.9 -- пикап
 
 -890.9, 102.2, 16.0  -- зона эвакуации.
--]]
 --[[
 
fade 0 100 
$ONMISSION = 1 // $ = int 
Model.Load(#MAVERICK)
Model.Load(#ARMY)
Model.Load(#SWAT)
Model.Load(#M60)
Model.Load(#M4)
Model.Load(#BARRACKS)
038B: load_requested_models 

0055: put_player $PLAYER_CHAR at -44.723 958.24 9.959 
01EB: set_traffic_density_multiplier_to 0.0 
$324 = Car.Create(#MAVERICK, -36.782, 952.42, 9.959)
0506: vehicle_model 163 set_next_variation -1 2 
$703 = Car.Create(#BARRACKS, -32.75, 967.884, 9.959)
053F: set_car $703 tires_vulnerable 1 
Car.SetImmunities($703, 1, 1, 1, 1, 1)
0423: car $703 improve_handling 1.5 
$704 = Marker.CreateAboveCar($703)
0167: $705 = create_marker_at 41.5 -1058.013 15.424 color 1 display 3 
Car.Angle($324) = 270.0
Car.Angle($703) = 270.0
0229: set_car $324 color_to 3 6 
Car.Health($324) = 3000
Car.Health($703) = 3000
0129: $LANCE_VANCE = create_actor 4 #SWAT in_car $324 driverseat 
0446: set_actor $LANCE_VANCE dismemberment_possible 0 
04F5: set_actor $LANCE_VANCE as_player_friend $PLAYER_CHAR flag 1 
0129: $323 = create_actor 8 #ARMY in_car $703 driverseat 
0526: set_actor $323 stay_in_car_when_jacked 1 
01C8: $686 = create_actor_pedtype 8 model #ARMY in_car $703 passenger_seat 0 
0526: set_actor $686 stay_in_car_when_jacked 1 
$690 = Actor.Create(Gang2, #ARMY, -686.0, 665.0, 10.1)
0464: put_actor $690 into_turret_on_car $703 at_car_offset 0.4242 -0.0679 1.1322 position 3 angle 270.0 with_weapon 26 
$687 = Actor.Create(Gang2, #ARMY, -686.0, 664.0, 10.1)
0464: put_actor $687 into_turret_on_car $703 at_car_offset -0.7608 -1.4 1.1138 position 1 angle 270.0 with_weapon 26 
$688 = Actor.Create(Gang2, #ARMY, -686.0, 663.0, 10.1)
0464: put_actor $688 into_turret_on_car $703 at_car_offset -0.7595 -3.5704 1.1108 position 2 angle 270.0 with_weapon 26 
$689 = Actor.Create(Gang2, #ARMY, -686.0, 662.0, 10.1)
0464: put_actor $689 into_turret_on_car $703 at_car_offset 0.7158 -3.6996 1.1162 position 2 angle 270.0 with_weapon 26 
0464: put_actor $PLAYER_ACTOR into_turret_on_car $324 at_car_offset -1.4 1.0 -0.1 position 1 angle 90.0 with_weapon 32 
0169: set_fade_color 1 1 1 
$37 = 20.46167 // $ = float 
$38 = 1126.089 // $ = float 
$39 = 17.12396 // $ = float 
$12 = 0 // $ = int 
$706 = 0 // $ = float 
$13 = 0 // $ = int 
fade 1 1000 
Camera.Restore_WithJumpCut
054C: use_GXT_table 'BARON5' 
00BC: text_highpriority 'ASS1_1' time 5000 1  // ?to mecto ha?o?heho ?p?gypkam?...octopo?hee...
00AE: set_vehicle $703 traffic_behavior_to 2 
Car.DriveTo($703, 21.43, 961.824, 10.189)
$28 = 0 // $ = int 
create_thread @FLY 
03CC: add_stuck_car_check $703 distance 0.5 time 4500 

:NONAME_3_1053
wait 50 
0208: $707 = random_float -3.0 3.0 
000B: $37 += $707 // @ += float  // Note: the incorrect math opcode was used here
000B: $38 += $707 // @ += float  // Note: the incorrect math opcode was used here
if 
01AF:   car $703 0 0.219 964.186 9.959 radius 1.0 1.0 3.0 
else_jump @NONAME_3_1053 
Car.PutAt($703, 20.583, 961.337, 9.792)
Player.CanMove($PLAYER_CHAR) = True
03CC: add_stuck_car_check $703 distance 0.5 time 4500 

:NONAME_3_1176
wait 50 
0208: $707 = random_float -3.0 3.0 
000B: $37 += $707 // @ += float  // Note: the incorrect math opcode was used here
000B: $38 += $707 // @ += float  // Note: the incorrect math opcode was used here
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 21.43 961.824 10.189 radius 7.5 7.5 3.0 
else_jump @NONAME_3_1176 
Car.DriveTo($703, 33.628, 1016.851, 10.203)

:NONAME_3_1298
wait 50 
0208: $707 = random_float -3.0 3.0 
000B: $37 += $707 // @ += float  // Note: the incorrect math opcode was used here
000B: $38 += $707 // @ += float  // Note: the incorrect math opcode was used here
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 33.628 1016.851 10.203 radius 3.5 3.5 3.0 
else_jump @NONAME_3_1298 
Car.DriveTo($703, 99.155, 1003.941, 10.314)
$708 = 99.155 // $ = float 
$709 = 1003.155 // $ = float 
$710 = 10.314 // $ = float 
Car.SetMaxSpeed($703, 15.0)

:NONAME_3_1460
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 99.155 1003.941 10.314 radius 7.5 7.5 3.0 
else_jump @NONAME_3_1460 
Car.DriveTo($703, 191.167, 974.139, 23.774)
$37 = 425.7164 // $ = float 
$38 = 964.298 // $ = float 
$39 = 13.21138 // $ = float 
$708 = 191.16 // $ = float 
$709 = 974.139 // $ = float 
$710 = 23.774 // $ = float 
Model.Destroy(#MAVERICK)
Model.Destroy(#M60)
Model.Destroy(#M4)
Model.Destroy(#ARMY)
Model.Destroy(#BARRACKS)
Model.Destroy(#SWAT)

:NONAME_3_1639
wait 50 
0208: $707 = random_float -3.0 3.0 
000B: $37 += $707 // @ += float  // Note: the incorrect math opcode was used here
000B: $38 += $707 // @ += float  // Note: the incorrect math opcode was used here
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 191.167 974.139 23.774 radius 3.5 3.5 3.0 
else_jump @NONAME_3_1639 
Car.DriveTo($703, 383.17, 945.997, 14.46)
$708 = 383.1 // $ = float 
$709 = 945.997 // $ = float 
$710 = 14.46 // $ = float 

:NONAME_3_1791
wait 50 
0208: $707 = random_float -3.0 3.0 
000B: $37 += $707 // @ += float  // Note: the incorrect math opcode was used here
000B: $38 += $707 // @ += float  // Note: the incorrect math opcode was used here
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 383.17 945.997 14.46 radius 7.5 7.5 3.0 
else_jump @NONAME_3_1791 
Car.DriveTo($703, 419.034, 905.716, 10.808)
$708 = 419.03 // $ = float 
$709 = 905.716 // $ = float 
$710 = 10.808 // $ = float 

:NONAME_3_1943
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 419.034 905.716 10.808 radius 3.5 3.5 3.0 
else_jump @NONAME_3_1943 
Car.DriveTo($703, 457.768, 901.227, 11.897)
$708 = 457.768 // $ = float 
$709 = 901.227 // $ = float 
$710 = 11.897 // $ = float 

:NONAME_3_2064
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 457.768 901.227 11.897 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2064 
Car.DriveTo($703, 525.657, 933.35, 15.861)
$708 = 525.657 // $ = float 
$709 = 933.35 // $ = float 
$710 = 15.861 // $ = float 

:NONAME_3_2185
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 525.657 933.35 15.861 radius 3.5 3.5 3.0 
else_jump @NONAME_3_2185 
Car.DriveTo($703, 560.371, 877.003, 13.791)
$708 = 560.371 // $ = float 
$709 = 877.003 // $ = float 
$710 = 13.791 // $ = float 

:NONAME_3_2306
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 560.371 877.003 13.791 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2306 
Car.DriveTo($703, 550.393, 778.349, 10.458)
$708 = 550.393 // $ = float 
$709 = 778.349 // $ = float 
$710 = 10.458 // $ = float 

:NONAME_3_2469
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 550.393 778.349 10.458 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2469 
Car.DriveTo($703, 533.877, 609.485, 10.315)
$708 = 533.877 // $ = float 
$709 = 609.485 // $ = float 
$710 = 10.315 // $ = float 

:NONAME_3_2632
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 533.877 609.485 10.315 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2632 
Car.DriveTo($703, 539.2, 520.983, 10.317)
$708 = 539.2 // $ = float 
$709 = 520.98 // $ = float 
$710 = 10.317 // $ = float 

:NONAME_3_2795
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 539.2 520.983 10.317 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2795 
Car.DriveTo($703, 507.52, 325.362, 10.318)
$708 = 507.52 // $ = float 
$709 = 325.36 // $ = float 
$710 = 10.31 // $ = float 

:NONAME_3_2958
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 507.52 325.362 10.318 radius 7.5 7.5 3.0 
else_jump @NONAME_3_2958 
Car.DriveTo($703, 502.796, 187.215, 10.323)
$708 = 502.79 // $ = float 
$709 = 187.21 // $ = float 
$710 = 10.32 // $ = float 

:NONAME_3_3121
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 502.796 187.215 10.323 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3121 
Car.DriveTo($703, 491.488, 50.615, 10.308)
$708 = 491.488 // $ = float 
$709 = 50.615 // $ = float 
$710 = 10.308 // $ = float 

:NONAME_3_3284
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 491.488 50.615 10.308 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3284 
Car.DriveTo($703, 511.771, -34.737, 10.316)
$708 = 511.771 // $ = float 
$709 = -34.737 // $ = float 
$710 = 10.316 // $ = float 

:NONAME_3_3447
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 511.771 -34.737 10.316 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3447 
Car.DriveTo($703, 515.583, -91.693, 9.816)
$708 = 515.583 // $ = float 
$709 = -91.693 // $ = float 
$710 = 9.816 // $ = float 

:NONAME_3_3610
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 515.583 -91.693 9.816 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3610 
Car.DriveTo($703, 482.694, -110.727, 9.985)
$708 = 482.69 // $ = float 
$709 = -110.727 // $ = float 
$710 = 9.985 // $ = float 

:NONAME_3_3773
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 482.694 -110.727 9.985 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3773 
Car.DriveTo($703, 481.993, -195.508, 10.01)
$708 = 481.993 // $ = float 
$709 = -195.508 // $ = float 
$710 = 10.01 // $ = float 

:NONAME_3_3936
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 481.993 -195.508 10.01 radius 7.5 7.5 3.0 
else_jump @NONAME_3_3936 
Car.DriveTo($703, 465.495, -294.288, 9.602)
$708 = 465.495 // $ = float 
$709 = -294.288 // $ = float 
$710 = 9.6 // $ = float 

:NONAME_3_4099
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 465.495 -294.288 9.602 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4099 
Car.DriveTo($703, 419.819, -399.419, 9.374)
$708 = 419.819 // $ = float 
$709 = -399.419 // $ = float 
$710 = 9.3 // $ = float 

:NONAME_3_4262
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 419.819 -399.419 9.374 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4262 
Car.DriveTo($703, 347.116, -465.604, 9.313)
$708 = 347.11 // $ = float 
$709 = -465.6 // $ = float 
$710 = 9.313 // $ = float 

:NONAME_3_4425
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 347.116 -465.604 9.313 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4425 
Car.DriveTo($703, 308.319, -516.693, 9.299)
$708 = 308.319 // $ = float 
$709 = -516.69 // $ = float 
$710 = 9.29 // $ = float 

:NONAME_3_4588
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 308.319 -516.693 9.299 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4588 
Car.DriveTo($703, 255.006, -602.285, 9.3)
$708 = 255.006 // $ = float 
$709 = -602.28 // $ = float 
$710 = 9.3 // $ = float 

:NONAME_3_4751
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 255.006 -602.285 9.3 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4751 
Car.DriveTo($703, 238.407, -660.259, 9.444)
$708 = 238.407 // $ = float 
$709 = -660.259 // $ = float 
$710 = 9.444 // $ = float 

:NONAME_3_4914
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 238.407 -660.259 9.444 radius 7.5 7.5 3.0 
else_jump @NONAME_3_4914 
Car.DriveTo($703, 213.64, -752.812, 9.698)
$708 = 213.64 // $ = float 
$709 = -752.812 // $ = float 
$710 = 9.698 // $ = float 

:NONAME_3_5077
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 213.64 -752.812 9.698 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5077 
Car.DriveTo($703, 153.882, -843.362, 9.698)
$708 = 153.882 // $ = float 
$709 = -843.362 // $ = float 
$710 = 9.698 // $ = float 

:NONAME_3_5240
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 153.882 -843.362 9.698 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5240 
Car.DriveTo($703, 105.745, -911.075, 9.698)
$708 = 105.745 // $ = float 
$709 = -911.075 // $ = float 
$710 = 9.698 // $ = float 

:NONAME_3_5403
0208: $711 = random_float -5.0 5.0 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
wait 50 
if 
01AF:   car $703 0 105.745 -911.075 9.698 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5403 
Car.DriveTo($703, 80.194, -968.506, 9.716)
$708 = 80.194 // $ = float 
$709 = -968.506 // $ = float 
$710 = 9.716 // $ = float 

:NONAME_3_5566
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 80.194 -968.506 9.716 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5566 
Car.DriveTo($703, 75.003, -1016.882, 9.728)
$708 = 75.003 // $ = float 
$709 = -1016.88 // $ = float 
$710 = 9.72 // $ = float 

:NONAME_3_5729
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 75.003 -1016.882 9.728 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5729 
Car.DriveTo($703, 74.905, -1044.98, 9.729)
$708 = 74.905 // $ = float 
$709 = -1044.98 // $ = float 
$710 = 9.72 // $ = float 

:NONAME_3_5892
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 74.905 -1044.98 9.729 radius 7.5 7.5 3.0 
else_jump @NONAME_3_5892 
Car.DriveTo($703, 64.869, -1065.978, 9.879)
$708 = 64.869 // $ = float 
$709 = -1065.978 // $ = float 
$710 = 9.879 // $ = float 

:NONAME_3_6055
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 64.869 -1065.978 9.879 radius 3.5 3.5 3.0 
else_jump @NONAME_3_6055 
Car.DriveTo($703, 63.354, -1074.404, 9.879)
$708 = 64.869 // $ = float 
$709 = -1074.404 // $ = float 
$710 = 9.879 // $ = float 

:NONAME_3_6218
wait 50 
if 
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 63.354 -1074.404 9.879 radius 3.5 3.5 3.0 
else_jump @NONAME_3_6218 
020A: set_car $703 door_status_to 7 
03CD: car $703 remove_from_stuck_car_check 
$706 = 1 // $ = float 
04A2: heli $324 fly_to 91.09 -1072.919 40.0 speed 30 
$28 = 1 // $ = float 

:NONAME_3_6337
wait 50 
if 
01AF:   car $324 0 91.09 -1072.919 40.0 radius 5.5 5.5 3.0 
else_jump @NONAME_3_6337 
04A2: heli $324 fly_to 37.088 -995.257 40.481 speed 30 

:NONAME_3_6411
wait 50 
if 
01AF:   car $324 0 37.088 -995.257 40.481 radius 5.5 5.5 3.0 
else_jump @NONAME_3_6411 
04A2: heli $324 fly_to 21.952 -1090.334 39.482 speed 30 

:NONAME_3_6485
wait 50 
if 
01AF:   car $324 0 21.952 -1090.334 39.482 radius 5.5 5.5 3.0 
else_jump @NONAME_3_6485 
04A2: heli $324 fly_to 82.794 -1081.043 40.332 speed 30 
$28 = 1 // $ = float 
$706 = 1 // $ = float 
fade 0 500 
wait 500 
Car.Destroy($703)
Marker.Disable($712)
Actor.DestroyInstantly($686)
Actor.DestroyInstantly($323)
Actor.DestroyInstantly($687)
Actor.DestroyInstantly($688)
Actor.DestroyInstantly($689)
Model.Load(#BKA)
Model.Load(#BKB)
Model.Load(#FREEWAY)
Model.Load(#TEC9)
Model.Load(#BARREL4)
Model.Load(#ARMY)
Model.Load(#M4)
Model.Load(#BARRACKS)
038B: load_requested_models 
wait 500 
end_thread_named 'DRIVE' 
0506: vehicle_model 163 set_next_variation -1 2 
$703 = Car.Create(#BARRACKS, 80.155, -1058.201, 9.332)
053F: set_car $703 tires_vulnerable 1 
0423: car $703 improve_handling 1.5 
Car.Health($703) = 4000
0129: $323 = create_actor 8 #ARMY in_car $703 driverseat 
0526: set_actor $323 stay_in_car_when_jacked 1 
01C8: $686 = create_actor_pedtype 8 model #ARMY in_car $703 passenger_seat 0 
0526: set_actor $686 stay_in_car_when_jacked 1 
$713 = Object.Create(#BARREL4, -5.382, 1014.999, 8.811)
Object.CollisionDetection($713) = True
035C: place_object $713 relative_to_car $703 offset 0.7608 -1.4 1.1138 
$714 = Object.Create(#BARREL4, -4.839, 1014.808, 8.811)
Object.CollisionDetection($714) = True
035C: place_object $714 relative_to_car $703 offset -0.7608 -1.4 1.1138 
$715 = Object.Create(#BARREL4, -4.26, 1014.602, 8.811)
Object.CollisionDetection($715) = True
035C: place_object $715 relative_to_car $703 offset 0.7608 -0.0679 1.1138 
$716 = Object.Create(#BARREL4, -6.343, 1014.089, 8.811)
Object.CollisionDetection($716) = True
035C: place_object $716 relative_to_car $703 offset -0.7608 -0.0679 1.1138 
050E: sort_out_object $713 collision_with_car $703 
050E: sort_out_object $714 collision_with_car $703 
050E: sort_out_object $715 collision_with_car $703 
050E: sort_out_object $716 collision_with_car $703 
00AE: set_vehicle $703 traffic_behavior_to 2 
$695 = Car.Create(#FREEWAY, 76.753, -1207.015, 9.332)
$696 = Car.Create(#FREEWAY, 76.098, -1212.526, 9.332)
$697 = Car.Create(#FREEWAY, 81.4, -1212.963, 9.332)
$698 = Car.Create(#FREEWAY, 82.166, -1206.512, 9.332)
0129: $699 = create_actor 7 #BKA in_car $695 driverseat 
Actor.Health($699) = 300
0129: $700 = create_actor 7 #BKA in_car $696 driverseat 
Actor.Health($700) = 300
0129: $701 = create_actor 7 #BKA in_car $697 driverseat 
Actor.Health($701) = 300
0129: $702 = create_actor 7 #BKA in_car $698 driverseat 
Actor.Health($702) = 300
$691 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $691 into_turret_on_car $695 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Actor.Health($691) = 500
$692 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $692 into_turret_on_car $696 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Actor.Health($692) = 500
$693 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $693 into_turret_on_car $697 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Actor.Health($693) = 500
$694 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $694 into_turret_on_car $698 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Actor.Health($694) = 500
00AE: set_vehicle $695 traffic_behavior_to 2 
00AE: set_vehicle $696 traffic_behavior_to 2 
00AE: set_vehicle $697 traffic_behavior_to 2 
00AE: set_vehicle $698 traffic_behavior_to 2 
Car.SetMaxSpeed($703, 20.0)
Car.SetMaxSpeed($695, 25.0)
Car.SetMaxSpeed($696, 25.0)
Car.SetMaxSpeed($697, 25.0)
Car.SetMaxSpeed($698, 25.0)
01C9: actor $691 kill_actor $686 
01C9: actor $692 kill_actor $686 
01C9: actor $693 kill_actor $686 
01C9: actor $694 kill_actor $686 
03CC: add_stuck_car_check $703 distance 0.5 time 4500 
$685 = 1 // $ = int 
$717 = 0 // $ = int 
$718 = 0 // $ = int 
$719 = 0 // $ = int 
$720 = 0 // $ = int 
create_thread @DRIVE 
create_thread @BJT 
$706 = 0 // $ = float 
Car.PutAt($324, 82.794, -1081.043, 40.332)
041D: set_camera_near_clip 0.1 
Camera.SetPosition(78.321, -1041.475, 12.186, 0.0, 0.0, 0.0)
Camera.PointAt(81.182, -1074.379, 12.186, 2)
fade 1 500 
Player.CanMove($PLAYER_CHAR) = True
wait 500 
Model.Destroy(#ARMY)
Model.Destroy(#M4)
Model.Destroy(#BARRACKS)

:NONAME_3_7737
wait 50 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 80.155 -1058.201 9.332 radius 7.0 7.0 3.0 
else_jump @NONAME_3_7737 
02D3: boat $703 drive_to 77.016 -1034.766 9.332 
$708 = 77.016 // $ = float 
$709 = -1034.766 // $ = float 
$710 = 9.322 // $ = float 
$685 = 2 // $ = int 

:NONAME_3_7862
wait 50 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if 
01AF:   car $703 0 77.016 -1034.766 9.332 radius 7.5 7.5 3.0 
else_jump @NONAME_3_7862 
Car.DriveTo($703, 21.43, 961.824, 10.189)
$708 = 21.43 // $ = float 
$709 = 961.824 // $ = float 
$710 = 10.189 // $ = float 
$28 = 0 // $ = int 
020A: set_car $703 door_status_to 1 
Camera.Restore
Player.CanMove($PLAYER_CHAR) = True

:NONAME_3_8045
wait 50 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0208: $711 = random_float -5.0 5.0 
0407: create_coordinate $37 $38 $39 from_car $703 offset $711 60.0 0.0 
if or
   not Actor.Dead($323)
   not Actor.Dead($686)
else_jump @DRIVE_535 
if or
   Actor.InCar($323, $703)
   Actor.InCar($686, $703)
else_jump @DRIVE_535 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
if 
01AF:   car $703 0 21.43 961.824 10.189 radius 7.5 7.5 3.0 
else_jump @NONAME_3_8289 
0394: play_music 1 
0465: remove_actor $PLAYER_ACTOR from_turret_mode 
0318: set_latest_mission_passed 'COL_2'  // mo?? ˜©cko???
$22 = 1 // $ = int 
$24 = 1 // $ = int 
$21 = 1 // $ = int 
$128 = 1 // $ = int 
$25 = 1 // $ = int 
$130 = 0 // $ = int 
$82 = 1 // $ = int 
return 

:NONAME_3_8289
if 
01AF:   car $324 0 395.78 949.56 21.996 radius 7.5 7.5 3.0 
else_jump @NONAME_3_8344 
$685 = 1 // $ = int 

:NONAME_3_8344
if 
   not $717 == 3 // $ == int 
else_jump @NONAME_3_8472 
if 
   not Car.Wrecked($695)
else_jump @NONAME_3_8815 
if 
   not Actor.Dead($699)
else_jump @NONAME_3_8815 
if 
   Actor.InCar($699, $695)
else_jump @NONAME_3_8815 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
02C2: car $695 drive_to_point $721 $722 $723 

:NONAME_3_8472
if 
   not $718 == 3 // $ == int 
else_jump @NONAME_3_8584 
if 
   Actor.InCar($700, $696)
else_jump @NONAME_3_9071 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
if 
   not Car.Wrecked($696)
else_jump @NONAME_3_9071 
02C2: car $696 drive_to_point $721 $722 $723 

:NONAME_3_8584
if 
   not $719 == 3 // $ == int 
else_jump @NONAME_3_8696 
if 
   Actor.InCar($701, $697)
else_jump @NONAME_3_9327 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
if 
   not Car.Wrecked($697)
else_jump @NONAME_3_9327 
02C2: car $697 drive_to_point $721 $722 $723 

:NONAME_3_8696
if 
   not $720 == 3 // $ == int 
else_jump @NONAME_3_8045 
if 
   Actor.InCar($702, $698)
else_jump @NONAME_3_9583 
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
if 
   not Car.Wrecked($698)
else_jump @NONAME_3_9583 
02C2: car $698 drive_to_point $721 $722 $723 
jump @NONAME_3_8045 

:NONAME_3_8815
Car.RemoveReferences($695)
Actor.RemoveReferences($699)
Actor.RemoveReferences($691)
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $724 $725 $726 from_car $703 offset 0.0 -10.0 0.0 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
02C1: set $727 $728 $729 to_car_path_coords_closest_to $724 $725 $726 
$717 += 1 // $ += int 
$695 = Car.Create(#FREEWAY, $727, $728, $729)
0129: $699 = create_actor 7 #BKA in_car $695 driverseat 
Actor.Health($699) = 300
$691 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $691 into_turret_on_car $695 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Car.SetMaxSpeed($695, 25.0)
Actor.Health($691) = 500
01C9: actor $691 kill_actor $686 
02C2: car $695 drive_to_point $721 $722 $723 
jump @NONAME_3_8472 

:NONAME_3_9071
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $724 $725 $726 from_car $703 offset 0.0 -10.0 0.0 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
02C1: set $727 $728 $729 to_car_path_coords_closest_to $724 $725 $726 
$718 += 1 // $ += int 
Car.RemoveReferences($696)
Actor.RemoveReferences($700)
Actor.RemoveReferences($692)
$696 = Car.Create(#FREEWAY, $727, $728, $729)
0129: $700 = create_actor 7 #BKA in_car $696 driverseat 
Actor.Health($700) = 300
$692 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $692 into_turret_on_car $696 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Car.SetMaxSpeed($696, 25.0)
Actor.Health($692) = 500
01C9: actor $692 kill_actor $686 
02C2: car $696 drive_to_point $721 $722 $723 
jump @NONAME_3_8584 

:NONAME_3_9327
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $724 $725 $726 from_car $703 offset 0.0 -10.0 0.0 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
02C1: set $727 $728 $729 to_car_path_coords_closest_to $724 $725 $726 
$719 += 1 // $ += int 
Car.RemoveReferences($697)
Actor.RemoveReferences($701)
Actor.RemoveReferences($693)
$697 = Car.Create(#FREEWAY, $727, $728, $729)
0129: $701 = create_actor 7 #BKA in_car $697 driverseat 
Actor.Health($701) = 300
$693 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
0464: put_actor $693 into_turret_on_car $697 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Car.SetMaxSpeed($697, 25.0)
Actor.Health($693) = 500
01C9: actor $693 kill_actor $686 
02C2: car $697 drive_to_point $721 $722 $723 
jump @NONAME_3_8696 

:NONAME_3_9583
if 
   not Car.Wrecked($703)
else_jump @DRIVE_535 
0407: create_coordinate $724 $725 $726 from_car $703 offset 0.0 -10.0 0.0 
0407: create_coordinate $721 $722 $723 from_car $703 offset 0.0 -0.5 0.0 
02C1: set $727 $728 $729 to_car_path_coords_closest_to $724 $725 $726 
$720 += 1 // $ += int 
Car.RemoveReferences($698)
Actor.RemoveReferences($702)
Actor.RemoveReferences($694)
$698 = Car.Create(#FREEWAY, $727, $728, $729)
0129: $702 = create_actor 7 #BKA in_car $698 driverseat 
Actor.Health($702) = 300
$694 = Actor.Create(Gang1, #BKB, 84.166, -1208.512, 9.332)
Actor.Health($694) = 500
0464: put_actor $694 into_turret_on_car $698 at_car_offset 0.0 -0.679 0.6322 position 3 angle 270.0 with_weapon 22 
Car.SetMaxSpeed($698, 25.0)
02AB: set_actor $694 immunities BP 1 FP 1 EP 1 CP 1 MP 1 
01C9: actor $694 kill_actor $686 
02C2: car $698 drive_to_point $721 $722 $723 
jump @NONAME_3_8045 
 
 
 
 ]]
 
 
 -- end
 
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
