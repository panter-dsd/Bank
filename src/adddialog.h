#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QtGui/QDialog>

class QLabel;
class QLineEdit;
class QDialogButtonBox;

class AddDialog : public QDialog
{
	Q_OBJECT

public:
	QLabel*						familLabel_;
	QLineEdit*					familEdit_;
	QLabel*						nameLabel_;
	QLineEdit*					nameEdit_;
	QLabel*						otchLabel_;
	QLineEdit*					otchEdit_;
	QLabel*						schetLabel_;
	QLineEdit*					schetEdit_;
	QLabel*						moneyLabel_;
	QLineEdit*					moneyEdit_;

	QDialogButtonBox*	buttons_;

private:
	void createControls();
	void setLayouts();
	void createConnects();
	bool check();
	void setErrorPalette(QWidget* widget) {
		QBrush brush(Qt::red);
		QPalette palette=widget->palette();
		palette.setBrush(QPalette::Active, QPalette::Base, brush);
		widget->setPalette(palette);
	}
	void setWarningPalette(QWidget* widget) {
		QBrush brush(Qt::yellow);
		QPalette palette=widget->palette();
		palette.setBrush(QPalette::Active, QPalette::Base, brush);
		widget->setPalette(palette);
	}

public:
	AddDialog( QWidget * parent = 0, Qt::WFlags f = Qt::WindowSystemMenuHint);

public Q_SLOTS:
	int startCheck();

private Q_SLOTS:
	void checkAndAccept();
};

#endif //ADDDIALOG_H
