#include <Qt/settingsmanager.h>

constexpr const auto key_romdir = "romdir";
constexpr const auto key_min_game_on_start = "minimize_gui_on_game_start";
constexpr const auto key_frame_rate = "frame_rate";
constexpr const auto key_sort_mode = "QOL/sort_mode";
constexpr const auto key_ascend_order = "QOL/ascending_order";

SettingsManager::SettingsManager()
{
}

QStringList SettingsManager::get_rom_dirs()
{
    return m_settings.value(key_romdir, QStringList()).toStringList();
}

void SettingsManager::set_rom_dir(QStringList dir)
{
    m_settings.setValue(key_romdir, dir);
}

void SettingsManager::set_rom_dir(QString dir)
{
    auto rom_dirs = get_rom_dirs();
    rom_dirs.append(dir);

    m_settings.setValue(key_romdir, rom_dirs);
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
    return m_settings.value(key_min_game_on_start, false).toBool();
}

void SettingsManager::set_minimize_gui_on_game_start(const bool b)
{
    return m_settings.setValue(key_min_game_on_start, b);
}

float SettingsManager::frame_rate() const
{
    return m_settings.value(key_frame_rate, 60.f).toFloat();
}

void SettingsManager::set_frame_rate(const float frame_rate)
{
    m_settings.setValue(key_frame_rate, frame_rate);
}

void SettingsManager::set_sort_mode(const RomList::SortMode sort_mode)
{
    m_settings.setValue(key_sort_mode, sort_mode);
}

RomList::SortMode SettingsManager::sort_mode() const
{
    return RomList::SortMode(m_settings.value(key_sort_mode, RomList::AZ).toInt());
}

Qt::SortOrder SettingsManager::ascending_order() const
{
    return Qt::SortOrder(m_settings.value(key_ascend_order, Qt::AscendingOrder).toInt());
}

void SettingsManager::set_ascending_order(const Qt::SortOrder sort_order)
{
    m_settings.setValue(key_ascend_order, sort_order);
}