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
#include "obs.h"
#include "obs-module-loader.h"
#include "util/dstr.h"
#include "util/platform.h"

#include <Carbon/Carbon.h>


void obs_add_core_modules() 
{
	NSURL *pluginURL = [[NSBundle mainBundle] builtInPlugInsURL];
	NSString *pluginModulePath = [[pluginURL path] stringByAppendingString:@"/%module%.plugin/Contents/MacOS/"];
	NSString *pluginDataPath = [[pluginURL path] stringByAppendingString:@"/%module%.plugin/Contents/Resources/"];

	obs_add_module_path_info(pluginModulePath.UTF8String, pluginDataPath.UTF8String, CORE);
}

void obs_add_plugin_modules(bool portable_mode)
{
	UNUSED_PARAMETER(portable_mode);
	// Is there a better way to grab the base module directory path on MacOS using obj-c?
	char module_path[PATH_MAX];
	int ret = os_get_config_path(module_path, sizeof(module_path), "obs-studio/plugins/%module%.plugin");
	if(ret <= 0) {
		blog(LOG_ERROR, "Failed to get module path");
		return;
	}

	struct dstr bin_path;
	dstr_init_copy(&bin_path, module_path);
	dstr_cat(&bin_path, "/Contents/MacOS");

	struct dstr data_path;
	dstr_init_copy(&data_path, module_path);
	dstr_cat(&data_path, "/Contents/Resources");


	obs_add_module_path_info(bin_path.array, data_path.array, PLUGIN);

	dstr_free(&bin_path);
	dstr_free(&data_path);
}

void obs_add_additional_plugin_modules()
{
    char *path = getenv("OBS_PLUGINS_PATH");
    if (!path) {
        return;
    }

    struct dstr bin;
    dstr_init_copy(&bin, path);
    dstr_cat(&bin, "/%module%.plugin/Contents/MacOS");

    struct dstr data;
    dstr_init_copy(&data, path);
    dstr_cat(&data, "/%module%.plugin/Contents/Resources");

    obs_add_module_path_info(bin.array, data.array, PLUGIN);

    dstr_free(&bin);
    dstr_free(&data);
}

void obs_add_legacy_plugin_modules()
{
#ifndef __aarch64__
//	/* Legacy System Library Search Path */
	char system_legacy_module_dir[PATH_MAX];
    int ret = os_get_program_data_path(system_legacy_module_dir, sizeof(system_legacy_module_dir), "obs-studio/plugins/%module%");
    if(ret <= 0) {
        blog(LOG_ERROR, "Failed to get module path");
        return;
    }

    struct dstr system_bin_path;
	dstr_init_copy(&system_bin_path, system_legacy_module_dir);
	dstr_cat(&system_bin_path, "/bin");

	struct dstr system_data_path;
	dstr_init_copy(&system_data_path, system_legacy_module_dir);
	dstr_cat(&system_data_path, "/data");


    obs_add_module_path_info(system_bin_path.array, system_data_path.array, LEGACY);

    dstr_free(&system_bin_path);
	dstr_free(&system_data_path);

	/* Legacy User Application Support Search Path */
	char user_legacy_module_dir[PATH_MAX];
    ret = os_get_config_path(user_legacy_module_dir, sizeof(user_legacy_module_dir), "obs-studio/plugins/%module%");
    if(ret <= 0) {
        blog(LOG_ERROR, "Failed to get module path");
        return;
    }

    struct dstr user_bin_path;
	dstr_init_copy(&user_bin_path, user_legacy_module_dir);
	dstr_cat(&user_bin_path, "/bin");

	struct dstr user_data_path;
	dstr_init_copy(&user_data_path, user_legacy_module_dir);
	dstr_cat(&user_data_path, "/data");


    obs_add_module_path_info(user_bin_path.array, user_data_path.array, LEGACY);

    dstr_free(&user_bin_path);
	dstr_free(&user_data_path);
#endif
}

void obs_add_additional_legacy_plugin_modules()
{
    char *bin_path = getenv("OBS_LEGACY_PLUGINS_PATH");
    char *data_path = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
    
    if (!bin_path || !data_path) {
        return;
    }

    struct dstr bin;
    dstr_init_copy(&bin, bin_path);
    dstr_cat(&bin, "/%module%/bin");

    struct dstr data;
    dstr_init_copy(&data, data_path);
    dstr_cat(&data, "/%module%/data");

    obs_add_module_path_info(bin.array, data.array, LEGACY_PLUGIN);

    dstr_free(&bin);
    dstr_free(&data);
}
