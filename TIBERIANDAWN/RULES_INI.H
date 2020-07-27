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

template<class T> T ReadRuleValue(SectionName section, RuleName rule);

template<class T> T ReadRuleValue(RulesIniRuleKey key);

#define BuildDifficultySection(name, firepower, groundspeed, airspeed, armor, rof, cost, repairDelay, buildDelay, buildSpeed, buildSlowdown) \
	RulesIniSection::BuildSection(name)\
		.WithDefaultType(DOUBLE_RULE)\
		.WithRules([](IRulesIniSection& s) {\
			s << FIREPOWER_DIFFICULTY_RULE << firepower                         \
			  << GROUNDSPEED_DIFFICULTY_RULE << groundspeed                     \
			  << AIRSPEED_DIFFICULTY_RULE << airspeed                           \
			  << ARMOR_DIFFICULTY_RULE << armor                                 \
			  << RATE_OF_FIRE_DIFFICULTY_RULE << rof                            \
			  << COST_DIFFICULTY_RULE << cost                                   \
			  << REPAIR_DELAY_DIFFICULTY_RULE << repairDelay                    \
			  << BUILD_DELAY_DIFFICULTY_RULE << buildDelay                      \
			  << BUILD_SPEED_DIFFICULTY_RULE << buildSpeed                      \
			  << BUILD_SLOWDOWN_DIFFICULTY_RULE << BOOL_RULE << buildSlowdown;  \
		})

#define BuildSuperweaponSection(name, rechargeTime)               \
	RulesIniSection::BuildSection(name)                           \
		.WithDefaultType(UNSIGNED_INT_RULE)                       \
		.WithRules([](IRulesIniSection& s) {                      \
			s << SUPERWEAPON_RECHARGE_TIME_RULE << rechargeTime;  \
		})                                                        \

//NCO config
void MarkRulesIniAsInvalid();

bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

void Set_Current_Log_Level(LogLevel level);

char* Current_Log_Path();

bool Lua_Is_Enabled();

bool Lua_Console_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

int Rules_Get_Game_Tick_Interval_In_Ms();
