#ifndef QMYPREFERENCES_H_
#define QMYPREFERENCES_H_
#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QSettings;

class QMyPreferences: public QDialog
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
	QMyPreferences(QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowSystemMenuHint);

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

#endif /*QMYPREFERENCES_H_*/
