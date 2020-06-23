#ifndef PARSE_H
#define PARSE_H

#include "defines.h"

HousesType Parse_House_Type(char* houseTypeString, bool* parseError);

char* House_Type_To_String(HousesType houseType);

StructType Parse_Structure_Type(char* structTypeString, bool* parseError);

char* Structure_Type_To_String(StructType structType);

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError);

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError);

char* Weapon_Type_To_String(WeaponType weaponType);

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError);

InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError);

#endif
