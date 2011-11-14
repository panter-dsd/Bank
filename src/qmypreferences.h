#ifndef QMYPREFERENCES_H_
#define QMYPREFERENCES_H_
#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QSettings;
//
class QMyPreferences: public QDialog
{
Q_OBJECT
private:
	QLabel*						qlOpenPath;
	QLineEdit*					qleOpenPath;
	QPushButton*				qpbOpenPath;
	QLabel*						qlImportPath;
	QLineEdit*					qleImportPath;
	QPushButton*				qpbImportPath;
	QLabel*						qlArchivesPath;
	QLineEdit*					qleArchivesPath;
	QPushButton*				qpbArchivesPath;
	QLabel*						qlOutPath;
	QLineEdit*					qleOutPath;
	QPushButton*				qpbOutPath;
	
	QDialogButtonBox*	qdbbButtonBox;
	
	QSettings*					qsetAppSettings;

public:
	QMyPreferences(QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowSystemMenuHint);
private:
	void createWidgets();
	void createLayouts();
	void savePreferences();
	void createConnects();
private slots:
	void slotSavePreferences();
	void slotSetOpenPath();
	void slotSetImportPath();
	void slotSetArchivesPath();
	void slotSetOutPath();
};

#endif /*QMYPREFERENCES_H_*/
