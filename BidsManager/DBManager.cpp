#include "DBManager.h"
#include "ConfigManager.h"
#include <QSqlRecord>
#include <QDebug>

DBManager* DBManager::Instance()
{
	static DBManager* ptr = new DBManager;
	return ptr;
}
DBManager::DBManager() :curPrimaryKeyID(0)
{
	selfBidRecordsSummary.title.sprintf("开标记录汇总表");
	otherBidRecordsSummary.title.sprintf("XX单位开标记录表");
	proxyBidRecordsSummary.title.sprintf("XX招标代理记录表");
	otherCompanySummary.title = "施工单位汇总表";
	otherCompanySummary.tableHeader.push_back("序号");
	otherCompanySummary.tableHeader.push_back("施工单位");
	otherCompanySummary.tableHeader.push_back("投标个数");
	otherCompanySummary.tableHeader.push_back("备注");
	proxyCompanySummary.title = "招标代理汇总表";
	proxyCompanySummary.tableHeader.push_back("序号");
	proxyCompanySummary.tableHeader.push_back("招标代理单位");
	proxyCompanySummary.tableHeader.push_back("投标个数");
	proxyCompanySummary.tableHeader.push_back("备注");
	initTableHeader(selfBidRecordsSummary.tableHeader, CompanyType_SelfCompany);
	initTableHeader(otherBidRecordsSummary.tableHeader, CompanyType_otherCompany);
	initTableHeader(proxyBidRecordsSummary.tableHeader, CompanyType_ProxyCompany);
}
void DBManager::initTableHeader(QStringList& list, UserQueryCompanyType type)
{
	list.push_back("序号");
	list.push_back("项目名称");
	list.push_back("开标日期");
	list.push_back("控制价（万元）");
	list.push_back("Q1");
	list.push_back("K1");
	list.push_back("K2");
	list.push_back("平均报价");
	list.push_back("报价比重");
	list.push_back("最优报价");
	list.push_back("报价比重");
	if (CompanyType_otherCompany == type)
	{
		list.push_back("本单位报价");
		list.push_back("报价比重");
	}
	list.push_back("项目经理");
	list.push_back("业绩要求");
	list.push_back("招标规定方法");
	list.push_back("实际方法");
	list.push_back("评标方式");
	list.push_back("工程地点");
	list.push_back("投标数量");
	if (CompanyType_SelfCompany == type)
	{
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
		list.push_back("数量");
		list.push_back("占比");
	}
}
void DBManager::deleteSameOldRecord(const OpenBidRecords* recordsPtr)
{
	QString deleteSql;
	deleteSql.sprintf("delete from OpenBidRecords where project_name = '%s'", recordsPtr->projectName.toUtf8().data());
	sql_query.prepare(deleteSql);
	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
	}
	else
	{
		qDebug() << "delete same old record succeed!";
	}
}
DBExecuteResult DBManager::deleteSingleRecord(QString projectName)
{
	QString sql;
	sql.sprintf("select count(*) from OpenBidRecords where project_name = '%s'", projectName.toUtf8().data());
	sql_query.prepare(sql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			if (sql_query.value(0) <= 0)
			{
				return DB_RET_RECORD_NOT_EXSIT;
			}
		}
	}
	else
	{
		qDebug() << sql_query.lastError();
		return DB_EXECUTE_FAILED;
	}
	
	QString deleteSql;
	deleteSql.sprintf("delete from OpenBidRecords where project_name = '%s'", projectName.toUtf8().data());
	sql_query.prepare(deleteSql);
	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
		return DB_EXECUTE_FAILED;
	}
	else
	{
		qDebug() << "delete succeed!";
		return DB_RET_OK;
	}

}
int DBManager::storeBidRecords(const OpenBidRecords* record)
{
	deleteSameOldRecord(record);
	QString insert_sql = "insert into OpenBidRecords values (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
	sql_query.prepare(insert_sql);
	sql_query.addBindValue(record->projectName);
	sql_query.addBindValue(record->openBidDate);
	sql_query.addBindValue(record->buildCompany);
	sql_query.addBindValue(record->proxyCompany);
	sql_query.addBindValue(record->projectLocation);
	sql_query.addBindValue(record->bidTotalCount);
	sql_query.addBindValue(record->bidFangshi);
	sql_query.addBindValue(record->bidBanfa);
	sql_query.addBindValue(record->averagePrice);
	sql_query.addBindValue(record->averagePricePercent);
	sql_query.addBindValue(record->goodPrice);
	sql_query.addBindValue(record->goodPricePercent);
	sql_query.addBindValue(record->bidRule);
	sql_query.addBindValue(record->bidFinalRule);
	sql_query.addBindValue(record->projectManagerRequirement);
	sql_query.addBindValue(record->companyPerformanceRequirement);
	sql_query.addBindValue(record->projectManagerPerformanceRequirement);
	sql_query.addBindValue(record->performancetext);
	sql_query.addBindValue(record->controlPrice);
	sql_query.addBindValue(record->K1);
	sql_query.addBindValue(record->K2);
	sql_query.addBindValue(record->Q);
	sql_query.addBindValue(record->comments);
	sql_query.addBindValue(record->dataKind);
	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
		return 1;
	}
	else
	{
		qDebug() << "inserted succeed!";
	}

	QString getLastPrimaryKeyIdSql = "select last_insert_rowid()";
	sql_query.prepare(getLastPrimaryKeyIdSql);
	if (!sql_query.exec(getLastPrimaryKeyIdSql))
	{
		qDebug() << sql_query.lastError();
	}
	else
	{
		while (sql_query.next())
		{
			curPrimaryKeyID = sql_query.value(0).toInt();
			//break;
		}
	}

	return 0;
}
int DBManager::storeBidCompanyInfo(const QString& biddate, const QString& name, const QString& quotedPrice, const QString& proportion)
{
	QString insert_sql = "insert into BidCompanyInfo values (?, ?, ?, ?, ?)";
	sql_query.prepare(insert_sql);
	sql_query.addBindValue(biddate);
	sql_query.addBindValue(name);
	sql_query.addBindValue(curPrimaryKeyID);
	sql_query.addBindValue(quotedPrice);
	sql_query.addBindValue(proportion);

	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
		return 1;
	}
	else
	{
		qDebug() << "inserted succeed!";
	}
	return 0;
}

int DBManager::init()
{
	//设置数据库驱动名称
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	//设置数据库名
	database.setDatabaseName(ConfigManager::Instance()->getCurDBName());
	//看是否能正确打开
	if (!database.open())
	{
		qDebug()<<database.lastError().text();
		return 1;
	} 
	sql_query = QSqlQuery(ConfigManager::Instance()->getCurDBName());
	sql_query_another = QSqlQuery(ConfigManager::Instance()->getCurDBName());

	QString setforeignkeyOnSql("PRAGMA foreign_keys = ON;");
	sql_query.prepare(setforeignkeyOnSql);
	if (!sql_query.exec())
	{
		qDebug() << "Set foreign_keys ON failed!" << sql_query.lastError();
	}
	else
	{
		qDebug() << "foreign_keys is ON!";
	}

	QString create_bid_records_table_sql = "create table OpenBidRecords(\
		project_id INTEGER PRIMARY KEY,\
		project_name VARCHAR(20) NOT NULL,\
		openbid_date DATE,\
		build_company VARCHAR(10),\
		proxy_company VARCHAR(10),\
		project_location VARCHAR(10),\
		bid_totalCount VARCHAR(10),\
		bid_fangshi VARCHAR(10),\
		bid_banfa VARCHAR(10),\
		bid_averagePrice VARCHAR(10),\
		bid_averagePricePercent VARCHAR(10),\
		bid_goodPrice VARCHAR(10),\
		bid_goodPricePercent VARCHAR(10),\
		bid_rule VARCHAR(10),\
		bid_final_rule VARCHAR(10),\
		project_manager_requirement VARCHAR(10),\
		companyPerformance_requirement DOUBLE(10),\
		ProjectManagerPerformance_requirement DOUBLE(10),\
		PerformanceText VARCHAR(10),\
		control_price DOUBLE(10),\
		K1 VARCHAR(10),\
		K2 VARCHAR(10),\
		Q VARCHAR(10),\
		comments VARCHAR(20),\
		data_kind VARCHAR(10))";
	sql_query.prepare(create_bid_records_table_sql);
	if (!sql_query.exec())
	{
		qDebug() << "Error: Fail to create table." << sql_query.lastError();
	}
	else
	{
		qDebug() << "Table created!";
	}
	//ON DELETE CASCADE级联删除 
	QString creat_bid_companys_info_table_sql = "create table BidCompanyInfo(\
		bid_date DATE,\
		company_name VARCHAR(10) NOT NULL,\
		project_id INTEGER REFERENCES  OpenBidRecords(project_id)  ON DELETE CASCADE,\
		quoted_price VARCHAR(10) NOT NULL,\
		proportion VARCHAR(10) NOT NULL)";
	sql_query.prepare(creat_bid_companys_info_table_sql);
	if (!sql_query.exec())
	{
		qDebug() << "Error: Fail to create table." << sql_query.lastError();
	}
	else
	{
		qDebug() << "Table created!";
	}
	return 0;
}
void DBManager::updateQueryCondition()
{
	selfCompanyQueryConditon.sprintf(" openbid_date >= '%s' and openbid_date <= '%s'",
									ConfigManager::Instance()->getCurSavedUserConfig()->selfStartDate.toUtf8().data(),
									ConfigManager::Instance()->getCurSavedUserConfig()->selfEndDate.toUtf8().data());
	//工程地点
	QString location = ConfigManager::Instance()->getCurSavedUserConfig()->selfProjectLocation;
	if (!location.isEmpty())
	{
		//AND (instr(project_location, '南京市区')> 0 or instr(project_location, '江宁') > 0)
		QStringList list = location.split(',');
		QString condition(" AND (");
		for (int i = 0; i < list.size(); ++i)
		{
			QString place = list.at(i);
			if (!place.isEmpty())
			{
				QString childSql;
				childSql.sprintf("instr(project_location, '%s')> 0", place.toUtf8().data());
				condition.append(childSql);
				if (i < list.size() - 1 && !list.at(i + 1).isEmpty())
				{
					condition.append(" or ");
				}
			}
		}
		condition.append(")");
		selfCompanyQueryConditon.append(condition);
	}
	//控制价
	QString minPrice = ConfigManager::Instance()->getCurSavedUserConfig()->controlPriceMin;
	QString maxPrice = ConfigManager::Instance()->getCurSavedUserConfig()->controlPriceMax;
	if (!minPrice.isEmpty() && !maxPrice.isEmpty())
	{
		//AND control_price between '1111' AND '2222'
		QString controlPriceSql;
		controlPriceSql.sprintf(" AND control_price between '%s' and '%s'", minPrice.toUtf8().data(), maxPrice.toUtf8().data());
		selfCompanyQueryConditon.append(controlPriceSql);
	}
	//评标方法
	QString bidway = ConfigManager::Instance()->getCurSavedUserConfig()->bidWay;
	QString bidwayReal = ConfigManager::Instance()->getCurSavedUserConfig()->bidWayReal;
	if (!bidway.isEmpty())
	{
		QString waySql;
		waySql.sprintf(" AND bid_rule = '%s'", bidway.toUtf8().data());
		selfCompanyQueryConditon.append(waySql);		
	}
	if (!bidwayReal.isEmpty())
	{
		QString wayRealSql;
		wayRealSql.sprintf(" AND bid_final_rule = '%s'", bidwayReal.toUtf8().data());
		selfCompanyQueryConditon.append(wayRealSql);
	}
	//项目经理资质要求
	QString managerReq = ConfigManager::Instance()->getCurSavedUserConfig()->projectManagerRequest;
	if (!managerReq.isEmpty())
	{
		QString mgrSql;
		mgrSql.sprintf(" AND project_manager_requirement LIKE '%%%s%%'", managerReq.toUtf8().data());
		selfCompanyQueryConditon.append(mgrSql);
	}
	//业绩要求
	QString perforMgr = ConfigManager::Instance()->getCurSavedUserConfig()->projectManagerPerformance;
	QString perforCompany = ConfigManager::Instance()->getCurSavedUserConfig()->companyPerformance;
	if (!perforMgr.isEmpty())
	{
		//AND ProjectManagerPerformance_requirement between '1111' AND '2222'
		QStringList list = perforMgr.split('-');
		QString perfroMgrReqSql;
		perfroMgrReqSql.sprintf(" AND ProjectManagerPerformance_requirement between '%s' AND '%s'", list.at(0).toUtf8().data(), list.at(1).toUtf8().data());
		selfCompanyQueryConditon.append(perfroMgrReqSql);
	}
	if (!perforCompany.isEmpty())
	{
		//AND companyPerformance_requirement between '1111' AND '2222'
		QStringList list = perforCompany.split('-');
		QString companyPerReqSql;
		companyPerReqSql.sprintf(" AND companyPerformance_requirement between '%s' AND '%s'", list.at(0).toUtf8().data(), list.at(1).toUtf8().data());
		selfCompanyQueryConditon.append(companyPerReqSql);
	}

	//升序排序
	selfCompanyQueryConditon.append(" order by openbid_date");
}
QVector<MethodDuringTime*>* DBManager::selfQueryLineChartData()
{
	//清理
	for (int i = 0; i < collectMethodDuring.size(); ++i)
	{
		delete collectMethodDuring.at(i);
	}
	collectMethodDuring.clear();
	//查询以时间升序排序的数据
	QString querySql;
	querySql.sprintf("select openbid_date, bid_banfa from OpenBidRecords where %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(querySql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			if (sql_query.value(1).compare("方法一") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_ONE;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else if (sql_query.value(1).compare("方法二") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_TWO;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else
			{
				continue;;
			}
		}
	}
	else
	{
		qDebug() << sql_query.lastError();
	}
	return &collectMethodDuring;
}
QVector<MethodDuringTime*>* DBManager::otherQueryLineChartData()
{
	//清理
	for (int i = 0; i < collectMethodDuring.size(); ++i)
	{
		delete collectMethodDuring.at(i);
	}
	collectMethodDuring.clear();
	//查询以时间升序排序的数据
	QString querySql;
	querySql.sprintf("select openbid_date, bid_banfa from OpenBidRecords, BidCompanyInfo where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					 and BidCompanyInfo.company_name LIKE '%%%s%%' \
					 and openbid_date >= '%s'\
					 and openbid_date <= '%s' \
					 order by openbid_date",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());
	sql_query.prepare(querySql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			if (sql_query.value(1).compare("方法一") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_ONE;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else if (sql_query.value(1).compare("方法二") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_TWO;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else
			{
				continue;
			}
		}
	}

	return &collectMethodDuring;
}
QVector<MethodDuringTime*>* DBManager::proxyQueryLineChartData()
{
	//清理
	for (int i = 0; i < collectMethodDuring.size(); ++i)
	{
		delete collectMethodDuring.at(i);
	}
	collectMethodDuring.clear();
	//查询以时间升序排序的数据
	QString querySql;
	querySql.sprintf("select openbid_date, bid_banfa from OpenBidRecords where proxy_company LIKE '%%%s%%' \
					 and openbid_date >= '%s' \
					 and openbid_date <= '%s' \
					 order by openbid_date",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());
	sql_query.prepare(querySql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			if (sql_query.value(1).compare("方法一") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_ONE;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else if (sql_query.value(1).compare("方法二") == 0)
			{
				MethodDuringTime* singleRecord = new MethodDuringTime;
				singleRecord->methodType = METHOD_TWO;
				singleRecord->date = sql_query.value(0).toString();
				collectMethodDuring.push_back(singleRecord);
			}
			else
			{
				continue;
			}
		}
	}
	return &collectMethodDuring;
}
MethodCounts* DBManager::selfQueryPieChartData()
{
	QString sqlFirst;
	sqlFirst.sprintf("select count(*) from OpenBidRecords where  bid_banfa= '方法一' AND %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodFirstCount = sql_query.value(0).toInt();
		}
	}

	QString sqlSecond;
	sqlSecond.sprintf("select count(*) from OpenBidRecords where  bid_banfa= '方法二' AND %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlSecond);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodSecondCount = sql_query.value(0).toInt();
		}
	}
	return &collectCounts;
}
MethodCounts* DBManager::otherQueryPieChartData()
{
	QString sqlFirst;
	sqlFirst.sprintf("select count(*) from OpenBidRecords, BidCompanyInfo \
					 where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					 and BidCompanyInfo.company_name  LIKE '%%%s%%' \
					 and bid_banfa= '方法一' \
					 and openbid_date >= '%s' \
					 and openbid_date <= '%s'", 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodFirstCount = sql_query.value(0).toInt();
		}
	}

	QString sqlSecond;
	sqlSecond.sprintf("select count(*) from OpenBidRecords, BidCompanyInfo \
					  where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					  and BidCompanyInfo.company_name  LIKE '%%%s%%' \
					  and bid_banfa= '方法二' \
					  and openbid_date >= '%s' \
					  and openbid_date <= '%s'", 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());
	sql_query.prepare(sqlSecond);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodSecondCount = sql_query.value(0).toInt();
		}
	}
	return &collectCounts;
}
MethodCounts* DBManager::proxyQueryPieChartData()
{
	QString sqlFirst;
	sqlFirst.sprintf("select count(*) from OpenBidRecords where proxy_company LIKE '%%%s%%' and bid_banfa= '方法一' \
					 and openbid_date >= '%s' \
					 and openbid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodFirstCount = sql_query.value(0).toInt();
		}
	}

	QString sqlSecond;
	sqlSecond.sprintf("select count(*) from OpenBidRecords where proxy_company LIKE '%%%s%%' and bid_banfa= '方法二' \
					  and openbid_date >= '%s' \
					  and openbid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());
	sql_query.prepare(sqlSecond);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			collectCounts.methodSecondCount = sql_query.value(0).toInt();
		}
	}
	return &collectCounts;
}
QVector<AveragePricePercent*>* DBManager::selfQueryEVALineChartData()
{
	//清理
	for (int i = 0; i < collectAveragePricePercent.size(); ++i)
	{
		delete collectAveragePricePercent.at(i);
	}
	collectAveragePricePercent.clear();

	QString sqlFirst;
	sqlFirst.sprintf("select bid_averagePricePercent,openbid_date from OpenBidRecords where data_kind in ('方法一', '方法二') and %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePricePercent* singleRecord = new AveragePricePercent;
			singleRecord->percent = sql_query.value(0).toDouble() * 100;
			singleRecord->date = sql_query.value(1).toString();
			collectAveragePricePercent.push_back(singleRecord);
		}
	}
	return &collectAveragePricePercent;
}
QVector<AveragePricePercent*>* DBManager::otherQueryEVALineChartData()
{
	//清理
	for (int i = 0; i < collectAveragePricePercent.size(); ++i)
	{
		delete collectAveragePricePercent.at(i);
	}
	collectAveragePricePercent.clear();

	QString sqlFirst;
	sqlFirst.sprintf("select bid_averagePricePercent,openbid_date  from OpenBidRecords,BidCompanyInfo where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					 and BidCompanyInfo.company_name LIKE '%%%s%%' \
					 and openbid_date >= '%s' \
					 and openbid_date <= '%s' \
					 and data_kind in ('方法一', '方法二') \
					 order by openbid_date",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePricePercent* singleRecord = new AveragePricePercent;
			singleRecord->percent = sql_query.value(0).toDouble()*100;
			singleRecord->date = sql_query.value(1).toString();
			collectAveragePricePercent.push_back(singleRecord);
		}
	}

	return &collectAveragePricePercent;
}
QVector<AveragePricePercent*>* DBManager::proxyQueryEVALineChartData()
{
	//清理
	for (int i = 0; i < collectAveragePricePercent.size(); ++i)
	{
		delete collectAveragePricePercent.at(i);
	}
	collectAveragePricePercent.clear();

	QString sqlFirst;
	sqlFirst.sprintf("select bid_averagePricePercent,openbid_date  from OpenBidRecords where proxy_company LIKE '%%%s%%' \
					 					 and openbid_date >= '%s' \
										 and openbid_date <= '%s' \
										 and data_kind in ('方法一', '方法二') \
										 order by openbid_date",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(), 
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());
	sql_query.prepare(sqlFirst);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePricePercent* singleRecord = new AveragePricePercent;
			singleRecord->percent = sql_query.value(0).toDouble() * 100;
			singleRecord->date = sql_query.value(1).toString();
			collectAveragePricePercent.push_back(singleRecord);
		}
	}
	return &collectAveragePricePercent;
}
//方法一平均报价占比饼状图
QVector<AveragePriceIntervalPercent*>* DBManager::selfQueryMethodOnePieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodOne.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodOne.at(i);
	}
	collectAVEIntervalPercentMethodOne.clear();

	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->otherAVEPriceIntervalPercentONE.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法一' and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s' and %s ",
			min.toUtf8().data(),
			max.toUtf8().data(),
			selfCompanyQueryConditon.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i);
				collectAVEIntervalPercentMethodOne.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法一' and bid_averagePricePercent < '%s' and %s", smallestValue.toUtf8().data() , selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法一' and bid_averagePricePercent >= '%s' and %s", biggestValue.toUtf8().data(), selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}
	return &collectAVEIntervalPercentMethodOne;
}
QVector<AveragePriceIntervalPercent*>* DBManager::otherQueryMethodOnePieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodOne.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodOne.at(i);
	}
	collectAVEIntervalPercentMethodOne.clear();

	QString baseSql;
	baseSql.sprintf("select count(*) from OpenBidRecords, BidCompanyInfo where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					and BidCompanyInfo.company_name LIKE '%%%s%%' \
					and openbid_date >= '%s' \
					and openbid_date <= '%s' and data_kind = '方法一'",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());


	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->otherAVEPriceIntervalPercentONE.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("%s and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s'",
			baseSql.toUtf8().data(),
			min.toUtf8().data(),
			max.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i);
				collectAVEIntervalPercentMethodOne.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("%s and bid_averagePricePercent < '%s'", baseSql.toUtf8().data(), smallestValue.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("%s and bid_averagePricePercent >= '%s'", baseSql.toUtf8().data(), biggestValue.toUtf8().data());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}
	
	return &collectAVEIntervalPercentMethodOne;
}
QVector<AveragePriceIntervalPercent*>* DBManager::proxyQueryMethodOnePieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodOne.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodOne.at(i);
	}
	collectAVEIntervalPercentMethodOne.clear();

	QString baseSql;
	baseSql.sprintf("select count(*) from OpenBidRecords where proxy_company LIKE '%%%s%%' and openbid_date >= '%s' \
															and openbid_date <= '%s' and data_kind = '方法一'",
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(),
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(),
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());


	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->proxyAVEPriceIntervalPercentONE.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("%s and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s'",
			baseSql.toUtf8().data(),
			min.toUtf8().data(),
			max.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i);
				collectAVEIntervalPercentMethodOne.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("%s and bid_averagePricePercent < '%s'", baseSql.toUtf8().data(), smallestValue.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("%s and bid_averagePricePercent >= '%s'", baseSql.toUtf8(), biggestValue.toUtf8());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodOne.push_back(singleRecord);
		}
	}
	return &collectAVEIntervalPercentMethodOne;
}
//方法二平均报价占比饼状图							 
QVector<AveragePriceIntervalPercent*>* DBManager::selfQueryMethodTwoPieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodTwo.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodTwo.at(i);
	}
	collectAVEIntervalPercentMethodTwo.clear();

	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->otherAVEPriceIntervalPercentTWO.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法二' and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s' and %s",
			min.toUtf8().data(),
			max.toUtf8().data(),
			selfCompanyQueryConditon.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i);
				collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法二' and bid_averagePricePercent < '%s' and %s", smallestValue.toUtf8().data(), selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("select count(*) from OpenBidRecords where data_kind = '方法二' and bid_averagePricePercent >= '%s' and %s", biggestValue.toUtf8().data(), selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}

	return &collectAVEIntervalPercentMethodTwo;
}
QVector<AveragePriceIntervalPercent*>* DBManager::otherQueryMethodTwoPieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodTwo.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodTwo.at(i);
	}
	collectAVEIntervalPercentMethodTwo.clear();

	QString baseSql;
	baseSql.sprintf("select count(*) from OpenBidRecords, BidCompanyInfo where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					and BidCompanyInfo.company_name LIKE '%%%s%%' \
					and openbid_date >= '%s' \
					and openbid_date <= '%s' \
					and data_kind = '方法二'",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());


	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->otherAVEPriceIntervalPercentTWO.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("%s and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s'",
			baseSql.toUtf8().data(),
			min.toUtf8().data(),
			max.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i);
				collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("%s and bid_averagePricePercent < '%s'", baseSql.toUtf8().data(), smallestValue.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("%s and bid_averagePricePercent >= '%s'", baseSql.toUtf8().data(), biggestValue.toUtf8().data());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}

	return &collectAVEIntervalPercentMethodTwo;
}
QVector<AveragePriceIntervalPercent*>* DBManager::proxyQueryMethodTwoPieChartData()
{
	//清理
	for (int i = 0; i < collectAVEIntervalPercentMethodTwo.size(); ++i)
	{
		delete collectAVEIntervalPercentMethodTwo.at(i);
	}
	collectAVEIntervalPercentMethodTwo.clear();

	QString baseSql;
	baseSql.sprintf("select count(*) from OpenBidRecords where proxy_company LIKE '%%%s%%' and openbid_date >= '%s' \
															and openbid_date <= '%s' and data_kind = '方法二'",
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(),
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(),
															ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());


	QStringList intervalList = ConfigManager::Instance()->getCurSavedUserConfig()->proxyAVEPriceIntervalPercentTWO.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("%s and bid_averagePricePercent >= '%s' and bid_averagePricePercent < '%s'",
			baseSql.toUtf8().data(),
			min.toUtf8().data(),
			max.toUtf8().data());
		sql_query.prepare(sql);
		if (sql_query.exec())
		{
			while (sql_query.next())
			{
				AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
				singleRecord->projectCount = sql_query.value(0).toDouble();
				singleRecord->percentInterval = intervalList.at(i).toUtf8();
				collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("%s and bid_averagePricePercent < '%s'", baseSql.toUtf8().data(), smallestValue.toUtf8().data());
	sql_query.prepare(queryUnderMinsql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("%s and bid_averagePricePercent >= '%s'", baseSql.toUtf8().data(), biggestValue.toUtf8().data());
	sql_query.prepare(queryAboveMaxSql);
	if (sql_query.exec())
	{
		while (sql_query.next())
		{
			AveragePriceIntervalPercent* singleRecord = new AveragePriceIntervalPercent;
			singleRecord->projectCount = sql_query.value(0).toDouble();
			singleRecord->percentInterval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			collectAVEIntervalPercentMethodTwo.push_back(singleRecord);
		}
	}
	return &collectAVEIntervalPercentMethodTwo;
}
//开标记录表数据
void DBManager::getExcelIntervalPercent(int projectId, QVector<ExcelIntervalPercent*>& percentArray, int bidTotalCount)
{
	QString interval = ConfigManager::Instance()->getCurSavedUserConfig()->baojiaqujian;
	if (interval.isEmpty())
	{
		return;
	}
	QStringList intervalList = interval.split(',');
	int size = intervalList.size();
	QString smallestValue, biggestValue;
	QString labelOfsmallest, labelOfbiggest;
	for (int i = 0; i < size; ++i)
	{
		if (intervalList.at(i).isEmpty())
		{
			continue;
		}
		QStringList minAndMax;
		minAndMax = intervalList.at(i).split('-');
		if (minAndMax.size() != 2)
		{
			continue;
		}
		QString min, max;
		min.sprintf("0.%s", minAndMax.at(0).toUtf8().data());
		max.sprintf("0.%s", minAndMax.at(1).toUtf8().data());
		if (0 == i)
		{
			smallestValue = min;
			labelOfsmallest = minAndMax.at(0);
		}
		if (size - 1 == i)
		{
			biggestValue = max;
			labelOfbiggest = minAndMax.at(1);
		}
		QString sql;
		sql.sprintf("select count(*) from BidCompanyInfo where project_id = %d and proportion >= '%s' and proportion < '%s'",
			projectId,
			min.toUtf8().data(),
			max.toUtf8().data());
		sql_query_another.prepare(sql);
		if (sql_query_another.exec())
		{
			while (sql_query_another.next())
			{
				ExcelIntervalPercent* singleRecord = new ExcelIntervalPercent;
				singleRecord->count = sql_query_another.value(0).toString();
				singleRecord->interval = intervalList.at(i).toUtf8();
				double percent = 0;
				double currentbidCount = 0;
				currentbidCount = singleRecord->count.toDouble();
				if (bidTotalCount > 0)
				{
					percent = currentbidCount / bidTotalCount;
				}
				singleRecord->percent = QString::number(percent, 'f', 4);
				percentArray.push_back(singleRecord);
			}
		}
	}

	QString queryUnderMinsql;
	queryUnderMinsql.sprintf("select count(*) from BidCompanyInfo where project_id = %d and proportion < '%s'", projectId, smallestValue.toUtf8().data());
	sql_query_another.prepare(queryUnderMinsql);
	if (sql_query_another.exec())
	{
		while (sql_query_another.next())
		{
			ExcelIntervalPercent* singleRecord = new ExcelIntervalPercent;
			singleRecord->count = sql_query_another.value(0).toString();
			singleRecord->interval.sprintf("%s以下", labelOfsmallest.toUtf8().data());
			double percent = 0;
			double currentbidCount = 0;
			currentbidCount = singleRecord->count.toDouble();
			if (bidTotalCount > 0)
			{
				percent = currentbidCount / bidTotalCount;
			}
			singleRecord->percent = QString::number(percent, 'f', 4);
			percentArray.push_back(singleRecord);
		}
	}

	QString queryAboveMaxSql;
	queryAboveMaxSql.sprintf("select count(*) from BidCompanyInfo where project_id = %d and proportion >= '%s'", projectId, biggestValue.toUtf8().data());
	sql_query_another.prepare(queryAboveMaxSql);
	if (sql_query_another.exec())
	{
		while (sql_query_another.next())
		{
			ExcelIntervalPercent* singleRecord = new ExcelIntervalPercent;
			singleRecord->count = sql_query_another.value(0).toString();
			singleRecord->interval.sprintf("%s以上", labelOfbiggest.toUtf8().data());
			double percent = 0;
			double currentbidCount = 0;
			currentbidCount = singleRecord->count.toDouble();
			if (bidTotalCount > 0)
			{
				percent = currentbidCount / bidTotalCount;
			}
			singleRecord->percent = QString::number(percent, 'f', 4);
			percentArray.push_back(singleRecord);
		}
	}
}
void DBManager::getBidRecords(QVector<ExcelBidRecord*>& summary)
{
	int serialNum = 0;
	double avaragePercentTotal = 0;
	double goodPricePercentTotal = 0;
	double mypricePercentTotal = 0;
	while (sql_query.next())
	{
		++serialNum;
		ExcelBidRecord* oneRowData = new ExcelBidRecord;
		int projectId = sql_query.value(0).toInt();
		oneRowData->serialNumber = QString::number(serialNum);
		int index1 = sql_query.record().indexOf("project_name");
		oneRowData->projectName = sql_query.value(index1).toString();
		int index2 = sql_query.record().indexOf("openbid_date");
		oneRowData->openBidDate = sql_query.value(index2).toString();
		int index23 = sql_query.record().indexOf("control_price");
		oneRowData->controlPrice = sql_query.value(index23).toString();
		int index3 = sql_query.record().indexOf("Q");
		oneRowData->Q = sql_query.value(index3).toString();
		int index4 = sql_query.record().indexOf("K1");
		oneRowData->K1 = sql_query.value(index4).toString();
		int index5 = sql_query.record().indexOf("K2");
		oneRowData->K2 = sql_query.value(index5).toString();
		int index6 = sql_query.record().indexOf("bid_averagePrice");
		oneRowData->averagePrice = sql_query.value(index6).toString();
		int index7 = sql_query.record().indexOf("bid_averagePricePercent");
		oneRowData->averagePricePercent = sql_query.value(index7).toString();
		avaragePercentTotal += sql_query.value(index7).toDouble();
		int index8 = sql_query.record().indexOf("bid_goodPrice");
		oneRowData->goodPrice = sql_query.value(index8).toString();
		int index9 = sql_query.record().indexOf("bid_goodPricePercent");
		goodPricePercentTotal += sql_query.value(index9).toDouble();
		oneRowData->goodPricePercent = sql_query.value(index9).toString();
		int index10 = sql_query.record().indexOf("project_manager_requirement");
		oneRowData->projectManagerRequirement = sql_query.value(index10).toString();
		int index11 = sql_query.record().indexOf("PerformanceText");
		oneRowData->performanceRequirement = sql_query.value(index11).toString().toUtf8().data();
		int index12 = sql_query.record().indexOf("bid_rule");
		oneRowData->bidRule = sql_query.value(index12).toString();
		int index13 = sql_query.record().indexOf("bid_final_rule");
		oneRowData->bidFinalRule = sql_query.value(index13).toString();
		int index14 = sql_query.record().indexOf("bid_fangshi");
		oneRowData->bidWay = sql_query.value(index14).toString();
		int index15 = sql_query.record().indexOf("project_location");
		oneRowData->location = sql_query.value(index15).toString();
		int index16 = sql_query.record().indexOf("bid_totalCount");
		oneRowData->bidCounts = sql_query.value(index16).toString();
		int index17 = sql_query.record().indexOf("quoted_price");
		oneRowData->myPrice = sql_query.value(index17).toString();
		int index18 = sql_query.record().indexOf("proportion");
		mypricePercentTotal += sql_query.value(index18).toDouble();
		oneRowData->myPricePercent = sql_query.value(index18).toString();
		getExcelIntervalPercent(projectId, oneRowData->bidInterval, oneRowData->bidCounts.toInt());
		summary.push_back(oneRowData);
	}
	//插入“数据汇总”行
	ExcelBidRecord* summaryRow = new ExcelBidRecord;
	summaryRow->projectName.sprintf("数据汇总");
	if (serialNum > 0)
	{
		avaragePercentTotal = avaragePercentTotal / serialNum;
		goodPricePercentTotal = goodPricePercentTotal / serialNum;
		mypricePercentTotal = mypricePercentTotal / serialNum;
	}
	summaryRow->averagePricePercent = QString::number(avaragePercentTotal, 'f', 4);
	summaryRow->goodPricePercent = QString::number(goodPricePercentTotal, 'f', 4);
	summaryRow->myPricePercent = QString::number(mypricePercentTotal, 'f', 4);
	getSummaryIntervalPercent(summaryRow, summary);
	summary.push_back(summaryRow);
}
void DBManager::getSummaryIntervalPercent(ExcelBidRecord* summaryRow, QVector<ExcelBidRecord*>& records)
{
	int size = records.size();
	//去除标题行
	if (size - 1 <= 0)
	{
		return;
	}
	int intervalNums = records.at(size - 1)->bidInterval.size();
	if (intervalNums <= 0)
	{
		return;
	}
	for (int m = 0; m < intervalNums; ++m)
	{
		ExcelIntervalPercent* interval = new ExcelIntervalPercent;
		interval->interval = records.at(size - 1)->bidInterval.at(m)->interval;
		summaryRow->bidInterval.push_back(interval);
	}
	double * percentArray = new double[intervalNums];
	memset(percentArray, 0, sizeof(double)* intervalNums);
	for (int i = 1; i < size; ++i)
	{
		for (int k = 0; k < records.at(i)->bidInterval.size(); ++k)
		{
			double tempPercent = records.at(i)->bidInterval.at(k)->percent.toDouble();
			percentArray[k] += tempPercent;
		}
	}
	for (int i = 0; i < intervalNums; ++i)
	{
		if (size -1 > 0)
		{
			double percent = percentArray[i];
			percentArray[i] = percent / (size - 1);
		}
		
		summaryRow->bidInterval.at(i)->percent = QString::number(percentArray[i], 'f', 4);
	}
	delete percentArray;
}
ExcelBidRecordsSummary* DBManager::getSelfBidRecordsSummaryData()
{
	//清理
	for (int i = 0; i < selfBidRecordsSummary.MethodOneRecords.size(); ++i)
	{
		for (int j = 0; j < selfBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.size(); ++j)
		{
			delete selfBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.at(j);
		}
		delete selfBidRecordsSummary.MethodOneRecords.at(i);
	}
	selfBidRecordsSummary.MethodOneRecords.clear();
	for (int i = 0; i < selfBidRecordsSummary.MethodTwoRecords.size(); ++i)
	{
		for (int j = 0; j < selfBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.size(); ++j)
		{
			delete selfBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.at(j);
		}
		delete selfBidRecordsSummary.MethodTwoRecords.at(i);
	}
	selfBidRecordsSummary.MethodTwoRecords.clear();
	for (int i = 0; i < selfBidRecordsSummary.DataFakeRecords.size(); ++i)
	{
		for (int j = 0; j < selfBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.size(); ++j)
		{
			delete selfBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.at(j);
		}
		delete selfBidRecordsSummary.DataFakeRecords.at(i);
	}
	selfBidRecordsSummary.DataFakeRecords.clear();
	//插入“方法一”标题行
	ExcelBidRecord* methodOneTitleRow = new ExcelBidRecord;
	methodOneTitleRow->serialNumber.sprintf("一");
	methodOneTitleRow->projectName.sprintf("评标方法为方法一");
	selfBidRecordsSummary.MethodOneRecords.push_back(methodOneTitleRow);
	//方法一开标记录统计
	QString sqlMethodOne;
	sqlMethodOne.sprintf("select * from OpenBidRecords where data_kind = '方法一' and %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlMethodOne);
	if (sql_query.exec())
	{
		getBidRecords(selfBidRecordsSummary.MethodOneRecords);
	}

	//插入“方法二”标题行
	ExcelBidRecord* methodTwoTitleRow = new ExcelBidRecord;
	methodTwoTitleRow->serialNumber.sprintf("二");
	methodTwoTitleRow->projectName.sprintf("评标方法为方法二");
	selfBidRecordsSummary.MethodTwoRecords.push_back(methodTwoTitleRow);
	//方法二开标记录统计
	QString sqlMethodTwo;
	sqlMethodTwo.sprintf("select * from OpenBidRecords where data_kind = '方法二' and %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlMethodTwo);
	if (sql_query.exec())
	{
		getBidRecords(selfBidRecordsSummary.MethodTwoRecords);
	}
	//插入“数据离散”标题行
	ExcelBidRecord* DataFakeTitleRow = new ExcelBidRecord;
	DataFakeTitleRow->serialNumber.sprintf("三");
	DataFakeTitleRow->projectName.sprintf("数据离散的项目");
	selfBidRecordsSummary.DataFakeRecords.push_back(DataFakeTitleRow);
	//数据离散开标记录统计
	QString sqlDataFake;
	sqlDataFake.sprintf("select * from OpenBidRecords where data_kind = '数据离散' and %s", selfCompanyQueryConditon.toUtf8().data());
	sql_query.prepare(sqlDataFake);
	if (sql_query.exec())
	{
		getBidRecords(selfBidRecordsSummary.DataFakeRecords);
	}
	return &selfBidRecordsSummary;
}
//其他单位开标记录表数据
ExcelBidRecordsSummary* DBManager::getOtherBidRecordsSummaryData()
{
	//清理
	for (int i = 0; i < otherBidRecordsSummary.MethodOneRecords.size(); ++i)
	{
		for (int j = 0; j < otherBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.size(); ++j)
		{
			delete otherBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.at(j);
		}
		delete otherBidRecordsSummary.MethodOneRecords.at(i);
	}
	otherBidRecordsSummary.MethodOneRecords.clear();
	for (int i = 0; i < otherBidRecordsSummary.MethodTwoRecords.size(); ++i)
	{
		for (int j = 0; j < otherBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.size(); ++j)
		{
			delete otherBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.at(j);
		}
		delete otherBidRecordsSummary.MethodTwoRecords.at(i);
	}
	otherBidRecordsSummary.MethodTwoRecords.clear();
	for (int i = 0; i < otherBidRecordsSummary.DataFakeRecords.size(); ++i)
	{
		for (int j = 0; j < otherBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.size(); ++j)
		{
			delete otherBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.at(j);
		}
		delete otherBidRecordsSummary.DataFakeRecords.at(i);
	}
	otherBidRecordsSummary.DataFakeRecords.clear();

	QString baseSql;
	baseSql.sprintf("select * from OpenBidRecords, BidCompanyInfo where BidCompanyInfo.project_id = OpenBidRecords.project_id \
					and BidCompanyInfo.company_name LIKE '%%%s%%' \
					and openbid_date >= '%s' and openbid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());

	otherBidRecordsSummary.title.sprintf("%s开标记录表", ConfigManager::Instance()->getCurSavedUserConfig()->otherCompanyName.toUtf8().data());
	//插入“方法一”标题行
	ExcelBidRecord* methodOneTitleRow = new ExcelBidRecord;
	methodOneTitleRow->serialNumber.sprintf("一");
	methodOneTitleRow->projectName.sprintf("评标方法为方法一");
	otherBidRecordsSummary.MethodOneRecords.push_back(methodOneTitleRow);
	//方法一开标记录统计
	QString sqlMethodOne;
	sqlMethodOne.sprintf("%s and data_kind = '方法一'", baseSql.toUtf8().data());
	sql_query.prepare(sqlMethodOne);
	if (sql_query.exec())
	{
		getBidRecords(otherBidRecordsSummary.MethodOneRecords);
	}

	//插入“方法二”标题行
	ExcelBidRecord* methodTwoTitleRow = new ExcelBidRecord;
	methodTwoTitleRow->serialNumber.sprintf("二");
	methodTwoTitleRow->projectName.sprintf("评标方法为方法二");
	otherBidRecordsSummary.MethodTwoRecords.push_back(methodTwoTitleRow);
	//方法二开标记录统计
	QString sqlMethodTwo;
	sqlMethodTwo.sprintf("%s and data_kind = '方法二'", baseSql.toUtf8().data());
	sql_query.prepare(sqlMethodTwo);
	if (sql_query.exec())
	{
		getBidRecords(otherBidRecordsSummary.MethodTwoRecords);
	}

	//插入“数据离散”标题行
	ExcelBidRecord* DataFakeTitleRow = new ExcelBidRecord;
	DataFakeTitleRow->serialNumber.sprintf("三");
	DataFakeTitleRow->projectName.sprintf("数据离散的项目");
	otherBidRecordsSummary.DataFakeRecords.push_back(DataFakeTitleRow);
	//数据离散开标记录统计
	QString sqlDataFake;
	sqlDataFake.sprintf("%s and data_kind = '数据离散'", baseSql.toUtf8().data());
	sql_query.prepare(sqlDataFake);
	if (sql_query.exec())
	{
		getBidRecords(otherBidRecordsSummary.DataFakeRecords);
	}

	return &otherBidRecordsSummary;
}
QString DBManager::getOtherCompanyBidCount(const QString& name)
{
	QString sql;
	sql.sprintf("select count(distinct project_id) from BidCompanyInfo where bid_date >= '%s' and bid_date <= '%s' and company_name = '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data(), 
		name.toUtf8().data());
	sql_query_another.prepare(sql);
	if (sql_query_another.exec())
	{
		while (sql_query_another.next())
		{
			return sql_query_another.value(0).toString();
		}
	}
	return NULL;
}
ExcelCompanySummary* DBManager::getOtherCompanySummaryData()
{
	//清理
	for (int i = 0; i < otherCompanySummary.companyArray.size(); ++i)
	{
		delete otherCompanySummary.companyArray.at(i);
	}
	otherCompanySummary.companyArray.clear();

	QString sql;
	sql.sprintf("select distinct company_name from BidCompanyInfo where bid_date >= '%s' and bid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->otherStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->otherEndDate.toUtf8().data());
	sql_query.prepare(sql);
	if (sql_query.exec())
	{
		int serialNum = 0;
		while (sql_query.next())
		{
			++serialNum;
			ExcelCompanyInfo* singleRecord = new ExcelCompanyInfo;
			singleRecord->serialNum = QString::number(serialNum);
			singleRecord->name = sql_query.value(0).toString();
			singleRecord->bidCount = getOtherCompanyBidCount(singleRecord->name);
			otherCompanySummary.companyArray.push_back(singleRecord);
		}
	}
	return &otherCompanySummary;
}
//代理单位开标记录表数据
ExcelBidRecordsSummary* DBManager::getProxyBidRecordsSummaryData()
{
	//清理
	for (int i = 0; i < proxyBidRecordsSummary.MethodOneRecords.size(); ++i)
	{
		for (int j = 0; j < proxyBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.size(); ++j)
		{
			delete proxyBidRecordsSummary.MethodOneRecords.at(i)->bidInterval.at(j);
		}
		delete proxyBidRecordsSummary.MethodOneRecords.at(i);
	}
	proxyBidRecordsSummary.MethodOneRecords.clear();
	for (int i = 0; i < proxyBidRecordsSummary.MethodTwoRecords.size(); ++i)
	{
		for (int j = 0; j < proxyBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.size(); ++j)
		{
			delete proxyBidRecordsSummary.MethodTwoRecords.at(i)->bidInterval.at(j);
		}
		delete proxyBidRecordsSummary.MethodTwoRecords.at(i);
	}
	proxyBidRecordsSummary.MethodTwoRecords.clear();
	for (int i = 0; i < proxyBidRecordsSummary.DataFakeRecords.size(); ++i)
	{
		for (int j = 0; j < proxyBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.size(); ++j)
		{
			delete proxyBidRecordsSummary.DataFakeRecords.at(i)->bidInterval.at(j);
		}
		delete proxyBidRecordsSummary.DataFakeRecords.at(i);
	}
	proxyBidRecordsSummary.DataFakeRecords.clear();

	QString baseSql;
	baseSql.sprintf("select * from OpenBidRecords where proxy_company LIKE '%%%s%%' and openbid_date >= '%s' and openbid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());

	proxyBidRecordsSummary.title.sprintf("%s招标代理记录表", ConfigManager::Instance()->getCurSavedUserConfig()->proxyCompanyName.toUtf8().data());
	//插入“方法一”标题行
	ExcelBidRecord* methodOneTitleRow = new ExcelBidRecord;
	methodOneTitleRow->serialNumber.sprintf("一");
	methodOneTitleRow->projectName.sprintf("评标方法为方法一");
	proxyBidRecordsSummary.MethodOneRecords.push_back(methodOneTitleRow);
	//方法一开标记录统计
	QString sqlMethodOne;
	sqlMethodOne.sprintf("%s and data_kind = '方法一'", baseSql.toUtf8().data());
	sql_query.prepare(sqlMethodOne);
	if (sql_query.exec())
	{
		getBidRecords(proxyBidRecordsSummary.MethodOneRecords);
	}

	//插入“方法二”标题行
	ExcelBidRecord* methodTwoTitleRow = new ExcelBidRecord;
	methodTwoTitleRow->serialNumber.sprintf("二");
	methodTwoTitleRow->projectName.sprintf("评标方法为方法二");
	proxyBidRecordsSummary.MethodTwoRecords.push_back(methodTwoTitleRow);
	//方法二开标记录统计
	QString sqlMethodTwo;
	sqlMethodTwo.sprintf("%s and data_kind = '方法二'", baseSql.toUtf8().data());
	sql_query.prepare(sqlMethodTwo);
	if (sql_query.exec())
	{
		getBidRecords(proxyBidRecordsSummary.MethodTwoRecords);
	}

	//插入“数据离散”标题行
	ExcelBidRecord* DataFakeTitleRow = new ExcelBidRecord;
	DataFakeTitleRow->serialNumber.sprintf("三");
	DataFakeTitleRow->projectName.sprintf("数据离散的项目");
	proxyBidRecordsSummary.DataFakeRecords.push_back(DataFakeTitleRow);
	//数据离散开标记录统计
	QString sqlDataFake;
	sqlDataFake.sprintf("%s and data_kind = '数据离散'", baseSql.toUtf8().data());
	sql_query.prepare(sqlDataFake);
	if (sql_query.exec())
	{
		getBidRecords(proxyBidRecordsSummary.DataFakeRecords);
	}

	return &proxyBidRecordsSummary;
}
QString DBManager::getProxyCompanyBidCount(const QString& name)
{
	QString sql;
	sql.sprintf("select count(*) from OpenBidRecords where openbid_date >= '%s' and openbid_date <= '%s' and proxy_company = '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data(),
		name.toUtf8().data());
	sql_query_another.prepare(sql);
	if (sql_query_another.exec())
	{
		while (sql_query_another.next())
		{
			return sql_query_another.value(0).toString();
		}
	}
	return NULL;
}
ExcelCompanySummary* DBManager::getProxyCompanySummaryData()
{
	//清理
	for (int i = 0; i < proxyCompanySummary.companyArray.size(); ++i)
	{
		delete proxyCompanySummary.companyArray.at(i);
	}
	proxyCompanySummary.companyArray.clear();

	QString sql;
	sql.sprintf("select distinct proxy_company from OpenBidRecords where openbid_date >= '%s' and openbid_date <= '%s'",
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyStartDate.toUtf8().data(),
		ConfigManager::Instance()->getCurSavedUserConfig()->proxyEndDate.toUtf8().data());
	sql_query.prepare(sql);
	if (sql_query.exec())
	{
		int serialNum = 0;
		while (sql_query.next())
		{
			++serialNum;
			ExcelCompanyInfo* singleRecord = new ExcelCompanyInfo;
			singleRecord->serialNum = QString::number(serialNum);
			singleRecord->name = sql_query.value(0).toString();
			singleRecord->bidCount = getProxyCompanyBidCount(singleRecord->name);
			proxyCompanySummary.companyArray.push_back(singleRecord);
		}
	}
	return &proxyCompanySummary;
}