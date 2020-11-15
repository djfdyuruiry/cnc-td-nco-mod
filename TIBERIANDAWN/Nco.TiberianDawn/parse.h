#pragma once

#include "../DEFINES.h"

// generic parsers
DiffType Parse_Difficulty_Type(char * difficultyTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Difficulty_Type_To_String(DiffType difficultyType);

HousesType Parse_House_Type(char* houseTypeString, bool* parseError, bool ignoreModTypes = false);

HousesType Parse_House_Type(const char* houseTypeString, bool* parseError, bool ignoreModTypes = false);

int Parse_House_Name_List_Csv(char* houseListCsv, bool* parseError, bool ignoreModTypes = false);

int Parse_House_Name_List_Csv(const char* houseListCsv, bool* parseError, bool ignoreModTypes = false);

const char* House_Type_To_String(HousesType houseType);

StructType Prerequisite_To_Structure_Type(long prerequisite);

const char * Prerequisite_To_String(long prerequisite);

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError, bool ignoreModTypes = false);

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Weapon_Type_To_String(WeaponType weaponType, bool ignoreModTypes = false);

ArmorType Parse_Armor_Type(char* armorTypeString, bool* parseError, bool ignoreModTypes = false);

ArmorType Parse_Armor_Type(const char* armorTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Armor_Type_To_String(ArmorType armorType);

BulletType Parse_Bullet_Type(char* bulletTypeString, bool* parseError, bool ignoreModTypes = false);

BulletType Parse_Bullet_Type(const char* bulletTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Bullet_Type_To_String(BulletType bulletType, bool ignoreModTypes = false);

WarheadType Parse_Warhead_Type(char* warheadTypeString, bool* parseError, bool ignoreModTypes = false);

WarheadType Parse_Warhead_Type(const char* warheadTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Warhead_Type_To_String(WarheadType warheadType, bool ignoreModTypes = false);

// infantry specific
InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError, bool ignoreModTypes = false);

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Infantry_Type_To_String(InfantryType infantryType, bool ignoreModTypes = false);

// unit specific
UnitType Parse_Unit_Type(char* unitTypeString, bool* parseError, bool ignoreModTypes = false);

UnitType Parse_Unit_Type(const char* unitTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Unit_Type_To_String(UnitType unitType, bool ignoreModTypes = false);

SpeedType Parse_Unit_Speed_Type(char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes = false);

SpeedType Parse_Unit_Speed_Type(const char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Unit_Speed_Type_To_String(SpeedType unitSpeedType);

// aircraft specific
AircraftType Parse_Aircraft_Type(char* aircraftTypeString, bool* parseError, bool ignoreModTypes = false);

AircraftType Parse_Aircraft_Type(const char* aircraftTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Aircraft_Type_To_String(AircraftType aircraftType, bool ignoreModTypes = false);

// building specific
StructType Parse_Structure_Type(char* structTypeString, bool* parseError, bool ignoreModTypes = false);

StructType Parse_Structure_Type(const char* structTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Structure_Type_To_String(StructType structType, bool ignoreModTypes = false);

long Structure_Type_To_Prerequisite(StructType structType, bool* parseError, bool ignoreModTypes = false);

FactoryType Parse_Factory_Type(char* factoryTypeString, bool* parseError, bool ignoreModTypes = false);

FactoryType Parse_Factory_Type(const char* factoryTypeString, bool* parseError, bool ignoreModTypes = false);

const char* Factory_Type_To_String(FactoryType factoryType);
