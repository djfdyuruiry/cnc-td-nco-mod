#pragma once

#include <stdio.h>

#include <strings.h>
#include <Thread.h>
#include <utils.h>

#include "ILuaRuntime.h"
#include "LuaType.h"

class LuaRepl : public Thread
{
private:
	const int MAX_INPUT_LENGTH = 2048;

	ILuaRuntime& luaRuntime;
	HANDLE stdOut;
	HANDLE stdIn;
	char* input;

	LuaRepl(ILuaRuntime& luaRuntime) : Thread("Lua REPL"), luaRuntime(luaRuntime), input(NULL)
	{
	}

	bool PrintLuaOutput(const char* output)
	{
		auto length = strlen(output);

		if (!WriteConsole(stdOut, output, length, NULL, NULL))
		{
			With_Win32_Error_Message([&] (auto e) {
				LogError("Lua REPL error: %s", e);
			});

			return false;
		}

		return true;
	}

	bool OutputLuaResult(int stackIndex, unsigned int depth = 0)
	{
		auto& luaState = luaRuntime.GetState();

		auto valueAsString = luaState.ToString(stackIndex);
		auto luaType = luaState.GetLuaType(stackIndex);

		if (luaType.value == LuaType::Bool->value)
		{
			return PrintLuaOutput(
				Strings_Are_Equal(valueAsString, "TRUE") ? "true" : "false"
			);
		}

		if (luaType.value == LuaType::String->value)
		{
			return PrintLuaOutput("\"")
				&& PrintLuaOutput(valueAsString)
				&& PrintLuaOutput("\"");
		}

		if (luaType.value == LuaType::Table->value)
		{
			auto index = -1;
			auto printStatus = PrintLuaOutput("{\r\n");

			if (!printStatus)
			{
				return false;
			}

			auto indent = RepeatString("  ", depth + 1);

			luaRuntime.GetState().IterateOverTable(stackIndex, [&]() {
				index++;

				if (!printStatus)
				{
					return;
				}

				printStatus = index == 0 ? PrintLuaOutput(indent) : PrintLuaOutput(",\r\n") && PrintLuaOutput(indent);

				if (printStatus && !luaState.IsInt(-1))
				{
					// print key if it is not a number index
					printStatus = printStatus && PrintLuaOutput(luaState.ToString(-1)) 
						&& PrintLuaOutput(" = \r\n")
						&& PrintLuaOutput(indent);
				}

				// output value
				printStatus = printStatus && OutputLuaResult(-2, depth + 1);
			});

			delete indent;

			indent = RepeatString("  ", depth);

			printStatus = printStatus && PrintLuaOutput("\r\n") 
				&& PrintLuaOutput(indent)
				&& PrintLuaOutput("}");

			delete indent;

			return printStatus;
		}
		
		if (valueAsString != NULL)
		{
			return PrintLuaOutput(valueAsString);
		}

		return PrintLuaOutput(luaType.value);
	}

	bool PrintLuaResult()
	{
		return OutputLuaResult(-1)
			&& PrintLuaOutput("\r\n");
	}

	bool ReadLuaString()
	{
		if (input != NULL)
		{
			delete input;
		}

		// reset all C and Lua variables
		input = Allocate_String(MAX_INPUT_LENGTH);

		luaRuntime.GetState().ClearStack();

		// read input
		DWORD charsRead;

		if (!WriteConsole(stdOut, "> ", 2, NULL, NULL)
			|| !ReadConsole(stdIn, input, MAX_INPUT_LENGTH, &charsRead, NULL))
		{
			With_Win32_Error_Message([&] (auto e) {
				LogError("Lua REPL error: %s", e);
			});

			return false;
		}

		return true;
	}

	bool ProcessInput()
	{
		if (!ReadLuaString())
		{
			return false;
		}

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
			auto printResult = false;

			printResult = PrintLuaOutput(executeResult.GetError())
				&& PrintLuaOutput("\r\n");

			delete &executeResult;

			return printResult;
		}

		delete& executeResult;

		if (String_Starts_With(input, "return"))
		{
			return PrintLuaResult();
		}

		return true;
	}

protected:
	DWORD Run()
	{
		stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		stdIn = GetStdHandle(STD_INPUT_HANDLE);

		if (!Win32HandleIsValid(stdOut) || !Win32HandleIsValid(stdIn))
		{
			stdOut = NULL;
			stdIn = NULL;

			LogError("Lua REPL error: failed to open I/O streams");

			return 1;
		}

		GetLogger().ToggleConsoleLogging();
		system("cls");

		puts("=======================");
		puts("  NCO Mod Lua Console  ");
		puts("=======================");
		printf("\nHelp: https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki/09.-Lua-Scripting-API\n\n");

		auto processInputResult = false;

		while (processInputResult = ProcessInput())
		{
		}

		GetLogger().ToggleConsoleLogging();

		return processInputResult ? 0 : 1;
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

		CloseWin32HandleIfValid(stdOut);
		CloseWin32HandleIfValid(stdIn);
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
