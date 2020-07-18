#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Infantry_Rule(InfantryTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, FEMALE_RULE_UPPER))
    {
        auto isFemale = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFemale), Convert_Boolean_To_String(isFemale));

            type->IsFemale = isFemale;
        }
    }
    else if (Strings_Are_Equal(ruleName, CRAWLING_RULE_UPPER))
    {
        auto isCrawling = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrawling), Convert_Boolean_To_String(isCrawling));

            type->IsCrawling = isCrawling;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE_UPPER))
    {
        auto isCapture = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCapture), Convert_Boolean_To_String(isCapture));

            type->IsCapture = isCapture;
        }
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE_UPPER))
    {
        auto isFraidyCat = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFraidyCat), Convert_Boolean_To_String(isFraidyCat));

            type->IsFraidyCat = isFraidyCat;
        }
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE_UPPER))
    {
        auto isCivilian = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCivilian), Convert_Boolean_To_String(isCivilian));

            type->IsCivilian = isCivilian;
        }
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE_UPPER))
    {
        auto isAvoidingTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsAvoidingTiberium), Convert_Boolean_To_String(isAvoidingTiberium));

            type->IsAvoidingTiberium = isAvoidingTiberium;
        }
    }
    else if (Strings_Are_Equal(ruleName, IMMUNE_TO_TIBERIUM_RULE_UPPER))
    {
        auto isImmuneToTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Infantry_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsImmuneToTiberium), Convert_Boolean_To_String(isImmuneToTiberium));

            type->IsImmuneToTiberium = isImmuneToTiberium;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_C4_CHARGES_RULE_UPPER))
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

static bool Read_Infantry_Rule(lua_State* lua, InfantryTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, FEMALE_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFemale));

        lua_pushboolean(lua, type->IsFemale);
    }
    else if (Strings_Are_Equal(ruleName, CRAWLING_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrawling));

        lua_pushboolean(lua, type->IsCrawling);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCapture));

        lua_pushboolean(lua, type->IsCapture);
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFraidyCat));

        lua_pushboolean(lua, type->IsFraidyCat);
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCivilian));

        lua_pushboolean(lua, type->IsCivilian);
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAvoidingTiberium));

        lua_pushboolean(lua, type->IsAvoidingTiberium);
    }
    else if (Strings_Are_Equal(ruleName, IMMUNE_TO_TIBERIUM_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsImmuneToTiberium));

        lua_pushboolean(lua, type->IsImmuneToTiberium);
    }
    else if (Strings_Are_Equal(ruleName, HAS_C4_CHARGES_RULE_UPPER))
    {
        Log_Trace("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->HasC4Charges));

        lua_pushboolean(lua, type->HasC4Charges);
    }
    else
    {
        return false;
    }

    return true;
}

static InfantryType ParseInfantryType(lua_State* lua, const char* infantryTypeName, bool* parseError)
{
    auto upperTypeName = Convert_String_To_Upper_Case(infantryTypeName);

    auto infantryType = Parse_Infantry_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaL_error(lua, "infantry type passed was not recognised: %s", infantryTypeName);
        return INFANTRY_NONE;
    }

    return infantryType;
}

static InfantryTypeClass* ResolveInfantry(lua_State* lua, const char* infantryTypeName)
{
    bool parseError = false;
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

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
        lua,
        "Infantry",
        "infantryTypeName",
        &ResolveInfantry,
        &Read_Infantry_Rule
    );
}

int Lua_Set_Infantry_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        lua,
        "Infantry",
        "infantryTypeName",
        &ResolveInfantry,
        &Write_Infantry_Rule
    );
}

bool Register_Infantry_Rule_Functions()
{
    Log_Debug("Registering Infantry Rules Lua API functions");

    Register_Lua_Function("getInfantryRule", Lua_Get_Infantry_Rule);
    Register_Lua_Function("setInfantryRule", Lua_Set_Infantry_Rule);

    return true;
}
