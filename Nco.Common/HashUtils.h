#pragma once

#include "strings.h"

typedef unsigned long StringHash;

class HashUtils final
{
private:
	static const auto HASH_SEED = 5381ul;

	HashUtils()
	{
	}

public:
	static StringHash HashString(const char* string)
	{
		auto stringUpper = Convert_String_To_Upper_Case(string);
		auto stringChar = 0;
		auto hash = HASH_SEED;

		auto iterator = stringUpper;

		while (stringChar = *iterator++)
		{
			hash = ((hash << 5) + hash) + stringChar;
		}

		delete stringUpper;

		return hash;
	}

	static bool HashesMatch(StringHash left, StringHash right)
	{
		return left == right;
	}

	static bool HashesMatch(const char* left, StringHash right)
	{
		return HashString(left) == right;
	}

	static bool HashesMatch(StringHash left, const char* right)
	{
		return left == HashString(right);
	}

	static bool HashesMatch(const char* left, const char* right)
	{
		return HashString(left) == HashString(right);
	}
};
