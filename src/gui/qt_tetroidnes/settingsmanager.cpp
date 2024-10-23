#include <settingsmanager.h>

SettingsManager::SettingsManager()
{
}

QStringList SettingsManager::get_rom_dirs()
{
    return m_settings.value("romdir", QStringList()).toStringList();
}

void SettingsManager::set_rom_dir(QStringList dir)
{
    m_settings.setValue("romdir", dir);
}

void SettingsManager::set_rom_dir(QString dir)
{
    auto rom_dirs = get_rom_dirs();
    rom_dirs.append(dir);

    m_settings.setValue("romdir", rom_dirs);
}

void SettingsManager::set_save_dir(QString dir)
{
    m_settings.setPath(QSettings::IniFormat, QSettings::UserScope, dir);
}

QString SettingsManager::save_dir() const
{
    return m_settings.fileName();
}

bool SettingsManager::minimize_gui_on_game_start() const
{
    return m_settings.value("minimize_gui_on_game_start", false).toBool();
}

void SettingsManager::set_minimize_gui_on_game_start(const bool b)
{
    return m_settings.setValue("minimize_gui_on_game_start", b);
}

float SettingsManager::frame_rate() const
{
    return m_settings.value("frame_rate", 60.0).toFloat();
}

void SettingsManager::set_frame_rate(const float frame_rate)
{
    m_settings.setValue("frame_rate", frame_rate);
}