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

static void InjectIqRules(IRulesRuntime& r)
{
    Rule.MaxIQ = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, MAX_IQ_RULE);
    Rule.IQSuperWeapons = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SUPER_WEAPONS_RULE);
    Rule.IQProduction = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_PRODUCTION_RULE);
    Rule.IQGuardArea = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_GUARD_AREA_RULE);
    Rule.IQRepairSell = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_REPAIR_SELL_RULE);
    Rule.IQCrush = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_CRUSH_RULE);
    Rule.IQScatter = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SCATTER_RULE);
    Rule.IQContentScan = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_CONTENT_SCAN_RULE);
    Rule.IQAircraft = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_AIRCRAFT_RULE);
    Rule.IQHarvester = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_HARVESTER_RULE);
    Rule.IQSellBack = r.ReadRuleValue<unsigned int>(IQ_SECTION_NAME, IQ_SELL_BACK_RULE);
}

static void InjectAiRules(IRulesRuntime& r)
{
    Rule.AttackInterval = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, ATTACK_INTERVAL_RULE);
    Rule.BaseSizeAdd = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BASE_SIZE_ADD_RULE);
    Rule.PowerSurplus = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, POWER_SURPLUS_RULE);
    Rule.AttackDelay = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, ATTACK_DELAY_RULE);
    Rule.PowerEmergencyFraction = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, POWER_EMERGENCY_FRACTION_RULE);
    Rule.HelipadRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, HELIPAD_RATIO_RULE);
    Rule.HelipadLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, HELIPAD_LIMIT_RULE);
    Rule.AARatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AA_RATIO_RULE);
    Rule.AALimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AA_LIMIT_RULE);
    Rule.DefenseRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, DEFENSE_RATIO_RULE);
    Rule.DefenseLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, DEFENSE_LIMIT_RULE);
    Rule.WarRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, WAR_RATIO_RULE);
    Rule.WarLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, WAR_LIMIT_RULE);
    Rule.BarracksRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BARRACKS_RATIO_RULE);
    Rule.BarracksLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, BARRACKS_LIMIT_RULE);
    Rule.RefineryRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, REFINERY_RATIO_RULE);
    Rule.RefineryLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, REFINERY_LIMIT_RULE);
    Rule.AirstripRatio = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AIRSTRIP_RATIO_RULE);
    Rule.AirstripLimit = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, AIRSTRIP_LIMIT_RULE);
    Rule.InfantryReserve = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, INFANTRY_RESERVE_RULE);
    Rule.InfantryBaseMult = r.ReadRuleValue<unsigned int>(AI_SECTION_NAME, INFANTRY_BASE_MULT_RULE);
    Rule.IsComputerParanoid = r.ReadRuleValue<bool>(AI_SECTION_NAME, PARANOID_RULE);
}

void TiberianDawnNcoRuntime::InjectRules()
{
    InjectRulesForType<WarheadType, WarheadTypeClass>(WARHEAD_FIRST, WARHEAD_COUNT, [&](auto typeString, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWarheadSection(typeString);
        WarheadTypeClass::ReadWarheadRules(instance);
    });

    InjectRulesForType<BulletType, BulletTypeClass>(BULLET_FIRST, BULLET_COUNT, [&](auto typeString, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildBulletSection(instance->RulesName);
        BulletTypeClass::ReadBulletRules(instance);
    });

    InjectRulesForType<WeaponType, WeaponTypeClass>(WEAPON_FIRST, WEAPON_COUNT, [&](auto typeString, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWeaponSection(typeString);
        WeaponTypeClass::ReadWeaponRules(instance);
    });

    InjectRulesForType<InfantryType, InfantryTypeClass>(INFANTRY_FIRST, INFANTRY_COUNT, [&](auto typeString, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildInfantrySection(instance->IniName);
        InfantryTypeClass::ReadInfantryRules(instance);
    });

    InjectRulesForType<UnitType, UnitTypeClass>(UNIT_FIRST, UNIT_COUNT, [&](auto _, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildUnitSection(instance->IniName);
        UnitTypeClass::ReadUnitRules(instance);
    });

    InjectRulesForType<AircraftType, AircraftTypeClass>(AIRCRAFT_FIRST, AIRCRAFT_COUNT, [&](auto _, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildAircraftSection(instance->IniName);
        AircraftTypeClass::ReadAircraftRules(instance);
    });

    InjectRulesForType<StructType, BuildingTypeClass>(STRUCT_FIRST, STRUCT_COUNT, [&](auto _, auto instance)
    {
        rulesRuntime.GetRules() << TiberianDawnRuleSectionBuilder::BuildBuildingSection(instance->IniName);
        BuildingTypeClass::ReadBuildingRules(instance);
    });

    InjectAiRules(rulesRuntime);
    InjectIqRules(rulesRuntime);

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
