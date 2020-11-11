#pragma once

#include <RulesIniTypes.h>

enum TiberianDawnRuleType : char
{
	HOUSE_LIST_RULE = FIXED_RULE + 1,
	WEAPON_RULE,
	ARMOR_TYPE_RULE,
	UNIT_SPEED_TYPE_RULE,
	FACTORY_RULE_TYPE,
	WARHEAD_RULE,
	BULLET_RULE,
	PREREQ_RULE
};
