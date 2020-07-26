#pragma once

#include "RulesIni.h"
#include "RulesIniRuleKey.h"

class RulesIniReader
{
private:
	RulesIni& rulesIni;

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
			auto minValueInclusive = rule.GetMinOrDefault(0.0);
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

		auto onePercent = defaultValue  / (defaultValueAsPercentage * 100);
		auto ruleValueAsPercentage = ruleValueAsDouble * 100;

		return nearbyint(ruleValueAsPercentage * onePercent);
	}

	RulesIniReader(RulesIni& rulesIniToRead) : rulesIni(rulesIniToRead)
	{
	}

public:
	static RulesIniReader& ReaderFor(RulesIni& rulesIni)
	{
		return *(new RulesIniReader(rulesIni));
	}

	RulesIniRule& GetRule(RulesIniRuleKey key)
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

	template<class T> T ReadRuleValue(RulesIniRule& rule)
	{
		if (rule.HasValue())
		{
			return rule.GetValue<T>();
		}

		auto ruleType = rule.GetType();

		if (ruleType == INT_RULE)
		{
			rule.SetValue(
				rulesIni.ReadIntRule(rule)
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
		else if (ruleType == BOOL_RULE)
		{
			rule.SetValue(
				ReadBoolRule(rule)
			);
		}
		else if (ruleType == STRING_RULE)
		{
			rule.SetValue(
				rulesIni.ReadStringRule(rule)
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

	template<class T> T ReadRuleValue(RulesIniRuleKey key)
	{
		return ReadRuleValue<T>(
			GetRule(key)
		);
	}
	template<class T> T ReadRuleValue(SectionName section, RuleName rule)
	{
		return ReadRuleValue<T>(
			GetRule(
				RulesIniRuleKey(section, rule)
			)
		);
	}
};
