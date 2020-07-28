#pragma once

#include "rules_ini.h"

// aircraft rules
#define CANT_HOVER_RULE "CantHover"

#define CAN_LAND_RULE "CanLand"

#define HAS_ROTOR_RULE "HasRotor"

// aircraft rules
#define Read_Cant_Hover(section, defaultValue) ReadRuleValue<bool>(section, CANT_HOVER_RULE, defaultValue)

#define Read_Has_Rotor(section, defaultValue) ReadRuleValue<bool>(section, HAS_ROTOR_RULE, defaultValue)

#define Read_Can_Land(section, defaultValue) ReadRuleValue<bool>(section, CAN_LAND_RULE, defaultValue)
