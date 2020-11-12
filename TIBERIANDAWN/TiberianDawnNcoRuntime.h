#pragma once

#include <NcoRuntime.h>
#include <RulesRuntime.h>

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "TiberianDawnRulesInfo.h"
#include "TiberianDawnRulesReader.h"

#define TdNcoRuntime() TiberianDawnNcoRuntime::GetInstance()
#define NcoRulesRuntime() TdNcoRuntime().GetRulesRuntime()
#define NcoLuaRuntime() TdNcoRuntime().GetLuaRuntime()

class TiberianDawnNcoRuntime final : public NcoRuntime
{
private:
	static TiberianDawnNcoRuntime* INSTANCE;

    TiberianDawnRulesInfo& rulesInfo;

    TiberianDawnNcoRuntime() :
        NcoRuntime(
            RulesRuntime<TiberianDawnRulesReader>::Build(
                TICKS_PER_SECOND,
                [](IRulesIni& r)
                {
                    r << TiberianDawnRuleSectionBuilder::BuildModRules()
                      << TiberianDawnRuleSectionBuilder::BuildGameRules()

                      << TiberianDawnRuleSectionBuilder::BuildSuperweaponRules(AIRSTRIKE_SECTION_NAME, 8u)
                      << TiberianDawnRuleSectionBuilder::BuildSuperweaponRules(ION_CANNON_SECTION_NAME, 10u)
                      << TiberianDawnRuleSectionBuilder::BuildSuperweaponRules(NUCLEAR_STRIKE_SECTION_NAME, 14u);
                }
            )
        ),
        rulesInfo(TiberianDawnRulesInfo::BuildRuleInfo(rulesRuntime.GetRules()))
    {
    }

    bool InitialiseLuaApi();

    bool InitialiseLuaEvents();

public:
    static TiberianDawnNcoRuntime& GetInstance();

	static void Shutdown();

	TiberianDawnRulesInfo& GetRulesInfo();

};
