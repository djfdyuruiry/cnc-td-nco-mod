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

    Log_Debug("Lua game state intialised");

    if (!Initialise_Lua_Api())
    {
        return false;
    }

    Log_Debug("Lua API intialised");

    if (!Initialise_Events())
    {
        return false;
    }

    Log_Debug("Lua events system initialised");

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
    Log_Debug("Registering load C function: %s", name);

    lua_pushcfunction(LUA, function);
    lua_setglobal(LUA, name);
}

bool Execute_Lua_Script(char* luaScript)
{
    auto executionOK = !luaL_dostring(LUA, luaScript);

    Log_Debug("Executing Lua snippet: \n%s\n", luaScript);
    
    if (!executionOK)
    {
        Show_Error("Error executing Lua snippet: \n<<\n%s\n>>\nCheck the syntax is correct", luaScript);
    }

    return executionOK;
}

bool Execute_Lua_File(char* scriptFile)
{
    Log_Info("Attempting to load Lua script: %s", scriptFile);

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

const char * Get_Lua_Value_As_String(unsigned int index)
{
    if (lua_isboolean(LUA, index))
    {
        return Convert_Boolean_To_String(lua_toboolean(LUA, index));
    }

    return lua_tostring(LUA, index);
}
