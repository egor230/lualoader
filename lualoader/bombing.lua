require("lualoader/mod")

-- ============================================================================
-- БОМБАРДИРОВЩИК — самолёт SKIMMER летает вокруг координат Томми.
-- Центр = живые координаты игрока, цель +20 по Z, старт +30 по Z.
-- Подробности: бомба каждые 90°, отдельный поток бомбы,
-- телеметрия в lualoader/log.txt (высота/скорость/координаты).
-- ============================================================================

local ALTITUDE   = 20.0  -- цель: координаты Томми + 50 по Z (было 40, плюс 10)
local SPAWN_ALT  = ALTITUDE + 30.0  -- старт на 30 м выше цели (было +10)
local RADIUS     = 40.0  -- было 90, минус 1
local PLANE_SPEED = 70  -- было 70, плюс 30
local LAPS       = 7
local BOMB_MODEL = 564
local BOMB_FUSE  = 3600

local function bomb_watch(plane, label)
  local bx, by, bz = getcarcoordes(plane)
  local bomb = create_obj(BOMB_MODEL, bx, by, bz)
  if bomb == nil then
    log_line("BOMB " .. tostring(label) .. ": НЕ создалась")
    return
  end
  place_object_relative_to_car(bomb, plane, 0.0, 0.9, -2.70)
  set_object_rotation(bomb, -90.0, 0.0, 0.0)
  set_object_collision(bomb, true)
  set_object_records_collisions(bomb, true)
  set_object_dynamic(bomb, true)
  local born = get_game_timer()
  local last_z = select(3, getobjcoordes(bomb))
  local still = 0
  log_line("BOMB " .. tostring(label) .. ": сброшена z=" .. tostring(last_z))
  while isobject(bomb) do
    wait(50)
    local ox, oy, oz = getobjcoordes(bomb)
    local gz = world_find_ground_z_for_coord(ox, oy)
    local now = get_game_timer()
    local boom = (now - born >= BOMB_FUSE) or is_object_in_water(bomb) or (oz <= gz + 0.8)
    if now - born > 400 then
      if math.abs(oz - last_z) < 0.01 then still = still + 1 else still = 0 end
      last_z = oz
      if still > 6 then boom = true end
    else
      last_z = oz
    end
    if boom then
      create_explosion(6, ox, oy, oz)
      delete_entity(bomb)
      log_line("BOMB " .. tostring(label) .. ": BOOM")
      return
    end
  end
  log_line("BOMB " .. tostring(label) .. ": исчезла")
end

local function fly_around_player(player, laps)
  local px, py, pz = getpedcoordes(player)

  -- наш самолёт создаётся БЕЗ игрока за рулём — включаем флаг полёта AIRSHIP,
  -- он даёт SKIMMER тягу вверх, как у обычного летающего транспорта.
  set_airship_cheat(true)
  log_line("FLY: AIRSHIP-флаг полёта включён")

  -- как в CLEO: 04C4 from_actor offset 0, 4.2, 0 -> создать SKIMMER
  local plane = Createcar("MODEL_SKIMMER", px, py, pz + SPAWN_ALT)
  if plane ~= nil and isvehicle(plane) then
    setcarcoordes(plane, px, py, pz + SPAWN_ALT)  -- гарантия: те же X,Y, что у Томми, +30 Z
    log_line(string.format("FLY: спавн самолёта x=%.1f y=%.1f z=%.1f (том x=%.1f y=%.1f z=%.1f)",
      px, py, pz + SPAWN_ALT, px, py, pz))
    m = create_marker_car(plane)
  end
  if plane == nil or not isvehicle(plane) then
    log_line("FLY: SKIMMER не создан")
    return
  end
  -- иммунитеты (02AB/02AC из CLEO) — чтобы самолёт не разбивался
  -- set_ped_immunities(player, 1, 1, 1, 1, 1)
  -- set_car_immunities(plane, 1, 1, 1, 1, 1)

  -- СРАЗУ ускорение, как вы просили
  setcarspeed(plane, 90)
  carmovespeed(plane, 0.0, 2.5, 1.5)
  set_car_gas(plane, 1.0)   -- педаль газа: тяга полёта с самого старта
  -- камера как при обычной езде на автомобиле: 0158 camera_on_vehicle 15 2
  --set_camera_on_car(plane, 15, 2)
  log_line("FLY: SKIMMER создан, ускорение задано, камера прикреплена")

  local lap = 0
  local cur_speed = PLANE_SPEED
  local alt_check = get_game_timer()
  local alt_prev = select(3, getcarcoordes(plane))
  local tick = 0
  local last_sector = nil
  local prev_z = select(3, getcarcoordes(plane))

  -- цель: ДАЛЕКО ВПЕРЁДИ (как 0,330,100), но в сторону точки на окружности
  -- вокруг ИГРОКА — так самолёт летит и заворачивает к вам, а не улетает.
  -- ЦЕНТР — ЖИВЫЕ координаты Томми каждый такт (как попросили)
  local wp = 1
  local function flight_target()
    local cx, cy, cz = getpedcoordes(player)
    local x, y, z = getcarcoordes(plane)
    local a = math.rad((wp - 1) * 90)
    local gx = cx + RADIUS * math.cos(a)
    local gy = cy + RADIUS * math.sin(a)
    local dx, dy = gx - x, gy - y
    local d = math.sqrt(dx * dx + dy * dy)
    if d < 0.001 then d = 0.001 end
    return x + dx / d * 330.0, y + dy / d * 330.0, cz + ALTITUDE, d
  end
  local tx, ty, tz = flight_target()
  plane_goto_coords(plane, tx, ty, tz, cur_speed)

  printmessage("~g~bombing: полёт вокруг Томми (+20 Z), бомба каждые 90° (VK_N — стоп)", 3500, 1)

  while lap < laps do
    if not isvehicle(plane) then
      log_line("FLY: самолёт исчез из пула")
      break
    end
    if Keypress("VK_N") then break end

    -- Томми остаётся на земле; самолёт читает ЕГО координаты как центр
    local x, y, z = getcarcoordes(plane)

    -- === удержание высоты: раз в секунду смотрим, куда она пошла ===
    local now_ctl = get_game_timer()
    if now_ctl - alt_check >= 1000 then
      if z < alt_prev - 0.5 then
        cur_speed = math.min(cur_speed + 1, 100)   -- падает -> прибавить 1
        log_line("FLY: высота падает, скорость+1 = " .. tostring(cur_speed))
      elseif z > alt_prev + 0.5 then
        cur_speed = math.max(cur_speed - 1, 30)    -- растёт больше, чем было -> убавить 1
        log_line("FLY: высота растёт, скорость-1 = " .. tostring(cur_speed))
      end
      alt_check = now_ctl
      alt_prev = z
    end

    -- каждые 20 тактов — телеметрия
    tick = tick + 1
    if tick % 20 == 0 then
      local speed = get_car_speed(plane) or -1
      local cz = select(3, getpedcoordes(player))
      local cx, cy = getpedcoordes(player)
      log_line(string.format("FLY t=%d lap=%d x=%.1f y=%.1f z=%.1f | tom x=%.1f y=%.1f z=%.1f | cmd=%d real=%.2f dz=%.2f",
        tick, lap + 1, x, y, z, cx, cy, cz, cur_speed, speed, z - prev_z))
    end
    prev_z = z

    -- цель каждый кадр: вперёд на 330 в сторону вершины вокруг ИГРОКА
    local d
    tx, ty, tz, d = flight_target()
    set_car_gas(plane, 1.0)   -- держим тягу каждый такт
    plane_goto_coords(plane, tx, ty, tz, cur_speed)

    -- бомба, когда самолёт ПЕРЕСЁК точку по X/Y (высота не важна)
    if d < 20.0 then
      create_newthread(bomb_watch, plane, tostring((wp - 1) * 90))
      log_line("FLY: точка " .. tostring((wp - 1) * 90) .. "° пересечена (X/Y) — бомба")
      wp = wp + 1
      if wp > 4 then
        wp = 1
        lap = lap + 1
        log_line("FLY: круг " .. lap .. "/" .. laps)
      end
    end

    wait(10)
  end

  set_ped_immunities(player, 0, 0, 0, 0, 0)
  set_airship_cheat(false)
  restore_camera()
  if isvehicle(plane) then remove_car(plane) end
  log_line("FLY: конец")
end

function main()
  while true do
    wait(10)
    local player = findplayer()
    if player ~= nil then
      if Keypress("VK_G") then
        Giveweaponped(player, 600, "uzi")
        fly_around_player(player, LAPS)
      end
    end
  end
end
