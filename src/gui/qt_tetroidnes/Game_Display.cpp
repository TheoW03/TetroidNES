#include <Qt/gamedisplay.h>

#include <QCoreApplication>
#include <QtLogging>
#include <QUrl>
#include <QMessageBox>
#include <QMutexLocker>
#include <QFile>

#include <Qt/Emulator_Worker.h>
#include <Qt/util.h>
#include <Qt/utilemulator.h>
#include <Qt/settingsmanager.h>

constexpr const unsigned int rgb_data_size = NES_RES_A * 4;

GameDisplay::GameDisplay(QWidget *parent, QString rom_url) : QWidget{parent},
                                                             render_window(new sf::RenderWindow(sf::VideoMode({800, 600}), "OpenGL", sf::Style::Default)),
                                                             frames_per_sec_timer(new QTimer(this)),
                                                             time_between_draw_timer(new QTimer(this)),
                                                             m_paused(false),
                                                             emu_worker(new EmulatorWorker(rom_url, mutex, m_paused)),
                                                             crt_shader(new sf::Shader()),
                                                             err_code(0),
                                                             texture({NES_RES_L, NES_RES_W}),
                                                             sprite(texture)
{
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

    SettingsManager &settings = SettingsManager::instance();
    m_is_emu_on_dif_thread = settings.run_emulator_on_seperate_thread();

    frames_per_sec_timer->setInterval(1000);
    frames_per_sec_timer->setTimerType(Qt::PreciseTimer);

    time_between_draw_timer->setInterval(1);

    QAction *pause_toggle_key = new QAction(this);
    pause_toggle_key->setShortcut(QKeySequence(Qt::Key_G));

    if(m_is_emu_on_dif_thread)
    {
        emu_worker->moveToThread(&emu_thread);
    }
    else
    {
        emu_worker->setParent(this); // Auto-sets a QObject's thread to the parent's
    }

    // Events
    connect(pause_toggle_key, &QAction::triggered, this, &GameDisplay::on_pause_toggle_key_triggered);
    connect(&settings, &SettingsManager::crt_shader_changed, this, &GameDisplay::on_crt_shader_changed);
    connect(emu_worker, &EmulatorWorker::draw_frame, this, &GameDisplay::on_update);
    connect(emu_worker, &EmulatorWorker::push_error, this, &GameDisplay::on_push_error);
    connect(&emu_thread, &QThread::started, emu_worker, &EmulatorWorker::on_start_main_thread);
    connect(frames_per_sec_timer, &QTimer::timeout, this, &GameDisplay::on_framerate_timer_timeout);
    connect(time_between_draw_timer, &QTimer::timeout, this, [this]()
            { time_between_draw_ms += 1; });

}

void GameDisplay::on_crt_shader_changed(const bool b)
{
    if (b)
    {
        draw_func = [this](sf::Drawable &drawable){render_window->draw(drawable, crt_shader.get());};
    }
    else
    {
        draw_func = [this](sf::Drawable &drawable){render_window->draw(drawable);};
    }
}

void GameDisplay::on_pause_toggle_key_triggered()
{
    if (is_paused())
    {
        mutex.lock();
        pause_game();
    }
    else
    {
        mutex.unlock();
        if (m_is_emu_on_dif_thread)
        {
            emu_worker->start_frame_timer();
        }

    }
}

bool GameDisplay::is_paused() const
{
    return m_paused;
}

void GameDisplay::pause_game()
{
    m_paused = true;
    if(!m_is_emu_on_dif_thread)
    {
        emu_worker->stop_frame_timer();
    }
}

void GameDisplay::on_push_error(QString msg, int error_code)
{
    mutex.lock();
    pause_game();
    QMessageBox::critical(
        this,
        "TetroidNES - " + tr("Error"),
        msg);
    mutex.unlock();

    this->err_code = error_code;

    qCritical() << msg;

    close();
}

void GameDisplay::on_init()
{
    qInfo() << "Initializing game window...";

    sf::Vector2i texture_rect_size = sprite.getTextureRect().size;

    sprite.setOrigin({(float)(texture_rect_size.x) / 2, (float)(texture_rect_size.y) / 2});
    sprite.setTexture(texture);
    update_game_scale();

    qInfo() << "About to start thread...";
    if (m_is_emu_on_dif_thread)
    {
        emu_thread.start();
    }
    else
    {
        emu_worker->on_start_main_thread();
    }

    frames_per_sec_timer->start();
    time_between_draw_timer->start();
}

void GameDisplay::on_update(std::vector<uint8_t> rgb_data_vector)
{
    uint8_t rgb_data[rgb_data_size];
    std::copy(rgb_data_vector.begin(), rgb_data_vector.end(), rgb_data);

    // Display next frame
    render_window->clear();
    texture.update(rgb_data);
    draw_func(sprite);

    render_window->display();

    frame_count += 1;
    //qDebug() << "Milliseconds from previous draw call:" << time_between_draw_ms;
    time_between_draw_ms = 0;
}

void GameDisplay::on_framerate_timer_timeout()
{
    setWindowTitle(
        QString("Speed: %%1 | FPS: %2").arg(QString::number(speed_percent(frame_count, ntsc_frame_rate)), QString::number(frame_count)));
    frame_count = 0;
}

void GameDisplay::showEvent(QShowEvent *event)
{
    // Initial initialization of the SFML widget
    if (!m_initialized)
    {

        // Create an SFML window for rendering with the id of the window in which the drawing will be done
        render_window->create(sf::WindowHandle(winId()));

        // Setup shader component
        auto shader_qfile = QFile(":/shaders/crt_shader.frag");
        if (!shader_qfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            on_push_error(QString("Failed loading shader file from QFile"), EXIT_FAILURE);
            return;
        }

        QTextStream in(&shader_qfile);
        std::string shader_text = in.readAll().toStdString();

        if (!crt_shader->loadFromMemory(shader_text, sf::Shader::Type::Fragment))
        {
            on_push_error(QString("Failed loading shader from memory"), EXIT_FAILURE);
            return;
        }

        on_crt_shader_changed(SettingsManager::instance().crt_shader());

        // Initializing drawing objects
        on_init();

        m_initialized = true;
    }
}

void GameDisplay::close_game()
{
    emu_worker->shutdown_game();
    render_window->close();
    if (m_is_emu_on_dif_thread)
    {
        emu_thread.quit();
        emu_thread.wait();
    }

}

void GameDisplay::closeEvent(QCloseEvent *event)
{

    if (!m_initialized)
    {

        close_game();

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
        close_game();

        if (err_code == EXIT_SUCCESS)
        {
            qInfo() << "CPU exited successfully";
        }
        else
        {
            qInfo() << "CPU exited unsuccessfully";
        }
        event->accept();
    }
}

void GameDisplay::update_game_scale()
{
    QSize widget_size = size();
    sf::Vector2u texture_size = sprite.getTexture().getSize();
    sprite.setScale(
        {static_cast<float>(widget_size.width()) / texture_size.x,
        static_cast<float>(widget_size.height()) / texture_size.y});
}

void GameDisplay::center_display()
{
}

bool GameDisplay::initialized() const { return m_initialized; }

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
