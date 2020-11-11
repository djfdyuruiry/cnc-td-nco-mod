#pragma once

extern const char* TRUE_STRING;
extern const char* FALSE_STRING;

typedef const char* SectionName;
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
	FIXED_RULE
};
