#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDir>
#include <QCoreApplication>

constexpr const qsizetype QUEUE_MAX_SIZE = 10000;

static QtMessageHandler originalHandler = nullptr;

class LogNotifier : public QObject
{
    Q_OBJECT
public:
    static LogNotifier &instance()
    {
        static LogNotifier instance;
        return instance;
    }
    LogNotifier(const LogNotifier &) = delete;
    LogNotifier &operator=(const LogNotifier &) = delete;
signals:
    void log_pushed(const QString &message);
private slots:
    void on_cooldown_timeout()
    {

        write_to_file(queue);

        queue.clear();
    }

    void on_about_to_quit()
    {
        cooldown->stop();
        on_cooldown_timeout();
    }

private:
    QString queue;
    std::string current_file_path;

    QTimer *cooldown;

    LogNotifier()
    {
        const int WRITE_TO_FILE_INTERVAL_MS = 300;

        cooldown = new QTimer(this);
        cooldown->setInterval(WRITE_TO_FILE_INTERVAL_MS);
        cooldown->setSingleShot(true);

        queue.reserve(QUEUE_MAX_SIZE);

        init_logs();

        connect(cooldown, &QTimer::timeout, this, &LogNotifier::on_about_to_quit);
        connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &LogNotifier::on_about_to_quit);
    }

    void init_logs()
    {
        originalHandler = qInstallMessageHandler(push_log);
        check_log_dir();
        qSetMessagePattern(QStringLiteral("%{type} | %{function}:%{line} | %{time dd/MM/yyyy h:mm:ss} | %{message}"));
        current_file_path = QCoreApplication::applicationDirPath()
                                .toStdString() +
                            "/logs/log_" +
                            std::to_string(std::time(0)) +
                            ".txt";
    }

    static void push_log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        auto &instance = LogNotifier::instance();

        if (originalHandler)
        {
            originalHandler(type, context, msg);
        }

        if (instance.queue.size() + msg.size() >= QUEUE_MAX_SIZE)
        {
            instance.cooldown->stop();
            instance.on_cooldown_timeout();
        }

        if (!instance.cooldown->isActive())
        {
            instance.cooldown->start();
        }

        instance.queue.append(
            QString("%1%2")
                .arg(
                    qFormatLogMessage(type, context, msg),
                    QStringLiteral("\n")));
    }

    void write_to_file(const QString &msg)
    {

        static FILE *f = fopen(current_file_path.c_str(), "a");
        fprintf(f, "%s\n", qPrintable(msg));
        fflush(f);

        emit log_pushed(msg);
    }

    void check_log_dir()
    {
        const qsizetype MAX_FILES = 32;

        QDir log_dir(
            QCoreApplication::applicationDirPath() + QStringLiteral("/logs"),
            QStringLiteral("*.txt"),
            QDir::Name,
            QDir::Files);

        if (!log_dir.exists())
        {
            log_dir.mkdir(log_dir.absolutePath());
        }

        QStringList log_dir_contents = log_dir.entryList();
        qint64 log_dir_size = log_dir_contents.length();

        if (log_dir_size > MAX_FILES)
        {
            QStringList logs_to_be_deleted = log_dir_contents.sliced(0, log_dir_size - MAX_FILES);
            qDebug() << "Over" << MAX_FILES << "logs reached, deleting older logs:" << logs_to_be_deleted.join(", ");
            for (auto &file_name : logs_to_be_deleted)
            {
                if (!log_dir.remove(file_name))
                {
                    qWarning() << "Could not remove log file:" << file_name;
                }
            }
        }
    }
};