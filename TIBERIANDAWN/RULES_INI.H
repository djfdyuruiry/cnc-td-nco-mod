#pragma once

#include "logger.h"
#include "lua.h"
#include "rules_cache_key.h"
#include "RulesIni.h"
#include "RulesIniReader.h"

// rules ini sections
#define NCO_RULES_SECTION_NAME "NCO"

// nco config
#define ENABLE_LUA_SCRIPTS_RULE "EnableLuaScripts"
#define ENABLE_LUA_SCRIPTS_RULE_UPPER "ENABLELUASCRIPTS"

#define LUA_SCRIPTS_RULE "LuaScripts"
#define LUA_SCRIPTS_RULE_UPPER "LUASCRIPTS"

#define ENABLE_LUA_CONSOLE_RULE "EnableLuaConsole"
#define ENABLE_LUA_CONSOLE_RULE_UPPER "ENABLELUACONSOLE"

#define GAME_TICK_INTERVAL_IN_MS_RULE "GameTickIntervalInMs"
#define GAME_TICK_INTERVAL_IN_MS_RULE_UPPER "LUASCRIPTS"

void Ensure_Rules_Ini_Is_Loaded();

RulesIniReader& Get_Rules_Reader();

//NCO config
void Rules_Ini_Failed_Validation(bool value);

bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

void Set_Current_Log_Level(LogLevel level);

char* Current_Log_Path();

bool Lua_Is_Enabled();

bool Lua_Console_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

int Rules_Get_Game_Tick_Interval_In_Ms();
