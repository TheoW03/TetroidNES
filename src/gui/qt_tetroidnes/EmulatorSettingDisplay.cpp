#include <Qt/emulatorsettingsdisplay.h>

#include <QVBoxLayout>

#include <Qt/settingsmanager.h>

EmulatorSettingsDisplay::EmulatorSettingsDisplay(QWidget *parent) : QWidget{parent}
{
    SettingsManager &settings = SettingsManager::instance();
    const auto settings_speed = settings.speed();
    const auto settings_crt_shader = settings.crt_shader();
    const auto settings_run_on_dif_thread = settings.run_emulator_on_seperate_thread();
    const auto default_combobox_key = QStringLiteral("100%");
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
    speed_combobox->addItem(QStringLiteral("25%"), QVariant(0.25f));
    speed_combobox->addItem(QStringLiteral("50%"), QVariant(0.5f));
    speed_combobox->addItem(QStringLiteral("75%"), QVariant(0.75f));
    speed_combobox->addItem(default_combobox_key, QVariant(1.f));
    speed_combobox->addItem(QStringLiteral("200%"), QVariant(2.f));
    speed_combobox->addItem(QStringLiteral("400%"), QVariant(4.f));
    speed_combobox->addItem(QStringLiteral("Unlimited"), QVariant(0.f));

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