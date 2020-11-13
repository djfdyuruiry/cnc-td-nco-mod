#pragma once

#define PARSER(t, p) t(*p)(const char*, bool*, bool)

#define SERIALISER(u, n) const char* (*n)(u, bool)
