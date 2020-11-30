#pragma once

#include "rules_ini_enhancements.h"
#include "RulesIniRuleKey.h"

#define InitEnhancementKey(key, rule) key = key != NULL ? key : &RulesIniRuleKey::BuildRuleKey(ENHANCEMENTS_RULES_SECTION_NAME, rule)

class EnhancementKeys final
{
private:
	EnhancementKeys()
	{
	}

public:
	static RulesIniRuleKey* RALLY_POINTS_RULE_KEY;
	static RulesIniRuleKey* A_STAR_PATH_FINDING_RULE_KEY;
	static RulesIniRuleKey* COMMANDO_GUARD_RULE_KEY;

	static RulesIniRuleKey* AI_HARVESTERS_PER_REFINERY_RULE_KEY;
	static RulesIniRuleKey* HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY;

	static RulesIniRuleKey* MULTI_WALL_LENGTH_RULE_KEY;
	static RulesIniRuleKey* FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY;

	static void InitIfNeeded()
	{
		InitEnhancementKey(RALLY_POINTS_RULE_KEY, RALLY_POINTS_RULE);

		InitEnhancementKey(A_STAR_PATH_FINDING_RULE_KEY, A_STAR_PATH_FINDING_RULE);
		InitEnhancementKey(COMMANDO_GUARD_RULE_KEY, COMMANDO_GUARD_RULE);

		InitEnhancementKey(AI_HARVESTERS_PER_REFINERY_RULE_KEY, AI_HARVESTERS_PER_REFINERY_RULE);
		InitEnhancementKey(HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY, HUMAN_HARVESTERS_PER_REFINERY_RULE);

		InitEnhancementKey(MULTI_WALL_LENGTH_RULE_KEY, MULTI_WALL_LENGTH_RULE);
		InitEnhancementKey(FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY, FULL_COST_MULTI_WALLS_LENGTH_RULE);
	}
};
