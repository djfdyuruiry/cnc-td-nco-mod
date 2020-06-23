#ifndef RULES_INI_H
#define RULES_INI_H

#include "defines.h"
#include "logger.h"

// generic rules - apply to all types
#define COST_RULE "Cost"
#define COST_RULE_UPPER "COST"

#define BUILDABLE_RULE "Buildable"
#define BUILDABLE_RULE_UPPER "BUILDABLE"

#define FLAMMABLE_RULE "Flammable"
#define FLAMMABLE_RULE_UPPER "FLAMMABLE"

#define MAX_SPEED_RULE "MaxSpeed"
#define MAX_SPEED_RULE_UPPER "MAXSPEED"

#define MAX_STRENGTH_RULE "MaxStrength"
#define MAX_STRENGTH_RULE_UPPER "MAXSTRENGTH"

#define OWNABLE_RULE "Ownable"
#define OWNABLE_RULE_UPPER "OWNABLE"

#define PRIMARY_WEAPON_RULE "PrimaryWeapon"
#define PRIMARY_WEAPON_RULE_UPPER "PRIMARYWEAPON"

#define SECONDARY_WEAPON_RULE "SecondaryWeapon"
#define SECONDARY_WEAPON_RULE_UPPER "SECONDARYWEAPON"

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

int Read_Prerequisite(
	const char* section,
	StructType defaultValue
);

int Read_House_List_From_Rules_Ini(
	const char* section,
	int defaultValue,
	const char* defaultValueAsString
);

WeaponType Read_Weapon_Type_From_Rules_Ini(
	const char* section,
	const char* entry,
	WeaponType defaultValue
);

// shared rules
#define Read_Build_Level(section, defaultValue) (unsigned char)Read_Int_From_Rules_Ini(section, "BuildLevel", defaultValue, 1, 99)

#define Read_Scenario_Level(section, defaultValue) Read_Int_From_Rules_Ini(section, "ScenarioLevel", defaultValue, 1, 99)

#define Read_Cost(section, defaultValue) Read_Int_From_Rules_Ini(section, "Cost", defaultValue, 0, INT_MAX)

#define Read_Is_Buildable(section, defaultValue) Read_Bool_From_Rules_Ini(section, "Buildable", defaultValue)

#define Read_Max_Speed(section, defaultValue) (MPHType)Read_Int_From_Rules_Ini(section, "MaxSpeed", defaultValue, 1, UCHAR_MAX)

#define Read_Strength(section, defaultValue) (unsigned short)Read_Int_From_Rules_Ini(section, "Strength", defaultValue, 1, USHRT_MAX)

#define Read_Primary_Weapon(section, defaultValue) Read_Weapon_Type_From_Rules_Ini(section, PRIMARY_WEAPON_RULE, defaultValue)

#define Read_Secondary_Weapon(section, defaultValue) Read_Weapon_Type_From_Rules_Ini(section, "SecondaryWeapon", defaultValue)

// infantry rules
#define Read_Is_Fraidy_Cat(section, defaultValue) Read_Bool_From_Rules_Ini(section, "FraidyCat", defaultValue)

#define Read_Can_Capture_Buildings(section, defaultValue) Read_Bool_From_Rules_Ini(section, "CanCaptureBuildings", defaultValue)

// unit rules
#define Read_Can_Cloak(section, defaultValue) Read_Bool_From_Rules_Ini(section, "CanCloak", defaultValue)

// building rules
#define Read_Is_Capturable(section, defaultValue) Read_Bool_From_Rules_Ini(section, "Capturable", defaultValue)

#define Read_Is_Flammable(section, defaultValue) Read_Bool_From_Rules_Ini(section, "Flammable", defaultValue)

#define Read_Storage_Capacity(section, defaultValue) (unsigned)Read_Int_From_Rules_Ini(section, "StorageCapacity", defaultValue, 0, INT_MAX)

#define Read_Power_Output(section, defaultValue) Read_Int_From_Rules_Ini(section, "PowerOutput", defaultValue, 0, INT_MAX)

#define Read_Power_Input(section, defaultValue) Read_Int_From_Rules_Ini(section, "PowerInput", defaultValue, 0, INT_MAX)

// NCO config
bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

bool Lua_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

#endif
