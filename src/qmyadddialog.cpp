#include <QtGui>
#include "qmyadddialog.h"
//
QMyAddDialog::QMyAddDialog(QWidget * parent, Qt::WFlags f)
	: QDialog(parent,f)
{
	createControls();
	setLayouts();
	createConnects();
}
//
void QMyAddDialog::createControls()
{
	qlFamil=new QLabel(tr("Famil"),this);
	qleFamil=new QLineEdit(this);
	qlName=new QLabel(tr("Name"),this);
	qleName=new QLineEdit(this);
	qlOtch=new QLabel(tr("Otch"),this);
	qleOtch=new QLineEdit(this);
	qlSchet=new QLabel(tr("Schet"),this);
	qleSchet=new QLineEdit(this);
	qleSchet->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,20}"),this));
	qleSchet->setMinimumWidth(QFontMetrics(qleSchet->font()).width("0000000000000000000000"));
	qlMoney=new QLabel(tr("Money"),this);
	qleMoney=new QLineEdit(this);
	qleMoney->setValidator(new QDoubleValidator(this));
	
	qdbbButtons=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}
//
void QMyAddDialog::setLayouts()
{
	QGridLayout* qglLayout=new QGridLayout();
	qglLayout->addWidget(qlFamil,0,0);
	qglLayout->addWidget(qleFamil,0,1);
	qglLayout->addWidget(qlName,1,0);
	qglLayout->addWidget(qleName,1,1);
	qglLayout->addWidget(qlOtch,2,0);
	qglLayout->addWidget(qleOtch,2,1);
	qglLayout->addWidget(qlSchet,3,0);
	qglLayout->addWidget(qleSchet,3,1);
	qglLayout->addWidget(qlMoney,4,0);
	qglLayout->addWidget(qleMoney,4,1);
	
	QVBoxLayout* qvblLayout=new QVBoxLayout();
	qvblLayout->addLayout(qglLayout);
	qvblLayout->addWidget(qdbbButtons);
	
	setLayout(qvblLayout);
}
//
void QMyAddDialog::slotAccept()
{
	int checkRez=slotCheck();
	switch (checkRez)
	{
	case 0: {accept(); break;}
	case 1:
		{
			QMessageBox::critical(this,
						tr("Error"),
						tr("Find errors"));
			break;
		}
	case 2:
		{
			int rez=QMessageBox::critical(this,
						tr("Warning"),
						tr("Find warnings. Continue?"),
						QMessageBox::Yes,
						QMessageBox::No);
			if (rez==QMessageBox::Yes)
			accept();
			break;
		}
	}
}
//
int QMyAddDialog::slotCheck()
{
	int iValid=0;
	if (qleFamil->isVisible() && (qleFamil->text().isEmpty()))
	{
		setErrorPalette(qleFamil);
		iValid=1;
	}
	else
		qleFamil->setPalette(QApplication::palette());
	if (qleName->isVisible() && (qleName->text().isEmpty()))
	{
		setErrorPalette(qleName);
		iValid=1;
	}
	else
		qleName->setPalette(QApplication::palette());
	if (qleOtch->isVisible() && (qleOtch->text().isEmpty()))
	{
		setWarningPalette(qleOtch);
		if (iValid==0) iValid=2;
	}
	else
		qleOtch->setPalette(QApplication::palette());
	if (qleSchet->isVisible() && (qleSchet->text().isEmpty() || (qleSchet->text().count()!=20)))
	{
		setErrorPalette(qleSchet);
		iValid=1;
	}
	else
		qleSchet->setPalette(QApplication::palette());
	return iValid;
}
//
void QMyAddDialog::createConnects()
{
	connect(qleFamil,SIGNAL(editingFinished ()),this,SLOT(slotCheck()));
	connect(qleName,SIGNAL(editingFinished ()),this,SLOT(slotCheck()));
	connect(qleOtch,SIGNAL(editingFinished ()),this,SLOT(slotCheck()));
	connect(qleSchet,SIGNAL(editingFinished ()),this,SLOT(slotCheck()));
	connect(qdbbButtons,SIGNAL(accepted()),this,SLOT(slotAccept()));
	connect(qdbbButtons,SIGNAL(rejected()),this,SLOT(reject()));
}
//
