#include "function.h"

static void Push_Rules_Section_Table_Onto_Lua_Stack(lua_State* lua, IRulesIniSection& section)
{
    std::vector<CacheKey>& sectionKeys = section.GetRuleKeys();

    lua_createtable(lua, 0, sectionKeys.size());

    auto idx = 1;

    for (auto key : sectionKeys)
    {
        RulesIniRule& rule = section[key];

        lua_createtable(lua, 0, sectionKeys.size());
        
        lua_pushstring(lua, rule.GetName());
        
        lua_rawseti(lua, -2, idx);

        idx++;
    }
}

static void Read_Object_Names(
    lua_State* lua,
    char first,
    char count,
    void* toStringFunctionPtr
)
{
    auto toStringFunction = (char* (*)(char, bool))toStringFunctionPtr;

    lua_createtable(lua, 0, count);

    for (char i = first; i < count; i++)
    {
        auto name = toStringFunction(i, false);

        Log_Trace("Pushing object name onto Lua table: %s", name);

        lua_pushstring(lua, name);
        lua_rawseti(lua, -2, i + 1);
    }
}

static int Lua_Get_Infantry_Types(lua_State* lua)
{
    InitialiseModsIfRequired();

    Read_Object_Names(lua, INFANTRY_FIRST, Read_Infantry_Count(), &(Infantry_Type_To_String));

    return 1;
}

static int Lua_Get_Unit_Types(lua_State* lua)
{
    InitialiseModsIfRequired();

    Read_Object_Names(lua, UNIT_FIRST, Read_Unit_Count(), &(Unit_Type_To_String));

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

static int Lua_Get_Warhead_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& warheadRules = GetRulesInfo()
        .GetWarheadRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(warheadRules);

    return 1;
}

static int Lua_Get_Bullet_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& bulletRules = GetRulesInfo()
        .GetBulletRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(bulletRules);

    return 1;
}

static int Lua_Get_Weapon_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& weaponRules = GetRulesInfo()
        .GetWeaponRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(weaponRules);

    return 1;
}

static int Lua_Get_Game_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& gameRules = GetRulesInfo()
        .GetGameRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(gameRules);

    return 1;
}

static int Lua_Get_Infantry_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& infantryRules = GetRulesInfo()
        .GetInfantryRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(infantryRules);

    return 1;
}

static int Lua_Get_Unit_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& unitRules = GetRulesInfo()
        .GetUnitRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(unitRules);

    return 1;
}

static int Lua_Get_Aircraft_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& aircraftRules = GetRulesInfo()
        .GetAircraftRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(aircraftRules);

    return 1;
}

static int Lua_Get_Building_Rule_Names(lua_State* lua)
{
    std::vector<const char*>& buildingRules = GetRulesInfo()
        .GetBuildingRules()
        .GetRuleNames();

    Push_String_Vector_Onto_Lua_Stack(buildingRules);

    return 1;
}

static int Lua_Get_Warhead_Types(lua_State* lua)
{
    Read_Object_Names(lua, WARHEAD_FIRST, WARHEAD_COUNT, &(Warhead_Type_To_String));

    return 1;
}

static int Lua_Get_Bullet_Types(lua_State* lua)
{
    Read_Object_Names(lua, BULLET_SNIPER, BULLET_COUNT, &(Bullet_Type_To_String));

    return 1;
}

static int Lua_Get_Weapon_Types(lua_State* lua)
{
    Read_Object_Names(lua, WEAPON_FIRST, WEAPON_COUNT, &(Weapon_Type_To_String));

    return 1;
}

static int Lua_Get_Event_Names(lua_State* lua)
{
    Push_String_Array_Onto_Lua_Stack(LUA_EVENTS, LUA_EVENT_COUNT);

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
