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
		excel->setProperty("Visible", false); //���ش򿪵�excel�ļ�����
		excel->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
		QAxObject *workbooks = excel->querySubObject("WorkBooks");
		if (!workbooks)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("��ȡ����������ʧ�ܡ�"));
			//QMessageBox::warning(NULL, "����", "��ȡ����������ʧ�ܡ�", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject *workbook = workbooks->querySubObject("Open(QString, QVariant)", filePath.toUtf8().data()); //���ļ�
		if (!workbook)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("�򿪼�¼�ļ�ʧ�ܡ�"));
			//QMessageBox::warning(NULL, "����", "�򿪼�¼�ļ�ʧ�ܡ�", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1); //���ʵ�һ��������
		if (!worksheet)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("�򿪵�һ��Sheetҳʧ�ܡ�"));
			//QMessageBox::warning(NULL, "����", "�򿪵�һ��Sheetҳʧ�ܡ�", QMessageBox::Apply);
			excel->dynamicCall("Quit()");
			return;
		}

		QString errorMsg("����ʧ�ܣ��ļ����ݸ�ʽУ��ʧ�ܣ�ԭ��");
		bool findError = false;
		OpenBidRecords record;
		QString range_ProjectName = EXCEL_POS_PROJECT_NAME;
		QAxObject * projectNameCell = worksheet->querySubObject("Range(QString)", range_ProjectName);
		QVariant projectNameCellValue = projectNameCell->property("Value");
		record.projectName = projectNameCellValue.toString();

		if (record.projectName.isEmpty())
		{
			errorMsg.append("���������ơ�δ������Ч���ݣ�");
			findError = true;
		}

		QString range_OpenBidDate = EXCEL_POS_OPENBID_DATE;
		QAxObject * OpenBidDateCell = worksheet->querySubObject("Range(QString)", range_OpenBidDate);
		QVariant OpenBidDateCellValue = OpenBidDateCell->property("Value");
		//record.openBidDate = OpenBidDateCellValue.toString();
		QString openBidDate = OpenBidDateCellValue.toString();
		if (openBidDate.isEmpty())
		{
			errorMsg.append("������ʱ�䡱δ������Ч���ݣ�");
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
				errorMsg.append("������ʱ�䡱δ������Ч���ݣ�");
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
			errorMsg.append("�����赥λ����������λ��δ������Ч���ݣ�");
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
			int index1 = record.bidBanfa.indexOf("����һ");
			int index2 = record.bidBanfa.indexOf("������");
			if (index1 < 0 && index2 < 0)
			{
				errorMsg.append("������취��δ������Ч���ݣ�");
				findError = true;
			}
		}
		else
		{
			errorMsg.append("������취��δ������Ч���ݣ�");
			findError = true;
		}

		QString range_averagePrice = EXCEL_POS_AVERGAE_PRICE;
		QAxObject * range_averagePriceCell = worksheet->querySubObject("Range(QString)", range_averagePrice);
		QVariant range_averagePriceCellValue = range_averagePriceCell->property("Value");
		record.averagePrice = range_averagePriceCellValue.toString();
		double checkAVEPrice = record.averagePrice.toDouble();
		if (record.averagePrice.isEmpty() || checkAVEPrice <= 0)
		{
			errorMsg.append("��ƽ��ֵ��δ������Ч���ݣ�");
			findError = true;
		}

		QString range_averagePricePercent = EXCEL_POS_AVERAGE_PRICE_PERCENT;
		QAxObject * range_averagePricePercentCell = worksheet->querySubObject("Range(QString)", range_averagePricePercent);
		QVariant range_averagePricePercentCellValue = range_averagePricePercentCell->property("Value");
		record.averagePricePercent = range_averagePricePercentCellValue.toString();
		double checkAVEPercent = record.averagePricePercent.toDouble();
		if (record.averagePricePercent.isEmpty() || checkAVEPercent <= 0)
		{
			errorMsg.append("��ƽ��ֵռ�ȡ�δ������Ч���ݣ�");
			findError = true;
		}

		QString range_BidRule = EXCEL_POS_BID_RULE;
		QAxObject * BidRuleCell = worksheet->querySubObject("Range(QString)", range_BidRule);
		QVariant BidRuleCellValue = BidRuleCell->property("Value");
		record.bidRule = BidRuleCellValue.toString();
// 		if (!record.bidRule.isEmpty())
// 		{
// 			int index1 = record.bidRule.indexOf("����ͼ۷�");
// 			int index2 = record.bidRule.indexOf("������Χ��");
// 			if (index1 < 0 && index2 < 0)
// 			{
// 				errorMsg.append("���б�涨������δ������Ч���ݣ�");
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
// 			int index1 = record.projectManagerRequirement.indexOf("�߼�����ʦ");
// 			int index2 = record.projectManagerRequirement.indexOf("����ʦ");
// 			int index3 = record.projectManagerRequirement.indexOf("һ������ʦ");
// 			int index4 = record.projectManagerRequirement.indexOf("��������ʦ");
// 			if (index1 < 0 && index2 < 0 && index3 < 0 && index4 < 0)
// 			{
// 				errorMsg.append("����Ŀ����Ҫ��δ������Ч���ݣ�");
// 				findError = true;
// 			}
// 		}

		QString range_PerformReq = EXCEL_POS_PERFORMANCE_REQUIREMENT;
		QAxObject * PerformReqCell = worksheet->querySubObject("Range(QString)", range_PerformReq);
		QVariant PerformReqCellValue = PerformReqCell->property("Value");
		QString requirement = PerformReqCellValue.toString();
		QRegExp rx("(\\d+)");//ƥ������
		int indexOfmanager = requirement.indexOf("��Ŀ����");
		int indexOfCompany = requirement.indexOf("��ҵ");
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
			errorMsg.append("�����Ƽۡ�δ������Ч���ݣ�");
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
			int index1 = record.dataKind.indexOf("����һ");
			int index2 = record.dataKind.indexOf("������");
			int index3 = record.dataKind.indexOf("������ɢ");
			if (index1 < 0 && index2 < 0 && index3 < 0)
			{
				errorMsg.append("�����ݹ��ࡱδ������Ч���ݣ�");
				findError = true;
			}
		}
		else
		{
			errorMsg.append("�����ݹ��ࡱδ������Ч���ݣ�");
			findError = true;
		}

		//Ͷ���ʩ����λ��Ϣ��sheetҳ��2ҳ
		QAxObject * worksheet2 = workbook->querySubObject("WorkSheets(int)", 2); //���ʵڶ���������
		if (!worksheet2)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("�򿪵ڶ���Sheetҳʧ�ܡ�"));
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * usedrange = worksheet2->querySubObject("UsedRange");
		if (!usedrange)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("��ȡ�ڶ���sheetҳ���û���ʹ�÷�Χʧ�ܡ�"));
			excel->dynamicCall("Quit()");
			return;
		}
		QAxObject * rows = usedrange->querySubObject("Rows");
		if (!rows)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("��ȡ�ڶ���sheetҳ����������ʧ�ܡ�"));
			excel->dynamicCall("Quit()");
			return;
		}
		int intRows = rows->property("Count").toInt(); //����
		if (intRows <= 0)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("��ȡ�ڶ���sheetҳ����������Ϊ0��"));
			excel->dynamicCall("Quit()");
			return;
		}
		QString Range;
		Range.sprintf("B%d:D%d", EXCEL_POS_COMPANY_INFO_START_LINE, intRows);
		QAxObject *allCompanyInfoData = worksheet2->querySubObject("Range(QString)", Range); //��ȡ��Χ
		QVariant allCompanyInfoDataQVariant = allCompanyInfoData->property("Value");
		QVariantList allCompanyInfoDataList = allCompanyInfoDataQVariant.toList();

		double goodPrice = 0;
		int bid_count = 0;
		for (int i = 0; i < intRows - EXCEL_POS_COMPANY_INFO_START_LINE + 1; ++i)
		{
			QVariantList allCompanyInfoList_i = allCompanyInfoDataList[i].toList();
			QString companyName = allCompanyInfoList_i[0].toString(); //��i�е�1�е�����
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
			errorMsg.append("Ͷ�굥λ�����Ϣ��δ������Ч���ݣ�");
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
			QString companyName = allCompanyInfoList_i[0].toString(); //��i�е�1�е�����
			QString quotedPrice = allCompanyInfoList_i[1].toString();
			QString proportion = allCompanyInfoList_i[2].toString();

			DBManager::Instance()->storeBidCompanyInfo(record.openBidDate, companyName, quotedPrice, proportion);
		}

		workbooks->dynamicCall("Close(Boolean)", false);
		excel->dynamicCall("Quit()");
		emit updateJobStatusSignal((int)JOB_FINISH, QString("�����¼�ɹ���"));
	}
	else
	{
		emit updateJobStatusSignal((int)JOB_WARNING, QString("δ�ܴ��� Excel �����밲װ Microsoft Excel��"));
	}
	
}
int ExcelDataAnalyser::readFile()
{
	start();
	return 0;
}