#include "lua.h"
#include "rules_cache.h"
#include "rules_ini.h"
#include "rules_ini_game.h"
#include "strings.h"
#include "utils.h"

static const char* GET_GAME_RULE_FUNCTION = "\
  function getGameRule(ruleName)                    \n\
    local gameRules = getGameRuleNames()            \n\
    local gameRuleTypes = getGameRuleTypeNames()    \n\
                                                    \n\
    for idx, rule in ipairs(gameRules) do           \n\
      if rule == ruleName then                      \n\
        local value                                 \n\
                                                    \n\
        if gameRuleTypes[idx] == 'string' then      \n\
          value = getStringGameRule(rule)           \n\
        elseif gameRuleTypes[idx] == 'int' then     \n\
          value = getIntGameRule(rule)              \n\
        elseif gameRuleTypes[idx] == 'double' then  \n\
          value = getDoubleGameRule(rule)           \n\
        else                                        \n\
          value = getBoolGameRule(rule)             \n\
        end                                         \n\
                                                    \n\
        return value                                \n\
      end                                           \n\
    end                                             \n\
                                                    \n\
    return nil                                      \n\
  end                                               \n\
";

static const char* SET_GAME_RULE_FUNCTION = "\
  function setGameRule(ruleName, ruleValue)         \n\
    local gameRules = getGameRuleNames()            \n\
    local gameRuleTypes = getGameRuleTypeNames()    \n\
                                                    \n\
    for idx, rule in ipairs(gameRules) do           \n\
      if rule == ruleName then                      \n\
        local value                                 \n\
                                                    \n\
        if gameRuleTypes[idx] == 'string' then      \n\
          setStringGameRule(rule, ruleValue)        \n\
        elseif gameRuleTypes[idx] == 'int' then     \n\
          setIntGameRule(rule, ruleValue)           \n\
        elseif gameRuleTypes[idx] == 'double' then  \n\
          setDoubleGameRule(rule, ruleValue)        \n\
        else                                        \n\
          setBoolGameRule(rule, ruleValue)          \n\
        end                                         \n\
      end                                           \n\
    end                                             \n\
  end                                               \n\
";

static int Lua_Get_String_Game_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_String_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "getStringGameRule requires at least one argument");
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    bool valueFound = false;
    auto ruleValue = Read_Optional_String_From_Rules_Ini(GAME_RULES_SECTION_NAME, ruleName, &valueFound, false);

    if (valueFound)
    {
        lua_pushstring(lua, ruleValue);
    }
    else
    {
        lua_pushnil(lua);
    }

    return 1;
}

static int Lua_Get_Int_Game_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Int_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "getIntGameRule requires at least one argument");
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    bool valueFound = false;
    auto ruleValue = Read_Optional_Int_From_Rules_Ini(GAME_RULES_SECTION_NAME, ruleName, &valueFound);

    if (valueFound)
    {
        lua_pushnumber(lua, ruleValue);
    }
    else
    {
        lua_pushnil(lua);
    }

    return 1;
}

static int Lua_Get_Double_Game_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Double_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "getDoubleGameRule requires at least one argument");
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    bool valueFound = false;
    auto ruleValue = Read_Optional_Double_From_Rules_Ini(GAME_RULES_SECTION_NAME, ruleName, &valueFound);

    if (valueFound)
    {
        lua_pushnumber(lua, ruleValue);
    }
    else
    {
        lua_pushnil(lua);
    }

    return 1;
}

static int Lua_Get_Boolean_Game_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Boolean_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "getBoolGameRule requires at least one argument");
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    bool valueFound = false;
    auto ruleValue = Read_Optional_Bool_From_Rules_Ini(GAME_RULES_SECTION_NAME, ruleName, &valueFound);

    if (valueFound)
    {
        lua_pushboolean(lua, ruleValue);
    }
    else
    {
        lua_pushnil(lua);
    }

    return 1;
}

template<typename T> static int Lua_Set_Game_Rule(
    lua_State* lua,
    const char* typeName,
    T(*readValue)(lua_State*, int),
    void(*writeCacheValue)(const char*, const char*, T))
{
    Log_Trace("Lua_Set_%s_Game_Rule called from Lua", typeName);

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "set%sGameRule requires at least two arguments", typeName);
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(1);
    auto ruleValue = readValue(lua, 2);

    writeCacheValue(GAME_RULES_SECTION_NAME, ruleName, ruleValue);

    return 0;
}

static const char* Lua_To_String_Wrapper(lua_State* lua, int index)
{
    return Convert_String_To_Upper_Case(
        Get_Lua_Value_As_String(index)
    );
}

static int Lua_Set_String_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "String", &Lua_To_String_Wrapper, Cache_String_Rule);
}

static int Lua_To_Int_Wrapper(lua_State* lua, int index)
{
    return luaL_checknumber(lua, index);
}

static int Lua_Set_Int_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Int", &Lua_To_Int_Wrapper, Cache_Int_Rule);
}

static int Lua_Set_Double_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Double", &luaL_checknumber, Cache_Double_Rule);
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

static int Lua_Set_Boolean_Game_Rule(lua_State* lua)
{
    return Lua_Set_Game_Rule(lua, "Bool", &Lua_To_Bool_Wrapper, Cache_Bool_Rule);
}

static bool Load_Lua_Api_Functions()
{
    return Execute_Lua_Script(GET_GAME_RULE_FUNCTION)
        && Execute_Lua_Script(SET_GAME_RULE_FUNCTION);
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

    return Load_Lua_Api_Functions();
}
