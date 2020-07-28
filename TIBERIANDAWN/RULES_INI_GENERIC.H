#pragma once

#include "defines.h"

#include "rules_ini.h"

// generic rules - apply to all types
#define FRIENDLY_NAME_RULE "FriendlyName"

#define BUILD_LEVEL_RULE "BuildLevel"

#define SCENARIO_LEVEL_RULE "ScenarioLevel"

#define PREREQUISITE_RULE "Prerequisite"

#define COST_RULE "Cost"

#define BUILDABLE_RULE "Buildable"

#define FLAMMABLE_RULE "Flammable"

#define SPEED_RULE "Speed"

#define STRENGTH_RULE "Strength"

#define OWNER_RULE "Owner"

#define HOUSES_RULE "Houses"

#define PRIMARY_WEAPON_RULE "PrimaryWeapon"

#define SECONDARY_WEAPON_RULE "SecondaryWeapon"

#define ARMOR_RULE "Armor"

#define AMMO_RULE "Ammo"

#define SIGHT_RANGE_RULE "SightRange"

#define REWARD_RULE "Reward"

#define CRUSHABLE_RULE "Crushable"

#define DETECT_CLOAKED_OBJECTS_RULE "DetectCloakedObjects"

#define LEADER_RULE "Leader"

#define TWO_SHOOTER_RULE "TwoShooter"

#define HAS_THEATER_GFX_RULE "HasTheaterSpecificGraphic"

#define SHOW_NAME_RULE "ShowName"

#define IS_MOD_TYPE_RULE "IsModType"

// shared unit/aircraft rules
#define RATE_OF_TURN_RULE "RateOfTurn"

#define TRANSPORTER_RULE "CanTransportInfantry"

#define TRANSPORT_CAPACITY_RULE "TransportCapacity"

#define DEFAULT_TRANSPORT_CAPACITY 5u

// non-infantry shared rules
#define REPAIRABLE_RULE "Repairable"

#define HAS_CREW_RULE "HasCrew"

#define TURRET_EQUIPPED_RULE "TurretEquipped"

// shared rules
#define Read_Build_Level(section, defaultValue) (unsigned char)ReadRuleValue<unsigned int>(section, BUILD_LEVEL_RULE, defaultValue)

#define Read_Scenario_Level(section, defaultValue) ReadRuleValue<unsigned int>(section, SCENARIO_LEVEL_RULE, defaultValue)

#define Read_Cost(section, defaultValue) ReadRuleValue<unsigned int>(section, COST_RULE, defaultValue)

#define Read_Is_Buildable(section, defaultValue) ReadRuleValue<bool>(section, BUILDABLE_RULE, defaultValue)

#define Read_Is_Flammable(section, defaultValue) ReadRuleValue<bool>(section, FLAMMABLE_RULE, defaultValue)

#define Read_Speed(section, defaultValue) (MPHType)ReadRuleValue<unsigned int>(section, SPEED_RULE, defaultValue)

#define Read_Strength(section, defaultValue) (unsigned short)ReadRuleValue<unsigned int>(section, STRENGTH_RULE, defaultValue)

#define Read_Primary_Weapon(section, defaultValue) ReadRuleValue<WeaponType>(section, PRIMARY_WEAPON_RULE, defaultValue)

#define Read_Secondary_Weapon(section, defaultValue) ReadRuleValue<WeaponType>(section, SECONDARY_WEAPON_RULE, defaultValue)

#define Read_Armor(section, defaultValue) ReadRuleValue<ArmorType>(section, ARMOR_RULE, defaultValue)

#define Read_Ammo(section, defaultValue) ReadRuleValue<int>(section, AMMO_RULE, defaultValue)

#define Read_Sight_Range(section, defaultValue) ReadRuleValue<unsigned int>(section, SIGHT_RANGE_RULE, defaultValue)

#define Read_Reward(section, defaultRewardValue) ReadRuleValue<unsigned int>(section, REWARD_RULE, defaultRewardValue)

#define Read_Detects_Cloak(section, defaultValue) ReadRuleValue<bool>(section, DETECT_CLOAKED_OBJECTS_RULE, defaultValue)

#define Read_Is_Crushable(section, defaultValue) ReadRuleValue<bool>(section, CRUSHABLE_RULE, defaultValue)

#define Read_Is_Transport(section, defaultValue) ReadRuleValue<bool>(section, TRANSPORTER_RULE, defaultValue)

// unit/aircraft shared rules
#define Read_Rate_Of_Turn(section, defaultValue) (unsigned char)ReadRuleValue<unsigned int>(section, RATE_OF_TURN_RULE)
