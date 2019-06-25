#include "Controller.h"
#include "DBManager.h"
#include "ConfigManager.h"

void Controller::run()
{
	switch (companyType)
	{
	case CompanyType_SelfCompany:
	{
		DBManager::Instance()->updateQueryCondition();
		MethodCounts* methodCountDataPtr = DBManager::Instance()->selfQueryPieChartData();
		emit updateUISignal(methodCountDataPtr, UI_UPDATE_METHOD_PIE_CHART);
		QVector<MethodDuringTime*>* methodDuringTimeDataPtr = DBManager::Instance()->selfQueryLineChartData();
		emit updateUISignal(methodDuringTimeDataPtr, UI_UPDATE_LINE_CHART);
		QVector<AveragePricePercent*>* averPricePercentPtr = DBManager::Instance()->selfQueryEVALineChartData();
		emit updateUISignal(averPricePercentPtr, UI_UPDATE_AVERAGE_PRICE_LINE_CHART);
		QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodOnePtr = DBManager::Instance()->selfQueryMethodOnePieChartData();
		emit updateUISignal(aveIntervalPercentMethodOnePtr, UI_UPDATE_METHOD_FIRST_PIE_CHART);
		QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodTwoPtr = DBManager::Instance()->selfQueryMethodTwoPieChartData();
		emit updateUISignal(aveIntervalPercentMethodTwoPtr, UI_UPDATE_METHOD_SECOND_PIE_CHART);
		//数据记录表格
		ExcelBidRecordsSummary* summaryPtr = DBManager::Instance()->getSelfBidRecordsSummaryData();
		emit updateUISignal(summaryPtr, UI_UPDATE_BIDRECORDS_SUMMARY);
		break;
	}
	case CompanyType_otherCompany:
	{
		if (ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.isEmpty())
		{
			//单位名称为空时，统计所有单位信息
			ExcelCompanySummary* companySummaryPtr = DBManager::Instance()->getOtherCompanySummaryData();
			emit updateUISignal(companySummaryPtr, UI_UPDATE_COMPANYINFO_SUMMARY);
		}
		else
		{
			MethodCounts* methodCountDataPtr = DBManager::Instance()->otherQueryPieChartData();
			emit updateUISignal(methodCountDataPtr, UI_UPDATE_METHOD_PIE_CHART);
			QVector<MethodDuringTime*>* methodDuringTimeDataPtr = DBManager::Instance()->otherQueryLineChartData();
			emit updateUISignal(methodDuringTimeDataPtr, UI_UPDATE_LINE_CHART);
			QVector<AveragePricePercent*>* averPricePercentPtr = DBManager::Instance()->otherQueryEVALineChartData();
			emit updateUISignal(averPricePercentPtr, UI_UPDATE_AVERAGE_PRICE_LINE_CHART);
			QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodOnePtr = DBManager::Instance()->otherQueryMethodOnePieChartData();
			emit updateUISignal(aveIntervalPercentMethodOnePtr, UI_UPDATE_METHOD_FIRST_PIE_CHART);
			QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodTwoPtr = DBManager::Instance()->otherQueryMethodTwoPieChartData();
			emit updateUISignal(aveIntervalPercentMethodTwoPtr, UI_UPDATE_METHOD_SECOND_PIE_CHART);
			//数据记录表格
			ExcelBidRecordsSummary* summaryPtr = DBManager::Instance()->getOtherBidRecordsSummaryData();
			emit updateUISignal(summaryPtr, UI_UPDATE_BIDRECORDS_SUMMARY);
		}

		break;
	}
	case CompanyType_ProxyCompany:
	{
		if (ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.isEmpty())
		{
			//单位名称为空时，统计所有单位信息
			ExcelCompanySummary* companySummaryPtr = DBManager::Instance()->getProxyCompanySummaryData();
			emit updateUISignal(companySummaryPtr, UI_UPDATE_COMPANYINFO_SUMMARY);
		}
		else
		{
			MethodCounts* methodCountDataPtr = DBManager::Instance()->proxyQueryPieChartData();
			emit updateUISignal(methodCountDataPtr, UI_UPDATE_METHOD_PIE_CHART);
			QVector<MethodDuringTime*>* methodDuringTimeDataPtr = DBManager::Instance()->proxyQueryLineChartData();
			emit updateUISignal(methodDuringTimeDataPtr, UI_UPDATE_LINE_CHART);
			QVector<AveragePricePercent*>* averPricePercentPtr = DBManager::Instance()->proxyQueryEVALineChartData();
			emit updateUISignal(averPricePercentPtr, UI_UPDATE_AVERAGE_PRICE_LINE_CHART);
			QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodOnePtr = DBManager::Instance()->proxyQueryMethodOnePieChartData();
			emit updateUISignal(aveIntervalPercentMethodOnePtr, UI_UPDATE_METHOD_FIRST_PIE_CHART);
			QVector<AveragePriceIntervalPercent*>* aveIntervalPercentMethodTwoPtr = DBManager::Instance()->proxyQueryMethodTwoPieChartData();
			emit updateUISignal(aveIntervalPercentMethodTwoPtr, UI_UPDATE_METHOD_SECOND_PIE_CHART);
			//数据记录表格
			ExcelBidRecordsSummary* summaryPtr = DBManager::Instance()->getProxyBidRecordsSummaryData();
			emit updateUISignal(summaryPtr, UI_UPDATE_BIDRECORDS_SUMMARY);
		}
		break;
	}
	}
	emit jobFinishSignal();
}
int Controller::queryRecords(UserQueryCompanyType type)
{
	companyType = type;
	start();
	return 0;
}