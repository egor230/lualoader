require("lualoader/mod")

-- ============================================================================
-- Автопрохождение миссии 19 «Demolition Man» — активация словом "destr"
-- ПОСЛЕДОВАТЕЛЬНОСТЬ ИЗ ИСХОДНИКА:
--   сцена ждёт IS_BUTTON_PRESSED 0 16 -> SWITCH_WIDESCREEN 0, SET_PLAYER_CONTROL 1,
--   $3391 = 1  (= управление вертолётом пришло к игроку).
--   4 бомбы -> 4 РАЗНЫЕ точки закладки $3367-$3378, высота +2.0 по Z.
--   счётчики: бомба1=$3392, бомба2=$3393, бомба3=$3394, бомба4=$3395.
-- ============================================================================

local MISSION_ID = 19
local RC_MODEL = 231
local BOMB_START = {
  {229.852, -347.11, 10.0998},
  {229.852, -344.028, 10.1517},
  {229.852, -341.11, 10.5602},
  {229.852, -338.11, 10.4648},
}
-- 4 РАЗНЫЕ точки закладки из исходника $3367-$3378
local PLANT_POINT = {
  {97.574,   -381.658,  7.6407},
  {156.549,  -348.028, 16.8517},
  {88.1474,  -361.868, 26.076},
  {140.008,  -383.125, 35.3172},
}
local PLANT_Z_ADD = 2.0
local BOMB_STATE = {3392, 3393, 3394, 3395}

-- подробная строка состояния всех счётчиков
local function dump_states(label)
  -- log_line(string.format("STATE [%s]: $3391=%s $3392=%s $3393=%s $3394=%s $3395=%s remote=%s",
    -- label,
    -- tostring(get_script_int(3391) or -1),
    -- tostring(get_script_int(3392) or -1),
    -- tostring(get_script_int(3393) or -1),
    -- tostring(get_script_int(3394) or -1),
    -- tostring(get_script_int(3395) or -1),
    -- tostring(player_is_in_remote_mode())))
end

function main()
  while true do
    wait(10)
    local player = findplayer()
    if player ~= nil and not busy and cheat("destr") then
      busy = true
      -- log_line("======== DEMOLITION START ========")
      dump_states("старт")

      -- 1) Томми к фургону
      local fx, fy, fz = 226.915, -352.211, 9.9086
      setpedcoordes(player, fx, fy, fz)
      cleanarea(fx, fy, fz, 3, 1)
      -- log_line("STEP1: Томми у фургона")

      -- 2) миссия (проверка/восстановление здания — внутри C++ start_mission_original)
      start_mission_original(MISSION_ID)
      -- log_line("STEP2: миссия 19 запущена")
      dump_states("после старта миссии")

      -- 3) сесть в фургон
      local truck = find_car_by_model(189)
      local tries = 0
      while (truck == nil or not isvehicle(truck)) and tries < 15 do
        wait(1000); truck = find_car_by_model(189); tries = tries + 1
      end
      if truck ~= nil and isvehicle(truck) then putincar(player, truck) end
      -- log_line("STEP3: Томми в фургоне")
      dump_states("после посадки в фургон")

      -- 4) ждать режим пульта
      tries = 0
      while tries < 60 do
        local remote = player_is_in_remote_mode()
        -- if tries % 5 == 0 then log_line("STEP4: пульт remote=" .. tostring(remote) .. " t=" .. tostring(tries)) end
        if remote ~= nil and remote ~= 0 then
          -- log_line("STEP4: РЕЖИМ ПУЛЬТА АКТИВЕН")
          break
        end
        wait(1000); tries = tries + 1
      end
      dump_states("после режима пульта")

      -- === 4b) ЖДАТЬ, КОГДА УПРАВЛЕНИЕ ВЕРТОЛЁТОМ ПРИДЁТ К ИГРОКУ ===
      -- В исходнике: PRINT_NOW 'TEX3_27' -> цикл пока IS_BUTTON_PRESSED 0 16,
      -- затем SWITCH_WIDESCREEN 0, SET_PLAYER_CONTROL 1 и $3391 = 1.
      -- Значит надёжный сигнал = $3391 == 1. Пробуем также нажать кнопку (ЛКМ),
      -- чтобы сцена сдвинулась с места, если игрок её не пропустил.
      local control_ready = false
      tries = 0
      while tries < 200 do
        local s3391 = get_script_int(3391) or 0
        if tries % 10 == 0 then
          -- log_line("STEP4b: t=" .. tostring(tries) .. " $3391=" .. tostring(s3391) ..
                   -- " cut=" .. tostring(is_cutscene_running()))
        end
        if s3391 >= 1 then
          control_ready = true
          -- log_line("STEP4b: >>> УПРАВЛЕНИЕ ВЕРТОЛЁТОМ ПРИШЛО К ИГРОКУ ($3391=1) <<<")
          break
        end
        -- раз в 2 секунды жмём ЛКМ/Enter, чтобы пропустить сцену
        if tries % 10 == 0 then
          sys_mouse_down(); wait(60); sys_mouse_up()
          sys_key_down(13); wait(60); sys_key_up(13)   -- 13 = VK_RETURN (пропуск сцены)
          -- log_line("STEP4b: попытка пропустить сцену (ЛКМ + Enter)")
        end
        wait(200); tries = tries + 1
      end
      dump_states("после ожидания управления")

      if not control_ready then
        -- log_line("!!! УПРАВЛЕНИЕ ВЕРТОЛЁТОМ НЕ ПРИШЛО — НИЧЕГО НЕ ТРОГАЕМ !!!")
        busy = false
        return
      end

      -- 5) вертолёт 231, который дала миссия
      local heli = find_car_by_model(RC_MODEL)
      tries = 0
      while (heli == nil or not isvehicle(heli)) and tries < 20 do
        wait(1000); heli = find_car_by_model(RC_MODEL); tries = tries + 1
      end
      -- log_line("STEP5: вертолёт=" .. tostring(heli ~= nil and isvehicle(heli)))
      dump_states("после поиска вертолёта")

      if heli ~= nil and isvehicle(heli) then
        -- ВСЕ ИММУНИТЕТЫ
        set_car_immunities(heli, 1, 1, 1, 1, 1)
        set_car_tires_invulnerable(heli, true)
        set_car_bproof(heli, true)
        set_car_fproof(heli, true)
        set_car_rproof(heli, true)
        set_car_health(heli, 5000)
        -- log_line("IMMUNITY: все иммунитеты вертолёту включены")

        -- ТАЙМЕР +5 минут
        local old_timer = get_script_int(3402) or 0
        set_script_int(3402, old_timer + 300000)
        -- log_line("TIMER: $3402 " .. tostring(old_timer) .. " -> " .. tostring(old_timer + 300000))

        set_airship_cheat(true)

        for i = 1, 4 do
          local bs = BOMB_START[i]
          local pp = PLANT_POINT[i]
          local sv = BOMB_STATE[i]
          -- log_line(string.format("======== BOMB %d / счётчик $%d ========", i, sv))
          dump_states("начало бомбы " .. tostring(i))

          -- 6a) найти бомбу в радиусе 1.5 м и телепортироваться ТОЧНО на неё
          local found, bomb = findobj_in_cord(bs[1], bs[2], bs[3], 1.5)
          local bx, by, bz = bs[1], bs[2], bs[3]
          if found and bomb ~= nil and isobject(bomb) then
            bx, by, bz = getobjcoordes(bomb)
          end
          -- log_line(string.format("BOMB %d: цель bombard (%.2f %.2f %.2f) expected (%.2f %.2f %.2f) found=%s",
            -- i, bx, by, bz, bs[1], bs[2], bs[3], tostring(found)))
          setcarcoordes(heli, bx, by, bz)
          -- НЕ спим фиксированное время: ниже цикл ждёт РЕАЛЬНОГО условия
          -- от игры ($BOMB_STATE[i] >= 1 = вертолёт в радиусе 0.7 м от бомбы).
          local hx, hy, hz = getcarcoordes(heli)
          -- log_line(string.format("BOMB %d: вертолёт у бомбы (%.2f %.2f %.2f) $%d=%s",
            -- i, hx, hy, hz, sv, tostring(get_script_int(sv) or 0)))

          -- 6b) ждём счётчик >= 1 (условие, не задержка)
          local w = 0
          while w < 60 do
            local s = get_script_int(sv) or 0
            if s >= 1 then break end
            -- if w % 10 == 0 then log_line("BOMB " .. i .. " ждём $" .. sv .. ">=1: " .. tostring(s) .. " t=" .. tostring(w)) end
            wait(100); w = w + 1
          end
          -- log_line("BOMB " .. i .. ": перед подбором $" .. sv .. "=" .. tostring(get_script_int(sv) or 0))
          dump_states("перед ЛКМ бомбы " .. tostring(i))

          -- 6c) ЛКМ — подбор
          set_script_int(3402, (get_script_int(3402) or 0) + 300000)
          sys_mouse_down(); wait(80); sys_mouse_up()
          -- log_line("BOMB " .. i .. ": ЛКМ ПОДБОР")

          -- 6d) ждём $ >= 2
          w = 0
          while w < 60 do
            local s = get_script_int(sv) or 0
            -- if w % 10 == 0 then log_line("BOMB " .. i .. " после ЛКМ $" .. sv .. "=" .. tostring(s) .. " t=" .. tostring(w)) end
            if s >= 2 then
              -- log_line("BOMB " .. i .. ": ПОДОБРАНА ($" .. sv .. ">=2)")
              break
            end
            wait(50); w = w + 1
          end
          dump_states("после подбора бомбы " .. tostring(i))

          -- 6e) НЕ фиксированные 2 секунды, а УСЛОВИЕ ИГРЫ:
          -- после подбора ($=2) движок подтверждает прицепление и меняет
          -- счётчик (в исходнике: HAS_OBJECT_COLLIDED -> $3392 = 0).
          -- 40 тактов x 50мс — только страховка; выход — по изменению условия.
          local aw = 0
          while aw < 40 do
            local st = get_script_int(sv) or 0
            if st ~= 2 then
              -- log_line("BOMB " .. i .. ": игра подтвердила прицепление $" .. sv .. "=" .. tostring(st) ..
                       -- " за " .. tostring(aw) .. " тактов")
              break
            end
            aw = aw + 1
            wait(50)
          end
          if aw >= 40 then
            -- log_line("BOMB " .. i .. ": счётчик не изменился — страховка 2 сек истекла")
          end

          -- 6f) ТЕЛЕПОРТ К СВОЕЙ точке закладки +2.0 по Z (4 РАЗНЫЕ точки!)
          local plant_z = pp[3] + PLANT_Z_ADD
          -- log_line(string.format("BOMB %d: >>> ТЕЛЕПОРТ К ТОЧКЕ ЗАКЛАДКИ #%d (%.2f %.2f %.2f +2.0 -> %.2f) <<<",
            -- i, i, pp[1], pp[2], pp[3], plant_z))
          setcarcoordes(heli, pp[1], pp[2], plant_z)
          -- НЕ фиксированная задержка, а УСЛОВИЕ: вертолёт реально в точке закладки
          local cw = 0
          while cw < 20 do
            local cx, cy, cz = getcarcoordes(heli)
            local dist = math.sqrt((cx - pp[1])^2 + (cy - pp[2])^2 + (cz - plant_z)^2)
            if dist < 1.0 then break end
            cw = cw + 1
            wait(50)
          end
          local tx2, ty2, tz2 = getcarcoordes(heli)
          -- log_line(string.format("BOMB %d: вертолёт В ТОЧКЕ #%d (%.2f %.2f %.2f) $%d=%s settle=%d",
            -- i, i, tx2, ty2, tz2, sv, tostring(get_script_int(sv) or 0), cw))

          -- 6g) ЛКМ — закладка
          sys_mouse_down(); wait(80); sys_mouse_up()
          -- log_line("BOMB " .. i .. ": ЛКМ ЗАКЛАДКА")

          -- 6h) ждём $ >= 3
          w = 0
          while w < 60 do
            local s = get_script_int(sv) or 0
            -- if w % 10 == 0 then log_line("BOMB " .. i .. " после закладки $" .. sv .. "=" .. tostring(s) .. " t=" .. tostring(w)) end
            if s >= 3 then
              -- log_line(string.format("BOMB %d: ЗАЛОЖЕНА ($%d=3) ✓ в точке #%d", i, sv, i))
              break
            end
            wait(50); w = w + 1
          end
          dump_states("после закладки бомбы " .. tostring(i))
        end

        -- log_line("======== DEMOLITION END: все 4 бомбы обработаны ========")
        dump_states("финал")
        -- airship cheat НЕ выключаем
      end
      busy = false
    end
  end
end
