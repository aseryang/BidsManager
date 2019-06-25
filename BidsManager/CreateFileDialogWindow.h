#pragma once
#include "GlobalDefine.h"
#include <QDialog>
#include "ui_createfile.h"
class CreateRecFileDialog: public QDialog
{
	Q_OBJECT
public:
	CreateRecFileDialog();
protected:
	int createFile(QString filePath,QString fileName);	
private slots:
	void btnCreateFileClicked();
	void btnChooseDirClicked();
	void btnCancleClicked();
	void fileNameInputed();
signals:
	void updateUIfilePath(int cmd, void* arg);
private:
	Ui::createFileDialog      ui;
	QString					  filePath;
};