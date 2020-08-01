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

	RulesIniSection(SectionName name)
	{
		this->name = name;
		this->key = Build_Rule_Key(this->name);
		this->defaultType = DEFAULT_RULE_TYPE;
	}

	void AddRule(RulesIniRule& rule)
	{
		auto key = rule.GetKey();

		rules[key] = &rule;
		ruleNames.push_back(rule.GetName());
		ruleKeys.push_back(key);
	}

public:
	static RulesIniSection& BuildSection(SectionName name)
	{
		return *(new RulesIniSection(name));
	}

	~RulesIniSection()
	{
		for (auto key : ruleKeys)
		{
			delete rules[key];
		}
	}

	RulesIniSection& WithDefaultType(RulesIniType type)
	{
		defaultType = type;

		return *this;
	}

	IRulesIniSection& WithRules(RulesSectionInitialiser initialiser)
	{
		if (initialiser != NULL)
		{
			initialiser(*this);
		}

		return *this;
	}

	SectionName GetName()
	{
		return name;
	}

	CacheKey GetKey()
	{
		return key;
	}

	CacheKey BuildKey(RuleName rule)
	{
		return Build_Rule_Key(name, rule);
	}

	void SetDefaultType(RulesIniType type)
	{
		defaultType = type;
	}

	bool HasRule(CacheKey key)
	{
		return rules.find(key) != rules.end();
	}

	bool HasRule(RulesIniRule& rule)
	{
		return HasRule(rule.GetKey());
	}

	bool HasRule(RuleName rule)
	{
		return HasRule(BuildKey(rule));
	}

	const RulesIniRule& GetRule(CacheKey key)
	{
		return *rules[key];
	}

	RulesIniRule& BuildRule(RuleName name)
	{
		return RulesIniRule::BuildRule(this->name, name).OfType(defaultType);
	}

	std::vector<RuleName>& GetRuleNames()
	{
		return ruleNames;
	}

	std::vector<CacheKey>& GetRuleKeys()
	{
		return ruleKeys;
	}

	IRulesIniSection& operator<<(RulesIniRule& rule)
	{
		AddRule(rule);

		return *this;
	}

	IRulesIniSection& operator<<(RuleName ruleName)
	{
		ruleInStream = &RulesIniRule::BuildRule(this->name, ruleName)
			.OfType(defaultType);

		AddRule(*ruleInStream);

		return *this;
	}

	IRulesIniSection& operator<<(RulesIniType type)
	{
		ruleInStream->SetType(type);

		return *this;
	}

	IRulesIniSection& operator<<(IRulesIniSection& section)
	{
		return section;
	}

	RulesIniRule& operator[](RuleName ruleName)
	{
		return *rules[BuildKey(ruleName)];
	}

	RulesIniRule& operator[](CacheKey ruleKey)
	{
		return *rules[ruleKey];
	}
};
