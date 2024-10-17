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

#define VERSION "1.0.0"

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
    // QFile file("version");
    a.setApplicationVersion(VERSION);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    // {
    //     qWarning() << "Could not open version file!";
    //     a.setApplicationVersion("Version file could not be opened");
    // }
    // else
    // {
    //     QTextStream stream(&file);

    //     while (!stream.atEnd())
    //     {
    //         QString line = file.readLine();
    //         qDebug() << "Reading version file:" << line;
    //         if (line.startsWith("version="))
    //         {
    //             a.setApplicationVersion(line.split("=").back());
    //             break;
    //         }
    //     }
    //     // Checking if a version was found
    //     if (a.applicationVersion().isEmpty())
    //     {
    //         qWarning() << "Could not find version in version file!";
    //         a.setApplicationVersion("Version could not be found");
    //     }
    //     stream.flush();
    // }

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
    int exit_code = a.exec();
    qInfo() << "ENDING" << a.applicationName() << "VERSION" << a.applicationVersion() << "EXIT CODE" << exit_code;
    return exit_code;
}