#pragma once

#include <map>
#include <vector>

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

	bool initialised;
	std::map<T, U*> modTypeInstances;
	std::vector<char*>& modTypes;

	NcoGameMod(
		IRulesRuntime& runtime,
		const char* modTypeName,
		RuleName modTypesRuleName,
		RuleName modTypeCountRuleName,
		const RulesIniRuleKey& typeCountRuleKey,
		T totalTypeCount
	)
		: runtime(runtime),
		typeName(modTypeName),
		typeKey(HashUtils::HashString(modTypeName)),
		modTypesRule(modTypesRuleName),
		modTypeCountRule(modTypeCountRuleName),
		typeCountRule(typeCountRuleKey),
		originalTypeCount(totalTypeCount),
		initialised(false),
		modTypes(*(new std::vector<char*>))
	{
	}

	virtual void ReadRulesAndAddType(U* newType) = 0;

	virtual T ParseType(SectionName typeString, bool* parseError) = 0;

	bool SetupNewType(SectionName typeString, T type, SectionName baseTypeString)
	{
		if (String_Is_Empty(typeString))
		{
			Show_Error("Blank mod %s type in rules file", typeName);

			return false;
		}

		Log_Info("Setting up new mod %s type: %s", typeName, typeString);

		bool parseError = false;
		auto baseType = ParseType(baseTypeString, &parseError);

		if (parseError)
		{
			return false;
		}

		Log_Info("Mod %s type base: %s", typeName, baseTypeString);

		auto baseTypeInstance = U::By_Type(baseType);

		auto typeClone = (U*)malloc(sizeof(U));

		memcpy(typeClone, baseTypeInstance, sizeof(U));

		typeClone->IsModType = true;

		strcpy(typeClone->ModBaseIniName, baseTypeString);
		strcpy(typeClone->IniName, typeString);

		typeClone->Type = type;

		modTypeInstances[typeClone->Type] = typeClone;

		ReadRulesAndAddType(typeClone);

		Log_Info("Mod %s type setup successful", typeName);

		return true;
	}

	virtual void AddRulesSection(SectionName typeString) = 0;

public:
	void ReadTypes()
	{
		if (modTypes.size() > 0)
		{
			Log_Debug("Attempt was made to initialise %s mod types more than once, ignoring", typeName);
			return;
		}

		Log_Info("Reading %s Mod Types", typeName);

		auto newTypesCsv = runtime.ReadRuleValue<char*>(MOD_RULES_SECTION_NAME, modTypesRule);

		if (String_Is_Empty(newTypesCsv)) {
			Log_Debug("No %s mod types found", typeName);
			return;
		}

		auto newTypesCount = 0u;
		auto modTypeCsvEntries = Parse_Csv_String(newTypesCsv, RulesIniRule::RULES_INI_ID_SIZE, &newTypesCount);

		if (newTypesCount < 1u || modTypeCsvEntries == NULL) {
			Log_Debug("No %s mod types found", typeName);
			return;
		}

		for (auto i = 0u; i < newTypesCount; i++)
		{
			Convert_String_To_Upper_Case(modTypeCsvEntries[i]);

			modTypes.push_back(modTypeCsvEntries[i]);
		}

		auto totalModTypeCount = originalTypeCount + newTypesCount;

		runtime.GetBaseRulesReader()
			.GetRule(MOD_RULES_SECTION_NAME, modTypeCountRule)
			.SetValue(newTypesCount);

		runtime.GetBaseRulesReader()
			.GetRule(typeCountRule)
			.SetValue(totalModTypeCount);

		Log_Info("Mod %s types read: %s", typeName, newTypesCsv);
		Log_Info("Total game %s types: %u", typeName, totalModTypeCount);
	}

	void InitialiseTypes()
	{
		if (initialised)
		{
			return;
		}

		initialised = true;

		Log_Info("Initialising %s Mod Types", typeName);

		T type = originalTypeCount;

		for (auto typeString : modTypes)
		{
			bool baseTypeFound = false;

			AddRulesSection(typeString);

			auto baseTypeString = runtime.ReadRuleValue<char*>(typeString, BASE_TYPE_RULE);

			if (String_Is_Empty(baseTypeString))
			{
				Show_Error("Mod %s type has no %s rule provided: %s", typeName, typeString, BASE_TYPE_RULE);

				runtime.GetRules().MarkAsInvalid();

				continue;
			}

			if (!SetupNewType(typeString, type, baseTypeString))
			{
				runtime.GetRules().MarkAsInvalid();
			}

			type = (T)((char)type + 1);
		}

		Log_Info("%s Mod Types Initialised", typeName);
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

			if (Strings_Are_Equal(iniName, typeIniName))
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
