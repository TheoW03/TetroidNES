#include <Qt/widgets/qmainwindow/main_window.h>

#include <QVBoxLayout>
#include <QIcon>
#include <QtLogging>
#include <QScrollBar>
#include <QMimeData>
#include <QMessageBox>
#include <QEvent>
#include <QStatusBar>

#include <Qt/widgets/qframe/filter_control_frame.h>
#include <Qt/widgets/qwidget/game_display.h>
#include <Qt/objects/settings_manager.h>
#include <Qt/utils/util.h>

#include <Emulator/LoadRom.h>

constexpr const float slide_pos_multiplier = 0.1f;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          centralwidget(new QWidget(this)),
                                          main_menubar(new MenuBar(this)),
                                          sort_control_frame(new FilterControlFrame(centralwidget)),
                                          rom_list_scroll(new QScrollArea(centralwidget)),
                                          rom_list(new RomList(centralwidget)),
                                          page_info(new QLabel("Page 1 of 1", this))
{
    setAttribute(Qt::WA_AcceptDrops, true);
    setAttribute(Qt::WA_QuitOnClose, true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    resize(800, 600);

    auto *widget_layout = new QVBoxLayout();

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
    centralwidget->setLayout(widget_layout);

    // setup
    setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), qApp->applicationVersion()));
    setMenuBar(main_menubar);
    setCentralWidget(centralwidget);
    page_info->setObjectName("PageInfo");
    statusBar()->addPermanentWidget(page_info);
    update_page_info();

    // events
    connect(sort_control_frame->sort_mode_button_group, &QButtonGroup::idReleased, this, &MainWindow::sort_mode_button_released);
    connect(sort_control_frame->sort_ascending_button, &QPushButton::toggled, this, &MainWindow::sort_order_button_toggled);
    connect(sort_control_frame->search_bar, &QLineEdit::textEdited, this, &MainWindow::search_bar_edited);
    connect(rom_list_scroll->verticalScrollBar(), &QScrollBar::valueChanged, this, &MainWindow::rom_list_scroll_value_changed);
}

void MainWindow::create_display(QString rom_link)
{

    // std::shared_ptr<GameDisplay> display = std::make_shared<GameDisplay>(this, rom_link);
    std::optional<Rom> rom = load_rom(file_tobyte_vector(rom_link.toStdString()));
    if (!rom.has_value())
    {
        constexpr const auto err = "Not a INES v1.0 ROM.\n(You should see NES at the top of the file if it is a INES v1.0 ROM and be greater then in size)";
        qInfo() << err;
        QMessageBox::critical(
            this,
            QString("TetroidNES - %1").arg(tr("Error")),
            err
        );
        return;
    }
    auto *display = new GameDisplay(rom.value(), nullptr, rom_link);
    connect(display, &QWidget::destroyed, this, &MainWindow::on_gamedisplay_destroyed);

    display->show();

    if (SettingsManager::instance().minimize_gui_on_game_start())
    {
        setWindowState(Qt::WindowMinimized);
    }
}

void MainWindow::on_gamedisplay_destroyed()
{
    if (isMinimized() && !is_a_game_running())
    {
        showNormal();
    }
}

void MainWindow::update_page_info()
{
    page_info->setText(
        QString("%1 %2 %3 %4 | %5 %6").arg(
            tr("Page"),
            QString::number(rom_list->current_page()),
            tr("of", "Something out of something"),
            QString::number(rom_list->total_pages()),
            tr("Items displayed:"),
            QString::number(rom_list->items_per_page())
        )
    );
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

    auto *scrollbar = rom_list_scroll->verticalScrollBar();

    if (scrollbar->isVisible())
    {
        return;
    }

    const bool scrolled_up = (event->angleDelta().y() > 0);
    const int current_page = rom_list->current_page();
    const int total_pages = rom_list->total_pages();
    const int min = scrollbar->minimum();
    int max = scrollbar->maximum();

    if (!scrolled_up && current_page < total_pages)
    {
        rom_list->set_current_page(current_page + 1);
        qApp->processEvents(); // Makes sure scroll bar updates max/min values
        max = scrollbar->maximum();
        scrollbar->setSliderPosition(min + static_cast<int>(max * slide_pos_multiplier));
    }
    else if (scrolled_up && current_page > 1)
    {
        rom_list->set_current_page(current_page - 1);
        qApp->processEvents(); // Makes sure scroll bar updates max/min values
        max = scrollbar->maximum();
        scrollbar->setSliderPosition(max - static_cast<int>(max * slide_pos_multiplier));
    }
    update_page_info();

    QMainWindow::wheelEvent(event);
}

void MainWindow::rom_list_scroll_value_changed(const int value)
{
    auto *scrollbar = rom_list_scroll->verticalScrollBar();
    const auto current_page = rom_list->current_page();
    const int total_pages = rom_list->total_pages();
    const int min = scrollbar->minimum();
    int max = scrollbar->maximum();
    // qDebug()  << "Current Page Before:" << current_page
    //           << "Value:" << value
    //           << "Max/Min Value:" << scrollbar->maximum() << "/" << scrollbar->minimum()
    //           << "Total pages:" << rom_list->total_pages();

    if (value >= max && current_page < total_pages)
    {
        // qDebug() << "Going up from page" << current_page << "to" << current_page + 1;

        rom_list->set_current_page(current_page + 1);
        qApp->processEvents(); // Makes sure scroll bar updates max/min values
        max = scrollbar->maximum();
        scrollbar->setSliderPosition(min + static_cast<int>(max * slide_pos_multiplier));
        update_page_info();
    }
    else if (value <= min && current_page > 1)
    {
        // qDebug() << "Going down from page" << current_page << "to" << current_page - 1;

        rom_list->set_current_page(current_page - 1);
        qApp->processEvents(); // Makes sure scroll bar updates max/min values
        max = scrollbar->maximum();
        scrollbar->setSliderPosition(max - static_cast<int>(max * slide_pos_multiplier));
        update_page_info();
    }
    // qDebug() << "Current Page After:" << current_page;
}

void MainWindow::sort_mode_button_released(const int id) const
{
    const auto sort_mode = SortMode(id);
    auto &settings = SettingsManager::instance();
    QString search_bar_text = sort_control_frame->search_bar->text();
    const bool regex = !search_bar_text.isEmpty();

    if (sort_mode == settings.sort_mode())
    {
        return;
    }

    rom_list->set_current_mode(sort_mode, regex);
    settings.set_sort_mode(sort_mode);

    if (regex)
    {
        rom_list->search(search_bar_text);
    }
}

void MainWindow::sort_order_button_toggled(const bool toggled) const
{
    const auto sort_order = Qt::SortOrder(!toggled);

    rom_list->set_current_order(sort_order);
    SettingsManager::instance().set_ascending_order(sort_order);
}

void MainWindow::search_bar_edited(QString string) const
{
    rom_list->search(string);
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

    if (url.isValid() && url.isLocalFile() && url.toLocalFile().endsWith(QStringLiteral(".nes")))
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
    const bool game_run = is_a_game_running();
    qInfo() << "Quitting... games are running: " << game_run;
    if (game_run)
    {
        int message_box_result = QMessageBox::question(
            this,
            QString("TetroidNES - %1").arg(tr("Confirmation")),
            tr("Are you sure you want to quit? (Games are still running)"),
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
        {
            qDebug() << "nul";
        }
        qDebug() << "closing window";

        event->accept();
    }
}

MainWindow::~MainWindow()
{
}
