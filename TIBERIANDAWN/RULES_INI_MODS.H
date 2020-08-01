#pragma once

#include "defines.h"

#include "RulesIniTypes.h"

#define MOD_RULES_SECTION_NAME "Mods"

//#define NEW_WEAPONS_RULE "NewWeapons"

//#define NEW_BULLETS_RULE "NewBullets"

//#define NEW_WARHEADS_RULE "NewWarheads"

#define NEW_INFANTRY_RULE "NewInfantry"

#define NEW_UNITS_RULE "NewUnits"

//#define NEW_AIRCRAFT_RULE "NewAircraft"

//#define NEW_BUILDINGS_RULE "NewBuildings"

#define BASE_TYPE_RULE "BaseType"

// virtual rules calculated on load of the rules file
#define NEW_INFANTRY_COUNT_RULE "NewInfantryCount"
#define INFANTRY_COUNT_RULE "InfantryCount"

#define NEW_UNIT_COUNT_RULE "NewUnitCount"
#define UNIT_COUNT_RULE "UnitCount"

void ReadMods();
void InitialiseModsIfRequired();

SectionName GetNewInfantryIniName(InfantryType infantryType);
InfantryType GetNewInfantryType(SectionName iniName, bool* matchFound);

SectionName GetNewUnitIniName(UnitType type);
UnitType GetNewUnitType(SectionName iniName, bool* matchFound);