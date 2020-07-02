#ifndef RULES_INI_BUILDING
#define RULES_INI_BUILDING

// building rules
#define BIBBED_RULE "Bibbed"
#define BIBBED_RULE_UPPER "BIBBED"

#define WALL_RULE "Wall"
#define WALL_RULE_UPPER "WALL"

#define FACTORY_RULE "Factory"
#define FACTORY_RULE_UPPER "FACTORY"

#define SIMPLE_DAMAGE_RULE "SimpleDamage"
#define SIMPLE_DAMAGE_RULE_UPPER "SIMPLEDAMAGE"

#define STURDY_RULE "Sturdy"
#define STURDY_RULE_UPPER "STURDY"

#define CAPTUREABLE_RULE "Captureable"
#define CAPTUREABLE_RULE_UPPER "CAPTUREABLE"

#define FIXED_SPEED_ANIMATION_RULE "FixedSpeedAnimation"
#define FIXED_SPEED_ANIMATION_RULE_UPPER "FIXEDSPEEDANIMATION"

#define UNSELLABLE_RULE "Unsellable"
#define UNSELLABLE_RULE_UPPER "UNSELLABLE"

#define FACTORY_TYPE_RULE "FactoryType"
#define FACTORY_TYPE_RULE_UPPER "FACTORYTYPE"

#define STORAGE_CAPACITY_RULE "StorageCapacity"
#define STORAGE_CAPACITY_RULE_UPPER "STORAGECAPACITY"

#define POWER_OUTPUT_RULE "PowerOutput"
#define POWER_OUTPUT_RULE_UPPER "POWEROUTPUT"

#define POWER_INPUT_RULE "PowerInput"
#define POWER_INPUT_RULE_UPPER "POWERINPUT"

#define BUILDING_RULES BIBBED_RULE, \
    WALL_RULE, \
    FACTORY_RULE, \
    SIMPLE_DAMAGE_RULE, \
    STURDY_RULE, \
    CAPTUREABLE_RULE, \
    FIXED_SPEED_ANIMATION_RULE, \
    UNSELLABLE_RULE, \
    FACTORY_TYPE_RULE, \
    STORAGE_CAPACITY_RULE, \
    POWER_OUTPUT_RULE, \
    POWER_INPUT_RULE, \
    SHARED_RULES

#define BUILDING_RULE_COUNT (unsigned) 13 + SHARED_RULE_COUNT

// building rules
#define Read_Is_Bibbed(section, defaultValue) Read_Bool_From_Rules_Ini(section, BIBBED_RULE, defaultValue)

#define Read_Is_Captureable(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAPTUREABLE_RULE, defaultValue)

#define Read_Storage_Capacity(section, defaultValue) (unsigned)Read_Int_From_Rules_Ini(section, STORAGE_CAPACITY_RULE, defaultValue, 0, INT_MAX)

#define Read_Power_Output(section, defaultValue) Read_Int_From_Rules_Ini(section, POWER_OUTPUT_RULE, defaultValue, 0, INT_MAX)

#define Read_Power_Input(section, defaultValue) Read_Int_From_Rules_Ini(section, POWER_INPUT_RULE, defaultValue, 0, INT_MAX)

#define Read_Factory_Type(section, defaultValue) (RTTIType)Read_Factory_Type_From_Rules_Ini(section, FACTORY_TYPE_RULE, defaultValue)

#define Read_Is_Wall(section, defaultValue) Read_Bool_From_Rules_Ini(section, WALL_RULE, defaultValue)

#define Read_Is_Factory(section, defaultValue) Read_Bool_From_Rules_Ini(section, FACTORY_RULE, defaultValue)

#define Read_Use_Simple_Damage(section, defaultValue) Read_Bool_From_Rules_Ini(section, SIMPLE_DAMAGE_RULE, defaultValue)

#define Read_Is_Fixed_Speed(section, defaultValue) Read_Bool_From_Rules_Ini(section, FIXED_SPEED_ANIMATION_RULE, defaultValue)

#define Read_Is_Unsellable(section, defaultValue) Read_Bool_From_Rules_Ini(section, UNSELLABLE_RULE, defaultValue)

#define Read_Is_Sturdy(section, defaultValue) Read_Bool_From_Rules_Ini(section, STURDY_RULE, defaultValue)

FactoryType Read_Factory_Type_From_Rules_Ini(
    const char* section,
    const char* entry,
    FactoryType defaultValue
);

#endif
