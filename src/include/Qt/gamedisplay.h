#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <chrono>

#include <QWidget>
#include <QTimer>
#include <QMutex>
#include <QChronoTimer>
#include <QSharedPointer>
#include <QCloseEvent>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Qt/emulatorthread.h>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

class GameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr, QString rom_url = QString());
    void update_game_scale();
    void center_display();
    bool initialized() const;
    ~GameDisplay();

private:
    void on_init();

    int frame_count = 0;
    QTimer *frames_per_sec_timer;
    QTimer *time_between_draw_timer;
    QScopedPointer<sf::RenderWindow> render_window;
    bool m_initialized = false;
    sf::Texture texture;
    sf::Sprite sprite;
    QScopedPointer<EmulatorThread> emu_thread;
    QSharedPointer<EmulatorWorker> emu_worker;
    int err_code;
    int time_between_draw_ms = 0;
    QSharedPointer<QMutex> mutex;

private slots:
    void on_framerate_timer_timeout();
    void on_pause_toggle_key_triggered();
    void on_push_error(QString msg, int error_code);
    void on_update(std::vector<uint8_t> rgb_data_vector);

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QPaintEngine *paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAMEDISPLAY_H
