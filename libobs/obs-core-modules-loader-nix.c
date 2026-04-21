// TODO- Modify this to load core modules properly by name.

void obs_core_modules_load(obs_find_module_callback2_t callback, void *param)
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