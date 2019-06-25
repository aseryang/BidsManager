#pragma once
#pragma execution_character_set("utf-8") //设置文件编码格式为UTF-8
#include <QDialog>
#include <QDateEdit>
#include <QScrollBar>
#include <QDateTime>
#include "qchartviewer.h"

double QDateTimeToChartTime(QDateTime q);
class LineChartFrame : public QFrame {
	Q_OBJECT
public:
	LineChartFrame(const char* chartTitle,
		const char* yAxisTitle,
		const char* yAxisDataTitle,
		DoubleArray& timeStamps,
		DoubleArray& datas,
		QWidget *parent = 0);
	~LineChartFrame();

private:
	//
	// Data arrays for the scrollable / zoomable chart.
	// - In this demo, we just use a RanTable object to generate random data for the chart.
	//
	RanSeries *m_ranSeries;
	DoubleArray m_timeStamps;
	DoubleArray m_dataSeriesA;
	DoubleArray m_dataSeriesB;
	DoubleArray m_dataSeriesC;

	QDateEdit *m_StartDate;
	QDateEdit *m_EndDate;
	QScrollBar *m_HScrollBar;
	QChartViewer *m_ChartViewer;

	const char* m_chartTitle;
	const char* m_yAxisTitle;
	const char* m_yAxisDataTitle;


	void loadData();                                // Load data into data arrays
	void initChartViewer(QChartViewer *viewer);     // Initialize the QChartViewer
	void drawChart(QChartViewer *viewer);           // Draw chart
	void trackLineLabel(XYChart *c, int mouseX);    // Draw Track Cursor
	void updateControls(QChartViewer *viewer);      // Update other controls

	private slots:
	void onStartDateChanged(QDateTime date);
	void onEndDateChanged(QDateTime date);
	void onHScrollBarChanged(int value);
	void onMouseUsageChanged(int mouseUsage);
	void onViewPortChanged();
	void onMouseMovePlotArea(QMouseEvent *event);
	void onMouseWheelChart(QWheelEvent *event);
};