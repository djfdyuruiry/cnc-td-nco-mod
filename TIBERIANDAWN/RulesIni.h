#pragma once

#include <map>

#include "function.h"

#include "RulesIniSection.h"

class RulesIni
{
private:
	static const unsigned int RULES_STRING_LENGTH = MAX_PATH * 15u;

	std::vector<char*> rulesIniBuffers;

	std::map<CacheKey, IRulesIniSection*> sections;
	std::vector<const char*> sectionNames;
	std::vector<CacheKey> sectionKeys;
	
	bool rulesAreValid;

	SectionName sectionNameInStream;
	RulesIniType sectionDefaultTypeInStream;

	RulesIni()
	{
		rulesAreValid = true;
	}

	RulesIni& SourceRulesFrom(const char* rulesFilePath)
	{
		auto rulesFile = RawFileClass(rulesFilePath);

		if (!rulesFile.Is_Available())
		{
			return *this;
		}

		auto rulesIniBuffer = Allocate_String(rulesFile.Size());

		rulesFile.Read(rulesIniBuffer, rulesFile.Size());
		rulesFile.Close();

		rulesIniBuffers.push_back(rulesIniBuffer);

		return *this;
	}

	void AddSection(IRulesIniSection* section)
	{
		auto key = section->GetKey();

		sections[key] = section;
		sectionNames.push_back(section->GetName());
		sectionKeys.push_back(key);
	}

	void ValidateStringRuleValue(RulesIniRule* rule, char* valueBuffer)
	{
		auto valueIsValid = false;
		auto validValues = rule->GetValidValues<const char*>();

		for (auto validValue : validValues)
		{
			if (Strings_Are_Equal(valueBuffer, validValue))
			{
				valueIsValid = true;
				break;
			}
		}

		if (valueIsValid)
		{
			return;
		}

		rulesAreValid = false;

		auto validValuesSize = validValues.size();
		auto validValuesCsv = Allocate_String(validValuesSize * RULES_STRING_LENGTH);
		auto idx = 0;

		for (auto validValue : validValues)
		{
			strcat(validValuesCsv, validValue);

			if (idx != validValuesSize - 1)
			{
				strcat(validValuesCsv, ", ");
			}

			idx++;
		}

		Show_Error(
			"Rule [%s] must be in the list (%s). Value provided: %s",
			rule->AsString(),
			validValuesCsv,
			valueBuffer
		);

		delete validValuesCsv;
	}

public:
	static RulesIni& SourceRulesFrom(const char* rulesFilePath)
	{
		auto rules = new RulesIni();

		rules->SourceRulesFrom(rulesFilePath);

		return *rules;
	}

	RulesIni& AndThenFrom(const char* rulesFilePath)
	{
		return SourceRulesFrom(rulesFilePath);
	}

	char* ReadOptionalStringRule(
		RulesIniRule* rule,
		bool* valueFound
	)
	{
		if (valueFound == NULL)
		{
			bool fallbackValueFound = false;
			valueFound = &fallbackValueFound;
		}

		Log_Trace("Resolving optional rule value: %s", rule->AsString());

		auto valueBuffer = Allocate_String(RULES_STRING_LENGTH);

		for (auto buffer: rulesIniBuffers)
		{
			WWGetPrivateProfileString(
				rule->GetSection(),
				rule->GetName(),
				valueBuffer,
				RULES_STRING_LENGTH,
				buffer,
				valueFound
			);

			if (*valueFound)
			{
				break;
			}
		}

		return valueBuffer;
	}

	const char* ReadStringRule(RulesIniRule* rule)
	{
		auto defaultValue = rule->GetDefaultValueOr("");

		Log_Trace("Resolving rule value: %s", rule->AsString());
		Log_Trace("Default value: %s", defaultValue);

		bool valueFound = false;
		auto valueBuffer = ReadOptionalStringRule(rule, &valueFound);

		if (!valueFound)
		{
			Log_Trace("No rules ini value found, default will be returned");

			auto defaultCopy = strdup(rule->GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		if (String_Is_Empty(valueBuffer))
		{
			Log_Trace("Resolved rule value was empty, default will be returned");

			auto defaultCopy = strdup(rule->GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		strupr(valueBuffer);

		if(rule->HasValidValues())
		{
			ValidateStringRuleValue(rule, valueBuffer);
		}

		Log_Trace("Resolved value: %s", valueBuffer);
		Log_Debug("Setting rule [%s] = %s", rule->AsString(), valueBuffer);

		return valueBuffer;
	}

	int ReadOptionalIntRule(RulesIniRule* rule, bool* valueFound)
	{
		if (valueFound == NULL)
		{
			bool fallbackValueFound = false;
			valueFound = &fallbackValueFound;
		}

		Log_Trace("Resolving optional rule value: %s", rule->AsString());

		auto value = 0;

		for (auto buffer : rulesIniBuffers)
		{
			value = WWGetPrivateProfileInt(
				rule->GetSection(),
				rule->GetName(),
				buffer,
				valueFound
			);

			if (*valueFound)
			{
				break;
			}
		}

		return value;
	}

	int ReadIntRule(RulesIniRule* rule)
	{
		auto defaultValue = rule->GetDefaultValueOr(0);

		Log_Trace("Resolving rule value: %s", rule->AsString());
		Log_Trace("Default value: %d", defaultValue);

		bool valueFound = false;

		auto ruleValue = ReadOptionalIntRule(rule, &valueFound);

		if (!valueFound)
		{
			Log_Trace("No rules ini value found, default will be used");

			return defaultValue;
		}

		Log_Trace("Rules ini value: %d", ruleValue);

		if (!rule->HasValueToAllowAlways() || ruleValue == rule->GetValueToAllowAlways<int>())
		{
			auto minValueInclusive = rule->GetMinOrDefault(INT_MIN);
			auto maxValueInclusive = rule->GetMaxOrDefault(INT_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesAreValid = false;

				Show_Error(
					"Rule [%s] must be between %d and %d (inclusive). Value provided: %d",
					rule->AsString(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		Log_Trace("Resolved value: %d", ruleValue);
		Log_Debug("Setting rule [%s] = %d", rule->AsString(), ruleValue);

		return ruleValue;
	}

	bool HasSection(CacheKey key)
	{
		return sections.find(key) == sections.end();
	}

	bool HasSectionForRule(RulesIniRule* rule)
	{
		return HasSection(rule->GetSectionKey());
	}

	IRulesIniSection* GetSectionForRule(RulesIniRule* rule)
	{
		return (*this)[rule->GetSectionKey()];
	}

	RulesIni& operator<<(SectionName section)
	{
		sectionNameInStream = section;
		sectionDefaultTypeInStream = IRulesIniSection::DEFAULT_RULE_TYPE;

		return *this;
	}

	RulesIni& operator<<(RulesIniType sectionDefaultType)
	{
		sectionDefaultTypeInStream = sectionDefaultType;

		return *this;
	}

	RulesIni& operator<<(RulesSectionInitialiser initialiser)
	{		
		AddSection(new RulesIniSection(sectionNameInStream, sectionDefaultTypeInStream, initialiser));

		return *this;
	}

	RulesIni& operator<<(RulesIniRule* rule)
	{
		auto section = new RulesIniSection(rule->GetSection());

		(*section) << rule;

		AddSection(section);

		return *this;
	}

	IRulesIniSection* operator[](CacheKey sectionKey)
	{
		return sections[sectionKey];
	}

	IRulesIniSection* operator[](SectionName name)
	{
		auto sectionKey = Build_Rule_Key(name);

		return (*this)[sectionKey];
	}
};
