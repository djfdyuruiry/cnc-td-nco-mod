#pragma once

#include <NcoRuntime.h>

#include "lua_events.h"
#include "tiberian_dawn_rules.h"
#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnNcoRuntime.h"

#define NcoStartup() NcoRuntime::Startup<TiberianDawnNcoRuntime>()

#define NcoShutdown() NcoRuntime::Shutdown<TiberianDawnNcoRuntime>()
