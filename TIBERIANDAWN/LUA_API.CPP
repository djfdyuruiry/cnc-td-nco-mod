#include "logger.h"
#include "lua.h"
#include "lua_api_functions_air_rules.h"
#include "lua_api_functions_build_rules.h"
#include "lua_api_functions_bullet_rules.h"
#include "lua_api_functions_game.h"
#include "lua_api_functions_game_rules.h"
#include "lua_api_functions_inf_rules.h"
#include "lua_api_functions_info.h"
#include "lua_api_functions_unit_rules.h"
#include "lua_api_functions_util.h"
#include "lua_api_functions_warhead_rules.h"
#include "lua_api_functions_weapon_rules.h"

static bool API_INITIALISED = false;

static bool Load_Nco_Lua_Lib()
{
    return Execute_Lua_Script("require('nco')");
}

static bool Load_Api_Functions()
{
    Log_Debug("Initialising Lua API functions");

    return Load_Nco_Lua_Lib()
        && Register_Util_Functions()
        && Register_Info_Functions()
        && Register_Game_Rule_Functions()
        && Register_Weapon_Functions()
        && Register_Bullet_Functions()
        && Register_Warhead_Functions()
        && Register_Infantry_Rule_Functions()
        && Register_Unit_Rule_Functions()
        && Register_Aircraft_Rule_Functions()
        && Register_Building_Rule_Functions()
        && Register_Game_Functions();
}

bool Initialise_Lua_Api()
{
    if (API_INITIALISED)
    {
        Log_Warn("Attempted to initialise Lua API more than once, ignoring");
        return API_INITIALISED;
    }

    Log_Info("Initialising Lua API");

    API_INITIALISED = Load_Api_Functions();

    return API_INITIALISED;
}
