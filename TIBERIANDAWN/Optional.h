#pragma once

class Optional
{
private:
	void* data;

public:
	Optional()
	{
		data = NULL;
	}

	bool Present()
	{
		return data != NULL;
	}

	template<class T> T Get()
	{
		return *(T*)data;
	}

	template<class T> T GetOrDefault(T defaultValue)
	{
		return Present()
			? *(T*)data
			: defaultValue;
	}

	template<class T> void Set(T value)
	{
		data = calloc(1, sizeof(T));

		*(T*)data = value;
	}
};
