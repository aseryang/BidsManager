#pragma once
#include "GlobalDefine.h"
#include <QSettings>
class ConfigManager
{
private:
	ConfigManager();
public:
	static ConfigManager* Instance();
	~ConfigManager();
	void setProjectType(ProjectType type);
	ProjectType getProjectType(){ return projectType; }
	QString getProjectTypeStr();
	UserConfig* getCurSavedUserConfig();
	QString getCurDBName();
	void saveUserCfgFile();
	void saveAPPcfgFile();
	void loadINIFile();

private:
	QSettings *AppConfigIni;
	QSettings *UserConfigIni;
	UserConfig jingguan;
	UserConfig shizheng;
	UserConfig shuili;
	UserConfig zhuangshi;
	UserConfig huanbao;
	ProjectType projectType;
};