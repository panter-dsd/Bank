#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>

#include "preferences.h"

Preferences::Preferences(QWidget * parent, Qt::WindowFlags f)
	:QDialog(parent,f)
{
	applicationSettings_=new QSettings(QSettings::IniFormat,QSettings::UserScope,"PanteR","Bank");
	createWidgets();
	createLayouts();
	createConnects();
}

void Preferences::createWidgets()
{
	openPathLabel_=new QLabel(tr("Dir with reestr"),this);
	openPathEdit_=new QLineEdit(QDir::toNativeSeparators(applicationSettings_->value("PATH/OpenPath","").toString()),this);
	openPathButton_=new QPushButton(tr("Browse..."),this);
	importPathLabel_=new QLabel(tr("Dir for import"),this);
	importPathEdit_=new QLineEdit(QDir::toNativeSeparators(applicationSettings_->value("PATH/ImportPath","").toString()),this);
	importPathButton_=new QPushButton(tr("Browse..."),this);
	archivesPathLabel_=new QLabel(tr("Dir for archives"),this);
	archivesPathEdit_=new QLineEdit(QDir::toNativeSeparators(applicationSettings_->value("PATH/ArchivesPath","").toString()),this);
	archivesPathButton_=new QPushButton(tr("Browse..."),this);
	outPathLabel_=new QLabel(tr("Dir for out"),this);
	outPathEdit_=new QLineEdit(QDir::toNativeSeparators(applicationSettings_->value("PATH/OutPath","").toString()),this);
	outPathButton_=new QPushButton(tr("Browse..."),this);

	buttons=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
}

void Preferences::createLayouts()
{
	QGridLayout* qglGridLayout=new QGridLayout();
	qglGridLayout->addWidget(openPathLabel_,0,0);
	qglGridLayout->addWidget(openPathEdit_,0,1);
	qglGridLayout->addWidget(openPathButton_,0,2);
	qglGridLayout->addWidget(importPathLabel_,1,0);
	qglGridLayout->addWidget(importPathEdit_,1,1);
	qglGridLayout->addWidget(importPathButton_,1,2);
	qglGridLayout->addWidget(archivesPathLabel_,2,0);
	qglGridLayout->addWidget(archivesPathEdit_,2,1);
	qglGridLayout->addWidget(archivesPathButton_,2,2);
	qglGridLayout->addWidget(outPathLabel_,3,0);
	qglGridLayout->addWidget(outPathEdit_,3,1);
	qglGridLayout->addWidget(outPathButton_,3,2);

	QVBoxLayout* qvblMainLayout=new QVBoxLayout();
	qvblMainLayout->addLayout(qglGridLayout);
	qvblMainLayout->addWidget(buttons);
	setLayout(qvblMainLayout);
}

void Preferences::savePreferences()
{
	applicationSettings_->setValue("PATH/OpenPath",openPathEdit_->text());
	applicationSettings_->setValue("PATH/ImportPath",importPathEdit_->text());
	applicationSettings_->setValue("PATH/ArchivesPath",archivesPathEdit_->text());
	applicationSettings_->setValue("PATH/OutPath",outPathEdit_->text());
	applicationSettings_->sync();
}

void Preferences::createConnects()
{
	connect(buttons->button(QDialogButtonBox::Apply),SIGNAL(pressed()),this,SLOT(savePreferences()));
	connect(buttons->button(QDialogButtonBox::Ok),SIGNAL(pressed()),this,SLOT(savePreferences()));
	connect(buttons->button(QDialogButtonBox::Ok),SIGNAL(pressed()),this,SLOT(accept()));
	connect(buttons->button(QDialogButtonBox::Cancel),SIGNAL(pressed()),this,SLOT(reject()));

	connect(openPathButton_,SIGNAL(pressed()),this,SLOT(setOpenPath()));
	connect(importPathButton_,SIGNAL(pressed()),this,SLOT(setImportPath()));
	connect(archivesPathButton_,SIGNAL(pressed()),this,SLOT(setArchivesPath()));
	connect(outPathButton_,SIGNAL(pressed()),this,SLOT(setOutPath()));
}

void Preferences::setOpenPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",openPathEdit_->text());
	if (!qsDir.isEmpty())
		openPathEdit_->setText(QDir::toNativeSeparators(qsDir));
}

void Preferences::setImportPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",importPathEdit_->text());
	if (!qsDir.isEmpty())
		importPathEdit_->setText(QDir::toNativeSeparators(qsDir));
}

void Preferences::setArchivesPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",archivesPathEdit_->text());
	if (!qsDir.isEmpty())
		archivesPathEdit_->setText(QDir::toNativeSeparators(qsDir));
}

void Preferences::setOutPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",outPathEdit_->text());
	if (!qsDir.isEmpty())
		outPathEdit_->setText(QDir::toNativeSeparators(qsDir));
}

