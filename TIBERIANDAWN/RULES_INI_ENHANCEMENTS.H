#ifndef RULES_INI_ENHANCEMENTS_H
#define RULES_INI_ENHANCEMENTS_H

#include "rules_ini.h"

#define ENHANCEMENTS_RULES_SECTION_NAME "Enhancements"

#define RALLY_POINTS_RULE "RallyPoints"
#define RALLY_POINTS_RULE_UPPER "RALLYPOINTS"
#define RALLY_POINTS_RULE_TYPE "bool"

#define ENHANCEMENTS_RULES RALLY_POINTS_RULE

#define ENHANCEMENTS_RULE_COUNT 1u

#define ENHANCEMENTS_RULE_TYPES RALLY_POINTS_RULE_TYPE

#define Read_Bool_Enhancements_Rule(entry, cacheKey, defaultValue) Read_Cached_Bool_From_Rules_Ini(ENHANCEMENTS_RULES_SECTION_NAME, entry, cacheKey, defaultValue)

#endif
