#pragma once

#include "function.h"

#include "IRulesIniSection.h"
#include "lua.h"
#include "rules_cache_key.h"
#include "RulesIniInfo.h"
#include <strings.h>

bool Write_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName, const char* uppercaseValue, const char* originalValue, bool* valueParseError);

bool Read_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName);

/**
 * Full defintions required here, see: https://stackoverflow.com/questions/456713/why-do-i-get-unresolved-external-symbol-errors-when-using-templates
 */
template<typename T> int Lua_Get_Rule(
    const char* typeName,
    const char* paramName,
    T* (*typeParser)(const char*),
    IRulesIniSection& rulesInfo,
    bool(*readTypeRule)(T*, const char*),
    bool searchTechnoType = true
)
{
    auto &luaState = LuaRuntime().GetState();

    Log_Trace("Lua_Get_%s_Rule called from Lua", typeName);

    int argCount = luaState.GetStackTop();

    if (argCount < 2)
    {
        luaState.RaiseError("get%sRule requires exactly two arguments", typeName);

        return 0;
    }

    auto &typeInstanceNameResult = luaState.ReadString(1);

    if (typeInstanceNameResult.IsErrorResult() || String_Is_Empty(typeInstanceNameResult.GetValue()))
    {
        luaState.RaiseError("get%sRule argument `%s` was nil or blank", typeName, paramName);

        delete &typeInstanceNameResult;

        return 0;
    }

    auto typeInstance = typeParser(typeInstanceNameResult.GetValue());

    if (typeInstance == NULL)
    {
        delete &typeInstanceNameResult;

        return 0;
    }

    auto &ruleNameParameterResult = luaState.ReadString(2);

    if (ruleNameParameterResult.IsErrorResult() || String_Is_Empty(ruleNameParameterResult.GetValue()))
    {
        luaState.RaiseError("get%sRule argument `ruleName` was nil", typeName);
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }

    auto ruleKey = rulesInfo.BuildKey(ruleNameParameterResult.GetValue());
    auto ruleIsValid = rulesInfo.HasRule(ruleKey);

    if (!ruleIsValid)
    {
        luaState.RaiseError("rule name type passed get%sRule was not recognised for this type: %s", typeName, ruleNameParameterResult.GetValue());
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }

    Log_Debug("Lua_Get_%s_Rule => attempting to read value of rule '%s'", typeName, ruleNameParameterResult.GetValue());

    auto ruleName = rulesInfo[ruleKey].GetName();
    auto ruleMatched = false;

    if (searchTechnoType)
    {
        Log_Trace("Lua_Get_%s_Rule => looking for rule '%s' in Techno Type", typeName, ruleNameParameterResult.GetValue());

        ruleMatched = Read_TechnoType_Rule((TechnoTypeClass*)typeInstance, ruleName);

        if (ruleMatched)
        {
            Log_Debug("Lua_Get_%s_Rule => Rule matched in Techno Type", typeName);
        
            delete &typeInstanceNameResult;
            delete &ruleNameParameterResult;

            return 1;
        }
    }

    Log_Trace("Lua_Get_%s_Rule => looking for rule in %s Type", typeName, typeName);

    ruleMatched = readTypeRule(typeInstance, ruleName);

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_%s_Rule => Rule matched in %s Type", typeName, typeName);
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 1;
    }

    luaState.RaiseError("rule name type passed get%sRule was not matched: %s", typeName, ruleNameParameterResult.GetValue());

    delete &typeInstanceNameResult;
    delete &ruleNameParameterResult;

    return 0;
}

template<typename T> int Lua_Set_Rule(
    const char* typeName,
    const char* paramName,
    T* (*typeParser)(const char*),
    IRulesIniSection& rulesInfo,
    bool(*writeTypeRule)(T*, const char*, const char*, const char*, bool*),
    bool searchTechnoType = true
)
{
    auto &luaState = LuaRuntime().GetState();

    Log_Trace("Lua_Set_%s_Rule called from Lua", typeName);

    int argCount = luaState.GetStackTop();

    if (argCount < 3)
    {
        luaState.RaiseError("set%sRule requires exactly three arguments", typeName);

        return 0;
    }

    auto &typeInstanceNameResult = luaState.ReadString(1);

    if (String_Is_Empty(typeInstanceNameResult.GetValue()))
    {
        luaState.RaiseError("set%sRule argument `%s` was nil or blank", typeName, paramName);

        delete &typeInstanceNameResult;

        return 0;
    }

    auto typeInstance = typeParser(typeInstanceNameResult.GetValue());

    if (typeInstance == NULL)
    {
        return 0;
    }

    auto &ruleNameParameterResult = luaState.ReadString(2);
    auto ruleValue = luaState.ToString(3);

    if (ruleNameParameterResult.IsErrorResult() || String_Is_Empty(ruleNameParameterResult.GetValue()))
    {
        luaState.RaiseError("set%sRule argument `ruleName` was nil or blank", typeName);
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }
    else if (String_Is_Empty(ruleValue))
    {
        luaState.RaiseError("set%sRule argument `ruleValue` was nil or blank", typeName);
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }

    auto uppercaseRuleValue = Convert_String_To_Upper_Case(ruleValue);

    Log_Debug("Lua_Set_%s_Rule => attempting to set rule '%s' to value: %s", typeName, ruleNameParameterResult.GetValue(), ruleValue);

    auto ruleKey = rulesInfo.BuildKey(ruleNameParameterResult.GetValue());
    auto ruleIsValid = rulesInfo.HasRule(ruleKey);

    if (!ruleIsValid)
    {
        luaState.RaiseError("rule name type passed get%sRule was not recognised for this type: %s", typeName, ruleNameParameterResult.GetValue());
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }

    Log_Debug("Lua_Get_%s_Rule => attempting to read value of rule '%s'", typeName, ruleNameParameterResult.GetValue());

    auto ruleName = rulesInfo[ruleKey].GetName();

    auto ruleMatched = false;
    bool parseError = false;
 
    if (searchTechnoType)
    {
        Log_Trace("Lua_Set_%s_Rule => looking for rule in Techno Type", typeName);

        ruleMatched = Write_TechnoType_Rule(
            (TechnoTypeClass*)typeInstance,
            ruleName,
            uppercaseRuleValue,
            ruleValue,
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_%s_Rule => Rule matched in Techno Type", typeName);

            delete uppercaseRuleValue;
        
            delete &typeInstanceNameResult;
            delete &ruleNameParameterResult;

            return 0;
        }
    }

    if (!parseError)
    {
        Log_Trace("Lua_Set_%s_Rule => looking for rule in %s Type", typeName, typeName);

        ruleMatched = writeTypeRule(
            typeInstance,
            ruleName,
            uppercaseRuleValue,
            ruleValue,
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_%s_Rule => Rule matched in %s Type", typeName, typeName);

            delete uppercaseRuleValue;
        
            delete &typeInstanceNameResult;
            delete &ruleNameParameterResult;

            return 0;
        }
    }
    else
    {
        luaState.RaiseError("value for rule `%s` passed to set%sRule was not valid: %s", typeName, ruleNameParameterResult.GetValue(), ruleValue);

        delete uppercaseRuleValue;
        
        delete &typeInstanceNameResult;
        delete &ruleNameParameterResult;

        return 0;
    }

    luaState.RaiseError("rule name passed to set%sRule was not matched: %s", typeName, ruleNameParameterResult.GetValue());

    delete uppercaseRuleValue;
        
    delete &typeInstanceNameResult;
    delete &ruleNameParameterResult;

    return 0;
}
