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
#include <QFileInfo>
#include <Qt/util.h>
#include <Qt/settingsmanager.h>

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
    const auto settings_rom_dirs = SettingsManager::instance().get_rom_dirs();

    QVBoxLayout *layout = new QVBoxLayout();

    // Search directories groupbox

    QGroupBox *directory_groupbox = new QGroupBox(tr("Search Directories:"), general);
    QVBoxLayout *directory_groupbox_layout = new QVBoxLayout();
    QPlainTextEdit *directories = new QPlainTextEdit(directory_groupbox);
    QPushButton *add_directory = new QPushButton(directory_groupbox);

    add_directory->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));
    add_directory->setToolTip(tr("Add directory for TetroidNES to search for ROMs in"));

    directories->setObjectName("rom_directory");
    directories->setPlainText(settings_rom_dirs.join("\n"));
    directories->setToolTip(tr("Add/Remove/Edit directories for TetroidNES to search for ROMs in"));

    directory_groupbox_layout->addWidget(directories);
    directory_groupbox_layout->addWidget(add_directory);
    directory_groupbox_layout->setAlignment(add_directory, Qt::AlignLeft);
    directory_groupbox->setLayout(directory_groupbox_layout);

    // Layouts
    layout->addWidget(directory_groupbox);
    general->setLayout(layout);

    // Events
    connect(add_directory, &QPushButton::clicked, this, &SettingsDisplay::on_add_directory_clicked);
}

void SettingsDisplay::setup_emulator(QWidget *emulator)
{
}

void SettingsDisplay::setup_about(QWidget *about)
{
    QVBoxLayout *layout = new QVBoxLayout();

    QTextEdit *about_text = new QTextEdit("Dummy text", about);
    about_text->setReadOnly(true);

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

bool SettingsDisplay::eventFilter(QObject *obj, QEvent *ev)
{
    return QStackedWidget::eventFilter(obj, ev);
}