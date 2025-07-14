#pragma once

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>

#include <Qt/objects/log_notifier.h>

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