#ifndef LUA_EVENTS_H
#define LUA_EVENTS_H

#include <windows.h>

static const auto ON_SCENARIO_START_EVENT = "ScenarioStart";
static const auto ON_SAVE_LOAD_EVENT = "SaveLoad";
static const auto ON_GAME_TICK_EVENT = "GameTick";

static const auto LUA_EVENT_COUNT = 3u;

static const auto LUA_EVENTS = new const char*[LUA_EVENT_COUNT]
{
    ON_SCENARIO_START_EVENT,
    ON_SAVE_LOAD_EVENT,
    ON_GAME_TICK_EVENT
};

boolean Initialise_Events();

void On_Scenario_Load(char* name);

void On_Save_Load(HousesType playerHouse, unsigned int scenarioNumber);

DWORD WINAPI Process_Game_Events(LPVOID lpParam);

void Push_Game_Loop_Message(GameLoopMessageType messageType);

GameLoopMessage Get_Game_Loop_Message();

#endif
