#define VERSION "1.0.0" // DO NOT EDIT: CMake handles this

#include <mainwindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <filesystem>
#include <QDir>
#include <Qt/log.h>

int main(int argc, char *argv[])
{
    // originalHandler = qInstallMessageHandler(logToFile);
    InitLogs();

    QApplication a(argc, argv);
    // auto a = make
    a.setApplicationName("TetroidNES");

    a.setApplicationVersion(VERSION);

    qInfo() << "STARTING" << a.applicationName() << "VERSION" << a.applicationVersion();
    // QObject::connect(&a, &QApplication::aboutToQuit, cleanUp);

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