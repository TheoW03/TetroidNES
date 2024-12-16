#ifndef EMULATORTHREAD_H
#define EMULATORTHREAD_H

#include <chrono>

#include <QThread>
#include <QChronoTimer>
#include <QMutex>
#include <QWaitCondition>

#include <Emulator/Execute.h>

class EmulatorThread : public QThread
{
    Q_OBJECT

public:
    explicit EmulatorThread(QString rom, QWidget *parent = nullptr);

    void quit();
    void init();

signals:
    void draw_frame(std::vector<uint8_t> vector);
    void push_error(QString msg, int error_code);

private:
    QChronoTimer *cpu_timer;
    QChronoTimer *time_between_cycle_timer;
    QChronoTimer *frame_timer;
    Execute exe;
    QString rom_url;
    uint32_t cpu_cycle_count = 0;
    long long nanosecond_between_cycles_count = 0;
    bool m_initialized = false;

private slots:
    void set_frame_time(float speed);
    void process_cpu();
    void render_frame();

    void on_start();
};

#endif // EMULATORTHREAD_H