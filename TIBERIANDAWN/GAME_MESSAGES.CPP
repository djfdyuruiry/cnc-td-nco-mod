#include "function.h"

#include "DLLInterface.h"
#include "SidebarGlyphx.h"

#include "game_utils.h"

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

bool Push_Refresh_Map_Ui_Message()
{
	if (GAME_UI_MESSAGE.unread)
	{
		return false;
	}

	GAME_UI_MESSAGE.type = REFRESH_MAP_MESSAGE;
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
		Show_Game_Notification(GAME_UI_MESSAGE.message, GAME_UI_MESSAGE.durationInSeconds);

		delete GAME_UI_MESSAGE.message;
	}
	else if (type == REFRESH_SIDEBAR_MESSAGE)
	{
		Refresh_Sidebar();
	}
	else if (type == REFRESH_MAP_MESSAGE)
	{
		Refresh_Game_Map();
	}

	GAME_UI_MESSAGE.unread = false;
}
