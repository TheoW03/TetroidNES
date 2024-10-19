#include <menubar.h>
#include <mainwindow.h>

#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QKeySequence>

MenuBar::MenuBar(QWidget *parent): QMenuBar{parent}
{

    // declare vars
    QMenu *file;
    QAction *file_open;
    QMenu *edit;
    QMenu *tools;
    QMenu *help;

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
    

    // events
    connect(file_open, &QAction::triggered, this, &MenuBar::open_rom);
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
        MainWindow *mw = static_cast<MainWindow*>(parent());
        foreach(QUrl url, file_dialog.selectedUrls())
        {
            mw->create_display(url.toLocalFile());
        }
        
    }
}

MenuBar::~MenuBar()
{
}
