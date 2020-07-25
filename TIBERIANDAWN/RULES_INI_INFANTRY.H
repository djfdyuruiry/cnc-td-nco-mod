#pragma once

// infantry rules
#define FEMALE_RULE "Female"
#define FEMALE_RULE_UPPER "FEMALE"

#define CRAWLING_RULE "Crawling"
#define CRAWLING_RULE_UPPER "CRAWLING"

#define CAN_CAPTURE_RULE "CanCapture"
#define CAN_CAPTURE_RULE_UPPER "CANCAPTURE"

#define FRAIDY_CAT_RULE "FraidyCat"
#define FRAIDY_CAT_RULE_UPPER "FRAIDYCAT"

#define CIVILIAN_RULE "Civilian"
#define CIVILIAN_RULE_UPPER "CIVILIAN"

#define AVOIDS_TIBERIUM_RULE "AvoidsTiberium"
#define AVOIDS_TIBERIUM_RULE_UPPER "AVOIDSTIBERIUM"

#define IMMUNE_TO_TIBERIUM_RULE "ImmuneToTiberium"
#define IMMUNE_TO_TIBERIUM_RULE_UPPER "IMMUNETOTIBERIUM"

#define HAS_C4_CHARGES_RULE "HasC4Charges"
#define HAS_C4_CHARGES_RULE_UPPER "HASC4CHARGES"

#define INFANTRY_RULES SHARED_RULES, \
    FEMALE_RULE, \
    CRAWLING_RULE, \
    CAN_CAPTURE_RULE, \
    FRAIDY_CAT_RULE, \
    CIVILIAN_RULE, \
    AVOIDS_TIBERIUM_RULE, \
    IMMUNE_TO_TIBERIUM_RULE, \
    HAS_C4_CHARGES_RULE

#define INFANTRY_RULE_COUNT 8u + SHARED_RULE_COUNT

// infantry rules
#define Read_Is_Female(section, defaultValue) Read_Bool_From_Rules_Ini(section, FEMALE_RULE, defaultValue)

#define Read_Is_Crawling(section, defaultValue) Read_Bool_From_Rules_Ini(section, CRAWLING_RULE, defaultValue)

#define Read_Can_Capture_Buildings(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_CAPTURE_RULE, defaultValue)

#define Read_Is_Fraidy_Cat(section, defaultValue) Read_Bool_From_Rules_Ini(section, FRAIDY_CAT_RULE, defaultValue)

#define Read_Is_Civilian(section, defaultValue) Read_Bool_From_Rules_Ini(section, CIVILIAN_RULE, defaultValue)

#define Read_Avoids_Tiberium(section, defaultValue) Read_Bool_From_Rules_Ini(section, AVOIDS_TIBERIUM_RULE, defaultValue)
