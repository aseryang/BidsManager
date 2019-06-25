#include "ExportDataHelper.h"
#include <QDir>
ExportDataHelper* ExportDataHelper::Instance()
{
	static ExportDataHelper* ptr = new ExportDataHelper;
	return ptr;
}
void ExportDataHelper::setFilePath(QString& path)
{
	savedFilePath = path;
	isFilePathSeted = true;
}
void ExportDataHelper::reset()
{
	isFilePathSeted = false;
}
bool ExportDataHelper::getFilePathSetStatus()
{
	return isFilePathSeted;
}
void ExportDataHelper::run()
{
	int column_offset = 0;
	int row_offset = 0;
	int sheet_num = 0;
	if (title.indexOf("开标记录汇总表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 1;
	}
	else if (title.indexOf("开标记录表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 2;
	}
	else if (title.indexOf("施工单位汇总表") >= 0)
	{
		column_offset = 20;
		row_offset = 4;
		sheet_num = 2;
	}
	else if (title.indexOf("招标代理记录表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 3;
	}
	else if (title.indexOf("招标代理汇总表") >= 0)
	{
		column_offset = 20;
		row_offset = 4;
		sheet_num = 3;
	}
	if (!savedFilePath.isEmpty())
	{
		QAxObject *excel = new QAxObject;
		if (excel->setControl("Excel.Application"))
		{
			excel->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
			excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
			QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
			if (!workbooks)
			{
				//QMessageBox::warning(NULL, "错误", "获取工作簿集合失败。", QMessageBox::Apply);
				emit updateJobStatusSignal((int)JOB_WARNING, QString("获取工作簿集合失败。"));
				return;
			}
			workbooks->dynamicCall("Open (const QString&)", templateFilePath);
			QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
			if (!workbook)
			{
				emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Template.xlsx失败。"));
				//QMessageBox::warning(NULL, "错误", "打开Template.xlsx失败。", QMessageBox::Apply);
				return;
			}
			QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", sheet_num);
			if (!worksheet)
			{
				emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Sheet页失败。"));
				//QMessageBox::warning(NULL, "错误", "打开Sheet页失败。", QMessageBox::Apply);
				return;
			}
			int i, j;
			int colcount = table->columnCount();
			int rowcount = table->rowCount();

			//设置标题单元格
			QAxObject* titleCell = worksheet->querySubObject("Cells(int,int)", 1, 1);
			titleCell->dynamicCall("SetValue(const QString&)", title);

			//QTableWidget 获取表格数据部分
			for (i = 0; i < rowcount; i++)
			{
				for (j = 0; j < colcount; j++)
				{
					QAxObject* cell = worksheet->querySubObject("Cells(int,int)", i + row_offset, j + column_offset);
					if (cell)
					{
						cell->dynamicCall("SetValue(const QString&)", table->item(i, j) ? table->item(i, j)->text() : "");
					}
				}
			}
			//画框线
			QString lrange;
			lrange.append(column_offset - 1 + 'A');
			lrange.append("4:");
			if (colcount == 38)
			{
				lrange.append("AL");
			}
			else
			{
				lrange.append(column_offset - 1 + colcount - 1 + 'A');
			}
			lrange.append(QString::number(table->rowCount() + 3));
			QAxObject *range = worksheet->querySubObject("Range(const QString&)", lrange);
			range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
			range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
			//调整数据区行高
			QString rowsName;
			rowsName.append("4:");
			rowsName.append(QString::number(table->rowCount() + 3));
			range = worksheet->querySubObject("Range(const QString&)", rowsName);
			range->setProperty("RowHeight", 25.5);
			//获取字体
			QAxObject *font = range->querySubObject("Font");
			//设置字体
			font->setProperty("Name", "宋体");
			//设置字体大小
			font->setProperty("Size", 9);
			workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(savedFilePath));//保存至fileName
			workbook->dynamicCall("Close()");//关闭工作簿
			excel->dynamicCall("Quit()");//关闭excel
			delete excel;
			excel = NULL;
			emit updateJobStatusSignal((int)JOB_QUESTION, savedFilePath);
// 			if (QMessageBox::question(NULL, "完成", "文件已经导出，是否现在打开？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
// 			{
// 				QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(savedFilePath)));
// 			}
		}
		else
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("未能创建 Excel 对象，请安装 Microsoft Excel。"));
			//QMessageBox::warning(NULL, "错误", "未能创建 Excel 对象，请安装 Microsoft Excel。", QMessageBox::Apply);
		}
	}
}
void ExportDataHelper::table2Excel(QTableWidget *tableTemp, QString titleTemp)
{
	table = tableTemp;
	title = titleTemp;
	start();
}
