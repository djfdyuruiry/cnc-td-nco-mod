#pragma once

#include <map>
#include <vector>

#include <ResultWithValue.h>
#include <TwoWayStringMap.h>

#include "IGameMod.h"
#include "IRulesRuntime.h"
#include "rules_ini_mods.h"
#include "RulesIniRule.h"
#include "RulesIniRuleKey.h"

template<class T, class U> class NcoGameMod : public IGameMod
{
protected:
	IRulesRuntime& runtime;
	const char* typeName;
	StringHash typeKey;
	RuleName modTypesRule;
	RuleName modTypeCountRule;
	const RulesIniRuleKey& typeCountRule;

	T originalTypeCount;

	TwoWayStringMap<T>& typeModMap;

	bool initialised;
	std::map<T, U*>& modTypeInstances;
	std::vector<char*>& modTypes;

	NcoGameMod(
		IRulesRuntime& runtime,
		const char* modTypeName,
		RuleName modTypesRuleName,
		RuleName modTypeCountRuleName,
		const RulesIniRuleKey& typeCountRuleKey,
		T totalTypeCount,
		TwoWayStringMap<T>& typeModMap
	)
		: runtime(runtime),
		typeName(modTypeName),
		typeKey(HashUtils::HashString(modTypeName)),
		modTypesRule(modTypesRuleName),
		modTypeCountRule(modTypeCountRuleName),
		typeCountRule(typeCountRuleKey),
		originalTypeCount(totalTypeCount),
		typeModMap(typeModMap),
		initialised(false),
		modTypeInstances(*(new std::map<T, U*>())),
		modTypes(*(new std::vector<char*>))
	{
	}

	virtual void ReadRulesAndAddType(U* newType) = 0;

	virtual U* GetTypeInstance(T type) = 0;

	virtual U* CloneType(const char* baseTypeString, const char* typeString, T baseType, T type)
	{
		auto baseTypeInstance = GetTypeInstance(baseType);

		auto typeClone = (U*)malloc(sizeof(U));

		memcpy(typeClone, baseTypeInstance, sizeof(U));

		return typeClone;
	}

	virtual ResultWithValue<T>& ParseType(SectionName typeString) = 0;

	bool SetupNewType(SectionName typeString, T type, SectionName baseTypeString)
	{
		if (StringIsEmpty(typeString))
		{
			ShowError("Blank mod %s type in rules file", typeName);

			return false;
		}

		LogInfo("Setting up new mod %s type: %s", typeName, typeString);

		bool parseError = false;
		auto& baseTypeResult = ParseType(baseTypeString);

		if (baseTypeResult.IsErrorResult())
		{
			delete &baseTypeResult;

			return false;
		}

		auto baseType = baseTypeResult.GetValue();

		delete &baseTypeResult;

		LogInfo("Mod %s type base: %s", typeName, baseTypeString);

		auto typeClone = CloneType(baseTypeString, typeString, baseType, type);

		modTypeInstances[type] = typeClone;

		typeModMap.AddKey(typeString, type);

		ReadRulesAndAddType(typeClone);

		LogInfo("Mod %s type setup successful", typeName);

		return true;
	}

	virtual void AddRulesSection(SectionName typeString) = 0;

public:
	~NcoGameMod()
	{
		for (auto [_, modTypeInstance] : modTypeInstances)
		{
			delete modTypeInstance;
		}

		delete &modTypeInstances;

		for (auto modType : modTypes)
		{
			delete modType;
		}

		delete &modTypes;
	}

	void ReadTypes()
	{
		if (modTypes.size() > 0)
		{
			LogDebug("Attempt was made to initialise %s mod types more than once, ignoring", typeName);
			return;
		}

		LogInfo("Reading %s Mod Types", typeName);

		auto newTypesCsv = runtime.ReadRuleValue<char*>(MOD_RULES_SECTION_NAME, modTypesRule);

		if (StringIsEmpty(newTypesCsv)) {
			LogDebug("No %s mod types found", typeName);
			return;
		}

		auto newTypesCount = 0u;
		auto modTypeCsvEntries = ParseCsvString(newTypesCsv, RulesIniRule::RULES_INI_ID_SIZE, &newTypesCount);

		if (newTypesCount < 1u || modTypeCsvEntries == NULL) {
			LogDebug("No %s mod types found", typeName);
			return;
		}

		for (auto i = 0u; i < newTypesCount; i++)
		{
			ConvertStringToUpperCase(modTypeCsvEntries[i]);

			modTypes.push_back(modTypeCsvEntries[i]);
		}

		auto totalModTypeCount = originalTypeCount + newTypesCount;

		runtime.GetBaseRulesReader()
			.GetRule(MOD_RULES_SECTION_NAME, modTypeCountRule)
			.SetValue(newTypesCount);

		runtime.GetBaseRulesReader()
			.GetRule(typeCountRule)
			.SetValue(totalModTypeCount);

		LogInfo("Mod %s types read: %s", typeName, newTypesCsv);
		LogInfo("Total game %s types: %u", typeName, totalModTypeCount);
	}

	bool InitialiseTypes()
	{
		if (initialised)
		{
			return initialised;
		}

		LogInfo("Initialising %s Mod Types", typeName);

		initialised = true;

		T type = originalTypeCount;

		for (auto typeString : modTypes)
		{
			bool baseTypeFound = false;

			AddRulesSection(typeString);

			auto baseTypeString = runtime.ReadRuleValue<char*>(typeString, BASE_TYPE_RULE);

			if (StringIsEmpty(baseTypeString))
			{
				ShowError("Mod %s type has no %s rule provided: %s", typeName, typeString, BASE_TYPE_RULE);

				runtime.GetRules().MarkAsInvalid();

				continue;
			}

			if (!SetupNewType(typeString, type, baseTypeString))
			{
				initialised = false;
			}

			type = (T)((char)type + 1);
		}

		if (initialised)
		{
			LogInfo("%s Mod Types Initialised", typeName);
		}

		return initialised;
	}

	StringHash GetTypeKey()
	{
		return typeKey;
	}

	const char* GetTypeName()
	{
		return typeName;
	}

	SectionName GetTypeIniName(T type)
	{
		return modTypeInstances[type]->IniName;
	}

	T GetTypeByIniName(SectionName iniName, bool* matchFound)
	{
		for (auto typeKvp : modTypeInstances)
		{
			auto typeIniName = typeKvp.second->IniName;

			if (StringsAreEqual(iniName, typeIniName))
			{
				if (matchFound != NULL)
				{
					*matchFound = true;
				}

				return typeKvp.first;
			}
		}

		if (matchFound != NULL)
		{
			*matchFound = false;
		}

		return (T)-1;
	}

};
