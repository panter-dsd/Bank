#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QRegExpValidator>
#include <QtGui/QGridLayout>
#include <QtGui/QApplication>

#include "qmyadddialog.h"

QMyAddDialog::QMyAddDialog(QWidget * parent, Qt::WFlags f)
	: QDialog(parent,f)
{
	createControls();
	setLayouts();
	createConnects();
}

void QMyAddDialog::createControls()
{
	familLabel_=new QLabel(tr("Famil"),this);
	familEdit_=new QLineEdit(this);
	nameLabel_=new QLabel(tr("Name"),this);
	nameEdit_=new QLineEdit(this);
	otchLabel_=new QLabel(tr("Otch"),this);
	otchEdit_=new QLineEdit(this);
	schetLabel_=new QLabel(tr("Schet"),this);
	schetEdit_=new QLineEdit(this);
	schetEdit_->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,20}"),this));
	schetEdit_->setMinimumWidth(QFontMetrics(schetEdit_->font()).width("0000000000000000000000"));
	moneyLabel_=new QLabel(tr("Money"),this);
	moneyEdit_=new QLineEdit(this);
	moneyEdit_->setValidator(new QDoubleValidator(this));

	buttons_=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}

void QMyAddDialog::setLayouts()
{
	QGridLayout* qglLayout=new QGridLayout();
	qglLayout->addWidget(familLabel_,0,0);
	qglLayout->addWidget(familEdit_,0,1);
	qglLayout->addWidget(nameLabel_,1,0);
	qglLayout->addWidget(nameEdit_,1,1);
	qglLayout->addWidget(otchLabel_,2,0);
	qglLayout->addWidget(otchEdit_,2,1);
	qglLayout->addWidget(schetLabel_,3,0);
	qglLayout->addWidget(schetEdit_,3,1);
	qglLayout->addWidget(moneyLabel_,4,0);
	qglLayout->addWidget(moneyEdit_,4,1);

	QVBoxLayout* qvblLayout=new QVBoxLayout();
	qvblLayout->addLayout(qglLayout);
	qvblLayout->addWidget(buttons_);

	setLayout(qvblLayout);
}

void QMyAddDialog::checkAndAccept()
{
	int checkRez=startCheck();
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

int QMyAddDialog::startCheck()
{
	int iValid=0;
	if (familEdit_->isVisible() && (familEdit_->text().isEmpty()))
	{
		setErrorPalette(familEdit_);
		iValid=1;
	}
	else
		familEdit_->setPalette(QApplication::palette());
	if (nameEdit_->isVisible() && (nameEdit_->text().isEmpty()))
	{
		setErrorPalette(nameEdit_);
		iValid=1;
	}
	else
		nameEdit_->setPalette(QApplication::palette());
	if (otchEdit_->isVisible() && (otchEdit_->text().isEmpty()))
	{
		setWarningPalette(otchEdit_);
		if (iValid==0) iValid=2;
	}
	else
		otchEdit_->setPalette(QApplication::palette());
	if (schetEdit_->isVisible() && (schetEdit_->text().isEmpty() || (schetEdit_->text().count()!=20)))
	{
		setErrorPalette(schetEdit_);
		iValid=1;
	}
	else
		schetEdit_->setPalette(QApplication::palette());
	return iValid;
}

void QMyAddDialog::createConnects()
{
	connect(familEdit_,SIGNAL(editingFinished ()),this,SLOT(startCheck()));
	connect(nameEdit_,SIGNAL(editingFinished ()),this,SLOT(startCheck()));
	connect(otchEdit_,SIGNAL(editingFinished ()),this,SLOT(startCheck()));
	connect(schetEdit_,SIGNAL(editingFinished ()),this,SLOT(startCheck()));
	connect(buttons_,SIGNAL(accepted()),this,SLOT(checkAndAccept()));
	connect(buttons_,SIGNAL(rejected()),this,SLOT(reject()));
}

