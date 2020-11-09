#include <string>

#include <HashUtils.h>
#include <strings.h>

#include "rules_cache_key.h"

CacheKey Build_Rule_Key(const char* section, const char* ruleName)
{
	auto keyString = Allocate_String(MAX_KEY_LENGTH);

	sprintf(keyString, "%s__%s", section, ruleName);

	auto hash = HashUtils::HashString(keyString);

	delete keyString;

	return hash;
}

CacheKey Build_Rule_Key(const char* section)
{
	return Build_Rule_Key(section, "SECTION__KEY");
}
