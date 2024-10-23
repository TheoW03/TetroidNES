#define VERSION "1.0.0-Debug" // DO NOT EDIT: CMake handles this

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtLogging>

#include <mainwindow.h>
#include <log.h>

int main(int argc, char *argv[])
{
    // originalHandler = qInstallMessageHandler(logToFile);
    InitLogs();

    QApplication a(argc, argv);
    // auto a = make
    a.setApplicationName("TetroidNES");

    a.setApplicationVersion(VERSION);

    qInfo() << "STARTING" << a.applicationName() << "VERSION" << a.applicationVersion();
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "tetroidnes_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}