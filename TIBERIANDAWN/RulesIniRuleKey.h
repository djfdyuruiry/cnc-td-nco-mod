#pragma once

#include "rules_cache_key.h"
#include "RulesIniTypes.h"

class RulesIniRuleKey
{
public:
	SectionName Section;
	RuleName Rule;
	CacheKey SectionKey;
	CacheKey RuleKey;

	RulesIniRuleKey(SectionName sectionName, RuleName ruleName)
	{
		Section = sectionName;
		Rule = ruleName;
		SectionKey = Build_Rule_Key(sectionName);
		RuleKey = Build_Rule_Key(sectionName, ruleName);
	}
};
