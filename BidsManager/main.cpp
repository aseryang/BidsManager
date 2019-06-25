#include "BidsManager.h"
#include <QtWidgets/QApplication>
#include <QTextCodec> 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BidsManager w;
	w.setFixedSize(w.width(), w.height());
	w.setWindowIcon(QIcon(":/computer.png"));
	w.setWindowTitle(QString(APP_TITLE) + APP_VERSION);
	w.show();
	return a.exec();
}
