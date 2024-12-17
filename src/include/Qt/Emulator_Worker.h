#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <chrono>

#include <QObject>
#include <QChronoTimer>

#include <Emulator/Execute.h>

class EmulatorWorker : public QObject
{
    Q_OBJECT

public:
    explicit EmulatorWorker(QString rom, QWidget *parent = nullptr);
    friend class EmulatorThread;
public slots:
    void on_start();
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
    void init();

private slots:
    void set_frame_time(float speed);
    void process_cpu();
    void render_frame();

};


#endif // EMULATORWORKER_H