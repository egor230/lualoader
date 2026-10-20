require("lualoader/mod")

-- ============================================================================
-- МИССИЯ "Riot" ("Мятеж") — перенос ОРИГИНАЛА (97 миссий, поток LAWYER4).
--
-- ПОТОК ОРИГИНАЛА:
-- 1. Миссия начинается у ОФИСА КЕНА (118.0, -825.9, 10.5). Значок ставит
--    штатная обёртка Star_mission_marker (C++ блайп, пересоздаётся
--    для повтора автоматически): с 19.09.2026 метка дальнобойная
--    (ADD_SPRITE_BLIP_FOR_CONTACT_POINT 0x2A7) — видна на карте издалека.
-- 2. SET_PLAYER_MOOD 1 60000, REQUEST_ANIMATION 'RIOT'. Два блайпа (оба
--    дальнобойные — видны с любого расстояния):
--    магазин Рафаэля — СПРАЙТ-БЛАЙП значка одежды 28 (02A8
--    ADD_SPRITE_BLIP_FOR_COORD ... 28 $59);
--    стройплощадка — ЖЁЛТЫЙ квадрат (018A ADD_BLIP_FOR_COORD 299 -314.6 11.4
--    $19 даёт цвет 5, затем 016B CHANGE_BLIP_COLOUR 4 = жёлтый). Сфер
--    не заливать: сфера только У ОДЕЖДЫ радиусом 3.
-- 3. Переодевание у Рафаэля (GOSUB L48df8 — КАТСЦЕНА): пешком у двери →
--    01B4 SET_PLAYER_CONTROL 0 + 02A3 SWITCH_WIDESCREEN 1 + 015F/0160 камера →
--    0211 GOTO_COORD_ON_FOOT → 016A DO_FADE 1000 0 (ЗАТЕМНЕНИЕ) → 0352 UNDRESS
--    'PLAYER3' + 038B LOAD_ALL_MODELS_NOW + 0353 DRESS → 0055 SET_PLAYER_
--    COORDINATES 94.9 -1135 9.4 + 0171 SET_PLAYER_HEADING 57.9 → 016A DO_FADE
--    1500 1 (проявление) → 03C8/02EB камера обратно + 01B4 CONTROL 1. Блайп снят.
-- 4. Подъезд к стройке (LOCATE_PLAYER_ANY_MEANS_3D 70/70/30) → сцена бунта:
--    21 пикетчик (SPANDXA/SPANDXB), 3 GDA, 3 грузовика SPAND, 2 объекта -114.
--    Один раз печатается LAW4_13 "Уничтожь грузовики".
-- 5. БОЙ (L4725d): здоровье ≤99 → в бой; умер → intensity -6; укушено>3 или
--    убито>2 → вся толпа (GOSUB L48970), $2205=1. ЛЮБОЙ урон → сразу.
-- 6. ЭСКАЛАЦИЯ: разветвление по ОДЕЖДЕ. БЕЗ одежды — все бьют игрока
--    (kill_player_on_foot). В одежде — дерутся друг с другом (threat 16 без
--    угрозы игроку). В ЛЮБОМ СЛУЧАЕ ОТКРЫВАЮТСЯ ВОРОТА $-33
--    SLIDE_OBJECT 298,-313.6,11 → 304,-313.6,11 (ворота стройки), GDA-боссы
--    получают personality 30 + threat_search, LAW4_14 ($2206=1).
--    Ворота ищутся по координатам (get_obj_in_cord) — модель -33 встречается
--    по всей карте, find_object_by_model вернёт первый попавшийся!
-- 7. Победа — 3 грузовика уничтожены → miss(1000). Провал — M_FAIL ($1778
--    SET_OBJECT_COORDINATES обратно 298,-313.6,11 = закрыть ворота).
--
-- ВАЖНО ПРО АРХИТЕКТУРУ (9-я ревизия): вспомогательные функции живут ВЫШЕ main()
-- (dressed_as_worker, spawn_trucks, start_riot) — состояние миссии передаётся в
-- них параметрами. «Переоделся ли Томми» определяется НЕ флагом, а реальным скином
-- через C++ get_player_skin (CModelInfo) — см. dressed_as_worker ниже.
-- ============================================================================

-- ============================================================================
-- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ МИССИИ (вынесены выше main — так читаемее; создаются
-- один раз при загрузке скрипта, а не на каждом старте миссии). Состояние миссии
-- (step, rioters, escalated…) живёт в main и передаётся сюда параметрами — в
-- глобалы не выносить (см. ПЕРЕНОС_МИССИИ_RIOT.md §1).
-- ============================================================================

-- dressed_as_worker(): Томми в одежде рабочего? Определяется по НАСТОЯЩЕМУ скину
-- игрока — C++ get_player_skin (funcs.cpp) читает CModelInfo::GetModelInfo, а НЕ
-- кэш set_skin и не флаг-переменную: переживает перезагрузку скрипта (Ctrl) и
-- учитывает любой путь смены скина, в т.ч. оставшийся с прошлого прохождения.
-- Сравнение в нижнем регистре — движок хранит имена моделей в разном регистре.
local function dressed_as_worker()
 local s = get_player_skin()  -- имя текущей модели игрока ("player" — обычный Томми, "player3" — в одежде рабочего)
 if s == nil then return false end
 return string.lower(s) == "player3"
end

-- spawn_trucks(): создаёт 3 грузовика рабочих и маркеры на них; ВОЗВРАЩАЕТ
-- truck1, truck2, truck3, mt1, mt2, mt3. БЕЗ потоков-наблюдателей (8-я ревизия):
-- create_marker_car (funcs.cpp:1249) пишет markeron.emplace в ОБЩИЙ map БЕЗ
-- мьютекса + findcarinpool (сон 1мс + скан пула) — 3 параллельных потока давили
-- data race: краш через ~47с после старта бунта (trace.txt обрывался на их
-- getflagmission-трассах, БЕЗ «destroy: ВХОД» и БЕЗ «игрок погиб» = жёсткий краш,
-- а не провал). Снятие маркеров — в главном цикле, каждый кадр (даже быстрее
-- потоков с wait(100)); подробно — ПОТОКИ_CREATE_NEWTHREAD.md.
local function spawn_trucks()
 local t1 = Createcar("MODEL_SPAND", 286.7, -302.1, 11.9) if t1 ~= nil then setangle(t1, 270) end  -- 3 грузовика рабочих (MODEL_SPAND=213), углы как в оригинале
 local t2 = Createcar("MODEL_SPAND", 312.2, -304.7, 11.9) if t2 ~= nil then setangle(t2, 90) end
 local t3 = Createcar("MODEL_SPAND", 312.2, -300.7, 11.9) if t3 ~= nil then setangle(t3, 90) end
 local m1, m2, m3
 if t1 ~= nil then m1 = create_marker(t1) end  -- маркер над грузовиком (mod.lua:709 → create_marker_car → ADD_BLIP_FOR_CAR)
 if t2 ~= nil then m2 = create_marker(t2) end
 if t3 ~= nil then m3 = create_marker(t3) end
 ma_trace("riot: 3 грузовика SPAND созданы, маркеры навешены (главный поток)")
 return t1, t2, t3, m1, m2, m3
end

-- start_riot(pl, rioters, agit1, agit2, leader, guard_target): ЭСКАЛАЦИЯ мятежа —
-- вызывается ОДИН раз при первом уроне/убийстве рабочего. Одежду определяет
-- dressed_as_worker() (по скину, не по флагу). guard_target — таблица
-- «охранник → цель-мятежник», мутируется тут и читается главным циклом для
-- перебинда, если цель умрёт; escalated = 1 выставляет ВЫЗЫВАЮЩИЙ.
-- Ветвление: БЕЗ одежды — толпа И охранники валят ИГРОКА (01CA); В ОЖЕДЕ рабочего —
-- рабочие дерутся друг с другом (0216 по «соседу»), а охранники GDA бегут и бьют
-- мятежников: 0350-флаг бригадира («держать позицию») снимается, personality 30 +
-- threat 16 (прохожие) и ЯВНАЯ цель 0216 — без явной цели ped_search_threat не
-- атакует (урок 6-й ревизии).
local function start_riot(pl, rioters, agit1, agit2, leader, guard_target)
 local guards = {}  -- список охранников БЕЗ дыр (ipairs остановился бы на nil-дыре и пропустил бы бригадира)
 if agit1  ~= nil then guards[#guards + 1] = agit1  end
 if agit2  ~= nil then guards[#guards + 1] = agit2  end
 if leader ~= nil then guards[#guards + 1] = leader end

 if not dressed_as_worker() then  -- (БЕЗ одежды: рабочие поняли, что игрок — чужак → ВСЯ толпа валит НА ИГРОКА)
  for _, r in ipairs(rioters) do
   if isped(r) then
    clean_ped_wait(r)                        -- 053D: снять wait-state 29 (иначе педы стоят и не дерются)
    set_ped_personality(r, 30)               -- 0243
    ped_search_threat(r, 16)                 -- 011A (прохожие)
    ped_search_threat(r, 1)                  -- 011A (игрок — главная цель)
    set_char_obj_kill_player_on_foot(r, pl)  -- 01CA: ЯВНАЯ цель — игрок
   end
  end
  for _, g in ipairs(guards) do  -- охранники БЕЗ одежды открывают огонь ПО ИГРОКУ, мятежников не трогают
   if isped(g) then
    set_ped_personality(g, 30)
    ped_search_threat(g, 1)
    if g == leader then set_ped_accuracy(leader, 10) end  -- стрелок с colt45
    set_char_obj_kill_player_on_foot(g, pl)  -- 01CA: ЯВНАЯ цель — игрок
   end
  end
  --printmessage("~r~They've seen you! Get out!", 4000, 1)
 else  -- (В одежде рабочего: рабочие дерутся ДРУГ С ДРУГОМ; охранники добивают мятежников)
  local n = #rioters
  for i, r in ipairs(rioters) do
   if isped(r) then
    clean_ped_wait(r)                         -- 053D
    set_ped_only_damaged_by_player(r, false)  -- 02A9: снять «только игрок ранит» — иначе урон между рабочими не проходит
    set_ped_personality(r, 30)                -- 0243
    ped_search_threat(r, 16)                  -- 011A (прохожие — бьют друг друга)
    local t = rioters[(i % n) + 1]            -- цель-«сосед»: каждый рабочий СРАЗУ целится в соседа
    if t ~= nil and isped(t) and t ~= r then
     set_char_obj_kill_char_on_foot(r, t)     -- 0216: гарантирует реальное нападение на другого рабочего
    end
   end
  end
  for _, g in ipairs(guards) do  -- охранники В ОДЕЖДЕ бегут к мятежниками и бьют их (агитаторы — с nitestick, бригадир — с colt45)
   if isped(g) then
    set_ped_stay_when_attacked(g, false)      -- 0350: СНЯТЬ «держать позицию» (бригадир сто́ял с 0350=true — не бежал на цель)
    set_ped_personality(g, 30)                -- 0243
    ped_search_threat(g, 16)                  -- 011A (прохожие = мятежники в рабочей одежде)
    if g == leader then set_ped_accuracy(leader, 10) end
    local gt = nil                            -- первый живой мятежник — цель для охранника
    for _, r in ipairs(rioters) do if isped(r) and not ispeddead(r) then gt = r break end end
    if gt ~= nil then
     set_char_obj_kill_char_on_foot(g, gt)    -- 0216: ЯВНАЯ цель — мятежник (без неё threat_search не атакует)
     guard_target[g] = gt                     -- запомнить для перебинда в главном цикле, если цель умрёт
    end
   end
  end
  --printmessage("~r~The workers have turned on each other! Get the trucks!", 4000, 1)
 end
 ma_trace("riot: вся толпа взбешена ($2205=1), dressed=" .. tostring(dressed_as_worker()))
end

-- drop_agitator_colt(agit, dropped): зачинщик GDA умер → выпадает его колт из
-- трупа (оригинал 04A5 GET_DEAD_CHAR_PICKUP_COORDS + 032B CREATE_PICKUP_WITH_AMMO
-- 274 3 17). dropped = «колт уже выпал» (1 = да). Возвращает обновлённый флаг:
-- 1, если колт выпал сейчас (или уже был), иначе старое значение.
local function drop_agitator_colt(agit, dropped)
 if dropped == 0 and agit ~= nil and isped(agit) and ispeddead(agit) then
  local dx, dy, dz = get_dead_char_pickup_coords(agit)
  if dx ~= nil then
   create_weapon_pickup(274, 3, 17, dx, dy, dz + 0.5)
   return 1
  end
 end
 return dropped
end

-- dress_cutscene(pp, dress_blip, dress_sphere): катсцена переодевания у Рафаэля
-- (оригинал GOSUB L48df8): заморозка + широкий экран + камера на дверь + Томми
-- идёт к двери + ЗАТЕМНЕНИЕ + set_skin("PLAYER3") + телепорт из магазина +
-- проявление + камера обратно. Скин меняется ТОЛЬКО за чёрным экраном (§2 правил).
-- Возвращает (dress_blip, dress_sphere) — снимает их после переодевания (одежду
-- дальше определяет dressed_as_worker() по реальному скину, а не флаг).
local function dress_cutscene(pp, dress_blip, dress_sphere)
ma_trace("riot: катсцена переодевания (L48df8): начало, затемнение")
ped_frozen(0)  -- 01B4 SET_PLAYER_CONTROL 0 — заморозить игрока на время катсцены
set_widescreen(true)  -- 02A3 SWITCH_WIDESCREEN 1 — широкий экран
set_camera_position(91.813, -1131.12, 17.251, 0.0, 0.0, 0.0)  -- 015F SET_FIXED_CAMERA_POSITION — камера на магазин
camera_at_point(92.39, -1131.74, 16.724, 2)  -- 0160 POINT_CAMERA_AT_POINT — направление на дверь
set_char_obj_goto_coord_on_foot(pp, 96.4, -1136.7, 9.4)  -- 0211 SET_CHAR_OBJ_GOTO_COORD_ON_FOOT — Томми идёт к двери
wait(1300)
fade(0, 1000)  -- 016A DO_FADE 1000 0 — ЗАТЕМНЕНИЕ: скин меняется за чёрным экраном
wait(1000)
set_skin(pp, "PLAYER3")  -- 0352 UNDRESS 'PLAYER3' + 038B LOAD_ALL_MODELS_NOW + 0353 DRESS — переодевание в рабочего
setcord(pp, 94.9, -1135.0, 9.4)  -- 0055 SET_PLAYER_COORDINATES — выход из магазина
setangle(pp, 57.9)  -- 0171 SET_PLAYER_HEADING
fade(1, 1500)  -- 016A DO_FADE 1500 1 — проявление
wait(1500)
restore_camera()  -- 03C8 SET_CAMERA_IN_FRONT_OF_PLAYER + 02EB RESTORE_CAMERA_JUMPCUT — камера обратно
ped_frozen(1)  -- 01B4 SET_PLAYER_CONTROL 1 — разморозить игрока
set_widescreen(false)  -- 02A3 SWITCH_WIDESCREEN 0 — широкий экран выкл
if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
--printmessage("~g~You look like a worker. ~y~Now hit the construction site!", 5000, 1)
ma_trace("riot: Томми переоделся в рабочего у Рафаэля ($2213=1)")
  return dress_blip, dress_sphere
 end

-- spawn_rioters(rioters, bite_flag, dead_flag, hp_prev): создаёт 21 пикетчика
-- (SPANDXA/SPANDXB) с настройками оригинала L46c6c. Таблицы передаются ПО ССЫЛКЕ и
-- заполняются тут: rioters — сами педы, bite_flag/dead_flag — флаги урона/смерти
-- (0), hp_prev — float-HP на момент создания (база детектора «HP упал относительно
-- прошлого кадра»).
local function spawn_rioters(rioters, bite_flag, dead_flag, hp_prev)
 for i, p in ipairs({
  {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
  {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8},
  {296.2,-325.3},{299.3,-329.3},{303.8,-329.7},{308.5,-328.4},
  {310.2,-324.2},{305.6,-322.2},{300.0,-317.5},{294.9,-317.6},
  {291.0,-320.0},{293.0,-324.8},{291.6,-317.2},{301.1,-324.0},
  {297.6,-327.649}
 }) do  -- 21 ПИКЕТЧИК (CREATE_CHAR 4 112/113), каждому — как оригинал L46c6c
  local skin = (i % 2 == 1) and "SPANDXA" or "SPANDXB"  -- оригинал: чередование 112/113
  local r = Createped(skin, p[1], p[2], 10.9)  -- пед на координате пикетчика
  if r ~= nil then
   ped_clean_threat(r)  -- 01ED CLEAR_CHAR_THREAT_SEARCH
   set_ped_personality(r, 16)  -- (оригинал 0243 SET_CHAR_PERSONALITY 16: обычный горожанин)
   set_ped_only_damaged_by_player(r, true)  -- (оригинал 02A9 SET_CHAR_ONLY_DAMAGED_BY_PLAYER: ранить может только игрок)
   set_ped_wait_state(r, 29, 99999999)  -- (оригинал 0372 SET_CHAR_WAIT_STATE 29: стоит, пока игрок не ранит)
   table.insert(rioters, r)
   bite_flag[#rioters] = 0
   dead_flag[#rioters] = 0
   hp_prev[#rioters] = getpedhealthf(r)
  end
 end
end

-- spawn_riot_scene(player, rioters, bite_flag, dead_flag, hp_prev, riot_intensity):
-- создаёт ВСЮ сцену бунта на стройплощадке — оружие игроку, уровень беспорядков,
-- 3 грузовика (с маркерами), 2 объекта -114, ворота, 21 пикетчика (spawn_rioters)
-- и 3 зачинщика GDA. ВОЗВРАЩАЕТ все созданные сущности — вызов присваивает их
-- локалам main: truck1..3, mt1..3, obj1..2, gate, agit1..2, leader. Таблицы
-- (rioters/bite_flag/dead_flag/hp_prev) мутируются по ссылке.
local function spawn_riot_scene(player, rioters, bite_flag, dead_flag, hp_prev, riot_intensity)
 Giveweaponped(player, 6, "molotov")  -- коктейли Молотова — основное оружие
 set_current_weapon_ped(player, 15)  -- положить в руки (molotov = тип 15)

 set_riot_intensity(riot_intensity)  -- (оригинал 0552 SET_RIOT_INTENSITY 127: уровень беспорядков)

 local truck1, truck2, truck3, mt1, mt2, mt3 = spawn_trucks()  -- 3 грузовика SPAND + маркеры на каждом (снятие маркеров — в главном цикле, см. блок в step==3)

 local obj1 = create_obj(-114, 309.3, -298.6, 10.9) if obj1 ~= nil then make_obj_targettable(obj1, true) end  -- 2 объекта -114 + 035D MAKE_OBJECT_TARGETTABLE (как в оригинале)
 local obj2 = create_obj(-114, 284.3, -312.4, 10.9) if obj2 ~= nil then make_obj_targettable(obj2, true) end

 local gate = get_obj_in_cord(298.0, -313.6, 11.0, 8.0)  -- (оригинал 00 - Initial.txt:739: ворота стройки — объект -33; ищем ПО КООРДИНАТАМ, модель -33 есть в нескольких местах карты)
 ma_trace("riot: ворота -33 найдены по координатам")

 spawn_rioters(rioters, bite_flag, dead_flag, hp_prev)

 local agit1 = Createped("GDA", 301.2, -311.5, 10.9) if agit1 ~= nil then setangle(agit1, 180) end  -- (оригинал CREATE_CHAR 5 91: 3 зачинщика GDA, углы 180)
 local agit2 = Createped("GDA", 303.2, -311.5, 10.9) if agit2 ~= nil then setangle(agit2, 180) end
 local leader = Createped("GDA", 304.0, -298.7, 10.9) if leader ~= nil then setangle(leader, 180) end
 if agit1 ~= nil then Giveweaponped(agit1, 0, "nitestick") set_current_weapon_ped(agit1, 4) end  -- (оригинал 01B2 GIVE_WEAPON_TO_CHAR 4 0: nitestick, тип 4)
 if agit2 ~= nil then Giveweaponped(agit2, 0, "nitestick") set_current_weapon_ped(agit2, 4) end
 if leader ~= nil then  -- (оригинал: бригадир 0223 SET_CHAR_HEALTH 70 + GIVE_WEAPON 17 300 (colt45) + 0350 STAY)
  sethealth(leader, 70)
  Giveweaponped(leader, 300, "colt45")
  set_current_weapon_ped(leader, 17)
  set_ped_stay_when_attacked(leader, true)
 end
 if agit1 ~= nil then set_ped_only_damaged_by_player(agit1, true) end  -- (оригинал 02A9: всех трёх зачинщиков можно ранить только игроком)
 if agit2 ~= nil then set_ped_only_damaged_by_player(agit2, true) end
 if leader ~= nil then set_ped_only_damaged_by_player(leader, true) end

 return truck1, truck2, truck3, mt1, mt2, mt3, obj1, obj2, gate, agit1, agit2, leader
end

function main()
while true do wait()  local player = findplayer()  -- указатель игрока (обновляется каждую итерацию внешнего цикла)

 if Star_mission_marker(9, 118.0, -825.9, 10.5)
 then
  ped_frozen(1)  -- разморозить игрока: обёртка SMM погасила экран (mod.lua:1410); ped_frozen(0)=ЗАМОРОЗИТЬ, 1=РАЗМОРОЗИТЬ (races() мод.lua:401/414)
  fade(1, 1200)  -- проявление экрана после Star_mission_marker
  wait(1200)

  local step = 1          -- этап миссии: 1 = переодевание/подъезд к стройке, 3 = бой, nil = конец
  local bitten = 0        -- сколько пикетчиков игрок уже «укусил» ($2212)
  local killed = 0        -- сколько пикетчиков умерло ($2198)
  local escalated = 0     -- толпа взбешена ($2205)
  local riot_intensity = 127  -- уровень беспорядков (0552), убывает по убийствам пикетчиков
  local gates_open = 0    -- ворота стройки открыты ($2206)
  local gate_frames = 0   -- осталось кадров движения ворот (6ед / 0.1ед-в-кадр = 60 кадров ≈ 2 с; как оригинал L47f4e)
  local law4_13_shown = 0  -- LAW4_13 печатается один раз при подъезде близко
  local frame = 0          -- счётчик кадров боя (step==3): для низкочастотной диагностики — боевой цикл не оставляет трейсов, иначе краш/ошибку не локализовать
  local agit1_drop = 0    -- колт уже выпал из трупа агитатора 1
  local agit2_drop = 0    -- колт уже выпал из трупа агитатора 2

  local rioters = {}      -- 21 пикетчик (SPANDXA/SPANDXB)
  local bite_flag = {}    -- bite_flag[i] — «укушен» ли пикетчик i
  local dead_flag = {}    -- dead_flag[i] — умер ли пикетчик i
  local hp_prev = {}      -- hp_prev[i] — HP пикетчика i в прошлом кадре (float): любой урон = падение HP
   local truck1, truck2, truck3  -- 3 грузовика рабочих (MODEL_SPAND=213) — главная цель миссии
  local mt1, mt2, mt3  -- маркеры грузовиков; снимаются главным циклом В КАДР уничтожения (потоки-наблюдатели убраны — см. spawn_trucks)
  local obj1, obj2        -- 2 объекта -114 (приоритетные цели, 035D)
  local agit1, agit2, leader  -- 3 зачинщика GDA: 2 с nitestick + бригадир с colt45
  local gate              -- ворота стройки (глобальный объект -33) — открываются при эскалации
  set_wanted(0)  -- очистить розыск перед началом миссии
  showtext(" Riot", 500, 1)  -- заголовок миссии на экране
  ma_trace("riot: СТАРТ у офиса Кена (118.0, -825.9, 10.5)")

  --printmessage("~w~Ken: Tommy! The workers at Avery's site have gone on the rampage!", 5000, 1)  -- катсцена LAW_4 → короткий текст (LOAD_CUTSCENE в API нет)
  wait(2000)
  --printmessage("~w~Ken: Get some work clothes from Rafael's so they don't recognize you!", 6000, 1)
  wait(2000)

  set_ped_mood(1, 60000)  -- 04E3 SET_PLAYER_MOOD 1 60000 — настроение игрока на 60 сек
  request_animation("RIOT")  -- (оригинал 04ED REQUEST_ANIMATION 'RIOT': анимации толпы)

   local dress_blip = radar_add_sprite_blip_for_coord(95.4, -1135.5, 9.4, 28)  -- (оригинал 02A8 ADD_SPRITE_BLIP_FOR_COORD 95.4 -1135.5 9.4 28: значок одежды на карте)
   local dress_sphere = create_sphere(95.4, -1135.5, 9.4, 3.0)  -- (макет: малая сфера у двери Рафаэля, радиус 3 — без «заливки» квартала)

   local site_blip = radar_add_blip_for_coord(299.0, -314.6, 11.4)  -- (оригинал 018A ADD_BLIP_FOR_COORD 299 -314.6 11.4: квадрат на карте, 018A даёт цвет 5)
   radar_change_blip_colour(site_blip, 4)  -- (оригинал 016B CHANGE_BLIP_COLOUR 4: жёлтый квадрат бунта — от предыдущего штатного цвета 5 (пурпур))

  --printmessage("~g~Get the work outfit at Rafael's, then hit the construction site.", 6000, 1)
   ma_trace("riot: блайпы Рафаэля (спрайт 28) и стройки созданы, сферы не дикие")

   local guard_target = {}  -- guard_target[g] = текущая цель-мятежник охранника g (для перебинда, если цель умрёт; ключи — lightuserdata, валидны в Lua)

   while true == Getflagmission() do --wait(10)
    local pp = findplayer()  -- указатель игрока на этой итерации цикла миссии
   if pp == nil then pp = player end  -- запасной: свежий не получен — берём внешний

    if (step == 1 or step == 3) and not dressed_as_worker() and not is_ped_in_car(pp)  -- шаг 3 тоже: игрок мог прийти на стройку БЕЗ одежды — переодевание остаётся доступным всю миссию, пока не оденется (фикс софтлока «пикап одежды пропадает»); одежду проверяет dressed_as_worker() по скину
    and ped_in_point_in_radius(pp, 95.4, -1135.5, 9.4, 1.2, 1.2, 3.0) then  -- (оригинал 00F6 LOCATE_PLAYER_ON_FOOT_3D 95.4 -1135.5 9.4 1.2 1.2 3: игрок пешком у двери Рафаэля)
    dress_blip, dress_sphere = dress_cutscene(pp, dress_blip, dress_sphere)
   end

   if step == 1 and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 70.0, 70.0, 30.0) then  -- (оригинал L46bee: LOCATE_PLAYER_ANY_MEANS_3D 299 -314.6 11.4 70 70 30: подъезд к стройплощадке)
     if site_blip ~= nil then remove_blip(site_blip) site_blip = nil end
     if dressed_as_worker() then  -- игрок УЖЕ в одежде рабочего (проверка по скину, не по флагу) → блайп и сферу у Рафаэля убираем (больше не нужны); БЕЗ одежды — ОСТАВЛЯЕМ: игрок пришёл на стройку первым, и переодевание всё ещё должно быть доступно (софтлок: иначе метка одежды пропадала навсегда)
      if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
      if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
     end

      truck1, truck2, truck3, mt1, mt2, mt3, obj1, obj2, gate, agit1, agit2, leader = spawn_riot_scene(player, rioters, bite_flag, dead_flag, hp_prev, riot_intensity)  -- создать всю сцену бунта (оружие, грузовики, объекты, ворота, 21 пикетчик, 3 GDA) — сущности возвращаются функцией, таблицы мутируются по ссылке

    --printmessage("~g~Destroy the workers' trucks to break the strike!", 6000, 1)
    --printmessage("~r~Workers: This is our site! Get out!", 5000, 1)
    --printmessage("~r~Workers: Burn everything! Strike forever!", 5000, 1)
    ma_trace("riot: сцена создана (3 SPAND, 2 obj -114, 21 пикетчик, 3 GDA); бунт")
    step = 3
   end

   if step == 3 and law4_13_shown == 0
    and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 20.0, 20.0, 10.0) then  -- (оригинал @L4849f: LOCATE_PLAYER_ANY_MEANS_3D 299 -314.6 11.4 20 20 10: LAW4_13, игрок близко к стройке)
    law4_13_shown = 1
    --printmessage("~r~Destroy the trucks! Now!", 8000, 1)
   end

 if step == 3 then  -- (оригинал @L4725d: главный цикл боя — любой урон пикетчику от игрока ЛЮБЫМ оружием → вся толпа сразу дерётся; 5-я ревизия: HP-детектор по кадрам, 6-я: явные цели kill_char_on_foot)
     for i, r in ipairs(rioters) do
     if isped(r) then
      if not ispeddead(r) then
       local h = getpedhealthf(r)  -- float-HP текущего кадра (оригинал 8184 NOT IS_CHAR_HEALTH_GREATER $2177 99 заменить надёжным детектором)
       if bite_flag[i] == 0 and escalated == 0 and h < hp_prev[i] then  -- ЛЮБОЙ урон (даже минимальный, ЛЮБЫМ оружием): HP упало относительно прошлого кадра
        bite_flag[i] = 1
        bitten = bitten + 1
       end
       hp_prev[i] = h  -- запомнить HP кадра для сравнения на следующем
      else
       if dead_flag[i] == 0 then  -- (оригинал L49043: умер пикетчик → SET_RIOT_INTENSITY -= 6)
        dead_flag[i] = 1
        killed = killed + 1
        riot_intensity = riot_intensity - 6
        if riot_intensity < 0 then riot_intensity = 0 end
        set_riot_intensity(riot_intensity)
       end
      end
     end
    end

      if escalated == 0 and (bitten > 0 or killed > 0) then  -- (оригинал L47c68 → GOSUB L48970: любой урон/убийство рабочего → ВСЯ толпа вступает; порог >3/>2 заменён на «сразу»)
       start_riot(pp, rioters, agit1, agit2, leader, guard_target)
       escalated = 1  -- эскалация прошла (start_riot выставляет поведение педов, флаг — здесь: это состояние главного цикла)
      end

      if escalated == 1 and gates_open == 0 then  -- (оригинал L47dfc: ворота открываются ВСЕГДА после эскалации — SLIDE_OBJECT $1778 → 304 -313.6 11, LAW4_14; одежда определяет КОГО бьют охранники, но не факт открытия ворот — теперь это в start_riot)
       gates_open = 1
       gate_frames = 60  -- SLIDE_OBJECT по 0.1/кадр: 298→304 = 6 ед = 60 кадров (оригинал L47f4e)
       if gate ~= nil then
        ma_trace("riot: ворота открываются (SLIDE_OBJECT 298 → 304 -313.6 11)")  -- движение ниже, каждый кадр (как L47f4e: SLIDE_OBJECT двигает на 0.1 за кадр, пока не доедет)
       end
       --printmessage("~r~Lawyers and cops are on their way! Get the trucks!", 8000, 1)
      end

     if escalated == 1 and dressed_as_worker() then  -- перебинд целей охранников: если цель-мятежник умерла, охранник сразу получает НОВУЮ живую цель (один стартовый бинд в start_riot быстро протухает — за бой гибнет много мятежников); только в одежде (проверка по скину)
      for g, gt in pairs(guard_target) do
       if isped(g) and not ispeddead(g) then
        if gt == nil or not isped(gt) or ispeddead(gt) then  -- цель протухла
         local ng = nil
         for _, r in ipairs(rioters) do if isped(r) and not ispeddead(r) then ng = r break end end
         if ng ~= nil then
          set_char_obj_kill_char_on_foot(g, ng)  -- 0216: новая ЯВНАЯ цель
          guard_target[g] = ng
         end
        end
       end
      end
     end

     if gates_open == 1 and gate ~= nil and gate_frames > 0 then  -- (оригинал @L47f4e: движение ворот БЕЗ кадрового getobjcoordes, каждый кадр SLIDE_OBJECT — ровно 60 кадров, потом стоп; getobjcoordes каждый кадр навсегда = findobjinpool sleep_for(1ms)+скан пула → зависание кадра)
     move_obj(gate, 304.0, -313.6, 11.0, 0.1, 0.1, 0.0, 0)
     gate_frames = gate_frames - 1
    end

    if killed > 14 then  -- убито >14 → SET_RIOT_INTENSITY 0 (оригинал L47cac)
     set_riot_intensity(0)
    end

     agit1_drop = drop_agitator_colt(agit1, agit1_drop)  -- колт из трупа зачинщика 1 (если он умер и колт ещё не выпадал)
     agit2_drop = drop_agitator_colt(agit2, agit2_drop)  -- колт из трупа зачинщика 2

      -- снять маркер с грузовика В КАДР его уничтожения — главный цикл, ОДНОПОТОЧНО. Условие упрощено до iscardead/iscarfire: iscardead(stale-указатель) теперь безопасно возвращает true (C++ NULL-guard, 10-я ревизия) — обломок, вывезенный игрой, считается уничтоженным
      if mt1 ~= nil and (truck1 == nil or iscardead(truck1) or iscarfire(truck1)) then remove_blip(mt1) mt1 = nil ma_trace("riot: грузовик 1 уничтожен — маркер снят (frame=" .. frame .. ")") end
      if mt2 ~= nil and (truck2 == nil or iscardead(truck2) or iscarfire(truck2)) then remove_blip(mt2) mt2 = nil ma_trace("riot: грузовик 2 уничтожен — маркер снят (frame=" .. frame .. ")") end
      if mt3 ~= nil and (truck3 == nil or iscardead(truck3) or iscarfire(truck3)) then remove_blip(mt3) mt3 = nil ma_trace("riot: грузовик 3 уничтожен — маркер снят (frame=" .. frame .. ")") end

       if (truck1 == nil or iscardead(truck1))
       and (truck2 == nil or iscardead(truck2))
       and (truck3 == nil or iscardead(truck3)) then  -- (оригинал L4850c: все 3 грузовика уничтожены → ПОБЕДА $1000; маркеры уже сняты кадровым блоком выше — каждый в кадр уничтожения своего грузовика)
      remove_animation("RIOT")  -- (оригинал 04EF REMOVE_ANIMATION 'RIOT' + SET_RIOT_INTENSITY 0, очистка L4864f)
     set_riot_intensity(0)
     --printmessage("~g~The strike is broken! Avery's site is secure.", 4000, 1)
     wait(4000)
     for _, r in ipairs(rioters) do if isped(r) then remove_ped(r) end end
     if agit1 ~= nil and isped(agit1) then remove_ped(agit1) end
     if agit2 ~= nil and isped(agit2) then remove_ped(agit2) end
     if leader ~= nil and isped(leader) then remove_ped(leader) end
     if truck1 ~= nil and isvehicle(truck1) then remove_car(truck1) end
     if truck2 ~= nil and isvehicle(truck2) then remove_car(truck2) end
     if truck3 ~= nil and isvehicle(truck3) then remove_car(truck3) end
     if obj1 ~= nil then remove_obj(obj1) end
     if obj2 ~= nil then remove_obj(obj2) end
     ma_trace("riot: МИССИЯ ВЫПОЛНЕНА (3 грузовика уничтожены; ворота: " .. tostring(gates_open) .. ")")
     miss(1000)
     step = nil
     break
    end
   end

  end

    if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end  -- пост-цикловая очистка: смерть/арест (destroy() снимает педов/авто) — блайпы, сфера, уровень беспорядков и ворота возвращаем как было
    if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
     if site_blip ~= nil then remove_blip(site_blip) site_blip = nil end
    if mt1 ~= nil then remove_blip(mt1) mt1 = nil end  -- и маркеры грузовиков (если миссия провалена до их уничтожения — иначе висели бы на мёртвых/чужих авто)
    if mt2 ~= nil then remove_blip(mt2) mt2 = nil end
    if mt3 ~= nil then remove_blip(mt3) mt3 = nil end
   if step == 3 then set_riot_intensity(0) end
  if gates_open == 1 and gate ~= nil then  -- (оригинал @L485a1 M_FAIL: ворота открывались — закрыть обратно SLIDE_OBJECT в 298,-313.6,11)
   move_obj(gate, 298.0, -313.6, 11.0, 0.15, 0.15, 0.0, 0)
   ma_trace("riot: ворота закрыты (M_FAIL)")
  end
  step = nil
 end
end
end
