#include "function.h"

static WeaponType ParseWeaponType(lua_State* lua, const char* weaponTypeName, bool* parseError)
{
    auto weaponType = Parse_Weapon_Type(
        Convert_String_To_Upper_Case(weaponTypeName),
        parseError
    );

    if (*parseError)
    {
        luaL_error(lua, "weapon type passed was not recognised: %s", weaponTypeName);
        return WEAPON_NONE;
    }

    return weaponType;
}

static WeaponTypeClass* ResolveWeapon(lua_State* lua, const char* weaponTypeName)
{
    bool parseError = false;
    auto weaponType = ParseWeaponType(lua, weaponTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("weapon type parsed: %s -> %s", weaponTypeName, Weapon_Type_To_String(weaponType));

    return (WeaponTypeClass*)&WeaponTypeClass::As_Reference(weaponType);
}

static int Lua_Set_Weapon_Rule(lua_State* lua)
{
    Log_Trace("Lua_Set_Weapon_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 3)
    {
        luaL_error(lua, "getWeaponRule requires exactly three arguments");
    }

    auto weaponTypeName = Get_Lua_Value_As_String(1);

    if (String_Is_Empty(weaponTypeName))
    {
        luaL_error(lua, "getWeaponRule argument `weaponTypeName` was nil or blank");
        return 0;
    }

    auto weapon = ResolveWeapon(lua, weaponTypeName);

    if (weapon == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);
    auto value = Get_Lua_Value_As_String(3);

    if (String_Is_Empty(ruleName))
    {
        luaL_error(lua, "getWeaponRule argument `ruleName` was nil or blank");
        return 0;
    }

    Log_Debug("Lua_Set_Weapon_Rule => attempting to read value of rule '%s'", ruleName);

    auto upperRuleName = Convert_String_To_Upper_Case(ruleName);

    bool valueParseError = false;

    if (Strings_Are_Equal(upperRuleName, WEAPON_DAMAGE_RULE_UPPER))
    {
        valueParseError = !Is_Int_String(value);

        if (!valueParseError)
        {
            auto damage = atoi(value);
            valueParseError = damage < 0 || damage > UCHAR_MAX;

            if (!valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %u -> %u", weapon->Attack, damage);

                weapon->Attack = damage;
            }
        }
    }
    else if (Strings_Are_Equal(upperRuleName, WEAPON_RATE_OF_FIRE_RULE_UPPER))
    {
        valueParseError = !Is_Int_String(value);

        if (!valueParseError)
        {
            auto rof = atoi(value);
            valueParseError = rof < 0 || rof > UCHAR_MAX;

            if (!valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %u -> %u", weapon->ROF, rof);

                weapon->ROF = rof;
            }
        }
    }
    else if (Strings_Are_Equal(upperRuleName, WEAPON_RANGE_RULE_UPPER))
    {
        valueParseError = !Is_Int_String(value);

        if (!valueParseError)
        {
            auto range = atoi(value);
            valueParseError = range < 0 || range > INT_MAX;

            if (!valueParseError)
            {
                Log_Trace("Set_Weapon_Rule => Rule value: %d -> %d", weapon->Range, range);

                weapon->Range = range;
            }
        }
    }
    else if (Strings_Are_Equal(upperRuleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Set_Weapon_Rule => Rule value: %s -> %s", weapon->FriendlyName, value);

        weapon->FriendlyName = strdup(value);
    }
    else
    {
        luaL_error(lua, "weapon rule name passed to setWeaponRule was not recognised: %s", ruleName);
    }

    if (valueParseError)
    {
        luaL_error(lua, "value for weapon rule '%s' passed to setWeaponRule was invalid: %s", ruleName, value);
    }

    return 0;
}

static int Lua_Get_Weapon_Rule(lua_State* lua)
{
    Log_Trace("Lua_Get_Weapon_Rule called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "getWeaponRule requires exactly two arguments");
    }

    auto weaponTypeName = Get_Lua_Value_As_String(1);

    if (weaponTypeName == NULL)
    {
        luaL_error(lua, "getWeaponRule argument `weaponTypeName` was nil");
        return 0;
    }

    auto weapon = ResolveWeapon(lua, weaponTypeName);

    if (weapon == NULL)
    {
        return 0;
    }

    auto ruleName = Get_Lua_Value_As_String(2);

    if (String_Is_Empty(ruleName))
    {
        luaL_error(lua, "getWeaponRule argument `ruleName` was nil or blank");
        return 0;
    }

    Log_Debug("Lua_Get_Weapon_Rule => attempting to read value of rule '%s'", ruleName);

    auto upperRuleName = Convert_String_To_Upper_Case(ruleName);

    if (Strings_Are_Equal(upperRuleName, WEAPON_DAMAGE_RULE_UPPER))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->Attack);

        lua_pushnumber(lua, weapon->Attack);
    }
    else if (Strings_Are_Equal(upperRuleName, WEAPON_RATE_OF_FIRE_RULE_UPPER))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->ROF);

        lua_pushnumber(lua, weapon->ROF);
    }
    else if (Strings_Are_Equal(upperRuleName, WEAPON_RANGE_RULE_UPPER))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %u", weapon->Range);

        lua_pushnumber(lua, weapon->Range);
    }
    else if (Strings_Are_Equal(upperRuleName, FRIENDLY_NAME_RULE_UPPER))
    {
        Log_Trace("Read_Weapon_Rule => Rule value: %s", weapon->FriendlyName);

        lua_pushstring(lua, weapon->FriendlyName);
    }
    else
    {
        luaL_error(lua, "weapon rule name passed to getWeaponRule was not recognised: %s", ruleName);

        return 0;
    }

	return 1;
}

bool Register_Weapon_Functions()
{
	Log_Debug("Initialising Lua API weapon functions");

    Register_Lua_Function("getWeaponRule", Lua_Get_Weapon_Rule);
    Register_Lua_Function("setWeaponRule", Lua_Set_Weapon_Rule);

	return true;
}
