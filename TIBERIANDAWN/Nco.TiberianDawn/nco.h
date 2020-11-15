#pragma once

#include <NcoRuntime.h>

#include "lua_events.h"
#include "tiberian_dawn_rules.h"
#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnNcoRuntime.h"

#define NCO_Startup() NcoRuntime::Startup<TiberianDawnNcoRuntime>()

#define NCO_Shutdown() NcoRuntime::Shutdown<TiberianDawnNcoRuntime>()
