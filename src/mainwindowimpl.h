#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
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
#include <QSqlDatabase>
#include <QMainWindow>
#include "qmyadddialog.h"
#include "qmypreferences.h"
//
class MainWindowImpl : public QMainWindow
{
Q_OBJECT
private:
	QSqlDatabase						qsdBase;
	QWidget*								qwCentralWidget;
	QTableWidget*					qtwTable;
	QStatusBar*						qsbStatusBar;
	QToolBar*							qtbFileToolbar;
	QToolBar*							qtbReestrToolbar;
	QMenuBar*							qmbMainMenu;
	QLabel*								qlSumm;

	QGroupBox*						qgbHeaderGroup;
	QLabel*								qlOrganizationName;
	QLineEdit*							qleOrganizationName;
	QLabel*								qlReestrNumber;
	QSpinBox*							qsbReestrNumber;
	QLabel*								qlFilialName;
	QLineEdit*							qleFilialName;
	QLabel*								qlSchetOrganization;
	QLineEdit*							qleSchetOrganization;
	QLabel*								qlDogovorNumber;
	QLineEdit*							qleDogovorNumber;
	QLabel*								qlDogovorDate;
	QDateEdit*							qdeDogovorDate;
	QLabel*								qlPlatNumber;
	QLineEdit*							qlePlatNumber;
	QLabel*								qlPlatDate;
	QDateEdit*							qdePlatDate;
	QLabel*								qlVid;
	QComboBox*						qcbVid;

	QAction*								qaNew_cmd;//
	QAction*								qaImport_cmd;
	QAction*								qaOpen_cmd;//
	QAction*								qaSave_cmd;//
	QAction*								qaSaveAs_cmd;//
	QAction*								qaAdd_cmd;//
	QAction*								qaEdit_cmd;//
	QAction*								qaDelete_cmd;//
	QAction*								qaChangeMoney_cmd;
	QAction*								qaNullMoney_cmd;
	QAction*								qaSaveHeader_cmd;
	QAction*								qaAbout_cmd;//
	QAction*								qaAboutQt_cmd;//
	QAction*								qaExit_cmd;//
	QAction*								qaOut_cmd;
	QAction*								qaPreferences_cmd;//
	
	QSettings*							qsetAppSettings;
	QString								qsCurrentFile;
	QString								qsVersion;
	
	QMyPreferences*				qmpPreferences;
	
protected:
	void closeEvent(QCloseEvent* event);
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void setVersion(const QString& qsAppVersion) {qsVersion=qsAppVersion;}
	~MainWindowImpl();
	bool loadFile(const QString& qsFileName);
	void import(const QString& qsFileName );
private:
	void createControls();
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void setLayouts();
	void setConnects();
	bool okToContinue();
	
	double getSumm();
	void loadSettings();
	void saveSettings();
	void loadHeader();
	bool saveFile(const QString& qsFileName);
	void setCurrentFile(const QString& qsFileName);
	int checkReestr();
	bool checkSchet(const QString& qsSchet);
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
private slots:
	void slotNewReestr();
	void slotOpenReestr();
	bool slotSaveReestr();
	bool slotSaveAsReestr();
	void slotAbout();
	void slotImport();
	void slotUpdateActions();
	void slotChangeMoney();
	void slotSaveHeader();
	void slotAdd();
	void slotEdit();
	void slotDelete();
	void slotNullMoney();
	bool slotCheckReestr() {checkReestr(); return true;};
	void slotItemActivated(QTableWidgetItem* item) {slotChangeMoney();}
	void slotOut();
	void slotPreferences();
};
#endif

