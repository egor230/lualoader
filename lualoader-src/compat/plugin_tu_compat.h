// Шапка совместимости ТОЛЬКО для единицы трансляции plugin.cpp.
//
// Подключается через -include plugin_tu_compat.h (см. build.sh), сам plugin.cpp не меняется.
// Нужна, потому что plugin.cpp писался под Lua 5.1 и под старую версию Plugin-SDK:
//
//  1) std::experimental::filesystem  — libstdc++ >= 9 experimental-реализацию выпилил,
//     оставляем алиас на std::filesystem (plugin.cpp:124, 186).
//  2) CAnimManager                    — funcs.cpp регистрирует CAnimManager.h, а
//     plugin.cpp дёргает CAnimManager::BlendAnimation (plugin.cpp:509).
// 3) lua_resume(L, from, narg)       — сигнатура до-Lua-5.2; в Lua 5.4 четыре аргумента
//     (plugin.cpp:146,162,166,170,761).
//  4) m_nCurrentPage                  — в новом Plugin-SDK поле переименовано в
//     m_nCurrentMenuPage; чинится флагом -D в build.sh (макрос, а не правка plugin.cpp).
//  5) search / std::thread(search)    — см. пункт 0 выше (переименование на время
//     разбора <algorithm>).
//
// Определения из п.1-3 не имеют внешней линковки (static inline / алиас пространства имён),
// поэтому на funcs.cpp/register.cpp не влияют.


#ifndef LUALOADER_PLUGIN_TU_COMPAT_H
#define LUALOADER_PLUGIN_TU_COMPAT_H

// 0) РАНЬШЕ здесь был хак «переименовать search на время разбора <algorithm>».
// Он был нужен, потому что старая версия plugin.cpp определяла глобальную функцию
// search(), и на `std::thread(search)` неимя-поиск видел {::search, std::search,
// std::search(pstl)} — CTAD не мог вывести аргумент, и сборка падала. Сейчас в plugin.cpp
// функция называется search_scripts(), глобальной search() нет, и хак только вредит:
// <functional>, который тянет DynAddress.h из Plugin-SDK, тянет std::search ПОСЛЕ
// #undef и падал с «no member named 'search' in namespace 'std'».

#include <windows.h>
#include <winuser.h>
#include <thread>
#include <string>
#include <filesystem>

// 1) Lua-loader API 5.1/5.2 -> C++17 std::filesystem
namespace std {
namespace experimental {
namespace filesystem = ::std::filesystem;
}
}

// 2) SDK-заголовок, используемый plugin.cpp
#include "CAnimManager.h"

// 3) lua_resume(L, from, narg) — сигнатура до-Lua-5.2; в Lua 5.4 четыре аргумента
//     (plugin.cpp:146,162,166,170,761). Для C++ добавляем перегрузку.
//     Второй аргумент во всех вызовах plugin.cpp — NULL либо lua_State*, поэтому
//     достаточно ровно одной перегрузки с 3 аргументами (две были бы неоднозначны).
#include <lua.hpp>
static inline int lua_resume(lua_State* L, lua_State* from, int narg) {
	return lua_resume(L, from, narg, nullptr);
}

#endif // LUALOADER_PLUGIN_TU_COMPAT_H
