#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BidsManager.h"
#include <QHBoxLayout>
#include <QLabel>
#include "qnavigationwidget.h"
#include "GlobalDefine.h"
#include "imagepanel.h"
#include "ChartCreateHelper.h"
#include "CreateFileDialogWindow.h"
#include "RecordsDirImport.h"
#include "DeleteBidsRecord.h"

class BidsManager : public QMainWindow
{
	Q_OBJECT

public:
	BidsManager(QWidget *parent = Q_NULLPTR);
public slots:
	void onDeleteRecordSlot();
	void onBatchImportSlot();
	void onSelfCompanyQuerySlot();
	void onImportExcelDataSlot();
	void onOtherCompanyQuerySlot();
	void onProxyCompanyQuerySlot();
	void onExportDataToExcelFileSlot();
	void onUpdateUI(void* data, UI_UpdateMsg_ID id);
	void onCheckboxStateChanged(bool checked);
	void onRB_helidijiafaStateChanged(bool checked);
	void onRB_pingbiaoruweifaStateChanged(bool checked);
	void onRB_otherbidwayStateChanged(bool checked);
	void onRB_mgrReqOtherStateChanged(bool checked);
	void onCB_qiyeyejiStateChanged(bool checked);
	void onCB_jingliyejiStateChanged(bool checked);
	void jobFinished();
	void onProjectSwitch(const int& projectId);
	void onUpdateJobStatus(int status, QString info);
	void bidwayButtonsClicked(int id);
	void mgrReqButtonsClicked(int id);
public:
	void setInterval(QString& interval, IntervalType type);
	QString getIntervalParam(IntervalType type);
	QString checkIntervalParam(QString& src);
	void getCurQueryConfig();
	void uiSpecialSetting();
	void uiSetInterval();
	void uiReset();
	void enterStat(AppStatus status);
	void updateAPPStatus(AppStatus status);
	void updateMyTableWidget(void* data, UI_UpdateMsg_ID id);
	void doUpdateBidSummaryTableWidget(ExcelBidRecordsSummary* data);
	void doUpdateCompanySummaryTableWidget(ExcelCompanySummary* data);
	

private:
	RecordsDirImport batchImportWnd;
	DeleteBidsRecord deleteRecordWnd;
	Ui::BidsManagerClass ui;
	QWidget *mainWidget;
	QWidget *rightWidget;
	QHBoxLayout *mainLayout;
	QVBoxLayout *rightLayout;
	QNavigationWidget *navigationWidget;
	QLabel *tipsLabel;
	UserQueryCompanyType companyType;
	AppStatus curStatus;
	LineChartFrame* panelMethodDuringTime_LineChart;
	ImagePanel* panelMethodCounts_PieChart;
	LineChartFrame* panelAveragePrice_LineChart;
	ImagePanel* panelFirstMethod_PieChart;
	ImagePanel* panelSecondMethod_PieChart;
	QButtonGroup* bidWayBtnGroup;
	QButtonGroup* managerReqBtnGroup;
	CreateRecFileDialog childWindow;
	QVector<QLineEdit *> baojiaqujianArray;
	QVector<QLineEdit *> methodOneQujianArray;
	QVector<QLineEdit *> methodTwoQujianArray;
};
