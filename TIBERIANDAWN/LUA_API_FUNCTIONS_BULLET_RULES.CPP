#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Bullet_Rule(BulletTypeClass* bullet, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BULLET_HIGH_RULE_UPPER))
    {
        auto isHigh = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsHigh), Convert_Boolean_To_String(isHigh));

            bullet->IsHigh = isHigh;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ANTI_AIRCRAFT_RULE_UPPER))
    {
        auto antiAircraft = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsAntiAircraft), Convert_Boolean_To_String(antiAircraft));

            bullet->IsAntiAircraft = antiAircraft;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_TRANSLUCENT_RULE_UPPER))
    {
        auto translucent = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsTranslucent), Convert_Boolean_To_String(translucent));

            bullet->IsTranslucent = translucent;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARCING_RULE_UPPER))
    {
        auto arcing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsArcing), Convert_Boolean_To_String(arcing));

            bullet->IsArcing = arcing;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_HOMING_RULE_UPPER))
    {
        auto homing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsHoming), Convert_Boolean_To_String(homing));

            bullet->IsHoming = homing;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_DROPPING_RULE_UPPER))
    {
        auto dropping = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsDropping), Convert_Boolean_To_String(dropping));

            bullet->IsDropping = dropping;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INVISIBLE_RULE_UPPER))
    {
        auto invisibile = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsInvisible), Convert_Boolean_To_String(invisibile));

            bullet->IsInvisible = invisibile;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_PROXIMITY_ARMED_RULE_UPPER))
    {
        auto proximityArmed = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsProximityArmed), Convert_Boolean_To_String(proximityArmed));

            bullet->IsProximityArmed = proximityArmed;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FLAME_EQUIPPED_RULE_UPPER))
    {
        auto flameEquipped = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsProximityArmed), Convert_Boolean_To_String(flameEquipped));

            bullet->IsProximityArmed = flameEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FUELED_RULE_UPPER))
    {
        auto fueled = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsFueled), Convert_Boolean_To_String(fueled));

            bullet->IsFueled = fueled;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FACELESS_RULE_UPPER))
    {
        auto faceless = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsFaceless), Convert_Boolean_To_String(faceless));

            bullet->IsFaceless = faceless;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INACCURATE_RULE_UPPER))
    {
        auto inaccurate = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsInaccurate), Convert_Boolean_To_String(inaccurate));

            bullet->IsInaccurate = inaccurate;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_WARHEAD_RULE_UPPER))
    {
        auto warheadType = Parse_Warhead_Type(value, valueParseError);

        if (!*valueParseError)
        {
            auto currentType = Warhead_Type_To_String(bullet->Warhead);

            Log_Trace("Write_Bullet_Rule => Rule value: %s -> %s", currentType,  value);

            bullet->Warhead = warheadType;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARMING_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto arming = atoi(value);

            Log_Trace("Write_Bullet_Rule => Rule value: %d -> %d", bullet->Arming, arming);

            bullet->Arming = arming;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_RANGE_RULE_UPPER))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto range = atoi(value);

            Log_Trace("Write_Bullet_Rule => Rule value: %d -> %d", bullet->Range, range);

            bullet->Range = range;
        }
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);
        
        if (!*valueParseError)
        {
            auto speed = strtoul(value, NULL,  10);

            *valueParseError = speed > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_Bullet_Rule => Rule value: %u -> %u", bullet->MaxSpeed, speed);

                bullet->MaxSpeed = (MPHType)speed;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);
        
        if (!*valueParseError)
        {
            auto rot = strtoul(value, NULL,  10);

            *valueParseError = rot > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Write_Bullet_Rule => Rule value: %u -> %u", bullet->ROT, rot);

                bullet->ROT = rot;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Write_Bullet_Rule => Rule value: %s -> %s", bullet->FriendlyName,  originalValue);

        bullet->FriendlyName = strdup(originalValue);
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Bullet_Rule(lua_State* lua, BulletTypeClass* bullet, const char* ruleName)
{
    if (Strings_Are_Equal(ruleName, BULLET_HIGH_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsHigh));

        lua_pushboolean(lua, bullet->IsHigh);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ANTI_AIRCRAFT_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsAntiAircraft));

        lua_pushboolean(lua, bullet->IsAntiAircraft);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_TRANSLUCENT_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsTranslucent));

        lua_pushboolean(lua, bullet->IsTranslucent);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARCING_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsArcing));

        lua_pushboolean(lua, bullet->IsArcing);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_HOMING_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsHoming));

        lua_pushboolean(lua, bullet->IsHoming);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_DROPPING_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsDropping));

        lua_pushboolean(lua, bullet->IsDropping);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INVISIBLE_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsInvisible));

        lua_pushboolean(lua, bullet->IsInvisible);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_PROXIMITY_ARMED_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsProximityArmed));

        lua_pushboolean(lua, bullet->IsProximityArmed);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FLAME_EQUIPPED_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFlameEquipped));

        lua_pushboolean(lua, bullet->IsFlameEquipped);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FUELED_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFueled));

        lua_pushboolean(lua, bullet->IsFueled);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FACELESS_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFaceless));

        lua_pushboolean(lua, bullet->IsFaceless);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INACCURATE_RULE_UPPER))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsInaccurate));

        lua_pushboolean(lua, bullet->IsInaccurate);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_WARHEAD_RULE_UPPER))
    {
        auto value = Warhead_Type_To_String(bullet->Warhead);

        Log_Trace("Read_Bullet_Rule => Rule value: %s", value);

        lua_pushstring(lua, value);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARMING_RULE_UPPER))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %d", bullet->Arming);

        lua_pushnumber(lua, bullet->Arming);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_RANGE_RULE_UPPER))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %d", bullet->Range);

        lua_pushnumber(lua, bullet->Range);
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE_UPPER))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %u", bullet->MaxSpeed);

        lua_pushnumber(lua, bullet->MaxSpeed);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE_UPPER))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %u", bullet->ROT);

        lua_pushnumber(lua, bullet->ROT);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", bullet->FriendlyName);

        lua_pushstring(lua, bullet->FriendlyName);
    }
    else
    {
        return false;
    }

    return true;
}

static BulletType ParseBulletType(lua_State* lua, const char* bulletTypeName, bool* parseError)
{
    auto upperBulletTypeName = Convert_String_To_Upper_Case(bulletTypeName);

    auto bulletType = Parse_Bullet_Type(
        upperBulletTypeName,
        parseError
    );

    delete upperBulletTypeName;

    if (*parseError)
    {
        luaL_error(lua, "bullet type passed was not recognised: %s", bulletTypeName);

        return BULLET_NONE;
    }

    return bulletType;
}

static BulletTypeClass* ResolveBullet(lua_State* lua, const char* bulletTypeName)
{
    bool parseError = false;
    auto bulletType = ParseBulletType(lua, bulletTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("bullet type parsed: %s -> %s", bulletTypeName, Bullet_Type_To_String(bulletType));

    return (BulletTypeClass*)&BulletTypeClass::As_Reference(bulletType);
}

static int Lua_Set_Bullet_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        lua,
        "Bullet",
        "bulletTypeName",
        &ResolveBullet,
        &Write_Bullet_Rule,
        false
    );
}

static int Lua_Get_Bullet_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        lua,
        "Bullets",
        "bulletTypeName",
        &ResolveBullet,
        &Read_Bullet_Rule,
        false
    );
}

bool Register_Bullet_Functions()
{
	Log_Debug("Initialising Lua API bullet functions");

    Register_Lua_Function("getBulletRule", Lua_Get_Bullet_Rule);
    Register_Lua_Function("setBulletRule", Lua_Set_Bullet_Rule);

	return true;
}
