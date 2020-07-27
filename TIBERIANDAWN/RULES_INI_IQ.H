#pragma once

#include "rules_ini.h"
#include "IRulesIniSection.h"

#define IQ_SECTION_NAME "IQ"

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

#define Read_Max_Iq_Rule() ReadRuleValue<unsigned int>(IQ_SECTION_NAME, MAX_IQ_RULE)
#define Read_Iq_Rule(entry) ReadRuleValue<unsigned int>(IQ_SECTION_NAME, entry)
