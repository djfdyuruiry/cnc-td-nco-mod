#pragma once

#include "./logger.h"
#include "./lua.h"
#include "./lua_events.h"
#include "./nco.h"
#include "./parse.h"
#include "./rules_ini.h"
#include "./rules_ini_ai.h"
#include "./rules_ini_aircraft.h"
#include "./rules_ini_building.h"
#include "./rules_ini_bullet.h"
#include "./rules_ini_difficulty.h"
#include "./rules_ini_enhancements.h"
#include "./rules_ini_game.h"
#include "./rules_ini_generic.h"
#include "./rules_ini_infantry.h"
#include "./rules_ini_iq.h"
#include "./rules_ini_mods.h"
#include "./rules_ini_superweapon.h"
#include "./rules_ini_unit.h"
#include "./rules_ini_warhead.h"
#include "./rules_ini_weapon.h"
#include "./strings.h"
#include "./utils.h"

bool NCO_Startup();

void NCO_Shutdown();
