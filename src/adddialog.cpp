#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QRegExpValidator>
#include <QtGui/QGridLayout>
#include <QtGui/QApplication>

#include "adddialog.h"

AddDialog::AddDialog (QWidget *parent, Qt::WFlags f)
	: QDialog (parent, f)
{
	createControls();
	setLayouts();
	createConnects();
}

void AddDialog::createControls()
{
	familLabel_ = new QLabel (tr ("Famil"), this);
	familEdit_ = new QLineEdit (this);
	nameLabel_ = new QLabel (tr ("Name"), this);
	nameEdit_ = new QLineEdit (this);
	otchLabel_ = new QLabel (tr ("Otch"), this);
	otchEdit_ = new QLineEdit (this);
	schetLabel_ = new QLabel (tr ("Schet"), this);
	schetEdit_ = new QLineEdit (this);
	schetEdit_->setValidator (new QRegExpValidator (QRegExp ("[0-9]{1,20}"), this));
	schetEdit_->setMinimumWidth (QFontMetrics (schetEdit_->font()).width ("0000000000000000000000"));
	moneyLabel_ = new QLabel (tr ("Money"), this);
	moneyEdit_ = new QLineEdit (this);
	moneyEdit_->setValidator (new QDoubleValidator (this));

	buttons_ = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}

void AddDialog::setLayouts()
{
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->addWidget (familLabel_, 0, 0);
	gridLayout->addWidget (familEdit_, 0, 1);
	gridLayout->addWidget (nameLabel_, 1, 0);
	gridLayout->addWidget (nameEdit_, 1, 1);
	gridLayout->addWidget (otchLabel_, 2, 0);
	gridLayout->addWidget (otchEdit_, 2, 1);
	gridLayout->addWidget (schetLabel_, 3, 0);
	gridLayout->addWidget (schetEdit_, 3, 1);
	gridLayout->addWidget (moneyLabel_, 4, 0);
	gridLayout->addWidget (moneyEdit_, 4, 1);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout (gridLayout);
	mainLayout->addWidget (buttons_);

	setLayout (mainLayout);
}

void AddDialog::checkAndAccept()
{
	const int checkResult = startCheck();

	switch (checkResult) {
		case 0: {
			accept();
			break;
		}
		case 1: {
			QMessageBox::critical (this, tr ("Error"), tr ("Find errors"));
			break;
		}
		case 2: {
			const int result = QMessageBox::critical (this,
											 tr ("Warning"),
											 tr ("Find warnings. Continue?"),
											 QMessageBox::Yes,
											 QMessageBox::No);

			if (result == QMessageBox::Yes) {
				accept();
			}

			break;
		}
	}
}

int AddDialog::startCheck()
{
	int isValid = 0;

	if (familEdit_->isVisible() && (familEdit_->text().isEmpty())) {
		setErrorPalette (familEdit_);
		isValid = 1;
	} else
		familEdit_->setPalette (QApplication::palette());

	if (nameEdit_->isVisible() && (nameEdit_->text().isEmpty())) {
		setErrorPalette (nameEdit_);
		isValid = 1;
	} else
		nameEdit_->setPalette (QApplication::palette());

	if (otchEdit_->isVisible() && (otchEdit_->text().isEmpty())) {
		setWarningPalette (otchEdit_);

		if (isValid == 0)
			isValid = 2;
	} else
		otchEdit_->setPalette (QApplication::palette());

	if (schetEdit_->isVisible() && (schetEdit_->text().isEmpty() || (schetEdit_->text().count() != 20))) {
		setErrorPalette (schetEdit_);
		isValid = 1;
	} else
		schetEdit_->setPalette (QApplication::palette());

	return isValid;
}

void AddDialog::createConnects()
{
	connect (familEdit_, SIGNAL (editingFinished ()), SLOT (startCheck()));
	connect (nameEdit_, SIGNAL (editingFinished ()), SLOT (startCheck()));
	connect (otchEdit_, SIGNAL (editingFinished ()), SLOT (startCheck()));
	connect (schetEdit_, SIGNAL (editingFinished ()), SLOT (startCheck()));
	connect (buttons_, SIGNAL (accepted()), SLOT (checkAndAccept()));
	connect (buttons_, SIGNAL (rejected()), SLOT (reject()));
}
