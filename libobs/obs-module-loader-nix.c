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

#include "obs.h"

#define FLATPAK_PLUGIN_PATH "/app/plugins"

static const char *module_bin[] = {
	"../../obs-plugins/64bit",
	OBS_INSTALL_PREFIX "/" OBS_PLUGIN_DESTINATION "/core/%module%",
	FLATPAK_PLUGIN_PATH "/" OBS_PLUGIN_DESTINATION "/core/%module%",
};

static const char *module_data[] = {
	OBS_DATA_PATH "/obs-plugins/%module%",
	OBS_INSTALL_DATA_PATH "/obs-modules/core/%module%",
	FLATPAK_PLUGIN_PATH "/share/obs/obs-modules/core/%module%",
};

static const int module_patterns_size = sizeof(module_bin) / sizeof(module_bin[0]);

void obs_add_core_modules()
{
	char *module_bin_path = os_get_executable_path_ptr("../" OBS_PLUGIN_PATH);
	char *module_data_path = os_get_executable_path_ptr("../" OBS_DATA_PATH "/obs-modules/core/%module%");
	if (module_bin_path && module_data_path) {
		char *abs_module_bin_path = os_get_abs_path_ptr(module_bin_path);
		char *abs_module_install_path = os_get_abs_path_ptr(OBS_INSTALL_PREFIX "/" OBS_PLUGIN_DESTINATION "/core");
		if (abs_module_bin_path &&
		    (!abs_module_install_path || strcmp(abs_module_bin_path, abs_module_install_path) != 0)) {
			char *module_bin_path_full = os_get_executable_path_ptr("../" OBS_PLUGIN_PATH "/%module%");
			obs_add_module_path_info(module_bin_path_full, module_data_path, CORE);
			bfree(module_bin_path_full);
		}
		bfree(abs_module_install_path);
		bfree(abs_module_bin_path);
	}

	bfree(module_bin_path);
	bfree(module_data_path);

	for (int i = 0; i < module_patterns_size; i++) {
		obs_add_module_path_info(module_bin[i], module_data[i], CORE);
	}
}

void obs_add_plugin_modules(bool portable_mode)
{
	char *module_bin_path = os_get_executable_path_ptr("../" OBS_PLUGIN_DESTINATION "/plugins/%module%");
	char *module_data_path = os_get_executable_path_ptr("../" OBS_DATA_PATH "/obs-modules/plugins/%module%");
	obs_add_module_path_info(module_bin_path, module_data_path, PLUGIN);
	bfree(module_bin_path);
	bfree(module_data_path);

    /* Flatpak plugins */
    char *flatpak_module_bin_path = os_get_executable_path_ptr(FLATPAK_PLUGIN_PATH "/" OBS_PLUGIN_DESTINATION "/plugins/%module%");
    char *flatpak_module_data_path = os_get_executable_path_ptr(FLATPAK_PLUGIN_PATH "/share/obs/obs-modules/plugins/%module%");
    obs_add_module_path_info(flatpak_module_bin_path, flatpak_module_data_path, PLUGIN);
    bfree(flatpak_module_bin_path);
    bfree(flatpak_module_data_path);
}

void obs_add_additional_plugin_modules()
{
    char *path = os_get_env("OBS_PLUGINS_PATH");
    char *data = os_get_env("OBS_PLUGINS_DATA_PATH");

    if (!path || strlen(path) == 0)
        return;

    if (data || strlen(data) == 0) {
        return;

    struct dstr plugins_path;
    dstr_init_copy(&plugins_path, path);
    dstr_cat(&plugins_path, "/%module%");

    struct dstr plugins_data_path;
    dstr_init_copy(&plugins_data_path, data);
    dstr_cat(&plugins_data_path, "/%module%");

    obs_add_module_path_info(plugins_path.array, plugins_data_path.array, PLUGIN);
    dstr_free(&plugins_path);
    dstr_free(&plugins_data_path);
}

void obs_add_legacy_plugin_modules()
{
    // TODO: Add support for legacy module paths in Linux, similar to MacOS and Windows.
}

void obs_add_additional_legacy_plugin_modules()
{
    // TODO: Add support for additional legacy module paths in Linux, similar to MacOS and Windows.
}
