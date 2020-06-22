#include "function.h"

#include "lua.hpp"

static lua_State* LUA = NULL;

static int Lua_Log(lua_State* _)
{
    int argCount = lua_gettop(LUA);

    if (argCount > 0)
    {
        auto logMessage = lua_tostring(LUA, 1);

        Log_Info("Lua => %s", logMessage);
    }

    return 0;
}

static int Lua_Show_Error(lua_State * _)
{
    int argCount = lua_gettop(LUA);

    if (argCount > 0)
    {
        auto logMessage = lua_tostring(LUA, 1);

        Show_Error(logMessage);
    }

    return 0;
}

static void Load_Lua_Scripts()
{
    auto scripts = Rules_Get_Lua_Scripts();

    // init global game state
    Execute_Lua_Script("\
      _G.Game =                           \
      {                                   \
        ScenarioStartEventHandlers = {}   \
      }                                   \
    ");

    // wire up utility native functions
    lua_pushcfunction(LUA, Lua_Show_Error);
    lua_setglobal(LUA, "showError");

    lua_pushcfunction(LUA, Lua_Log);
    lua_setglobal(LUA, "log");

    // declare event handler functions
    Execute_Lua_Script("\
      _G.onScenarioStart = function (h)      \n\
        table.insert(                        \
          Game.ScenarioStartEventHandlers,   \
          h                                  \
        )                                    \n\
      end                                    \
    ");

    for (auto i = 0; i < scripts.ScriptFileCount; i++)
    {
        auto scriptFile = scripts.ScriptFiles[i];

        Log_Info("Attempting to loadLua script: %s", scriptFile);

        auto scriptError = luaL_dofile(LUA, scriptFile);

        // todo store LUA_HAS_ERRORS somewhere and exit if this is true in startup
        if (scriptError)
        {
            Show_Error("Error loading Lua script: %s\nCheck the file is present and the syntax is correct", scriptFile);
            continue;
        }

        Log_Info("Loaded Lua script: %s", scriptFile);
    }
}

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

    Load_Lua_Scripts();
}

void Execute_Lua_Script(char* luaScript)
{
    auto scriptError = luaL_dostring(LUA, luaScript);

    if (scriptError)
    {
        Show_Error("Error executing Lua snippet: `%s`", luaScript);
    }
}
