#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filtercontrolframe.h"

#include <QVBoxLayout>
#include <QIcon>
#include <QtLogging>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *widget_layout =             new QVBoxLayout();
    MenuBar *main_menubar =                  new MenuBar(this);
    FilterControlFrame *sort_control_frame = new FilterControlFrame(ui->centralwidget);
    QScrollArea *rom_list_scroll =           new QScrollArea(ui->centralwidget);
    RomList *rom_list =                      new RomList(ui->centralwidget);

    // widget layout
    widget_layout->addWidget(sort_control_frame);
    widget_layout->addWidget(rom_list_scroll);
    widget_layout->setAlignment(Qt::AlignTop);

    // rom list scroll bar
    rom_list_scroll->setWidgetResizable(true);
    rom_list_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rom_list_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    rom_list_scroll->setWidget(rom_list);

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

void MainWindow::sort_mode_button_released(const int id) const
{
    RomList *list = ui->centralwidget->findChild<RomList*>();
    QString search_bar_text = ui->centralwidget->findChild<QLineEdit*>()->text();
    const bool regex = search_bar_text.isEmpty();

    list->set_current_mode(RomList::SortMode(id), regex);
    if(!regex)
    {list->search(search_bar_text);}

}

void MainWindow::sort_order_button_toggled(const bool toggled) const
{
    this->findChild<RomList*>()->set_current_order(Qt::SortOrder(toggled));
}

void MainWindow::search_bar_edited(QString string) const
{
    //if (!string.isEmpty())
    //{string += QString("*");}

    this->findChild<RomList*>()->search(string);
}

MainWindow::~MainWindow()
{
    delete ui;
}
