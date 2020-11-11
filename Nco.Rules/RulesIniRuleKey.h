#pragma once

#include "RuleHashUtils.h"
#include "RulesIniTypes.h"

class RulesIniRuleKey
{
private:
	RulesIniRuleKey(SectionName titleCaseSectionName, RuleName ruleName)
	{
		Section = titleCaseSectionName;
		Rule = ruleName;
		SectionKey = RuleHashUtils::BuildRuleKey(titleCaseSectionName);
		RuleKey = RuleHashUtils::BuildRuleKey(titleCaseSectionName, ruleName);
	}

public:
	SectionName Section;
	RuleName Rule;
	StringHash SectionKey;
	StringHash RuleKey;

	static RulesIniRuleKey& BuildRuleKey(SectionName titleCaseSectionName, RuleName ruleName)
	{
		return *(new RulesIniRuleKey(titleCaseSectionName, ruleName));
	}
};

// enhancement rules
extern const RulesIniRuleKey* RALLY_POINTS_RULE_KEY;
extern const RulesIniRuleKey* A_STAR_PATH_FINDING_RULE_KEY;
extern const RulesIniRuleKey* COMMANDO_GUARD_RULE_KEY;

extern const RulesIniRuleKey* AI_HARVESTERS_PER_REFINERY_RULE_KEY;
extern const RulesIniRuleKey* HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY;

extern const RulesIniRuleKey* MULTI_WALL_LENGTH_RULE_KEY;
extern const RulesIniRuleKey* FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY;

void InitaliseRuleKeys();
