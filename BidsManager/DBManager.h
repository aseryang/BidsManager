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
	//��ѯ����һ�����������ָ�������
	QVector<MethodDuringTime*>* selfQueryLineChartData();
	QVector<MethodDuringTime*>* otherQueryLineChartData();
	QVector<MethodDuringTime*>* proxyQueryLineChartData();
	MethodCounts* selfQueryPieChartData();
	MethodCounts* otherQueryPieChartData();
	MethodCounts* proxyQueryPieChartData();
	//��ѯͶ�걨��ƽ��ֵ����ͼ����
	QVector<AveragePricePercent*>* selfQueryEVALineChartData();
	QVector<AveragePricePercent*>* otherQueryEVALineChartData();
	QVector<AveragePricePercent*>* proxyQueryEVALineChartData();
	//ƽ�����۱��أ�������һ����������ָ�����仮��ռ�ȱ�״ͼ����
	//����һ
	QVector<AveragePriceIntervalPercent*>* selfQueryMethodOnePieChartData();
	QVector<AveragePriceIntervalPercent*>* otherQueryMethodOnePieChartData();
	QVector<AveragePriceIntervalPercent*>* proxyQueryMethodOnePieChartData();
	//������ 
	QVector<AveragePriceIntervalPercent*>* selfQueryMethodTwoPieChartData();
	QVector<AveragePriceIntervalPercent*>* otherQueryMethodTwoPieChartData();
	QVector<AveragePriceIntervalPercent*>* proxyQueryMethodTwoPieChartData();
	/******Excel��*****/
	void getExcelIntervalPercent(int projectId, QVector<ExcelIntervalPercent*>& percentArray, int bidTotalCount);
	void getSummaryIntervalPercent(ExcelBidRecord* summaryRow, QVector<ExcelBidRecord*>& records);
	void getBidRecords(QVector<ExcelBidRecord*>& summary);
	//�����¼������
	ExcelBidRecordsSummary* getSelfBidRecordsSummaryData();
	//������λ�����¼������
	ExcelBidRecordsSummary* getOtherBidRecordsSummaryData();
	QString getOtherCompanyBidCount(const QString& name);
	ExcelCompanySummary* getOtherCompanySummaryData();
	//����λ�����¼������
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
	//����һ�����������ʱ�״ͼ
	MethodCounts collectCounts;
	//����һ���������ֲ�����ͼ
	QVector<MethodDuringTime*> collectMethodDuring;
	//ƽ������ռ�ȷֲ�����ͼ
	QVector<AveragePricePercent*> collectAveragePricePercent;
	//����һƽ������ռ�Ȼ��ֱ�״ͼ
	QVector<AveragePriceIntervalPercent*> collectAVEIntervalPercentMethodOne;
	//������ƽ������ռ�Ȼ��ֱ�״ͼ
	QVector<AveragePriceIntervalPercent*> collectAVEIntervalPercentMethodTwo;
	//�����¼������
	ExcelBidRecordsSummary selfBidRecordsSummary;
	//������λ�����¼������
	ExcelBidRecordsSummary otherBidRecordsSummary;
	ExcelCompanySummary otherCompanySummary;
	//����λ�����¼������
	ExcelBidRecordsSummary proxyBidRecordsSummary;
	ExcelCompanySummary proxyCompanySummary;
};