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

#include <string>

#include "obs.h"
#include "util/platform.h"

#include "PluginModuleLoader.hpp"

const std::string portablePluginModuleBin = "../../plugins/%module%";
const std::string portablePluginModule_Data = "../../plugins/%module%/data";
const std::string portableLegacyPluginModuleBin = "../../obs-plugins/64bit";
const std::string portableLegacyPluginModuleData = "../../data/obs-plugins/%module%";

const std::string pluginPath = "obs-studio/plugins/%module%";
const std::string dataPath = "obs-studio/plugins/%module%/data";

void loadPluginModules(bool portableMode, struct obs_module_failure_info &mfi);
void loadAdditionalPluginModules(struct obs_module_failure_info &mfi);
void loadLegacyPluginModules(struct obs_module_failure_info &mfi);
void loadAdditionalLegacyPluginModules(struct obs_module_failure_info &mfi);

void loadPlugins(bool portableMode, struct obs_module_failure_info& mfi)
{
	loadPluginModules(portableMode, mfi);
	loadAdditionalPluginModules(mfi);
}

void loadLegacyPlugins(struct obs_module_failure_info& mfi)
{
	loadLegacyPluginModules(mfi);
	loadAdditionalLegacyPluginModules(mfi);
}

void loadPluginModules(bool portableMode, struct obs_module_failure_info &mfi)
{
	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	if (portableMode) {
		omp.bin = bstrdup(portablePluginModuleBin.c_str());
		omp.data = bstrdup(portablePluginModule_Data.c_str());
	} else {
		char plugin_bin_path[512];
		char plugin_data_path[512];
		int ret = os_get_program_data_path(plugin_bin_path, sizeof(plugin_bin_path), pluginPath.c_str());
		if (ret <= 0)
			return;
		ret = os_get_program_data_path(plugin_data_path, sizeof(plugin_data_path), dataPath.c_str());
		if (ret <= 0)
			return;

		omp.bin = plugin_bin_path;
		omp.data = plugin_data_path;
	}
	obs_load_plugins(&omp, &mfi);

	// TODO: Throw an exception here if mfi indicates core modules not loaded.
	//       Exception should be caught in plugin manager and trigger a dialog
	//       indicating core plugin load failure, with button to shut down app.
}

void loadAdditionalPluginModules(struct obs_module_failure_info &mfi)
{
	
	char *s = getenv("OBS_PLUGINS_PATH");
	std::string path = s ? s : "";
	
	if (path.empty())
		return;

	std::string plugins_bin_path = path + "/%module%";
	std::string plugins_data_path = path + "/%module%/data";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = plugins_bin_path.c_str();
	omp.data = plugins_data_path.c_str();

	obs_load_plugins(&omp, &mfi);
}

void loadLegacyPluginModules(struct obs_module_failure_info &mfi)
{
	// Legacy plugin modules stored globally
	char base_module_dir[512];
	int ret = os_get_program_data_path(base_module_dir, sizeof(base_module_dir), pluginPath.c_str());
	if (ret <= 0)
		return;

	std::string bin_path = base_module_dir;
	bin_path += "/bin/64bit";

	std::string data_path = base_module_dir;
	data_path += "/data";

	struct obs_module_path omp;
	omp.module_type = LEGACY_PLUGIN;
	omp.bin = bin_path.c_str();
	omp.data = data_path.c_str();

	obs_load_plugins(&omp, &mfi);

	// Legacy plugin modules stored locally
	// This is the same as the location for legacy portable plugins.
	struct obs_module_path omp_local;
	omp_local.module_type = LEGACY_PLUGIN;
	omp_local.bin = portableLegacyPluginModuleBin.c_str();
	omp_local.data = portableLegacyPluginModuleData.c_str();

	obs_load_plugins(&omp_local, &mfi);
}

void loadAdditionalLegacyPluginModules(struct obs_module_failure_info &mfi)
{
	char *s = getenv("OBS_LEGACY_PLUGINS_PATH");
	std::string bin_path = s ? s : "";
	s = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
	std::string data_path = s ? s : "";

	if (bin_path.empty()) {
		return;
	}
	if (data_path.empty()) {
		return;
	}

	std::string data = data_path + "/%module%";

	struct obs_module_path omp;
	omp.module_type = LEGACY_PLUGIN;
	omp.bin = bin_path.c_str();
	omp.data = data.c_str();

	obs_load_plugins(&omp, &mfi);
}
