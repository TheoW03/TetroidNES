#include <settingsdisplay.h>

#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QFocusEvent>
#include <QEvent>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QCheckBox>

#include <QFileInfo>
#include <Qt/util.h>
#include <Qt/settingsmanager.h>

#define ABOUT_TEXT "dummy text"

SettingsDisplay::SettingsDisplay(QWidget *parent) : QStackedWidget{parent}
{
    general = new QWidget(this);
    emulator = new QWidget(this);
    about = new QWidget(this);

    setup_general(general);
    setup_emulator(emulator);
    setup_about(about);

    addWidget(general);
    addWidget(emulator);
    addWidget(about);

    setCurrentIndex(0);
}

SettingsDisplay::~SettingsDisplay()
{
}

void SettingsDisplay::setup_general(QWidget *general)
{
    SettingsManager &settings = SettingsManager::instance();
    const auto settings_rom_dirs = settings.get_rom_dirs();
    const auto settings_min_on_game_start = settings.minimize_gui_on_game_start();

    QVBoxLayout *layout = new QVBoxLayout();

    // General settings groupbox
    QGroupBox *general_groupbox =                   new QGroupBox(tr("General"), general);
    QVBoxLayout *general_groupbox_layout =          new QVBoxLayout();
    QCheckBox *general_min_on_game_start_checkbox = new QCheckBox(tr("Minimize GUI on game start"), general_groupbox);

    general_min_on_game_start_checkbox->setObjectName("min_gui_on_game_start");
    general_min_on_game_start_checkbox->setChecked(settings_min_on_game_start);

    general_groupbox_layout->addWidget(general_min_on_game_start_checkbox);
    general_groupbox->setLayout(general_groupbox_layout);

    // Search directories groupbox

    QGroupBox *directory_groupbox =          new QGroupBox(tr("Search Directories"), general);
    QVBoxLayout *directory_groupbox_layout = new QVBoxLayout();
    QPlainTextEdit *directories =            new QPlainTextEdit(directory_groupbox);
    QPushButton *add_directory =             new QPushButton(directory_groupbox);

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
    general->setLayout(layout);

    // Events
    connect(add_directory, &QPushButton::clicked, this, &on_add_directory_clicked);
    connect(general_min_on_game_start_checkbox, &QCheckBox::toggled, this, &on_min_gui_on_start_checkbox_toggled);
}

void SettingsDisplay::setup_emulator(QWidget *emulator)
{
}

void SettingsDisplay::setup_about(QWidget *about)
{
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *about_text = new QLabel(ABOUT_TEXT, about);
    about_text->setTextFormat(Qt::PlainText);

    // Layouts
    layout->addWidget(about_text);

    about->setLayout(layout);
}

void SettingsDisplay::on_add_directory_clicked()
{
    QFileDialog file_dialog;
    QPlainTextEdit *text_edit = findChild<QPlainTextEdit *>("rom_directory");

    file_dialog.setFileMode(QFileDialog::Directory);

    if (file_dialog.exec())
    {
        text_edit->appendPlainText(file_dialog.selectedFiles().join("\n"));
    }
}

void SettingsDisplay::on_min_gui_on_start_checkbox_toggled(const bool toggled)
{

}

bool SettingsDisplay::eventFilter(QObject *obj, QEvent *ev)
{
    return QStackedWidget::eventFilter(obj, ev);
}