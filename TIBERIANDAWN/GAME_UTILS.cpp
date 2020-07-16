#include "function.h"

#include "DLLInterface.h"

void Show_Game_Notification(char* message, float durationInSeconds)
{
	On_Message(message, durationInSeconds);
}

void Refresh_Game_Map()
{
	Map.Flag_To_Redraw(true);
}

void Refresh_Sidebar()
{
	for (auto index = 0; index < Buildings.Count(); index++) {
		Buildings.Ptr(index)->Update_Buildables();
	}

	Refresh_Game_Map();
}
