#pragma once

#include <HashUtils.h>
#include <rules_ini_mods.h>

// virtual rules calculated on load of the rules file
#define NEW_AIRCRAFT_RULE "NewAircraft"
#define NEW_AIRCRAFT_COUNT_RULE "NewAircraftCount"
#define AIRCRAFT_COUNT_RULE "AircraftCount"

#define NEW_BUILDINGS_RULE "NewBuildings"
#define NEW_BUILDING_COUNT_RULE "NewBuildingCount"
#define BUILDING_COUNT_RULE "BuildingCount"

#define NEW_BULLETS_RULE "NewBullets"
#define NEW_BULLET_COUNT_RULE "NewBulletCount"
#define BULLET_COUNT_RULE "BulletCount"

#define NEW_INFANTRY_RULE "NewInfantry"
#define NEW_INFANTRY_COUNT_RULE "NewInfantryCount"
#define INFANTRY_COUNT_RULE "InfantryCount"

#define NEW_UNITS_RULE "NewUnits"
#define NEW_UNIT_COUNT_RULE "NewUnitCount"
#define UNIT_COUNT_RULE "UnitCount"

#define NEW_WARHEADS_RULE "NewWarheads"
#define NEW_WARHEAD_COUNT_RULE "NewWarheadCount"
#define WARHEAD_COUNT_RULE "WarheadCount"

#define NEW_WEAPONS_RULE "NewWeapons"
#define NEW_WEAPON_COUNT_RULE "NewWeaponCount"
#define WEAPON_COUNT_RULE "WeaponCount"

#define GetModByTypeKey(t, u, p1) NcoModsRuntime().GetModByTypeKey<t, u>(HashUtils::HashString(p1))

#define GetModTypeIniName(t, u, p1, p2) GetModByTypeKey(t, u, p1).GetTypeIniName(p2)

#define GetModType(t, u, p1, p2, p3) GetModByTypeKey(t, u, p1).GetTypeByIniName(p2, p3)

#define GetNewAircraftIniName(t) GetModTypeIniName(AircraftType, AircraftTypeClass, "Aircraft", t)

#define GetNewAircraftType(i, t) GetModType(AircraftType, AircraftTypeClass, "Aircraft", i, t)

#define GetNewBuildingIniName(t) GetModTypeIniName(StructType, BuildingTypeClass, "Building", t)

#define GetNewBuildingType(i, t) GetModType(StructType, BuildingTypeClass, "Building", i, t)

#define GetNewBulletIniName(t) GetModTypeIniName(BulletType, BulletTypeClass, "Bullet", t)

#define GetNewBulletType(i, t) GetModType(BulletType, BulletTypeClass, "Bullet", i, t)

#define GetNewInfantryIniName(t) GetModTypeIniName(InfantryType, InfantryTypeClass, "Infantry", t)

#define GetNewInfantryType(i, t) GetModType(InfantryType, InfantryTypeClass, "Infantry", i, t)

#define GetNewUnitIniName(t) GetModTypeIniName(UnitType, UnitTypeClass, "Unit", t)

#define GetNewUnitType(i, t) GetModType(UnitType, UnitTypeClass, "Unit", i, t)

#define GetNewWarheadIniName(t) GetModTypeIniName(WarheadType, WarheadTypeClass, "Warhead", t)

#define GetNewWarheadType(i, t) GetModType(WarheadType, WarheadTypeClass, "Warhead", i, t)

#define GetNewWeaponIniName(t) GetModTypeIniName(WeaponType, WeaponTypeClass, "Weapon", t)

#define GetNewWeaponType(i, t) GetModType(WeaponType, WeaponTypeClass, "Weapon", i, t)
