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
      load_requested_models()                               -- догрузить запрошенные модели
      wait(800)                                             -- ждём загрузки телефона
      log("STEP0: модель телефона 258 загружена")           -- лог: телефон готов
      hold_cellphone(player, 1)                             -- Томми подносит телефон к уху
      wait(1300)                                            -- пауза перед фразой
      showtext("~b~I'm sick of these pricks. / Меня уже тошнит от этих придурков.", 3000, 1) -- фраза cok1_6
      play_wav_file("Audio/fin_1a.wav")                     -- звук звонка из taxi.cs
      wait(2300)                                            -- ждём звук/фразу
      showtext("~y~Такси подъезжает / Taxi rides to You", 2500, 1) -- текст: такси едет
      hold_cellphone(player, 0)                             -- убираем телефон
      log("STEP1: звонок, фраза cok1_6, текст 'такси подъезжает'") -- лог шага 1

      -- 2) загрузить модели и создать такси у ближайшей дороги
      loadmodel(TAXI_MODEL)                                 -- загрузить модель такси
      loadmodel(DRIVER_MODEL)                               -- загрузить модель водителя
      load_requested_models()                               -- догрузить модели
      wait(1500)                                            -- ждём загрузки

      -- спавн +170 (как в версии, где такси доезжало)
      local sx, sy, sz = find_road_for_car(px + 170.3, py + 170.3, pz) -- ищем ближайшую дорогу
      -- ПРИЧИНА "полёта": местность под такси не загружена. Грузим сцену заранее.
      load_scene(sx, sy, sz)                                -- грузим местность у спавна
      load_scene(px, py, pz)                                -- грузим местность у Томми
      wait(1000)                                            -- ждём загрузки сцены
      log(string.format("STEP2a: сцена загружена. Томми=(%.1f %.1f %.1f) дорога=(%.1f %.1f %.1f)",
        px, py, pz, sx, sy, sz))                            -- лог координат спавна
      local taxi = Createcar(TAXI_MODEL, sx, sy, sz)        -- создаём такси на дороге
      local heading = general_angle_between_points(sx, sy, px, py) -- угол к Томми
      setcarangle(taxi, heading)                            -- разворачиваем нос к Томми
      if taxi == nil or not isvehicle(taxi) then            -- если машина не создалась
        log("ОШИБКА: такси не создалось")                   -- пишем ошибку
        busy = false                                        -- снимаем флаг занятости
        return                                              -- выходим из скрипта
      end                                                   -- конец проверки
      -- водитель сразу создаётся ВНУТРИ такси за рулём (0129 CREATE_CHAR_INSIDE_CAR)
      local driver = setcardrive(taxi, DRIVER_MODEL, 4)     -- создаём водителя в машине
      local marker = create_marker_car(taxi)                -- маркер над такси (0186)
      -- Сразу разворачиваем нос такси к Томми и включаем игнор светофоров
      set_car_driving_style_stop_for_cars_ignore_lights(taxi) -- не останавливаться на светофорах
      log(string.format("STEP2b: маркер=%s нос такси=%.1f в сторону Томми, светофоры игнорируются",
        tostring(marker), heading))                         -- лог маркера и угла

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
      if driver ~= nil then set_ped_max_drive_speed(driver, 45.0) end -- макс скорость водителя
      log("STEP2: такси cruise 45, traction 1.5, style 2")  -- лог настроек

      -- 4) ТАКСИ ЕДЕТ К ИГРОКУ, НО ЦЕЛЬ — БЛИЖАЙШАЯ ДОРОГА, а не координаты Томми
      -- (иначе такси сворачивает в переулок и застревает — 02C1 из taxi.cs)
      local gx, gy, gz = find_road_for_car(px, py, pz)      -- цель — дорога у Томми
      log(string.format("STEP3road: цель на дороге (%.1f %.1f %.1f)", gx, gy, gz)) -- лог цели
      cardrive(taxi, gx, gy, gz)                            -- ехать к этой точке
      local arrived = false                                 -- флаг прибытия
      local t = 0                                           -- счётчик тактов
      local stuck_t = 0                                     -- счётчик стояния
      set_camera_on_car(taxi, 18, 2)                        -- камера позади такси
      while t < 300 do                                      -- ждём до 30 секунд
        if not isvehicle(taxi) then break end               -- машина исчезла — выходим
        local cx, cy, cz = getcarcoordes(taxi)              -- координаты такси
        -- камера У такси, направлена на игрока
        -- ОБЫЧНАЯ камера позади такси (как в игре), не сверху
        local dist = get_distance_between_coords_2d(cx, cy, px, py) -- дистанция до Томми
        if dist < 10.0 then                                 -- доехали
          arrived = true                                    -- ставим флаг прибытия
          log(string.format("STEP3ARRIVE: такси (%.1f %.1f %.1f) dist=%.1f", cx, cy, cz, dist)) -- лог
          break                                             -- выходим из цикла
        end                                                 -- конец проверки
        if t % 5 == 0 then                                  -- раз в 0,5 сек
          log(string.format("TAXIPOS t=%d car=(%.1f %.1f %.1f) speed=%.1f player=(%.1f %.1f) dist=%.1f alive=%s",
            t, cx, cy, cz, get_car_speed(taxi), px, py, dist, tostring(isvehicle(taxi)))) -- лог позиции
        end                                                 -- конец логирования
        -- как в taxi.cs @check: застряло -> ставим обратно на ближайшую дорогу
        if is_car_stopped(taxi) then stuck_t = stuck_t + 1 else stuck_t = 0 end -- считаем стояние
        if stuck_t > 30 and dist > 12.0 then                 -- стоит 3 сек и далеко
          local rx, ry, rz = find_road_for_car(cx, cy, cz)   -- ближайшая дорога к машине
          -- setcarcoordes(taxi, rx, ry, rz)                    -- переставляем машину на дорогу
          cardrive(taxi, gx, gy, gz)                         -- снова ехать к цели
              log(string.format("STUCK: такси переставлено на дорогу (%.1f %.1f %.1f)", rx, ry, rz)) -- лог
          stuck_t = 0                                        -- сбрасываем счётчик
          wait(500)                                          -- пауза после перестановки
        end                                                 -- конец обработки застревания
        t = t + 1                                           -- следующий такт
        wait(100)                                           -- пауза 100 мс
      end                                                   -- конец цикла ожидания

      if not arrived then                                   -- если не доехал
        log("ОШИБКА: такси не доехало до игрока")           -- пишем ошибку
      else                                                  -- если доехал
        log("STEP3b: такси у игрока")                       -- лог прибытия
        -- останавливаем у игрока, чтобы Томми сел
        wait(600)                                           -- ждём остановки

        -- 5) Томми садится ПАССАЖИРОМ (если не сел — принудительно)
        set_char_obj_enter_car_as_passenger(player, taxi)   -- команда сесть пассажиром
        t = 0                                               -- счётчик попыток
        while t < 60 and not is_ped_passenger_car(player, taxi) do -- ждём посадки
          wait(100); t = t + 1                              -- пауза и счётчик
        end                                                 -- конец ожидания
        if not is_ped_passenger_car(player, taxi) then      -- если не сел
          warp_ped_passenger(player, taxi, 0)               -- сажаем принудительно
          wait(300)                                         -- пауза
        end                                                 -- конец проверки
        set_player_control(0)                               -- выключаем управление игроком
        log("STEP4: Томми пассажир, управление выключено")   -- лог шага 4

        -- теперь включаем АГРЕССИВНОЕ вождение в путь
        -- 6) ПОКАЗЫВАЕМ КАРТУ, ждём E
        map_overlay_on()                                    -- открываем большую карту
        log("STEP5: карта открыта — нажмите E")             -- лог: ждём E
        t = 0                                               -- счётчик ожидания
        while t < 600 and not Keypress("VK_E") do           -- ждём нажатия E
          wait(100); t = t + 1                              -- пауза и счётчик
        end                                                 -- конец ожидания

        -- 7) E: записываем координату перекрестия, закрываем карту
        local u, v, wx, wy, ok = map_cross_get()            -- берём точку перекрестия
        map_overlay_off()                                   -- закрываем карту
        log(string.format("STEP6: E — точка карты u=%.3f v=%.3f мир=(%.1f %.1f) ok=%s",
          u, v, wx, wy, tostring(ok)))                      -- лог выбранной точки

        -- 8) ближайшая дорога к точке и поездка туда (команда и скорость каждый такт)
        local tx, ty, tz = find_road_for_car(wx, wy, pz)    -- дорога рядом с точкой
        log(string.format("STEP7: ближайшая дорога (%.1f %.1f %.1f)", tx, ty, tz)) -- лог дороги

        cardrive(taxi, tx, ty, tz)                          -- ехать к точке
          local arrived2 = false                            -- флаг прибытия в точку
        t = 0                                               -- счётчик тактов
        while t < 600 do                                    -- ждём до 60 секунд
          if not isvehicle(taxi) then break end             -- машина исчезла — выходим
          -- команда ехать даётся один раз перед циклом (см. ниже)
          local ccx, ccy, ccz = getcarcoordes(taxi)         -- координаты такси
          set_camera_on_car(taxi, 18, 2)                    -- камера за такси
          local cx, cy = ccx, ccy                           -- короткие имена координат
          if get_distance_between_coords_2d(cx, cy, tx, ty) < 15.0 then -- доехали
            arrived2 = true                                 -- флаг прибытия
            break                                           -- выходим
          end                                               -- конец проверки
          -- фразы ТОЛЬКО во время поездки (как в .cs)
          if t == 30 then                                   -- через 3 секунды
            showtext("~b~I'm sick of these pricks. / Меня уже тошнит от этих придурков.", 3500, 1) -- фраза 1
            log("STEP8a: фраза cok1_6 во время поездки")    -- лог фразы 1
          end                                               -- конец
          if t == 70 then                                   -- через 7 секунд
            showtext("~r~Yeah, you'll have somebody's eye out! / Ага, ты так кому-нибудь глаз выбьешь!", 3500, 1) -- фраза 2
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
      end                                                   -- конец ветки успеха
                                                            -- пустая строка
      log("=== TAXICALL END ===")                           -- лог конца сценария
      busy = false                                          -- снимаем флаг занятости
    end                                                     -- конец условия активации
  end                                                       -- конец главного цикла
end                                                         -- конец main
