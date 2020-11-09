#pragma once

#include "LuaTypeWrapper.h"
#include "LuaResult.h"
#include "TypeApi.h"

#define EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(it, t, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(it, f) EXTRACTOR(it, i.f)
#define SIMPLE_INJECTOR(it, t, f) INJECTOR(it, t, i.f) 

template<class T, class U> class TypeWrapperApi : public TypeApi<T>
{
protected:
	U(*typeParser)(const char*, bool*, bool);
	LuaTypeWrapper<T>& technoTypeWrapper;

	TypeWrapperApi(const char* typeName, U(*typeParser)(const char*, bool*, bool)) : 
		TypeApi(typeName),
		typeParser(typeParser),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
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
