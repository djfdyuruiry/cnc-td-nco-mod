#pragma once

#include <RulesIniRule.h>
#include <RulesIniTypes.h>

const RulesIniType HOUSE_LIST_RULE = RULE_TYPE_EXTENSION(0);
const RulesIniType WEAPON_RULE = RULE_TYPE_EXTENSION(1);
const RulesIniType ARMOR_TYPE_RULE = RULE_TYPE_EXTENSION(2);
const RulesIniType UNIT_SPEED_TYPE_RULE = RULE_TYPE_EXTENSION(3);
const RulesIniType FACTORY_RULE_TYPE = RULE_TYPE_EXTENSION(4);
const RulesIniType WARHEAD_RULE = RULE_TYPE_EXTENSION(5);
const RulesIniType BULLET_RULE = RULE_TYPE_EXTENSION(6);
const RulesIniType PREREQ_RULE = RULE_TYPE_EXTENSION(7);

void RegisterTiberianDawnRuleTypes();
