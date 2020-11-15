#pragma once

#include <stdio.h>

#include <strings.h>
#include <Thread.h>

#include "ILuaRuntime.h"
#include "LuaType.h"

class LuaRepl : public Thread
{
private:
	ILuaRuntime& luaRuntime;
	HANDLE stdOut;
	HANDLE stdIn;
	char* input;

	LuaRepl(ILuaRuntime& luaRuntime) : Thread("Lua REPL"), luaRuntime(luaRuntime), input(NULL)
	{
	}

	void PrintLuaOutput(const char* output)
	{
		auto length = strlen(output);

		if (!WriteConsole(stdOut, output, length, NULL, NULL))
		{
			Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

			exit(1);
		}
	}

	void OutputLuaResult(int stackIndex)
	{
		auto& luaState = luaRuntime.GetState();

		auto valueAsString = luaState.ToString(stackIndex);
		auto luaType = luaState.GetLuaType(stackIndex);

		if (luaType.value == LuaType::Bool.value)
		{
			PrintLuaOutput(
				Strings_Are_Equal(valueAsString, "TRUE") ? "true" : "false"
			);

			return;
		}

		if (luaType.value == LuaType::String.value)
		{
			PrintLuaOutput("\"");
			
			PrintLuaOutput(valueAsString);

			PrintLuaOutput("\"");

			return;
		}

		if (luaType.value == LuaType::Table.value)
		{
			auto index = 0;

			PrintLuaOutput("{\r\n");

			luaRuntime.GetState().IterateOverTable(stackIndex, [&]() {
				PrintLuaOutput(index == 0 ? "  " : ",\r\n  ");

				OutputLuaResult(-2);

				index++;
			});

			PrintLuaOutput("\r\n}");

			return;
		}
		
		if (valueAsString != NULL)
		{
			PrintLuaOutput(valueAsString);

			return;
		}

		PrintLuaOutput(luaType.value);
	}

	void PrintLuaResult()
	{
		OutputLuaResult(-1);

		PrintLuaOutput("\r\n");
	}

	void ReadLuaString()
	{
		if (input != NULL)
		{
			delete input;
		}

		// reset all C and Lua variables
		input = Allocate_String(2048);

		luaRuntime.GetState().ClearStack();

		// read input
		DWORD charsRead;

		if (!WriteConsole(stdOut, "> ", 2, NULL, NULL)
			|| !ReadConsole(stdIn, input, 2048, &charsRead, NULL))
		{
			Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

			exit(1);
		}
	}

	bool ProcessInput()
	{
		ReadLuaString();

		// strip newline
		auto trimmedInput = ExtractSubstring(input, -2);

		if (Strings_Are_Equal(trimmedInput, "exit"))
		{
			delete trimmedInput;

			return false;
		}

		if (Strings_Are_Equal(trimmedInput, "clear"))
		{
			system("cls");
			delete trimmedInput;

			return true;
		}

		if (String_Is_Empty(trimmedInput))
		{
			delete trimmedInput;

			return true;
		}

		auto& executeResult = luaRuntime.ExecuteScript(trimmedInput);

		delete trimmedInput;

		// evaluate lua string
		if (executeResult.IsErrorResult())
		{
			PrintLuaOutput(executeResult.GetError());
			PrintLuaOutput("\r\n");

			delete &executeResult;

			return true;
		}

		delete& executeResult;

		if (String_Starts_With(input, "return"))
		{
			PrintLuaResult();
		}

		return true;
	}

protected:
	DWORD Run()
	{
		stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		stdIn = GetStdHandle(STD_INPUT_HANDLE);

		Toggle_Console_Logging();
		system("cls");

		puts("=======================");
		puts("  NCO Mod Lua Console  ");
		puts("=======================");
		printf("\nHelp: https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki/09.-Lua-Scripting-API\n\n");

		while (ProcessInput())
		{
		}

		Toggle_Console_Logging();

		return 0;
	}

public:
	static LuaRepl& Build(ILuaRuntime& luaRuntime)
	{
		return *(new LuaRepl(luaRuntime));
	}

	~LuaRepl()
	{
		if (input != NULL)
		{
			delete input;
		}

		CloseHandle(stdOut);
		CloseHandle(stdIn);
	}

	void Enter()
	{
		Run();
	}

	bool EnterAsync()
	{
		return Start();
	}

};
