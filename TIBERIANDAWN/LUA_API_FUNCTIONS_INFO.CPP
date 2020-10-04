#include "function.h"

static void Read_Object_Names(
    char first,
    char count,
    void* toStringFunctionPtr
)
{
    auto toStringFunction = (char* (*)(char, bool))toStringFunctionPtr;
    auto &vector = std::vector<const char*>();

    for (char i = first; i < count; i++)
    {
        auto name = toStringFunction(i, false);

        vector.push_back(name);
    }

    LuaRuntime().GetState().WriteArray(vector);
}

static int Lua_Get_Infantry_Types(lua_State* _)
{
    InitialiseModsIfRequired();

    Read_Object_Names(INFANTRY_FIRST, Read_Infantry_Count(), &(Infantry_Type_To_String));

    return 1;
}

static int Lua_Get_Unit_Types(lua_State* _)
{
    InitialiseModsIfRequired();

    Read_Object_Names(UNIT_FIRST, Read_Unit_Count(), &(Unit_Type_To_String));

    return 1;
}

static int Lua_Get_Aircraft_Types(lua_State* _)
{
    Read_Object_Names(AIRCRAFT_FIRST, Read_Aircraft_Count(), &(Aircraft_Type_To_String));

    return 1;
}

static int Lua_Get_Building_Types(lua_State* _)
{
    Read_Object_Names(STRUCT_FIRST, Read_Building_Count(), &(Structure_Type_To_String));

    return 1;
}

static int Lua_Get_Warhead_Rule_Names(lua_State* _)
{
    std::vector<const char*>& warheadRules = GetRulesInfo()
        .GetWarheadRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(warheadRules);

    return 1;
}

static int Lua_Get_Bullet_Rule_Names(lua_State* _)
{
    std::vector<const char*>& bulletRules = GetRulesInfo()
        .GetBulletRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(bulletRules);

    return 1;
}

static int Lua_Get_Weapon_Rule_Names(lua_State* _)
{
    std::vector<const char*>& weaponRules = GetRulesInfo()
        .GetWeaponRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(weaponRules);

    return 1;
}

static int Lua_Get_Game_Rule_Names(lua_State* _)
{
    std::vector<const char*>& gameRules = GetRulesInfo()
        .GetGameRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(gameRules);

    return 1;
}

static int Lua_Get_Infantry_Rule_Names(lua_State* _)
{
    std::vector<const char*>& infantryRules = GetRulesInfo()
        .GetInfantryRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(infantryRules);

    return 1;
}

static int Lua_Get_Unit_Rule_Names(lua_State* _)
{
    std::vector<const char*>& unitRules = GetRulesInfo()
        .GetUnitRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(unitRules);

    return 1;
}

static int Lua_Get_Aircraft_Rule_Names(lua_State* _)
{
    std::vector<const char*>& aircraftRules = GetRulesInfo()
        .GetAircraftRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(aircraftRules);

    return 1;
}

static int Lua_Get_Building_Rule_Names(lua_State* _)
{
    std::vector<const char*>& buildingRules = GetRulesInfo()
        .GetBuildingRules()
        .GetRuleNames();

    LuaRuntime().GetState().WriteArray(buildingRules);

    return 1;
}

static int Lua_Get_Warhead_Types(lua_State* _)
{
    Read_Object_Names(WARHEAD_FIRST, Read_Warhead_Count(), &(Warhead_Type_To_String));

    return 1;
}

static int Lua_Get_Bullet_Types(lua_State* _)
{
    Read_Object_Names(BULLET_SNIPER, Read_Bullet_Count(), &(Bullet_Type_To_String));

    return 1;
}

static int Lua_Get_Weapon_Types(lua_State* _)
{
    Read_Object_Names(WEAPON_FIRST, Read_Weapon_Count(), &(Weapon_Type_To_String));

    return 1;
}

static int Lua_Get_Event_Names(lua_State* _)
{
    auto& events = std::vector<const char*>();

    for (auto i = 0; i < LUA_EVENT_COUNT; i++)
    {
        events.push_back(LUA_EVENTS[i]);
    }

    LuaRuntime().GetState().WriteArray(events);

    return 1;
}

bool Register_Info_Functions()
{
    Log_Debug("Registering Info Lua API functions");

    Register_Lua_Function("getEventNames", Lua_Get_Event_Names);

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
