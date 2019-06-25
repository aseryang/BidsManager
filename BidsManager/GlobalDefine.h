#pragma once
#pragma execution_character_set("utf-8") //�����ļ������ʽΪUTF-8
#include <QString>
#include <QVector>
#define APP_VERSION "V1.2"
#define APP_TITLE "�Ͼ���ʢ����Ͷ�����ϵͳ "
#define EXCEL_POS_PROJECT_NAME "B2"			//�������� = B2             
#define EXCEL_POS_OPENBID_DATE "G2"			//����ʱ�� = G2
#define EXCEL_POS_BUILD_COMPANY "B3"		//���赥λ = B3
#define EXCEL_POS_PROXY_COMPANY "B4"		//����λ = B4
#define EXCEL_POS_PROJECT_LOCATION "G3"		//���̵ص� = G3
#define EXCEL_POS_BID_FANGSHI "B5"			//���귽ʽ = B5
#define EXCEL_POS_BID_BANFA "B8"			//����취 = B8
#define EXCEL_POS_GOODPRICE_FANGFAYI "B9"   //����һ��׼ֵ = B9
#define EXCEL_POS_GOODPRICE_FANGFAER "B10"  //��������׼ֵ = B10
#define EXCEL_POS_AVERAGE_PRICE_PERCENT "D8" //ƽ��ֵռ�� = D8
#define EXCEL_POS_BID_RULE "D5"				//�б�涨���� = D5
#define EXCEL_POS_BID_FINAL_RULE "I5"			//���շ��� = I5
#define EXCEL_POS_PROJECT_MANAGER_REQUIREMENT "D6"//��Ŀ����Ҫ�� = D6
#define EXCEL_POS_PERFORMANCE_REQUIREMENT "H6"		//ҵ��Ҫ�� = H6
#define EXCEL_POS_K  "D9"                           //Kֵ = D9
#define EXCEL_POS_K1 "D10"							//K1ֵ = D10
#define EXCEL_POS_K2 "F10"							//K2ֵ = F10
#define EXCEL_POS_Q "H10"							//Qֵ = H10
#define EXCEL_POS_CONTROL_PRICE "B7"				//���Ƽ� = B7
#define EXCEL_POS_AVERGAE_PRICE "D7"					//ƽ������
#define EXCEL_POS_COMMENTS "H7"						//��ע = H7
#define EXCEL_POS_DATA_KIND "H11"                   //���ݹ��� = H11
#define EXCEL_POS_COMPANY_INFO_START_LINE 2		//Ͷ�굥λ��ʼ�� = 2

#define DEFAULT_AVERAGE_PRICE_INTERVAL "76-77,77-78,78-79,79-80,80-81,81-82,82-83,83-84"
#define SELF_COMPANY_NAME "�Ͼ���ʢ���۽������޹�˾"
#define FILE_POSTFIX ".xlsx"
#define PROJECT_JINGGUAN "���۹���"
#define PROJECT_SHIZHENG "��������"
#define PROJECT_SHUILI   "ˮ������"
#define PROJECT_ZHUAGNSHI "װ�ι���"
#define PROJECT_HUANBAO   "��������"
#define TOTAL_INTERVAL_PIECES_COUNT 16

enum RetCode
{
	RET_OK,
	RET_FAILED,
};
struct BatchImportRunningInfo 
{
	QString runningInfo;
	QString currentFileIndex;
	QString errorInfo;
};
enum BatchImportStatus
{
	BIS_RUNNING_INFO,
	BIS_ERROR_INFO,
	BIS_FINISHED,
};
enum DBExecuteResult
{
	DB_RET_OK,
	DB_RET_RECORD_ALREADY_EXSIT,
	DB_RET_RECORD_NOT_EXSIT,
	DB_DELETE_SUCCEED,
	DB_EXECUTE_FAILED,
};
enum ProjectType{
	PROJECT_TYPE_JINGGUANG,
	PROJECT_TYPE_SHIZHENG,
	PROJECT_TYPE_SHUILI,
	PROJECT_TYPE_ZHUANGSHI,
	PROJECT_TYPE_HUANBAO,
};

enum UserQueryCompanyType{
	CompanyType_SelfCompany,
	CompanyType_otherCompany,
	CompanyType_ProxyCompany,
};

enum AppStatus{
	STAT_NORMAL,
	STAT_IMPORTING,
	STAT_EXPORTING,
	STAT_QUERYING,
};

enum UI_UpdateMsg_ID
{
	UI_UPDATE_LINE_CHART, //����һ������������ʱ��ֲ�ͼ
	UI_UPDATE_METHOD_PIE_CHART,//����һ�������������ʱ�״ͼ
	UI_UPDATE_AVERAGE_PRICE_LINE_CHART, //ƽ��ֵռ������ͼ
	UI_UPDATE_METHOD_FIRST_PIE_CHART,//����һ�������������ռ�����䣬��״ͼ
	UI_UPDATE_METHOD_SECOND_PIE_CHART,//�������������������ռ�����䣬��״ͼ
	UI_UPDATE_BIDRECORDS_SUMMARY,
	UI_UPDATE_COMPANYINFO_SUMMARY,
};
enum JobStatus
{
	JOB_FINISH,
	JOB_WARNING,
	JOB_QUESTION,
};

struct OpenBidRecords 
{
	QString projectName;
	QString openBidDate;
	QString buildCompany;
	QString proxyCompany;
	QString projectLocation;
	QString bidTotalCount;
	QString bidFangshi;
	QString bidBanfa;
	QString averagePrice;
	QString averagePricePercent;
	QString goodPrice;
	QString goodPricePercent;
	QString bidRule;
	QString bidFinalRule;
	QString projectManagerRequirement;
	QString companyPerformanceRequirement;
	QString projectManagerPerformanceRequirement;
	QString performancetext;
	QString controlPrice;
	QString K1;
	QString K2;
	QString Q;
	QString comments;
	QString dataKind;
};

//ͼ�����ݽṹ
/*����һ��������������ͼ����*/
struct MethodCounts
{
	int methodFirstCount;
	int methodSecondCount;
	MethodCounts() :methodFirstCount(0), methodSecondCount(0){}
};
/*����һ����������ʱ������ͼ����*/
enum METHOD_TYPE{
	METHOD_ONE = 1,
	METHOD_TWO = 2,
};
struct  MethodDuringTime
{
	METHOD_TYPE methodType; //����һ��������
	QString date;
};
/*ƽ������ռ������ͼ����*/
struct AveragePricePercent 
{
	double percent;
	QString date;
};
/*ƽ��ֵռ�������ְ�����һ�����������֣�����һ���������ְ�ռ�����仮��*/
struct AveragePriceIntervalPercent 
{
	double projectCount;     //��Ŀ��
	QString percentInterval; //ռ������
};
struct UserConfig 
{
	QString selfStartDate;
	QString selfEndDate;
	QString selfProjectLocation;
	QString controlPriceMin;
	QString controlPriceMax;
	QString bidWay;
	QString bidWayReal;
	QString projectManagerRequest;
	QString companyPerformance;
	QString projectManagerPerformance;
	QString baojiaqujian;
	QString AVEPriceIntervalPercentONE;
	QString AVEPriceIntervalPercentTWO;
	QString otherCompanyName;
	QString otherStartDate;
	QString otherEndDate;
	QString otherAVEPriceIntervalPercentONE;
	QString otherAVEPriceIntervalPercentTWO;
	QString proxyCompanyName;
	QString proxyStartDate;
	QString proxyEndDate;
	QString proxyAVEPriceIntervalPercentONE;
	QString proxyAVEPriceIntervalPercentTWO;
	UserConfig() :AVEPriceIntervalPercentONE(DEFAULT_AVERAGE_PRICE_INTERVAL),
		AVEPriceIntervalPercentTWO(DEFAULT_AVERAGE_PRICE_INTERVAL),
		otherAVEPriceIntervalPercentONE(DEFAULT_AVERAGE_PRICE_INTERVAL),
		otherAVEPriceIntervalPercentTWO(DEFAULT_AVERAGE_PRICE_INTERVAL),
		proxyAVEPriceIntervalPercentONE(DEFAULT_AVERAGE_PRICE_INTERVAL),
		proxyAVEPriceIntervalPercentTWO(DEFAULT_AVERAGE_PRICE_INTERVAL)
	{}
};
struct ExcelIntervalPercent 
{
	QString count;
	QString interval;
	QString percent;
};
struct ExcelBidRecord 
{
	QString serialNumber;
	QString projectName;
	QString openBidDate;
	QString controlPrice;
	QString Q;
	QString K1;
	QString K2;
	QString averagePrice;
	QString averagePricePercent;
	QString goodPrice;
	QString goodPricePercent;
	QString myPrice;
	QString myPricePercent;
	QString projectManagerRequirement;
	QString performanceRequirement;
	QString bidRule;
	QString bidFinalRule;	
	QString bidWay;
	QString location;
	QString bidCounts;
	QVector<ExcelIntervalPercent*> bidInterval;
};
struct ExcelBidRecordsSummary
{
	QStringList tableHeader;
	QString title;
	QVector<ExcelBidRecord*> MethodOneRecords;
	QVector<ExcelBidRecord*> MethodTwoRecords;
	QVector<ExcelBidRecord*> DataFakeRecords;
};
struct ExcelCompanyInfo 
{
	QString serialNum;
	QString name;
	QString bidCount;
	QString comment;
};
struct ExcelCompanySummary
{
	QString title;
	QStringList tableHeader;
	QVector<ExcelCompanyInfo*> companyArray;
};

enum IntervalType
{
	TNTERVAL_BAOJIA,
	INTERVAL_METHODONE,
	TNTERVAL_METHODTWO,
};
















