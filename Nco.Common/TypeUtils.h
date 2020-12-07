#pragma once

#include <map>
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
			std::is_assignable_v<T, char>
			|| std::is_assignable_v<T, unsigned char>
			|| std::is_assignable_v<T, short>
			|| std::is_assignable_v<T, unsigned short>
			|| std::is_assignable_v<T, int>
			|| std::is_assignable_v<T, unsigned int>
			|| std::is_assignable_v<T, long>
			|| std::is_assignable_v<T, unsigned long>
			|| std::is_assignable_v<T, float>
			|| std::is_assignable_v<T, double>
			|| std::is_assignable_v<T, bool>
		)
		{
			return (T)0;
		}
		else
		{
			return (T)NULL;
		}
	}

	template<class T, class U> static std::map<T, U>& InitMapAsRef(std::map<T, U> valueMap)
	{
		auto& refMap = *(new std::map<T, U>());

		for (auto& [k, v] : valueMap)
		{
			refMap[k] = v;
		}

		return refMap;
	}

};
