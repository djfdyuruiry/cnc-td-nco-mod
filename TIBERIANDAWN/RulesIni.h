#pragma once

#include <map>

#include "profile.h"
#include "RawFile.h"

#include "RulesIniSection.h"
#include "utils.h"

class RulesIni
{
private:
	std::vector<char*> rulesIniBuffers;

	std::map<CacheKey, IRulesIniSection*> sections;
	std::vector<const char*> sectionNames;
	std::vector<CacheKey> sectionKeys;
	
	bool rulesAreValid;

	IRulesIniSection* sectionInStream;

	RulesIni(const char* rulesFilePath)
	{
		rulesAreValid = true;

		DoSourceRulesFrom(rulesFilePath);
	}

	RulesIni& DoSourceRulesFrom(const char* rulesFileName)
	{
		auto fullRulesFilePath = Allocate_String(MAX_PATH);

		sprintf(fullRulesFilePath, "%s\\%s", Get_Mod_Data_Path(), rulesFileName);

		auto rulesFile = RawFileClass(fullRulesFilePath);

		if (!rulesFile.Is_Available())
		{
			delete fullRulesFilePath;

			return *this;
		}

		auto rulesIniBuffer = Allocate_String(rulesFile.Size());

		rulesFile.Read(rulesIniBuffer, rulesFile.Size());
		rulesFile.Close();

		rulesIniBuffers.push_back(rulesIniBuffer);

		delete fullRulesFilePath;

		return *this;
	}

	void AddSection(IRulesIniSection* section)
	{
		auto key = section->GetKey();

		sections[key] = section;
		sectionNames.push_back(section->GetName());
		sectionKeys.push_back(key);
	}

	void ValidateStringRuleValue(RulesIniRule& rule, char* valueBuffer)
	{
		auto valueIsValid = false;
		auto validValues = rule.GetValidValues<const char*>();

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
			rule.GetStringKey(),
			validValuesCsv,
			valueBuffer
		);

		delete validValuesCsv;
	}

public:
	static constexpr unsigned int RULES_STRING_LENGTH = MAX_PATH * 15u;

	static RulesIni& SourceRulesFrom(const char* rulesFilePath)
	{
		return *(new RulesIni(rulesFilePath));
	}

	RulesIni& AndThenFrom(const char* rulesFilePath)
	{
		return DoSourceRulesFrom(rulesFilePath);
	}

	RulesIni& WithSections(void (*rulesSetup)(RulesIni&))
	{
		if (rulesSetup != NULL)
		{
			rulesSetup(*this);
		}

		return *this;
	}

	Optional& ReadOptionalStringRule(RulesIniRule& rule)
	{
		Log_Trace("Resolving optional rule value: %s", rule.GetStringKey());

		bool valueFound = false;
		Optional& valueBufferOptional = Optional::BuildOptional();
		auto valueBuffer = Allocate_String(RULES_STRING_LENGTH);

		for (auto buffer: rulesIniBuffers)
		{
			WWGetPrivateProfileString(
				rule.GetSection(),
				rule.GetName(),
				valueBuffer,
				RULES_STRING_LENGTH,
				buffer,
				&valueFound
			);

			if (valueFound)
			{
				valueBufferOptional.Set<char*>(valueBuffer);
				break;
			}
		}

		return valueBufferOptional;
	}

	const char* ReadStringRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr("");

		Log_Trace("Resolving rule value: %s", rule.GetStringKey());
		Log_Trace("Default value: %s", defaultValue);

		auto valueBufferOptional = ReadOptionalStringRule(rule);

		if (!valueBufferOptional.Present())
		{
			Log_Trace("No rules ini value found, default will be returned");

			auto defaultCopy = strdup(rule.GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		auto valueBuffer = valueBufferOptional.Get<char*>();

		if (String_Is_Empty(valueBuffer))
		{
			Log_Trace("Resolved rule value was empty, default will be returned");

			auto defaultCopy = strdup(rule.GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		strupr(valueBuffer);

		if(rule.HasValidValues())
		{
			ValidateStringRuleValue(rule, valueBuffer);
		}

		Log_Trace("Resolved value: %s", valueBuffer);
		Log_Debug("Setting rule [%s] = %s", rule.GetStringKey(), valueBuffer);

		return valueBuffer;
	}

	Optional& ReadOptionalIntRule(RulesIniRule& rule)
	{
		Log_Trace("Resolving optional rule value: %s", rule.GetStringKey());

		bool valueFound = false;
		Optional& valueOptional = Optional::BuildOptional();

		for (auto buffer : rulesIniBuffers)
		{
			auto value = WWGetPrivateProfileInt(
				rule.GetSection(),
				rule.GetName(),
				buffer,
				&valueFound
			);

			if (valueFound)
			{
				valueOptional.Set<int>(value);
				break;
			}
		}

		return valueOptional;
	}

	int ReadIntRule(RulesIniRule& rule)
	{
		auto defaultValue = rule.GetDefaultValueOr(0);

		Log_Trace("Resolving rule value: %s", rule.GetStringKey());
		Log_Trace("Default value: %d", defaultValue);

		bool valueFound = false;

		auto ruleValueOptional = ReadOptionalIntRule(rule);

		if (!ruleValueOptional.Present())
		{
			Log_Trace("No rules ini value found, default will be used");

			return defaultValue;
		}

		auto ruleValue = ruleValueOptional.Get<int>();

		Log_Trace("Rules ini value: %d", ruleValue);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<int>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(INT_MIN);
			auto maxValueInclusive = rule.GetMaxOrDefault(INT_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesAreValid = false;

				Show_Error(
					"Rule [%s] must be between %d and %d (inclusive). Value provided: %d",
					rule.GetStringKey(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		Log_Trace("Resolved value: %d", ruleValue);
		Log_Debug("Setting rule [%s] = %d", rule.GetStringKey(), ruleValue);

		return ruleValue;
	}

	bool HasSection(CacheKey key)
	{
		return sections.find(key) != sections.end();
	}

	bool HasSectionForRule(RulesIniRule& rule)
	{
		return HasSection(rule.GetSectionKey());
	}

	IRulesIniSection& GetSectionForRule(RulesIniRule& rule)
	{
		return (*this)[rule.GetSectionKey()];
	}

	void MarkAsInvalid()
	{
		rulesAreValid = false;
	}

	bool IsValid()
	{
		return rulesAreValid;
	}

	RulesIni& operator<<(SectionName section)
	{
		sectionInStream = &RulesIniSection::BuildSection(section);

		AddSection(sectionInStream);

		return *this;
	}

	RulesIni& operator<<(RulesIniType sectionDefaultType)
	{
		sectionInStream->SetDefaultType(sectionDefaultType);

		return *this;
	}

	RulesIni& operator<<(IRulesIniSection& section)
	{		
		AddSection(&section);

		return *this;
	}

	RulesIni& operator<<(RulesIniRule& rule)
	{
		IRulesIniSection& section = RulesIniSection::BuildSection(rule.GetSection());

		section << rule;

		AddSection(&section);

		return *this;
	}

	IRulesIniSection& operator[](CacheKey sectionKey)
	{
		return *sections[sectionKey];
	}

	IRulesIniSection& operator[](SectionName name)
	{
		auto sectionKey = Build_Rule_Key(name);

		return (*this)[sectionKey];
	}
};
