#include <Qt/generalsettingsdisplay.h>

#include <QVBoxLayout>

#include <Qt/settingsmanager.h>

GeneralSettingsDisplay::GeneralSettingsDisplay(QWidget *parent) : QWidget{parent}
{
    SettingsManager &settings = SettingsManager::instance();
    const auto settings_rom_dirs = settings.get_rom_dirs();
    const auto settings_min_on_game_start = settings.minimize_gui_on_game_start();

    QVBoxLayout *layout = new QVBoxLayout();

    // General settings groupbox
    general_groupbox = new QGroupBox(tr("General"), this);
    QVBoxLayout *general_groupbox_layout = new QVBoxLayout();
    min_on_game_start_checkbox = new QCheckBox(tr("Minimize GUI on game start"), general_groupbox);

    min_on_game_start_checkbox->setObjectName("min_gui_on_game_start");
    min_on_game_start_checkbox->setChecked(settings_min_on_game_start);

    general_groupbox_layout->addWidget(min_on_game_start_checkbox);
    general_groupbox->setLayout(general_groupbox_layout);

    // Search directories groupbox

    directory_groupbox = new QGroupBox(tr("Search Directories"), this);
    QVBoxLayout *directory_groupbox_layout = new QVBoxLayout();
    directories = new QPlainTextEdit(directory_groupbox);
    add_directory = new QPushButton(directory_groupbox);

    add_directory->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));
    add_directory->setToolTip(tr("Add directory for TetroidNES to search for ROMs in"));

    directories->setObjectName("rom_directory");
    directories->setPlainText(settings_rom_dirs.join(QStringLiteral("\n")));
    directories->setToolTip(tr("Add/Remove/Edit directories for TetroidNES to search for ROMs in"));

    directory_groupbox_layout->addWidget(directories);
    directory_groupbox_layout->addWidget(add_directory);
    directory_groupbox_layout->setAlignment(add_directory, Qt::AlignLeft);
    directory_groupbox->setLayout(directory_groupbox_layout);

    // Layout
    layout->addWidget(general_groupbox);
    layout->addWidget(directory_groupbox);
    setLayout(layout);
}

GeneralSettingsDisplay::~GeneralSettingsDisplay()
{
}