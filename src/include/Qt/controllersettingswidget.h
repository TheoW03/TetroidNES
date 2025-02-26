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
    QPushButton *header_edit_profile_name;
    QPushButton *header_add_profile;
    QPushButton *header_remove_profile;

private slots:
    void update_type_combobox();
    void update_add_remove_buttons();
    void refresh_profiles_selection();
    void on_add_profile_clicked();
    void on_remove_profile_clicked();
    void on_edit_profile_clicked();
    void on_type_combobox_activated(int idx);
    void on_profile_combobox_activated(int idx);
};