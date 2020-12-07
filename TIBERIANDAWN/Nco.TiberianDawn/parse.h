#pragma once

#include <ResultWithValue.h>

#include "../DEFINES.h"

// generic parsers
ResultWithValue<int>& ParseHouseNameListCsv(char* houseListCsv, bool ignoreModTypes = false);

ResultWithValue<int>& ParseHouseNameListCsv(const char* houseListCsv, bool ignoreModTypes = false);

StructType PrerequisiteToStructureType(long prerequisite);

const char * PrerequisiteToString(long prerequisite);

ResultWithValue<long>& StructureTypeToPrerequisite(StructType structType, bool ignoreModTypes = false);
