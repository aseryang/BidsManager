#pragma once
#include "GlobalDefine.h"
#include <QObject>
#include <QThread>
class Controller: public QThread
{
	Q_OBJECT
private:
	Controller(){}
public:
	static Controller* Instance()
	{
		static Controller* instance = new Controller;
		return instance;
	}
	int queryRecords(UserQueryCompanyType type);
protected:
	void run();
signals:
	void jobFinishSignal();
	void updateUISignal(void*, UI_UpdateMsg_ID);

protected:
private:
	UserQueryCompanyType companyType;
};