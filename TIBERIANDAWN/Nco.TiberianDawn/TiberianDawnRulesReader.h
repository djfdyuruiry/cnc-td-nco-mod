#pragma once

#include <RulesIniReader.h>

#include "../DEFINES.H"

#include "parse.h"
#include "tiberian_dawn_rule_types.h"

class TiberianDawnRulesReader : public RulesIniReader
{
private:
	TiberianDawnRulesReader(IRulesIni& rules) : RulesIniReader(rules)
	{
	}

	long ReadPrerequisiteRule(RulesIniRule& rule)
	{
		Optional& structValueStrOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!structValueStrOptional.Present())
		{
			delete& structValueStrOptional;

			return rule.GetDefaultValueOr(STRUCTF_NONE);
		}

		auto structValueStr = structValueStrOptional.Get<char*>();

		Convert_String_To_Upper_Case(structValueStr);

		bool parseError = false;
		auto structValue = Parse_Structure_Type(structValueStr, &parseError);

		delete& structValueStrOptional;

		if (parseError)
		{
			// unable to parse entry as a structure type
			rulesIni.MarkAsInvalid();

			Show_Error("Failed to parse prerequisite for [%s]: %s", rule.GetStringKey(), structValueStr);

			return STRUCTF_NONE;
		}

		if (structValue == STRUCT_NONE)
		{
			return STRUCTF_NONE;
		}

		return 1L << structValue;
	}

	int ReadHouseListRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"houses",
			&Parse_House_Name_List_Csv,
			(int)HOUSEF_NONE
		);
	}

	WeaponType ReadWeaponRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"weapon",
			&Parse_Weapon_Type,
			WEAPON_NONE
		);
	}

	ArmorType ReadArmorRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"armor",
			&Parse_Armor_Type,
			ARMOR_NONE
		);
	}

	SpeedType ReadUnitSpeedRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"unit speed",
			&Parse_Unit_Speed_Type,
			SPEED_NONE
		);
	}

	FactoryType ReadFactoryRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"factory type",
			&Parse_Factory_Type,
			FACTORY_TYPE_NONE
		);
	}

	WarheadType ReadWarheadRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"warhead",
			&Parse_Warhead_Type,
			WARHEAD_NONE
		);
	}

	BulletType ReadBulletRule(RulesIniRule& rule)
	{
		return GetParsedStringRule(
			rule,
			"bullet",
			&Parse_Bullet_Type,
			BULLET_NONE
		);
	}

	bool SetDefaultRuleValue(RulesIniRule& rule)
	{
		auto ruleType = rule.GetType();

		if (ruleType == HOUSE_LIST_RULE)
		{
			rule.SetValue(
				ReadHouseListRule(rule)
			);
		}
		else if (ruleType == WEAPON_RULE)
		{
			rule.SetValue(
				ReadWeaponRule(rule)
			);
		}
		else if (ruleType == ARMOR_TYPE_RULE)
		{
			rule.SetValue(
				ReadArmorRule(rule)
			);
		}
		else if (ruleType == UNIT_SPEED_TYPE_RULE)
		{
			rule.SetValue(
				ReadUnitSpeedRule(rule)
			);
		}
		else if (ruleType == FACTORY_RULE_TYPE)
		{
			rule.SetValue(
				ReadFactoryRule(rule)
			);
		}
		else if (ruleType == WARHEAD_RULE)
		{
			rule.SetValue(
				ReadWarheadRule(rule)
			);
		}
		else if (ruleType == BULLET_RULE)
		{
			rule.SetValue(
				ReadBulletRule(rule)
			);
		}
		else if (ruleType == PREREQ_RULE)
		{
			rule.SetValue(
				ReadPrerequisiteRule(rule)
			);
		}
		else
		{
			return RulesIniReader::SetDefaultRuleValue(rule);
		}

		return true;
	}

public:
	static TiberianDawnRulesReader& Build(IRulesIni& rules)
	{
		return *(new TiberianDawnRulesReader(rules));
	}

};
