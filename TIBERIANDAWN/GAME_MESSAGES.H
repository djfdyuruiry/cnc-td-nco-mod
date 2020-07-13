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

#endif
