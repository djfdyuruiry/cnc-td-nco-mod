#include <RulesIniRule.h>

#include "tiberian_dawn_rule_types.h"

template<class T = int> static void RegisterRuleType(RulesIniType type)
{
	RulesIniRule::RegisterExtendedType<T>(type);
}

void RegisterTiberianDawnRuleTypes()
{
	RegisterRuleType<>(HOUSE_LIST_RULE);
	RegisterRuleType<>(WEAPON_RULE);
	RegisterRuleType<>(UNIT_SPEED_TYPE_RULE);
	RegisterRuleType<>(FACTORY_RULE_TYPE);
	RegisterRuleType<>(WARHEAD_RULE);
	RegisterRuleType<>(BULLET_RULE);

	RegisterRuleType<unsigned int>(ARMOR_TYPE_RULE);

	RegisterRuleType<long>(PREREQ_RULE);
}
