#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui/QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QSettings;

class Preferences: public QDialog
{
	Q_OBJECT

private:
	QLabel*						openPathLabel_;
	QLineEdit*					openPathEdit_;
	QPushButton*				openPathButton_;
	QLabel*						importPathLabel_;
	QLineEdit*					importPathEdit_;
	QPushButton*				importPathButton_;
	QLabel*						archivesPathLabel_;
	QLineEdit*					archivesPathEdit_;
	QPushButton*				archivesPathButton_;
	QLabel*						outPathLabel_;
	QLineEdit*					outPathEdit_;
	QPushButton*				outPathButton_;

	QDialogButtonBox*	buttons;

	QSettings*					applicationSettings_;

public:
	Preferences(QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowSystemMenuHint);

private:
	void createWidgets();
	void createLayouts();
	void createConnects();

private Q_SLOTS:
	void savePreferences();
	void setOpenPath();
	void setImportPath();
	void setArchivesPath();
	void setOutPath();
};

#endif //PREFERENCES_H
