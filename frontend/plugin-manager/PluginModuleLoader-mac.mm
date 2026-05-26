#include <string>

#include "obs.h"
#include "util/platform.h"

#include "PluginModuleLoader.hpp"

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
	UNUSED_PARAMETER(portableMode);
	
	
	
	// Is there a better way to grab the base module directory path on MacOS using obj-c?
	char module_path[PATH_MAX];
	int ret = os_get_config_path(module_path, sizeof(module_path), "obs-studio/plugins/%module%.plugin");
	if(ret <= 0) {
		blog(LOG_ERROR, "Failed to get module path");
		return;
	}

	std::string binPath = module_path;
	binPath += "/Contents/MacOS";

	std::string dataPath = module_path;
	dataPath += "/Contents/Resources";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = binPath.c_str();
	omp.data = dataPath.c_str();
	
	obs_load_plugins(&omp, &mfi);
	
	// TODO: Throw an exception here if mfi indicates core modules not loaded.
	//       Exception should be caught in plugin manager and trigger a dialog
	//       indicating core plugin load failure, with button to shut down app.
}

void loadAdditionalPluginModules(struct obs_module_failure_info &mfi)
{
	char *path = getenv("OBS_PLUGINS_PATH");
	if (!path) {
		return;
	}
	
	std::string bin = path;
	bin += "/%module%.plugin/Contents/MacOS";

	std::string data = path;
	data += "/%module%.plugin/Contents/Resources";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = bin.c_str();
	omp.data = data.c_str();
	
	obs_load_plugins(&omp, &mfi);
}

void loadLegacyPluginModules(struct obs_module_failure_info &mfi)
{
#ifndef __aarch64__
//	/* Legacy System Library Search Path */
	char system_legacy_module_dir[PATH_MAX];
	int ret = os_get_program_data_path(system_legacy_module_dir, sizeof(system_legacy_module_dir), "obs-studio/plugins/%module%");
	if(ret <= 0) {
		blog(LOG_ERROR, "Failed to get module path");
		return;
	}

	std::string systemBinPath = system_legacy_module_dir;
	systemBinPath += "/bin";

	std::string systemDataPath = system_legacy_module_dir;
	systemDataPath += "/data";

	struct obs_module_path ompSystem;
	ompSystem.module_type = LEGACY_PLUGIN;
	ompSystem.bin = systemBinPath.c_str();
	ompSystem.data = systemDataPath.c_str();
	
	obs_load_plugins(&ompSystem, &mfi);

	/* Legacy User Application Support Search Path */
	char user_legacy_module_dir[PATH_MAX];
	ret = os_get_config_path(user_legacy_module_dir, sizeof(user_legacy_module_dir), "obs-studio/plugins/%module%");
	if(ret <= 0) {
		blog(LOG_ERROR, "Failed to get module path");
		return;
	}

	std::string userBinPath = user_legacy_module_dir;
	userBinPath += "/bin";

	std::string userDataPath = user_legacy_module_dir;
	userDataPath += "/data";

	struct obs_module_path ompUser;
	ompUser.module_type = LEGACY_PLUGIN;
	ompUser.bin = userBinPath.c_str();
	ompUser.data = userDataPath.c_str();
	
	obs_load_plugins(&ompUser, &mfi);
#else
	UNUSED_PARAMETER(mfi);
#endif
}

void loadAdditionalLegacyPluginModules(struct obs_module_failure_info &mfi)
{
	char *bin_path = getenv("OBS_LEGACY_PLUGINS_PATH");
	char *data_path = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
	
	if (!bin_path || !data_path) {
	    return;
	}

	std::string bin = bin_path;
	bin += "/%module%/bin";

	std::string data = data_path;
	data += "/%module%/data";

	struct obs_module_path omp;
	omp.module_type = PLUGIN;
	omp.bin = bin.c_str();
	omp.data = data.c_str();
	
	obs_load_plugins(&omp, &mfi);
}
