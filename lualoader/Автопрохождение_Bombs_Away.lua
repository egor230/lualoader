require("lualoader/mod")

-- Миссия 61: самолёт летит САМ, ищет ВСЕ цели (педы 83/84 + лодки 223/182/202/164),
-- летит к каждой на высоте 45 м, сбрасывает бомбу (ЛКМ + объект 564),
-- убирает погибшую цель из списка, когда все уничтожены — выход.

local MISSION_ID = 61
local PLANE_MODEL = 194
-- РАДАР: все противники, созданные миссией 61 (из исходника):
--  лодки/машины: 223($5888), 182($5887), 202($5889), 164($5890)
--  педы: 9 штук моделей 83/84 ($5891..$5899)
local PED_MODELS = {}  -- педы не нужны для прохождения
local BOAT_MODELS = {223, 182, 202, 164}
local CRUISE_Z = 40.0
local DROP_DIST = 12.0

local busy = false

-- сброс бомбы: эмуляция ЛКМ + объект 564 (проверенный способ)
local function drop_bomb(plane)
  -- СИСТЕМНАЯ ЛКМ (mouse_event, уровень ОС) — реальное нажатие кнопки мыши
  sys_mouse_down(); wait(50); sys_mouse_up()
  local bx, by, bz = getcarcoordes(plane)
  local bomb = create_obj(564, bx, by, bz)
  if bomb == nil then return end
  -- инерция: бомба наследует скорость самолёта, чтобы лететь вперёд при сбросе
  local svx, svy, svz = get_car_velocity(plane)
  if svx ~= nil then set_object_velocity(bomb, svx, svy, 0.0) end
  place_object_relative_to_car(bomb, plane, 0.0, 0.9, -2.70)
  set_object_rotation(bomb, -90.0, 0.0, 0.0)
  set_object_collision(bomb, true)
  set_object_records_collisions(bomb, 1)
  set_object_dynamic(bomb, true)
  local born = get_game_timer()
  local last_z = select(3, getobjcoordes(bomb))
  local still = 0
  local logtick = 0
  -- камеру НЕ переключаем (тормозит миссию)
  while isobject(bomb) do
    wait(50)
    local ox, oy, oz = getobjcoordes(bomb)
    -- запись каждую секунду (50 мс * 20 = 1 сек)
    logtick = logtick + 1
    if logtick % 20 == 0 then
      log_line(string.format("BOMB t=%d bomb=%.1f %.1f %.1f",
        logtick*50/10, ox, oy, oz))
    end
    -- взрыв по таймеру 3 сек — бомба с 36 м долетела до воды (~5 м).
    -- world_find_ground_z возвращает крышу здания (20 м) — НЕ используем.
    local boom = (get_game_timer() - born >= 3000)
    if boom then
      create_explosion(5, ox, oy, oz)  -- 5 = EXPLOSION_BOAT (топит лодки)
      delete_entity(bomb)
      log_line(string.format("BOMB BOOM at %.1f %.1f %.1f", ox, oy, oz))
      return
    end
  end
end

-- ПОСТОЯННЫЙ СПИСОК целей: создаётся один раз, координаты обновляются каждый такт.
-- Цель удаляется ТОЛЬКО когда iscardead == true (не по "не найдена"!).
local targets = {}   -- {model=223, obj=..., x=,y=,z=, dead=false}

local function init_targets()
  targets = {}
  -- лодки/машины
  for _, m in ipairs(BOAT_MODELS) do
    local b = find_car_by_model(m)
    table.insert(targets, {model = m, kind = "car", obj = b, x = 0, y = 0, z = 0, dead = false})
  end
  -- педы: ВСЕ из пула (радар find_all_peds_by_model), а не только первый
  for _, m in ipairs(PED_MODELS) do
    local peds, count = find_all_peds_by_model(m)
    for j = 1, (count or 0) do
      local p = peds[j]
      if p ~= nil then
        table.insert(targets, {model = m, kind = "ped", obj = p, x = 0, y = 0, z = 0, dead = false})
      end
    end
  end
  log_line("RADAR: в список добавлено " .. tostring(#targets) .. " целей")
end

local function refresh_targets()
  local alive = 0
  for _, t in ipairs(targets) do
    if not t.dead then
      if t.kind == "ped" then
        -- пёд
        if t.obj == nil or not isped(t.obj) or ispeddead(t.obj) then
          local p = find_ped_by_model(t.model)
          if p ~= nil and isped(p) and not ispeddead(p) then t.obj = p end
        end
        if t.obj ~= nil and isped(t.obj) then
          if ispeddead(t.obj) then
            t.dead = true
            log_line("RADAR: пёд m=" .. tostring(t.model) .. " погиб - исключён")
          else
            t.x, t.y, t.z = getpedcoordes(t.obj)
            alive = alive + 1
          end
        end
      else
        -- машина/лодка
        if t.obj == nil or not isvehicle(t.obj) or iscardead(t.obj) then
          local b = find_car_by_model(t.model)
          if b ~= nil and isvehicle(b) and not iscardead(b) then t.obj = b end
        end
        if t.obj ~= nil and isvehicle(t.obj) then
          if iscardead(t.obj) then
            t.dead = true   -- УДАЛИТЬ из списка только по смерти
            log_line("RADAR: лодка m=" .. tostring(t.model) .. " уничтожена - исключена")
          else
            t.x, t.y, t.z = getcarcoordes(t.obj)  -- самообновляемые координаты
            alive = alive + 1
          end
        end
      end
    end
  end
  return alive
end

local function get_live_target()
  for _, t in ipairs(targets) do
    if not t.dead then return t end
  end
  return nil
end

function main()
  while true do
    wait(10)
    local player = findplayer()
    if player ~= nil and not busy and Keypress("VK_G") then
      busy = true

      -- посадка в грузовик (как в миссии)
      local gx, gy, gz = -808.945, -162.921, 10.0458
      setpedcoordes(player, gx, gy, gz)
      cleanarea(gx, gy, gz, 3, 1)
      start_mission_original(MISSION_ID)
      wait(4000)
      init_targets()
      local truck = find_car_by_model(189)
      local tries = 0
      while (truck == nil or not isvehicle(truck)) and tries < 15 do
        wait(1000); truck = find_car_by_model(189); tries = tries + 1
      end
      if truck ~= nil and isvehicle(truck) then putincar(player, truck) end

      local plane = find_car_by_model(PLANE_MODEL)
      tries = 0
      while (plane == nil or not isvehicle(plane)) and tries < 20 do
        wait(1000); plane = find_car_by_model(PLANE_MODEL); tries = tries + 1
      end

      if plane ~= nil and isvehicle(plane) then
        set_car_can_be_damaged(plane, false)
        rc_detonate_on_contact_enable(plane, false)
        set_airship_cheat(true)
        set_car_gas(plane, 1.0)
        setcarspeed(plane, 70)
        set_camera_on_car(plane, 18, 2)

        -- цикл: пока есть живые цели
        local guard = 0
        while guard < 2000 do
          if not isvehicle(plane) then break end
          set_camera_on_car(plane, 18, 2)   -- камера каждый такт
          set_car_gas(plane, 1.0)

          local alive = refresh_targets()
          if alive == 0 then
            -- все лодки уничтожены: миссия требует $5872 == 12 для M_PASS
            set_script_int(5872, 12)
            log_line("ALL BOATS DESTROYED - $5872=12 выставлен")
            break
          end

          -- берём первую живую цель, обновляем координаты каждый такт
          local tgt = get_live_target()
          if tgt == nil then
            wait(30)
          else
          local tx, ty, tz = tgt.x, tgt.y, tgt.z
          local hx, hy, hz = getcarcoordes(plane)
          -- МАТЕМАТИКА: бомба падает с 40 м ~2.5 сек = 150 кадров.
          -- get_car_velocity — units/КАДР, поэтому упреждение = v * 150.
          -- Лодки часто на якоре (v≈0) — тогда целимся точно в координаты.
          local aimx, aimy = tx, ty
          if tgt.obj ~= nil and isvehicle(tgt.obj) then
            local bvx, bvy = get_car_velocity(tgt.obj)
            if bvx ~= nil and bvy ~= nil then
              aimx = tx + bvx * 150.0
              aimy = ty + bvy * 150.0
            end
          end
          local dx, dy = aimx - hx, aimy - hy
          local d = math.sqrt(dx*dx + dy*dy)
          if d < 0.001 then d = 0.001 end
          -- РАБОЧИЙ вариант: вперёд на 200 + сброс при d<25 (бомбы скидывались)
          plane_goto_coords(plane, hx + dx/d*200.0, hy + dy/d*200.0, CRUISE_Z, 60)
          if d < 25.0 then
            create_newthread(drop_bomb, plane)
            wait(615)
          end
          end  -- if tx == nil
          if guard % 33 == 0 then
            -- координаты ВСЕХ лодок-целей каждую секунду
            for _, bm in ipairs(BOAT_MODELS) do
              local bb = find_car_by_model(bm)
              if bb ~= nil and isvehicle(bb) then
                local bx2, by2, bz2 = getcarcoordes(bb)
                log_line(string.format("BOAT m=%d pos=%.1f %.1f %.1f hp=%.0f dead=%s",
                  bm, bx2, by2, bz2, getcarhealth(bb) or -1, tostring(iscardead(bb))))
              else
                log_line("BOAT m=" .. tostring(bm) .. "Destroyed/not found")
              end
            end
          end
          if guard % 33 == 0 then  -- ~1 сек при wait(30)
            local px, py, pz = getcarcoordes(plane)
            local spd = get_car_speed(plane) or -1
            log_line(string.format("PLANE t=%d pos=%.1f %.1f %.1f spd=%.1f target=%.1f %.1f",
              guard, px, py, pz, spd, tx or -1, ty or -1))
          end
          guard = guard + 1
          wait(30)
        end
        set_airship_cheat(false)
        restore_camera()
      end
      busy = false
    end
  end
end
