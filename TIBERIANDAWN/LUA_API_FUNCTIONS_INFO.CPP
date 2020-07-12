#include "function.h"

static const auto GAME_RULE_NAMES = new const char* [GAME_RULE_COUNT] {
    GAME_RULES
};

static const auto GAME_RULE_TYPE_NAMES = new const char* [GAME_RULE_COUNT] {
    GAME_RULE_TYPES
};

static const auto WEAPON_RULE_NAMES = new const char* [WEAPON_RULE_COUNT] {
    WEAPON_RULES
};

static const auto BULLET_RULE_NAMES = new const char* [BULLET_RULE_COUNT] {
    BULLET_RULES
};

static const auto WARHEAD_RULE_NAMES = new const char* [WARHEAD_RULE_COUNT] {
    WARHEAD_RULES
};

static const auto INFANTRY_RULE_NAMES = new const char* [INFANTRY_RULE_COUNT] {
    INFANTRY_RULES
};

static const auto UNIT_RULE_NAMES = new const char* [UNIT_RULE_COUNT] {
    UNIT_RULES
};

static const auto AIRCRAFT_RULE_NAMES = new const char* [AIRCRAFT_RULE_COUNT] {
    AIRCRAFT_RULES
};

static const auto BUILDING_RULE_NAMES = new const char* [BUILDING_RULE_COUNT] {
    BUILDING_RULES
};

static void Read_Object_Names(
    lua_State* lua,
    char first,
    char count,
    void* toStringFunctionPtr
)
{
    auto toStringFunction = (char* (*)(char))toStringFunctionPtr;

    lua_createtable(lua, 0, count);

    for (char i = first; i < count; i++)
    {
        auto name = toStringFunction(i);

        Log_Trace("Pushing object name onto Lua table: %s", name);

        lua_pushstring(lua, name);
        lua_rawseti(lua, -2, i + 1);
    }
}

static void Read_Rule_Names(lua_State* lua, const char** ruleNames, unsigned ruleCount)
{
    lua_createtable(lua, 0, ruleCount);

    for (unsigned i = 0; i < ruleCount; i++)
    {
        Log_Trace("Pushing object rule name onto Lua table: %s", ruleNames[i]);

        lua_pushstring(lua, ruleNames[i]);
        lua_rawseti(lua, -2, i + 1);
    }
}

static int Lua_Get_Game_Rule_Type_Names(lua_State* lua)
{
    Read_Rule_Names(lua, GAME_RULE_TYPE_NAMES, GAME_RULE_COUNT);

    return 1;
}

static int Lua_Get_Weapon_Types(lua_State* lua)
{
    Read_Object_Names(lua, WEAPON_FIRST, WEAPON_COUNT, &(Weapon_Type_To_String));

    return 1;
}

static int Lua_Get_Bullet_Types(lua_State* lua)
{
    Read_Object_Names(lua, BULLET_SNIPER, BULLET_COUNT, &(Bullet_Type_To_String));

    return 1;
}

static int Lua_Get_Warhead_Types(lua_State* lua)
{
    Read_Object_Names(lua, WARHEAD_FIRST, WARHEAD_COUNT, &(Warhead_Type_To_String));

    return 1;
}

static int Lua_Get_Infantry_Types(lua_State* lua)
{
    Read_Object_Names(lua, INFANTRY_FIRST, INFANTRY_COUNT, &(Infantry_Type_To_String));

    return 1;
}

static int Lua_Get_Unit_Types(lua_State* lua)
{
    Read_Object_Names(lua, UNIT_FIRST, UNIT_COUNT, &(Unit_Type_To_String));

    return 1;
}

static int Lua_Get_Aircraft_Types(lua_State* lua)
{
    Read_Object_Names(lua, AIRCRAFT_FIRST, AIRCRAFT_COUNT, &(Aircraft_Type_To_String));

    return 1;
}

static int Lua_Get_Building_Types(lua_State* lua)
{
    Read_Object_Names(lua, STRUCT_FIRST, STRUCT_COUNT, &(Structure_Type_To_String));

    return 1;
}

static int Lua_Get_Game_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, GAME_RULE_NAMES, GAME_RULE_COUNT);

    return 1;
}

static int Lua_Get_Weapon_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, WEAPON_RULE_NAMES, WEAPON_RULE_COUNT);

    return 1;
}

static int Lua_Get_Bullet_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, BULLET_RULE_NAMES, BULLET_RULE_COUNT);

    return 1;
}

static int Lua_Get_Warhead_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, WARHEAD_RULE_NAMES, WARHEAD_RULE_COUNT);

    return 1;
}

static int Lua_Get_Infantry_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, INFANTRY_RULE_NAMES, INFANTRY_RULE_COUNT);

    return 1;
}

static int Lua_Get_Unit_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, UNIT_RULE_NAMES, UNIT_RULE_COUNT);

    return 1;
}

static int Lua_Get_Aircraft_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, AIRCRAFT_RULE_NAMES, AIRCRAFT_RULE_COUNT);

    return 1;
}

static int Lua_Get_Building_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, BUILDING_RULE_NAMES, BUILDING_RULE_COUNT);

    return 1;
}

bool Register_Info_Functions()
{
    Log_Debug("Registering Info Lua API functions");

    Register_Lua_Function("getGameRuleTypeNames", Lua_Get_Game_Rule_Type_Names);
    Register_Lua_Function("getWeaponTypes", Lua_Get_Weapon_Types);
    Register_Lua_Function("getBulletTypes", Lua_Get_Bullet_Types);
    Register_Lua_Function("getWarheadTypes", Lua_Get_Warhead_Types);
    Register_Lua_Function("getInfantryTypes", Lua_Get_Infantry_Types);
    Register_Lua_Function("getUnitTypes", Lua_Get_Unit_Types);
    Register_Lua_Function("getAircraftTypes", Lua_Get_Aircraft_Types);
    Register_Lua_Function("getBuildingTypes", Lua_Get_Building_Types);

    Register_Lua_Function("getGameRuleNames", Lua_Get_Game_Rule_Names);
    Register_Lua_Function("getWeaponRuleNames", Lua_Get_Weapon_Rule_Names);
    Register_Lua_Function("getBulletRuleNames", Lua_Get_Bullet_Rule_Names);
    Register_Lua_Function("getWarheadRuleNames", Lua_Get_Warhead_Rule_Names);
    Register_Lua_Function("getInfantryRuleNames", Lua_Get_Infantry_Rule_Names);
    Register_Lua_Function("getUnitRuleNames", Lua_Get_Unit_Rule_Names);
    Register_Lua_Function("getAircraftRuleNames", Lua_Get_Aircraft_Rule_Names);
    Register_Lua_Function("getBuildingRuleNames", Lua_Get_Building_Rule_Names);

    return true;
}
