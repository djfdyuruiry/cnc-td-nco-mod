#ifndef RULES_INI_H
#define RULES_INI_H

#include "defines.h"
#include "logger.h"

// rules ini sections
#define NCO_RULES_SECTION "NCO"
#define GAME_RULES_SECTION "Game"

// nco config
#define ENABLE_LUA_SCRIPTS_RULE "EnableLuaScripts"
#define ENABLE_LUA_SCRIPTS_RULE_UPPER "ENABLELUASCRIPTS"

#define LUA_SCRIPTS_RULE "LuaScripts"
#define LUA_SCRIPTS_RULE_UPPER "LUASCRIPTS"

// game rules (often scenario or techno type specific)
#define RENAME_TECH_CENTER_TO_PRISON_RULE "RenameTechCenterToPrision"
#define RENAME_TECH_CENTER_TO_PRISON_RULE_UPPER "RENAMETECHCENTERTOPRISION"

// generic rules - apply to all types
#define BUILD_LEVEL_RULE "BuildLevel"
#define BUILD_LEVEL_RULE_UPPER "BUILDLEVEL"

#define SCENARIO_LEVEL_RULE "ScenarioLevel"
#define SCENARIO_LEVEL_RULE_UPPER "SCENARIOLEVEL"

#define PREREQUISITE_RULE "Prerequisite"
#define PREREQUISITE_RULE_UPPER "PREREQUISITE"

#define COST_RULE "Cost"
#define COST_RULE_UPPER "COST"

#define BUILDABLE_RULE "Buildable"
#define BUILDABLE_RULE_UPPER "BUILDABLE"

#define FLAMMABLE_RULE "Flammable"
#define FLAMMABLE_RULE_UPPER "FLAMMABLE"

#define SPEED_RULE "Speed"
#define SPEED_RULE_UPPER "SPEED"

#define STRENGTH_RULE "Strength"
#define STRENGTH_RULE_UPPER "STRENGTH"

#define HOUSES_RULE "Houses"
#define HOUSES_RULE_UPPER "HOUSES"

#define PRIMARY_WEAPON_RULE "PrimaryWeapon"
#define PRIMARY_WEAPON_RULE_UPPER "PRIMARYWEAPON"

#define SECONDARY_WEAPON_RULE "SecondaryWeapon"
#define SECONDARY_WEAPON_RULE_UPPER "SECONDARYWEAPON"

#define AMMO_RULE "Ammo"
#define AMMO_RULE_UPPER "AMMO"

#define SIGHT_RANGE_RULE "SightRange"
#define SIGHT_RANGE_RULE_UPPER "SightRange"

#define RISK_RULE "Risk"
#define RISK_RULE_UPPER "RISK"

#define REWARD_RULE "Reward"
#define REWARD_RULE_UPPER "REWARD"

#define SHOW_NAME_RULE "ShowName"
#define SHOW_NAME_RULE_UPPER "SHOWNAME"

// infantry rules
#define FEMALE_RULE "Female"
#define FEMALE_RULE_UPPER "FEMALE"

#define CRAWLING_RULE "Crawling"
#define CRAWLING_RULE_UPPER "CRAWLING"

#define CAN_CAPTURE_RULE "CanCapture"
#define CAN_CAPTURE_RULE_UPPER "CANCAPTURE"

#define FRAIDY_CAT_RULE "FraidyCat"
#define FRAIDY_CAT_RULE_UPPER "FRAIDYCAT"

#define CIVILIAN_RULE "Civilian"
#define CIVILIAN_RULE_UPPER "CIVILIAN"

#define AVOIDS_TIBERIUM_RULE "AvoidsTiberium"
#define AVOIDS_TIBERIUM_RULE_UPPER "AVOIDSTIBERIUM"

// unit rules
#define CAN_BE_FOUND_IN_CRATE_RULE "CanBeFoundInCrate"
#define CAN_BE_FOUND_IN_CRATE_RULE_UPPER "CANBEFOUNDINCRATE"

#define LIMITED_FACINGS_RULE "LimitedFacings"
#define LIMITED_FACINGS_RULE_UPPER "LIMITEDFACINGS"

#define CAN_CRUSH_INFANTRY_RULE "CanCrushInfantry"
#define CAN_CRUSH_INFANTRY_RULE_UPPER "CANCRUSHINFANTRY"

#define CAN_HARVEST_RULE "CanHarvest"
#define CAN_HARVEST_RULE_UPPER "CANHARVEST"

#define CHUNKY_SHAPE_RULE "ChunkyShape"
#define CHUNKY_SHAPE_RULE_UPPER "CHUNKYSHAPE"

#define RADAR_EQUIPPED_RULE "RadarEquipped"
#define RADAR_EQUIPPED_RULE_UPPER "RADAREQUIPPED"

#define HAS_FIRE_ANIMATION_RULE "HasFireAnimation"
#define HAS_FIRE_ANIMATION_RULE_UPPER "HASFIREANIMATION"

#define LOCK_TURRET_RULE "LockTurret"
#define LOCK_TURRET_RULE_UPPER "LOCKTURRET"

#define HAS_TRACKS_RULE "HasTracks"
#define HAS_TRACKS_RULE_UPPER "HASTRACKS"

#define HUGE_RULE "Huge"
#define HUGE_RULE_UPPER "HUGE"

#define CAN_CLOAK_RULE "CanCloak"
#define CAN_CLOAK_RULE_UPPER "CANCLOAK"

#define CONSTANT_ANIMATION_RULE "ConstantAnimation"
#define CONSTANT_ANIMATION_RULE_UPPER "CONSTANTANIMATION"

#define UNIT_SPEED_RULE "UnitSpeed"
#define UNIT_SPEED_RULE_UPPER "UNITSPEED"

#define RATE_OF_TURN_RULE "RateOfTurn"
#define RATE_OF_TURN_RULE_UPPER "RATEOFTURN"

#define MAX_SIZE_RULE "MaxSize"
#define MAX_SIZE_RULE_UPPER "MAXSIZE"

// building rules
#define BIDDED_RULE "Bibbed"
#define BIDDED_RULE_UPPER "BIBBED"

#define WALL_RULE "Wall"
#define WALL_RULE_UPPER "WALL"

#define FACTORY_RULE "Factory"
#define FACTORY_RULE_UPPER "FACTORY"

#define SIMPLE_DAMAGE_RULE "SimpleDamage"
#define SIMPLE_DAMAGE_RULE_UPPER "SIMPLEDAMAGE"

#define STURDY_RULE "Sturdy"
#define STURDY_RULE_UPPER "STURDY"

#define CAPTUREABLE_RULE "Captureable"
#define CAPTUREABLE_RULE_UPPER "CAPTUREABLE"

#define FIXED_SPEED_ANIMATION_RULE "FixedSpeedAnimation"
#define FIXED_SPEED_ANIMATION_RULE_UPPER "FIXEDSPEEDANIMATION"

#define UNSELLABLE_RULE "Unsellable"
#define UNSELLABLE_RULE_UPPER "UNSELLABLE"

#define FACTORY_TYPE_RULE "FactoryType"
#define FACTORY_TYPE_RULE_UPPER "FACTORYTYPE"

#define CAN_BE_ENTERED_RULE "CanBeEntered"
#define CAN_BE_ENTERED_RULE_UPPER "CANBEENTERED"

#define STORAGE_CAPACITY_RULE "StorageCapacity"
#define STORAGE_CAPACITY_RULE_UPPER "STORAGECAPACITY"

#define POWER_OUTPUT_RULE "PowerOutput"
#define POWER_OUTPUT_RULE_UPPER "POWEROUTPUT"

#define POWER_INPUT_RULE "PowerInput"
#define POWER_INPUT_RULE_UPPER "POWERINPUT"

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
#define Read_Build_Level(section, defaultValue) (unsigned char)Read_Int_From_Rules_Ini(section, BUILD_LEVEL_RULE, defaultValue, 1, 99)

#define Read_Scenario_Level(section, defaultValue) Read_Int_From_Rules_Ini(section, SCENARIO_LEVEL_RULE, defaultValue, 1, 99)

#define Read_Cost(section, defaultValue) Read_Int_From_Rules_Ini(section, COST_RULE, defaultValue, 0, INT_MAX)

#define Read_Is_Buildable(section, defaultValue) Read_Bool_From_Rules_Ini(section, BUILDABLE_RULE, defaultValue)

#define Read_Is_Flammable(section, defaultValue) Read_Bool_From_Rules_Ini(section, FLAMMABLE_RULE, defaultValue)

#define Read_Speed(section, defaultValue) (MPHType)Read_Int_From_Rules_Ini(section, SPEED_RULE, defaultValue, 1, UCHAR_MAX)

#define Read_Strength(section, defaultValue) (unsigned short)Read_Int_From_Rules_Ini(section, STRENGTH_RULE, defaultValue, 1, USHRT_MAX)

#define Read_Primary_Weapon(section, defaultValue) Read_Weapon_Type_From_Rules_Ini(section, PRIMARY_WEAPON_RULE, defaultValue)

#define Read_Secondary_Weapon(section, defaultValue) Read_Weapon_Type_From_Rules_Ini(section, SECONDARY_WEAPON_RULE, defaultValue)

#define Read_Sight_Range(section, defaultValue) Read_Int_From_Rules_Ini(section, SIGHT_RANGE_RULE, defaultValue, 0, INT_MAX)

#define Read_Risk_Reward(section, defaultRiskValue, defaultRewardValue) Read_Int_From_Rules_Ini(section, RISK_RULE, defaultRiskValue, 0, INT_MAX), \
 Read_Int_From_Rules_Ini(section, REWARD_RULE, defaultRewardValue, 0, INT_MAX)

// infantry rules
#define Read_Is_Female(section, defaultValue) Read_Bool_From_Rules_Ini(section, FEMALE_RULE, defaultValue)

#define Read_Is_Crawling(section, defaultValue) Read_Bool_From_Rules_Ini(section, CRAWLING_RULE, defaultValue)

#define Read_Can_Capture_Buildings(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_CAPTURE_RULE, defaultValue)

#define Read_Is_Fraidy_Cat(section, defaultValue) Read_Bool_From_Rules_Ini(section, FRAIDY_CAT_RULE, defaultValue)

#define Read_Is_Civilian(section, defaultValue) Read_Bool_From_Rules_Ini(section, CIVILIAN_RULE, defaultValue)

#define Read_Avoids_Tiberium(section, defaultValue) Read_Bool_From_Rules_Ini(section, AVOIDS_TIBERIUM_RULE, defaultValue)

// unit rules
#define Read_Can_Cloak(section, defaultValue) Read_Bool_From_Rules_Ini(section, "CanCloak", defaultValue)

// building rules
#define Read_Is_Captureable(section, defaultValue) Read_Bool_From_Rules_Ini(section, "Captureable", defaultValue)

#define Read_Storage_Capacity(section, defaultValue) (unsigned)Read_Int_From_Rules_Ini(section, "StorageCapacity", defaultValue, 0, INT_MAX)

#define Read_Power_Output(section, defaultValue) Read_Int_From_Rules_Ini(section, "PowerOutput", defaultValue, 0, INT_MAX)

#define Read_Power_Input(section, defaultValue) Read_Int_From_Rules_Ini(section, "PowerInput", defaultValue, 0, INT_MAX)

// NCO config
bool Rules_Ini_Failed_Validation();

LogLevel Current_Log_Level();

bool Lua_Is_Enabled();

LuaScripts Rules_Get_Lua_Scripts();

#endif
