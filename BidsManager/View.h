#pragma once
#include "GlobalDefine.h"
class View
{
private:
	View(){}
public:
	static View* Instance()
	{
		static View* handle = new View;
		return handle;
	}
	void setqueryConditionSelfCompay(const QString& condition)
	{
		queryConditionSelfCompay = condition;
	}
	void setqueryConditionOtherCompany(const QString& condition)
	{
		queryConditionOtherCompany = condition;
	}
	void setqueryConditionProxyCompany(const QString& condition)
	{
		queryConditionProxyCompany = condition;
	}
protected:
private:
	//条件字符串格式： 开标时间|工程地点|控制价|评标方法|项目经理|业绩要求|投标报价区间
	QString queryConditionSelfCompay;
	//条件字符串格式： 单位名称|开标时间
	QString queryConditionOtherCompany;
	QString queryConditionProxyCompany;
};