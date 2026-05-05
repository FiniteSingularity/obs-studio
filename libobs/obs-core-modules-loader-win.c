#include <obs-core-modules.h>
#include <util/platform.h>
#include <util/dstr.h>

#include <obs.h>
#include <obs-internal.h>

const char *core_module_bin = "../../core/%module%/%module%";
const char *core_module_data = "../../core/%module%/data";

extern bool find_core_module(struct obs_module_path *omp, obs_find_module_callback2_t callback, void *param);

void obs_core_modules_load(obs_find_module_callback2_t callback, void *param)
{
    char *core_bin_path = os_get_abs_path_ptr(core_module_bin);
    char *core_data_path = os_get_abs_path_ptr(core_module_data);

    for (unsigned int i = 0; i < obs_core_modules_count; i++) {
	const char *name = obs_core_modules[i];
	struct dstr bin_path = {0};
	struct dstr data_path = {0};
	dstr_init_copy(&bin_path, core_bin_path);
	dstr_init_copy(&data_path, core_data_path);
	dstr_replace(&bin_path, "%module%", name);
	dstr_replace(&data_path, "%module%", name);


	// Convert windows backslash to forward slash
	dstr_replace(&bin_path, "\\", "/");
	dstr_replace(&data_path, "\\", "/");

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
}
