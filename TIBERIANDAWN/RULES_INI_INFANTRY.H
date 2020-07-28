#pragma once

#include "rules_ini.h"

// infantry rules
#define FEMALE_RULE "Female"

#define CRAWLING_RULE "Crawling"

#define CAN_CAPTURE_RULE "CanCapture"

#define FRAIDY_CAT_RULE "FraidyCat"

#define CIVILIAN_RULE "Civilian"

#define AVOIDS_TIBERIUM_RULE "AvoidsTiberium"

#define IMMUNE_TO_TIBERIUM_RULE "ImmuneToTiberium"

#define HAS_C4_CHARGES_RULE "HasC4Charges"

// infantry rules
#define Read_Is_Female(section, defaultValue) ReadRuleValue<bool>(section, FEMALE_RULE, defaultValue)

#define Read_Is_Crawling(section, defaultValue) ReadRuleValue<bool>(section, CRAWLING_RULE, defaultValue)

#define Read_Can_Capture_Buildings(section, defaultValue) ReadRuleValue<bool>(section, CAN_CAPTURE_RULE, defaultValue)

#define Read_Is_Fraidy_Cat(section, defaultValue) ReadRuleValue<bool>(section, FRAIDY_CAT_RULE, defaultValue)

#define Read_Is_Civilian(section, defaultValue) ReadRuleValue<bool>(section, CIVILIAN_RULE, defaultValue)

#define Read_Avoids_Tiberium(section, defaultValue) ReadRuleValue<bool>(section, AVOIDS_TIBERIUM_RULE, defaultValue)
