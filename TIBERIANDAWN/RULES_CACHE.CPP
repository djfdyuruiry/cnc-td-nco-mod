#include <map>

#include "logger.h"
#include "rules_cache_key.h"
#include "strings.h"
#include "utils.h"

using namespace std;

static map<CacheKey, char*>* STRING_RULES_CACHE;
static map<CacheKey, int>* INT_RULES_CACHE;
static map<CacheKey, unsigned int>* UINT_RULES_CACHE;
static map<CacheKey, double>* DOUBLE_RULES_CACHE;
static map<CacheKey, bool>* BOOL_RULES_CACHE;

template<typename T> static T Get_Cached_Rule(
	map<CacheKey, T> cache,
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

template<typename T> static void Cache_Rule(map<CacheKey, T>& cache, const char* section, const char* ruleName, T ruleValue)
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

	Log_Debug("Initalising Rule Cache");

	STRING_RULES_CACHE = new map<CacheKey, char*>();
	INT_RULES_CACHE = new map<CacheKey, int>();
	UINT_RULES_CACHE = new map<CacheKey, unsigned int>();
	DOUBLE_RULES_CACHE = new map<CacheKey, double>();
	BOOL_RULES_CACHE = new map<CacheKey, bool>();
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

char* Get_Cached_String_Rule(CacheKey key, bool* cacheHit)
{
	if (STRING_RULES_CACHE->find(key) == STRING_RULES_CACHE->end())
	{
		if (cacheHit != NULL)
		{
			*cacheHit = false;
		}

		return false;
	}

	if (cacheHit != NULL)
	{
		*cacheHit = true;
	}

	return (*STRING_RULES_CACHE)[key];
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

int Get_Cached_Int_Rule(CacheKey key, bool* cacheHit)
{
	if (INT_RULES_CACHE->find(key) == INT_RULES_CACHE->end())
	{
		if (cacheHit != NULL)
		{
			*cacheHit = false;
		}

		return false;
	}

	if (cacheHit != NULL)
	{
		*cacheHit = true;
	}

	return (*INT_RULES_CACHE)[key];
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

unsigned int Get_Cached_Unsigned_Int_Rule(CacheKey key, bool* cacheHit)
{
	if (UINT_RULES_CACHE->find(key) == UINT_RULES_CACHE->end())
	{
		if (cacheHit != NULL)
		{
			*cacheHit = false;
		}

		return false;
	}

	if (cacheHit != NULL)
	{
		*cacheHit = true;
	}

	return (*UINT_RULES_CACHE)[key];
}

void Cache_Unsigned_Int_Rule(const char* section, const char* ruleName, unsigned int ruleValue)
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

double Get_Cached_Double_Rule(CacheKey key, bool* cacheHit)
{
	if (DOUBLE_RULES_CACHE->find(key) == DOUBLE_RULES_CACHE->end())
	{
		if (cacheHit != NULL)
		{
			*cacheHit = false;
		}

		return false;
	}

	if (cacheHit != NULL)
	{
		*cacheHit = true;
	}

	return (*DOUBLE_RULES_CACHE)[key];
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

bool Get_Cached_Bool_Rule(CacheKey key, bool* cacheHit)
{
	if (BOOL_RULES_CACHE->find(key) == BOOL_RULES_CACHE->end())
	{
		if (cacheHit != NULL)
		{
			*cacheHit = false;
		}

		return false;
	}

	if (cacheHit != NULL)
	{
		*cacheHit = true;
	}

	return (*BOOL_RULES_CACHE)[key];
}

void Cache_Bool_Rule(const char* section, const char* ruleName, bool ruleValue)
{
	Ensure_Rule_Cache_Initialised();

	Cache_Rule(*BOOL_RULES_CACHE, section, ruleName, ruleValue);
}

void Delete_Rules_Cache()
{
	Ensure_Rule_Cache_Initialised();

	Log_Debug("Deleting Rule Cache");

	delete INT_RULES_CACHE;
	delete UINT_RULES_CACHE;
	delete DOUBLE_RULES_CACHE;
	delete BOOL_RULES_CACHE;
	delete STRING_RULES_CACHE;
}
