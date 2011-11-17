#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QFileInfo>

#include <QtGui/QApplication>

#include "mainwindowimpl.h"

QString version = "2.1.1.1";

int main (int argc, char **argv)
{
	QApplication app (argc, argv);

	QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));
	app.setApplicationName (QApplication::tr ("Bank") + " " + version);

	QTranslator translator;
	translator.load (app.applicationDirPath() + "/../share/Languages/Bank_" + QLocale::system().name() + ".qm");
	app.installTranslator (&translator);

	QTranslator translatorQt;
	translatorQt.load (app.applicationDirPath() + "/../share/Languages/qt_" + QLocale::system().name() + ".qm");
	app.installTranslator (&translatorQt);

	MainWindowImpl win;
	win.setVersion (version);

	QStringList args = app.arguments();

	if (args.count() > 1) {
		QFileInfo qfFile (args.at (1));

		if (qfFile.exists()) {
			if (qfFile.suffix().toLower() == "dbf")
				win.loadFile (qfFile.absoluteFilePath());
			else
				win.import (qfFile.absoluteFilePath());
		}
	}

	win.show();
	app.connect (&app, SIGNAL (lastWindowClosed()), &app, SLOT (quit()));

	return app.exec();
}
