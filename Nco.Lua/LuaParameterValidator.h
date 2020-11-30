#pragma once

#include <strings.h>

#include "ILuaStateWrapper.h"
#include "LuaFunctionInfo.h"

class LuaParameterValidator final
{
private:
	static bool ValidateParameter(LuaFunctionInfo& functionInfo, ILuaStateWrapper& lua, LuaVariableInfo& parameter)
	{
		auto parameterIndex = parameter.GetParameterIndex();
		auto& parameterType = parameter.GetType();

		if (LuaType::AreEqual(parameterType, LuaType::Any))
		{
			if (!lua.IsNil(parameterIndex))
			{
				return true;
			}

			lua.RaiseError(
				"%s parameter `%s` must be not be nil",
				functionInfo.GetName(),
				parameter.GetName()
			);

			return false;
		}

		auto& actualType = lua.GetLuaType(parameterIndex);

		if (LuaType::AreEqual(parameterType, actualType))
		{
			return true;
		}

		lua.RaiseError(
			"%s parameter `%s` must be a %s",
			functionInfo.GetName(),
			parameter.GetName(),
			parameterType.value
		);

		return false;
	}

	static bool NumberOfPararmetersIsCorrect(LuaFunctionInfo& functionInfo, ILuaStateWrapper& lua)
	{
		unsigned int numberOfParametersExpected = functionInfo.GetRequiredParameterCount();
		unsigned int numberOfParametersPassed = lua.GetStackTop();

		if (numberOfParametersPassed < numberOfParametersExpected)
		{
			lua.RaiseError("%s requires at least %d parameters", functionInfo.GetName(), numberOfParametersExpected);

			return false;
		}

		return true;
	}

public:
	static bool ValidateInvocationParameters(LuaFunctionInfo& functionInfo, ILuaStateWrapper& lua)
	{
		if (!functionInfo.HasParameters())
		{
			return true;
		}

		if (!NumberOfPararmetersIsCorrect(functionInfo, lua))
		{
			return false;
		}

		for (auto parameter : functionInfo.GetParameters())
		{
			if (!ValidateParameter(functionInfo, lua, *parameter))
			{
				return false;
			}
		}

		return true;
	}

};
