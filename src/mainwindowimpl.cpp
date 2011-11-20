#include <QtCore/QSettings>
#include <QtCore/QTextStream>

#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QDateEdit>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QStatusBar>
#include <QtGui/QMenuBar>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "adddialog.h"
#include "preferences.h"

#include "mainwindowimpl.h"

MainWindowImpl::MainWindowImpl (QWidget *parent, Qt::WFlags f)
	: QMainWindow (parent, f)
{
	applicationSettings_ = new QSettings (QSettings::IniFormat, QSettings::UserScope, "PanteR", "Bank");
	QSqlDatabase::addDatabase ("QODBC");
	createControls();
	setLayouts();
	createActions();
	createMenus();
	createContextMenu();
	createToolBars();
	loadSettings();
	loadHeader();
	setConnects();
	setCurrentFile (QString::null);
	updateActions();
	checkReestr();
}

MainWindowImpl::~MainWindowImpl()
{
	saveSettings();
}

void MainWindowImpl::createControls()
{
	setCentralWidget (new QWidget (this));
//Header
	headerGroup_ = new QGroupBox (this);

	organizationNameLabel_ = new QLabel (tr ("Organization name, OGRN"), this);

	organizationNameEdit_ = new QLineEdit (this);
	organizationNameEdit_->setMaxLength (30);

	reestrNumberLabel_ = new QLabel (tr ("Number reestr"), this);

	reestrNumberEdit_ = new QSpinBox (this);
	reestrNumberEdit_->setRange (1, 999);
	reestrNumberEdit_->setAlignment (Qt::AlignRight);

	filialNameLabel_ = new QLabel (tr ("Filial name, number"), this);

	filialNameEdit_ = new QLineEdit (this);
	filialNameEdit_->setMaxLength (30);

	schetOrganizationLabel_ = new QLabel (tr ("Organization schet"), this);

	schetOrganizationEdit_ = new QLineEdit (this);
	schetOrganizationEdit_->setValidator (new QRegExpValidator (QRegExp ("[0-9]{1,20}"), this));

	dogovorNumberLabel_ = new QLabel (tr ("Number dogovor"), this);

	dogovorNumberEdit_ = new QLineEdit (this);
	dogovorNumberEdit_->setValidator (new QRegExpValidator (QRegExp ("[0-9]{1,8}"), this));

	dogovorDateLabel_ = new QLabel (tr ("Date dogovor"), this);

	dogovorDateEdit_ = new QDateEdit (this);
	dogovorDateEdit_->setCalendarPopup (true);

	platNumberLabel_ = new QLabel (tr ("Number plat"), this);

	platNumberEdit_ = new QLineEdit (this);
	platNumberEdit_->setValidator (new QRegExpValidator (QRegExp ("[0-9]{1,6}"), this));

	platDateLabel_ = new QLabel (tr ("Date plat"), this);

	platDateEdit_ = new QDateEdit (this);
	platDateEdit_->setCalendarPopup (true);

	vidLabel_ = new QLabel (tr ("Vid oplata"), this);

	vidEdit_ = new QComboBox (this);
	vidEdit_->addItem (tr ("Заработная плата"));
	vidEdit_->addItem (tr ("Стипендия учащимся"));
	vidEdit_->addItem (tr ("Пенсия социальная"));
	vidEdit_->addItem (tr ("Пенсия негосударственных пенсионных фондов (кроме НПФ Сбербанка)"));
	vidEdit_->addItem (tr ("Пособия и другие выплаты по безработице"));
	vidEdit_->addItem (tr ("Пособия на детей"));
	vidEdit_->addItem (tr ("Прочие выплаты"));

	table_ = new QTableWidget (this);
	table_->setSelectionBehavior (QAbstractItemView::SelectRows);
	table_->setEditTriggers (QAbstractItemView::NoEditTriggers);
	table_->verticalHeader()->setResizeMode (QHeaderView::ResizeToContents);
	table_->setColumnCount (5);

	QStringList headers;
	headers << tr ("Famil") << tr ("Name")
			<< tr ("Otch") << tr ("Schet") << tr ("Money");
	table_->setHorizontalHeaderLabels (headers);

	statusBar_ = new QStatusBar (this);
	setStatusBar (statusBar_);

	sumLabel_ = new QLabel (this);
	statusBar_->addWidget (sumLabel_);

	mainMenu_ = new QMenuBar (this);
	setMenuBar (mainMenu_);
}

void MainWindowImpl::createActions()
{
	actionNew_ = new QAction (tr ("New"), this);
	actionNew_->setShortcut (QKeySequence::New);
	actionNew_->setStatusTip (tr ("Create new reestr"));
	actionNew_->setIcon (QIcon (":/share/images/qaNew_cmd.png"));
	addAction (actionNew_);

	actionImport_ = new QAction (tr ("Import"), this);
	actionImport_->setStatusTip (tr ("Import old file format"));
	addAction (actionImport_);

	actionOpen_ = new QAction (tr ("Open"), this);
	actionOpen_->setShortcut (QKeySequence::Open);
	actionOpen_->setStatusTip (tr ("Open reestr"));
	actionOpen_->setIcon (QIcon (":/share/images/qaOpen_cmd.png"));
	addAction (actionOpen_);

	actionSave_ = new QAction (tr ("Save"), this);
	actionSave_->setShortcut (QKeySequence::Save);
	actionSave_->setStatusTip (tr ("Save reestr"));
	actionSave_->setIcon (QIcon (":/share/images/qaSave_cmd.png"));
	addAction (actionSave_);

	actionSaveAs_ = new QAction (tr ("Save as..."), this);
	actionSaveAs_->setShortcut (Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	actionSaveAs_->setIcon (QIcon (":/share/images/qaSaveAs_cmd.png"));
	addAction (actionSaveAs_);
	actionAdd_ = new QAction (tr ("Add"), this);
	actionAdd_->setShortcut (Qt::Key_Insert);
	actionAdd_->setIcon (QIcon (":/share/images/qaAdd_cmd.png"));
	addAction (actionAdd_);

	actionEdit_ = new QAction (tr ("Edit"), this);
	actionEdit_->setShortcut (Qt::Key_F4);
	actionEdit_->setIcon (QIcon (":/share/images/qaEdit_cmd.png"));
	addAction (actionEdit_);

	actionChangeMoney_ = new QAction (tr ("Change money"), this);
	addAction (actionChangeMoney_);

	actionDelete_ = new QAction (tr ("Delete"), this);
	actionDelete_->setShortcut (Qt::Key_Delete);
	actionDelete_->setIcon (QIcon (":/share/images/qaDelete_cmd.png"));
	addAction (actionDelete_);

	actionNullMoney_ = new QAction (tr ("Null money"), this);
	addAction (actionNullMoney_);

	actionSaveHeader_ = new QAction (tr ("Save header"), this);
	actionSaveHeader_->setStatusTip (tr ("Save header as default"));
	addAction (actionSaveHeader_);

	actionAbout_ = new QAction (tr ("About..."), this);
	actionAbout_->setStatusTip (tr ("About this application"));
	actionAbout_->setIcon (QIcon (":/share/images/qaAbout_cmd.png"));
	addAction (actionAbout_);

	actionAboutQt_ = new QAction (tr ("About Qt"), this);
	actionAboutQt_->setStatusTip (tr ("About Qt"));
	actionAboutQt_->setIcon (QIcon (":/share/images/qaAboutQt_cmd.png"));
	addAction (actionAboutQt_);

	actionExit_ = new QAction (tr ("Exit"), this);
	actionExit_->setShortcut (Qt::ALT + Qt::Key_X);
	actionExit_->setStatusTip (tr ("Exit from this application"));
	actionExit_->setIcon (QIcon (":/share/images/qaExit_cmd.png"));
	addAction (actionExit_);

	actionOut_ = new QAction (tr ("Out"), this);
	actionOut_->setStatusTip (tr ("Out reestr on disk"));
	addAction (actionOut_);

	actionPreferences_ = new QAction (tr ("Preferences"), this);
	actionPreferences_->setStatusTip (tr ("Application preferences"));
	actionPreferences_->setIcon (QIcon (":/share/images/qaPreferences_cmd.png"));
	addAction (actionPreferences_);
}

void MainWindowImpl::createMenus()
{
	QMenu *fileMenu = mainMenu_->addMenu (tr ("File"));
	fileMenu->addAction (actionNew_);
	fileMenu->addAction (actionOpen_);
	fileMenu->addAction (actionSave_);
	fileMenu->addAction (actionSaveAs_);
	fileMenu->addAction (actionOut_);
	fileMenu->addAction (actionImport_);
	fileMenu->addSeparator();
	fileMenu->addAction (actionPreferences_);
	fileMenu->addSeparator();
	fileMenu->addAction (actionExit_);

	QMenu *reestrMenu = mainMenu_->addMenu (tr ("Reestr"));
	reestrMenu->addAction (actionAdd_);
	reestrMenu->addAction (actionEdit_);
	reestrMenu->addAction (actionDelete_);
	reestrMenu->addAction (actionChangeMoney_);
	reestrMenu->addAction (actionNullMoney_);
	reestrMenu->addAction (actionSaveHeader_);

	QMenu *helpMenu = mainMenu_->addMenu (tr ("Help"));
	helpMenu->addAction (actionAbout_);
	helpMenu->addAction (actionAboutQt_);
}

void MainWindowImpl::setLayouts()
{
	QGridLayout *headerLayout = new QGridLayout();
	headerLayout->addWidget (organizationNameLabel_, 0, 0);
	headerLayout->addWidget (organizationNameEdit_, 1, 0);
	headerLayout->addWidget (reestrNumberLabel_, 0, 1);
	headerLayout->addWidget (reestrNumberEdit_, 1, 1);
	headerLayout->addWidget (filialNameLabel_, 0, 2);
	headerLayout->addWidget (filialNameEdit_, 1, 2);
	headerLayout->addWidget (schetOrganizationLabel_, 0, 3);
	headerLayout->addWidget (schetOrganizationEdit_, 1, 3);
	headerLayout->addWidget (dogovorNumberLabel_, 3, 0);
	headerLayout->addWidget (dogovorNumberEdit_, 4, 0);
	headerLayout->addWidget (dogovorDateLabel_, 3, 1);
	headerLayout->addWidget (dogovorDateEdit_, 4, 1);
	headerLayout->addWidget (platNumberLabel_, 3, 2);
	headerLayout->addWidget (platNumberEdit_, 4, 2);
	headerLayout->addWidget (platDateLabel_, 3, 3);
	headerLayout->addWidget (platDateEdit_, 4, 3);
	headerLayout->addWidget (vidLabel_, 5, 0);
	headerLayout->addWidget (vidEdit_, 5, 1, 1, 3);

	headerGroup_->setLayout (headerLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget (headerGroup_);
	mainLayout->addWidget (table_);
	centralWidget ()->setLayout (mainLayout);
}

void MainWindowImpl::setConnects()
{
	connect (actionNew_, SIGNAL (triggered()), SLOT (newReestr()));
	connect (actionImport_, SIGNAL (triggered()), SLOT (import()));
	connect (actionOpen_, SIGNAL (triggered()), SLOT (openReestr()));
	connect (actionSave_, SIGNAL (triggered()), SLOT (saveReestr()));
	connect (actionSaveAs_, SIGNAL (triggered()), SLOT (saveAsReestr()));
	connect (actionAdd_, SIGNAL (triggered()), SLOT (addRecord()));
	connect (actionEdit_, SIGNAL (triggered()), SLOT (editRecord()));
	connect (actionDelete_, SIGNAL (triggered()), SLOT (deleteRecord()));
	connect (actionChangeMoney_, SIGNAL (triggered()), SLOT (changeMoney()));
	connect (actionNullMoney_, SIGNAL (triggered()), SLOT (nullMoney()));
	connect (actionSaveHeader_, SIGNAL (triggered()), SLOT (saveHeader()));
	connect (actionAbout_, SIGNAL (triggered()), SLOT (about()));
	connect (actionAboutQt_, SIGNAL (triggered()), qApp, SLOT (aboutQt()));
	connect (actionExit_, SIGNAL (triggered()), SLOT (close()));
	connect (actionOut_, SIGNAL (triggered()), SLOT (out()));
	connect (actionPreferences_, SIGNAL (triggered()), SLOT (preferences()));

	connect (organizationNameEdit_, SIGNAL (editingFinished ()), SLOT (onCheckReestr()));
	connect (filialNameEdit_, SIGNAL (editingFinished ()), SLOT (onCheckReestr()));
	connect (schetOrganizationEdit_, SIGNAL (editingFinished ()), SLOT (onCheckReestr()));
	connect (dogovorNumberEdit_, SIGNAL (editingFinished ()), SLOT (onCheckReestr()));
	connect (platNumberEdit_, SIGNAL (editingFinished()), SLOT (onCheckReestr()));
	connect (table_, SIGNAL (itemActivated (QTableWidgetItem *)), SLOT (itemActivated (QTableWidgetItem *)));
}

void MainWindowImpl::loadSettings()
{
	move (applicationSettings_->value ("MainWindow/pos", QPoint (0, 0)).toPoint());
	resize (applicationSettings_->value ("MainWindow/size", QSize (640, 480)).toSize());
	const bool isMaximized = applicationSettings_->value ("MainWindow/IsMaximized", false).toBool();

	if (isMaximized) {
		setWindowState (Qt::WindowMaximized);
	}

	for (int i = 0, count = table_->columnCount(); i < count; i++) {
		table_->setColumnWidth (i, applicationSettings_->value (QString ("MainTableColumns/Column_%1").arg (i), 50).toInt());
	}

	reestrNumberEdit_->setValue (applicationSettings_->value ("Global/ReestrNumber", 1).toInt());
}

void MainWindowImpl::saveSettings()
{
	if (windowState() != Qt::WindowMaximized) {
		applicationSettings_->setValue ("MainWindow/pos", pos());
		applicationSettings_->setValue ("MainWindow/size", size());
		applicationSettings_->setValue ("MainWindow/IsMaximized", false);
	} else {
		applicationSettings_->setValue ("MainWindow/IsMaximized", true);
	}

	for (int i = 0, count = table_->columnCount(); i < count; i++) {
		applicationSettings_->setValue (QString ("MainTableColumns/Column_%1").arg (i), table_->columnWidth (i));
	}

	applicationSettings_->setValue ("Global/ReestrNumber", reestrNumberEdit_->value());
	applicationSettings_->sync();
}

void MainWindowImpl::import (const QString &qsFileName)
{
	QFile file;
	file.setFileName (qsFileName);

	if (!file.open (QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical (this, tr ("Error"), tr ("Error open file"));
		return;
	}

	table_->clearContents();

	QStringList importList;

	QTextStream stream (&file);
	stream.setCodec ("IBM 866");

	QProgressDialog progress (tr ("Loading wait..."), tr ("No"), 0, 0, this);
	progress.setValue (0);
	progress.setWindowModality (Qt::WindowModal);

	while (!stream.atEnd()) {
		importList.append (stream.readLine());
	}

	file.close();

	progress.setMaximum (importList.count());
	table_->setRowCount (importList.count());

	for (int i = 0, count = importList.count(); i < count; i++) {
		if ( (i / 10) * 10 == i) {
			progress.setValue (i);
			QCoreApplication::processEvents();
		}

		const QStringList tmpList = importList [i].split ("|");

		if (tmpList.count() != 10) {
			progress.setValue (progress.maximum());
			const int result = QMessageBox::critical (this,
							   tr ("Error"),
							   QString (tr ("String %1 is not valid\nBreak?")).arg (i + 1),
							   QMessageBox::Yes | QMessageBox::No);

			if (result == QMessageBox::No) {
				table_->setRowCount (table_->rowCount() - 1);
			} else {
				table_->clearContents();
				table_->setRowCount (0);
				return;
			}
		} else {
			QTableWidgetItem *item;
			item = new QTableWidgetItem (tmpList.at (2));
			table_->setItem (i, 0, item);
			item = new QTableWidgetItem (tmpList.at (3));
			table_->setItem (i, 1, item);
			item = new QTableWidgetItem (tmpList.at (4));
			table_->setItem (i, 2, item);
			item = new QTableWidgetItem (tmpList.at (5));
			table_->setItem (i, 3, item);
			item = new QTableWidgetItem (tmpList.at (8));
			item->setTextAlignment (Qt::AlignRight | Qt::AlignVCenter);
			table_->setItem (i, 4, item);
		}
	}

	table_->selectRow (0);
	setCurrentFile (QString::null);
	loadHeader();
	updateActions();
	checkReestr();
}

void MainWindowImpl::openReestr()
{
	if (okToContinue()) {
		const QString fileName = QFileDialog::getOpenFileName (this,
								 tr ("Open file"),
								 applicationSettings_->value ("PATH/OpenPath").toString(),
								 tr ("DBase files (*.dbf)"));

		if (!fileName.isEmpty()) {
			applicationSettings_->setValue ("PATH/OpenPath", QFileInfo (fileName).absolutePath());
			loadFile (fileName);
		}
	}
}

void MainWindowImpl::import()
{
	if (okToContinue()) {
		const QString fileName = QFileDialog::getOpenFileName (this,
								 tr ("Open file"),
								 applicationSettings_->value ("PATH/ImportPath").toString(),
								 tr ("Old format files (*.s??)\nAll files (*.*)"));

		if (!fileName.isEmpty()) {
			applicationSettings_->setValue ("PATH/ImportPath", QFileInfo (fileName).absolutePath());
			import (fileName);
		}
	}
}

bool MainWindowImpl::loadFile (const QString &fileName)
{
	QSqlDatabase::database ().close();

	const QString connectionString = "DRIVER={Microsoft dBase Driver (*.dbf)};"
									 "FIL={dBase III;};DefaultDir="
									 + QFileInfo (fileName).absolutePath();
	QSqlDatabase::database ().setDatabaseName (connectionString);

	if (!QSqlDatabase::database ().open()) {
		QMessageBox::critical (0, tr ("Error connect"), QSqlDatabase::database ().lastError().text());
		return false;
	}

	QSqlQuery query;
	query.prepare ("SELECT COUNT(A) FROM " + QFileInfo (fileName).baseName());

	if (!query.exec()) {
		QMessageBox::critical (0, tr ("Error load table"), query.lastError().text());
		return false;
	}

	query.first();
	QProgressDialog progressDialog (tr ("Loading wait..."), tr ("No"), 0, query.value (0).toInt() , this);
	progressDialog.setWindowModality (Qt::WindowModal);

	query.prepare ("SELECT * FROM " + QFileInfo (fileName).baseName());

	if (!query.exec()) {
		QMessageBox::critical (0, tr ("Error load table"), query.lastError().text());
		return false;
	}

	query.first();
	table_->clearContents();
	table_->setRowCount (0);

	QTableWidgetItem *item = 0;
	int row = 0;

	//Read header
	filialNameEdit_->setText (query.value (0).toString().replace (tr ("#"), tr ("№")));
	query.next();
	progressDialog.setValue (progressDialog.value() + 1);
	platNumberEdit_->setText (query.value (1).toString());
	platDateEdit_->setDate (QDate().fromString (query.value (3).toString(), "dd.MM.yyyy"));
	query.next();
	progressDialog.setValue (progressDialog.value() + 1);
	vidEdit_->setCurrentIndex (query.value (1).toInt() - 1);
	query.next();
	progressDialog.setValue (progressDialog.value() + 1);
	organizationNameEdit_->setText (query.value (1).toString().replace (tr ("#"), tr ("№")));
	schetOrganizationEdit_->setText (query.value (2).toString());
	query.next();
	progressDialog.setValue (progressDialog.value() + 1);
	dogovorNumberEdit_->setText (query.value (1).toString());
	dogovorDateEdit_->setDate (QDate().fromString (query.value (3).toString(), "dd.MM.yyyy"));
	query.next();
	progressDialog.setValue (progressDialog.value() + 1);
//End read header
	table_->setUpdatesEnabled (false);
	table_->setRowCount (progressDialog.maximum() - 5);

	while (query.next()) {
		item = new QTableWidgetItem (query.value (2).toString());
		table_->setItem (row, 0, item);
		item = new QTableWidgetItem (query.value (3).toString());
		table_->setItem (row, 1, item);
		item = new QTableWidgetItem (query.value (4).toString());
		table_->setItem (row, 2, item);
		item = new QTableWidgetItem (query.value (1).toString());
		table_->setItem (row, 3, item);
		item = new QTableWidgetItem (query.value (5).toString());
		item->setTextAlignment (Qt::AlignRight | Qt::AlignVCenter);
		table_->setItem (row, 4, item);
		++row;
		progressDialog.setValue (progressDialog.value() + 1);
		QCoreApplication::processEvents (QEventLoop::ExcludeUserInputEvents);
	}

	progressDialog.setValue (progressDialog.maximum());
	QCoreApplication::processEvents (QEventLoop::ExcludeUserInputEvents);
	query.clear();
	QSqlDatabase::database ().close();

	table_->removeRow (table_->rowCount() - 1);
	table_->selectRow (0);
	setCurrentFile (fileName);
	updateActions();
	checkReestr();
	table_->setUpdatesEnabled (true);
	return true;
}

bool MainWindowImpl::saveReestr()
{
	if (currentFile_.isEmpty()) {
		return saveAsReestr();
	}

	if (checkReestr() == 1) {
		QMessageBox::critical (this, tr ("Error saving"), tr ("Find errors in reestr"));
		return false;
	}

	if (checkReestr() == 2) {
		const int result = QMessageBox::warning (this,
						   tr ("Warning saving"),
						   tr ("Find warnings in reestr.\nSave?"),
						   QMessageBox::Ok,
						   QMessageBox::Cancel);

		if (result == QMessageBox::Cancel)
			return false;
	}

	return saveFile (currentFile_);
}

bool MainWindowImpl::saveAsReestr()
{
	if (checkReestr() == 1) {
		QMessageBox::critical (this, tr ("Error saving"), tr ("Find errors in reestr"));
		return false;
	}

	if (checkReestr() == 2) {
		const int result = QMessageBox::warning (this,
						   tr ("Warning saving"),
						   tr ("Find warnings in reestr.\nSave?"),
						   QMessageBox::Ok,
						   QMessageBox::Cancel);

		if (result == QMessageBox::Cancel)
			return false;
	}

	const QString fileName = QFileDialog::getSaveFileName (this,
							 tr ("Open file"),
							 applicationSettings_->value ("PATH/OpenPath").toString(),
							 tr ("DBase files (*.dbf)"));

	if (fileName.isEmpty()) {
		return false;
	}

	applicationSettings_->setValue ("PATH/OpenPath", QFileInfo (fileName).absolutePath());
	return saveFile (fileName);
}

bool MainWindowImpl::saveFile (const QString &fileName)
{
	if (QFileInfo (fileName).exists()) {
		QFile::remove (fileName);
	}

	QSqlDatabase::database ().close();

	const QString connectionString = "DRIVER={Microsoft dBase Driver (*.dbf)};"
									 "FIL={dBase III;};DefaultDir="
									 + QFileInfo (fileName).absolutePath();
	QSqlDatabase::database ().setDatabaseName (connectionString);

	if (!QSqlDatabase::database ().open()) {
		QMessageBox::critical (this, tr ("Error connect"), QSqlDatabase::database ().lastError().text());
		return false;
	}

	const QString tableName = QFileInfo (fileName).baseName();

	QSqlQuery query;

	query.prepare ("CREATE TABLE "
				   + tableName
				   + " (A CHAR(30),B CHAR(30),C CHAR(30),D CHAR(30),E CHAR(30),F CHAR(30),G CHAR(30));");

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save file"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A) VALUES (:a);");
	query.bindValue (":a", filialNameEdit_->text().replace (tr ("№"), tr ("#")));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save file"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A,B,C,D) VALUES (:a,:b,:c,:d);");
	query.bindValue (":a", tr ("К платежному поручению #"));
	query.bindValue (":b", platNumberEdit_->text());
	query.bindValue (":c", tr ("от"));

	query.bindValue (":d",platNumberEdit_->text().isEmpty()
	? ""
	: platDateEdit_->date().toString ("dd.MM.yyyy"));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A,B) VALUES (:a,:b);");
	query.bindValue (":a", tr ("Зачисление"));
	query.bindValue (":b", tr ("0") + QString().setNum (vidEdit_->currentIndex() + 1));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A,B,C) VALUES (:a,:b,:c);");
	query.bindValue (":a", tr ("Наименование,ОГРН,# банк.счета"));
	query.bindValue (":b", organizationNameEdit_->text().replace (tr ("№"), tr ("#")));
	query.bindValue (":c", schetOrganizationEdit_->text());

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A,B,C,D) VALUES (:a,:b,:c,:d);");
	query.bindValue (":a", tr ("По договору:"));
	query.bindValue (":b", dogovorNumberEdit_->text());
	query.bindValue (":c", tr ("от"));
	query.bindValue (":d", dogovorDateEdit_->date().toString ("dd.MM.yyyy"));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();

	query.prepare ("INSERT INTO " + tableName + "(A,B,C,D,E,F,G) VALUES (:a,:b,:c,:d,:e,:f,:g);");
	query.bindValue (":a", tr ("# п/п"));
	query.bindValue (":b", tr ("Номер счета"));
	query.bindValue (":c", tr ("Фамилия"));
	query.bindValue (":d", tr ("Имя"));
	query.bindValue (":e", tr ("Отчество"));
	query.bindValue (":f", tr ("Сумма"));
	query.bindValue (":g", tr ("Примечание"));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();

	int nullCount = 0;

	for (int i = 0, count = table_->rowCount(); i < count; ++i) {
		if (table_->item (i, 4)->text().toDouble() == 0.00) {
			++nullCount;
			continue;
		}

		query.prepare ("INSERT INTO " + tableName + "(A,B,C,D,E,F) VALUES (:a,:b,:c,:d,:e,:f);");
		query.bindValue (":a", QString().setNum (i + 1 - nullCount));
		query.bindValue (":b", table_->item (i, 3)->text());
		query.bindValue (":c", table_->item (i, 0)->text());
		query.bindValue (":d", table_->item (i, 1)->text());
		query.bindValue (":e", table_->item (i, 2)->text());
		query.bindValue (":f", table_->item (i, 4)->text());

		if (!query.exec()) {
			QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
			return false;
		}

		query.clear();
	}

	query.prepare ("INSERT INTO " + tableName + "(B,F) VALUES (:b,:f);");
	query.bindValue (":b", tr ("ИТОГО:"));
	query.bindValue (":f", QString().setNum (getSum(), 'f', 2));

	if (!query.exec()) {
		QMessageBox::critical (this, tr ("Error save base"), query.lastError().text());
		return false;
	}

	query.clear();
	QSqlDatabase::database ().close();
	setCurrentFile (fileName);
	return true;
}

double MainWindowImpl::getSum()
{
	double sum = 0;

	for (int i = 0, count = table_->rowCount(); i < count; ++i) {
		sum += table_->item (i, 4)->text().toDouble();
	}

	return sum;
}

void MainWindowImpl::updateActions()
{
	sumLabel_->setText (tr ("Count = %1 Summa = %2").arg (table_->rowCount()).arg (getSum(), 0, 'f', 2));
	const bool isEmptyTable = ( (table_->rowCount() > 0)
	&& (table_->currentRow() > -1));
	actionEdit_->setEnabled (isEmptyTable);
	actionChangeMoney_->setEnabled (isEmptyTable);
	actionDelete_->setEnabled (isEmptyTable);
	actionNullMoney_->setEnabled (table_->rowCount() > 0);
}

void MainWindowImpl::addRecord()
{
	AddDialog addDialog (this);
	addDialog.setWindowTitle (tr ("Adding"));
	addDialog.setModal (true);
	addDialog.startCheck();

	if (addDialog.exec()) {
		table_->insertRow (table_->rowCount());
		const int row = table_->rowCount() - 1;
		QTableWidgetItem *item;
		item = new QTableWidgetItem (addDialog.familEdit_->text());
		table_->setItem (row, 0, item);
		item = new QTableWidgetItem (addDialog.nameEdit_->text());
		table_->setItem (row, 1, item);
		item = new QTableWidgetItem (addDialog.otchEdit_->text());
		table_->setItem (row, 2, item);
		item = new QTableWidgetItem (addDialog.schetEdit_->text());
		table_->setItem (row, 3, item);
		item = new QTableWidgetItem (QString().setNum (addDialog.moneyEdit_->text().toDouble(), 'f', 2).replace (',', '.'));
		item->setTextAlignment (Qt::AlignRight | Qt::AlignVCenter);
		table_->setItem (row, 4, item);
		table_->selectRow (row);
		setWindowModified (true);
	}

	updateActions();
	checkReestr();
}

void MainWindowImpl::editRecord()
{
	const int row = table_->currentRow();

	if (row < 0)
		return;

	AddDialog editDialog (this);
	editDialog.setWindowTitle (tr ("Editing"));
	editDialog.familEdit_->setText (table_->item (row, 0)->text());
	editDialog.nameEdit_->setText (table_->item (row, 1)->text());
	editDialog.otchEdit_->setText (table_->item (row, 2)->text());
	editDialog.schetEdit_->setText (table_->item (row, 3)->text());
	editDialog.moneyEdit_->setText (table_->item (row, 4)->text());
	editDialog.startCheck();

	if (editDialog.exec()) {
		table_->item (row, 0)->setText (editDialog.familEdit_->text());
		table_->item (row, 1)->setText (editDialog.nameEdit_->text());
		table_->item (row, 2)->setText (editDialog.otchEdit_->text());
		table_->item (row, 3)->setText (editDialog.schetEdit_->text());
		table_->item (row, 4)->setText (QString().setNum (editDialog.moneyEdit_->text().toDouble(), 'f', 2).replace (',', '.'));
		setWindowModified (true);
	}

	updateActions();
	checkReestr();
}

void MainWindowImpl::deleteRecord()
{
	const int row = table_->currentRow();

	if (row < 0) {
		return;
	}

	table_->removeRow (row);

	if (table_->currentIndex().isValid()) {
		table_->selectRow (table_->currentIndex().row());
	}

	setWindowModified (true);
	updateActions();
	checkReestr();
}

void MainWindowImpl::changeMoney()
{
	const int row = table_->currentRow();

	if (row < 0) {
		return;
	}

	AddDialog dialog (this);
	dialog.setWindowTitle (tr ("Changing money"));
	dialog.familEdit_->setVisible (false);
	dialog.nameEdit_->setVisible (false);
	dialog.otchEdit_->setVisible (false);
	dialog.schetEdit_->setVisible (false);
	dialog.familLabel_->setVisible (false);
	dialog.nameLabel_->setVisible (false);
	dialog.otchLabel_->setVisible (false);
	dialog.schetLabel_->setVisible (false);
	dialog.moneyEdit_->setText (table_->item (row, 4)->text());
	dialog.moneyEdit_->selectAll();

	if (dialog.exec()) {
		table_->item (row, 4)->setText (QString().setNum (dialog.moneyEdit_->text().toDouble(), 'f', 2).replace (',', '.'));
		setWindowModified (true);
	}

	updateActions();
	checkReestr();
}

void MainWindowImpl::nullMoney()
{
	for (int i = 0, count = table_->rowCount(); i < count; ++i) {
		table_->item (i, 4)->setText ("0.00");
	}

	setWindowModified (true);
	updateActions();
	checkReestr();
}

int MainWindowImpl::checkReestr()
{
	int isValid = 0;

	if (organizationNameEdit_->text().isEmpty()) {
		setErrorPalette (organizationNameEdit_);
		isValid = 1;
	} else {
		organizationNameEdit_->setPalette (QApplication::palette());
	}

	if (filialNameEdit_->text().isEmpty()) {
		setErrorPalette (filialNameEdit_);
		isValid = 1;
	} else {
		filialNameEdit_->setPalette (QApplication::palette());
	}

	if ( (schetOrganizationEdit_->text().isEmpty()) || (schetOrganizationEdit_->text().count() != 20)) {
		setErrorPalette (schetOrganizationEdit_);
		isValid = 1;
	} else {
		schetOrganizationEdit_->setPalette (QApplication::palette());
	}

	if ( (dogovorNumberEdit_->text().isEmpty()) || (dogovorNumberEdit_->text().count() != 8)) {
		setErrorPalette (dogovorNumberEdit_);
		isValid = 1;
	} else {
		dogovorNumberEdit_->setPalette (QApplication::palette());
	}

	if ( (platNumberEdit_->text().isEmpty()) || (platNumberEdit_->text().count() != 6)) {
		setWarningPalette (platNumberEdit_);

		if (isValid == 0) {
			isValid = 2;
	}
	} else {
		platNumberEdit_->setPalette (QApplication::palette());
	}

	if (table_->rowCount() <= 0) {
		isValid = 1;
	}

	for (int i = 0; i < table_->rowCount(); i++) {
		if (table_->item (i, 0)->text().isEmpty()) { //Famil
			table_->item (i, 0)->setBackgroundColor (Qt::red);
			isValid = 1;
		} else {
			table_->item (i, 0)->setBackgroundColor (Qt::white);
		}

		if (table_->item (i, 1)->text().isEmpty()) { //Name
			table_->item (i, 1)->setBackgroundColor (Qt::red);
			isValid = 1;
		} else {
			table_->item (i, 1)->setBackgroundColor (Qt::white);
		}

		if (table_->item (i, 2)->text().isEmpty()) { //Otch
			table_->item (i, 2)->setBackgroundColor (Qt::yellow);

			if (isValid == 0) {
				isValid = 2;
			}
		} else {
			table_->item (i, 2)->setBackgroundColor (Qt::white);
		}

		if (!checkSchet (table_->item (i, 3)->text())) { //Schet
			table_->item (i, 3)->setBackgroundColor (Qt::red);
			isValid = 1;
		} else {
			table_->item (i, 3)->setBackgroundColor (Qt::white);
		}

		if ( (table_->item (i, 4)->text().isEmpty()) || (table_->item (i, 4)->text().toDouble() < 0)) { //Money
			table_->item (i, 4)->setBackgroundColor (Qt::red);
			isValid = 1;
		} else {
			table_->item (i, 4)->setBackgroundColor (Qt::white);
		}
	}

	return isValid;
}

bool MainWindowImpl::checkSchet (const QString &schet)
{
	return schet.length() == 20
	&& schet.mid (5, 3) == QLatin1String ("810");
}

void MainWindowImpl::saveHeader()
{
	applicationSettings_->setValue ("Header/OrganizationName", organizationNameEdit_->text());
	applicationSettings_->setValue ("Header/FilialName", filialNameEdit_->text());
	applicationSettings_->setValue ("Header/SchetOrganization", schetOrganizationEdit_->text());
	applicationSettings_->setValue ("Header/DogovorNumber", dogovorNumberEdit_->text());
	applicationSettings_->setValue ("Header/DogovorDate", dogovorDateEdit_->date());
	applicationSettings_->setValue ("Header/Vid", vidEdit_->currentIndex());
	applicationSettings_->sync();
}

void MainWindowImpl::loadHeader()
{
	organizationNameEdit_->setText (applicationSettings_->value ("Header/OrganizationName", QString()).toString());
	filialNameEdit_->setText (applicationSettings_->value ("Header/FilialName", QString()).toString());
	schetOrganizationEdit_->setText (applicationSettings_->value ("Header/SchetOrganization", QString()).toString());
	dogovorNumberEdit_->setText (applicationSettings_->value ("Header/DogovorNumber", QString()).toString());
	dogovorDateEdit_->setDate (applicationSettings_->value ("Header/DogovorDate", QString()).toDate());
	vidEdit_->setCurrentIndex (applicationSettings_->value ("Header/Vid", QString()).toInt());
}

void MainWindowImpl::closeEvent (QCloseEvent *event)
{
	if (okToContinue()) {
		event->accept();
	} else {
		event->ignore();
	}
}

void MainWindowImpl::createContextMenu()
{
	table_->addAction (actionAdd_);
	table_->addAction (actionEdit_);
	table_->addAction (actionDelete_);
	table_->addAction (actionChangeMoney_);
	table_->addAction (actionNullMoney_);
	table_->setContextMenuPolicy (Qt::ActionsContextMenu);
}

void MainWindowImpl::createToolBars()
{
	fileToolbar_ = new QToolBar (tr ("File"));
	fileToolbar_->addAction (actionNew_);
	fileToolbar_->addAction (actionOpen_);
	fileToolbar_->addAction (actionSave_);
	fileToolbar_->addAction (actionSaveAs_);
	fileToolbar_->addAction (actionOut_);
	fileToolbar_->addAction (actionImport_);
	fileToolbar_->addAction (actionPreferences_);
	addToolBar (fileToolbar_);

	reestrToolbar_ = new QToolBar (tr ("Reestr"));
	reestrToolbar_->addAction (actionAdd_);
	reestrToolbar_->addAction (actionEdit_);
	reestrToolbar_->addAction (actionDelete_);
	reestrToolbar_->addAction (actionChangeMoney_);
	reestrToolbar_->addAction (actionNullMoney_);
	addToolBar (reestrToolbar_);
}

bool MainWindowImpl::okToContinue()
{
	if (isWindowModified()) {
		const int result = QMessageBox::warning (this,
						   QApplication::applicationName(),
						   tr ("The reestr has been modified.\nDo you want to save your changes?"),
						   QMessageBox::Yes | QMessageBox::Default,
						   QMessageBox::No,
						   QMessageBox::Cancel);

		if (result == QMessageBox::Yes) {
			return saveReestr();
		}

		if (result == QMessageBox::Cancel) {
			return false;
		}
	}

	return true;
}

void MainWindowImpl::newReestr()
{
	if (okToContinue()) {
		table_->clearContents();
		table_->setRowCount (0);
		setCurrentFile (QString::null);
	}
}

void MainWindowImpl::setCurrentFile (const QString &fileName)
{
	currentFile_ = fileName;
	setWindowModified (false);
	const QString showName = currentFile_.isEmpty()
	? tr ("Untitled")
	: QFileInfo (currentFile_).fileName();

	setWindowTitle (tr ("%1 %2[*]").arg (QApplication::applicationName()).arg (showName));
	updateActions();
}

void MainWindowImpl::about()
{
	QMessageBox::about (this, tr ("About Bank"),
						tr ("<h2>") + QApplication::applicationName() + tr ("</h2>") +
						tr ("<p>Author: PanteR"
							"<p>e-mail: panter.dsd@gmail.com"
							"<p>tel: 89094119675"
							"<p>License: GNU GPL v2"));
}

void MainWindowImpl::out()
{
	if (checkReestr() == 1) {
		QMessageBox::critical (this, tr ("Error saving"), tr ("Find errors in reestr"));
		return;
	}

	if (checkReestr() == 2) {
		const int result = QMessageBox::warning (this,
						   tr ("Warning saving"),
						   tr ("Find warnings in reestr.\nSave?"),
						   QMessageBox::Ok,
						   QMessageBox::Cancel);

		if (result == QMessageBox::Cancel) {
			return;
		}
	}

	QDir dir (applicationSettings_->value ("PATH/ArchivesPath", QString()).toString());

	if (!dir.exists()) {
		dir.setCurrent (QCoreApplication::applicationDirPath());
		dir.cdUp();
		dir.cd ("archives");
	}

	const bool isModified = isWindowModified();
	const QString curFile = currentFile_;
	QFileInfo outFile (dir.absolutePath() +
					   "/" +
					   QDate::currentDate().toString ("yyMMdd") +
					   QTime::currentTime().toString ("ss") +
					   ".dbf");

	if (saveFile (outFile.filePath())) {
		setCurrentFile (curFile);
		setWindowModified (isModified);
		const QString outName = applicationSettings_->value ("PATH/OutPath", QString()).toString() +
								QDir::separator() +
								"t1820" +
								QString ("%1").arg (reestrNumberEdit_->value(), 3, 10, QChar ('0')) +
								".dbf";

		if (!QFile::copy (outFile.filePath(), outName)) {
			QMessageBox::warning (this,
								  tr ("Error out"),
								  tr ("Error out file %1").arg (outName));
		} else {
			QMessageBox::information (this,
									  tr ("Out"),
									  tr ("Out is suxcessed"));
			reestrNumberEdit_->setValue (reestrNumberEdit_->value() + 1);
		}
	}
}

void MainWindowImpl::preferences()
{
	Preferences preferences;
	preferences.setWindowTitle (tr ("Preferences"));
	preferences.exec();
}
