#pragma once

#include <vector>

#include <ILuaStateWrapper.h>
#include <Logger.h>
#include <LuaApi.h>
#include <LuaFunctionInfo.h>
#include <LuaVariableInfo.h>
#include <strings.h>

#include "../FUNCTION.H"

#include "game_utils.h"
#include "parse.h"
#include "rules_ini_superweapon.h"
#include "TiberianDawnNcoRuntime.h"

class GameApi : public LuaApi
{
private:
    static HousesType Lua_ParseHouseType(int index, const char* callingFunctionName, bool* parseError)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto& houseTypeStringResult = luaState.ReadString(index);
        auto upperHouseTypeString = ConvertStringToUpperCase(houseTypeStringResult.GetValue());
        auto houseType = ParseHouseType(upperHouseTypeString, parseError);

        delete upperHouseTypeString;

        if (*parseError)
        {
            luaState.RaiseError(
                "%s parameter `houseName` was not a valid house: %s",
                callingFunctionName,
                houseTypeStringResult.GetValue()
            );

            delete &houseTypeStringResult;

            return HOUSE_NONE;
        }

        delete &houseTypeStringResult;

        return houseType;
    }

    static int Lua_Clear_House_Messages(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Clear_House_Messages called from Lua");

        auto argCount = luaState.GetStackTop();

        if (argCount < 1)
        {
            luaState.RaiseError("clearHouseMessages requires at least one argument");

            return 0;
        }

        bool parseError = false;
        auto house = Lua_ParseHouseType(1, "clearHouseMessages", &parseError);

        if (parseError)
        {
            return 0;
        }

        Clear_House_Messages(house);

        return 0;
    }

    static int Lua_Clear_Game_Loop_Messages(lua_State* _)
    {
        Clear_Game_Loop_Messages();

        return 0;
    }

    static int Lua_Clear_Game_Ui_Messages(lua_State* _)
    {
        Clear_Game_Ui_Messages();

        return 0;
    }


    static HouseClass* Get_House_By_Type(HousesType houseType, bool* housePresent)
    {
        for (auto i = 0; i < Houses.Count(); i++)
        {
            auto house = Houses.Raw_Ptr(i);

            if (house != NULL && house->Class->House == houseType)
            {
                if (housePresent != NULL)
                {
                    *housePresent = true;
                }

                return house;
            }
        }

        if (housePresent != NULL)
        {
            *housePresent = false;
        }

        return NULL;
    }

    static SuperweaponType Lua_ParseSuperweaponType(int index, const char* callingFunctionName, bool* invalidValue)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto& superWeaponNameResult = luaState.ReadString(index);

        if (superWeaponNameResult.IsErrorResult() || StringIsEmpty(superWeaponNameResult.GetValue()))
        {
            luaState.RaiseError("%s parameter `superWeaponName` was nil or empty", callingFunctionName);

            if (invalidValue != NULL)
            {
                *invalidValue = true;
            }

            delete &superWeaponNameResult;

            return NO_SUPERWEAPON;
        }

        auto weapon = NO_SUPERWEAPON;
        auto uppercaseName = ConvertStringToUpperCase(superWeaponNameResult.GetValue());

        if (StringsAreEqual(uppercaseName, AIRSTRIKE_SECTION_NAME_UPPER))
        {
            weapon = AIRSTRIKE;
        }
        else if (StringsAreEqual(uppercaseName, ION_CANNON_SECTION_NAME_UPPER))
        {
            weapon = ION_CANNON;
        }
        else if (StringsAreEqual(uppercaseName, NUCLEAR_STRIKE_SECTION_NAME_UPPER))
        {
            weapon = NUCLEAR_STRIKE;
        }
        else
        {
            luaState.RaiseError(
                "%s parameter `superWeaponName` was not recognised as a superweapon name: %s",
                callingFunctionName,
                superWeaponNameResult.GetValue()
            );
        }

        delete uppercaseName;
        delete &superWeaponNameResult;

        return weapon;
    }

    static int Lua_Send_House_Super_Weapon_Message(
        const char* callingFunctionName,
        const char* callingLuaFunctionName,
        SuperweaponMessageType messageType
    ) {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("%s called from Lua", callingFunctionName);

        auto argCount = luaState.GetStackTop();

        if (argCount < 2)
        {
            luaState.RaiseError("%s requires at least two arguments", callingLuaFunctionName);
            return 0;
        }

        bool parseError = false;
        auto house = Lua_ParseHouseType(1, callingLuaFunctionName, &parseError);

        if (parseError)
        {
            return 0;
        }

        auto superweapon = Lua_ParseSuperweaponType(2, callingLuaFunctionName, &parseError);

        if (parseError)
        {
            return 0;
        }

        auto oneTime = false;

        if (argCount > 2)
        {
            if (!luaState.IsBool(3))
            {
                luaState.RaiseError("%s parameter `oneTimeOnly` must be a boolean", callingLuaFunctionName);
                return 0;
            }

            oneTime = luaState.ReadBool(3).GetValue();
        }

        HouseMessage message{};

        message.oneTime = oneTime;
        message.superweapon = superweapon;
        message.superweaponMessage = messageType;

        Push_House_Message(house, SUPERWEAPON_MESSAGE, message);

        return 0;
    }

    static int Lua_Disable_House_Super_Weapon(lua_State* _) {
        return Lua_Send_House_Super_Weapon_Message(
            "Lua_Enable_House_Super_Weapon_Message",
            "disableSuperWeaponForHouse",
            DISABLE_SUPERWEAPON
        );
    }

    static int Lua_Charge_House_Super_Weapon(lua_State* _) {
        return Lua_Send_House_Super_Weapon_Message(
            "Lua_Enable_House_Super_Weapon_Message",
            "chargeSuperWeaponForHouse",
            CHARGE_SUPERWEAPON
        );
    }

    static int Lua_Enable_House_Super_Weapon(lua_State* _) {
        return Lua_Send_House_Super_Weapon_Message(
            "Lua_Enable_House_Super_Weapon_Message",
            "enableSuperWeaponForHouse",
            ENABLE_SUPERWEAPON
        );
    }

    static int Lua_Modifiy_House_Credits(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Modify_House_Credits called from Lua");

        int argCount = luaState.GetStackTop();

        if (argCount < 2)
        {
            luaState.RaiseError("modifyHouseCredits requires exactly two arguments");
            return 0;
        }

        bool invalidValue = false;
        auto houseType = Lua_ParseHouseType(1, "modifyHouseCredits", &invalidValue);

        if (invalidValue)
        {
            return 0;
        }

        if (!luaState.IsNumber(2))
        {
            luaState.RaiseError("modifyHouseCredits parmeter `creditsModifier` must be a number");
            return 0;
        }

        bool housePresent = false;
        auto& creditsModifierResult = luaState.ReadInteger(2);
        auto house = Get_House_By_Type(houseType, &housePresent);

        if (!housePresent)
        {
            LogError("Ignoring modifyHouseCredits call as house '%s' is not in the current game", HouseTypeToString(houseType));

            delete &creditsModifierResult;

            return 0;
        }

        house->Credits += creditsModifierResult.GetValue();

        luaState.WriteInteger(house->Credits);

        delete &creditsModifierResult;

        return 1;
    }

    static int Lua_Get_Active_Houses(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Get_Active_Houses called from Lua");

        auto activeHousesCount = Houses.Count();
        auto houses = std::vector<const char*>();

        for (auto i = 0; i < activeHousesCount; i++)
        {
            auto house = Houses.Raw_Ptr(i);

            houses.push_back(
                HouseTypeToString(house->Class->House)
            );
        }

        luaState.WriteArray(houses);

        return 1;
    }

    static int Lua_Get_Player_Base_House(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Get_Player_House called from Lua");

        auto playerBaseHouse = HouseTypeToString(PlayerPtr->ActLike);

        luaState.WriteString(playerBaseHouse);

        return 1;
    }

    static int Lua_Get_Player_House(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Get_Player_House called from Lua");

        auto playerHouse = HouseTypeToString(PlayerPtr->Class->House);

        luaState.WriteString(playerHouse);

        return 1;
    }

    static int Lua_Hide_Map(lua_State* _)
    {
        LogTrace("Lua_Hide_Map called from Lua");

        Push_Hide_Map_Ui_Message();

        return 0;
    }

    static int Lua_Reveal_Map(lua_State* _)
    {
        LogTrace("Lua_Reveal_Map called from Lua");

        Push_Reveal_Map_Ui_Message();

        return 0;
    }

    static int Lua_Refresh_Map(lua_State* _)
    {
        LogTrace("Lua_Refresh_Map called from Lua");

        Push_Refresh_Map_Ui_Message();

        return 0;
    }

    static int Lua_Refresh_Sidebar(lua_State* _)
    {
        LogTrace("Lua_Refresh_Sidebar called from Lua");

        Push_Refresh_Sidebar_Ui_Message();

        return 0;
    }

    static int Lua_Show_Game_Message(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        LogTrace("Lua_Show_Game_Message called from Lua");

        int argCount = luaState.GetStackTop();

        if (argCount < 2)
        {
            luaState.RaiseError("showGameMessage requires exactly two arguments");
            return 0;
        }

        if (!luaState.IsString(1))
        {
            luaState.RaiseError("showGameMessage parameter `message` must be a string");
            return 0;
        }

        if (!luaState.IsNumber(2))
        {
            luaState.RaiseError("showGameMessage parameter `durationInSeconds` must be a number");
            return 0;
        }

        auto& messageResult = luaState.ReadString(1);
        auto& durationInSecondsResult = luaState.ReadDouble(2);

        if (messageResult.IsErrorResult() || messageResult.GetValue() == NULL)
        {
            luaState.RaiseError("showGameMessage parameter `message` was nil");
            return 0;
        }

        if (durationInSecondsResult.GetValue() < 0.1)
        {
            luaState.RaiseError("showGameMessage parameter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");
            return 0;
        }

        Push_Show_Notification_Ui_Message(messageResult.GetValue(), durationInSecondsResult.GetValue());

        delete &messageResult;
        delete &durationInSecondsResult;

        return 0;
    }

    GameApi()
    {
        WithName("Game");
        WithDescription("Game info and control functions");

        WithFunction("showGameMessage", Lua_Show_Game_Message, [](LuaFunctionInfo& f) {
            f.WithDescription("Show the player an in-game message")
             .WithParameter("message", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             })
             .WithParameter("durationInSeconds", [](LuaVariableInfo& p) {
                p.WithDescription("Display the message for this many seconds; minimum value is `0.1`. Note: this function is async")
                 .WithType(LuaType::Number);
             });
        });

        WithFunction("refreshSidebar", Lua_Refresh_Sidebar, [](LuaFunctionInfo& f) {
            f.WithDescription("Force a refresh of the sidebar; useful if you make a rule change that affects what the user can build and want to reflect that right away. Note: this function is async");
        });

        WithFunction("revealEntireMap", Lua_Reveal_Map, [](LuaFunctionInfo& f) {
            f.WithDescription("Reveal the entire map to the player; this is permanent unless you call `hideEntireMap`. Note: this function is async");
        });

        WithFunction("hideEntireMap", Lua_Hide_Map, [](LuaFunctionInfo& f) {
            f.WithDescription("Hide any areas of the map without player units/buildings from the player. Note: this function is async");
        });

        WithFunction("getPlayerHouse", Lua_Get_Player_House, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the current player house")
             .WithReturnValue("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("The house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("getPlayerBaseHouse", Lua_Get_Player_Base_House, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the base house for the current player house. Every house has a base house which it acts like, usually NOD or GDI.")
             .WithReturnValue("baseHouseName", [](LuaVariableInfo& p) {
                p.WithDescription("The base house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("getActiveHouses", Lua_Get_Active_Houses, [](LuaFunctionInfo& f) {
            f.WithDescription("Get all houses that are active in the current scenario")
             .WithReturnValue("activeHouseNames", [](LuaVariableInfo& p) {
                p.WithDescription("List of strings, containing the names of active houses.")
                 .WithType(LuaType::Table);
             });
        });

        WithFunction("modifyHouseCredits", Lua_Modifiy_House_Credits, [](LuaFunctionInfo& f) {
            f.WithDescription("Write a info line to the log file")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to modify")
                 .WithType(LuaType::String);
             })
             .WithParameter("amount", [](LuaVariableInfo& p) {
                p.WithDescription("Amount of credits to either add or subtract for current house balance; use negative numbers to deduct credits.")
                 .WithType(LuaType::Number);
             });
        });

        WithFunction("enableSuperweaponForHouse", Lua_Enable_House_Super_Weapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Enable a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to enable superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("chargeSuperweaponForHouse", Lua_Charge_House_Super_Weapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Charge a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to charge superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("disableSuperweaponForHouse", Lua_Disable_House_Super_Weapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Disable a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to disable superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("clearGameUiMessages", Lua_Clear_Game_Ui_Messages, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending async game UI changes (refresh sidebar, reveal map etc.)");
        });

        WithFunction("clearGameLoopMessages", Lua_Clear_Game_Loop_Messages, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending async game events (on game tick etc.)");
        });

        WithFunction("clearHouseMessages", Lua_Clear_House_Messages, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending events for a given house (enable/disable superweapon etc.)")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                 p.WithDescription("Name of the house to cancelevents for")
                  .WithType(LuaType::String);
             });
        });
    }

public:
    static ILuaApi& Build()
    {
        return *(new GameApi());
    }

};
