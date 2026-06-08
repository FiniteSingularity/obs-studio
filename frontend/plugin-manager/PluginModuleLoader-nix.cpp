#include <vector>
#include <string>

#include "obs.h"
#include <util/platform.h>
#include <util/dstr.h>
#include "PluginModuleLoader.hpp"

#define FLATPAK_PLUGIN_PATH "/app/plugins"

std::vector<std::string> loadPluginModules();
std::vector<std::string> loadAdditionalPluginModules();
std::vector<std::string> loadLegacyPluginModules();

static const std::vector<std::string> moduleBin = {
    "../../obs-plugins/64bit",
	OBS_INSTALL_PREFIX "/" OBS_PLUGIN_DESTINATION,
	FLATPAK_PLUGIN_PATH "/" OBS_PLUGIN_DESTINATION
};

static const std::vector<std::string> moduleData = {
	OBS_DATA_PATH "/obs-plugins/%module%",
	OBS_INSTALL_DATA_PATH "/obs-plugins/%module%",
	FLATPAK_PLUGIN_PATH "/share/obs/obs-plugins/%module%"
};

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

    return failedModules;
}

std::vector<std::string> loadPluginModules()
{ 
 	std::vector<std::string> failedPlugins{};
    char *module_bin_path = os_get_executable_path_ptr("../" OBS_PLUGIN_DESTINATION "/plugins/%module%");
	char *module_data_path = os_get_executable_path_ptr("../" OBS_DATA_PATH "/obs-modules/plugins/%module%");

	struct obs_module_path ompBase;
	ompBase.module_type = PLUGIN;
    ompBase.bin = module_bin_path;
    ompBase.data = module_data_path;

	struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);

    obs_load_plugins(&ompBase, &mfi);

	bfree(module_bin_path);
	bfree(module_data_path);

    /* Flatpak plugins */
    char *flatpak_module_bin_path = os_get_executable_path_ptr(FLATPAK_PLUGIN_PATH "/" OBS_PLUGIN_DESTINATION "/plugins/%module%");
    char *flatpak_module_data_path = os_get_executable_path_ptr(FLATPAK_PLUGIN_PATH "/share/obs/obs-modules/plugins/%module%");

	struct obs_module_path ompFlatPak;
	ompFlatPak.module_type = PLUGIN;
    ompFlatPak.bin = flatpak_module_bin_path;
    ompFlatPak.data = flatpak_module_data_path;
    obs_load_plugins(&ompFlatPak, &mfi);

    bfree(flatpak_module_bin_path);
    bfree(flatpak_module_data_path);

    for (size_t i = 0; i < mfi.count; ++i) {
		const char *failedPluginName = mfi.failed_modules.array[i].array;
		failedPlugins.emplace_back(failedPluginName);
	}

	obs_module_failure_info_free(&mfi);

    return failedPlugins;
}

std::vector<std::string> loadAdditionalPluginModules()
{
	std::vector<std::string> failedPlugins{};
    char *pluginPathEnv = getenv("OBS_PLUGINS_PATH");
    char *pluginDataPathEnv = getenv("OBS_PLUGINS_DATA_PATH");

	std::string pluginPath = pluginPathEnv ? pluginPathEnv : "";
	std::string pluginDataPath = pluginDataPathEnv ? pluginDataPathEnv : "";

    if (pluginPath.empty() || pluginDataPath.empty()) {
        return failedPlugins;
    }

    pluginPath += "/%module%";
    pluginDataPath += "/%module%";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = pluginPath.c_str();
	omp.data = pluginDataPath.c_str();

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

std::vector<std::string> loadLegacyPluginModules()
{
    std::vector<std::string> failedPlugins{};

    struct obs_module_failure_info mfi;
	obs_module_failure_info_init(&mfi);

    for (size_t i = 0; i < moduleBin.size(); ++i) {
        struct obs_module_path omp;
        omp.module_type = LEGACY_PLUGIN;
        omp.bin = moduleBin[i].c_str();
        omp.data = moduleData[i].c_str();

        obs_load_plugins(&omp, &mfi);
	}

    for (size_t i = 0; i < mfi.count; ++i) {
        const char *failedPluginName = mfi.failed_modules.array[i].array;
        failedPlugins.emplace_back(failedPluginName);
    }

    obs_module_failure_info_free(&mfi);

    return failedPlugins;
}