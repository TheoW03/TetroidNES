#pragma once

#include <QObject>
#include <QMutex>
#include <QChronoTimer>

#include <Emulator/Execute.h>

class EmulatorWorker : public QObject
{
    Q_OBJECT

public:
    explicit EmulatorWorker(Rom rom, QString rom_url, QWidget *parent = nullptr);
    void shutdown_game();
    void log_cpu();
    void on_start_main_thread();
    int clock_interval() const;
    bool is_running() const;
public slots:
    void set_clock_interval_speed(float speed);
    void on_frame_timer_timeout();
    void on_pause_toggle(bool paused);
signals:
    void draw_frame(std::vector<uint8_t>* vector);
    void push_error(QString msg, int error_code);

private:
    bool is_frame_generated;
    QChronoTimer *frame_timer;
    Execute exe;
    QString rom_url;
    Rom rom;
    bool m_initialized;
    bool m_is_running;
    int m_clock_interval;
    void init();

private slots:
    void process_cpu();
    void render_frame();
};