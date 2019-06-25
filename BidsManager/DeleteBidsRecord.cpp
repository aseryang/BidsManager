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
		QMessageBox::warning(NULL, "提示", "未填入有效的工程名称！", "确定"/*QMessageBox::Apply*/);
		return;
	}
	QString info;
	QString projectType;
	projectType = ConfigManager::Instance()->getProjectTypeStr();
	info.sprintf("是否删除【%s】数据库中，工程名为：%s 的记录?", projectType.toUtf8().data(), name.toUtf8().data());
	if (QMessageBox::question(NULL, "提示", info, "是", "否") == 0)
	{
		DBExecuteResult ret = DBManager::Instance()->deleteSingleRecord(name);
		switch (ret)
		{
		case DB_RET_OK:
			QMessageBox::information(NULL, "提示", "删除工程记录成功！", "确定");
			break;
		case DB_RET_RECORD_NOT_EXSIT:
			QMessageBox::warning(NULL, "提示", "该工程记录不存在！", "确定");
			break;
		case DB_EXECUTE_FAILED:
			QMessageBox::warning(NULL, "提示", "删除工程记录失败！", "确定");
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
