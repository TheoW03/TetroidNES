#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDir>

#include <Qt/objects/log_notifier.h>

static QtMessageHandler originalHandler = nullptr;

inline void check_log_dir()
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

inline void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const auto LOG_FILENAME = QStringLiteral(u"/logs");

    QString message = qFormatLogMessage(type, context, msg);
    auto log_dir = QDir(QCoreApplication::applicationDirPath() + LOG_FILENAME);

    if (!log_dir.exists())
    {
        log_dir.mkdir(log_dir.absolutePath());
    }

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

    emit LogNotifier::instance().log_pushed(message);
}

inline void InitLogs()
{
    originalHandler = qInstallMessageHandler(logToFile);
    check_log_dir();
    qSetMessagePattern(QStringLiteral("%{type} | %{function}:%{line} | %{time dd/MM/yyyy h:mm:ss} | %{message}"));
}