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

static bool Read_Aircraft_Rule(AircraftTypeClass* type, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFixedWing));

        luaState.WriteBool(type->IsFixedWing);
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLandable));

        luaState.WriteBool(type->IsLandable);
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRotorEquipped));

        luaState.WriteBool(type->IsRotorEquipped);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %u", type->ROT);

        luaState.WriteInteger(type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE))
    {
        Log_Trace("Read_Aircraft_Rule => Rule value: %u", type->TransportCapacity);

        luaState.WriteInteger(type->TransportCapacity);
    }
    else
    {
        return false;
    }

    return true;
}

static AircraftType ParseAircraftType(const char* aircraftTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();

    auto upperTypeName = Convert_String_To_Upper_Case(aircraftTypeName);

    auto aircraftType = Parse_Aircraft_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaState.RaiseError("aircraft type passed was not recognised: %s", aircraftTypeName);

        return AIRCRAFT_NONE;
    }

    return aircraftType;
}

static AircraftTypeClass* ResolveAircraft(const char* aircraftTypeName)
{
    auto& luaState = LuaRuntime().GetState();

    bool parseError = false;
    auto aircraftType = ParseAircraftType(aircraftTypeName, &parseError);

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
        "Aircraft",
        "aircraftTypeName",
        &ResolveAircraft,
        GetRulesInfo().GetAircraftRules(),
        &Write_Aircraft_Rule
    );
}

bool Register_Aircraft_Rule_Functions()
{
    LuaRuntime().RegisterApi("aircraft rules", [](ILuaApi& a) {
        a.WithDescription("Aircraft rule info and control functions")

         .WithFunction("getAircraftRule", Lua_Get_Aircraft_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Get the current value of a rule for a given Aircraft type")
              .WithParameter("typeName", [](LuaVariableInfo& p) {
                 p.WithDescription("The aircraft type, e.g. 'ORCA', 'A10' etc.")
                  .WithType(LuaType::String);
              })
              .WithParameter("ruleName", [](LuaVariableInfo& p) {
                 p.WithDescription("The rule value to return")
                  .WithType(LuaType::String);
              })
              .WithReturnValue("ruleValue", [](LuaVariableInfo& p) {
                 p.WithDescription("The rule value, if present, otherwise nil")
                  .WithType(LuaType::Any);
              });
         })

         .WithFunction("setAircraftRule", Lua_Set_Aircraft_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Set the current value of a rule for a given Aircraft type")
              .WithParameter("typeName", [](LuaVariableInfo& p) {
                 p.WithDescription("The aircraft type, e.g. 'ORCA', 'A10' etc.")
                  .WithType(LuaType::String);
              })
              .WithParameter("ruleName", [](LuaVariableInfo& p) {
                 p.WithDescription("The rule to update")
                  .WithType(LuaType::String);
              })
              .WithParameter("ruleValue", [](LuaVariableInfo& p) {
                 p.WithDescription("The rule value to set")
                  .WithType(LuaType::Any);
              });
         });
     });
    return true;
}
