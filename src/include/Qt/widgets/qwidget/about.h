#pragma once

#include <QWidget>
#include <QLabel>

class About : public QWidget
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

    friend class SettingsDisplay;

private:
    QLabel *text;

};