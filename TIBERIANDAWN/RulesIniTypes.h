#pragma once

typedef const char* RuleName;
typedef void* RuleValue;

enum RulesIniType
{
	NO_RULE_TYPE,
	BOOL_RULE,
	INT_RULE,
	UNSIGNED_INT_RULE,
	DOUBLE_RULE,
	FIXED_RULE,
	STRING_RULE
};

typedef const char* SectionName;
