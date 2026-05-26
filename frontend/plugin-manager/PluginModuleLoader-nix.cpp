#include <vector>
#include <string>

#include "obs.h"
#include <util/platform.h>
#include "PluginModuleLoader.hpp"

#define FLATPAK_PLUGIN_PATH "/app/plugins"

void loadPluginModules(bool portableMode, struct obs_module_failure_info &mfi);
void loadAdditionalPluginModules(struct obs_module_failure_info &mfi);
void loadLegacyPluginModules(struct obs_module_failure_info &mfi);

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

void loadPlugins(bool portableMode, struct obs_module_failure_info& mfi)
{
    loadPluginModules(portableMode, mfi);
    loadAdditionalPluginModules(mfi);
}

void loadLegacyPlugins(struct obs_module_failure_info& mfi)
{
    loadLegacyPluginModules(mfi);
}

void loadPluginModules(bool portableMode, struct obs_module_failure_info &mfi)
{
    UNUSED_PARAMETER(portableMode);
    char *module_bin_path = os_get_executable_path_ptr("../" OBS_PLUGIN_DESTINATION "/plugins/%module%");
	char *module_data_path = os_get_executable_path_ptr("../" OBS_DATA_PATH "/obs-modules/plugins/%module%");

	struct obs_module_path ompBase;
	ompBase.module_type = PLUGIN;
    ompBase.bin = module_bin_path;
    ompBase.data = module_data_path;
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

	// TODO: Throw an exception here if mfi indicates core modules not loaded.
	//       Exception should be caught in plugin manager and trigger a dialog
	//       indicating core plugin load failure, with button to shut down app.
}

void loadAdditionalPluginModules(struct obs_module_failure_info &mfi)
{
	
    char *pluginPathEnv = getenv("OBS_PLUGINS_PATH");
    char *pluginDataPathEnv = getenv("OBS_PLUGINS_DATA_PATH");

	std::string pluginPath = pluginPathEnv ? pluginPathEnv : "";
	std::string pluginDataPath = pluginDataPathEnv ? pluginDataPathEnv : "";

    if (pluginPath.empty() || pluginDataPath.empty()) {
        return;
    }

    pluginPath += "/%module%";
    pluginDataPath += "/%module%";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = pluginPath.c_str();
	omp.data = pluginDataPath.c_str();  

	obs_load_plugins(&omp, &mfi);
}

void loadLegacyPluginModules(struct obs_module_failure_info &mfi)
{
    for (size_t i = 0; i < moduleBin.size(); ++i) {
        struct obs_module_path omp;
        omp.module_type = LEGACY_PLUGIN;
        omp.bin = moduleBin[i].c_str();
        omp.data = moduleData[i].c_str();

        obs_load_plugins(&omp, &mfi);
	}
}