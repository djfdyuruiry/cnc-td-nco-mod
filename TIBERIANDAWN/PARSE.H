#ifndef PARSE_H
#define PARSE_H

#include "defines.h"

HousesType Parse_House_Type(char* houseTypeString, bool* parseError);

HousesType Parse_House_Type(const char* houseTypeString, bool* parseError);

char* House_Type_To_String(HousesType houseType);

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError);

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError);

char* Weapon_Type_To_String(WeaponType weaponType);
StructType Prerequisite_To_Structure_Type(long prerequisite);

SpeedType Parse_Unit_Speed_Type(char* unitSpeedTypeString, bool* parseError);

SpeedType Parse_Unit_Speed_Type(const char* unitSpeedTypeString, bool* parseError);

char* Unit_Speed_Type_To_String(SpeedType unitSpeedType);

InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError);

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError);

char* Infantry_Type_To_String(InfantryType infantryType);

UnitType Parse_Unit_Type(char* unitTypeString, bool* parseError);

UnitType Parse_Unit_Type(const char* unitTypeString, bool* parseError);

char* Unit_Type_To_String(UnitType unitType);

StructType Parse_Structure_Type(char* structTypeString, bool* parseError);

StructType Parse_Structure_Type(const char* structTypeString, bool* parseError);

char* Structure_Type_To_String(StructType structType);


#endif
