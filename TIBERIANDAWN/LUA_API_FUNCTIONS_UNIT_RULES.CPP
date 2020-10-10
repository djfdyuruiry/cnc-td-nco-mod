#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Unit_Rule(UnitTypeClass* type, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, CAN_BE_FOUND_IN_CRATE_RULE))
    {
        auto isCrateGoodie = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrateGoodie), Convert_Boolean_To_String(isCrateGoodie));

            type->IsCrateGoodie = isCrateGoodie;
        }
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE))
    {
        auto isPieceOfEight = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => %s -> %s", Convert_Boolean_To_String(type->IsPieceOfEight), Convert_Boolean_To_String(isPieceOfEight));

            type->IsPieceOfEight = isPieceOfEight;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE))
    {
        auto isCrusher = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrusher), Convert_Boolean_To_String(isCrusher));

            type->IsCrusher = isCrusher;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE))
    {
        auto isHarvester = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsToHarvest), Convert_Boolean_To_String(isHarvester));

            type->IsToHarvest = isHarvester;
        }
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE))
    {
        auto isRadarEquipped = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRadarEquipped), Convert_Boolean_To_String(isRadarEquipped));

            type->IsRadarEquipped = isRadarEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE))
    {
        auto hasFireAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFireAnim), Convert_Boolean_To_String(hasFireAnim));

            type->IsFireAnim = hasFireAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE))
    {
        auto lockTurret = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsLockTurret), Convert_Boolean_To_String(lockTurret));

            type->IsLockTurret = lockTurret;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE))
    {
        auto hasTracks = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTracked), Convert_Boolean_To_String(hasTracks));

            type->IsTracked = hasTracks;
        }
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE))
    {
        auto isHuge = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsGigundo), Convert_Boolean_To_String(isHuge));

            type->IsGigundo = isHuge;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE))
    {
        auto canCloak = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCloakable), Convert_Boolean_To_String(canCloak));

            type->IsCloakable = canCloak;
        }
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE))
    {
        auto constAnim = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsAnimating), Convert_Boolean_To_String(constAnim));

            type->IsAnimating = constAnim;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE))
    {
        auto speed = Parse_Unit_Speed_Type(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->Speed, speed);

            type->Speed = speed;
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
                Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->ROT, rot);

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

            Log_Trace("Write_Unit_Rule => Rule value: %u -> %u", type->TransportCapacity, capacity);

            type->TransportCapacity = capacity;
        }
    }
    else if (Strings_Are_Equal(ruleName, SELF_REPAIR_RULE))
    {
        auto canSelfRepair = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->CanSelfRepair), Convert_Boolean_To_String(canSelfRepair));

            type->CanSelfRepair = canSelfRepair;
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Unit_Rule(UnitTypeClass* type, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, CAN_BE_FOUND_IN_CRATE_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrateGoodie));

        luaState.WriteBool(type->IsCrateGoodie);
    }
    else if (Strings_Are_Equal(ruleName, LIMITED_FACINGS_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsPieceOfEight));

        luaState.WriteBool(type->IsPieceOfEight);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CRUSH_INFANTRY_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrusher));

        luaState.WriteBool(type->IsCrusher);
    }
    else if (Strings_Are_Equal(ruleName, CAN_HARVEST_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsToHarvest));

        luaState.WriteBool(type->IsToHarvest);
    }
    else if (Strings_Are_Equal(ruleName, RADAR_EQUIPPED_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRadarEquipped));

        luaState.WriteBool(type->IsRadarEquipped);
    }
    else if (Strings_Are_Equal(ruleName, HAS_FIRE_ANIMATION_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFireAnim));

        luaState.WriteBool(type->IsFireAnim);
    }
    else if (Strings_Are_Equal(ruleName, LOCK_TURRET_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLockTurret));

        luaState.WriteBool(type->IsLockTurret);
    }
    else if (Strings_Are_Equal(ruleName, HAS_TRACKS_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTracked));

        luaState.WriteBool(type->IsTracked);
    }
    else if (Strings_Are_Equal(ruleName, HUGE_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsGigundo));

        luaState.WriteBool(type->IsGigundo);
    }
    else if (Strings_Are_Equal(ruleName, CAN_CLOAK_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCloakable));

        luaState.WriteBool(type->IsCloakable);
    }
    else if (Strings_Are_Equal(ruleName, CONSTANT_ANIMATION_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsAnimating));

        luaState.WriteBool(type->IsAnimating);
    }
    else if (Strings_Are_Equal(ruleName, UNIT_SPEED_RULE))
    {
        auto unitSpeed = Unit_Speed_Type_To_String(type->Speed);

        Log_Trace("Read_Unit_Rule => Rule value: %s", unitSpeed);

        luaState.WriteString(unitSpeed);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %u", type->ROT);

        luaState.WriteInteger(type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORT_CAPACITY_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %u", type->TransportCapacity);

        luaState.WriteInteger(type->ROT);
    }
    else if (Strings_Are_Equal(ruleName, SELF_REPAIR_RULE))
    {
        Log_Trace("Read_Unit_Rule => Rule value: %s", Convert_Boolean_To_String(type->CanSelfRepair));

        luaState.WriteBool(type->CanSelfRepair);
    }
    else
    {
        return false;
    }

    return true;
}

static UnitType ParseUnitType(const char* unitTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();
    auto upperTypeName = Convert_String_To_Upper_Case(unitTypeName);

    auto unitType = Parse_Unit_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaState.RaiseError("unit type passed was not recognised: %s", unitTypeName);
        return UNIT_NONE;
    }

    return unitType;
}

static UnitTypeClass* ResolveUnit(const char* unitTypeName)
{
    bool parseError = false;
    auto unitType = ParseUnitType(unitTypeName, &parseError);

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
        "Unit",
        "unitTypeName",
        &ResolveUnit,
        GetRulesInfo().GetUnitRules(),
        &Read_Unit_Rule
    );
}

static int Lua_Set_Unit_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        "Unit",
        "unitTypeName",
        &ResolveUnit,
        GetRulesInfo().GetUnitRules(),
        &Write_Unit_Rule
    );
}

bool Register_Unit_Rule_Functions()
{
    LuaRuntime().RegisterApi("unit rules", [](ILuaApi& a) {
        a.WithDescription("Unit info and control functions")

         .WithFunction("getUnitRule", Lua_Get_Unit_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         })

         .WithFunction("setUnitRule", Lua_Set_Unit_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         });
    });

    return true;
}
