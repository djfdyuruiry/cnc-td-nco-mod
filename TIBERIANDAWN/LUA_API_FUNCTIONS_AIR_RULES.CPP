#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Aircraft_Rule(AircraftTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE))
    {
        auto isFixedWing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFixedWing), Convert_Boolean_To_String(isFixedWing));

            type->IsFixedWing = isFixedWing;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE))
    {
        auto isLandable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsLandable), Convert_Boolean_To_String(isLandable));

            type->IsLandable = isLandable;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE))
    {
        auto hasRotor = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Aircraft_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRotorEquipped), Convert_Boolean_To_String(hasRotor));

            type->IsRotorEquipped = hasRotor;
        }
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
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
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto capacity = strtoul(value, NULL, 10);

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
    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFixedWing));

        lua_pushboolean(lua, type->IsFixedWing);
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLandable));

        lua_pushboolean(lua, type->IsLandable);
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRotorEquipped));

        lua_pushboolean(lua, type->IsRotorEquipped);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %u", type->ROT);

        lua_pushnumber(lua, type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE))
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
    auto upperTypeName = Convert_String_To_Upper_Case(aircraftTypeName);

    auto aircraftType = Parse_Aircraft_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

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
    return Lua_Get_Rule(
        lua,
        "Aircraft",
        "aircraftTypeName",
        &ResolveAircraft,
        GetRulesInfo().GetAircraftRules(),
        &Read_Aircraft_Rule
    );
}

static int Lua_Set_Aircraft_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        lua,
        "Aircraft",
        "aircraftTypeName",
        &ResolveAircraft,
        GetRulesInfo().GetAircraftRules(),
        &Write_Aircraft_Rule
    );
}

bool Register_Aircraft_Rule_Functions()
{
    Log_Debug("Registering Aircraft Rules Lua API functions");

    Register_Lua_Function("getAircraftRule", Lua_Get_Aircraft_Rule);
    Register_Lua_Function("setAircraftRule", Lua_Set_Aircraft_Rule);

    return true;
}
