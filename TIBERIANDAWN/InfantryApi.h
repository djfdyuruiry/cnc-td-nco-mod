#pragma once

#include <functional>

#include <lua.hpp>

#include <LuaMethod.h>

#include "IRulesIniSection.h"
#include "parse.h"
#include "TechnoTypeApi.h"
#include "type.h"

int InfantryReadRuleProxy(lua_State* lua);

int InfantryWriteRuleProxy(lua_State* lua);

class InfantryApi : public TechnoTypeApi<InfantryTypeClass>
{
private:
	InfantryApi(IRulesIniSection& rulesInfo) : TechnoTypeApi("Infantry", rulesInfo)
	{
		Init();
	}

protected:
	lua_CFunction GetReadRuleProxy()
	{
		return InfantryReadRuleProxy;
	}

	lua_CFunction GetWriteRuleProxy()
	{
		return InfantryWriteRuleProxy;
	}

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
