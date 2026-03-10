/******************************************************************************
    Copyright (C) 2026 by FiniteSingularity <finitesingularityttv@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include "obs.h"

#ifdef __cplusplus
#define MODULE_EXPORT extern "C" EXPORT
#define MODULE_EXTERN extern "C"
#else
#define MODULE_EXPORT EXPORT
#define MODULE_EXTERN extern
#endif


MODULE_EXPORT void obs_add_core_modules();
MODULE_EXPORT void obs_add_plugin_modules(bool portable_mode);
/* Adds modules from environment variables */
MODULE_EXPORT void obs_add_additional_plugin_modules();

/* legacy will be deprecated */
MODULE_EXPORT void obs_add_legacy_plugin_modules();
/* Adds legacy modules from environment variables */
MODULE_EXPORT void obs_add_additional_legacy_plugin_modules();
