#include "defines.h"

#include "logger.h"
#include "InfantryTypeMod.h"
#include "RulesIniTypes.h"
#include "UnitTypeMod.h"

static InfantryTypeMod* INFANTRY_MOD;
static UnitTypeMod* UNIT_MOD;

static auto MODS_INITIALISED = false;

void ReadMods()
{
	Log_Info("Reading Mods");

	INFANTRY_MOD = &InfantryTypeMod::Build();
	UNIT_MOD = &UnitTypeMod::Build();

	INFANTRY_MOD->ReadTypes();
	UNIT_MOD->ReadTypes();
}

void InitialiseModsIfRequired()
{
	if (MODS_INITIALISED)
	{
		return;
	}

	INFANTRY_MOD->InitialiseTypes();
	UNIT_MOD->InitialiseTypes();

	MODS_INITIALISED = true;
}

SectionName GetNewInfantryIniName(InfantryType type)
{
	InitialiseModsIfRequired();

	return INFANTRY_MOD->GetTypeIniName(type);
}

InfantryType GetNewInfantryType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return INFANTRY_MOD->GetTypeByIniName(iniName, matchFound);
}

SectionName GetNewUnitIniName(UnitType type)
{
	InitialiseModsIfRequired();

	return UNIT_MOD->GetTypeIniName(type);
}

UnitType GetNewUnitType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return UNIT_MOD->GetTypeByIniName(iniName, matchFound);
}
