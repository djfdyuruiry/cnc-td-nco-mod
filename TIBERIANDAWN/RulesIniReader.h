#pragma once

#include "RulesIni.h"

class RulesIniReader
{
private:
	RulesIni* rulesIni;

public:
	RulesIniReader(RulesIni* rulesIni)
	{
		this->rulesIni = rulesIni;
	}

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
			// TODO: port rule function to this class
		}
		else if (ruleType == STRING_RULE)
		{
			rule->SetValue(
				rulesIni->ReadStringRule(rule, defaultValue)
			);
		}
		else
		{
			rule->SetValue(
				rule->GetDefaultValueOr(defaultValue)
			);
		}

		return rule->GetValue();
	}
};
