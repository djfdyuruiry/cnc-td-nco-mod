#include "defines.h"

#include "logger.h"
#include "InfantryTypeMod.h"
#include "RulesIniTypes.h"

static InfantryTypeMod* INFANTRY_MOD;

void ReadMods()
{
	Log_Info("Reading Mods");

	INFANTRY_MOD = &InfantryTypeMod::Build();

	INFANTRY_MOD->ReadTypes();
}

void InitialiseInfantryModTypes()
{
	Log_Info("Initialising Infantry Mods");

	INFANTRY_MOD->InitialiseTypes();

	Log_Info("Infantry Mods Initialised");
}

SectionName GetNewInfantryIniName(InfantryType type)
{
	return INFANTRY_MOD->GetTypeIniName(type);
}

InfantryType GetNewInfantryType(SectionName iniName, bool* matchFound)
{
	return INFANTRY_MOD->GetTypeByIniName(iniName, matchFound);
}
