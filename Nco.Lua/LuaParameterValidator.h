#pragma once

#include <strings.h>

#include "ILuaStateWrapper.h"
#include "LuaFunctionInfo.h"

class LuaParameterValidator final
{
public:
	// TODO: use in non-method based calls by getting function info ptr upvalue added by LuaAppi
	static bool ValidateInvocationParameters(LuaFunctionInfo& functionInfo, ILuaStateWrapper& lua)
	{
		if (!functionInfo.HasParameters())
		{
			return true;
		}

		unsigned int numberOfParametersPassed = lua.GetStackTop();

		for (auto parameterPtr : functionInfo.GetParameters())
		{
			auto& parameter = *parameterPtr;
			auto parameterIndex = parameter.GetParameterIndex();
			auto isPresent = numberOfParametersPassed >= parameterIndex;

			if (!isPresent)
			{
				if (parameter.IsOptional())
				{
					continue;
				}

				lua.RaiseError("%s parameter `%s` is required", functionInfo.GetName(), parameter.GetName());

				return false;
			}

			auto& parameterType = parameter.GetType();
			auto& actualType = lua.GetLuaType(parameterIndex);

			if (LuaType::AreEqual(parameterType, actualType))
			{
				continue;
			}

			lua.RaiseError(
				"%s parameter `%s` must be a %s",
				functionInfo.GetName(),
				parameter.GetName(),
				parameterType.value
			);

			return false;
		}

		return true;
	}

};
