#pragma once

extern const char* TRUE_STRING;
extern const char* FALSE_STRING;

typedef const char* RuleName;
typedef void* RuleValue;

enum RulesIniType : char
{
	NO_RULE_TYPE = -1,
	STRING_RULE,
	INT_RULE,
	BOOL_RULE,
	UNSIGNED_INT_RULE,
	DOUBLE_RULE,
	FIXED_RULE,
	HOUSE_LIST_RULE,
	WEAPON_RULE,
	ARMOR_TYPE_RULE,
	UNIT_SPEED_TYPE_RULE,
	FACTORY_RULE_TYPE,
	WARHEAD_RULE,
	BULLET_RULE,
	PREREQ_RULE
};

typedef const char* SectionName;
