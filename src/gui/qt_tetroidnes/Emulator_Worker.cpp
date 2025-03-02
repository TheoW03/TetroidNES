#include <Qt/Emulator_Worker.h>

#include <Qt/settingsmanager.h>
#include <Qt/controllermanager.h>
#include <Qt/utilemulator.h>

#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>

const size_t cpu_cycles_frame = 29782;
EmulatorWorker::EmulatorWorker(Rom rom, QString rom_dest, QWidget *parent) : QObject{parent},
                                                                             rom_url(rom_dest),
                                                                             m_initialized(false),
                                                                             m_is_running(false),
                                                                             m_clock_interval(frame_interval_ns),
                                                                             is_frame_generated(false)
{
    this->rom = rom;
    qWarning() << "resetting";
}
void EmulatorWorker::log_cpu()
{
    exe.log_Cpu();
}
void EmulatorWorker::shutdown_game()
{

    this->m_is_running = false;
    // exe.log_Cpu();
}

void EmulatorWorker::init()
{

    if (m_initialized)
    {
        qWarning() << "Initialization function was already called, returning...";
        return;
    }

    if (!QUrl(rom_url).isValid())
    {
        emit push_error(tr("Could not start game, ROM URL is invalid") + QStringLiteral(": ") + rom_url, EXIT_FAILURE);
        return;
    }

    // Constructor Code
    frame_timer = new QChronoTimer(this);

    SettingsManager &settings = SettingsManager::instance();

    set_clock_interval_speed(settings.speed());

    frame_timer->setTimerType(Qt::PreciseTimer);
    frame_timer->setInterval(std::chrono::nanoseconds(m_clock_interval));

    qDebug() << "frame timer cycle" << frame_timer->interval().count() << "Nanoseconds";
    qDebug() << "Game Path:" << rom_url;

    // Events
    connect(frame_timer, &QChronoTimer::timeout, this, &EmulatorWorker::on_frame_timer_timeout);

    // Setup CPU
    initializeInstructionMap();
    // auto rom = load_rom(file_tobyte_vector(rom_url.toStdString()));
    // if (rom.has_value() == 0)
    // {
    //     emit push_error("Unrecongnized file format, needs to be NES v1.0 format.", EXIT_FAILURE);
    //     return;
    // }

    Bus bus = Bus(this->rom, NES_START);
    CPU cpu = CPU();
    cpu.bus = bus;
    this->exe = Execute(cpu);
    this->exe.reset();
    // bus.fill(bus.read_16bit(0xfffc));
    // // printf("0x%x\n", bus.get_PC());

    // cpu.bus = bus;
    // cpu.A_Reg = 0;
    // cpu.status.val = 0;
    // cpu.X_Reg = 0;
    // cpu.Y_Reg = 0;
    // cpu.bus.clock_cycles = 0;
    // cpu.interrupt = {};
    is_frame_generated = false;
    qInfo() << "pc: " << num_to_hexa(cpu.bus.get_PC());

    m_initialized = false;
}

void EmulatorWorker::on_frame_timer_timeout()
{
    qDebug() << "Frame timeout, is frame generated?" << is_frame_generated;
    if (is_frame_generated)
    {
        render_frame();
        is_frame_generated = false;
        process_cpu();
    }
}

void EmulatorWorker::on_pause_toggle(bool paused)
{

    if (paused)
    {
        frame_timer->stop();
    }
    else
    {
        frame_timer->start();
    }
}

void EmulatorWorker::on_start_main_thread()
{
    init();
    m_is_running = true;

    frame_timer->start();
    process_cpu(); // Kickstarting the CPU so is_frame_generated can become true
}

void EmulatorWorker::render_frame()
{
    // qDebug() << "Emitting draw_frame signal";
    std::vector<uint8_t> render = exe.render();
    emit draw_frame(render);
}

bool EmulatorWorker::is_running() const
{
    return m_is_running;
}

void EmulatorWorker::process_cpu()
{
    // Process CPU
    CPU result;
    int clock_cycles = 0;
    qInfo() << "processing cpu";

    while (clock_cycles < cpu_cycles_frame)
    {
        result = exe.run();
        if (result.error_code == EXIT_FAILURE)
        {
            qCritical() << "potential error with the cpu at pc=0x" << num_to_hexa(result.bus.get_PC());
            auto error_cpu = result.bus.check_error().value_or("error with emulator, please check the ROM for faulty instructions");
            auto err_mess = QString("%1-- at PC= 0x%2").arg(QString::fromStdString(error_cpu), QString::fromStdString(num_to_hexa(result.bus.get_PC())));
            emit push_error(err_mess, EXIT_FAILURE);
            is_frame_generated = false;
            return;
        }

        clock_cycles += exe.reset_clock();
    }

    // Input Handling
    auto &controller_manager = ControllerManager::instance();
    auto &settings_manager = SettingsManager::instance();

    auto input_map_name = settings_manager.active_input_profile();
    auto input_map = controller_manager.from_json(input_map_name);
    for (const auto button : InputMap::AllButtons)
    {
        const auto key = ControllerManager::get_button_keyboard(input_map, button);

        if (sf::Keyboard::isKeyPressed(key))
        {
            exe.joypad1(AllController[button], 1);
            qInfo() << "Button" << QString::number(button) << "pressed";
        }
    }
    is_frame_generated = true;
}

int EmulatorWorker::clock_interval() const
{
    return m_clock_interval;
}

void EmulatorWorker::set_clock_interval_speed(const float speed)
{
    const float new_interval = static_cast<int>(frame_interval_ns * speed);
    qInfo() << "Setting new clock interval\nOld interval:" << m_clock_interval << "New interval:" << new_interval << "Speed multiplier:" << speed;
    m_clock_interval = new_interval;
}