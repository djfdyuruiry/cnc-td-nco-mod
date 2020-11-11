#include <logger.h>
#include <strings.h>
#include <utils.h>

#include "game_messages.h"
#include "lua_events.h"
#include "TiberianDawnNcoRuntime.h"

using namespace std;

static const auto FIRE_EVENT_FORMAT = "Game.Events.fire('%s', %s)";

static bool EVENTS_INITIALISED = false;

static void Fire_Lua_Event(const char* name, const char* argumentFormat = "nil", ...)
{
    if (!TiberianDawnNcoRuntime::GetInstance().GetRulesRuntime().LuaIsEnabled())
    {
        Log_Trace("Lua is disabled, skipping firing event: %s", name);
        return;
    }

    Log_Trace("Firing Lua event: %s", name);

    auto argumentsBuffer = Allocate_String(1024);

    va_list formatArgs;
    va_start(formatArgs, argumentFormat);

    vsnprintf(argumentsBuffer, 1024, argumentFormat, formatArgs);

    va_end(formatArgs);

    auto scriptBuffer = Allocate_String(2048);

    sprintf(scriptBuffer, FIRE_EVENT_FORMAT, name, argumentsBuffer);

    delete argumentsBuffer;

    auto& executeResult = TiberianDawnNcoRuntime::GetInstance().GetLuaRuntime().ExecuteScript(scriptBuffer);

    if (executeResult.IsErrorResult())
    {
        Show_Error("Error firing Lua event: %s\n%s", name, executeResult.GetError());
    }
    else
    {
        Log_Trace("Lua event fired ok: %s", name);
    }

    delete &executeResult;
    delete scriptBuffer;
}

bool Initialise_Events()
{
    if (EVENTS_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua events more than once, ignoring");
        return true;
    }

    Log_Trace("Initialising Lua events");


    auto& executeResult = TiberianDawnNcoRuntime::GetInstance().GetLuaRuntime().ExecuteScript("Game.Events.initHandlers()");
    auto error = executeResult.IsErrorResult();
    
    if (error)
    {
        Show_Error("Error initialising event handlers: %s", executeResult.GetError());
    }

    delete &executeResult;

    return !error;
};

// sync messages
void On_Scenario_Load(char* name)
{
    Fire_Lua_Event(
        ON_SCENARIO_START_EVENT,
        "'%s'",
        name
    );
}

void On_Save_Load(const char* playerHouse, unsigned int scenarioNumber)
{
    Fire_Lua_Event(
        ON_SAVE_LOAD_EVENT,
        "'%s', %u",
        playerHouse,
        scenarioNumber
    );
}

// async messages
void On_Game_Tick()
{
    Fire_Lua_Event(ON_GAME_TICK_EVENT);
}
