#ifndef NCO_H
#define NCO_H

#include "./logger.h"
#include "./lua.h"
#include "./lua_events.h"
#include "./lua_api.h"
#include "./lua_api_functions.h"
#include "./nco.h"
#include "./parse.h"
#include "./rules_ini.h"
#include "./strings.h"
#include "./utils.h"

bool NCO_Startup();

void NCO_Shutdown();

#endif
