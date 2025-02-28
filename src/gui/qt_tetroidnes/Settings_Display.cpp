#include <settingsdisplay.h>

#include <QVBoxLayout>
#include <QFocusEvent>
#include <QEvent>
#include <QFileDialog>

#include <Qt/util.h>
#include <Qt/settingsmanager.h>

#define ABOUT_TEXT "dummy text" // TODO: Placeholder until we figure out how to store long strings of text, maybe using resources?
//
// QSTACKEDWIDGET CONTROLLER
//
SettingsDisplay::SettingsDisplay(QWidget *parent) : QStackedWidget{parent}
{
    general = new GeneralSettingsDisplay(this);
    emulator = new EmulatorSettingsDisplay(this);
    about = new About(this);

    addWidget(general);
    addWidget(emulator);
    addWidget(about);

    setCurrentIndex(0);

    // Events

    connect(general->add_directory, &QPushButton::clicked, this, &SettingsDisplay::on_add_directory_clicked);
    connect(general->min_on_game_start_checkbox, &QCheckBox::toggled, this, &SettingsDisplay::on_min_gui_on_start_checkbox_toggled);

    connect(emulator->speed_combobox, &QComboBox::currentIndexChanged, this, &SettingsDisplay::on_speed_index_changed);
}

SettingsDisplay::~SettingsDisplay()
{
}

void SettingsDisplay::on_speed_index_changed(const int idx)
{
}

void SettingsDisplay::on_add_directory_clicked()
{
    QFileDialog file_dialog;

    file_dialog.setFileMode(QFileDialog::Directory);

    if (file_dialog.exec())
    {
        general->directories->appendPlainText(file_dialog.selectedFiles().join("\n"));
    }
}

void SettingsDisplay::on_min_gui_on_start_checkbox_toggled(const bool toggled)
{
}
//
// GENERAL SETTINGS
//
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
    directories->setPlainText(settings_rom_dirs.join("\n"));
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
//
// EMULATOR SETTINGS
//
EmulatorSettingsDisplay::EmulatorSettingsDisplay(QWidget *parent) : QWidget{parent}
{
    SettingsManager &settings = SettingsManager::instance();
    const auto settings_speed = settings.speed();
    const auto settings_crt_shader = settings.crt_shader();
    const auto settings_run_on_dif_thread = settings.run_emulator_on_seperate_thread();
    const auto default_combobox_key = QString("100%");
    int speed_combobox_current_idx;

    auto *layout = new QVBoxLayout();
    auto *emulator_groupbox_layout = new QVBoxLayout();
    auto *emu_speed_groupbox_layout = new QVBoxLayout();

    // Emulator GroupBox
    emulator_groupbox = new QGroupBox(tr("Emulator"), this);

    // Threaded CheckBox
    threaded_checkbox = new QCheckBox(tr("Enable Threading"), this);
    threaded_checkbox->setObjectName("run_emu_in_different_thread");
    threaded_checkbox->setChecked(settings_run_on_dif_thread);

    // Crt shader CheckBox
    crt_shader_checkbox = new QCheckBox("CRT Filter", this);
    crt_shader_checkbox->setObjectName("crt_shader");
    crt_shader_checkbox->setChecked(settings_crt_shader);

    // Emu Speed GroupBox
    emu_speed_groupbox = new QGroupBox(tr("Emulation Speed"), this);

    // Emu speed ComboBox
    speed_combobox = new QComboBox(emu_speed_groupbox);
    speed_combobox->setObjectName("speed");
    speed_combobox->addItem("25%", QVariant(0.25f));
    speed_combobox->addItem("50%", QVariant(0.5f));
    speed_combobox->addItem("75%", QVariant(0.75f));
    speed_combobox->addItem(default_combobox_key, QVariant(1.f));
    speed_combobox->addItem("200%", QVariant(2.f));
    speed_combobox->addItem("400%", QVariant(4.f));
    speed_combobox->addItem("Unlimited", QVariant(0.f));

    // Check for invalid speed value in settings file
    speed_combobox_current_idx = speed_combobox->findData(QVariant(settings_speed));
    if (speed_combobox_current_idx == -1)
    {
        float default_combobox_value;

        qWarning()
            << "Emulation speed multiplier"
            << settings.speed()
            << "was not found in speed_combobox, setting to"
            << default_combobox_key;

        speed_combobox_current_idx = speed_combobox->findText(default_combobox_key);

        default_combobox_value = speed_combobox->itemData(speed_combobox_current_idx).toFloat();

        settings.set_speed(default_combobox_value); // Correct invalid speed value to default
    }
    speed_combobox->setCurrentIndex(speed_combobox_current_idx);

    // Emulator GroupBox Layout
    emulator_groupbox_layout->addWidget(threaded_checkbox);
    emulator_groupbox_layout->addWidget(crt_shader_checkbox);
    emulator_groupbox->setLayout(emulator_groupbox_layout);

    // Emulator Speed GroupBox Layout
    emu_speed_groupbox_layout->addWidget(speed_combobox);
    emu_speed_groupbox->setLayout(emu_speed_groupbox_layout);

    layout->addWidget(emulator_groupbox);
    layout->addWidget(emu_speed_groupbox);
    setLayout(layout);
}

EmulatorSettingsDisplay::~EmulatorSettingsDisplay()
{
}
//
// ABOUT
//
About::About(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout();

    text = new QLabel(ABOUT_TEXT, this);
    text->setTextFormat(Qt::PlainText); // PlainText is placeholder until we decide what text format we want

    layout->addWidget(text);

    setLayout(layout);
}

About::~About()
{
}