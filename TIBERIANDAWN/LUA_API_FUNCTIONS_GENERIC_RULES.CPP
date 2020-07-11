#include "function.h"

bool Write_TechnoType_Rule(TechnoTypeClass* type, const char* ruleName, const char* uppercaseValue, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BUILD_LEVEL_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < 1 && numValue > 99;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->Level, numValue);

                type->Level = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, SCENARIO_LEVEL_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < 1 && numValue > 99;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->Scenario, numValue);

                type->Scenario = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, PREREQUISITE_RULE_UPPER))
    {
        auto scructType = Parse_Structure_Type(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            auto prerequisite = Structure_Type_To_Prerequisite(scructType, valueParseError);

            if (!*valueParseError)
            {
                Log_Trace(
                    "Write_TechnoType_Rule => Rule value: %s -> %s",
                    Prerequisite_To_String(type->Pre),
                    Prerequisite_To_String(prerequisite)
                );

                type->Pre = prerequisite;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);

            Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->Cost, numValue);

            type->Cost = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        auto isBuildable = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->IsBuildable, isBuildable);

            type->IsBuildable = isBuildable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        auto isFlammable = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsFlammable), Convert_Boolean_To_String(isFlammable));

            type->IsFlammable = isFlammable;
        }
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < 0 && numValue > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->MaxSpeed, numValue);

                type->MaxSpeed = (MPHType)numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, STRENGTH_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < 0 && numValue > USHRT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", type->MaxStrength, numValue);

                type->MaxStrength = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, HOUSES_RULE_UPPER))
    {
        auto owner = Parse_House_Name_List_Csv(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", type->HouseListCsv, uppercaseValue);

            type->Ownable = owner;
            type->HouseListCsv = strdup(uppercaseValue); // ensure house list csv is up-to-date for future reference
        }
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Weapon_Type_To_String(type->Primary), Weapon_Type_To_String(weapon));

            type->Primary = weapon;

            type->Calculate_Risk(); // make sure the Risk value now reflects the new primary weapon
        }
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        auto weapon = Parse_Weapon_Type(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Weapon_Type_To_String(type->Secondary), Weapon_Type_To_String(weapon));

            type->Secondary = weapon;
        }
    }
    else if (Strings_Are_Equal(ruleName, ARMOR_RULE_UPPER))
    {
        auto armourType = Parse_Armor_Type(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %u -> %u", Armor_Type_To_String(type->Armor), Armor_Type_To_String(armourType));

            type->Armor = armourType;
        }
    }
    else if (Strings_Are_Equal(ruleName, AMMO_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < -1 && numValue > INT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %d -> %d", type->MaxAmmo, numValue);

                type->MaxAmmo = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, SIGHT_RANGE_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < -1 && numValue > INT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %d -> %d", type->SightRange, numValue);

                type->SightRange = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, REWARD_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(uppercaseValue);

        if (!*valueParseError)
        {
            auto numValue = atoi(uppercaseValue);
            *valueParseError = numValue < -1 && numValue > INT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_TechnoType_Rule => Rule value: %d -> %d", type->Reward, numValue);

                type->Reward = numValue;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, DETECT_CLOAKED_OBJECTS_RULE_UPPER))
    {
        auto isScanner = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsScanner), Convert_Boolean_To_String(isScanner));

            type->IsScanner = isScanner;
        }
    }
    else if (Strings_Are_Equal(ruleName, CRUSHABLE_RULE_UPPER))
    {
        auto isCrushable = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrushable), Convert_Boolean_To_String(isCrushable));

            type->IsCrushable = isCrushable;
        }
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORTER_RULE_UPPER))
    {
        auto isTransporter = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTransporter), Convert_Boolean_To_String(isTransporter));

            type->IsTransporter = isTransporter;
        }
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", type->FriendlyName, originalValue);

        type->FriendlyName = strdup(originalValue);
    }
    else if (Strings_Are_Equal(ruleName, REPAIRABLE_RULE_UPPER))
    {
        auto isRepairable = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsRepairable), Convert_Boolean_To_String(isRepairable));

            type->IsRepairable = isRepairable;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_CREW_RULE_UPPER))
    {
        auto hasCrew = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsCrew), Convert_Boolean_To_String(hasCrew));

            type->IsCrew = hasCrew;
        }
    }
    else if (Strings_Are_Equal(ruleName, LEADER_RULE_UPPER))
    {
        auto isLeader = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsLeader), Convert_Boolean_To_String(isLeader));

            type->IsLeader = isLeader;
        }
    }
    else if (Strings_Are_Equal(ruleName, TWO_SHOOTER_RULE_UPPER))
    {
        auto isTwoShooter = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTwoShooter), Convert_Boolean_To_String(isTwoShooter));

            type->IsTwoShooter = isTwoShooter;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_THEATER_GFX_RULE_UPPER))
    {
        auto hasTheaterGfx = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTheater), Convert_Boolean_To_String(hasTheaterGfx));

            type->IsTheater = hasTheaterGfx;
        }
    }
    else if (Strings_Are_Equal(ruleName, TURRED_EQUIPPED_RULE_UPPER))
    {
        auto turretEquipped = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsTurretEquipped), Convert_Boolean_To_String(turretEquipped));

            type->IsTurretEquipped = turretEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, SHOW_NAME_RULE_UPPER))
    {
        auto showName = Parse_Boolean(uppercaseValue, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(type->IsNominal), Convert_Boolean_To_String(showName));

            type->IsNominal = showName;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Read_TechnoType_Rule(lua_State* lua, TechnoTypeClass* type, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, BUILD_LEVEL_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %u", type->Level);

        lua_pushnumber(lua, type->Level);
    }
    else if (Strings_Are_Equal(ruleName, SCENARIO_LEVEL_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %u", type->Scenario);

        lua_pushnumber(lua, type->Scenario);
    }
    else if (Strings_Are_Equal(ruleName, PREREQUISITE_RULE_UPPER))
    {
        auto structType = Prerequisite_To_Structure_Type(type->Pre);
        auto structTypeStr = Structure_Type_To_String(structType);

        Log_Trace("Read_TechnoType_Rule => Rule value: %s", structTypeStr);

        lua_pushstring(lua, structTypeStr);
    }
    else if (Strings_Are_Equal(ruleName, COST_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %d", type->Cost);

        lua_pushnumber(lua, type->Cost);
    }
    else if (Strings_Are_Equal(ruleName, BUILDABLE_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsBuildable));

        lua_pushboolean(lua, type->IsBuildable);
    }
    else if (Strings_Are_Equal(ruleName, FLAMMABLE_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFlammable));

        lua_pushboolean(lua, type->IsFlammable);
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %u", type->MaxSpeed);

        lua_pushnumber(lua, type->MaxSpeed);
    }
    else if (Strings_Are_Equal(ruleName, STRENGTH_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %u", type->MaxStrength);

        lua_pushnumber(lua, type->MaxStrength);
    }
    else if (Strings_Are_Equal(ruleName, HOUSES_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", type->HouseListCsv);

        lua_pushstring(lua, type->HouseListCsv);
    }
    else if (Strings_Are_Equal(ruleName, PRIMARY_WEAPON_RULE_UPPER))
    {
        auto weaponStr = Weapon_Type_To_String(type->Primary);

        Log_Trace("Read_TechnoType_Rule => Rule value: %s", weaponStr);

        lua_pushstring(lua, weaponStr);
    }
    else if (Strings_Are_Equal(ruleName, SECONDARY_WEAPON_RULE_UPPER))
    {
        auto weaponStr = Weapon_Type_To_String(type->Secondary);

        Log_Trace("Read_TechnoType_Rule => Rule value: %s", weaponStr);

        lua_pushstring(lua, weaponStr);
    }
    else if (Strings_Are_Equal(ruleName, ARMOR_RULE_UPPER))
    {
        auto armorStr = Armor_Type_To_String(type->Armor);

        Log_Trace("Read_TechnoType_Rule => Rule value: %s", armorStr);

        lua_pushstring(lua, armorStr);
    }
    else if (Strings_Are_Equal(ruleName, AMMO_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %d", type->MaxAmmo);

        lua_pushnumber(lua, type->MaxAmmo);
    }
    else if (Strings_Are_Equal(ruleName, SIGHT_RANGE_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %d", type->SightRange);

        lua_pushnumber(lua, type->SightRange);
    }
    else if (Strings_Are_Equal(ruleName, REWARD_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %d", type->Reward);

        lua_pushnumber(lua, type->Reward);
    }
    else if (Strings_Are_Equal(ruleName, DETECT_CLOAKED_OBJECTS_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsScanner));

        lua_pushboolean(lua, type->IsScanner);
    }
    else if (Strings_Are_Equal(ruleName, CRUSHABLE_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrushable));

        lua_pushboolean(lua, type->IsCrushable);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORTER_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTransporter));

        lua_pushboolean(lua, type->IsTransporter);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", type->FriendlyName);

        lua_pushstring(lua, type->FriendlyName);
    }
    else if (Strings_Are_Equal(ruleName, REPAIRABLE_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRepairable));

        lua_pushboolean(lua, type->IsRepairable);
    }
    else if (Strings_Are_Equal(ruleName, HAS_CREW_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrew));

        lua_pushboolean(lua, type->IsCrew);
    }
    else if (Strings_Are_Equal(ruleName, LEADER_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLeader));

        lua_pushboolean(lua, type->IsLeader);
    }
    else if (Strings_Are_Equal(ruleName, TWO_SHOOTER_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTwoShooter));

        lua_pushboolean(lua, type->IsTwoShooter);
    }
    else if (Strings_Are_Equal(ruleName, HAS_THEATER_GFX_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTheater));

        lua_pushboolean(lua, type->IsTheater);
    }
    else if (Strings_Are_Equal(ruleName, TURRED_EQUIPPED_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTurretEquipped));

        lua_pushboolean(lua, type->IsTurretEquipped);
    }
    else if (Strings_Are_Equal(ruleName, SHOW_NAME_RULE_UPPER))
    {
        Log_Trace("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsNominal));

        lua_pushboolean(lua, type->IsNominal);
    }
    else
    {
        return false;
    }

    return true;
}
