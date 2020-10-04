#include "lua.h"
#include "rules_ini.h"
#include "rules_ini_game.h"
#include <strings.h>
#include <utils.h>

static int Lua_Get_Game_Rule(lua_State* _)
{
    Log_Trace("Lua_Get_Game_Rule called from Lua");

    auto &luaState = LuaRuntime().GetState();
    auto argCount = luaState.GetStackTop();

    if (argCount < 1)
    {
        luaState.RaiseError("getGameRule requires at least one argument");
        return 0;
    }

    auto &ruleNameResult = luaState.ReadString(1);

    auto &gameRules = GetRulesInfo().GetGameRules();
    auto ruleKey = gameRules.BuildKey(ruleNameResult.GetValue());

    if (!gameRules.HasRule(ruleKey))
    {
        luaState.RaiseError("Rule '%s' passed to getGameRule was not recognised", ruleNameResult.GetValue());

        delete &ruleNameResult;

        return 0;
    }

    auto& rule = gameRules[ruleKey];

    if (rule.HasValue())
    {
        if (Current_Log_Level() == TRACE)
        {
            auto valueStr = Allocate_String(32);

            rule.WriteValueToString(valueStr);

            Log_Trace("Lua_Get_Game_Rule: Read rule value %s: %s", rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        rule.PushValueOntoLuaState(luaState);
    }
    else if (rule.HasDefaultValue())
    {
        if (Current_Log_Level() == TRACE)
        {
            auto valueStr = Allocate_String(32);

            rule.WriteDefaultValueToString(valueStr);

            Log_Trace("Lua_Get_Game_Rule: Read rule value %s: %s", rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        rule.PushDefaultValueOntoLuaState(luaState);
    }
    else
    {
        Log_Trace("Lua_Get_Game_Rule: No value for rule %s, returning nil", rule.GetStringKey());

        luaState.WriteNil();
    }

    delete &ruleNameResult;

    return 1;
}

static int Lua_Set_Game_Rule(lua_State* lua)
{
    Log_Trace("Lua_Set_Game_Rule called from Lua");
    
    auto &luaState = LuaRuntime().GetState();
    auto argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaState.RaiseError("setGameRule requires at least two arguments");
        return 0;
    }

    auto &ruleNameResult = luaState.ReadString(1);

    auto &gameRules = GetRulesInfo().GetGameRules();
    auto ruleKey = gameRules.BuildKey(ruleNameResult.GetValue());

    if (!gameRules.HasRule(ruleKey))
    {
        luaState.RaiseError("Rule '%s' passed to set%sGameRule was not recognised", ruleNameResult.GetValue());

        delete &ruleNameResult;

        return 0;
    }

    auto& rule = gameRules[ruleKey];

    auto setOk = rule.SetValueFromLuaState(luaState, 2);

    if (setOk && Current_Log_Level() == TRACE)
    {
        auto valueStr = Allocate_String(32);

        rule.WriteValueToString(valueStr);

        Log_Trace("Lua_Set_Game_Rule: Set rule value %s: %s", rule.GetStringKey(), valueStr);

        delete valueStr;
    }

    delete &ruleNameResult;

    return 0;
}

bool Register_Game_Rule_Functions()
{
    Log_Debug("Registering Game Rules Lua API functions");

    Register_Lua_Function("getGameRule", Lua_Get_Game_Rule);
    Register_Lua_Function("setGameRule", Lua_Set_Game_Rule);

    return true;
}
