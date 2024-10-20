#include <settings.h>

#include <QMessageBox>
#include <QSettings>
#include <QTextEdit>
#include <QFileInfo>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent}
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle(tr("TetroidNES - Settings"));

    layout =           new QVBoxLayout();
    layout_controls =  new QHBoxLayout();
    controls_frame =   new QFrame(this);
    setting_category = new QListWidget(this);
    setting_display =  new SettingsDisplay(this);
    button_statusbar = new QStatusBar(this);
    apply_changes =    new QPushButton(tr("Apply"), button_statusbar);
    cancel_changes =   new QPushButton(tr("Cancel"), button_statusbar);

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
    layout_controls->addWidget(setting_display, 100);
    controls_frame->setLayout(layout_controls);

    layout->addWidget(controls_frame);
    layout->addWidget(button_statusbar);
    setLayout(layout);

    // Events
    connect(setting_category, &QListWidget::itemClicked, this,
        [this](QListWidgetItem *item){on_setting_category_item_clicked(setting_category->row(item));}
    );
    connect(apply_changes, &QPushButton::clicked, this, &SettingsWidget::on_apply_changes_clicked);
    connect(cancel_changes, &QPushButton::clicked, this, &SettingsWidget::on_cancel_changes_clicked);

}

SettingsWidget::~SettingsWidget()
{

}

void SettingsWidget::on_setting_category_item_clicked(const int index)
{
    findChild<SettingsDisplay*>()->setCurrentIndex(index);
}

void SettingsWidget::on_apply_changes_clicked()
{

    QStringList string_list;
    QTextEdit *rom_dir = findChild<QTextEdit*>("rom_directory");
    QSettings settings = QSettings("config.cfg", QSettings::IniFormat);
    
    for (auto &string : rom_dir->toPlainText().split("\n"))
    {
        if (QFileInfo(string).isDir() && QFileInfo(string).isAbsolute())
        {
            string_list << string;
        }
    }

    settings.setValue("rom_dirs", string_list);
}

void SettingsWidget::on_cancel_changes_clicked()
{

}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
    if (!findChild<QPushButton*>("apply")->isEnabled())
    {
        QWidget::closeEvent(event);
        return;
    }

    int message_box_result = QMessageBox::question(
        this,
        tr("TetroidNES - Confirmation"),
        tr("Settings are unsaved!\nAre you sure you want to close the settings?"),
        QMessageBox::Yes | QMessageBox::Cancel
    );

    if (message_box_result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}