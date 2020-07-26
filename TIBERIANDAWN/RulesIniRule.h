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
	Optional validValues;

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

		return (*this);
	}

	template<class T> RulesIniRule& WithDefault(T defaultValue)
	{
		this->defaultValue.Set(defaultValue);

		return (*this);
	}

	template<class T> RulesIniRule& WithDefaultAsPercentage(T defaultValue)
	{
		this->defaultValueAsPercentage.Set(defaultValue);

		return (*this);
	}

	template<class T> RulesIniRule& WithMin(T min)
	{
		this->min.Set(min);

		return (*this);
	}

	template<class T> RulesIniRule& WithMax(T max)
	{
		this->max.Set(max);

		return (*this);
	}

	template<class T> RulesIniRule& AlwaysAllow(T alwaysAllowThisValue)
	{
		valueToAllowAlways.Set(alwaysAllowThisValue);

		return (*this);
	}

	template<class T> RulesIniRule& OnlyAccept(std::vector<T>& validStrings)
	{
		validValues.Set(validStrings);

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
		return defaultValue.Get();
	}

	template<class T> T GetDefaultValueOr(T fallback)
	{
		return defaultValue.GetOrDefault(fallback);
	}

	template<class T> void SetDefaultValue(T value)
	{
		return defaultValue.Set(value);
	}

	bool HasValue()
	{
		return value.Present();
	}

	template<class T> T GetValue()
	{
		return value.Get<();
	}

	template<class T> void SetValue(T value)
	{
		return value.Set(value);
	}

	template<class T> T GetMin()
	{
		return min.Get();
	}

	template<class T> T GetMinOrDefault(T defaultValue)
	{
		return min.GetOrDefault(defaultValue);
	}

	template<class T> T GetMax()
	{
		return max.Get();
	}

	template<class T> T GetMaxOrDefault(T defaultValue)
	{
		return max.GetOrDefault(defaultValue);
	}

	bool HasValueToAllowAlways()
	{
		return valueToAllowAlways.Present();
	}
	
	template<class T> T GetValueToAllowAlways()
	{
		return valueToAllowAlways.Get();
	}

	bool HasValidValues()
	{
		return validValues.Present();
	}

	template<class T> std::vector<T>& GetValidValues()
	{
		return validValues.Get();
	}

	const char* AsString()
	{
		asString;
	}
};
