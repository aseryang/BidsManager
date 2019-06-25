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
	//�����ַ�����ʽ�� ����ʱ��|���̵ص�|���Ƽ�|���귽��|��Ŀ����|ҵ��Ҫ��|Ͷ�걨������
	QString queryConditionSelfCompay;
	//�����ַ�����ʽ�� ��λ����|����ʱ��
	QString queryConditionOtherCompany;
	QString queryConditionProxyCompany;
};