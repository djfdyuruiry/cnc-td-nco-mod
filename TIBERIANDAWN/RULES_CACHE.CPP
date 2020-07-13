#include <map>
#include <string>

#include "logger.h"
#include "strings.h"
#include "utils.h"

using namespace std;

static const auto MAX_KEY_LENGTH = 1024;

static map<unsigned long, char*>* STRING_RULES_CACHE;
static map<unsigned long, int>* INT_RULES_CACHE;
static map<unsigned long, unsigned int>* UINT_RULES_CACHE;
static map<unsigned long, double>* DOUBLE_RULES_CACHE;
static map<unsigned long, bool>* BOOL_RULES_CACHE;

unsigned long Build_Rule_Key(const char* section, const char* ruleName)
{
	auto keyString = Allocate_String(MAX_KEY_LENGTH);
	auto upperSection = Convert_String_To_Upper_Case(section);
	auto upperRuleName = Convert_String_To_Upper_Case(ruleName);

	sprintf(keyString, "%s__%s", upperSection, upperRuleName);

	auto hash = 5381ul;
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

template<typename T> static T Get_Cached_Rule(
	map<unsigned long, T> cache,
	const char* section,
	const char* ruleName,
	T defaultIfMissing,
	bool* cacheHit
)
{
	auto key = Build_Rule_Key(section, ruleName);

	if (cache.find(key) == cache.end())
	{
		Log_Trace("Cache miss for key %d", key);

		*cacheHit = false;

		return defaultIfMissing;
	}

	Log_Trace("Cache hit for key %d", key);

	*cacheHit = true;

	auto value = cache[key];

	Log_Trace("Resolved rule value from cache: %s -> %s", section, ruleName);

	return value;
}

template<typename T> static void Cache_Rule(map<unsigned long, T>& cache, const char* section, const char* ruleName, T ruleValue)
{
	auto key = Build_Rule_Key(section, ruleName);

	cache[key] = ruleValue;
}

static void Ensure_Rule_Cache_Initialised()
{
	if (STRING_RULES_CACHE != NULL
		&& INT_RULES_CACHE != NULL
		&& UINT_RULES_CACHE != NULL
		&& DOUBLE_RULES_CACHE != NULL
		&& BOOL_RULES_CACHE != NULL)
	{
		return;
	}

	STRING_RULES_CACHE = new map<unsigned long, char*>();
	INT_RULES_CACHE = new map<unsigned long, int>();
	UINT_RULES_CACHE = new map<unsigned long, unsigned int>();
	DOUBLE_RULES_CACHE = new map<unsigned long, double>();
	BOOL_RULES_CACHE = new map<unsigned long, bool>();
}

char* Get_Cached_String_Rule(const char* section, const char* ruleName, bool* cacheHit)
{
	Ensure_Rule_Cache_Initialised();

	auto value = Get_Cached_Rule(*STRING_RULES_CACHE, section, ruleName, (char*)"", cacheHit);

	if (*cacheHit)
	{
		Log_Trace("Rule value: %s", value);
	}

	return value;
}

void Cache_String_Rule(const char* section, const char* ruleName, char* ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*STRING_RULES_CACHE, section, ruleName, ruleValue);
}

void Cache_String_Rule(const char* section, const char* ruleName, const char* ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*STRING_RULES_CACHE, section, ruleName, strdup(ruleValue));
}

int Get_Cached_Int_Rule(const char* section, const char* ruleName, bool* cacheHit)
{
	Ensure_Rule_Cache_Initialised();

	auto value = Get_Cached_Rule(*INT_RULES_CACHE, section, ruleName, 0, cacheHit);

	if (*cacheHit)
	{
		Log_Trace("Rule value: %d", value);
	}

	return value;
}

void Cache_Int_Rule(const char* section, const char* ruleName, int ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*INT_RULES_CACHE, section, ruleName, ruleValue);
}

unsigned int Get_Cached_Unsigned_Int_Rule(const char* section, const char* ruleName, bool* cacheHit)
{
	Ensure_Rule_Cache_Initialised();

	auto value = Get_Cached_Rule(*UINT_RULES_CACHE, section, ruleName, 0u, cacheHit);

	if (*cacheHit)
	{
		Log_Trace("Rule value: %u", value);
	}

	return value;
}

void Cache_Unisgned_Int_Rule(const char* section, const char* ruleName, unsigned int ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*UINT_RULES_CACHE, section, ruleName, ruleValue);
}

double Get_Cached_Double_Rule(const char* section, const char* ruleName, bool* cacheHit)
{
	Ensure_Rule_Cache_Initialised();

	auto value = Get_Cached_Rule(*DOUBLE_RULES_CACHE, section, ruleName, (double)0.0f, cacheHit);

	if (*cacheHit)
	{
		Log_Trace("Rule value: %f", value);
	}

	return value;
}

void Cache_Double_Rule(const char* section, const char* ruleName, double ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*DOUBLE_RULES_CACHE, section, ruleName, ruleValue);
}

bool Get_Cached_Bool_Rule(const char* section, const char* ruleName, bool* cacheHit)
{
	Ensure_Rule_Cache_Initialised();

	auto value = Get_Cached_Rule(*BOOL_RULES_CACHE, section, ruleName, false, cacheHit);

	if (*cacheHit)
	{
		Log_Trace("Rule value: %s", Convert_Boolean_To_String(value));
	}

	return value;
}

void Cache_Bool_Rule(const char* section, const char* ruleName, bool ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*BOOL_RULES_CACHE, section, ruleName, ruleValue);
}
