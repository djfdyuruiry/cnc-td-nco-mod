#pragma once

#include <IRulesIniSection.h>
#include <LuaReplThread.h>
#include <RulesApi.h>
#include <utils.h>

#include "../FUNCTION.H"

#include "AircraftApi.h"
#include "AircraftTypeMod.h"
#include "BuildingApi.h"
#include "BuildingTypeMod.h"
#include "BulletApi.h"
#include "BulletTypeMod.h"
#include "GameApi.h"
#include "GameMessagesThread.h"
#include "InfantryApi.h"
#include "InfantryTypeMod.h"
#include "InfoApi.h"
#include "lua_events.h"
#include "RulesInjector.h"
#include "TiberianDawnNcoRuntime.h"
#include "UnitApi.h"
#include "UnitTypeMod.h"
#include "UtilApi.h"
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
        ShowError("Loading NCO lua library failed: %s", loadResult.GetError());
    }

    delete &loadResult;

    return !loadFailed;
}

bool TiberianDawnNcoRuntime::InitialiseLuaCppApi()
{
    LogDebug("Initialising Lua API functions");

    luaRuntime.RegisterApi(RulesApi::Build(rulesRuntime));
    
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
    RegisterApi<UtilApi>(luaRuntime);

    luaRuntime.RegisterApi(InfoApi::Build(luaRuntime));

    return true;
}

bool TiberianDawnNcoRuntime::InitialiseLuaNativeApi()
{
    return LoadNcoLuaLib(luaRuntime);
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

void TiberianDawnNcoRuntime::RegisterThreads()
{
    if (!rulesRuntime.LuaIsEnabled() || !LuaInitWasSuccessful())
    {
        return;
    }

    RegisterThread<GameMessagesThread>();

    if (!rulesRuntime.LuaConsoleIsEnabled())
    {
        return;
    }

    StartConsoleOutput();
    RegisterThread(LuaReplThread::Build(luaRuntime.GetState()));
}

void TiberianDawnNcoRuntime::Initialise()
{
    InitaliseTiberianDawnRuleKeys();
    RegisterTiberianDawnRuleTypes();

    NcoRuntime::Initialise();
}

void TiberianDawnNcoRuntime::InjectRules()
{
    auto& rulesInjector = RulesInjector::Build(rulesRuntime.GetRules(), *rulesReader);

    rulesInjector.InjectTypeRules<WarheadType, WarheadTypeClass>(WARHEAD_FIRST, WARHEAD_COUNT);
    rulesInjector.InjectTypeRules<BulletType, BulletTypeClass>(BULLET_FIRST, BULLET_COUNT);
    rulesInjector.InjectTypeRules<WeaponType, WeaponTypeClass>(WEAPON_FIRST, WEAPON_COUNT);
    rulesInjector.InjectTypeRules<InfantryType, InfantryTypeClass>(INFANTRY_FIRST, INFANTRY_COUNT);
    rulesInjector.InjectTypeRules<UnitType, UnitTypeClass>(UNIT_FIRST, UNIT_COUNT);
    rulesInjector.InjectTypeRules<AircraftType, AircraftTypeClass>(AIRCRAFT_FIRST, AIRCRAFT_COUNT);
    rulesInjector.InjectTypeRules<StructType, BuildingTypeClass>(STRUCT_FIRST, STRUCT_COUNT);

    rulesInjector.InjectAiRules(Rule);
    rulesInjector.InjectIqRules(Rule);

    delete &rulesInjector;

    FactoryClass::STEP_COUNT = rulesRuntime.ReadRuleValue<unsigned int>(TOTAL_PRODUCTION_STEPS_RULE_KEY);
}

bool TiberianDawnNcoRuntime::InternalShutdown()
{
    auto result = NcoRuntime::InternalShutdown();
    
    if (
        rulesRuntime.LuaIsEnabled()
        && LuaInitWasSuccessful()
        && rulesRuntime.LuaConsoleIsEnabled()
        )
    {
        StopConsoleOutput();
    }

    return result;
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
    if (INSTANCE == NULL)
    {
        return;
    }

    if (!INSTANCE->InternalShutdown())
    {
        LogError("NCO shutdown failed");
    }

    delete INSTANCE;
    INSTANCE = NULL;
}

TiberianDawnRulesInfo& TiberianDawnNcoRuntime::GetRulesInfo()
{
    return rulesInfo;
}

TiberianDawnTypeConverter& TiberianDawnNcoRuntime::GetTypeConverter()
{
    return typeConverter;
}
