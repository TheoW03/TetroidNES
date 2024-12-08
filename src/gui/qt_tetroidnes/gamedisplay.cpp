#include <gamedisplay.h>

#include <QCoreApplication>
#include <QtLogging>
#include <QUrl>
#include <QMessageBox>

#include <Qt/util.h>
#include <Qt/settingsmanager.h>

#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>

constexpr const unsigned int rgb_data_size = NES_RES_A * 4;
// TODO: Class is hard-coded to work only for NTSC, make a way to use both
constexpr const float ntsc_frame_rate = 60.0f;
constexpr const float pal_frame_rate = 50.0f;

GameDisplay::GameDisplay(QWidget *parent, QString rom_url) : QWidget{parent},
                                                             render_window(new sf::RenderWindow(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default)),
                                                             frame_timer(new QChronoTimer(this)),
                                                             frames_per_sec_timer(new QTimer(this)),
                                                             m_rom_url(rom_url)
{
    SettingsManager &settings = SettingsManager::instance();

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::Window);
    resize(800, 600); // TODO: MAKE THIS MORE FLEXIBLE IN SETTINGS

    setWindowTitle(QString("%1 - %2").arg(
        qApp->applicationName(),
        QUrl(rom_url).fileName().split(".").front() // Should probably clean this up
        ));

    setFocusPolicy(Qt::StrongFocus);

    set_frame_time(settings.speed());
    frame_timer->setTimerType(Qt::PreciseTimer);

    frames_per_sec_timer->setInterval(1000);
    frames_per_sec_timer->setTimerType(Qt::PreciseTimer);

    // Events
    connect(frame_timer, &QChronoTimer::timeout, this, &GameDisplay::on_timeout);
    connect(frames_per_sec_timer, &QTimer::timeout, this, &GameDisplay::on_framerate_timer_timeout);
    connect(&settings, &SettingsManager::speed_changed, this, &GameDisplay::set_frame_time);
}

void GameDisplay::on_init()
{
    initializeInstructionMap();
    auto rom = load_rom(file_tobyte_vector(m_rom_url.toStdString()));
    frame_timer->start();
    frames_per_sec_timer->start();
    if (rom.has_value() == 0)
    {
        qCritical() << "unrecongnized file format needs to be NES v1.0 format";
        this->err_code = EXIT_FAILURE;
        QMessageBox::critical(this,
                              "TetroidNES - " + tr("Error"),
                              "wrong NES file format");
        this->close();
        return;
    }

    Bus bus = Bus(rom.value(), NES_START);
    CPU cpu = CPU();
    bus.fill(bus.read_16bit(0xfffc));
    printf("0x%x\n", bus.get_PC());

    cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;
    cpu.interrupt = {};

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
    // for (int i = 0; i < 100; i++)
    // {
    auto result = exe.run();

    // printf("0x%x\n", result.bus.get_PC());

    // this->cpu = result;
    if (result.error_code == EXIT_FAILURE)
    {
        // this->exe.log_Cpu();
        qInfo() << "potential error with the cpu";
        auto err_mess = QString::fromStdString(result.bus.check_error().value());
        QMessageBox::critical(this,
                              "TetroidNES - " + tr("Error"),
                              (err_mess) + "-- at PC addr= 0x" + QString::fromStdString(num_to_hexa(result.bus.get_PC())));
        err_code = EXIT_FAILURE;
        close();
        return;

        // TODO: close error and log the CPU stats
    }

    //
    // Generate next frame
    auto rgb_data_vector = exe.render();
    uint8_t rgb_data[rgb_data_size];
    std::copy(rgb_data_vector.begin(), rgb_data_vector.end(), rgb_data);

    // Display next frame
    render_window->clear();
    texture.update(rgb_data);
    render_window->draw(sprite);

    frames_within_second += 1;
    // }
}

void GameDisplay::on_timeout()
{
    on_update();
    render_window->display();
}

void GameDisplay::on_framerate_timer_timeout()
{
    setWindowTitle(
        QString("Speed: %%1 | FPS: %2").arg(QString::number(speed_percent(ntsc_frame_rate)), QString::number(frames_within_second)));
    frames_within_second = 0;
}

void GameDisplay::showEvent(QShowEvent *event)
{
    // Initial initialization of the SFML widget
    if (!m_initialized)
    {
        // Create an SFML window for rendering with the id of the window in which the drawing will be done
        render_window->create(sf::WindowHandle(winId()));
        // Initializing drawing objects
        if (!m_rom_url.isEmpty())
        {
            on_init();
        }
        else
        {
            qInfo() << "Started game display without url, will not initialize";
        }

        m_initialized = true;
    }
}

void GameDisplay::closeEvent(QCloseEvent *event)
{

    if (!m_initialized)
    {
        render_window->close();

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
        qInfo() << "exiting game " << QUrl(m_rom_url).fileName();
        exe.log_Cpu();

        if (this->err_code == EXIT_SUCCESS)
        {
            qInfo() << "CPU exited successfully";
        }
        else
        {
            qInfo() << "CPU exited unsuccessfully";
        }
        render_window->close();
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

std::chrono::nanoseconds GameDisplay::frame_time() const
{
    return frame_timer->interval();
}

void GameDisplay::set_frame_time(const float speed)
{
    const auto ns = framerate_to_ns(ntsc_frame_rate * speed);
    frame_timer->setInterval(ns);
}

void GameDisplay::center_display()
{
}

bool GameDisplay::initialized() const { return m_initialized; }

int GameDisplay::speed_percent(const float frame_rate) const
{
    return static_cast<int>(
        static_cast<float>(frames_within_second) / frame_rate * 100.f);
}

QPaintEngine *GameDisplay::paintEngine() const
{
    return nullptr;
}

void GameDisplay::resizeEvent(QResizeEvent *event)
{
    update_game_scale();
}

GameDisplay::~GameDisplay()
{
}
