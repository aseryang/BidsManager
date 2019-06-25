#include "ConfigManager.h"

ConfigManager* ConfigManager::Instance()
{
	static ConfigManager* ptr = new ConfigManager;
	return ptr;
}
ConfigManager::ConfigManager() :projectType(PROJECT_TYPE_JINGGUANG)
{
	AppConfigIni = new QSettings("AppConfig.ini", QSettings::IniFormat);
	AppConfigIni->setIniCodec("UTF8");
	UserConfigIni = new QSettings("UserConfig.ini", QSettings::IniFormat);
	UserConfigIni->setIniCodec("UTF8");
	projectType = (ProjectType)UserConfigIni->value("/CurrentProjectType/Type").toInt();
}
ConfigManager::~ConfigManager()
{
	delete AppConfigIni;
	delete UserConfigIni;
}
QString ConfigManager::getCurDBName()
{
	QString name;
	switch (projectType)
	{
	case PROJECT_TYPE_JINGGUANG:
		name = "jingguan.db";
		break;
	case PROJECT_TYPE_SHIZHENG:
		name = "shizheng.db";
		break;
	case PROJECT_TYPE_SHUILI:
		name = "shuili.db";
		break;
	case PROJECT_TYPE_ZHUANGSHI:
		name = "zhuangshi.db";
		break;
	case PROJECT_TYPE_HUANBAO:
		name = "huanbao.db";
		break;
	}
	return name;
}
QString ConfigManager::getProjectTypeStr()
{
	QString name;
	switch (projectType)
	{
	case PROJECT_TYPE_JINGGUANG:
		name = PROJECT_JINGGUAN;
		break;
	case PROJECT_TYPE_SHIZHENG:
		name = PROJECT_SHIZHENG;
		break;
	case PROJECT_TYPE_SHUILI:
		name = PROJECT_SHUILI;
		break;
	case PROJECT_TYPE_ZHUANGSHI:
		name = PROJECT_ZHUAGNSHI;
		break;
	case PROJECT_TYPE_HUANBAO:
		name = PROJECT_HUANBAO;
		break;
	}
	return name;
}
UserConfig* ConfigManager::getCurSavedUserConfig()
{
	UserConfig* ptr = NULL;
	switch (projectType)
	{
	case PROJECT_TYPE_JINGGUANG:
		ptr = &jingguan;
		break;
	case PROJECT_TYPE_SHIZHENG:
		ptr = &shizheng;
		break;
	case PROJECT_TYPE_SHUILI:
		ptr = &shuili;
		break;
	case PROJECT_TYPE_ZHUANGSHI:
		ptr = &zhuangshi;
		break;
	case PROJECT_TYPE_HUANBAO:
		ptr = &huanbao;
		break;
	}
	return ptr;
}
void ConfigManager::saveUserCfgFile()
{
	if (PROJECT_TYPE_JINGGUANG == projectType)
	{
		QString jingguancfg;
		jingguancfg += jingguan.selfStartDate;
		jingguancfg.append("|");
		jingguancfg += jingguan.selfEndDate;
		jingguancfg.append("|");
		jingguancfg += jingguan.selfProjectLocation;
		jingguancfg.append("|");
		jingguancfg += jingguan.controlPriceMin;
		jingguancfg.append("|");
		jingguancfg += jingguan.controlPriceMax;
		jingguancfg.append("|");
		jingguancfg += jingguan.bidWay;
		jingguancfg.append("|");
		jingguancfg += jingguan.bidWayReal;
		jingguancfg.append("|");
		jingguancfg += jingguan.projectManagerRequest;
		jingguancfg.append("|");
		jingguancfg += jingguan.companyPerformance;
		jingguancfg.append("|");
		jingguancfg += jingguan.projectManagerPerformance;
		jingguancfg.append("|");
		jingguancfg += jingguan.baojiaqujian;
		jingguancfg.append("|");
		jingguancfg += jingguan.AVEPriceIntervalPercentONE;
		jingguancfg.append("|");
		jingguancfg += jingguan.AVEPriceIntervalPercentTWO;
		jingguancfg.append("|");
		jingguancfg += jingguan.otherCompanyName;
		jingguancfg.append("|");
		jingguancfg += jingguan.otherStartDate;
		jingguancfg.append("|");
		jingguancfg += jingguan.otherEndDate;
		jingguancfg.append("|");
		jingguancfg += jingguan.proxyCompanyName;
		jingguancfg.append("|");
		jingguancfg += jingguan.proxyStartDate;
		jingguancfg.append("|");
		jingguancfg += jingguan.proxyEndDate;
		UserConfigIni->setValue("/Jingguan/UserConfig", jingguancfg);
	} 
	else if (PROJECT_TYPE_SHIZHENG == projectType)
	{
		QString shizhengcfg;
		shizhengcfg += shizheng.selfStartDate;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.selfEndDate;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.selfProjectLocation;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.controlPriceMin;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.controlPriceMax;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.bidWay;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.bidWayReal;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.projectManagerRequest;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.companyPerformance;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.projectManagerPerformance;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.baojiaqujian;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.AVEPriceIntervalPercentONE;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.AVEPriceIntervalPercentTWO;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.otherCompanyName;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.otherStartDate;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.otherEndDate;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.proxyCompanyName;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.proxyStartDate;
		shizhengcfg.append("|");
		shizhengcfg += shizheng.proxyEndDate;
		UserConfigIni->setValue("/Shizheng/UserConfig", shizhengcfg);
	}
	else if (PROJECT_TYPE_SHUILI == projectType)
	{
		QString shuilicfg;
		shuilicfg += shuili.selfStartDate;
		shuilicfg.append("|");
		shuilicfg += shuili.selfEndDate;
		shuilicfg.append("|");
		shuilicfg += shuili.selfProjectLocation;
		shuilicfg.append("|");
		shuilicfg += shuili.controlPriceMin;
		shuilicfg.append("|");
		shuilicfg += shuili.controlPriceMax;
		shuilicfg.append("|");
		shuilicfg += shuili.bidWay;
		shuilicfg.append("|");
		shuilicfg += shuili.bidWayReal;
		shuilicfg.append("|");
		shuilicfg += shuili.projectManagerRequest;
		shuilicfg.append("|");
		shuilicfg += shuili.companyPerformance;
		shuilicfg.append("|");
		shuilicfg += shuili.projectManagerPerformance;
		shuilicfg.append("|");
		shuilicfg += shuili.baojiaqujian;
		shuilicfg.append("|");
		shuilicfg += shuili.AVEPriceIntervalPercentONE;
		shuilicfg.append("|");
		shuilicfg += shuili.AVEPriceIntervalPercentTWO;
		shuilicfg.append("|");
		shuilicfg += shuili.otherCompanyName;
		shuilicfg.append("|");
		shuilicfg += shuili.otherStartDate;
		shuilicfg.append("|");
		shuilicfg += shuili.otherEndDate;
		shuilicfg.append("|");
		shuilicfg += shuili.proxyCompanyName;
		shuilicfg.append("|");
		shuilicfg += shuili.proxyStartDate;
		shuilicfg.append("|");
		shuilicfg += shuili.proxyEndDate;
		UserConfigIni->setValue("/Shuili/UserConfig", shuilicfg);
	}
	else if (PROJECT_TYPE_ZHUANGSHI == projectType)
	{
		QString zhuangshicfg;
		zhuangshicfg += zhuangshi.selfStartDate;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.selfEndDate;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.selfProjectLocation;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.controlPriceMin;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.controlPriceMax;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.bidWay;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.bidWayReal;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.projectManagerRequest;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.companyPerformance;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.projectManagerPerformance;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.baojiaqujian;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.AVEPriceIntervalPercentONE;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.AVEPriceIntervalPercentTWO;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.otherCompanyName;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.otherStartDate;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.otherEndDate;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.proxyCompanyName;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.proxyStartDate;
		zhuangshicfg.append("|");
		zhuangshicfg += zhuangshi.proxyEndDate;
		UserConfigIni->setValue("/Zhuangshi/UserConfig", zhuangshicfg);
	}
	else if (PROJECT_TYPE_HUANBAO == projectType)
	{
		QString huanbaocfg;
		huanbaocfg += huanbao.selfStartDate;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.selfEndDate;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.selfProjectLocation;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.controlPriceMin;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.controlPriceMax;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.bidWay;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.bidWayReal;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.projectManagerRequest;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.companyPerformance;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.projectManagerPerformance;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.baojiaqujian;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.AVEPriceIntervalPercentONE;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.AVEPriceIntervalPercentTWO;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.otherCompanyName;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.otherStartDate;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.otherEndDate;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.proxyCompanyName;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.proxyStartDate;
		huanbaocfg.append("|");
		huanbaocfg += huanbao.proxyEndDate;
		UserConfigIni->setValue("/Huanbao/UserConfig", huanbaocfg);
	}
	UserConfigIni->setValue("/CurrentProjectType/Type", projectType);
	UserConfigIni->sync();
}
void ConfigManager::setProjectType(ProjectType type)
{ 
	projectType = type; 
	UserConfigIni->setValue("/CurrentProjectType/Type", projectType);
	UserConfigIni->sync();
}
void ConfigManager::saveAPPcfgFile()
{
}
void ConfigManager::loadINIFile()
{
	const int fieldNums = 19;
	if (PROJECT_TYPE_JINGGUANG == projectType)
	{
		QString jingguanCfg = UserConfigIni->value("/Jingguan/UserConfig").toString();
		if (!jingguanCfg.isEmpty())
		{
			QStringList list = jingguanCfg.split('|');
			if (list.size() == fieldNums)
			{
				jingguan.selfStartDate = list.at(0);
				jingguan.selfEndDate = list.at(1);
				jingguan.selfProjectLocation = list.at(2);
				jingguan.controlPriceMin = list.at(3);
				jingguan.controlPriceMax = list.at(4);
				jingguan.bidWay = list.at(5);
				jingguan.bidWayReal = list.at(6);
				jingguan.projectManagerRequest = list.at(7);
				jingguan.companyPerformance = list.at(8);
				jingguan.projectManagerPerformance = list.at(9);
				jingguan.baojiaqujian = list.at(10);
				jingguan.AVEPriceIntervalPercentONE = list.at(11);
				jingguan.AVEPriceIntervalPercentTWO = list.at(12);
				jingguan.otherCompanyName = list.at(13);
				jingguan.otherStartDate = list.at(14);
				jingguan.otherEndDate = list.at(15);
				jingguan.otherAVEPriceIntervalPercentONE = list.at(11);
				jingguan.otherAVEPriceIntervalPercentTWO = list.at(12);
				jingguan.proxyCompanyName = list.at(16);
				jingguan.proxyStartDate = list.at(17);
				jingguan.proxyEndDate = list.at(18);
				jingguan.proxyAVEPriceIntervalPercentONE = list.at(11);
				jingguan.proxyAVEPriceIntervalPercentTWO = list.at(12);
			}
		}
	}
	else if (PROJECT_TYPE_SHIZHENG == projectType)
	{
		QString ShizhengCfg = UserConfigIni->value("/Shizheng/UserConfig").toString();
		if (!ShizhengCfg.isEmpty())
		{
			QStringList list = ShizhengCfg.split('|');
			if (list.size() == fieldNums)
			{
				shizheng.selfStartDate = list.at(0);
				shizheng.selfEndDate = list.at(1);
				shizheng.selfProjectLocation = list.at(2);
				shizheng.controlPriceMin = list.at(3);
				shizheng.controlPriceMax = list.at(4);
				shizheng.bidWay = list.at(5);
				shizheng.bidWayReal = list.at(6);
				shizheng.projectManagerRequest = list.at(7);
				shizheng.companyPerformance = list.at(8);
				shizheng.projectManagerPerformance = list.at(9);
				shizheng.baojiaqujian = list.at(10);
				shizheng.AVEPriceIntervalPercentONE = list.at(11);
				shizheng.AVEPriceIntervalPercentTWO = list.at(12);
				shizheng.otherCompanyName = list.at(13);
				shizheng.otherStartDate = list.at(14);
				shizheng.otherEndDate = list.at(15);
				shizheng.otherAVEPriceIntervalPercentONE = list.at(11);
				shizheng.otherAVEPriceIntervalPercentTWO = list.at(12);
				shizheng.proxyCompanyName = list.at(16);
				shizheng.proxyStartDate = list.at(17);
				shizheng.proxyEndDate = list.at(18);
				shizheng.proxyAVEPriceIntervalPercentONE = list.at(11);
				shizheng.proxyAVEPriceIntervalPercentTWO = list.at(12);
			}
		}
	}
	else if (PROJECT_TYPE_SHUILI == projectType)
	{
		QString ShuiliCfg = UserConfigIni->value("/Shuili/UserConfig").toString();
		if (!ShuiliCfg.isEmpty())
		{
			QStringList list = ShuiliCfg.split('|');
			if (list.size() == fieldNums)
			{
				shuili.selfStartDate = list.at(0);
				shuili.selfEndDate = list.at(1);
				shuili.selfProjectLocation = list.at(2);
				shuili.controlPriceMin = list.at(3);
				shuili.controlPriceMax = list.at(4);
				shuili.bidWay = list.at(5);
				shuili.bidWayReal = list.at(6);
				shuili.projectManagerRequest = list.at(7);
				shuili.companyPerformance = list.at(8);
				shuili.projectManagerPerformance = list.at(9);
				shuili.baojiaqujian = list.at(10);
				shuili.AVEPriceIntervalPercentONE = list.at(11);
				shuili.AVEPriceIntervalPercentTWO = list.at(12);
				shuili.otherCompanyName = list.at(13);
				shuili.otherStartDate = list.at(14);
				shuili.otherEndDate = list.at(15);
				shuili.otherAVEPriceIntervalPercentONE = list.at(11);
				shuili.otherAVEPriceIntervalPercentTWO = list.at(12);
				shuili.proxyCompanyName = list.at(16);
				shuili.proxyStartDate = list.at(17);
				shuili.proxyEndDate = list.at(18);
				shuili.proxyAVEPriceIntervalPercentONE = list.at(11);
				shuili.proxyAVEPriceIntervalPercentTWO = list.at(12);
			}
		}
	}
	else if (PROJECT_TYPE_ZHUANGSHI == projectType)
	{
		QString ZhuangshiCfg = UserConfigIni->value("/Zhuangshi/UserConfig").toString();
		if (!ZhuangshiCfg.isEmpty())
		{
			QStringList list = ZhuangshiCfg.split('|');
			if (list.size() == fieldNums)
			{
				zhuangshi.selfStartDate = list.at(0);
				zhuangshi.selfEndDate = list.at(1);
				zhuangshi.selfProjectLocation = list.at(2);
				zhuangshi.controlPriceMin = list.at(3);
				zhuangshi.controlPriceMax = list.at(4);
				zhuangshi.bidWay = list.at(5);
				zhuangshi.bidWayReal = list.at(6);
				zhuangshi.projectManagerRequest = list.at(7);
				zhuangshi.companyPerformance = list.at(8);
				zhuangshi.projectManagerPerformance = list.at(9);
				zhuangshi.baojiaqujian = list.at(10);
				zhuangshi.AVEPriceIntervalPercentONE = list.at(11);
				zhuangshi.AVEPriceIntervalPercentTWO = list.at(12);
				zhuangshi.otherCompanyName = list.at(13);
				zhuangshi.otherStartDate = list.at(14);
				zhuangshi.otherEndDate = list.at(15);
				zhuangshi.otherAVEPriceIntervalPercentONE = list.at(11);
				zhuangshi.otherAVEPriceIntervalPercentTWO = list.at(12);
				zhuangshi.proxyCompanyName = list.at(16);
				zhuangshi.proxyStartDate = list.at(17);
				zhuangshi.proxyEndDate = list.at(18);
				zhuangshi.proxyAVEPriceIntervalPercentONE = list.at(11);
				zhuangshi.proxyAVEPriceIntervalPercentTWO = list.at(12);
			}
		}
	}
	else if (PROJECT_TYPE_HUANBAO == projectType)
	{
		QString HuanBaoCfg = UserConfigIni->value("/HuanBao/UserConfig").toString();
		if (!HuanBaoCfg.isEmpty())
		{
			QStringList list = HuanBaoCfg.split('|');
			if (list.size() == fieldNums)
			{
				huanbao.selfStartDate = list.at(0);
				huanbao.selfEndDate = list.at(1);
				huanbao.selfProjectLocation = list.at(2);
				huanbao.controlPriceMin = list.at(3);
				huanbao.controlPriceMax = list.at(4);
				huanbao.bidWay = list.at(5);
				huanbao.bidWayReal = list.at(6);
				huanbao.projectManagerRequest = list.at(7);
				huanbao.companyPerformance = list.at(8);
				huanbao.projectManagerPerformance = list.at(9);
				huanbao.baojiaqujian = list.at(10);
				huanbao.AVEPriceIntervalPercentONE = list.at(11);
				huanbao.AVEPriceIntervalPercentTWO = list.at(12);
				huanbao.otherCompanyName = list.at(13);
				huanbao.otherStartDate = list.at(14);
				huanbao.otherEndDate = list.at(15);
				huanbao.otherAVEPriceIntervalPercentONE = list.at(11);
				huanbao.otherAVEPriceIntervalPercentTWO = list.at(12);
				huanbao.proxyCompanyName = list.at(16);
				huanbao.proxyStartDate = list.at(17);
				huanbao.proxyEndDate = list.at(18);
				huanbao.proxyAVEPriceIntervalPercentONE = list.at(11);
				huanbao.proxyAVEPriceIntervalPercentTWO = list.at(12);
			}
		}
	}	
}