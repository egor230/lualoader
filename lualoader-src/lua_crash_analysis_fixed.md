# �?е�?ал�?н�?й анализ в�?ле�?а п�?и па�?зе Lua-ск�?ип�?ов (lualoader ASI)

## �?�?и�?и�?еский �?�?ейса и диагнос�?ика п�?и�?ин �?�?Н�?�?�?НН�?�?�? �?Ы�?�?ТА

### Анализ последнего сеанса (нов�?й билд)

Т�?ейс (trace.txt, последние с�?�?оки нового сеанса):
printmessage
C++| load_model_before_avalible: �?А�?Р�?С модели id=280
модел�? id=280 заг�?�?жена
�?АУ�?А: в�?од (thread=472), ск�?ип�?�? замо�?ажива�?�?ся (yield-па�?за без teardown)
[�?�?РЫ�? �?? иг�?а в�?ле�?ела]
```

**Фак�?�?:**
1. Ск�?ип�?�? в�?полнялис�? (printmessage, load_model �?спе�?но заве�?�?�?н)
2. `pump_owner START` Н�?Т в �?�?ейсе �?? либо не �?спел записа�?�?ся, до�?�?л до э�?ой с�?�?оки
3. �?АУ�?А записана из thread=472 (reload-по�?ок)
4. �?осле �?АУ�?Ы �?�?Н�?�?�?ННЫ�? в�?ле�? (не зависание, п�?одолжение)

**�?�?вод:** �?�?ле�? п�?оис�?оди�? **�?НУТР�?** `pause_scripts()` или **СРА�?У �?�?С�?�?** (в reload-по�?оке о�?ве�? на дейс�?вия reload-по�?ока).

---

A) �?о�?агов�?й момен�?а в�?ле�?а

Сос�?ояние сис�?ем�? момен�? Ctrl

**�?о�?ок иг�?�? (main thread GTA VC):**
- �?�?полняе�? иг�?овой �?икл (�?енде�?, �?изика, иг�?�?)
`gameProcessEvent` в�?з�?вае�?ся кажд�?й кад�?
Рабо�?ае�? с памя�?�?�?: `CTheScripts::ScriptSpace`, п�?л�? об�?ек�?ов (CVehicle, CPed �?.д.)

**Reload-по�?ок (thread=472, detached):**
�?�?�?�?и�?ся �?икле `while (KeyPressed(VK_CONTROL)) Sleep(1);`
�?бна�?�?живае�? о�?п�?скание Ctrl
�?�?з�?вае�? `pause_scripts()`:
  ```cpp
  scripts_disabled.store(true);
  star_thread::set(false);
  CMessages::AddMessageJumpQ(L"Scripts disabled", 2000, 1);  // �?� �?�?АСН�?!
  pause_scripts();
  **Pump-по�?ок(и) (detached worker threads):**
`pump_owner` (LUA_OK != lua_status(L))`
�? Ctrl може�? на�?оди�?�?ся:
  �?н�?�?�?и `sleep_for(1ms)` на�?але �?икла
  `lua_resume(L, ...)` (в�?полняе�? Lua-код)
  C-�?�?нк�?ии `load_model_before_avalible` (б�?л в�?зван Lua, е�?�? ве�?н�?лся)
  �?ежд�? и�?е�?а�?иями (п�?ове�?яе�? `scripts_paused.load()`)

�?о�?аговая последова�?ел�?нос�?�? (ве�?оя�?ная)

**T=0: Ск�?ип�? в�?з�?вае�? load_model_before_avalible(280)**

Pump-по�?ок:
lua_resume(L, ...) в�?полняе�? Lua-код load_model_before_avalible(280)
                                          �??
                                   void-�?�?нк�?ия, блоки�?�?�?�?ий �?икл:
                                   while (!HAS_MODEL_LOADED(280)) {
                                       if (teardown_active.load()) break;
                                       sleep(1ms);
                                       REQUEST_MODEL(280);
                                   }
                                   возв�?а�? (модел�? заг�?�?жена)
                                   �??
                                   lua_resume возв�?а�?ае�? �?п�?авление Lua
                                   �??
                                   Lua-код п�?одолжае�?ся (след�?�?�?ая инс�?�?�?к�?ия после load_model)
**T=100ms: �?ол�?зова�?ел�? жм�?�? Ctrl**

Reload-по�?ок обна�?�?живае�? нажа�?ие, жд�?�? о�?п�?скания.

**T=500ms: о�?п�?скае�? Reload-по�?ок:
```cpp
reload-по�?ок Н�? по�?ок!)
scripts_disabled.store(true);  А�?ома�?но, безопасно
star_thread::set(false);       �?�?АСН�?! (см. п�?нк�? C)
pause_scripts();  �?Х�?�?
**T=501ms: pause_scripts()**

int pause_scripts() {
    bool k = false;
    star_coroutine::set(k);  безопасно
    
    unsigned int& OnAMissionFlag *(unsigned int*)0x978748;  �?�?АСН�?!
    CTheScripts::ScriptSpace[OnAMissionFlag] k;  �?А�?�?СЬ �?�?Р�?�?УЮ �?А�?ЯТЬ �?�? ЧУ�?�?�?�? �?�?Т�?�?А!
    
    scripts_paused.store(true);  
    cpp_tracef("�?АУ�?А: (thread=%lu), замо�?ажива�?�?ся", GetCurrentThreadId());
    �?�?С�?�?�?НЯЯ СТР�?�?А ТР�?�?С�?
    
    return 0;
}
**�?Р�?Т�?Ч�?С�?�?�? �?�?�?�?НТ:** �?апис�? `CTheScripts::ScriptSpace[OnAMissionFlag]` reload-по�?ока **�?�?Н�?�?Р�?�?�?НН�?** �?�?ением/запис�?�? иг�?ового по�?ока **DATA RACE** undefined behavior �?РАШ.

�?о�?ем�? (а зависание)?

**Data race `CTheScripts::ScriptSpace`:**

�?г�?овой по�?ок э�?о�? може�?:
Чи�?а�?�? для п�?ове�?ки ак�?ивнос�?и миссии
�?апис�?ва�?�? соседние я�?ейки `CTheScripts::ScriptSpace`
�?б�?аба�?�?ва�?�? ск�?ип�?ов�?е пе�?еменн�?е

**�?�?�? С�?НХР�?Н�?�?АЦ�?�?** пи�?е�?:
false;
**�?озможн�?е ис�?од�?:**

**Access Violation (наиболее ве�?оя�?но):**
   записи об�?аба�?�?вае�? `ScriptSpace`
   CPU-кэ�?и �?ассин�?�?онизи�?ован�?
   �?и�?ае�? �?ас�?и�?но записанное зна�?ение (torn read)
   �?оп�?�?ка испол�?зова�?�? би�?ое дос�?�?п к невалидном�? ад�?ес�? **CRASH**

**SEH Exception:**
   VC испол�?з�?е�? SEH (Structured Exception Handling)
   Data искл�?�?ение иг�?овом по�?оке
   Не�? об�?або�?�?ика п�?о�?есс заве�?�?ае�?ся

**Heap Corruption:**
   �?сли `ScriptSpace` �?асположен �?ядом heap-с�?�?�?к�?�?�?ами
   �?�?жого пов�?еждение heap metadata
   След�?�?�?ий `malloc`/`free` попал �?�?ейс?

**�?а�?иан�? 1: �?�?�?е�?иза�?ия в�?вода**

`cpp_tracef` `fstream::flush()`, но:
ава�?ийно заве�?�?ился (SIGTERM, access violation), б�?�?е�?�? сб�?ас�?ва�?�?ся
�?аже `flush()` �?С де�?жа�?�? данн�?е kernel buffer

**Ре�?ение:** �?спол�?зова�?�? unbuffered I/O:

void cpp_tracef(const char* format, FILE* f fopen("lualoader/trace.txt", "a");
    (!f) return;
    
    setvbuf(f, NULL, _IONBF, 0);  �?Т�?�?ЮЧ�?ТЬ �?УФ�?Р�?�?АЦ�?Ю
    
    va_list args;
    va_start(args, format);
    vfprintf(f, args);
    va_end(args);
    
    fprintf(f, "\n");
    fclose(f);  fclose ав�?ома�?и�?ески flush
2: зап�?с�?ился**

(`printmessage`, `load_model`), но записан, возможно:

`startscipt` делае�? **пе�?в�?й** args, NULL)` **�?�?** создания pump-по�?ока
Э�?о�? resume `main()` ск�?ип�?а син�?�?онно
�?а�?ем созда�?�?ся `std::thread(pump_owner, L, L1, **послед�?�?�?и�?** resume
п�?оизо�?�?л с�?а�?�?а pump-по�?ока, записа�?�?ся

**�?�?ове�?ка кода `startscipt`:** Н�?жно �?виде�?�? полн�?й код, ве�?оя�?но �?ам ес�?�?:

startscipt(const path) lua_State* L luaL_newstate();
    luaL_openlibs(L);
    funs(L);  Регис�?�?а�?ия API
    
    luaL_loadfile(L, path);
    lua_pcall(L, 0, ск�?ип�?, �?егис�?�?и�?�?е�? main()
    
    lua_getglobal(L, "main");
    L1 lua_newthread(L);
    lua_xmove(L, �?е�?еме�?аем main L1
    
    �?�?Р�?Ы�? (син�?�?онно �?ек�?�?ем по�?оке!)
    NULL);  �?�?�?СЬ в�?полняе�?ся main() пе�?вого yield
    
    �?АТ�?�? созда�?м pump-по�?ок
    std::thread(pump_owner, false).detach();
�?ак, �?о `printmessage` `load_model` в�?полнилис�? по�?оке (ко�?о�?�?й сам б�?�?�? detached `gameProcessEvent`).

B) `CTheScripts::ScriptSpace` �?Р�?Т�?Ч�?С�?АЯ �?Ш�?�?�?А

�?�?облема

(thread=472)
unsigned int*)0x978748;
false;  �?�?Т�?�?А!
**Э�?о �?�?Т�?�?�?�?�?�?�?�?АСН�?.**

э�?о массив памя�?и, ко�?о�?ом�? об�?а�?ае�?ся:
(�?�?ение/запис�? кад�?)
�?г�?ов�?е (�?е�?ез опкод�?)

behavior.

�?А�?�?Т иг�?�? СРА�?У

**�?е�?анизм:**

`ScriptSpace[OnAMissionFlag]`:
   ```cpp
   �?де-�?о движке:
   (CTheScripts::ScriptSpace[OnAMissionFlag]) {
       �?б�?або�?ка ак�?ивной миссии
   }
   пи�?е�?:
   false;
   �?ассин�?�?онизи�?ован�? (кажд�?й сво�?м яд�?е):
   запис�?вае�? `false` свой L1-cache
   **с�?а�?ое** своего �?ли наобо�?о�?: **�?ас�?и�?но записанное** read 32-bit i686 �?едко, возможно если запис�? aligned)

**Worst case:** движок `OnAMissionFlag` как **индекс** д�?�?гой массив:
   mission_data_index CTheScripts::ScriptSpace[OnAMissionFlag];
   MissionData* data &missions[mission_data_index];  �?�?ТЫ�? �?Н�?�?�?С!
   data->some_field ...;  ACCESS VIOLATION!
   �?РА�?�?�?ЬН�?�? Р�?Ш�?Н�?�?: Т�?�?Ь�?�? по�?ока

**С�?ема:**

�?с�?анавливае�? **а�?ома�?н�?й �?лаг** "зап�?о�?ена па�?за"
(`gameProcessEvent`) **кажд�?й кад�?** п�?ове�?яе�? �?лаг
�?лаг �?с�?ановлен **СА�?** пи�?е�? `ScriptSpace`

**�?од:**

�?лобал�?н�?е �?лаги
std::atomic<bool> pause_requested{false};
resume_requested{false};

(Н�? �?�?огае�? иг�?ов�?�? памя�?�?!)
reload() (true) {
        Sleep(10);
        
        if {
            �?д�?м о�?п�?скания
            Sleep(1);
            
            �?с�?анавливаем �?лаг, �?�?огаем ScriptSpace!
            pause_requested.store(true);
            scripts_disabled.store(true);
            star_thread::set(false);
            
            // в�?з�?ваем CMessages::AddMessageJumpQ здес�?! C)
        }
    }

(gameProcessEvent gameProcessEvent() �?�?ове�?ка зап�?оса па�?з�?
    (pause_requested.load()) pause_requested.store(false);  Сб�?ас�?ваем �?лаг
        
        // �?�?�?�?�?АСН�?: пи�?ем ScriptSpace �?�?Р�?�?�?�?�? �?�?Т�?�?А
        int*)0x978748;
        false;
        
        star_coroutine::set(false);
        scripts_paused.store(true);
        по�?ока
        paused", 1);
        
        cpp_tracef("�?АУ�?А: �?с�?ановлена (thread=%lu)", }
    
    возобновление
    (resume_requested.load()) resume_requested.store(false);
        
        true;  н�?жно
        
        star_coroutine::set(true);
        scripts_paused.store(false);
        
        resumed", 1);
        
        cpp_tracef("�?�?�?�?�?Н�?�?�?�?Н�?�?: �?с�?ановлено 
    // �?с�?ал�?ная логика gameProcessEvent
    (scripts_disabled.load()) Не спавним нов�?е ск�?ип�?�?
        return;
    ...
C) `CMessages::AddMessageJumpQ` 1);
**GTA SDK �?�?нк�?ии �?�?Т�?�?�?�?�?�?�?�?АСНЫ.**

`CMessages::AddMessageJumpQ`:
�?обавляе�? сооб�?ение о�?е�?ед�? о�?об�?ажения
�?зменяе�? вн�?�?�?енние с�?�?�?к�?�?�?�? `CMessages` (массив�?, с�?�?�?�?ики)
�?оже�? в�?з�?ва�?�? по�?окобезопасно VC)

**�?�?зов по�?ока:**
вн�?�?�?енни�? с�?�?�?к�?�?�?а�? `CMessages`
Heap corruption (если кас�?омн�?й аллока�?о�?)
**�?РАШ**

�?�?з�?ва�?�? См. код п�?нк�?е B пе�?енес�?н `gameProcessEvent`.

D) Pump-по�?оки сос�?оянии YIELD resume

Ч�?о п�?оис�?оди�??

**Pump-по�?ок:**
lua_status(L) && !lua_state_obsolete(L)) this_thread::sleep_for(chrono::milliseconds(1));
    
    if (scripts_paused.load()) continue;  �?А�?�?СА�?Т �?�?�?СЬ
    
    ... lua_resume **�?о�?�?�?ина L:**
На�?оди�?ся `LUA_YIELD` (в�?звана `lua_yield` `wait()`)
`lua_status(L) == LUA_YIELD` �?словие �?икла `LUA_OK lua_status(L)` ис�?инно
Цикл п�?одолжае�?ся, `scripts_paused true` `continue` снова `sleep(1ms)` ...

**ЭТ�? Н�?Р�?А�?ЬН�?.** �?о�?ок па�?к�?е�?ся, к�?�?�?и�?ся п�?с�?ом �?икле, **Н�? resume** ко�?�?�?ина ос�?а�?�?ся YIELD.

�?езопаснос�?�?

**Не�? п�?облем�?**, �?С�?�?:
памя�?�? (�?ол�?ко а�?ома�?н�?е �?лаги)
`lua_State L` валидн�?м зак�?�?�?, �?дал�?н)

**�?�?облема �?�?�?�?Т б�?�?�?**, �?г�?а заве�?�?ае�?ся (пол�?зова�?ел�? в�?�?ел) �?бивае�?ся по�?оки заве�?�?ен�?** �?бивае�? и�? п�?ин�?ди�?ел�?но
�?бийс�?ва вн�?�?�?и `lua_resume` (нап�?име�?, C-�?�?нк�?ии) к�?а�?
Но об�?�?но ко�?�?ек�?но заве�?�?ае�? даже ак�?ивн�?ми по�?оками

�?�?авил�?н�?й teardown в�?�?оде иг�?�?

`final_scripts()` (или аналог) в�?�?оде
`teardown_active`
�?д�?�? заве�?�?ения все�? pump-по�?оков (join wait condition_variable)
�?ак�?�?вае�? все `lua_State`

std::mutex scripts_mutex;
std::condition_variable scripts_cv;
std::set<lua_State*> active_scripts;
teardown_active{false};

register_script(lua_State* L) std::lock_guard<std::mutex> lock(scripts_mutex);
    active_scripts.insert(L);
unregister_script(lua_State* active_scripts.erase(L);
    scripts_cv.notify_all();
pump_owner(lua_State* first_resume) register_script(L);
    
    script_started(L);
    cpp_tracef("pump_owner START L=%p (void*)L, (first_resume) NULL);
    
    !lua_state_obsolete(L) !teardown_active.load()) this_thread::sleep_for(chrono::milliseconds(1));
        
        continue;
        
        ...
    
    script_stopped(L);
    END Teardown: зак�?�?ваем С�?�?Ё�? по�?оке
    (teardown_active.load() || lua_state_obsolete(L)) lua_close(L);
    } else drop_owner(L);
    
    unregister_script(L);
final_scripts() cpp_tracef("final_scripts: на�?ало teardown");
    
    teardown_active.store(true);
    
    pump-по�?оков
    std::unique_lock<std::mutex> (!active_scripts.empty()) ожидание %zu ск�?ип�?ов", active_scripts.size());
        scripts_cv.wait_for(lock, std::chrono::seconds(1));
    
    cpp_tracef("final_scripts: заве�?�?ен�?");
**Н�?:** зак�?�?л окно, Alt+F4), `final_scripts` в�?зва�?�?ся. Detached б�?д�?�? �?би�?�? �?С, �?�?о безопасно.

E) �?локи�?овка па�?зе

Тек�?�?ий код

iters++;
        return;  EARLY RETURN па�?зе
    ос�?ал�?ная �?РА�?�?�?ЬН�?**, Н�?:

должен **�?С�?�?�?А** п�?ове�?я�?�? �?лаги па�?з�? (`pause_requested`, `resume_requested`)
`scripts_disabled true`, н�?жно об�?аба�?�?ва�?�? возобновление

**Ул�?�?�?енн�?й код:**

�?С�?�?�?А п�?ове�?яем па�?з�?/возобновления (даже disabled)
    pause_requested.store(false);
        
        false;
        
        star_coroutine::set(false);
        �?с�?анови�?�? спавн ск�?ип�?ов
        
        cpp_tracef("�?АУ�?А: true;
        
        scripts_paused.store(false);
        scripts_disabled.store(false);  Раз�?е�?и�?�? спавн
        
        disabled ск�?ип�?�?
    (спавн ск�?ип�?ов �?.д.)
    (star_thread::get()) Спавн start_lualoder
        F) Lightuserdata �?с�?а�?ев�?ие �?каза�?ели SDK

**C-�?�?нк�?ии пе�?еда�?�? пойн�?е�?�? иг�?ов�?�? Lua:**

create_ped(lua_State* CPed* ped new CPed(...);
    lua_pushlightuserdata(L, ped);
    1;
remove_ped(lua_State* (CPed*)lua_tolightuserdata(L, 1);
    delete ped;  �?�?АСН�? �?же �?дал�?н!
    **�?�?и па�?зе/в�?�?оде:**
�?дали�?�? `CPed` (миссия заве�?�?ена, иг�?ок в�?�?ел)
Lua-ко�?�?�?ина вс�? де�?жи�? `lightuserdata` �?каза�?елем �?дал�?нн�?й об�?ек�?
�?�?и возобновлении ск�?ип�? `remove_ped(с�?а�?�?й_ped)` **USE-AFTER-FREE** �?РАШ

Список подоз�?и�?ел�?н�?�? �?�?нк�?ий

**�?се �?�?нк�?ии, �?або�?а�?�?ие иг�?ов�?ми пойн�?е�?ами:**

**create_ped, create_spec_ped, remove_ped**
   Созда�?�?/�?даля�?�? `CPed*`
   �?дал�?н иг�?ой

**pipir** ясно делае�?, по�?оже �?або�?�? об�?ек�?ами)

**spawner** об�?ек�?ов/�?�?анспо�?�?а)
   `CVehicle*`, `CObject*`

**autoaim** (�?або�?а �?елями)
   `CPed*`, `CVehicle*`

5. **mission_watch** (кон�?�?ол�? миссии)
   сс�?лки mission-об�?ек�?�?

6. **draw, corona** (�?енде�?инг)
   �?ог�?�? `CEntity*`

7. **getcord** (коо�?дина�?�? об�?ек�?ов)
   �?и�?а�?�? `CVector*` �?дал�?нн�?�? об�?ек�?ов

�?�?авил�?ная за�?и�?а

�?алида�?ия пойн�?е�?ов пе�?ед испол�?зованием**

валиднос�?и:

is_valid_ped(CPed* ped) SDK: п�?ове�?ка п�?инадлежнос�?и п�?л�?
    CPools::GetPedPool()->IsValidPtr(ped);
(scripts_paused.load() teardown_active.load()) 0;  �?а�?за/в�?�?од ни�?его делаем
    (!is_valid_ped(ped)) lua_pushboolean(L, false);
        1;  Ped �?дал�?н
    ped;
    true);
    �?лобал�?н�?й "иг�?а заве�?�?ае�?ся"**

game_shutting_down{false};

game_shutting_down.load()) иг�?ов�?е об�?ек�?�?
    
    CPed* 1);
    
    (is_valid_ped(ped)) иг�?�?:
game_shutting_down.store(true);
    teardown_active.store(true);
    Э�?алонн�?й па�?�?е�?н �?С�?Х API

api_XXX(lua_State* сос�?ояния
    lua_yield(L, �?а�?за yield
    �?�?�?од а�?г�?мен�?ов (пойн�?е�?�?, индекс�?)
    void* ptr lua_tolightuserdata(L, (!is_valid_pointer(ptr)) �?б�?ек�? Рабо�?а об�?ек�?ами
    ...
    
    �?озв�?а�? �?ез�?л�?�?а�?а
    �?олн�?й список �?�?нк�?ий, �?�?еб�?�?�?и�? п�?ове�?ки

**�?�?и�?и�?еские (�?або�?а�?�? пойн�?е�?ами):**
`create_ped`, `create_spec_ped`, `remove_ped`
`spawner` (создание/�?даление �?�?анспо�?�?а/об�?ек�?ов)
`autoaim` �?елями)
`pipir` ясно, н�?жен анализ)

**С�?едний �?иск (�?и�?а�?�? памя�?�?):**
`getcord` об�?ек�?ов)
`mission_watch` (сос�?ояние миссии)
`draw`, `corona` (�?енде�?инг, мог�?�? `CEntity*`)

**Низкий �?або�?а�?�? пойн�?е�?ами нап�?ям�?�?):**
`wait`, `printmessage`, `load_model_before_avalible`
добавля�?�? п�?ове�?к�? `scripts_paused` ко�?�?ек�?ной па�?з�?

G) �?�?зов API **С�?ена�?ий:**

Pump-по�?ок `load_model_before_avalible(280)` (блоки�?�?�?�?ая C-�?�?нк�?ия)
`scripts_paused`**
`OnAMissionFlag false`
заг�?�?жена)
**след�?�?�?ая инс�?�?�?к�?ия** `remove_ped(с�?а�?�?й_ped)`)
`remove_ped` **�?�?С�?�?** �?с�?ановки п�?�?ае�?ся ped, ко�?о�?�?й иг�?ой закон�?ена) Ре�?ение: во C-�?�?нк�?ия�?

**�?�?авило:** �?А�?�?АЯ C-�?�?нк�?ия должна:
НАЧА�?�? дела�?�? `lua_yield`
Ц�?�?�?АХ `lua_yield`

**�?сп�?авление `load_model_before_avalible`:**

�?Ы�?�? (Н�?�?РА�?�?�?ЬН�?):
load_model_before_avalible(int model_id) REQUEST_MODEL(model_id);
    
    (!HAS_MODEL_LOADED(model_id)) break;  Тол�?ко teardown
        СТА�?�? (�?РА�?�?�?ЬН�?):
load_model_before_avalible(lua_State* {  Т�?�?�?РЬ �?�?�?�?РАЩА�?Т int, п�?инимае�? L
    �?а�?за
    �?�?�?од
    
    model_id luaL_checkinteger(L, cpp_tracef("load_model_before_avalible: id=%d", model_id);
    
    Ц�?�?�?�?
        п�?е�?ван (teardown)");
            0;
        }
        
        this_thread::sleep_for(chrono::milliseconds(1));
    id=%d заг�?�?жена", model_id);
    **�?Р�?Т�?ЧН�?:** возв�?а�?а **�?�?�?�?Т** п�?одолжи�?�? в�?полнение па�?за сня�?а), **�?�?�?** па�?к�?е�?ся ак�?ивна). След�?�?�?ая `remove_ped`) **ТА�?�?�?** должна `scripts_paused`.

(funcs.cpp):**

**wait** �??
**load_model_before_avalible** �?С�?РА�?�?ТЬ (добави�?�? �?икл)
**play_voice** �?? (но �?беди�?�?ся, �?А�?�?�?�? и�?е�?а�?ии)
(де�?а�?-по�?ок, основная �?�?нк�?ия п�?ове�?я�?�?)
**expectations** (�?икл ожидания видимос�?и ав�?о)
remove_ped** �?�?�?А�?�?ТЬ на�?ало
п�?ове�?к�?
8. 9. 10. 11. 12. **printmessage** (низкий �?иск, консис�?ен�?нос�?и)

H) �?иагнос�?ика в�?ле�?а: SEH, MiniDump, �?�?асси�?овка

**мгновенн�?й**, �?�?ейс об�?�?вае�?ся с�?аз�? н�?жна де�?ал�?ная диагнос�?ика.

Ре�?ение SetUnhandledExceptionFilter

**�?од Windows (GTA VC):**

#include <windows.h>
<dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

LONG WINAPI crash_handler(EXCEPTION_POINTERS* ex_info) fopen("lualoader/crash_dump.txt", "w");
    (f) "=== CRASH DUMP ===\n");
        "Exception code: 0x%08X\n", ex_info->ExceptionRecord->ExceptionCode);
        address: (DWORD)ex_info->ExceptionRecord->ExceptionAddress);
        "Thread ID: %lu\n", GetCurrentThreadId());
        С�?ек в�?зовов (�?п�?о�?�?нно)
        "\n=== STACK TRACE CONTEXT* ctx ex_info->ContextRecord;
        "EIP: ctx->Eip);
        "ESP: ctx->Esp);
        "EBP: ctx->Ebp);
        
        �?амп �?егис�?�?ов
        REGISTERS "EAX: 0x%08X  EBX: ECX: EDX: 0x%08X\n",
                ctx->Eax, ctx->Ebx, ctx->Ecx, ctx->Edx);
        "ESI: EDI: ctx->Esi, ctx->Edi);
        
        fclose(f);
    �?аписа�?�? основной �?�?ейс
    cpp_tracef("CRASH: 0x%08X, ад�?ес %lu",
               ex_info->ExceptionRecord->ExceptionCode,
               (DWORD)ex_info->ExceptionRecord->ExceptionAddress,
               GetCurrentThreadId());
    
    (опасно, л�?�?�?е заве�?�?и�?�?ся)
    EXCEPTION_CONTINUE_EXECUTION;
    
    �?аве�?�?и�?�? п�?о�?есс
    EXCEPTION_EXECUTE_HANDLER;
install_crash_handler() SetUnhandledExceptionFilter(crash_handler);
    cpp_tracef("Crash handler �?с�?ановлен");
DllMain plugin_init:
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) (ul_reason_for_call DLL_PROCESS_ATTACH) install_crash_handler();
    TRUE;
MiniDump (полн�?й дамп п�?о�?есса)

crash_handler_minidump(EXCEPTION_POINTERS* Созда�?�? minidump
    HANDLE dump_file CreateFileA("lualoader/crash.dmp", GENERIC_WRITE, NULL, 
                                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    (dump_file INVALID_HANDLE_VALUE) MINIDUMP_EXCEPTION_INFORMATION dump_info;
        dump_info.ThreadId GetCurrentThreadId();
        dump_info.ExceptionPointers ex_info;
        dump_info.ClientPointers FALSE;
        
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dump_file,
                          MiniDumpNormal, &dump_info, NULL);
        
        CloseHandle(dump_file);
        cpp_tracef("MiniDump записан: lualoader/crash.dmp");
    3: �?оги�?ование �?�?Р�?�? опасн�?ми опе�?а�?иями

**�?обави�?�? ма�?ке�?�? �?�? �?�?С�?�?:**

cpp_tracef("pause_scripts: �?Х�?�? star_coroutine::set(k);
    star_coroutine �?с�?ановлен");
    
    int*)0x978748;
    0x%08X", (DWORD)&OnAMissionFlag);
    �?�?АСНАЯ �?�?�?РАЦ�?Я
    запис�?�? ScriptSpace[%u]", OnAMissionFlag);
    k;
    �?�?С�?�? ScriptSpace");
    
    scripts_paused.store(true);
    scripts_paused �?ЫХ�?�?");
    **�?ап�?с�?и�?�? смо�?�?е�?�? �?�?ейс:** �?оследняя с�?�?ока покаже�?, где именно к�?а�?.

4: ад�?еса запис�?�?

валиднос�?и ад�?еса
    (IsBadReadPtr((void*)0x978748, sizeof(unsigned int))) cpp_tracef("�?Ш�?�?�?А: 0x978748 невалиден!");
        0;
    index OnAMissionFlag;
    (index >= 0x10000) диапазона (ScriptSpace ~64KB)
        индекс %u вне диапазона!", index);
        target &CTheScripts::ScriptSpace[index];
    (IsBadWritePtr(target, sizeof(bool))) ScriptSpace[%u] 0x%p невалиден!", index, target);
        (ад�?ес 0x%p)", target);
    CTheScripts::ScriptSpace[index] 
    scripts_paused.store(true);
    I) Связ�? заве�?�?ением миссии

Набл�?дение �?�?ейса

[миссия swine в�?полнена]
givemoney +250
�?АУ�?А
[в�?ле�?]
**�?ипо�?еза:** миссии, когда:
в�?звал `destroy` (�?даление peds, vehicles)
Флаг сб�?о�?ен ва�?им кодом)
де�?жа�? �?дал�?нн�?е об�?ек�?�?

гипо�?ез�?

**�? аналоге):**

mission_destroy() cpp_tracef("mission_destroy: на�?ало");
    
    Удаление peds
    for (auto : mission_peds) �?даление %p", ped);
            ped;
        mission_peds.clear();
    
    cpp_tracef("mission_destroy: заве�?�?ено");
**�?сли СРА�?У `mission_destroy`:**
Peds �?дален�?
peds
возобновления, п�?одолжае�? в�?полня�?�?ся) в�?зов USE-AFTER-FREE �?нвалида�?ия �?далении 1:** **userdata** lightuserdata) ме�?а�?абли�?ей:

struct PedWrapper valid;
};


    PedWrapper* wrapper (PedWrapper*)lua_newuserdata(L, sizeof(PedWrapper));
    wrapper->ped wrapper->valid true;
    
    luaL_setmetatable(L, "PedMeta");  �?е�?а�?абли�?а __gc
    
    return PedWrapper* (PedWrapper*)luaL_checkudata(L, 1, "PedMeta");
    (!wrapper->valid) cpp_tracef("remove_ped: �?дал�?н");
        1;
    (is_valid_ped(wrapper->ped)) wrapper->ped;
    �?нвалида�?ия
    __gc ме�?аме�?од
ped_gc(lua_State* (wrapper->valid is_valid_ped(wrapper->ped)) cpp_tracef("ped_gc: ав�?о�?даление wrapper->ped);
        wrapper->ped;
        **�?�?еим�?�?ес�?во:** Lua в�?зове�? `__gc` сбо�?ке м�?со�?а, �?далив J) Финал�?н�?й п�?авил�?н�?й алго�?и�?м С�?ема

**Reload-по�?ок:** �?лаги, памя�?�?
**�?г�?овой (`gameProcessEvent`):** �?б�?аба�?�?вае�? изменяе�? `ScriptSpace`, **Pump-по�?оки:** �?а�?к�?�?�?ся `scripts_paused`, п�?ове�?я�?�? C-�?�?нк�?ии
**�?озобновление:** Сня�?ие �?лага п�?одолжа�?�? мес�?а yield
**Teardown:** Ус�?ановка `teardown_active`, pump-по�?оков, зак�?�?�?ие #### plugin.cpp: �?лаги

<atomic>
<mutex>
<condition_variable>
<set>

Флаги сос�?ояния
scripts_paused{false};        (об�?а�?имо)
scripts_disabled{false};      �?�?кл�?�?ение спавна ск�?ип�?ов
teardown_active{false};       shutdown (необ�?а�?имо)
game_shutting_down{false};    Флаги-зап�?ос�? (для связи reload gameProcessEvent)
Син�?�?ониза�?ия pump-по�?оков
pause_scripts resume_scripts

У�?А�?�?ТЬ СТАРЫ�? он �?епе�?�? gameProcessEvent

Нов�?й: �?ол�?ко �?с�?ановка �?лага-зап�?оса
request_pause() pause_requested.store(true);
    cpp_tracef("request_pause: GetCurrentThreadId());
request_resume() resume_requested.store(true);
    cpp_tracef("request_resume: reload

cpp_tracef("reload: зап�?�?ен paused false;
    
    (!teardown_active.load()) Sleep(10);
        
        ctrl_pressed (GetAsyncKeyState(VK_CONTROL) & 0x8000) (ctrl_pressed !paused) Нажали жд�?м нажа�?, о�?п�?скания");
            
            ((GetAsyncKeyState(VK_CONTROL) 0 {
                Sleep(1);
            }
            
            �?�?п�?с�?или зап�?оси�?�? па�?з�?
            о�?п�?�?ен, зап�?ос па�?з�?");
            request_pause();
            true;
            
        } (!ctrl_pressed paused) �?аз (о�?п�?с�?или) возобновление
            возобновления");
            request_resume();
            заве�?�?�?н");
об�?аба�?�?ваем �?лаги-зап�?ос�? pause_requested.store(false);
        
        cpp_tracef("gameProcessEvent: НАЧА�?�? int*)0x978748;
        
        ад�?еса
        (!IsBadWritePtr(&CTheScripts::ScriptSpace[OnAMissionFlag], false;
            cpp_tracef("gameProcessEvent: сб�?о�?ен");
        �?Ш�?�?�?А OnAMissionFlag");
        }
        
        scripts_disabled.store(true);
        УСТАН�?�?�?�?НА");
    �?�?�?�?�?Н�?�?�?�?Н�?�? GetCurrentThreadId());
        
        восс�?ановлен");
        scripts_disabled.store(false);
        
        CMessages::AddMessageJumpQ(L"Scripts УСТАН�?�?�?�?Н�?");
    ск�?ип�?ов)
    start_lualoder зап�?�?ен)
        pump_owner

active_scripts.insert(L);
    cpp_tracef("register_script: L=%p, всего active_scripts.size());
cpp_tracef("unregister_script: ос�?алос�? active_scripts.size());
    static nres;
        status &nres);
        cpp_tracef("pump_owner: пе�?в�?й status=%d", status);
    this_thread::sleep_for(chrono::milliseconds(1));
        �?а�?за: �?ес�?меим, п�?ос�?о спим
            continue;
        Resume-логика (�?�?�? hook-yield)
        (L1 NULL LUA_TFUNCTION lua_type(L1, -1) 
            LUA_YIELD star_coroutine::get()) 
            for (int i 1; <= args; i++) lua_pushvalue(L1, i);
            
            int nres;
            lua_resume(L1, &nres);
            (status LUA_OK LUA_YIELD) о�?ибка status=%d: %s", 
                           status, lua_tostring(L1, -1));
                break;
            LUA_YIELD lua_status(L1)) %s", 
                           status, lua_tostring(L, (LUA_YIELD 
            (L1 lua_status(L1) !star_coroutine::get())) break;
        script_stopped(L);
    L=%p", (void*)L);
        final_scripts (teardown)

(максим�?м 5 сек)
    
    auto deadline std::chrono::steady_clock::now() + std::chrono::seconds(5);
    
    size_t count active_scripts.size();
        count);
        
        (scripts_cv.wait_until(lock, deadline) std::cv_status::timeout) TIMEOUT! �?с�?алос�? ск�?ип�?ов, п�?ин�?ди�?ел�?ное заве�?�?ение", 
                       active_scripts.size());
            funcs.cpp: wait

wait(lua_State* �?Р�?�?�?Р�?А �?АУ�?Ы/TEARDOWN НАЧА�?�?
    cpp_tracef("wait: па�?за, yield 0);
    teardown, в�?�?од (lua_state_obsolete(L)) obsolete, time п�?ове�?кой па�?з�?
    (time > 0) yield");
            0);
        в�?�?од");
            obsolete step 10) ? 10 time;
        std::this_thread::sleep_for(std::chrono::milliseconds(step));
        -= step;
    load_model_before_avalible


    int REQUEST_MODEL(model_id);
    п�?ове�?кой
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    С�?СТ�?ЯН�?Я 0;
    
    �?А�?�?�?АЦ�?Я АР�?У�?�?НТ�?�?
    РА�?�?ТА С �?�?Р�?�?Ы�?�? �?�?Ъ�?�?ТА�?�?
    ес�?�? Ц�?�?�?�?
    (�?словие) 0;
        
        Рабо�?а
    �?�?�?�?РАТ Р�?�?У�?ЬТАТА
    N;
исп�?авления

**�?�?и�?и�?н�?е (�?�?Я�?АТ�?�?ЬН�? добави�?�? п�?ове�?ки):**

исп�?авлен
�?Р�?�?�?Р�?ТЬ �?икле
(и worker-по�?оке)
�?�?�?А�?�?ТЬ
�?�?�?А�?�?ТЬ

�?�?огов�?й �?еклис�? исп�?авлений

[ ] **Ус�?анови�?�? crash handler** (`SetUnhandledExceptionFilter`)
де�?ал�?ное логи�?ование** `pause_scripts` (�?�? каждой опе�?а�?ии)
**Уб�?а�?�? reload-по�?ока**, пе�?енес�?и `gameProcessEvent`
**Реализова�?�? �?лаги-зап�?ос�?** `scripts_paused`** �?С�? �?икл�?)
**�?сп�?ави�?�? `load_model_before_avalible`** (сдела�?�? yieldable, hook-yield** полнос�?�?�? (�?же сделано, �?беди�?�?ся)
`final_scripts()`** син�?�?ониза�?ией **�?спол�?зова�?�? I/O** (`setvbuf(_IONBF)`)
**�?алида�?ия ад�?есов** (`IsBadWritePtr`)
**�?е�?ей�?и userdata** (оп�?ионал�?но, �?екоменд�?е�?ся)

�?акл�?�?ение

**Наиболее ве�?оя�?ная п�?и�?ина �?Ы�?�?ТА:**

`CTheScripts::ScriptSpace[OnAMissionFlag]`** однов�?еменно violation **Ре�?ение:**

**�?С�?** изменения памя�?и (`ScriptSpace`, `CMessages`) **Т�?�?Ь�?�?** (`gameProcessEvent`)
�?лаги-зап�?ос�?
зап�?ос�? кад�?

**�?осле э�?и�? исп�?авлений:** п�?ек�?а�?и�?�?ся, б�?де�? �?або�?а�?�? с�?абил�?но.