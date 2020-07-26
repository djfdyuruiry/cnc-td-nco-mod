#pragma once

#include "rules_ini.h"
#include "IRulesIniSection.h"

#define MAX_IQ_RULE "MaxIQ"
#define IQ_SUPER_WEAPONS_RULE "SuperWeapons"
#define IQ_PRODUCTION_RULE "Production"
#define IQ_GUARD_AREA_RULE "GuardArea"
#define IQ_REPAIR_SELL_RULE "RepairSell"
#define IQ_CRUSH_RULE "Crush"
#define IQ_SCATTER_RULE "Scatter"
#define IQ_CONTENT_SCAN_RULE "Scan"
#define IQ_AIRCRAFT_RULE "Aircraft"
#define IQ_HARVESTER_RULE "Harvester"
#define IQ_SELL_BACK_RULE "SellBack"

extern IRulesIniSection& IqRules;

#define Read_Max_Iq_Rule() Read_Unsigned_Int_From_Rules_Ini(IqRules[MAX_IQ_RULE], 0, 0, UINT_MAX)
#define Read_Iq_Rule(entry, defaultValue) Read_Unsigned_Int_From_Rules_Ini(IqRules[entry], defaultValue, 1, Read_Max_Iq_Rule())
