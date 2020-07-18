#include "function.h"

#include "lua_api.h"

static const auto INIT_GAME_STATE = "_G.Game = {}";

static lua_State* LUA = NULL;

static bool Load_Lua_Scripts()
{
    auto executionOk = true;
    auto scripts = Rules_Get_Lua_Scripts();

    for (auto i = 0; i < scripts.ScriptFileCount; i++)
    {
        auto scriptFile = scripts.ScriptFiles[i];
        auto scriptFilePath = Build_Mod_Data_File_Path(scriptFile);

        if (!Execute_Lua_File(scriptFilePath))
        {
            executionOk = false;
        }

        delete scriptFilePath;
    }

    return executionOk;
}

static bool Load_Lua_Components()
{        
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

    return true;
}

static bool Set_Lua_Package_Path()
{
    auto modPath = Get_Mod_Data_Path();
    auto setPackagePathScript = Allocate_String(MAX_PATH + 64);

    sprintf(setPackagePathScript, "package.path = [[%s\\lib\\?.lua;]] .. package.path", modPath);

    delete modPath;

    auto setPackagePathResult = Execute_Lua_Script(setPackagePathScript);

    if (setPackagePathResult)
    {
        Log_Debug("Lua package path updated");
    }

    delete setPackagePathScript;

    return setPackagePathResult;
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

    return Set_Lua_Package_Path()
        && Load_Lua_Components()
        && Load_Lua_Scripts();
}

void Register_Lua_Function(const char* name, lua_CFunction function)
{
    Log_Debug("Registering Lua C function: %s", name);

    lua_pushcfunction(LUA, function);
    lua_setglobal(LUA, name);
}

bool Execute_Lua_Script(char* luaScript, bool beQuiet)
{
    auto executionOK = !luaL_dostring(LUA, luaScript);

    if (!beQuiet)
    {
        Log_Trace("Executing Lua snippet: \n%s\n", luaScript);
    }

    if (!executionOK)
    {
        auto luaError = lua_tostring(LUA, -1);
        auto errorMessageFormat = "Error executing Lua snippet: \n<<\n%s\n>>\n\n%s";

        if (!beQuiet)
        {
            Show_Error(errorMessageFormat, luaScript, luaError);
        }
        else
        {
            Log_Error(errorMessageFormat, luaScript, luaError);
        }
    }

    return executionOK;
}

bool Execute_Lua_Script(const char* luaScript, bool beQuiet)
{
    auto luaScriptStr = strdup(luaScript);

    auto result = Execute_Lua_Script(luaScriptStr, beQuiet);

    delete luaScriptStr;

    return result;
}

bool Execute_Lua_Script(char* luaScript)
{
    return Execute_Lua_Script(luaScript, false);
}

bool Execute_Lua_Script(const char* luaScript)
{
    auto luaScriptStr = strdup(luaScript);

    auto result = Execute_Lua_Script(luaScriptStr, false);

    delete luaScriptStr;

    return result;
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
        auto luaError = lua_tostring(LUA, -1);
        Show_Error("Error loading Lua script: %s\n\n%s", scriptFile, luaError);
    }

    return executionOK;
}


bool Execute_Lua_File(const char* scriptFile)
{
    auto scriptFileStr = strdup(scriptFile);

    auto result = Execute_Lua_File(scriptFileStr);

    delete scriptFileStr;

    return result;
}

const char * Get_Lua_Value_As_String(int index)
{
    if (lua_isboolean(LUA, index))
    {
        return Convert_Boolean_To_String(lua_toboolean(LUA, index));
    }

    return lua_tostring(LUA, index);
}

const char* Get_Lua_Value_Type(int index)
{
    return lua_typename(LUA, lua_type(LUA, index));
}

void Clear_Lua_Stack()
{
    lua_settop(LUA, 0);
}

void Shutdown_Lua()
{
    if (LUA != NULL)
    {
        Log_Info("Shutting down %s instance", LUA_VERSION);

        lua_close(LUA);
    }
}
