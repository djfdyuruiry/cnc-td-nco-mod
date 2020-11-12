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
