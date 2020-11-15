#pragma once

const auto ON_SCENARIO_START_EVENT = "ScenarioStart";
const auto ON_SAVE_LOAD_EVENT = "SaveLoad";
const auto ON_GAME_TICK_EVENT = "GameTick";

bool Initialise_Events();

int Get_Event_Count();

const char** Get_Event_Names();

// sync events
void On_Scenario_Load(char* name);

void On_Save_Load(const char* playerHouse, unsigned int scenarioNumber);

// async events
void On_Game_Tick();
