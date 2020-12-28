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
                    return &TiberianDawnRulesReader::Build(*rules, typeConverter);
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

    template<class T, class U> void InjectRulesForType(T first, T count, std::function<void(const char*, U*)> injector)
    {
        for (auto t = first; t < count; t++)
        {
            auto typeString = typeConverter.ToStringOrDefault<T>(t, "", true);

            injector(typeString, (U*)U::By_Type(t));
        }
    }

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
