#include "function.h"

#include "InfantryApi.h"

int InfantryReadRuleProxy(lua_State* lua)
{
	auto instancePtrUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto instancePtr = (TypeApi<InfantryTypeClass>*)instancePtrUserData;

	return instancePtr->ReadRuleLua(lua);
}

int InfantryWriteRuleProxy(lua_State* lua)
{
	auto instancePtrUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto instancePtr = (TypeApi<InfantryTypeClass>*)instancePtrUserData;

	return instancePtr->WriteRuleLua(lua);
}
