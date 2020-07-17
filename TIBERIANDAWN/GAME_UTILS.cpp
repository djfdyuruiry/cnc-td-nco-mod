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

void Reveal_Entire_Map_To_Player()
{
	for (auto c = 0; c < MAP_CELL_TOTAL; c++) {
		Map.Map_Cell(c, PlayerPtr, true);
	}

	Refresh_Game_Map();
}

void Hide_Entire_Map_From_Player()
{
	for (auto c = 0; c < MAP_CELL_TOTAL; c++)
	{
		auto cellptr = &Map[c];

		if (cellptr->Is_Mapped(PlayerPtr) || cellptr->Is_Visible(PlayerPtr))
		{
			cellptr->Redraw_Objects();
			cellptr->Set_Mapped(PlayerPtr, false);
			cellptr->Set_Visible(PlayerPtr, false);
		}
	}

	for (auto i = 0; i < Map.Layer[LAYER_GROUND].Count(); i++)
	{
		ObjectClass* object = Map.Layer[LAYER_GROUND][i];

		if (object && object->Is_Techno() && ((TechnoClass*)object)->House == PlayerPtr)
		{
			object->Look();
		}
	}

	Refresh_Game_Map();
}
