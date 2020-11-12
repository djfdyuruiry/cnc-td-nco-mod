#pragma once

#include <functional>

#include "IRulesIniSection.h"
#include "parse.h"
#include "rules_ini_infantry.h"
#include "TechnoTypeApi.h"
#include "type.h"

#define EXTRACTOR_INF(f) EXTRACTOR(InfantryTypeClass, f)
#define INJECTOR_INF(t, f) INJECTOR(InfantryTypeClass, t, f)
#define SIMPLE_EXTRACTOR_INF(f) SIMPLE_EXTRACTOR(InfantryTypeClass, f)
#define SIMPLE_INJECTOR_INF(t, f) SIMPLE_INJECTOR(InfantryTypeClass, t, f)

class InfantryApi : public TechnoTypeApi<InfantryTypeClass, InfantryType>
{
private:
	InfantryApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		TechnoTypeApi("Infantry", rulesInfo, INFANTRY_FIRST, getCount, Parse_Infantry_Type, Infantry_Type_To_String)
	{
		technoTypeWrapper.WithFieldWrapper(
			FEMALE_RULE,
			EXTRACTOR_INF((bool)i.IsFemale),
			SIMPLE_INJECTOR_INF(bool, IsFemale),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CRAWLING_RULE,
			EXTRACTOR_INF((bool)i.IsCrawling),
			SIMPLE_INJECTOR_INF(bool, IsCrawling),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_CAPTURE_RULE,
			EXTRACTOR_INF((bool)i.IsCapture),
			SIMPLE_INJECTOR_INF(bool, IsCapture),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			FRAIDY_CAT_RULE,
			EXTRACTOR_INF((bool)i.IsFraidyCat),
			SIMPLE_INJECTOR_INF(bool, IsFraidyCat),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CIVILIAN_RULE,
			EXTRACTOR_INF((bool)i.IsCivilian),
			SIMPLE_INJECTOR_INF(bool, IsCivilian),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			AVOIDS_TIBERIUM_RULE,
			EXTRACTOR_INF((bool)i.IsAvoidingTiberium),
			SIMPLE_INJECTOR_INF(bool, IsAvoidingTiberium),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			IMMUNE_TO_TIBERIUM_RULE,
			EXTRACTOR_INF((bool)i.IsImmuneToTiberium),
			SIMPLE_INJECTOR_INF(bool, IsImmuneToTiberium),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_C4_CHARGES_RULE,
			EXTRACTOR_INF((bool)i.HasC4Charges),
			SIMPLE_INJECTOR_INF(bool, HasC4Charges),
			PrimitiveTypeValidator<bool>::Build()
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo, std::function<int(void)> getCount)
	{
		return *(new InfantryApi(rulesInfo, getCount));
	}

};
