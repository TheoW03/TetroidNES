#include <gamedisplay.h>

#include <QIODevice>
#include <QCoreApplication>
#include <QtLogging>

#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>

GameDisplay::GameDisplay(QWidget *parent) : QWidget{parent},
                                            sf::RenderWindow(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), "Game name goes here!!!!"));

    setFocusPolicy(Qt::StrongFocus);

    QTimer *frame_timer = new QTimer(this);

    frame_timer->setInterval(frame_time);
}

void GameDisplay::on_init()
{
    initializeInstructionMap();
    Rom rom = load_rom(file_tobyte_vector("Test.nes"));
    Bus bus = Bus(rom, NES_START);
    bus.fill(bus.read_16bit(0xfffc));
    cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;

    texture.create(NES_RES_L, NES_RES_W);

    sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
    sprite.setPosition(256, 240);
    sprite.setTexture(texture);
    update_game_scale();

    exe = Execute(cpu);
}

void GameDisplay::on_update()
{
    // auto frame = exe.render();
    auto rgb_data_vector = cpu.bus.render_texture({NES_RES_L, NES_RES_W});
    uint8_t rgb_data[NES_RES_A * 4];
    std::copy(rgb_data_vector.begin(), rgb_data_vector.end(), rgb_data);
    clear();
    texture.update(rgb_data);
    // printf("in run ======");

    CPU result = exe.run();
    if (a.error_code == EXIT_FAILURE)
    {
        // TODO: close error and log the CPU stats
    }

    draw(sprite);
}

void GameDisplay::on_timeout()
{
    repaint();
}

void GameDisplay::showEvent(QShowEvent *event)
{
    // Initial initialization of the SFML widget
    if (!initialized)
    {
        // Create an SFML window for rendering with the id of the window in which the drawing will be done
        RenderWindow::create(sf::WindowHandle(winId()));

        // Initializing drawing objects
        on_init();

        // Setting the timer to restart the widget's rendering
        connect(&frame_timer, &QTimer::timeout, this, &GameDisplay::on_timeout);
        frame_timer.start();

        initialized = true;
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
