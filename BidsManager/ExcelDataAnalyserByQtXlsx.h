#pragma once
#include "GlobalDefine.h"
#include "xlsx\xlsxdocument.h"
#include "xlsx\xlsxworkbook.h"
#include <QThread>
class ExcelDataAnalyserByQtXlsx: public QThread
{
	Q_OBJECT
private:
	ExcelDataAnalyserByQtXlsx(){}
public:
	static ExcelDataAnalyserByQtXlsx* Instance();
	~ExcelDataAnalyserByQtXlsx(){};
	void run();
	int readFile();
	void setFilePath(QString& path)
	{
		filePath = path;
	}
signals:
	void updateJobStatusSignal(int status, QString info);
protected:
private:
	QString filePath;
};