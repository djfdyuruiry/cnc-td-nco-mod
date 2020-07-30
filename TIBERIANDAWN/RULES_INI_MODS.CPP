#include <stdlib.h>

#include "function.h"

#include "logger.h"
#include "rules_ini.h"
#include "RulesIniRuleKey.h"

static char** MOD_INFANTRY_TYPES = NULL;

void Read_Infantry_Mods()
{
	Log_Info("Reading Infantry Mods");

	auto newInfantryCsv = ReadRuleValue<char*>(MOD_RULES_SECTION_NAME, NEW_INFANTRY_RULE);

	if (String_Is_Empty(newInfantryCsv)) {
		Log_Debug("No infantry mods found");
		return;
	}

	auto newInfantryCount = 0u;

	MOD_INFANTRY_TYPES = Parse_Csv_String(newInfantryCsv, RulesIniRule::RULES_INI_ID_SIZE, &newInfantryCount);

	if (newInfantryCount < 1u || MOD_INFANTRY_TYPES == NULL) {
		Log_Debug("No infantry mods found");
		return;
	}

	auto totalInfantryCount = INFANTRY_COUNT + newInfantryCount;

	GetRulesReader()
		.GetRule(MOD_RULES_SECTION_NAME, NEW_INFANTRY_COUNT_RULE)
		.SetValue(newInfantryCount);

	GetRulesReader()
		.GetRule(*INFANTRY_COUNT_RULE_KEY)
		.SetValue(totalInfantryCount);

	Log_Info("Mod infantry types read: %s", newInfantryCsv);
	Log_Info("Total game infantry types: %u", newInfantryCount, totalInfantryCount);
}

void Read_Mods()
{
	Log_Info("Reading Mods");

	Read_Infantry_Mods();
}

static bool Setup_New_Infantry_Type(char* typeString, InfantryType type, char* baseTypeString)
{
	if (String_Is_Empty(typeString))
	{
		Show_Error("Blank mod infantry type in rules file");

		return false;
	}

	Log_Info("Setting up new mod infantry type: %s", typeString);

	bool parseError = false;
	auto baseType = Parse_Infantry_Type(baseTypeString, &parseError);

	if (parseError)
	{
		return false;
	}

	Log_Info("Mod infantry type base: %s", baseTypeString);

	auto baseInfantry = InfantryTypeClass::By_Type(baseType);

	auto infantryClone = (InfantryTypeClass*)malloc(sizeof(InfantryTypeClass));

	memcpy(infantryClone, baseInfantry, sizeof(InfantryTypeClass));

	infantryClone->IsModType = true;

	strcpy(infantryClone->ModBaseIniName, baseTypeString);
	strcpy(infantryClone->IniName, typeString);

	infantryClone->Type = type;

	InfantryTypeClass::Read_Infantry_Rules(infantryClone);
	TechnoTypeClass::Read_Techno_Rules(infantryClone);
	ObjectTypeClass::Read_Object_Rules(infantryClone);

	InfantryTypeClass::Add_Infantry_Type(type, infantryClone);

	Log_Info("Mod infantry type setup successful");

	return true;
}

void Initialise_Infantry_Mod_Types()
{
	Log_Info("Initialising Infantry Mods");
	
	auto infantryCount = Read_Infantry_Count();

	for (auto type = INFANTRY_COUNT; type < infantryCount; type++)
	{		
		bool baseTypeFound = false;
		auto typeString = MOD_INFANTRY_TYPES[type - INFANTRY_COUNT];

		Convert_String_To_Upper_Case(typeString);

		auto baseTypeString = ReadRuleValue<char*>(typeString, BASE_TYPE_RULE);

		if (String_Is_Empty(baseTypeString))
		{
			Show_Error("Mod infantry type has no %s rule provided: %s", typeString, BASE_TYPE_RULE);

			MarkRulesIniAsInvalid();

			continue;
		}

		if (!Setup_New_Infantry_Type(typeString, type, baseTypeString))
		{
			MarkRulesIniAsInvalid();
		}
	}

	Log_Info("Infantry Mods Initialised");
}

const char* Get_New_Infantry_Ini_Name(char type)
{
	return (const char*)MOD_INFANTRY_TYPES[type - INFANTRY_COUNT];
}

char Get_New_Infantry_Type(char* iniName, bool* matchFound)
{
	auto infantryCount = Read_Infantry_Count();

	for (auto type = INFANTRY_COUNT; type < infantryCount; type++)
	{
		auto typeString = MOD_INFANTRY_TYPES[type - INFANTRY_COUNT];

		if (Strings_Are_Equal(iniName, typeString))
		{
			if (matchFound != NULL)
			{
				*matchFound = true;
			}

			return type;
		}
	}

	if (matchFound != NULL)
	{
		*matchFound = false;
	}

	return INFANTRY_NONE;
}
