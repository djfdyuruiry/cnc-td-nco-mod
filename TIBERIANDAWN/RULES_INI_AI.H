#pragma once

#include "rules_ini.h"
#include "IRulesIniSection.h"

#define ATTACK_INTERVAL_RULE "AttackInterval"
#define BASE_SIZE_ADD_RULE "BaseSizeAdd"
#define POWER_SURPLUS_RULE "PowerSurplus"
#define ATTACK_DELAY_RULE "AttackDelay"
#define POWER_EMERGENCY_FRACTION_RULE "PowerEmergency"
#define HELIPAD_RATIO_RULE "HelipadRatio"
#define HELIPAD_LIMIT_RULE "HelipadLimit"
#define AA_RATIO_RULE "AARatio"
#define AA_LIMIT_RULE "AALimit"
#define DEFENSE_RATIO_RULE "DefenseRatio"
#define DEFENSE_LIMIT_RULE "DefenseLimit"
#define WAR_RATIO_RULE "WarRatio"
#define WAR_LIMIT_RULE "WarLimit"
#define BARRACKS_RATIO_RULE "BarracksRatio"
#define BARRACKS_LIMIT_RULE "BarracksLimit"
#define REFINERY_LIMIT_RULE "RefineryLimit"
#define REFINERY_RATIO_RULE "RefineryRatio"
#define AIRSTRIP_RATIO_RULE "AirstripRatio"
#define AIRSTRIP_LIMIT_RULE "AirstripLimit"
#define INFANTRY_RESERVE_RULE "InfantryReserve"
#define INFANTRY_BASE_MULT_RULE "InfantryBaseMult"
#define PARANOID_RULE "Paranoid"

extern IRulesIniSection& AiRules;

#define Read_Unsigned_Int_Ai_Rule(entry, defaultValue) Read_Unsigned_Int_From_Rules_Ini(AiRules[entry], defaultValue, 0, UINT_MAX)
#define Read_Bool_Ai_Rule(entry, defaultValue) Read_Bool_From_Rules_Ini(AiRules[entry], defaultValue)
#define Read_Ratio_Ai_Rule(entry, defaultValue, defaultAsPercentage) Read_Fixed_From_Rules_Ini(AiRules[entry], defaultValue, defaultAsPercentage)
#define Read_Fraction_Ai_Rule(entry, defaultValue, defaultAsPercentage) Read_Fixed_From_Rules_Ini(AiRules[entry], defaultValue, defaultAsPercentage)
