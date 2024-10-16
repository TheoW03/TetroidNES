#include <gamedisplay.h>
#include <QBuffer>

GameDisplay::GameDisplay(QWidget *parent) : QWidget{parent},
                                            sf::RenderWindow(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default),
                                            initialized(false)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setWindowTitle(tr("TetroidNES - %1").arg("Game name goes here!!!!"));

    setFocusPolicy(Qt::StrongFocus);

    QTimer *frame_timer = new QTimer(this);

    frame_timer->setInterval(frame_time);
}

void GameDisplay::on_init()
{
    QPixmap pixmap("C:\\Users\\tyler\\Downloads\\silly.jpg");
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG");
    buffer.close();
    // sf::GetLastError();
    texture.loadFromMemory(buffer.data().data(), buffer.data().size());
    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
    sprite.setPosition(200, 200);
}

void GameDisplay::on_update()
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    clear(sf::Color(128, 0, 0));
    sprite.rotate(0.0125);

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

QPaintEngine *GameDisplay::paintEngine() const
{
    return nullptr;
}

void GameDisplay::paintEvent(QPaintEvent *event)
{
    on_update();
    display();
}

GameDisplay::~GameDisplay()
{
}
