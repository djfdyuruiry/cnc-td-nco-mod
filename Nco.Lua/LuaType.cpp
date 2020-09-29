#include "LuaType.h"

const LuaType& LuaType::Number = *(new LuaType("number"));
const LuaType& LuaType::Bool = *(new LuaType("bool"));
const LuaType& LuaType::String = *(new LuaType("string"));
const LuaType& LuaType::Function = *(new LuaType("function"));
const LuaType& LuaType::Table = *(new LuaType("table"));
const LuaType& LuaType::Nil = *(new LuaType("nil"));
