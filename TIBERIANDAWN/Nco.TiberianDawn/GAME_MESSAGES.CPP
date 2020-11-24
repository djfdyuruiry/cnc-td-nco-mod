#include <map>
#include <queue>

#include <Logger.h>
#include <utils.h>

#include "game_messages.h"
#include "game_utils.h"
#include "lua_events.h"
#include "TiberianDawnNcoRuntime.h"

static std::queue<GameUiMessage> GAME_UI_MESSAGE_QUEUE;
static std::queue<GameLoopMessage> GAME_LOOP_MESSAGE_QUEUE;
static std::map<char, std::map<HouseMessageType, std::queue<HouseMessage>>> HOUSE_MESSAGE_QUEUES;

static auto LAST_TICK_TIME_IN_EPOCH_MILLIS = 0LL;

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

void PushShowNotificationUiMessage(const char* message, float durationInSeconds)
{
	GameUiMessage uiMessage;

	uiMessage.message = strdup(message);
	uiMessage.durationInSeconds = durationInSeconds;

	Push_Message_Of_Type(SHOW_NOTIFICATION_MESSAGE, uiMessage);
}

void PushRefreshSidebarUiMessage()
{
	Push_Message_Of_Type(REFRESH_SIDEBAR_MESSAGE);
}

void PushRefreshMapUiMessage()
{
	Push_Message_Of_Type(REFRESH_MAP_MESSAGE);
}

void PushRevealMapUiMessage()
{
	Push_Message_Of_Type(REVEAL_ENTIRE_MAP_MESSAGE);
}

void PushHideMapUiMessage()
{
	Push_Message_Of_Type(HIDE_ENTIRE_MAP_MESSAGE);
}

// game ui message handlers

static void Process_Game_Ui_Message(std::map<unsigned int, bool>& messageTypesProcessed)
{
	auto uiMessage = GAME_UI_MESSAGE_QUEUE.front();
	auto type = uiMessage.type;

	if (messageTypesProcessed.find(type) != messageTypesProcessed.end())
	{
		LogTrace("Dropping duplicate UI message for game tick: %u", type);

		GAME_UI_MESSAGE_QUEUE.pop();

		return;
	}

	auto processResult = Process_Game_Ui_Message(uiMessage);
	
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
	std::map<unsigned int, bool> messageTypesProcessed;

	while (!GAME_UI_MESSAGE_QUEUE.empty())
	{
		Process_Game_Ui_Message(messageTypesProcessed);
	}
}

void ClearGameUiMessages()
{
	while (!GAME_UI_MESSAGE_QUEUE.empty())
	{
		auto uiMessage = GAME_UI_MESSAGE_QUEUE.front();

		if (uiMessage.type == SHOW_NOTIFICATION_MESSAGE && uiMessage.message != NULL)
		{
			delete uiMessage.message;
		}

		GAME_UI_MESSAGE_QUEUE.pop();
	}
}

// game loop message handlers
static void Process_Game_Loop_Message(GameLoopMessage loopMessage)
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

		On_Game_Tick();

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
			auto loopMessage = GAME_LOOP_MESSAGE_QUEUE.front();

			Process_Game_Loop_Message(loopMessage);

			GAME_LOOP_MESSAGE_QUEUE.pop();
		}

		Sleep(10);
	}
}

void PushGameLoopMessage(GameLoopMessageType messageType)
{
	GameLoopMessage loopMessage;

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
		HOUSE_MESSAGE_QUEUES[houseType] = std::map<HouseMessageType, std::queue<HouseMessage>>();
	}

	auto houseMessageQueues = HOUSE_MESSAGE_QUEUES[houseType];

	if (houseMessageQueues.find(messageType) == houseMessageQueues.end())
	{
		houseMessageQueues[messageType] = std::queue<HouseMessage>();
	}
}

void PushHouseMessage(HousesType houseType, HouseMessageType messageType, HouseMessage message)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	HOUSE_MESSAGE_QUEUES[houseType][messageType].push(message);
}

bool IsHouseMessagePresent(HousesType houseType, HouseMessageType messageType)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	auto houseMessageQueues = HOUSE_MESSAGE_QUEUES[houseType];

	return !houseMessageQueues[messageType].empty();
}

HouseMessage PopHouseMessage(HousesType houseType, HouseMessageType messageType)
{
	Ensure_House_Message_Queue_Initialised(houseType, messageType);

	auto messageQueue = HOUSE_MESSAGE_QUEUES[houseType][messageType];
	auto message = messageQueue.front();

	messageQueue.pop();

	HOUSE_MESSAGE_QUEUES[houseType][messageType] = messageQueue;

	return message;
}

void ClearHouseMessages(HousesType houseType)
{
	if (HOUSE_MESSAGE_QUEUES.find(houseType) == HOUSE_MESSAGE_QUEUES.end())
	{
		return;
	}

	for (auto const& messageQueueKvp : HOUSE_MESSAGE_QUEUES[houseType])
	{
		auto messageQueue = messageQueueKvp.second;

		while (!messageQueue.empty())
		{
			messageQueue.pop();
		}
	}

	HOUSE_MESSAGE_QUEUES[houseType].clear();
}
