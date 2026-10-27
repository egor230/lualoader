// Заглушки игрового слоя для стенда планировщика (MA_SCHED_UNIT_TEST).
// Повторяют ровно те имена, которые ma_sched.cpp берёт из funcs.h/diag.h/ma_tasks.h.
#ifndef MA_SCHED_UNIT_STUBS_H
#define MA_SCHED_UNIT_STUBS_H

#include <lua.hpp>
#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <list>
#include <string>

void funs(lua_State* L);
void writelog(const char* s);
void dellod();
int destroy(lua_State* L);
void stop_mission_watch();
void lua_state_unstamp(lua_State* L);

extern int gGameState;   // в игре/меню (в плагине — CGame.h)
extern std::list<lua_State*> luastate;
extern std::list<char*> listfile;
extern std::atomic<bool> teardown_active;
extern std::atomic<bool> scripts_paused;

struct ma_task_base_stub {
    const char* name = "stub";
};

struct ma_task_registry_stub {
    ma_task_base_stub* find_task(const char*) { return nullptr; }
};
ma_task_registry_stub& ma_tasks();

namespace ma_diag {
    void set_thread_name(const char* name);
    void lua_error_note(const char* fmt, ...);
}

#endif
