#pragma once

#include <QObject>
#include <QSettings>

#include <Qt/utils/util.h>

class SettingsManager : public QObject
{
    Q_OBJECT

public:

    static SettingsManager& instance()
    {
        static SettingsManager instance;
        return instance;
    }

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

    QStringList get_rom_dirs();
    void set_rom_dir(QStringList dir);
    void set_rom_dir(QString dir);

    void set_save_dir(QString dir);
    QString save_dir() const;

    bool minimize_gui_on_game_start() const;
    void set_minimize_gui_on_game_start(const bool b);

    float speed() const;
    void set_speed(const float speed);

    bool run_emulator_on_seperate_thread() const;
    void set_run_emulator_on_seperate_thread(bool b);

    SortMode sort_mode() const;
    void set_sort_mode(const SortMode sort_mode);

    Qt::SortOrder ascending_order() const;
    void set_ascending_order(const Qt::SortOrder sort_order);

    bool crt_shader() const;
    void set_crt_shader(bool b);

    QStringList get_recent_roms();
    void append_recent_roms(QString dir);
    void set_recent_roms(QStringList dirs);

    QString active_input_profile() const;
    void set_active_input_profile(QString name);

signals:
    void crt_shader_changed(bool on);
    void speed_changed(float speed);
    void recent_roms_changed(QStringList dirs);
    void rom_dirs_changed(QStringList dirs);

private:
    SettingsManager();
    QSettings m_settings = QSettings(SAVE_DIR, QSettings::IniFormat, this);
};