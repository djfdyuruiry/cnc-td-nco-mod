#ifndef GAME_MESSAGES_H
#define GAME_MESSAGES_H

#include "super.h"

enum HouseSuperweaponMessageType : unsigned char {
	ENABLE_SUPERWEAPON,
	DISABLE_SUPERWEAPON,
	CHARGE_SUPERWEAPON
};

struct HouseSuperweaponMessage {
	bool unread;
	HouseSuperweaponMessageType type;
	SuperClass* weapon;
	bool oneTime;
};

enum GameLoopMessageType : unsigned char {
	GAME_TICK_ELAPSED,
};

struct GameLoopMessage {
	bool unread;
	GameLoopMessageType type;
	char* message;
};

enum GameUiMessageType : unsigned char {
	NO_MESSAGE,
	SHOW_NOTIFICATION_MESSAGE,
	REFRESH_SIDEBAR_MESSAGE,
	REFRESH_MAP_MESSAGE,
	REVEAL_ENTIRE_MAP_MESSAGE,
	HIDE_ENTIRE_MAP_MESSAGE
};

struct GameUiMessage {
	bool unread;
	GameUiMessageType type;
	char* message;
	float durationInSeconds;
};

extern GameUiMessage GAME_UI_MESSAGE;

// message queuing
bool Push_Show_Notification_Ui_Message(const char* message, float durationInSeconds);

bool Push_Refresh_Sidebar_Ui_Message();

bool Push_Refresh_Map_Ui_Message();

bool Push_Reveal_Map_Ui_Message();

bool Push_Hide_Map_Ui_Message();

// message handlers
void Process_Game_Ui_Message();

#endif
