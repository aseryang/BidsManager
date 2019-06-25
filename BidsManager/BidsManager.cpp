#include "BidsManager.h"
//#include "ExcelDataAnalyser.h"
#include "ExcelDataAnalyserByQtXlsx.h"
#include "DBManager.h"
#include "Controller.h"
#include "ConfigManager.h"
#include <qfiledialog.h>
//#include "ExportDataHelper.h"
#include "ExportDataHelperByQtXlsx.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

BidsManager::BidsManager(QWidget *parent)
	: QMainWindow(parent), panelMethodDuringTime_LineChart(NULL), 
	panelMethodCounts_PieChart(NULL), 
	panelAveragePrice_LineChart(NULL),
	panelFirstMethod_PieChart(NULL),
	panelSecondMethod_PieChart(NULL)
{

	mainWidget = new QWidget;
	rightWidget = new QWidget;
	rightLayout = new QVBoxLayout(rightWidget);
	mainLayout = new QHBoxLayout(mainWidget);
	navigationWidget = new QNavigationWidget();
	tipsLabel = new QLabel("Item: 0");

	rightWidget->setFixedWidth(mainWidget->width() - navigationWidget->width());
	navigationWidget->setRowHeight(50);
	navigationWidget->addItem(PROJECT_JINGGUAN);
	navigationWidget->addItem(PROJECT_SHIZHENG);
	navigationWidget->addItem(PROJECT_SHUILI);
	navigationWidget->addItem(PROJECT_ZHUAGNSHI);
	navigationWidget->addItem(PROJECT_HUANBAO);
	connect(navigationWidget, SIGNAL(currentItemChanged(const int &)), this, SLOT(onProjectSwitch(const int&)));
	rightLayout->addWidget(tipsLabel, 0, Qt::AlignCenter);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(rightWidget);

	ui.setupUi(this);
	QHBoxLayout * horizontalLayout = new QHBoxLayout(ui.centralWidget);
	horizontalLayout->setSpacing(6);
	horizontalLayout->setContentsMargins(11, 11, 11, 11);
	horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
	horizontalLayout->addWidget(navigationWidget);
	horizontalLayout->addWidget(ui.MainPage);

	ui.statusBar->showMessage("就绪");

	curStatus = STAT_NORMAL;
	companyType = CompanyType_SelfCompany;
	uiSpecialSetting();
	navigationWidget->setCurrentIndex(ConfigManager::Instance()->getProjectType());
	onProjectSwitch(ConfigManager::Instance()->getProjectType());
	qRegisterMetaType<UI_UpdateMsg_ID>("UI_UpdateMsg_ID");
	connect(Controller::Instance(), SIGNAL(jobFinishSignal()), this, SLOT(jobFinished()));
	connect(Controller::Instance(), SIGNAL(updateUISignal(void*, UI_UpdateMsg_ID)), this, SLOT(onUpdateUI(void*, UI_UpdateMsg_ID)));
	connect(ExcelDataAnalyserByQtXlsx::Instance(), SIGNAL(updateJobStatusSignal(int, QString)), this, SLOT(onUpdateJobStatus(int, QString)));
	connect(ExportDataHelperByQtXlsx::Instance(), SIGNAL(updateJobStatusSignal(int, QString)), this, SLOT(onUpdateJobStatus(int, QString)));
}
void BidsManager::bidwayButtonsClicked(int id)
{
	//手动设置组中其他RadioButton的状态
	if (bidWayBtnGroup->button(id)->isChecked())
	{
		for (int i = 1; i < 7; ++i)
		{
			if (i != id)
			{
				bidWayBtnGroup->button(i)->setChecked(false);
			}
		}
	}
}
void BidsManager::mgrReqButtonsClicked(int id)
{
	//手动设置组中其他RadioButton的状态
	if (managerReqBtnGroup->button(id)->isChecked())
	{
		for (int i = 1; i < 6; ++i)
		{
			if (i != id)
			{
				managerReqBtnGroup->button(i)->setChecked(false);
			}
		}
	}
}
void BidsManager::uiSpecialSetting()
{
	ui.self_le_qiyeyeji_begin->setEnabled(false);
	ui.self_le_qiyeyeji_end->setEnabled(false);
	ui.self_le_xiangmujinliyeji_begin->setEnabled(false);
	ui.self_le_xiangmujinliyeji_end->setEnabled(false);
	ui.self_le_places->setEnabled(false);
	ui.Self_dateEdit_start->setCalendarPopup(true);
	ui.Self_dateEdit_end->setCalendarPopup(true);
	ui.other_dateedit_start->setCalendarPopup(true);
	ui.other_dateedit_end->setCalendarPopup(true);
	ui.proxy_dateedit_start->setCalendarPopup(true);
	ui.proxy_dateedit_end->setCalendarPopup(true);
	bidWayBtnGroup = new QButtonGroup(this);
	bidWayBtnGroup->addButton(ui.self_rb_helidijiafa, 1);
	bidWayBtnGroup->addButton(ui.self_rb_pingbiaoruweifa, 2);
	bidWayBtnGroup->addButton(ui.self_rb_junzhiruweifa, 3);
	bidWayBtnGroup->addButton(ui.self_rb_dijiapaixufa, 4);
	bidWayBtnGroup->addButton(ui.self_rb_otherfangfa, 5);
	bidWayBtnGroup->addButton(ui.self_rb_quanburuweifa, 6);
	bidWayBtnGroup->setExclusive(false);
	managerReqBtnGroup = new QButtonGroup(this);
	managerReqBtnGroup->addButton(ui.self_rb_gaojigongchengshi, 1);
	managerReqBtnGroup->addButton(ui.self_rb_gonchengshi, 2);
	managerReqBtnGroup->addButton(ui.self_rb_yijijianzaoshi, 3);
	managerReqBtnGroup->addButton(ui.self_rb_erjijianzaoshi, 4);
	managerReqBtnGroup->addButton(ui.self_rb_mgrOtherReq, 5);
	managerReqBtnGroup->setExclusive(false);
	connect(bidWayBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(bidwayButtonsClicked(int)));
	connect(managerReqBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(mgrReqButtonsClicked(int)));

	QRegExp regx("[0-9]+$");
	QValidator *minPriceVal = new QRegExpValidator(regx, ui.self_le_controlPrice_begin);
	ui.self_le_controlPrice_begin->setValidator(minPriceVal);
	QValidator *maxPriceVal = new QRegExpValidator(regx, ui.self_le_controlPrice_end);
	ui.self_le_controlPrice_end->setValidator(maxPriceVal);
	QValidator *minCompanyVal = new QRegExpValidator(regx, ui.self_le_qiyeyeji_begin);
	ui.self_le_qiyeyeji_begin->setValidator(minCompanyVal);
	QValidator *maxCompanyVal = new QRegExpValidator(regx, ui.self_le_qiyeyeji_end);
	ui.self_le_qiyeyeji_end->setValidator(maxCompanyVal);
	QValidator *minManagerVal = new QRegExpValidator(regx, ui.self_le_xiangmujinliyeji_begin);
	ui.self_le_xiangmujinliyeji_begin->setValidator(minManagerVal);
	QValidator *maxManagerVal = new QRegExpValidator(regx, ui.self_le_xiangmujinliyeji_end);
	ui.self_le_xiangmujinliyeji_end->setValidator(maxManagerVal);

	uiSetInterval();
	//创建空数据图
	panelMethodCounts_PieChart = new ImagePanel();
	ui.MethodCountsLayout->addWidget(panelMethodCounts_PieChart);
	MethodCounts nullData_MethodCounts;
	panelMethodCounts_PieChart->drawMethodCountPieChart(&nullData_MethodCounts);

	panelFirstMethod_PieChart = new ImagePanel();
	ui.APMethodOnePieLayout->addWidget(panelFirstMethod_PieChart);
	QVector<AveragePriceIntervalPercent*>  nullData_AVEMethodOne;
	panelFirstMethod_PieChart->drawAVEMethodOnePieChart(&nullData_AVEMethodOne);

	panelSecondMethod_PieChart = new ImagePanel();
	ui.APMethodTwoPieLayout->addWidget(panelSecondMethod_PieChart);
	QVector<AveragePriceIntervalPercent*> nullData_AVEMethodTwo;
	panelSecondMethod_PieChart->drawAVEMethodTwoPieChart(&nullData_AVEMethodTwo);
}
void BidsManager::uiSetInterval()
{
	baojiaqujianArray.push_back(ui.self_le_quanjian);
	baojiaqujianArray.push_back(ui.self_le_quanjian_2);
	baojiaqujianArray.push_back(ui.self_le_quanjian_3);
	baojiaqujianArray.push_back(ui.self_le_quanjian_4);
	baojiaqujianArray.push_back(ui.self_le_quanjian_5);
	baojiaqujianArray.push_back(ui.self_le_quanjian_6);
	baojiaqujianArray.push_back(ui.self_le_quanjian_7);
	baojiaqujianArray.push_back(ui.self_le_quanjian_8);
	baojiaqujianArray.push_back(ui.self_le_quanjian_9);
	baojiaqujianArray.push_back(ui.self_le_quanjian_10);
	baojiaqujianArray.push_back(ui.self_le_quanjian_11);
	baojiaqujianArray.push_back(ui.self_le_quanjian_12);
	baojiaqujianArray.push_back(ui.self_le_quanjian_13);
	baojiaqujianArray.push_back(ui.self_le_quanjian_14);
	baojiaqujianArray.push_back(ui.self_le_quanjian_15);
	baojiaqujianArray.push_back(ui.self_le_quanjian_16);

	methodOneQujianArray.push_back(ui.le_MethodOne);
	methodOneQujianArray.push_back(ui.le_MethodOne2);
	methodOneQujianArray.push_back(ui.le_MethodOne3);
	methodOneQujianArray.push_back(ui.le_MethodOne4);
	methodOneQujianArray.push_back(ui.le_MethodOne5);
	methodOneQujianArray.push_back(ui.le_MethodOne6);
	methodOneQujianArray.push_back(ui.le_MethodOne7);
	methodOneQujianArray.push_back(ui.le_MethodOne8);
	methodOneQujianArray.push_back(ui.le_MethodOne9);
	methodOneQujianArray.push_back(ui.le_MethodOne10);
	methodOneQujianArray.push_back(ui.le_MethodOne11);
	methodOneQujianArray.push_back(ui.le_MethodOne12);
	methodOneQujianArray.push_back(ui.le_MethodOne13);
	methodOneQujianArray.push_back(ui.le_MethodOne14);
	methodOneQujianArray.push_back(ui.le_MethodOne15);
	methodOneQujianArray.push_back(ui.le_MethodOne16);

	methodTwoQujianArray.push_back(ui.le_MethodTwo);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_2);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_3);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_4);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_5);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_6);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_7);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_8);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_9);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_10);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_11);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_12);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_13);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_14);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_15);
	methodTwoQujianArray.push_back(ui.le_MethodTwo_16);

	for (int i = 0; i < TOTAL_INTERVAL_PIECES_COUNT; ++i)
	{
		//设置lineedit，只能输入0，99的数字
		baojiaqujianArray.at(i)->setValidator(new QIntValidator(1, 99, this));
		methodOneQujianArray.at(i)->setValidator(new QIntValidator(1, 99, this));
		methodTwoQujianArray.at(i)->setValidator(new QIntValidator(1, 99, this));
	}
}
void BidsManager::uiReset()
{
	ui.self_le_otherfangfa->setEnabled(false);
	ui.self_le_mgrReqOther->setEnabled(false);
	ui.self_rb_quanburuweifa->setChecked(false);
	ui.self_rb_junzhiruweifa->setChecked(false);
	ui.self_rb_dijiapaixufa->setChecked(false);
	ui.self_rb_otherfangfa->setChecked(false);
	ui.self_rb_mgrOtherReq->setChecked(false);
	ui.self_cb_gaochun->setChecked(false);
	ui.self_cb_lishui->setChecked(false);
	ui.self_cb_nanjing->setChecked(false);
	ui.self_cb_liuhe->setChecked(false);
	ui.self_cb_pukou->setChecked(false);
	ui.self_cb_other->setChecked(false);
	ui.self_rb_erjijianzaoshi->setChecked(false);
	ui.self_rb_yijijianzaoshi->setChecked(false);
	ui.self_rb_gaojigongchengshi->setChecked(false);
	ui.self_rb_gonchengshi->setChecked(false);
	ui.self_rb_helidijiafa->setChecked(false);
	ui.self_rb_pingbiaoruweifa->setChecked(false);
	ui.self_cb_qiyeyeji->setChecked(false);
	ui.self_cb_xiangmujinliyeji->setChecked(false);
	ui.self_le_places->setText(NULL);
	ui.self_le_controlPrice_begin->setText(NULL);
	ui.self_le_controlPrice_end->setText(NULL);
	ui.self_le_qiyeyeji_begin->setText(NULL);
	ui.self_le_qiyeyeji_end->setText(NULL);
	ui.self_le_xiangmujinliyeji_begin->setText(NULL);
	ui.self_le_xiangmujinliyeji_end->setText(NULL);
	ui.other_le_companyName->setText(NULL);
	ui.proxy_le_companyName->setText(NULL);
	ui.self_le_otherfangfa->setText(NULL);
	ui.self_le_mgrReqOther->setText(NULL);
	for (int i = 0; i < TOTAL_INTERVAL_PIECES_COUNT; ++i)
	{
		baojiaqujianArray.at(i)->setText(NULL);
		methodOneQujianArray.at(i)->setText(NULL);
		methodTwoQujianArray.at(i)->setText(NULL);
	}
}
void BidsManager::onUpdateJobStatus(int status, QString info)
{
	switch (status)
	{
	case JOB_FINISH:
	{
		ui.statusBar->showMessage("导入记录完成。");
		QMessageBox::information(this, "完成", info);
		break;
	}
	case JOB_WARNING:
	{
		ui.statusBar->showMessage(info);
		QMessageBox::warning(NULL, "错误", info, "确定"/*QMessageBox::Apply*/);
		break;
	}
	case JOB_QUESTION:
	{
		ui.statusBar->showMessage("导出记录完成。");
		if (QMessageBox::question(NULL, "完成", "文件已经导出，是否现在打开？", "是", "否") == 0)
		{
			QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(info)));
		}
		break;
	}
	}
	enterStat(STAT_NORMAL);
}
void BidsManager::onProjectSwitch(const int& projectId)
{
	ConfigManager::Instance()->setProjectType((ProjectType)projectId);
	ConfigManager::Instance()->loadINIFile();
	DBManager::Instance()->init();
	UserConfig* cfg = ConfigManager::Instance()->getCurSavedUserConfig();
	uiReset();
	switch ((ProjectType)projectId)
	{
	case PROJECT_TYPE_JINGGUANG:
		ui.MainPage->setTitle(PROJECT_JINGGUAN);
		break;
	case PROJECT_TYPE_SHIZHENG:
		ui.MainPage->setTitle(PROJECT_SHIZHENG);
		break;
	case PROJECT_TYPE_SHUILI:
		ui.MainPage->setTitle(PROJECT_SHUILI);
		break;
	case PROJECT_TYPE_ZHUANGSHI:
		ui.MainPage->setTitle(PROJECT_ZHUAGNSHI);
		break;
	case PROJECT_TYPE_HUANBAO:
		ui.MainPage->setTitle(PROJECT_HUANBAO);
		break;
	}
	if (!cfg->selfStartDate.isEmpty())
	{
		ui.Self_dateEdit_start->setDate(QDate::fromString(cfg->selfStartDate, "yyyy-MM-dd"));
	}
	if (!cfg->selfEndDate.isEmpty())
	{
		ui.Self_dateEdit_end->setDate(QDate::fromString(cfg->selfEndDate, "yyyy-MM-dd"));
	}
	if (!cfg->selfProjectLocation.isEmpty())
	{
		QStringList list = cfg->selfProjectLocation.split(',');
		QString otherPlaces;
		for (int i = 0; i < list.size(); ++i)
		{		
			if (!list.at(i).isEmpty())
			{
				if (list.at(i).compare("南京市区") == 0)
				{
					ui.self_cb_nanjing->setChecked(true);
				}
				else if (list.at(i).compare("高淳") == 0)
				{
					ui.self_cb_gaochun->setChecked(true);
				}
				else if (list.at(i).compare("溧水") == 0)
				{
					ui.self_cb_lishui->setChecked(true);
				}
				else if (list.at(i).compare("六合") == 0)
				{
					ui.self_cb_liuhe->setChecked(true);
				}
				else if (list.at(i).compare("浦口") == 0)
				{
					ui.self_cb_pukou->setChecked(true);
				}
				else
				{
					otherPlaces += list.at(i);
					if (i < list.size() - 1)
					{
						otherPlaces += ",";
					}
				}
			}
		}
		if (!otherPlaces.isEmpty())
		{
			ui.self_cb_other->setChecked(true);
			ui.self_le_places->setText(otherPlaces);
		}
	}
	if (!cfg->controlPriceMin.isEmpty())
	{
		ui.self_le_controlPrice_begin->setText(cfg->controlPriceMin);
	}
	if (!cfg->controlPriceMax.isEmpty())
	{
		ui.self_le_controlPrice_end->setText(cfg->controlPriceMax);
	}
	if (!cfg->bidWay.isEmpty())
	{
		if (cfg->bidWay.compare("合理低价法") == 0)
		{
			ui.self_rb_helidijiafa->setChecked(true);
		}
		else if (cfg->bidWay.compare("评标入围法") == 0)
		{
			ui.self_rb_pingbiaoruweifa->setChecked(true);
		}
		else if (cfg->bidWay.compare("均值入围法") == 0)
		{
			ui.self_rb_junzhiruweifa->setChecked(true);
		}
		else if (cfg->bidWay.compare("低价排序法") == 0)
		{
			ui.self_rb_dijiapaixufa->setChecked(true);
		}
		else if (cfg->bidWay.compare("全部入围法") == 0)
		{
			ui.self_rb_quanburuweifa->setChecked(true);
		}
		else
		{
			ui.self_rb_otherfangfa->setChecked(true);
			ui.self_le_otherfangfa->setText(cfg->bidWay);
		}
	}
	if (!cfg->projectManagerRequest.isEmpty())
	{
		if (cfg->projectManagerRequest.compare("高级工程师") == 0)
		{
			ui.self_rb_gaojigongchengshi->setChecked(true);
		}
		else if (cfg->projectManagerRequest.compare("工程师") == 0)
		{
			ui.self_rb_gonchengshi->setChecked(true);
		}
		else if (cfg->projectManagerRequest.compare("一级建造师") == 0)
		{
			ui.self_rb_yijijianzaoshi->setChecked(true);
		}
		else if (cfg->projectManagerRequest.compare("二级建造师") == 0)
		{
			ui.self_rb_erjijianzaoshi->setChecked(true);
		}
		else if (!cfg->projectManagerRequest.isEmpty())
		{
			ui.self_rb_mgrOtherReq->setChecked(true);
			ui.self_le_mgrReqOther->setText(cfg->projectManagerRequest);
		}
	}
	if (!cfg->companyPerformance.isEmpty())
	{
		ui.self_cb_qiyeyeji->setChecked(true);
		QStringList list = cfg->companyPerformance.split('-');
		if (list.size() == 2)
		{
			if (!list.at(0).isEmpty())
			{
				ui.self_le_qiyeyeji_begin->setText(list.at(0));
			}
			if (!list.at(1).isEmpty())
			{
				ui.self_le_qiyeyeji_end->setText(list.at(1));
			}
		}
	}
	if (!cfg->projectManagerPerformance.isEmpty())
	{
		ui.self_cb_xiangmujinliyeji->setChecked(true);
		QStringList list = cfg->projectManagerPerformance.split('-');
		if (list.size() == 2)
		{
			if (!list.at(0).isEmpty())
			{
				ui.self_le_xiangmujinliyeji_begin->setText(list.at(0));
			}
			if (!list.at(1).isEmpty())
			{
				ui.self_le_xiangmujinliyeji_end->setText(list.at(1));
			}
		}
	}
	if (!cfg->baojiaqujian.isEmpty())
	{
		setInterval(cfg->baojiaqujian, TNTERVAL_BAOJIA);
	}
	if (!cfg->AVEPriceIntervalPercentONE.isEmpty())
	{
		setInterval(cfg->AVEPriceIntervalPercentONE, INTERVAL_METHODONE);
	}
	if (!cfg->AVEPriceIntervalPercentTWO.isEmpty())
	{
		setInterval(cfg->AVEPriceIntervalPercentTWO, TNTERVAL_METHODTWO);
	}
	if (!cfg->otherCompanyName.isEmpty())
	{
		ui.other_le_companyName->setText(cfg->otherCompanyName);
	}
	if (!cfg->otherStartDate.isEmpty())
	{
		ui.other_dateedit_start->setDate(QDate::fromString(cfg->otherStartDate, "yyyy-MM-dd"));
	}
	if (!cfg->otherEndDate.isEmpty())
	{
		ui.other_dateedit_end->setDate(QDate::fromString(cfg->otherEndDate, "yyyy-MM-dd"));
	}
	if (!cfg->proxyCompanyName.isEmpty())
	{
		ui.proxy_le_companyName->setText(cfg->proxyCompanyName);
	}
	if (!cfg->proxyStartDate.isEmpty())
	{
		ui.proxy_dateedit_start->setDate(QDate::fromString(cfg->proxyStartDate, "yyyy-MM-dd"));
	}
	if (!cfg->proxyEndDate.isEmpty())
	{
		ui.proxy_dateedit_end->setDate(QDate::fromString(cfg->proxyEndDate, "yyyy-MM-dd"));
	}
}
void BidsManager::setInterval(QString& interval, IntervalType type)
{
	QVector<QLineEdit *>* array_Ptr = NULL;
	switch (type)
	{
	case TNTERVAL_BAOJIA:
		array_Ptr = &baojiaqujianArray;
		break;
	case INTERVAL_METHODONE:
		array_Ptr = &methodOneQujianArray;
		break;
	case TNTERVAL_METHODTWO:
		array_Ptr = &methodTwoQujianArray;
		break;
	}
	QRegExp rx("(\\d+)");//匹配数字
	int pos = 0;
	int index = 0;
	while ((pos = rx.indexIn(interval, pos)) != -1)
	{

		array_Ptr->at(index)->setText(rx.cap(0));
		pos += rx.matchedLength();
		++index;
	}
}
QString BidsManager::getIntervalParam(IntervalType type)
{
	QString interval;
	switch (type)
	{
	case TNTERVAL_BAOJIA:
	{
		for (int i = 0; i < TOTAL_INTERVAL_PIECES_COUNT - 1; ++i)
		{
			if (baojiaqujianArray.at(i)->text().isEmpty() || baojiaqujianArray.at(i+1)->text().isEmpty())
			{
				continue;
			}
			if (!interval.isEmpty())
			{
				interval.append(',');
			}
			interval.append(baojiaqujianArray.at(i)->text());
			interval.append('-');
			interval.append(baojiaqujianArray.at(i + 1)->text());
			i += 1;
		}
	}
		break;
	case INTERVAL_METHODONE:
	{
		for (int i = 0; i < TOTAL_INTERVAL_PIECES_COUNT - 1; ++i)
		{
			if (methodOneQujianArray.at(i)->text().isEmpty() || methodOneQujianArray.at(i + 1)->text().isEmpty())
			{
				continue;
			}
			if (!interval.isEmpty())
			{
				interval.append(',');
			}
			interval.append(methodOneQujianArray.at(i)->text());
			interval.append('-');
			interval.append(methodOneQujianArray.at(i + 1)->text());
			i += 1;
		}
	}
		break;
	case TNTERVAL_METHODTWO:
	{
		for (int i = 0; i < TOTAL_INTERVAL_PIECES_COUNT - 1; ++i)
		{
			if (methodTwoQujianArray.at(i)->text().isEmpty() || methodTwoQujianArray.at(i + 1)->text().isEmpty())
			{
				continue;
			}
			if (!interval.isEmpty())
			{
				interval.append(',');
			}
			interval.append(methodTwoQujianArray.at(i)->text());
			interval.append('-');
			interval.append(methodTwoQujianArray.at(i + 1)->text());
			i += 1;
		}
	}
		break;
	default:
		break;
	}
	return checkIntervalParam(interval);
}
QString BidsManager::checkIntervalParam(QString& src)
{
	if (src.isEmpty())
	{
		return src;
	}
	QString dst;
	QRegExp rx("(\\d+)");//匹配数字
	int pos = 0;
	int numberCount = 0;
	bool isNumberInOrder = true;
	double min = 0;
	while ((pos = rx.indexIn(src, pos)) != -1)
	{
		++numberCount;
		double num = rx.cap(0).toDouble();
		if (num < min)
		{
			isNumberInOrder = false;
			break;
		}
		else 
		{
			min = num;
		}
		pos += rx.matchedLength();
	}
	if (numberCount % 2 != 0 || !isNumberInOrder)
	{
		QString errorInfo;
		errorInfo.sprintf("区间： %s 输入格式无效！", src.toUtf8().data());
		QMessageBox::warning(NULL, "错误", errorInfo, "确定");
		return dst;
	}

	QStringList list = src.split(',');
	for (int i = 0; i < list.size(); ++i)
	{
		if (list.at(i).isEmpty())
		{
			continue;
		}
		QStringList numList = list.at(i).split('-');
		if (numList.size() == 2)
		{
			int min = numList.at(0).toInt();
			int max = numList.at(1).toInt();
			if (min > 0 && min < 100 && max > 0 && max < 100)
			{
				if (!dst.isEmpty())
				{
					dst.append(",");
				}
				dst.append(list.at(i));
			}
		}
	}
	if (dst.isEmpty())
	{
		QString errorInfo;
		errorInfo.sprintf("区间： %s 输入格式无效！", src.toUtf8().data());
		QMessageBox::warning(NULL, "错误", errorInfo, QMessageBox::Apply);
	}
	return dst;
}
void BidsManager::getCurQueryConfig()
{
	UserConfig* cfg = ConfigManager::Instance()->getCurSavedUserConfig();

	QString projectLocation;
	if (ui.self_cb_nanjing->isChecked())
	{
		projectLocation += "南京市区";
	}
	if (ui.self_cb_gaochun->isChecked())
	{
		if (!projectLocation.isEmpty())
		{
			projectLocation.append(",");
		}
		projectLocation += "高淳";
	}
	if (ui.self_cb_lishui->isChecked())
	{
		if (!projectLocation.isEmpty())
		{
			projectLocation.append(",");
		}
		projectLocation += "溧水";
	}
	if (ui.self_cb_liuhe->isChecked())
	{
		if (!projectLocation.isEmpty())
		{
			projectLocation.append(",");
		}
		projectLocation += "六合";
	}
	if (ui.self_cb_pukou->isChecked())
	{
		if (!projectLocation.isEmpty())
		{
			projectLocation.append(",");
		}
		projectLocation += "浦口";
	}
	if (ui.self_cb_other->isChecked() && !ui.self_le_places->text().isEmpty())
	{
		QString otherPlaces =  ui.self_le_places->text().replace("，", ",");
		QStringList list = otherPlaces.split(',');
		for (int i = 0; i < list.size(); ++i)
		{
			if (!list.at(i).isEmpty())
			{
				if (!projectLocation.isEmpty())
				{
					projectLocation.append(",");
				}
				projectLocation.append(list.at(i));
			}
			else
			{
				continue;
			}
		}
	}

	QString bidWay;
	QString bidWayReal;
	if (ui.self_rb_helidijiafa->isChecked())
	{
		bidWay = "合理低价法";
	}
	else if (ui.self_rb_pingbiaoruweifa->isChecked())
	{
		bidWay = "评标入围法";
	}
	else if (ui.self_rb_dijiapaixufa->isChecked())
	{
		bidWay = "低价排序法";
	}
	else if (ui.self_rb_quanburuweifa->isChecked())
	{
		bidWay = "全部入围法";
	}
	else if (ui.self_rb_junzhiruweifa->isChecked())
	{
		bidWay = "均值入围法";
	}
	else if (ui.self_rb_otherfangfa->isChecked())
	{
		bidWay = ui.self_le_otherfangfa->text().toUtf8().data();
	}
	QString projectManagerRequest;
	if (ui.self_rb_gaojigongchengshi->isChecked())
	{
		projectManagerRequest.sprintf("%s", "高级工程师");
	}
	else if (ui.self_rb_gonchengshi->isChecked())
	{
		projectManagerRequest.sprintf("%s", "工程师");
	}
	else if (ui.self_rb_yijijianzaoshi->isChecked())
	{
		projectManagerRequest.sprintf("%s", "一级建造师");
	}
	else if (ui.self_rb_erjijianzaoshi->isChecked())
	{
		projectManagerRequest.sprintf("%s", "二级建造师");
	}
	else if (ui.self_rb_mgrOtherReq->isChecked())
	{
		projectManagerRequest.append(ui.self_le_mgrReqOther->text().toUtf8().data());
	}

	QString companyPerformance;
	if (ui.self_cb_qiyeyeji->isChecked())
	{
		if (!ui.self_le_qiyeyeji_begin->text().isEmpty() && !ui.self_le_qiyeyeji_end->text().isEmpty())
		{
			companyPerformance.sprintf("%s-%s", ui.self_le_qiyeyeji_begin->text().toUtf8().data(), ui.self_le_qiyeyeji_end->text().toUtf8().data());
		}
	}
	QString projectManagerPerformance;
	if (ui.self_cb_xiangmujinliyeji->isChecked())
	{
		if (!ui.self_le_xiangmujinliyeji_begin->text().isEmpty() && !ui.self_le_xiangmujinliyeji_end->text().isEmpty())
		{
			projectManagerPerformance.sprintf("%s-%s", ui.self_le_xiangmujinliyeji_begin->text().toUtf8().data(), ui.self_le_xiangmujinliyeji_end->text().toUtf8().data());
		}
	}
	QString baojiaqujian;
	baojiaqujian = getIntervalParam(TNTERVAL_BAOJIA);

	cfg->selfStartDate = ui.Self_dateEdit_start->date().toString("yyyy-MM-dd").toUtf8().data();
	cfg->selfEndDate = ui.Self_dateEdit_end->date().toString("yyyy-MM-dd").toUtf8().data();
	cfg->selfProjectLocation = projectLocation;
	cfg->controlPriceMin = ui.self_le_controlPrice_begin->text().toUtf8().data();
	cfg->controlPriceMax = ui.self_le_controlPrice_end->text().toUtf8().data();
	cfg->bidWay = bidWay;
	cfg->bidWayReal = bidWayReal;
	cfg->projectManagerRequest = projectManagerRequest;
	cfg->companyPerformance = companyPerformance;
	cfg->projectManagerPerformance = projectManagerPerformance;
	cfg->baojiaqujian = baojiaqujian;
	cfg->otherCompanyName = ui.other_le_companyName->text().toUtf8().data();
	cfg->otherStartDate = ui.other_dateedit_start->date().toString("yyyy-MM-dd").toUtf8().data();
	cfg->otherEndDate = ui.other_dateedit_end->date().toString("yyyy-MM-dd").toUtf8().data();
	cfg->proxyCompanyName = ui.proxy_le_companyName->text().toUtf8().data();
	cfg->proxyStartDate = ui.proxy_dateedit_start->date().toString("yyyy-MM-dd").toUtf8().data();
	cfg->proxyEndDate = ui.proxy_dateedit_end->date().toString("yyyy-MM-dd").toUtf8().data();
	QString methodoneInterval = getIntervalParam(INTERVAL_METHODONE);
	QString defaultAVEInterval(DEFAULT_AVERAGE_PRICE_INTERVAL);
	if (!methodoneInterval.isEmpty())
	{
		cfg->AVEPriceIntervalPercentONE = methodoneInterval;
		cfg->otherAVEPriceIntervalPercentONE = methodoneInterval;
		cfg->proxyAVEPriceIntervalPercentONE = methodoneInterval;
	}
	else
	{
		cfg->AVEPriceIntervalPercentONE = defaultAVEInterval;
		cfg->otherAVEPriceIntervalPercentONE = defaultAVEInterval;
		cfg->proxyAVEPriceIntervalPercentONE = defaultAVEInterval;
	}
	QString methodTwoInterval = getIntervalParam(TNTERVAL_METHODTWO);
	if (!methodTwoInterval.isEmpty())
	{
		cfg->AVEPriceIntervalPercentTWO = methodTwoInterval;
		cfg->otherAVEPriceIntervalPercentTWO = methodTwoInterval;
		cfg->proxyAVEPriceIntervalPercentTWO = methodTwoInterval;
	}
	else
	{
		cfg->AVEPriceIntervalPercentTWO = defaultAVEInterval;
		cfg->otherAVEPriceIntervalPercentTWO = defaultAVEInterval;
		cfg->proxyAVEPriceIntervalPercentTWO = defaultAVEInterval;
	}
	ConfigManager::Instance()->saveUserCfgFile();
}
void BidsManager::jobFinished()
{
	enterStat(STAT_NORMAL);
}
void BidsManager::onCheckboxStateChanged(bool checked)
{
	if (checked)
	{
		ui.self_le_places->setEnabled(true);
	}
	else
	{
		ui.self_le_places->setEnabled(false);
	}
}
void BidsManager::onRB_helidijiafaStateChanged(bool checked)
{
	if (checked)
	{
		//ui.self_le_helidijiafa_real->setEnabled(true);
	}
	else
	{
		//ui.self_le_helidijiafa_real->setEnabled(false);
	}
}
void BidsManager::onRB_pingbiaoruweifaStateChanged(bool checked)
{
	if (checked)
	{
		//ui.self_le_pingbiaoruweifa_real->setEnabled(true);
	}
	else
	{
		//ui.self_le_pingbiaoruweifa_real->setEnabled(false);
	}
}
void BidsManager::onRB_otherbidwayStateChanged(bool checked)
{
	if (checked)
	{
		ui.self_le_otherfangfa->setEnabled(true);
	}
	else
	{
		ui.self_le_otherfangfa->setEnabled(false);
	}
}
void BidsManager::onRB_mgrReqOtherStateChanged(bool checked)
{
	if (checked)
	{
		ui.self_le_mgrReqOther->setEnabled(true);
	}
	else
	{
		ui.self_le_mgrReqOther->setEnabled(false);
	}
}
void BidsManager::onCB_qiyeyejiStateChanged(bool checked)
{
	if (checked)
	{
		ui.self_le_qiyeyeji_begin->setEnabled(true);
		ui.self_le_qiyeyeji_end->setEnabled(true);
	}
	else
	{
		ui.self_le_qiyeyeji_begin->setEnabled(false);
		ui.self_le_qiyeyeji_end->setEnabled(false);
	}
}
void BidsManager::onCB_jingliyejiStateChanged(bool checked)
{
	if (checked)
	{
		ui.self_le_xiangmujinliyeji_begin->setEnabled(true);
		ui.self_le_xiangmujinliyeji_end->setEnabled(true);
	}
	else
	{
		ui.self_le_xiangmujinliyeji_begin->setEnabled(false);
		ui.self_le_xiangmujinliyeji_end->setEnabled(false);
	}
}
void BidsManager::onUpdateUI(void* data, UI_UpdateMsg_ID id)
{
	switch (id)
	{
	case UI_UPDATE_LINE_CHART:
	{
		if (panelMethodDuringTime_LineChart)
		{
			ui.MeThodDuringTimeLayout->removeWidget(panelMethodDuringTime_LineChart);
			delete panelMethodDuringTime_LineChart;
			panelMethodDuringTime_LineChart = NULL;
		}
		panelMethodDuringTime_LineChart = ChartCreateHelper::Instance()->createMethodLineChart((QVector<MethodDuringTime*>*)data);
		ui.MeThodDuringTimeLayout->addWidget(panelMethodDuringTime_LineChart);
		break;
	}
	case UI_UPDATE_METHOD_PIE_CHART:
	{
		if (panelMethodCounts_PieChart)
		{
			ui.MethodCountsLayout->removeWidget(panelMethodCounts_PieChart);
			delete panelMethodCounts_PieChart;
			panelMethodCounts_PieChart = NULL;
		}
		panelMethodCounts_PieChart = new ImagePanel();
		ui.MethodCountsLayout->addWidget(panelMethodCounts_PieChart);
		panelMethodCounts_PieChart->drawMethodCountPieChart((MethodCounts*)data);
		break;
	}
	case UI_UPDATE_AVERAGE_PRICE_LINE_CHART:
	{
		if (panelAveragePrice_LineChart)
		{
			ui.AveragePriceLineLayout->removeWidget(panelAveragePrice_LineChart);
			delete panelAveragePrice_LineChart;
			panelAveragePrice_LineChart = NULL;
		}
		panelAveragePrice_LineChart = ChartCreateHelper::Instance()->createAVELineChart((QVector<AveragePricePercent*>*)data);
		ui.AveragePriceLineLayout->addWidget(panelAveragePrice_LineChart);
		break;
	}
	case UI_UPDATE_METHOD_FIRST_PIE_CHART:
	{
		if (panelFirstMethod_PieChart)
		{
			ui.APMethodOnePieLayout->removeWidget(panelFirstMethod_PieChart);
			delete panelFirstMethod_PieChart;
			panelFirstMethod_PieChart = NULL;
		}
		panelFirstMethod_PieChart = new ImagePanel(); 
		panelFirstMethod_PieChart->drawAVEMethodOnePieChart((QVector<AveragePriceIntervalPercent*>*)data);
		ui.APMethodOnePieLayout->addWidget(panelFirstMethod_PieChart);
		break;
	}
		
	case UI_UPDATE_METHOD_SECOND_PIE_CHART:
	{
		if (panelSecondMethod_PieChart)
		{
			ui.APMethodTwoPieLayout->removeWidget(panelSecondMethod_PieChart);
			delete panelSecondMethod_PieChart;
			panelSecondMethod_PieChart = NULL;
		}
		panelSecondMethod_PieChart = new ImagePanel();
		panelSecondMethod_PieChart->drawAVEMethodTwoPieChart((QVector<AveragePriceIntervalPercent*>*)data);
		ui.APMethodTwoPieLayout->addWidget(panelSecondMethod_PieChart);
		break;
	}
	case UI_UPDATE_BIDRECORDS_SUMMARY:
	{
		updateMyTableWidget(data, id);
		break;
	}
	case UI_UPDATE_COMPANYINFO_SUMMARY:
	{
		updateMyTableWidget(data, id);
		break;
	}
	}
}
void BidsManager::doUpdateBidSummaryTableWidget(ExcelBidRecordsSummary* data)
{
	ExcelBidRecordsSummary* summary = (ExcelBidRecordsSummary*)data;
	ui.lb_searchResultTableName->setText(summary->title.toUtf8().data());
	//设置行列数(只有列存在的前提下，才可以设置列标签)
	int HlableCnt = summary->tableHeader.count();
	int methodOneRecordCount = summary->MethodOneRecords.size();
	int methodTwoRecordCount = summary->MethodTwoRecords.size();
	int dataFakeRecordCount = summary->DataFakeRecords.size();
	int rowCount = methodOneRecordCount + methodTwoRecordCount + dataFakeRecordCount;
	QString result;
	result.sprintf("共：%d 条记录", rowCount - 6);
	ui.lb_searchResultInfo->setText(result);
	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(rowCount);
	ui.tableWidget->setColumnCount(HlableCnt);
	//设置列标签
	ui.tableWidget->setHorizontalHeaderLabels(summary->tableHeader);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:rgb(228, 228, 228);color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
	ui.tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {background-color:rgb(228, 228, 228);color: black;padding-left: 4px;border: 1px solid #6c6c6c}");
	int column_offset = 0;
	int i = 0;
	for (; i < methodOneRecordCount; ++i)
	{
		column_offset = 0;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->serialNumber));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->projectName));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->openBidDate));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->controlPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->Q));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->K1));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->K2));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->averagePrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->averagePricePercent));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->goodPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->goodPricePercent));
		++column_offset;
		if (CompanyType_otherCompany == companyType)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->myPrice));
			++column_offset;
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->myPricePercent));
			++column_offset;
		}
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->projectManagerRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->performanceRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->bidRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->bidFinalRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->bidWay));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->location));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->bidCounts));
		++column_offset;
		for (int k = 0; k < summary->MethodOneRecords.at(i)->bidInterval.size(); ++k)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodOneRecords.at(i)->bidInterval.at(k)->count));
			++column_offset;
			QString percent;
			percent.append(summary->MethodOneRecords.at(i)->bidInterval.at(k)->percent);
			percent.append("(");
			percent.append(summary->MethodOneRecords.at(i)->bidInterval.at(k)->interval);
			percent.append(")");
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(percent));
			++column_offset;
		}
	}

	int j = 0;
	for (; i < rowCount - dataFakeRecordCount; ++i)
	{
		column_offset = 0;
		j = i - methodOneRecordCount;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->serialNumber));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->projectName));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->openBidDate));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->controlPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->Q));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->K1));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->K2));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->averagePrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->averagePricePercent));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->goodPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->goodPricePercent));
		++column_offset;
		if (CompanyType_otherCompany == companyType)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->myPrice));
			++column_offset;
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->myPricePercent));
			++column_offset;
		}
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->projectManagerRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->performanceRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->bidRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->bidFinalRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->bidWay));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->location));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->bidCounts));
		++column_offset;
		for (int k = 0; k < summary->MethodTwoRecords.at(j)->bidInterval.size(); ++k)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->MethodTwoRecords.at(j)->bidInterval.at(k)->count));
			++column_offset;
			QString percent;
			percent.append(summary->MethodTwoRecords.at(j)->bidInterval.at(k)->percent);
			percent.append("(");
			percent.append(summary->MethodTwoRecords.at(j)->bidInterval.at(k)->interval);
			percent.append(")");
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(percent));
			++column_offset;
		}
	}

	int m = 0;
	for (; i < rowCount; ++i)
	{
		column_offset = 0;
		m = i - methodOneRecordCount - methodTwoRecordCount;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->serialNumber));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->projectName));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->openBidDate));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->controlPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->Q));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->K1));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->K2));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->averagePrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->averagePricePercent));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->goodPrice));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->goodPricePercent));
		++column_offset;
		if (CompanyType_otherCompany == companyType)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->myPrice));
			++column_offset;
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->myPricePercent));
			++column_offset;
		}
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->projectManagerRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->performanceRequirement));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->bidRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->bidFinalRule));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->bidWay));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->location));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->bidCounts));
		++column_offset;
		for (int n = 0; n < summary->DataFakeRecords.at(m)->bidInterval.size(); ++n)
		{
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(summary->DataFakeRecords.at(m)->bidInterval.at(n)->count));
			++column_offset;
			QString percent;
			percent.append(summary->DataFakeRecords.at(m)->bidInterval.at(n)->percent);
			percent.append("(");
			percent.append(summary->DataFakeRecords.at(m)->bidInterval.at(n)->interval);
			percent.append(")");
			ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(percent));
			++column_offset;
		}
	}
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();
	ui.tableWidget->setShowGrid(true);
}
void BidsManager::doUpdateCompanySummaryTableWidget(ExcelCompanySummary* data)
{
	ui.lb_searchResultTableName->setText(data->title.toUtf8().data());
	//设置行列数(只有列存在的前提下，才可以设置列标签)
	int HlableCnt = data->tableHeader.count();
	int companyCount = data->companyArray.size();
	QString result;
	result.sprintf("共：%d 条记录", companyCount);
	ui.lb_searchResultInfo->setText(result);
	ui.tableWidget->setRowCount(companyCount);
	ui.tableWidget->setColumnCount(HlableCnt);
	//设置列标签
	ui.tableWidget->setHorizontalHeaderLabels(data->tableHeader);
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();
	ui.tableWidget->setShowGrid(true);

	int column_offset = 0;
	for (int i = 0; i < companyCount; ++i)
	{
		column_offset = 0;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(data->companyArray.at(i)->serialNum));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(data->companyArray.at(i)->name));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(data->companyArray.at(i)->bidCount));
		++column_offset;
		ui.tableWidget->setItem(i, column_offset, new QTableWidgetItem(data->companyArray.at(i)->comment));
		++column_offset;
	}
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();
	ui.tableWidget->setShowGrid(true);
}
void BidsManager::updateMyTableWidget(void* data, UI_UpdateMsg_ID id)
{
	switch (id)
	{
	case UI_UPDATE_BIDRECORDS_SUMMARY:
	{
		doUpdateBidSummaryTableWidget((ExcelBidRecordsSummary*)data);
		break;
	}
	case UI_UPDATE_COMPANYINFO_SUMMARY:
	{
		doUpdateCompanySummaryTableWidget((ExcelCompanySummary*)data);
		break;
	}	
	}
}
void BidsManager::enterStat(AppStatus status)
{
	if (curStatus == status)
	{
		return;
	}
	switch (status)
	{
	case STAT_NORMAL:
	{
		ui.statusBar->showMessage("完成。");
		curStatus = status;
		break;
	}
	case STAT_IMPORTING:
	{
		curStatus = status;
		ui.statusBar->showMessage("正在校验文件格式并导入...");
		ExcelDataAnalyserByQtXlsx::Instance()->readFile();
		break;
	}
	case STAT_EXPORTING:
	{
		curStatus = status;
		ui.statusBar->showMessage("正在导出汇总记录...");
		ExportDataHelperByQtXlsx::Instance()->table2Excel(ui.tableWidget, ui.lb_searchResultTableName->text());
		break;
	}
	case STAT_QUERYING:
	{
		curStatus = status;
		ui.statusBar->showMessage("正在查询...");
		getCurQueryConfig();
		Controller::Instance()->queryRecords(companyType);	
		break;
	}
	}
	updateAPPStatus(status);
}
void BidsManager::updateAPPStatus(AppStatus status)
{
	switch (status)
	{
	case STAT_IMPORTING:
	case STAT_EXPORTING:
	case STAT_QUERYING:
	{
		navigationWidget->setEnabled(false);
		ui.Self_btn_Search->setEnabled(false);
		ui.other_btn_search->setEnabled(false);
		ui.proxy_btn_search->setEnabled(false);
		ui.btn_exportSearchResultToExcel->setEnabled(false);
		ui.btn_importExcelDataFile->setEnabled(false);
		ui.btn_importBatch->setEnabled(false);
		ui.btn_deleteRecord->setEnabled(false);
		break; 
	}
	case STAT_NORMAL:
	{
		navigationWidget->setEnabled(true);
		ui.Self_btn_Search->setEnabled(true);
		ui.other_btn_search->setEnabled(true);
		ui.proxy_btn_search->setEnabled(true);
		ui.btn_exportSearchResultToExcel->setEnabled(true);
		ui.btn_importExcelDataFile->setEnabled(true);
		ui.btn_importBatch->setEnabled(true);
		ui.btn_deleteRecord->setEnabled(true);
		break;
	}
	}
}
void BidsManager::onDeleteRecordSlot()
{
	deleteRecordWnd.setWindowModality(Qt::ApplicationModal);
	deleteRecordWnd.show();
}
void BidsManager::onBatchImportSlot()
{
	batchImportWnd.setWindowModality(Qt::ApplicationModal);
	batchImportWnd.show();
}
void BidsManager::onSelfCompanyQuerySlot()
{
	companyType = CompanyType_SelfCompany;
	enterStat(STAT_QUERYING);
}
void BidsManager::onOtherCompanyQuerySlot()
{
	companyType = CompanyType_otherCompany;
	enterStat(STAT_QUERYING);
}
void BidsManager::onProxyCompanyQuerySlot()
{
	companyType = CompanyType_ProxyCompany;
	enterStat(STAT_QUERYING);
}
void BidsManager::onImportExcelDataSlot()
{
	QFileDialog::Options options;
	QString selectedFilter;
	QString fileName = QFileDialog::getOpenFileName(this,
		"选择文件",
		NULL,
		tr("*.xlsx;;*.xls"),
		&selectedFilter,
		options);
	if (!fileName.isEmpty())
	{
		ExcelDataAnalyserByQtXlsx::Instance()->setFilePath(fileName);
		enterStat(STAT_IMPORTING);
	}
}
void BidsManager::onExportDataToExcelFileSlot()
{
	ExportDataHelperByQtXlsx::Instance()->reset();
	childWindow.exec();
	if (ExportDataHelperByQtXlsx::Instance()->getFilePathSetStatus())
	{
		enterStat(STAT_EXPORTING);
	}
}
