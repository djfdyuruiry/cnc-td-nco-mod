#pragma once

#include <NcoRuntime.h>
#include <RuleSectionApi.h>

#include "AircraftApi.h"
#include "BuildingApi.h"
#include "BulletApi.h"
#include "defines.h"
#include "GameApi.h"
#include "InfantryApi.h"
#include "InfoApi.h"
#include "IRulesIniSection.h"
#include "lua.h"
#include "lua_api_functions_util.h"
#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRulesReader.h"
#include "TiberianDawnRulesInfo.h"
#include "UnitApi.h"
#include "WarheadApi.h"
#include "WeaponApi.h"

static unsigned int ReadTypeCount(const RulesIniRuleKey& key)
{
    return TiberianDawnNcoRuntime::GetInstance()
        .GetRulesRuntime()
        .ReadRuleValue<unsigned int>(key);
}

class TiberianDawnNcoRuntime : public NcoRuntime
{
private:
	static TiberianDawnNcoRuntime* INSTANCE;

    TiberianDawnRulesInfo& rulesInfo;

	TiberianDawnNcoRuntime() :
        NcoRuntime(RulesRuntime<TiberianDawnRulesReader>::Build(TICKS_PER_SECOND)), 
        rulesInfo(TiberianDawnRulesInfo::BuildRuleInfo(rulesRuntime.GetRules()))
	{
        InitaliseTiberianDawnRuleKeys();
	}

    template<class T> void RegisterApi()
    {
        luaRuntime.RegisterApi(T::Build());
    }

    template<class T> void RegisterApi(IRulesIniSection& ruleInfo)
    {
        luaRuntime.RegisterApi(T::Build(ruleInfo));
    }

    template<class T> void RegisterApi(IRulesIniSection& ruleInfo, std::function<int(void)> getCount)
    {
        luaRuntime.RegisterApi(T::Build(ruleInfo, getCount));
    }

    bool LoadNcoLuaLib()
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

    bool InitialiseLuaApi()
    {
        Log_Debug("Initialising Lua API functions");

        auto legacyLoadersResult = LoadNcoLuaLib()
            && Register_Util_Functions();

        RegisterApi<RuleSectionApi>(rulesInfo.GetNcoRules());
        RegisterApi<RuleSectionApi>(rulesInfo.GetEnhancementRules());
        RegisterApi<RuleSectionApi>(rulesInfo.GetGameRules());

        RegisterApi<InfantryApi>(
            rulesInfo.GetInfantryRules(),
            []() { return ReadTypeCount(*INFANTRY_COUNT_RULE_KEY); }
        );
        RegisterApi<UnitApi>(
            rulesInfo.GetUnitRules(),
            []() { return ReadTypeCount(*UNIT_COUNT_RULE_KEY); }
        );
        RegisterApi<AircraftApi>(
            rulesInfo.GetAircraftRules(),
            []() { return ReadTypeCount(*AIRCRAFT_COUNT_RULE_KEY); }
        );
        RegisterApi<BuildingApi>(
            rulesInfo.GetBuildingRules(),
            []() { return ReadTypeCount(*BUILDING_COUNT_RULE_KEY); }
        );
        RegisterApi<WeaponApi>(
            rulesInfo.GetWeaponRules(),
            []() { return ReadTypeCount(*WEAPON_COUNT_RULE_KEY); }
        );
        RegisterApi<BulletApi>(
            rulesInfo.GetBulletRules(),
            []() { return ReadTypeCount(*BULLET_COUNT_RULE_KEY); }
        );
        RegisterApi<WarheadApi>(
            rulesInfo.GetWarheadRules(),
            []() { return ReadTypeCount(*WARHEAD_COUNT_RULE_KEY); }
        );

        RegisterApi<GameApi>();
        RegisterApi<InfoApi>();

        return legacyLoadersResult;
    }

	bool InitialiseLuaEvents()
	{

	}

public:
	static TiberianDawnNcoRuntime& GetInstance()
	{
		if (INSTANCE != NULL)
		{
			return *INSTANCE;
		}

		INSTANCE = new TiberianDawnNcoRuntime();

		INSTANCE->Initialise();
	}

	static void Shutdown()
	{
		if (INSTANCE != NULL)
		{
			delete INSTANCE;
		}
	}

    TiberianDawnRulesInfo& GetRulesInfo()
    {
        return rulesInfo;
    }

};
