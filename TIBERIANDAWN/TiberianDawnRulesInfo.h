#pragma once

#include <IRulesIni.h>
#include <IRulesIniSection.h>
#include <rules_ini_enhancements.h>
#include <rules_ini_iq.h>
#include <rules_ini_nco.h>

#include "rules_ini_game.h"
#include "TiberianDawnRuleSectionBuilder.h"

static auto INFO_SECTION = "__INFO__";

class TiberianDawnRulesInfo
{
private:
	IRulesIni& rules;

	IRulesIniSection& weaponRules;
	IRulesIniSection& bulletRules;
	IRulesIniSection& warheadRules;
	IRulesIniSection& infantryRules;
	IRulesIniSection& unitRules;
	IRulesIniSection& aircraftRules;
	IRulesIniSection& buildingRules;

	TiberianDawnRulesInfo(IRulesIni& rulesIni)
		: rules(rulesIni),
		weaponRules(TiberianDawnRuleSectionBuilder::BuildWeaponSection(INFO_SECTION)),
		bulletRules(TiberianDawnRuleSectionBuilder::BuildBulletSection(INFO_SECTION)),
		warheadRules(TiberianDawnRuleSectionBuilder::BuildWarheadSection(INFO_SECTION)),
		infantryRules(TiberianDawnRuleSectionBuilder::BuildInfantrySection(INFO_SECTION)),
		unitRules(TiberianDawnRuleSectionBuilder::BuildUnitSection(INFO_SECTION)),
		aircraftRules(TiberianDawnRuleSectionBuilder::BuildAircraftSection(INFO_SECTION)),
		buildingRules(TiberianDawnRuleSectionBuilder::BuildBuildingSection(INFO_SECTION))
	{
	}

public:
	static TiberianDawnRulesInfo& BuildRuleInfo(IRulesIni& rulesIni)
	{
		return *(new TiberianDawnRulesInfo(rulesIni));
	}

	~TiberianDawnRulesInfo()
	{
		delete &weaponRules;
		delete &bulletRules;
		delete &warheadRules;
		delete &infantryRules;
		delete &unitRules;
		delete &aircraftRules;
		delete &buildingRules;
	}

	IRulesIniSection& GetNcoRules()
	{
		return rules[NCO_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetEnhancementRules()
	{
		return rules[ENHANCEMENTS_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetModRules()
	{
		return rules[MOD_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetGameRules()
	{
		return rules[GAME_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetDifficultyRules(SectionName difficulty)
	{
		return rules[difficulty];
	}

	IRulesIniSection& GetAiRules()
	{
		return rules[GAME_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetIqRules()
	{
		return rules[IQ_SECTION_NAME];
	}

	IRulesIniSection& GetSuperweaponRules(SectionName superwepaon)
	{
		return rules[superwepaon];
	}

	IRulesIniSection& GetWeaponRules()
	{
		return weaponRules;
	}

	IRulesIniSection& GetBulletRules()
	{
		return bulletRules;
	}

	IRulesIniSection& GetWarheadRules()
	{
		return warheadRules;
	}

	IRulesIniSection& GetInfantryRules()
	{
		return infantryRules;
	}

	IRulesIniSection& GetUnitRules()
	{
		return unitRules;
	}

	IRulesIniSection& GetAircraftRules()
	{
		return aircraftRules;
	}

	IRulesIniSection& GetBuildingRules()
	{
		return buildingRules;
	}
};
