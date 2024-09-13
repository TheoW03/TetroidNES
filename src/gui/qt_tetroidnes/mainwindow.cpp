#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menubar.h"
#include "filtercontrolframe.h"

#include <QVBoxLayout>
#include <QIcon>
#include <QtLogging>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *widget_layout =             new QVBoxLayout();
    menubar *main_menubar =                  new menubar(this);
    filtercontrolframe *sort_control_frame = new filtercontrolframe(ui->centralwidget);
    romlist *rom_list =                      new romlist(ui->centralwidget);

    // widget layout
    widget_layout->addWidget(sort_control_frame);
    widget_layout->addWidget(rom_list);
    widget_layout->setAlignment(Qt::AlignTop);

    // central widget
    ui->centralwidget->setLayout(widget_layout);

    // setup
    setMenuBar(main_menubar);

    // events
    connect(sort_control_frame->findChild<QButtonGroup*>(), &QButtonGroup::idReleased, this,
            [this](int id){sort_mode_button_released(id);});
    connect(sort_control_frame->findChild<QPushButton*>("SortOrder"), &QPushButton::toggled, this,
            [this](bool toggled){sort_order_button_toggled(toggled);});
    connect(sort_control_frame->findChild<QLineEdit*>(), &QLineEdit::textEdited, this,
            [this](QString text){search_bar_edited(text);});
}

void MainWindow::sort_mode_button_released(int id)
{
    this->findChild<romlist*>()->set_current_mode(romlist::SortMode(id));
}

void MainWindow::sort_order_button_toggled(bool toggled)
{
    this->findChild<romlist*>()->set_current_order(Qt::SortOrder(toggled));
}

void MainWindow::search_bar_edited(QString string)
{
    this->findChild<romlist*>()->sort_regex(string + QString("*"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
