#include <string>

#include "rules_cache_key.h"
#include <strings.h>

static const CacheKey HASH_SEED = 5381ul;

CacheKey Build_Rule_Key(const char* section, const char* ruleName)
{
	auto keyString = Allocate_String(MAX_KEY_LENGTH);
	auto upperSection = Convert_String_To_Upper_Case(section);
	auto upperRuleName = Convert_String_To_Upper_Case(ruleName);

	sprintf(keyString, "%s__%s", upperSection, upperRuleName);

	auto hash = HASH_SEED;
	auto stringChar = 0;

	auto iterator = keyString;

	while (stringChar = *iterator++)
	{
		hash = ((hash << 5) + hash) + stringChar;
	}

	delete keyString;
	delete upperSection;
	delete upperRuleName;

	return hash;
}

CacheKey Build_Rule_Key(const char* section)
{
	return Build_Rule_Key(section, "SECTION__KEY");
}
