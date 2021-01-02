#pragma once

#include <map>
#include <vector>

#include "HashUtils.h"
#include "ResultWithValue.h"
#include "strings.h"
#include "TypeUtils.h"
#include "utils.h"

template <class T> class TwoWayStringMap
{
private:
	const char* typeName;
	std::map<StringHash, T>& keyToValueMap;
	std::map<T, const char*>& valueToKeyMap;

	TwoWayStringMap(const char* typeName, std::map<T, const char*> typeMap) :
		typeName(typeName),
		keyToValueMap(*(new std::map<StringHash, T>())),
		valueToKeyMap(*(new std::map<T, const char*>()))
	{
		for (auto& [value, key] : typeMap)
		{
			AddKey(key, value);
		}
	}

public:
	static TwoWayStringMap& Build(const char* typeName, std::map<T, const char*> keyValueMap = {})
	{
		return *(new TwoWayStringMap(typeName, keyValueMap));
	}

	~TwoWayStringMap()
	{
		delete &keyToValueMap;
		delete &valueToKeyMap;
	}

	const char* GetTypeName()
	{
		return typeName;
	}

	void AddKey(const char* keyString, T value)
	{
		auto keyHash = HashUtils::HashString(keyString);

		keyToValueMap[keyHash] = value;
		valueToKeyMap[value] = keyString;
	}

	bool HasKey(const char* keyString)
	{
		auto key = HashUtils::HashString(keyString);

		return keyToValueMap.find(key) != keyToValueMap.end();
	}

	bool HasValue(T value)
	{
		return valueToKeyMap.find(value) != valueToKeyMap.end();
	}

	const char* operator [](T value)
	{
		return valueToKeyMap[value];
	}

	T operator [](const char* keyString)
	{
		auto key = HashUtils::HashString(keyString);

		return keyToValueMap[key];
	}

	ResultWithValue<T>& GetValueForKey(const char* stringValue)
	{
		if (StringIsEmpty(stringValue))
		{
			return ResultWithValue<T>::BuildWithError("%s type string was null or blank", GetTypeName());
		}

		if (!HasKey(stringValue))
		{
			return ResultWithValue<T>::BuildWithError("String not recognised as a %s type: %s", GetTypeName(), stringValue);
		}

		return ResultWithValue<T>::BuildWithValue(
			(*this)[stringValue]
		);
	}

	ResultWithValue<const char*>& GetKeyForValue(T value)
	{
		if (!HasValue(value))
		{
			return ResultWithValue<const char*>::BuildWithError("Value is not support for to string operation, type: %s", GetTypeName());
		}

		return ResultWithValue<const char*>::BuildWithValue(
			(*this)[value]
		);
	}

};
