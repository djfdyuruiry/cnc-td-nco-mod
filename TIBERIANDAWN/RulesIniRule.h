#pragma once

#include "rules_cache_key.h"
#include "strings.h"

typedef const char* SectionName;
typedef const char* RuleName;
typedef void* RuleValue;

enum RulesIniType
{
	BOOL_RULE,
	INT_RULE,
	UNSIGNED_INT_RULE,
	DOUBLE_RULE,
	STRING_RULE
};

class RulesIniRule
{
private:
	SectionName section;
	RuleName name;
	RuleName uppercaseName;

	RulesIniType type;
	CacheKey key;

	RuleValue defaultValue;

public:
	RulesIniRule(SectionName section, RuleName name)
	{
		this->section = section;
		this->name = name;
		this->uppercaseName = Convert_String_To_Upper_Case(name);

		this->key = Build_Rule_Key(section, name);

		this->defaultValue = NULL;
	}

	void SetRuleType(RulesIniType ruleType)
	{
		this->type = ruleType;
	}

	void SetDefaultValue(RuleValue defaultValue)
	{
		this->defaultValue = defaultValue;
	}

	SectionName GetSection()
	{
		return section;
	}

	RuleName GetName()
	{
		return name;
	}

	const RulesIniType GetType()
	{
		return type;
	}

	bool HasDefaultValue()
	{
		return defaultValue != NULL;
	}

	template<class T> T* GetDefaultValue()
	{
		return (T*)defaultValue;
	}

	const CacheKey GetKey()
	{
		return key;
	}
};
