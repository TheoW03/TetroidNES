#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

#include <Qt/util.h>
#include <Qt/romlist.h>

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

    float frame_rate() const;
    void set_frame_rate(const float frame_rate);

    RomList::SortMode sort_mode() const;
    void set_sort_mode(const RomList::SortMode sort_mode);

    Qt::SortOrder ascending_order() const;
    void set_ascending_order(const Qt::SortOrder sort_order);

private:
    SettingsManager();
    QSettings m_settings = QSettings(SAVE_DIR, QSettings::IniFormat, this);
};

#endif // SETTINGSMANAGER_H