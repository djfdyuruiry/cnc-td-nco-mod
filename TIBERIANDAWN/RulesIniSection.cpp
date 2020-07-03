#include "function.h"

void RulesIniSection::Update_Rule(IniRule* rule, const char* entry, void* value, unsigned int valueSize)
{
	Log_Trace("Caching Game rule %s value: %d", entry, value);

	delete rule->ruleValue;

	if (valueSize > 0 && value != NULL)
	{
		rule->ruleValue = malloc(valueSize);

		memcpy(rule->ruleValue, value, valueSize);
	}
}

void RulesIniSection::Store_Rule(const char* entry, void* value, const char* valueType, unsigned int valueSize)
{
	Log_Trace("Caching %s rule %s value", sectionName, entry);

	currentGameRules[size] = (IniRule*)malloc(sizeof(IniRule));

	currentGameRules[size]->ruleName = Convert_String_To_Upper_Case(entry);
	currentGameRules[size]->valueType = valueType;

	if (valueSize > 0 && value != NULL)
	{
		currentGameRules[size]->ruleValue = malloc(valueSize);
		memcpy(currentGameRules[size]->ruleValue, value, valueSize);
	}

	size++;
}

RulesIniSection::RulesIniSection(const char* sectionName, const char** ruleNames, const char** ruleTypes, unsigned int ruleCount)
{
	this->sectionName = sectionName;
	this->ruleNames = ruleNames;
	this->ruleTypes = ruleTypes;
	this->ruleCount = ruleCount;

	size = 0;
}

bool RulesIniSection::Init_Rules()
{
	currentGameRules = new IniRule * [RulesIniSection::MAX_GAME_RULES];
	auto valid = true;

	for (unsigned int i = 0; i < ruleCount; i++)
	{
		auto ruleName = ruleNames[i];
		auto typeName = ruleTypes[i];
		auto valueSize = 0;
		void* value = NULL;
		void* invalidValue;

		if (Strings_Are_Equal(typeName, "string"))
		{
			value = Read_String_From_Rules_Ini(sectionName, ruleName, NULL);
			valueSize = value != NULL ? strlen((char*)value) : 0;
		}
		else if (Strings_Are_Equal(typeName, "bool"))
		{
			auto boolValueStr = Read_String_From_Rules_Ini(sectionName, ruleName, NULL);

			if (Is_Boolean_String((char*)boolValueStr))
			{
				auto boolValue = Parse_Boolean_Or_Default(boolValueStr, false);

				value = &boolValue;
				valueSize = sizeof(bool);
			}
			else if (boolValueStr != NULL)
			{
				invalidValue = boolValueStr;
				valid = false;
			}
		}
		else if (Strings_Are_Equal(typeName, "int"))
		{
			auto intValueStr = Read_String_From_Rules_Ini(sectionName, ruleName, NULL);

			if (Is_Number_String((char*)intValueStr))
			{
				auto intValue = atoi(intValueStr);

				value = &intValue;
				valueSize = sizeof(int);
			}
			else if (intValueStr != NULL)
			{
				invalidValue = intValueStr;
				valid = false;
			}
		}

		if (!valid)
		{
			Show_Error(
				"%s rule %s should be of type %s, but the current value could not be parsed: %s",
				sectionName,
				ruleName,
				typeName,
				invalidValue
			);

			break;
		}

		Store_Rule(ruleName, value, typeName, valueSize);
	}

	return valid;
}

const char* RulesIniSection::Get_Name()
{
	return sectionName;
}

const char** RulesIniSection::Get_Rule_Names()
{
	return ruleNames;
}

IniRule* RulesIniSection::Get_Rule(const char* entry)
{
	auto upperCaseEntry = Convert_String_To_Upper_Case(entry);

	Log_Trace("Reading %s rule %s", sectionName, entry);

	for (unsigned i = 0; i < size; i++)
	{
		auto gameRule = currentGameRules[i];

		if (gameRule == NULL)
		{
			continue;
		}

		if (gameRule->ruleValue == NULL)
		{
			continue;
		}

		if (Strings_Are_Equal(gameRule->ruleName, upperCaseEntry))
		{
			if (Strings_Are_Equal(gameRule->valueType, "string"))
			{
				Log_Trace("Read Game rule %s value from cache: %s", entry, gameRule->ruleValue);
			}
			else if (Strings_Are_Equal(gameRule->valueType, "bool"))
			{
				Log_Trace("Read Game rule %s value from cache: %d", entry, *(bool*)gameRule->ruleValue);
			}
			else
			{
				Log_Trace("Read Game rule %s value from cache: %d", entry, *(int*)gameRule->ruleValue);
			}

			return gameRule;
		}
	}

	return NULL;
}

char* RulesIniSection::Read_Rule(const char* entry, char* defaultValue)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL && rule->ruleValue != NULL)
	{
		return (char*)rule->ruleValue;
	}

	auto resolvedValue = Read_String_From_Rules_Ini(sectionName, entry, defaultValue);

	Store_Rule(entry, resolvedValue, "string", strlen(resolvedValue));

	return resolvedValue;
}

bool RulesIniSection::Update_Current_Rule_Value(const char* entry, char* value)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL)
	{
		if (!Strings_Are_Equal((char*)rule->valueType, "string"))
		{
			return false;
		}

		Update_Rule(rule, entry, strdup(value), strlen(value));

		return true;
	}

	Store_Rule(entry, strdup(value), "string", strlen(value));

	return true;
}

char* RulesIniSection::Read_Rule(const char* entry, const char* defaultValue)
{
	auto defaultValueStr = strdup(defaultValue);

	auto value = Read_Rule(entry, defaultValueStr);

	delete defaultValueStr;

	return value;
}

bool RulesIniSection::Update_Current_Rule_Value(const char* entry, const char* value)
{
	auto defaultValueStr = strdup(value);

	auto result = Update_Current_Rule_Value(entry, defaultValueStr);

	delete defaultValueStr;

	return result;
}

int RulesIniSection::Read_Int_Rule(const char* entry, int defaultValue, int minInclusive, int maxInclusive)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL && rule->ruleValue != NULL)
	{
		return *(int*)rule->ruleValue;
	}

	auto resolvedValue = Read_Int_From_Rules_Ini(sectionName, entry, defaultValue, minInclusive, maxInclusive);
	int* value = &resolvedValue;

	Store_Rule(entry, value, "int", sizeof(int));

	return resolvedValue;
}

bool RulesIniSection::Update_Current_Int_Rule_Value(const char* entry, int value)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL)
	{
		if (!Strings_Are_Equal((char*)rule->valueType, "int"))
		{
			return false;
		}

		Update_Rule(rule, entry, &value, sizeof(int));

		return true;
	}

	Store_Rule(entry, &value, "int", sizeof(int));

	return true;
}

bool RulesIniSection::Read_Boolean_Rule(const char* entry, bool defaultValue)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL && rule->ruleValue != NULL)
	{
		return *(bool*)rule->ruleValue;
	}

	auto resolvedValue = Read_Bool_From_Rules_Ini(sectionName, entry, defaultValue);
	bool* value = &resolvedValue;

	Store_Rule(entry, value, "bool", sizeof(bool));

	return resolvedValue;
}

bool RulesIniSection::Update_Current_Boolean_Rule_Value(const char* entry, bool value)
{
	auto rule = Get_Rule(entry);

	if (rule != NULL)
	{
		if (!Strings_Are_Equal((char*)rule->valueType, "bool"))
		{
			return false;
		}

		Update_Rule(rule, entry, &value, sizeof(bool));

		return true;
	}

	Store_Rule(entry, &value, "bool", sizeof(bool));

	return true;
}
