#include <map>
#include <queue>

#include <Logger.h>
#include <utils.h>

#include "game_messages.h"
#include "game_utils.h"
#include "lua_events.h"
#include "TiberianDawnNcoRuntime.h"

std::queue<GameUiMessage>& GAME_UI_MESSAGE_QUEUE = std::queue<GameUiMessage>();
std::queue<GameLoopMessage>& GAME_LOOP_MESSAGE_QUEUE = std::queue<GameLoopMessage>();
std::map<char, std::map<HouseMessageType, std::queue<HouseMessage>*>*>& HOUSE_MESSAGE_QUEUES
	= std::map<char, std::map<HouseMessageType, std::queue<HouseMessage>*>*>();

static auto LAST_TICK_TIME_IN_EPOCH_MILLIS = 0LL;

// message queuing
static void PushMessageOfType(GameUiMessageType type, GameUiMessage& uiMessage)
{
	uiMessage.type = type;

	GAME_UI_MESSAGE_QUEUE.push(uiMessage);
}

static void PushMessageOfType(GameUiMessageType type)
{
	auto& uiMessage = GameUiMessage{};

	PushMessageOfType(type, uiMessage);
}

void PushShowNotificationUiMessage(const char* message, float durationInSeconds)
{
	auto& uiMessage = GameUiMessage{};

	uiMessage.message = strdup(message);
	uiMessage.durationInSeconds = durationInSeconds;

	PushMessageOfType(SHOW_NOTIFICATION_MESSAGE, uiMessage);
}

void PushRefreshSidebarUiMessage()
{
	PushMessageOfType(REFRESH_SIDEBAR_MESSAGE);
}

void PushRefreshMapUiMessage()
{
	PushMessageOfType(REFRESH_MAP_MESSAGE);
}

void PushRevealMapUiMessage()
{
	PushMessageOfType(REVEAL_ENTIRE_MAP_MESSAGE);
}

void PushHideMapUiMessage()
{
	PushMessageOfType(HIDE_ENTIRE_MAP_MESSAGE);
}

// game ui message handlers

static void ProcessNextGameUiMessage(std::map<unsigned int, bool>& messageTypesProcessed)
{
	auto& uiMessage = GAME_UI_MESSAGE_QUEUE.front();
	auto type = uiMessage.type;

	if (messageTypesProcessed.find(type) != messageTypesProcessed.end())
	{
		LogTrace("Dropping duplicate UI message for game tick: %u", type);

		GAME_UI_MESSAGE_QUEUE.pop();

		return;
	}

	auto processResult = ProcessGameUiMessage(uiMessage);
	
	if (!processResult)
	{
		LogError("Encountered unknown Game UI message type: %u", type);

		GAME_UI_MESSAGE_QUEUE.pop();

		return;
	}

	messageTypesProcessed[type] = true;

	GAME_UI_MESSAGE_QUEUE.pop();
}

void ProcessGameUiMessages()
{
	auto& messageTypesProcessed = std::map<unsigned int, bool>();

	while (!GAME_UI_MESSAGE_QUEUE.empty())
	{
		ProcessNextGameUiMessage(messageTypesProcessed);
	}
}

void ClearGameUiMessages()
{
	while (!GAME_UI_MESSAGE_QUEUE.empty())
	{
		auto& uiMessage = GAME_UI_MESSAGE_QUEUE.front();

		if (uiMessage.type == SHOW_NOTIFICATION_MESSAGE && uiMessage.message != NULL)
		{
			delete uiMessage.message;
		}

		GAME_UI_MESSAGE_QUEUE.pop();
	}
}

// game loop message handlers
static void Process_Game_Loop_Message(GameLoopMessage& loopMessage)
{
	auto type = loopMessage.type;

	if (type == GAME_TICK_ELAPSED)
	{
		auto nowInEpochMillis = GetNowInEpochMillis();
		auto msSinceLastTick = nowInEpochMillis - LAST_TICK_TIME_IN_EPOCH_MILLIS;

		if (msSinceLastTick < NcoRulesRuntime().GetGameTickIntervalInMs())
		{
			// fire game tick at most every GAME_TICK_INTERVAL_IN_MS millis
			return;
		}

		OnGameTick();

		LAST_TICK_TIME_IN_EPOCH_MILLIS = nowInEpochMillis;
	}
	else
	{
		LogDebug("Encountered Unknown Game Loop message type : %u", type);
	}
}

void ProcessGameLoopMessages()
{
	while (true)
	{
		while (!GAME_LOOP_MESSAGE_QUEUE.empty())
		{
			auto& loopMessage = GAME_LOOP_MESSAGE_QUEUE.front();

			Process_Game_Loop_Message(loopMessage);

			GAME_LOOP_MESSAGE_QUEUE.pop();
		}

		Sleep(10);
	}
}

void PushGameLoopMessage(GameLoopMessageType messageType)
{
	auto& loopMessage = GameLoopMessage{};

	loopMessage.type = messageType;

	GAME_LOOP_MESSAGE_QUEUE.push(loopMessage);
}

bool GameLoopMessagesPending()
{
	return !GAME_LOOP_MESSAGE_QUEUE.empty();
}

void ClearGameLoopMessages()
{
	while (!GAME_LOOP_MESSAGE_QUEUE.empty())
	{
		GAME_LOOP_MESSAGE_QUEUE.pop();
	}
}

// house message handlers
static void Ensure_House_Message_Queue_Initialised(HousesType houseType, HouseMessageType messageType)
{
	if (HOUSE_MESSAGE_QUEUES.find(houseType) == HOUSE_MESSAGE_QUEUES.end())
	{
		HOUSE_MESSAGE_QUEUES[houseType] = new std::map<HouseMessageType, std::queue<HouseMessage>*>();
	}

	auto& houseMessageQueue = *(HOUSE_MESSAGE_QUEUES[houseType]);

	if (houseMessageQueue.find(messageType) == houseMessageQueue.end())
	{
		houseMessageQueue[messageType] = new std::queue<HouseMessage>();
	}
}

void PushHouseMessage(HousesType houseType, HouseMessageType messageType, HouseMessage& message)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	auto& houseQueue = *(HOUSE_MESSAGE_QUEUES[houseType]);

	houseQueue[messageType]->push(message);
}

bool IsHouseMessagePresent(HousesType houseType, HouseMessageType messageType)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	auto& houseMessageQueues = *(HOUSE_MESSAGE_QUEUES[houseType]);
	auto& houseMessageQueue = *(houseMessageQueues[messageType]);

	return !houseMessageQueue.empty();
}

HouseMessage PopHouseMessage(HousesType houseType, HouseMessageType messageType)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	auto& houseQueue = *(HOUSE_MESSAGE_QUEUES[houseType]);
	auto& messageQueue = *(houseQueue[messageType]);
	auto& message = messageQueue.front();

	messageQueue.pop();

	return message;
}

void ClearHouseMessages(HousesType houseType)
{
	if (HOUSE_MESSAGE_QUEUES.find(houseType) == HOUSE_MESSAGE_QUEUES.end())
	{
		return;
	}

	auto& houseMessageQueues = *(HOUSE_MESSAGE_QUEUES[houseType]);

	for (auto const& messageQueueKvp : houseMessageQueues)
	{
		auto& messageQueue = *(messageQueueKvp.second);

		while (!messageQueue.empty())
		{
			messageQueue.pop();
		}
	}

	houseMessageQueues.clear();
}
