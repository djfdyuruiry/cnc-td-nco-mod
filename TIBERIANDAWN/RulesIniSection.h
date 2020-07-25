#pragma once

#include <map>
#include <vector>

#include "rules_cache_key.h"
#include "RulesIniRule.h"

class RulesIniSection
{
private:
	SectionName name;

	std::map<CacheKey, RulesIniRule*> rules;
	std::vector<RuleName> ruleNames;
	std::vector<CacheKey> ruleKeys;

	RulesIniRule* ruleInStream;

public:
	RulesIniSection(SectionName name, void (*initialiser)(RulesIniSection&))
	{
		this->name = name;

		if (initialiser != NULL)
		{
			initialiser(*this);
		}
	}

	const char* GetName()
	{
		return name;
	}

	const RulesIniRule* GetRule(CacheKey key)
	{
		return rules[key];
	}

	RulesIniSection& operator<<(RuleName ruleName)
	{
		auto rule = new RulesIniRule(this->name, ruleName);
		auto key = rule->GetKey();

		rules[key] = rule;
		ruleNames.push_back(ruleName);
		ruleKeys.push_back(key);

		ruleInStream = rule;

		return *this;
	}

	RulesIniSection& operator<<(RulesIniType type)
	{
		ruleInStream->SetRuleType(type);

		return *this;
	}

	template<class T> RulesIniSection& operator<<(T defaultValue)
	{
		void* defaultValueCopy = calloc(1, sizeof(T));

		memcpy(defaultValueCopy, &defaultValue, sizeof(T));

		ruleInStream->SetDefaultValue(defaultValueCopy);

		return *this;
	}

	RulesIniRule* operator[](RuleName ruleName)
	{
		return rules[Build_Rule_Key(name, ruleName)];
	}

	RulesIniRule* operator[](CacheKey ruleKey)
	{
		return rules[ruleKey];
	}

	std::vector<RuleName>& GetRuleNames()
	{
		return ruleNames;
	}

	std::vector<CacheKey>& GetRuleKeys()
	{
		return ruleKeys;
	}
};
