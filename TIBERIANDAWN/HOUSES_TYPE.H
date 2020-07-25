#pragma once

/**********************************************************************
**	The houses that can be played are listed here. Each has their own
**	personality and strengths.
*/
typedef enum HousesType : char {
	HOUSE_NONE = -1,

	HOUSE_GOOD = 0,				// Global Defense Initiative
	HOUSE_BAD = 1,				// Brotherhood of Nod
	HOUSE_NEUTRAL = 2,			// Civilians
	HOUSE_JP = 3,					// Disaster Containment Team
	HOUSE_MULTI1 = 4,				// Multi-Player house #1
	HOUSE_MULTI2 = 5,				// Multi-Player house #2
	HOUSE_MULTI3 = 6,				// Multi-Player house #3
	HOUSE_MULTI4 = 7,				// Multi-Player house #4
	HOUSE_MULTI5 = 8,				// Multi-Player house #5
	HOUSE_MULTI6 = 9,				// Multi-Player house #6

	HOUSE_COUNT,
	HOUSE_FIRST = HOUSE_GOOD
} HousesType;

//inline HousesType operator++(HousesType &, int);
