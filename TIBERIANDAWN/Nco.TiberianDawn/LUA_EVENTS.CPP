#include <Logger.h>
#include <strings.h>
#include <utils.h>

#include "game_messages.h"
#include "lua_events.h"
#include "TiberianDawnNcoRuntime.h"

static auto FIRE_EVENT_FORMAT = "Nco.Events.fire('%s', %s)";
static const auto MAX_EVENT_ARGS_LENGTH = 1024;
static const auto LUA_EVENT_COUNT = 3u;

static auto LUA_EVENTS = new const char* [LUA_EVENT_COUNT]
{
    ON_SCENARIO_START_EVENT,
    ON_SAVE_LOAD_EVENT,
    ON_GAME_TICK_EVENT
};

static void FireLuaEvent(const char* name, const char* argumentFormat = "nil", ...)
{
    if (!NcoRulesRuntime().LuaIsEnabled())
    {
        LogTrace("Lua is disabled, ignoring event: %s", name);
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

    delete scriptBuffer;

    if (executeResult.IsErrorResult())
    {
        ShowError("Error firing Lua event: %s\n%s", name, executeResult.GetError());
    }
    else
    {
        LogTrace("Lua event fired ok: %s", name);
    }

    delete &executeResult;
}

int GetEventCount()
{
    return LUA_EVENT_COUNT;
}

const char** GetEventNames()
{
    return LUA_EVENTS;
}

// sync messages
void OnScenarioLoad(char* name)
{
    FireLuaEvent(
        ON_SCENARIO_START_EVENT,
        "'%s'",
        name
    );
}

void OnSaveLoad(const char* playerHouse, unsigned int scenarioNumber)
{
    FireLuaEvent(
        ON_SAVE_LOAD_EVENT,
        "'%s', %u",
        playerHouse,
        scenarioNumber
    );
}

// async messages
void OnGameTick()
{
    FireLuaEvent(ON_GAME_TICK_EVENT);
}
