#pragma once

#include "IRulesIniSection.h"
#include "rules_ini_nco.h"
#include "rules_ini_game.h"
#include "RulesIni.h"
#include "RulesIniTemplates.h"

static auto INFO = "INFO";

class RulesIniInfo
{
private:
	RulesIni& rules;

	IRulesIniSection& weaponRules;
	IRulesIniSection& bulletRules;
	IRulesIniSection& warheadRules;
	IRulesIniSection& infantryRules;
	IRulesIniSection& unitRules;
	IRulesIniSection& aircraftRules;
	IRulesIniSection& buildingRules;

	RulesIniInfo(RulesIni& rulesIni)
		: rules(rulesIni),
		weaponRules(RulesIniTemplates::BuildWeaponSection(INFO)),
		bulletRules(RulesIniTemplates::BuildBulletSection(INFO)),
		warheadRules(RulesIniTemplates::BuildWarheadSection(INFO)),
		infantryRules(RulesIniTemplates::BuildInfantrySection(INFO)),
		unitRules(RulesIniTemplates::BuildUnitSection(INFO)),
		aircraftRules(RulesIniTemplates::BuildAircraftSection(INFO)),
		buildingRules(RulesIniTemplates::BuildBuildingSection(INFO))
	{
	}

public:
	static RulesIniInfo& BuildRuleInfo(RulesIni& rulesIni)
	{
		return *(new RulesIniInfo(rulesIni));
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
