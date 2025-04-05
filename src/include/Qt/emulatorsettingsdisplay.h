#pragma once

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckbox>

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