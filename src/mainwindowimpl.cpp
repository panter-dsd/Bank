#include <QtGui>
#include <QtSql>
#include "mainwindowimpl.h"
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f)
	: QMainWindow(parent, f)
{
	qsetAppSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,"PanteR","Bank");
	qsdBase=QSqlDatabase::addDatabase("QODBC");
	createControls();
	setLayouts();
	createActions();
	createMenus();
	createContextMenu();
	createToolBars();
	loadSettings();
	loadHeader();
	setConnects();
	setCurrentFile("");
	slotUpdateActions();
	checkReestr();
}
//
MainWindowImpl::~MainWindowImpl()
{
	saveSettings();
}
//
void MainWindowImpl::createControls()
{
	qwCentralWidget=new QWidget(this);
	setCentralWidget(qwCentralWidget);
//Header
	qgbHeaderGroup=new QGroupBox(this);
	qlOrganizationName=new QLabel(tr("Organization name, OGRN"),this);
	qleOrganizationName=new QLineEdit(this);
	qleOrganizationName->setMaxLength(30);
	qlReestrNumber=new QLabel(tr("Number reestr"),this);
	qsbReestrNumber=new QSpinBox(this);
	qsbReestrNumber->setRange(1,999);
	qsbReestrNumber->setAlignment(Qt::AlignRight);
	qlFilialName=new QLabel(tr("Filial name, number"),this);
	qleFilialName=new QLineEdit(this);
	qleFilialName->setMaxLength(30);
	qlSchetOrganization=new QLabel(tr("Organization schet"),this);
	qleSchetOrganization=new QLineEdit(this);
	qleSchetOrganization->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,20}"),this));
	qlDogovorNumber=new QLabel(tr("Number dogovor"),this);
	qleDogovorNumber=new QLineEdit(this);
	qleDogovorNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,8}"),this));
	qlDogovorDate=new QLabel(tr("Date dogovor"),this);
	qdeDogovorDate=new QDateEdit(this);
	qdeDogovorDate->setCalendarPopup(true);
	qlPlatNumber=new QLabel(tr("Number plat"),this);
	qlePlatNumber=new QLineEdit(this);
	qlePlatNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,6}"),this));
	qlPlatDate=new QLabel(tr("Date plat"),this);
	qdePlatDate=new QDateEdit(this);
	qdePlatDate->setCalendarPopup(true);
	qlVid=new QLabel(tr("Vid oplata"),this);
	qcbVid=new QComboBox(this);
	qcbVid->addItem(tr("Заработная плата"));
	qcbVid->addItem(tr("Стипендия учащимся"));
	qcbVid->addItem(tr("Пенсия социальная"));
	qcbVid->addItem(tr("Пенсия негосударственных пенсионных фондов (кроме НПФ Сбербанка)"));
	qcbVid->addItem(tr("Пособия и другие выплаты по безработице"));
	qcbVid->addItem(tr("Пособия на детей"));
	qcbVid->addItem(tr("Прочие выплаты"));

//End header
	qtwTable=new QTableWidget(this);
	qtwTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	qtwTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	qtwTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	qtwTable->setColumnCount(5);
	QStringList Headers;
	Headers << tr("Famil") << tr("Name") << tr("Otch") << tr("Schet") << tr("Money");
	qtwTable->setHorizontalHeaderLabels(Headers);
	qsbStatusBar=new QStatusBar(this);
	setStatusBar(qsbStatusBar);
	qlSumm=new QLabel(this);
	qsbStatusBar->addWidget(qlSumm);
	qmbMainMenu=new QMenuBar(this);
	setMenuBar(qmbMainMenu);
}
//
void MainWindowImpl::createActions()
{
	qaNew_cmd=new QAction(tr("New"),this);
	qaNew_cmd->setShortcut(QKeySequence::New);
	qaNew_cmd->setStatusTip(tr("Create new reestr"));
	qaNew_cmd->setIcon(QIcon(":/share/images/qaNew_cmd.png"));
	addAction(qaNew_cmd);
	qaImport_cmd=new QAction(tr("Import"),this);
	qaImport_cmd->setStatusTip(tr("Import old file format"));
	addAction(qaImport_cmd);
	qaOpen_cmd=new QAction(tr("Open"),this);
	qaOpen_cmd->setShortcut(QKeySequence::Open);
	qaOpen_cmd->setStatusTip(tr("Open reestr"));
	qaOpen_cmd->setIcon(QIcon(":/share/images/qaOpen_cmd.png"));
	addAction(qaOpen_cmd);
	qaSave_cmd=new QAction(tr("Save"),this);
	qaSave_cmd->setShortcut(QKeySequence::Save);
	qaSave_cmd->setStatusTip(tr("Save reestr"));
	qaSave_cmd->setIcon(QIcon(":/share/images/qaSave_cmd.png"));
	addAction(qaSave_cmd);
	qaSaveAs_cmd=new QAction(tr("Save as..."),this);
	qaSaveAs_cmd->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_S);
	qaSaveAs_cmd->setIcon(QIcon(":/share/images/qaSaveAs_cmd.png"));
	addAction(qaSaveAs_cmd);
	qaAdd_cmd=new QAction(tr("Add"),this);
	qaAdd_cmd->setShortcut(Qt::Key_Insert);
	qaAdd_cmd->setIcon(QIcon(":/share/images/qaAdd_cmd.png"));
	addAction(qaAdd_cmd);
	qaEdit_cmd=new QAction(tr("Edit"),this);
	qaEdit_cmd->setShortcut(Qt::Key_F4);
	qaEdit_cmd->setIcon(QIcon(":/share/images/qaEdit_cmd.png"));
	addAction(qaEdit_cmd);
	qaChangeMoney_cmd=new QAction(tr("Change money"),this);
	addAction(qaChangeMoney_cmd);
	qaDelete_cmd=new QAction(tr("Delete"),this);
	qaDelete_cmd->setShortcut(Qt::Key_Delete);
	qaDelete_cmd->setIcon(QIcon(":/share/images/qaDelete_cmd.png"));
	addAction(qaDelete_cmd);
	qaNullMoney_cmd=new QAction(tr("Null money"),this);
	addAction(qaNullMoney_cmd);
	qaSaveHeader_cmd=new QAction(tr("Save header"),this);
	qaSaveHeader_cmd->setStatusTip(tr("Save header as default"));
	addAction(qaSaveHeader_cmd);
	qaAbout_cmd=new QAction(tr("About..."),this);
	qaAbout_cmd->setStatusTip(tr("About this application"));
	qaAbout_cmd->setIcon(QIcon(":/share/images/qaAbout_cmd.png"));
	addAction(qaAbout_cmd);
	qaAboutQt_cmd=new QAction(tr("About Qt"),this);
	qaAboutQt_cmd->setStatusTip(tr("About Qt"));
	qaAboutQt_cmd->setIcon(QIcon(":/share/images/qaAboutQt_cmd.png"));
	addAction(qaAboutQt_cmd);
	qaExit_cmd=new QAction(tr("Exit"),this);
	qaExit_cmd->setShortcut(Qt::ALT + Qt::Key_X);
	qaExit_cmd->setStatusTip(tr("Exit from this application"));
	qaExit_cmd->setIcon(QIcon(":/share/images/qaExit_cmd.png"));
	addAction(qaExit_cmd);
	qaOut_cmd=new QAction(tr("Out"),this);
	qaOut_cmd->setStatusTip(tr("Out reestr on disk"));
	addAction(qaOut_cmd);
	qaPreferences_cmd=new QAction(tr("Preferences"),this);
	qaPreferences_cmd->setStatusTip(tr("Application preferences"));
	qaPreferences_cmd->setIcon(QIcon(":/share/images/qaPreferences_cmd.png"));
	addAction(qaPreferences_cmd);
}
//
void MainWindowImpl::createMenus()
{
	QMenu* qmFile=qmbMainMenu->addMenu(tr("File"));
	qmFile->addAction(qaNew_cmd);
	qmFile->addAction(qaOpen_cmd);
	qmFile->addAction(qaSave_cmd);
	qmFile->addAction(qaSaveAs_cmd);
	qmFile->addAction(qaOut_cmd);
	qmFile->addAction(qaImport_cmd);
	qmFile->addSeparator();
	qmFile->addAction(qaPreferences_cmd);
	qmFile->addSeparator();
	qmFile->addAction(qaExit_cmd);

	QMenu* qmReestr=qmbMainMenu->addMenu(tr("Reestr"));
	qmReestr->addAction(qaAdd_cmd);
	qmReestr->addAction(qaEdit_cmd);
	qmReestr->addAction(qaDelete_cmd);
	qmReestr->addAction(qaChangeMoney_cmd);
	qmReestr->addAction(qaNullMoney_cmd);
	qmReestr->addAction(qaSaveHeader_cmd);

	QMenu* qmHelp=qmbMainMenu->addMenu(tr("Help"));
	qmHelp->addAction(qaAbout_cmd);
	qmHelp->addAction(qaAboutQt_cmd);
}
//
void MainWindowImpl::setLayouts()
{
	QGridLayout* qglHeaderLayout=new QGridLayout();
	qglHeaderLayout->addWidget(qlOrganizationName,0,0);
	qglHeaderLayout->addWidget(qleOrganizationName,1,0);
	qglHeaderLayout->addWidget(qlReestrNumber,0,1);
	qglHeaderLayout->addWidget(qsbReestrNumber,1,1);
	qglHeaderLayout->addWidget(qlFilialName,0,2);
	qglHeaderLayout->addWidget(qleFilialName,1,2);
	qglHeaderLayout->addWidget(qlSchetOrganization,0,3);
	qglHeaderLayout->addWidget(qleSchetOrganization,1,3);
	qglHeaderLayout->addWidget(qlDogovorNumber,3,0);
	qglHeaderLayout->addWidget(qleDogovorNumber,4,0);
	qglHeaderLayout->addWidget(qlDogovorDate,3,1);
	qglHeaderLayout->addWidget(qdeDogovorDate,4,1);
	qglHeaderLayout->addWidget(qlPlatNumber,3,2);
	qglHeaderLayout->addWidget(qlePlatNumber,4,2);
	qglHeaderLayout->addWidget(qlPlatDate,3,3);
	qglHeaderLayout->addWidget(qdePlatDate,4,3);
	qglHeaderLayout->addWidget(qlVid,5,0);
	qglHeaderLayout->addWidget(qcbVid,5,1,1,3);

	qgbHeaderGroup->setLayout(qglHeaderLayout);

	QVBoxLayout* qvbMainLayout=new QVBoxLayout();
	qvbMainLayout->addWidget(qgbHeaderGroup);
	qvbMainLayout->addWidget(qtwTable);
	qwCentralWidget->setLayout(qvbMainLayout);
}
//
void MainWindowImpl::setConnects()
{
	connect(qaNew_cmd,SIGNAL(triggered()),this,SLOT(slotNewReestr()));
	connect(qaImport_cmd,SIGNAL(triggered()),this,SLOT(slotImport()));
	connect(qaOpen_cmd,SIGNAL(triggered()),this,SLOT(slotOpenReestr()));
	connect(qaSave_cmd,SIGNAL(triggered()),this,SLOT(slotSaveReestr()));
	connect(qaSaveAs_cmd,SIGNAL(triggered()),this,SLOT(slotSaveAsReestr()));
	connect(qaAdd_cmd,SIGNAL(triggered()),this,SLOT(slotAdd()));
	connect(qaEdit_cmd,SIGNAL(triggered()),this,SLOT(slotEdit()));
	connect(qaDelete_cmd,SIGNAL(triggered()),this,SLOT(slotDelete()));
	connect(qaChangeMoney_cmd,SIGNAL(triggered()),this,SLOT(slotChangeMoney()));
	connect(qaNullMoney_cmd,SIGNAL(triggered()),this,SLOT(slotNullMoney()));
	connect(qaSaveHeader_cmd,SIGNAL(triggered()),this,SLOT(slotSaveHeader()));
	connect(qaAbout_cmd,SIGNAL(triggered()),this,SLOT(slotAbout()));
	connect(qaAboutQt_cmd,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(qaExit_cmd,SIGNAL(triggered()),this,SLOT(close()));
	connect(qaOut_cmd,SIGNAL(triggered()),this,SLOT(slotOut()));
	connect(qaPreferences_cmd,SIGNAL(triggered()),this,SLOT(slotPreferences()));

	connect(qleOrganizationName,SIGNAL(editingFinished ()),this,SLOT(slotCheckReestr()));
	connect(qleFilialName,SIGNAL(editingFinished ()),this,SLOT(slotCheckReestr()));
	connect(qleSchetOrganization,SIGNAL(editingFinished ()),this,SLOT(slotCheckReestr()));
	connect(qleDogovorNumber,SIGNAL(editingFinished ()),this,SLOT(slotCheckReestr()));
	connect(qlePlatNumber,SIGNAL(editingFinished()),this,SLOT(slotCheckReestr()));
	connect(qtwTable,SIGNAL(itemActivated(QTableWidgetItem*)),this,SLOT(slotItemActivated(QTableWidgetItem*)));
}
//
void MainWindowImpl::loadSettings()
{
	move(qsetAppSettings->value("MainWindow/pos",QPoint(0,0)).toPoint());
	resize(qsetAppSettings->value("MainWindow/size",QSize(640,480)).toSize());
	bool bIsMaximized=qsetAppSettings->value("MainWindow/IsMaximized",false).toBool();
	if (bIsMaximized) setWindowState(Qt::WindowMaximized);
	for (int i=0; i<qtwTable->columnCount(); i++)
	{
		qtwTable->setColumnWidth(i,qsetAppSettings->value(QString("MainTableColumns/Column_%1").arg(i),50).toInt());
	}
	qsbReestrNumber->setValue(qsetAppSettings->value("Global/ReestrNumber",1).toInt());
}
//
void MainWindowImpl::saveSettings()
{
	if (windowState()!=Qt::WindowMaximized)
	{
		qsetAppSettings->setValue("MainWindow/pos",pos());
		qsetAppSettings->setValue("MainWindow/size",size());
		qsetAppSettings->setValue("MainWindow/IsMaximized",false);
	}
	else
		qsetAppSettings->setValue("MainWindow/IsMaximized",true);

	for (int i=0; i<qtwTable->columnCount(); i++)
	{
		qsetAppSettings->setValue(QString("MainTableColumns/Column_%1").arg(i),qtwTable->columnWidth(i));
	}
	qsetAppSettings->setValue("Global/ReestrNumber",qsbReestrNumber->value());
	qsetAppSettings->sync();
}
//
void MainWindowImpl::import(const QString& qsFileName)
{
	QFile file;
	file.setFileName(qsFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Error"),tr("Error open file"));
		return;
	}
	qtwTable->clearContents();
	QStringList ImportList;
	QTextStream FileStream(&file);
	FileStream.setCodec("IBM 866");
	QProgressDialog progress(tr("Loading wait..."), tr("No"),0, 0, this);
	progress.setValue(0);
	progress.setWindowModality(Qt::WindowModal);
	while (!FileStream.atEnd())
	{
		QCoreApplication::processEvents();
		ImportList.append(FileStream.readLine());
	}
	file.close();

	progress.setMaximum(ImportList.count());
	qtwTable->setRowCount(ImportList.count());
	for (int i=0; i<ImportList.count(); i++)
	{
		if ((i/10)*10==i)
		{
			progress.setValue(i);
			QCoreApplication::processEvents();
		}
		QStringList tmpList=ImportList[i].split("|");
		if (tmpList.count()!=10)
		{
			progress.setValue(progress.maximum());
			int rez=QMessageBox::critical(this,
									tr("Error"),
									QString(tr("String %1 is not valid\nBreak?")).arg(i+1),
									QMessageBox::Yes | QMessageBox::No);
			if (rez==QMessageBox::No)
				qtwTable->setRowCount(qtwTable->rowCount()-1);
			else
			{
				qtwTable->clearContents();
				qtwTable->setRowCount(0);
				return;
			}
		}
		else
		{
			QTableWidgetItem *item;
			item=new QTableWidgetItem(tmpList.at(2));
			qtwTable->setItem(i,0,item);
			item=new QTableWidgetItem(tmpList.at(3));
			qtwTable->setItem(i,1,item);
			item=new QTableWidgetItem(tmpList.at(4));
			qtwTable->setItem(i,2,item);
			item=new QTableWidgetItem(tmpList.at(5));
			qtwTable->setItem(i,3,item);
			item=new QTableWidgetItem(tmpList.at(8));
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			qtwTable->setItem(i,4,item);
		}
		tmpList.clear();
	}
	qtwTable->selectRow(0);
	setCurrentFile("");
	loadHeader();
	slotUpdateActions();
	checkReestr();
}
//
void MainWindowImpl::slotOpenReestr()
{
	if (okToContinue())
	{
		QString qsFileName;
		qsFileName = QFileDialog::getOpenFileName(this,
										tr("Open file"),
										qsetAppSettings->value("PATH/OpenPath",QString()).toString(),
										tr("DBase files (*.dbf)"));
		if (!qsFileName.isEmpty())
		{
			qsetAppSettings->setValue("PATH/OpenPath",QFileInfo(qsFileName).absolutePath());
			loadFile(qsFileName);
		}
	}
}
//
void MainWindowImpl::slotImport()
{
	if (okToContinue())
	{
		QString qsFileName;
		qsFileName=QFileDialog::getOpenFileName(this,
											tr("Open file"),
											qsetAppSettings->value("PATH/ImportPath",QString()).toString(),
											tr("Old format files (*.s??)\nAll files (*.*)"));
		if (!qsFileName.isEmpty())
		{
			qsetAppSettings->setValue("PATH/ImportPath",QFileInfo(qsFileName).absolutePath());
			import(qsFileName);
		}
	}
}
//
bool MainWindowImpl::loadFile(const QString& qsFileName)
{
	qsdBase.close();
	QString qsConnectionString="DRIVER={Microsoft dBase Driver (*.dbf)};FIL={dBase III;};DefaultDir="+QFileInfo(qsFileName).absolutePath();
	qsdBase.setDatabaseName(qsConnectionString);
	if (!qsdBase.open())
	{
		QMessageBox::critical(0, tr("Error connect"), qsdBase.lastError().text());
		return false;
	}
	QSqlQuery* qsqQuery=new QSqlQuery();
	qsqQuery->prepare("SELECT COUNT(A) FROM "+QFileInfo(qsFileName).baseName());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error load table"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->first();
	QProgressDialog* qpdProgress=new QProgressDialog(tr("Loading wait..."), tr("No"),0,qsqQuery->value(0).toInt() , this);
	qpdProgress->setWindowModality(Qt::WindowModal);
	qsqQuery->clear();

	qsqQuery->prepare("SELECT * FROM "+QFileInfo(qsFileName).baseName());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error load table"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->first();
	qtwTable->clearContents();
	qtwTable->setRowCount(0);
	QTableWidgetItem *item;
	int iRow=0;
//Read header
	qleFilialName->setText(qsqQuery->value(0).toString().replace(tr("#"),tr("№")));
	qsqQuery->next();
	qpdProgress->setValue(qpdProgress->value()+1);
	qlePlatNumber->setText(qsqQuery->value(1).toString());
	qdePlatDate->setDate(QDate().fromString(qsqQuery->value(3).toString(),"dd.MM.yyyy"));
	qsqQuery->next();
	qpdProgress->setValue(qpdProgress->value()+1);
	qcbVid->setCurrentIndex(qsqQuery->value(1).toInt()-1);
	qsqQuery->next();
	qpdProgress->setValue(qpdProgress->value()+1);
	qleOrganizationName->setText(qsqQuery->value(1).toString().replace(tr("#"),tr("№")));
	qleSchetOrganization->setText(qsqQuery->value(2).toString());
	qsqQuery->next();
	qpdProgress->setValue(qpdProgress->value()+1);
	qleDogovorNumber->setText(qsqQuery->value(1).toString());
	qdeDogovorDate->setDate(QDate().fromString(qsqQuery->value(3).toString(),"dd.MM.yyyy"));
	qsqQuery->next();
	qpdProgress->setValue(qpdProgress->value()+1);
//End read header
	qtwTable->setUpdatesEnabled(false);
	qtwTable->setRowCount(qpdProgress->maximum()-5);
	while (qsqQuery->next())
	{
		item=new QTableWidgetItem(qsqQuery->value(2).toString());
		qtwTable->setItem(iRow,0,item);
		item=new QTableWidgetItem(qsqQuery->value(3).toString());
		qtwTable->setItem(iRow,1,item);
		item=new QTableWidgetItem(qsqQuery->value(4).toString());
		qtwTable->setItem(iRow,2,item);
		item=new QTableWidgetItem(qsqQuery->value(1).toString());
		qtwTable->setItem(iRow,3,item);
		item=new QTableWidgetItem(qsqQuery->value(5).toString());
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		qtwTable->setItem(iRow,4,item);
		iRow++;
		qpdProgress->setValue(qpdProgress->value()+1);
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
	qpdProgress->setValue(qpdProgress->maximum());
	QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	delete qpdProgress;
	qsqQuery->clear();
	qsdBase.close();

	qtwTable->removeRow(qtwTable->rowCount()-1);
	qtwTable->selectRow(0);
	setCurrentFile(qsFileName);
	slotUpdateActions();
	checkReestr();
	qtwTable->setUpdatesEnabled(true);
	return true;
}
//
bool MainWindowImpl::slotSaveReestr()
{
	if (qsCurrentFile.isEmpty())
	{
		return slotSaveAsReestr();
	}
	if (checkReestr()==1)
	{
		QMessageBox::critical(this,tr("Error saving"),tr("Find errors in reestr"));
		return false;
	}
	if (checkReestr()==2)
	{
		int res=QMessageBox::warning(this,
								tr("Warning saving"),
								tr("Find warnings in reestr.\nSave?"),
								QMessageBox::Ok,
								QMessageBox::Cancel);
		if (res==QMessageBox::Cancel)
			return false;
	}
	return saveFile(qsCurrentFile);

}
//
bool MainWindowImpl::slotSaveAsReestr()
{
	if (checkReestr()==1)
	{
		QMessageBox::critical(this,tr("Error saving"),tr("Find errors in reestr"));
		return false;
	}
	if (checkReestr()==2)
	{
		int res=QMessageBox::warning(this,
								tr("Warning saving"),
								tr("Find warnings in reestr.\nSave?"),
								QMessageBox::Ok,
								QMessageBox::Cancel);
		if (res==QMessageBox::Cancel)
			return false;
	}
	QString qsFileName;
	qsFileName = QFileDialog::getSaveFileName(this,
							tr("Open file"),
							qsetAppSettings->value("PATH/OpenPath",QString()).toString(),
							tr("DBase files (*.dbf)"));
	if (qsFileName.isEmpty())
		return false;
	qsetAppSettings->setValue("PATH/OpenPath",QFileInfo(qsFileName).absolutePath());
	return saveFile(qsFileName);
}
//
bool MainWindowImpl::saveFile(const QString& qsFileName)
{
	if (QFileInfo(qsFileName).exists())
		QFile::remove(qsFileName);
	qsdBase.close();
	QString qsConnectionString="DRIVER={Microsoft dBase Driver (*.dbf)};FIL={dBase III;};DefaultDir="+QFileInfo(qsFileName).absolutePath();
	qsdBase.setDatabaseName(qsConnectionString);
	if (!qsdBase.open())
	{
		QMessageBox::critical(this, tr("Error connect"), qsdBase.lastError().text());
		return false;
	}
	QString qsTableName=QFileInfo(qsFileName).baseName();
	QSqlQuery* qsqQuery=new QSqlQuery();
	qsqQuery->prepare("CREATE TABLE "+qsTableName+" (A CHAR(30),B CHAR(30),C CHAR(30),D CHAR(30),E CHAR(30),F CHAR(30),G CHAR(30));");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save file"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A) VALUES (:a);");
	qsqQuery->bindValue(":a",qleFilialName->text().replace(tr("№"),tr("#")));
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save file"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B,C,D) VALUES (:a,:b,:c,:d);");
	qsqQuery->bindValue(":a",tr("К платежному поручению #"));
	qsqQuery->bindValue(":b",qlePlatNumber->text());
	qsqQuery->bindValue(":c",tr("от"));
	if (!qlePlatNumber->text().isEmpty())
		qsqQuery->bindValue(":d",qdePlatDate->date().toString("dd.MM.yyyy"));
	else
		qsqQuery->bindValue(":d","");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B) VALUES (:a,:b);");
	qsqQuery->bindValue(":a",tr("Зачисление"));
	qsqQuery->bindValue(":b",tr("0")+QString().setNum(qcbVid->currentIndex()+1));
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B,C) VALUES (:a,:b,:c);");
	qsqQuery->bindValue(":a",tr("Наименование,ОГРН,# банк.счета"));
	qsqQuery->bindValue(":b",qleOrganizationName->text().replace(tr("№"),tr("#")));
	qsqQuery->bindValue(":c",qleSchetOrganization->text());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B,C,D) VALUES (:a,:b,:c,:d);");
	qsqQuery->bindValue(":a",tr("По договору:"));
	qsqQuery->bindValue(":b",qleDogovorNumber->text());
	qsqQuery->bindValue(":c",tr("от"));
	qsqQuery->bindValue(":d",qdeDogovorDate->date().toString("dd.MM.yyyy"));
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B,C,D,E,F,G) VALUES (:a,:b,:c,:d,:e,:f,:g);");
	qsqQuery->bindValue(":a",tr("# п/п"));
	qsqQuery->bindValue(":b",tr("Номер счета"));
	qsqQuery->bindValue(":c",tr("Фамилия"));
	qsqQuery->bindValue(":d",tr("Имя"));
	qsqQuery->bindValue(":e",tr("Отчество"));
	qsqQuery->bindValue(":f",tr("Сумма"));
	qsqQuery->bindValue(":g",tr("Примечание"));
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
//
	int iNullCount=0;
	for (int i=0; i<qtwTable->rowCount(); i++)
	{
		if (qtwTable->item(i,4)->text().toDouble()==0.00)
		{
			iNullCount++;
			continue;
		}
		qsqQuery->prepare("INSERT INTO "+qsTableName+"(A,B,C,D,E,F) VALUES (:a,:b,:c,:d,:e,:f);");
		qsqQuery->bindValue(":a",QString().setNum(i+1-iNullCount));
		qsqQuery->bindValue(":b",qtwTable->item(i,3)->text());
		qsqQuery->bindValue(":c",qtwTable->item(i,0)->text());
		qsqQuery->bindValue(":d",qtwTable->item(i,1)->text());
		qsqQuery->bindValue(":e",qtwTable->item(i,2)->text());
		qsqQuery->bindValue(":f",qtwTable->item(i,4)->text());
		if (!qsqQuery->exec())
		{
			QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
			delete qsqQuery;
			return false;
		}
		qsqQuery->clear();
	}
//
	qsqQuery->prepare("INSERT INTO "+qsTableName+"(B,F) VALUES (:b,:f);");
	qsqQuery->bindValue(":b",tr("ИТОГО:"));
	qsqQuery->bindValue(":f",QString().setNum(getSumm(),'f',2));
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(this, tr("Error save base"), qsqQuery->lastError().text());
		delete qsqQuery;
		return false;
	}
	qsqQuery->clear();
	delete qsqQuery;
	qsdBase.close();
	setCurrentFile(qsFileName);
	return true;
}
//
double MainWindowImpl::getSumm()
{
	double summ=0;
	for (int i=0; i<qtwTable->rowCount(); i++)
	{
		summ+=qtwTable->item(i,4)->text().toDouble();
	}
	return summ;
}
//
void MainWindowImpl::slotUpdateActions()
{
	qlSumm->setText(tr("Count = %1 Summa = %2").arg(qtwTable->rowCount()).arg(getSumm(),0,'f',2));
	bool bIsEmptyTable=((qtwTable->rowCount()>0) && (qtwTable->currentRow()>-1));
	qaEdit_cmd->setEnabled(bIsEmptyTable);
	qaChangeMoney_cmd->setEnabled(bIsEmptyTable);
	qaDelete_cmd->setEnabled(bIsEmptyTable);
	qaNullMoney_cmd->setEnabled(qtwTable->rowCount()>0);
}
//
void MainWindowImpl::slotAdd()
{
	QMyAddDialog* qmadDialog=new QMyAddDialog(this);
	qmadDialog->setWindowTitle(tr("Adding"));
	qmadDialog->setModal(true);
	qmadDialog->startCheck();
	if (qmadDialog->exec())
	{
		qtwTable->insertRow(qtwTable->rowCount());
		int iRow=qtwTable->rowCount()-1;
		QTableWidgetItem* item;
		item=new QTableWidgetItem(qmadDialog->qleFamil->text());
		qtwTable->setItem(iRow,0,item);
		item=new QTableWidgetItem(qmadDialog->qleName->text());
		qtwTable->setItem(iRow,1,item);
		item=new QTableWidgetItem(qmadDialog->qleOtch->text());
		qtwTable->setItem(iRow,2,item);
		item=new QTableWidgetItem(qmadDialog->qleSchet->text());
		qtwTable->setItem(iRow,3,item);
		item=new QTableWidgetItem(QString().setNum(qmadDialog->qleMoney->text().toDouble(),'f',2).replace(',','.'));
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		qtwTable->setItem(iRow,4,item);
		qtwTable->selectRow(iRow);
		setWindowModified(true);
	}
	delete qmadDialog;
	slotUpdateActions();
	checkReestr();
}
//
void MainWindowImpl::slotEdit()
{
	int iRow=qtwTable->currentRow();
	if (iRow<0) return;
	QMyAddDialog* qmadDialog=new QMyAddDialog(this);
	qmadDialog->setWindowTitle(tr("Editing"));
	qmadDialog->qleFamil->setText(qtwTable->item(iRow,0)->text());
	qmadDialog->qleName->setText(qtwTable->item(iRow,1)->text());
	qmadDialog->qleOtch->setText(qtwTable->item(iRow,2)->text());
	qmadDialog->qleSchet->setText(qtwTable->item(iRow,3)->text());
	qmadDialog->qleMoney->setText(qtwTable->item(iRow,4)->text());
	qmadDialog->startCheck();
	if (qmadDialog->exec())
	{
		qtwTable->item(iRow,0)->setText(qmadDialog->qleFamil->text());
		qtwTable->item(iRow,1)->setText(qmadDialog->qleName->text());
		qtwTable->item(iRow,2)->setText(qmadDialog->qleOtch->text());
		qtwTable->item(iRow,3)->setText(qmadDialog->qleSchet->text());
		qtwTable->item(iRow,4)->setText(QString().setNum(qmadDialog->qleMoney->text().toDouble(),'f',2).replace(',','.'));
		setWindowModified(true);
	}
	delete qmadDialog;
	slotUpdateActions();
	checkReestr();
}
//
void MainWindowImpl::slotDelete()
{
	int iRow=qtwTable->currentRow();
	if (iRow<0) return;
	qtwTable->removeRow(iRow);
	if (qtwTable->currentIndex().isValid())
		qtwTable->selectRow(qtwTable->currentIndex().row());
	setWindowModified(true);
	slotUpdateActions();
	checkReestr();
}
//
void MainWindowImpl::slotChangeMoney()
{
	int iRow=qtwTable->currentRow();
	if (iRow<0) return;
	QMyAddDialog* qmadDialog=new QMyAddDialog(this);
	qmadDialog->setWindowTitle(tr("Changing money"));
	qmadDialog->qleFamil->setVisible(false);
	qmadDialog->qleName->setVisible(false);
	qmadDialog->qleOtch->setVisible(false);
	qmadDialog->qleSchet->setVisible(false);
	qmadDialog->qlFamil->setVisible(false);
	qmadDialog->qlName->setVisible(false);
	qmadDialog->qlOtch->setVisible(false);
	qmadDialog->qlSchet->setVisible(false);
	qmadDialog->qleMoney->setText(qtwTable->item(iRow,4)->text());
	qmadDialog->qleMoney->selectAll();
	if (qmadDialog->exec())
	{
		qtwTable->item(iRow,4)->setText(QString().setNum(qmadDialog->qleMoney->text().toDouble(),'f',2).replace(',','.'));
		setWindowModified(true);
	}
	delete qmadDialog;
	slotUpdateActions();
	checkReestr();
}
//
void MainWindowImpl::slotNullMoney()
{
	for (int i=0; i<qtwTable->rowCount(); i++)
	{
		qtwTable->item(i,4)->setText("0.00");
	}
	setWindowModified(true);
	slotUpdateActions();
	checkReestr();
}
//
int MainWindowImpl::checkReestr()
{
	int iValid=0;
	if (qleOrganizationName->text().isEmpty())
	{
		setErrorPalette(qleOrganizationName);
		iValid=1;
	}
	else
		qleOrganizationName->setPalette(QApplication::palette());
	if (qleFilialName->text().isEmpty())
	{
		setErrorPalette(qleFilialName);
		iValid=1;
	}
	else
		qleFilialName->setPalette(QApplication::palette());
	if ((qleSchetOrganization->text().isEmpty()) || (qleSchetOrganization->text().count()!=20))
	{
		setErrorPalette(qleSchetOrganization);
		iValid=1;
	}
	else
		qleSchetOrganization->setPalette(QApplication::palette());
	if ((qleDogovorNumber->text().isEmpty()) || (qleDogovorNumber->text().count()!=8))
	{
		setErrorPalette(qleDogovorNumber);
		iValid=1;
	}
	else
		qleDogovorNumber->setPalette(QApplication::palette());
	if ((qlePlatNumber->text().isEmpty()) || (qlePlatNumber->text().count()!=6))
	{
		setWarningPalette(qlePlatNumber);
		if (iValid==0) iValid=2;
	}
	else
		qlePlatNumber->setPalette(QApplication::palette());

	if (qtwTable->rowCount()<=0)
		iValid=1;
	for (int i=0; i<qtwTable->rowCount(); i++)
	{
		if (qtwTable->item(i,0)->text().isEmpty())//Famil
		{
			qtwTable->item(i,0)->setBackgroundColor(Qt::red);
			iValid=1;
		}
		else
		{
			qtwTable->item(i,0)->setBackgroundColor(Qt::white);
		}
		if (qtwTable->item(i,1)->text().isEmpty())//Name
		{
			qtwTable->item(i,1)->setBackgroundColor(Qt::red);
			iValid=1;
		}
		else
			qtwTable->item(i,1)->setBackgroundColor(Qt::white);
		if (qtwTable->item(i,2)->text().isEmpty())//Otch
		{
			qtwTable->item(i,2)->setBackgroundColor(Qt::yellow);
			if (iValid==0) iValid=2;
		}
		else
			qtwTable->item(i,2)->setBackgroundColor(Qt::white);
		if (!checkSchet(qtwTable->item(i,3)->text()))//Schet
		{
			qtwTable->item(i,3)->setBackgroundColor(Qt::red);
			iValid=1;
		}
		else
			qtwTable->item(i,3)->setBackgroundColor(Qt::white);
		if ((qtwTable->item(i,4)->text().isEmpty()) || (qtwTable->item(i,4)->text().toDouble()<0))//Money
		{
			qtwTable->item(i,4)->setBackgroundColor(Qt::red);
			iValid=1;
		}
		else
			qtwTable->item(i,4)->setBackgroundColor(Qt::white);
	}
	return iValid;
}
//
bool MainWindowImpl::checkSchet(const QString& qsSchet)
{
	if (qsSchet.length()!=20) return false;
	if (qsSchet.mid(5,3)!="810") return false;
	if (qsSchet.mid(9,4)!=qleDogovorNumber->text().mid(0,4)) return false;
	return true;
}
//
void MainWindowImpl::slotSaveHeader()
{
	qsetAppSettings->setValue("Header/OrganizationName",qleOrganizationName->text());
	qsetAppSettings->setValue("Header/FilialName",qleFilialName->text());
	qsetAppSettings->setValue("Header/SchetOrganization",qleSchetOrganization->text());
	qsetAppSettings->setValue("Header/DogovorNumber",qleDogovorNumber->text());
	qsetAppSettings->setValue("Header/DogovorDate",qdeDogovorDate->date());
	qsetAppSettings->setValue("Header/Vid",qcbVid->currentIndex());
	qsetAppSettings->sync();
}
//
void MainWindowImpl::loadHeader()
{
	qleOrganizationName->setText(qsetAppSettings->value("Header/OrganizationName",QString()).toString());
	qleFilialName->setText(qsetAppSettings->value("Header/FilialName",QString()).toString());
	qleSchetOrganization->setText(qsetAppSettings->value("Header/SchetOrganization",QString()).toString());
	qleDogovorNumber->setText(qsetAppSettings->value("Header/DogovorNumber",QString()).toString());
	qdeDogovorDate->setDate(qsetAppSettings->value("Header/DogovorDate",QString()).toDate());
	qcbVid->setCurrentIndex(qsetAppSettings->value("Header/Vid",QString()).toInt());
}
//
void MainWindowImpl::closeEvent(QCloseEvent* event)
{
	if (okToContinue())
		event->accept();
	else
		event->ignore();
}
//
void MainWindowImpl::createContextMenu()
{
	qtwTable->addAction(qaAdd_cmd);
	qtwTable->addAction(qaEdit_cmd);
	qtwTable->addAction(qaDelete_cmd);
	qtwTable->addAction(qaChangeMoney_cmd);
	qtwTable->addAction(qaNullMoney_cmd);
	qtwTable->setContextMenuPolicy(Qt::ActionsContextMenu);
}
//
void MainWindowImpl::createToolBars()
{
	qtbFileToolbar=new QToolBar(tr("File"));
	qtbFileToolbar->addAction(qaNew_cmd);
	qtbFileToolbar->addAction(qaOpen_cmd);
	qtbFileToolbar->addAction(qaSave_cmd);
	qtbFileToolbar->addAction(qaSaveAs_cmd);
	qtbFileToolbar->addAction(qaOut_cmd);
	qtbFileToolbar->addAction(qaImport_cmd);
	qtbFileToolbar->addAction(qaPreferences_cmd);
	addToolBar(qtbFileToolbar);

	qtbReestrToolbar=new QToolBar(tr("Reestr"));
	qtbReestrToolbar->addAction(qaAdd_cmd);
	qtbReestrToolbar->addAction(qaEdit_cmd);
	qtbReestrToolbar->addAction(qaDelete_cmd);
	qtbReestrToolbar->addAction(qaChangeMoney_cmd);
	qtbReestrToolbar->addAction(qaNullMoney_cmd);
	addToolBar(qtbReestrToolbar);
}
//
bool MainWindowImpl::okToContinue()
{
	if (isWindowModified())
	{
		int res=QMessageBox::warning(this,
								QApplication::applicationName(),
								tr("The reestr has been modified.\nDo you want to save your changes?"),
								QMessageBox::Yes | QMessageBox::Default,
								QMessageBox::No,
								QMessageBox::Cancel);
		if (res==QMessageBox::Yes)
			return slotSaveReestr();
		if (res==QMessageBox::Cancel)
			return false;
	}
	return true;
}
//
void MainWindowImpl::slotNewReestr()
{
	if (okToContinue())
	{
		qtwTable->clearContents();
		qtwTable->setRowCount(0);
		setCurrentFile("");
	}
}
//
void MainWindowImpl::setCurrentFile(const QString& qsFileName)
{
	qsCurrentFile=qsFileName;
	setWindowModified(false);
	QString qsShowName=tr("Untitled");
	if (!qsCurrentFile.isEmpty())
		qsShowName=QFileInfo(qsCurrentFile).fileName();
	setWindowTitle(tr("%1 %2[*]").arg(QApplication::applicationName()).arg(qsShowName));
	slotUpdateActions();
}
//
void MainWindowImpl::slotAbout()
{
	QMessageBox::about(this,tr("About Bank"),
						tr("<h2>") + QApplication::applicationName() +tr ("</h2>")+
						tr("<p>Author: PanteR"
							"<p>e-mail: panter.dsd@gmail.com"
							"<p>tel: 89094119675"
							"<p>License: GNU GPL v2"));
}
//
void MainWindowImpl::slotOut()
{
	if (checkReestr()==1)
	{
		QMessageBox::critical(this,tr("Error saving"),tr("Find errors in reestr"));
		return;
	}
	if (checkReestr()==2)
	{
		int res=QMessageBox::warning(this,
								tr("Warning saving"),
								tr("Find warnings in reestr.\nSave?"),
								QMessageBox::Ok,
								QMessageBox::Cancel);
		if (res==QMessageBox::Cancel)
			return;
	}
	QDir dir(qsetAppSettings->value("PATH/ArchivesPath",QString()).toString());
	if (!dir.exists())
	{
		dir.setCurrent(QCoreApplication::applicationDirPath());
		dir.cdUp();
		dir.cd("archives");
	}
	bool bIsWindowModified=isWindowModified();
	QString qsCurFile=qsCurrentFile;
	QFileInfo qfiOutFile(dir.absolutePath()+
						"/"+
						QDate::currentDate().toString("yyMMdd")+
						QTime::currentTime().toString("ss")+
						".dbf");
	if (saveFile(qfiOutFile.filePath()))
	{
		setCurrentFile(qsCurFile);
		setWindowModified(bIsWindowModified);
		QString qsOutName=qsetAppSettings->value("PATH/OutPath",QString()).toString()+
							QDir::separator()+
							"t1820"+
							QString("%1").arg(qsbReestrNumber->value(),3,10,QChar('0'))+
							".dbf";
		if (!QFile::copy(qfiOutFile.filePath(),qsOutName))
			QMessageBox::warning(this,
								tr("Error out"),
								tr("Error out file %1").arg(qsOutName));
		else
		{
			QMessageBox::information(this,
								tr("Out"),
								tr("Out is suxcessed"));
			qsbReestrNumber->setValue(qsbReestrNumber->value()+1);
		}
	}
}
//
void MainWindowImpl::slotPreferences()
{
	qmpPreferences=new QMyPreferences(this);
	qmpPreferences->setWindowTitle(tr("Preferences"));
	qmpPreferences->exec();
	delete qmpPreferences;
}
//

