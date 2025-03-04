#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFileInfo>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include <Qt/settingswidget.h>
#include <Qt/settingsmanager.h>
#include <Qt/util.h>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent}
{
    constexpr const int stretch_setting_display = 100;

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle(QString("TetroidNES - %1").arg(tr("Settings")));

    auto *layout = new QVBoxLayout();
    auto *layout_controls = new QHBoxLayout();

    controls_frame = new QFrame(this);
    setting_category = new QListWidget(this);
    setting_display = new SettingsDisplay(this);
    button_statusbar = new QStatusBar(this);
    apply_changes = new QPushButton(tr("Apply"), button_statusbar);
    cancel_changes = new QPushButton(tr("Cancel"), button_statusbar);

    // setting_category setup
    QStringList items({
        QStringLiteral("General"),
        QStringLiteral("Emulator"),
        QStringLiteral("About")
    });
    setting_category->setSelectionMode(QListWidget::SingleSelection);
    setting_category->addItems(items);
    setting_category->item(0)->setSelected(true);

    // Buttons setup
    apply_changes->setObjectName("apply");
    cancel_changes->setObjectName("cancel");

    // Status bar setup
    button_statusbar->addPermanentWidget(apply_changes);
    button_statusbar->addPermanentWidget(cancel_changes);

    // Apply layouts
    layout_controls->addWidget(setting_category);
    layout_controls->addWidget(setting_display, stretch_setting_display);
    controls_frame->setLayout(layout_controls);

    layout->addWidget(controls_frame);
    layout->addWidget(button_statusbar);
    setLayout(layout);

    // Events
    connect(setting_category, &QListWidget::itemClicked, this, &SettingsWidget::on_setting_category_item_clicked);
    connect(apply_changes, &QPushButton::clicked, this, &SettingsWidget::on_apply_changes_clicked);
    connect(cancel_changes, &QPushButton::clicked, this, &SettingsWidget::on_cancel_changes_clicked);
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::on_setting_category_item_clicked(const QListWidgetItem *item)
{
    const int category_index = setting_category->row(item);
    setting_display->setCurrentIndex(category_index);
}

void SettingsWidget::on_apply_changes_clicked()
{

    auto &settings = SettingsManager::instance();
    auto rom_dir = setting_display->findChild<QPlainTextEdit *>("rom_directory");
    auto min_gui_on_game_start = setting_display->findChild<QCheckBox *>("min_gui_on_game_start");
    auto speed = setting_display->findChild<QComboBox *>("speed");
    auto threaded = setting_display->findChild<QCheckBox *>("run_emu_in_different_thread");
    auto crt_shader = setting_display->findChild<QCheckBox *>("crt_shader");

    // ROM Directories
    QStringList string_list;

    for (auto &string : rom_dir->toPlainText().split(QStringLiteral("\n")))
    {
        if (QFileInfo(string).isDir() && QFileInfo(string).isAbsolute())
        {
            string_list << string;
        }
    }

    if (string_list != settings.get_rom_dirs())
    {
        settings.set_rom_dir(string_list);
    }

    // Minimize GUI on game start
    const bool min_gui_on_game_start_checked = min_gui_on_game_start->isChecked();
    if (min_gui_on_game_start_checked != settings.minimize_gui_on_game_start())
    {
        settings.set_minimize_gui_on_game_start(min_gui_on_game_start_checked);
    }

    // Game speed multiplier
    const float speed_multipler = speed->currentData().toFloat();
    if (speed_multipler != settings.speed())
    {
        settings.set_speed(speed_multipler);
    }

    // Emulator threading mode
    const bool is_threaded = threaded->isChecked();
    settings.set_run_emulator_on_seperate_thread(is_threaded);

    // Emulator crt shader
    const bool is_crt_shader_on = crt_shader->isChecked();
    settings.set_crt_shader(is_crt_shader_on);

    // Finished saving settings
    qInfo() << "saving settings in " << SAVE_DIR;
    QMessageBox::information(
        this, 
        tr("Settings saved"),
        tr("Your settings have been saved")
    );
}

void SettingsWidget::on_cancel_changes_clicked()
{
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
    if (!apply_changes->isEnabled())
    {
        QWidget::closeEvent(event);
        qInfo() << "You didnt save your settings ";
        return;
    }

    int message_box_result = QMessageBox::question(
        this,
        QString("TetroidNES - %1").arg(tr("Confirmation")),
        QString("%1\n%2").arg(
            tr("Settings are unsaved!"),
            tr("Are you sure you want to close the settings?")
        ),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (message_box_result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}