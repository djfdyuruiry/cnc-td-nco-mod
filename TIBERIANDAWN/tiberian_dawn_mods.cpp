#include <IRulesRuntime.h>
#include <logger.h>
#include <RulesIniTypes.h>

#include "function.h"

#include "AircraftTypeMod.h"
#include "BuildingTypeMod.h"
#include "BulletTypeMod.h"
#include "InfantryTypeMod.h"
#include "UnitTypeMod.h"
#include "WarheadTypeMod.h"
#include "WeaponTypeMod.h"

static AircraftTypeMod* AIRCRAFT_MOD;
static BuildingTypeMod* BUILDING_MOD;
static BulletTypeMod* BULLET_MOD;
static InfantryTypeMod* INFANTRY_MOD;
static UnitTypeMod* UNIT_MOD;
static WarheadTypeMod* WARHEAD_MOD;
static WeaponTypeMod* WEAPON_MOD;

static auto MODS_INITIALISED = false;

void ReadMods(IRulesRuntime& runtime)
{
	Log_Info("Reading Mods");

	WARHEAD_MOD = &WarheadTypeMod::Build(runtime);
	BULLET_MOD = &BulletTypeMod::Build(runtime);
	WEAPON_MOD = &WeaponTypeMod::Build(runtime);
	BUILDING_MOD = &BuildingTypeMod::Build(runtime);
	AIRCRAFT_MOD = &AircraftTypeMod::Build(runtime);
	INFANTRY_MOD = &InfantryTypeMod::Build(runtime);
	UNIT_MOD = &UnitTypeMod::Build(runtime);

	WARHEAD_MOD->ReadTypes();
	BULLET_MOD->ReadTypes();
	WEAPON_MOD->ReadTypes();
	BUILDING_MOD->ReadTypes();
	AIRCRAFT_MOD->ReadTypes();
	INFANTRY_MOD->ReadTypes();
	UNIT_MOD->ReadTypes();
}

void InitialiseModsIfRequired()
{
	if (MODS_INITIALISED)
	{
		return;
	}

	MODS_INITIALISED = true;

	Log_Trace("InitialiseModsIfRequired starting");

	WARHEAD_MOD->InitialiseTypes();
	BULLET_MOD->InitialiseTypes();
	WEAPON_MOD->InitialiseTypes();
	BUILDING_MOD->InitialiseTypes();
	AIRCRAFT_MOD->InitialiseTypes();
	INFANTRY_MOD->InitialiseTypes();
	UNIT_MOD->InitialiseTypes();
}

SectionName GetNewAircraftIniName(AircraftType type)
{
	InitialiseModsIfRequired();

	return AIRCRAFT_MOD->GetTypeIniName(type);
}

AircraftType GetNewAircraftType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return AIRCRAFT_MOD->GetTypeByIniName(iniName, matchFound);
}

SectionName GetNewBuildingIniName(StructType type)
{
	InitialiseModsIfRequired();

	return BUILDING_MOD->GetTypeIniName(type);
}

StructType GetNewBuildingType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return BUILDING_MOD->GetTypeByIniName(iniName, matchFound);
}

SectionName GetNewBulletIniName(BulletType type)
{
	InitialiseModsIfRequired();

	return BULLET_MOD->GetTypeIniName(type);
}

BulletType GetNewBulletType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return BULLET_MOD->GetTypeByIniName(iniName, matchFound);
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

SectionName GetNewWarheadIniName(WarheadType type)
{
	InitialiseModsIfRequired();

	return WARHEAD_MOD->GetTypeIniName(type);
}

WarheadType GetNewWarheadType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return WARHEAD_MOD->GetTypeByIniName(iniName, matchFound);
}

SectionName GetNewWeaponIniName(WeaponType type)
{
	InitialiseModsIfRequired();

	return WEAPON_MOD->GetTypeIniName(type);
}

WeaponType GetNewWeaponType(SectionName iniName, bool* matchFound)
{
	InitialiseModsIfRequired();

	return WEAPON_MOD->GetTypeByIniName(iniName, matchFound);
}
