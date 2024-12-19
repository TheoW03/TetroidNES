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
                                                                                                paused_ptr(&paused)
{
}

void EmulatorWorker::shutdown_game()
{
    m_is_running = false;
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
    frame_timer = new QChronoTimer(this);

    SettingsManager &settings = SettingsManager::instance();

    auto cpu_time_ns = std::chrono::nanoseconds(emulator_clock_ns);

    cpu_timer->setTimerType(Qt::PreciseTimer);
    cpu_timer->setInterval(cpu_time_ns);
    cpu_timer->setSingleShot(true);

    time_between_cycle_timer->setInterval(std::chrono::nanoseconds(1));

    //qDebug() << "CPU clock cycle" << cpu_timer->interval().count() << "Nanoseconds";
    qDebug() << "Game Path:" << rom_url;

    frame_timer->setTimerType(Qt::PreciseTimer);
    set_frame_time(settings.speed());

    qDebug() << "Connecting events!";
    // Events
    connect(cpu_timer, &QChronoTimer::timeout, this, &EmulatorWorker::process_cpu);
    connect(frame_timer, &QChronoTimer::timeout, this, &EmulatorWorker::render_frame);
    connect(&settings, &SettingsManager::speed_changed, this, &EmulatorWorker::set_frame_time);
    connect(time_between_cycle_timer, &QChronoTimer::timeout, this,
            [this](){ nanosecond_between_cycles_count += 1; });
    connect(QThread::currentThread(), &QThread::finished, this, &EmulatorWorker::deleteLater);
    qDebug() << "Finished connecting events!";

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
    //printf("0x%x\n", bus.get_PC());

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

void EmulatorWorker::on_start_threaded()
{
    QThread *current_thread = QThread::currentThread();
    qInfo() << "Started game on a new thread:" << QUrl(rom_url).fileName();
    init();

    //cpu_timer->start();
    //time_between_cycle_timer->start();
    //frame_timer->start();

    m_is_running = true;
    while(m_is_running){
        mutex_ptr->lock();
        if(paused_ptr)
        {
            *paused_ptr = false;
        }
        mutex_ptr->unlock();
        const int clock_cycles = process_cpu();
        current_thread->sleep(std::chrono::nanoseconds(emulator_clock_ns * clock_cycles));
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
    // this->cpu = exe.run();

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
        // this->exe.log_Cpu();
        qInfo() << "potential error with the cpu";

        auto err_mess = QString("%1-- at PC addr= 0x%2").arg(QString::fromStdString(result.bus.check_error().value()), QString::fromStdString(num_to_hexa(result.bus.get_PC())));

        emit push_error(err_mess, EXIT_FAILURE);

        // TODO: close error and log the CPU stats
    }
    int clock_cycle = exe.reset_clock();

    cpu_cycle_count += clock_cycle;
    //qDebug()
    //    << "Nanoseconds from previous cycle:" << nanosecond_between_cycles_count;
    nanosecond_between_cycles_count = 0;
    //cpu_timer->setInterval(std::chrono::nanoseconds(clock_cycle*emulator_clock_ns));
    //cpu_timer->start();

    return clock_cycle;
}

void EmulatorWorker::set_frame_time(const float speed)
{
    const auto ns = framerate_to_ns(ntsc_frame_rate * speed);
    frame_timer->setInterval(ns);
}