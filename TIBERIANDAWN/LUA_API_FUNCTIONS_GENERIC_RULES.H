#ifndef LUA_API_FUNCTIONS_GENERIC_RULES_H
#define LUA_API_FUNCTIONS_GENERIC_RULES_H

#include "type.h"

#include "lua.h"
#include "strings.h"

bool Write_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName, const char* uppercaseValue, const char* originalValue, bool* valueParseError);

bool Read_TechnoType_Rule(lua_State* lua, TechnoTypeClass* type, const char* ruleName);

/**
 * Full defintions required here, see: https://stackoverflow.com/questions/456713/why-do-i-get-unresolved-external-symbol-errors-when-using-templates
 */
template<typename T> int Lua_Get_Rule(
    lua_State* lua,
    const char* typeName,
    const char* paramName,
    T* (*typeParser)(lua_State*, const char*),
    bool(*readTypeRule)(lua_State*, T*, const char*),
    bool searchTechnoType = true
)
{
    Log_Trace("Lua_Get_%s_Rule called from Lua", typeName);

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "get%sRule requires exactly two arguments", typeName);
    }

    auto typeInstanceName = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(typeInstanceName))
    {
        luaL_error(lua, "get%sRule argument `%s` was nil or blank", typeName, paramName);
        return 0;
    }

    auto typeInstance = typeParser(lua, typeInstanceName);

    if (typeInstance == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto uppercaseRuleName = Convert_String_To_Upper_Case(ruleName);

    if (ruleName == NULL)
    {
        luaL_error(lua, "get%sRule argument `ruleName` was nil", typeName);
        return 0;
    }

    Log_Debug("Lua_Get_%s_Rule => attempting to read value of rule '%s'", typeName, ruleName);

    auto ruleMatched = false;

    if (searchTechnoType)
    {
        Log_Trace("Lua_Get_%s_Rule => looking for rule '%s' in Techno Type", typeName, ruleName);

        auto uppercaseRuleName = Convert_String_To_Upper_Case(ruleName);

        ruleMatched = Read_TechnoType_Rule(lua, (TechnoTypeClass*)typeInstance, uppercaseRuleName);

        if (ruleMatched)
        {
            Log_Debug("Lua_Get_%s_Rule => Rule matched in Techno Type", typeName);

            delete uppercaseRuleName;

            return 1;
        }
    }

    Log_Trace("Lua_Get_%s_Rule => looking for rule in %s Type", typeName, typeName);

    ruleMatched = readTypeRule(lua, typeInstance, uppercaseRuleName);

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_%s_Rule => Rule matched in %s Type", typeName, typeName);

        delete uppercaseRuleName;

        return 1;
    }

    luaL_error(lua, "rule name type passed get%sRule was not recognised: %s", typeName, ruleName);

    delete uppercaseRuleName;

    return 0;
}

template<typename T> int Lua_Set_Rule(
    lua_State* lua,
    const char* typeName,
    const char* paramName,
    T* (*typeParser)(lua_State*, const char*),
    bool(*writeTypeRule)(T*, const char*, const char*, const char*, bool*),
    bool searchTechnoType = true
)
{
    Log_Trace("Lua_Set_%s_Rule called from Lua", typeName);

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "set%sRule requires exactly three arguments", typeName);
        return 0;
    }

    auto typeInstanceName = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(typeInstanceName))
    {
        luaL_error(lua, "set%sRule argument `%s` was nil or blank", typeName, paramName);
        return 0;
    }

    auto typeInstance = typeParser(lua, typeInstanceName);

    if (typeInstance == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto ruleValue = Get_Lua_Value_As_String(3);

    if (String_Is_Empty(ruleName))
    {
        luaL_error(lua, "set%sRule argument `ruleName` was nil or blank", typeName);

        return 0;
    }
    else if (String_Is_Empty(ruleValue))
    {
        luaL_error(lua, "set%sRule argument `ruleValue` was nil or blank", typeName);

        return 0;
    }

    auto uppercaseRuleName = Convert_String_To_Upper_Case(ruleName);
    auto uppercaseRuleValue = Convert_String_To_Upper_Case(ruleValue);

    Log_Debug("Lua_Set_%s_Rule => attempting to set rule '%s' to value: %s", typeName, ruleName, ruleValue);

    auto ruleMatched = false;
    bool parseError = false;
 
    if (searchTechnoType)
    {
        Log_Trace("Lua_Set_%s_Rule => looking for rule in Techno Type", typeName);

        ruleMatched = Write_TechnoType_Rule(
            (TechnoTypeClass*)typeInstance,
            uppercaseRuleName,
            uppercaseRuleValue,
            ruleValue,
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_%s_Rule => Rule matched in Techno Type", typeName);

            delete uppercaseRuleName;
            delete uppercaseRuleValue;

            return 0;
        }
    }

    if (!parseError)
    {
        Log_Trace("Lua_Set_%s_Rule => looking for rule in %s Type", typeName, typeName);

        ruleMatched = writeTypeRule(
            typeInstance,
            uppercaseRuleName,
            uppercaseRuleValue,
            ruleValue,
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_%s_Rule => Rule matched in %s Type", typeName, typeName);

            delete uppercaseRuleName;
            delete uppercaseRuleValue;

            return 0;
        }
    }
    else
    {
        luaL_error(lua, "value for rule `%s` passed to set%sRule was not valid: %s", typeName, ruleName, ruleValue);

        delete uppercaseRuleName;
        delete uppercaseRuleValue;

        return 0;
    }

    luaL_error(lua, "rule name passed to set%sRule was not recognised: %s", typeName, ruleName);

    delete uppercaseRuleName;
    delete uppercaseRuleValue;

    return 0;
}

#endif
