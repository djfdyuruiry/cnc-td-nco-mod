#include "function.h"

static bool API_INITIALISED = false;

static void Load_Api_Functions()
{
    Log_Debug("Initialising Lua API functions");

    Register_Lua_Function("getGameRule", Lua_Get_Game_Rule);
    Register_Lua_Function("setGameRule", Lua_Set_Game_Rule);

    Register_Lua_Function("getInfantryTypes", Lua_Get_Infantry_Types);
    Register_Lua_Function("getUnitTypes", Lua_Get_Unit_Types);
    Register_Lua_Function("getAircraftTypes", Lua_Get_Aircraft_Types);
    Register_Lua_Function("getBuildingTypes", Lua_Get_Building_Types);

    Register_Lua_Function("getInfantryRuleNames", Lua_Get_Infantry_Rule_Names);
    Register_Lua_Function("getUnitRuleNames", Lua_Get_Unit_Rule_Names);
    Register_Lua_Function("getAircraftRuleNames", Lua_Get_Aircraft_Rule_Names);
    Register_Lua_Function("getBuildingRuleNames", Lua_Get_Building_Rule_Names);

    Register_Lua_Function("getInfantryRule", Lua_Get_Infantry_Rule);
    Register_Lua_Function("setInfantryRule", Lua_Set_Infantry_Rule);

    Register_Lua_Function("getUnitRule", Lua_Get_Unit_Rule);
    Register_Lua_Function("setUnitRule", Lua_Set_Unit_Rule);

    Register_Lua_Function("getAircraftRule", Lua_Get_Aircraft_Rule);
    Register_Lua_Function("setAircraftRule", Lua_Set_Aircraft_Rule);

    Register_Lua_Function("getBuildingRule", Lua_Get_Building_Rule);
    Register_Lua_Function("setBuildingRule", Lua_Set_Building_Rule);
}

static int Lua_Set_Log_Level(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logLevel = Parse_Log_Level(Get_Lua_Value_As_String(1));

        Set_Current_Log_Level(logLevel);
    }

    return 0;
}

static int Lua_Show_Error(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logMessage = Get_Lua_Value_As_String(1);

        Show_Error(logMessage);
    }

    return 0;
}

static int Lua_Log(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logMessage = Get_Lua_Value_As_String(1);

        Log_Info("Lua => %s", logMessage);
    }

    return 0;
}

static void Load_Utility_Functions()
{
    Log_Debug("Initialising Lua API utility functions");

    Register_Lua_Function("log", Lua_Log);
    Register_Lua_Function("print", Lua_Log);
    Register_Lua_Function("showError", Lua_Show_Error);
    Register_Lua_Function("setLogLevel", Lua_Set_Log_Level);
}

bool Initialise_Lua_Api()
{
    if (API_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua API more than once, ignoring");
        return API_INITIALISED;
    }

    Log_Info("Initialising Lua API");

    Load_Utility_Functions();
    Load_Api_Functions();

    API_INITIALISED = true;

    return API_INITIALISED;
}
