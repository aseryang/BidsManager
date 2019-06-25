#include "BatchImportHelper.h"
#include "xlsx\xlsxdocument.h"
#include "xlsx\xlsxworkbook.h"
#include <QDir>
#include <QDate>
#include "DBManager.h"

void BatchImportHelper::doImport(QString dir)
{
	isStop = false;
	excelsDir = dir;
	start();
}
void BatchImportHelper::stopImport()
{
	isStop = true;
	quit();
	wait();
}

static QFileInfoList getFileNames(const QString &path)
{
	QDir dir(path);
	QStringList nameFilters;
	nameFilters << "*.xlsx" << "*.xls";
	QFileInfoList list = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	return list;
}

void BatchImportHelper::run()
{
	info.currentFileIndex.clear();
	info.errorInfo.clear();
	info.runningInfo.clear();
	QFileInfoList list = getFileNames(excelsDir);
	int size = list.length();
	if (!size)
	{
		info.errorInfo = "该目录下未发现Excel文件！";
		emit runningInfoSignal(BIS_ERROR_INFO, &info);
		return;
	}
	for (int i = 0; i < size; ++i)
	{
		if (isStop)
		{
			break;
		}
		QString name = list.at(i).fileName();
		QString path = list.at(i).filePath();
		info.currentFileIndex.sprintf("%d/%d", i + 1, size);
		info.runningInfo.sprintf("正在导入 %s ...", name.toUtf8().data());
		emit runningInfoSignal(BIS_RUNNING_INFO, &info);
		RetCode ret = work(path);
		if (ret)
		{
			info.errorInfo.append(name + "\n");
		}
	}
	if (!info.errorInfo.isEmpty())
	{
		info.errorInfo.append("以上文件导入失败！请检查文件内容！");
	}
	emit runningInfoSignal(BIS_FINISHED, &info);
}
RetCode BatchImportHelper::work(QString path)
{
	QXlsx::Document xlsx(path.toUtf8().data());
	QXlsx::Workbook *workBook = xlsx.workbook();
	if (!workBook)
	{
		return RET_FAILED;
	}
	QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));
	if (!workSheet)
	{
		return RET_FAILED;
	}

	bool findError = false;
	OpenBidRecords record;
	QString range_ProjectName = EXCEL_POS_PROJECT_NAME;
	record.projectName = workSheet->cellAt(range_ProjectName)->value().toString();
	if (record.projectName.isEmpty())
	{
		return RET_FAILED;
	}

	QString range_OpenBidDate = EXCEL_POS_OPENBID_DATE;
	QXlsx::Cell *OpenBidDatecell = workSheet->cellAt(range_OpenBidDate);
	if (OpenBidDatecell && OpenBidDatecell->isDateTime())
	{
		record.openBidDate = OpenBidDatecell->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	}
	else
	{
		return RET_FAILED;
	}

	QString range_BuildCompany = EXCEL_POS_BUILD_COMPANY;
	record.buildCompany = workSheet->cellAt(range_BuildCompany)->value().toString();

	QString range_ProxyCompany = EXCEL_POS_PROXY_COMPANY;
	record.proxyCompany = workSheet->cellAt(range_ProxyCompany)->value().toString();

	if (record.buildCompany.isEmpty() && record.proxyCompany.isEmpty())
	{
		return RET_FAILED;
	}

	QString range_ProjectLocation = EXCEL_POS_PROJECT_LOCATION;
	record.projectLocation = workSheet->cellAt(range_ProjectLocation)->value().toString();

	QString range_BidFangshi = EXCEL_POS_BID_FANGSHI;
	record.bidFangshi = workSheet->cellAt(range_BidFangshi)->value().toString();

	QString range_BidBanfa = EXCEL_POS_BID_BANFA;
	record.bidBanfa = workSheet->cellAt(range_BidBanfa)->value().toString();
	if (!record.bidBanfa.isEmpty())
	{
		int index1 = record.bidBanfa.indexOf("方法一");
		int index2 = record.bidBanfa.indexOf("方法二");
		if (index1 < 0 && index2 < 0)
		{
			return RET_FAILED;
		}
	}
	else
	{
		return RET_FAILED;
	}

	QString range_averagePrice = EXCEL_POS_AVERGAE_PRICE;
	record.averagePrice = workSheet->cellAt(range_averagePrice)->value().toString();
	double checkAVEPrice = record.averagePrice.toDouble();
	if (record.averagePrice.isEmpty() || checkAVEPrice <= 0)
	{
		return RET_FAILED;
	}
	else
	{
		record.averagePrice = QString::number(checkAVEPrice, 'f', 4);
	}

	QString range_averagePricePercent = EXCEL_POS_AVERAGE_PRICE_PERCENT;
	record.averagePricePercent = workSheet->cellAt(range_averagePricePercent)->value().toString();
	double checkAVEPercent = record.averagePricePercent.toDouble();
	if (record.averagePricePercent.isEmpty() || checkAVEPercent <= 0)
	{
		return RET_FAILED;
	}
	else
	{
		record.averagePricePercent = QString::number(checkAVEPercent, 'f', 4);
	}

	QString range_BidRule = EXCEL_POS_BID_RULE;
	record.bidRule = workSheet->cellAt(range_BidRule)->value().toString();

	QString range_BidFinalRule = EXCEL_POS_BID_FINAL_RULE;
	record.bidFinalRule = workSheet->cellAt(range_BidFinalRule)->value().toString();

	QString range_BidMgrReq = EXCEL_POS_PROJECT_MANAGER_REQUIREMENT;
	record.projectManagerRequirement = workSheet->cellAt(range_BidMgrReq)->value().toString();


	QString range_PerformReq = EXCEL_POS_PERFORMANCE_REQUIREMENT;
	QString requirement = workSheet->cellAt(range_PerformReq)->value().toString();
	QRegExp rx("(\\d+)");//匹配数字
	int indexOfmanager = requirement.indexOf("项目经理");
	int indexOfCompany = requirement.indexOf("企业");
	int pos = 0;
	if (indexOfmanager >= 0 && indexOfCompany >= 0)
	{
		if (indexOfmanager < indexOfCompany)
		{
			if ((pos = rx.indexIn(requirement, pos)) != -1)
			{
				record.projectManagerPerformanceRequirement = rx.cap(0);
				pos += rx.matchedLength();
			}
			if ((pos = rx.indexIn(requirement, pos)) != -1)
			{
				record.companyPerformanceRequirement = rx.cap(0);
			}
		}
		else
		{
			if ((pos = rx.indexIn(requirement, pos)) != -1)
			{
				record.companyPerformanceRequirement = rx.cap(0);
				pos += rx.matchedLength();
			}
			if ((pos = rx.indexIn(requirement, pos)) != -1)
			{
				record.projectManagerPerformanceRequirement = rx.cap(0);
			}
		}

	}
	else if (indexOfmanager >= 0)
	{
		if ((pos = rx.indexIn(requirement, pos)) != -1)
		{
			record.projectManagerPerformanceRequirement = rx.cap(0);
		}
	}
	else if (indexOfCompany >= 0)
	{
		if ((pos = rx.indexIn(requirement, pos)) != -1)
		{
			record.companyPerformanceRequirement = rx.cap(0);
		}
	}
	record.performancetext = requirement;

	if (record.bidBanfa.indexOf("方法一") >= 0)
	{
		QString range_K = EXCEL_POS_K;
		record.K1 = workSheet->cellAt(range_K)->value().toString();
	}
	else
	{
		QString range_K1 = EXCEL_POS_K1;
		record.K1 = workSheet->cellAt(range_K1)->value().toString();

		QString range_K2 = EXCEL_POS_K2;
		record.K2 = workSheet->cellAt(range_K2)->value().toString();

		QString range_Q = EXCEL_POS_Q;
		record.Q = workSheet->cellAt(range_Q)->value().toString();
	}


	QString range_ControlPrice = EXCEL_POS_CONTROL_PRICE;
	record.controlPrice = workSheet->cellAt(range_ControlPrice)->value().toString();
	double checkPrice = record.controlPrice.toDouble();

	if (record.controlPrice.isEmpty() || checkPrice <= 0)
	{
		return RET_FAILED;
	}
	else
	{
		record.controlPrice = QString::number(checkPrice, 'f', 4);
	}

	QString range_Comments = EXCEL_POS_COMMENTS;
	record.comments = workSheet->cellAt(range_Comments)->value().toString();

	QString range_DataKind = EXCEL_POS_DATA_KIND;
	record.dataKind = workSheet->cellAt(range_DataKind)->value().toString();
	if (!record.dataKind.isEmpty())
	{
		int index1 = record.dataKind.indexOf("方法一");
		int index2 = record.dataKind.indexOf("方法二");
		int index3 = record.dataKind.indexOf("数据离散");
		if (index1 < 0 && index2 < 0 && index3 < 0)
		{
			return RET_FAILED;
		}
	}
	else
	{
		return RET_FAILED;
	}

	double goodPrice = 0;
	double goodPricePercent = 0;
	if (record.bidBanfa.indexOf("方法一") >= 0)
	{
		QString range_gp_fangfayi = EXCEL_POS_GOODPRICE_FANGFAYI;
		goodPrice = workSheet->cellAt(range_gp_fangfayi)->value().toDouble();
		if (goodPrice <= 0)
		{
			return RET_FAILED;
		}
		else
		{
			goodPricePercent = goodPrice / checkPrice;
		}
	}
	else
	{
		QString range_gp_fangfaer = EXCEL_POS_GOODPRICE_FANGFAER;
		goodPrice = workSheet->cellAt(range_gp_fangfaer)->value().toDouble();
		if (goodPrice <= 0)
		{
			return RET_FAILED;
		}
		else
		{
			goodPricePercent = goodPrice / checkPrice;
		}
	}
	if (!findError)
	{
		record.goodPrice = QString::number(goodPrice, 'f', 4);
		record.goodPricePercent = QString::number(goodPricePercent, 'f', 4);
	}

	if (findError)
	{
		return RET_FAILED;
	}

	//投标的施工单位信息，sheet页第2页
	QXlsx::Worksheet *workSheet2 = static_cast<QXlsx::Worksheet*>(workBook->sheet(1));
	if (!workSheet2)
	{
		return RET_FAILED;
	}

	int bid_count = 0;
	QStringList nameList, priceList, proportionList;
	for (int i = EXCEL_POS_COMPANY_INFO_START_LINE; i < workSheet2->dimension().rowCount(); ++i)
	{
		QString companyName = workSheet2->cellAt(i, 2)->value().toString();
		QString quotedPrice = workSheet2->cellAt(i, 3)->value().toString();
		QString adjustProportion, adjustQuotedPrice;
		double bidPrice = quotedPrice.toDouble();
		double checkProportion = bidPrice / checkPrice;
		if (companyName.isEmpty() || quotedPrice.isEmpty() ||
			bidPrice <= 0 || checkProportion <= 0)
		{
			break;
		}
		++bid_count;
		adjustProportion = QString::number(checkProportion, 'f', 4);
		adjustQuotedPrice = QString::number(bidPrice, 'f', 4);
		nameList.push_back(companyName);
		priceList.push_back(adjustQuotedPrice);
		proportionList.push_back(adjustProportion);
	}
	if (bid_count <= 0)
	{
		findError = true;
	}
	if (findError)
	{
		return RET_FAILED;
	}
	record.bidTotalCount = QString::number(bid_count);
	DBManager::Instance()->storeBidRecords(&record);
	for (int i = 0; i < bid_count; ++i)
	{
		DBManager::Instance()->storeBidCompanyInfo(record.openBidDate, nameList.at(i), priceList.at(i), proportionList.at(i));
	}
	return RET_OK;
}
