#include <Qt/gamedisplay.h>

#include <QCoreApplication>
#include <QRegularExpression>
#include <QtLogging>
#include <QUrl>
#include <QMessageBox>
#include <Emulator/LoadRom.h>
#include <QFile>

#include <Qt/Emulator_Worker.h>
#include <Qt/util.h>
#include <Qt/utilemulator.h>
#include <Qt/settingsmanager.h>

using namespace std::literals::string_literals;

constexpr const uint32_t rgb_data_size = NES_RES_A * 4;

GameDisplay::GameDisplay(Rom rom, QWidget *parent, QString rom_url) : QWidget{parent},
                                                                      render_window(new sf::RenderWindow(sf::VideoMode({800, 600}), "OpenGL", sf::State::Windowed)),
                                                                      texture({NES_RES_L, NES_RES_W}),
                                                                      frames_per_sec_timer(new QTimer(this)),
                                                                      time_between_draw_timer(new QTimer(this)),
                                                                      m_paused(false),
                                                                      sprite(new sf::Sprite(texture)),
                                                                      emu_thread(new QThread(this)),
                                                                      emu_worker(new EmulatorWorker(rom, rom_url)),
                                                                      crt_shader(new sf::Shader()),
                                                                      err_code(0),
                                                                      frame_count(0),
                                                                      time_between_draw_ms(0)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::Window);
    resize(800, 600); // TODO: MAKE THIS MORE FLEXIBLE IN SETTINGS

    game_title = QString::fromStdString(
        std::filesystem::path(rom_url.toStdString())
            .filename()
            .replace_extension()
            .string());

    setWindowTitle(QString("%1 - %2").arg(
        qApp->applicationName(),
        game_title));

    setFocusPolicy(Qt::StrongFocus);

    SettingsManager &settings = SettingsManager::instance();
    m_is_emu_on_dif_thread = settings.run_emulator_on_seperate_thread();

    frames_per_sec_timer->setInterval(1000);
    frames_per_sec_timer->setTimerType(Qt::PreciseTimer);

    time_between_draw_timer->setInterval(1);

    if (m_is_emu_on_dif_thread)
    {
        emu_worker->moveToThread(emu_thread);
    }
    else
    {
        emu_worker->setParent(this); // Auto-sets a QObject's thread to the parent's
    }

    // Events
    connect(&settings, &SettingsManager::crt_shader_changed, this, &GameDisplay::on_crt_shader_changed);
    connect(emu_worker, &EmulatorWorker::draw_frame, this, &GameDisplay::on_update);
    connect(emu_worker, &EmulatorWorker::push_error, this, &GameDisplay::on_push_error);
    connect(&settings, &SettingsManager::speed_changed, emu_worker, &EmulatorWorker::set_clock_interval_speed);
    connect(this, &GameDisplay::pause_toggle, emu_worker, &EmulatorWorker::on_pause_toggle);
    connect(emu_thread, &QThread::started, emu_worker, &EmulatorWorker::on_start_main_thread);
    connect(emu_thread, &QThread::finished, emu_worker, &EmulatorWorker::deleteLater);
    connect(frames_per_sec_timer, &QTimer::timeout, this, &GameDisplay::on_framerate_timer_timeout);
    connect(time_between_draw_timer, &QTimer::timeout, this, [this]()
            { time_between_draw_ms++; });
}

void GameDisplay::on_crt_shader_changed(const bool b)
{
    if (b)
    {
        draw_func = [this](sf::Drawable &drawable)
        {
            crt_shader->setUniform("time", (float)(time_between_draw_ms) * 0.001f);

            render_window->draw(drawable, crt_shader.get());
        };
    }
    else
    {
        draw_func = [this](sf::Drawable &drawable)
        { render_window->draw(drawable); };
    }
}

bool GameDisplay::is_paused() const
{
    return m_paused;
}

void GameDisplay::set_paused(const bool b)
{
    if (b == m_paused)
    {
        return;
    }

    m_paused = b;

    qInfo() << "Pause toggle triggered! Is paused?" << m_paused;
    emit pause_toggle(m_paused);
}

void GameDisplay::on_push_error(QString msg, int error_code)
{
    qInfo() << "error";
    set_paused(true);
    QMessageBox::critical(
        this,
        QString("TetroidNES - %1").arg(tr("Error")),
        msg);

    this->err_code = error_code;

    qCritical() << msg;

    close();
}

void GameDisplay::on_init()
{
    qInfo() << "Initializing game window...";

    update_game_scale();

    qInfo() << "About to start thread...";
    if (m_is_emu_on_dif_thread)
    {
        emu_thread->start();
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

    draw_func(*sprite.get());
    render_window->display();

    frame_count++;
    qDebug() << "Milliseconds from previous draw call:" << time_between_draw_ms;
    time_between_draw_ms = 0;
}

void GameDisplay::on_framerate_timer_timeout()
{
    setWindowTitle(
        QString("%1 Speed: %%2 | FPS: %3").arg(game_title, QString::number(speed_percent(frame_count, ntsc_frame_rate)), QString::number(frame_count)));
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
        QFile shader_qfile(QStringLiteral(":/shaders/crt_shader.frag"));
        if (!shader_qfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            on_push_error(QStringLiteral("Failed loading shader file from QFile"), EXIT_FAILURE);
            return;
        }

        QTextStream in(&shader_qfile);
        std::string shader_text = in.readAll().toStdString();

        if (!crt_shader->loadFromMemory(shader_text, sf::Shader::Type::Fragment))
        {
            on_push_error(QStringLiteral("Failed loading shader from memory"), EXIT_FAILURE);
            return;
        }

        // Setting shader's variables
        crt_shader->setUniform("density"s, 1.9f);
        crt_shader->setUniform("opacityScanline"s, 0.2f);
        crt_shader->setUniform("opacityNoise"s, 0.2f);
        crt_shader->setUniform("curvature"s, 7.5f);
        crt_shader->setUniform("vigantteWidth"s, 50.0f);
        crt_shader->setUniform("Res"s, sf::Glsl::Vec2({800.0f, 600.0f}));
        crt_shader->setUniform("brightness"s, 0.9f);
        crt_shader->setUniform("warp_brightness"s, 0.1f);

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

    emu_thread->quit();
    emu_thread->wait();
}

void GameDisplay::closeEvent(QCloseEvent *event)
{

    if (!m_initialized || err_code == EXIT_FAILURE)
    {

        close_game();
        qInfo() << "CPU exited unsuccessfully";

        event->accept();
    }
    else
    {
        int message_box_result = QMessageBox::question(
            this,
            QString("TetroidNES - %1").arg(tr("Confirmation")),
            QString("%1\n%2").arg(
                tr("Are you sure you want to quit?"),
                tr("(Remember to save before quitting!)")
            ),
            QMessageBox::Yes | QMessageBox::No);
        if (message_box_result == QMessageBox::No)
        {
            event->ignore();
        }
        else
        {
            close_game();
            emu_worker->log_cpu();

            qInfo() << "CPU exited successfully";
            event->accept();
        }
    }
}

void GameDisplay::update_game_scale()
{
    QSize widget_size = size();
    sf::Vector2u texture_size = sprite->getTexture().getSize();
    sprite->setScale({
        static_cast<float>(widget_size.width()) / texture_size.x,
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

void GameDisplay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_G)
    {
        set_paused(!m_paused);
    }
}

GameDisplay::~GameDisplay()
{
}
