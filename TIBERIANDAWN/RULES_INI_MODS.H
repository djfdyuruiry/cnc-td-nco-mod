#ifndef RULES_INI_MODS_H
#define RULES_INI_MODS_H

#define MOD_RULES_SECTION_NAME "Mods"

#define NEW_INFANTRY_RULE "NewInfantry"
#define NEW_INFANTRY_RULE_UPPER "NEWINFANTRY"
#define NEW_INFANTRY_RULE_TYPE "string"

#define BASE_TYPE_RULE "BaseType"
#define BASE_TYPE_RULE_UPPER "BASETYPE"
#define BASE_TYPE_RULE_TYPE "string"

// virtual rules calculated on load of the rules file
#define NEW_INFANTRY_COUNT_RULE "NewInfantryCount"
#define NEW_INFANTRY_COUNT_RULE_UPPER "NEWINFANTRYCOUNT"
#define INFANTRY_COUNT_RULE_TYPE "uint"

#define INFANTRY_COUNT_RULE "InfantryCount"
#define INFANTRY_COUNT_RULE_UPPER "INFANTRYCOUNT"
#define INFANTRY_COUNT_RULE_TYPE "uint"

#define MOD_RULES NEW_INFANTRY_RULE

#define MOD_RULE_COUNT 1u

#define MOD_RULE_TYPES NEW_INFANTRY_RULE_UPPER

#define Read_New_Infantry_Count() (char)Read_Unsigned_Int_From_Rules_Ini(MOD_RULES_SECTION_NAME, NEW_INFANTRY_COUNT_RULE, 0, 0, CHAR_MAX)

#define Read_Infantry_Count(baseInfantryCount) (char)Read_Cached_Unsigned_Int_From_Rules_Ini(MOD_RULES_SECTION_NAME, INFANTRY_COUNT_RULE, INFANTRY_COUNT_RULE_KEY, baseInfantryCount, INFANTRY_FIRST, CHAR_MAX)

void Read_Mods();

void Initialise_Infantry_Mod_Types();

const char* Get_New_Infantry_Ini_Name(char infantryType);

char Get_New_Infantry_Type(char* iniName, bool* matchFound);

#endif
