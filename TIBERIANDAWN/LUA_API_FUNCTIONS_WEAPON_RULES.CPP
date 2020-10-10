#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Weapon_Rule(WeaponTypeClass* weapon, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, WEAPON_PROJECTILE_RULE))
    {
        auto bullet = Parse_Bullet_Type(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Set_Weapon_Rule => Rule value: %s -> %s", Bullet_Type_To_String(weapon->Projectile), value);

            weapon->Projectile = bullet;
        }
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_DAMAGE_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto damage = atoi(value);
            *valueParseError = damage < 0 || damage > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %u -> %u", weapon->Attack, damage);

                weapon->Attack = damage;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_RATE_OF_FIRE_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto rof = atoi(value);
            *valueParseError = rof < 0 || rof > UCHAR_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %u -> %u", weapon->ROF, rof);

                weapon->ROF = rof;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_RANGE_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto range = atoi(value);
            *valueParseError = range < 0 || range > INT_MAX;

            if (!*valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %d -> %d", weapon->Range, range);

                weapon->Range = range;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Set_Weapon_Rule => Rule value: %s -> %s", weapon->FriendlyName, originalValue);

        weapon->FriendlyName = strdup(originalValue);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        auto isModType = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Set_Weapon_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(weapon->IsModType), Convert_Boolean_To_String(isModType));

            weapon->IsModType = isModType;
        }
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        auto baseType = value;

        *valueParseError = String_Is_Empty(baseType) || strlen(baseType) > 32;

        if (!*valueParseError)
        {
            Log_Trace("Write_Weapon_Rule => Rule value: %s -> %s", weapon->ModBaseIniName, baseType);

            strcpy(weapon->ModBaseIniName, strdup(baseType));
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Weapon_Rule(WeaponTypeClass* weapon, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, WEAPON_PROJECTILE_RULE))
    {
        auto bullet = Bullet_Type_To_String(weapon->Projectile);

        Log_Trace("Read_Weapon_Rule => Rule value: %s", bullet);

        luaState.WriteString(bullet);
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_DAMAGE_RULE))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->Attack);

        luaState.WriteInteger(weapon->Attack);
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_RATE_OF_FIRE_RULE))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->ROF);

        luaState.WriteInteger(weapon->ROF);
    }
    else if (Strings_Are_Equal(ruleName, WEAPON_RANGE_RULE))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->Range);

        luaState.WriteInteger(weapon->Range);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %s", weapon->FriendlyName);

        luaState.WriteString(weapon->FriendlyName);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %s", weapon->IsModType);

        luaState.WriteBool(weapon->IsModType);
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        if (weapon->IsModType)
        {
            Log_Trace("Read_Weapon_Rule => Rule value: %s", weapon->ModBaseIniName);

            luaState.WriteString(weapon->ModBaseIniName);
        }
        else
        {
            // fallback to ini name for non-mod type instances
            Log_Trace("Read_Weapon_Rule => Rule value: %s", weapon->IniName);

            luaState.WriteString(weapon->IniName);
        }
    }
    else
    {
        return false;
    }

    return true;
}

static WeaponType ParseWeaponType(const char* weaponTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();
    auto upperTypeName = Convert_String_To_Upper_Case(weaponTypeName);

    auto weaponType = Parse_Weapon_Type(
        upperTypeName,
        parseError
    );

    delete upperTypeName;

    if (*parseError)
    {
        luaState.RaiseError("weapon type passed was not recognised: %s", weaponTypeName);
        return WEAPON_NONE;
    }

    return weaponType;
}

static WeaponTypeClass* ResolveWeapon(const char* weaponTypeName)
{
    bool parseError = false;
    auto weaponType = ParseWeaponType(weaponTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("weapon type parsed: %s -> %s", weaponTypeName, Weapon_Type_To_String(weaponType));

    return (WeaponTypeClass*)&WeaponTypeClass::As_Reference(weaponType);
}

static int Lua_Set_Weapon_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        "Weapon",
        "weaponTypeName",
        &ResolveWeapon,
        GetRulesInfo().GetWeaponRules(),
        &Write_Weapon_Rule,
        false
    );
}

static int Lua_Get_Weapon_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        "Weapon",
        "weaponTypeName",
        &ResolveWeapon,
        GetRulesInfo().GetWeaponRules(),
        &Read_Weapon_Rule,
        false
    );
}

bool Register_Weapon_Functions()
{
    LuaRuntime().RegisterApi("weapon rules", [](ILuaApi& a) {
        a.WithDescription("Weapon info and control functions")

         .WithFunction("getWeaponRule", Lua_Get_Weapon_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         })

         .WithFunction("setWeaponRule", Lua_Set_Weapon_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         });
    });

    return true;
}
