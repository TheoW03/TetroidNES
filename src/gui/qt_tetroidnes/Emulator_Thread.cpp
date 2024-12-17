#include <QDebug>

#include <Qt/emulatorthread.h>
#include <Qt/utilemulator.h>

EmulatorThread::EmulatorThread(
    QSharedPointer<QMutex> mutex_ref,
    QSharedPointer<EmulatorWorker> worker_ptr,
    QObject *parent) : QThread{parent},
                        mutex_scptr(mutex_ref),
                        worker_scptr(worker_ptr)
{
    qDebug() << "Emulator Thread constructed!";
}

void EmulatorThread::run()
{
    mutex_scptr->lock();
    worker_scptr->on_start();
    mutex_scptr->unlock();

    while(true)
    {
        mutex_scptr->lock();
        pause_check();
        worker_scptr->process_cpu();
        mutex_scptr->unlock();
        sleep(std::chrono::nanoseconds(emulator_clock_ns));
    }
}

void EmulatorThread::pause()
{
    qDebug() << "Pausing from main thread?:" << QThread::currentThread()->isMainThread();
    paused = true;
}

void EmulatorThread::resume()
{
    qDebug() << "Resuming from main thread?:" << QThread::currentThread()->isMainThread();
    paused = false;
}

void EmulatorThread::pause_check()
{
    if (paused)
    {
        paused = false;
    }
}

void EmulatorThread::quit()
{
    qInfo() << "Closing thread.";
    mutex_scptr->lock();
    worker_scptr->exe.log_Cpu();
    mutex_scptr->unlock();

    QThread::quit();
}
