#include "lua.h"
#include "rules_ini.h"
#include "rules_ini_game.h"
#include "strings.h"
#include "utils.h"

static int Lua_Get_Rule_Value(lua_State* lua)
{
    Log_Trace("Lua_Get_Rule_Value called from Lua");

    auto argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getRuleValue requires at least two arguments");
        return 0;
    }

    auto sectionName = Get_Lua_Value_As_String(1);
    auto ruleName = Get_Lua_Value_As_String(2);

    auto& rules = GetRules();
    auto sectionKey = Build_Rule_Key(sectionName);

    if (!rules.HasSection(sectionKey))
    {
        luaL_error(lua, "Section '%s' passed to getRuleValue was not recognised", sectionName);
        return 0;
    }

    auto& rulesSection = rules[sectionKey];
    auto ruleKey = rulesSection.BuildKey(ruleName);

    if (!rulesSection.HasRule(ruleKey))
    {
        luaL_error(lua, "Rule '%s' passed to getRuleValue for section '%s' was not recognised", ruleName, sectionName);
        return 0;
    }

    auto& rule = rulesSection[ruleKey];

    if (rule.HasValue())
    {
        if (Current_Log_Level() == TRACE)
        {
            auto valueStr = Allocate_String(32);

            rule.WriteValueToString(valueStr);

            Log_Trace("Lua_Get_Rule_Value: Read rule value %s: %s", rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        rule.PushValueOntoLuaState(lua);
    }
    else if (rule.HasDefaultValue())
    {
        if (Current_Log_Level() == TRACE)
        {
            auto valueStr = Allocate_String(32);

            rule.WriteDefaultValueToString(valueStr);

            Log_Trace("Lua_Get_Rule_Value: Read rule value %s: %s", rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        rule.PushDefaultValueOntoLuaState(lua);
    }
    else
    {
        Log_Trace("Lua_Get_Rule_Value: No value for rule %s, returning nil", rule.GetStringKey());

        lua_pushnil(lua);
    }

    return 1;
}

static int Lua_Set_Rule_Value(lua_State* lua)
{
    Log_Trace("Lua_Set_Rule_Value called from Lua");

    auto argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setRuleValue requires at least three arguments");
        return 0;
    }

    auto sectionName = Get_Lua_Value_As_String(1);
    auto ruleName = Get_Lua_Value_As_String(2);

    auto& rules = GetRules();
    auto sectionKey = Build_Rule_Key(sectionName);

    if (!rules.HasSection(sectionKey))
    {
        luaL_error(lua, "Section '%s' passed to setRuleValue was not recognised", sectionName);
        return 0;
    }

    auto& rulesSection = rules[sectionKey];
    auto ruleKey = rulesSection.BuildKey(ruleName);

    if (!rulesSection.HasRule(ruleKey))
    {
        luaL_error(lua, "Rule '%s' passed to setRuleValue was not recognised", ruleName);
        return 0;
    }

    auto& rule = rulesSection[ruleKey];

    auto setOk = rule.SetValueFromLuaState(lua, 3);

    if (setOk && Current_Log_Level() == TRACE)
    {
        auto valueStr = Allocate_String(32);

        rule.WriteValueToString(valueStr);

        Log_Trace("Lua_Set_Rule_Value: Set rule value %s: %s", rule.GetStringKey(), valueStr);

        delete valueStr;
    }

    return 0;
}

bool Register_Game_Rule_Functions()
{
    Log_Debug("Registering Game Rules Lua API functions");

    Register_Lua_Function("getRuleValue", Lua_Get_Rule_Value);
    Register_Lua_Function("setRuleValue", Lua_Set_Rule_Value);

    return true;
}
