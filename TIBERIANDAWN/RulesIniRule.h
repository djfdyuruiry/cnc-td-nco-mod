#pragma once

#include <malloc.h>
#include <vector>

#include "lua.h"
#include "Optional.h"
#include "RulesIniTypes.h"
#include "strings.h"
#include "utils.h"

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

	RulesIniRule(SectionName section, RuleName name)
		: defaultValue(Optional::BuildOptional()),
		  defaultValueAsPercentage(Optional::BuildOptional()),
		  value(Optional::BuildOptional()),
		  minValue(Optional::BuildOptional()),
		  maxValue(Optional::BuildOptional()),
		  valueToAllowAlways(Optional::BuildOptional()),
	      validValues(*(new std::vector<const char*>()))
	{
		this->section = section;
		this->name = name;

		sectionKey = Build_Rule_Key(this->section);
		uppercaseName = Convert_String_To_Upper_Case(name);
		key = Build_Rule_Key(section, name);
		type = NO_RULE_TYPE;

		auto keyStr = Allocate_String(RULES_INI_ID_SIZE * 2 + 4);

		sprintf(keyStr, "%s -> %s", this->section, this->name);

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

	void PushRuleValueOntoLuaState(lua_State* lua, Optional& ruleValue)
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
			lua_pushinteger(lua, ruleValue.Get<int>());
		}
		else if (type == BOOL_RULE)
		{
			lua_pushboolean(lua, ruleValue.Get<bool>());
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			lua_pushinteger(lua, ruleValue.Get<unsigned int>());
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			lua_pushnumber(lua, ruleValue.Get<double>());
		}
		else if (type == PREREQ_RULE)
		{
			lua_pushinteger(lua, ruleValue.Get<long>());
		}
		else if (type == STRING_RULE)
		{
			lua_pushstring(lua, ruleValue.Get<char*>());
		}
		else
		{
			lua_pushnil(lua);
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

	const char* GetStringKey()
	{
		return stringKey;
	}

	void PushDefaultValueOntoLuaState(lua_State* lua)
	{
		PushRuleValueOntoLuaState(lua, defaultValue);
	}

	void PushValueOntoLuaState(lua_State* lua)
	{
		PushRuleValueOntoLuaState(lua, value);
	}

	bool SetValueFromLuaState(lua_State* lua, int index)
	{
		if (lua_isnil(lua, index))
		{
			luaL_error(lua, "Value for rule %s must not be nil", GetStringKey());
			return false;
		}

		if (type != BOOL_RULE && !lua_isnumber(lua, index))
		{
			luaL_error(lua, "Value for rule %s must be an integer", GetStringKey());
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
			SetValue<int>((int)lua_tointeger(lua, index));
		}
		else if (type == BOOL_RULE)
		{
			if (!lua_isboolean(lua, index))
			{
				luaL_error(lua, "Value for rule %s must be a boolean", GetStringKey());
				return false;
			}

			SetValue<bool>((bool)lua_toboolean(lua, index));
		}
		else if (type == UNSIGNED_INT_RULE || type == ARMOR_TYPE_RULE)
		{
			auto value = lua_tointeger(lua, index);

			if (value < 0)
			{
				luaL_error(lua, "Value for rule %s must be an integer with a value of at least zero", GetStringKey());
				return false;
			}

			SetValue<unsigned int>((unsigned int)value);
		}
		else if (type == DOUBLE_RULE || type == FIXED_RULE)
		{
			SetValue<double>((double)lua_tonumber(lua, index));
		}
		else if (type == PREREQ_RULE)
		{
			SetValue<long>((long)lua_tointeger(lua, index));
		}
		else
		{
			luaL_error(lua, "Unable to convert value for rule %s to a lua type", GetStringKey());

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
};
