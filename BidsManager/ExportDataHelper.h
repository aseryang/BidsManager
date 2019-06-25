#pragma once
#include "GlobalDefine.h"
#include<ActiveQt/QAxObject>
#include <QTableWidget>
#include <QCoreApplication>
#include <QThread>
class ExportDataHelper: public QThread
{
	Q_OBJECT
private:
	ExportDataHelper() :isFilePathSeted(false), table(NULL)
	{
		templateFilePath.append(QCoreApplication::applicationDirPath());
		templateFilePath += "/Template.xlsx";
	}
public:
	static ExportDataHelper* Instance();
	void setFilePath(QString& path);
	void reset();
	bool getFilePathSetStatus();
	void table2Excel(QTableWidget *tableTemp, QString titleTemp);
	void run();
signals:
	void updateJobStatusSignal(int status, QString info);
protected:
private:
	QString savedFilePath;
	QString templateFilePath;
	bool isFilePathSeted;
	QTableWidget* table;
	QString title;
};