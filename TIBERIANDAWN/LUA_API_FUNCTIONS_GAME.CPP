#include "function.h"

#include "DLLInterface.h"

static int Lua_Show_Game_Message(lua_State* lua)
{
    Log_Trace("Lua_Set_Building_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "showGameMessage requires exactly two arguments");
        return 0;
    }

    if (!lua_isnumber(lua, 2))
    {
        luaL_error(lua, "showGameMessage parmeter `durationInSeconds` must be a number");
        return 0;
    }

    auto message = Get_Lua_Value_As_String(1);
    auto durationInSeconds = luaL_checknumber(lua, 2);

    if (message == NULL)
    {
        luaL_error(lua, "showGameMessage parmeter `message` was nil");
        return 0;
    }

    if (durationInSeconds < 0.1)
    {
        luaL_error(lua, "showGameMessage parmeter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");
        return 0;
    }

    On_Message(message, durationInSeconds);

    return 0;
}

bool Register_Game_Functions()
{
    Log_Debug("Initialising Lua API game functions");

    Register_Lua_Function("showGameMessage", Lua_Show_Game_Message);

    return true;
}

