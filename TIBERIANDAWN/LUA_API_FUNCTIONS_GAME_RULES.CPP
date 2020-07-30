#include "lua.h"
#include "rules_ini.h"
#include "rules_ini_game.h"
#include "strings.h"
#include "utils.h"

template<typename T> static int Lua_Get_Game_Rule(
    lua_State* lua,
    const char* typeName,
    void(*writeValue)(lua_State*, T)
)
{
    Log_Trace("Lua_Get_%s_Game_Rule called from Lua", typeName);

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "get%sGameRule requires at least one argument", typeName);
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    IRulesIniSection& gameRules = GetRulesInfo().GetGameRules();
    auto ruleKey = gameRules.BuildKey(ruleName);

    if (!gameRules.HasRule(ruleKey))
    {
        luaL_error(lua, "Rule '%s' passed to get%sGameRule was not recognised", ruleName);
        return 0;
    }

    auto rule = gameRules[ruleKey];

    if (rule.HasValue())
    {
        T value = rule.GetValue<T>();

        writeValue(lua, value);
    }
    else if (rule.HasDefaultValue())
    {
        T value = rule.GetDefaultValue<T>();

        writeValue(lua, value);
    }
    else
    {
        lua_pushnil(lua);
    }

    return 0;
}

template<typename T> static int Lua_Set_Game_Rule(
    lua_State* lua,
    const char* typeName,
    T(*readValue)(lua_State*, int)
)
{
    Log_Trace("Lua_Set_%s_Game_Rule called from Lua", typeName);

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "set%sGameRule requires at least two arguments", typeName);
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    IRulesIniSection& gameRules = GetRulesInfo().GetGameRules();
    auto ruleKey = gameRules.BuildKey(ruleName);

    if (!gameRules.HasRule(ruleKey))
    {
        luaL_error(lua, "Rule '%s' passed to set%sGameRule was not recognised", ruleName);
        return 0;
    }

    auto ruleValue = readValue(lua, 2);

    auto rule = gameRules[ruleKey];

    rule.SetValue<T>(ruleValue);

    return 0;
}

static void Lua_From_String_Wrapper(lua_State* lua, char* str)
{
    lua_pushstring(lua, str);
}


static bool Lua_To_Bool_Wrapper(lua_State* lua, int index)
{
    bool parseError = false;
    auto value = Parse_Boolean(Get_Lua_Value_As_String(index), &parseError);

    if (parseError)
    {
        luaL_error(lua, "Failed to parse value as boolean");
    }

    return value;
}

static int Lua_To_Int_Wrapper(lua_State* lua, int index)
{
    return luaL_checknumber(lua, index);
}

static const char* Lua_To_String_Wrapper(lua_State* lua, int index)
{
    return Get_Lua_Value_As_String(index);
}

static int Lua_Get_String_Game_Rule(lua_State* lua)
{
    return Lua_Get_Game_Rule(lua, "String", &Lua_From_String_Wrapper);
}

static int Lua_Get_Int_Game_Rule(lua_State* lua)
{
    return Lua_Get_Game_Rule(lua, "Int", &lua_pushnumber);
}

static int Lua_Get_Double_Game_Rule(lua_State* lua)
{
    return Lua_Get_Game_Rule(lua, "Double", &lua_pushnumber);
}

static int Lua_Get_Boolean_Game_Rule(lua_State* lua)
{
    return Lua_Get_Game_Rule(lua, "Bool", &lua_pushboolean);
}

static int Lua_Set_String_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "String", &Lua_To_String_Wrapper);
}

static int Lua_Set_Int_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Int", &Lua_To_Int_Wrapper);
}

static int Lua_Set_Double_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Double", &luaL_checknumber);
}

static int Lua_Set_Boolean_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Bool", &Lua_To_Bool_Wrapper);
}

bool Register_Game_Rule_Functions()
{
    Log_Debug("Registering Game Rules Lua API functions");

    Register_Lua_Function("getStringGameRule", Lua_Get_String_Game_Rule);
    Register_Lua_Function("getIntGameRule", Lua_Get_Int_Game_Rule);
    Register_Lua_Function("getDoubleGameRule", Lua_Get_Double_Game_Rule);
    Register_Lua_Function("getBoolGameRule", Lua_Get_Boolean_Game_Rule);

    Register_Lua_Function("setStringGameRule", Lua_Set_String_Game_Rule);
    Register_Lua_Function("setIntGameRule", Lua_Set_Int_Game_Rule);
    Register_Lua_Function("setDoubleGameRule", Lua_Set_Double_Game_Rule);
    Register_Lua_Function("setBoolGameRule", Lua_Set_Boolean_Game_Rule);

    return true;
}
