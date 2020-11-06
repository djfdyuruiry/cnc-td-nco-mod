#pragma once

#include <functional>
#include <map>
#include <vector>

#include <ILuaStateWrapper.h>
#include <ILuaValueValidator.h>
#include <LuaValueAdapter.h>

#include "RULES_CACHE_KEY.H"

template<class T> class LuaTypeWrapper
{
private:
	std::map<CacheKey, std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&)>>& extractors;
	std::map<CacheKey, std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&, int stackIndex)>>& injectors;
	std::map<CacheKey, ILuaValueValidator*>& validators;

	LuaTypeWrapper() :
		extractors(*(new std::map<CacheKey, std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&)>>())),
		injectors(*(new std::map<CacheKey, std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&, int stackIndex)>>)),
		validators(*(new std::map<CacheKey, ILuaValueValidator*>))
	{
	}

public:
	static LuaTypeWrapper& Build()
	{
		return *(new LuaTypeWrapper<T>());
	}

	~LuaTypeWrapper()
	{
		delete &extractors;
		delete &injectors;

		for (auto [_, validator] : validators) {
			delete validator;
		}

		delete& validators;
	}

	LuaTypeWrapper& WithFieldWrapper(
		const char* fieldName,
		std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&)> extractor,
		std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&, int stackIndex)> injector,
		ILuaValueValidator& validator
	)
	{
		auto fieldKey = Build_Rule_Key(fieldName);

		extractors[fieldKey] = extractor;
		injectors[fieldKey] = injector;
		validators[fieldKey] = &validator;

		return *this;
	}

	LuaTypeWrapper& WithFieldsWrapper(
		std::vector<const char*> fieldNames,
		std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&)> extractor,
		std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&, int stackIndex)> injector,
		ILuaValueValidator& validator
	)
	{
		for (auto fieldName : fieldNames)
		{
			WithFieldWrapper(fieldName, extractor, injector, validator);
		}

		return *this;
	}

	LuaValueAdapter& BuildValueAdapater(CacheKey fieldKey)
	{
		return LuaValueAdapter::Build(*validators[fieldKey]);
	}

	void ReadFieldValue(T& typeInstance, CacheKey fieldKey, ILuaStateWrapper& lua)
	{
		auto& adapter = BuildValueAdapater(fieldKey);

		extractors[fieldKey](typeInstance, lua, adapter);

		delete &adapter;
	}

	bool WriteFieldValue(T& typeInstance, CacheKey fieldKey, ILuaStateWrapper& lua, int stackIndex)
	{
		if (!validators[fieldKey]->IsValid(lua, stackIndex))
		{
			// TODO: change to return lua result with value and store validation error in it
			return false;
		}

		auto& adapter = BuildValueAdapater(fieldKey);

		injectors[fieldKey](typeInstance, lua, adapter, stackIndex);
		
		delete &adapter;

		return true;
	}
};
