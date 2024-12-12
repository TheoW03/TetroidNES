#include <Qt/emulatorthread.h>
#include <Qt/settingsmanager.h>
#include <Qt/utilemulator.h>

#include <Emulator/InstructionMap.h>
#include <Emulator/LoadRom.h>
const size_t cpu_cycles_frame = 29782;
EmulatorThread::EmulatorThread(QString rom_dest, QWidget *parent) : QThread{parent},
                                                                    cpu_timer(new QChronoTimer(this)),
                                                                    time_between_cycle_timer(new QChronoTimer(this)),
                                                                    frame_timer(new QChronoTimer(this)),
                                                                    rom_url(rom_dest)
{
    SettingsManager &settings = SettingsManager::instance();

    auto cpu_time_ns = std::chrono::nanoseconds(emulator_clock_ns);

    cpu_timer->setTimerType(Qt::PreciseTimer);
    cpu_timer->setInterval(cpu_time_ns);
    cpu_timer->setSingleShot(true);

    time_between_cycle_timer->setInterval(std::chrono::nanoseconds(1));

    qDebug() << "CPU clock cycle" << cpu_timer->interval().count() << "Nanoseconds";

    frame_timer->setTimerType(Qt::PreciseTimer);
    set_frame_time(settings.speed());

    // Setup CPU
    initializeInstructionMap();
    auto rom = load_rom(file_tobyte_vector(rom_url.toStdString()));
    if (rom.has_value() == 0)
    {
        qCritical() << "unrecongnized file format needs to be NES v1.0 format";

        emit push_error("Wrong NES file format.", EXIT_FAILURE);
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

    exe = Execute(cpu);

    // Events
    connect(this, &QThread::started, this, &EmulatorThread::on_start);
    connect(cpu_timer, &QChronoTimer::timeout, this, &EmulatorThread::process_cpu);
    connect(frame_timer, &QChronoTimer::timeout, this, &EmulatorThread::render_frame);
    connect(&settings, &SettingsManager::speed_changed, this, &EmulatorThread::set_frame_time);
    connect(time_between_cycle_timer, &QChronoTimer::timeout, this, [this]()
            { nanosecond_between_cycles_count += 1; });
}

void EmulatorThread::on_start()
{
    qInfo() << "Started game " << QUrl(rom_url).fileName();

    cpu_timer->start();
    time_between_cycle_timer->start();
    // frame_timer->start();
}

void EmulatorThread::quit()
{
    qInfo() << "exiting game " << QUrl(rom_url).fileName();
    exe.log_Cpu();

    QThread::quit();
}

void EmulatorThread::render_frame()
{
    qDebug() << "Emitting draw_frame signal";
    std::vector<uint8_t> render = exe.render();
    emit draw_frame(render);
}

void EmulatorThread::process_cpu()
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
    qDebug()
        << "Nanoseconds from previous cycle:" << nanosecond_between_cycles_count;
    nanosecond_between_cycles_count = 0;
    cpu_timer->start();
}

void EmulatorThread::set_frame_time(const float speed)
{
    const auto ns = framerate_to_ns(ntsc_frame_rate * speed);
    frame_timer->setInterval(ns);
}