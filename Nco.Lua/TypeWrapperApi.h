#pragma once

#include <functional>
#include <vector>

#include "LuaObjectUtils.h"
#include "LuaTypeWrapper.h"
#include "LuaResult.h"
#include "TypeApi.h"

#define EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(it, t, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(it, f) EXTRACTOR(it, i.f)
#define SIMPLE_INJECTOR(it, t, f) INJECTOR(it, t, i.f) 

template<class T, class U> class TypeWrapperApi : public TypeApi<T>
{
private:
	U first;
	std::function<int(void)> getCount;
	const char* (*typeToString)(U, bool);

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
			auto name = typeToString(i, false);

			vector.push_back(name);
		}

		lua.WriteArray(vector);

		return 1;
	}

protected:
	U(*typeParser)(const char*, bool*, bool);
	LuaTypeWrapper<T>& technoTypeWrapper;

	TypeWrapperApi(
		const char* typeName,
		U first,
		std::function<int(void)> getCount,
		U(*typeParser)(const char*, bool*, bool),
		const char* (*typeToString)(U, bool)
	) :
		TypeApi(typeName),
		first(first),
		getCount(getCount),
		typeParser(typeParser),
		typeToString(typeToString),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
		WithMethod(FormatString("get%sTypes", titleCaseTypeName), this, GetTypesProxy)
			.WithDescription(FormatString("Get a list of all %s types (including mod types)", titleCaseTypeName));
	}

	bool ValidateTypeName(const char* name)
	{
		bool parseError = false;

		typeParser(name, &parseError, false);

		return !parseError;
	}

	T& ParseType(const char* name)
	{
		return (T&)T::As_Reference(
			typeParser(name, NULL, false)
		);
	}

	LuaResult& ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName)
	{
		if (!ValidateRule(ruleName))
		{
			return LuaResult::Build(FormatString("Rule not recognised: %s", ruleName));
		}

		return technoTypeWrapper.ReadFieldValue(typeInstance, ruleName, lua);
	}

	LuaResult& WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex)
	{
		if (!ValidateRule(ruleName))
		{
			return LuaResult::Build(FormatString("Rule not recognised: %s", ruleName));
		}

		return technoTypeWrapper.WriteFieldValue(typeInstance, ruleName, lua, ruleValueStackIndex);
	}

public:
	~TypeWrapperApi()
	{
		delete &technoTypeWrapper;
	}

};
