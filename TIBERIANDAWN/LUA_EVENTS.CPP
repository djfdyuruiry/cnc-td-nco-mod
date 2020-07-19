#include <queue>

#include "game_messages.h"
#include "lua_events.h"
#include "rules_ini.h"
#include "strings.h"
#include "utils.h"

using namespace std;

static const auto FIRE_EVENT_FORMAT = "Game.Events.fire('%s', %s)";

static bool EVENTS_INITIALISED = false;
static long long LAST_TICK_TIME_IN_EPOCH_MILLIS = 0;

static queue<GameLoopMessage> GAME_LOOP_MESSAGE_QUEUE;

static void Fire_Lua_Event(const char* name, const char* argumentFormat = "nil", ...)
{
    if (!Lua_Is_Enabled())
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

    if (!Execute_Lua_Script(scriptBuffer))
    {
        Show_Error("Error firing Lua event: %s", name);
    }
    else
    {
        Log_Trace("Lua event fired ok: %s", name);
    }

    delete scriptBuffer;
}

boolean Initialise_Events()
{
    if (EVENTS_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua events more than once, ignoring");
        return true;
    }

    Log_Trace("Initialising Lua events");

    return Execute_Lua_Script("Game.Events.initHandlers()");
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
static void On_Game_Tick()
{
    auto nowInEpochMillis = Get_Now_In_Epoch_Millis();
    auto msSinceLastTick = nowInEpochMillis - LAST_TICK_TIME_IN_EPOCH_MILLIS;

    if (msSinceLastTick < Rules_Get_Game_Tick_Interval_In_Ms())
    {
        // fire game tick at most every GAME_TICK_INTERVAL_IN_MS millis
        return;
    }

    Fire_Lua_Event(ON_GAME_TICK_EVENT);

    LAST_TICK_TIME_IN_EPOCH_MILLIS = nowInEpochMillis;
}

DWORD WINAPI Process_Game_Events(LPVOID lpParam)
{
    while (true)
    {
        while (!GAME_LOOP_MESSAGE_QUEUE.empty())
        {
            auto loopMessage = GAME_LOOP_MESSAGE_QUEUE.front();
            auto type = loopMessage.type;

            if (type == GAME_TICK_ELAPSED)
            {
                On_Game_Tick();
            }
            else
            {
                Log_Error("Encountered Unknown Game Loop message type : %u", type);
            }

            GAME_LOOP_MESSAGE_QUEUE.pop();
        }

        Sleep(10);
    }
}

void Push_Game_Loop_Message(GameLoopMessageType messageType)
{
    GameLoopMessage loopMessage;

    loopMessage.type = messageType;

    GAME_LOOP_MESSAGE_QUEUE.push(loopMessage);
}

bool Game_Loop_Messages_Pending()
{
    return !GAME_LOOP_MESSAGE_QUEUE.empty();
}
