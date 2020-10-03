#include <logger.h>
#include "lua.h"
#include "rules_ini.h"
#include <strings.h>
#include <utils.h>

static int Lua_Build_Mod_Data_File_Path(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "buildModDataFilePath requires exactly one argument");
        return 0;
    }

    auto filePath = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(filePath))
    {
        luaL_error(lua, "buildModDataFilePath parameter `filePath` was nil or blank");
        return 0;
    }

    lua_pushstring(lua, Build_Mod_Data_File_Path(filePath));

    return 1;
}

static int Lua_Get_Mod_Data_Path(lua_State* lua)
{
    lua_pushstring(lua, Get_Mod_Data_Path());

    return 1;
}

static int Lua_Get_Now_In_Epoch_Millis(lua_State* lua)
{
    lua_pushnumber(lua, Get_Now_In_Epoch_Millis());

    return 1;
}

static int Lua_Toggle_Console_Logging(lua_State* lua)
{
    Toggle_Console_Logging();

    return 0;
}

static int Lua_Set_Log_Level(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "setLogLevel requires exactly one argument");
        return 0;
    }

    auto logLevelStr = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(logLevelStr))
    {
        luaL_error(lua, "setLogLevel parameter `logLevel` was nil or blank");
        return 0;
    }

    auto logLevelUpper = Convert_String_To_Upper_Case(logLevelStr);

    auto logLevel = Parse_Log_Level(logLevelUpper);

    delete logLevelUpper;

    Set_Current_Log_Level(logLevel);

    return 0;
}

static int Lua_Get_Log_Level(lua_State* lua)
{
    auto logLevel = Log_Level_To_String(Current_Log_Level());

    lua_pushstring(lua, logLevel);

    return 1;
}

static int Lua_Show_Error(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "showError requires exactly one argument");
        return 0;
    }

    auto message = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(message))
    {
        luaL_error(lua, "showError parameter `message` was nil or blank");
        return 0;
    }

    Show_Error(message);

    return 0;
}

static int Lua_Print(lua_State* lua)
{
    int argCount = lua_gettop(lua);

    if (argCount > 0)
    {
        auto logMessage = Get_Lua_Value_As_String(1);

        puts(logMessage);
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

bool Register_Util_Functions()
{
    Log_Debug("Initialising Lua API utility functions");

    Register_Lua_Function("logString", Lua_Log);
    Register_Lua_Function("printString", Lua_Print);
    Register_Lua_Function("showErrorString", Lua_Show_Error);
    
    Register_Lua_Function("getLogLevel", Lua_Get_Log_Level);
    Register_Lua_Function("setLogLevel", Lua_Set_Log_Level);
    Register_Lua_Function("toggleConsoleLog", Lua_Toggle_Console_Logging);

    Register_Lua_Function("getNowInEpochMillis", Lua_Get_Now_In_Epoch_Millis);

    Register_Lua_Function("getModDataPath", Lua_Get_Mod_Data_Path);
    Register_Lua_Function("buildModDataFilePath", Lua_Build_Mod_Data_File_Path);

    return true;
}
