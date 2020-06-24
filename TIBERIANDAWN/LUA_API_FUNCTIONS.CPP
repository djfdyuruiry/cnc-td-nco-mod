#include "function.h"

static bool Write_Infantry_Rule(InfantryTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, FEMALE_RULE_UPPER))
    {
        type->IsFemale = Parse_Boolean(value);
    }
    else if(Strings_Are_Equal(ruleName, CRAWLING_RULE_UPPER))
    {
        type->IsCrawling = Parse_Boolean(value);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE_UPPER))
    {
        type->IsCapture = Parse_Boolean(value);
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE_UPPER))
    {
        type->IsFraidyCat = Parse_Boolean(value);
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE_UPPER))
    {
        type->IsCivilian = Parse_Boolean(value);
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE_UPPER))
    {
        type->IsAvoidingTiberium = Parse_Boolean(value);
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
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFemale));

        lua_pushboolean(lua, type->IsFemale);
    }
    else if (Strings_Are_Equal(ruleName, CRAWLING_RULE_UPPER))
    {
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrawling));

        lua_pushboolean(lua, type->IsCrawling);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE_UPPER))
    {
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCapture));

        lua_pushboolean(lua, type->IsCapture);
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE_UPPER))
    {
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFraidyCat));

        lua_pushboolean(lua, type->IsFraidyCat);
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE_UPPER))
    {
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCivilian));

        lua_pushboolean(lua, type->IsCivilian);
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE_UPPER))
    {
        Log_Debug("Read_Infantry_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAvoidingTiberium));

        lua_pushboolean(lua, type->IsAvoidingTiberium);
    }
    else
    {
        return false;
    }

    return true;
}

static bool Write_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BUILD_LEVEL_RULE_UPPER))
    {
        auto numValue = (unsigned char)atoi(value);
        *valueParseError = numValue < 1 && numValue > 99;

        if (!*valueParseError)
        {
            type->Level = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, SCENARIO_LEVEL_RULE_UPPER))
    {
        auto numValue = (unsigned char)atoi(value);
        *valueParseError = numValue < 1 && numValue > 99;

        if (!*valueParseError)
        {
            type->Scenario = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, PREREQUISITE_RULE_UPPER))
    {
        auto scructType = Parse_Structure_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Pre = 1L << scructType;
        }
    }
    else if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        type->Cost = atoi(value);
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        type->IsBuildable = value == "TRUE";
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        type->IsFlammable = value == "TRUE";
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        auto numValue = (unsigned short)atoi(value);
        *valueParseError = numValue < 0 && numValue > UCHAR_MAX;

        if (!*valueParseError)
        {
            type->MaxSpeed = (MPHType)numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, STRENGTH_RULE_UPPER))
    {
        auto numValue = (unsigned short)atoi(value);
        *valueParseError = numValue < 0 && numValue > USHRT_MAX;

        if (!*valueParseError)
        {
            type->MaxStrength = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, HOUSES_RULE_UPPER))
    {
        auto houseType = Parse_House_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Ownable = houseType;
        }
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Primary = weapon;
        }
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Secondary = weapon;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_TechnoType_Rule(lua_State* lua, TechnoTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, BUILD_LEVEL_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %u", type->Level);

        lua_pushnumber(lua, type->Level);
    }
    else if (Strings_Are_Equal(ruleName, SCENARIO_LEVEL_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %u", type->Scenario);

        lua_pushnumber(lua, type->Scenario);
    }
    else if (Strings_Are_Equal(ruleName, PREREQUISITE_RULE_UPPER))
    {
        auto scructType = (StructType)(1L >> type->Pre);
        auto scrutTypeStr = Structure_Type_To_String(scructType);
        
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", scrutTypeStr);

        lua_pushstring(lua, scrutTypeStr);
    }
    else if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %d", type->Cost);

        lua_pushnumber(lua, type->Cost);
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsBuildable));

        lua_pushboolean(lua, type->IsBuildable);
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFlammable));

        lua_pushboolean(lua, type->IsFlammable);
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %u", type->MaxSpeed);

        lua_pushnumber(lua, type->MaxSpeed);
    }
    else if (Strings_Are_Equal(ruleName, STRENGTH_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %u", type->MaxStrength);

        lua_pushnumber(lua, type->MaxStrength);
    }
    else if (Strings_Are_Equal(ruleName, HOUSES_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", type->HouseListCsv);

        lua_pushstring(lua, type->HouseListCsv);
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        auto weaponStr = Weapon_Type_To_String(type->Primary);

        Log_Debug("Read_TechnoType_Rule => Rule value: %s", weaponStr);

        lua_pushstring(lua, weaponStr);
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        auto weaponStr = Weapon_Type_To_String(type->Secondary);

        Log_Debug("Read_TechnoType_Rule => Rule value: %s", weaponStr);

        lua_pushstring(lua, weaponStr);
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

int Lua_Set_Infantry_Rule(lua_State* lua)
{
    Log_Debug("Lua_Set_Infantry_Rule called from Lua");

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

    bool parseError = false;
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    Log_Debug("Lua_Set_Infantry_Rule => infantry type parsed: %s", Infantry_Type_To_String(infantryType));

    auto infantry = (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
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

    Log_Debug("Lua_Set_Infantry_Rule => looking for rule in Techno Type");

    auto ruleMatched = Write_TechnoType_Rule(
        infantry,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Infantry_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Debug("Lua_Get_Infantry_Rule => looking for rule in Infantry Type");

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

    luaL_error(lua, "rule name type passed setInfantryRule was not recognised: %s", ruleName);

    return 0;
}

int Lua_Get_Infantry_Rule(lua_State* lua)
{
    Log_Debug("Lua_Get_Infantry_Rule called from Lua");

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

    bool parseError = false;
    auto infantryType = ParseInfantryType(lua, infantryTypeName, &parseError);

    if (parseError)
    {
        return 0;
    }

    Log_Debug("Lua_Get_Infantry_Rule => infantry type parsed: %s", Infantry_Type_To_String(infantryType));

    auto infantry = (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
    auto ruleName = Get_Lua_Value_As_String(2);

    if (ruleName == NULL)
    {
        luaL_error(lua, "getInfantryRule argument `ruleName` was nil");
        return 0;
    }

    Log_Debug("Lua_Get_Infantry_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Infantry_Rule => Rule matched in Techno Type");
        return 1;
    }

    ruleMatched = Read_Infantry_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Infantry_Rule => Rule matched in Infantry Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getInfantryRule was not recognised: %s", ruleName);

    return 0;
}
