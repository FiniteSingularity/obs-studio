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
#include "obs.h"
#include "obs-internal.h"

static const char *core_module_bin = "../../core/%module%";
static const char *core_module_data = "../../core/%module%/data";
static const char *portable_plugin_module_bin = "../../plugins/%module%";
static const char *portable_plugin_module_data = "../../plugins/%module%/data";
static const char *portable_legacy_plugin_module_bin = "../../obs-plugins/64bit";
static const char *portable_legacy_plugin_module_data = "../../data/obs-plugins/%module%";

static const char *plugin_path = "obs-studio/plugins/%module%";
static const char *data_path = "obs-studio/plugins/%module%/data";

void obs_add_core_modules() 
{
	obs_add_module_path_info(core_module_bin, core_module_data, CORE);
}

void obs_add_plugin_modules(bool portable_mode)
{
	if (portable_mode) {
		obs_add_module_path_info(portable_plugin_module_bin, portable_plugin_module_data, PLUGIN);
		obs_add_module_path_info(portable_legacy_plugin_module_bin, portable_legacy_plugin_module_data, LEGACY_PLUGIN);
	} else {
		char plugin_bin_path[512];
		char plugin_data_path[512];
		int ret = os_get_program_data_path(plugin_bin_path, sizeof(plugin_bin_path), plugin_path);
		if (ret <= 0)
			return;
		ret = os_get_program_data_path(plugin_data_path, sizeof(plugin_data_path), data_path);
		if (ret <= 0)
			return;

		obs_add_module_path_info(plugin_bin_path, plugin_data_path, PLUGIN);
	}
}

void obs_add_additional_plugin_modules()
{
	char *path = getenv("OBS_PLUGINS_PATH");
	if (!path ||strlen(path) == 0)
		return;

	struct dstr plugins_bin_path;
	dstr_init_copy(&plugins_bin_path, path);
	dstr_cat(&plugins_bin_path, "/%module%");

	struct dstr plugins_data_path;
	dstr_init_copy(&plugins_data_path, path);
	dstr_cat(&plugins_data_path, "/%module%/data");

	obs_add_module_path_info(plugins_bin_path.array, plugins_data_path.array, PLUGIN);
	dstr_free(&plugins_bin_path);
	dstr_free(&plugins_data_path);
}

void obs_add_legacy_plugin_modules()
{
	// Legacy plugin modules stored globally
	char base_module_dir[512];
	int ret = os_get_program_data_path(base_module_dir, sizeof(base_module_dir), plugin_path);
	if (ret <= 0)
		return;

	struct dstr bin_path;
	dstr_init_copy(&bin_path, base_module_dir);
	dstr_cat(&bin_path, "/bin/64bit");

	struct dstr data_path;
	dstr_init_copy(&data_path, base_module_dir);
	dstr_cat(&data_path, "/data");

	obs_add_module_path_info(bin_path.array, data_path.array, LEGACY_PLUGIN);

	dstr_free(&bin_path);
	dstr_free(&data_path);

	// Legacy plugin modules stored locally
	// This is the same as the location for legacy portable plugins.
	obs_add_module_path_info(portable_legacy_plugin_module_bin, portable_legacy_plugin_module_data, LEGACY_PLUGIN);
}

void obs_add_additional_legacy_plugin_modules()
{
	char *bin_path = getenv("OBS_LEGACY_PLUGINS_PATH");
	char *data_path = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
	
	if (!bin_path || strlen(bin_path) == 0) {
		return;
	}
	if (!data_path || strlen(data_path) == 0) {
		return;
	}

	struct dstr data;
	dstr_init_copy(&data, data_path);
	dstr_cat(&data, "/%module%");

	obs_add_module_path_info(bin_path, data.array, LEGACY_PLUGIN);
	dstr_free(&data);
}
