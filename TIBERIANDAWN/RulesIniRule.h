#pragma once

#include <malloc.h>
#include <vector>

#include <ILuaStateWrapper.h>

#include "Optional.h"
#include "RulesIniTypes.h"
#include "RULES_CACHE_KEY.h"
#include <strings.h>
#include <utils.h>

class RulesIniRule
{
private:
	SectionName section;
	RuleName name;

	CacheKey sectionKey;
	RuleName uppercaseName;
	CacheKey key;
	RulesIniType type;

	const char* stringKey;

	Optional& defaultValue;
	Optional& defaultValueAsPercentage;
	Optional& value;

	Optional& minValue;
	Optional& maxValue;
	Optional& valueToAllowAlways;
	std::vector<const char*>& validValues;

	RulesIniRule(SectionName titleCaseSectionName, RuleName ruleName)
		: section(titleCaseSectionName),
		  name(ruleName),
		  type(NO_RULE_TYPE),
		  defaultValue(Optional::BuildOptional()),
		  defaultValueAsPercentage(Optional::BuildOptional()),
		  value(Optional::BuildOptional()),
		  minValue(Optional::BuildOptional()),
		  maxValue(Optional::BuildOptional()),
		  valueToAllowAlways(Optional::BuildOptional()),
		  validValues(*(new std::vector<const char*>()))
	{
		sectionKey = Build_Rule_Key(section);
		uppercaseName = Convert_String_To_Upper_Case(name);
		key = Build_Rule_Key(section, name);

		auto keyStr = Allocate_String(RULES_INI_ID_SIZE * 2 + 4);

		sprintf(keyStr, "%s -> %s", section, name);

		stringKey = keyStr;
	}

	void SetDefaultForRuleType()
	{
		if (
			type == INT_RULE
			  || type == HOUSE_LIST_RULE
			  || type == WEAPON_RULE
			  || type == UNIT_SPEED_TYPE_RULE
			  || type == FACTORY_RULE_TYPE
			  || type == WARHEAD_RULE
			  || type == BULLET_RULE
		)
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
		else if (type == PREREQ_RULE)
		{
			SetDefaultValue<long>(0L);
		}
	}

	void PushRuleValueOntoLuaState(ILuaStateWrapper& luaState, Optional& ruleValue)
	{
		if (
			type == INT_RULE
			|| type == HOUSE_LIST_RULE
			|| type == WEAPON_RULE
			|| type == UNIT_SPEED_TYPE_RULE
			|| type == FACTORY_RULE_TYPE
			|| type == WARHEAD_RULE
			|| type == BULLET_RULE
		)
		{
			luaState.PushValue(ruleValue.Get<int>());
		}
		else if (type == BOOL_RULE)
		{
			luaState.PushValue(ruleValue.Get<bool>());
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			luaState.PushValue(ruleValue.Get<unsigned int>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			luaState.PushValue(ruleValue.Get<double>());
		}
		else if (type == PREREQ_RULE)
		{
			luaState.PushValue(ruleValue.Get<long>());
		}
		else if (type == STRING_RULE)
		{
			luaState.PushValue(ruleValue.Get<char*>());
		}
		else
		{
			luaState.WriteNil();
		}
	}

	void WriteRuleValueToString(char* string, Optional& ruleValue)
	{
		if (
			type == INT_RULE
			|| type == HOUSE_LIST_RULE
			|| type == WEAPON_RULE
			|| type == UNIT_SPEED_TYPE_RULE
			|| type == FACTORY_RULE_TYPE
			|| type == WARHEAD_RULE
			|| type == BULLET_RULE
			)
		{
			sprintf(string, "%d", ruleValue.Get<int>());
		}
		else if (type == BOOL_RULE)
		{
			sprintf(string, "%s", Convert_Boolean_To_String(ruleValue.Get<bool>()));
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			sprintf(string, "%u", ruleValue.Get<int>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			sprintf(string, "%f", ruleValue.Get<double>());
		}
		else if (type == PREREQ_RULE)
		{
			sprintf(string, "%ld", ruleValue.Get<long>());
		}
		else if (type == STRING_RULE)
		{
			sprintf(string, "%s", ruleValue.Get<char*>());
		}
		else
		{
			sprintf(string, "%s", "(null)");
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

	~RulesIniRule()
	{
		delete &defaultValue;
		delete &defaultValueAsPercentage;
		delete &value;

		delete &minValue;
		delete &maxValue;
		delete &valueToAllowAlways;

		delete &validValues;
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
		for (T validString : validStrings)
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

	const char* GetStringKey()
	{
		return stringKey;
	}

	void PushDefaultValueOntoLuaState(ILuaStateWrapper& luaState)
	{
		PushRuleValueOntoLuaState(luaState, defaultValue);
	}

	void PushValueOntoLuaState(ILuaStateWrapper& luaState)
	{
		PushRuleValueOntoLuaState(luaState, value);
	}

	bool SetValueFromLuaState(ILuaStateWrapper& luaState, int index)
	{
		if (luaState.IsNil(index))
		{
			luaState.RaiseError("Value for rule %s must not be nil", GetStringKey());
			return false;
		}

		if (type != BOOL_RULE && !luaState.IsNumber(index))
		{
			luaState.RaiseError("Value for rule %s must be an integer", GetStringKey());
			return false;
		}

		auto setOk = true;

		if (
			type == INT_RULE
			|| type == HOUSE_LIST_RULE
			|| type == WEAPON_RULE
			|| type == UNIT_SPEED_TYPE_RULE
			|| type == FACTORY_RULE_TYPE
			|| type == WARHEAD_RULE
			|| type == BULLET_RULE
		)
		{
			auto& result = luaState.ReadInteger(index);

			SetValue<int>(result.GetValue());

			delete &result;
		}
		else if (type == BOOL_RULE)
		{
			if (!luaState.IsBool(index))
			{
				luaState.RaiseError("Value for rule %s must be a boolean", GetStringKey());
				return false;
			}

			auto& result = luaState.ReadBool(index);

			SetValue<bool>(result.GetValue());

			delete &result;
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			auto& result = luaState.ReadInteger(index);
			auto value = result.GetValue();

			if (value < 0)
			{
				luaState.RaiseError("Value for rule %s must be an integer with a value of at least zero", GetStringKey());

				delete &result;

				return false;
			}

			SetValue<unsigned int>((unsigned int)value);

			delete &result;
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			auto& result = luaState.ReadDouble(index);

			SetValue<double>(result.GetValue());

			delete &result;
		}
		else if (type == PREREQ_RULE)
		{
			auto& result = luaState.ReadInteger(index);

			SetValue<long>((long)result.GetValue());

			delete &result;
		}
		else
		{
			luaState.RaiseError("Unable to convert value for rule %s to a lua type", GetStringKey());

			setOk = false;
		}

		return setOk;
	}

	void WriteDefaultValueToString(char* string)
	{
		WriteRuleValueToString(string, defaultValue);
	}

	void WriteValueToString(char* string)
	{
		WriteRuleValueToString(string, value);
	}

	void Dispose()
	{

	}
};
