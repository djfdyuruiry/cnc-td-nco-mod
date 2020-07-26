#pragma once

#include "rules_cache_key.h"
#include "utils.h"

char* Get_Cached_String_Rule(const char* section, const char* ruleName, bool* cacheHit);

char* Get_Cached_String_Rule(CacheKey key, bool* cacheHit);

void Cache_String_Rule(const char* section, const char* ruleName, char* ruleValue);

void Cache_String_Rule(const char* section, const char* ruleName, const char* ruleValue);

void Cache_String_Rule(CacheKey key, char* ruleValue);

int Get_Cached_Int_Rule(const char* section, const char* ruleName, bool* cacheHit);

int Get_Cached_Int_Rule(CacheKey key, bool* cacheHit);

void Cache_Int_Rule(const char* section, const char* ruleName, int ruleValue);

unsigned int Get_Cached_Unsigned_Int_Rule(const char* section, const char* ruleName, bool* cacheHit);

unsigned int Get_Cached_Unsigned_Int_Rule(CacheKey key, bool* cacheHit);

void Cache_Unsigned_Int_Rule(const char* section, const char* ruleName, unsigned int ruleValue);

void Cache_Unsigned_Int_Rule(CacheKey key, unsigned int ruleValue);

double Get_Cached_Double_Rule(const char* section, const char* ruleName, bool* cacheHit);

double Get_Cached_Double_Rule(CacheKey key, bool* cacheHit);

void Cache_Double_Rule(const char* section, const char* ruleName, double ruleValue);

bool Get_Cached_Bool_Rule(const char* section, const char* ruleName, bool* cacheHit);

bool Get_Cached_Bool_Rule(CacheKey key, bool* cacheHit);

void Cache_Bool_Rule(const char* section, const char* ruleName, bool ruleValue);

void Delete_Rules_Cache();
