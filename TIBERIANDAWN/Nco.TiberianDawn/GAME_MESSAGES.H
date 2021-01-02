#pragma once

#include <map>
#include <queue>

#include "../HOUSES_TYPE.H"

// Game Loop messages
enum GameLoopMessageType : unsigned char {
	GAME_TICK_ELAPSED,
};

struct GameLoopMessage {
	bool unread;
	GameLoopMessageType type;
	char* message;
};

// UI messages
enum GameUiMessageType : unsigned char {
	NO_MESSAGE,
	SHOW_NOTIFICATION_MESSAGE,
	REFRESH_SIDEBAR_MESSAGE,
	REFRESH_MAP_MESSAGE,
	REVEAL_ENTIRE_MAP_MESSAGE,
	HIDE_ENTIRE_MAP_MESSAGE
};

struct GameUiMessage {
	GameUiMessageType type;
	char* message;
	float durationInSeconds;
};

// House messages
enum HouseMessageType : unsigned char {
	SUPERWEAPON_MESSAGE
};

enum SuperweaponMessageType : unsigned char {
	ENABLE_SUPERWEAPON,
	DISABLE_SUPERWEAPON,
	CHARGE_SUPERWEAPON
};

enum SuperweaponType : unsigned char {
	NO_SUPERWEAPON,
	ION_CANNON,
	NUCLEAR_STRIKE,
	AIRSTRIKE,

	SUPERWEAPON_FIRST = 1,
	SUPERWEAPON_LAST = 4
};

struct HouseMessage {
	SuperweaponMessageType superweaponMessage;
	SuperweaponType superweapon; // weapon is really a pointer to a Superweapon object, void* used here to prevent include conflicts
	bool oneTime;
};

extern std::queue<GameUiMessage>& GAME_UI_MESSAGE_QUEUE;
extern std::queue<GameLoopMessage>& GAME_LOOP_MESSAGE_QUEUE;
extern std::map<char, std::map<HouseMessageType, std::queue<HouseMessage>*>*>& HOUSE_MESSAGE_QUEUES;

// message queuing
void PushShowNotificationUiMessage(const char* message, float durationInSeconds);

void PushRefreshSidebarUiMessage();

void PushRefreshMapUiMessage();

void PushRevealMapUiMessage();

void PushHideMapUiMessage();

// message handlers
void ProcessGameUiMessages();

void ClearGameUiMessages();

// game loop message handlers
void ProcessGameLoopMessages();

void PushGameLoopMessage(GameLoopMessageType messageType);

bool GameLoopMessagesPending();

void ClearGameLoopMessages();

// house message handlers
void PushHouseMessage(HousesType houseType, HouseMessageType messageType, HouseMessage& message);

bool IsHouseMessagePresent(HousesType houseType, HouseMessageType messageType);

HouseMessage PopHouseMessage(HousesType houseType, HouseMessageType messageType);

void ClearHouseMessages(HousesType houseType);
