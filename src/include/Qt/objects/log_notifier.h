#pragma once

#include <QObject>
#include <QString>

class LogNotifier : public QObject
{
    Q_OBJECT
public:
    static LogNotifier& instance()
    {
        static LogNotifier instance;
        return instance;
    }
    LogNotifier(const LogNotifier&) = delete;
    LogNotifier& operator=(const LogNotifier&) = delete;
signals:
    void log_pushed(QString message);
private:
    LogNotifier()
    {
    }
};