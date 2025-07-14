#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QPushButton>

class GeneralSettingsDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettingsDisplay(QWidget *parent = nullptr);
    ~GeneralSettingsDisplay();

    friend class SettingsDisplay;

private:
    QGroupBox *general_groupbox;
    QCheckBox *min_on_game_start_checkbox;

    QGroupBox *directory_groupbox;
    QPlainTextEdit *directories;
    QPushButton *add_directory;

};