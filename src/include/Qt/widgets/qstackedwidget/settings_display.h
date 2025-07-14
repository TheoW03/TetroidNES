#pragma once

#include <QStackedWidget>

#include <Qt/widgets/qwidget/about.h>
#include <Qt/widgets/qwidget/emulator_settings_display.h>
#include <Qt/widgets/qwidget/general_settings_display.h>

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