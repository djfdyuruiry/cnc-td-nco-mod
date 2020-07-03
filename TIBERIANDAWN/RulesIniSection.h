#ifndef RULES_INI_SECTION
#define RULES_INI_SECTION

#include "rules_ini.h"

typedef struct IniRule {
	const char* ruleName;
	void* ruleValue;
	const char* valueType;
} GameRules;

class RulesIniSection {
	private:
		static const unsigned int MAX_SECTION_RULES = 50;

		const char* sectionName;
		const char** ruleNames;
		const char** ruleTypes;
		unsigned int ruleCount;
		unsigned int size;

		IniRule** currentGameRules;

		void Update_Rule(IniRule* rule, const char* entry, void* value, unsigned int valueSize);
		void Store_Rule(const char* entry, void* value, const char* valueType, unsigned int valueSize);

	public:
		RulesIniSection(const char* sectionName, const char** ruleNames, const char** ruleTypes, unsigned int ruleCount);

		bool Init_Rules();

		const char* Get_Name();

		const char** Get_Rule_Names();

		IniRule* Get_Rule(const char* entry);

		char* Read_Rule(const char* entry, char* defaultValue);

		bool Update_Current_Rule_Value(const char* entry, char* value);

		char* Read_Rule(const char* entry, const char* defaultValue);

		bool Update_Current_Rule_Value(const char* entry, const char* value);

		int Read_Int_Rule(const char* entry, int defaultValue, int minInclusive, int maxInclusive);

		bool Update_Current_Int_Rule_Value(const char* entry, int value);

		bool Read_Boolean_Rule(const char* entry, bool defaultValue);

		bool Update_Current_Boolean_Rule_Value(const char* entry, bool value);
};

#endif
