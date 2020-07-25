#include "rules_ini_iq.h"
#include "RulesIniSection.h"

RulesIniSection IqRules("IQ", [](RulesIniSection& s) {
	s << MAX_IQ_RULE << 5
		<< IQ_SUPER_WEAPONS_RULE
		<< IQ_PRODUCTION_RULE
		<< IQ_GUARD_AREA_RULE
		<< IQ_REPAIR_SELL_RULE
		<< IQ_CRUSH_RULE
		<< IQ_SCATTER_RULE
		<< IQ_CONTENT_SCAN_RULE
		<< IQ_AIRCRAFT_RULE
		<< IQ_HARVESTER_RULE
		<< IQ_SELL_BACK_RULE;
});
