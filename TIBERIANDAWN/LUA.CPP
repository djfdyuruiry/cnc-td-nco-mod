#include "function.h"

static char * INIT_GAME_STATE = "_G.Game = {}";

static lua_State* LUA = NULL;

static bool Load_Lua_Scripts()
{
    auto executionOK = true;
        
    if (!Execute_Lua_Script(INIT_GAME_STATE))
    {
        return false;
    }

    if (!Initialise_Lua_Api())
    {
        return false;
    }
    
    if (!Initialise_Events())
    {
        return false;
    }

    auto scripts = Rules_Get_Lua_Scripts();

    for (auto i = 0; i < scripts.ScriptFileCount; i++)
    {
        auto scriptFile = scripts.ScriptFiles[i];

        if (!Execute_Lua_File(scriptFile))
        {
            executionOK = false;
        }
    }

    return executionOK;
}

bool Initialise_Lua()
{
    if (LUA != NULL)
    {
        Log_Warn("Attempted to initialise %s more than once, ignoring", LUA_VERSION);
        return true;
    }

    Log_Info("Attempting to initialise %s", LUA_VERSION);

    LUA = luaL_newstate();
    luaL_openlibs(LUA);

    Log_Info("%s Initialised", LUA_VERSION);

    return Load_Lua_Scripts();
}

void Register_Lua_Function(const char* name, lua_CFunction function)
{
    lua_pushcfunction(LUA, function);
    lua_setglobal(LUA, name);
}

bool Execute_Lua_Script(char* luaScript)
{
    auto executionOK = !luaL_dostring(LUA, luaScript);

    Log_Debug("Executing Lua snippet: `%s`", luaScript);
    
    if (!executionOK)
    {
        Show_Error("Error executing Lua snippet: `%s`\nCheck the syntax is correct", luaScript);
    }

    return executionOK;
}

bool Execute_Lua_File(char* scriptFile)
{
    Log_Info("Attempting to loadLua script: %s", scriptFile);

    auto executionOK = !luaL_dofile(LUA, scriptFile);

    if (executionOK)
    {
        Log_Info("Loaded Lua script: %s", scriptFile);
    }
    else
    {
        Show_Error("Error loading Lua script: %s\nCheck the file is present and the syntax is correct", scriptFile);
    }

    return executionOK;
}

struct LuaString Read_Lua_String(int index)
{
    LuaString luaString = {
        lua_rawlen(LUA, index),
        lua_tostring(LUA, index)
    };

    return luaString;
}
