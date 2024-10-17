#define VERSION "UNDEFINED" // DO NOT EDIT: CMake handles this

#include <mainwindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <stdio.h>
#include <stdlib.h>

QtMessageHandler originalHandler = nullptr;

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("log.txt", "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);

    if (originalHandler)
    {
        originalHandler(type, context, msg);
    }
        
}

int main(int argc, char *argv[])
{

    originalHandler = qInstallMessageHandler(logToFile);
    qSetMessagePattern("%{type} | %{function}:%{line} | %{time h:mm:ss.zzz} | %{message}");

    QApplication a(argc, argv);
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