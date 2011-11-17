#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

class QWidget;
class QTableWidget;
class QTableWidgetItem;
class QStatusBar;
class QToolBar;
class QMenuBar;
class QAction;
class QSettings;
class QGroupBox;
class QLabel;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QBrush;
class QPalette;
class QSpinBox;

#include <QtGui/QMainWindow>

class MainWindowImpl : public QMainWindow
{
	Q_OBJECT
private:
	QTableWidget					*table_;
	QStatusBar						*statusBar_;
	QToolBar							*fileToolbar_;
	QToolBar							*reestrToolbar_;
	QMenuBar							*mainMenu_;
	QLabel								*sumLabel_;

	QGroupBox						*headerGroup_;
	QLabel								*organizationNameLabel_;
	QLineEdit							*organizationNameEdit_;
	QLabel								*reestrNumberLabel_;
	QSpinBox							*reestrNumberEdit_;
	QLabel								*filialNameLabel_;
	QLineEdit							*filialNameEdit_;
	QLabel								*schetOrganizationLabel_;
	QLineEdit							*schetOrganizationEdit_;
	QLabel								*dogovorNumberLabel_;
	QLineEdit							*dogovorNumberEdit_;
	QLabel								*dogovorDateLabel_;
	QDateEdit							*dogovorDateEdit_;
	QLabel								*platNumberLabel_;
	QLineEdit							*platNumberEdit_;
	QLabel								*platDateLabel_;
	QDateEdit							*platDateEdit_;
	QLabel								*vidLabel_;
	QComboBox						*vidEdit_;

	QAction								*actionNew_;
	QAction								*actionImport_;
	QAction								*actionOpen_;
	QAction								*actionSave_;
	QAction								*actionSaveAs_;
	QAction								*actionAdd_;
	QAction								*actionEdit_;
	QAction								*actionDelete_;
	QAction								*actionChangeMoney_;
	QAction								*actionNullMoney_;
	QAction								*actionSaveHeader_;
	QAction								*actionAbout_;
	QAction								*actionAboutQt_;
	QAction								*actionExit_;
	QAction								*actionOut_;
	QAction								*actionPreferences_;

	QSettings							*applicationSettings_;
	QString								currentFile_;
	QString								version_;

protected:
	void closeEvent (QCloseEvent *event);

public:
	MainWindowImpl (QWidget *parent = 0, Qt::WFlags f = 0);
	~MainWindowImpl();

	void setVersion (const QString &version) {
		version_ = version;
	}
	bool loadFile (const QString &fileName);
	void import (const QString &fileName);

private:
	void createControls();
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void setLayouts();
	void setConnects();
	bool okToContinue();

	double getSum();
	void loadSettings();
	void saveSettings();
	void loadHeader();
	bool saveFile (const QString &fileName);
	void setCurrentFile (const QString &fileName);
	int checkReestr();
	bool checkSchet (const QString &schet);
	void setErrorPalette (QWidget *widget) {
		QBrush brush (Qt::red);
		QPalette palette = widget->palette();
		palette.setBrush (QPalette::Active, QPalette::Base, brush);
		widget->setPalette (palette);
	}
	void setWarningPalette (QWidget *widget) {
		QBrush brush (Qt::yellow);
		QPalette palette = widget->palette();
		palette.setBrush (QPalette::Active, QPalette::Base, brush);
		widget->setPalette (palette);
	}

private Q_SLOTS:
	void newReestr();
	void openReestr();
	bool saveReestr();
	bool saveAsReestr();
	void about();
	void import();
	void updateActions();
	void changeMoney();
	void saveHeader();
	void addRecord();
	void editRecord();
	void deleteRecord();
	void nullMoney();
	bool onCheckReestr() {
		checkReestr();
		return true;
	}
	void itemActivated (QTableWidgetItem *item) {
		changeMoney();
	}
	void out();
	void preferences();
};
#endif

