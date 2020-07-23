#ifndef GAME_MESSAGES_H
#define GAME_MESSAGES_H

// Lua event firing
enum GameLoopMessageType : unsigned char {
	GAME_TICK_ELAPSED,
};

struct GameLoopMessage {
	bool unread;
	GameLoopMessageType type;
	char* message;
};

// UI message processing
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

// House message processing
enum HouseSuperweaponMessageType : unsigned char {
	ENABLE_SUPERWEAPON,
	DISABLE_SUPERWEAPON,
	CHARGE_SUPERWEAPON
};

struct HouseSuperweaponMessage {
	bool unread;
	HouseSuperweaponMessageType type;
	// weapon is really a pointer to a Superweapon object, void* used here to prevent include conflicts
	void* weapon;
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

#endif
