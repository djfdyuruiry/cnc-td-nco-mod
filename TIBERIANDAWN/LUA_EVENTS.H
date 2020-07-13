#ifndef LUA_EVENTS_H
#define LUA_EVENTS_H

#include <windows.h>

boolean Initialise_Events();

void On_Scenario_Load(char* name);

void On_Save_Load(HousesType playerHouse, unsigned int scenarioNumber);

DWORD WINAPI Process_Game_Events(LPVOID lpParam);

void Push_Game_Loop_Message(GameLoopMessageType messageType);

#endif
