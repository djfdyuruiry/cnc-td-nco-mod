#pragma once

class Optional
{
private:
	void* data;

	Optional() 
	{
		data = NULL;
	}

public:
	static Optional& BuildOptional()
	{
		return *(new Optional());
	}

	~Optional()
	{
		if (Present())
		{
			delete data;
		}
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
		if (Present())
		{
			delete (T*)data;
		}

		data = calloc(1, sizeof(T));

		*(T*)data = value;
	}
};
