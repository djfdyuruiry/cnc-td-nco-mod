#pragma once

template<class T> class TypeApiParameters final
{
private:
	bool invalidParameters;

	TypeApiParameters()
	{
	}

public:
	const char* typeInstanceName;
	const char* ruleName;
	T* typeInstance;

	static TypeApiParameters& BuildValid(const char* typeInstanceName, const char* ruleName, T* typeInstance)
	{
		auto& params = *(new TypeApiParameters());

		params.invalidParameters = false;
		params.typeInstanceName = typeInstanceName;
		params.ruleName = ruleName;
		params.typeInstance = typeInstance;

		return params;
	}

	static TypeApiParameters& BuildInvalid()
	{
		auto& params = *(new TypeApiParameters());

		params.invalidParameters = true;

		return params;
	}

	bool IsValid()
	{
		return !invalidParameters;
	}
};
