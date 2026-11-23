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

local function log(s) log_line(s) end                       -- короткий алиас для лога

--bled.loc: ждём, пока машина окажется рядом с точкой
local function wait_car_near(car, x, y, radius, max_t)      -- функция ожидания машины у точки
  local t = 0                                               -- счётчик тактов
  while t < (max_t or 200) do                               -- пока не истёк лимит
    if not isvehicle(car) then return false end             -- машина исчезла — выходим
    local cx, cy = getcarcoordes(car)                       -- берём координаты машины
    if get_distance_between_coords_2d(cx, cy, x, y) < radius then return true end -- близко — готово
    t = t + 1                                               -- следующий такт
    wait(100)                                               -- пауза 100 мс
  end                                                       -- конец цикла
  return false                                              -- не доехали
end                                                         -- конец функции
 local function car_to_point(driver, taxi, gx, gy, gz)
 set_car_driving_style_stop_for_cars_ignore_lights(taxi) -- не останавливаться на светофорах

 -- 3) ПОВЕДЕНИЕ КАК В МИССИИ 23 "The Driver" (постоянная скорость 45):
 --    0423 traction 1.5, 00AD cruise 45, 00AE driving style 2
 set_car_immunities(taxi, 1, 1, 1, 1, 1)               -- иммунитеты: пули/огонь/взрывы/...
 set_car_traction(taxi, 1.5)                           -- сцепление 1.5 (0423)
 setcarspeed(taxi, 45.0)                               -- крейсерская скорость 45 (00AD)
 set_driving_style_op(taxi, 2)                         -- стиль вождения 2 (00AE)
 setcarstrong(taxi, 1)                                 -- машина усиленная
 set_car_watertight(taxi, true)                        -- не тонет
 car_upside_safe(taxi, true)                           -- не ломается при перевороте
 set_car_only_damaged_by_player(taxi, true)            -- урон только от игрока
 set_ped_max_drive_speed(driver, 65.0) -- макс скорость водителя

 cardrive(taxi, gx, gy, gz)                            -- ехать к этой точке
end

local function take_passage(player, taxi, marker)
  local t = 0                                               -- счётчик тактов (ЛОКАЛЬНЫЙ)
  local stuck_t = 0                                         -- счётчик стояния (ЛОКАЛЬНЫЙ)
  -- ЦЕЛЬ — МАКСИМАЛЬНО БЛИЗКО К ИГРОКУ: его текущие координаты
  local ppx, ppy, ppz = getpedcoordes(player)               -- стартовая позиция игрока
  cardrive(taxi, ppx, ppy, ppz)                             -- ехать прямо к игроку
  while t < 300 do                                          -- ждём до 30 секунд
    if not isvehicle(taxi) then break end                   -- машина исчезла — выходим
    local cx, cy, cz = getcarcoordes(taxi)                  -- координаты ТАКСИ каждый такт
    local x, y, z = getpedcoordes(player)                   -- координаты ИГРОКА каждый такт
    local dist = get_distance_between_coords_2d(cx, cy, x, y) -- дистанция между ними
    if dist < 4.5 then                                      -- подъехали максимально близко
      set_car_forward_speed(taxi, 0)                        -- ОСТАНАВЛИВАЕМ такси
      log(string.format("STEP3ARRIVE: такси (%.1f %.1f %.1f) dist=%.1f — СТОП", cx, cy, cz, dist))
    end
    if is_car_stopped(taxi)   -- возвращает true/false
    then
      removemarker(marker)
      break                                                 -- выходим из цикла
    end                                                     -- конец проверки
    -- if t % 5 == 0 then                                      -- раз в 0,5 сек
    --   log(string.format("TAXIPOS t=%d car=(%.1f %.1f %.1f) speed=%.1f player=(%.1f %.1f) dist=%.1f alive=%s",
    --     t, cx, cy, cz, get_car_speed(taxi), x, y, dist, tostring(isvehicle(taxi)))) -- лог позиции
    end                                                     -- конец логирования
    -- если стоит и далеко — обновляем цель по СВЕЖИМ координатам игрока
  set_char_obj_enter_car_as_passenger(player, taxi)         -- команда сесть пассажиром
  warp_ped_passenger(player, taxi, 0)                     -- сажаем принудительно

  while true  do -- ждём посадки
  wait(20);
  if is_ped_passenger_car(player, taxi)  then
  break
  end                                                       -- конец проверки
  log("STEP4b: Томми сел в такси")                          -- лог посадки
end
end                                                         -- конец функции

function main()                                             -- главная функция скрипта
  while true do                                             -- бесконечный цикл опроса
    wait(10)                                                -- пауза, чтобы не грузить CPU
    local player = findplayer()                             -- находим Томми
    if player ~= nil and not busy and Keypress("VK_T") then -- если нажали T и мы свободны
      busy = true                                           -- ставим флаг занятости
      log("=== TAXICALL START === activation=T")            -- пишем старт в лог

      -- 1) ЗВОНОК: сначала грузим модель телефона (id 258 из data/default.ide)
      local px, py, pz = getpedcoordes(player)              -- координаты Томми
      loadmodel(258)                                        -- загрузить модель CELLPHONE

      loadmodel(TAXI_MODEL)                                 -- загрузить модель такси
      loadmodel(DRIVER_MODEL)                               -- загрузить модель водителя
      load_requested_models()                               -- догрузить модели
      while true do wait(1)
        if availablemodel(258) and availablemodel(TAXI_MODEL) and availablemodel(DRIVER_MODEL)
        then break end  end       -- догрузить запрошенные модели
      hold_cellphone(player, 1)                             -- Томми подносит телефон к уху
      wait(1300)                                            -- пауза перед фразой
      show_text_key("COK1_6", 3000, 1) -- фраза cok1_6 по GXT-ключу (таблица BARON1)
      play_wav_file("Audio/fin_1a.wav")                     -- звук звонка из taxi.cs
      wait(2300)                                            -- ждём звук/фразу
      showtext("~y~Такси подъезжает / Taxi rides to You", 2500, 1) -- текст: такси едет
      hold_cellphone(player, 0)                             -- убираем телефон
      releasemodel(258)
      log("STEP1: звонок, фраза cok1_6, текст 'такси подъезжает'") -- лог шага 1

      -- 2) загрузить модели и создать такси у ближайшей дороги
      -- спавн +170 (как в версии, где такси доезжало)
      local sx, sy, sz = find_road_for_car(px + 110.3, py + 110.3, pz) -- ищем ближайшую дорогу
      -- ПРИЧИНА "полёта": местность под такси не загружена. Грузим сцену заранее.
      load_scene(sx, sy, sz)                                -- грузим местность у спавна
      load_scene(px, py, pz)                                -- грузим местность у Томми
      local taxi = Createcar(TAXI_MODEL, sx, sy, sz)        -- создаём такси на дороге
      if taxi == nil or not isvehicle(taxi) then            -- если машина не создалась
        log("ОШИБКА: такси не создалось")                   -- пишем ошибку
        busy = false                                        -- снимаем флаг занятости
        return                                              -- выходим из скрипта
      end                                                   -- конец проверки
      local heading = angle_entity_to_entity(taxi, player) -- угол к Томми (только когда taxi есть)
      setcarangle(taxi, heading)
      wait(1000)                                            -- ждём загрузки сцены
      log(string.format("STEP2a: сцена загружена. Томми=(%.1f %.1f %.1f) дорога=(%.1f %.1f %.1f)",
        px, py, pz, sx, sy, sz))                            -- лог координат спавна
                   -- разворачиваем нос к Томми
      -- водитель сразу создаётся ВНУТРИ такси за рулём (0129 CREATE_CHAR_INSIDE_CAR)
      local driver = setcardrive(taxi, DRIVER_MODEL, 4)     -- создаём водителя в машине
      local marker = create_marker_car(taxi)                -- маркер над такси (0186)

      -- 4) ТАКСИ ЕДЕТ К ИГРОКУ, НО ЦЕЛЬ — БЛИЖАЙШАЯ ДОРОГА, а не координаты Томми
      -- (иначе такси сворачивает в переулок и застревает — 02C1 из taxi.cs)
      local gx, gy, gz = find_road_for_car(px, py, pz)      -- цель — дорога у Томми
      log(string.format("STEP3road: цель на дороге (%.1f %.1f %.1f)", gx, gy, gz)) -- лог цели
      car_to_point(driver, taxi, gx, gy, gz)-- ехать к точке
      local arrived = false                                 -- флаг прибытия
      local t = 0                                           -- счётчик тактов
      local stuck_t = 0                                     -- счётчик стояния
      set_camera_on_car(taxi, 18, 2)                        -- камера позади такси
      wait(3400)
      restore_camera()
      take_passage(player, taxi, marker)                           -- подъезд, СТОП, посадка
      set_player_control(0)                               -- выключаем управление игроком
      log("STEP4: Томми пассажир, управление выключено")   -- лог шага 4

      -- теперь включаем АГРЕССИВНОЕ вождение в путь
      -- 6) ПОКАЗЫВАЕМ КАРТУ, ждём E
      map_overlay_on()                                    -- открываем большую карту
      log("STEP5: карта открыта — нажмите E")             -- лог: ждём E
      while not Keypress("VK_E") do           -- ждём нажатия E
        wait(100);                            -- пауза и счётчик
      end                                                 -- конец ожидания

      -- 7) E: записываем координату перекрестия, закрываем карту
      local u, v, wx, wy, ok = map_cross_get()            -- берём точку перекрестия
      map_overlay_off()                                   -- закрываем карту
      local blip = radar_add_blip_for_coord(wx, wy, 10)
      radar_change_blip_colour(blip, 4)
      log(string.format("STEP6: E — точка карты u=%.3f v=%.3f мир=(%.1f %.1f) ok=%s",
        u, v, wx, wy, tostring(ok)))                      -- лог выбранной точки
      car_to_point(driver, taxi, wx, wy, 10)
      -- 8) ближайшая дорога к точке и поездка туда (команда и скорость каждый такт)
      local tx, ty, tz = find_road_for_car(u, v, pz)    -- дорога рядом с точкой
      log(string.format("STEP7: ближайшая дорога (%.1f %.1f %.1f)", tx, ty, tz)) -- лог дороги

      local arrived2 = false                            -- флаг прибытия в точку
      t = 0                                               -- счётчик тактов
      while true do wait(10)                                    -- ждём до 60 секунд
        if not isvehicle(taxi) then break end             -- машина исчезла — выходим
        -- команда ехать даётся один раз перед циклом (см. ниже)
        local ccx, ccy, ccz = getcarcoordes(taxi)         -- координаты такси
        --set_camera_on_car(taxi, 18, 2)                    -- камера за такси
        local cx, cy = ccx, ccy                           -- короткие имена координат
        if get_distance_between_coords_2d(cx, cy, tx, ty) < 15.0 then -- доехали
          --stop
          arrived2 = true                                 -- флаг прибытия
          break                                           -- выходим
        end                                               -- конец проверки
        -- фразы ТОЛЬКО во время поездки (как в .cs)
        if t == 30 then                                   -- через 3 секунды
          show_text_key("COK1_6", 3500, 1) -- фраза cok1_6 по GXT-ключу
          log("STEP8a: фраза cok1_6 во время поездки")    -- лог фразы 1
        end                                               -- конец
        if t == 70 then                                   -- через 7 секунд
          show_text_key("BNK4_3J", 3500, 1) -- фраза bnk4_3j по GXT-ключу (таблица BANKJ4)
          log("STEP8b: фраза bnk4_3j во время поездки")   -- лог фразы 2
        end                                               -- конец
        t = t + 1                                         -- следующий такт
        wait(100)                                         -- пауза 100 мс
      end                                                 -- конец цикла поездки
      if arrived2 then log("STEP9: такси прибыло")        -- лог успеха
      else log("ОШИБКА: такси не доехало до точки") end   -- лог ошибки

      t = 0                                               -- счётчик ожидания остановки
      while t < 200 and not is_car_stopped(taxi) do       -- ждём, пока машина встанет
        wait(100); t = t + 1                              -- пауза и счётчик
      end                                                 -- конец ожидания

      -- 10) Томми выходит
      exitcar(player)                                     -- Томми выходит из машины
      set_player_control(1)                               -- включаем управление
      showtext("~b~we're here, boss. / Мы на месте, босс.", 3000, 1) -- финальная фраза
      log("STEP10: Томми вышел, управление включено")     -- лог шага 10

    log("=== TAXICALL END ===")                           -- лог конца сценария
    busy = false                                          -- снимаем флаг занятости
  end                                                     -- конец условия активации
end                                                       -- конец главного цикла
end                                                         -- конец main
