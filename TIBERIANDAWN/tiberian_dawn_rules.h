#pragma once

#include <RulesIniRuleKey.h>

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnNcoRuntime.h"

// enhancement lookups
#define Read_Rally_Points_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(RALLY_POINTS_RULE_KEY)

#define Read_A_Star_Path_Finding_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(A_STAR_PATH_FINDING_RULE_KEY)

#define Read_Commando_Guard_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(COMMANDO_GUARD_RULE_KEY)

#define Read_Ai_Harvesters_Per_Refinery_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(AI_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Human_Harvesters_Per_Refinery_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Multi_Wall_Length_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MULTI_WALL_LENGTH_RULE_KEY)

#define Read_Full_Cost_Multi_Wall_Length_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY)

// mod lookups
#define Read_New_Aircraft_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_AIRCRAFT_COUNT_RULE)

#define Read_Aircraft_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(AIRCRAFT_COUNT_RULE_KEY)

#define Read_New_Building_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_BUILDING_COUNT_RULE)

#define Read_Building_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(BUILDING_COUNT_RULE_KEY)

#define Read_New_Bullet_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_BULLET_COUNT_RULE)

#define Read_Bullet_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(BULLET_COUNT_RULE_KEY)

#define Read_New_Infantry_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_INFANTRY_COUNT_RULE)

#define Read_Infantry_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(INFANTRY_COUNT_RULE_KEY)

#define Read_New_Unit_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_UNIT_COUNT_RULE)

#define Read_Unit_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(UNIT_COUNT_RULE_KEY)

#define Read_New_Warhead_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_WARHEAD_COUNT_RULE)

#define Read_Warhead_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(WARHEAD_COUNT_RULE_KEY)

#define Read_New_Weapon_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_WEAPON_COUNT_RULE)

#define Read_Weapon_Count() (char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(WEAPON_COUNT_RULE_KEY)

// game lookups
#define Read_Build_Distance_Game_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MAX_BUILD_DISTANCE_RULE_KEY)

#define Read_Credits_Per_Scoop_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(CREDITS_PER_TIBERIUM_SCOOP_RULE_KEY)

#define Read_Harvester_Capacity_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(MAX_HARVESTER_CAPACITY_RULE_KEY)

#define Read_Harvester_Max_Credits_Rule() Read_Credits_Per_Scoop_Rule() * Read_Harvester_Capacity_Rule()

#define Read_Production_Steps_Per_Tick_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(PRODUCTION_STEPS_PER_TICK_RULE_KEY)

#define Read_Factory_Count_Multiplier_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<double>(FACTORY_COUNT_STEP_MULTIPLER_RULE_KEY)

#define Read_Total_Production_Steps_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(TOTAL_PRODUCTION_STEPS_RULE_KEY)

#define Read_Unit_Repair_Step_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(UNIT_REPAIR_STEP_RULE_KEY)

#define Read_Aircraft_Repair_Step_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(AIRCRAFT_REPAIR_STEP_RULE_KEY)

// difficulty lookups
#define Read_Double_Difficulty_Rule(difficultyName, rule) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<double>(difficultyName, rule)

// ai lookups
#define Read_Unsigned_Int_Ai_Rule(entry) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(AI_SECTION_NAME, entry)

#define Read_Bool_Ai_Rule(entry) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(AI_SECTION_NAME, entry)

#define Read_Ratio_Ai_Rule(entry) Read_Unsigned_Int_Ai_Rule(entry)

#define Read_Fraction_Ai_Rule(entry) Read_Unsigned_Int_Ai_Rule(entry)

// iq lookups
#define Read_Max_Iq_Rule() TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(IQ_SECTION_NAME, MAX_IQ_RULE)

#define Read_Iq_Rule(entry) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(IQ_SECTION_NAME, entry)

// weapon lookups
#define Read_Weapon_Damage(bulletType, defaultValue) (unsigned char) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(bulletType, WEAPON_DAMAGE_RULE, defaultValue)

#define Read_Weapon_Rate_Of_Fire(bulletType, defaultValue) (unsigned char) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(bulletType, WEAPON_RATE_OF_FIRE_RULE, defaultValue)

#define Read_Weapon_Range(bulletType, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(bulletType, WEAPON_RANGE_RULE, defaultValue)

// generic lookups
#define Read_Build_Level(section, defaultValue) (unsigned char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, BUILD_LEVEL_RULE, defaultValue)

#define Read_Scenario_Level(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, SCENARIO_LEVEL_RULE, defaultValue)

#define Read_Cost(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, COST_RULE, defaultValue)

#define Read_Is_Buildable(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, BUILDABLE_RULE, defaultValue)

#define Read_Is_Flammable(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, FLAMMABLE_RULE, defaultValue)

#define Read_Speed(section, defaultValue) (MPHType)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, SPEED_RULE, defaultValue)

#define Read_Strength(section, defaultValue) (unsigned short)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, STRENGTH_RULE, defaultValue)

#define Read_Primary_Weapon(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<WeaponType>(section, PRIMARY_WEAPON_RULE, defaultValue)

#define Read_Secondary_Weapon(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<WeaponType>(section, SECONDARY_WEAPON_RULE, defaultValue)

#define Read_Armor(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<ArmorType>(section, ARMOR_RULE, defaultValue)

#define Read_Ammo(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<int>(section, AMMO_RULE, defaultValue)

#define Read_Sight_Range(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, SIGHT_RANGE_RULE, defaultValue)

#define Read_Reward(section, defaultRewardValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, REWARD_RULE, defaultRewardValue)

#define Read_Detects_Cloak(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, DETECT_CLOAKED_OBJECTS_RULE, defaultValue)

#define Read_Is_Crushable(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CRUSHABLE_RULE, defaultValue)

#define Read_Is_Transport(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, TRANSPORTER_RULE, defaultValue)

// infantry lookups
#define Read_Is_Female(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, FEMALE_RULE, defaultValue)

#define Read_Is_Crawling(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CRAWLING_RULE, defaultValue)

#define Read_Can_Capture_Buildings(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_CAPTURE_RULE, defaultValue)

#define Read_Is_Fraidy_Cat(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, FRAIDY_CAT_RULE, defaultValue)

#define Read_Is_Civilian(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CIVILIAN_RULE, defaultValue)

#define Read_Avoids_Tiberium(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, AVOIDS_TIBERIUM_RULE, defaultValue)

// unit/aircraft lookups
#define Read_Rate_Of_Turn(section, defaultValue) (unsigned char)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, RATE_OF_TURN_RULE, defaultValue)

// unit lookups
#define Read_Can_Cloak(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_CLOAK_RULE, defaultValue)

#define Read_Can_Be_Found_In_Crate(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_BE_FOUND_IN_CRATE_RULE, defaultValue)

#define Read_Limited_Facings(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, LIMITED_FACINGS_RULE, defaultValue)

#define Read_Can_Crush_Infantry(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_CRUSH_INFANTRY_RULE, defaultValue)

#define Read_Can_Harvest(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_HARVEST_RULE, defaultValue)

#define Read_Radar_Equipped(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, RADAR_EQUIPPED_RULE, defaultValue)

#define Read_Has_Fire_Animation(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, HAS_FIRE_ANIMATION_RULE, defaultValue)

#define Read_Lock_Turret(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, LOCK_TURRET_RULE, defaultValue)

#define Read_Has_Tracks(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, HAS_TRACKS_RULE, defaultValue)

#define Read_Is_Huge(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, HUGE_RULE, defaultValue)

#define Read_Has_Constant_Animation(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CONSTANT_ANIMATION_RULE, defaultValue)

#define Read_Unit_Speed(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<SpeedType>(section, UNIT_SPEED_RULE, defaultValue)

// aircraft lookups
#define Read_Cant_Hover(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CANT_HOVER_RULE, defaultValue)

#define Read_Has_Rotor(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, HAS_ROTOR_RULE, defaultValue)

#define Read_Can_Land(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAN_LAND_RULE, defaultValue)

// building lookups
#define Read_Is_Bibbed(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, BIBBED_RULE, defaultValue)

#define Read_Is_Captureable(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, CAPTUREABLE_RULE, defaultValue)

#define Read_Storage_Capacity(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, STORAGE_CAPACITY_RULE, defaultValue)

#define Read_Power_Output(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, POWER_OUTPUT_RULE, defaultValue)

#define Read_Power_Input(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<unsigned int>(section, POWER_INPUT_RULE, defaultValue)

#define Read_Factory_Type(section, defaultValue) (RTTIType)TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<FactoryType>(section, FACTORY_TYPE_RULE, defaultValue)

#define Read_Is_Wall(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, WALL_RULE, defaultValue)

#define Read_Is_Factory(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, FACTORY_RULE, defaultValue)

#define Read_Use_Simple_Damage(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, SIMPLE_DAMAGE_RULE, defaultValue)

#define Read_Is_Fixed_Speed(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, FIXED_SPEED_ANIMATION_RULE, defaultValue)

#define Read_Is_Unsellable(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, UNSELLABLE_RULE, defaultValue)

#define Read_Is_Sturdy(section, defaultValue) TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().ReadRuleValue<bool>(section, STURDY_RULE, defaultValue)
