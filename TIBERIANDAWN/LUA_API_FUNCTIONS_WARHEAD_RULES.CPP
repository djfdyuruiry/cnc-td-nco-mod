#include "function.h"

#include "lua_api_functions_generic_rules.h"

static bool Write_Warhead_Rule(WarheadTypeClass* warhead, const char* ruleName, const char* value, const char* originalValue, bool* valueParseError)
{
    if (Strings_Are_Equal(ruleName, WARHEAD_SPREAD_FACTOR_RULE))
    {
        *valueParseError = !Is_Int_String(value);

        if (!*valueParseError)
        {
            auto spreadFactor = atoi(value);
            *valueParseError = spreadFactor < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %d -> %d", warhead->SpreadFactor, spreadFactor);

                warhead->SpreadFactor = spreadFactor;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_WALLS_RULE))
    {
        auto destroysWalls = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => %s -> %s", Convert_Boolean_To_String(warhead->IsWallDestroyer), Convert_Boolean_To_String(destroysWalls));

            warhead->IsWallDestroyer = destroysWalls;
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_WOOD_RULE))
    {
        auto destroysWoodWalls = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => %s -> %s", Convert_Boolean_To_String(warhead->IsWoodDestroyer), Convert_Boolean_To_String(destroysWoodWalls));

            warhead->IsWoodDestroyer = destroysWoodWalls;
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_TIBERIUM_RULE))
    {
        auto destroysTiberium = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_Unit_Rule => %s -> %s", Convert_Boolean_To_String(warhead->IsTiberiumDestroyer), Convert_Boolean_To_String(destroysTiberium));

            warhead->IsTiberiumDestroyer = destroysTiberium;
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_NO_ARMOR_MODIFIER_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto modifier = strtoul(value, NULL, 10);
            *valueParseError = modifier < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %u -> %u", warhead->Modifier[ARMOR_NONE], modifier);

                warhead->Modifier[ARMOR_NONE] = modifier;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_WOOD_ARMOR_MODIFIER_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto modifier = strtoul(value, NULL, 10);
            *valueParseError = modifier < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %u -> %u", warhead->Modifier[ARMOR_WOOD], modifier);

                warhead->Modifier[ARMOR_WOOD] = modifier;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto modifier = strtoul(value, NULL, 10);
            *valueParseError = modifier < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %u -> %u", warhead->Modifier[ARMOR_ALUMINUM], modifier);

                warhead->Modifier[ARMOR_ALUMINUM] = modifier;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_STEEL_ARMOR_MODIFIER_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto modifier = strtoul(value, NULL, 10);
            *valueParseError = modifier < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %u -> %u", warhead->Modifier[ARMOR_STEEL], modifier);

                warhead->Modifier[ARMOR_STEEL] = modifier;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE))
    {
        *valueParseError = !Is_Unsigned_Int_String(value);

        if (!*valueParseError)
        {
            auto modifier = strtoul(value, NULL, 10);
            *valueParseError = modifier < 1;

            if (!*valueParseError)
            {
                Log_Trace("Write_Warhead_Rule => Rule value: %u -> %u", warhead->Modifier[ARMOR_CONCRETE], modifier);

                warhead->Modifier[ARMOR_CONCRETE] = modifier;
            }
        }
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Write_Warhead_Rule => Rule value: %s -> %s", warhead->FriendlyName, originalValue);

        delete warhead->FriendlyName;

        warhead->FriendlyName = strdup(originalValue);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        auto isModType = Parse_Boolean(value, valueParseError);

        if (!*valueParseError)
        {
            Log_Trace("Write_TechnoType_Rule => Rule value: %s -> %s", Convert_Boolean_To_String(warhead->IsModType), Convert_Boolean_To_String(isModType));

            warhead->IsModType = isModType;
        }
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        auto baseType = value;

        *valueParseError = String_Is_Empty(baseType) || strlen(baseType) > 32;

        if (!*valueParseError)
        {
            Log_Trace("Write_Warhead_Rule => Rule value: %s -> %s", warhead->ModBaseIniName, baseType);

            strcpy(warhead->ModBaseIniName, strdup(baseType));
        }
    }
    else
    {
        return false;
    }

    return true;
}

static bool Read_Warhead_Rule(WarheadTypeClass* warhead, const char* ruleName)
{
    auto& luaState = LuaRuntime().GetState();

    if (Strings_Are_Equal(ruleName, WARHEAD_SPREAD_FACTOR_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %d", warhead->SpreadFactor);

        luaState.WriteInteger(warhead->SpreadFactor);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_WALLS_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(warhead->IsWallDestroyer));

        luaState.WriteBool(warhead->IsWallDestroyer);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_WOOD_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(warhead->IsWoodDestroyer));

        luaState.WriteBool(warhead->IsWoodDestroyer);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_DESTORY_TIBERIUM_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(warhead->IsTiberiumDestroyer));

        luaState.WriteBool(warhead->IsTiberiumDestroyer);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_NO_ARMOR_MODIFIER_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %u", warhead->Modifier[ARMOR_NONE]);

        luaState.WriteInteger(warhead->Modifier[ARMOR_NONE]);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_WOOD_ARMOR_MODIFIER_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %u", warhead->Modifier[ARMOR_WOOD]);

        luaState.WriteInteger(warhead->Modifier[ARMOR_WOOD]);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %u", warhead->Modifier[ARMOR_ALUMINUM]);

        luaState.WriteInteger(warhead->Modifier[ARMOR_ALUMINUM]);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_STEEL_ARMOR_MODIFIER_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %u", warhead->Modifier[ARMOR_STEEL]);

        luaState.WriteInteger(warhead->Modifier[ARMOR_STEEL]);
    }
    else if (Strings_Are_Equal(ruleName, WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %u", warhead->Modifier[ARMOR_CONCRETE]);

        luaState.WriteInteger(warhead->Modifier[ARMOR_CONCRETE]);
    }
    else if (Strings_Are_Equal(ruleName, FRIENDLY_NAME_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", warhead->FriendlyName);

        luaState.WriteString(warhead->FriendlyName);
    }
    else if (Strings_Are_Equal(ruleName, IS_MOD_TYPE_RULE))
    {
        Log_Trace("Read_Warhead_Rule => Rule value: %s", Convert_Boolean_To_String(warhead->IsModType));

        luaState.WriteBool(warhead->IsModType);
    }
    else if (Strings_Are_Equal(ruleName, BASE_TYPE_RULE))
    {
        if (warhead->IsModType)
        {
            Log_Trace("Read_Warhead_Rule => Rule value: %s", warhead->ModBaseIniName);

            luaState.WriteString(warhead->ModBaseIniName);
        }
        else
        {
            // fallback to ini name for non-mod type instances
            Log_Trace("Read_Warhead_Rule => Rule value: %s", warhead->IniName);

            luaState.WriteString(warhead->IniName);
        }
    }

    else
    {
        return false;
    }

    return true;
}

static WarheadType ParseWarheadType(const char* warheadTypeName, bool* parseError)
{
    auto& luaState = LuaRuntime().GetState();
    auto upperWarheadTypeName = Convert_String_To_Upper_Case(warheadTypeName);

    auto warheadType = Parse_Warhead_Type(
        upperWarheadTypeName,
        parseError
    );

    delete upperWarheadTypeName;

    if (*parseError)
    {
        luaState.RaiseError("warhead type passed was not recognised: %s", warheadTypeName);

        return WARHEAD_NONE;
    }

    return warheadType;
}

static WarheadTypeClass* ResolveWarhead(const char* warheadTypeName)
{
    bool parseError = false;
    auto warheadType = ParseWarheadType(warheadTypeName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    Log_Trace("warhead type parsed: %s -> %s", warheadTypeName, Warhead_Type_To_String(warheadType));

    return (WarheadTypeClass*)&WarheadTypeClass::As_Reference(warheadType);
}

static int Lua_Set_Warhead_Rule(lua_State* lua)
{
    return Lua_Set_Rule(
        "Warhead",
        "warheadTypeName",
        &ResolveWarhead,
        GetRulesInfo().GetWarheadRules(),
        &Write_Warhead_Rule,
        false
    );
}

static int Lua_Get_Warhead_Rule(lua_State* lua)
{
    return Lua_Get_Rule(
        "Warhead",
        "warheadTypeName",
        &ResolveWarhead,
        GetRulesInfo().GetWarheadRules(),
        &Read_Warhead_Rule,
        false
    );
}

bool Register_Warhead_Functions()
{
    LuaRuntime().RegisterApi("warhead rules", [](ILuaApi& a) {
        a.WithDescription("Warhead info and control functions")

         .WithFunction("getWarheadRule", Lua_Get_Warhead_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         })

         .WithFunction("setWarheadRule", Lua_Set_Warhead_Rule, [](LuaFunctionInfo& f) {
             f.WithDescription("Write a info line to the log file")
              .WithParameter("str", [](LuaVariableInfo& p) {
                 p.WithType(LuaType::String);
              });
         });
    });

    return true;
}
