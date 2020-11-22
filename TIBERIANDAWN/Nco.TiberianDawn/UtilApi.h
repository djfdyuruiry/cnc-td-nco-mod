#pragma once

#include <vector>

#include <ILuaStateWrapper.h>
#include <Logger.h>
#include <LuaFunctionInfo.h>
#include <LuaVariableInfo.h>
#include <strings.h>
#include <utils.h>

#include "TiberianDawnNcoRuntime.h"

class UtilApi : public LuaApi
{
private:
    static int Lua_Build_Mod_Data_File_Path(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        int argCount = luaState.GetStackTop();

        if (argCount < 1)
        {
            luaState.RaiseError("buildModDataFilePath requires exactly one argument");
            return 0;
        }

        auto& filePathResult = luaState.ReadString(1);

        if (filePathResult.IsErrorResult() || String_Is_Empty(filePathResult.GetValue()))
        {
            luaState.RaiseError("buildModDataFilePath parameter `filePath` was nil or blank");
        
            delete &filePathResult;
        
            return 0;
        }

        luaState.WriteString(
            Build_Mod_Data_File_Path(filePathResult.GetValue())
        );
    
        delete &filePathResult;

        return 1;
    }

    static int Lua_Get_Mod_Data_Path(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        luaState.WriteString(Get_Mod_Data_Path());

        return 1;
    }

    static int Lua_Get_Now_In_Epoch_Millis(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        luaState.WriteBigInteger(Get_Now_In_Epoch_Millis());

        return 1;
    }

    static int Lua_Toggle_Console_Logging(lua_State* _)
    {
        GetLogger().ToggleConsoleLogging();

        return 0;
    }

    static int Lua_Set_Log_Level(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        int argCount = luaState.GetStackTop();

        if (argCount < 1)
        {
            luaState.RaiseError("setLogLevel requires exactly one argument");
            return 0;
        }

        auto& logLevelStrResult = luaState.ReadString(1);

        if (logLevelStrResult.IsErrorResult() || String_Is_Empty(logLevelStrResult.GetValue()))
        {
            luaState.RaiseError("setLogLevel parameter `logLevel` was nil or blank");

            delete &logLevelStrResult;

            return 0;
        }

        auto logLevelStr = logLevelStrResult.GetValue();
        auto logLevelUpper = Convert_String_To_Upper_Case(logLevelStr);

        auto logLevel = ParseLogLevel(logLevelUpper);

        delete logLevelUpper;

        GetLogger().SetLogLevel(logLevel);

        delete &logLevelStrResult;

        return 0;
    }

    static int Lua_Get_Log_Level(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto logLevel = LogLevelToString(GetLogger().GetLogLevel());

        luaState.WriteString(logLevel);

        return 1;
    }

    static int Lua_Show_Error(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        int argCount = luaState.GetStackTop();

        if (argCount < 1)
        {
            luaState.RaiseError("showError requires exactly one argument");
            return 0;
        }

        auto& messageResult = luaState.ReadString(1);

        if (messageResult.IsErrorResult() || String_Is_Empty(messageResult.GetValue()))
        {
            luaState.RaiseError("showError parameter `message` was nil or blank");

            delete &messageResult;

            return 0;
        }

        Show_Error(messageResult.GetValue());

        delete &messageResult;

        return 0;
    }

    static int Lua_Print(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto argCount = luaState.GetStackTop();

        if (argCount > 0)
        {
            auto& printMessageResult = luaState.ReadString(1);

            puts(printMessageResult.GetValue());

            delete &printMessageResult;
        }

        return 0;
    }

    static int Lua_Log(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto argCount = luaState.GetStackTop();

        if (argCount > 0)
        {
            auto& logMessageResult = luaState.ReadString(1);

            LogInfo("Lua => %s", logMessageResult.GetValue());

            delete &logMessageResult;
        }

        return 0;
    }

    UtilApi()
    {
        WithName("utils");

        WithDescription("Utility functions");

        WithFunction("logString", Lua_Log, [](LuaFunctionInfo& f) {
            f.WithDescription("Write a info line to the log file")
             .WithParameter("str", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });
         
        WithFunction("printString", Lua_Print, [](LuaFunctionInfo& f) {
            f.WithDescription("Print a string to standard out")
             .WithParameter("str", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });
         
        WithFunction("showErrorString", Lua_Show_Error, [](LuaFunctionInfo& f) {
            f.WithDescription("Display an error to the user in a message box")
             .WithParameter("errorString", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });
         
        WithFunction("getLogLevel", Lua_Get_Log_Level, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the current log level")
             .WithReturnValue("logLevel", [](LuaVariableInfo& p) {
                p.WithDescription("See the logging config for valid values")
                 .WithType(LuaType::String);
             });
        });
         
        WithFunction("setLogLevel", Lua_Set_Log_Level, [](LuaFunctionInfo& f) {
            f.WithDescription("Set the current log level")
             .WithParameter("logLevel", [](LuaVariableInfo& p) {
                p.WithDescription("See the logging config for valid values")
                 .WithType(LuaType::String);
             });
        });
         
        WithFunction("toggleConsoleLog", Lua_Toggle_Console_Logging, [](LuaFunctionInfo& f) {
            f.WithDescription("Toggle logging to the console on/off");
        });

        WithFunction("getNowInEpochMillis", Lua_Get_Now_In_Epoch_Millis, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the number of milliseconds between unix epoch and now")
             .WithReturnValue("nowInEpochMillis", [](LuaVariableInfo& p) {
                p.WithType(LuaType::Number);
             });
        });

        WithFunction("getModDataPath", Lua_Get_Mod_Data_Path, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the absolute path to the mod data directory")
             .WithReturnValue("modDataDirPath", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });

        WithFunction("buildModDataFilePath", Lua_Build_Mod_Data_File_Path, [](LuaFunctionInfo& f) {
            f.WithDescription("Build the absolute path to a file in the mod data directory")
             .WithParameter("filename", [](LuaVariableInfo& p) {
                p.WithDescription("Mod data filename")
                 .WithType(LuaType::String);
             })
             .WithReturnValue("filePath", [](LuaVariableInfo& p) {
                 p.WithDescription("Absolute file path for given filename")
                  .WithType(LuaType::String);
              });
        });
    }

public:
    static ILuaApi& Build()
    {
        return *(new UtilApi());
    }

};
