#pragma once

#include <functional>

#include <lua.hpp>

#include <LuaLambda.h>

#include "IRulesIniSection.h"
#include "parse.h"
#include "TechnoTypeApi.h"
#include "type.h"

class InfantryApi : public TechnoTypeApi<InfantryTypeClass>
{
private:
	const LuaLambda& ReadRuleLambda;
	const LuaLambda& WriteRuleLambda;

	InfantryApi(IRulesIniSection& rulesInfo) :
		TechnoTypeApi("Infantry", rulesInfo),
		ReadRuleLambda(std::bind(&InfantryApi::ReadRuleLua, this, std::placeholders::_1)),
		WriteRuleLambda(std::bind(&InfantryApi::WriteRuleLua, this, std::placeholders::_1))
	{
		Init();
	}

protected:
	const LuaLambda& GetReadRuleLambda()
	{
		return ReadRuleLambda;
	}

	const LuaLambda& GetWriteRuleLambda()
	{
		return WriteRuleLambda;
	}

	bool ValidateTypeName(const char* name)
	{
		bool parseError = false;
		
		Parse_Infantry_Type(name, &parseError);

		return parseError;
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
