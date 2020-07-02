#ifndef RULES_INI_H
#define RULES_INI_H

#include "logger.h"

#include "rules_ini_aircraft.h"
#include "rules_ini_building.h"
#include "rules_ini_game.h"
#include "rules_ini_generic.h"
#include "rules_ini_infantry.h"
#include "rules_ini_unit.h"

// rules ini sections
#define NCO_RULES_SECTION "NCO"
#define GAME_RULES_SECTION "Game"

// nco config
#define ENABLE_LUA_SCRIPTS_RULE "EnableLuaScripts"
#define ENABLE_LUA_SCRIPTS_RULE_UPPER "ENABLELUASCRIPTS"

#define LUA_SCRIPTS_RULE "LuaScripts"
#define LUA_SCRIPTS_RULE_UPPER "LUASCRIPTS"

char* Read_Rules_Ini();

int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive,
	int valueToAllowAlways
);
int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive
);

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue,
	const char* validValues[],
	int validValueCount
);

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue
);

bool Read_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool defaultValue
);

// NCO config
bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

void Set_Current_Log_Level(LogLevel level);

bool Lua_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

#endif
