#pragma once

#include <QWidget>
#include <QComboBox>

#include <Qt/controllersettingsdisplay.h>

class InputSettings : public QWidget
{
    Q_OBJECT

public:
    explicit InputSettings(QWidget *parent = nullptr);
    ~InputSettings();
private:
    InputSettingsDisplay *display_widget;
    QWidget *header_widget;
    QComboBox *header_input_profiles;
    QComboBox *header_input_type;
private slots:
    void on_type_combobox_current_index_changed(int idx);
    void on_profile_editing_finished();
    void on_profile_combobox_current_index_changed(int idx);
};