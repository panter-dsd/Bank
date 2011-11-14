#include <QtGui>
#include "qmypreferences.h"

QMyPreferences::QMyPreferences(QWidget * parent, Qt::WindowFlags f)
	:QDialog(parent,f)
{
	qsetAppSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,"PanteR","Bank");
	createWidgets();
	createLayouts();
	createConnects();
}
//
void QMyPreferences::createWidgets()
{
	qlOpenPath=new QLabel(tr("Dir with reestr"),this);
	qleOpenPath=new QLineEdit(QDir::toNativeSeparators(qsetAppSettings->value("PATH/OpenPath","").toString()),this);
	qpbOpenPath=new QPushButton(tr("Browse..."),this);
	qlImportPath=new QLabel(tr("Dir for import"),this);
	qleImportPath=new QLineEdit(QDir::toNativeSeparators(qsetAppSettings->value("PATH/ImportPath","").toString()),this);
	qpbImportPath=new QPushButton(tr("Browse..."),this);
	qlArchivesPath=new QLabel(tr("Dir for archives"),this);
	qleArchivesPath=new QLineEdit(QDir::toNativeSeparators(qsetAppSettings->value("PATH/ArchivesPath","").toString()),this);
	qpbArchivesPath=new QPushButton(tr("Browse..."),this);
	qlOutPath=new QLabel(tr("Dir for out"),this);
	qleOutPath=new QLineEdit(QDir::toNativeSeparators(qsetAppSettings->value("PATH/OutPath","").toString()),this);
	qpbOutPath=new QPushButton(tr("Browse..."),this);
	
	qdbbButtonBox=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
}
//
void QMyPreferences::createLayouts()
{
	QGridLayout* qglGridLayout=new QGridLayout();
	qglGridLayout->addWidget(qlOpenPath,0,0);
	qglGridLayout->addWidget(qleOpenPath,0,1);
	qglGridLayout->addWidget(qpbOpenPath,0,2);
	qglGridLayout->addWidget(qlImportPath,1,0);
	qglGridLayout->addWidget(qleImportPath,1,1);
	qglGridLayout->addWidget(qpbImportPath,1,2);
	qglGridLayout->addWidget(qlArchivesPath,2,0);
	qglGridLayout->addWidget(qleArchivesPath,2,1);
	qglGridLayout->addWidget(qpbArchivesPath,2,2);
	qglGridLayout->addWidget(qlOutPath,3,0);
	qglGridLayout->addWidget(qleOutPath,3,1);
	qglGridLayout->addWidget(qpbOutPath,3,2);
	
	QVBoxLayout* qvblMainLayout=new QVBoxLayout();
	qvblMainLayout->addLayout(qglGridLayout);
	qvblMainLayout->addWidget(qdbbButtonBox);
	this->setLayout(qvblMainLayout);
}
//
void QMyPreferences::slotSavePreferences()
{
	qsetAppSettings->setValue("PATH/OpenPath",qleOpenPath->text());
	qsetAppSettings->setValue("PATH/ImportPath",qleImportPath->text());
	qsetAppSettings->setValue("PATH/ArchivesPath",qleArchivesPath->text());
	qsetAppSettings->setValue("PATH/OutPath",qleOutPath->text());
	qsetAppSettings->sync();
}
//
void QMyPreferences::createConnects()
{
	connect(qdbbButtonBox->button(QDialogButtonBox::Apply),SIGNAL(pressed()),this,SLOT(slotSavePreferences()));
	connect(qdbbButtonBox->button(QDialogButtonBox::Ok),SIGNAL(pressed()),this,SLOT(slotSavePreferences()));	
	connect(qdbbButtonBox->button(QDialogButtonBox::Ok),SIGNAL(pressed()),this,SLOT(accept()));
	connect(qdbbButtonBox->button(QDialogButtonBox::Cancel),SIGNAL(pressed()),this,SLOT(reject()));
	
	connect(qpbOpenPath,SIGNAL(pressed()),this,SLOT(slotSetOpenPath()));
	connect(qpbImportPath,SIGNAL(pressed()),this,SLOT(slotSetImportPath()));
	connect(qpbArchivesPath,SIGNAL(pressed()),this,SLOT(slotSetArchivesPath()));
	connect(qpbOutPath,SIGNAL(pressed()),this,SLOT(slotSetOutPath()));
}
//
void QMyPreferences::slotSetOpenPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",qleOpenPath->text());
	if (!qsDir.isEmpty())
		qleOpenPath->setText(QDir::toNativeSeparators(qsDir));
}
//
void QMyPreferences::slotSetImportPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",qleImportPath->text());
	if (!qsDir.isEmpty())
		qleImportPath->setText(QDir::toNativeSeparators(qsDir));
}
//
void QMyPreferences::slotSetArchivesPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",qleArchivesPath->text());
	if (!qsDir.isEmpty())
		qleArchivesPath->setText(QDir::toNativeSeparators(qsDir));
}
//
void QMyPreferences::slotSetOutPath()
{
	QString qsDir=QFileDialog::getExistingDirectory(this,"",qleOutPath->text());
	if (!qsDir.isEmpty())
		qleOutPath->setText(QDir::toNativeSeparators(qsDir));
}
//
