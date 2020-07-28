#pragma once

#include "rules_ini.h"
#include "RulesIniRuleKey.h"

#define GAME_RULES_SECTION_NAME "Game"

// C&C internal debug rules
#define DEBUG_LOGGING_RULE "EnableDebugLogging"

// map rules
#define MAX_BUILD_DISTANCE_RULE "MaxBuildDistance"

#define PREVENT_BUILDING_IN_SHROUD_RULE "PreventBuildingInShroud"

#define ALLOW_BUILDING_BESIDE_WALLS_RULE "AllowBuildingBesideWalls"

#define TIBERIUM_GROWS_RULE "TiberiumGrows"

#define TIBERIUM_SPREADS_RULE "TiberiumSpreads"

#define SLOW_TIBERIUM_RULE "SlowTiberiumGrowthAndSpread"

#define TIBERIUM_GROWTH_RATE_RULE "TiberiumGrowthRate"

#define TIBERIUM_SPREAD_RATE_RULE "TiberiumSpreadRate"

#define TIBERIUM_INFANTRY_DAMAGE_RULE "TiberiumInfantryDamage"

// harvesting rules
#define CREDITS_PER_TIBERIUM_SCOOP_RULE "CreditsPerTiberiumScoop"

#define MAX_HARVESTER_CAPACITY_RULE "MaxHarvesterCapacity"

// factory rules
#define PRODUCTION_STEPS_PER_TICK_RULE "ProductionStepsPerTick"

#define FACTORY_COUNT_STEP_MULTIPLER_RULE "FactoryCountStepMultiplier"

#define TOTAL_PRODUCTION_STEPS_RULE "TotalProductionSteps"

// repair rules
#define UNIT_REPAIR_FACTOR_RULE "UnitRepairFactor"

#define UNIT_REPAIR_STEP_RULE "UnitRepairStrengthStep"

#define AIRCRAFT_REPAIR_FACTOR_RULE "AircraftRepairFactor"

#define AIRCRAFT_REPAIR_STEP_RULE "AircraftRepairStrengthStep"

// misc rules
#define DEFEND_AGAINST_FRIENDLY_FIRE_RULE "DefendAgainstFriendlyFire"

#define TARGET_TREES_RULE "TargetTrees"

#define MCV_REDEPLOY_RULE "McvRedeployable"

#define VISCEROIDS_RULE "SpawnVisceroids"

#define UNITS_INDESTRUCTIBLE_RULE "UnitsAreIndestructible"

#define THREE_POINT_TURNS_RULE "VehiclesDoThreePointTurns"

#define SHOW_BIBS_RULE "ShowBibsOnBuildings"

#define NAME_CIVILIAN_BUILDINGS_RULE "ShowCivilianBuildingNames"

#define CAN_BUY_HELIPAD_ONLY_RULE "HelipadsAndAircraftBoughtSeperately"

#define ONLY_ALLOW_NUKE_IF_PARTS_COLLECTED_RULE "OnlyAllowNukeIfAllPartsHaveBeenCollected"

#define ONE_TIME_NUKE_RULE "OnlyAllowUsingOneNukePerScenario"

#define SURVIVORS_FROM_BUILDINGS_RULE "DestroyedBuildingsHaveSurvivors"

// house specific rules
#define HIDE_TMPL_FROM_GDI_RULE "HideTempleFromGdi"

#define HIDE_OBLI_FROM_GDI_RULE "HideObeliskFromGdi"

#define HIDE_APC_FROM_NOD_RULE "HideApcFromNod"

#define HIDE_MSAM_FROM_NOD_RULE "HideRocketLauncherFromNod"

#define HIDE_HELIPAD_FROM_NOD_RULE "HideHelipadFromNod"

#define HIDE_ADV_COMM_FROM_NOD_RULE "HideAdvancedCommCenterFromNod"

#define ONLY_GDI_CAN_USE_ION_CANNON_RULE "OnlyGdiCanUseIonCannon"

// level specific rules
#define SET_GDI_SCENARIO_2_BUILD_LEVEL_RULE "SetBuildLevelTo1InGdiScenario2"

#define RENAME_TECH_CENTER_TO_PRISON_RULE "RenameTechCenterToPrisionInScenario3"

#define HIDE_E3_FROM_GDI_RULE "HideBazookaFromGdiUntilSenario8"

#define HIDE_MSAM_FROM_GDI_RULE "HideRocketLauncherFromGdiUntilScenario9"

#define HIDE_SBAG_FROM_GDI_RULE "HideSandbagFromGdiUntilScenario9"

#define ALLOW_NOD_TO_BUILD_NUK2_EARLY_RULE "AllowNodToBuildAdvancedPowerInScenario12"

// cheats
#define ALLOW_BUILDING_ALL_FOR_HOUSE_RULE "AllowBuildingAllForCurrentHouse"

#define AUTO_SCATTER_RULE "InfantryAutoScatters"

#define ATTACKER_ADVANTAGE_RULE "GiveAttackersAnAdvantage"

#define SPEEDY_BUILDS_RULE "SpeedyBuilds"

// lookups
#define Read_Build_Distance_Game_Rule() ReadRuleValue<unsigned int>(MAX_BUILD_DISTANCE_RULE_KEY)

#define Read_Credits_Per_Scoop_Rule() ReadRuleValue<unsigned int>(CREDITS_PER_TIBERIUM_SCOOP_RULE_KEY)

#define Read_Harvester_Capacity_Rule() ReadRuleValue<unsigned int>(MAX_HARVESTER_CAPACITY_RULE_KEY)

#define Read_Harvester_Max_Credits_Rule() Read_Credits_Per_Scoop_Rule() * Read_Harvester_Capacity_Rule()

#define Read_Production_Steps_Per_Tick_Rule() ReadRuleValue<unsigned int>(PRODUCTION_STEPS_PER_TICK_RULE_KEY)

#define Read_Factory_Count_Multiplier_Rule() ReadRuleValue<double>(FACTORY_COUNT_STEP_MULTIPLER_RULE_KEY)

#define Read_Total_Production_Steps_Rule() ReadRuleValue<unsigned int>(TOTAL_PRODUCTION_STEPS_RULE_KEY)

#define Read_Unit_Repair_Step_Rule() ReadRuleValue<unsigned int>(UNIT_REPAIR_STEP_RULE_KEY)

#define Read_Aircraft_Repair_Step_Rule() ReadRuleValue<unsigned int>(AIRCRAFT_REPAIR_STEP_RULE_KEY)
