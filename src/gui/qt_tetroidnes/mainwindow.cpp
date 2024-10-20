#include <mainwindow.h>
#include <filtercontrolframe.h>
#include <gamedisplay.h>
#include "ui_mainwindow.h"
#include <util.h>

#include <QVBoxLayout>
#include <QIcon>
#include <QtLogging>
#include <QScrollBar>
#include <QMimeData>
#include <QMessageBox>
#include <qevent.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_AcceptDrops, true);
    setAttribute(Qt::WA_QuitOnClose, true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto *widget_layout = new QVBoxLayout();
    main_menubar =        new MenuBar(this);
    sort_control_frame =  new FilterControlFrame(ui->centralwidget);
    rom_list_scroll =     new QScrollArea(ui->centralwidget);
    rom_list =            new RomList(ui->centralwidget);
    page_info =           new QLabel("Page 1 of 1", this);

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
    setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), qApp->applicationVersion()));
    setMenuBar(main_menubar);
    page_info->setObjectName("PageInfo");
    statusBar()->addPermanentWidget(page_info);
    update_page_info();

    // events
    connect(sort_control_frame->findChild<QButtonGroup *>(), &QButtonGroup::idReleased, this,
            [this](int id)
            { sort_mode_button_released(id); });
    connect(sort_control_frame->findChild<QPushButton *>("SortOrder"), &QPushButton::toggled, this,
            [this](bool toggled)
            { sort_order_button_toggled(toggled); });
    connect(sort_control_frame->findChild<QLineEdit *>(), &QLineEdit::textEdited, this,
            [this](QString text)
            { search_bar_edited(text); });
    connect(rom_list_scroll->verticalScrollBar(), &QScrollBar::valueChanged, this,
            [this](int val)
            { rom_list_scroll_value_changed(val); });
}

void MainWindow::create_display(QString rom_link)
{
    // std::shared_ptr<GameDisplay> display = std::make_shared<GameDisplay>(this, rom_link);
    auto *display = new GameDisplay(this, rom_link);
    // installEventFilter(display.get());
    // installEventFilter(display);
    display->show();
}

void MainWindow::update_page_info()
{
    RomList *list = findChild<RomList *>();

    findChild<QLabel *>("PageInfo")->setText(tr("%1 %2 %3 %4 | %5 %6").arg("Page", QString::number(list->current_page()), "of", QString::number(list->total_pages()), "Items displayed:", QString::number(list->items_per_page())));
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

    auto *list = this->findChild<RomList *>();
    auto *scrollbar = this->findChild<QScrollArea *>()->verticalScrollBar();
    const bool scrolled_up = (event->angleDelta().y() > 0);

    if (scrollbar->isVisible())
    {
        return;
    }

    if (!scrolled_up && list->current_page() < list->total_pages())
    {
        list->set_current_page(list->current_page() + 1);
        scrollbar->setSliderPosition(scrollbar->minimum() + scrollbar->singleStep());
    }
    else if (scrolled_up && list->current_page() > 1)
    {
        list->set_current_page(list->current_page() - 1);
        scrollbar->setSliderPosition(scrollbar->maximum() - scrollbar->singleStep());
    }
    update_page_info();

    QMainWindow::wheelEvent(event);
}

void MainWindow::rom_list_scroll_value_changed(const int value)
{
    auto *list = findChild<RomList *>();
    auto *scrollbar = findChild<QScrollArea *>()->verticalScrollBar();
    unsigned int current_page = list->current_page();
    // qDebug() << "Current Page Before:" << current_page
    //          << "Value:" << value
    //          << "Max/Min Value:" << scrollbar->maximum() << "/" << scrollbar->minimum()
    //          << "Total pages:" << list->total_pages();

    if (value >= scrollbar->maximum() && current_page < list->total_pages())
    {
        list->set_current_page(current_page + 1);
        scrollbar->setSliderPosition(scrollbar->minimum() + scrollbar->singleStep());
        update_page_info();
    }
    else if (value <= scrollbar->minimum() && current_page > 1)
    {
        list->set_current_page(current_page - 1);
        scrollbar->setSliderPosition(scrollbar->maximum() - scrollbar->singleStep());
        update_page_info();
    }
    // qDebug() << "Current Page After:" << current_page;
}

void MainWindow::sort_mode_button_released(const int id) const
{
    RomList *list = ui->centralwidget->findChild<RomList *>();
    QString search_bar_text = ui->centralwidget->findChild<QLineEdit *>()->text();
    const bool regex = search_bar_text.isEmpty();

    list->set_current_mode(RomList::SortMode(id), regex);
    if (!regex)
    {
        list->search(search_bar_text);
    }
}

void MainWindow::sort_order_button_toggled(const bool toggled) const
{
    findChild<RomList *>()->set_current_order(Qt::SortOrder(toggled));
}

void MainWindow::search_bar_edited(QString string) const
{
    findChild<RomList *>()->search(string);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    auto mime_data = event->mimeData();
    QUrl url;
    if (mime_data->hasUrls() && !mime_data->urls().isEmpty())
    {
        url = mime_data->urls()[0];
    }
    else
    {
        qDebug() << "Drag enter event data does not have urls or is empty";
        event->setDropAction(Qt::DropAction::IgnoreAction);
        return;
    }

    if (url.isValid() && url.isLocalFile() && url.toLocalFile().endsWith(".nes"))
    {
        qDebug() << "Drag enter event data is a valid QUrl:" << url.toLocalFile();
        event->setDropAction(Qt::DropAction::MoveAction);
        event->accept();
    }
    else
    {
        qDebug() << "Drag enter event data is not a valid QUrl:" << url;
        event->setDropAction(Qt::DropAction::IgnoreAction);
    }

    QMainWindow::dragEnterEvent(event);
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->setDropAction(Qt::DropAction::MoveAction);
    event->accept();
    QMainWindow::dragMoveEvent(event);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    create_display(event->mimeData()->urls()[0].toLocalFile());
    QMainWindow::dropEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool game_run = is_a_game_running();
    qInfo() << "Quitting... games are running: " << game_run;
    if (game_run)
    {
        int message_box_result = QMessageBox::question(
            this,
            tr("TetroidNES - Confirmation"),
            tr("Are you sure you want to quit?\n(Games are still running)"),
            QMessageBox::Yes | QMessageBox::No);

        if (message_box_result == QMessageBox::No)
        {
            event->ignore();
        }
        else
        {
            event->accept();
        }
    }
    else
    {
        if (event == nullptr)
            std::cout << "nul" << std::endl;
        qDebug() << "closing window";

        event->accept();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
