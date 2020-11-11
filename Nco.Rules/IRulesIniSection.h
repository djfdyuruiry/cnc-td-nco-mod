#pragma once

#include <vector>

#include <HashUtils.h>

#include "RulesIniRule.h"

class IRulesIniSection
{
protected:
	RulesIniRule* ruleInStream;

	IRulesIniSection()
	{
	}

public:
	static const RulesIniType DEFAULT_RULE_TYPE = BOOL_RULE;

	virtual ~IRulesIniSection()
	{
	}

	virtual SectionName GetName() = 0;

	virtual StringHash GetKey() = 0;

	virtual StringHash BuildKey(RuleName rule) = 0;

	virtual void SetDefaultType(RulesIniType type) = 0;

	virtual bool HasRule(StringHash key) = 0;

	virtual bool HasRule(RulesIniRule& rule) = 0;

	virtual bool HasRule(RuleName rule) = 0;

	virtual const RulesIniRule& GetRule(StringHash key) = 0;

	virtual RulesIniRule& BuildRule(RuleName name) = 0;

	virtual std::vector<RuleName>& GetRuleNames() = 0;

	virtual std::vector<StringHash>& GetRuleKeys() = 0;

	virtual IRulesIniSection& operator<<(RulesIniRule& rule) = 0;

	virtual IRulesIniSection& operator<<(RuleName ruleName) = 0;

	virtual IRulesIniSection& operator<<(RulesIniType type) = 0;

	virtual IRulesIniSection& operator<<(char type) = 0;

	virtual IRulesIniSection& operator<<(IRulesIniSection& type) = 0;

	template<class T> IRulesIniSection& operator<<(T defaultValue)
	{
		ruleInStream->SetDefaultValue(defaultValue);

		return *this;
	}

	virtual RulesIniRule& operator[](RuleName ruleName) = 0;

	virtual RulesIniRule& operator[](StringHash ruleKey) = 0;
};

typedef void (*RulesSectionInitialiser)(IRulesIniSection&);
