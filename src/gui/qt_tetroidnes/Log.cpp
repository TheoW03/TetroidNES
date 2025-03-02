#include <QDebug>
#include <QDir>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

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

#include <Qt/Log_Display.h>

QtMessageHandler originalHandler = nullptr;

void check_log_dir()
{
    const short int max_size = 32;
    QStringList log_dir = QDir(QStringLiteral("logs"), QStringLiteral("*.txt"), QDir::Name, QDir::Files).entryList();
    auto log_dir_size = log_dir.length();

    if (log_dir_size > max_size)
    {
        auto logs_to_be_deleted = log_dir.sliced(0, log_dir_size - max_size);
        qDebug() << "Over" << max_size << "logs reached, deleting older logs:" << logs_to_be_deleted.join(", ");
        for (auto &file_name : logs_to_be_deleted)
        {
            if (!QFile::remove(QStringLiteral("logs/") + file_name))
            {
                qWarning() << "Could not remove log file:" << file_name;
            }
        }
    }
}
void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    LogNotifier &log_notifier = LogNotifier::instance();
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

    emit log_notifier.log_pushed(message);
}

void InitLogs()
{
    originalHandler = qInstallMessageHandler(logToFile);
    check_log_dir();
    qSetMessagePattern(QStringLiteral("%{type} | %{function}:%{line} | %{time dd/MM/yyyy h:mm:ss} | %{message}"));
}
