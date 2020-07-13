#include "function.h"

#include "DLLInterface.h"

GameUiMessage GAME_UI_MESSAGE;

void Process_Game_Ui_Message()
{
	if (!GAME_UI_MESSAGE.unread)
	{
		return;
	}

	On_Message(GAME_UI_MESSAGE.message, GAME_UI_MESSAGE.durationInSeconds);

	delete GAME_UI_MESSAGE.message;
	GAME_UI_MESSAGE.unread = false;
}
