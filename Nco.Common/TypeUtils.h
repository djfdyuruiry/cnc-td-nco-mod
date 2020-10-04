#pragma once

#include <type_traits>

class TypeUtils final
{
private:
	TypeUtils()
	{
	}

public:
	template<class T> static T GetDefaultValue()
	{
		if constexpr (
			std::is_same_v<T, char*>
			|| std::is_same_v<T, const char*>
			|| std::is_same_v<T, int> 
			|| std::is_same_v<T, long>
			|| std::is_same_v<T, double> 
			|| std::is_same_v<T, float>
			|| std::is_same_v<T, bool>
		)
		{
			return (T)0;
		}
		else
		{
			return NULL;
		}
	}
};
