#pragma once

#include <Logger.h>
#include <TypePatterns.h>

#include "IRulesIni.h"
#include "IRulesIniReader.h"
#include "RulesIniRuleKey.h"

class RulesIniReader : public IRulesIniReader
{
protected:
	RulesIniReader(IRulesIni& rulesIniToRead) : IRulesIniReader(rulesIniToRead)
	{
	}

	template<class T> T GetParsedStringRule(
		RulesIniRule& rule,
		const char* typeName,
		PARSER(T, parser),
		T defaultValue
	)
	{
		auto stringValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!stringValueOptional.Present())
		{
			return rule.GetDefaultValueOr<T>(defaultValue);
		}

		auto stringValue = stringValueOptional.Get<char*>();

		Convert_String_To_Upper_Case(stringValue);

		bool parseError = false;
		auto parsedValue = parser(stringValue, &parseError, parseError);

		if (parseError)
		{
			rulesIni.MarkAsInvalid();

			Show_Error("Failed to parse %s for [%s]: %s", typeName, rule.GetStringKey(), stringValue);

			return rule.GetDefaultValueOr<T>(defaultValue);
		}

		return parsedValue;
	}

	// TODO: move all these private converters into seperate classes with a interface and mapping from type to converter
	bool ReadBoolRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(false);

		Optional& ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			LogTrace("No rules ini value found, default will be used");

			delete& ruleValueOptional;

			return defaultValue;
		}

		auto ruleValue = ruleValueOptional.Get<char*>();

		Convert_String_To_Upper_Case(ruleValue);

		auto boolValue = Strings_Are_Equal(ruleValue, "TRUE");

		delete& ruleValueOptional;

		return boolValue;
	}

	unsigned int ReadUnsignedIntRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(0u);

		LogTrace("Resolving rule value: %s", rule.GetStringKey());
		LogTrace("Default value: %u", defaultValue);

		Optional& ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			LogTrace("No rules ini value found, default will be used");

			delete& ruleValueOptional;

			return defaultValue;
		}

		auto ruleValueStr = ruleValueOptional.Get<char*>();

		bool isValid = Is_Unsigned_Int_String(ruleValueStr);

		if (!isValid)
		{
			rulesIni.MarkAsInvalid();

			Show_Error(
				"Rule [%s] must be a unsigned integer number. Value provided: %s",
				rule.GetStringKey(),
				ruleValueStr
			);

			delete ruleValueStr;
			delete& ruleValueOptional;

			return defaultValue;
		}

		auto ruleValue = strtoul(ruleValueStr, NULL, 10);

		LogTrace("Rules ini value: %s", ruleValueStr);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<unsigned int>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(0u);
			auto maxValueInclusive = rule.GetMaxOrDefault(UINT_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesIni.MarkAsInvalid();

				Show_Error(
					"Rule [%s] must be between %u and %u (inclusive). Value provided: %u",
					rule.GetStringKey(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		LogTrace("Resolved value: %u", ruleValue);
		LogDebug("Setting rule [%s] = %u", rule.GetStringKey(), ruleValue);

		delete ruleValueStr;
		delete& ruleValueOptional;

		return ruleValue;
	}

	double ReadDoubleRule(RulesIniRule& rule, bool* valueFound = NULL)
	{
		auto defaultValue = rule.GetDefaultValueOr(0.0);

		LogTrace("Resolving rule value: %s", rule.GetStringKey());
		LogTrace("Default value: %f", defaultValue);

		auto& ruleValueOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!ruleValueOptional.Present())
		{
			LogTrace("No rules ini value found, default will be used");

			if (valueFound != NULL)
			{
				*valueFound = false;
			}

			delete& ruleValueOptional;

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
				rule.GetStringKey(),
				ruleValueStr
			);

			delete ruleValueStr;
			delete& ruleValueOptional;

			return defaultValue;
		}

		auto ruleValue = strtod(ruleValueStr, NULL);

		LogTrace("Rules ini value: %s", ruleValueStr);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<double>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(DBL_MIN);
			auto maxValueInclusive = rule.GetMaxOrDefault(DBL_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesIni.MarkAsInvalid();

				Show_Error(
					"Rule [%s] must be between %f and %f (inclusive). Value provided: %f",
					rule.GetStringKey(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		LogTrace("Resolved value: %f", ruleValue);
		LogDebug("Setting rule [%s] = %f", rule.GetStringKey(), ruleValue);

		delete ruleValueStr;
		delete& ruleValueOptional;

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
				rule.GetStringKey(),
				ruleValueAsDouble
			);

			return defaultValue;
		}

		auto defaultValueAsPercentage = rule.GetDefaultValueAsPercentage<double>();

		auto onePercent = defaultValue / (defaultValueAsPercentage * 100);
		auto ruleValueAsPercentage = ruleValueAsDouble * 100;

		return nearbyint(ruleValueAsPercentage * onePercent);
	}

	virtual bool SetDefaultRuleValue(RulesIniRule& rule)
	{
		return false;
	}

public:
	static IRulesIniReader& Build(IRulesIni& rules)
	{
		return *(new RulesIniReader(rules));
	}

	RulesIniRule& GetRule(const RulesIniRuleKey& key)
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

	RulesIniRule& GetRule(SectionName section, RuleName ruleName)
	{
		const RulesIniRuleKey& key = RulesIniRuleKey::BuildRuleKey(section, ruleName);

		RulesIniRule& rule = GetRule(key);

		delete& key;

		return rule;
	}

};
