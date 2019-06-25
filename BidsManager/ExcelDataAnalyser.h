#pragma once
#include<ActiveQt/QAxObject>
#include <QThread>
class ExcelDataAnalyser: public QThread
{
	Q_OBJECT
private:
	ExcelDataAnalyser(){}
public:
	static ExcelDataAnalyser* Instance();
	~ExcelDataAnalyser(){};
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