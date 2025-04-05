#pragma once

#include <QStackedWidget>

#include <Qt/generalsettingsdisplay.h>
#include <Qt/about.h>
#include <Qt/emulatorsettingsdisplay.h>

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