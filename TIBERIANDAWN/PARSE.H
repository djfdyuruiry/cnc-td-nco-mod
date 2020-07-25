#pragma once

#include "defines.h"

// generic parsers
DiffType Parse_Difficulty_Type(char * difficultyTypeString, bool* parseError);

const char* Difficulty_Type_To_String(DiffType difficultyType);

HousesType Parse_House_Type(char* houseTypeString, bool* parseError);

HousesType Parse_House_Type(const char* houseTypeString, bool* parseError);

int Parse_House_Name_List_Csv(char* houseListCsv, bool* parseError);

int Parse_House_Name_List_Csv(const char* houseListCsv, bool* parseError);

const char* House_Type_To_String(HousesType houseType);

StructType Prerequisite_To_Structure_Type(long prerequisite);

const char * Prerequisite_To_String(long prerequisite);

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError);

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError);

const char* Weapon_Type_To_String(WeaponType weaponType);

ArmorType Parse_Armor_Type(char* armorTypeString, bool* parseError);

ArmorType Parse_Armor_Type(const char* armorTypeString, bool* parseError);

const char* Armor_Type_To_String(ArmorType armorType);

BulletType Parse_Bullet_Type(char* bulletTypeString, bool* parseError);

BulletType Parse_Bullet_Type(const char* bulletTypeString, bool* parseError);

const char* Bullet_Type_To_String(BulletType bulletType);

WarheadType Parse_Warhead_Type(char* warheadTypeString, bool* parseError);

WarheadType Parse_Warhead_Type(const char* warheadTypeString, bool* parseError);

const char* Warhead_Type_To_String(WarheadType warheadType);

// infantry specific
InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError);

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError);

const char* Infantry_Type_To_String(InfantryType infantryType);

// unit specific
UnitType Parse_Unit_Type(char* unitTypeString, bool* parseError);

UnitType Parse_Unit_Type(const char* unitTypeString, bool* parseError);

const char* Unit_Type_To_String(UnitType unitType);

SpeedType Parse_Unit_Speed_Type(char* unitSpeedTypeString, bool* parseError);

SpeedType Parse_Unit_Speed_Type(const char* unitSpeedTypeString, bool* parseError);

const char* Unit_Speed_Type_To_String(SpeedType unitSpeedType);

// aircraft specific
AircraftType Parse_Aircraft_Type(char* aircraftTypeString, bool* parseError);

AircraftType Parse_Aircraft_Type(const char* aircraftTypeString, bool* parseError);

const char* Aircraft_Type_To_String(AircraftType aircraftType);

// building specific

StructType Parse_Structure_Type(char* structTypeString, bool* parseError);

StructType Parse_Structure_Type(const char* structTypeString, bool* parseError);

const char* Structure_Type_To_String(StructType structType);

long Structure_Type_To_Prerequisite(StructType structType, bool* parseError);

FactoryType Parse_Factory_Type(char* factoryTypeString, bool* parseError);

FactoryType Parse_Factory_Type(const char* factoryTypeString, bool* parseError);

const char* Factory_Type_To_String(FactoryType factoryType);
