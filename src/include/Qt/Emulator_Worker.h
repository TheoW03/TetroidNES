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
public slots:
    void on_start_threaded();
signals:
    void draw_frame(std::vector<uint8_t> vector);
    void push_error(QString msg, int error_code);
private:
    QChronoTimer *cpu_timer;
    QChronoTimer *time_between_cycle_timer;
    QChronoTimer *frame_timer;
    Execute exe;
    QString rom_url;
    uint32_t cpu_cycle_count;
    long long nanosecond_between_cycles_count;
    bool m_initialized;
    QMutex *mutex_ptr;
    bool *paused_ptr;
    void init();

private slots:
    void set_frame_time(float speed);
    void process_cpu();
    void render_frame();

};


#endif // EMULATORWORKER_H