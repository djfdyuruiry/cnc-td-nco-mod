#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Building_Rule(BuildingTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BIBBED_RULE))
    {
        auto isBibbed = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsBibbed), Convert_Boolean_To_String(isBibbed));

            type->IsBibbed = isBibbed;
        }
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE))
    {
        auto isWall = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsWall), Convert_Boolean_To_String(isWall));

            type->IsWall = isWall;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE))
    {
        auto isFactory = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFactory), Convert_Boolean_To_String(isFactory));

            type->IsFactory = isFactory;
        }
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE))
    {
        auto isSimpleDamage = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsSimpleDamage), Convert_Boolean_To_String(isSimpleDamage));

            type->IsSimpleDamage = isSimpleDamage;
        }
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE))
    {
        auto isSturdy = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsSturdy), Convert_Boolean_To_String(isSturdy));

            type->IsSturdy = isSturdy;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE))
    {
        auto isCaptureable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCaptureable), Convert_Boolean_To_String(isCaptureable));

            type->IsCaptureable = isCaptureable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE))
    {
        auto isRegulated = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRegulated), Convert_Boolean_To_String(isRegulated));

            type->IsRegulated = isRegulated;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE))
    {
        auto isUnsellable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsUnsellable), Convert_Boolean_To_String(isUnsellable));

            type->IsUnsellable = isUnsellable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE))
    {
        auto factoryType = Parse_Factory_Type(value, valueParseError, false);

        if (!*valueParseError)
        {
            Log_Trace(
                "Write_Building_Rule => Rule value: %s -> %s",
                Factory_Type_To_String((FactoryType)type->ToBuild),
                Factory_Type_To_String(factoryType)
            );

            type->ToBuild = (RTTIType)factoryType;
        }
    }
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);
    
        if (!*valueParseError)
        {
            auto capacity = strtoul(value, NULL, 10);

            Log_Trace("Write_Building_Rule => Rule value: %u -> %u", type->Capacity, capacity);

            type->Capacity = capacity;
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE))
    {
        *valueParseError = !Is_Int_String(value);
    
        if (!*valueParseError)
        {
            auto power = atoi(value);
            *valueParseError = power < 0;

            if (!*valueParseError)
            {
                Log_Trace("Write_Building_Rule => Rule value: %d -> %d", type->Power, power);

                type->Power = power;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE))
    {
        *valueParseError = !Is_Int_String(value);
    
        if (!*valueParseError)
        {
            auto drain = atoi(value);
            *valueParseError = drain < 0 || drain > INT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_Building_Rule => Rule value: %d -> %d", type->Drain, drain);

                type->Drain = drain;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Building_Rule(BuildingTypeClass* type, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, BIBBED_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsBibbed));

        luaState.WriteBool(type->IsBibbed);
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsWall));

        luaState.WriteBool(type->IsWall);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFactory));

        luaState.WriteBool(type->IsFactory);
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSimpleDamage));

        luaState.WriteBool(type->IsSimpleDamage);
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSturdy));

        luaState.WriteBool(type->IsSturdy);
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCaptureable));

        luaState.WriteBool(type->IsCaptureable);
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRegulated));

        luaState.WriteBool(type->IsRegulated);
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsUnsellable));

        luaState.WriteBool(type->IsUnsellable);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE))
    {
        auto factoryType = Factory_Type_To_String((FactoryType)type->ToBuild);

        Log_Trace("Read_Building_Rule => Rule value: %s", factoryType);

        luaState.WriteString(factoryType);
    }
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %u", type->Capacity);

        luaState.WriteInteger(type->Capacity);
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %d", type->Power);

        luaState.WriteInteger(type->Power);
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE))
    {
        Log_Trace("Read_Building_Rule => Rule value: %d", type->Drain);

        luaState.WriteInteger(type->Drain);
    }
    else
    {
        return false;
    }

    return true;
}

static StructType ParseBuildingType(const char* buildingTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();
    auto upperTypeName = Convert_String_To_Upper_Case(buildingTypeName);

    auto buildingType = Parse_Structure_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaState.RaiseError("building type passed was not recognised: %s", buildingTypeName);

        return STRUCT_NONE;
    }

    return buildingType;
}

static BuildingTypeClass* ResolveBuilding(const char* buildingTypeName)
{
    bool parseError = false;
    auto buildingType = ParseBuildingType(buildingTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("building type parsed: %s -> %s", buildingTypeName, Structure_Type_To_String(buildingType));

    return (BuildingTypeClass*)&BuildingTypeClass::As_Reference(buildingType);
}

static int Lua_Get_Building_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        "Building",
        "buildingTypeName",
        &ResolveBuilding,
        GetRulesInfo().GetBuildingRules(),
        &Read_Building_Rule
    );
}

static int Lua_Set_Building_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        "Building",
        "buildingTypeName",
        &ResolveBuilding,
        GetRulesInfo().GetBuildingRules(),
        &Write_Building_Rule
    );
}

bool Register_Building_Rule_Functions()
{
    LuaRuntime().RegisterApi("building rules", [](ILuaApi& a) {
        a.WithDescription("Building rule info and control functions")

         .WithFunction("getBuildingRule", Lua_Get_Building_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Get the current value of a rule for a given Building type")
              .WithParameter("typeName", [](LuaVariableInfo& p) {
                 p.WithDescription("The building type, e.g. 'NUKE', 'PYLE' etc.")
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

         .WithFunction("setBuildingRule", Lua_Set_Building_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         });
    });

    return true;
}
