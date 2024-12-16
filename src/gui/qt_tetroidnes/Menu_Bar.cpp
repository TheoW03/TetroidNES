#include <Qt/menubar.h>
#include <Qt/mainwindow.h>
#include <Qt/settingswidget.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QKeySequence>

#include <Qt/settingsmanager.h>

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent},
                                    file(addMenu(tr("File"))),
                                    file_open(file->addAction(tr("Open..."))),
                                    file_open_recent(file->addMenu(tr("Open Recent"))),
                                    edit(addMenu(tr("Edit"))),
                                    settings_open(edit->addAction(tr("Settings"))),
                                    tools(addMenu(tr("Tools"))),
                                    help(addMenu(tr("Help")))
{
    auto &settings = SettingsManager::instance();

    // setup
    setNativeMenuBar(true);
    refresh_recent_roms(settings.get_recent_roms());

    // setup menus
    // file
    file_open->setShortcut(QKeySequence("Ctrl+O"));

    // edit
    settings_open->setShortcut(QKeySequence("Ctrl+B"));

    // events
    connect(file_open, &QAction::triggered, this, &MenuBar::open_rom);
    connect(settings_open, &QAction::triggered, this, &MenuBar::open_settings);
    connect(&settings, &SettingsManager::recent_roms_changed, this, &MenuBar::refresh_recent_roms);
}

void MenuBar::open_settings()
{
    SettingsWidget *settings = new SettingsWidget(qobject_cast<MainWindow *>(parent()));
    settings->show();
}

void MenuBar::refresh_recent_roms(QStringList dirs)
{
    file_open_recent->clear();

    for (auto &s : dirs)
    {
        auto url = QUrl(s);
        auto *action = new QAction(url.fileName(), file_open_recent);
        action->setData(QVariant(s));

        connect(action, &QAction::triggered, this, [this, action](){start_rom(action->data().toString());});

        file_open_recent->addAction(action);
    }
}

void MenuBar::open_rom()
{
    auto file_dialog = QFileDialog(
        nullptr,
        tr("Choose ROM to open..."),
        QString(),
        QString("NES ROM (*.nes)"));
    file_dialog.setFileMode(QFileDialog::ExistingFile);

    if (file_dialog.exec() == QFileDialog::Accepted && !file_dialog.selectedUrls().isEmpty())
    {
        
        for (auto &url : file_dialog.selectedUrls())
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
