#pragma once
#include <QtWidgets/QMainWindow>
#include "chartdir.h"
#include "qchartviewer.h"
#include "GlobalDefine.h"
class ImagePanel : public QWidget
{
	Q_OBJECT

public:
	typedef enum  {
		IMAGE_TYPE_BARCHART = 0,
		IMAGE_TYPE_PIECHART,
		IMAGE_TYPE_LINECHART,
	}IMAGE_TYPE;
	ImagePanel(QWidget *parent = 0);
	~ImagePanel();
	void drawMethodCountPieChart(MethodCounts* ptr);
	void drawMethodLineChart(QVector<MethodDuringTime*>* dataVector);
	void drawAVELineChart(QVector<AveragePricePercent*>* dataVector);
	void drawAVEMethodOnePieChart(QVector<AveragePriceIntervalPercent*>* dataVector);
	void drawAVEMethodTwoPieChart(QVector<AveragePriceIntervalPercent*>* dataVector);
	void drawBarChart();

	protected slots:
	//void onSaveAs();

private:
	QChartViewer* m_qViewer;
	XYChart* m_chart;
	PieChart* m_PieChart;
};