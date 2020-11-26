#include <Logger.h>
#include <strings.h>
#include <utils.h>

#include "game_messages.h"
#include "lua_events.h"
#include "TiberianDawnNcoRuntime.h"

using namespace std;

static const auto FIRE_EVENT_FORMAT = "Game.Events.fire('%s', %s)";
static const auto MAX_EVENT_ARGS_LENGTH = 1024;
static const auto LUA_EVENT_COUNT = 3u;

static bool EVENTS_INITIALISED = false;
static const char** LUA_EVENTS = NULL;

static void Fire_Lua_Event(const char* name, const char* argumentFormat = "nil", ...)
{
    if (!NcoRulesRuntime().LuaIsEnabled())
    {
        LogTrace("Lua is disabled, skipping firing event: %s", name);
        return;
    }

    LogTrace("Firing Lua event: %s", name);

    va_list formatArgs;
    va_start(formatArgs, argumentFormat);

    auto argumentsBuffer = FormatString(argumentFormat, MAX_EVENT_ARGS_LENGTH, formatArgs);

    va_end(formatArgs);

    auto scriptBuffer = FormatString(FIRE_EVENT_FORMAT, MAX_EVENT_ARGS_LENGTH * 2, name, argumentsBuffer);

    delete argumentsBuffer;

    auto& executeResult = NcoLuaRuntime().ExecuteScript(scriptBuffer);

    if (executeResult.IsErrorResult())
    {
        ShowError("Error firing Lua event: %s\n%s", name, executeResult.GetError());
    }
    else
    {
        LogTrace("Lua event fired ok: %s", name);
    }

    delete& executeResult;
    delete scriptBuffer;
}

bool InitialiseEvents()
{
    if (EVENTS_INITIALISED)
    {
        LogWarn("Attempted to initialise Lua events more than once, ignoring");
        return true;
    }

    LogTrace("Initialising Lua events");

    auto& executeResult = NcoLuaRuntime().ExecuteScript("Game.Events.initHandlers()");
    auto error = executeResult.IsErrorResult();

    if (error)
    {
        ShowError("Error initialising event handlers: %s", executeResult.GetError());
    }

    delete& executeResult;

    EVENTS_INITIALISED = true;

    return !error;
};

int GetEventCount()
{
    return LUA_EVENT_COUNT;
}

const char** GetEventNames()
{
    if (LUA_EVENTS == NULL)
    {
        LUA_EVENTS = new const char* [LUA_EVENT_COUNT]
        {
            ON_SCENARIO_START_EVENT,
                ON_SAVE_LOAD_EVENT,
                ON_GAME_TICK_EVENT
        };
    }

    return LUA_EVENTS;
}

// sync messages
void OnScenarioLoad(char* name)
{
    Fire_Lua_Event(
        ON_SCENARIO_START_EVENT,
        "'%s'",
        name
    );
}

void OnSaveLoad(const char* playerHouse, unsigned int scenarioNumber)
{
    Fire_Lua_Event(
        ON_SAVE_LOAD_EVENT,
        "'%s', %u",
        playerHouse,
        scenarioNumber
    );
}

// async messages
void OnGameTick()
{
    Fire_Lua_Event(ON_GAME_TICK_EVENT);
}
