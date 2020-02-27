require("lualoader/mod")

function main()
while lualoader == nil do
 wait()-- задержка.
  player = findplayer()-- получить игрока
  if Keypress(VK_H) -- если клавиша H.
 then -- 
 x,y,z = getcoordinates_on_y(player,10)-- получить координаты на 5 м впереди.-- 
 printmessage("yes", 2000,1)-- 
 mycar = Createcar(MODEL_INFERNUS,x,y,z) -- создать авто на 5 впереди.
 Giveweaponped(player,600,"uzi")-- дать педу оружие.
   -- set_widescreen(true)--
 -- setcord(player,0.0,0.0,0.0)
-- givemoney(3000) setcarcoordes(mycar,0.0,0.0,0.0)
end	-- 
end
end  
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
