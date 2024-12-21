#include <Qt/Emulator_Worker.h>

#include <Qt/settingsmanager.h>
#include <Qt/utilemulator.h>

#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>
#include "Emulator_Worker.h"

const size_t cpu_cycles_frame = 29782;
EmulatorWorker::EmulatorWorker(QString rom_dest, QMutex &mutex, bool &paused, QWidget *parent) : QObject{parent},
                                                                                                 rom_url(rom_dest),
                                                                                                 m_initialized(false),
                                                                                                 cpu_cycle_count(0),
                                                                                                 nanosecond_between_cycles_count(0),
                                                                                                 m_is_running(false),
                                                                                                 mutex_ptr(&mutex),
                                                                                                 paused_ptr(&paused),
                                                                                                 m_clock_interval(0)
{
}

void EmulatorWorker::shutdown_game()
{

    m_is_running = false;
    exe.log_Cpu();
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
        emit push_error(QString("Could not start game, ROM URL is invalid: %1").arg(rom_url), EXIT_FAILURE);
        return;
    }

    // Constructor Code
    cpu_timer = new QChronoTimer(this);
    time_between_cycle_timer = new QChronoTimer(this);

    SettingsManager &settings = SettingsManager::instance();

    set_clock_interval_speed(settings.speed());

    auto cpu_time_ns = std::chrono::nanoseconds(m_clock_interval);

    cpu_timer->setTimerType(Qt::PreciseTimer);
    cpu_timer->setInterval(cpu_time_ns);
    cpu_timer->setSingleShot(true);

    time_between_cycle_timer->setInterval(std::chrono::nanoseconds(1));

    // qDebug() << "CPU clock cycle" << cpu_timer->interval().count() << "Nanoseconds";
    qDebug() << "Game Path:" << rom_url;

    // Events
    connect(&settings, &SettingsManager::speed_changed, this, &EmulatorWorker::set_clock_interval_speed); // TODO: this doesn't work
    connect(time_between_cycle_timer, &QChronoTimer::timeout, this,
            [this]()
            { nanosecond_between_cycles_count += 1; });
    connect(QThread::currentThread(), &QThread::finished, this, &EmulatorWorker::deleteLater);
    connect(cpu_timer, &QChronoTimer::timeout, this, &EmulatorWorker::on_cpu_timer_timeout);

    // Setup CPU
    initializeInstructionMap();
    auto rom = load_rom(file_tobyte_vector(rom_url.toStdString()));
    if (rom.has_value() == 0)
    {
        emit push_error("Unrecongnized file format, needs to be NES v1.0 format.", EXIT_FAILURE);
        return;
    }

    Bus bus = Bus(rom.value(), NES_START);
    CPU cpu = CPU();
    bus.fill(bus.read_16bit(0xfffc));
    // printf("0x%x\n", bus.get_PC());

    cpu.bus = bus;
    cpu.A_Reg = 0;
    cpu.status.val = 0;
    cpu.X_Reg = 0;
    cpu.Y_Reg = 0;
    cpu.bus.clock_cycles = 0;
    cpu.interrupt = {};

    exe = Execute(cpu);

    m_initialized = true;
}

void EmulatorWorker::on_start_main_thread()
{
    init();
    m_is_running = true;

    cpu_timer->start();
}

void EmulatorWorker::start_cpu_timer()
{
    cpu_timer->start();
}

void EmulatorWorker::stop_cpu_timer()
{
    if(cpu_timer->isActive())
    {
        cpu_timer->stop();
    }
}

void EmulatorWorker::on_cpu_timer_timeout()
{
    if(!m_is_running)
    {
        return; // cpu_timer should be oneshot, ends the loop
    }

    const int clock_cycles = process_cpu();
    cpu_timer->setInterval(std::chrono::nanoseconds(m_clock_interval * clock_cycles));
    cpu_timer->start();
}

void EmulatorWorker::on_start_threaded()
{
    QThread *current_thread = QThread::currentThread();
    qInfo() << "Started game on a new thread:" << QUrl(rom_url).fileName();
    init();

    // time_between_cycle_timer->start();

    m_is_running = true;
    while (m_is_running)
    {
        mutex_ptr->lock();
        if (paused_ptr)
        {
            *paused_ptr = false;
        }
        mutex_ptr->unlock();
        const int clock_cycles = process_cpu();
        current_thread->sleep(std::chrono::nanoseconds(m_clock_interval * clock_cycles));
    }
}

void EmulatorWorker::render_frame()
{
    qDebug() << "Emitting draw_frame signal";
    std::vector<uint8_t> render = exe.render();
    emit draw_frame(render);
}

bool EmulatorWorker::is_running() const
{
    return m_is_running;
}

int EmulatorWorker::process_cpu()
{
    // Process CPU

    if (cpu_cycle_count >= cpu_cycles_frame)
    {
        render_frame();
        exe.reset_clock();
        cpu_cycle_count = 0;
    }

    auto result = exe.run();

    // printf("0x%x\n", result.bus.get_PC());

    // this->cpu = result;
    if (result.error_code == EXIT_FAILURE)
    {
        qInfo() << "potential error with the cpu";

        auto err_mess = QString("%1-- at PC addr= 0x%2").arg(QString::fromStdString(result.bus.check_error().value()), QString::fromStdString(num_to_hexa(result.bus.get_PC())));

        emit push_error(err_mess, EXIT_FAILURE);

    }
    int clock_cycle = exe.reset_clock();

    cpu_cycle_count += clock_cycle;

    nanosecond_between_cycles_count = 0;

    return clock_cycle;
}

int EmulatorWorker::clock_interval() const
{
    return m_clock_interval;
}

void EmulatorWorker::set_clock_interval_speed(const float speed)
{
    m_clock_interval = static_cast<int>(static_cast<float>(emulator_clock_ns) * speed);
}