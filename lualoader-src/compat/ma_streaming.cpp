// Менеджер потоковой загрузки моделей (18.10.2026) — быстрый жизненный цикл.
//
// ЗАЧЕМ (замер по коду до фикса): load_model_before_avalible на КАЖДУЮ сущность
// делал с worker-потока: REQUEST_MODEL (круг очереди ~кадр) + LOAD_ALL_MODELS_NOW
// (второй круг + блокирующая синхронная загрузка ВСЕХ запрошенных моделей с диска)
// + цикл «HAS_MODEL_LOADED / sleep 1мс / REQUEST_MODEL снова» (по 2 круга на итерацию)
// + финальный HAS. Для УЖЕ загруженной модели это ~6 кругов ≈ 100-200 мс на объект;
// сцена с 21 пикетчиком — секунды «замороженного» создания. LOAD_ALL вдобавок
// грузил синхронно всё, что игра заказала фоном — фриз кадра.
//
// КАК СЕЙЧАС (как делает оригинальный SCM): REQUEST_MODEL помечает модель
// загружаемой (SCRIPTOWNED|DEPENDENCY|NOFADE = 0x16), дальше CStreaming::Update()
// каждый кадр (Game.cpp) сам ведёт загрузку. Worker-поток не ходит кругами: он
// один раз регистрирует интерес (want), а кадровый обработчик на игровом потоке
// шлёт REQUEST и следит за состоянием (HasModelLoaded). Когда модель в памяти —
// condition_variable будит ожидающих. Семафор загруженности зеркалируется в
// памяти; таймаут (10 с) — только предохранитель от мёртвого стриминга.
//
// (26.10) ИСКЛЮЧЕНИЕ — ВЫЗОВ НА ИГРОВОМ ПОТОКЕ: там wait_sync() догружает модель
// сам (Request* + LoadAllRequestedModels), потому что кадровый обработчик во
// время ожидания не крутится, а lua_yield внутри составной C-функции её обрывает
// (см. большой комментарий у wait_sync). После загрузки кадровый обработчик
// продолжает вести только «фоновые» want-записи от loadmodel/releasemodel.
//
// Спец-модели (LOAD_SPECIAL_CHARACTER, 0x23C): RequestSpecialChar(slot-1, name,
// DEPENDENCY|SCRIPTOWNED=0x06) один раз за цикл want; статус — HasSpecialCharLoaded.
// IsObjectInCdImage для них НЕ проверяем: CdPosnAndSize спец-модели заполняется
// движком только при первом RequestSpecialModel (reVC Streaming.cpp:1050), а не в
// default.ide — проверка ложно объявила бы модель «невозможной».
//
// ГЛАВНОЕ ПРАВИЛО ФАЙЛА: никаких глобальных конструкторов (иначе Wine-висяк при
// загрузке DLL: RtlpWaitForCriticalSection «blocked by 0000» — см. diag.cpp).
// Всё состояние — function-local statics, создаётся при первом обращении из уже
// работающего плагина.

#include "ma_streaming.h"
#include "diag.h"

#include <windows.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Events.h"
#include "CStreaming.h"
#include "CModelInfo.h"
#include "CTimer.h"

// Флаг единой разборки (plugin.cpp): при reload блокирующие API рвут свои циклы.
extern std::atomic<bool> teardown_active;

namespace {

// Флаги запроса — в точности как SCM-опкоды (reVC Script3.cpp):
//   0x247 REQUEST_MODEL          = RequestModel(model, DEPENDENCY|NOFADE|SCRIPTOWNED)
//   0x23C LOAD_SPECIAL_CHARACTER = RequestSpecialChar(slot-1, name, DEPENDENCY|SCRIPTOWNED)
const int kFlagsModel   = 0x02 | 0x04 | 0x10;   // SCRIPTOWNED|DEPENDENCY|NOFADE
const int kFlagsSpecial = 0x02 | 0x04;          // SCRIPTOWNED|DEPENDENCY

struct Want {
    int         refs = 0;        // кто интересуется (loadmodel/load_model_before_avalible)
    int         holds = 0;       // «держат» созданные сущности (см. ma_stream_hold)
    bool        special = false; // спец-слот (109..116)
    bool        sent = false;    // REQUEST уже отправлен в этом цикле want
    bool        loaded = false;  // зеркало движка: HasModelLoaded/HasSpecialCharLoaded
    bool        impossible = false; // модели нет в CD-образе — ждать бессмысленно
    unsigned long long since = 0;// когда зарегистрирован (для отчёта/лога)
    std::string name;            // для спец-моделей
};

struct StreamState {
    std::mutex              mx;
    std::condition_variable cv;
    std::unordered_map<int, Want> wants;
    // owner (lua_State*) -> model -> сколько держаний. Нужен, чтобы destroy(скрипт)
    // снял РОВНО свои держания и не выгрузил модель у чужого живого скрипта.
    std::unordered_map<void*, std::unordered_map<int, int>> owners;
    unsigned long long frames = 0;   // кадров обработчика (для журнала)
    unsigned long long requests = 0; // сколько REQUEST ушло в движок
};

// Ленивое создание (никаких глобальных конструкторов).
StreamState& st() { static StreamState s; return s; }

// Новый цикл интереса: зеркало загруженности устарело (модель могла быть
// выгружена движком после снятия брони) — подтверждаем заново по движку.
void reset_want(Want& w, bool special, const char* name) {
    w.special = special;
    w.sent = false;
    w.loaded = false;
    w.impossible = false;
    w.since = GetTickCount64();
    if (name) w.name = name; else w.name.clear();
}

// «Взять»: общий код want/want_special. Вызывается под захваченным ст() mx.
void want_locked(int model, bool special, const char* name) {
    Want& w = st().wants[model];
    if (w.refs == 0 && w.holds == 0) reset_want(w, special, name);
    w.refs++;
}

// «Держать»: сущность создана, модель обязана остаться в памяти. Владелец —
// lua_State* (или иной указатель); holds снимается только let_go/let_go_all.
void hold_locked(int model, bool special, const char* name, void* owner) {
    Want& w = st().wants[model];
    if (w.refs == 0 && w.holds == 0) reset_want(w, special, name);
    w.holds++;
    if (owner) st().owners[owner][model]++;
}

// Имя спец-модели — в НИЖНИЙ регистр (как SCM LOAD_SPECIAL_CHARACTER).
void lower_copy(const char* name, char out[64]) {
    int n = 0;
    if (name) {
        for (; name[n] && n < 63; ++n) {
            char c = name[n];
            out[n] = (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
        }
    }
    out[n] = '\0';
}

} // namespace

// ---------------------------------------------------------------------------
// Worker-сторона
// ---------------------------------------------------------------------------

void ma_stream_want(int model) {
    // Отрицательные id — старый путь Command<> (UsedObjectArray). За верхней
    // границей ms_modelInfoPtrs — OOB в стримере (класс AV 0x580A3C), поэтому
    // не регистрируем такой want вовсе.
    if (model < 0 || model >= CModelInfo::ms_modelInfoCount) return;
    std::lock_guard<std::mutex> lk(st().mx);
    want_locked(model, false, nullptr);
}

void ma_stream_want_special(int slot, const char* name) {
    if (slot <= 0 || slot > 8) return;// спец-слоты LOAD_SPECIAL_CHARACTER: 1..8 (109..116)
    // (26.10) Имя спец-модели приводим к НИЖНЕМУ регистру — как это делает
    // оригинальный SCM-опкод LOAD_SPECIAL_CHARACTER (Script3.cpp: tolower каждого
    // символа). ВАЖНО: CStreaming::RequestSpecialModel сравнивает имя модели
    // ЧУВСТВИТЕЛЬНО к регистру (CGeneral::faststrcmp — не faststricmp). Если
    // запросить "SPANDXA", а движок/Undress позже попросит "spandxa", он
    // посчитает это СМЕНОЙ модели: RemoveModel убьёт живых педов и переименует
    // слот. Один регистр на все запросы исключает этот класс.
    char lower[64];
    lower_copy(name, lower);
    std::lock_guard<std::mutex> lk(st().mx);
    want_locked(108 + slot, true, lower);
}

// ---------------------------------------------------------------------------
// Держание модели сущностью (26.10): модель НЕ выгружается, пока живёт
// созданная под неё сущность/скрипт. MARK (0249) по ней не шлём.
// ---------------------------------------------------------------------------

void ma_stream_hold(void* owner, int model) {
    if (model < 0 || model >= CModelInfo::ms_modelInfoCount) return;
    std::lock_guard<std::mutex> lk(st().mx);
    hold_locked(model, false, nullptr, owner);
}

void ma_stream_hold_special(void* owner, int slot, const char* name) {
    if (slot <= 0 || slot > 8) return;
    char lower[64];
    lower_copy(name, lower);
    std::lock_guard<std::mutex> lk(st().mx);
    hold_locked(108 + slot, true, lower, owner);
}

void ma_stream_let_go(void* owner, int model) {
    if (model < 0) return;
    std::lock_guard<std::mutex> lk(st().mx);
    // Снимать счётчик держаний можно ТОЛЬКО если этот владелец действительно
    // держал модель. Иначе releasemodel() из Lua по чужой модели увёл бы holds
    // в минус и выкинул запись, которую держит другая сущность (и её модель
    // стала бы выгружаемой).
    bool had_hold = false;
    auto oit = st().owners.find(owner);
    if (oit != st().owners.end()) {
        auto mit = oit->second.find(model);
        if (mit != oit->second.end()) {
            had_hold = true;
            if (--mit->second <= 0) oit->second.erase(mit);
            if (oit->second.empty()) st().owners.erase(oit);
        }
    }
    if (!had_hold) return;
    auto wit = st().wants.find(model);
    if (wit == st().wants.end()) return;
    if (--wit->second.holds <= 0) wit->second.holds = 0;
    if (wit->second.holds == 0 && wit->second.refs <= 0) st().wants.erase(wit);
}

void ma_stream_let_go_all(void* owner) {
    if (!owner) return;
    std::lock_guard<std::mutex> lk(st().mx);
    auto oit = st().owners.find(owner);
    if (oit == st().owners.end()) return;
    for (auto& kv : oit->second) {
        auto wit = st().wants.find(kv.first);
        if (wit == st().wants.end()) continue;
        wit->second.holds -= kv.second;
        if (wit->second.holds <= 0) wit->second.holds = 0;
        if (wit->second.holds == 0 && wit->second.refs <= 0) st().wants.erase(wit);
    }
    st().owners.erase(oit);
}

void ma_stream_let_go_everything() {
    std::lock_guard<std::mutex> lk(st().mx);
    for (auto& kv : st().wants) {
        kv.second.holds = 0;
        if (kv.second.refs <= 0) kv.second.refs = 0;
    }
    for (auto it = st().wants.begin(); it != st().wants.end();) {
        if (it->second.holds == 0 && it->second.refs == 0) it = st().wants.erase(it);
        else ++it;
    }
    st().owners.clear();
}

void ma_stream_release(int model) {
    if (model < 0) return;
    std::lock_guard<std::mutex> lk(st().mx);
    auto it = st().wants.find(model);
    if (it == st().wants.end()) return;
    if (--it->second.refs <= 0) {
        it->second.refs = 0;
        // Держания живущих сущностей переживают снятие эфемерного интереса.
        if (it->second.holds == 0) st().wants.erase(it);
    }
}

// ---------------------------------------------------------------------------
// Ожидание загрузки (контекст-зависимое, 23.10)
// ---------------------------------------------------------------------------

// Спрашивает ДВИЖОК напрямую (корректно для спец-слотов: HasSpecialCharLoaded).
static bool model_loaded_now(int model, bool special) {
    return special ? CStreaming::HasSpecialCharLoaded(model - 109)
                   : CStreaming::HasModelLoaded(model);
}

// (26.10) СИНХРОННАЯ догрузка на игровом потоке. Раньше здесь стояла
// «кооперативная» парковка: ma_sched_park + lua_yield. Это НЕВЕРНО для любой
// C-функции, которой нужно продолжить работу ПОСЛЕ ожидания: lua_yield без
// lua_yieldk завершает C-вызов при возобновлении (ldo.c: resume → ci->u.c.k==NULL
// → luaD_poscall), и весь C++-код после yield НЕ исполняется. Симптомы в log.txt:
// 21× Createped(SPANDXA/B) «wait_start», только 14× «wait_done» — 7 педов не
// созданы; 3× Createcar(SPAND) вернули nil без единой C++-строки об ошибке
// (функция обрывалась на yield до Command<CREATE_CAR>). Заодно обрывался
// lock_guard g_lua_api_mx и оставались висеть want-записи.
//
// Теперь на игровом потоке загрузку ведём сами: RequestModel/RequestSpecialChar
// + LoadAllRequestedModels (как SCM-опкод 038B, включая Stop/Update таймера).
// Это короткая одноразовая загрузка на сцену (модели 213/112/113), а не
// покадровый шторм: после первой загрузки Has*Loaded сразу true. Зато
// вызывающая C-функция гарантированно продолжается и создаёт сущность.
static bool wait_sync(int model, bool special, int timeout_ms) {
    const unsigned long long start = GetTickCount64();
    if (timeout_ms <= 0) timeout_ms = 10000;
    std::string name;
    for (;;) {
        if (model_loaded_now(model, special)) {
            std::lock_guard<std::mutex> lk(st().mx);
            auto it = st().wants.find(model);
            if (it != st().wants.end() && !it->second.loaded) {
                it->second.loaded = true;
                st().cv.notify_all();
            }
            return true;
        }
        if (teardown_active.load()) return false;
        if (ma_stream_impossible(model)) {
            ma_diag::mark("STREAM: модель id=%d отсутствует в CD-образе — ожидание снято", model);
            return false;
        }

        // Запрос шлём здесь же: кадровый ma_stream_frame во время этого цикла
        // не работает (мы держим игровой поток), а ждать REQUEST от него нельзя.
        bool request = false;
        bool want_special = special;
        name.clear();
        {
            std::lock_guard<std::mutex> lk(st().mx);
            auto it = st().wants.find(model);
            if (it != st().wants.end()) {
                Want& w = it->second;
                want_special = w.special;
                if (!w.sent) {
                    request = true;
                    w.sent = true;
                    st().requests++;
                    name = w.name;
                }
            }
            else if (!special) {
                request = true;   // без want: обычную модель всё равно просим
            }
        }
        if (request) {
            if (want_special) {
                if (!name.empty())
                    CStreaming::RequestSpecialChar(model - 109, name.c_str(), kFlagsSpecial);
            }
            else {
                // (26.10, безопасная загрузка) Гейт CD-образа — как в ma_stream_frame.
                // RequestModel по «дырявому» id (границы ок, но модели нет в .ide)
                // разыменовывает ms_modelInfoPtrs[id] == NULL — вылет в стримере.
                // Спец-слоты 109..116 не проверяем: их CdPosnAndSize заполняет
                // движок только при первом RequestSpecialChar (reVC Streaming.cpp).
                if (!CStreaming::IsObjectInCdImage(model)) {
                    {
                        std::lock_guard<std::mutex> lk(st().mx);
                        auto it = st().wants.find(model);
                        if (it != st().wants.end()) it->second.impossible = true;
                        st().cv.notify_all();
                    }
                    ma_diag::mark("STREAM: модель id=%d отсутствует в CD-образе — ожидание снято (wait_sync)", model);
                    return false;
                }
                CStreaming::RequestModel(model, kFlagsModel);
            }
        }

        // Как SCM 038B LOAD_ALL_MODELS_NOW: таймер игры на время синхронной
        // загрузки останавливается, иначе кадр получает огромный delta.
        CTimer::Stop();
        CStreaming::LoadAllRequestedModels(false);
        CTimer::Update();

        if (model_loaded_now(model, special)) {
            std::lock_guard<std::mutex> lk(st().mx);
            auto it = st().wants.find(model);
            if (it != st().wants.end() && !it->second.loaded) {
                it->second.loaded = true;
                st().cv.notify_all();
            }
            return true;
        }
        // Модель была загружена и выгружена движком — разрешаем новый REQUEST
        // (как ma_stream_frame при смене loaded→unloaded).
        {
            std::lock_guard<std::mutex> lk(st().mx);
            auto it = st().wants.find(model);
            if (it != st().wants.end() && it->second.loaded) {
                it->second.loaded = false;
                it->second.sent = false;
            }
        }
        if ((int)(GetTickCount64() - start) >= timeout_ms) {
            ma_diag::mark("STREAM: модель id=%d не загрузилась за %d мс — таймаут (стриминг мёртв?)",
                          model, timeout_ms);
            return false;
        }
        Sleep(1);
    }
}

bool ma_stream_wait(int model, int timeout_ms) {
    if (model < 0 || model >= CModelInfo::ms_modelInfoCount) return false;// OOB-защита
    const bool special = (model >= 109 && model <= 116);

    if (ma_cmdq_on_game_thread()) {
        // Никакого lua_yield внутри составных API (см. комментарий wait_sync).
        return wait_sync(model, special, timeout_ms);
    }

    // Не-игровой поток (legacy): событийное ожидание на cv (загруженность-зеркало
    // обновляет кадровый обработчик; notify при смене состояния).
    const unsigned long long start = GetTickCount64();
    std::unique_lock<std::mutex> lk(st().mx);
    for (;;) {
        auto it = st().wants.find(model);
        if (it == st().wants.end()) return false;        // интерес снят (release)
        Want& w = it->second;
        if (w.loaded) return true;
        if (w.impossible) return false;
        if (teardown_active.load()) return false;        // reload: скрипты сворачиваются
        unsigned long long elapsed = GetTickCount64() - start;
        if ((int)elapsed >= timeout_ms) {
            lk.unlock();
            ma_diag::mark("STREAM: модель id=%d не загрузилась за %d мс — таймаут (стриминг мёртв?)",
                          model, timeout_ms);
            return false;
        }
        // Событийное ожидание; 100 мс — перепроверка teardown/таймаута.
        st().cv.wait_for(lk, std::chrono::milliseconds(100));
    }
}

// Объявлена ли модель «невозможной» (нет в CD-образе) — для ожидающих до таймаута.
bool ma_stream_impossible(int model) {
    std::lock_guard<std::mutex> lk(st().mx);
    auto it = st().wants.find(model);
    return it != st().wants.end() && it->second.impossible;
}

// Сколько моделей сейчас в реестре «хочу» (для heartbeat-диагностики: большое
// число на фоне «модель грузится вечно» = источник кадровой работы стриминга).
unsigned ma_stream_wants() {
    std::lock_guard<std::mutex> lk(st().mx);
    return (unsigned)st().wants.size();
}

// ---------------------------------------------------------------------------
// Игровой поток: кадровый обработчик
// ---------------------------------------------------------------------------

void ma_stream_frame() {
    // Хук висит на Events::gameProcessEvent — по построению игровой поток.
    // Проверка защитная (id выставляет ma_cmdq_drain первой строкой каждого кадра).
    const unsigned gt = (unsigned)ma_diag_game_thread();
    if (gt && GetCurrentThreadId() != gt) return;

    // Снимок заявок (23.10, вечер): ПЕРСИСТЕНТНЫЙ буфер (function-local static —
    // без глобальных конструкторов): capacity растёт один раз и живёт, malloc/
    // free каждый кадр исключён. Движковые вызовы — БЕЗ удержания mutex
    // (worker не тормозим), мутации карты (want/release) во время обхода
    // исключены.
    static std::vector<std::pair<int, Want>> snap_buf;
    snap_buf.clear();
    {
        std::lock_guard<std::mutex> lk(st().mx);
        st().frames++;
        for (auto& kv : st().wants) snap_buf.push_back(kv);
    }
    if (snap_buf.empty()) return;

    for (auto& kv : snap_buf) {
        const int model = kv.first;
        const Want& w = kv.second;

        bool nowLoaded = false;
        if (w.special) {
            const int slot = model - 108;               // 1-based
            if (!w.sent) {
                // Один раз за цикл интереса. Повторный вызов с ТЕМ ЖЕ именем
                // безвреден (reVC: имя совпало => обычный RequestModel), а с
                // чужим — опасен (движок удаляет педов той модели): one-shot.
                CStreaming::RequestSpecialChar(slot - 1, w.name.c_str(), kFlagsSpecial);
                std::lock_guard<std::mutex> lk(st().mx);
                auto it = st().wants.find(model);
                if (it != st().wants.end()) { it->second.sent = true; st().requests++; }
            }
            nowLoaded = CStreaming::HasSpecialCharLoaded(slot - 1);
        } else {
            // (23.10, вечер) sent-гейт: RequestModel — ОДИН раз за цикл интереса
            // (как у спец-моделей). Старый код шёл RequestModel КАЖДЫЙ кадр, пока
            // модель не встала: для «мёртвой» модели (WATCHDOG: «модель id=91
            // грузится 6323 мс — стрим не идёт») это = стриминговая работа
            // движка ЕЖЕКАДРОВО на весь период миссии = обвал FPS ВСЕЙ игры.
            // Движок сам ведёт загрузку (CStreaming::Update) — переспрашивать
            // не нужно.
            if (!w.sent) {
                if (!CStreaming::IsObjectInCdImage(model)) {
                    // Модели нет в CD-образе: загрузка невозможна в принципе.
                    // Помечаем — ожидающие просыпаются с false вместо 10 с тыка.
                    {
                        std::lock_guard<std::mutex> lk(st().mx);
                        auto it = st().wants.find(model);
                        if (it != st().wants.end()) it->second.impossible = true;
                        st().cv.notify_all();
                    }
                    ma_diag::mark("STREAM: модель id=%d отсутствует в CD-образе — загрузка невозможна", model);
                    continue;
                }
                CStreaming::RequestModel(model, kFlagsModel);
                std::lock_guard<std::mutex> lk(st().mx);
                auto it = st().wants.find(model);
                if (it != st().wants.end()) { it->second.sent = true; st().requests++; }
            }
            nowLoaded = CStreaming::HasModelLoaded(model);
        }

        // Обновление зеркала + побудка ожидающих при смене состояния.
        bool changed = false;
        {
            std::lock_guard<std::mutex> lk(st().mx);
            auto it = st().wants.find(model);
            if (it != st().wants.end()) {
                Want& w2 = it->second;
                if (w2.loaded != nowLoaded) {
                    w2.loaded = nowLoaded;
                    changed = true;
                    // (23.10) Модель была загружена, но движок её ВЫГРУЗИЛ
                    // (давление памяти): разрешаем один повторный REQUEST.
                    if (!nowLoaded && !w2.impossible) w2.sent = false;
                }
            }
        }
        if (changed) st().cv.notify_all();
    }
}

void ma_stream_install_hook() {
    // Идемпотентная регистрация из Message (plugin.cpp), после ma_cmdq_install_hook().
    // ⛔ НЕ регистрировать из глобального конструктора: список Events на тот момент
    // ещё не инициализирован ("blocked by 0000" в RtlpWaitForCriticalSection), и НЕ
    // изнутри обхода Events (порча std::list) — только отсюда.
    static bool installed = false;
    if (installed) return;
    installed = true;
    plugin::Events::gameProcessEvent += [] { ma_stream_frame(); };
}
