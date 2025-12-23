#pragma once

#include <stdbool.h>

extern const char *obs_core_modules[];
extern const unsigned int obs_core_modules_count;

extern bool obs_is_core_module(const char *name);
