#pragma once

#include <vector>

#include <lua.hpp>
#include <ILuaRuntime.h>

bool Initialise_Lua();

bool Execute_Lua_Script(const char* luaScript, bool beQuiet);

bool Execute_Lua_Script(const char* luaScript);

bool Execute_Lua_File(const char* scriptFile);

void Shutdown_Lua();

ILuaRuntime& LuaRuntime();
