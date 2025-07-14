#include <Qt/widgets/qstackedwidget/settings_display.h>

#include <QVBoxLayout>
#include <QFocusEvent>
#include <QEvent>
#include <QFileDialog>

#include <Qt/utils/util.h>
#include <Qt/objects/settings_manager.h>

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