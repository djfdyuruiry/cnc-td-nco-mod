#include <map>
#include <queue>

#include "game_messages.h"
#include "game_utils.h"
#include "logger.h"

using namespace std;

static queue<GameUiMessage> GAME_UI_MESSAGE_QUEUE;

// message queuing
static void Push_Message_Of_Type(GameUiMessageType type, GameUiMessage uiMessage)
{
	uiMessage.type = type;

	GAME_UI_MESSAGE_QUEUE.push(uiMessage);
}

static void Push_Message_Of_Type(GameUiMessageType type)
{
	GameUiMessage uiMessage{};

	Push_Message_Of_Type(type, uiMessage);
}

void Push_Show_Notification_Ui_Message(const char* message, float durationInSeconds)
{
	GameUiMessage uiMessage;

	uiMessage.message = strdup(message);
	uiMessage.durationInSeconds = durationInSeconds;

	Push_Message_Of_Type(SHOW_NOTIFICATION_MESSAGE, uiMessage);
}

void Push_Refresh_Sidebar_Ui_Message()
{
	Push_Message_Of_Type(REFRESH_SIDEBAR_MESSAGE);
}

void Push_Refresh_Map_Ui_Message()
{
	Push_Message_Of_Type(REFRESH_MAP_MESSAGE);
}

void Push_Reveal_Map_Ui_Message()
{
	Push_Message_Of_Type(REVEAL_ENTIRE_MAP_MESSAGE);
}

void Push_Hide_Map_Ui_Message()
{
	Push_Message_Of_Type(HIDE_ENTIRE_MAP_MESSAGE);
}

// message handlers
static void Process_Game_Ui_Message(map<unsigned int, bool>& messageTypesProcessed)
{
	auto uiMessage = GAME_UI_MESSAGE_QUEUE.front();
	auto type = uiMessage.type;

	if (messageTypesProcessed.find(type) != messageTypesProcessed.end())
	{
		Log_Trace("Dropping duplicate UI message for game tick: %u", type);

		GAME_UI_MESSAGE_QUEUE.pop();

		return;
	}

	auto processResult = Process_Game_Ui_Message(uiMessage);
	
	if (!processResult)
	{
		Log_Error("Encountered unknown Game UI message type: %u", type);

		GAME_UI_MESSAGE_QUEUE.pop();

		return;
	}

	messageTypesProcessed[type] = true;

	GAME_UI_MESSAGE_QUEUE.pop();
}

void Process_Game_Ui_Messages()
{
	map<unsigned int, bool> messageTypesProcessed;

	while (!GAME_UI_MESSAGE_QUEUE.empty())
	{
		Process_Game_Ui_Message(messageTypesProcessed);
	}
}
