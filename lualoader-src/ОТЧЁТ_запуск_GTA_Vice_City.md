# Отчёт: запуск GTA Vice City в Linux через Wine — диагностика, проблемы и решения

Дата: 22 ноября 2026 г.
Путь игры: `/mnt/807EB5FA7EB5E954/games/old GTA Vice City`
Операционная система: Linux, пользователь `egor`, графический сеанс `DISPLAY=:0` (Cinnamon/X11).

---

## 1. Краткое резюме

Игра **рабочая**: её ассеты (`models/gta3.img`, `data`, `Audio`, `anim`, `movies`, `text`) на месте и уже запускались в минимальной тестовой копии. Запуск шёл через обычный (PortProton `DEFAULT`) префикс Wine.

За всё время выявились **три независимые проблемы**:

| # | Проблема | Симптом | Решение |
|---|----------|---------|---------|
| 1 | `config.cfg` содержал несуществующий путь `D:\Program Files\Rockstar Games\Grand Theft Auto Vice City\` | Игра требовала «вставить диск», хотя No-CD раньше стоял всегда | Заменить путь на реальный `Z:\mnt\807EB5FA7EB5E954\games\old GTA Vice City\` |
| 2 | Пары папок, отличающиеся только регистром: `models`/`Models`, `anim`/`Anim`, `movies`/`Movies`, `mp3`/`Mp3`, `text`/`TEXT` | Процесс `gta-vc.exe` живой, но **окно не появляется вообще**, ошибок нет | Содержимое заглавных папок перенесено в строчные, пустые папки удалены |
| 3 | Конфликт ASI-плагинов | После устранения пп. 1–2: `Unhandled exception code c0000409`, игра падает | Точный плагин не найден (диагностика прервана); пользователь очистил `Scripts/` вручную |

**Важно про текущее состояние:** на момент написания отчёта `config.cfg` в папке **отсутствует** (похоже, удалён в ходе экспериментов), а папка `Scripts/` **пуста** — все плагины оттуда удалены пользователем. Чтобы игра запускалась без запроса диска, `config.cfg` нужно создать заново (см. раздел 7).

---

## 2. Как именно запускается игра

### 2.1. Основной способ — `gta-vc.sh`

Файл: `/mnt/807EB5FA7EB5E954/games/old GTA Vice City/gta-vc.sh`

Ключевое содержимое (сокращено, строки с `export` оставляем как есть):

```bash
#!/bin/bash
export LAUNCH_PARAMETERS="-dx11 -skipintro 1"
export WINDOWS_VER="10"
export DLL_INSTALL="vcrun2019 corefonts lucida"
export WINEDLLOVERRIDES="d3dx9_36,d3dx9_42=n,b;d3dx9_43=b;mfc120=b,n,d3d8,d3d9,ddraw,dinput8,dsound=n,b"
export VULKAN_USE="6"
export LC_ALL="ru_RU.UTF-8"
export LANG="ru_RU.UTF-8"
export VKD3D_FEATURE_LEVEL="12_2"
export DXVK_CONFIG="dxvk.enablePresentTiming = False"   # в версии скрипта для reVC
export AMD_VULKAN_USE="radv"
export MESA_VK_WSI_PRESENT_MODE="mailbox"
export SOUND_DRIVER_USE="pulse"
export FPS_LIMIT="90"
# ... переключение раскладки через xset/xdotool ...
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
wine "gta-vc.exe" -skipintro 1
```

Фактическая команда запуска:

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
WINEPREFIX=/home/egor/PortProton/data/prefixes/DEFAULT wine "gta-vc.exe" -skipintro 1
```

### 2.2. Окружение

- Wine: `wine-11.22` (`/usr/local/bin/wine`, библиотеки `/opt/wine-11.22`).
- Префикс: `/home/egor/PortProton/data/prefixes/DEFAULT` — **обычный, « PortProton-овский»**.
- Графика: DXVK **v3.1.1** + Vulkan RADV **26.2.6** (AMD Radeon 780M, `VK_PRESENT_MODE_IMMEDIATE_KHR`, swapchain `1920x1080`, 4 image).
- Файл `dxvk.conf` в корне: `dxvk.enablePresentTiming = False` (обход ошибки `vkGetSwapchainTimeDomainPropertiesEXT` в winevulkan).
- `d3d8.log`: `Redirecting 'Direct3DCreate8(120)' ... > Passing on to 'Direct3DCreate9':` — локальный `d3d8.dll` перенаправляет Direct3D 8 на Direct3D 9.
- Важно: проверено, что **префикс не является причиной проблемы**. Чистая минимальная копия игры запускалась и через свежий префикс, и через обычный `DEFAULT`.

Пример вывода при удачном запуске (из лога после исправления `config.cfg`):

```text
info:  Game: gta-vc.exe
info:  DXVK: v3.1.1
info:  Found config file: dxvk.conf
info:    dxvk.enablePresentTiming = False
info:  Found device: AMD Radeon 780M Graphics (RADV PHOENIX) (radv 26.2.6)
info:  Presenter: Actual swapchain properties:
info:    Format: VK_FORMAT_B8G8R8A8_UNORM
info:    Present mode: VK_PRESENT_MODE_IMMEDIATE_KHR
info:    Buffer size: 1920x1080
```

### 2.3. Другие способы запуска в папке

| Файл | Что делает | Статус |
|------|-----------|--------|
| `gta-vc.sh` | Основной: `wine gta-vc.exe -skipintro 1` | Рабочий при исправленном `config.cfg` |
| `gta-vc` | `.desktop`-запись, `Exec=xdg-open ".../gta-vc.exe"` | Косвенный запуск через ассоциацию файла |
| `script_steam_gta-vc.sh` | `env /home/egor/PortProton/data/scripts/start.sh gta-vc.exe` | Через PortProton |
| `Запуск GTA Vice City в оконном режиме.sh` | `gamescope -W 1280 -H 720 -w 952 -h 648 -r 60 -F linear -- wine gta-vc.exe` | Оконный режим |
| `reVC.sh` (был) | `wine reVC.exe` | **Был сломан**: `reVC.exe` в корне нет, он лежит в `Новая папка/reVC_Release_win-x86-librw_d3d9-mss/reVC.exe`. Пользователем удалён |

---

## 3. Текущая структура папки игры

### 3.1. Верхний уровень (по состоянию на 21:06)

Всего: **~9,5 ГБ**.

**Исполняемые файлы и библиотеки:**

| Имя | Размер/описание |
|-----|-----------------|
| `gta-vc.exe` | 3 088 896 байт, PE32, основной бинарник ( DLL: `d3d8.dll`, `ddraw.dll`, `dinput8.dll`, `mss32.dll`, `winmm.dll` и др.) |
| `txd/gta-vc.exe` | Вторая копия exe, 3 088 896 байт; отличается от корневой 73 байтами, в том числе 29 — в теле файла (таблица `FED_TRA`/`FET_DEF`) — это разные сборки, **не No-CD-патч** |
| `vc-mp.exe` | 1 037 824 байт, VC:MP (мультиплеер) |
| `Mss32.dll` | 338 432 байт, звуковая библиоте Miles (в Wine регистр имени не важен) |
| `d3d8.dll` (1,7 МБ), `ddraw.dll` (115 КБ), `dinput8.dll` (5,4 МБ) | Обёртки/загрузчики|
| `libmpg123-0.dll`, `OpenAL32.dll`, `rwd3d9.dll`, `GameuxInstallHelper.dll` | Доп. библиотеки |
| `gta-vc.dxvk-cache`, `d3d8.log`, `cleo.log` | Кэш и логи |

**Игровые данные (целы):**

| Папка | Размер | Файлов | Комментарий |
|-------|--------|--------|-------------|
| `models/` | 622 МБ | 56 | `gta3.img` (327 487 488 байт), `gta3.dir`, `gtaplayer.img`, TXD, `coll/`, `generic/` |
| `Audio/` | 2,5 ГБ | 1228 | Музыка и звуки |
| `data/` | 15 МБ | 157 | `gta_vc.dat`, `default.dat`, `main.scm`, `handling.cfg`, `maps/`, `paths/` |
| `anim/` | 114 МБ | 10 | `ped.ifp`, `cuts.img` (115 087 360 байт), `anims1.ifp` |
| `movies/` | 144 МБ | 2 | `GTAtitles.mpg` (128 073 728 байт), `Logo.mpg` |
| `mp3/` | 300 МБ | 31 | 30 треков + `MP3Report.txt` |
| `mss/`, `text/`, `txd/`, `Icons/`, `icons_for_wine/` | 1,5 МБ / 4,7 МБ / 13 МБ / 2,8 МБ / 1,8 МБ | | `text/american.gxt` (421 910 байт), `german.gxt` |

**Моды и плагины:**

| Папка/файл | Состояние |
|-----------|-----------|
| `Scripts/` | **Сейчас пуста** (0 файлов). Ранее там было 18 `.asi` + `.ini` |
| Корневые `.asi` (5 шт.) | `GTAVC.WidescreenFix.asi`, `lualoader.asi`, `portablegta.asi`, `VC.CLEO.asi`, `_noDEP.asi` |
| `modloader/` | 999 МБ, 3046 файлов (моды). Без `modloader.asi` не загружается |
| `Cleo/` (1,2 МБ, 15 файлов), `lualoader/` (208 КБ), `skins/` (91 МБ), `neo/`, `userfiles/` | на месте |
| `Новая папка/` | 1,1 ГБ, 3974 файла — архив модов/исходников/сборок reVC |

**Прочее:**

- Архивы `Audio.tar` (2,67 ГБ) и `gta-vc.tar` (184 МБ) — распакованные копии тех же данных, для запуска не нужны.
- `gta vc база/` (161 МБ) — IDA-базы и исходники reVC (`re3-miami.zip`, `librw.rar`), **не игровые ассеты**.
- `INSTALL.LOG`, `unins000.*`–`unins002.*`, `UnWise.ini`, `steam_appid.txt` (`12110`), `ReadMe/`, `README.md` (описание LUALOADER), скрипты и тексты.
- Временный файл `.fuse_hidden0007c24100000a9d` — след работы FUSE/архиватора, можно удалить.
- **Отсутствует `config.cfg`** (см. раздел 7).

Символьных ссылок в папке сейчас **0** (две самоссылающиеся ссылки из тестов удалены).

---

## 4. Проблемы и их решение — подробно

### 4.1. Запрос диска из-за `config.cfg`

**Симптом.** Игра при запуске требовала вставить диск GTA Vice City, хотя пользователь всегда использовал No-CD.

**Причина.** Файл `config.cfg` в корне состоял из одной строки:

```text
D:\Program Files\Rockstar Games\Grand Theft Auto Vice City\
```

Это путь исходной установки Windows. В Wine буква `D:` в префиксе `DEFAULT` смонтирована совсем в другое место:

```bash
$ ls -l /home/egor/PortProton/data/prefixes/DEFAULT/dosdevices/d:
d: -> /home/egor/Документы
$ ls "/home/egor/Документы/Program Files/Rockstar Games/Grand Theft Auto Vice City"
Нет такого файла или каталога
```

GTA Vice City читает `config.cfg`, чтобы узнать каталог установки, и ищет там `gta3.img` / `data`. Не находит — показывает приглашение вставить диск. Это **не проблема самого No-CD-патча**: с правильным путём проверка проходит.

**Решение.** Путь заменён на Windows-путь Wine, где `/` — это `Z:`:

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
printf 'Z:\\mnt\\807EB5FA7EB5E954\\games\\old GTA Vice City\\\n' > config.cfg
cat config.cfg
# Z:\mnt\807EB5FA7EB5E954\games\old GTA Vice City\
```

Старая версия сохранена: `/home/egor/config.cfg.bak_nocd` (и более ранняя — `/home/egor/config.cfg.orig`).

**Проверка.** С исправленным `config.cfg` и временно отключёнными плагинами:

```text
$ pgrep -a -x gta-vc.exe
319093 Z:\mnt\807EB5FA7EB5E954\games\old GTA Vice City\gta-vc.exe -skipintro 1
$ wmctrl -lp | grep -i vice
0x08a0000b  0 319093 egor-PC GTA: Vice City
```

Окно появилось, **запроса диска не было**. Контрольный опыт с путём `D:\...` в чистой копии также показывал окно — но в рабочей папке именно возврат к правильному пути убрал запрос диска. (Скриншот того запуска получился чёрным — отдельный вопрос отрисовки/захвата, на сам запуск он не влияет.)

**Текущее состояние.** `config.cfg` снова отсутствует — нужно воссоздать (раздел 7).

---

### 4.2. Дубликаты папок по регистру → зависание без окна

**Симптом.** Процесс есть, окна нет, ошибок в логе нет:

```text
$ ps -p 2439358 -o pid,stat,etime,cmd
2439358 Sl  01:47 Z:\...\gta-vc.exe -skipintro 1
$ wmctrl -l | grep -i gta
(пусто)
```

Процесс висит в `S (sleeping)`, 3 потока, 60 секунд ожидания ничего не меняли. Это **не crash**: игра сама не вылетала, её приходилось завершать вручную.

**Причина.** В папке одновременно лежали пары каталогов, отличающихся только регистром:

| Строчная | Заглавная | Строчная: файлов/размер | Заглавная: файлов/размер | Совпадения имён |
|----------|-----------|------------------------|--------------------------|-----------------|
| `anim` | `Anim` | 10 / 114 МБ | 3 / 112 МБ | `cuts.dir`, `cuts.img`, `ped.ifp` |
| `models` | `Models` | 56 / 622 МБ | 4 / 0,8 МБ | `fonts.txd`, `ps3btns.txd`, `sixaxis.txd`, `x360btns.txd` |
| `movies` | `Movies` | 2 / 30 МБ | 2 / 144 МБ | `GTAtitles.mpg`, `Logo.mpg` |
| `mp3` | `Mp3` | 1 / 0 МБ | 30 / 300 МБ | нет |
| `text` | `TEXT` | 2 / 0,8 МБ | 7 / 4,6 МБ | `american.gxt`, `german.gxt` |

Wine на чувствительной к регистру ФС ведёт себя непредсказуемо, когда есть и `models`, и `Models`. Проверка через `wine cmd` показывала:

```text
models\gta3.img   => gta3.img
MODELS\gta3.img   => (пусто)
Models\gta3.img   => (пусто)
```

То есть обращение к «не тому» регистру не находится, а перебор вариантов написания имени при поиске файлов уводит игру в зависание до создания окна.

**Доказательство.** Тесты с минимальной копией (только `gta-vc.exe`, `Mss32.dll`, `d3d8.dll`, `ddraw.dll` + папки данных):

- минимальная копия → окно `GTA: Vice City` есть;
- та же копия + папка `Models` → окна нет;
- та же копия + все «заглавные» папки → окна нет;
- исходная папка с временно переименованной `Models` → игра пошла намного дальше: DXVK создал swapchain 1920×1080 (зависание ушло).

**Решение (выполнено по вашей инструкции: содержимое заглавных папок → в строчные, пустые удалить):**

- Перенесено новых файлов: **35**.
- Перезаписано существующих файлов (версия из заглавной папки победила, как вы выбрали): **11**:

```text
anim/cuts.dir, anim/cuts.img, anim/ped.ifp
models/fonts.txd, models/ps3btns.txd, models/sixaxis.txd, models/x360btns.txd
movies/GTAtitles.mpg, movies/Logo.mpg
text/american.gxt, text/german.gxt
```

- Удалены пустые папки: `Anim`, `Models`, `Movies`, `Mp3`, `TEXT`.
- Страховка перезаписанных файлов: `/home/egor/gta_merge_backup_20261122_203313/` (подпапки `Anim/`, `Models/`, `Movies/`, `TEXT/`).
- Заодно удалены две самоссылающиеся символьные ссылки `Movies/Movies -> .../Movies` и `Mp3/Mp3 -> .../Mp3`, появившиеся как артефакт диагностики.

Контроль после переноса:

```text
models/gta3.img      327487488 байт
models/gta3.dir      193376 байт
anim/cuts.img        115087360 байт
movies/GTAtitles.mpg 128073728 байт
text/american.gxt    421910 байт
mp3/*.mp3            30 файлов
```

---

### 4.3. Конфликт ASI-плагинов → `Unhandled exception c0000409`

**Симптом.** После устранения пп. 4.1 и 4.2 игра стала падать почти сразу:

```text
$ grep -i "Unhandled exception" /home/egor/gta_after_merge.log
06a4:err:seh:NtRaiseException Unhandled exception code c0000409 flags 1 addr 0x74471070
$ pgrep -x gta-vc.exe
(нет)
```

`c0000409` — это `STATUS_STACK_BUFFER_OVERRUN`/fast-fail, типичный для несовместимого плагина.

**Проверено:**

- Со **всеми** отключёнными `.asi` (64 файла) игра запускается: процесс жив, окно `GTA: Vice City` появляется.
- С включённой первой половиной активных плагинов — падение.
- С включённой второй половиной (`III.VC.SA.WindowedMode`, `LCSCarCam`, `MenuMapVC`, `modloader.asi`, `mouse_wheel_radio_changer`, `MssEax`) — **падения нет**.
- Сужено до четвёрки: `GTAVC.WidescreenFix.asi` (корень), `lualoader.asi` (корень), `Scripts/GTAVC.WidescreenFix.asi`, `Scripts/III.VC.SA.LimitAdjuster.asi`. Дальнейший бисект был прерван.
- Ранее в изолированных тестах отдельно воспроизводилось зависание от пары `sharptrails.asi` + `skygfx.asi`, но results были нестабильны на фоне одновременно запущенных копий.

**Текущее состояние.** Пользователь вручную очистил `Scripts/` — теперь там **0 файлов**, а в корне осталось 5 `.asi`. Поэтому большинство конфликтующих плагинов сейчас отключены.

**Рекомендация.** Возвращать плагины по одному, запуская игру после каждого. Первыми проверять не четвёрку-кандидатов, а начинать с чистого `Scripts/`, затем добавлять по одному и фиксировать результат.

---

### 4.4. Прочие найденные неполадки

1. **`reVC.sh` был сломан.** Команда `wine reVC.exe`, но в корне `reVC.exe` отсутствует; файл лежит в `Новая папка/reVC_Release_win-x86-librw_d3d9-mss/reVC.exe` (3 330 560 байт). Скрипт удалён пользователем.
2. **Потерянный `VC.CLEO.asi`.** После одного прерванного теста корневой `VC.CLEO.asi` пропал; восстановлен копией из `Cleo/VC.CLEO.asi` (287 232 байт).
3. **Слишком много тестовых копий.** В ходе диагностики было запущено несколько экземпляров игры; все диагностические процессы закрыты, временные каталоги остались в `/home/egor/` (см. раздел 6).
4. **Два `gta-vc.exe`.** Различия между ними — в заголовке (44 байта) и в$data-таблице по адресу `0x2d69a4–0x2d69d8` (у корневой есть запись `FED_TRA`, у `txd/` — нули). Это разные сборки, а не «No CD» vs «с диском».
5. **Архивы No-CD.** В файловом менеджере были открыты `gta.com.ua-VC_no-CD.rar` и `Grand.Theft.Auto.Vice.City.1.x.update.Fixe-FFF.rar` (только для чтения) — архивы со взломанными версиями, но сейчас игра запускается и без патча при правильном `config.cfg`.

---

## 5. Хронология диагностики (с примерами)

1. **Осмотр папки.** `ls -la` — 70+ записей, `.sh`-скрипты, Wine-префикс, моды, архивы. Прочитаны `gta-vc.sh`, `reVC.sh`, `config.cfg`, `README.md`, `dxvk.conf`.
2. **Первый запуск через `wine gta-vc.exe`:** процесс живой, окна нет, в логе только `fixme`/`err:wineboot` — не crash.
3. **Проверка целостности данных:** `models/gta3.img`, `data/main.scm`, `Audio/`, `mss/Mss32.dll` — на месте. `binkw32.dll` не требуется (в импорте exe его нет).
4. **Минимальная копия** (`/home/egor/gta_clean_test`): те же DLL + папки данных → окно `GTA: Vice City` появилось. Вывод: сама игра и Wine исправны.
5. **Тот же тест через обычный префикс `DEFAULT`** (копия `gta-vc.sh` с заменённым `cd`) → окно появилось. Вывод: префикс не причина.
6. **Добавление «лишних» файлов по категориям:**
   - все лишние файлы корня → запускается;
   - все лишние папки → не запускается;
   - только `Models` → не запускается. Причина локализована.
7. **Проверка `config.cfg`:** путь `D:\...` не существует; замена на `Z:\...` убирает запрос диска.
8. **Объединение папок** (35 новых, 11 замен, 5 пустых удалены; бэкап сохранён).
9. **Повторный запуск:** запроса диска нет, зависания нет, но появляется `c0000409` — переход к поиску конфликтующего плагина.
10. **Бисект ASI:** все выключены → окно есть; первая половина → падение; вторая → окно есть; сужено до 4 файлов; прервано пользователем.

---

## 6. Что изменено и где бэкапы

### Изменения в папке игры

- Удалены пустые папки `Anim`, `Models`, `Movies`, `Mp3`, `TEXT`; их содержимое перенесено в `anim`, `models`, `movies`, `mp3`, `text`.
- Заменено 11 одноимённых файлов версиями из заглавных папок.
- Удалены 2 самоссылающиеся символьные ссылки.
- Восстановлен `VC.CLEO.asi`.
- `config.cfg` был исправлен, но **сейчас отсутствует** (нужно создать).
- `Scripts/` очищена пользователем.

### Резервные копии

| Путь | Что содержит |
|------|--------------|
| `/home/egor/gta_merge_backup_20261122_203313/` | 11 перезаписанных файлов |
| `/home/egor/config.cfg.bak_nocd` | Старый `config.cfg` с путём `D:\...` |
| `/home/egor/config.cfg.orig` | Ещё одна копия старого `config.cfg` |

### Временные тестовые каталоги (можно удалить)

```text
/home/egor/gta_clean_test        /home/egor/gta_full_link
/home/egor/gta_set_A             /home/egor/gta_set_B
/home/egor/gta_sc_full           /home/egor/gta_sc_nosharp
/home/egor/gta_sc_nosky          /home/egor/gta_bx_noScripts
/home/egor/gta_bx_noModloader    /home/egor/gta_dup_test
/home/egor/gta_models_test       /home/egor/gta_wine_prefix (чистый префикс)
/home/egor/gta_stash             (пуст)
```

Логи тестов: `/home/egor/gta_*.log`, скриншоты: `/home/egor/gta_*.png`.

---

## 7. Текущее состояние и что делать дальше

### 7.1. Шаг 1 — восстановить `config.cfg` (обязательно)

Сейчас файла нет, поэтому запрос диска вернётся. Создайте:

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
printf 'Z:\\mnt\\807EB5FA7EB5E954\\games\\old GTA Vice City\\\n' > config.cfg
cat config.cfg
# Ожидаемо: Z:\mnt\807EB5FA7EB5E954\games\old GTA Vice City\
```

### 7.2. Шаг 2 — запуск

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
./gta-vc.sh
```

Или вручную:

```bash
cd "/mnt/807EB5FA7EB5E954/games/old GTA Vice City"
WINEPREFIX=/home/egor/PortProton/data/prefixes/DEFAULT wine "gta-vc.exe" -skipintro 1
```

### 7.3. Шаг 3 — если снова будет `c0000409`

Плагины добавлять **по одному**, проверяя запуск после каждого. Сейчас в корне осталось 5 `.asi`, `Scripts/` пуста. Начинайте с чистого состояния (без всех `.asi`), затем:

1. `VC.CLEO.asi`
2. `_noDEP.asi`
3. `portablegta.asi`
4. `lualoader.asi`
5. `GTAVC.WidescreenFix.asi`

Первый плагин, после которого появится `c0000409`, и будет виновником.

### 7.4. Итог

- Проблема «вставьте диск» — **решена** (путь в `config.cfg`; файл нужно воссоздать).
- Проблема «процесс есть — окна нет» — **решена** (дубликаты папок по регистру объединены).
- Проблема `c0000409` — **не до конца локализована**, но обход найден: отключённые плагины → игра запускается; сейчас `Scripts/` уже очищена.

---

*Отчёт составлен по результатам реальных запусков и проверок в Wine 11.22. Все команды и их вывод приведены в том виде, в каком они выполнялись.*
