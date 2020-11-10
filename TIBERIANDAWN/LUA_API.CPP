#include "function.h"

#include <logger.h>

#include "AircraftApi.h"
#include "BuildingApi.h"
#include "BulletApi.h"
#include "GameApi.h"
#include "InfantryApi.h"
#include "InfoApi.h"
#include "IRulesIniSection.h"
#include "lua.h"
#include "lua_api_functions_util.h"
#include "rules_ini.h"
#include "RuleSectionApi.h"
#include "UnitApi.h"
#include "WarheadApi.h"
#include "WeaponApi.h"

static bool API_INITIALISED = false;

template<class T> static void RegisterApi()
{
    LuaRuntime().RegisterApi(T::Build());
}

template<class T> static void RegisterApi(IRulesIniSection& ruleInfo)
{
    LuaRuntime().RegisterApi(T::Build(ruleInfo));
}

template<class T> static void RegisterApi(IRulesIniSection& ruleInfo, std::function<int(void)> getCount)
{
    LuaRuntime().RegisterApi(T::Build(ruleInfo, getCount));
}

static bool Load_Nco_Lua_Lib()
{
    return Execute_Lua_Script("require('nco')");
}

static bool Load_Api_Functions()
{
    Log_Debug("Initialising Lua API functions");

    auto legacyLoadersResult = Load_Nco_Lua_Lib()
        && Register_Util_Functions();

    auto& rulesInfo = GetRulesInfo();

    RegisterApi<RuleSectionApi>(rulesInfo.GetNcoRules());
    RegisterApi<RuleSectionApi>(rulesInfo.GetEnhancementRules());
    RegisterApi<RuleSectionApi>(rulesInfo.GetGameRules());

    RegisterApi<InfantryApi>(
        rulesInfo.GetInfantryRules(),
        []() { return Read_Infantry_Count(); }
    );
    RegisterApi<UnitApi>(
        rulesInfo.GetUnitRules(),
        []() { return Read_Unit_Count(); }
    );
    RegisterApi<AircraftApi>(
        rulesInfo.GetAircraftRules(),
        []() { return Read_Aircraft_Count(); }
    );
    RegisterApi<BuildingApi>(
        rulesInfo.GetBuildingRules(),
        []() { return Read_Building_Count(); }
    );
    RegisterApi<WeaponApi>(
        rulesInfo.GetWeaponRules(),
        []() { return Read_Weapon_Count(); }
    );
    RegisterApi<BulletApi>(
        rulesInfo.GetBulletRules(),
        []() { return Read_Bullet_Count(); }
    );
    RegisterApi<WarheadApi>(
        rulesInfo.GetWarheadRules(),
        []() { return Read_Warhead_Count(); }
    );

    RegisterApi<GameApi>();
    RegisterApi<InfoApi>();

    return legacyLoadersResult;
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
