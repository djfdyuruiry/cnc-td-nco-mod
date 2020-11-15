#pragma once

#include <map>
#include <vector>

#include <HashUtils.h>

#include "IRulesIniSection.h"
#include "RuleHashUtils.h"

class RulesIniSection : public IRulesIniSection
{
private:
	SectionName name;
	StringHash key;
	RulesIniType defaultType;

	std::map<StringHash, RulesIniRule*>& rules;
	std::vector<RuleName>& ruleNames;
	std::vector<StringHash>& ruleKeys;

	RulesIniSection(SectionName name) :
		rules(*(new std::map<StringHash, RulesIniRule*>())),
		ruleNames(*(new std::vector<RuleName>())),
		ruleKeys(*(new std::vector<StringHash>()))
	{
		this->name = name;
		this->key = RuleHashUtils::BuildRuleKey(this->name);
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

	virtual ~RulesIniSection()
	{
		for (auto [_, rule] : rules)
		{
			delete rule;
		}

		delete &rules;
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

	StringHash GetKey()
	{
		return key;
	}

	StringHash BuildKey(RuleName rule)
	{
		return RuleHashUtils::BuildRuleKey(name, rule);
	}

	void SetDefaultType(RulesIniType type)
	{
		defaultType = type;
	}

	bool HasRule(StringHash key)
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

	const RulesIniRule& GetRule(StringHash key)
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

	std::vector<StringHash>& GetRuleKeys()
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

	IRulesIniSection& operator<<(char type)
	{
		ruleInStream->SetType((RulesIniType)type);

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

	RulesIniRule& operator[](StringHash ruleKey)
	{
		return *rules[ruleKey];
	}
};
