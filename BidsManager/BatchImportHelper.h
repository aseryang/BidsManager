#pragma once
#include <QThread>
#include "GlobalDefine.h"

class BatchImportHelper: public QThread
{
	Q_OBJECT
public:
	BatchImportHelper() :isStop(false){}
	~BatchImportHelper(){}
	void doImport(QString dir);
	void stopImport();
protected:
	void run();
	RetCode work(QString path);
signals:
	void runningInfoSignal(BatchImportStatus status, BatchImportRunningInfo* info);
private:
	QString excelsDir;
	BatchImportStatus status;
	BatchImportRunningInfo info;
	bool isStop;
};