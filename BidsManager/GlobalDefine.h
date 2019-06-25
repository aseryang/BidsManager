#pragma once
#pragma execution_character_set("utf-8") //设置文件编码格式为UTF-8
#include <QString>
#include <QVector>
#define APP_VERSION "V1.2"
#define APP_TITLE "南京嘉盛景观投标管理系统 "
#define EXCEL_POS_PROJECT_NAME "B2"			//工程名称 = B2             
#define EXCEL_POS_OPENBID_DATE "G2"			//开标时间 = G2
#define EXCEL_POS_BUILD_COMPANY "B3"		//建设单位 = B3
#define EXCEL_POS_PROXY_COMPANY "B4"		//代理单位 = B4
#define EXCEL_POS_PROJECT_LOCATION "G3"		//工程地点 = G3
#define EXCEL_POS_BID_FANGSHI "B5"			//评标方式 = B5
#define EXCEL_POS_BID_BANFA "B8"			//评标办法 = B8
#define EXCEL_POS_GOODPRICE_FANGFAYI "B9"   //方法一基准值 = B9
#define EXCEL_POS_GOODPRICE_FANGFAER "B10"  //方法二基准值 = B10
#define EXCEL_POS_AVERAGE_PRICE_PERCENT "D8" //平均值占比 = D8
#define EXCEL_POS_BID_RULE "D5"				//招标规定方法 = D5
#define EXCEL_POS_BID_FINAL_RULE "I5"			//最终方法 = I5
#define EXCEL_POS_PROJECT_MANAGER_REQUIREMENT "D6"//项目经理要求 = D6
#define EXCEL_POS_PERFORMANCE_REQUIREMENT "H6"		//业绩要求 = H6
#define EXCEL_POS_K  "D9"                           //K值 = D9
#define EXCEL_POS_K1 "D10"							//K1值 = D10
#define EXCEL_POS_K2 "F10"							//K2值 = F10
#define EXCEL_POS_Q "H10"							//Q值 = H10
#define EXCEL_POS_CONTROL_PRICE "B7"				//控制价 = B7
#define EXCEL_POS_AVERGAE_PRICE "D7"					//平均报价
#define EXCEL_POS_COMMENTS "H7"						//备注 = H7
#define EXCEL_POS_DATA_KIND "H11"                   //数据归类 = H11
#define EXCEL_POS_COMPANY_INFO_START_LINE 2		//投标单位起始行 = 2

#define DEFAULT_AVERAGE_PRICE_INTERVAL "76-77,77-78,78-79,79-80,80-81,81-82,82-83,83-84"
#define SELF_COMPANY_NAME "南京嘉盛景观建设有限公司"
#define FILE_POSTFIX ".xlsx"
#define PROJECT_JINGGUAN "景观工程"
#define PROJECT_SHIZHENG "市政工程"
#define PROJECT_SHUILI   "水利工程"
#define PROJECT_ZHUAGNSHI "装饰工程"
#define PROJECT_HUANBAO   "环保工程"
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
	UI_UPDATE_LINE_CHART, //方法一，方法二，按时间分布图
	UI_UPDATE_METHOD_PIE_CHART,//方法一，方法二，概率饼状图
	UI_UPDATE_AVERAGE_PRICE_LINE_CHART, //平均值占比折线图
	UI_UPDATE_METHOD_FIRST_PIE_CHART,//方法一按，日期区间和占比区间，饼状图
	UI_UPDATE_METHOD_SECOND_PIE_CHART,//方法二按，日期区间和占比区间，饼状图
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

//图表数据结构
/*方法一，方法二，概率图数据*/
struct MethodCounts
{
	int methodFirstCount;
	int methodSecondCount;
	MethodCounts() :methodFirstCount(0), methodSecondCount(0){}
};
/*方法一，方法二，时间折线图数据*/
enum METHOD_TYPE{
	METHOD_ONE = 1,
	METHOD_TWO = 2,
};
struct  MethodDuringTime
{
	METHOD_TYPE methodType; //方法一，方法二
	QString date;
};
/*平均报价占比折线图数据*/
struct AveragePricePercent 
{
	double percent;
	QString date;
};
/*平均值占比数据又按方法一，方法二划分；方法一，方法二又按占比区间划分*/
struct AveragePriceIntervalPercent 
{
	double projectCount;     //项目数
	QString percentInterval; //占比区间
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
















