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
    void append_line(const QString &line);

private:
    QPlainTextEdit *text_display;

};