#pragma once

#define MOD_RULES_SECTION_NAME "Mods"

#define NEW_INFANTRY_RULE "NewInfantry"

#define BASE_TYPE_RULE "BaseType"

// virtual rules calculated on load of the rules file
#define NEW_INFANTRY_COUNT_RULE "NewInfantryCount"
#define INFANTRY_COUNT_RULE "InfantryCount"

void Read_Mods();

void Initialise_Infantry_Mod_Types();

const char* Get_New_Infantry_Ini_Name(char infantryType);

char Get_New_Infantry_Type(char* iniName, bool* matchFound);
