#include <menubar.h>
#include <QMenuBar>
#include <QAction>

MenuBar::MenuBar(QWidget *parent): QMenuBar{parent}
{

    // declare vars
    QAction *file;
    QAction *edit;
    QAction *tools;
    QAction *help;

    // assign vars
    file = addAction(tr("File"));
    edit = addAction(tr("Edit"));
    tools = addAction(tr("Tools"));
    help = addAction(tr("Help"));

    // setup
    setNativeMenuBar(true);
}

MenuBar::~MenuBar()
{
}
