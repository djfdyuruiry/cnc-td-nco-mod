#ifndef GAME_MESSAGES_H
#define GAME_MESSAGES_H

#include <windows.h>

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

// message queuing
void Push_Show_Notification_Ui_Message(const char* message, float durationInSeconds);

void Push_Refresh_Sidebar_Ui_Message();

void Push_Refresh_Map_Ui_Message();

void Push_Reveal_Map_Ui_Message();

void Push_Hide_Map_Ui_Message();

// message handlers
void Process_Game_Ui_Messages();

void Clear_Game_Ui_Messages();

// game loop message handlers
DWORD WINAPI Process_Game_Loop_Messages(LPVOID lpParam);

void Push_Game_Loop_Message(GameLoopMessageType messageType);

bool Game_Loop_Messages_Pending();

void Clear_Game_Loop_Messages();

// house message handlers

void Push_House_Message(char houseType, HouseMessageType messageType, HouseMessage message);

bool Is_House_Message_Present(char houseType, HouseMessageType messageType);

HouseMessage Pop_House_Message(char houseType, HouseMessageType messageType);

void Clear_House_Messages(char houseType);

#endif
