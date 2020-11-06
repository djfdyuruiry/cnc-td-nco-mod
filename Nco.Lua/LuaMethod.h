#pragma once

#include <functional>

#include <lua.hpp>

typedef std::function<int(lua_State*)> LuaMethod;
