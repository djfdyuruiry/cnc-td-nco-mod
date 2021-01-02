#include "../FUNCTION.H"
#include "../DLLInterface.h"

void ShowGameNotification(const char* message, float durationInSeconds)
{
	On_Message(message, durationInSeconds);
}

void RefreshGameMap()
{
	Map.Flag_To_Redraw(true);
}

void RefreshSidebar()
{
	for (auto index = 0; index < Buildings.Count(); index++)
	{
		Buildings.Ptr(index)->Update_Buildables();
	}

	RefreshGameMap();
}

void RevealEntireMapToPlayer()
{
	for (auto c = 0; c < MAP_CELL_TOTAL; c++)
	{
		Map.Map_Cell(c, PlayerPtr, true);
	}

	RefreshGameMap();
}

void HideEntireMapFromPlayer()
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

		if (object != NULL && object->Is_Techno() && ((TechnoClass*)object)->House == PlayerPtr)
		{
			object->Look();
		}
	}

	RefreshGameMap();
}

bool ProcessGameUiMessage(GameUiMessage& uiMessage)
{
	auto type = uiMessage.type;

	if (type == SHOW_NOTIFICATION_MESSAGE && uiMessage.message != NULL)
	{
		ShowGameNotification(uiMessage.message, uiMessage.durationInSeconds);

		delete uiMessage.message;
	}
	else if (type == REFRESH_SIDEBAR_MESSAGE)
	{
		RefreshSidebar();
	}
	else if (type == REFRESH_MAP_MESSAGE)
	{
		RefreshGameMap();
	}
	else if (type == REVEAL_ENTIRE_MAP_MESSAGE)
	{
		RevealEntireMapToPlayer();
	}
	else if (type == HIDE_ENTIRE_MAP_MESSAGE)
	{
		HideEntireMapFromPlayer();
	}
	else
	{
		return false;
	}

	return true;
}
