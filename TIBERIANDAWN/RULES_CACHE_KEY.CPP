#include <string>

#include "rules_cache_key.h"
#include "rules_ini_enhancements.h"
#include "strings.h"

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

/**
 *  Cache keys calculated constants - only cache entries which require a high 
 *  performance get call should be listed here. (game loop rules etc.)
 **/

const CacheKey RALLY_POINTS_RULE_CACHE_KEY = Build_Rule_Key(ENHANCEMENTS_RULES_SECTION_NAME, RALLY_POINTS_RULE);
