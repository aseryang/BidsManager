#include "ChartCreateHelper.h"

ChartCreateHelper* ChartCreateHelper::Instance()
{
	static ChartCreateHelper* ptr = new ChartCreateHelper;
	return ptr;
}
LineChartFrame* ChartCreateHelper::createMethodLineChart(QVector<MethodDuringTime*>* dataVector)
{
	int dataSize = (*dataVector).size();
	if (0 == dataSize)
	{
		return NULL;
	}
	double* dataArray = new double[dataSize];
	double* timeStampArray = new double[dataSize];
	memset(dataArray, 0, dataSize);
	memset(timeStampArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		MethodDuringTime* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->methodType;
		//QStringList list = singleRecord->date.split('-');
		//double timestamp = Chart::chartTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());	
		QDateTime dateTime = QDateTime::fromString(singleRecord->date, "yyyy-MM-dd hh:mm:ss");
		double timestamp = QDateTimeToChartTime(dateTime);
		timeStampArray[i] = timestamp;
	}
	DoubleArray datas(dataArray, dataSize);
	DoubleArray timeStamps(timeStampArray, dataSize);
	const char* chartTitle = "����һ���������ֲ�ͼ";
	const char* yAxisTitle = "����һ��������";
	const char* yAxisDataTitle = "����";
	LineChartFrame * framePtr = new LineChartFrame(chartTitle, yAxisTitle, 
		yAxisDataTitle, timeStamps, datas);


	return framePtr;
}
LineChartFrame* ChartCreateHelper::createAVELineChart(QVector<AveragePricePercent*>* dataVector)
{
	int dataSize = (*dataVector).size();
	if (0 == dataSize)
	{
		return NULL;
	}
	double* dataArray = new double[dataSize];
	double* timeStampArray = new double[dataSize];
	memset(dataArray, 0, dataSize);
	memset(timeStampArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		AveragePricePercent* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->percent;
		//QStringList list = singleRecord->date.split('-');
		//double timestamp = Chart::chartTime(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
		QDateTime dateTime = QDateTime::fromString(singleRecord->date, "yyyy-MM-dd hh:mm:ss");
		double timestamp = QDateTimeToChartTime(dateTime);
		timeStampArray[i] = timestamp;
	}
	DoubleArray datas(dataArray, dataSize);
	DoubleArray timeStamps(timeStampArray, dataSize);
	const char* chartTitle = "����ƽ��ֵռ�ȷֲ�ͼ";
	const char* yAxisTitle = "ռ��";
	const char* yAxisDataTitle = "eee";
	LineChartFrame * framePtr = new LineChartFrame(chartTitle, yAxisTitle,
		yAxisDataTitle, timeStamps, datas);

	return framePtr;
}