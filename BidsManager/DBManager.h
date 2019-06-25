#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
//#include <QVector>
#include "GlobalDefine.h"
class DBManager
{
private:
	DBManager();
	void initTableHeader(QStringList& list, UserQueryCompanyType type);
public:
	static DBManager* Instance();
	int init();
	void deleteSameOldRecord(const OpenBidRecords* recordsPtr);
	DBExecuteResult deleteSingleRecord(QString projectName);
	int storeBidRecords(const OpenBidRecords* recordsPtr);
	int storeBidCompanyInfo(const QString& biddate, const QString& name, const QString& quotedPrice, const QString& proportion);
public:
	//查询方法一，方法二出现概率数据
	QVector<MethodDuringTime*>* selfQueryLineChartData();
	QVector<MethodDuringTime*>* otherQueryLineChartData();
	QVector<MethodDuringTime*>* proxyQueryLineChartData();
	MethodCounts* selfQueryPieChartData();
	MethodCounts* otherQueryPieChartData();
	MethodCounts* proxyQueryPieChartData();
	//查询投标报价平均值折线图数据
	QVector<AveragePricePercent*>* selfQueryEVALineChartData();
	QVector<AveragePricePercent*>* otherQueryEVALineChartData();
	QVector<AveragePricePercent*>* proxyQueryEVALineChartData();
	//平均报价比重，按方法一，方法二和指定区间划分占比饼状图数据
	//方法一
	QVector<AveragePriceIntervalPercent*>* selfQueryMethodOnePieChartData();
	QVector<AveragePriceIntervalPercent*>* otherQueryMethodOnePieChartData();
	QVector<AveragePriceIntervalPercent*>* proxyQueryMethodOnePieChartData();
	//方法二 
	QVector<AveragePriceIntervalPercent*>* selfQueryMethodTwoPieChartData();
	QVector<AveragePriceIntervalPercent*>* otherQueryMethodTwoPieChartData();
	QVector<AveragePriceIntervalPercent*>* proxyQueryMethodTwoPieChartData();
	/******Excel表*****/
	void getExcelIntervalPercent(int projectId, QVector<ExcelIntervalPercent*>& percentArray, int bidTotalCount);
	void getSummaryIntervalPercent(ExcelBidRecord* summaryRow, QVector<ExcelBidRecord*>& records);
	void getBidRecords(QVector<ExcelBidRecord*>& summary);
	//开标记录表数据
	ExcelBidRecordsSummary* getSelfBidRecordsSummaryData();
	//其他单位开标记录表数据
	ExcelBidRecordsSummary* getOtherBidRecordsSummaryData();
	QString getOtherCompanyBidCount(const QString& name);
	ExcelCompanySummary* getOtherCompanySummaryData();
	//代理单位开标记录表数据
	ExcelBidRecordsSummary* getProxyBidRecordsSummaryData();
	QString getProxyCompanyBidCount(const QString& name);
	ExcelCompanySummary* getProxyCompanySummaryData();
	void updateQueryCondition();
private:
	QString selfCompanyQueryConditon;
	int curPrimaryKeyID;
	QSqlQuery sql_query;
	QSqlQuery sql_query_another;
private:
	//方法一，方法二概率饼状图
	MethodCounts collectCounts;
	//方法一，方法二分布折线图
	QVector<MethodDuringTime*> collectMethodDuring;
	//平均报价占比分布折线图
	QVector<AveragePricePercent*> collectAveragePricePercent;
	//方法一平均报价占比划分饼状图
	QVector<AveragePriceIntervalPercent*> collectAVEIntervalPercentMethodOne;
	//方法二平均报价占比划分饼状图
	QVector<AveragePriceIntervalPercent*> collectAVEIntervalPercentMethodTwo;
	//开标记录表数据
	ExcelBidRecordsSummary selfBidRecordsSummary;
	//其他单位开标记录表数据
	ExcelBidRecordsSummary otherBidRecordsSummary;
	ExcelCompanySummary otherCompanySummary;
	//代理单位开标记录表数据
	ExcelBidRecordsSummary proxyBidRecordsSummary;
	ExcelCompanySummary proxyCompanySummary;
};