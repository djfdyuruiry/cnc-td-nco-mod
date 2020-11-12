#pragma once

#include <RuleSectionApi.h>
#include <IRulesIniSection.h>

#include "function.h"

#include "AircraftApi.h"
#include "BuildingApi.h"
#include "BulletApi.h"
#include "GameApi.h"
#include "InfantryApi.h"
#include "InfoApi.h"
#include "lua_events.h"
#include "lua_api_functions_util.h"
#include "TiberianDawnNcoRuntime.h"
#include "UnitApi.h"
#include "WarheadApi.h"
#include "WeaponApi.h"

#define ReadTypeCount(key) TiberianDawnNcoRuntime::GetInstance() \
        .GetRulesRuntime() \
        .ReadRuleValue<unsigned int>(key);

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

    auto legacyLoadersResult = LoadNcoLuaLib(luaRuntime)
        && Register_Util_Functions();

    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetNcoRules());
    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetEnhancementRules());
    RegisterApi<RuleSectionApi>(luaRuntime, rulesInfo.GetGameRules());

    RegisterApi<InfantryApi>(
        luaRuntime,
        rulesInfo.GetInfantryRules(),
        []() { return ReadTypeCount(*INFANTRY_COUNT_RULE_KEY); }
    );
    RegisterApi<UnitApi>(
        luaRuntime,
        rulesInfo.GetUnitRules(),
        []() { return ReadTypeCount(*UNIT_COUNT_RULE_KEY); }
    );
    RegisterApi<AircraftApi>(
        luaRuntime,
        rulesInfo.GetAircraftRules(),
        []() { return ReadTypeCount(*AIRCRAFT_COUNT_RULE_KEY); }
    );
    RegisterApi<BuildingApi>(
        luaRuntime,
        rulesInfo.GetBuildingRules(),
        []() { return ReadTypeCount(*BUILDING_COUNT_RULE_KEY); }
    );
    RegisterApi<WeaponApi>(
        luaRuntime,
        rulesInfo.GetWeaponRules(),
        []() { return ReadTypeCount(*WEAPON_COUNT_RULE_KEY); }
    );
    RegisterApi<BulletApi>(
        luaRuntime,
        rulesInfo.GetBulletRules(),
        []() { return ReadTypeCount(*BULLET_COUNT_RULE_KEY); }
    );
    RegisterApi<WarheadApi>(
        luaRuntime,
        rulesInfo.GetWarheadRules(),
        []() { return ReadTypeCount(*WARHEAD_COUNT_RULE_KEY); }
    );

    RegisterApi<GameApi>(luaRuntime);

    luaRuntime.RegisterApi(InfoApi::Build(luaRuntime));

    return legacyLoadersResult;
}

bool TiberianDawnNcoRuntime::InitialiseLuaEvents()
{
    return Initialise_Events();
}

TiberianDawnNcoRuntime* TiberianDawnNcoRuntime::INSTANCE = NULL;

TiberianDawnNcoRuntime& TiberianDawnNcoRuntime::GetInstance()
{
    if (INSTANCE != NULL)
    {
        return *INSTANCE;
    }

    InitaliseTiberianDawnRuleKeys();
    RegisterTiberianDawnRuleTypes();

    INSTANCE = new TiberianDawnNcoRuntime();

    INSTANCE->Initialise();

    ReadMods(INSTANCE->GetRulesRuntime());

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
