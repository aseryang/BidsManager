#pragma once
#include "imagepanel.h"
#include "linechartframe.h"
#include "GlobalDefine.h"
class ChartCreateHelper
{
private:
	ChartCreateHelper(){}
public:
	~ChartCreateHelper(){}
	static ChartCreateHelper* Instance();
	LineChartFrame* createMethodLineChart(QVector<MethodDuringTime*>* dataVector);
	LineChartFrame* createAVELineChart(QVector<AveragePricePercent*>* dataVector);
protected:
private:
};