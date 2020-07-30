#pragma once

#include <malloc.h>
#include <vector>

#include "Optional.h"
#include "RulesIniTypes.h"
#include "strings.h"

class RulesIniRule
{
private:
	SectionName section;
	RuleName name;

	CacheKey sectionKey;
	RuleName uppercaseName;
	CacheKey key;
	RulesIniType type;

	char* asString;

	Optional defaultValue;
	Optional defaultValueAsPercentage;
	Optional value;

	Optional minValue;
	Optional maxValue;
	Optional valueToAllowAlways;
	std::vector<const char*> validValues;

	RulesIniRule(SectionName section, RuleName name)
	{
		this->section = section;
		this->name = name;

		sectionKey = Build_Rule_Key(this->section);
		uppercaseName = Convert_String_To_Upper_Case(name);
		key = Build_Rule_Key(section, name);
		type = NO_RULE_TYPE;

		asString = Allocate_String(RULES_INI_ID_SIZE * 2 + 4);

		sprintf(asString, "%s -> %s", this->section, this->name);
	}

	void SetDefaultForRuleType()
	{
		if (type == INT_RULE || type == HOUSE_LIST_RULE)
		{
			SetDefaultValue<int>(0);
		}
		else if (type == BOOL_RULE)
		{
			SetDefaultValue<bool>(false);
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			SetDefaultValue<unsigned int>(0u);
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			SetDefaultValue<double>(0.0);
		}
		else if (type == WEAPON_RULE 
			|| type == UNIT_SPEED_TYPE_RULE 
			|| type == FACTORY_RULE_TYPE
			|| type == WARHEAD_RULE
			|| type == BULLET_RULE
			|| type == PREREQ_RULE)
		{
			SetDefaultValue<double>(0L);
		}
	}

public:
	static const unsigned int RULES_INI_ID_SIZE = 32u;

	static RulesIniRule& BuildRule(SectionName section, RuleName ruleName)
	{
		return *(new RulesIniRule(section, ruleName));
	}

	static RulesIniRule& BuildBoolRule(SectionName section, RuleName ruleName)
	{
		std::vector<const char*> validBoolStrings { TRUE_STRING, FALSE_STRING };

		return BuildRule(section, ruleName)
			.OfType(BOOL_RULE)
			.OnlyAccept(validBoolStrings);
	}

	RulesIniRule& OfType(RulesIniType type)
	{
		this->type = type;

		if (!HasDefaultValue())
		{
			SetDefaultForRuleType();
		}

		return (*this);
	}

	template<class T> RulesIniRule& WithDefault(T defaultValue)
	{
		this->defaultValue.Set<T>(defaultValue);

		return (*this);
	}

	template<class T> RulesIniRule& WithDefaultAsPercentage(T defaultValue)
	{
		this->defaultValueAsPercentage.Set<T>(defaultValue);

		return (*this);
	}

	template<class T> RulesIniRule& WithMin(T min)
	{
		minValue.Set<T>(min);

		return (*this);
	}

	template<class T> RulesIniRule& WithMax(T max)
	{
		maxValue.Set<T>(max);

		return (*this);
	}

	template<class T> RulesIniRule& AlwaysAllow(T alwaysAllowThisValue)
	{
		valueToAllowAlways.Set(alwaysAllowThisValue);

		return (*this);
	}

	template<class T> RulesIniRule& OnlyAccept(std::vector<T>& validStrings)
	{
		for (auto validString : validStrings)
		{
			validValues.push_back(validString);
		}

		return (*this);
	}

	void SetType(RulesIniType type)
	{
		this->type = type;
	}

	SectionName GetSection()
	{
		return section;
	}

	CacheKey GetSectionKey()
	{
		return sectionKey;
	}

	RuleName GetName()
	{
		return name;
	}

	RuleName GetNameAsUppercase()
	{
		return name;
	}

	const CacheKey GetKey()
	{
		return key;
	}

	const RulesIniType GetType()
	{
		return type;
	}

	bool HasDefaultValue()
	{
		return defaultValue.Present();
	}

	template<class T> T GetDefaultValue()
	{
		return defaultValue.Get<T>();
	}

	template<class T> T GetDefaultValueOr(T fallback)
	{
		return defaultValue.GetOrDefault<T>(fallback);
	}

	template<class T> void SetDefaultValue(T value)
	{
		return defaultValue.Set<T>(value);
	}

	template<class T> T GetDefaultValueAsPercentage()
	{
		return defaultValueAsPercentage.Get<T>();
	}

	bool HasValue()
	{
		return value.Present();
	}

	template<class T> T GetValue()
	{
		return value.Get<T>();
	}

	template<class T> void SetValue(T value)
	{
		return this->value.Set<T>(value);
	}

	template<class T> T GetMin()
	{
		return minValue.Get<T>();
	}

	template<class T> T GetMinOrDefault(T defaultValue)
	{
		return minValue.GetOrDefault<T>(defaultValue);
	}

	template<class T> T GetMax()
	{
		return maxValue.Get<T>();
	}

	template<class T> T GetMaxOrDefault(T defaultValue)
	{
		return maxValue.GetOrDefault<T>(defaultValue);
	}

	bool HasValueToAllowAlways()
	{
		return valueToAllowAlways.Present();
	}
	
	template<class T> T GetValueToAllowAlways()
	{
		return valueToAllowAlways.Get<T>();
	}

	bool HasValidValues()
	{
		return !validValues.empty();
	}

	template<class T> std::vector<T>& GetValidValues()
	{
		return validValues;
	}

	const char* AsString()
	{
		return asString;
	}
};
