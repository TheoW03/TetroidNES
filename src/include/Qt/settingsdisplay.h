#pragma once

#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

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

class EmulatorSettingsDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit EmulatorSettingsDisplay(QWidget *parent = nullptr);
    ~EmulatorSettingsDisplay();

    friend class SettingsDisplay;

private:
    QGroupBox *emulator_groupbox;
    QGroupBox *emu_speed_groupbox;
    QComboBox *speed_combobox;
    QCheckBox *threaded_checkbox;
    QCheckBox *crt_shader_checkbox;

};

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

class SettingsDisplay : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SettingsDisplay(QWidget *parent = nullptr);
    ~SettingsDisplay();

private:
    GeneralSettingsDisplay *general;
    EmulatorSettingsDisplay *emulator;
    About *about;

private slots:
    void on_add_directory_clicked();
    void on_min_gui_on_start_checkbox_toggled(const bool toggled);
    void on_speed_index_changed(const int idx);

};