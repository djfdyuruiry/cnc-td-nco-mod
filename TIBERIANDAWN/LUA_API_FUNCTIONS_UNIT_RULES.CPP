#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Unit_Rule(UnitTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, CAN_BE_FOUND_IN_CRATE_RULE_UPPER))
    {
        auto isCrateGoodie = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrateGoodie), Convert_Boolean_To_String(isCrateGoodie));

            type->IsCrateGoodie = isCrateGoodie;
        }
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE_UPPER))
    {
        auto isPieceOfEight = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => %s -> %s", Convert_Boolean_To_String(type->IsPieceOfEight), Convert_Boolean_To_String(isPieceOfEight));

            type->IsPieceOfEight = isPieceOfEight;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE_UPPER))
    {
        auto isCrusher = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrusher), Convert_Boolean_To_String(isCrusher));

            type->IsCrusher = isCrusher;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE_UPPER))
    {
        auto isHarvester = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsToHarvest), Convert_Boolean_To_String(isHarvester));

            type->IsToHarvest = isHarvester;
        }
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE_UPPER))
    {
        auto isRadarEquipped = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRadarEquipped), Convert_Boolean_To_String(isRadarEquipped));

            type->IsRadarEquipped = isRadarEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE_UPPER))
    {
        auto hasFireAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFireAnim), Convert_Boolean_To_String(hasFireAnim));

            type->IsFireAnim = hasFireAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE_UPPER))
    {
        auto lockTurret = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsLockTurret), Convert_Boolean_To_String(lockTurret));

            type->IsLockTurret = lockTurret;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE_UPPER))
    {
        auto hasTracks = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTracked), Convert_Boolean_To_String(hasTracks));

            type->IsTracked = hasTracks;
        }
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE_UPPER))
    {
        auto isHuge = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsGigundo), Convert_Boolean_To_String(isHuge));

            type->IsGigundo = isHuge;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE_UPPER))
    {
        auto canCloak = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCloakable), Convert_Boolean_To_String(canCloak));

            type->IsCloakable = canCloak;
        }
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE_UPPER))
    {
        auto constAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsAnimating), Convert_Boolean_To_String(constAnim));

            type->IsAnimating = constAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE_UPPER))
    {
        auto speed = Parse_Unit_Speed_Type(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->Speed, speed);

            type->Speed = speed;
        }
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto rot = atoi(value);
            *valueParseError = rot < 0 || rot > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->ROT, rot);

                type->ROT = rot;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE_UPPER))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto capacity = strtoul(value, NULL, 10);

            Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->TransportCapacity, capacity);

            type->TransportCapacity = capacity;
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
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrateGoodie));

        lua_pushboolean(lua, type->IsCrateGoodie);
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsPieceOfEight));

        lua_pushboolean(lua, type->IsPieceOfEight);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrusher));

        lua_pushboolean(lua, type->IsCrusher);
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsToHarvest));

        lua_pushboolean(lua, type->IsToHarvest);
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRadarEquipped));

        lua_pushboolean(lua, type->IsRadarEquipped);
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFireAnim));

        lua_pushboolean(lua, type->IsFireAnim);
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLockTurret));

        lua_pushboolean(lua, type->IsLockTurret);
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTracked));

        lua_pushboolean(lua, type->IsTracked);
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsGigundo));

        lua_pushboolean(lua, type->IsGigundo);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCloakable));

        lua_pushboolean(lua, type->IsCloakable);
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAnimating));

        lua_pushboolean(lua, type->IsAnimating);
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE_UPPER))
    {
        auto unitSpeed = Unit_Speed_Type_To_String(type->Speed);

        Log_Trace("Read_Unit_Rule => Rule value: %s", unitSpeed);

        lua_pushstring(lua, unitSpeed);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %u", type->ROT);

        lua_pushnumber(lua, type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE_UPPER))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %u", type->TransportCapacity);

        lua_pushnumber(lua, type->ROT);
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

    Log_Trace("unit type parsed: %s -> %s", unitTypeName, Unit_Type_To_String(unitType));

    return (UnitTypeClass*)&UnitTypeClass::As_Reference(unitType);
}

static int Lua_Get_Unit_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        lua,
        "Unit",
        "unitTypeName",
        &ResolveUnit,
        &Read_Unit_Rule
    );
}

static int Lua_Set_Unit_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        lua,
        "Unit",
        "unitTypeName",
        &ResolveUnit,
        &Write_Unit_Rule
    );
}

bool Register_Unit_Rule_Functions()
{
    Log_Debug("Registering Unit Rules Lua API functions");

    Register_Lua_Function("getUnitRule", Lua_Get_Unit_Rule);
    Register_Lua_Function("setUnitRule", Lua_Set_Unit_Rule);

    return true;
}
