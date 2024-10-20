#define VERSION "1.0.0-Debug" // DO NOT EDIT: CMake handles this

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

QtMessageHandler originalHandler = nullptr;

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    std::filesystem::create_directories("logs");
    std::time_t t = std::time(0); // t is an integer type
    // char *intStr = itoa(t);
    // std::string str = std::string(intStr);
    std::string time_stamp = std::to_string(t);
    auto file = "logs/log_" + time_stamp + ".txt";
    static FILE *f = fopen(file.c_str(), "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);
    if (originalHandler)
    {
        originalHandler(type, context, msg);
    }
}

void cleanUp()
{
    std::cout << "Application is about to quit. Performing clean-up..." << std::endl;
    // Add your custom clean-up logic here
}

int main(int argc, char *argv[])
{

    originalHandler = qInstallMessageHandler(logToFile);
    qSetMessagePattern("%{type} | %{function}:%{line} | %{time h:mm:ss.zzz} | %{message}");

    QApplication a(argc, argv);
    // auto a = make
    a.setApplicationName("TetroidNES");

    a.setApplicationVersion(VERSION);

    qInfo() << "STARTING" << a.applicationName() << "VERSION" << a.applicationVersion();
    QObject::connect(&a, &QApplication::aboutToQuit, cleanUp);

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

    // int exit_code = a.exec();

    // qInfo() << "ENDING" << a.applicationName() << "VERSION" << a.applicationVersion() << "EXIT CODE" << exit_code;
    w.show();

    return a.exec();
}