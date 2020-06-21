#include "function.h"

#include "lua.hpp"

static lua_State* LUA = NULL;

void Initialise_Lua()
{
    if (LUA != NULL)
    {
        Log_Warn("Attempted to initialise %s more than once, ignoring", LUA_VERSION);
        return;
    }

    Log_Info("Attempting to initialise %s", LUA_VERSION);

    LUA = luaL_newstate();
    luaL_openlibs(LUA);

    Log_Info("%s Initialised", LUA_VERSION);

    // test
    luaL_dostring(LUA, "return 55");
    auto returnValue = lua_tonumber(LUA, -1);

    Log_Info("Lua test returned: %f", returnValue);
}
