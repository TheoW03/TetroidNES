#include <gamedisplay.h>

#include <QIODevice>
#include <QCoreApplication>
#include <QtLogging>
#include <QUrl>
#include <QMessageBox>
#include <Qt/util.h>
#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>

GameDisplay::GameDisplay(QWidget *parent, QString rom_url) : QWidget{parent},
                                                             sf::RenderWindow(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    // TODO: MAKE THIS MORE FLEXIBLE WHEN WE ADD USER SETTINGS
    setWindowFlag(Qt::WindowType::Window);
    resize(800, 600);

    setWindowTitle(QString("%1 - %2").arg(
        qApp->applicationName(),
        QUrl(rom_url).fileName().split(".").front() // Should probably clean this up
        ));

    setFocusPolicy(Qt::StrongFocus);

    m_rom_url = rom_url;
    QTimer *frame_timer = new QTimer(this);

    frame_timer->setInterval(frame_time);
}

void GameDisplay::on_init()
{
    initializeInstructionMap();
    Rom rom = load_rom(file_tobyte_vector(m_rom_url.toStdString()));
    Bus bus = Bus(rom, NES_START);
    CPU cpu = CPU();
    bus.fill(bus.read_16bit(0xfffc));
    printf("0x%x\n", bus.get_PC());

    cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;
    err_code = EXIT_SUCCESS;
    std::cout << "err_code: " << cpu.status.val << std::endl;

    if (!texture.create(NES_RES_L, NES_RES_W))
    {
        qCritical() << "Texture failed to be created!";
    }

    sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
    sprite.setTexture(texture);
    update_game_scale();
    qInfo() << "Started game " << QUrl(m_rom_url).fileName();

    exe = Execute(cpu);
}

void GameDisplay::on_update()
{
    // Process CPU
    // this->cpu = exe.run();
    auto result = exe.run();

    // printf("0x%x\n", result.bus.get_PC());

    // this->cpu = result;
    if (result.error_code == EXIT_FAILURE)
    {
        // this->exe.log_Cpu();
        qInfo() << "potential error with the cpu";
        QMessageBox::critical(this,
                              "TetroidNES - " + tr("Error"),
                              tr("potential error with the CPU"));
        this->err_code = EXIT_FAILURE;
        this->QWidget::close();
        // this->sf::Window::close();

        return;

        // TODO: close error and log the CPU stats
    }

    // Generate next frame
    auto rgb_data_vector = exe.render();
    uint8_t rgb_data[NES_RES_A * 4];
    std::copy(rgb_data_vector.begin(), rgb_data_vector.end(), rgb_data);

    // Display next frame
    clear();
    texture.update(rgb_data);
    draw(sprite);
}

void GameDisplay::on_timeout()
{
    repaint();
}

void GameDisplay::showEvent(QShowEvent *event)
{
    // Initial initialization of the SFML widget
    if (!m_initialized)
    {
        // Create an SFML window for rendering with the id of the window in which the drawing will be done
        RenderWindow::create(sf::WindowHandle(winId()));
        // Initializing drawing objects
        if (!m_rom_url.isEmpty())
        {
            on_init();
        }
        else
        {
            qInfo() << "Started game display without url, will not initialize";
        }

        // Setting the timer to restart the widget's rendering
        connect(&frame_timer, &QTimer::timeout, this, &GameDisplay::on_timeout);
        frame_timer.start();

        m_initialized = true;
    }
}

void GameDisplay::closeEvent(QCloseEvent *event)
{

    if (!m_initialized)
    {
        this->sf::Window::close();

        event->accept();
        return;
    }

    int message_box_result = QMessageBox::question(
        this,
        "TetroidNES - " + tr("Confirmation"),
        tr("Are you sure you want to quit?") + "\n" + tr("(Remember to save before quitting!)"),
        QMessageBox::Yes | QMessageBox::No);

    if (message_box_result == QMessageBox::No)
    {
        event->ignore();
    }
    else
    {
        exe.log_Cpu();
        if (this->err_code == EXIT_SUCCESS)
        {
            qInfo() << "CPU exited successfully";
        }
        else
        {
            qInfo() << "CPU exited unsuccessfully";
        }
        this->sf::Window::close();
        event->accept();
    }
}

void GameDisplay::update_game_scale()
{
    QSize widget_size = size();
    sf::Vector2u texture_size = texture.getSize();
    sprite.setScale(
        static_cast<float>(widget_size.width()) / texture_size.x,
        static_cast<float>(widget_size.height()) / texture_size.y);
}

void GameDisplay::center_display()
{
}

bool GameDisplay::initialized() const { return m_initialized; }

QPaintEngine *GameDisplay::paintEngine() const
{
    return nullptr;
}

void GameDisplay::paintEvent(QPaintEvent *event)
{
    on_update();
    display();
}

void GameDisplay::resizeEvent(QResizeEvent *event)
{
    update_game_scale();
}

GameDisplay::~GameDisplay()
{
}
