#include "function.h"

static bool Write_Unit_Rule(UnitTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, CAN_BE_FOUND_IN_CRATE_RULE_UPPER))
    {
        auto isCrateGoodie = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCrateGoodie = isCrateGoodie;
        }
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE_UPPER))
    {
        auto isPieceOfEight = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsPieceOfEight = isPieceOfEight;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE_UPPER))
    {
        auto isCrusher = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCrusher = isCrusher;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE_UPPER))
    {
        auto isHarvester = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsToHarvest = isHarvester;
        }
    }
    else if (Strings_Are_Equal(ruleName, CHUNKY_SHAPE_RULE_UPPER))
    {
        auto isChunky = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsChunkyShape = isChunky;
        }
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE_UPPER))
    {
        auto isRadarEquipped = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsRadarEquipped = isRadarEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE_UPPER))
    {
        auto hasFireAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFireAnim = hasFireAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE_UPPER))
    {
        auto lockTurret = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsLockTurret = lockTurret;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE_UPPER))
    {
        auto hasTracks = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsTracked = hasTracks;
        }
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE_UPPER))
    {
        auto isHuge = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsGigundo = isHuge;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE_UPPER))
    {
        auto canCloak = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCloakable = canCloak;
        }
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE_UPPER))
    {
        auto constAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsAnimating = constAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE_UPPER))
    {
        auto speed = Parse_Unit_Speed_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Speed = speed;
        }
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        auto rot = (unsigned char) atoi(value);
        *valueParseError = rot < 0 || rot > UCHAR_MAX;

        if (!*valueParseError)
        {
            type->ROT = rot;
        }
    }
    else if (Strings_Are_Equal(ruleName, MAX_SIZE_RULE_UPPER))
    {
        auto maxSize = atoi(value);
        *valueParseError = maxSize < 1;

        if (!*valueParseError)
        {
            type->MaxSize = maxSize;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Unit_Rule(lua_State* lua, UnitTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, CAN_BE_FOUND_IN_CRATE_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrateGoodie));

        lua_pushboolean(lua, type->IsCrateGoodie);
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsPieceOfEight));

        lua_pushboolean(lua, type->IsPieceOfEight);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrusher));

        lua_pushboolean(lua, type->IsCrusher);
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsToHarvest));

        lua_pushboolean(lua, type->IsToHarvest);
    }
    else if (Strings_Are_Equal(ruleName, CHUNKY_SHAPE_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsChunkyShape));

        lua_pushboolean(lua, type->IsChunkyShape);
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRadarEquipped));

        lua_pushboolean(lua, type->IsRadarEquipped);
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFireAnim));

        lua_pushboolean(lua, type->IsFireAnim);
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLockTurret));

        lua_pushboolean(lua, type->IsLockTurret);
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTracked));

        lua_pushboolean(lua, type->IsTracked);
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsGigundo));

        lua_pushboolean(lua, type->IsGigundo);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCloakable));

        lua_pushboolean(lua, type->IsCloakable);
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAnimating));

        lua_pushboolean(lua, type->IsAnimating);
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE_UPPER))
    {
        auto unitSpeed = Unit_Speed_Type_To_String(type->Speed);
     
        Log_Debug("Read_Unit_Rule => Rule value: %s", unitSpeed);

        lua_pushstring(lua, unitSpeed);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %u", type->ROT);

        lua_pushnumber(lua, type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, MAX_SIZE_RULE_UPPER))
    {
        Log_Debug("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->MaxSize));

        lua_pushnumber(lua, type->MaxSize);
    }
    else
    {
        return false;
    }

    return true;
}

static bool Write_Infantry_Rule(InfantryTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, FEMALE_RULE_UPPER))
    {
        auto isFemale = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFemale = isFemale;
        }
    }
    else if(Strings_Are_Equal(ruleName, CRAWLING_RULE_UPPER))
    {
        auto isCrawling = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCrawling = isCrawling;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CAPTURE_RULE_UPPER))
    {
        auto isCapture = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCapture = isCapture;
        }
    }
    else if (Strings_Are_Equal(ruleName, FRAIDY_CAT_RULE_UPPER))
    {
        auto isFraidyCat = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFraidyCat = isFraidyCat;
        }
    }
    else if (Strings_Are_Equal(ruleName, CIVILIAN_RULE_UPPER))
    {
        auto isCivilian = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCivilian = isCivilian;
        }
    }
    else if (Strings_Are_Equal(ruleName, AVOIDS_TIBERIUM_RULE_UPPER))
    {
        auto isAvoidingTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsAvoidingTiberium = isAvoidingTiberium;
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
        auto isBuildable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsBuildable = isBuildable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        auto isFlammable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFlammable = isFlammable;
        }
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
        auto scructType = Prerequisite_To_Structure_Type(type->Pre);
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

static UnitType ParseUnitType(lua_State* lua, const char* unitTypeName, bool* parseError)
{
    auto unitType = Parse_Unit_Type(
        Convert_String_To_Upper_Case(unitTypeName),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "unit type passed was not recognised: %s", unitTypeName);
        return UNIT_NONE;
    }

    return unitType;
}

static UnitTypeClass* ResolveUnit(lua_State* lua, const char* unitTypeName)
{
    bool parseError = false;
    auto unitType = ParseUnitType(lua, unitTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Debug("unit type parsed: %s", Unit_Type_To_String(unitType));

    return (UnitTypeClass*)&UnitTypeClass::As_Reference(unitType);
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

    Log_Debug("infantry type parsed: %s", Infantry_Type_To_String(infantryType));

    return (InfantryTypeClass*)&InfantryTypeClass::As_Reference(infantryType);
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

    Log_Debug("Lua_Set_Infantry_Rule => looking for rule in Techno Type");

    bool parseError = false;
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

    luaL_error(lua, "rule name passed setInfantryRule was not recognised: %s", ruleName);

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

int Lua_Set_Unit_Rule(lua_State* lua)
{
    Log_Debug("Lua_Set_Unit_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setUnitRule requires exactly three arguments");
        return 0;
    }

    auto unitTypeName = Get_Lua_Value_As_String(1);

    if (unitTypeName == NULL)
    {
        luaL_error(lua, "setUnitRule argument `infantryTypeName` was nil");
        return 0;
    }

    auto unit = ResolveUnit(lua, unitTypeName);

    if (unit == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto ruleValue = Get_Lua_Value_As_String(3);

    if (ruleName == NULL)
    {
        luaL_error(lua, "setUnitRule argument `ruleName` was nil");
        return 0;
    }

    if (ruleValue == NULL)
    {
        luaL_error(lua, "setUnitRule argument `ruleValue` was nil");
        return 0;
    }

    Log_Debug("Lua_Set_Unit_Rule => attempting to set rule '%s' to value: %s", ruleName, ruleValue);

    Log_Debug("Lua_Set_Unit_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        unit,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Unit_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Debug("Lua_Set_Unit_Rule => looking for rule in Unit Type");

        ruleMatched = Write_Unit_Rule(
            unit,
            Convert_String_To_Upper_Case(ruleName),
            Convert_String_To_Upper_Case(ruleValue),
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_Unit_Rule => Rule matched in Unit Type");
            return 1;
        }
    }

    if (parseError)
    {
        luaL_error(lua, "value for rule `%s` passed to setUnitRule was not valid: %s", ruleName, ruleValue);

        return 0;
    }

    luaL_error(lua, "rule name passed setUnitRule was not recognised: %s", ruleName);

    return 0;
}

int Lua_Get_Unit_Rule(lua_State* lua)
{
    Log_Debug("Lua_Get_Unit_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getUnitRule requires exactly two arguments");
    }

    auto unitTypeName = Get_Lua_Value_As_String(1);

    if (unitTypeName == NULL)
    {
        luaL_error(lua, "getUnitRule argument `unitTypeName` was nil");
        return 0;
    }


    auto unit = ResolveUnit(lua, unitTypeName);

    if (unit == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);

    if (ruleName == NULL)
    {
        luaL_error(lua, "getUnitRule argument `ruleName` was nil");
        return 0;
    }

    Log_Debug("Lua_Get_Unit_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, unit, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Unit_Rule => Rule matched in Techno Type");
        return 1;
    }

    ruleMatched = Read_Unit_Rule(lua, unit, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Unit_Rule => Rule matched in Unit Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getUnitRule was not recognised: %s", ruleName);

    return 0;
}
