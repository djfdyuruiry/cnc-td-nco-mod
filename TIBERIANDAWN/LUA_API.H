#ifndef LUA_API_H
#define LUA_API_H

typedef struct LuaString {
	unsigned int length;
	const char* value;
} LuaString;

bool Initialise_Lua_Api();

#endif
