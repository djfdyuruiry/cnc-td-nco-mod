#include <LuaStateFactory.h>
#include <LuaStateWrapper.h>
#include <LuaRuntime.h>

#include "lua.h"
#include "lua_api.h"
#include "lua_events.h"
#include "rules_ini.h"

static lua_State* LUA = NULL;
static ILuaStateWrapper* LUA_WRAPPER = NULL;
static ILuaRuntime* LUA_RUNTIME = NULL;

static bool Load_Lua_Scripts()
{
    auto executionOk = true;
    auto& scripts = Rules_Get_Lua_Scripts();

    for (auto scriptFile : scripts) {
        auto scriptFilePath = Build_Mod_Data_File_Path(scriptFile);

        if (!Execute_Lua_File(scriptFilePath))
        {
            executionOk = false;
        }
    }

    return executionOk;
}

static bool Load_Lua_Components()
{
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

bool Initialise_Lua()
{
    if (LUA != NULL)
    {
        Log_Warn("Attempted to initialise %s more than once, ignoring", LUA_VERSION);
        return true;
    }

    Log_Info("Attempting to initialise %s", LUA_VERSION);

    LUA = LuaStateFactory::Build(Get_Mod_Data_Path());
    LUA_WRAPPER = &(LuaStateWrapper::Build(LUA));
    LUA_RUNTIME = &(LuaRuntime::Build(*LUA_WRAPPER));

    Log_Info("%s Initialised", LUA_VERSION);

    return Load_Lua_Components() && Load_Lua_Scripts();
}

bool Execute_Lua_Script(const char* luaScript, bool beQuiet)
{
    if (!beQuiet)
    {
        Log_Trace("Executing Lua snippet: \n%s\n", luaScript);
    }

    auto& result = LUA_WRAPPER->ExecuteScript(luaScript);
    auto executionOK = !result.IsErrorResult();

    if (!executionOK)
    {
        auto errorMessageFormat = "Error executing Lua snippet: \n<<\n%s\n>>\n\n%s";

        if (!beQuiet)
        {
            Show_Error(errorMessageFormat, luaScript, result.GetError());
        }
        else
        {
            Log_Error(errorMessageFormat, luaScript, result.GetError());
        }
    }

    delete& result;

    return executionOK;
}

bool Execute_Lua_Script(char* luaScript)
{
    return Execute_Lua_Script(luaScript, false);
}

bool Execute_Lua_Script(const char* luaScript)
{
    return Execute_Lua_Script(luaScript, false);
}

bool Execute_Lua_File(const char* scriptFile)
{
    Log_Info("Attempting to load Lua script: %s", scriptFile);

    auto& result = LUA_RUNTIME->ExecuteFile(scriptFile);
    auto executionOK = !result.IsErrorResult();

    if (executionOK)
    {
        Log_Info("Loaded Lua script: %s", scriptFile);
    }
    else
    {
        Show_Error("Error loading Lua script: %s\n\n%s", scriptFile, result.GetError());
    }

    delete &result;

    return executionOK;
}

bool Execute_Lua_File(char* scriptFile)
{
    return Execute_Lua_File((const char*)scriptFile);
}

void Shutdown_Lua()
{
    if (LUA_RUNTIME == NULL)
    {
        return;
    }

    Log_Info("Shutting down %s instance", LUA_VERSION);

    delete LUA_RUNTIME;
}

ILuaRuntime& LuaRuntime()
{
    if (LUA_RUNTIME == NULL)
    {
        Initialise_Lua();
    }

    return *LUA_RUNTIME;
}
