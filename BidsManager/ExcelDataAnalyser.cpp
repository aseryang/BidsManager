#include "ExcelDataAnalyser.h"
#include "DBManager.h"
#include <QDate>
#include <QMessageBox>

ExcelDataAnalyser* ExcelDataAnalyser::Instance()
{
	static ExcelDataAnalyser* ptr = new ExcelDataAnalyser;
	return ptr;
}
void ExcelDataAnalyser::run()
{
	QAxObject *excel = new QAxObject;
	if (excel->setControl("Excel.Application"))
	{
		excel->setProperty("Visible", false); //隐藏打开的excel文件界面
		excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
		QAxObject *workbooks = excel->querySubObject("WorkBooks");
		if (!workbooks)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("获取工作簿集合失败。"));
			//QMessageBox::warning(NULL, "错误", "获取工作簿集合失败。", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject *workbook = workbooks->querySubObject("Open(QString, QVariant)", filePath.toUtf8().data()); //打开文件
		if (!workbook)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开记录文件失败。"));
			//QMessageBox::warning(NULL, "错误", "打开记录文件失败。", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1); //访问第一个工作表
		if (!worksheet)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开第一个Sheet页失败。"));
			//QMessageBox::warning(NULL, "错误", "打开第一个Sheet页失败。", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}

		QString errorMsg("导入失败！文件内容格式校验失败，原因：");
		bool findError = false;
		OpenBidRecords record;
		QString range_ProjectName = EXCEL_POS_PROJECT_NAME;
		QAxObject * projectNameCell = worksheet->querySubObject("Range(QString)", range_ProjectName);
		QVariant projectNameCellValue = projectNameCell->property("Value");
		record.projectName = projectNameCellValue.toString();

		if (record.projectName.isEmpty())
		{
			errorMsg.append("“工程名称”未填入有效数据；");
			findError = true;
		}

		QString range_OpenBidDate = EXCEL_POS_OPENBID_DATE;
		QAxObject * OpenBidDateCell = worksheet->querySubObject("Range(QString)", range_OpenBidDate);
		QVariant OpenBidDateCellValue = OpenBidDateCell->property("Value");
		//record.openBidDate = OpenBidDateCellValue.toString();
		QString openBidDate = OpenBidDateCellValue.toString();
		if (openBidDate.isEmpty())
		{
			errorMsg.append("“开标时间”未填入有效数据；");
			findError = true;
		}
		else
		{
			int index = openBidDate.indexOf('T');
			if (index > 0)
			{
				record.openBidDate = openBidDate.left(index);
			}
			else
			{
				errorMsg.append("“开标时间”未填入有效数据；");
				findError = true;
			}
		}


		QString range_BuildCompany = EXCEL_POS_BUILD_COMPANY;
		QAxObject * BuildCompanyCell = worksheet->querySubObject("Range(QString)", range_BuildCompany);
		QVariant BuildCompanyCellValue = BuildCompanyCell->property("Value");
		record.buildCompany = BuildCompanyCellValue.toString();

		QString range_ProxyCompany = EXCEL_POS_PROXY_COMPANY;
		QAxObject * ProxyCompanyCell = worksheet->querySubObject("Range(QString)", range_ProxyCompany);
		QVariant ProxyCompanyCellValue = ProxyCompanyCell->property("Value");
		record.proxyCompany = ProxyCompanyCellValue.toString();

		if (record.buildCompany.isEmpty() && record.proxyCompany.isEmpty())
		{
			errorMsg.append("“建设单位”，“代理单位”未填入有效数据；");
			findError = true;
		}

		QString range_ProjectLocation = EXCEL_POS_PROJECT_LOCATION;
		QAxObject * ProjectLocationCell = worksheet->querySubObject("Range(QString)", range_ProjectLocation);
		QVariant ProjectLocationCellValue = ProjectLocationCell->property("Value");
		record.projectLocation = ProjectLocationCellValue.toString();

		QString range_BidFangshi = EXCEL_POS_BID_FANGSHI;
		QAxObject * BidFangshiCell = worksheet->querySubObject("Range(QString)", range_BidFangshi);
		QVariant BidFangshiCellValue = BidFangshiCell->property("Value");
		record.bidFangshi = BidFangshiCellValue.toString();

		QString range_BidBanfa = EXCEL_POS_BID_BANFA;
		QAxObject * BidBanfaCell = worksheet->querySubObject("Range(QString)", range_BidBanfa);
		QVariant BidBanfaCellValue = BidBanfaCell->property("Value");
		record.bidBanfa = BidBanfaCellValue.toString();
		if (!record.bidBanfa.isEmpty())
		{
			int index1 = record.bidBanfa.indexOf("方法一");
			int index2 = record.bidBanfa.indexOf("方法二");
			if (index1 < 0 && index2 < 0)
			{
				errorMsg.append("“评标办法”未填入有效数据；");
				findError = true;
			}
		}
		else
		{
			errorMsg.append("“评标办法”未填入有效数据；");
			findError = true;
		}

		QString range_averagePrice = EXCEL_POS_AVERGAE_PRICE;
		QAxObject * range_averagePriceCell = worksheet->querySubObject("Range(QString)", range_averagePrice);
		QVariant range_averagePriceCellValue = range_averagePriceCell->property("Value");
		record.averagePrice = range_averagePriceCellValue.toString();
		double checkAVEPrice = record.averagePrice.toDouble();
		if (record.averagePrice.isEmpty() || checkAVEPrice <= 0)
		{
			errorMsg.append("“平均值”未填入有效数据；");
			findError = true;
		}

		QString range_averagePricePercent = EXCEL_POS_AVERAGE_PRICE_PERCENT;
		QAxObject * range_averagePricePercentCell = worksheet->querySubObject("Range(QString)", range_averagePricePercent);
		QVariant range_averagePricePercentCellValue = range_averagePricePercentCell->property("Value");
		record.averagePricePercent = range_averagePricePercentCellValue.toString();
		double checkAVEPercent = record.averagePricePercent.toDouble();
		if (record.averagePricePercent.isEmpty() || checkAVEPercent <= 0)
		{
			errorMsg.append("“平均值占比”未填入有效数据；");
			findError = true;
		}

		QString range_BidRule = EXCEL_POS_BID_RULE;
		QAxObject * BidRuleCell = worksheet->querySubObject("Range(QString)", range_BidRule);
		QVariant BidRuleCellValue = BidRuleCell->property("Value");
		record.bidRule = BidRuleCellValue.toString();
// 		if (!record.bidRule.isEmpty())
// 		{
// 			int index1 = record.bidRule.indexOf("合理低价法");
// 			int index2 = record.bidRule.indexOf("评标入围法");
// 			if (index1 < 0 && index2 < 0)
// 			{
// 				errorMsg.append("“招标规定方法”未填入有效数据；");
// 				findError = true;
// 			}
// 		}

		QString range_BidFinalRule = EXCEL_POS_BID_FINAL_RULE;
		QAxObject * BidFinalRuleCell = worksheet->querySubObject("Range(QString)", range_BidFinalRule);
		QVariant BidFinalRuleCellValue = BidFinalRuleCell->property("Value");
		record.bidFinalRule = BidFinalRuleCellValue.toString();

		QString range_BidMgrReq = EXCEL_POS_PROJECT_MANAGER_REQUIREMENT;
		QAxObject * BidMgrReqCell = worksheet->querySubObject("Range(QString)", range_BidMgrReq);
		QVariant BidMgrReqCellValue = BidMgrReqCell->property("Value");
		record.projectManagerRequirement = BidMgrReqCellValue.toString();
// 		if (!record.projectManagerRequirement.isEmpty())
// 		{
// 			int index1 = record.projectManagerRequirement.indexOf("高级工程师");
// 			int index2 = record.projectManagerRequirement.indexOf("工程师");
// 			int index3 = record.projectManagerRequirement.indexOf("一级建造师");
// 			int index4 = record.projectManagerRequirement.indexOf("二级建造师");
// 			if (index1 < 0 && index2 < 0 && index3 < 0 && index4 < 0)
// 			{
// 				errorMsg.append("“项目经理要求”未填入有效数据；");
// 				findError = true;
// 			}
// 		}

		QString range_PerformReq = EXCEL_POS_PERFORMANCE_REQUIREMENT;
		QAxObject * PerformReqCell = worksheet->querySubObject("Range(QString)", range_PerformReq);
		QVariant PerformReqCellValue = PerformReqCell->property("Value");
		QString requirement = PerformReqCellValue.toString();
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


		QString range_K1 = EXCEL_POS_K1;
		QAxObject * K1Cell = worksheet->querySubObject("Range(QString)", range_K1);
		QVariant K1CellValue = K1Cell->property("Value");
		record.K1 = K1CellValue.toString();

		QString range_K2 = EXCEL_POS_K2;
		QAxObject * K2Cell = worksheet->querySubObject("Range(QString)", range_K2);
		QVariant K2CellValue = K2Cell->property("Value");
		record.K2 = K2CellValue.toString();

		QString range_Q = EXCEL_POS_Q;
		QAxObject * QCell = worksheet->querySubObject("Range(QString)", range_Q);
		QVariant QCellValue = QCell->property("Value");
		record.Q = QCellValue.toString();

		QString range_ControlPrice = EXCEL_POS_CONTROL_PRICE;
		QAxObject * ControlPriceCell = worksheet->querySubObject("Range(QString)", range_ControlPrice);
		QVariant ControlPriceCellValue = ControlPriceCell->property("Value");
		record.controlPrice = ControlPriceCellValue.toString();
		double checkPrice = ControlPriceCellValue.toDouble();

		if (record.controlPrice.isEmpty() || checkPrice <= 0)
		{
			errorMsg.append("“控制价”未填入有效数据；");
			findError = true;
		}

		QString range_Comments = EXCEL_POS_COMMENTS;
		QAxObject * CommentsCell = worksheet->querySubObject("Range(QString)", range_Comments);
		QVariant CommentsCellValue = CommentsCell->property("Value");
		record.comments = CommentsCellValue.toString();

		QString range_DataKind = EXCEL_POS_DATA_KIND;
		QAxObject * DataKindCell = worksheet->querySubObject("Range(QString)", range_DataKind);
		QVariant DataKindCellValue = DataKindCell->property("Value");
		record.dataKind = DataKindCellValue.toString();
		if (!record.dataKind.isEmpty())
		{
			int index1 = record.dataKind.indexOf("方法一");
			int index2 = record.dataKind.indexOf("方法二");
			int index3 = record.dataKind.indexOf("数据离散");
			if (index1 < 0 && index2 < 0 && index3 < 0)
			{
				errorMsg.append("“数据归类”未填入有效数据；");
				findError = true;
			}
		}
		else
		{
			errorMsg.append("“数据归类”未填入有效数据；");
			findError = true;
		}

		//投标的施工单位信息，sheet页第2页
		QAxObject * worksheet2 = workbook->querySubObject("WorkSheets(int)", 2); //访问第二个工作表
		if (!worksheet2)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开第二个Sheet页失败。"));
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * usedrange = worksheet2->querySubObject("UsedRange");
		if (!usedrange)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("获取第二个sheet页，用户已使用范围失败。"));
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * rows = usedrange->querySubObject("Rows");
		if (!rows)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("获取第二个sheet页，内容行数失败。"));
			excel->dynamicCall("Quit()");
			return;
		}
		int intRows = rows->property("Count").toInt(); //行数
		if (intRows <= 0)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("获取第二个sheet页，内容行数为0。"));
			excel->dynamicCall("Quit()");
			return;
		}
		QString Range;
		Range.sprintf("B%d:D%d", EXCEL_POS_COMPANY_INFO_START_LINE, intRows);
		QAxObject *allCompanyInfoData = worksheet2->querySubObject("Range(QString)", Range); //读取范围
		QVariant allCompanyInfoDataQVariant = allCompanyInfoData->property("Value");
		QVariantList allCompanyInfoDataList = allCompanyInfoDataQVariant.toList();

		double goodPrice = 0;
		int bid_count = 0;
		for (int i = 0; i < intRows - EXCEL_POS_COMPANY_INFO_START_LINE + 1; ++i)
		{
			QVariantList allCompanyInfoList_i = allCompanyInfoDataList[i].toList();
			QString companyName = allCompanyInfoList_i[0].toString(); //第i行第1列的数据
			QString quotedPrice = allCompanyInfoList_i[1].toString();
			QString proportion = allCompanyInfoList_i[2].toString();
			double checkPrice = quotedPrice.toDouble();
			double checkProportion = proportion.toDouble();
			if (companyName.isEmpty() || quotedPrice.isEmpty() ||
				proportion.isEmpty() || checkPrice <= 0 ||
				checkProportion <= 0)
			{
				break;
			}
			++bid_count;
			double price = allCompanyInfoList_i[1].toDouble();
			if (0 == i)
			{
				goodPrice = price;
				record.goodPrice = quotedPrice;
				record.goodPricePercent = proportion;
			}
			else if (price < goodPrice)
			{
				goodPrice = price;
				record.goodPrice = quotedPrice;
				record.goodPricePercent = proportion;
			}
		}
		if (bid_count <= 0)
		{
			errorMsg.append("投标单位相关信息，未填入有效数据；");
			findError = true;
		}
		if (findError)
		{
			workbooks->dynamicCall("Close(Boolean)", false);
			excel->dynamicCall("Quit()");
			emit updateJobStatusSignal((int)JOB_WARNING, errorMsg);
			return;
		}
		record.bidTotalCount = QString::number(bid_count);
		DBManager::Instance()->storeBidRecords(&record);
		for (int i = 0; i < bid_count; ++i)
		{
			QVariantList allCompanyInfoList_i = allCompanyInfoDataList[i].toList();
			QString companyName = allCompanyInfoList_i[0].toString(); //第i行第1列的数据
			QString quotedPrice = allCompanyInfoList_i[1].toString();
			QString proportion = allCompanyInfoList_i[2].toString();

			DBManager::Instance()->storeBidCompanyInfo(record.openBidDate, companyName, quotedPrice, proportion);
		}

		workbooks->dynamicCall("Close(Boolean)", false);
		excel->dynamicCall("Quit()");
		emit updateJobStatusSignal((int)JOB_FINISH, QString("导入记录成功。"));
	}
	else
	{
		emit updateJobStatusSignal((int)JOB_WARNING, QString("未能创建 Excel 对象，请安装 Microsoft Excel。"));
	}
	
}
int ExcelDataAnalyser::readFile()
{
	start();
	return 0;
}