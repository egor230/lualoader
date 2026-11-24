require("lualoader/mod")                                    -- подключаем библиотеку mod.lua

-- ============================================================================
-- Автопрохождение "Вызов такси" (Taxi Call) — активация кнопкой T.
-- По мотивам скрипта taxi.cs (CLEO, LibertyCity.Ru):
--   звонок -> такси приезжает -> Томми пассажир -> карта ->
--   E = точка перекрестия -> такси едет к ближайшей дороге ->
--   по прибытии Томми выходит.
-- ============================================================================

local TAXI_MODEL   = 150                                    -- модель TAXI
local DRIVER_MODEL = 74                                     -- модель водителя WMOCA
local busy = false                                          -- флаг: сценарий уже идёт


 local function car_to_point(driver, taxi, gx, gy, gz)
 set_car_driving_style_stop_for_cars_ignore_lights(taxi) -- не останавливаться на светофорах
 -- 3) ПОВЕДЕНИЕ КАК В МИССИИ 23 "The Driver" (постоянная скорость 45):
 --    0423 traction 1.5, 00AD cruise 45, 00AE driving style 2
 set_car_immunities(taxi, 1, 1, 1, 1, 1)               -- иммунитеты: пули/огонь/взрывы/...
 set_car_traction(taxi, 1.5)                           -- сцепление 1.5 (0423)
 setcarspeed(taxi, 35.0)                               -- крейсерская скорость 45 (00AD)
 set_driving_style_op(taxi, 2)                         -- стиль вождения 2 (00AE)
 set_car_driving_style_avoid_cars(taxi)   -- ОБГОНЯТЬ другие машины (не таранить!)
 stop_car_radio()                                 -- выключить играющее радио (9 = Radio Off)
 setcarstrong(taxi, 1)                                 -- машина усиленная
 set_car_watertight(taxi, true)                        -- не тонет
 car_upside_safe(taxi, true)                           -- не ломается при перевороте
 set_car_only_damaged_by_player(taxi, true)            -- урон только от игрока
 set_ped_max_drive_speed(driver, 75.0) -- макс скорость водителя
-- car_join_road_goto(taxi, x, y, z, true)   -- встроить такси в систему дорог и ехать к точке
 cardrive(taxi, gx, gy, gz)                            -- ехать к этой точке
end

local function take_passage(player, taxi, marker)
  -- ЦЕЛЬ — МАКСИМАЛЬНО БЛИЗКО К ИГРОКУ: его текущие координаты
  while true do wait(1600)                                          -- ждём до 30 секунд
    if not isvehicle(taxi) then break end                   -- машина исчезла — выходим
    if car_just_sunk(taxi) == 1 then break end              -- машина утонула — выходим
    if is_car_upsidedown(taxi) then break end               -- машина перевернулась — выходим
    if iscarfire(taxi) then break end                       -- машина горит — выходим

    local ppx, ppy, ppz = getpedcoordes(player)               -- стартовая позиция игрока
    cardrive(taxi, ppx, ppy, ppz)                             -- ехать прямо к игроку
    local cx, cy, cz = getcarcoordes(taxi)                  -- координаты ТАКСИ каждый такт
    local x, y, z = getpedcoordes(player)                   -- координаты ИГРОКА каждый такт
    local dist = get_distance_between_coords_2d(cx, cy, x, y) -- дистанция между ними
    if dist < 7.5 then                                      -- подъехали максимально близко
      removemarker(marker) end
    if dist < 5.5 then                                      -- подъехали максимально близко
      set_car_forward_speed(taxi, 0)                        -- ОСТАНАВЛИВАЕМ такси
    end
    if is_car_stopped(taxi)   -- возвращает true/false
    then
      break                                                 -- выходим из цикла
    end                                                     -- конец проверки
  end                                                     -- конец логирования
    -- если стоит и далеко — обновляем цель по СВЕЖИМ координатам игрока
  set_char_obj_enter_car_as_passenger(player, taxi)         -- команда сесть пассажиром
  warp_ped_passenger(player, taxi, 0)                     -- сажаем принудительно

  while true  do   wait(20);-- ждём посадки
  if is_ped_passenger_car(player, taxi)  then
  break
  end                                                       -- конец проверки
  end
end                                                         -- конец функции

-- ЛОКАЛЬНАЯ ФУНКЦИЯ: вызов такси (звонок + создание + подача).
-- Возвращает ВСЁ, что нужно дальше: taxi, driver, marker, gx, gy, gz
local function call_taxi(player)
  local px, py, pz = getpedcoordes(player)                  -- координаты Томми

  -- 1) ЗВОНОК: грузим модель телефона (258) и модели такси/водителя
  loadmodel(258)
  loadmodel(TAXI_MODEL)
  loadmodel(DRIVER_MODEL)
  load_requested_models()
  while true do
    wait(1)
    if availablemodel(258) and availablemodel(TAXI_MODEL) and availablemodel(DRIVER_MODEL) then break end
  end
  hold_cellphone(player, 1)                                 -- телефон к уху
  wait(1300)
  show_text_key("COK1_6", 3000, 1)                          -- фраза cok1_6 (BARON1)
  play_wav_file("Audio/fin_1a.wav")                         -- звук звонка
  wait(2300)
  showtext("~y~Такси подъезжает / Taxi rides to You", 2500, 1)
  hold_cellphone(player, 0)
  releasemodel(258)

  -- 2) местность и создание такси у ближайшей дороги
  local sx, sy, sz = find_road_for_car(px + 110.3, py + 110.3, pz)
  load_scene(sx, sy, sz)
  load_scene(px, py, pz)
  local taxi = Createcar(TAXI_MODEL, sx, sy, sz)
  if taxi == nil or not isvehicle(taxi) then
    return nil, nil, nil, nil, nil, nil
  end
  local heading = angle_entity_to_entity(taxi, player)
  setcarangle(taxi, heading)
  wait(1000)
  local driver = setcardrive(taxi, DRIVER_MODEL, 4)         -- водитель внутри такси
  local marker = create_marker_car(taxi)                    -- маркер над такси

  -- 3) ехать к дороге у Томми
  local gx, gy, gz = find_road_for_car(px, py, pz)
  car_to_point(driver, taxi, gx, gy, gz)
  set_camera_on_car(taxi, 18, 2)
  wait(3400)
  restore_camera()
  stop_car_radio()         -- выключить радио в такси
  return taxi, driver, marker, gx, gy, gz
end

function main()                                             -- главная функция скрипта
  while true do                                             -- бесконечный цикл опроса
    wait(10)                                                -- пауза, чтобы не грузить CPU
    local player = findplayer()                             -- находим Томми
    if player ~= nil and not busy and Keypress("VK_T") then -- если нажали T и мы свободны
      busy = true                                           -- ставим флаг занятости
      -- 1-4) вызов такси (звонок, создание, подача) через локальную функцию
      local taxi, driver, marker, gx, gy, gz = call_taxi(player)
      if taxi == nil then busy = false; return end
      take_passage(player, taxi, marker)                           -- подъезд, СТОП, посадка
      set_player_control(0)                               -- выключаем управление игроком
      -- 6) ПОКАЗЫВАЕМ КАРТУ, ждём E
      map_overlay_on()                                    -- открываем большую карту
      while not Keypress("VK_E") do           -- ждём выбора точки на карте
        wait(100)
      end                                       -- конец ожидания E
      open_west_diaz_gates()   -- западные ворота острова Диаса
      open_east_diaz_gates()   -- восточные ворота острова Диаса
        -- 7) E: записываем координату перекрестия, закрываем карту
      local u, v, wx, wy, ok = map_cross_get()
      map_overlay_off()     -- закрываем карту
      local blip = radar_add_blip_for_coord(wx, wy, 10)
      -- x = tonumber(string.format("%.1f", wx))
      -- y = tonumber(string.format("%.1f", wy))
      --
      -- cord = tostring(x)..", "..tostring(y).."\n"
      -- file = io.open('cordinat.txt', 'a') -- открыть файл для записи
      -- file:write(cord)
      -- file:close()
      --
      -- local blip = radar_add_blip_for_coord(u, v, 10)
      -- x = tonumber(string.format("%.1f", u))
      -- y = tonumber(string.format("%.1f", u))
      --
      -- cord = tostring(x)..", "..tostring(y).."\n"
      -- file = io.open('cordinat.txt', 'a') -- открыть файл для записи
      -- file:write(cord)
      -- file:close()
      radar_change_blip_colour(blip, 4)
      local gx, gy, gz = find_road_for_car(wx, wy, 10)
      car_to_point(driver, taxi, wx, wy, 10)
      wait(4000)                                   -- пауза перед фразой 1
      show_text_key("COK1_6", 3500, 1) -- фраза cok1_6 по GXT-ключу
      wait(4000)                                -- пауза перед фразой 2
      show_text_key("BNK4_3J", 3500, 1) -- фраза bnk4_3j по GXT-ключу (таблица BANKJ4)
      set_player_control(1)                               -- включаем управление
      while true do wait(10)                                    -- ждём до 60 секунд
       if not isvehicle(taxi) then break end             -- машина исчезла — выходим
        -- команда ехать даётся один раз перед циклом (см. ниже)
        if  car_in_point_in_radius(taxi, wx, wy, 10, 18.0)  then -- доехали
          remove_blip(blip)
          set_car_forward_speed(taxi, 0)                        -- ОСТАНАВЛИВАЕМ такси
          showtext("~b~we're here, boss. / Мы на месте, босс.", 3000, 1) -- финальная фраза
          wait(300)
          exitcar(player)
          break                                           -- выходим
        end                                               -- конец проверки
       if not is_ped_in_this_car(player, taxi) then -- игрок покинул такси
         break
       end
      end

      if busy then busy =false  end      -- конец условия активации
  end                                                       -- конец главного цикла
end

end                                                         -- конец main
