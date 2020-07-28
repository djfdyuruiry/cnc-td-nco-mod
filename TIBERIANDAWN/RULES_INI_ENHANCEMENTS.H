#pragma once

#include "rules_ini.h"

#define ENHANCEMENTS_RULES_SECTION_NAME "Enhancements"

#define RALLY_POINTS_RULE "RallyPoints"

#define A_STAR_PATH_FINDING_RULE "AStarPathFinding"

#define COMMANDO_GUARD_RULE "CommandoGuard"

#define AI_HARVESTERS_PER_REFINERY_RULE "AiHarvestersPerRefinery"

#define HUMAN_HARVESTERS_PER_REFINERY_RULE "HumanHarvestersPerRefinery"

#define MULTI_WALL_LENGTH_RULE "MultiWallLength"

#define FULL_COST_MULTI_WALLS_LENGTH_RULE "FullCostMultiWalls"

#define Read_Rally_Points_Rule() ReadRuleValue<bool>(RALLY_POINTS_RULE_KEY)

#define Read_A_Star_Path_Finding_Rule() ReadRuleValue<bool>(A_STAR_PATH_FINDING_RULE_KEY)

#define Read_Commando_Guard_Rule() ReadRuleValue<bool>(COMMANDO_GUARD_RULE_KEY)

#define Read_Ai_Harvesters_Per_Refinery_Rule() ReadRuleValue<unsigned int>(AI_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Human_Harvesters_Per_Refinery_Rule() ReadRuleValue<unsigned int>(HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY)

#define Read_Multi_Wall_Length_Rule() ReadRuleValue<unsigned int>(MULTI_WALL_LENGTH_RULE_KEY)

#define Read_Full_Cost_Multi_Wall_Length_Rule() ReadRuleValue<bool>(FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY)
