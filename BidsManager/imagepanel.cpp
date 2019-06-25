#include "imagepanel.h"
ImagePanel::ImagePanel(QWidget *parent)
	: QWidget(parent), m_qViewer(NULL), m_chart(NULL), m_PieChart(NULL)
{
	m_qViewer = new QChartViewer(this);
}
ImagePanel::~ImagePanel()
{
	if (m_chart)
	{
		delete m_chart;
		m_chart = NULL;
	}
	if (m_qViewer)
	{
		delete m_qViewer;
		m_qViewer = NULL;
	}
	if (m_PieChart)
	{
		delete m_PieChart;
		m_PieChart = NULL;
	}
}
void ImagePanel::drawMethodCountPieChart(MethodCounts* ptr)
{
	// The data for the pie chart
	double data[2] = {0};
	data[0] = ptr->methodFirstCount;
	data[1] = ptr->methodSecondCount;

// 	if (data[0] == 0 && data[1] == 0)
// 	{
// 		return;
// 	}

	// The labels for the pie chart
	const char *labels[2] = { "方法一", "方法二"};

	// Create a PieChart object of size 560 x 270 pixels, with a golden background and a 1 pixel 3D
	// border
	m_PieChart = new PieChart(370, 470, 0xE4E4E4, -1, 1);

	m_PieChart->setDefaultFonts("simsun.ttc");

	// Add a title box using 15pt Times Bold Italic font and metallic pink background color
	m_PieChart->addTitle("方法一，方法二概率图", "simsun.ttc", 10)->setBackground(0xE4E4E4,
		0x000000, Chart::glassEffect());

	// Set the center of the pie at (280, 135) and the radius to 110 pixels
	m_PieChart->setPieSize(185, 120, 55);

	// Draw the pie in 3D with 20 pixels 3D depth
	m_PieChart->set3D(20);

	// Use the side label layout method
	m_PieChart->setLabelLayout(Chart::SideLayout);

	// Set the label box background color the same as the sector color, with glass effect, and with
	// 5 pixels rounded corners
	TextBox *t = m_PieChart->setLabelStyle();
	t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::glassEffect());
	t->setRoundedCorners(5);

	// Set the border color of the sector the same color as the fill color. Set the line color of
	// the join line to black (0x0)
	m_PieChart->setLineColor(Chart::SameAsMainColor, 0x000000);

	// Set the start angle to 135 degrees may improve layout when there are many small sectors at
	// the end of the data array (that is, data sorted in descending order). It is because this
	// makes the small sectors position near the horizontal axis, where the text label has the least
	// tendency to overlap. For data sorted in ascending order, a start angle of 45 degrees can be
	// used instead.
	m_PieChart->setStartAngle(135);

	// Set the pie data and the pie labels
	if (0 != data[0] || 0 != data[1])
	{
		m_PieChart->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
	}

	// Output the chart
	m_qViewer->setChart(m_PieChart);
	delete m_PieChart;
	m_PieChart = NULL;
	//c->makeChart();
}
void ImagePanel::drawMethodLineChart(QVector<MethodDuringTime*>* dataVector)
{
	int dataSize = (*dataVector).size();
	double* dataArray = new double[dataSize];
	char** labelArray = new char*[dataSize];
	memset(dataArray, 0, dataSize);
	memset(labelArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		MethodDuringTime* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->methodType;
		QByteArray ba = singleRecord->date.toLatin1();
		labelArray[i] = new char[ba.size() + 1];
		memset(labelArray[i], 0, ba.size());
		strncpy(labelArray[i], ba.data(), ba.size() + 1);
	}
	// The data for the line chart
// 	double data0[] = { 42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
// 		76, 63, 67, 75, 64, 51 };
// 	double data1[] = { 50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
// 		77, 84, 82, 80, 84, 98 };
// 	double data2[] = { 36, 28, 25, 33, 38, 20, 22, 30, 25, 33, 30, 24, 28, 15, 21, 26, 46, 42, 48,
// 		45, 43, 52, 64, 60, 70 };
// 
// 	// The labels for the line chart
// 	const char *labels[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
// 		"13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24" };

	// Create an XYChart object of size 600 x 300 pixels, with a light blue (EEEEFF) background,
	// black border, 1 pxiel 3D border effect and rounded corners
	m_chart = new XYChart(555,500, 0xeeeeff, 0x000000, 1);
	m_chart->setDefaultFonts("simsun.ttc");
	//m_chart->setRoundedFrame();

	// Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white background. Turn on
	// both horizontal and vertical grid lines with light grey color (0xcccccc)
	m_chart->setPlotArea(55, 58, 420, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	// Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 9pt Arial Bold
	// font. Set the background and border color to Transparent.
	//m_chart->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	// Add a title box to the chart using 15pt Times Bold Italic font, on a light blue (CCCCFF)
	// background with glass effect. white (0xffffff) on a dark red (0x800000) background, with a 1
	// pixel 3D border.
	m_chart->addTitle("方法一，方法二分布图", "simsun.ttc", 10)->setBackground(0xccccff,
		0x000000, Chart::glassEffect());

	// Add a title to the y axis
	m_chart->yAxis()->setTitle("方法一、方法二","simsun.ttc", 10);

	// Set the labels on the x axis.
	m_chart->xAxis()->setLabels(StringArray(labelArray, dataSize));

	// Display 1 out of 3 labels on the x-axis.
	m_chart->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	m_chart->xAxis()->setTitle("日期", "simsun.ttc", 10);

	// Add a line layer to the chart
	LineLayer *layer = m_chart->addLineLayer();

	// Set the default line width to 2 pixels
	//layer->setLineWidth(2);

	// Add the three data sets to the line layer. For demo purpose, we use a dash line color for the
	// last line
	layer->addDataSet(DoubleArray(dataArray, dataSize), 0xff0000,
		"Server #1");
	//layer->setXData(0, 12);
// 	layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x008800,
// 		"Server #2");
// 	layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), m_chart->dashLineColor(
// 		0x3333ff, Chart::DashLine), "Server #3");

	m_qViewer->setChart(m_chart);
	// Output the chart
	//c->makeChart();

	//Free 
	for (int i = 0; i < dataSize; ++i)
	{
		delete []labelArray[i];
	}
	delete[] labelArray;
 	delete[] dataArray;
}
void ImagePanel::drawAVELineChart(QVector<AveragePricePercent*>* dataVector)
{
	int dataSize = (*dataVector).size();
	double* dataArray = new double[dataSize];
	char** labelArray = new char*[dataSize];
	memset(dataArray, 0, dataSize);
	memset(labelArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		AveragePricePercent* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->percent;
		QByteArray ba = singleRecord->date.toLatin1();
		labelArray[i] = new char[ba.size() + 1];
		memset(labelArray[i], 0, ba.size());
		strncpy(labelArray[i], ba.data(), ba.size() + 1);
	}
	// The data for the line chart
	// 	double data0[] = { 42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
	// 		76, 63, 67, 75, 64, 51 };
	// 	double data1[] = { 50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
	// 		77, 84, 82, 80, 84, 98 };
	// 	double data2[] = { 36, 28, 25, 33, 38, 20, 22, 30, 25, 33, 30, 24, 28, 15, 21, 26, 46, 42, 48,
	// 		45, 43, 52, 64, 60, 70 };
	// 
	// 	// The labels for the line chart
	// 	const char *labels[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
	// 		"13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24" };

	// Create an XYChart object of size 600 x 300 pixels, with a light blue (EEEEFF) background,
	// black border, 1 pxiel 3D border effect and rounded corners
	m_chart = new XYChart(555, 500, 0xeeeeff, 0x000000, 1);
	m_chart->setDefaultFonts("simsun.ttc");
	//m_chart->setRoundedFrame();

	// Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white background. Turn on
	// both horizontal and vertical grid lines with light grey color (0xcccccc)
	m_chart->setPlotArea(55, 58, 420, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	// Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 9pt Arial Bold
	// font. Set the background and border color to Transparent.
	//m_chart->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	// Add a title box to the chart using 15pt Times Bold Italic font, on a light blue (CCCCFF)
	// background with glass effect. white (0xffffff) on a dark red (0x800000) background, with a 1
	// pixel 3D border.
	m_chart->addTitle("平均值占比分布图", "simsun.ttc", 10)->setBackground(0xccccff,
		0x000000, Chart::glassEffect());

	// Add a title to the y axis
	m_chart->yAxis()->setTitle("占比", "simsun.ttc", 10);

	// Set the labels on the x axis.
	m_chart->xAxis()->setLabels(StringArray(labelArray, dataSize));

	// Display 1 out of 3 labels on the x-axis.
	m_chart->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	m_chart->xAxis()->setTitle("日期", "simsun.ttc", 10);

	// Add a line layer to the chart
	LineLayer *layer = m_chart->addLineLayer();

	// Set the default line width to 2 pixels
	//layer->setLineWidth(2);

	// Add the three data sets to the line layer. For demo purpose, we use a dash line color for the
	// last line
	layer->addDataSet(DoubleArray(dataArray, dataSize), 0xff0000,
		"Server #1");
	//layer->setXData(0, 12);
	// 	layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x008800,
	// 		"Server #2");
	// 	layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), m_chart->dashLineColor(
	// 		0x3333ff, Chart::DashLine), "Server #3");

	m_qViewer->setChart(m_chart);
	// Output the chart
	//c->makeChart();

	//Free 
	for (int i = 0; i < dataSize; ++i)
	{
		delete[]labelArray[i];
	}
	delete[] labelArray;
	delete[] dataArray;
}
void ImagePanel::drawAVEMethodOnePieChart(QVector<AveragePriceIntervalPercent*>* dataVector)
{
	bool haveData = false;
	int dataSize = (*dataVector).size();
	double* dataArray = new double[dataSize];
	char** labelArray = new char*[dataSize];
	memset(dataArray, 0, dataSize);
	memset(labelArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		AveragePriceIntervalPercent* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->projectCount;
		if (!haveData && (singleRecord->projectCount > 0))
		{
			haveData = true;
		}
		QByteArray ba = singleRecord->percentInterval.toUtf8();
		labelArray[i] = new char[ba.size() + 1];
		memset(labelArray[i], 0, ba.size());
		strncpy(labelArray[i], ba.data(), ba.size() + 1);
	}
	// The data for the pie chart
// 	double data[2] = { 0 };
// 	data[0] = ptr->methodFirstCount;
// 	data[1] = ptr->methodSecondCount;
// 
// 	// The labels for the pie chart
// 	const char *labels[2] = { "方法一", "方法二" };

	// Create a PieChart object of size 560 x 270 pixels, with a golden background and a 1 pixel 3D
	// border
	m_PieChart = new PieChart(370, 470, 0xE4E4E4, -1, 1);

	m_PieChart->setDefaultFonts("simsun.ttc");

	// Add a title box using 15pt Times Bold Italic font and metallic pink background color
	m_PieChart->addTitle("方法一平均值占比概率图", "simsun.ttc", 10)->setBackground(0xE4E4E4,
		0x000000, Chart::glassEffect());

	// Set the center of the pie at (280, 135) and the radius to 110 pixels
	m_PieChart->setPieSize(185, 120, 55);

	// Draw the pie in 3D with 20 pixels 3D depth
	m_PieChart->set3D(20);

	// Use the side label layout method
	m_PieChart->setLabelLayout(Chart::SideLayout);

	// Set the label box background color the same as the sector color, with glass effect, and with
	// 5 pixels rounded corners
	TextBox *t = m_PieChart->setLabelStyle();
	t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::glassEffect());
	t->setRoundedCorners(5);

	// Set the border color of the sector the same color as the fill color. Set the line color of
	// the join line to black (0x0)
	m_PieChart->setLineColor(Chart::SameAsMainColor, 0x000000);

	// Set the start angle to 135 degrees may improve layout when there are many small sectors at
	// the end of the data array (that is, data sorted in descending order). It is because this
	// makes the small sectors position near the horizontal axis, where the text label has the least
	// tendency to overlap. For data sorted in ascending order, a start angle of 45 degrees can be
	// used instead.
	m_PieChart->setStartAngle(135);

	// Set the pie data and the pie labels
	if (haveData)
	{
		m_PieChart->setData(DoubleArray(dataArray, dataSize), StringArray(labelArray, dataSize));
	}

	// Output the chart
	m_qViewer->setChart(m_PieChart);
	delete m_PieChart;
	m_PieChart = NULL;
	//c->makeChart();
	//Free 
	for (int i = 0; i < dataSize; ++i)
	{
		delete[]labelArray[i];
	}
	delete[] labelArray;
	delete[] dataArray;
}
void ImagePanel::drawAVEMethodTwoPieChart(QVector<AveragePriceIntervalPercent*>* dataVector)
{
	bool haveData = false;
	int dataSize = (*dataVector).size();
	double* dataArray = new double[dataSize];
	char** labelArray = new char*[dataSize];
	memset(dataArray, 0, dataSize);
	memset(labelArray, 0, dataSize);
	for (int i = 0; i < dataSize; ++i)
	{
		AveragePriceIntervalPercent* singleRecord = (*dataVector).at(i);
		dataArray[i] = singleRecord->projectCount;
		if (!haveData && (singleRecord->projectCount > 0))
		{
			haveData = true;
		}
		QByteArray ba = singleRecord->percentInterval.toUtf8();
		labelArray[i] = new char[ba.size() + 1];
		memset(labelArray[i], 0, ba.size());
		strncpy(labelArray[i], ba.data(), ba.size() + 1);
	}
	// The data for the pie chart
// 	double data[2] = { 0 };
// 	data[0] = ptr->methodFirstCount;
// 	data[1] = ptr->methodSecondCount;
// 
// 	// The labels for the pie chart
// 	const char *labels[2] = { "方法一", "方法二" };

	// Create a PieChart object of size 560 x 270 pixels, with a golden background and a 1 pixel 3D
	// border
	m_PieChart = new PieChart(370, 470, 0xE4E4E4, -1, 1);

	m_PieChart->setDefaultFonts("simsun.ttc");

	// Add a title box using 15pt Times Bold Italic font and metallic pink background color
	m_PieChart->addTitle("方法二平均值占比概率图", "simsun.ttc", 10)->setBackground(0xE4E4E4,
		0x000000, Chart::glassEffect());

	// Set the center of the pie at (280, 135) and the radius to 110 pixels
	m_PieChart->setPieSize(185, 120, 55);

	// Draw the pie in 3D with 20 pixels 3D depth
	m_PieChart->set3D(20);

	// Use the side label layout method
	m_PieChart->setLabelLayout(Chart::SideLayout);

	// Set the label box background color the same as the sector color, with glass effect, and with
	// 5 pixels rounded corners
	TextBox *t = m_PieChart->setLabelStyle();
	t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::glassEffect());
	t->setRoundedCorners(5);

	// Set the border color of the sector the same color as the fill color. Set the line color of
	// the join line to black (0x0)
	m_PieChart->setLineColor(Chart::SameAsMainColor, 0x000000);

	// Set the start angle to 135 degrees may improve layout when there are many small sectors at
	// the end of the data array (that is, data sorted in descending order). It is because this
	// makes the small sectors position near the horizontal axis, where the text label has the least
	// tendency to overlap. For data sorted in ascending order, a start angle of 45 degrees can be
	// used instead.
	m_PieChart->setStartAngle(135);

	// Set the pie data and the pie labels
	if (haveData)
	{
		m_PieChart->setData(DoubleArray(dataArray, dataSize), StringArray(labelArray, dataSize));
	}

	// Output the chart
	m_qViewer->setChart(m_PieChart);
	delete m_PieChart;
	m_PieChart = NULL;
	//c->makeChart();
	//Free 
	for (int i = 0; i < dataSize; ++i)
	{
		delete[]labelArray[i];
	}
	delete[] labelArray;
	delete[] dataArray;
}
void ImagePanel::drawBarChart()
{
	//const double data[] = { 34, 56, 12, 58, 98 };
	//const char* lbels[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };

	const double data[5] = { 0 };
	const char* lbels[5] = { 0 };

	m_chart = new XYChart(400, 300);
	//m_chart->setDefaultFonts("simsun.ttc");
	m_chart->setPlotArea(35, 25, 335, 240);

	// set the color of bar.
	BarLayer* blayer = m_chart->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0xFF);
	blayer->set3D();
	//m_chart->xAxis()->setLabelStyle("宋体", 8);
	m_chart->xAxis()->setLabels(StringArray(lbels, (int)(sizeof(lbels) / sizeof(lbels[0]))));
	blayer->setBarGap(0.5);

	m_qViewer->setChart(m_chart);
}
