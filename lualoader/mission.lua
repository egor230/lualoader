require("lualoader/mod")

-- Миссия 61: сначала перемещаем Томми к грузовику (как в миссии 28),
-- потом запускаем миссию, садимся в грузовик, ждём RC Baron и летим к лодке.

local MISSION_ID = 61
local TRUCK_MODEL = 189
local PLANE_MODEL = 194
local BOAT_X, BOAT_Y, BOAT_Z = -524.333, -227.579, 5.3
local FLY_ALT = 20.0

local busy = false

function main()
  while true do
    wait(10)
    local player = findplayer()
    if player ~= nil and not busy and Keypress("VK_G") then
      busy = true

      -- 0) поставить Томми у грузовика и очистить area (как в миссии 28)
      local gx, gy, gz = -808.945, -162.921, 10.0458
      setpedcoordes(player, gx, gy, gz)
      cleanarea(gx, gy, gz, 3, 1)
      log_line("DIAG: Томми перемещён к грузовику")

      -- 1) запустить миссию
      start_mission_original(MISSION_ID)
      log_line("DIAG: миссия запущена")
      wait(4000)

      -- 2) сесть в грузовик
      local truck = find_car_by_model(TRUCK_MODEL)
      local tries = 0
      while (truck == nil or not isvehicle(truck)) and tries < 15 do
        wait(1000)
        truck = find_car_by_model(TRUCK_MODEL)
        tries = tries + 1
      end
      if truck ~= nil and isvehicle(truck) then
        putincar(player, truck)
      end
      log_line("DIAG: грузовик найден=" .. tostring(truck ~= nil and isvehicle(truck)))

      -- 3) ждать самолётик RC Baron
      local plane = find_car_by_model(PLANE_MODEL)
      tries = 0
      while (plane == nil or not isvehicle(plane)) and tries < 20 do
        wait(1000)
        plane = find_car_by_model(PLANE_MODEL)
        tries = tries + 1
      end
      log_line("DIAG: самолёт найден=" .. tostring(plane ~= nil and isvehicle(plane)))

      if plane ~= nil and isvehicle(plane) then
        set_car_can_be_damaged(plane, false)
        rc_detonate_on_contact_enable(plane, false)
        set_airship_cheat(true)
        set_car_gas(plane, 1.0)
        setcarspeed(plane, 70)
        set_camera_on_car(plane, 18, 2)

        local targetZ = BOAT_Z + FLY_ALT
        local t = 0
        while t < 200 do
          if not isvehicle(plane) then break end
          set_car_gas(plane, 1.0)
          keep_rc(plane)  -- не давать игре удалить RC-машину через 2 сек
          local hx, hy, hz = getcarcoordes(plane)
          local dx, dy = BOAT_X - hx, BOAT_Y - hy
          local d = math.sqrt(dx * dx + dy * dy)
          if d < 0.001 then d = 0.001 end
          plane_goto_coords(plane, hx + dx / d * 200.0, hy + dy / d * 200.0, targetZ, 60)
          t = t + 1
          wait(30)
        end
        set_airship_cheat(false)
        restore_camera()
      end
      busy = false
      log_line("DIAG: конец")
    end
  end
end
