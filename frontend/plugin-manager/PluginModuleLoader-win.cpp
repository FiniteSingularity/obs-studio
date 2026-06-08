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

#include "util/dstr.h"

#include "obs.h"
#include "util/platform.h"

#include "PluginModuleLoader.hpp"

const std::string portablePluginModuleBin = "../../plugins/%module%";
const std::string portablePluginModule_Data = "../../plugins/%module%/data";
const std::string portableLegacyPluginModuleBin = "../../obs-plugins/64bit";
const std::string portableLegacyPluginModuleData = "../../data/obs-plugins/%module%";

const std::string pluginPath = "obs-studio/plugins/%module%";
const std::string dataPath = "obs-studio/plugins/%module%/data";

std::vector<std::string> loadPluginModules(bool portableMode);
std::vector<std::string> loadAdditionalPluginModules();
std::vector<std::string> loadLegacyPluginModules(bool portableMode);
std::vector<std::string> loadAdditionalLegacyPluginModules();

std::vector<std::string> loadPlugins(bool portableMode)
{
	auto failedModules = loadPluginModules(portableMode);
	auto failedAdditionalModules = loadAdditionalPluginModules();

	failedModules.reserve(failedModules.size() + failedAdditionalModules.size());
	failedModules.insert(failedModules.end(), failedAdditionalModules.begin(), failedAdditionalModules.end());

	return failedModules;
}

std::vector<std::string> loadLegacyPlugins(bool portableMode)
{
	auto failedModules = loadLegacyPluginModules(portableMode);
	auto failedAdditionalModules = loadAdditionalLegacyPluginModules();

	failedModules.reserve(failedModules.size() + failedAdditionalModules.size());
	failedModules.insert(failedModules.end(), failedAdditionalModules.begin(), failedAdditionalModules.end());

	return failedModules;
}

std::vector<std::string> loadPluginModules(bool portableMode)
{
	struct obs_module_path omp;
	omp.module_type = PLUGIN;

	std::vector<std::string> failedPlugins{};

	if (portableMode) {
		omp.bin = portablePluginModuleBin.c_str();
		omp.data = portablePluginModule_Data.c_str();
	} else {
		char plugin_bin_path[512];
		char plugin_data_path[512];
		int ret = os_get_program_data_path(plugin_bin_path, sizeof(plugin_bin_path), pluginPath.c_str());
		if (ret <= 0)
			return failedPlugins;
		ret = os_get_program_data_path(plugin_data_path, sizeof(plugin_data_path), dataPath.c_str());
		if (ret <= 0)
			return failedPlugins;

		omp.bin = plugin_bin_path;
		omp.data = plugin_data_path;
	}

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);
	obs_load_plugins(&omp, &mfi);

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
}

std::vector<std::string> loadAdditionalPluginModules()
{
	
	char *s = getenv("OBS_PLUGINS_PATH");
	std::string path = s ? s : "";

	std::vector<std::string> failedPlugins{};

	if (path.empty())
		return failedPlugins;

	std::string plugins_bin_path = path + "/%module%";
	std::string plugins_data_path = path + "/%module%/data";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = plugins_bin_path.c_str();
	omp.data = plugins_data_path.c_str();

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);

	obs_load_plugins(&omp, &mfi);

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
}

std::vector<std::string> loadLegacyPluginModules(bool portableMode)
{
	// Legacy plugin modules stored globally
	std::vector<std::string> failedPlugins{};
	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);

	if (!portableMode) {
		char base_module_dir[512];
		int ret = os_get_program_data_path(base_module_dir, sizeof(base_module_dir), pluginPath.c_str());
		if (ret <= 0)
			return failedPlugins;

		std::string bin_path = base_module_dir;
		bin_path += "/bin/64bit";

		std::string data_path = base_module_dir;
		data_path += "/data";

		struct obs_module_path omp;
		omp.module_type = LEGACY_PLUGIN;
		omp.bin = bin_path.c_str();
		omp.data = data_path.c_str();

		obs_load_plugins(&omp, &mfi);
	} else {
		// Legacy plugin modules stored locally
		// This is the same as the location for legacy portable plugins.
		struct obs_module_path omp_local;
		omp_local.module_type = LEGACY_PLUGIN;
		omp_local.bin = portableLegacyPluginModuleBin.c_str();
		omp_local.data = portableLegacyPluginModuleData.c_str();

		obs_load_plugins(&omp_local, &mfi);

		for (size_t i = 0; i < mfi.count; ++i) {
			const char *failedPluginName = mfi.failed_modules.array[i].array;
			failedPlugins.emplace_back(failedPluginName);
		}
	}
	obs_module_failure_info_free(&mfi);
	return failedPlugins;
}

std::vector<std::string> loadAdditionalLegacyPluginModules()
{
	std::vector<std::string> failedPlugins{};
	char *s = getenv("OBS_LEGACY_PLUGINS_PATH");
	std::string bin_path = s ? s : "";
	s = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
	std::string data_path = s ? s : "";

	if (bin_path.empty() || data_path.empty()) {
		return failedPlugins;
	}

	std::string data = data_path + "/%module%";

	struct obs_module_path omp;
	omp.module_type = LEGACY_PLUGIN;
	omp.bin = bin_path.c_str();
	omp.data = data.c_str();

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);

	obs_load_plugins(&omp, &mfi);

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);
	return failedPlugins;
}
