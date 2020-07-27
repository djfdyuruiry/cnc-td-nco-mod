#pragma once

#include "defines.h"

#include "RulesIni.h"
#include "RulesIniRuleKey.h"

class RulesIniReader
{
private:
	RulesIni& rulesIni;

	RulesIniReader(RulesIni& rulesIniToRead) : rulesIni(rulesIniToRead)
	{
	}

	// TODO: move all these private converters into seperate classes with a interface and mapping from type to converter
	bool ReadBoolRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(false);

		auto ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			Log_Trace("No rules ini value found, default will be used");

			return defaultValue;
		}

		auto ruleValue = ruleValueOptional.Get<char*>();

		Convert_String_To_Upper_Case(ruleValue);

		auto boolValue = Strings_Are_Equal(ruleValue, TRUE_STRING);

		return boolValue;
	}

	unsigned int ReadUnsignedIntRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(0u);

		Log_Trace("Resolving rule value: %s", rule.AsString());
		Log_Trace("Default value: %u", defaultValue);

		auto ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			Log_Trace("No rules ini value found, default will be used");

			return defaultValue;
		}

		auto ruleValueStr = ruleValueOptional.Get<char*>();

		bool isValid = Is_Unsigned_Int_String(ruleValueStr);

		if (!isValid)
		{
			rulesIni.MarkAsInvalid();

			Show_Error(
				"Rule [%s] must be a unsigned integer number. Value provided: %s",
				rule.AsString(),
				ruleValueStr
			);

			delete ruleValueStr;

			return defaultValue;
		}

		auto ruleValue = strtoul(ruleValueStr, NULL, 10);

		Log_Trace("Rules ini value: %s", ruleValueStr);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<unsigned int>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(0u);
			auto maxValueInclusive = rule.GetMaxOrDefault(UINT_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesIni.MarkAsInvalid();

				Show_Error(
					"Rule [%s] must be between %u and %u (inclusive). Value provided: %u",
					rule.AsString(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}


		Log_Trace("Resolved value: %u", ruleValue);
		Log_Debug("Setting rule [%s] = %u", rule.AsString(), ruleValue);

		delete ruleValueStr;

		return ruleValue;
	}

	double ReadDoubleRule(RulesIniRule& rule, bool* valueFound = NULL)
	{
		auto defaultValue = rule.GetDefaultValueOr(0.0);

		Log_Trace("Resolving rule value: %s", rule.AsString());
		Log_Trace("Default value: %f", defaultValue);

		auto ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			Log_Trace("No rules ini value found, default will be used");

			if (valueFound != NULL)
			{
				*valueFound = false;
			}

			return defaultValue;
		}

		if (valueFound != NULL)
		{
			*valueFound = true;
		}

		auto ruleValueStr = ruleValueOptional.Get<char*>();

		bool isValid = Is_Double_String(ruleValueStr);

		if (!isValid)
		{
			rulesIni.MarkAsInvalid();

			Show_Error(
				"Rule [%s] must be a floating point number. Value provided: %s",
				rule.AsString(),
				ruleValueStr
			);

			delete ruleValueStr;

			return defaultValue;
		}

		auto ruleValue = strtod(ruleValueStr, NULL);

		Log_Trace("Rules ini value: %s", ruleValueStr);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<double>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(DBL_MIN);
			auto maxValueInclusive = rule.GetMaxOrDefault(DBL_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesIni.MarkAsInvalid();

				Show_Error(
					"Rule [%s] must be between %f and %f (inclusive). Value provided: %f",
					rule.AsString(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		Log_Trace("Resolved value: %f", ruleValue);
		Log_Debug("Setting rule [%s] = %f", rule.AsString(), ruleValue);

		delete ruleValueStr;

		return ruleValue;
	}

	/// <summary>
	/// Fixed is a reference to the C++ type used in Red Alert for
	/// it's INI double values that are converted to unsigned integers
	/// </summary>
	/// <returns>The double rules value converted to a `fixed` unsigned int</returns>
	unsigned int ReadFixedRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(0u);

		bool valueFound = false;
		auto ruleValueAsDouble = ReadDoubleRule(rule, &valueFound);

		if (!valueFound)
		{
			return defaultValue;
		}

		if (ruleValueAsDouble < 0.00 || ruleValueAsDouble > 0.99)
		{
			rulesIni.MarkAsInvalid();

			Show_Error(
				"Rule [%s] must be a floating point number between 0.00 and 0.99 (inclusive), value provided: %f",
				rule.AsString(),
				ruleValueAsDouble
			);

			return defaultValue;
		}

		auto defaultValueAsPercentage = rule.GetDefaultValueAsPercentage<double>();

		auto onePercent = defaultValue / (defaultValueAsPercentage * 100);
		auto ruleValueAsPercentage = ruleValueAsDouble * 100;

		return nearbyint(ruleValueAsPercentage * onePercent);
	}

	template<class T> T GetParsedStringRule(
		RulesIniRule& rule,
		const char* typeName,
		T(*parser)(char*, bool*),
		T defaultValue
	)
	{
		auto stringValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!stringValueOptional.Present())
		{
			return rule.GetDefaultValue<T>(defaultValue);
		}

		auto stringValue = stringValueOptional.Get<char*>();

		Convert_String_To_Upper_Case(stringValue);

		bool parseError = false;
		auto parsedValue = parser(stringValue, &parseError);

		if (parseError)
		{
			rulesIni.MarkAsInvalid();

			Show_Error("Failed to parse %s for [%s]: %s", typeName, rule.AsString(), stringValue);

			return rule.GetDefaultValue<T>(defaultValue);
		}

		return parsedValue;
	}

	int ReadHouseListRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"houses",
			&Parse_House_Name_List_Csv,
			(int)HOUSEF_NONE
		);
	}

	WeaponType ReadWeaponRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"weapon",
			&Parse_Weapon_Type,
			WEAPON_NONE
		);
	}

	ArmorType ReadArmorRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"armor",
			&Parse_Armor_Type,
			ARMOR_NONE
		);
	}

	SpeedType ReadUnitSpeedRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"unit speed",
			&Parse_Unit_Speed_Type,
			SPEED_NONE
		);
	}

	FactoryType ReadFactoryRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"factory type",
			&Parse_Factory_Type,
			FACTORY_TYPE_NONE
		);
	}

	WarheadType ReadWarheadRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"warhead",
			&Parse_Warhead_Type,
			WARHEAD_NONE
		);
	}

	BulletType ReadBulletRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"bullet",
			&Parse_Bullet_Type,
			BULLET_NONE
		);
	}

public:
	static RulesIniReader& ReaderFor(RulesIni& rulesIni)
	{
		return *(new RulesIniReader(rulesIni));
	}

	RulesIniRule& GetRule(const RulesIniRuleKey key)
	{
		if (!rulesIni.HasSection(key.SectionKey))
		{
			rulesIni << key.Section;
		}

		IRulesIniSection& section = rulesIni[key.SectionKey];

		if (!section.HasRule(key.RuleKey))
		{
			section << key.Rule;
		}

		return section[key.RuleKey];
	}

	RulesIniRule& GetRule(SectionName section, RuleName rule)
	{
		const auto key = RulesIniRuleKey(section, rule);

		return GetRule(key);
	}

	template<class T> T ReadRuleValue(RulesIniRule& rule)
	{
		if (rule.HasValue())
		{
			return rule.GetValue<T>();
		}

		auto ruleType = rule.GetType();


		if (ruleType == STRING_RULE)
		{
			rule.SetValue(
				rulesIni.ReadStringRule(rule)
			);
		}
		 else if (ruleType == INT_RULE)
		{
			rule.SetValue(
				rulesIni.ReadIntRule(rule)
			);
		}
		else if (ruleType == BOOL_RULE)
		{
			rule.SetValue(
				ReadBoolRule(rule)
			);
		}
		else if (ruleType == UNSIGNED_INT_RULE)
		{
			rule.SetValue(
				ReadUnsignedIntRule(rule)
			);
		}
		else if (ruleType == DOUBLE_RULE)
		{
			rule.SetValue(
				ReadDoubleRule(rule)
			);
		}
		else if (ruleType == FIXED_RULE)
		{
			rule.SetValue(
				ReadFixedRule(rule)
			);
		}
		else if (ruleType == HOUSE_LIST_RULE)
		{
			rule.SetValue(
				ReadHouseListRule(rule)
			);
		}
		else if (ruleType == WEAPON_RULE)
		{
			rule.SetValue(
				ReadWeaponRule(rule)
			);
		}
		else if (ruleType == ARMOR_RULE)
		{
			rule.SetValue(
				ReadArmorRule(rule)
			);
		}
		else if (ruleType == UNIT_SPEED_RULE)
		{
			rule.SetValue(
				ReadUnitSpeedRule(rule)
			);
		}
		else if (ruleType == FACTORY_RULE)
		{
			rule.SetValue(
				ReadFactoryRule(rule)
			);
		}
		else if (ruleType == WARHEAD_RULE)
		{
			rule.SetValue(
				ReadWarheadRule(rule)
			);
		}
		else if (ruleType == BULLET_RULE)
		{
			rule.SetValue(
				ReadBulletRule(rule)
			);
		}
		else
		{
			rule.SetValue(
				rule.GetDefaultValue<T>()
			);
		}

		return rule.GetValue<T>();
	}

	template<class T> T ReadRuleValue(const RulesIniRuleKey key)
	{
		return ReadRuleValue<T>(
			GetRule(key)
		);
	}

	template<class T> T ReadRuleValue(SectionName section, RuleName rule)
	{
		const auto key = RulesIniRuleKey(section, rule);

		return ReadRuleValue<T>(key);
	}
};
