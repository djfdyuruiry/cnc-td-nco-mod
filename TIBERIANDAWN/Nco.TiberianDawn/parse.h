#pragma once

#include "../DEFINES.h"

// generic parsers
DiffType ParseDifficultyType(char * difficultyTypeString, bool* parseError, bool ignoreModTypes = false);

const char* DifficultyTypeToString(DiffType difficultyType);

HousesType ParseHouseType(char* houseTypeString, bool* parseError, bool ignoreModTypes = false);

HousesType ParseHouseType(const char* houseTypeString, bool* parseError, bool ignoreModTypes = false);

int ParseHouseNameListCsv(char* houseListCsv, bool* parseError, bool ignoreModTypes = false);

int ParseHouseNameListCsv(const char* houseListCsv, bool* parseError, bool ignoreModTypes = false);

const char* HouseTypeToString(HousesType houseType);

StructType PrerequisiteToStructureType(long prerequisite);

const char * PrerequisiteToString(long prerequisite);

WeaponType ParseWeaponType(char* weaponTypeString, bool* parseError, bool ignoreModTypes = false);

WeaponType ParseWeaponType(const char* weaponTypeString, bool* parseError, bool ignoreModTypes = false);

const char* WeaponTypeToString(WeaponType weaponType, bool ignoreModTypes = false);

ArmorType ParseArmorType(char* armorTypeString, bool* parseError, bool ignoreModTypes = false);

ArmorType ParseArmorType(const char* armorTypeString, bool* parseError, bool ignoreModTypes = false);

const char* ArmorTypeToString(ArmorType armorType);

BulletType ParseBulletType(char* bulletTypeString, bool* parseError, bool ignoreModTypes = false);

BulletType ParseBulletType(const char* bulletTypeString, bool* parseError, bool ignoreModTypes = false);

const char* BulletTypeToString(BulletType bulletType, bool ignoreModTypes = false);

WarheadType ParseWarheadType(char* warheadTypeString, bool* parseError, bool ignoreModTypes = false);

WarheadType ParseWarheadType(const char* warheadTypeString, bool* parseError, bool ignoreModTypes = false);

const char* WarheadTypeToString(WarheadType warheadType, bool ignoreModTypes = false);

// infantry specific
InfantryType ParseInfantryType(char* infantryTypeString, bool* parseError, bool ignoreModTypes = false);

InfantryType ParseInfantryType(const char* infantryTypeString, bool* parseError, bool ignoreModTypes = false);

const char* InfantryTypeToString(InfantryType infantryType, bool ignoreModTypes = false);

// unit specific
UnitType ParseUnitType(char* unitTypeString, bool* parseError, bool ignoreModTypes = false);

UnitType ParseUnitType(const char* unitTypeString, bool* parseError, bool ignoreModTypes = false);

const char* UnitTypeToString(UnitType unitType, bool ignoreModTypes = false);

SpeedType ParseUnitSpeedType(char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes = false);

SpeedType ParseUnitSpeedType(const char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes = false);

const char* UnitSpeedTypeToString(SpeedType unitSpeedType);

// aircraft specific
AircraftType ParseAircraftType(char* aircraftTypeString, bool* parseError, bool ignoreModTypes = false);

AircraftType ParseAircraftType(const char* aircraftTypeString, bool* parseError, bool ignoreModTypes = false);

const char* AircraftTypeToString(AircraftType aircraftType, bool ignoreModTypes = false);

// building specific
StructType ParseStructureType(char* structTypeString, bool* parseError, bool ignoreModTypes = false);

StructType ParseStructureType(const char* structTypeString, bool* parseError, bool ignoreModTypes = false);

const char* StructureTypeToString(StructType structType, bool ignoreModTypes = false);

long StructureTypeToPrerequisite(StructType structType, bool* parseError, bool ignoreModTypes = false);

FactoryType ParseFactoryType(char* factoryTypeString, bool* parseError, bool ignoreModTypes = false);

FactoryType ParseFactoryType(const char* factoryTypeString, bool* parseError, bool ignoreModTypes = false);

const char* FactoryTypeToString(FactoryType factoryType);
