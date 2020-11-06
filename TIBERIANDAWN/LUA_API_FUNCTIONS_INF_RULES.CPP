#include "function.h"

#include "InfantryApi.h"
#include "lua_api_functions_generic_rules.h"

static bool Write_Infantry_Rule(InfantryTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, FEMALE_RULE))
    {
        auto isFemale = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFemale), Convert_Boolean_To_String(isFemale));

            type->IsFemale = isFemale;
        }
    }
    else if (Strings_Are_Equal(ruleName, CRAWLING_RULE))
    {
        auto isCrawling = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrawling), Convert_Boolean_To_String(isCrawling));

            type->IsCrawling = isCrawling;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE))
    {
        auto isCapture = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCapture), Convert_Boolean_To_String(isCapture));

            type->IsCapture = isCapture;
        }
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE))
    {
        auto isFraidyCat = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFraidyCat), Convert_Boolean_To_String(isFraidyCat));

            type->IsFraidyCat = isFraidyCat;
        }
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE))
    {
        auto isCivilian = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCivilian), Convert_Boolean_To_String(isCivilian));

            type->IsCivilian = isCivilian;
        }
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE))
    {
        auto isAvoidingTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsAvoidingTiberium), Convert_Boolean_To_String(isAvoidingTiberium));

            type->IsAvoidingTiberium = isAvoidingTiberium;
        }
    }
    else if (Strings_Are_Equal(ruleName, IMMUNE_TO_TIBERIUM_RULE))
    {
        auto isImmuneToTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsImmuneToTiberium), Convert_Boolean_To_String(isImmuneToTiberium));

            type->IsImmuneToTiberium = isImmuneToTiberium;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_C4_CHARGES_RULE))
    {
        auto hasC4Charges = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->HasC4Charges), Convert_Boolean_To_String(hasC4Charges));

            type->HasC4Charges = hasC4Charges;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Infantry_Rule(InfantryTypeClass* type, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, FEMALE_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFemale));

        luaState.WriteBool(type->IsFemale);
    }
    else if (Strings_Are_Equal(ruleName, CRAWLING_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrawling));

        luaState.WriteBool(type->IsCrawling);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCapture));

        luaState.WriteBool(type->IsCapture);
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFraidyCat));

        luaState.WriteBool(type->IsFraidyCat);
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCivilian));

        luaState.WriteBool(type->IsCivilian);
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAvoidingTiberium));

        luaState.WriteBool(type->IsAvoidingTiberium);
    }
    else if (Strings_Are_Equal(ruleName, IMMUNE_TO_TIBERIUM_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsImmuneToTiberium));

        luaState.WriteBool(type->IsImmuneToTiberium);
    }
    else if (Strings_Are_Equal(ruleName, HAS_C4_CHARGES_RULE))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->HasC4Charges));

        luaState.WriteBool(type->HasC4Charges);
    }
    else
    {
        return false;
    }

    return true;
}

static InfantryType ParseInfantryType(const char* infantryTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();
    auto upperTypeName = Convert_String_To_Upper_Case(infantryTypeName);

    auto infantryType = Parse_Infantry_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaState.RaiseError("infantry type passed was not recognised: %s", infantryTypeName);
        return INFANTRY_NONE;
    }

    return infantryType;
}

static InfantryTypeClass* ResolveInfantry(const char* infantryTypeName)
{
    bool parseError = false;
    auto infantryType = ParseInfantryType(infantryTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("infantry type parsed: %s -> %s", infantryTypeName, Infantry_Type_To_String(infantryType));

    return (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
}

int Lua_Get_Infantry_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        "Infantry",
        "infantryTypeName",
        &ResolveInfantry,
        GetRulesInfo().GetInfantryRules(),
        &Read_Infantry_Rule
    );
}

int Lua_Set_Infantry_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        "Infantry",
        "infantryTypeName",
        &ResolveInfantry,
        GetRulesInfo().GetInfantryRules(),
        &Write_Infantry_Rule
    );
}

bool Register_Infantry_Rule_Functions()
{
    //LuaRuntime().RegisterApi("infantry rules", [](ILuaApi& a) {
    //    a.WithDescription("Infantry rule info and control functions")

    //     .WithFunction("getInfantryRule", &Lua_Get_Infantry_Rule, [](LuaFunctionInfo& f) {
    //         f.WithDescription("Write a info line to the log file")
    //          .WithParameter("str", [](LuaVariableInfo& p) {
    //             p.WithType(LuaType::String);
    //          });
    //     })
    //     .WithFunction("setInfantryRule", &Lua_Set_Infantry_Rule, [](LuaFunctionInfo& f) {
    //         f.WithDescription("Write a info line to the log file")
    //          .WithParameter("str", [](LuaVariableInfo& p) {
    //             p.WithType(LuaType::String);
    //          });
    //     });
    //});

    LuaRuntime().RegisterApi(
        InfantryApi::Build(
            GetRulesInfo().GetInfantryRules()
        )
    );

    return true;
}
