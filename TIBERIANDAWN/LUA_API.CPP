#include "function.h"

static bool API_INITIALISED = false;

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
        auto logMessage = Get_Lua_Value_As_String(1);

        Log_Info("Lua => %s", logMessage);
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
