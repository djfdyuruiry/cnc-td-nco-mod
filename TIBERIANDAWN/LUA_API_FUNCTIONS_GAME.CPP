#include "function.h"

static SuperClass* Get_House_Superweapon(lua_State* lua, int index, HouseClass* house, const char* callingFunctionName)
{
    auto superWeaponName = Get_Lua_Value_As_String(index);

    if (String_Is_Empty(superWeaponName))
    {
        luaL_error(lua, "%s parameter `superWeaponName` was nil or empty", callingFunctionName);

        return NULL;
    }

    auto uppercaseName = Convert_String_To_Upper_Case(superWeaponName);

    SuperClass* weapon = NULL;

    if (Strings_Are_Equal(uppercaseName, AIRSTRIKE_SECTION_NAME_UPPER))
    {
        weapon = (SuperClass*)&house->AirStrike;
    }
    else if (Strings_Are_Equal(uppercaseName, ION_CANNON_SECTION_NAME_UPPER))
    {
        weapon = (SuperClass*)&house->IonCannon;
    }
    else if (Strings_Are_Equal(uppercaseName, NUCLEAR_STRIKE_SECTION_NAME_UPPER))
    {
        weapon = (SuperClass*)&house->NukeStrike;
    }
    else
    {
        luaL_error(lua, "%s parameter `superWeaponName` was not recognised as a superweapon name: %s", callingFunctionName, superWeaponName);
    }

    return weapon;
}

static HouseClass* Get_House_By_Type(lua_State* lua, HousesType houseType)
{
    for (auto i = 0; i < Houses.Count(); i++)
    {
        auto house = Houses.Raw_Ptr(i);

        if (house != NULL && house->Class->House == houseType)
        {
            return house;
        }
    }

    luaL_error(lua, "House is not currently active in the game: %s", House_Type_To_String(houseType));

    return NULL;
}

static HousesType Lua_Parse_House_Type(lua_State* lua, const char* houseTypeString, const char* callingFunctionName, bool* parseError)
{
    auto upperHouseTypeString = Convert_String_To_Upper_Case(houseTypeString);
    auto houseType = Parse_House_Type(upperHouseTypeString, parseError);

    delete upperHouseTypeString;

    if (*parseError)
    {
        luaL_error(lua, "%s parameter `houseName` was not a valid house: %s", callingFunctionName, houseTypeString);

        return HOUSE_NONE;
    }

    return houseType;
}

static HouseClass* Lua_Get_House_Parameter(lua_State* lua, int index, const char* callingFunctionName)
{
    auto houseName = Get_Lua_Value_As_String(index);

    if (String_Is_Empty(houseName))
    {
        luaL_error(lua, "%s parameter `houseName` was nil or blank", callingFunctionName);
        return NULL;
    }

    bool parseError = false;
    auto houseType = Lua_Parse_House_Type(lua, houseName, callingFunctionName, &parseError);

    if (parseError)
    {
        return NULL;
    }

    auto house = Get_House_By_Type(lua, houseType);

    return house;
}

static int Lua_Send_House_Super_Weapon_Message(
    lua_State* lua,
    const char* callingFunctionName,
    const char* callingLuaFunctionName,
    HouseSuperweaponMessageType messageType
) {
    Log_Trace("%s called from Lua", callingFunctionName);

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "%s requires at least two arguments", callingLuaFunctionName);
        return 0;
    }

    auto house = Lua_Get_House_Parameter(lua, 1, callingLuaFunctionName);

    if (house == NULL)
    {
        return 0;
    }

    if (house->SuperweaponMessage.unread) {
        luaL_error(lua, "Given house already has a superweapon change pending");
        return 0;
    }

    auto superweapon = Get_House_Superweapon(lua, 2, house, callingLuaFunctionName);

    if (superweapon == NULL)
    {
        return 0;
    }

    auto oneTime = false;

    if (argCount > 2)
    {
        if (!lua_isboolean(lua, 3))
        {
            luaL_error(lua, "%s parameter `oneTimeOnly` must be a boolean", callingLuaFunctionName);
            return 0;
        }

        oneTime = lua_toboolean(lua, 3);
    }

    house->SuperweaponMessage.oneTime = oneTime;
    house->SuperweaponMessage.weapon = superweapon;
    house->SuperweaponMessage.type = messageType;

    house->SuperweaponMessage.unread = true;

    return 0;
}


static int Lua_Disable_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "disableSuperWeaponForHouse",
        DISABLE_SUPERWEAPON
    );
}

static int Lua_Charge_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "chargeSuperWeaponForHouse",
        CHARGE_SUPERWEAPON
    );
}

static int Lua_Enable_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "enableSuperWeaponForHouse",
        ENABLE_SUPERWEAPON
    );
}

static int Lua_Modifiy_House_Credits(lua_State* lua)
{
    Log_Trace("Lua_Modify_House_Credits called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "modifyHouseCredits requires exactly two arguments");
        return 0;
    }

    auto house = Lua_Get_House_Parameter(lua, 1, "modifyHouseCredits");

    if (!lua_isnumber(lua, 2))
    {
        luaL_error(lua, "modifyHouseCredits parmeter `creditsModifier` must be a number");
        return 0;
    }

    auto creditsModifier = luaL_checknumber(lua, 2);

    house->Credits += creditsModifier;

    lua_pushnumber(lua, house->Credits);

    return 1;
}

static int Lua_Game_Busy_Error(lua_State* lua)
{
    luaL_error(lua, "The game is has another command pending");
    return 0;
}

static int Lua_Refresh_Sidebar(lua_State* lua)
{
    Log_Trace("Lua_Refresh_Sidebar called from Lua");

    if (!Push_Refresh_Sidebar_Ui_Message())
    {
        return Lua_Game_Busy_Error(lua);
    }

    return 0;
}

static int Lua_Show_Game_Message(lua_State* lua)
{
    Log_Trace("Lua_Show_Game_Message called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "showGameMessage requires exactly two arguments");
        return 0;
    }

    if (!lua_isnumber(lua, 2))
    {
        luaL_error(lua, "showGameMessage parameter `durationInSeconds` must be a number");
        return 0;
    }

    auto message = Get_Lua_Value_As_String(1);
    auto durationInSeconds = luaL_checknumber(lua, 2);

    if (message == NULL)
    {
        luaL_error(lua, "showGameMessage parameter `message` was nil");
        return 0;
    }

    if (durationInSeconds < 0.1)
    {
        luaL_error(lua, "showGameMessage parameter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");
        return 0;
    }

    if (!Push_Show_Notification_Ui_Message(message, durationInSeconds))
    {
        return Lua_Game_Busy_Error(lua);
    }

    return 0;
}

bool Register_Game_Functions()
{
    Log_Debug("Initialising Lua API game functions");

    Register_Lua_Function("showGameMessage", Lua_Show_Game_Message);
    Register_Lua_Function("refreshSidebar", Lua_Refresh_Sidebar);
    Register_Lua_Function("modifyHouseCredits", Lua_Modifiy_House_Credits);
    Register_Lua_Function("enableSuperweaponForHouse", Lua_Enable_House_Super_Weapon);
    Register_Lua_Function("chargeSuperweaponForHouse", Lua_Charge_House_Super_Weapon);
    Register_Lua_Function("disableSuperweaponForHouse", Lua_Disable_House_Super_Weapon);

    return true;
}
