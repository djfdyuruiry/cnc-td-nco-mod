#pragma once

#include <TypeApi.h>

#include "LuaTypeWrapper.h"

#define EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(it, t, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(it, f) EXTRACTOR(it, i.f)
#define SIMPLE_INJECTOR(it, t, f) INJECTOR(it, t, i.f) 

template<class T, class U> class TypeWrapperApi : public TypeApi<T>
{
protected:
	IRulesIniSection& rulesInfo;
	U(*typeParser)(const char*, bool*, bool);
	LuaTypeWrapper<T>& technoTypeWrapper;

	TypeWrapperApi(const char* typeName, IRulesIniSection& rulesInfo, U(*typeParser)(const char*, bool*, bool)) : 
		TypeApi(typeName),
		rulesInfo(rulesInfo),
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

	bool ValidateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

	bool ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName)
	{
		if (!ValidateRule(ruleName))
		{
			return false;
		}

		technoTypeWrapper.ReadFieldValue(typeInstance, Build_Rule_Key(ruleName), lua);

		return true;
	}

	bool WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex)
	{
		if (!ValidateRule(ruleName))
		{
			return false;
		}

		return technoTypeWrapper.WriteFieldValue(typeInstance, Build_Rule_Key(ruleName), lua, ruleValueStackIndex);
	}

public:
	~TypeWrapperApi()
	{
		delete &technoTypeWrapper;
	}

};
