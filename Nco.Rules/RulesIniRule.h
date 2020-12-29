#pragma once

#include <malloc.h>
#include <vector>

#include <HashUtils.h>
#include <ILuaStateWrapper.h>
#include <Logger.h>
#include <strings.h>
#include <utils.h>

#include "Optional.h"
#include "RuleHashUtils.h"
#include "RulesIniTypes.h"

class RulesIniRule
{
private:
	static std::vector<RulesIniType>& INT_TYPES;
	static std::vector<RulesIniType>& UINT_TYPES;
	static std::vector<RulesIniType>& LONG_TYPES;

	SectionName section;
	RuleName name;

	StringHash sectionKey;
	RuleName uppercaseName;
	StringHash key;
	RulesIniType type;

	const char* stringKey;

	Optional& defaultValue;
	Optional& defaultValueAsPercentage;
	Optional& value;

	Optional& minValue;
	Optional& maxValue;
	Optional& valueToAllowAlways;
	std::vector<const char*>& validValues;

	static bool IsExtendedType(std::vector<RulesIniType>& extendedTypes, RulesIniType type)
	{
		for (auto intType : extendedTypes)
		{
			if (type == intType)
			{
				return true;
			}
		}

		return false;
	}

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
		sectionKey = RuleHashUtils::BuildRuleKey(section);
		uppercaseName = ConvertStringToUpperCase(name);
		key = RuleHashUtils::BuildRuleKey(section, name);

		stringKey = FormatString("%s -> %s", RULES_INI_ID_SIZE * 2 + 4, section, name);
	}

	void SetDefaultForRuleType()
	{
		if (type == INT_RULE || IsExtendedType(INT_TYPES, type))
		{
			SetDefaultValue<int>(0);
		}
		else if (type == BOOL_RULE)
		{
			SetDefaultValue<bool>(false);
		}
		else if (type == UNSIGNED_INT_RULE || IsExtendedType(UINT_TYPES, type))
		{
			SetDefaultValue<unsigned int>(0u);
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			SetDefaultValue<double>(0.0);
		}
		else if (IsExtendedType(LONG_TYPES, type))
		{
			SetDefaultValue<long>(0L);
		}
	}

	template<class T> bool ReadValueFromLuaState(ILuaStateWrapper& luaState, int stackIndex)
	{	
		auto& result = luaState.PullValue<T>(stackIndex);

		if (result.IsErrorResult())
		{
			luaState.RaiseError("Error setting value for rule %s: %s", GetStringKey(), result.GetError());

			delete &result;

			return false;
		}

		auto value = result.GetValue();

		if constexpr (std::is_same<T, unsigned char>())
		{
			if (value < 0)
			{
				luaState.RaiseError("Value for rule %s must be an integer with a value of at least zero", GetStringKey());

				delete &result;

				return false;
			}
		}

		SetValue<T>(value);

		delete &result;

		return true;
	}

	void PushRuleValueOntoLuaState(ILuaStateWrapper& luaState, Optional& ruleValue)
	{
		if (type == INT_RULE || IsExtendedType(INT_TYPES, type))
		{
			luaState.PushValue(ruleValue.Get<int>());
		}
		else if (type == BOOL_RULE)
		{
			luaState.PushValue(ruleValue.Get<bool>());
		}
		else if (type == UNSIGNED_INT_RULE || IsExtendedType(UINT_TYPES, type))
		{
			luaState.PushValue(ruleValue.Get<unsigned int>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			luaState.PushValue(ruleValue.Get<double>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			luaState.PushValue(ruleValue.Get<long>());
		}
		else if (type == STRING_RULE)
		{
			luaState.PushValue(ruleValue.Get<const char*>());
		}
		else
		{
			luaState.WriteNil();
		}
	}

	void WriteRuleValueToString(char* string, Optional& ruleValue)
	{
		if (type == INT_RULE || IsExtendedType(INT_TYPES, type))
		{
			sprintf(string, "%d", ruleValue.Get<int>());
		}
		else if (type == BOOL_RULE)
		{
			sprintf(string, "%s", ConvertBooleanToString(ruleValue.Get<bool>()));
		}
		else if (type == UNSIGNED_INT_RULE || IsExtendedType(UINT_TYPES, type))
		{
			sprintf(string, "%u", ruleValue.Get<int>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			sprintf(string, "%f", ruleValue.Get<double>());
		}
		else if (IsExtendedType(LONG_TYPES, type))
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
		std::vector<const char*> validBoolStrings { "TRUE", "FALSE" };

		return BuildRule(section, ruleName)
			.OfType(BOOL_RULE)
			.OnlyAccept(validBoolStrings);
	}

	template<class T> static void RegisterExtendedType(RulesIniType ruleType)
	{
		if constexpr (std::is_same_v<T, int>)
		{
			INT_TYPES.push_back(ruleType);
		}
		else if constexpr (std::is_same_v<T, unsigned int>)
		{
			UINT_TYPES.push_back(ruleType);
		}
		else if constexpr (std::is_same_v<T, long>)
		{
			LONG_TYPES.push_back(ruleType);
		}

		LogError("Attempted to register extended rule value type, but C++ type used was unsupported. Rule type number: %d", ruleType);
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

		delete &uppercaseName;
		delete &stringKey;
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

	StringHash GetSectionKey()
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

	const StringHash GetKey()
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
		if (type == NO_RULE_TYPE)
		{
			luaState.RaiseError("Rule %s has no type, so value cannot be set", GetStringKey());
			return false;
		}

		if (luaState.IsNil(index))
		{
			luaState.RaiseError("Value for rule %s must not be nil", GetStringKey());
			return false;
		}

		if (type == INT_RULE || IsExtendedType(INT_TYPES, type))
		{
			return ReadValueFromLuaState<int>(luaState, index);
		}
		
		if (type == UNSIGNED_INT_RULE || IsExtendedType(UINT_TYPES, type))
		{
			return ReadValueFromLuaState<unsigned int>(luaState, index);
		}
		
		if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			return ReadValueFromLuaState<double>(luaState, index);
		}
		
		if (IsExtendedType(LONG_TYPES, type))
		{
			return ReadValueFromLuaState<long>(luaState, index);
		}

		if (type == BOOL_RULE)
		{
			return ReadValueFromLuaState<bool>(luaState, index);
		}

		if (type == STRING_RULE)
		{
			return ReadValueFromLuaState<const char*>(luaState, index);
		}

		return false;
	}

	void WriteDefaultValueToString(char* string)
	{
		WriteRuleValueToString(string, defaultValue);
	}

	void WriteValueToString(char* string)
	{
		WriteRuleValueToString(string, value);
	}

};
