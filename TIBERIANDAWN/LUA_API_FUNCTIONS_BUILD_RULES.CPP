#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Building_Rule(BuildingTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BIBBED_RULE_UPPER))
    {
        auto isBibbed = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsBibbed), Convert_Boolean_To_String(isBibbed));

            type->IsBibbed = isBibbed;
        }
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE_UPPER))
    {
        auto isWall = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsWall), Convert_Boolean_To_String(isWall));

            type->IsWall = isWall;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE_UPPER))
    {
        auto isFactory = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFactory), Convert_Boolean_To_String(isFactory));

            type->IsFactory = isFactory;
        }
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE_UPPER))
    {
        auto isSimpleDamage = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsSimpleDamage), Convert_Boolean_To_String(isSimpleDamage));

            type->IsSimpleDamage = isSimpleDamage;
        }
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE_UPPER))
    {
        auto isSturdy = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsSturdy), Convert_Boolean_To_String(isSturdy));

            type->IsSturdy = isSturdy;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE_UPPER))
    {
        auto isCaptureable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCaptureable), Convert_Boolean_To_String(isCaptureable));

            type->IsCaptureable = isCaptureable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE_UPPER))
    {
        auto isRegulated = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRegulated), Convert_Boolean_To_String(isRegulated));

            type->IsRegulated = isRegulated;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE_UPPER))
    {
        auto isUnsellable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsUnsellable), Convert_Boolean_To_String(isUnsellable));

            type->IsUnsellable = isUnsellable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE_UPPER))
    {
        auto factoryType = Parse_Factory_Type(value, valueParseError);

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
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE_UPPER))
    {
        auto capacity = atoi(value);
        *valueParseError = capacity < 0 || capacity > UINT_MAX;

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %u -> %u", type->Capacity, capacity);

            type->Capacity = capacity;
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE_UPPER))
    {
        auto power = atoi(value);
        *valueParseError = power < 0 || power > INT_MAX;

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %d -> %d", type->Power, power);

            type->Power = power;
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE_UPPER))
    {
        auto drain = atoi(value);
        *valueParseError = drain < 0 || drain > INT_MAX;

        if (!*valueParseError)
        {
            Log_Trace("Write_Building_Rule => Rule value: %d -> %d", type->Drain, drain);

            type->Drain = drain;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Building_Rule(lua_State* lua, BuildingTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, BIBBED_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsBibbed));

        lua_pushboolean(lua, type->IsBibbed);
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsWall));

        lua_pushboolean(lua, type->IsWall);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFactory));

        lua_pushboolean(lua, type->IsFactory);
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSimpleDamage));

        lua_pushboolean(lua, type->IsSimpleDamage);
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSturdy));

        lua_pushboolean(lua, type->IsSturdy);
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCaptureable));

        lua_pushboolean(lua, type->IsCaptureable);
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRegulated));

        lua_pushboolean(lua, type->IsRegulated);
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsUnsellable));

        lua_pushboolean(lua, type->IsUnsellable);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE_UPPER))
    {
        auto factoryType = Factory_Type_To_String((FactoryType)type->ToBuild);

        Log_Trace("Read_Building_Rule => Rule value: %s", factoryType);

        lua_pushstring(lua, factoryType);
    }
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %u", type->Capacity);

        lua_pushnumber(lua, type->Capacity);
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %d", type->Power);

        lua_pushnumber(lua, type->Power);
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE_UPPER))
    {
        Log_Trace("Read_Building_Rule => Rule value: %d", type->Drain);

        lua_pushnumber(lua, type->Drain);
    }
    else
    {
        return false;
    }

    return true;
}

static StructType ParseBuildingType(lua_State* lua, const char* buildingTypeName, bool* parseError)
{
    auto buildingType = Parse_Structure_Type(
        Convert_String_To_Upper_Case(buildingTypeName),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "building type passed was not recognised: %s", buildingTypeName);
        return STRUCT_NONE;
    }

    return buildingType;
}

static BuildingTypeClass* ResolveBuilding(lua_State* lua, const char* buildingTypeName)
{
    bool parseError = false;
    auto buildingType = ParseBuildingType(lua, buildingTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("building type parsed: %s -> %s", buildingTypeName, Structure_Type_To_String(buildingType));

    return (BuildingTypeClass*)&BuildingTypeClass::As_Reference(buildingType);
}

static int Lua_Get_Building_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Building_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getBuildingRule requires exactly two arguments");
    }

    auto buildingTypeName = Get_Lua_Value_As_String(1);

    if (buildingTypeName == NULL)
    {
        luaL_error(lua, "getBuildingRule argument `buildingTypeName` was nil");
        return 0;
    }


    auto building = ResolveBuilding(lua, buildingTypeName);

    if (building == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);

    if (ruleName == NULL)
    {
        luaL_error(lua, "getBuildingRule argument `ruleName` was nil");
        return 0;
    }

    Log_Debug("Lua_Get_Building_Rule => attempting to read value of rule '%s'", ruleName);

    Log_Trace("Lua_Get_Building_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, building, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Building_Rule => Rule matched in Techno Type");
        return 1;
    }

    Log_Trace("Lua_Get_Building_Rule => looking for rule in Building Type");

    ruleMatched = Read_Building_Rule(lua, building, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Building_Rule => Rule matched in Building Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getBuildingRule was not recognised: %s", ruleName);

    return 0;
}

static int Lua_Set_Building_Rule(lua_State* lua)
{
    Log_Trace("Lua_Set_Building_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setUnitRule requires exactly three arguments");
        return 0;
    }

    auto buildingTypeName = Get_Lua_Value_As_String(1);

    if (buildingTypeName == NULL)
    {
        luaL_error(lua, "setBuildingRule argument `buildingTypeName` was nil");
        return 0;
    }

    auto building = ResolveBuilding(lua, buildingTypeName);

    if (building == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto ruleValue = Get_Lua_Value_As_String(3);

    if (ruleName == NULL)
    {
        luaL_error(lua, "setBuildingRule argument `ruleName` was nil");
        return 0;
    }

    if (ruleValue == NULL)
    {
        luaL_error(lua, "setBuildingRule argument `ruleValue` was nil");
        return 0;
    }

    Log_Debug("Lua_Set_Building_Rule => attempting to set rule '%s' to value: %s", ruleName, ruleValue);

    Log_Trace("Lua_Set_Building_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        building,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        ruleValue,
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Building_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Trace("Lua_Set_Building_Rule => looking for rule in Building Type");

        ruleMatched = Write_Building_Rule(
            building,
            Convert_String_To_Upper_Case(ruleName),
            Convert_String_To_Upper_Case(ruleValue),
            &parseError
        );

        if (ruleMatched && !parseError)
        {
            Log_Debug("Lua_Set_Building_Rule => Rule matched in Building Type");
            return 1;
        }
    }

    if (parseError)
    {
        luaL_error(lua, "value for rule `%s` passed to setBuildingRule was not valid: %s", ruleName, ruleValue);

        return 0;
    }

    luaL_error(lua, "rule name passed setBuildingRule was not recognised: %s", ruleName);

    return 0;
}

bool Register_Building_Rule_Functions()
{
    Log_Debug("Registering Building Rules Lua API functions");

    Register_Lua_Function("getBuildingRule", Lua_Get_Building_Rule);
    Register_Lua_Function("setBuildingRule", Lua_Set_Building_Rule);

    return true;
}
