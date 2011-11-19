#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>

#include "preferences.h"

Preferences::Preferences (QWidget *parent, Qt::WindowFlags f)
	: QDialog (parent, f)
{
	applicationSettings_ = new QSettings (QSettings::IniFormat, QSettings::UserScope, "PanteR", "Bank");
	createWidgets();
	createLayouts();
	createConnects();
}

void Preferences::createWidgets()
{
	openPathLabel_ = new QLabel (tr ("Dir with reestr"), this);
	openPathEdit_ = new QLineEdit (QDir::toNativeSeparators (applicationSettings_->value ("PATH/OpenPath", "").toString()), this);
	openPathButton_ = new QPushButton (tr ("Browse..."), this);
	importPathLabel_ = new QLabel (tr ("Dir for import"), this);
	importPathEdit_ = new QLineEdit (QDir::toNativeSeparators (applicationSettings_->value ("PATH/ImportPath", "").toString()), this);
	importPathButton_ = new QPushButton (tr ("Browse..."), this);
	archivesPathLabel_ = new QLabel (tr ("Dir for archives"), this);
	archivesPathEdit_ = new QLineEdit (QDir::toNativeSeparators (applicationSettings_->value ("PATH/ArchivesPath", "").toString()), this);
	archivesPathButton_ = new QPushButton (tr ("Browse..."), this);
	outPathLabel_ = new QLabel (tr ("Dir for out"), this);
	outPathEdit_ = new QLineEdit (QDir::toNativeSeparators (applicationSettings_->value ("PATH/OutPath", "").toString()), this);
	outPathButton_ = new QPushButton (tr ("Browse..."), this);

	buttons = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
}

void Preferences::createLayouts()
{
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->addWidget (openPathLabel_, 0, 0);
	gridLayout->addWidget (openPathEdit_, 0, 1);
	gridLayout->addWidget (openPathButton_, 0, 2);
	gridLayout->addWidget (importPathLabel_, 1, 0);
	gridLayout->addWidget (importPathEdit_, 1, 1);
	gridLayout->addWidget (importPathButton_, 1, 2);
	gridLayout->addWidget (archivesPathLabel_, 2, 0);
	gridLayout->addWidget (archivesPathEdit_, 2, 1);
	gridLayout->addWidget (archivesPathButton_, 2, 2);
	gridLayout->addWidget (outPathLabel_, 3, 0);
	gridLayout->addWidget (outPathEdit_, 3, 1);
	gridLayout->addWidget (outPathButton_, 3, 2);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout (gridLayout);
	mainLayout->addWidget (buttons);
	setLayout (mainLayout);
}

void Preferences::savePreferences()
{
	applicationSettings_->setValue ("PATH/OpenPath", openPathEdit_->text());
	applicationSettings_->setValue ("PATH/ImportPath", importPathEdit_->text());
	applicationSettings_->setValue ("PATH/ArchivesPath", archivesPathEdit_->text());
	applicationSettings_->setValue ("PATH/OutPath", outPathEdit_->text());
	applicationSettings_->sync();
}

void Preferences::createConnects()
{
	connect (buttons->button (QDialogButtonBox::Apply), SIGNAL (pressed()), SLOT (savePreferences()));
	connect (buttons->button (QDialogButtonBox::Ok), SIGNAL (pressed()), SLOT (savePreferences()));
	connect (buttons->button (QDialogButtonBox::Ok), SIGNAL (pressed()), SLOT (accept()));
	connect (buttons->button (QDialogButtonBox::Cancel), SIGNAL (pressed()), SLOT (reject()));

	connect (openPathButton_, SIGNAL (pressed()), SLOT (setOpenPath()));
	connect (importPathButton_, SIGNAL (pressed()), SLOT (setImportPath()));
	connect (archivesPathButton_, SIGNAL (pressed()), SLOT (setArchivesPath()));
	connect (outPathButton_, SIGNAL (pressed()), SLOT (setOutPath()));
}

void Preferences::setOpenPath()
{
	const QString dir = QFileDialog::getExistingDirectory (this, "", openPathEdit_->text());

	if (!dir.isEmpty()) {
		openPathEdit_->setText (QDir::toNativeSeparators (dir));
	}
}

void Preferences::setImportPath()
{
	const QString dir = QFileDialog::getExistingDirectory (this, "", importPathEdit_->text());

	if (!dir.isEmpty()) {
		importPathEdit_->setText (QDir::toNativeSeparators (dir));
	}
}

void Preferences::setArchivesPath()
{
	const QString dir = QFileDialog::getExistingDirectory (this, "", archivesPathEdit_->text());

	if (!dir.isEmpty()) {
		archivesPathEdit_->setText (QDir::toNativeSeparators (dir));
	}
}

void Preferences::setOutPath()
{
	const QString dir = QFileDialog::getExistingDirectory (this, "", outPathEdit_->text());

	if (!dir.isEmpty()) {
		outPathEdit_->setText (QDir::toNativeSeparators (dir));
	}
}

