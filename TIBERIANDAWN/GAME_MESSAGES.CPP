#include "function.h"

#include "DLLInterface.h"
#include "SidebarGlyphx.h"

GameUiMessage GAME_UI_MESSAGE;

bool Push_Show_Notification_Ui_Message(const char* message, float durationInSeconds)
{
	if (GAME_UI_MESSAGE.unread)
	{
		return false;
	}

	GAME_UI_MESSAGE.type = SHOW_NOTIFICATION_MESSAGE;
	GAME_UI_MESSAGE.message = strdup(message);
	GAME_UI_MESSAGE.durationInSeconds = durationInSeconds;

	GAME_UI_MESSAGE.unread = true;

	return true;
}

bool Push_Refresh_Sidebar_Ui_Message()
{
	if (GAME_UI_MESSAGE.unread)
	{
		return false;
	}

	GAME_UI_MESSAGE.type = REFRESH_SIDEBAR_MESSAGE;

	GAME_UI_MESSAGE.unread = true;

	return true;
}

void Process_Game_Ui_Message()
{
	if (!GAME_UI_MESSAGE.unread)
	{
		return;
	}

	auto type = GAME_UI_MESSAGE.type;

	if (type == SHOW_NOTIFICATION_MESSAGE && GAME_UI_MESSAGE.message != NULL)
	{
		On_Message(GAME_UI_MESSAGE.message, GAME_UI_MESSAGE.durationInSeconds);

		delete GAME_UI_MESSAGE.message;
	}
	else if (type == REFRESH_SIDEBAR_MESSAGE)
	{
		// TODO: figure out how to make this work????
		Map.PowerClass::IsToRedraw = true;

		Map.Flag_To_Redraw(false);
		
		Map.Column[0].Flag_To_Redraw();
		Map.Column[1].Flag_To_Redraw();
	}

	GAME_UI_MESSAGE.unread = false;
}
