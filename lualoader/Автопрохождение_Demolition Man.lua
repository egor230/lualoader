require("lualoader/mod")

-- ============================================================================
-- Автопрохождение миссии 19 «Demolition Man» (SERG3) — активация словом "destr"
-- Механика из исходника 19 - Demolition Man.txt:
--  .rc-вертолёт RCGOBLIN (231), 4 бомбы ($3335-$3338) у (229.852, -347..-338),
--   4 точки закладки ($3367-$3378). Поднести бомбу к точке (радиус 3.5) -> заложена.
-- ============================================================================

local MISSION_ID = 19
local RC_MODEL = 231   -- RCGOBLIN (RC-вертолёт)

-- стартовые позиции 4 бомб (из исходника $3343-$3354)
local BOMB_START = {
  {229.852, -347.11, 10.0998},
  {229.852, -344.028, 10.1517},
  {229.852, -341.11, 10.5602},
  {229.852, -338.11, 10.4648},
}
-- 4 точки закладки (из исходника $3367-$3378)
local PLANT_POINT = {
  {97.574, -381.658, 7.6407},
  {156.549, -348.028, 16.8517},
  {88.1474, -361.868, 26.076},
  {140.008, -383.125, 35.3172},
}
-- модель бомбы -124 в SCM = реальная модель ၁... подберём через findobj в координатах

local busy = false

local function log(s) log_line(s) end

function main()
  while true do
    wait(10)
    local player = findplayer()
    -- Активация словом "destr" (вводится в игре как чит-код)
    if player ~= nil and not busy and cheat("destr") then
      busy = true
      log("DEMOLITION: старт миссии 19 по слову destr")

      -- 1) запустить миссию 19
      start_mission_original(MISSION_ID)
      wait(4000)
      log("DEMOLITION: миссия запущена")

      -- 2) взять RC-вертолёт 231 через пульт (как в оригинале 046E/0484)
      local px, py, pz = getpedcoordes(player)
      rc_give_remote_controlled_model_to_player(RC_MODEL, px, py, pz + 10.0, 0.0)
      wait(2000)
      local idx = rc_get_remote_controlled_car()
      local heli = (idx ~= nil and idx >= 0) and get_car_by_pool_index(idx) or nil
      log("DEMOLITION: RC-вертолёт=" .. tostring(heli ~= nil and isvehicle(heli)))

      if heli ~= nil and isvehicle(heli) then
        set_airship_cheat(true)
        set_car_gas(heli, 1.0)

        -- 3) для каждой из 4 бомб
        for i = 1, 4 do
          local bs = BOMB_START[i]
          local pp = PLANT_POINT[i]
          log(string.format("BOMB %d: летим к бомбе (%.1f %.1f %.1f)", i, bs[1], bs[2], bs[3]))

          -- 3a) летим к стартовой позиции бомбы, ждём пока подберём
          local t = 0
          while t < 120 do
            if not isvehicle(heli) then break end
            set_car_gas(heli, 1.0)
            local hx, hy, hz = getcarcoordes(heli)
            local dx, dy = bs[1] - hx, bs[2] - hy
            local d = math.sqrt(dx*dx + dy*dy)
            plane_goto_coords(heli, hx + dx/d*100.0, hy + dy/d*100.0, bs[3] + 5.0, 40)
            if d < 8.0 then break end
            t = t + 1
            wait(30)
          end
          log("BOMB " .. i .. ": у бомбы, ждём подбор")
          wait(1500)

          -- 3b) летим к точке закладки
          t = 0
          while t < 120 do
            if not isvehicle(heli) then break end
            set_car_gas(heli, 1.0)
            local hx, hy, hz = getcarcoordes(heli)
            local dx, dy = pp[1] - hx, pp[2] - hy
            local d = math.sqrt(dx*dx + dy*dy)
            plane_goto_coords(heli, hx + dx/d*100.0, hy + dy/d*100.0, pp[3] + 5.0, 40)
            if d < 8.0 then
              -- ЛКМ — сбросить бомбу в точке закладки
              sys_mouse_down(); wait(50); sys_mouse_up()
              log(string.format("BOMB %d: ЛКМ в точке закладки (%.1f %.1f %.1f)", i, pp[1], pp[2], pp[3]))
              break
            end
            t = t + 1
            wait(30)
          end
          wait(1500) -- ждём, пока игра зафиксирует закладку
        end

        set_airship_cheat(false)
        log("DEMOLITION: все 4 бомбы заложены")
      end
      busy = false
    end
  end
end
