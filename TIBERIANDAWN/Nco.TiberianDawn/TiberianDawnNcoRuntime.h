#pragma once

#include <NcoRuntime.h>
#include <RulesRuntime.h>

#include "rules_ini_superweapon.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "TiberianDawnRulesInfo.h"
#include "TiberianDawnRulesReader.h"
#include "TiberianDawnTypeConverter.h"

#define TdNcoRuntime() TiberianDawnNcoRuntime::GetInstance()
#define NcoRulesRuntime() TdNcoRuntime().GetRulesRuntime()
#define NcoLuaRuntime() TdNcoRuntime().GetLuaRuntime()
#define NcoModsRuntime() TdNcoRuntime().GetModRuntime()
#define NcoTypeConverter() TdNcoRuntime().GetTypeConverter()

class TiberianDawnNcoRuntime final : public NcoRuntime
{
private:
	static TiberianDawnNcoRuntime* INSTANCE;

    TiberianDawnRulesInfo& rulesInfo;
    TiberianDawnRulesReader* rulesReader;
    TiberianDawnTypeConverter& typeConverter;

    TiberianDawnNcoRuntime() :
        NcoRuntime(
            RulesRuntime<TiberianDawnRulesReader>::Build(
                TICKS_PER_SECOND,
                [](auto& r)
                {
                    r << TiberianDawnRuleSectionBuilder::BuildModRules()
                      << TiberianDawnRuleSectionBuilder::BuildGameRules()

                      << TiberianDawnRuleSectionBuilder::BuildAirstrikeRules()
                      << TiberianDawnRuleSectionBuilder::BuildIonCannonRules()
                      << TiberianDawnRuleSectionBuilder::BuildNuclearStrikeRules();
                },
                [&](auto rules)
                {
                    rulesReader = &TiberianDawnRulesReader::Build(*rules, typeConverter);

                    return rulesReader;
                }
            )
        ),
        rulesInfo(TiberianDawnRulesInfo::BuildRuleInfo(rulesRuntime)),
        typeConverter(TiberianDawnTypeConverter::Build())
    {
    }

    bool InitialiseLuaCppApi();

    bool InitialiseLuaNativeApi();

    void RegisterMods();

    void RegisterThreads();

    void Initialise();

    bool InternalShutdown();

    void InjectRules();

public:
    static TiberianDawnNcoRuntime& GetInstance();

	static void Shutdown();

    ~TiberianDawnNcoRuntime()
    {
        delete &rulesInfo;
        delete &typeConverter;
    }

	TiberianDawnRulesInfo& GetRulesInfo();

    TiberianDawnTypeConverter& GetTypeConverter();

};
