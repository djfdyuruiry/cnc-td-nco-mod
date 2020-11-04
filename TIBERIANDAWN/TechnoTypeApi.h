#pragma once

#include <map>

#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <TypeApi.h>

#include "IRulesIniSection.h"
#include "LuaTypeWrapper.h"
#include "parse.h"
#include "rules_cache_key.h"
#include "rules_ini_generic.h"
#include "StructTypeValidator.h"
#include "type.h"

#define EXTRACTOR(f) [](T& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(t, f) [](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(f) EXTRACTOR(i.f)
#define SIMPLE_INJECTOR(t, f) INJECTOR(t, i.f) 

template<class T> class TechnoTypeApi : public TypeApi<T>
{
protected:
	IRulesIniSection& rulesInfo;
	LuaTypeWrapper<T>& technoTypeWrapper;

	TechnoTypeApi(const char* typeName, IRulesIniSection& rulesInfo) :
		TypeApi(typeName),
		rulesInfo(rulesInfo),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
		technoTypeWrapper.WithFieldWrapper(
				BUILD_LEVEL_RULE,
				SIMPLE_EXTRACTOR(Level),
				SIMPLE_INJECTOR(unsigned char, Level),
				NumbericRangeValidator<unsigned char>::Build(0, 99)
			).WithFieldWrapper(
				SCENARIO_LEVEL_RULE,
				SIMPLE_EXTRACTOR(Scenario),
				SIMPLE_INJECTOR(unsigned char, Scenario),
				NumbericRangeValidator<unsigned char>::Build(0, 99)
			).WithFieldWrapper(
				PREREQUISITE_RULE,
				EXTRACTOR(Prerequisite_To_String(i.Pre)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Pre = Structure_Type_To_Prerequisite(
						Parse_Structure_Type(va.Read<const char*>(l, si), NULL),
						NULL
					);
				},
				StructTypeValidator::Build()
			).WithFieldWrapper(
				COST_RULE,
				SIMPLE_EXTRACTOR(Cost),
				SIMPLE_INJECTOR(int, Cost),
				PrimitiveTypeValidator<int>::Build()
			);

			// TODO: migrate complete set of rules over
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
};
