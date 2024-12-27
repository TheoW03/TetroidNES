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
    void start_frame_timer();
    void stop_frame_timer();
    int clock_interval() const;
    bool is_running() const;
public slots:
    void set_clock_interval_speed(float speed);
    void on_frame_timer_timeout();
signals:
    void draw_frame(std::vector<uint8_t> vector);
    void push_error(QString msg, int error_code);
private:
    bool is_frame_generated;
    QChronoTimer *frame_timer;
    Execute exe;
    QString rom_url;
    bool m_initialized;
    bool m_is_running;
    QMutex *mutex_ptr;
    bool *paused_ptr;
    int m_clock_interval;
    void init();

private slots:
    void process_cpu();
    void render_frame();

};


#endif // EMULATORWORKER_H