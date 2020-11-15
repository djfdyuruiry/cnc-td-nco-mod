#pragma once

#include <Thread.h>

#include "game_messages.h"

class GameMessagesThread : public Thread
{
private:
	GameMessagesThread() : Thread("Game Messages")
	{
	}

protected:
	DWORD Run()
	{
		Process_Game_Loop_Messages();

		return 0;
	}

public:
	static GameMessagesThread& Build()
	{
		return *(new GameMessagesThread());
	}

};
