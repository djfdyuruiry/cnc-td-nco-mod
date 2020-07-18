#ifndef RULES_INI_ENHANCEMENTS_H
#define RULES_INI_ENHANCEMENTS_H

#include "rules_ini.h"

#define ENHANCEMENTS_RULES_SECTION_NAME "Enhancements"

#define RALLY_POINTS_RULE "RallyPoints"
#define RALLY_POINTS_RULE_UPPER "RALLYPOINTS"
#define RALLY_POINTS_RULE_TYPE "bool"

#define MULTI_WALL_LENGTH_RULE "MultiWallLength"
#define MULTI_WALL_LENGTH_RULE_UPPER "MULTIWALLLENGTH"
#define MULTI_WALL_LENGTH_RULE_TYPE "int"

#define FULL_COST_MULTI_WALLS_LENGTH_RULE "FullCostMultiWalls"
#define FULL_COST_MULTI_WALLS_LENGTH_RULE_UPPER "FULLCOSTMULTIWALLLS"
#define FULL_COST_MULTI_WALLS_RULE_TYPE "bool"

#define ENHANCEMENTS_RULES RALLY_POINTS_RULE, \
	MULTI_WALL_LENGTH_RULE, \
	FULL_COST_MULTI_WALLS_LENGTH_RULE

#define ENHANCEMENTS_RULE_COUNT 3u

#define ENHANCEMENTS_RULE_TYPES RALLY_POINTS_RULE_TYPE, \
	MULTI_WALL_LENGTH_RULE_TYPE, \
	FULL_COST_MULTI_WALLS_RULE_TYPE

#define Read_Bool_Enhancements_Rule(entry, cacheKey, defaultValue) Read_Cached_Bool_From_Rules_Ini(ENHANCEMENTS_RULES_SECTION_NAME, entry, cacheKey, defaultValue)

#define Read_Int_Enhancements_Rule(entry, cacheKey, defaultValue, minValue, maxValue) Read_Cached_Int_From_Rules_Ini(ENHANCEMENTS_RULES_SECTION_NAME, entry, cacheKey, defaultValue, minValue, maxValue)

#define Read_Multi_Wall_Length_Rule() Read_Int_Enhancements_Rule(MULTI_WALL_LENGTH_RULE, MULTI_WALL_LENGTH_RULE_KEY, 1, 1, 10)

#define Read_Full_Cost_Multi_Wall_Length_Rule() Read_Bool_Enhancements_Rule(FULL_COST_MULTI_WALLS_LENGTH_RULE, FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY, false)

#endif
