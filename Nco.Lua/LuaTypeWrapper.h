#pragma once

#include <functional>
#include <map>
#include <vector>

#include <Logger.h>
#include <HashUtils.h>

#include "ILuaStateWrapper.h"
#include "ILuaValueValidator.h"
#include "LuaResult.h"
#include "LuaValueAdapter.h"

#define LuaFieldExtractor std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&)>
#define LuaFieldInjector std::function<void(T&, ILuaStateWrapper&, LuaValueAdapter&, int stackIndex)>

template<class T> class LuaTypeWrapper
{
private:
	std::map<StringHash, LuaFieldExtractor>& extractors;
	std::map<StringHash, LuaFieldInjector>& injectors;
	std::map<StringHash, ILuaValueValidator*>& validators;

	LuaTypeWrapper() :
		extractors(*(new std::map<StringHash, LuaFieldExtractor>())),
		injectors(*(new std::map<StringHash, LuaFieldInjector>)),
		validators(*(new std::map<StringHash, ILuaValueValidator*>))
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

		delete &validators;
	}

	LuaTypeWrapper& WithFieldWrapper(
		const char* fieldName,
		LuaFieldExtractor extractor,
		LuaFieldInjector injector,
		ILuaValueValidator& validator
	)
	{
		auto fieldKey = HashUtils::HashString(fieldName);

		extractors[fieldKey] = extractor;
		injectors[fieldKey] = injector;
		validators[fieldKey] = &validator;

		return *this;
	}

	LuaTypeWrapper& WithFieldsWrapper(
		std::vector<const char*> fieldNames,
		LuaFieldExtractor extractor,
		LuaFieldInjector injector,
		ILuaValueValidator& validator
	)
	{
		for (auto fieldName : fieldNames)
		{
			WithFieldWrapper(fieldName, extractor, injector, validator);
		}

		return *this;
	}

	LuaValueAdapter& BuildValueAdapater(StringHash fieldKey)
	{
		return LuaValueAdapter::Build(*validators[fieldKey]);
	}

	LuaResult& ReadFieldValue(T& typeInstance, const char* fieldName, ILuaStateWrapper& lua)
	{
		auto fieldKey = HashUtils::HashString(fieldName);

		if (extractors.find(fieldKey) == extractors.end())
		{
			return LuaResult::BuildWithError("Reading field '%s' is currently not supported - no object field extractor configured", fieldName);
		}

		auto& adapter = BuildValueAdapater(fieldKey);

		extractors[fieldKey](typeInstance, lua, adapter);

		delete &adapter;

		return LuaResult::Build();
	}

	LuaResult& WriteFieldValue(T& typeInstance, const char* fieldName, ILuaStateWrapper& lua, int stackIndex)
	{
		auto fieldKey = HashUtils::HashString(fieldName);

		if (injectors.find(fieldKey) == injectors.end())
		{
			return LuaResult::BuildWithError("Writing field is currently not supported - no object field injector configured", fieldName);
		}

		if (validators.find(fieldKey) != validators.end())
		{
			auto& validationResult = validators[fieldKey]->IsValid(lua, stackIndex);

			if (validationResult.IsErrorResult())
			{
				auto& writeResult = LuaResult::BuildWithError("Invalid value provided for field '%s': %s", fieldName, validationResult.GetError());

				delete &validationResult;

				return writeResult;
			}
		}
		else
		{
			LogWarn("No validator configured for field '%s' - value will attempt to be set without validation", fieldName);
		}

		auto& adapter = BuildValueAdapater(fieldKey);

		injectors[fieldKey](typeInstance, lua, adapter, stackIndex);
		
		delete &adapter;

		return LuaResult::Build();
	}

};
