#pragma once
#include "ui_OBSPluginManager.h"

#include <QDialog>
#include <QWidget>
#include <obs-module.h>

#include "../widgets/OBSBasic.hpp"

class OBSBasic;

struct OBSModuleInfo {
	std::string display_name;
	std::string module_name;
	std::string id;
	std::string version;
	bool enabled;
	bool enabledAtLaunch;
	std::vector<std::string> sources;
	std::vector<std::string> outputs;
	std::vector<std::string> encoders;
	std::vector<std::string> services;
	std::vector<std::string> sourcesLoaded;
	std::vector<std::string> outputsLoaded;
	std::vector<std::string> encodersLoaded;
	std::vector<std::string> servicesLoaded;
};

class OBSPluginManager : public QDialog {
	Q_OBJECT
	std::unique_ptr<Ui::OBSPluginManager> ui;

public:
	explicit OBSPluginManager(std::vector<OBSModuleInfo> const &modules, QWidget *parent = nullptr);
	inline std::vector<OBSModuleInfo> const result() { return _modules; }

private:
	std::vector<OBSModuleInfo> _modules;
};

class OBSPluginManagerController {
private:
	std::vector<OBSModuleInfo> pmModules = {};
	std::vector<std::string> pmDisabledSources = {};
	std::vector<std::string> pmDisabledOutputs = {};
	std::vector<std::string> pmDisabledServices = {};
	std::vector<std::string> pmDisabledEncoders = {};
	std::string PMModulesPath();
	void PMLoadModules();
	void PMSaveModules();
	void PMDisableModules();
	void PMAddModuleTypes();
	static void PMAddNewModule(void *param, obs_module_t *newModule);

public:
	bool PMSourceDisabled(obs_source_t *source) const;
	bool PMOutputDisabled(obs_output_t *output) const;
	bool PMEncoderDisabled(obs_encoder_t *encoder) const;
	bool PMServiceDisabled(obs_service_t *service) const;
	friend OBSBasic;
	void OpenPMDialog();
};
