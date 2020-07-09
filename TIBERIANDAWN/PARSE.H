#ifndef PARSE_H
#define PARSE_H

#include "defines.h"

// generic parsers
HousesType Parse_House_Type(char* houseTypeString, bool* parseError);

HousesType Parse_House_Type(const char* houseTypeString, bool* parseError);

int Parse_House_Name_List_Csv(char* houseListCsv, bool* parseError);

int Parse_House_Name_List_Csv(const char* houseListCsv, bool* parseError);

char* House_Type_To_String(HousesType houseType);

StructType Prerequisite_To_Structure_Type(long prerequisite);

char * Prerequisite_To_String(long prerequisite);

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError);

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError);

char* Weapon_Type_To_String(WeaponType weaponType);

ArmorType Parse_Armor_Type(char* armorTypeString, bool* parseError);

ArmorType Parse_Armor_Type(const char* armorTypeString, bool* parseError);

char* Armor_Type_To_String(ArmorType armorType);


// infantry specific
InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError);

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError);

char* Infantry_Type_To_String(InfantryType infantryType);

// unit specific
UnitType Parse_Unit_Type(char* unitTypeString, bool* parseError);

UnitType Parse_Unit_Type(const char* unitTypeString, bool* parseError);

char* Unit_Type_To_String(UnitType unitType);

SpeedType Parse_Unit_Speed_Type(char* unitSpeedTypeString, bool* parseError);

SpeedType Parse_Unit_Speed_Type(const char* unitSpeedTypeString, bool* parseError);

char* Unit_Speed_Type_To_String(SpeedType unitSpeedType);

// aircraft specific
AircraftType Parse_Aircraft_Type(char* aircraftTypeString, bool* parseError);

AircraftType Parse_Aircraft_Type(const char* aircraftTypeString, bool* parseError);

char* Aircraft_Type_To_String(AircraftType aircraftType);

// building specific

StructType Parse_Structure_Type(char* structTypeString, bool* parseError);

StructType Parse_Structure_Type(const char* structTypeString, bool* parseError);

char* Structure_Type_To_String(StructType structType);

long Structure_Type_To_Prerequisite(StructType structType, bool* parseError);

FactoryType Parse_Factory_Type(char* factoryTypeString, bool* parseError);

FactoryType Parse_Factory_Type(const char* factoryTypeString, bool* parseError);

char* Factory_Type_To_String(FactoryType factoryType);

#endif
