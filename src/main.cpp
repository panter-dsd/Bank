#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QFileInfo>
#include "mainwindowimpl.h"
//
QString qsVersion="2.1.1.1";
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	app.setApplicationName(QApplication::tr("Bank")+" "+qsVersion);

	QTranslator translator;
	translator.load(app.applicationDirPath()+"/../share/Languages/Bank_"+QLocale::system().name()+".qm");
	app.installTranslator(&translator);
	
	QTranslator translatorQt;
	translatorQt.load(app.applicationDirPath()+"/../share/Languages/qt_"+QLocale::system().name()+".qm");
	app.installTranslator(&translatorQt);
	
	MainWindowImpl win;
	win.setVersion(qsVersion);
	QStringList qslArgs=app.arguments();
	if (qslArgs.count()>1)
	{
		QFileInfo qfFile(qslArgs.at(1));
		if (qfFile.exists())
		{
			if (qfFile.suffix().toLower()=="dbf")
				win.loadFile(qfFile.absoluteFilePath());
			else
				win.import(qfFile.absoluteFilePath());
		}
	}
	win.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
