#pragma once
#include <QThread>
#include <QMessageBox>

class MsgBoxThread : public QThread
{
	Q_OBJECT
		typedef enum {
		mbt_about = 0,
		mbt_aboutqt = 1,
		mbt_critical = 2,
		mbt_information = 3,
		mbt_question = 4,
		mbt_warning = 5
	} MSGBOXTYPE;
protected:
	int m_btnres;
	void about(QWidget * parent, const QString &title, const QString &text) {
		emit msgbox_sig(mbt_about, parent, title, text, QMessageBox::NoButton, QMessageBox::NoButton);
	}

	void aboutQt(QWidget *parent, const QString &title = QString()) {
		emit msgbox_sig(mbt_aboutqt, parent, title, tr(""), QMessageBox::NoButton, QMessageBox::NoButton);
	}

	int critical(QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButtons defaultButton = QMessageBox::NoButton) {
		emit msgbox_sig(mbt_critical, parent, title, text, buttons, defaultButton);
		return m_btnres;
	}

	int information(QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButtons defaultButton = QMessageBox::NoButton)
	{
		emit msgbox_sig(mbt_information, parent, title, text, buttons, defaultButton);
		return m_btnres;
	}

	int question(QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::StandardButtons defaultButton = QMessageBox::NoButton) {
		emit msgbox_sig(mbt_question, parent, title, text, buttons, defaultButton);
		return m_btnres;
	}

	int warning(QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButtons defaultButton = QMessageBox::NoButton) {
		emit msgbox_sig(mbt_warning, parent, title, text, buttons, defaultButton);
		return m_btnres;
	}


public:
	MsgBoxThread(QObject * parent = 0)
		:QThread(parent), m_btnres(QMessageBox::NoButton) {
		qRegisterMetaType<QMessageBox::StandardButtons>("QMessageBox::StandardButtons");
		connect(this, SIGNAL(msgbox_sig(MSGBOXTYPE, QWidget *, const QString, const QString, QMessageBox::StandardButtons, QMessageBox::StandardButtons)), SLOT(on_msgbox(MSGBOXTYPE, QWidget *, const QString, const QString, QMessageBox::StandardButtons, QMessageBox::StandardButtons)), Qt::BlockingQueuedConnection);
	}
signals:
	void msgbox_sig(MSGBOXTYPE type, QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButtons defaultButton);

private slots:
	void on_msgbox(MSGBOXTYPE type, QWidget * parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButtons defaultButton) {
		switch (type) {
		case mbt_about:
			QMessageBox::about(parent, title, text);
			break;
		case mbt_aboutqt:
			QMessageBox::aboutQt(parent, title);
			break;
		case mbt_critical:
			m_btnres = QMessageBox::critical(parent, title, text, buttons, defaultButton);
			break;
		case mbt_information:
			m_btnres = QMessageBox::information(parent, title, text, buttons, defaultButton);
			break;
		case mbt_question:
			m_btnres = QMessageBox::question(parent, title, text, buttons, defaultButton);
			break;
		case mbt_warning:
			m_btnres = QMessageBox::warning(parent, title, text, buttons, defaultButton);
			break;
		default:
			Q_ASSERT_X(false, "QMessageBox in thread", "invalid box type specified.");
		}
	}
};