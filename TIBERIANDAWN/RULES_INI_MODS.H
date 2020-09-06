#pragma once

#include "defines.h"

#include "RulesIniTypes.h"

#define MOD_RULES_SECTION_NAME "Mods"


#define NEW_AIRCRAFT_RULE "NewAircraft"

#define NEW_BUILDINGS_RULE "NewBuildings"

#define NEW_BULLETS_RULE "NewBullets"

#define NEW_INFANTRY_RULE "NewInfantry"

#define NEW_UNITS_RULE "NewUnits"

#define NEW_WARHEADS_RULE "NewWarheads"

#define NEW_WEAPONS_RULE "NewWeapons"


#define BASE_TYPE_RULE "BaseType"

// virtual rules calculated on load of the rules file
#define NEW_AIRCRAFT_COUNT_RULE "NewAircraftCount"
#define AIRCRAFT_COUNT_RULE "AircraftCount"

#define NEW_BUILDING_COUNT_RULE "NewBuildingCount"
#define BUILDING_COUNT_RULE "BuildingCount"

#define NEW_BULLET_COUNT_RULE "NewBulletCount"
#define BULLET_COUNT_RULE "BulletCount"

#define NEW_INFANTRY_COUNT_RULE "NewInfantryCount"
#define INFANTRY_COUNT_RULE "InfantryCount"

#define NEW_UNIT_COUNT_RULE "NewUnitCount"
#define UNIT_COUNT_RULE "UnitCount"

#define NEW_WARHEAD_COUNT_RULE "NewWarheadCount"
#define WARHEAD_COUNT_RULE "WarheadCount"

#define NEW_WEAPON_COUNT_RULE "NewWeaponCount"
#define WEAPON_COUNT_RULE "Count"

void ReadMods();
void InitialiseModsIfRequired();

SectionName GetNewAircraftIniName(AircraftType AircraftType);
AircraftType GetNewAircraftType(SectionName iniName, bool* matchFound);

SectionName GetNewBuildingIniName(StructType BuildingType);
StructType GetNewBuildingType(SectionName iniName, bool* matchFound);

SectionName GetNewBulletIniName(BulletType BulletType);
BulletType GetNewBulletType(SectionName iniName, bool* matchFound);

SectionName GetNewInfantryIniName(InfantryType infantryType);
InfantryType GetNewInfantryType(SectionName iniName, bool* matchFound);

SectionName GetNewUnitIniName(UnitType type);
UnitType GetNewUnitType(SectionName iniName, bool* matchFound);

SectionName GetNewWarheadIniName(WarheadType WarheadType);
WarheadType GetNewWarheadType(SectionName iniName, bool* matchFound);

SectionName GetNewWeaponIniName(WeaponType WeaponType);
WeaponType GetNewWeaponType(SectionName iniName, bool* matchFound);