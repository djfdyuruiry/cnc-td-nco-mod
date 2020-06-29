#include "function.h"

static auto INFANTRY_RULE_NAMES = new const char* [INFANTRY_RULE_COUNT] {
    INFANTRY_RULES
};

static auto UNIT_RULE_NAMES = new const char* [UNIT_RULE_COUNT] {
    UNIT_RULES
};

static auto AIRCRAFT_RULE_NAMES = new const char* [AIRCRAFT_RULE_COUNT] {
    AIRCRAFT_RULES
};

static auto BUILDING_RULE_NAMES = new const char* [BUILDING_RULE_COUNT] {
    BUILDING_RULES
};

static bool Write_Building_Rule(BuildingTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BIBBED_RULE_UPPER))
    {
        auto isBibbed = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsBibbed = isBibbed;
        }
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE_UPPER))
    {
        auto isWall = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsWall = isWall;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE_UPPER))
    {
        auto isFactory = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFactory = isFactory;
        }
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE_UPPER))
    {
        auto isSimpleDamage = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsSimpleDamage = isSimpleDamage;
        }
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE_UPPER))
    {
        auto isSturdy = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsSturdy = isSturdy;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE_UPPER))
    {
        auto isCaptureable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCaptureable = isCaptureable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE_UPPER))
    {
        auto isRegulated = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsRegulated = isRegulated;
        }
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE_UPPER))
    {
        auto isUnsellable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsUnsellable = isUnsellable;
        }
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE_UPPER))
    {
        auto factoryType = Parse_Factory_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->ToBuild = (RTTIType)factoryType;
        }
    }
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE_UPPER))
    {
        auto capacity = atoi(value);
        *valueParseError = capacity < 0 || capacity > UINT_MAX;

        if (!*valueParseError)
        {
            type->Capacity = capacity;
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE_UPPER))
    {
        auto power = atoi(value);
        *valueParseError = power < 0 || power > INT_MAX;

        if (!*valueParseError)
        {
            type->Power = power;
        }
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE_UPPER))
    {
        auto drain = atoi(value);
        *valueParseError = drain < 0 || drain > INT_MAX;

        if (!*valueParseError)
        {
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
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsBibbed));

        lua_pushboolean(lua, type->IsBibbed);
    }
    else if (Strings_Are_Equal(ruleName, WALL_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsWall));

        lua_pushboolean(lua, type->IsWall);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFactory));

        lua_pushboolean(lua, type->IsFactory);
    }
    else if (Strings_Are_Equal(ruleName, SIMPLE_DAMAGE_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSimpleDamage));

        lua_pushboolean(lua, type->IsSimpleDamage);
    }
    else if (Strings_Are_Equal(ruleName, STURDY_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsSturdy));

        lua_pushboolean(lua, type->IsSturdy);
    }
    else if (Strings_Are_Equal(ruleName, CAPTUREABLE_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCaptureable));

        lua_pushboolean(lua, type->IsCaptureable);
    }
    else if (Strings_Are_Equal(ruleName, FIXED_SPEED_ANIMATION_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRegulated));

        lua_pushboolean(lua, type->IsRegulated);
    }
    else if (Strings_Are_Equal(ruleName, UNSELLABLE_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsUnsellable));

        lua_pushboolean(lua, type->IsUnsellable);
    }
    else if (Strings_Are_Equal(ruleName, FACTORY_TYPE_RULE_UPPER))
    {
        auto factoryType = Factory_Type_To_String((FactoryType)type->ToBuild);

        Log_Debug("Read_Building_Rule => Rule value: %s", factoryType);

        lua_pushstring(lua, factoryType);
    }
    else if (Strings_Are_Equal(ruleName, STORAGE_CAPACITY_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %u", type->Capacity);

        lua_pushnumber(lua, type->Capacity);
    }
    else if (Strings_Are_Equal(ruleName, POWER_OUTPUT_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %d", type->Power);

        lua_pushnumber(lua, type->Power);
    }
    else if (Strings_Are_Equal(ruleName, POWER_INPUT_RULE_UPPER))
    {
        Log_Debug("Read_Building_Rule => Rule value: %d", type->Drain);

        lua_pushnumber(lua, type->Drain);
    }
    else
    {
        return false;
    }

    return true;
}

static bool Write_Aircraft_Rule(AircraftTypeClass* type, const char* ruleName, const char* value, bool* valueParseError)
{

    if (Strings_Are_Equal(ruleName, CANT_HOVER_RULE_UPPER))
    {
        auto isFixedWing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsFixedWing = isFixedWing;
        }
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE_UPPER))
    {
        auto isLandable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsLandable = isLandable;
        }
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE_UPPER))
    {
        auto hasRotor = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsRotorEquipped = hasRotor;
        }
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        auto rot = atoi(value);
        *valueParseError = rot < 0 || rot > UCHAR_MAX;

        if (!*valueParseError)
        {
            type->ROT = rot;
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
        Log_Debug("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsFixedWing));

        lua_pushboolean(lua, type->IsFixedWing);
    }
    else if (Strings_Are_Equal(ruleName, CAN_LAND_RULE_UPPER))
    {
        Log_Debug("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsLandable));

        lua_pushboolean(lua, type->IsLandable);
    }
    else if (Strings_Are_Equal(ruleName, HAS_ROTOR_RULE_UPPER))
    {
        Log_Debug("Read_Aircraft_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsRotorEquipped));

        lua_pushboolean(lua, type->IsRotorEquipped);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        Log_Debug("Read_Aircraft_Rule => Rule value: %u", type->ROT);

        lua_pushnumber(lua, type->ROT);
    }
    else
    {
        return false;
    }

    return true;
}

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
        auto rot = atoi(value);
        *valueParseError = rot < 0 || rot > UCHAR_MAX;

        if (!*valueParseError)
        {
            type->ROT = rot;
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
        auto numValue = atoi(value);
        *valueParseError = numValue < 1 && numValue > 99;

        if (!*valueParseError)
        {
            type->Level = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, SCENARIO_LEVEL_RULE_UPPER))
    {
        auto numValue = atoi(value);
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
        auto numValue = atoi(value);
        *valueParseError = numValue < 0 && numValue > UCHAR_MAX;

        if (!*valueParseError)
        {
            type->MaxSpeed = (MPHType)numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, STRENGTH_RULE_UPPER))
    {
        auto numValue = atoi(value);
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
            type->Calculate_Risk(); // make sure the Risk value now reflects the new primary weapon
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
    else if (Strings_Are_Equal(ruleName, ARMOR_RULE_UPPER))
    {
        auto armourType = Parse_Armor_Type(value, valueParseError);

        if (!*valueParseError)
        {
            type->Armor = armourType;
        }
    }
    else if (Strings_Are_Equal(ruleName, AMMO_RULE_UPPER))
    {
        auto numValue = atoi(value);
        *valueParseError = numValue < -1 && numValue > INT_MAX;

        if (!*valueParseError)
        {
            type->MaxAmmo = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, SIGHT_RANGE_RULE_UPPER))
    {
        auto numValue = atoi(value);
        *valueParseError = numValue < -1 && numValue > INT_MAX;

        if (!*valueParseError)
        {
            type->SightRange = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, REWARD_RULE_UPPER))
    {
        auto numValue = atoi(value);
        *valueParseError = numValue < -1 && numValue > INT_MAX;

        if (!*valueParseError)
        {
            type->Reward = numValue;
        }
    }
    else if (Strings_Are_Equal(ruleName, DETECT_CLOAKED_OBJECTS_RULE_UPPER))
    {
        auto isScanner = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsScanner = isScanner;
        }
    }
    else if (Strings_Are_Equal(ruleName, CRUSHABLE_RULE_UPPER))
    {
        auto isCrushable = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsCrushable = isCrushable;
        }
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORTER_RULE_UPPER))
    {
        auto isTransporter = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            type->IsTransporter = isTransporter;
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
    else if (Strings_Are_Equal(ruleName, ARMOR_RULE_UPPER))
    {
        auto armorStr = Armor_Type_To_String(type->Armor);

        Log_Debug("Read_TechnoType_Rule => Rule value: %s", armorStr);

        lua_pushstring(lua, armorStr);
    }
    else if (Strings_Are_Equal(ruleName, AMMO_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %d", type->MaxAmmo);

        lua_pushnumber(lua, type->MaxAmmo);
    }
    else if (Strings_Are_Equal(ruleName, SIGHT_RANGE_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %d", type->SightRange);

        lua_pushnumber(lua, type->SightRange);
    }
    else if (Strings_Are_Equal(ruleName, REWARD_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %d", type->Reward);

        lua_pushnumber(lua, type->Reward);
    }
    else if (Strings_Are_Equal(ruleName, DETECT_CLOAKED_OBJECTS_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsScanner));

        lua_pushboolean(lua, type->IsScanner);
    }
    else if (Strings_Are_Equal(ruleName, CRUSHABLE_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsCrushable));

        lua_pushboolean(lua, type->IsCrushable);
    }
    else if (Strings_Are_Equal(ruleName, TRANSPORTER_RULE_UPPER))
    {
        Log_Debug("Read_TechnoType_Rule => Rule value: %s", Convert_Boolean_To_String(type->IsTransporter));

        lua_pushboolean(lua, type->IsTransporter);
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

    Log_Debug("unit type parsed: %s", Structure_Type_To_String(buildingType));

    return (BuildingTypeClass*)&BuildingTypeClass::As_Reference(buildingType);
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

    Log_Debug("aircraft type parsed: %s", Aircraft_Type_To_String(aircraftType));

    return (AircraftTypeClass*)&AircraftTypeClass::As_Reference(aircraftType);
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

static void Read_Object_Names(
    lua_State* lua,
    char first,
    char count,
    void* toStringFunctionPtr
)
{
    auto toStringFunction = (char* (*)(char))toStringFunctionPtr;

    lua_newtable(lua);

    for (char i = first; i < count; i++)
    {
        lua_pushstring(lua, toStringFunction(i));
        lua_rawseti(lua, -2, i + 1);
    }
}

static void Read_Rule_Names(lua_State* lua, const char** ruleNames, unsigned ruleCount)
{
    lua_newtable(lua);

    for (unsigned i = 0; i < ruleCount; i++)
    {
        lua_pushstring(lua, ruleNames[i]);
        lua_rawseti(lua, -2, i + 1);
    }
}

int Lua_Get_Infantry_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, INFANTRY_RULE_NAMES, INFANTRY_RULE_COUNT);

    return 1;
}

int Lua_Get_Unit_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, UNIT_RULE_NAMES, UNIT_RULE_COUNT);

    return 1;
}

int Lua_Get_Aircraft_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, AIRCRAFT_RULE_NAMES, AIRCRAFT_RULE_COUNT);

    return 1;
}

int Lua_Get_Building_Rule_Names(lua_State* lua)
{
    Read_Rule_Names(lua, BUILDING_RULE_NAMES, BUILDING_RULE_COUNT);

    return 1;
}

int Lua_Get_Infantry_Types(lua_State* lua)
{
    Read_Object_Names(lua, INFANTRY_FIRST, INFANTRY_COUNT, &(Infantry_Type_To_String));

    return 1;
}

int Lua_Get_Unit_Types(lua_State* lua)
{
    Read_Object_Names(lua, UNIT_FIRST, UNIT_COUNT, &(Unit_Type_To_String));

    return 1;
}

int Lua_Get_Aircraft_Types(lua_State* lua)
{
    Read_Object_Names(lua, AIRCRAFT_FIRST, AIRCRAFT_COUNT, &(Aircraft_Type_To_String));

    return 1;
}

int Lua_Get_Building_Types(lua_State* lua)
{
    Read_Object_Names(lua, STRUCT_FIRST, STRUCT_COUNT, &(Structure_Type_To_String));

    return 1;
}

int Lua_Get_Game_Rule(lua_State* lua)
{
    Log_Debug("Lua_Get_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "getGameRule requires at least one argument");
    }

    auto ruleName = Get_Lua_Value_As_String(1);

    auto ruleValue = Read_Game_Rule(ruleName, false);

    lua_pushboolean(lua, ruleValue);

    return 1;
}

int Lua_Set_Game_Rule(lua_State* lua)
{
    Log_Debug("Lua_Set_Game_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "setGameRule requires at least two arguments");
    }

    bool parseError = false;

    auto ruleName = Get_Lua_Value_As_String(1);
    auto ruleValue = Parse_Boolean(Get_Lua_Value_As_String(2), &parseError);

    if (!parseError)
    {
        Update_Current_Game_Rule_Value(ruleName, ruleValue);
    }
    else
    {
        luaL_error(lua, "setGameRule argument `value` not set to a valid boolean value");
    }

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

    Log_Debug("Lua_Get_Infantry_Rule => looking for rule in Infantry Type");

    ruleMatched = Read_Infantry_Rule(lua, infantry, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Infantry_Rule => Rule matched in Infantry Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getInfantryRule was not recognised: %s", ruleName);

    return 0;
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

    Log_Debug("Lua_Get_Unit_Rule => looking for rule in Unit Type");

    ruleMatched = Read_Unit_Rule(lua, unit, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Unit_Rule => Rule matched in Unit Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getUnitRule was not recognised: %s", ruleName);

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
        luaL_error(lua, "setUnitRule argument `unitTypeName` was nil");
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

int Lua_Get_Aircraft_Rule(lua_State* lua)
{
    Log_Debug("Lua_Get_Aircraft_Rule called from Lua");

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

    Log_Debug("Lua_Get_Aircraft_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, aircraft, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Aircraft_Rule => Rule matched in Techno Type");
        return 1;
    }

    Log_Debug("Lua_Get_Aircraft_Rule => looking for rule in Aircraft Type");

    ruleMatched = Read_Aircraft_Rule(lua, aircraft, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Aircraft_Rule => Rule matched in Aircraft Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getAircraftRule was not recognised: %s", ruleName);

    return 0;
}

int Lua_Set_Aircraft_Rule(lua_State* lua)
{
    Log_Debug("Lua_Set_Aircraft_Rule called from Lua");

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

    Log_Debug("Lua_Set_Aircraft_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        aircraft,
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
        Log_Debug("Lua_Set_Unit_Rule => looking for rule in Aircraft Type");

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

int Lua_Get_Building_Rule(lua_State* lua)
{
    Log_Debug("Lua_Get_Building_Rule called from Lua");

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

    Log_Debug("Lua_Get_Building_Rule => looking for rule '%s' in Techno Type", ruleName);

    auto ruleMatched = Read_TechnoType_Rule(lua, building, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Building_Rule => Rule matched in Techno Type");
        return 1;
    }

    Log_Debug("Lua_Get_Building_Rule => looking for rule in Building Type");

    ruleMatched = Read_Building_Rule(lua, building, Convert_String_To_Upper_Case(ruleName));

    if (ruleMatched)
    {
        Log_Debug("Lua_Get_Building_Rule => Rule matched in Building Type");
        return 1;
    }

    luaL_error(lua, "rule name type passed getBuildingRule was not recognised: %s", ruleName);

    return 0;
}

int Lua_Set_Building_Rule(lua_State* lua)
{
    Log_Debug("Lua_Set_Building_Rule called from Lua");

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

    auto building= ResolveBuilding(lua, buildingTypeName);

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

    Log_Debug("Lua_Set_Building_Rule => looking for rule in Techno Type");

    bool parseError = false;
    auto ruleMatched = Write_TechnoType_Rule(
        building,
        Convert_String_To_Upper_Case(ruleName),
        Convert_String_To_Upper_Case(ruleValue),
        &parseError
    );

    if (ruleMatched && !parseError)
    {
        Log_Debug("Lua_Set_Building_Rule => Rule matched in Techno Type");
        return 1;
    }

    if (!parseError)
    {
        Log_Debug("Lua_Set_Building_Rule => looking for rule in Building Type");

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
