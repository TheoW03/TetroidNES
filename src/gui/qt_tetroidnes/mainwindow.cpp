#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menubar.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // declare vars
    QPushButton *mainbutton;
    QLabel *label;
    QVBoxLayout *widget_layout;
    menubar *main_menubar;

    // assign vars
    mainbutton = new QPushButton(tr("Click Me!"), ui->centralwidget);
    widget_layout = new QVBoxLayout();
    label = new QLabel(ui->centralwidget);
    main_menubar = new menubar(this);

    // widget_layout
    widget_layout->addWidget(mainbutton);
    widget_layout->addWidget(label);

    // central widget
    ui->centralwidget->setLayout(widget_layout);

    // img label
    label->setPixmap(QPixmap(":/resources/images/icon.jpg"));
    label->setVisible(false);

    // setup
    this->setMenuBar(main_menubar);

    // events
    connect(mainbutton, &QPushButton::clicked, this,
            [=](void){MainWindow::buttonPressed(mainbutton, label);}
    );
}

void MainWindow::buttonPressed(QPushButton *button, QLabel *label)
{
    button->setVisible(false);
    label->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
