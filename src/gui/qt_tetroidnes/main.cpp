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
#include <QDir>

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

void check_log_dir()
{
    const short int max_size = 32;
    QStringList log_dir = QDir("logs", "*.txt", QDir::Name, QDir::Files).entryList();
    auto log_dir_size = log_dir.length();

    if (log_dir_size > max_size)
    {
        auto logs_to_be_deleted = log_dir.sliced(0, log_dir_size - max_size);
        qDebug() << "Over" << max_size << "logs reached, deleting older logs:" << logs_to_be_deleted;
        for(auto &file_name : logs_to_be_deleted)
        {
            if(!QFile::remove("logs/" + file_name))
            {
                qWarning() << "Could not remove log file:" << file_name;
            }
        }
        
    }
}

int main(int argc, char *argv[])
{
    originalHandler = qInstallMessageHandler(logToFile);
    qSetMessagePattern("%{type} | %{function}:%{line} | %{time h:mm:ss.zzz} | %{message}");

    check_log_dir();

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