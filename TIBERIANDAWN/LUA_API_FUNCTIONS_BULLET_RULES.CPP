#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Bullet_Rule(BulletTypeClass* bullet, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, BULLET_HIGH_RULE))
    {
        auto isHigh = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsHigh), Convert_Boolean_To_String(isHigh));

            bullet->IsHigh = isHigh;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ANTI_AIRCRAFT_RULE))
    {
        auto antiAircraft = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsAntiAircraft), Convert_Boolean_To_String(antiAircraft));

            bullet->IsAntiAircraft = antiAircraft;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_TRANSLUCENT_RULE))
    {
        auto translucent = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsTranslucent), Convert_Boolean_To_String(translucent));

            bullet->IsTranslucent = translucent;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARCING_RULE))
    {
        auto arcing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsArcing), Convert_Boolean_To_String(arcing));

            bullet->IsArcing = arcing;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_HOMING_RULE))
    {
        auto homing = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsHoming), Convert_Boolean_To_String(homing));

            bullet->IsHoming = homing;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_DROPPING_RULE))
    {
        auto dropping = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsDropping), Convert_Boolean_To_String(dropping));

            bullet->IsDropping = dropping;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INVISIBLE_RULE))
    {
        auto invisibile = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsInvisible), Convert_Boolean_To_String(invisibile));

            bullet->IsInvisible = invisibile;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_PROXIMITY_ARMED_RULE))
    {
        auto proximityArmed = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsProximityArmed), Convert_Boolean_To_String(proximityArmed));

            bullet->IsProximityArmed = proximityArmed;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FLAME_EQUIPPED_RULE))
    {
        auto flameEquipped = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsProximityArmed), Convert_Boolean_To_String(flameEquipped));

            bullet->IsProximityArmed = flameEquipped;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FUELED_RULE))
    {
        auto fueled = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsFueled), Convert_Boolean_To_String(fueled));

            bullet->IsFueled = fueled;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FACELESS_RULE))
    {
        auto faceless = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsFaceless), Convert_Boolean_To_String(faceless));

            bullet->IsFaceless = faceless;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INACCURATE_RULE))
    {
        auto inaccurate = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsInaccurate), Convert_Boolean_To_String(inaccurate));

            bullet->IsInaccurate = inaccurate;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_WARHEAD_RULE))
    {
        auto warheadType = Parse_Warhead_Type(value, valueParseError);

        if (!*valueParseError)
        {
            auto currentType = Warhead_Type_To_String(bullet->Warhead);

            Log_Trace("Write_Bullet_Rule => Rule value: %s -> %s", currentType,  value);

            bullet->Warhead = warheadType;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARMING_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto arming = atoi(value);

            Log_Trace("Write_Bullet_Rule => Rule value: %d -> %d", bullet->Arming, arming);

            bullet->Arming = arming;
        }
    }
    else if (Strings_Are_Equal(ruleName, BULLET_RANGE_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto range = atoi(value);

            Log_Trace("Write_Bullet_Rule => Rule value: %d -> %d", bullet->Range, range);

            bullet->Range = range;
        }
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE))
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
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
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
    else if (Strings_Are_Equal(ruleName, IMAGE_RULE))
    {
        Log_Trace("Write_Bullet_Rule => Rule value: %s -> %s", bullet->Image, originalValue);

        bullet->Image = strdup(originalValue);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Write_Bullet_Rule => Rule value: %s -> %s", bullet->FriendlyName,  originalValue);

        bullet->FriendlyName = strdup(originalValue);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        auto isModType = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Bullet_Rule => %s -> %s", Convert_Boolean_To_String(bullet->IsModType), Convert_Boolean_To_String(isModType));

            bullet->IsModType = isModType;
        }
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        auto baseType = value;

        *valueParseError = String_Is_Empty(baseType) || strlen(baseType) > 32;

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", bullet->ModBaseIniName, baseType);

            strcpy(bullet->ModBaseIniName, strdup(baseType));
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Bullet_Rule(BulletTypeClass* bullet, const char* ruleName)
{
    auto &luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, BULLET_HIGH_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsHigh));

        luaState.WriteBool(bullet->IsHigh);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ANTI_AIRCRAFT_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsAntiAircraft));

        luaState.WriteBool(bullet->IsAntiAircraft);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_TRANSLUCENT_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsTranslucent));

        luaState.WriteBool(bullet->IsTranslucent);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARCING_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsArcing));

        luaState.WriteBool(bullet->IsArcing);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_HOMING_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsHoming));

        luaState.WriteBool(bullet->IsHoming);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_DROPPING_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsDropping));

        luaState.WriteBool(bullet->IsDropping);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INVISIBLE_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsInvisible));

        luaState.WriteBool(bullet->IsInvisible);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_PROXIMITY_ARMED_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsProximityArmed));

        luaState.WriteBool(bullet->IsProximityArmed);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FLAME_EQUIPPED_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFlameEquipped));

        luaState.WriteBool(bullet->IsFlameEquipped);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FUELED_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFueled));

        luaState.WriteBool(bullet->IsFueled);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_FACELESS_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsFaceless));

        luaState.WriteBool(bullet->IsFaceless);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_INACCURATE_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsInaccurate));

        luaState.WriteBool(bullet->IsInaccurate);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_WARHEAD_RULE))
    {
        auto value = Warhead_Type_To_String(bullet->Warhead);

        Log_Trace("Read_Bullet_Rule => Rule value: %s", value);

        luaState.WriteString(value);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_ARMING_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %d", bullet->Arming);

        luaState.WriteInteger(bullet->Arming);
    }
    else if (Strings_Are_Equal(ruleName, BULLET_RANGE_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %d", bullet->Range);

        luaState.WriteInteger(bullet->Range);
    }
    else if (Strings_Are_Equal(ruleName, SPEED_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %u", bullet->MaxSpeed);

        luaState.WriteInteger(bullet->MaxSpeed);
    }
    else if (Strings_Are_Equal(ruleName, RATE_OF_TURN_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %u", bullet->ROT);

        luaState.WriteInteger(bullet->ROT);
    }
    else if (Strings_Are_Equal(ruleName, IMAGE_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", bullet->Image);

        luaState.WriteString(bullet->Image);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", bullet->FriendlyName);

        luaState.WriteString(bullet->FriendlyName);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        Log_Trace("Read_Bullet_Rule => Rule value: %s", Convert_Boolean_To_String(bullet->IsModType));

        luaState.WriteBool(bullet->IsModType);
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        if (bullet->IsModType)
        {
            Log_Trace("Read_TechnoType_Rule => Rule value: %s", bullet->ModBaseIniName);

            luaState.WriteString(bullet->ModBaseIniName);
        }
        else
        {
            // fallback to ini name for non-mod type instances
            Log_Trace("Read_TechnoType_Rule => Rule value: %s", bullet->IniName);

            luaState.WriteString(bullet->IniName);
        }
    }
    else
    {
        return false;
    }

    return true;
}

static BulletType ParseBulletType(const char* bulletTypeName, bool* parseError)
{
    auto &luaState = LuaRuntime().GetState();
    auto upperBulletTypeName = Convert_String_To_Upper_Case(bulletTypeName);

    auto bulletType = Parse_Bullet_Type(
        upperBulletTypeName,
        parseError
    );

    delete upperBulletTypeName;

    if (*parseError)
    {
        luaState.RaiseError("bullet type passed was not recognised: %s", bulletTypeName);

        return BULLET_NONE;
    }

    return bulletType;
}

static BulletTypeClass* ResolveBullet(const char* bulletTypeName)
{
    bool parseError = false;
    auto bulletType = ParseBulletType(bulletTypeName, &parseError);

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
        "Bullet",
        "bulletTypeName",
        &ResolveBullet,
        GetRulesInfo().GetBulletRules(),
        &Write_Bullet_Rule,
        false
    );
}

static int Lua_Get_Bullet_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        "Bullets",
        "bulletTypeName",
        &ResolveBullet,
        GetRulesInfo().GetBulletRules(),
        &Read_Bullet_Rule,
        false
    );
}

bool Register_Bullet_Functions()
{
    LuaRuntime().RegisterApi("bullet rules", [](ILuaApi& a) {
        a.WithDescription("Bullet rule info and control functions")

         .WithFunction("getBulletRule", Lua_Get_Bullet_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         })

         .WithFunction("setBulletRule", Lua_Set_Bullet_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         });
    });

    return true;
}
