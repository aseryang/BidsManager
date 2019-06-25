#include "DeleteBidsRecord.h"
#include <QMessageBox>
#include "DBManager.h"
#include "ConfigManager.h"

DeleteBidsRecord::DeleteBidsRecord(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());
	setWindowIcon(QIcon(":/computer.png"));
}

DeleteBidsRecord::~DeleteBidsRecord()
{
}
void DeleteBidsRecord::onExecuteDeleteSlot()
{
	QString name = ui.le_projectName->text().trimmed();
	if (name.isEmpty())
	{
		QMessageBox::warning(NULL, "��ʾ", "δ������Ч�Ĺ������ƣ�", "ȷ��"/*QMessageBox::Apply*/);
		return;
	}
	QString info;
	QString projectType;
	projectType = ConfigManager::Instance()->getProjectTypeStr();
	info.sprintf("�Ƿ�ɾ����%s�����ݿ��У�������Ϊ��%s �ļ�¼?", projectType.toUtf8().data(), name.toUtf8().data());
	if (QMessageBox::question(NULL, "��ʾ", info, "��", "��") == 0)
	{
		DBExecuteResult ret = DBManager::Instance()->deleteSingleRecord(name);
		switch (ret)
		{
		case DB_RET_OK:
			QMessageBox::information(NULL, "��ʾ", "ɾ�����̼�¼�ɹ���", "ȷ��");
			break;
		case DB_RET_RECORD_NOT_EXSIT:
			QMessageBox::warning(NULL, "��ʾ", "�ù��̼�¼�����ڣ�", "ȷ��");
			break;
		case DB_EXECUTE_FAILED:
			QMessageBox::warning(NULL, "��ʾ", "ɾ�����̼�¼ʧ�ܣ�", "ȷ��");
			break;
		default:
			break;
		}
	}
	else
	{
		return;
	}
}
