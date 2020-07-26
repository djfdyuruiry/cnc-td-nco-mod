#pragma once

#include <vector>

#include "RulesIniRule.h"

class IRulesIniSection
{
protected:
	RulesIniRule* ruleInStream;

public:
	static const RulesIniType DEFAULT_RULE_TYPE = BOOL_RULE;

	virtual const char* GetName() = 0;

	virtual CacheKey GetKey() = 0;

	virtual bool HasRule(RulesIniRule* rule) = 0;

	virtual const RulesIniRule* GetRule(CacheKey key) = 0;

	virtual std::vector<RuleName>& GetRuleNames() = 0;

	virtual std::vector<CacheKey>& GetRuleKeys() = 0;

	virtual IRulesIniSection& operator<<(RulesIniRule* rule) = 0;

	virtual IRulesIniSection& operator<<(RuleName ruleName) = 0;

	virtual IRulesIniSection& operator<<(RulesIniType type) = 0;

	template<class T> IRulesIniSection& operator<<(T defaultValue)
	{
		ruleInStream->SetDefaultValue(defaultValue);

		return *this;
	}

	virtual RulesIniRule* operator[](RuleName ruleName) = 0;

	virtual RulesIniRule* operator[](CacheKey ruleKey) = 0;
};

typedef void (*RulesSectionInitialiser)(IRulesIniSection&);
