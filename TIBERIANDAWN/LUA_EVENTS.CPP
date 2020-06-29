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

const int LUA_EVENT_COUNT = 1;

const LuaEvent ON_SCENARIO_START_EVENT = "ScenarioStart";

static const LuaEvent* LUA_EVENTS = new LuaEvent[LUA_EVENT_COUNT]
{
    ON_SCENARIO_START_EVENT
};

static bool EVENTS_INITIALISED = false;

static void Fire_Lua_Event(LuaEvent name, const char* argumentFormat, ...)
{
    if (!Lua_Is_Enabled())
    {
        return;
    }

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

    delete scriptBuffer;
}

boolean Initialise_Events()
{
    if (EVENTS_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua events more than once, ignoring");
        return true;
    }

    auto executionOK = true;
    auto initEventScriptBuffer = Allocate_String(1024);

    for (auto i = 0; i < LUA_EVENT_COUNT; i++)
    {
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
