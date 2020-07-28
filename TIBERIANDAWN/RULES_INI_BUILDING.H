#pragma once

#include "rules_ini.h"

// building rules
#define BIBBED_RULE "Bibbed"

#define WALL_RULE "Wall"

#define FACTORY_RULE "Factory"

#define SIMPLE_DAMAGE_RULE "SimpleDamage"

#define STURDY_RULE "Sturdy"

#define CAPTUREABLE_RULE "Captureable"

#define FIXED_SPEED_ANIMATION_RULE "FixedSpeedAnimation"

#define UNSELLABLE_RULE "Unsellable"

#define FACTORY_TYPE_RULE "FactoryType"

#define STORAGE_CAPACITY_RULE "StorageCapacity"

#define POWER_OUTPUT_RULE "PowerOutput"

#define POWER_INPUT_RULE "PowerInput"

// building rules
#define Read_Is_Bibbed(section, defaultValue) ReadRuleValue<bool>(section, BIBBED_RULE, defaultValue)

#define Read_Is_Captureable(section, defaultValue) ReadRuleValue<bool>(section, CAPTUREABLE_RULE, defaultValue)

#define Read_Storage_Capacity(section, defaultValue) ReadRuleValue<unsigned int>(section, STORAGE_CAPACITY_RULE, defaultValue)

#define Read_Power_Output(section, defaultValue) ReadRuleValue<unsigned int>(section, POWER_OUTPUT_RULE, defaultValue)

#define Read_Power_Input(section, defaultValue) ReadRuleValue<unsigned int>(section, POWER_INPUT_RULE, defaultValue)

#define Read_Factory_Type(section, defaultValue) (RTTIType)ReadRuleValue<FactoryType>(section, FACTORY_TYPE_RULE, defaultValue)

#define Read_Is_Wall(section, defaultValue) ReadRuleValue<bool>(section, WALL_RULE, defaultValue)

#define Read_Is_Factory(section, defaultValue) ReadRuleValue<bool>(section, FACTORY_RULE, defaultValue)

#define Read_Use_Simple_Damage(section, defaultValue) ReadRuleValue<bool>(section, SIMPLE_DAMAGE_RULE, defaultValue)

#define Read_Is_Fixed_Speed(section, defaultValue) ReadRuleValue<bool>(section, FIXED_SPEED_ANIMATION_RULE, defaultValue)

#define Read_Is_Unsellable(section, defaultValue) ReadRuleValue<bool>(section, UNSELLABLE_RULE, defaultValue)

#define Read_Is_Sturdy(section, defaultValue) ReadRuleValue<bool>(section, STURDY_RULE, defaultValue)
