#include <Qt/settingsmanager.h>
#include "settingsmanager.h"

// Makes visible only the literal operators declared in StringLiterals
using namespace Qt::Literals::StringLiterals;

const auto key_romdir = u"romdir"_s;
const auto key_min_game_on_start = u"minimize_gui_on_game_start"_s;
const auto key_speed = u"emu_speed"_s;
const auto key_run_emulator_on_seperate_thread = u"run_emulator_on_seperate_thread"_s;
const auto key_crt_shader = u"crt_shader"_s;
const auto key_active_input_profile = u"active_input_profile"_s;
const auto key_sort_mode = u"QOL/sort_mode"_s;
const auto key_ascend_order = u"QOL/ascending_order"_s;
const auto key_recent_roms = u"QOL/recent_roms"_s;

constexpr const uint max_recent_roms = 10;

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

    emit rom_dirs_changed(dir);
}

void SettingsManager::set_rom_dir(QString dir)
{
    auto rom_dirs = get_rom_dirs();
    rom_dirs.append(dir);

    m_settings.setValue(key_romdir, rom_dirs);

    emit rom_dirs_changed(rom_dirs);
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

float SettingsManager::speed() const
{
    return m_settings.value(key_speed, 1.f).toFloat();
}

void SettingsManager::set_speed(const float speed)
{
    m_settings.setValue(key_speed, speed);
    emit speed_changed(speed);
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

bool SettingsManager::crt_shader() const
{
    return m_settings.value(key_crt_shader, false).toBool();
}

void SettingsManager::set_crt_shader(bool b)
{
    m_settings.setValue(key_crt_shader, b);
    emit crt_shader_changed(b);
}

void SettingsManager::set_recent_roms(QStringList dirs)
{
    if (dirs.length() > max_recent_roms)
    {
        dirs.resize(max_recent_roms);
    }

    dirs.removeDuplicates();

    m_settings.setValue(key_recent_roms, dirs);
    emit recent_roms_changed(dirs);
}

void SettingsManager::append_recent_roms(QString dir)
{
    auto recent_roms = get_recent_roms();
    recent_roms.append(dir);

    set_recent_roms(recent_roms);
}

QStringList SettingsManager::get_recent_roms()
{
    return m_settings.value(key_recent_roms, QStringList()).toStringList();
}

void SettingsManager::set_run_emulator_on_seperate_thread(bool b)
{
    m_settings.setValue(key_run_emulator_on_seperate_thread, b);
}

bool SettingsManager::run_emulator_on_seperate_thread() const
{
    return m_settings.value(key_run_emulator_on_seperate_thread, true).toBool();
}

QString SettingsManager::active_input_profile() const
{
    return m_settings.value(key_active_input_profile, u"default"_s).toString();
}

void SettingsManager::set_active_input_profile(QString name)
{
    m_settings.setValue(key_active_input_profile, name);
}