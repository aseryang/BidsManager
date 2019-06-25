#pragma once

#include <QWidget>
#include "ui_DeleteBidsRecord.h"
#include "GlobalDefine.h"

class DeleteBidsRecord : public QWidget
{
	Q_OBJECT

public:
	DeleteBidsRecord(QWidget *parent = Q_NULLPTR);
	~DeleteBidsRecord();
public slots:
	void onExecuteDeleteSlot();

private:
	Ui::DeleteBidsRecord ui;
};
