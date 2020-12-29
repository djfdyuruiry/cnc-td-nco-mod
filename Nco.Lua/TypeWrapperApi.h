#pragma once

#include <functional>
#include <vector>


#include "LuaObjectUtils.h"
#include "LuaTypeWrapper.h"
#include <Result.h>
#include "TypeApi.h"

#define EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define BOOL_EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, (bool)i.f); }
#define INJECTOR(it, t, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(it, f) EXTRACTOR(it, i.f)
#define SIMPLE_INJECTOR(it, t, f) INJECTOR(it, t, i.f) 

template<class T, class U> class TypeWrapperApi : public TypeApi<T>
{
private:
	U first;
	std::function<int(void)> getCount;
	std::function<ResultWithValue<const char*>* (U)> typeToString;

	static int GetTypesProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(TypeWrapperApi, GetTypes);
	}

	int GetTypes(ILuaStateWrapper& lua)
	{
		auto& vector = std::vector<const char*>();
		auto count = getCount();

		for (auto i = first; i < count; i++)
		{
			auto& nameResult = *typeToString(i);

			vector.push_back(nameResult.GetValue());

			delete &nameResult;
		}

		lua.WriteArray(vector);

		return 1;
	}

protected:
	std::function<ResultWithValue<U>* (const char*)> typeParser;
	LuaTypeWrapper<T>& technoTypeWrapper;

	TypeWrapperApi(
		char* typeName,
		U first,
		std::function<int(void)> getCount,
		std::function<ResultWithValue<U>* (const char*)> typeParser,
		std::function<ResultWithValue<const char*>* (U)> typeToString
	) :
		TypeApi(typeName),
		first(first),
		getCount(getCount),
		typeParser(typeParser),
		typeToString(typeToString),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
		WithMethod("getTypes", this, GetTypesProxy)
			.WithDescription(FormatString("Get a list of all %s types (including mod types)", titleCaseTypeName));
	}

	bool ValidateTypeName(const char* name)
	{
		auto& parseResult = *typeParser(name);

		auto isValid = !parseResult.IsErrorResult();

		delete &parseResult;

		return isValid;
	}

	T& ParseType(const char* name)
	{
		auto& parseResult = *typeParser(name);

		auto type = parseResult.GetValue();

		delete &parseResult;

		return (T&)T::As_Reference(type);
	}

	Result& ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName)
	{
		if (!ValidateRule(ruleName))
		{
			return Result::BuildWithError("Rule not recognised: %s", ruleName);
		}

		return technoTypeWrapper.ReadFieldValue(typeInstance, ruleName, lua);
	}

	Result& WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex)
	{
		if (!ValidateRule(ruleName))
		{
			return Result::BuildWithError("Rule not recognised: %s", ruleName);
		}

		return technoTypeWrapper.WriteFieldValue(typeInstance, ruleName, lua, ruleValueStackIndex);
	}

public:
	~TypeWrapperApi()
	{
		delete &technoTypeWrapper;
	}

};
