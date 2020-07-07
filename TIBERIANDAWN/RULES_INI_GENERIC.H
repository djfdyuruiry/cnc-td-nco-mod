#ifndef RULES_INI_GENERIC
#define RULES_INI_GENERIC

#include "defines.h"

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

#define ARMOR_RULE "Armor"
#define ARMOR_RULE_UPPER "ARMOR"

#define AMMO_RULE "Ammo"
#define AMMO_RULE_UPPER "AMMO"

#define SIGHT_RANGE_RULE "SightRange"
#define SIGHT_RANGE_RULE_UPPER "SIGHTRANGE"

#define REWARD_RULE "Reward"
#define REWARD_RULE_UPPER "REWARD"

#define CRUSHABLE_RULE "Crushable"
#define CRUSHABLE_RULE_UPPER "CRUSHABLE"

#define DETECT_CLOAKED_OBJECTS_RULE "DetectCloakedObjects"
#define DETECT_CLOAKED_OBJECTS_RULE_UPPER "DETECTCLOAKEDOBJECTS"

// virtual rule used in game logic, not the type itself
#define SHOW_NAME_RULE "ShowName"
#define SHOW_NAME_RULE_UPPER "SHOWNAME"

// virtual rule exposed by the Lua API only
#define FRIENDLY_NAME_RULE "FriendlyName"
#define FRIENDLY_NAME_RULE_UPPER "FRIENDLYNAME"

#define SHARED_RULES BUILD_LEVEL_RULE, \
    SCENARIO_LEVEL_RULE, \
    PREREQUISITE_RULE, \
    COST_RULE, \
    BUILDABLE_RULE, \
    FLAMMABLE_RULE, \
    SPEED_RULE, \
    STRENGTH_RULE, \
    HOUSES_RULE, \
    PRIMARY_WEAPON_RULE, \
    SECONDARY_WEAPON_RULE, \
    ARMOR_RULE, \
    AMMO_RULE, \
    SIGHT_RANGE_RULE, \
    REWARD_RULE, \
    CRUSHABLE_RULE, \
    DETECT_CLOAKED_OBJECTS_RULE, \
    FRIENDLY_NAME_RULE

#define SHARED_RULE_COUNT (unsigned) 18

// shared unit/aircraft rules
#define RATE_OF_TURN_RULE "RateOfTurn"
#define RATE_OF_TURN_RULE_UPPER "RATEOFTURN"

#define TRANSPORTER_RULE "CanTransportInfantry"
#define TRANSPORTER_RULE_UPPER "CANTRANSPORTINFANTRY"

#define TRANSPORT_CAPACITY_RULE "TransportCapacity"
#define TRANSPORT_CAPACITY_RULE_UPPER "TRANSPORTCAPACITY"

#define SHARED_UNIT_AIRCRAFT_RULES RATE_OF_TURN_RULE, \
    TRANSPORTER_RULE, \
    TRANSPORT_CAPACITY_RULE

#define SHARED_UNIT_AIRCRAFT_RULE_COUNT 3

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

#define Read_Armor(section, defaultValue) Read_Armor_Type_From_Rules_Ini(section, ARMOR_RULE, defaultValue)

#define Read_Ammo(section, defaultValue) Read_Int_From_Rules_Ini(section, AMMO_RULE, defaultValue, -1, INT_MAX)

#define Read_Sight_Range(section, defaultValue) Read_Int_From_Rules_Ini(section, SIGHT_RANGE_RULE, defaultValue, 0, INT_MAX)

#define Read_Reward(section, defaultRewardValue) Read_Int_From_Rules_Ini(section, REWARD_RULE, defaultRewardValue, 0, INT_MAX)

#define Read_Detects_Cloak(section, defaultValue) Read_Bool_From_Rules_Ini(section, DETECT_CLOAKED_OBJECTS_RULE, defaultValue)

#define Read_Is_Crushable(section, defaultValue) Read_Bool_From_Rules_Ini(section, CRUSHABLE_RULE, defaultValue)

#define Read_Is_Transport(section, defaultValue) Read_Bool_From_Rules_Ini(section, TRANSPORTER_RULE, defaultValue)

// unit/aircraft shared rules
#define Read_Rate_Of_Turn(section, defaultValue) (unsigned char)Read_Int_From_Rules_Ini(section, RATE_OF_TURN_RULE, defaultValue, 0, UCHAR_MAX)

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

ArmorType Read_Armor_Type_From_Rules_Ini(
    const char* section,
    const char* entry,
    ArmorType defaultValue
);

#endif
