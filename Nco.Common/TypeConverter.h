#pragma once

#include <type_traits>

#include "ConvertResult.h"
#include "Strings.h"

class TypeConverter
{
protected:
	TypeConverter()
	{
	}

	virtual bool IsExtendedType()
	{
		return false;
	}

	virtual template <class T> ConvertResult<T>& DeserialiseExtendedType(const char* stringValue)
	{

	}

public:
	template <class T> ConvertResult<T>& Deserialise(const char* stringValue)
	{
		if (StringIsEmpty(stringValue))
		{
			return ConvertResult<T>::BuildWithError("Unable to deserialise value from null/blank string");
		}

		if constexpr (
			std::is_assignable_v<T, char>
			|| std::is_assignable_v<T, unsigned char>
			|| std::is_assignable_v<T, short>
			|| std::is_assignable_v<T, unsigned short>
			|| std::is_assignable_v<T, int>
			|| std::is_assignable_v<T, unsigned int>
			|| std::is_assignable_v<T, long>
			|| std::is_assignable_v<T, unsigned long>
			|| std::is_assignable_v<T, long long>
			|| std::is_assignable_v<T, unsigned long long>
		)
		{
			if (IsIntString(stringValue))
			{
				return ConvertResult<T>::BuildWithValue(
					(T)strtoull(stringValue, null, 10)
				);
			}
		}

		return ConvertResult<T>::BuildWithError("No converter registered for type");
	}

};
