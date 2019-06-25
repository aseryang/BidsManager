#include "RecordsDirImport.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ConfigManager.h"
RecordsDirImport::RecordsDirImport(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());
	setWindowIcon(QIcon(":/computer.png"));
	ui.lb_runninginfo->clear();
	ui.lb_currentstep->clear();
	ui.pb_curStep->reset();
	qRegisterMetaType<BatchImportStatus>("BatchImportStatus");
	qRegisterMetaType<BatchImportRunningInfo>("BatchImportRunningInfo");
	connect(&batchImport, SIGNAL(runningInfoSignal(BatchImportStatus, BatchImportRunningInfo*)), this, SLOT(onUpdateRunningInfoSlot(BatchImportStatus, BatchImportRunningInfo*)));
}

RecordsDirImport::~RecordsDirImport()
{
}
void RecordsDirImport::onDirChooseSlot()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

	QString directory = QFileDialog::getExistingDirectory(this,
		"ѡ���ļ���",
		ui.le_batchDir->text(),
		options);
	if (!directory.isEmpty())
		ui.le_batchDir->setText(directory);
}
void RecordsDirImport::onStartBatchImportSlot()
{
	QString dir = ui.le_batchDir->text().trimmed();
	if (dir.isEmpty())
	{
		QMessageBox::warning(NULL, "��ʾ", "����ѡ��Excel�ļ�����Ŀ¼��", "ȷ��"/*QMessageBox::Apply*/);
		return;
	}
	QString projectType = ConfigManager::Instance()->getProjectTypeStr();
	QString info;
	info.sprintf("�Ƿ��뵽��%s�����ݿ��У�", projectType.toUtf8().data());
	if (QMessageBox::question(NULL, "��ʾ", info, "��", "��") == 0)
	{
		batchImport.doImport(dir);
		ui.btn_startBatchimport->setEnabled(false);
	}
	else
	{
		return;
	}
}
void RecordsDirImport::closeEvent(QCloseEvent*event)
{
	batchImport.stopImport();
}
void RecordsDirImport::UIReset()
{
	ui.lb_runninginfo->clear();
	ui.lb_currentstep->clear();
	ui.pb_curStep->reset();
	ui.btn_startBatchimport->setEnabled(true);
}
void RecordsDirImport::onUpdateRunningInfoSlot(BatchImportStatus status, BatchImportRunningInfo* info)
{
	switch (status)
	{
	case BIS_RUNNING_INFO:
	{
		ui.lb_runninginfo->setText(info->runningInfo);
		ui.lb_currentstep->setText(info->currentFileIndex);
		QStringList list;
		list = info->currentFileIndex.split('/');
		double cur = list.at(0).toDouble();
		double total = list.at(1).toDouble();
		if (total > 0)
		{
			int value = int(((cur - 1) / total) * 100);
			ui.pb_curStep->setValue(value);
		}
		break;
	}
	case BIS_ERROR_INFO:
	{
		QMessageBox::warning(NULL, "��ʾ", info->errorInfo, "ȷ��"/*QMessageBox::Apply*/);
		UIReset();
		break;
	}
	case BIS_FINISHED:
	{
		const int all_done = 100;
		ui.pb_curStep->setValue(all_done);
		ui.lb_runninginfo->setText("���������");
		if (info->errorInfo.isEmpty())
		{
			QMessageBox::information(NULL, "��ʾ", "����ɹ���", "ȷ��"/*QMessageBox::Apply*/);
		}
		else
		{
			QMessageBox::warning(NULL, "��ʾ", info->errorInfo, "ȷ��"/*QMessageBox::Apply*/);
		}
		UIReset();
		break;
	}
	default:
	{
		UIReset();
		break;
	}
	}
}
