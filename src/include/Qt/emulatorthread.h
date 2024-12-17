#ifndef EMULATORTHREAD_H
#define EMULATORTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include <Qt/Emulator_Worker.h>

class EmulatorThread : public QThread
{
    Q_OBJECT

public:
    explicit EmulatorThread(QSharedPointer<QMutex> mutex, QSharedPointer<EmulatorWorker> worker, QObject *parent = nullptr);
    void quit();
    void pause();
    void resume();
    bool get_paused()
    {
        return paused;
    };
private:
    void pause_check();
    bool paused = false;
    QSharedPointer<EmulatorWorker> worker_scptr;
    QSharedPointer<QMutex> mutex_scptr;
protected:
    void run() override;

};

#endif // EMULATORTHREAD_H