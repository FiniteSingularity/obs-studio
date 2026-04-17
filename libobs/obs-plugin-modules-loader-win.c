#include <stdlib.h>
#include "obs.h"
#include "obs-internal.h"

#include <obs-plugin-modules-loader.h>

static const char *portable_plugin_module_bin = "../../plugins/%module%";
static const char *portable_plugin_module_data = "../../plugins/%module%/data";
static const char *portable_legacy_plugin_module_bin = "../../obs-plugins/64bit";
static const char *portable_legacy_plugin_module_data = "../../data/obs-plugins/%module%";

static const char *plugin_path = "obs-studio/plugins/%module%";
static const char *data_path = "obs-studio/plugins/%module%/data";

extern void find_modules_in_path(struct obs_module_path *omp, obs_find_module_callback2_t callback, void *param);

void obs_plugins_load(struct obs_module_path *omp, obs_find_module_callback2_t callback, void *param)
{
	find_modules_in_path(omp, callback, param);
}

//void load_plugin_modules(bool portable_mode, obs_find_module_callback2_t callback, void *param)
//{
//	struct obs_module_path omp;
//	omp.module_type = PLUGIN;
//	if (portable_mode) {
//		omp.bin = bstrdup(portable_plugin_module_bin);
//		omp.data = bstrdup(portable_plugin_module_data);
//	} else {
//		char plugin_bin_path[512];
//		char plugin_data_path[512];
//		int ret = os_get_program_data_path(plugin_bin_path, sizeof(plugin_bin_path), plugin_path);
//		if (ret <= 0)
//			return;
//		ret = os_get_program_data_path(plugin_data_path, sizeof(plugin_data_path), data_path);
//		if (ret <= 0)
//			return;
//
//		omp.bin = bstrdup(plugin_bin_path);
//		omp.data = bstrdup(plugin_data_path);
//	}
//	find_modules_in_path(&omp, callback, param);
//}
//
//void load_additional_plugin_modules(obs_find_module_callback2_t callback, void *param)
//{
//	char *path = getenv("OBS_PLUGINS_PATH");
//	if (!path ||strlen(path) == 0)
//		return;
//
//	struct dstr plugins_bin_path;
//	dstr_init_copy(&plugins_bin_path, path);
//	dstr_cat(&plugins_bin_path, "/%module%");
//
//	struct dstr plugins_data_path;
//	dstr_init_copy(&plugins_data_path, path);
//	dstr_cat(&plugins_data_path, "/%module%/data");
//
//	//obs_add_module_path_info(plugins_bin_path.array, plugins_data_path.array, PLUGIN);
//	struct obs_module_path omp;
//	omp.module_type = PLUGIN;
//	omp.bin = bstrdup(plugins_bin_path.array);
//	omp.data = bstrdup(plugins_data_path.array);
//
//	find_modules_in_path(&omp, callback, param);
//
//	dstr_free(&plugins_bin_path);
//	dstr_free(&plugins_data_path);
//}
//
//void load_legacy_plugin_modules(obs_find_module_callback2_t callback, void *param)
//{
//	// Legacy plugin modules stored globally
//	char base_module_dir[512];
//	int ret = os_get_program_data_path(base_module_dir, sizeof(base_module_dir), plugin_path);
//	if (ret <= 0)
//		return;
//
//	struct dstr bin_path;
//	dstr_init_copy(&bin_path, base_module_dir);
//	dstr_cat(&bin_path, "/bin/64bit");
//
//	struct dstr data_path;
//	dstr_init_copy(&data_path, base_module_dir);
//	dstr_cat(&data_path, "/data");
//
//	struct obs_module_path omp;
//	omp.module_type = LEGACY_PLUGIN;
//	omp.bin = bstrdup(bin_path.array);
//	omp.data = bstrdup(data_path.array);
//
//	find_modules_in_path(&omp, callback, param);
//
//	dstr_free(&bin_path);
//	dstr_free(&data_path);
//
//	// Legacy plugin modules stored locally
//	// This is the same as the location for legacy portable plugins.
//	struct obs_module_path omp_local;
//	omp_local.module_type = LEGACY_PLUGIN;
//	omp_local.bin = bstrdup(portable_legacy_plugin_module_bin);
//	omp_local.data = bstrdup(portable_legacy_plugin_module_data);
//
//	find_modules_in_path(&omp_local, callback, param);
//}
//
//void load_additional_legacy_plugin_modules(obs_find_module_callback2_t callback, void *param)
//{
//	char *bin_path = getenv("OBS_LEGACY_PLUGINS_PATH");
//	char *data_path = getenv("OBS_LEGACY_PLUGINS_DATA_PATH");
//	
//	if (!bin_path || strlen(bin_path) == 0) {
//		return;
//	}
//	if (!data_path || strlen(data_path) == 0) {
//		return;
//	}
//
//	struct dstr data;
//	dstr_init_copy(&data, data_path);
//	dstr_cat(&data, "/%module%");
//
//	struct obs_module_path omp;
//	omp.module_type = LEGACY_PLUGIN;
//	omp.bin = bstrdup(bin_path);
//	omp.data = bstrdup(data.array);
//
//	find_modules_in_path(&omp, callback, param);
//	dstr_free(&data);
//}
