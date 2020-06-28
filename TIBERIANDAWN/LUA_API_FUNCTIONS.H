#ifndef LUA_API_FUNCTIONS_H
#define LUA_API_FUNCTIONS_H

#include "lua.hpp"

#include "defines.h"
#include "type.h"

int Lua_Get_Infantry_Rule(lua_State* lua);

int Lua_Set_Infantry_Rule(lua_State* lua);

int Lua_Get_Unit_Rule(lua_State* lua);

int Lua_Set_Unit_Rule(lua_State* lua);

int Lua_Get_Aircraft_Rule(lua_State* lua);

int Lua_Set_Aircraft_Rule(lua_State* lua);

int Lua_Get_Building_Rule(lua_State* lua);

int Lua_Set_Building_Rule(lua_State* lua);

#endif
