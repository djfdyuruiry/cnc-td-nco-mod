#pragma once

#include <map>
#include <vector>

#include "rules_cache_key.h"
#include "IRulesIniSection.h"

class RulesIniSection : public IRulesIniSection
{
private:
	SectionName name;
	CacheKey key;
	RulesIniType defaultType;

	std::map<CacheKey, RulesIniRule*> rules;
	std::vector<RuleName> ruleNames;
	std::vector<CacheKey> ruleKeys;

	void AddRule(RulesIniRule* rule)
	{
		auto key = rule->GetKey();

		rules[key] = rule;
		ruleNames.push_back(rule->GetName());
		ruleKeys.push_back(key);
	}

public:
	RulesIniSection(SectionName name, RulesIniType defaultType, RulesSectionInitialiser initialiser)
	{
		this->name = name;
		this->key = Build_Rule_Key(this->name);
		this->defaultType = defaultType;

		if (initialiser != NULL)
		{
			initialiser(*this);
		}
	}

	RulesIniSection(SectionName name, RulesSectionInitialiser initialiser) : RulesIniSection(name, DEFAULT_RULE_TYPE, initialiser)
	{
	}

	RulesIniSection(SectionName name) : RulesIniSection(name, NULL)
	{
	}

	const char* GetName()
	{
		return name;
	}

	CacheKey GetKey()
	{
		return key;
	}

	bool HasRule(RulesIniRule* rule)
	{
		return rules.find(rule->GetKey()) != rules.end();
	}

	const RulesIniRule* GetRule(CacheKey key)
	{
		return rules[key];
	}

	std::vector<RuleName>& GetRuleNames()
	{
		return ruleNames;
	}

	std::vector<CacheKey>& GetRuleKeys()
	{
		return ruleKeys;
	}

	IRulesIniSection& operator<<(RulesIniRule* rule)
	{
		AddRule(rule);

		return *this;
	}

	IRulesIniSection& operator<<(RuleName ruleName)
	{
		ruleInStream = &RulesIniRule::BuildRule(this->name, ruleName).WithDefault(defaultType);

		AddRule(ruleInStream);

		return *this;
	}

	IRulesIniSection& operator<<(RulesIniType type)
	{
		ruleInStream->SetType(type);

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
};
