#include "PluginModuleLoader.hpp"

#include "obs.h"
#include "util/platform.h"
#include "util/dstr.h"

#include <string>
#include <vector>

std::vector<std::string> loadPluginModules();
std::vector<std::string> loadAdditionalPluginModules();
std::vector<std::string> loadLegacyPluginModules();
std::vector<std::string> loadAdditionalLegacyPluginModules();

std::vector<std::string> loadPlugins(bool portableMode)
{
	UNUSED_PARAMETER(portableMode);
	auto failedModules = loadPluginModules();
	auto failedAdditionalModules = loadAdditionalPluginModules();

	failedModules.reserve(failedModules.size() + failedAdditionalModules.size());
	failedModules.insert(failedModules.end(), failedAdditionalModules.begin(), failedAdditionalModules.end());

	return failedModules;
}

std::vector<std::string> loadLegacyPlugins(bool portableMode)
{
	UNUSED_PARAMETER(portableMode);
	auto failedModules = loadLegacyPluginModules();
	auto failedAdditionalModules = loadAdditionalLegacyPluginModules();

	failedModules.reserve(failedModules.size() + failedAdditionalModules.size());
	failedModules.insert(failedModules.end(), failedAdditionalModules.begin(), failedAdditionalModules.end());

	return failedModules;
}

std::vector<std::string> loadPluginModules()
{
	char module_path[PATH_MAX];
	int ret = os_get_config_path(module_path, sizeof(module_path), "obs-studio/plugins/%module%.plugin");
	if(ret <= 0) {
		blog(LOG_ERROR, "Failed to get module path");
		throw;
	}

	std::string binPath = module_path;
	binPath += "/Contents/MacOS";

	std::string dataPath = module_path;
	dataPath += "/Contents/Resources";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = binPath.c_str();
	omp.data = dataPath.c_str();

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);
	
	obs_load_plugins(&omp, &mfi);

	std::vector<std::string> failedPlugins{};

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
}

std::vector<std::string> loadAdditionalPluginModules()
{
	char *path = getenv("OBS_PLUGINS_PATH");
	if (!path) {
		return {};
	}
	
	std::string bin = path;
	bin += "/%module%.plugin/Contents/MacOS";

	std::string data = path;
	data += "/%module%.plugin/Contents/Resources";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = bin.c_str();
	omp.data = data.c_str();

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);
	
	obs_load_plugins(&omp, &mfi);

	std::vector<std::string> failedPlugins {};

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
}

std::vector<std::string> loadLegacyPluginModules()
{
#ifndef __aarch64__
	/* Legacy System Library Search Path */
	char system_legacy_module_dir[PATH_MAX];

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);
	
	int ret = os_get_program_data_path(system_legacy_module_dir, sizeof(system_legacy_module_dir), "obs-studio/plugins/%module%");
	if(ret > 0) {
		std::string systemBinPath = system_legacy_module_dir;
		systemBinPath += "/bin";

		std::string systemDataPath = system_legacy_module_dir;
		systemDataPath += "/data";

		struct obs_module_path ompSystem;
		ompSystem.module_type = LEGACY_PLUGIN;
		ompSystem.bin = systemBinPath.c_str();
		ompSystem.data = systemDataPath.c_str();
		
		obs_load_plugins(&ompSystem, &mfi);
	}
	/* Legacy User Application Support Search Path */
	char user_legacy_module_dir[PATH_MAX];
	ret = os_get_config_path(user_legacy_module_dir, sizeof(user_legacy_module_dir), "obs-studio/plugins/%module%");
	if(ret > 0) {
		std::string userBinPath = user_legacy_module_dir;
		userBinPath += "/bin";

		std::string userDataPath = user_legacy_module_dir;
		userDataPath += "/data";

		struct obs_module_path ompUser;
		ompUser.module_type = LEGACY_PLUGIN;
		ompUser.bin = userBinPath.c_str();
		ompUser.data = userDataPath.c_str();
		
		obs_load_plugins(&ompUser, &mfi);
	}

	std::vector<std::string> failedPlugins{};

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
#else
	return {};
#endif
}

std::vector<std::string> loadAdditionalLegacyPluginModules()
{
	char *bin_path = getenv("OBS_LEGACY_PLUGINS_PATH");
	char *data_path = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
	
	if (!bin_path || !data_path) {
	    return {};
	}

	std::string bin = bin_path;
	bin += "/%module%/bin";

	std::string data = data_path;
	data += "/%module%/data";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = bin.c_str();
	omp.data = data.c_str();

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);
	
	obs_load_plugins(&omp, &mfi);

	std::vector<std::string> failedPlugins{};

	for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

	return failedPlugins;
}
