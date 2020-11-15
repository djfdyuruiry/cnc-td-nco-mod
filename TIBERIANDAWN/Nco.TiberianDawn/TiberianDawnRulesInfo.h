#pragma once

#include <IRulesIniSection.h>
#include <IRulesRuntime.h>
#include <rules_ini_enhancements.h>
#include <rules_ini_iq.h>
#include <rules_ini_nco.h>

#include "rules_ini_game.h"
#include "TiberianDawnRuleSectionBuilder.h"

static auto INFO_SECTION = "__INFO__";

class TiberianDawnRulesInfo
{
private:
	IRulesRuntime& rulesRuntime;

	IRulesIniSection& weaponRules;
	IRulesIniSection& bulletRules;
	IRulesIniSection& warheadRules;
	IRulesIniSection& infantryRules;
	IRulesIniSection& unitRules;
	IRulesIniSection& aircraftRules;
	IRulesIniSection& buildingRules;

	TiberianDawnRulesInfo(IRulesRuntime& rulesRuntime)
		: rulesRuntime(rulesRuntime),
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
	static TiberianDawnRulesInfo& BuildRuleInfo(IRulesRuntime& rulesRuntime)
	{
		return *(new TiberianDawnRulesInfo(rulesRuntime));
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
		return rulesRuntime.GetRules()[NCO_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetEnhancementRules()
	{
		return rulesRuntime.GetRules()[ENHANCEMENTS_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetModRules()
	{
		return rulesRuntime.GetRules()[MOD_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetGameRules()
	{
		return rulesRuntime.GetRules()[GAME_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetDifficultyRules(SectionName difficulty)
	{
		return rulesRuntime.GetRules()[difficulty];
	}

	IRulesIniSection& GetAiRules()
	{
		return rulesRuntime.GetRules()[GAME_RULES_SECTION_NAME];
	}

	IRulesIniSection& GetIqRules()
	{
		return rulesRuntime.GetRules()[IQ_SECTION_NAME];
	}

	IRulesIniSection& GetSuperweaponRules(SectionName superwepaon)
	{
		return rulesRuntime.GetRules()[superwepaon];
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
