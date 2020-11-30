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
		ProcessGameLoopMessages();

		return 0;
	}

public:
	static GameMessagesThread& Build()
	{
		return *(new GameMessagesThread());
	}

};
