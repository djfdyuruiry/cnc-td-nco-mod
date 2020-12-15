#pragma once

#include <map>

#include <FileUtils.h>
#include <IniReader.h>
#include <utils.h>

#include "IRulesIni.h"
#include "RulesIniSection.h"

class RulesIni : public IRulesIni
{
private:
	std::vector<IniReader*>& rulesIniReaders;

	std::map<StringHash, IRulesIniSection*>& sections;
	std::vector<const char*>& sectionNames;
	std::vector<StringHash>& sectionKeys;
	
	bool rulesAreValid;

	IRulesIniSection* sectionInStream;

	RulesIni(const char* rulesFilePath) : 
		rulesIniReaders(*(new std::vector<IniReader*>())),
		sections(*(new std::map<StringHash, IRulesIniSection*>())),
		sectionNames(*(new std::vector<const char*>())),
		sectionKeys(*(new std::vector<StringHash>()))
	{
		rulesAreValid = true;

		DoSourceRulesFrom(rulesFilePath);
	}

	IRulesIni& DoSourceRulesFrom(const char* rulesFileName)
	{
		auto fullRulesFilePath = FormatString("%s\\%s", MAX_PATH, GetModDataPath(), rulesFileName);

		if (!FileUtils::IsFile(fullRulesFilePath))
		{
			delete fullRulesFilePath;

			return *this;
		}

		auto rulesIniBuffer = FileUtils::ReadFileText(fullRulesFilePath);

		delete fullRulesFilePath;

		if (rulesIniBuffer == NULL)
		{
			return *this;
		}

		rulesIniReaders.push_back(
			&IniReader::Build(rulesIniBuffer)
		);

		return *this;
	}

	void AddSection(IRulesIniSection* section)
	{
		auto key = section->GetKey();

		sections[key] = section;
		sectionNames.push_back(section->GetName());
		sectionKeys.push_back(key);
	}

	void ValidateStringRuleValue(RulesIniRule& rule, const char* valueBuffer)
	{
		auto valueIsValid = false;
		auto validValues = rule.GetValidValues<const char*>();
		auto upperValue = ConvertStringToUpperCase(valueBuffer);

		for (auto validValue : validValues)
		{
			if (StringsAreEqual(upperValue, validValue))
			{
				valueIsValid = true;
				break;
			}
		}

		delete upperValue;

		if (valueIsValid)
		{
			return;
		}

		rulesAreValid = false;

		auto validValuesSize = validValues.size();
		auto validValuesCsv = AllocateString(validValuesSize * RULES_STRING_LENGTH);
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

		ShowError(
			"Rule [%s] must be in the list (%s). Value provided: %s",
			rule.GetStringKey(),
			validValuesCsv,
			valueBuffer
		);

		delete validValuesCsv;
	}

public:
	static constexpr unsigned int RULES_STRING_LENGTH = MAX_PATH * 15u;

	static IRulesIni& SourceRulesFrom(const char* rulesFilePath)
	{
		return *(new RulesIni(rulesFilePath));
	}

	~RulesIni()
	{
		for (auto reader : rulesIniReaders)
		{
			delete reader;
		}

		delete &rulesIniReaders;

		for (auto [_, section] : sections)
		{
			delete section;
		}

		delete &sections;
		
		delete &sectionNames;
		delete &sectionKeys;
	}

	IRulesIni& AndThenFrom(const char* rulesFilePath)
	{
		return DoSourceRulesFrom(rulesFilePath);
	}

	IRulesIni& WithSections(void (*rulesSetup)(IRulesIni&))
	{
		if (rulesSetup != NULL)
		{
			rulesSetup(*this);
		}

		return *this;
	}

	Optional& ReadOptionalStringRule(RulesIniRule& rule)
	{
		LogTrace("Resolving optional rule value: %s", rule.GetStringKey());

		bool valueFound = false;
		Optional& valueBufferOptional = Optional::BuildOptional();
		auto valueBuffer = AllocateString(RULES_STRING_LENGTH);

		for (auto reader: rulesIniReaders)
		{
			reader->ReadString(
				rule.GetSection(),
				rule.GetName(),
				valueBuffer,
				RULES_STRING_LENGTH,
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

		LogTrace("Resolving rule value: %s", rule.GetStringKey());
		LogTrace("Default value: %s", defaultValue);

		auto valueBufferOptional = ReadOptionalStringRule(rule);

		if (!valueBufferOptional.Present())
		{
			LogTrace("No rules ini value found, default will be returned");

			auto defaultCopy = strdup(rule.GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		auto valueBuffer = valueBufferOptional.Get<char*>();

		if (StringIsEmpty(valueBuffer))
		{
			LogTrace("Resolved rule value was empty, default will be returned");

			auto defaultCopy = strdup(rule.GetDefaultValueOr(defaultValue));

			return defaultCopy;
		}

		if(rule.HasValidValues())
		{
			ValidateStringRuleValue(rule, valueBuffer);
		}

		LogTrace("Resolved value: %s", valueBuffer);
		LogDebug("Setting rule [%s] = %s", rule.GetStringKey(), valueBuffer);

		return valueBuffer;
	}

	Optional& ReadOptionalIntRule(RulesIniRule& rule)
	{
		LogTrace("Resolving optional rule value: %s", rule.GetStringKey());

		bool valueFound = false;
		Optional& valueOptional = Optional::BuildOptional();

		for (auto reader : rulesIniReaders)
		{
			auto value = reader->ReadInt(
				rule.GetSection(),
				rule.GetName(),
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

		LogTrace("Resolving rule value: %s", rule.GetStringKey());
		LogTrace("Default value: %d", defaultValue);

		bool valueFound = false;

		auto ruleValueOptional = ReadOptionalIntRule(rule);

		if (!ruleValueOptional.Present())
		{
			LogTrace("No rules ini value found, default will be used");

			return defaultValue;
		}

		auto ruleValue = ruleValueOptional.Get<int>();

		LogTrace("Rules ini value: %d", ruleValue);

		if (!rule.HasValueToAllowAlways() || ruleValue != rule.GetValueToAllowAlways<int>())
		{
			auto minValueInclusive = rule.GetMinOrDefault(INT_MIN);
			auto maxValueInclusive = rule.GetMaxOrDefault(INT_MAX);

			if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
			{
				rulesAreValid = false;

				ShowError(
					"Rule [%s] must be between %d and %d (inclusive). Value provided: %d",
					rule.GetStringKey(),
					minValueInclusive,
					maxValueInclusive,
					ruleValue
				);
			}
		}

		LogTrace("Resolved value: %d", ruleValue);
		LogDebug("Setting rule [%s] = %d", rule.GetStringKey(), ruleValue);

		return ruleValue;
	}

	bool HasSection(StringHash key)
	{
		return sections.find(key) != sections.end();
	}

	bool HasSection(const char* key)
	{
		return HasSection(RuleHashUtils::BuildRuleKey(key));
	}

	bool HasSectionForRule(RulesIniRule& rule)
	{
		return HasSection(rule.GetSectionKey());
	}

	const std::vector<const char*>& GetSectionNames()
	{
		return sectionNames;
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

	IRulesIni& operator<<(SectionName section)
	{
		sectionInStream = &RulesIniSection::BuildSection(section);

		AddSection(sectionInStream);

		return *this;
	}

	IRulesIni& operator<<(RulesIniType sectionDefaultType)
	{
		sectionInStream->SetDefaultType(sectionDefaultType);

		return *this;
	}

	IRulesIni& operator<<(IRulesIniSection& section)
	{		
		AddSection(&section);

		return *this;
	}

	IRulesIni& operator<<(RulesIniRule& rule)
	{
		IRulesIniSection& section = RulesIniSection::BuildSection(rule.GetSection());

		section << rule;

		AddSection(&section);

		return *this;
	}

	IRulesIniSection& operator[](StringHash sectionKey)
	{
		return *sections[sectionKey];
	}

	IRulesIniSection& operator[](SectionName name)
	{
		auto sectionKey = RuleHashUtils::BuildRuleKey(name);

		return (*this)[sectionKey];
	}
};
