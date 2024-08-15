#include "menubar.h"
#include <QMenuBar>
#include <QAction>

menubar::menubar(QWidget *parent): QMenuBar{parent}
{

    // declare vars
    QAction *file;
    QAction *edit;
    QAction *tools;
    QAction *help;

    // assign vars
    file = this->addAction(tr("File"));
    edit = this->addAction(tr("Edit"));
    tools = this->addAction(tr("Tools"));
    help = this->addAction(tr("Help"));

    // setup
    this->setNativeMenuBar(true);
}

menubar::~menubar()
{
}
