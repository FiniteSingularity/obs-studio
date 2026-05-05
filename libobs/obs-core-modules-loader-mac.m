#include <Carbon/Carbon.h>
#include <obs-core-modules.h>

#include <util/platform.h>
#include <util/dstr.h>

#include <obs.h>
#include <obs-internal.h>

extern void find_modules_in_path(struct obs_module_path *omp, obs_find_module_callback2_t callback, void *param);

void obs_core_modules_load(obs_find_module_callback2_t callback, void *param)
{
	NSURL *pluginURL = [[NSBundle mainBundle] builtInPlugInsURL];
	NSString *pluginBasePath = [pluginURL path];

	for (unsigned int i = 0; i < obs_core_modules_count; i++) {
		const char *name = obs_core_modules[i];
		NSString *moduleName = [NSString stringWithUTF8String:name];

		NSString *binPath = [NSString stringWithFormat:
				     @"%@/%@.plugin/Contents/MacOS/%@", pluginBasePath, moduleName, moduleName];

		NSString *dataPath = [NSString stringWithFormat:
				      @"%@/%@.plugin/Contents/Resources/", pluginBasePath, moduleName];

		if (![[NSFileManager defaultManager] fileExistsAtPath:binPath]) {
			blog(LOG_ERROR, "Core Module %s required but missing!", name);
			continue;
		}

		struct obs_module_path omp;
		omp.bin = bstrdup([binPath UTF8String]);
		omp.data = bstrdup([dataPath UTF8String]);
		omp.module_type = CORE;

		if (!find_core_module(&omp, callback, param)) {
			blog(LOG_ERROR, "Failed to load core module %s", name);
		}

		bfree(omp.bin);
		bfree(omp.data);
	}
}
