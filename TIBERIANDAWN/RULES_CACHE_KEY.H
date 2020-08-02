#pragma once

const unsigned int MAX_KEY_LENGTH = 1024;

typedef unsigned long CacheKey;

CacheKey Build_Rule_Key(const char* section, const char* ruleName);

CacheKey Build_Rule_Key(const char* section);
