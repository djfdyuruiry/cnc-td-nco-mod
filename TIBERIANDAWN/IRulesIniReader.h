#pragma once

#include "parse.h"
#include "RulesIniRule.h"
#include "RulesIniRuleKey.h"
#include "RulesIniTypes.h"

class IRulesIniReader
{
protected:
	IRulesIni& rulesIni;

	IRulesIniReader(IRulesIni& rulesIniToRead) : rulesIni(rulesIniToRead)
	{
	}

	virtual bool ReadBoolRule(RulesIniRule& rule) = 0;
	virtual unsigned int ReadUnsignedIntRule(RulesIniRule& rule) = 0;
	virtual double ReadDoubleRule(RulesIniRule& rule, bool* valueFound = NULL) = 0;
	virtual unsigned int ReadFixedRule(RulesIniRule& rule) = 0;
	virtual long ReadPrerequisiteRule(RulesIniRule& rule) = 0;
	virtual int ReadHouseListRule(RulesIniRule& rule) = 0;
	virtual WeaponType ReadWeaponRule(RulesIniRule& rule) = 0;
	virtual ArmorType ReadArmorRule(RulesIniRule& rule) = 0;
	virtual SpeedType ReadUnitSpeedRule(RulesIniRule& rule) = 0;
	virtual FactoryType ReadFactoryRule(RulesIniRule& rule) = 0;
	virtual WarheadType ReadWarheadRule(RulesIniRule & rule) = 0;
	virtual BulletType ReadBulletRule(RulesIniRule& rule) = 0;

public:
	virtual ~IRulesIniReader()
	{
	}

	virtual RulesIniRule& GetRule(const RulesIniRuleKey& key) = 0;

	virtual RulesIniRule& GetRule(SectionName section, RuleName ruleName) = 0;

	template<class T> T ReadRuleValue(RulesIniRule& rule)
	{
		if (rule.HasValue())
		{
			return rule.GetValue<T>();
		}

		auto ruleType = rule.GetType();


		if (ruleType == STRING_RULE)
		{
			rule.SetValue(
				rulesIni.ReadStringRule(rule)
			);
		}
		else if (ruleType == INT_RULE)
		{
			rule.SetValue(
				rulesIni.ReadIntRule(rule)
			);
		}
		else if (ruleType == BOOL_RULE)
		{
			rule.SetValue(
				ReadBoolRule(rule)
			);
		}
		else if (ruleType == UNSIGNED_INT_RULE)
		{
			rule.SetValue(
				ReadUnsignedIntRule(rule)
			);
		}
		else if (ruleType == DOUBLE_RULE)
		{
			rule.SetValue(
				ReadDoubleRule(rule)
			);
		}
		else if (ruleType == FIXED_RULE)
		{
			rule.SetValue(
				ReadFixedRule(rule)
			);
		}
		else if (ruleType == HOUSE_LIST_RULE)
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
			rule.SetValue(
				rule.GetDefaultValue<T>()
			);
		}

		return rule.GetValue<T>();
	}

	template<class T> T ReadRuleValue(const RulesIniRuleKey& key)
	{
		return ReadRuleValue<T>(
			GetRule(key)
		);
	}

	template<class T> T ReadRuleValue(SectionName section, RuleName ruleName)
	{
		const RulesIniRuleKey& key = RulesIniRuleKey::BuildRuleKey(section, ruleName);

		auto ruleValue = ReadRuleValue<T>(key);

		delete &key;

		return ruleValue;
	}

	template<class T> T ReadRuleValue(SectionName section, RuleName ruleName, T defaultValue)
	{
		const RulesIniRuleKey& key = RulesIniRuleKey::BuildRuleKey(section, ruleName);

		RulesIniRule& rule = GetRule(key);

		delete &key;

		if (!rule.HasValue())
		{
			rule.SetDefaultValue<T>(defaultValue);
		}

		return ReadRuleValue<T>(rule);
	}
};
