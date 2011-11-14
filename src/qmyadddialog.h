#ifndef QMYADDDIALOG_H
#define QMYADDDIALOG_H
//
#include <QDialog>
class QLabel;
class QLineEdit;
class QDialogButtonBox;
//
class QMyAddDialog : public QDialog
{
Q_OBJECT
public:
	QLabel*						qlFamil;
	QLineEdit*					qleFamil;
	QLabel*						qlName;
	QLineEdit*					qleName;
	QLabel*						qlOtch;
	QLineEdit*					qleOtch;
	QLabel*						qlSchet;
	QLineEdit*					qleSchet;
	QLabel*						qlMoney;
	QLineEdit*					qleMoney;
	
	QDialogButtonBox*	qdbbButtons;
private:
	void createControls();
	void setLayouts();
	void createConnects();
	bool check();
	void setErrorPalette(QWidget* qwWidget)
	{
		QBrush brush(Qt::red);
		QPalette palette=qwWidget->palette();
		palette.setBrush(QPalette::Active, QPalette::Base, brush); 
		qwWidget->setPalette(palette);
	}
	void setWarningPalette(QWidget* qwWidget)
	{
		QBrush brush(Qt::yellow);
		QPalette palette=qwWidget->palette();
		palette.setBrush(QPalette::Active, QPalette::Base, brush); 
		qwWidget->setPalette(palette);
	}
public:
	QMyAddDialog( QWidget * parent = 0, Qt::WFlags f = Qt::WindowSystemMenuHint);
	void startCheck() {slotCheck();}
private slots:
	int slotCheck();
	void slotAccept();
};

#endif
