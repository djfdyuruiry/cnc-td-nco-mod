#include "function.h"

#include "InfantryApi.h"
#include "lua.h"
#include "rules_ini.h"

bool Register_Infantry_Rule_Functions()
{

    LuaRuntime().RegisterApi(
        InfantryApi::Build(
            GetRulesInfo().GetInfantryRules()
        )
    );

    return true;
}
