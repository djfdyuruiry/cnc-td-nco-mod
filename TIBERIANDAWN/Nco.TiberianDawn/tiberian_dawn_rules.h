#pragma once

#include "enhancement_keys.h"
#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnNcoRuntime.h"

// enhancement lookups
#define Read_Rally_Points_Rule() NcoRulesRuntime().ReadRuleValue<bool>(RALLY_POINTS_RULE_KEY)

#define Read_A_Star_Path_Finding_Rule() NcoRulesRuntime().ReadRuleValue<bool>(A_STAR_PATH_FINDING_RULE_KEY)

#define Read_Commando_Guard_Rule() NcoRulesRuntime().ReadRuleValue<bool>(COMMANDO_GUARD_RULE_KEY)

#define Read_Ai_Harvesters_Per_Refinery_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(AI_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Human_Harvesters_Per_Refinery_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Multi_Wall_Length_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(MULTI_WALL_LENGTH_RULE_KEY)

#define Read_Full_Cost_Multi_Wall_Length_Rule() NcoRulesRuntime().ReadRuleValue<bool>(FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY)

// mod lookups
#define ReadNewAircraftCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_AIRCRAFT_COUNT_RULE)

#define ReadAircraftCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(AIRCRAFT_COUNT_RULE_KEY)

#define ReadNewBuildingCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_BUILDING_COUNT_RULE)

#define ReadBuildingCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(BUILDING_COUNT_RULE_KEY)

#define ReadNewBulletCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_BULLET_COUNT_RULE)

#define ReadBulletCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(BULLET_COUNT_RULE_KEY)

#define ReadNewInfantryCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_INFANTRY_COUNT_RULE)

#define ReadInfantryCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(INFANTRY_COUNT_RULE_KEY)

#define ReadNewUnitCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_UNIT_COUNT_RULE)

#define ReadUnitCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(UNIT_COUNT_RULE_KEY)

#define ReadNewWarheadCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_WARHEAD_COUNT_RULE)

#define ReadWarheadCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(WARHEAD_COUNT_RULE_KEY)

#define ReadNewWeaponCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(MOD_RULES_SECTION_NAME, NEW_WEAPON_COUNT_RULE)

#define ReadWeaponCount() (char)NcoRulesRuntime().ReadRuleValue<unsigned int>(WEAPON_COUNT_RULE_KEY)

// game lookups
#define Read_Build_Distance_Game_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(MAX_BUILD_DISTANCE_RULE_KEY)

#define Read_Blossom_Tree_Damage_Game_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(TIBERIUM_BLOSSOM_TREE_DAMAGE_RULE_KEY)

#define Read_Credits_Per_Scoop_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(CREDITS_PER_TIBERIUM_SCOOP_RULE_KEY)

#define Read_Harvester_Capacity_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(MAX_HARVESTER_CAPACITY_RULE_KEY)

#define Read_Harvester_Max_Credits_Rule() Read_Credits_Per_Scoop_Rule() * Read_Harvester_Capacity_Rule()

#define Read_Production_Steps_Per_Tick_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(PRODUCTION_STEPS_PER_TICK_RULE_KEY)

#define ReadFactoryCount_Multiplier_Rule() NcoRulesRuntime().ReadRuleValue<double>(FACTORY_COUNT_STEP_MULTIPLER_RULE_KEY)

#define Read_Total_Production_Steps_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(TOTAL_PRODUCTION_STEPS_RULE_KEY)

#define Read_Unit_Repair_Step_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(UNIT_REPAIR_STEP_RULE_KEY)

#define Read_Aircraft_Repair_Step_Rule() NcoRulesRuntime().ReadRuleValue<unsigned int>(AIRCRAFT_REPAIR_STEP_RULE_KEY)

// difficulty lookups
#define Read_Double_Difficulty_Rule(difficultyName, rule) NcoRulesRuntime().ReadRuleValue<double>(difficultyName, rule)

// superwepaon lookups
#define Read_Superweapon_Rule(superwepon, entry) NcoRulesRuntime().ReadRuleValue<unsigned int>(superwepon, entry)
