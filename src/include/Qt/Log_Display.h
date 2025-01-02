#pragma once

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>

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

class LogDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit LogDisplay(QWidget *parent = nullptr);

public slots:
    void append_line(QString line);

private:
    LogNotifier &log_notifier;
    QPlainTextEdit *text_display;

};