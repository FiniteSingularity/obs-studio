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
#include <stdlib.h>

#include "obs-module-loader.h"
#include "obs-core-modules-loader.h"

void obs_add_core_modules() 
{
	//obs_load_core_modules();
}

void obs_add_plugin_modules(bool portable_mode)
{
	//obs_load_plugin_modules(portable_mode);
}

void obs_add_additional_plugin_modules()
{
	//obs_load_additional_plugin_modules();
}

void obs_add_legacy_plugin_modules()
{
	//obs_load_legacy_plugin_modules();
}

void obs_add_additional_legacy_plugin_modules()
{
	//obs_load_additional_legacy_plugin_modules();
}
