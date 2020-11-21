#pragma once

#include <string>

#include <HashUtils.h>
#include <strings.h>

class RuleHashUtils final
{
private:
	RuleHashUtils()
	{
	}

public:
	static const unsigned int MAX_KEY_LENGTH = 1024ul;

	static StringHash BuildRuleKey(const char* section, const char* ruleName)
	{
		auto keyString = FormatString("%s__%s", MAX_KEY_LENGTH, section, ruleName);

		auto hash = HashUtils::HashString(keyString);

		delete keyString;

		return hash;
	}

	static StringHash BuildRuleKey(const char* section)
	{
		return BuildRuleKey(section, "SECTION__KEY");
	}
};
