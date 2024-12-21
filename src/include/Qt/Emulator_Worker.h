#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <chrono>

#include <QObject>
#include <QMutex>
#include <QChronoTimer>

#include <Emulator/Execute.h>

class EmulatorWorker : public QObject
{
    Q_OBJECT

public:
    explicit EmulatorWorker(QString rom, QMutex &mutex, bool &paused, QWidget *parent = nullptr);
    void shutdown_game();
    void on_start_main_thread();
    void start_cpu_timer();
    void stop_cpu_timer();
    int clock_interval() const;
    bool is_running() const;
public slots:
    void on_start_threaded();
    void set_clock_interval_speed(float speed);
signals:
    void draw_frame(std::vector<uint8_t> vector);
    void push_error(QString msg, int error_code);
private:
    QChronoTimer *cpu_timer;
    QChronoTimer *time_between_cycle_timer;
    Execute exe;
    QString rom_url;
    uint32_t cpu_cycle_count;
    long long nanosecond_between_cycles_count;
    bool m_initialized;
    bool m_is_running;
    QMutex *mutex_ptr;
    bool *paused_ptr;
    int m_clock_interval;
    void init();

private slots:
    void on_cpu_timer_timeout();
    int process_cpu();
    void render_frame();

};


#endif // EMULATORWORKER_H