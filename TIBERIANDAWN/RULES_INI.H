#ifndef RULES_INI_H
#define RULES_INI_H

#include "logger.h"

#include "./lua.h"

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

int Read_Optional_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
);

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

double Read_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	double defaultValue,
	double minValueInclusive,
	double maxValueInclusive
);

double Read_Optional_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
);

char* Read_Optional_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound,
	bool isForConversion
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

bool Read_Optional_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
);

bool Read_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool defaultValue
);

#define Read_Unsigned_Int_From_Rules_Ini(section, entry, defaultValue, minValue, maxValue) (unsigned int)Read_Int_From_Rules_Ini(section, entry, defaultValue, minValue, maxValue)

#define Read_Unsigned_Bool_From_Rules_Ini(section, entry, defaultValue) (unsigned)Read_Bool_From_Rules_Ini(section, entry, defaultValue)

// NCO config
void Rules_Ini_Failed_Validation(bool value);

bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

void Set_Current_Log_Level(LogLevel level);

bool Lua_Is_Enabled();

bool Lua_Console_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

int Rules_Get_Game_Tick_Interval_In_Ms();

#endif
