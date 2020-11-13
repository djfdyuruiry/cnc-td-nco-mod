#pragma once

#include <RuleSectionApi.h>
#include <IRulesIniSection.h>

#include "function.h"

#include "AircraftApi.h"
#include "AircraftTypeMod.h"
#include "BuildingApi.h"
#include "BuildingTypeMod.h"
#include "BulletApi.h"
#include "BulletTypeMod.h"
#include "GameApi.h"
#include "InfantryApi.h"
#include "InfantryTypeMod.h"
#include "InfoApi.h"
#include "lua_events.h"
#include "lua_api_functions_util.h"
#include "TiberianDawnNcoRuntime.h"
#include "UnitApi.h"
#include "UnitTypeMod.h"
#include "WarheadApi.h"
#include "WarheadTypeMod.h"
#include "WeaponApi.h"
#include "WeaponTypeMod.h"

#define ReadTypeCount(key) TiberianDawnNcoRuntime::GetInstance() \
        .GetRulesRuntime() \
        .ReadRuleValue<unsigned int>(key);

#define ReadTypeCountLambda(key) []() { return ReadTypeCount(*key); }

template<class T> static void RegisterApi(ILuaRuntime& luaRuntime)
{
    luaRuntime.RegisterApi(T::Build());
}

template<class T> void RegisterApi(ILuaRuntime& luaRuntime, IRulesIniSection& ruleInfo)
{
    luaRuntime.RegisterApi(T::Build(ruleInfo));
}

template<class T> void RegisterApi(ILuaRuntime& luaRuntime, IRulesIniSection& ruleInfo, std::function<int(void)> getCount)
{
    luaRuntime.RegisterApi(T::Build(ruleInfo, getCount));
}

static bool LoadNcoLuaLib(ILuaRuntime& luaRuntime)
{
    auto& loadResult = luaRuntime.ExecuteScript("require('nco')");
    auto loadFailed = loadResult.IsErrorResult();

    if (loadFailed)
    {
        Log_Error("Loading NCO lua library failed: %s", loadResult.GetError());
    }

    delete &loadResult;

    return !loadFailed;
}

bool TiberianDawnNcoRuntime::InitialiseLuaApi()
{
    Log_Debug("Initialising Lua API functions");

    auto legacyLoadersResult = LoadNcoLuaLib(luaRuntime) && Register_Util_Functions();

    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetNcoRules());
    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetEnhancementRules());
    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetGameRules());

    RegisterApi<InfantryApi>(
        luaRuntime,
        rulesInfo.GetInfantryRules(),
        ReadTypeCountLambda(INFANTRY_COUNT_RULE_KEY)
    );
    RegisterApi<UnitApi>(
        luaRuntime,
        rulesInfo.GetUnitRules(),
        ReadTypeCountLambda(UNIT_COUNT_RULE_KEY)
    );
    RegisterApi<AircraftApi>(
        luaRuntime,
        rulesInfo.GetAircraftRules(),
        ReadTypeCountLambda(AIRCRAFT_COUNT_RULE_KEY)
    );
    RegisterApi<BuildingApi>(
        luaRuntime,
        rulesInfo.GetBuildingRules(),
        ReadTypeCountLambda(BUILDING_COUNT_RULE_KEY)
    );
    RegisterApi<WeaponApi>(
        luaRuntime,
        rulesInfo.GetWeaponRules(),
        ReadTypeCountLambda(WEAPON_COUNT_RULE_KEY)
    );
    RegisterApi<BulletApi>(
        luaRuntime,
        rulesInfo.GetBulletRules(),
        ReadTypeCountLambda(BULLET_COUNT_RULE_KEY)
    );
    RegisterApi<WarheadApi>(
        luaRuntime,
        rulesInfo.GetWarheadRules(),
        ReadTypeCountLambda(WARHEAD_COUNT_RULE_KEY)
    );

    RegisterApi<GameApi>(luaRuntime);

    luaRuntime.RegisterApi(InfoApi::Build(luaRuntime));

    return legacyLoadersResult;
}

bool TiberianDawnNcoRuntime::InitialiseLuaEvents()
{
    return Initialise_Events();
}

void TiberianDawnNcoRuntime::RegisterMods()
{
    modRuntime.RegisterMod<WarheadTypeMod>()
        .RegisterMod<BulletTypeMod>()
        .RegisterMod<WeaponTypeMod>()
        .RegisterMod<BuildingTypeMod>()
        .RegisterMod<AircraftTypeMod>()
        .RegisterMod<InfantryTypeMod>()
        .RegisterMod<UnitTypeMod>();
}

void TiberianDawnNcoRuntime::Initialise()
{
    InitaliseTiberianDawnRuleKeys();
    RegisterTiberianDawnRuleTypes();

    NcoRuntime::Initialise();
}

TiberianDawnNcoRuntime* TiberianDawnNcoRuntime::INSTANCE = NULL;

TiberianDawnNcoRuntime& TiberianDawnNcoRuntime::GetInstance()
{
    if (INSTANCE == NULL)
    {
        INSTANCE = new TiberianDawnNcoRuntime();

        INSTANCE->Initialise();
    }

    return *INSTANCE;
}

void TiberianDawnNcoRuntime::Shutdown()
{
    if (INSTANCE != NULL)
    {
        delete INSTANCE;
    }
}

TiberianDawnRulesInfo& TiberianDawnNcoRuntime::GetRulesInfo()
{
    return rulesInfo;
}