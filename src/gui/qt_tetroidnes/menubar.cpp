#include <Qt/menubar.h>
#include <Qt/mainwindow.h>
#include <Qt/settingswidget.h>

#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QKeySequence>

MenuBar::MenuBar(QWidget *parent): QMenuBar{parent}
{
    // assign vars
    file = addMenu(tr("File"));
    edit = addMenu(tr("Edit"));
    tools = addMenu(tr("Tools"));
    help = addMenu(tr("Help"));

    // setup
    setNativeMenuBar(true);

    // setup menus
    // file
    file_open = file->addAction("Open");
    file_open->setShortcut(QKeySequence("Ctrl+O"));

    // edit
    settings_open = edit->addAction("Settings");
    settings_open->setShortcut(QKeySequence("Ctrl+B"));
    

    // events
    connect(file_open, &QAction::triggered, this, &MenuBar::open_rom);
    connect(settings_open, &QAction::triggered, this, &MenuBar::open_settings);
}

void MenuBar::open_settings()
{
    SettingsWidget *settings = new SettingsWidget(qobject_cast<MainWindow*>(parent()));
    settings->show();
}

void MenuBar::open_rom()
{
    QFileDialog file_dialog = QFileDialog(
        nullptr,
        tr("Choose ROM to open..."),
        QString(),
        QString("NES ROM (*.nes)")
    );
    file_dialog.setFileMode(QFileDialog::ExistingFile);

    if (file_dialog.exec() == QFileDialog::Accepted && !file_dialog.selectedUrls().isEmpty())
    {
        MainWindow *mw = qobject_cast<MainWindow*>(parent());
        for(auto &url : file_dialog.selectedUrls())
        {
            mw->create_display(url.toLocalFile());
        }
        
    }
}

MenuBar::~MenuBar()
{
}
