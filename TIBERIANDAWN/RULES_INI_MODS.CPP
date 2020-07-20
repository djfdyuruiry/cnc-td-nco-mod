#include "function.h"

#include "rules_cache.h"

static char** MOD_INFANTRY_TYPES = NULL;

void Read_Infantry_Mods()
{
	Log_Info("Reading Infantry Mods");

	bool valueFound = false;
	auto newInfantryCsv = Read_Optional_String_From_Rules_Ini(MOD_RULES_SECTION_NAME, NEW_INFANTRY_RULE, &valueFound, true);

	if (valueFound = false || String_Is_Empty(newInfantryCsv)) {
		Log_Debug("No infantry mods found");
		return;
	}

	auto newInfantryCount = 0u;

	MOD_INFANTRY_TYPES = Parse_Csv_String(newInfantryCsv, 32, &newInfantryCount);

	if (newInfantryCount < 1u || MOD_INFANTRY_TYPES == NULL) {
		Log_Debug("No infantry mods found");
		return;
	}

	auto totalInfantryCount = INFANTRY_COUNT + newInfantryCount;

	Cache_Unsigned_Int_Rule(MOD_RULES_SECTION_NAME, NEW_INFANTRY_COUNT_RULE, newInfantryCount);
	Cache_Unsigned_Int_Rule(MOD_RULES_SECTION_NAME, INFANTRY_COUNT_RULE, totalInfantryCount);

	Log_Info("Mod infantry types read: %s", newInfantryCsv);
	Log_Info("Total game infantry types: %u", newInfantryCount, totalInfantryCount);
}

void Read_Mods()
{
	Log_Info("Reading Mods");

	Read_Infantry_Mods();
}

static bool Setup_New_Infantry_Type(char* typeString, char* baseTypeString)
{
	if (String_Is_Empty(typeString))
	{
		Show_Error("Blank mod infantry type in rules file");

		return false;
	}

	Log_Info("Setting up new mod infantry type: %s", typeString);

	if (String_Is_Empty(baseTypeString))
	{
		Show_Error("Missing `BaseType` rule for mod infantry type: %s", typeString);

		return false;
	}

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
	infantryClone->ModIniName = typeString;

	InfantryTypeClass::Read_Infantry_Rules(infantryClone);
	TechnoTypeClass::Read_Techno_Rules(infantryClone);
	ObjectTypeClass::Read_Object_Rules(infantryClone);

	Log_Info("Mod infantry type setup successful");

	return true;
}

void Initialise_Infantry_Mod_Types()
{
	Log_Info("Initialising Infantry Mods");

	for (auto type = INFANTRY_COUNT; type < Read_Infantry_Count(INFANTRY_COUNT); type++)
	{
		auto typeString = MOD_INFANTRY_TYPES[type];
		auto baseTypeString = MOD_INFANTRY_TYPES[type];

		if (!Setup_New_Infantry_Type(typeString, baseTypeString))
		{
			Rules_Ini_Failed_Validation(true);

			return;
		}
	}

	Log_Info("Infantry Mods Initialised");
}
