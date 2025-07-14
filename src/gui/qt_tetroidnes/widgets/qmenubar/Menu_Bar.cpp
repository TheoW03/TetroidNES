#include <Qt/widgets/qmenubar/menubar.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QKeySequence>

#include <Qt/objects/settings_manager.h>
#include <Qt/widgets/qwidget/log_display.h>
#include <Qt/widgets/qmainwindow/main_window.h>
#include <Qt/widgets/qwidget/settings_widget.h>
#include <Qt/widgets/qwidget/input_settings.h>

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent},
                                    file(addMenu(tr("File"))),
                                    file_open(file->addAction(tr("Open..."))),
                                    file_open_recent(file->addMenu(tr("Open Recent"))),
                                    edit(addMenu(tr("Edit"))),
                                    settings_open(edit->addAction(tr("Settings"))),
                                    input_settings_open(edit->addAction(tr("Controllers"))),
                                    tools(addMenu(tr("Tools"))),
                                    log_display_open(tools->addAction(tr("Open Log Display"))),
                                    help(addMenu(tr("Help")))
{
    auto &settings = SettingsManager::instance();
    const QKeyCombination file_open_combo(Qt::ControlModifier, Qt::Key_O);
    const QKeyCombination settings_open_combo(Qt::ControlModifier, Qt::Key_C);
    const QKeyCombination input_settings_open_combo(Qt::ControlModifier, Qt::Key_B);

    // setup
    setNativeMenuBar(true);
    refresh_recent_roms(settings.get_recent_roms());

    // setup menus
    // file
    file_open->setShortcut(QKeySequence(file_open_combo));

    // edit
    settings_open->setShortcut(QKeySequence(settings_open_combo));
    input_settings_open->setShortcut(QKeySequence(input_settings_open_combo));

    // tools
    log_display_open->setShortcut(QKeySequence(Qt::Key_F8));

    // events
    connect(file_open, &QAction::triggered, this, &MenuBar::open_rom);
    connect(settings_open, &QAction::triggered, this, &MenuBar::open_settings);
    connect(input_settings_open, &QAction::triggered, this, &MenuBar::open_input_settings);
    connect(log_display_open, &QAction::triggered, this, &MenuBar::open_log_display);
    connect(&settings, &SettingsManager::recent_roms_changed, this, &MenuBar::refresh_recent_roms);
}

void MenuBar::open_settings()
{
    SettingsWidget *settings = new SettingsWidget(qobject_cast<MainWindow *>(parent()));
    settings->show();
}

void MenuBar::open_input_settings()
{
    auto *input_settings = new InputSettings(qobject_cast<MainWindow *>(parent()));
    input_settings->show();
}

void MenuBar::open_log_display()
{
    LogDisplay *log_display = new LogDisplay(qobject_cast<MainWindow *>(parent()));
    log_display->show();
}

void MenuBar::refresh_recent_roms(QStringList dirs)
{
    file_open_recent->clear();

    for (auto &s : dirs)
    {
        auto url = QUrl(s);
        auto *action = new QAction(url.fileName(), file_open_recent);
        action->setData(QVariant(s));

        connect(action, &QAction::triggered, this, [this, action]()
                { start_rom(action->data().toString()); });

        file_open_recent->addAction(action);
    }
}

void MenuBar::open_rom()
{
    auto file_dialog = QFileDialog(
        nullptr,
        tr("Choose ROM to open..."),
        QStringLiteral(""),
        QStringLiteral("NES ROM (*.nes)"));
    file_dialog.setFileMode(QFileDialog::ExistingFile);

    const auto selected_urls = file_dialog.selectedUrls();

    if (file_dialog.exec() == QFileDialog::Accepted && !selected_urls.isEmpty())
    {

        for (auto &url : selected_urls)
        {
            SettingsManager::instance().append_recent_roms(url.toLocalFile());
            start_rom(url);
        }
    }
}

void MenuBar::start_rom(QUrl url)
{
    MainWindow *mw = qobject_cast<MainWindow *>(parent());
    mw->create_display(url.toLocalFile());
}

void MenuBar::start_rom(QString url)
{
    MainWindow *mw = qobject_cast<MainWindow *>(parent());
    mw->create_display(url);
}

MenuBar::~MenuBar()
{
}
