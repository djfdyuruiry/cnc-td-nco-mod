#pragma once

#include <functional>

#include <lua.hpp>

#include "IRulesIniSection.h"
#include "parse.h"
#include "rules_ini_building.h"
#include "TechnoTypeApi.h"
#include "type.h"

#define EXTRACTOR_BLD(f) EXTRACTOR(BuildingTypeClass, f)
#define INJECTOR_BLD(t, f) INJECTOR(BuildingTypeClass, t, f)
#define SIMPLE_EXTRACTOR_BLD(f) SIMPLE_EXTRACTOR(BuildingTypeClass, f)
#define SIMPLE_INJECTOR_BLD(t, f) SIMPLE_INJECTOR(BuildingTypeClass, t, f)

class BuildingApi : public TechnoTypeApi<BuildingTypeClass, StructType>
{
private:
	BuildingApi(IRulesIniSection& rulesInfo) : TechnoTypeApi("Building", rulesInfo, Parse_Structure_Type)
	{
		technoTypeWrapper.WithFieldWrapper(
			BIBBED_RULE,
			EXTRACTOR_BLD((bool)i.IsBibbed),
			SIMPLE_INJECTOR_BLD(bool, IsBibbed),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			WALL_RULE,
			EXTRACTOR_BLD((bool)i.IsWall),
			SIMPLE_INJECTOR_BLD(bool, IsWall),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			FACTORY_RULE,
			EXTRACTOR_BLD((bool)i.IsFactory),
			SIMPLE_INJECTOR_BLD(bool, IsFactory),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			SIMPLE_DAMAGE_RULE,
			EXTRACTOR_BLD((bool)i.IsSimpleDamage),
			SIMPLE_INJECTOR_BLD(bool, IsSimpleDamage),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			STURDY_RULE,
			EXTRACTOR_BLD((bool)i.IsSturdy),
			SIMPLE_INJECTOR_BLD(bool, IsSturdy),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAPTUREABLE_RULE,
			EXTRACTOR_BLD((bool)i.IsCaptureable),
			SIMPLE_INJECTOR_BLD(bool, IsCaptureable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			FIXED_SPEED_ANIMATION_RULE,
			EXTRACTOR_BLD((bool)i.IsRegulated),
			SIMPLE_INJECTOR_BLD(bool, IsRegulated),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			UNSELLABLE_RULE,
			EXTRACTOR_BLD((bool)i.IsUnsellable),
			SIMPLE_INJECTOR_BLD(bool, IsUnsellable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			UNSELLABLE_RULE,
			EXTRACTOR_BLD((bool)i.IsUnsellable),
			SIMPLE_INJECTOR_BLD(bool, IsUnsellable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			FACTORY_TYPE_RULE,
			EXTRACTOR_BLD(Factory_Type_To_String((FactoryType)i.ToBuild)),
			[](BuildingTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

				i.ToBuild = (RTTIType)Parse_Factory_Type(valueUpper, NULL);

				delete valueUpper;
			},
			ParseCheckValidator<FactoryType>::Build(Parse_Factory_Type)
		).WithFieldWrapper(
			STORAGE_CAPACITY_RULE,
			SIMPLE_EXTRACTOR_BLD(Capacity),
			SIMPLE_INJECTOR_BLD(unsigned int, Capacity),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		).WithFieldWrapper(
			POWER_OUTPUT_RULE,
			SIMPLE_EXTRACTOR_BLD(Power),
			SIMPLE_INJECTOR_BLD(unsigned int, Power),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		).WithFieldWrapper(
			POWER_INPUT_RULE,
			SIMPLE_EXTRACTOR_BLD(Drain),
			SIMPLE_INJECTOR_BLD(unsigned int, Drain),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo)
	{
		return *(new BuildingApi(rulesInfo));
	}

};
