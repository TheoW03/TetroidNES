#include <settingsdisplay.h>

#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QFocusEvent>
#include <QEvent>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>

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
    QSettings settings = QSettings("config.cfg", QSettings::IniFormat);
    auto settings_rom_dirs = settings.value("rom_dirs", QStringList()).toStringList();

    QVBoxLayout *layout = new QVBoxLayout();
    // Search directories groupbox
    QGroupBox *directory_groupbox =          new QGroupBox(tr("Search Directories:"), general);
    QVBoxLayout *directory_groupbox_layout = new QVBoxLayout();
    QTextEdit *directories =                 new QTextEdit(directory_groupbox);
    QPushButton *add_directory =             new QPushButton(directory_groupbox);

    add_directory->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));

    directories->setAcceptRichText(false);
    directories->setObjectName("rom_directory");
    directories->setText(settings_rom_dirs.join("\n"));

    directory_groupbox_layout->addWidget(directories);
    directory_groupbox_layout->addWidget(add_directory);
    directory_groupbox_layout->setAlignment(add_directory, Qt::AlignLeft);
    directory_groupbox->setLayout(directory_groupbox_layout);

    layout->addWidget(directory_groupbox);
    general->setLayout(layout);

    connect(add_directory, &QPushButton::clicked, this, &SettingsDisplay::on_add_directory_clicked);

}

void SettingsDisplay::setup_emulator(QWidget *emulator)
{

}

void SettingsDisplay::setup_about(QWidget *about)
{

}

void SettingsDisplay::on_add_directory_clicked()
{
    QFileDialog file_dialog = QFileDialog();
    QTextEdit *text_edit = findChild<QTextEdit*>("rom_directory");
    
    file_dialog.setFileMode(QFileDialog::Directory);

    if (file_dialog.exec())
    {
        text_edit->append(file_dialog.selectedFiles().join("\n"));
    }
}

bool SettingsDisplay::eventFilter(QObject *obj, QEvent *ev)
{
    return QStackedWidget::eventFilter(obj, ev);
}