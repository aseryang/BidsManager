#pragma once

#include <QWidget>
#include "ui_RecordsDirImport.h"
#include "GlobalDefine.h"
#include "BatchImportHelper.h"

class RecordsDirImport : public QWidget
{
	Q_OBJECT
public:
	RecordsDirImport(QWidget *parent = Q_NULLPTR);
	~RecordsDirImport();
public slots:
	void onDirChooseSlot();
	void onStartBatchImportSlot();
	void onUpdateRunningInfoSlot(BatchImportStatus status, BatchImportRunningInfo* info);
private:
	void UIReset();
	void closeEvent(QCloseEvent*event);

private:
	Ui::RecordsDirImport ui;
	BatchImportHelper batchImport;
};
