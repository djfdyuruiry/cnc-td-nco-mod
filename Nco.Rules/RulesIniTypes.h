#pragma once

typedef const char* SectionName;
typedef const char* RuleName;
typedef void* RuleValue;

#define RULE_TYPE_EXTENSION(idx) (RulesIniType)((char) idx + 1 + FIXED_RULE)

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
