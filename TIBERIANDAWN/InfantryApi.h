#pragma once

#include <functional>

#include <lua.hpp>

#include "IRulesIniSection.h"
#include "parse.h"
#include "TechnoTypeApi.h"
#include "type.h"

class InfantryApi : public TechnoTypeApi<InfantryTypeClass>
{
private:
	InfantryApi(IRulesIniSection& rulesInfo) : TechnoTypeApi("Infantry", rulesInfo)
	{
	}

protected:
	bool ValidateTypeName(const char* name)
	{
		bool parseError = false;
		
		Parse_Infantry_Type(name, &parseError);

		return !parseError;
	}

	InfantryTypeClass& ParseType(const char* name)
	{
		return (InfantryTypeClass&) InfantryTypeClass::As_Reference(
			Parse_Infantry_Type(name, NULL)
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo)
	{
		return *(new InfantryApi(rulesInfo));
	}

};
