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
	GAME_TICK_ELAPSED
};

struct GameLoopMessage {
	bool unread;
	GameLoopMessageType type;
};

#endif
