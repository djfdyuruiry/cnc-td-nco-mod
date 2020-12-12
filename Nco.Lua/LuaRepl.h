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
		if (StringIsEmpty(output))
		{
			return true;
		}

		auto length = strlen(output);

		if (!WriteConsole(stdOut, output, length, NULL, NULL))
		{
			WithWin32ErrorMessage([&] (auto e) {
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

		if (LuaType::AreEqual(luaType, LuaType::Bool))
		{
			return PrintLuaOutput(
				StringsAreEqual(valueAsString, "TRUE") ? "true" : "false"
			);
		}

		if (LuaType::AreEqual(luaType, LuaType::String))
		{
			return PrintLuaOutput("\"")
				&& PrintLuaOutput(valueAsString)
				&& PrintLuaOutput("\"");
		}

		if (LuaType::AreEqual(luaType, LuaType::Table))
		{
			auto index = -1;
			auto printStatus = PrintLuaOutput("{\r\n");

			if (!printStatus)
			{
				return false;
			}

			auto indent = RepeatString("  ", depth + 1);

			auto& tableResult = luaRuntime.GetState().IterateOverTable(stackIndex, [&](auto keyIndex, auto valueIndex, auto& _) {
				index++;

				if (!printStatus)
				{
					return;
				}

				printStatus = index == 0 ? PrintLuaOutput(indent) : PrintLuaOutput(",\r\n") && PrintLuaOutput(indent);

				if (printStatus && !luaState.IsInt(keyIndex))
				{
					// print key if it is not a number index
					printStatus = printStatus && PrintLuaOutput(luaState.ToString(keyIndex))
						&& PrintLuaOutput(" = ");
				}

				if (luaState.IsTable(valueIndex))
				{
					printStatus = printStatus && PrintLuaOutput("\r\n") && PrintLuaOutput(indent);
				}

				// output value
				printStatus = printStatus && OutputLuaResult(valueIndex, depth + 1);
			});

			delete &tableResult;
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
		input = AllocateString(MAX_INPUT_LENGTH);

		luaRuntime.GetState().ClearStack();

		// read input
		DWORD charsRead;

		if (!WriteConsole(stdOut, "> ", 2, NULL, NULL)
			|| !ReadConsole(stdIn, input, MAX_INPUT_LENGTH, &charsRead, NULL))
		{
			WithWin32ErrorMessage([&] (auto e) {
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

		if (StringsAreEqual(trimmedInput, "exit"))
		{
			delete trimmedInput;

			return false;
		}

		if (StringsAreEqual(trimmedInput, "clear"))
		{
			system("cls");
			delete trimmedInput;

			return true;
		}

		if (StringIsEmpty(trimmedInput))
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

		delete &executeResult;

		if (StringStartsWith(input, "return"))
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
