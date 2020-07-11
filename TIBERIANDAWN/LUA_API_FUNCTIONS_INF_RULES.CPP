#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Infantry_Rule(InfantryTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
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
    else
    {
        return false;
    }

    return true;
}

static InfantryType ParseInfantryType(lua_State* lua, const char* infantryTypeName, bool* parseError)
{
    auto infantryType = Parse_Infantry_Type(
        Convert_String_To_Upper_Case(infantryTypeName),
        parseError
    );

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
    Log_Trace("Lua_Get_Infantry_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getInfantryRule requires exactly two arguments");
    }

    auto infantryTypeName = Get_Lua_Value_As_String(1);

    if (infantryTypeName == NULL)
    {
        luaL_error(lua, "getInfantryRule argument `infantryTypeName` was nil");
        return 0;
    }

    auto infantry = ResolveInfantry(lua, infantryTypeName);

    if (infantry == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);

    if (ruleName == NULL)
    {
        luaL_error(lua, "getInfantryRule argument `ruleName` was nil");
        return 0;
    }

    Log_Trace("Lua_Get_Infantry_Rule => attempting to read value of rule '%s'", ruleName);

    Log_Debug("Lua_Get_Infantry_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Infantry_Rule => Rule matched in Techno Type");
        return 1;
    }

    Log_Trace("Lua_Get_Infantry_Rule => looking for rule in Infantry Type");

    ruleMatched = Read_Infantry_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Infantry_Rule => Rule matched in Infantry Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getInfantryRule was not recognised: %s", ruleName);

    return 0;
}

int Lua_Set_Infantry_Rule(lua_State* lua)
{
    Log_Trace("Lua_Set_Infantry_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setInfantryRule requires exactly three arguments");
        return 0;
    }

    auto infantryTypeName = Get_Lua_Value_As_String(1);

    if (infantryTypeName == NULL)
    {
        luaL_error(lua, "setInfantryRule argument `infantryTypeName` was nil");
        return 0;
    }

    auto infantry = ResolveInfantry(lua, infantryTypeName);

    if (infantry == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto ruleValue = Get_Lua_Value_As_String(3);

    if (ruleName == NULL)
    {
        luaL_error(lua, "setInfantryRule argument `ruleName` was nil");
        return 0;
    }

    if (ruleValue == NULL)
    {
        luaL_error(lua, "setInfantryRule argument `ruleValue` was nil");
        return 0;
    }

    Log_Debug("Lua_Set_Infantry_Rule => attempting to set rule '%s' to value: %s", ruleName, ruleValue);

    Log_Trace("Lua_Set_Infantry_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        infantry,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        ruleValue,
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Infantry_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Trace("Lua_Get_Infantry_Rule => looking for rule in Infantry Type");

        ruleMatched = Write_Infantry_Rule(
            infantry,
            Convert_String_To_Upper_Case(ruleName),
            Convert_String_To_Upper_Case(ruleValue),
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_Infantry_Rule => Rule matched in Infantry Type");
            return 1;
        }
    }

    if (parseError)
    {
        luaL_error(lua, "value for rule `%s` passed to setInfantryRule was not valid: %s", ruleName, ruleValue);

        return 0;
    }

    luaL_error(lua, "rule name passed setInfantryRule was not recognised: %s", ruleName);

    return 0;
}

bool Register_Infantry_Rule_Functions()
{
    Log_Debug("Registering Infantry Rules Lua API functions");

    Register_Lua_Function("getInfantryRule", Lua_Get_Infantry_Rule);
    Register_Lua_Function("setInfantryRule", Lua_Set_Infantry_Rule);

    return true;
}
