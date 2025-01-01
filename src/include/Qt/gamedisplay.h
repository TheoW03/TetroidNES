#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <chrono>
#include <functional>

#include <QWidget>
#include <QTimer>
#include <QChronoTimer>
#include <QSharedPointer>
#include <QCloseEvent>
#include <QThread>

#include <SFML/Graphics.hpp>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

#include <Qt/Emulator_Worker.h>

class GameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr, QString rom_url = QString());
    void update_game_scale();
    void center_display();
    bool is_paused() const;
    void pause_game();
    bool initialized() const;
    ~GameDisplay();
public slots:
    void on_push_error(QString msg, int error_code);
    void on_update(std::vector<uint8_t> rgb_data_vector);

private:
    void on_init();
    void close_game();

    int frame_count;
    QTimer *frames_per_sec_timer;
    QTimer *time_between_draw_timer;
    QScopedPointer<sf::RenderWindow> render_window;
    sf::Shader crt_shader;
    bool m_initialized = false;
    sf::Texture texture;
    sf::Sprite sprite;
    QThread emu_thread;
    std::function<void (sf::Drawable &drawable)> draw_func;
    EmulatorWorker *emu_worker;
    int err_code;
    int time_between_draw_ms;
    bool m_paused;
    bool m_is_emu_on_dif_thread;

private slots:
    void on_crt_shader_changed(const bool b);
    void on_framerate_timer_timeout();
    void on_pause_toggle_key_triggered();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QPaintEngine *paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAMEDISPLAY_H
