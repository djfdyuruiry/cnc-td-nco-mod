#pragma once

#include <type_traits>

#include <ResultWithValue.h>
#include <strings.h>
#include <TwoWayStringMap.h>
#include <TypeUtils.h>

#include "../DEFINES.H"

#include "tiberian_dawn_rules.h"

class TiberianDawnTypeConverter
{
private:
	static const auto HOUSE_NAME_MAX_LENGTH = 8;

	TwoWayStringMap<DiffType>& difficultyMap;
	TwoWayStringMap<HousesType>& housesMap;
	TwoWayStringMap<ArmorType>& armorMap;
	TwoWayStringMap<SpeedType>& unitSpeedMap;
	TwoWayStringMap<FactoryType>& factoryTypeMap;
	TwoWayStringMap<WarheadType>& warheadMap;
	TwoWayStringMap<WarheadType>& warheadModsMap;
	TwoWayStringMap<BulletType>& bulletMap;
	TwoWayStringMap<BulletType>& bulletModsMap;
	TwoWayStringMap<WeaponType>& weaponMap;
	TwoWayStringMap<WeaponType>& weaponModsMap;
	TwoWayStringMap<AircraftType>& aircraftMap;
	TwoWayStringMap<AircraftType>& aircraftModsMap;
	TwoWayStringMap<StructType>& structMap;
	TwoWayStringMap<StructType>& structModsMap;
	TwoWayStringMap<InfantryType>& infantryMap;
	TwoWayStringMap<InfantryType>& infantryModsMap;
	TwoWayStringMap<UnitType>& unitMap;
	TwoWayStringMap<UnitType>& unitModsMap;
	std::map<long, StructType>& prereqToStructMap;

	TiberianDawnTypeConverter() :
		difficultyMap(
			TwoWayStringMap<DiffType>::Build(
				"Difficulty",
				{
					{ DIFF_EASY, "EASY" },
					{ DIFF_NORMAL, "NORMAL" },
					{ DIFF_HARD, "HARD" }
				}
			)
		),
		housesMap(
			TwoWayStringMap<HousesType>::Build(
				"House",
				{
					{ HOUSE_NONE, "NONE"},
					{ HOUSE_GOOD, "GOODGUY" },
					{ HOUSE_BAD, "BADGUY"},
					{ HOUSE_NEUTRAL, "NEUTRAL" },
					{ HOUSE_JP, "JP" },
					{ HOUSE_MULTI1, "MULTI1" },
					{ HOUSE_MULTI2, "MULTI2"  },
					{ HOUSE_MULTI3, "MULTI3"  },
					{ HOUSE_MULTI4, "MULTI4"  },
					{ HOUSE_MULTI5, "MULTI5"  },
					{ HOUSE_MULTI6, "MULTI6"  }
				}
			)
		),
		armorMap(
			TwoWayStringMap<ArmorType>::Build(
				"Armor",
				{
					{ ARMOR_NONE, "NONE" },
					{ ARMOR_WOOD, "WOOD" },
					{ ARMOR_ALUMINUM, "ALUMINUM" },
					{ ARMOR_STEEL, "STEEL" },
					{ ARMOR_CONCRETE, "CONCRETE" }
				}
			)
		),
		unitSpeedMap(
			TwoWayStringMap<SpeedType>::Build(
				"Unit Speed",
				{
					{ SPEED_NONE, "NONE" },
					{ SPEED_FOOT, "FOOT" },
					{ SPEED_TRACK, "TRACK" },
					{ SPEED_HARVESTER, "HARVESTER" },
					{ SPEED_WHEEL, "WHEEL" },
					{ SPEED_WINGED, "WINGED" },
					{ SPEED_HOVER, "HOVER" },
					{ SPEED_FLOAT, "FLOAT" }
				}
			)
		),
		factoryTypeMap(
			TwoWayStringMap<FactoryType>::Build(
				"Factory Type",
				{
					{ FACTORY_TYPE_NONE, "NONE" },
					{ FACTORY_TYPE_INFANTRY, "INFANTRY" },
					{ FACTORY_TYPE_UNIT, "UNIT" },
					{ FACTORY_TYPE_AIRCRAFT, "AIRCRAFT" },
					{ FACTORY_TYPE_BUILDING, "BUILDING" }
				}
			)
		),
		warheadMap(
			TwoWayStringMap<WarheadType>::Build(
				"Warhead",
				{
					{ WARHEAD_SA, "SA" },
					{ WARHEAD_HE, "HE" },
					{ WARHEAD_AP, "AP" },
					{ WARHEAD_FIRE, "FIRE" },
					{ WARHEAD_LASER, "LASER" },
					{ WARHEAD_PB, "PB" },
					{ WARHEAD_FIST, "FIST" },
					{ WARHEAD_FOOT, "FOOT" },
					{ WARHEAD_HOLLOW_POINT, "HOLLOW_POINT" },
					{ WARHEAD_SPORE, "SPORE" },
					{ WARHEAD_HEADBUTT, "HEADBUTT" },
					{ WARHEAD_FEEDME, "FEEDME" }
				}
			)
		),
		warheadModsMap(TwoWayStringMap<WarheadType>::Build("Warhead")),
		bulletMap(
			TwoWayStringMap<BulletType>::Build(
				"Bullet",
				{
					{ BULLET_NONE, "NONE" },
					{ BULLET_SNIPER, "SNIPER" },
					{ BULLET_BULLET, "BULLET" },
					{ BULLET_SPREADFIRE, "SPREADFIRE" },
					{ BULLET_APDS, "APDS" },
					{ BULLET_HE, "ARTILLERY_SHELL" },
					{ BULLET_SSM, "SS_MISSILE" },
					{ BULLET_SSM2, "MLRS_ROCKET" },
					{ BULLET_SAM, "SAM_MISSILE" },
					{ BULLET_TOW, "TOW_MISSILE" },
					{ BULLET_FLAME, "FLAME" },
					{ BULLET_CHEMSPRAY, "CHEMICALS" },
					{ BULLET_NAPALM, "NAPALM_BOMB" },
					{ BULLET_GRENADE, "GRENADE_BOMB" },
					{ BULLET_LASER, "LASER_BEAM" },
					{ BULLET_NUKE_UP, "NUKE_UP" },
					{ BULLET_NUKE_DOWN, "NUKE_DOWN" },
					{ BULLET_HONEST_JOHN, "HONEST_JOHN_MISSILE" },
					{ BULLET_HEADBUTT, "GORE" },
					{ BULLET_TREXBITE, "CHEW" }
				}
			)
		),
		bulletModsMap(TwoWayStringMap<BulletType>::Build("Bullet")),
		weaponMap(
			TwoWayStringMap<WeaponType>::Build(
				"Weapon",
				{
					{ WEAPON_NONE, "NONE" },
					{ WEAPON_RIFLE, "RIFLE" },
					{ WEAPON_CHAIN_GUN, "CHAIN_GUN" },
					{ WEAPON_PISTOL, "PISTOL" },
					{ WEAPON_M16, "M16" },
					{ WEAPON_DRAGON, "DRAGON" },
					{ WEAPON_FLAMETHROWER, "FLAMETHROWER" },
					{ WEAPON_FLAME_TONGUE, "FLAME_TONGUE" },
					{ WEAPON_CHEMSPRAY, "CHEMSPRAY" },
					{ WEAPON_GRENADE, "GRENADE" },
					{ WEAPON_75MM, "75MM" },
					{ WEAPON_105MM, "105MM" },
					{ WEAPON_120MM, "120MM" },
					{ WEAPON_TURRET_GUN, "TURRET_GUN" },
					{ WEAPON_MAMMOTH_TUSK, "MAMMOTH_TUSK" },
					{ WEAPON_MLRS, "ROCKET_LAUNCHER" },
					{ WEAPON_155MM, "155MM" },
					{ WEAPON_M60MG, "M60MG" },
					{ WEAPON_TOMAHAWK, "TOMAHAWK" },
					{ WEAPON_TOW_TWO, "TOWTWO" },
					{ WEAPON_NAPALM, "NAPALM" },
					{ WEAPON_OBELISK_LASER, "OBELISK_LASER" },
					{ WEAPON_NIKE, "NIKE" },
					{ WEAPON_HONEST_JOHN, "HONEST_JOHN" },
					{ WEAPON_STEG, "DINO_RAM" },
					{ WEAPON_TREX, "DINO_BITE" }
				}
			)
		),
		weaponModsMap(TwoWayStringMap<WeaponType>::Build("Weapon")),
		aircraftMap(
			TwoWayStringMap<AircraftType>::Build(
				"Aircraft",
				{
					{ AIRCRAFT_NONE, "NONE" },
					{ AIRCRAFT_A10, "A10" },
					{ AIRCRAFT_CARGO, "C17" },
					{ AIRCRAFT_HELICOPTER, "HELI" },
					{ AIRCRAFT_ORCA, "ORCA" },
					{ AIRCRAFT_TRANSPORT, "TRAN" }
				}
			)
		),
		aircraftModsMap(TwoWayStringMap<AircraftType>::Build("Aircraft")),
		structMap(
			TwoWayStringMap<StructType>::Build(
				"Building",
				{
					{ STRUCT_NONE, "NONE" },
					{ STRUCT_WEAP, "WEAP" },
					{ STRUCT_GTOWER, "GTWR" },
					{ STRUCT_ATOWER, "ATWR" },
					{ STRUCT_OBELISK, "OBLI" },
					{ STRUCT_RADAR, "HQ" },
					{ STRUCT_TURRET, "GUN" },
					{ STRUCT_CONST, "FACT" },
					{ STRUCT_REFINERY, "PROC" },
					{ STRUCT_STORAGE, "SILO" },
					{ STRUCT_HELIPAD, "HPAD" },
					{ STRUCT_SAM, "SAM" },
					{ STRUCT_AIRSTRIP, "AFLD" },
					{ STRUCT_POWER, "NUKE" },
					{ STRUCT_ADVANCED_POWER, "NUK2" },
					{ STRUCT_HOSPITAL, "HOSP" },
					{ STRUCT_BARRACKS, "PYLE" },
					{ STRUCT_TANKER, "ARCO" },
					{ STRUCT_REPAIR, "FIX" },
					{ STRUCT_BIO_LAB, "BIO" },
					{ STRUCT_HAND, "HAND" },
					{ STRUCT_TEMPLE, "TMPL" },
					{ STRUCT_EYE, "EYE" },
					{ STRUCT_MISSION, "MISS" },
					{ STRUCT_V01, "V01" },
					{ STRUCT_V02, "V02" },
					{ STRUCT_V03, "V03" },
					{ STRUCT_V04, "V04" },
					{ STRUCT_V05, "V05" },
					{ STRUCT_V06, "V06" },
					{ STRUCT_V07, "V07" },
					{ STRUCT_V08, "V08" },
					{ STRUCT_V09, "V09" },
					{ STRUCT_V10, "V10" },
					{ STRUCT_V11, "V11" },
					{ STRUCT_V12, "V12" },
					{ STRUCT_V13, "V13" },
					{ STRUCT_V14, "V14" },
					{ STRUCT_V15, "V15" },
					{ STRUCT_V16, "V16" },
					{ STRUCT_V17, "V17" },
					{ STRUCT_V18, "V18" },
					{ STRUCT_PUMP, "V19" },
					{ STRUCT_V20, "V20" },
					{ STRUCT_V21, "V21" },
					{ STRUCT_V22, "V22" },
					{ STRUCT_V23, "V23" },
					{ STRUCT_V24, "V24" },
					{ STRUCT_V25, "V25" },
					{ STRUCT_V26, "V26" },
					{ STRUCT_V27, "V27" },
					{ STRUCT_V28, "V28" },
					{ STRUCT_V29, "V29" },
					{ STRUCT_V30, "V30" },
					{ STRUCT_V31, "V31" },
					{ STRUCT_V32, "V32" },
					{ STRUCT_V33, "V33" },
					{ STRUCT_V34, "V34" },
					{ STRUCT_V35, "V35" },
					{ STRUCT_V36, "V36" },
					{ STRUCT_V37, "V37" },
					{ STRUCT_SANDBAG_WALL, "SBAG" },
					{ STRUCT_CYCLONE_WALL, "CYCL" },
					{ STRUCT_BRICK_WALL, "BRIK" },
					{ STRUCT_BARBWIRE_WALL, "BARB" },
					{ STRUCT_WOOD_WALL, "WOOD" }
				}
			)
		),
		structModsMap(TwoWayStringMap<StructType>::Build("Building")),
		infantryMap(
			TwoWayStringMap<InfantryType>::Build(
				"Infantry",
				{
					{ INFANTRY_NONE, "NONE" },
					{ INFANTRY_E1, "E1" },
					{ INFANTRY_E2, "E2" },
					{ INFANTRY_E3, "E3" },
					{ INFANTRY_E4, "E4" },
					{ INFANTRY_E5, "E5" },
					{ INFANTRY_E7, "E6" },
					{ INFANTRY_RAMBO, "RMBO" },
					{ INFANTRY_C1, "C1" },
					{ INFANTRY_C2, "C2" },
					{ INFANTRY_C3, "C3" },
					{ INFANTRY_C4, "C4" },
					{ INFANTRY_C5, "C5" },
					{ INFANTRY_C6, "C6" },
					{ INFANTRY_C7, "C7" },
					{ INFANTRY_C8, "C8" },
					{ INFANTRY_C9, "C9" },
					{ INFANTRY_C10, "NIKOOMBA" },
					{ INFANTRY_MOEBIUS, "MOEBIUS" },
					{ INFANTRY_DELPHI, "DELPHI" },
					{ INFANTRY_CHAN, "CHAN" }
				}
			)
		),
		infantryModsMap(TwoWayStringMap<InfantryType>::Build("Infantry")),
		unitMap(
			TwoWayStringMap<UnitType>::Build(
				"Unit",
				{
					{ UNIT_NONE, "NONE" },
					{ UNIT_HTANK, "HTNK" },
					{ UNIT_MTANK, "MTNK" },
					{ UNIT_LTANK, "LTNK" },
					{ UNIT_STANK, "STNK" },
					{ UNIT_FTANK, "FTNK" },
					{ UNIT_VICE, "VICE" },
					{ UNIT_APC, "APC" },
					{ UNIT_MLRS, "MSAM" },
					{ UNIT_JEEP, "JEEP" },
					{ UNIT_BUGGY, "BGGY" },
					{ UNIT_HARVESTER, "HARV" },
					{ UNIT_ARTY, "ARTY" },
					{ UNIT_MSAM, "MLRS" },
					{ UNIT_HOVER, "LST" },
					{ UNIT_MHQ, "MHQ" },
					{ UNIT_GUNBOAT, "BOAT" },
					{ UNIT_MCV, "MCV" },
					{ UNIT_BIKE, "BIKE" },
					{ UNIT_TRIC, "TRIC" },
					{ UNIT_TREX, "TREX" },
					{ UNIT_RAPT, "RAPT" },
					{ UNIT_STEG, "STEG" }
				}
			)
		),
		unitModsMap(TwoWayStringMap<UnitType>::Build("Unit")),
		prereqToStructMap(TypeUtils::InitMapAsRef<long, StructType>({
			{ STRUCTF_NONE, STRUCT_NONE },
			{ STRUCTF_ADVANCED_POWER, STRUCT_ADVANCED_POWER },
			{ STRUCTF_REPAIR, STRUCT_REPAIR },
			{ STRUCTF_EYE, STRUCT_EYE },
			{ STRUCTF_TEMPLE, STRUCT_TEMPLE },
			{ STRUCTF_HAND, STRUCT_HAND },
			{ STRUCTF_BIO_LAB, STRUCT_BIO_LAB },
			{ STRUCTF_OBELISK, STRUCT_OBELISK },
			{ STRUCTF_ATOWER, STRUCT_ATOWER },
			{ STRUCTF_WEAP, STRUCT_WEAP },
			{ STRUCTF_GTOWER, STRUCT_GTOWER },
			{ STRUCTF_RADAR, STRUCT_RADAR },
			{ STRUCTF_TURRET, STRUCT_TURRET },
			{ STRUCTF_CONST, STRUCT_CONST },
			{ STRUCTF_REFINERY, STRUCT_REFINERY },
			{ STRUCTF_STORAGE, STRUCT_STORAGE },
			{ STRUCTF_HELIPAD, STRUCT_HELIPAD },
			{ STRUCTF_SAM, STRUCT_SAM },
			{ STRUCTF_AIRSTRIP, STRUCT_AIRSTRIP },
			{ STRUCTF_POWER, STRUCT_POWER },
			{ STRUCTF_HOSPITAL, STRUCT_HOSPITAL },
			{ STRUCTF_BARRACKS, STRUCT_BARRACKS },
			{ STRUCTF_TANKER, STRUCT_TANKER },
			{ STRUCTF_MISSION, STRUCT_MISSION }
		}))
	{
	}

public:
	static TiberianDawnTypeConverter& Build()
	{
		return *(new TiberianDawnTypeConverter());
	}

	~TiberianDawnTypeConverter()
	{
		delete& difficultyMap;
		delete& housesMap;
		delete& armorMap;
		delete& unitSpeedMap;
		delete& factoryTypeMap;
		delete& warheadModsMap;
		delete& bulletMap;
		delete& bulletModsMap;
		delete& weaponMap;
		delete& weaponModsMap;
		delete& aircraftMap;
		delete& aircraftModsMap;
		delete& structMap;
		delete& structModsMap;
		delete& infantryMap;
		delete& infantryModsMap;
		delete& unitMap;
		delete& unitModsMap;
	}

	template <class T> TwoWayStringMap<T>& GetTypeMap()
	{
		if constexpr (std::is_same<T, DiffType>())
		{
			return difficultyMap;
		}
		else if constexpr (std::is_same<T, HousesType>())
		{
			return housesMap;
		}
		else if constexpr (std::is_same<T, ArmorType>())
		{
			return armorMap;
		}
		else if constexpr (std::is_same<T, SpeedType>())
		{
			return unitSpeedMap;
		}
		else if constexpr (std::is_same<T, FactoryType>())
		{
			return factoryTypeMap;
		}
		else if constexpr (std::is_same<T, WarheadType>())
		{
			return warheadMap;
		}
		else if constexpr (std::is_same<T, BulletType>())
		{
			return bulletMap;
		}
		else if constexpr (std::is_same<T, WeaponType>())
		{
			return weaponMap;
		}
		else if constexpr (std::is_same<T, AircraftType>())
		{
			return aircraftMap;
		}
		else if constexpr (std::is_same<T, StructType>())
		{
			return structMap;
		}
		else if constexpr (std::is_same<T, InfantryType>())
		{
			return infantryMap;
		}
		else if constexpr (std::is_same<T, UnitType>())
		{
			return unitMap;
		}
	}

	template <class T> TwoWayStringMap<T>& GetModTypeMap()
	{
		if constexpr (std::is_same<T, WeaponType>())
		{
			return weaponModsMap;
		}
		else if constexpr (std::is_same<T, BulletType>())
		{
			return bulletModsMap;
		}
		else if constexpr (std::is_same<T, WarheadType>())
		{
			return warheadModsMap;
		}
		else if constexpr (std::is_same<T, AircraftType>())
		{
			return aircraftModsMap;
		}
		else if constexpr (std::is_same<T, StructType>())
		{
			return structModsMap;
		}
		else if constexpr (std::is_same<T, InfantryType>())
		{
			return infantryModsMap;
		}
		else if constexpr (std::is_same<T, UnitType>())
		{
			return unitModsMap;
		}
	}

	template <class T> ResultWithValue<T>& Parse(const char* stringValue, bool ignoreModTypes = false)
	{
		auto& result = GetTypeMap<T>().GetValueForKey(stringValue);

		if constexpr (
			std::is_same<T, WarheadType>()
			|| std::is_same<T, BulletType>()
			|| std::is_same<T, WeaponType>()
			|| std::is_same<T, AircraftType>()
			|| std::is_same<T, StructType>()
			|| std::is_same<T, InfantryType>()
			|| std::is_same<T, UnitType>()
			)
		{
			if (result.IsErrorResult() && !ignoreModTypes)
			{
				delete& result;

				return GetModTypeMap<T>().GetValueForKey(stringValue);
			}
		}

		return result;
	}

	template <class T> T ParseOrDefault(const char* stringValue, T defaultValue, bool ignoreModTypes = false)
	{
		auto& parseResult = Parse<T>(stringValue, ignoreModTypes);
		T value = defaultValue;

		if (!parseResult.IsErrorResult())
		{
			value = parseResult.GetValue();
		}

		delete& parseResult;

		return value;
	}

	template <class T> ResultWithValue<const char*>& ToString(T value, bool ignoreModTypes = false)
	{
		auto& result = GetTypeMap<T>().GetKeyForValue(value);

		if constexpr (
			std::is_same<T, WarheadType>()
			|| std::is_same<T, BulletType>()
			|| std::is_same<T, WeaponType>()
			|| std::is_same<T, AircraftType>()
			|| std::is_same<T, StructType>()
			|| std::is_same<T, InfantryType>()
			|| std::is_same<T, UnitType>()
			)
		{
			if (result.IsErrorResult() && !ignoreModTypes)
			{
				delete& result;

				return GetModTypeMap<T>().GetKeyForValue(value);
			}
		}

		return result;
	}

	template <class T> const char* ToStringOrDefault(T value, const char* defaultValueString = "", bool ignoreModTypes = false)
	{
		auto& stringResult = ToString(value, ignoreModTypes);

		auto valueString = stringResult.GetValue();

		delete& stringResult;

		return StringIsEmpty(valueString) ? defaultValueString : valueString;
	}

	ResultWithValue<int>& ParseHouseNameListCsv(char* houseListCsv, bool ignoreModTypes = false);

	ResultWithValue<int>& ParseHouseNameListCsv(const char* houseListCsv, bool ignoreModTypes = false);

	StructType PrerequisiteToStructureType(long prerequisite);

	const char* PrerequisiteToString(long prerequisite);

	ResultWithValue<long>& StructureTypeToPrerequisite(StructType structType, bool ignoreModTypes = false);

};
