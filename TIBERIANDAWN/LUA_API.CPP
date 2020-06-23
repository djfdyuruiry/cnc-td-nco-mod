#include "function.h"

static bool API_INITIALISED = false;

static bool Write_TechnoType_Rule(TechnoTypeClass * type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        type->Cost = atoi(value);
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        type->IsBuildable = value == "TRUE";
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        type->IsFlammable = value == "TRUE";
    }
    else if (Strings_Are_Equal(ruleName, MAX_SPEED_RULE_UPPER))
    {
        auto numValue = (unsigned short)atoi(value);
        *valueParseError = numValue > 0 && numValue <= UCHAR_MAX;

        if (!*valueParseError)
        {
            type->MaxSpeed = (MPHType)numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, MAX_STRENGTH_RULE_UPPER))
    {
        auto numValue = (unsigned short)atoi(value);
        *valueParseError = numValue > 0 && numValue <= USHRT_MAX;

        if (!*valueParseError)
        {
            type->MaxStrength = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, OWNABLE_RULE_UPPER))
    {
        auto houseType = Parse_House_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Ownable = houseType;
        }
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Primary = weapon;
        }
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Secondary = weapon;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_TechnoType_Rule(lua_State * lua, TechnoTypeClass * type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        lua_pushnumber(lua, type->Cost);
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        lua_pushboolean(lua, type->Cost);
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        lua_pushboolean(lua, type->IsFlammable);
    }
    else if (Strings_Are_Equal(ruleName, MAX_SPEED_RULE_UPPER))
    {
        lua_pushnumber(lua, type->MaxSpeed);
    }
    else if (Strings_Are_Equal(ruleName, MAX_STRENGTH_RULE_UPPER))
    {
        lua_pushnumber(lua, type->MaxStrength);
    }
    else if (Strings_Are_Equal(ruleName, OWNABLE_RULE_UPPER))
    {
        lua_pushstring(lua, type->HouseListCsv);
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        lua_pushstring(lua, Weapon_Type_To_String(type->Primary));
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        lua_pushstring(lua, Weapon_Type_To_String(type->Secondary));
    }
    else
    {
        return false;
    }

    return true;
}

static InfantryType ParseInfantryType(lua_State* lua, LuaString infantryTypeName, bool* parseError)
{
    auto infantryType = Parse_Infantry_Type(
        Convert_String_To_Upper_Case(infantryTypeName.value),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "infantry type passed was not recognised: %s", infantryTypeName.value);
        return INFANTRY_NONE;
    }

    return infantryType;
}

static int Lua_Set_Infantry_Rule(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setInfantryRule requires exactly three arguments");
        return 0;
    }

    auto infantryTypeName = Read_Lua_String(1);

    bool parseError = false;
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto infantry = (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
    auto ruleName = Read_Lua_String(2);
    auto ruleValue = Read_Lua_String(3);

    auto ruleMatched = Write_TechnoType_Rule(
        infantry,
        Convert_String_To_Upper_Case(ruleName.value),
        Convert_String_To_Upper_Case(ruleValue.value),
        &parseError
    );

    if (parseError)
    {
        luaL_error(lua, "value for rule `%s` passed to setInfantryRule was not valid: %s", ruleName.value, ruleValue.value);

        return 0;
    }

    if (ruleMatched)
    {
        return 1;
    }

    luaL_error(lua, "rule name type passed setInfantryRule was not recognised: %s", ruleName.value);

    return 0;
}

static int Lua_Get_Infantry_Rule(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getInfantryRule requires exactly two arguments");
    }

    auto infantryTypeName = Read_Lua_String(1);

    bool parseError = false;
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto infantry = (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
    auto ruleName = Read_Lua_String(2);

    auto ruleMatched = Read_TechnoType_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName.value));

    if (ruleMatched)
    {
        return 1;
    }

    luaL_error(lua, "rule name type passed getInfantryRule was not recognised: %s", ruleName.value);

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
