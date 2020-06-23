#include "function.h"

static bool API_INITIALISED = false;

// TODO: move to parse.cpp
static InfantryType ParseInfantryType(lua_State* lua, LuaString infantryTypeName, bool* parseError)
{
    auto infantryType = Parse_Infantry_Type(
        Convert_String_To_Upper_Case(infantryTypeName.value, infantryTypeName.length),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "infantry type passed was not recognised: %s", infantryType);
        return INFANTRY_NONE;
    }

    return infantryType;
}

static int Lua_Get_Infantry_Rule(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getInfantryRule requires exactly two arguments");
    }

    bool parseError = false;
    auto infantryTypeName = Read_Lua_String(1);
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto ruleName = Read_Lua_String(2);

    if (Strings_Are_Equal(Convert_String_To_Upper_Case(ruleName.value, ruleName.length), PRIMARY_WEAPON_RULE_UPPER))
    {
        auto infantry = ((InfantryTypeClass&)InfantryTypeClass::As_Reference(infantryType));
        auto weaponName = Weapon_Type_To_String(infantry.Primary);

        lua_pushstring(lua, weaponName);
    }
    else
    {
        luaL_error(lua, "rule name type passed getInfantryRule was not recognised: %s", ruleName);
    }

    return 1;
}

static int Lua_Set_Infantry_Rule(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setInfantryRule requires exactly three arguments");
        return 0;
    }

    bool parseError = false;
    auto infantryTypeName = Read_Lua_String(1);
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto ruleName = Read_Lua_String(2);
    auto ruleValue = Read_Lua_String(3);

    if (Strings_Are_Equal(Convert_String_To_Upper_Case(ruleName.value, ruleName.length), PRIMARY_WEAPON_RULE_UPPER))
    {
        parseError = false;
        auto weapon = Parse_Weapon_Type(Convert_String_To_Upper_Case(ruleValue.value, ruleValue.length), &parseError);

        if (parseError)
        {
            luaL_error(lua, "weapon type passed to setInfantryRule was not recognised: %s", ruleValue);
            return 0;
        }

        ((InfantryTypeClass&)InfantryTypeClass::As_Reference(infantryType)).Primary = weapon;
    }
    else
    {
        luaL_error(lua, "rule name type passed setInfantryRule was not recognised: %s", ruleName);
        return 0;
    }

    return 0;
}

static void Load_Infantry_Functions()
{
    Log_Debug("Initialising Lua API infantry functions");

    Register_Lua_Function("getInfantryRule", Lua_Get_Infantry_Rule);
    Register_Lua_Function("setInfantryRule", Lua_Set_Infantry_Rule);
}

static int Lua_Log(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logMessage = Read_Lua_String(1);

        Log_Info("Lua => %s", logMessage.value);
    }

    return 0;
}

static int Lua_Show_Error(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logMessage = Read_Lua_String(1);

        Show_Error(logMessage.value);
    }

    return 0;
}

static void Load_Utility_Functions()
{
    Log_Debug("Initialising Lua API utility functions");

    Register_Lua_Function("log", Lua_Log);
    Register_Lua_Function("showError", Lua_Show_Error);
}

bool Initialise_Lua_Api()
{
    if (API_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua API more than once, ignoring");
        return true;
    }

    auto executionOK = true;
    
    Log_Info("Initialising Lua API");

    Load_Utility_Functions();
    Load_Infantry_Functions();

    API_INITIALISED = executionOK;

    return executionOK;
}
