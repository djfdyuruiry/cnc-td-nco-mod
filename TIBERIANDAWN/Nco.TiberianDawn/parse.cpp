#include <ResultWithValue.h>
#include <strings.h>
#include <utils.h>

#include "../DEFINES.H"

#include "tiberian_dawn_rules.h"
#include "TiberianDawnNcoRuntime.h"

static const auto HOUSE_NAME_MAX_LENGTH = 8;

ResultWithValue<int>& ParseHouseNameListCsv(char* houseListCsv, bool ignoreModTypes)
{
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
        auto& houseResult = NcoTypeConverter().Parse<HousesType>(houseNameList[i], ignoreModTypes);

        if (houseResult.IsErrorResult())
        {
            auto& result = houseResult.ConvertType<int>();

            delete &houseResult;

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

ResultWithValue<int>& ParseHouseNameListCsv(const char* houseListCsv, bool ignoreModTypes)
{
    if (StringIsEmpty(houseListCsv))
    {
        return ResultWithValue<int>::BuildWithError("CSV passed to ParseHouseNameListCsv was null or empty");
    }

    auto houseTypeCsvStr = strdup(houseListCsv);

    auto& result = ParseHouseNameListCsv(houseTypeCsvStr, ignoreModTypes);

    delete houseTypeCsvStr;

    return result;
}

StructType PrerequisiteToStructureType(long prerequisite)
{
    if (prerequisite == STRUCTF_NONE || prerequisite < 0l)
    {
        return STRUCT_NONE;
    }
    else if (prerequisite == STRUCTF_ADVANCED_POWER)
    {
        return STRUCT_ADVANCED_POWER;
    }
    else if (prerequisite == STRUCTF_REPAIR)
    {
        return STRUCT_REPAIR;
    }
    else if (prerequisite == STRUCTF_EYE)
    {
        return STRUCT_EYE;
    }
    else if (prerequisite == STRUCTF_TEMPLE)
    {
        return STRUCT_TEMPLE;
    }
    else if (prerequisite == STRUCTF_HAND)
    {
        return STRUCT_HAND;
    }
    else if (prerequisite == STRUCTF_BIO_LAB)
    {
        return STRUCT_BIO_LAB;
    }
    else if (prerequisite == STRUCTF_OBELISK)
    {
        return STRUCT_OBELISK;
    }
    else if (prerequisite == STRUCTF_ATOWER)
    {
        return STRUCT_ATOWER;
    }
    else if (prerequisite == STRUCTF_WEAP)
    {
        return STRUCT_WEAP;
    }
    else if (prerequisite == STRUCTF_GTOWER)
    {
        return STRUCT_GTOWER;
    }
    else if (prerequisite == STRUCTF_RADAR)
    {
        return STRUCT_RADAR;
    }
    else if (prerequisite == STRUCTF_TURRET)
    {
        return STRUCT_TURRET;
    }
    else if (prerequisite == STRUCTF_CONST)
    {
        return STRUCT_CONST;
    }
    else if (prerequisite == STRUCTF_REFINERY)
    {
        return STRUCT_REFINERY;
    }
    else if (prerequisite == STRUCTF_STORAGE)
    {
        return STRUCT_STORAGE;
    }
    else if (prerequisite == STRUCTF_HELIPAD)
    {
        return STRUCT_HELIPAD;
    }
    else if (prerequisite == STRUCTF_SAM)
    {
        return STRUCT_SAM;
    }
    else if (prerequisite == STRUCTF_AIRSTRIP)
    {
        return STRUCT_AIRSTRIP;
    }
    else if (prerequisite == STRUCTF_POWER)
    {
        return STRUCT_POWER;
    }
    else if (prerequisite == STRUCTF_HOSPITAL)
    {
        return STRUCT_HOSPITAL;
    }
    else if (prerequisite == STRUCTF_BARRACKS)
    {
        return STRUCT_BARRACKS;
    }
    else if (prerequisite == STRUCTF_TANKER)
    {
        return STRUCT_TANKER;
    }
    else if (prerequisite == STRUCTF_MISSION)
    {
        return STRUCT_MISSION;
    }

    ShowError("Unable to convert prerequiste to structure type, returning STRUCT_NONE");

    return STRUCT_NONE;
}

const char* PrerequisiteToString(long prerequisite)
{
    auto& result = NcoTypeConverter().ToString<StructType>(
        PrerequisiteToStructureType(prerequisite)
    );

    if (result.IsErrorResult())
    {
        ShowError("Failed to convert prerequisite to string, returning STRUCT_NONE: %s", result.GetError());

        delete &result;

        NcoTypeConverter().ToString(STRUCT_NONE);
    }

    auto prereqString = result.GetValue();

    delete &result;

    return prereqString;
}

ResultWithValue<long>& StructureTypeToPrerequisite(StructType structType, bool ignoreModTypes)
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
