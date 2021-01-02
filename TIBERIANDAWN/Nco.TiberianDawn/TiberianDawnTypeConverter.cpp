#include "TiberianDawnNcoRuntime.h"
#include "TiberianDawnTypeConverter.h"

ResultWithValue<int>& TiberianDawnTypeConverter::ParseHouseNameListCsv(char* houseListCsv, bool ignoreModTypes)
{
	if (StringIsEmpty(houseListCsv))
	{
		return ResultWithValue<int>::BuildWithError("CSV passed to ParseHouseNameListCsv was null or empty");
	}

	auto houseNameListSize = 0u;
	auto houseNameList = ParseCsvString(houseListCsv, HOUSE_NAME_MAX_LENGTH, &houseNameListSize);

	if (houseNameList == NULL)
	{
		return ResultWithValue<int>::BuildWithError("Parsing house list CSV returned NULL");
	}

	auto houseListInitialised = false;
	auto houseList = 0;

	for (auto i = 0u; i < houseNameListSize; i++)
	{
		auto& houseResult = Parse<HousesType>(houseNameList[i], ignoreModTypes);

		if (houseResult.IsErrorResult())
		{
			auto& result = houseResult.ConvertType<int>();

			delete& houseResult;

			return result;
		}

		auto house = houseResult.GetValue();

		delete& houseResult;

		if (house == HOUSE_NONE)
		{
			// none overrides all entries in csv
			return ResultWithValue<int>::BuildWithValue(HOUSEF_NONE);
		}

		auto houseBit = 1 << house;

		if (!houseListInitialised)
		{
			houseList = houseBit;
			houseListInitialised = true;
		}
		else
		{
			houseList = houseList | houseBit;
		}
	}

	delete houseNameList;

	return ResultWithValue<int>::BuildWithValue(houseList);
}

ResultWithValue<int>& TiberianDawnTypeConverter::ParseHouseNameListCsv(const char* houseListCsv, bool ignoreModTypes)
{
	auto houseTypeCsvStr = strdup(houseListCsv);

	auto& result = ParseHouseNameListCsv(houseTypeCsvStr, ignoreModTypes);

	delete houseTypeCsvStr;

	return result;
}

StructType TiberianDawnTypeConverter::PrerequisiteToStructureType(long prerequisite)
{
	if (prereqToStructMap.find(prerequisite) != prereqToStructMap.end())
	{
		return prereqToStructMap[prerequisite];
	}

	ShowError("Unable to convert prerequiste to structure type, returning STRUCT_NONE");

	return STRUCT_NONE;
}

const char* TiberianDawnTypeConverter::PrerequisiteToString(long prerequisite)
{
	auto& result = NcoTypeConverter().ToString<StructType>(
		PrerequisiteToStructureType(prerequisite)
	);

	if (result.IsErrorResult())
	{
		ShowError("Failed to convert prerequisite to string, returning STRUCT_NONE: %s", result.GetError());

		delete &result;

		result = NcoTypeConverter().ToString(STRUCT_NONE);
	}

	auto prereqString = result.GetValue();

	delete& result;

	return prereqString;
}

ResultWithValue<long>& TiberianDawnTypeConverter::StructureTypeToPrerequisite(StructType structType, bool ignoreModTypes)
{
	if (structType == STRUCT_NONE)
	{
		return ResultWithValue<long>::BuildWithValue(STRUCTF_NONE);
	}

	auto structCount = ignoreModTypes ? STRUCT_COUNT : ReadBuildingCount();

	if (structType < STRUCT_FIRST || structType > structCount - 1)
	{
		return ResultWithValue<long>::BuildWithError("Structure type was not recognised: %d", structType);
	}

	return ResultWithValue<long>::BuildWithValue(1L << structType);
}
