#include "ExportDataHelperByQtXlsx.h"
#include "xlsx/xlsxdocument.h"
#include "xlsx/xlsxworkbook.h"
#include <QDir>
ExportDataHelperByQtXlsx* ExportDataHelperByQtXlsx::Instance()
{
	static ExportDataHelperByQtXlsx* ptr = new ExportDataHelperByQtXlsx;
	return ptr;
}
void ExportDataHelperByQtXlsx::setFilePath(QString& path)
{
	savedFilePath = path;
	isFilePathSeted = true;
}
void ExportDataHelperByQtXlsx::reset()
{
	isFilePathSeted = false;
}
bool ExportDataHelperByQtXlsx::getFilePathSetStatus()
{
	return isFilePathSeted;
}
void ExportDataHelperByQtXlsx::run()
{
	int column_offset = 0;
	int row_offset = 0;
	int sheet_num = 0;
	if (title.indexOf("开标记录汇总表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 0;
	}
	else if (title.indexOf("开标记录表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 1;
	}
	else if (title.indexOf("施工单位汇总表") >= 0)
	{
		column_offset = 22;
		row_offset = 4;
		sheet_num = 1;
	}
	else if (title.indexOf("招标代理记录表") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 2;
	}
	else if (title.indexOf("招标代理汇总表") >= 0)
	{
		column_offset = 20;
		row_offset = 4;
		sheet_num = 2;
	}
	if (!savedFilePath.isEmpty())
	{
		QXlsx::Document xlsx(templateFilePath);
		QXlsx::Workbook *workBook = xlsx.workbook();
		if (!workBook)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Template.xlsx失败。"));
			return;
		}
		QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(sheet_num));
		if (!workSheet)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Sheet页失败。"));
			return;
		}

		int i, j;
		int colcount = table->columnCount();
		int rowcount = table->rowCount();

		//设置标题单元格
		//开标记录汇总表，xx开标记录表，xx招标代理记录表 标题需要设置
		if (1 == column_offset && 4 == row_offset)
		{
			workSheet->write(1, 1, title);
		}

		QXlsx::Format format;                       //格式  
		format.setFont(QFont("宋体"));       //字体 
		format.setFontSize(9);
		format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
		format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
		format.setBorderStyle(QXlsx::Format::BorderThin);
		//QTableWidget 获取表格数据部分
		for (i = 0; i < rowcount; i++)
		{
			for (j = 0; j < colcount; j++)
			{
				workSheet->write(i + row_offset, j + column_offset, table->item(i, j) ? table->item(i, j)->text() : "", format);
			}
		}
		const double row_height = 25.5;
		workSheet->setRowHeight(row_offset, row_offset + rowcount - 1, row_height);
		// 		format.setFontBold(true);                   //加粗  
		// 		format.setFontItalic(true);                 //倾斜  
		// 		format.setFontUnderline(Format::FontUnderlineSingle);   //下划线  
		// 		format.setFontColor(Qt::red);               //字体颜色  
		// 		format.setHorizontalAlignment(Format::AlignRight);  //对齐方式  


		//画框线
// 		QString lrange;
// 		lrange.append(column_offset - 1 + 'A');
// 		lrange.append("4:");
// 		if (colcount == 38)
// 		{
// 			lrange.append("AL");
// 		}
// 		else
// 		{
// 			lrange.append(column_offset - 1 + colcount - 1 + 'A');
// 		}
// 		lrange.append(QString::number(table->rowCount() + 3));
// 		//QAxObject *range = worksheet->querySubObject("Range(const QString&)", lrange);
//		QXlsx::CellRange range(lrange);
// 		
// 		range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
// 		range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
// 		//调整数据区行高
// 		QString rowsName;
// 		rowsName.append("4:");
// 		rowsName.append(QString::number(table->rowCount() + 3));
// 		range = worksheet->querySubObject("Range(const QString&)", rowsName);
// 		range->setProperty("RowHeight", 25.5);
// 		//获取字体
// 		QAxObject *font = range->querySubObject("Font");
// 		//设置字体
// 		font->setProperty("Name", "宋体");
// 		//设置字体大小
// 		font->setProperty("Size", 9);
		//workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(savedFilePath));
		xlsx.saveAs(savedFilePath);//保存至fileName

		emit updateJobStatusSignal((int)JOB_QUESTION, savedFilePath);
	}
}
void ExportDataHelperByQtXlsx::table2Excel(QTableWidget *tableTemp, QString titleTemp)
{
	table = tableTemp;
	title = titleTemp;
	start();
}
