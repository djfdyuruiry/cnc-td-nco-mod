#ifndef RULES_CACHE_KEY_H
#define RULES_CACHE_KEY_H

const unsigned int MAX_KEY_LENGTH = 1024;

typedef unsigned long CacheKey;

CacheKey Build_Rule_Key(const char* section, const char* ruleName);

extern const CacheKey RALLY_POINTS_RULE_CACHE_KEY;

#endif
