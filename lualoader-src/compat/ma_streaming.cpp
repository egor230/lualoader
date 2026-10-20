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
// каждый кадр (Game.cpp) сам ведёт загрузку — LoadAllRequestedModels не нужен.
// Worker-поток не ходит кругами: он один раз регистрирует интерес (want), а
// кадровый обработчик на игровом потоке шлёт REQUEST и следит за состоянием
// (HasModelLoaded). Когда модель в памяти — condition_variable будит ожидающих.
// Семафор загруженности зеркалируется в памяти; таймаут (10 с) — только
// предохранитель от мёртвого стриминга, не рабочий режим.
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
    unsigned long long frames = 0;   // кадров обработчика (для журнала)
    unsigned long long requests = 0; // сколько REQUEST ушло в движок
};

// Ленивое создание (никаких глобальных конструкторов).
StreamState& st() { static StreamState s; return s; }

// «Взять»: общий код want/want_special. Вызывается под захваченным ст() mx.
void want_locked(int model, bool special, const char* name) {
    Want& w = st().wants[model];
    if (w.refs == 0) {
        // Новый цикл интереса: зеркало загруженности устарело (модель могла быть
        // выгружена движком после снятия брони) — подтверждаем заново по движку.
        w.special = special;
        w.sent = false;
        w.loaded = false;
        w.impossible = false;
        w.since = GetTickCount64();
        if (name) w.name = name; else w.name.clear();
    }
    w.refs++;
}

} // namespace

// ---------------------------------------------------------------------------
// Worker-сторона
// ---------------------------------------------------------------------------

void ma_stream_want(int model) {
    if (model < 0) return;                    // отрицательные id — старый путь Command<>
    std::lock_guard<std::mutex> lk(st().mx);
    want_locked(model, false, nullptr);
}

void ma_stream_want_special(int slot, const char* name) {
    if (slot <= 0) return;
    // Регистр НЕ трогаем: рабочий legacy слал имя как есть (SPANDXA) и это работало
    // (сравнение имён в движке без учёта регистра, reVC faststrcmp).
    std::lock_guard<std::mutex> lk(st().mx);
    want_locked(108 + slot, true, name ? name : "");
}

void ma_stream_release(int model) {
    if (model < 0) return;
    std::lock_guard<std::mutex> lk(st().mx);
    auto it = st().wants.find(model);
    if (it == st().wants.end()) return;
    if (--it->second.refs <= 0) st().wants.erase(it);
}

bool ma_stream_wait(int model, int timeout_ms) {
    // Игровой поток: ждать cv НЕЛЬЗЯ (кадровый обработчик — тот же поток: дедлок
    // до таймаута). Здесь можно спросить движок напрямую.
    if (ma_cmdq_on_game_thread()) return CStreaming::HasModelLoaded(model);

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

// ---------------------------------------------------------------------------
// Игровой поток: кадровый обработчик
// ---------------------------------------------------------------------------

void ma_stream_frame() {
    // Хук висит на Events::gameProcessEvent — по построению игровой поток.
    // Проверка защитная (id выставляет ma_cmdq_drain первой строкой каждого кадра).
    const unsigned gt = (unsigned)ma_diag_game_thread();
    if (gt && GetCurrentThreadId() != gt) return;

    // Снимок заявок: движковые вызовы — БЕЗ удержания mutex (worker не тормозим),
    // мутации карты (want/release) во время обхода исключены.
    std::vector<std::pair<int, Want>> snap;
    {
        std::lock_guard<std::mutex> lk(st().mx);
        st().frames++;
        snap.reserve(st().wants.size());
        for (auto& kv : st().wants) snap.push_back(kv);
    }
    if (snap.empty()) return;

    for (auto& kv : snap) {
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
            if (!w.loaded && !w.impossible) {
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
                if (w2.loaded != nowLoaded) { w2.loaded = nowLoaded; changed = true; }
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
