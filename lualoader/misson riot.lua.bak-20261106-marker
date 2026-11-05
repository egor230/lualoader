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
require("lualoader/mod")  -- подключаем модуль загрузчика игры

local function dressed_as_worker()  -- функция проверки скина рабочего
 local s = get_player_skin()  -- получаем скин игрока
 if s == nil then return false end  -- если скин пустой, возвращаем ложь
 return string.lower(s) == "player3"  -- сверяем название модели в нижнем регистре
end  -- конец функции dressed_as_worker

local function spawn_trucks()  -- функция создания грузовиков и маркеров
 local t1 = Createcar("MODEL_SPAND", 286.7, -302.1, 11.9) if t1 ~= nil then setangle(t1, 270) end  -- спавним первый грузовик
 local t2 = Createcar("MODEL_SPAND", 312.2, -304.7, 11.9) if t2 ~= nil then setangle(t2, 90) end   -- спавним второй грузовик
 local t3 = Createcar("MODEL_SPAND", 312.2, -300.7, 11.9) if t3 ~= nil then setangle(t3, 90) end   -- спавним третий грузовик
 local m1, m2, m3  -- переменные для блайпов
 if t1 ~= nil then m1 = create_marker(t1) end  -- создаем маркер над первой машиной через create_marker
 if t2 ~= nil then m2 = create_marker(t2) end  -- создаем маркер над второй машиной через create_marker
 if t3 ~= nil then m3 = create_marker(t3) end  -- создаем маркер над третьей машиной через create_marker
 return t1, t2, t3, m1, m2, m3  -- возвращаем машины и их блайпы наружу
end  -- конец функции spawn_trucks

local function update_trucks_and_check_win(state)  -- функция проверки грузовиков: уничтоженный теряет маркер, живой держит цель
 local all_dead = true  -- флаг победы по умолчанию истинный

 for i = 1, 3 do  -- цикл по трем грузовикам
  local tr = state.trucks[i]  -- получаем текущий грузовик
  local mk = state.markers[i]  -- получаем текущий маркер
  -- проверка авто ПЕРЕД снятием маркера: есть в пуле, не уничтожено, не горит (для вывезенной машины iscardead вернёт true)
  local alive = tr ~= nil or  iscardead(tr) or iscarfire(tr) and getcarhealth(tr)<200

  if alive then  -- если машина цела
   all_dead = false  -- сбрасываем флаг победы
  else  -- если машина уничтожена, горит или вывезена движком
   if mk ~= nil then  -- если блайп еще висит над уничтоженной машиной
    remove_blip(mk)  -- удаляем конкретный маркер уничтоженной машины
    state.markers[i] = nil  -- обнуляем ячейку маркера в таблице
    if tr ~= nil then printmessage("~y~Truck Wrecked!", 3000, 1) end  -- сообщение о разрушении
   end  -- конец проверки блайпа
  end  -- конец проверки состояния авто
 end  -- конец цикла

 return all_dead  -- возвращаем итоговое состояние цели
end  -- конец функции update_trucks_and_check_win

local function cleanup_riot_scene(rioters, agit1, agit2, leader, truck1, truck2, truck3, obj1, obj2)  -- функция полной очистки сцены
 remove_animation("RIOT")  -- выгружаем анимацию бунта
 set_riot_intensity(0)     -- сбрасываем интенсивность беспорядков
 for _, r in ipairs(rioters) do if isped(r) then remove_ped(r) end end  -- удаляем педов толпы
 if agit1 ~= nil and isped(agit1) then remove_ped(agit1) end  -- удаляем первого агитатора
 if agit2 ~= nil and isped(agit2) then remove_ped(agit2) end  -- удаляем второго агитатора
 if leader ~= nil and isped(leader) then remove_ped(leader) end  -- удаляем бригадира
 if truck1 ~= nil and isvehicle(truck1) then remove_car(truck1) end  -- удаляем грузовик 1
 if truck2 ~= nil and isvehicle(truck2) then remove_car(truck2) end  -- удаляем грузовик 2
 if truck3 ~= nil and isvehicle(truck3) then remove_car(truck3) end  -- удаляем грузовик 3
 if obj1 ~= nil then remove_obj(obj1) end  -- удаляем объект стройки 1
 if obj2 ~= nil then remove_obj(obj2) end  -- удаляем объект стройки 2
end  -- конец функции очистки

local function process_rioters_damage(rioters, bite_flag, dead_flag, hp_prev, bitten, killed, escalated, riot_intensity)  -- функция детектора урона
 for i, r in ipairs(rioters) do  -- перебираем пикетчиков
  if isped(r) then  -- если пед существует
   if not ispeddead(r) then  -- если пед жив
    local h = getpedhealthf(r)  -- считываем текущее здоровье
    if bite_flag[i] == 0 and escalated == 0 and h < hp_prev[i] then  -- если HP упало впервые
     bite_flag[i] = 1  -- фиксируем флаг укуса
     bitten = bitten + 1  -- инкрементируем укушенных
    end  -- конец проверки урона
    hp_prev[i] = h  -- сохраняем здоровье кадра
   else  -- если пед мертв
    if dead_flag[i] == 0 then  -- если смерть новая
     dead_flag[i] = 1  -- фиксируем смерть
     killed = killed + 1  -- увеличиваем счетчик убийств
     riot_intensity = riot_intensity - 6  -- снижаем интенсивность
     if riot_intensity < 0 then riot_intensity = 0 end  -- защита от отрицательных значений
     set_riot_intensity(riot_intensity)  -- применяем интенсивность
    end  -- конец фиксации смерти
   end  -- конец проверки жизни
  end  -- конец проверки педа
 end  -- конец цикла
 return bitten, killed, riot_intensity  -- возвращаем обновленные данные
end  -- конец функции process_rioters_damage

local function start_riot(pl, rioters, agit1, agit2, leader, guard_target)  -- функция эскалации бунта
 local guards = {}  -- таблица охраны
 if agit1  ~= nil then guards[#guards + 1] = agit1  end  -- добавляем агитатора 1
 if agit2  ~= nil then guards[#guards + 1] = agit2  end  -- добавляем агитатора 2
 if leader ~= nil then guards[#guards + 1] = leader end  -- добавляем лидера

 if not dressed_as_worker() then  -- если игрок без спецодежды
  for _, r in ipairs(rioters) do  -- цикл по толпе
   if isped(r) then
    clean_ped_wait(r)  -- снимаем ожидание
    set_ped_personality(r, 30)  -- агрессивная личность
    ped_search_threat(r, 16)    -- угроза прохожие
    ped_search_threat(r, 1)     -- угроза игрок
    set_char_obj_kill_player_on_foot(r, pl)  -- атаковать игрока
   end  -- конец настройки педа
  end  -- конец цикла толпы
  for _, g in ipairs(guards) do  -- цикл охраны
   if isped(g) then
    set_ped_personality(g, 30)  -- агрессия
    ped_search_threat(g, 1)     -- цель игрок
    if g == leader then set_ped_accuracy(leader, 10) end  -- меткость лидера
    set_char_obj_kill_player_on_foot(g, pl)  -- атаковать игрока
   end  -- конец настройки охранника
  end  -- конец цикла охраны
 else  -- если игрок в спецодежде
  local n = #rioters
  for i, r in ipairs(rioters) do  -- цикл по рабочим
   if isped(r) then
    clean_ped_wait(r)  -- снимаем ожидание
    set_ped_only_damaged_by_player(r, false)  -- разрешаем урон между ними
    set_ped_personality(r, 30)                -- агрессия
    ped_search_threat(r, 16)                  -- прохожие в цели
    local t = rioters[(i % n) + 1]            -- соседний рабочий
    if t ~= nil and isped(t) and t ~= r then
     set_char_obj_kill_char_on_foot(r, t)     -- драка рабочих друг с другом
    end  -- конец проверки цели
   end  -- конец настройки рабочего
  end  -- конец цикла рабочих
  for _, g in ipairs(guards) do  -- цикл охраны
   if isped(g) then
    set_ped_stay_when_attacked(g, false)      -- разрешаем двигаться
    set_ped_personality(g, 30)                -- агрессия
    ped_search_threat(g, 16)                  -- цели прохожие
    if g == leader then set_ped_accuracy(leader, 10) end  -- меткость лидера
    local gt = nil
    for _, r in ipairs(rioters) do if isped(r) and not ispeddead(r) then gt = r break end end  -- поиск мятежника
    if gt ~= nil then
     set_char_obj_kill_char_on_foot(g, gt)    -- атаковать мятежника
     guard_target[g] = gt                     -- сохранение цели
    end  -- конец проверки цели охранника
   end  -- конец настройки охранника
  end  -- конец цикла охраны
 end  -- конец ветвления одежды
end  -- конец функции start_riot

local function rebind_guard_targets(escalated, guard_target, rioters)  -- функция перепривязки охраны: потерянная цель → первый живой пикетчик
 if escalated == 1 and dressed_as_worker() then  -- перепривязка охраны
  for g, gt in pairs(guard_target) do  -- цикл по охранникам и их целям
   if isped(g) and not ispeddead(g) then  -- охранник жив
    if gt == nil or not isped(gt) or ispeddead(gt) then  -- цель потеряна (нет, удалена или убита)
     local ng = nil  -- новая цель
     for _, r in ipairs(rioters) do if isped(r) and not ispeddead(r) then ng = r break end end  -- ищем первого живого пикетчика
     if ng ~= nil then  -- нашли живого
      set_char_obj_kill_char_on_foot(g, ng)  -- приказываем атаковать цель
      guard_target[g] = ng  -- запоминаем новую цель
     end  -- конец новой цели
    end  -- конец мертвой цели
   end  -- конец живого охранника
  end  -- конец цикла
 end  -- конец перепривязки
end  -- конец функции rebind_guard_targets

local function stop_riot_when_many_killed(killed)  -- функция гашения бунта, когда пикетчиков убито много
 if killed > 14 then  -- если убито много
  set_riot_intensity(0)  -- сброс интенсивности
 end  -- конец проверки
end  -- конец функции stop_riot_when_many_killed

local function drop_agitator_colt(agit, dropped)  -- функция выпадения оружия
 if dropped == 0 and agit ~= nil and isped(agit) and ispeddead(agit) then  -- если босс мертв и колт не падал
  local dx, dy, dz = get_dead_char_pickup_coords(agit)  -- получаем координаты
  if dx ~= nil then
   create_weapon_pickup(274, 3, 17, dx, dy, dz + 0.5)  -- создаем пикап
   return 1  -- возвращаем флаг выпадения
  end  -- конец проверки координат
 end  -- конец условий
 return dropped  -- возвращаем старый флаг
end  -- конец функции drop_agitator_colt

local function dress_cutscene(pp, dress_blip, dress_sphere)  -- катсцена переодевания
 remove_blip(dress_blip)
 remove_sphere(dress_sphere)
 ped_frozen(0)  -- размораживаем управление
 set_widescreen(true)  -- включаем широкие полосы
 set_camera_position(91.813, -1131.12, 17.251, 0.0, 0.0, 0.0)  -- позиция камеры
 camera_at_point(92.39, -1131.74, 16.724, 2)  -- направление камеры
 set_char_obj_goto_coord_on_foot(pp, 96.4, -1136.7, 9.4)  -- Томми идет к двери
 wait(10)  -- ждем движения
 fade(0, 50)  -- затемнение экрана
 wait(5)  -- ждем затемнения
 set_skin(pp, "PLAYER3")  -- меняем скин
 setcord(pp, 94.9, -1135.0, 9.4)  -- телепорт на выход
 setangle(pp, 57.9)  -- угол поворота
 fade(1, 100)  -- проявление экрана
-- wait(100)  -- ждем проявления
 restore_camera()  -- возвращаем камеру
 ped_frozen(1)  -- заморозка управления
 set_widescreen(false)  -- отключаем широкие полосы
 if dress_blip ~= nil then dress_blip = nil end  -- удаляем блайп
 if dress_sphere ~= nil then dress_sphere = nil end  -- удаляем сферу
 printmessage("~g~You look like a worker. ~y~Now hit the construction site!", 5000, 1)  -- сообщение
 return dress_blip, dress_sphere  -- возвращаем значения
end  -- конец функции катсцены

local function spawn_rioters(rioters, bite_flag, dead_flag, hp_prev)  -- спавн пикетчиков
 for i, p in ipairs({  -- цикл по точкам
  {304.7,-316.8},{302.8,-318.8},{301.6,-316.7},{299.8,-318.2},
  {300.9,-321.6},{299.0,-322.1},{295.0,-320.7},{293.5,-317.8},
  {296.2,-325.3},{299.3,-329.3},{303.8,-329.7},{308.5,-328.4},
  {310.2,-324.2},{305.6,-322.2},{300.0,-317.5},{294.9,-317.6},
  {291.0,-320.0},{293.0,-324.8},{291.6,-317.2},{301.1,-324.0},
  {297.6,-327.649}
 }) do  -- конец списка координат
  local skin = (i % 2 == 1) and "SPANDXA" or "SPANDXB"  -- чередование скинов
  local r = Createped(skin, p[1], p[2], 10.9)  -- создание педа
  if r ~= nil then
   ped_clean_threat(r)  -- очистка угроз
   set_ped_personality(r, 16)  -- тип личности
   set_ped_only_damaged_by_player(r, true)  -- урон только от игрока
   set_ped_wait_state(r, 29, 99999999)  -- состояние ожидания
   table.insert(rioters, r)  -- добавление в таблицу
   bite_flag[#rioters] = 0   -- флаг укуса
   dead_flag[#rioters] = 0   -- флаг смерти
   hp_prev[#rioters] = getpedhealthf(r)  -- сохранение HP
  end  -- конец проверки создания
 end  -- конец цикла спавна
end  -- конец функции spawn_rioters

local function spawn_riot_scene(player, rioters, bite_flag, dead_flag, hp_prev, riot_intensity)  -- спавн сцены бунта
 -- Giveweaponped(player, 6, "molotov")  -- выдаем молотовы
 -- set_current_weapon_ped(player, 15)  -- берем молотов в руки
 set_riot_intensity(riot_intensity)  -- ставим интенсивность бунта

 local truck1, truck2, truck3, mt1, mt2, mt3 = spawn_trucks()  -- спавн машин и маркеров
 local obj1 = create_obj(-114, 309.3, -298.6, 10.9) if obj1 ~= nil then make_obj_targettable(obj1, true) end  -- объект 1
 local obj2 = create_obj(-114, 284.3, -312.4, 10.9) if obj2 ~= nil then make_obj_targettable(obj2, true) end  -- объект 2
 local gate = get_obj_in_cord(298.0, -313.6, 11.0, 8.0)  -- поиск ворот

 spawn_rioters(rioters, bite_flag, dead_flag, hp_prev)  -- спавн толпы

 local agit1 = Createped("GDA", 301.2, -311.5, 10.9) if agit1 ~= nil then setangle(agit1, 180) end  -- агитатор 1
 local agit2 = Createped("GDA", 303.2, -311.5, 10.9) if agit2 ~= nil then setangle(agit2, 180) end  -- агитатор 2
 local leader = Createped("GDA", 304.0, -298.7, 10.9) if leader ~= nil then setangle(leader, 180) end  -- бригадир
 if agit1 ~= nil then Giveweaponped(agit1, 0, "nitestick") set_current_weapon_ped(agit1, 4) end  -- дубинка 1
 if agit2 ~= nil then Giveweaponped(agit2, 0, "nitestick") set_current_weapon_ped(agit2, 4) end  -- дубинка 2
 if leader ~= nil then
  sethealth(leader, 70)  -- здоровье бригадира
  Giveweaponped(leader, 300, "colt45")  -- кольт бригадиру
  set_current_weapon_ped(leader, 17)   -- в руки
  set_ped_stay_when_attacked(leader, true)  -- удерживать позицию
 end  -- конец настройки бригадира
 if agit1 ~= nil then set_ped_only_damaged_by_player(agit1, true) end  -- урон агитатору 1
 if agit2 ~= nil then set_ped_only_damaged_by_player(agit2, true) end  -- урон агитатору 2
 if leader ~= nil then set_ped_only_damaged_by_player(leader, true) end  -- урон бригадиру

 return truck1, truck2, truck3, mt1, mt2, mt3, obj1, obj2, gate, agit1, agit2, leader  -- возврат сущностей
end  -- конец функции spawn_riot_scene

function main()  -- главная функция миссии
while true do wait() local player = findplayer()  -- бесконечный цикл

 if Star_mission_marker(9, 118.0, -825.9, 10.5)  -- старт миссии у маркера Кена
 then
  -- ped_frozen(1)  -- заморозка
  -- fade(1, 1200)  -- проявление
  -- wait(10)     -- ожидание

  local step = 1          -- этап 1
  local bitten = 0        -- укушенные
  local killed = 0        -- убитые
  local escalated = 0     -- эскалация
  local riot_intensity = 127  -- интенсивность
  local gates_open = 0    -- ворота закрыты
  local all_dead = false  -- все грузовики уничтожены
  local law4_13_shown = 0  -- сообщение не показано
  local agit1_drop = 0    -- колт 1
  local agit2_drop = 0    -- колт 2

  local rioters = {}      -- таблица толпы
  local bite_flag = {}    -- флаги укуса
  local dead_flag = {}    -- флаги смерти
  local hp_prev = {}      -- здоровье
  local truck1, truck2, truck3  -- грузовики
  local mt1, mt2, mt3           -- маркеры
  local obj1, obj2              -- объекты
  local agit1, agit2, leader    -- боссы
  local gate                    -- ворота

  set_wanted(0)  -- розыск 0
  showtext(" Riot", 500, 1)  -- название

  printmessage("~w~Ken: Tommy! The workers at Avery's site have gone on the rampage!", 5000, 1)  -- диалог
  wait(20)
  printmessage("~w~Ken: Get some work clothes from Rafael's so they don't recognize you!", 6000, 1)  -- диалог
  wait(20)

  local dress_blip = radar_add_sprite_blip_for_coord(95.4, -1135.5, 9.4, 28)  -- блайп магазина
  local dress_sphere = create_sphere(95.4, -1135.5, 9.4, 3.0)  -- сфера магазина
  local site_blip = radar_add_blip_for_coord(299.0, -314.6, 11.4)  -- блайп стройки
  radar_change_blip_colour(site_blip, 4)  -- цвет блайпа

  set_ped_mood(1, 60000)  -- настроение
  request_animation("RIOT")  -- анимация
  printmessage("~g~Get the work outfit at Rafael's, then hit the construction site.", 6000, 1)  -- задача

  local guard_target = {}  -- цели охраны

  while true == Getflagmission() do wait()  -- цикл миссии
   local pp = findplayer()  -- игрок
   if pp == nil then pp = player end  -- страховка

   if (step == 1 or step == 2 or step == 3) and not dressed_as_worker() and not is_ped_in_car(pp)
   and ped_in_point_in_radius(pp, 95.4, -1135.5, 9.4, 1.2, 1.2, 3.0) then  -- переодевание
    dress_blip, dress_sphere = dress_cutscene(pp, dress_blip, dress_sphere)  -- катсцена
   end  -- конец проверки магазина

   if step == 1 and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 70.0, 70.0, 10.0) then  -- подъезд к стройке
    truck1, truck2, truck3, mt1, mt2, mt3, obj1, obj2, gate, agit1, agit2, leader = spawn_riot_scene(player, rioters, bite_flag, dead_flag, hp_prev, riot_intensity)  -- спавн сцены
    if site_blip ~= nil then remove_blip(site_blip) site_blip = nil end  -- удаление блайпа
    if dressed_as_worker() then
     if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end
     if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end
    end  -- конец чистки магазина
    printmessage("~g~Destroy the workers' trucks to break the strike!", 6000, 1)  -- задача
    printmessage("~r~Workers: This is our site! Get out!", 5000, 1)  -- реплика
    printmessage("~r~Workers: Burn everything! Strike forever!", 5000, 1)  -- реплика
    step = 2  -- шаг 2
   end  -- конец шага 1

   if step == 2 and law4_13_shown == 0
    and ped_in_point_in_radius(pp, 299.0, -314.6, 11.4, 20.0, 20.0, 10.0) then  -- близко к стройке
    law4_13_shown = 1  -- флаг показа
    printmessage("~r~Destroy the trucks! Now!", 8000, 1)  -- сообщение
   end  -- конец брифинга

   if step == 2 or step == 3 then  -- проверка урона
    bitten, killed, riot_intensity = process_rioters_damage(rioters, bite_flag, dead_flag, hp_prev, bitten, killed, escalated, riot_intensity)  -- обработка урона

    if escalated == 0 and (bitten > 0 or killed > 0) then  -- запуск бунта
     start_riot(pp, rioters, agit1, agit2, leader, guard_target)  -- эскалация
     escalated = 1  -- флаг драки
     step = 3  -- шаг 3
    end  -- конец запуска
   end  -- конец шагов 2 и 3

   if step == 2 or step == 3 then  -- грузовики: уничтоженный теряет маркер сразу, ещё до эскалации
    local truck_state = {  -- состояние машин и маркеров для функции
     trucks = {truck1, truck2, truck3},
     markers = {mt1, mt2, mt3}
    }  -- конец таблицы

    all_dead = update_trucks_and_check_win(truck_state)  -- снимаем маркеры уничтоженных грузовиков

    mt1, mt2, mt3 = truck_state.markers[1], truck_state.markers[2], truck_state.markers[3]  -- синхронизация маркеров обратно в переменные
   end  -- конец проверки грузовиков

   if step == 3 then  -- этап боя
    if escalated == 1 and gates_open == 0 then  -- открытие ворот
     gates_open = 1  -- флаг ворот
     if gate ~= nil then
      move_obj_to(gate, 304.0, -313.6, 11.0, 3.0)  -- плавно открываем ворота: из 298 в 304 по X, скорость 3 ед/сек
     end  -- конец ворот
     printmessage("~r~Lawyers and cops are on their way! Get the trucks!", 8000, 1)  -- сообщение
    end  -- конец ворот

    rebind_guard_targets(escalated, guard_target, rioters)  -- перепривязка охраны: потерянная цель → первый живой пикетчик

    stop_riot_when_many_killed(killed)  -- если убито много: сброс интенсивности

    agit1_drop = drop_agitator_colt(agit1, agit1_drop)  -- колт 1
    agit2_drop = drop_agitator_colt(agit2, agit2_drop)  -- колт 2

    if all_dead then  -- проверка уничтожения всех грузовиков
     printmessage("~g~The strike is broken! Avery's site is secure.", 4000, 1)  -- успех
     wait(200)  -- задержка
     cleanup_riot_scene(rioters, agit1, agit2, leader, truck1, truck2, truck3, obj1, obj2)  -- чистка сцены
     miss(1000)  -- победа миссии
     step = nil  -- сброс шага
     break  -- выход из цикла
    end  -- конец победы
   end  -- конец шага 3

  end  -- конец цикла Getflagmission

  if dress_blip ~= nil then remove_blip(dress_blip) dress_blip = nil end  -- чистка блайпа одежды
  if dress_sphere ~= nil then remove_sphere(dress_sphere) dress_sphere = nil end  -- чистка сферы
  if site_blip ~= nil then remove_blip(site_blip) site_blip = nil end  -- чистка блайпа стройки
  if mt1 ~= nil then remove_blip(mt1) mt1 = nil end  -- чистка маркера 1
  if mt2 ~= nil then remove_blip(mt2) mt2 = nil end  -- чистка маркера 2
  if mt3 ~= nil then remove_blip(mt3) mt3 = nil end  -- чистка маркера 3
  if #rioters > 0 then set_riot_intensity(0) end  -- сброс интенсивности
  if gates_open == 1 and gate ~= nil then
   move_obj_to(gate, 298.0, -313.6, 11.0, 3.0)  -- плавно закрываем ворота: из 304 в 298 по X, скорость 3 ед/сек
  end  -- конец закрытия ворот
  step = nil  -- сброс шага
 end  -- конец маркера миссии
end  -- конец главного цикла
end  -- конец функции main
