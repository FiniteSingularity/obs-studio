#include <obs-core-modules.h>
#include <util/platform.h>
#include <util/dstr.h>

#include <obs.h>
#include <obs-internal.h>

const char *core_module_bin = "../" OBS_PLUGIN_PATH "/%module%/%module%";
const char *core_module_data = "../" OBS_DATA_PATH "/obs-modules/core/%module%";

extern bool find_core_module(struct obs_module_path *omp, obs_find_module_callback2_t callback, void *param);

void load_core_modules(obs_find_module_callback2_t callback, struct obs_module_failure_info *mfi)
{
	char *core_bin_path = os_get_executable_path_ptr(core_module_bin);
	char *core_data_path = os_get_executable_path_ptr(core_module_data);

    for (unsigned int i = 0; i < obs_core_modules_count; i++) {
		const char *name = obs_core_modules[i];
		struct dstr bin_path = {0};
		struct dstr data_path = {0};
		dstr_init_copy(&bin_path, core_bin_path);
		dstr_init_copy(&data_path, core_data_path);
		dstr_replace(&bin_path, "%module%", name);
		dstr_replace(&data_path, "%module%", name);

		struct obs_module_path omp;

		omp.bin = bstrdup(bin_path.array);
		omp.data = bstrdup(data_path.array);
		omp.module_type = CORE;

		if (!find_core_module(&omp, callback, param)) {
			blog(LOG_ERROR, "Failed to load core module %s", name);
		}

		bfree(omp.bin);
		bfree(omp.data);

		dstr_free(&bin_path);
		dstr_free(&data_path);
	}

	bfree(core_bin_path);
	bfree(core_data_path);

	// if (module_bin_path && module_data_path) {
	// 	char *abs_module_bin_path = os_get_abs_path_ptr(module_bin_path);
	// 	char *abs_module_install_path = os_get_abs_path_ptr(OBS_INSTALL_PREFIX "/" OBS_PLUGIN_DESTINATION "/core");
	// 	if (abs_module_bin_path &&
	// 	    (!abs_module_install_path || strcmp(abs_module_bin_path, abs_module_install_path) != 0)) {
	// 		char *module_bin_path_full = os_get_executable_path_ptr("../" OBS_PLUGIN_PATH "/%module%");
	// 		obs_add_module_path_info(module_bin_path_full, module_data_path, CORE);
	// 		bfree(module_bin_path_full);
	// 	}
	// 	bfree(abs_module_install_path);
	// 	bfree(abs_module_bin_path);
	// }

	// bfree(module_bin_path);
	// bfree(module_data_path);

	// for (int i = 0; i < module_patterns_size; i++) {
	// 	obs_add_module_path_info(module_bin[i], module_data[i], CORE);
	// }
}