#pragma once

#include <HashUtils.h>

const unsigned int MAX_KEY_LENGTH = 1024ul;

typedef StringHash CacheKey;

CacheKey Build_Rule_Key(const char* section, const char* ruleName);

CacheKey Build_Rule_Key(const char* section);
