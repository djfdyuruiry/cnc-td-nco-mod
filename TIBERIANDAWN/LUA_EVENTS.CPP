#include "function.h"

typedef const char* LuaEvent;

static auto INIT_EVENT_FORMAT = "\
  _G.Game.%1$sEventHandlers = {}                \n\
                                                \
  _G.on%1$s = function (h)                      \n\
    table.insert(Game.%1$sEventHandlers, h)     \n\
  end                                           \
";

static auto FIRE_EVENT_FORMAT = "\
  for _, h in ipairs(Game.%sEventHandlers) do   \n\
    local ok, err = pcall(h, %s)                \n\
                                                \
    if not ok then                              \n\
      showError(err)                            \n\
    end                                         \n\
  end                                           \
";

static const LuaEvent ON_SCENARIO_START_EVENT = "ScenarioStart";
static const LuaEvent ON_SAVE_LOAD_EVENT = "SaveLoad";
static const LuaEvent ON_GAME_TICK_EVENT = "GameTick";

static const auto LUA_EVENT_COUNT = 3;

static const LuaEvent* LUA_EVENTS = new LuaEvent[LUA_EVENT_COUNT]
{
    ON_SCENARIO_START_EVENT,
    ON_SAVE_LOAD_EVENT,
    ON_GAME_TICK_EVENT
};

static bool EVENTS_INITIALISED = false;
static long long LAST_TICK_TIME_IN_EPOCH_MILLIS = 0;

static void Fire_Lua_Event(LuaEvent name, const char* argumentFormat = "nil", ...)
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

    auto executionOK = true;
    auto initEventScriptBuffer = Allocate_String(1024);

    for (auto i = 0; i < LUA_EVENT_COUNT; i++)
    {
        Log_Trace("Initialising Lua event: %s", LUA_EVENTS[i]);

        _sprintf_p(
            initEventScriptBuffer,
            1024,
            INIT_EVENT_FORMAT,
            LUA_EVENTS[i]
        );

        if (!Execute_Lua_Script(initEventScriptBuffer))
        {
            executionOK = false;
        }
    }

    delete initEventScriptBuffer;

    EVENTS_INITIALISED = executionOK;

    return executionOK;
};

void On_Scenario_Load(char* name)
{
    Fire_Lua_Event(
        ON_SCENARIO_START_EVENT,
        "'%s'",
        name
    );
}

void On_Save_Load(HousesType playerHouse, unsigned int scenarioNumber)
{
    Fire_Lua_Event(
        ON_SAVE_LOAD_EVENT,
        "'%s', %u",
        House_Type_To_String(playerHouse),
        scenarioNumber
    );
}

void On_Game_Tick()
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
