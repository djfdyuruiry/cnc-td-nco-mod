#pragma once

#include <RulesIniReader.h>

#include "../DEFINES.H"

#include "tiberian_dawn_rule_types.h"
#include "TiberianDawnTypeConverter.h"

class TiberianDawnRulesReader : public RulesIniReader
{
private:
	TiberianDawnTypeConverter& typeConverter;

	TiberianDawnRulesReader(IRulesIni& rules, TiberianDawnTypeConverter& typeConverter) : RulesIniReader(rules), typeConverter(typeConverter)
	{
	}

	long ReadPrerequisiteRule(RulesIniRule& rule)
	{
		Optional& structValueStrOptional = rulesIni.ReadOptionalStringRule(rule);

		if (!structValueStrOptional.Present())
		{
			delete &structValueStrOptional;

			return rule.GetDefaultValueOr(STRUCTF_NONE);
		}

		auto structValueStr = structValueStrOptional.Get<char*>();

		ConvertStringToUpperCase(structValueStr);

		auto& structResult = typeConverter.Parse<StructType>(structValueStr);

		delete &structValueStrOptional;

		if (structResult.IsErrorResult())
		{
			// unable to parse entry as a structure type
			rulesIni.MarkAsInvalid();

			ShowError("Failed to parse prerequisite '%s' for [%s]: %s", structValueStr, rule.GetStringKey(), structResult.GetError());

			return STRUCTF_NONE;
		}

		auto structValue = structResult.GetValue();

		delete &structResult;

		if (structValue == STRUCT_NONE)
		{
			return STRUCTF_NONE;
		}

		return 1L << structValue;
	}

	int ReadHouseListRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<int>(
			rule,
			"houses",
			[&](const char* typeString) {
				return &typeConverter.ParseHouseNameListCsv(typeString);
			},
			(int)HOUSEF_NONE
		);
	}

	WeaponType ReadWeaponRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<WeaponType>(
			rule,
			"weapon",
			[&](const char* typeString) {
				return &typeConverter.Parse<WeaponType>(typeString);
			},
			WEAPON_NONE
		);
	}

	ArmorType ReadArmorRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<ArmorType>(
			rule,
			"armor",
			[&](const char* typeString) {
				return &typeConverter.Parse<ArmorType>(typeString);
			},
			ARMOR_NONE
		);
	}

	SpeedType ReadUnitSpeedRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<SpeedType>(
			rule,
			"unit speed",
			[&](const char* typeString) {
				return &typeConverter.Parse<SpeedType>(typeString);
			},
			SPEED_NONE
		);
	}

	FactoryType ReadFactoryRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<FactoryType>(
			rule,
			"factory type",
			[&](const char* typeString) {
				return &typeConverter.Parse<FactoryType>(typeString);
			},
			FACTORY_TYPE_NONE
		);
	}

	WarheadType ReadWarheadRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<WarheadType>(
			rule,
			"warhead",
			[&](const char* typeString) {
				return &typeConverter.Parse<WarheadType>(typeString);
			},
			WARHEAD_NONE
		);
	}

	BulletType ReadBulletRule(RulesIniRule& rule)
	{
		return GetParsedStringRule<BulletType>(
			rule,
			"bullet",
			[&](const char* typeString) {
				return &typeConverter.Parse<BulletType>(typeString);
			},
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
	static TiberianDawnRulesReader& Build(IRulesIni& rules, TiberianDawnTypeConverter& typeConverter)
	{
		return *(new TiberianDawnRulesReader(rules, typeConverter));
	}

};
