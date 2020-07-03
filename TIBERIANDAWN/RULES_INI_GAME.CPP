#include "function.h"

static RulesIniSection* GAME_RULES_SECTION;

bool Init_Game_Rules()
{
	GAME_RULES_SECTION = new RulesIniSection(
		GAME_RULES_SECTION_NAME,
		new const char* [GAME_RULE_COUNT] { GAME_RULES },
		new const char* [GAME_RULE_COUNT] { GAME_RULE_TYPES },
		GAME_RULE_COUNT
	);

	return GAME_RULES_SECTION->Init_Rules();
}

RulesIniSection* Get_Game_Rules()
{
	Ensure_Rules_Ini_Is_Loaded();

	return GAME_RULES_SECTION;
}
