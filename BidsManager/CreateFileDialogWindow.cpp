#include "CreateFileDialogWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "ConfigManager.h"
//#include "ExportDataHelper.h"
#include "ExportDataHelperByQtXlsx.h"
CreateRecFileDialog::CreateRecFileDialog()
{
	ui.setupUi(this);
	ui.btnCreateFile->setEnabled(false);
	QDir tempDir;
	//��ʱ�������ǰ·��
	QString currentDir = tempDir.currentPath();
	ui.fileDir->setText(currentDir);
	ui.btnChooseDir->setFlat(true);
	setWindowIcon(QIcon(":/computer.png"));
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}
int CreateRecFileDialog::createFile(QString filePath, QString fileName)
{
	QDir tempDir;
	//��ʱ�������ǰ·��
	QString currentDir = tempDir.currentPath();
	//���filePath·�������ڣ�������
	if(!tempDir.exists(filePath))
	{
		qDebug()<<"�����ڸ�·��"<<endl;
		tempDir.mkpath(filePath);
	}
	QFile *tempFile = new QFile;
	//�������ִ��·�����õ�filePath��
	tempDir.setCurrent(filePath);
	qDebug()<<tempDir.currentPath();
	//���filePath·�����Ƿ�����ļ�fileName,���ֹͣ������
	if(tempFile->exists(fileName))
	{
		qDebug()<<"�ļ�����";
		return 0;
	}
	//��ʱ��·����û��fileName�ļ���ʹ����������ڵ�ǰ·���´����ļ�
	tempFile->setFileName(fileName);
	if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
	{
		qDebug()<<"��ʧ��";
		return 1;
	}
	tempFile->close();
	//������ǰ·������Ϊԭ����·��
	tempDir.setCurrent(currentDir);
	qDebug()<<tempDir.currentPath();
	return 0;
}
void CreateRecFileDialog::fileNameInputed()
{
	if (ui.fileName->text().isEmpty())
	{
		ui.btnCreateFile->setEnabled(false);
	}
	else
	{
		ui.btnCreateFile->setEnabled(true);
	}
}
void CreateRecFileDialog::btnCreateFileClicked()
{
	if (ui.fileDir->text().isEmpty() || 
		ui.fileName->text().isEmpty())
	{
		return;
	}
	
	filePath = ui.fileDir->text() + "\\" + ui.fileName->text() + FILE_POSTFIX;
	filePath.replace("\\", "/");
	ExportDataHelperByQtXlsx::Instance()->setFilePath(filePath);
	//ConfigManager::GetInstance()->setDataFilePath(filePath);
	//emit updateUIfilePath(UI_UPDATE_FILEPATH, &filePath);
	hide();
}
void CreateRecFileDialog::btnChooseDirClicked()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

	QString directory = QFileDialog::getExistingDirectory(this,
		"ѡ���ļ���",
		ui.fileDir->text(),
		options);
	if (!directory.isEmpty())
		ui.fileDir->setText(directory);
}
void CreateRecFileDialog::btnCancleClicked()
{
	ExportDataHelperByQtXlsx::Instance()->reset();
	hide();
}