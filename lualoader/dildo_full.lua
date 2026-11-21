require("lualoader/mod")

-- ============================================================================
-- dildo_full.lua — миссия 28 «Dildo Dodo» (PORNO2), ТОЧНАЯ логика из main.scm
--
-- Механика оригинала ($4414 = 6 доставок):
--   для каждой из 6 точек:
--     1) проехать СИНЮЮ (x,y,z) -> blip становится КРАСНЫМ на ДРУГИХ координатах;
--     2) проехать КРАСНУЮ (x',y',z') -> $4414 -= 1, blip снова синий;
--   6 пар -> $4414 == 0 -> M_PASS.
--
-- Все координаты ВЗЯТЫ из исходника:
--   Source missions/Оригинал игры (97 миссий)/28 - Dildo Dodo.txt
--   синие: $4436..$4453, красные: подмены в @La3c06..@La4016.
-- КАМЕРУ НЕ ТРОГАЕМ. Скорость снижена на 30%.
-- ============================================================================

local MISSION_ID    = 28
local MODEL_SKIMMER = 190
local DRIVE_MS      = 4000
local LOAD_MS       = 3000

-- пары: { синяя, красная }
local ROUTE = {
  -- Оптимальный маршрут (перебор 6!): длина 2725 вместо 5528 у исходного порядка.
  { {-464.2, 1459.3, 40},   {-861.4, 1084.8, 54.5} },-- 3 (ближе всего к старту)
  { {-872.8, 805.8, 35},    {-1070.8, 88, 35} },     -- 4
  { {-1130.2, -182.7, 36},  {-1433.8, -807.1, 45.5} },--5
  { {-703, -1496, 45},      {-872, -574, 55} },      -- 1
  { {-867.4, -343.2, 40},   {-771.9, 304.6, 50.1} }, -- 6
  { {-601, 660, 44},        {-354.2, 1088, 47} },    -- 2
}

local function angle_of(dx, dy)
  local a = math.atan(dx / dy)
  if dy < 0 then a = a + math.pi end
  return math.deg(a) % 360
end

-- Точный пролёт через чекпоинт: телепорт в 8 м до него на его высоте,
-- скорость направлена прямо в центр -> самолёт входит в корону (радиус 10).
local function hit_point(plane, pt, label)
  local cx, cy, cz = getcarcoordes(plane)     -- текущие координаты самолёта
  local dx, dy = pt[1] - cx, pt[2] - cy
  local d = math.sqrt(dx * dx + dy * dy)
  if d < 0.001 then d = 0.001 end
  local ux, uy = dx / d, dy / d               -- единичный вектор на точку

  setcarangle(plane, angle_of(dx, dy))        -- носом на точку
  set_car_gas(plane, 1.0)                     -- газ держим всегда
  setcarcoordes(plane, pt[1] - ux * 8.0, pt[2] - uy * 8.0, pt[3]) -- телепорт в 8 м до точки -- в 8 м до (внутри радиуса 10)

  local spd = 0.855                           -- скорость полёта: units/frame (~51 м/с), минус 10 процентов
  set_car_velocity(plane, ux * spd, uy * spd, 0.0) -- скорость прямо в центр точки
  setcarangle(plane, angle_of(dx, dy))

  local waited = 0                            -- счётчик ожидания
  while waited < 40 do
    if not isvehicle(plane) then break end
    set_car_gas(plane, 1.0)
    local x, y, z = getcarcoordes(plane)
    local dxy = math.sqrt((x - pt[1])^2 + (y - pt[2])^2) -- расстояние по X/Y
    local dz = math.abs(z - pt[3])             -- отличие по высоте
    if dxy < 10.0 and dz < 10.0 then
      break
    end
    waited = waited + 1                        -- следующий такт
    wait(50)                                   -- klein пауза 50 мс
  end
  wait(600) -- пауза x2: дать игре засчитать прохождение чекпоинта
end

function main()
  while true do  wait()
    local player = findplayer()                -- найти Томми
    if player ~= nil and Keypress("VK_G") then

      local n = open_studio_gates()            -- открыть ворота киностудии
      wait(2400)

      start_mission_original(MISSION_ID)       -- запустить оригинальную миссию
      wait(LOAD_MS)

      local plane = find_car_by_model(MODEL_SKIMMER) -- найти гидроплан миссии
      local tries = 0
      while (plane == nil or not isvehicle(plane)) and tries < 12 do
        wait(2000)
        plane = find_car_by_model(MODEL_SKIMMER)
        tries = tries + 1
      end
      if plane ~= nil and isvehicle(plane) then
        putincar(player, plane)                -- посадить Томми в самолёт
         wait(2600)
        -- set_airship_cheat(true)                -- режим полёта AIRSHIP
        -- set_car_immunities(plane, 1, 1, 1, 1, 1) -- самолёт не разбивается
        -- set_ped_immunities(player, 1, 1, 1, 1, 1) -- игрок не умирает
        setcarspeed(plane, 47)                 -- стартовая скорость минус 10 процентов
        --set_car_gas(plane, 1.0)
        -- КАМЕРУ НЕ ТРОГАЕМ

        -- разгон вперёд ~3.2 сек с ПЛАВНЫМ набором высоты (естественный взлёт)
        local _, _, startZ = getcarcoordes(plane) -- стартовая высота
        local dt = 0                           -- счётчик разгона
        while dt < 160 do
          if not isvehicle(plane) then break end
          set_car_gas(plane, 1.0)
          local x, y, z = getcarcoordes(plane)
          local ang = math.rad(getcarangle(plane))
          -- цель по Z растёт каждый такт: самолёт плавно набирает ~35 м вверх
          local targetZ = startZ + dt * 0.22
          plane_goto_coords(plane, x + math.sin(ang) * 250, y + math.cos(ang) * 250, targetZ, 49) -- держим курс и набираем высоту
          dt = dt + 1
          wait(20)
        end

        -- 6 пар: синяя -> красная
        for i, pair in ipairs(ROUTE) do
          if not isvehicle(plane) then break end

          -- Метки не создаём: миссия сама ставит blip'ы на эти координаты
          -- и сама меняет им цвет. Просто летим на точки.
          hit_point(plane, pair[1], "точка " .. i .. " СИНЯЯ") -- проехать синюю
          hit_point(plane, pair[2], "точка " .. i .. " КРАСНАЯ") -- проехать красную

        end

        -- Вернуть управление игроку (01B4 в игровом потоке)
        set_player_control(1)                    -- управление снова у игрока (01B4)
        set_car_immunities(plane, 0, 0, 0, 0, 0)
        set_ped_immunities(player, 0, 0, 0, 0, 0)
        -- КАМЕРУ НЕ ТРОГАЕМ
      end
    end
  end
end
