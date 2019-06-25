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
	if (title.indexOf("�����¼���ܱ�") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 1;
	}
	else if (title.indexOf("�����¼��") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 2;
	}
	else if (title.indexOf("ʩ����λ���ܱ�") >= 0)
	{
		column_offset = 20;
		row_offset = 4;
		sheet_num = 2;
	}
	else if (title.indexOf("�б�����¼��") >= 0)
	{
		column_offset = 1;
		row_offset = 4;
		sheet_num = 3;
	}
	else if (title.indexOf("�б������ܱ�") >= 0)
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
			excel->dynamicCall("SetVisible (bool Visible)", "false");//����ʾ����
			excel->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
			QAxObject *workbooks = excel->querySubObject("WorkBooks");//��ȡ����������
			if (!workbooks)
			{
				//QMessageBox::warning(NULL, "����", "��ȡ����������ʧ�ܡ�", QMessageBox::Apply);
				emit updateJobStatusSignal((int)JOB_WARNING, QString("��ȡ����������ʧ�ܡ�"));
				return;
			}
			workbooks->dynamicCall("Open (const QString&)", templateFilePath);
			QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
			if (!workbook)
			{
				emit updateJobStatusSignal((int)JOB_WARNING, QString("��Template.xlsxʧ�ܡ�"));
				//QMessageBox::warning(NULL, "����", "��Template.xlsxʧ�ܡ�", QMessageBox::Apply);
				return;
			}
			QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", sheet_num);
			if (!worksheet)
			{
				emit updateJobStatusSignal((int)JOB_WARNING, QString("��Sheetҳʧ�ܡ�"));
				//QMessageBox::warning(NULL, "����", "��Sheetҳʧ�ܡ�", QMessageBox::Apply);
				return;
			}
			int i, j;
			int colcount = table->columnCount();
			int rowcount = table->rowCount();

			//���ñ��ⵥԪ��
			QAxObject* titleCell = worksheet->querySubObject("Cells(int,int)", 1, 1);
			titleCell->dynamicCall("SetValue(const QString&)", title);

			//QTableWidget ��ȡ������ݲ���
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
			//������
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
			//�����������и�
			QString rowsName;
			rowsName.append("4:");
			rowsName.append(QString::number(table->rowCount() + 3));
			range = worksheet->querySubObject("Range(const QString&)", rowsName);
			range->setProperty("RowHeight", 25.5);
			//��ȡ����
			QAxObject *font = range->querySubObject("Font");
			//��������
			font->setProperty("Name", "����");
			//���������С
			font->setProperty("Size", 9);
			workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(savedFilePath));//������fileName
			workbook->dynamicCall("Close()");//�رչ�����
			excel->dynamicCall("Quit()");//�ر�excel
			delete excel;
			excel = NULL;
			emit updateJobStatusSignal((int)JOB_QUESTION, savedFilePath);
// 			if (QMessageBox::question(NULL, "���", "�ļ��Ѿ��������Ƿ����ڴ򿪣�", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
// 			{
// 				QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(savedFilePath)));
// 			}
		}
		else
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("δ�ܴ��� Excel �����밲װ Microsoft Excel��"));
			//QMessageBox::warning(NULL, "����", "δ�ܴ��� Excel �����밲װ Microsoft Excel��", QMessageBox::Apply);
		}
	}
}
void ExportDataHelper::table2Excel(QTableWidget *tableTemp, QString titleTemp)
{
	table = tableTemp;
	title = titleTemp;
	start();
}
