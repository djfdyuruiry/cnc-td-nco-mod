#ifndef RULES_CACHE_H
#define RULES_CACHE_H

char* Get_Cached_String_Rule(const char* section, const char* ruleName, bool* cacheHit);

void Cache_String_Rule(const char* section, const char* ruleName, char* ruleValue);

void Cache_String_Rule(const char* section, const char* ruleName, const char* ruleValue);

int Get_Cached_Int_Rule(const char* section, const char* ruleName, bool* cacheHit);

void Cache_Int_Rule(const char* section, const char* ruleName, int ruleValue);

double Get_Cached_Double_Rule(const char* section, const char* ruleName, bool* cacheHit);

void Cache_Double_Rule(const char* section, const char* ruleName, double ruleValue);

bool Get_Cached_Bool_Rule(const char* section, const char* ruleName, bool* cacheHit);

void Cache_Bool_Rule(const char* section, const char* ruleName, bool ruleValue);

#endif
