#ifndef LUA_EVENTS_H
#define LUA_EVENTS_H

boolean Initialise_Events();

void On_Scenario_Load(char* name);

void On_Save_Load(HousesType playerHouse, unsigned int scenarioNumber);

void On_Game_Tick();

#endif
