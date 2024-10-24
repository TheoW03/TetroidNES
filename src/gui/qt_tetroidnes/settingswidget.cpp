#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFileInfo>

#include <Qt/settingswidget.h>
#include <Qt/settingsmanager.h>
#include <Qt/util.h>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent}
{
    const int stretch_setting_display = 100;

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle("TetroidNES - " + tr("Settings"));

    auto *layout = new QVBoxLayout();
    auto *layout_controls = new QHBoxLayout();

    controls_frame = new QFrame(this);
    setting_category = new QListWidget(this);
    setting_display = new SettingsDisplay(this);
    button_statusbar = new QStatusBar(this);
    apply_changes = new QPushButton(tr("Apply"), button_statusbar);
    cancel_changes = new QPushButton(tr("Cancel"), button_statusbar);

    // setting_category setup
    QStringList items;
    items << "General" << "Emulator" << "About";
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
    connect(setting_category, &QListWidget::itemClicked, this,
            [this](QListWidgetItem *item)
            { on_setting_category_item_clicked(setting_category->row(item)); });
    connect(apply_changes, &QPushButton::clicked, this, &SettingsWidget::on_apply_changes_clicked);
    connect(cancel_changes, &QPushButton::clicked, this, &SettingsWidget::on_cancel_changes_clicked);
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::on_setting_category_item_clicked(const int index)
{
    setting_display->setCurrentIndex(index);
}

void SettingsWidget::on_apply_changes_clicked()
{

    QStringList string_list;
    QPlainTextEdit *rom_dir = setting_display->findChild<QPlainTextEdit *>("rom_directory");
    auto &settings = SettingsManager::instance();

    for (auto &string : rom_dir->toPlainText().split("\n"))
    {
        if (QFileInfo(string).isDir() && QFileInfo(string).isAbsolute())
        {
            string_list << string;
        }
    }

    settings.set_rom_dir(string_list);
    qInfo() << "saving settings in " << SAVE_DIR;
    QMessageBox::information(this, tr("Settings saved"),
                             tr("Your settings have been saved"));
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
        "TetroidNES - " + tr("Confirmation"),
        tr("Settings are unsaved!\nAre you sure you want to close the settings?"),
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