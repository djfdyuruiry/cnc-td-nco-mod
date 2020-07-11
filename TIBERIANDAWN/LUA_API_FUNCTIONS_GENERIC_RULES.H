#ifndef LUA_API_FUNCTIONS_GENERIC_RULES_H
#define LUA_API_FUNCTIONS_GENERIC_RULES_H

#include "type.h"

#include "lua.h"

bool Write_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName, const char* uppercaseValue, const char* originalValue, bool* valueParseError);

bool Read_TechnoType_Rule(lua_State* lua, TechnoTypeClass* type, const char* ruleName);

#endif
