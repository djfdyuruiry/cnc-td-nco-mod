#pragma once

#include "RulesIni.h"

class RulesIniReader
{
private:
	RulesIni* rulesIni;

	bool ReadBoolRule(RulesIniRule* rule)
	{
		auto defaultValue = rule->GetDefaultValueOr(false);

		rule->SetDefaultValue(Convert_Boolean_To_String(defaultValue));

		auto ruleValue = rulesIni->ReadStringRule(rule);

		rule->SetDefaultValue(defaultValue);

		Convert_String_To_Upper_Case(ruleValue);

		auto boolValue = Strings_Are_Equal(ruleValue, TRUE_STRING);

		return boolValue;
	}

public:
	RulesIniReader(RulesIni* rulesIni)
	{
		this->rulesIni = rulesIni;
	}

	// TODO: Add optional equivalent that uses the `Optional` class
	template<class T> T ReadRuleValue(RulesIniRule* rule)
	{
		if (!rulesIni->HasSectionForRule(rule))
		{
			(*rulesIni) << rule;
		}

		auto section = rulesIni->GetSectionForRule(rule);

		if (!section->HasRule(rule))
		{
			(*section) << rule;
		}

		if (rule->HasValue())
		{
			return rule->GetValue();
		}

		auto ruleType = rule->GetType();

		if (ruleType == INT_RULE)
		{
			rule->SetValue(
				rulesIni->ReadIntRule(rule)
			);
		}
		else if (ruleType == UNSIGNED_INT_RULE)
		{
			// TODO: port rule function to this class
		}
		else if (ruleType == DOUBLE_RULE)
		{
			// TODO: port rule function to this class
		}
		else if (ruleType == BOOL_RULE)
		{
			rule->SetValue(
				ReadBoolRule(rule);
			)
		}
		else if (ruleType == STRING_RULE)
		{
			rule->SetValue(
				rulesIni->ReadStringRule(rule)
			);
		}
		else
		{
			rule->SetValue(
				rule->GetDefaultValue()
			);
		}

		return rule->GetValue();
	}
};
