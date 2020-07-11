#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Aircraft_Rule(AircraftTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE_UPPER))
    {
        auto isFixedWing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFixedWing), Convert_Boolean_To_String(isFixedWing));

            type->IsFixedWing = isFixedWing;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE_UPPER))
    {
        auto isLandable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsLandable), Convert_Boolean_To_String(isLandable));

            type->IsLandable = isLandable;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE_UPPER))
    {
        auto hasRotor = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRotorEquipped), Convert_Boolean_To_String(hasRotor));

            type->IsRotorEquipped = hasRotor;
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
                Log_Trace("Write_Aircraft_Rule => Rule value: %u -> %u", type->ROT, rot);

                type->ROT = rot;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto capacity = atoi(value);
            *valueParseError = capacity < 0 || capacity > UINT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_Aircraft_Rule => Rule value: %u -> %u", type->TransportCapacity, capacity);

                type->TransportCapacity = capacity;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Aircraft_Rule(lua_State* lua, AircraftTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE_UPPER))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFixedWing));

        lua_pushboolean(lua, type->IsFixedWing);
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE_UPPER))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLandable));

        lua_pushboolean(lua, type->IsLandable);
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE_UPPER))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRotorEquipped));

        lua_pushboolean(lua, type->IsRotorEquipped);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %u", type->ROT);

        lua_pushnumber(lua, type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE_UPPER))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %u", type->TransportCapacity);

        lua_pushnumber(lua, type->TransportCapacity);
    }
    else
    {
        return false;
    }

    return true;
}

static AircraftType ParseAircraftType(lua_State* lua, const char* aircraftTypeName, bool* parseError)
{
    auto aircraftType = Parse_Aircraft_Type(
        Convert_String_To_Upper_Case(aircraftTypeName),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "aircraft type passed was not recognised: %s", aircraftTypeName);
        return AIRCRAFT_NONE;
    }

    return aircraftType;
}

static AircraftTypeClass* ResolveAircraft(lua_State* lua, const char* aircraftTypeName)
{
    bool parseError = false;
    auto aircraftType = ParseAircraftType(lua, aircraftTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("aircraft type parsed: %s -> %s", aircraftTypeName, Aircraft_Type_To_String(aircraftType));

    return (AircraftTypeClass*)&AircraftTypeClass::As_Reference(aircraftType);
}

static int Lua_Get_Aircraft_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Aircraft_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getAircraftRule requires exactly two arguments");
    }

    auto aircraftTypeName = Get_Lua_Value_As_String(1);

    if (aircraftTypeName == NULL)
    {
        luaL_error(lua, "getAircraftRule argument `aircraftTypeName` was nil");
        return 0;
    }


    auto aircraft = ResolveAircraft(lua, aircraftTypeName);

    if (aircraft == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);

    if (ruleName == NULL)
    {
        luaL_error(lua, "getAircraftRule argument `ruleName` was nil");
        return 0;
    }

    Log_Debug("Lua_Get_Aircraft_Rule => attempting to read value of rule '%s'", ruleName);

    Log_Trace("Lua_Get_Aircraft_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, aircraft, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Aircraft_Rule => Rule matched in Techno Type");
        return 1;
    }

    Log_Trace("Lua_Get_Aircraft_Rule => looking for rule in Aircraft Type");

    ruleMatched = Read_Aircraft_Rule(lua, aircraft, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Aircraft_Rule => Rule matched in Aircraft Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getAircraftRule was not recognised: %s", ruleName);

    return 0;
}

static int Lua_Set_Aircraft_Rule(lua_State* lua)
{
    Log_Trace("Lua_Set_Aircraft_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setAircraftRule requires exactly three arguments");
        return 0;
    }

    auto aircraftTypeName = Get_Lua_Value_As_String(1);

    if (aircraftTypeName == NULL)
    {
        luaL_error(lua, "setAircraftRule argument `aircraftTypeName` was nil");
        return 0;
    }

    auto aircraft = ResolveAircraft(lua, aircraftTypeName);

    if (aircraft == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto ruleValue = Get_Lua_Value_As_String(3);

    if (ruleName == NULL)
    {
        luaL_error(lua, "setAircraftRule argument `ruleName` was nil");
        return 0;
    }

    if (ruleValue == NULL)
    {
        luaL_error(lua, "setAircraftRule argument `ruleValue` was nil");
        return 0;
    }

    Log_Debug("Lua_Set_Aircraft_Rule => attempting to set rule '%s' to value: %s", ruleName, ruleValue);

    Log_Trace("Lua_Set_Aircraft_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        aircraft,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        ruleValue,
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Unit_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Trace("Lua_Set_Unit_Rule => looking for rule in Aircraft Type");

        ruleMatched = Write_Aircraft_Rule(
            aircraft,
            Convert_String_To_Upper_Case(ruleName),
            Convert_String_To_Upper_Case(ruleValue),
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_Unit_Rule => Rule matched in Aircraft Type");
            return 1;
        }
    }

    if (parseError)
    {
        luaL_error(lua, "value for rule `%s` passed to setAircraftRule was not valid: %s", ruleName, ruleValue);

        return 0;
    }

    luaL_error(lua, "rule name passed setAircraftRule was not recognised: %s", ruleName);

    return 0;
}

bool Register_Aircraft_Rule_Functions()
{
    Log_Debug("Registering Aircraft Rules Lua API functions");

    Register_Lua_Function("getAircraftRule", Lua_Get_Aircraft_Rule);
    Register_Lua_Function("setAircraftRule", Lua_Set_Aircraft_Rule);

    return true;
}
