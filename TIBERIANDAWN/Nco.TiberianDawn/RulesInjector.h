#pragma once

#include <IRulesIni.h>
#include <IRulesIniReader.h>

#include "..\RULES.H"

#include "rules_ini_ai.h"
#include "rules_ini_iq.h"
#include "TiberianDawnRuleSectionBuilder.h"

class RulesInjector
{
private:
	IRulesIni& rules;
    IRulesIniReader& rulesReader;

	RulesInjector(IRulesIni& rules, IRulesIniReader& rulesReader) : rules(rules), rulesReader(rulesReader)
	{
	}

public:
	static RulesInjector& Build(IRulesIni& rules, IRulesIniReader& rulesReader)
	{
		return *(new RulesInjector(rules, rulesReader));
	}

	template<class T, class U> void InjectTypeRules(
		T first,
		T count
	)
	{
		for (auto t = first; t < count; t++)
		{
			auto& instance = (U&)U::As_Reference(t);

			rules << TiberianDawnRuleSectionBuilder::BuildSectionForType<U>(instance.Get_Rules_Name());

			instance.Read_Rules(rulesReader);
		}
	}

    void InjectIqRules(RulesClass rulesClassInstance)
    {
        rulesClassInstance.MaxIQ = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, MAX_IQ_RULE);
        rulesClassInstance.IQSuperWeapons = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SUPER_WEAPONS_RULE);
        rulesClassInstance.IQProduction = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_PRODUCTION_RULE);
        rulesClassInstance.IQGuardArea = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_GUARD_AREA_RULE);
        rulesClassInstance.IQRepairSell = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_REPAIR_SELL_RULE);
        rulesClassInstance.IQCrush = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_CRUSH_RULE);
        rulesClassInstance.IQScatter = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SCATTER_RULE);
        rulesClassInstance.IQContentScan = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_CONTENT_SCAN_RULE);
        rulesClassInstance.IQAircraft = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_AIRCRAFT_RULE);
        rulesClassInstance.IQHarvester = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_HARVESTER_RULE);
        rulesClassInstance.IQSellBack = rulesReader.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SELL_BACK_RULE);
    }

    void InjectAiRules(RulesClass rulesClassInstance)
    {
        rulesClassInstance.AttackInterval = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, ATTACK_INTERVAL_RULE);
        rulesClassInstance.BaseSizeAdd = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BASE_SIZE_ADD_RULE);
        rulesClassInstance.PowerSurplus = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, POWER_SURPLUS_RULE);
        rulesClassInstance.AttackDelay = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, ATTACK_DELAY_RULE);
        rulesClassInstance.PowerEmergencyFraction = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, POWER_EMERGENCY_FRACTION_RULE);
        rulesClassInstance.HelipadRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, HELIPAD_RATIO_RULE);
        rulesClassInstance.HelipadLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, HELIPAD_LIMIT_RULE);
        rulesClassInstance.AARatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AA_RATIO_RULE);
        rulesClassInstance.AALimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AA_LIMIT_RULE);
        rulesClassInstance.DefenseRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, DEFENSE_RATIO_RULE);
        rulesClassInstance.DefenseLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, DEFENSE_LIMIT_RULE);
        rulesClassInstance.WarRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, WAR_RATIO_RULE);
        rulesClassInstance.WarLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, WAR_LIMIT_RULE);
        rulesClassInstance.BarracksRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BARRACKS_RATIO_RULE);
        rulesClassInstance.BarracksLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BARRACKS_LIMIT_RULE);
        rulesClassInstance.RefineryRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, REFINERY_RATIO_RULE);
        rulesClassInstance.RefineryLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, REFINERY_LIMIT_RULE);
        rulesClassInstance.AirstripRatio = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AIRSTRIP_RATIO_RULE);
        rulesClassInstance.AirstripLimit = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AIRSTRIP_LIMIT_RULE);
        rulesClassInstance.InfantryReserve = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, INFANTRY_RESERVE_RULE);
        rulesClassInstance.InfantryBaseMult = rulesReader.ReadRuleValue<unsigned int>(AI_SECTION_NAME, INFANTRY_BASE_MULT_RULE);
        rulesClassInstance.IsComputerParanoid = rulesReader.ReadRuleValue<bool>(AI_SECTION_NAME, PARANOID_RULE);
    }

};
