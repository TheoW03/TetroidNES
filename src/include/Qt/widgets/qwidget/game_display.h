#pragma once

#include <functional>

#include <QWidget>
#include <QTimer>
#include <QChronoTimer>
#include <QSharedPointer>
#include <QCloseEvent>
#include <QThread>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

#include <Qt/objects/Emulator_Worker.h>

class GameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GameDisplay(Rom rom, QWidget *parent = nullptr, QString rom_url = QString());
    void update_game_scale();
    void center_display();
    bool is_paused() const;
    void set_paused(const bool b);
    bool initialized() const;
    ~GameDisplay();

signals:
    void pause_toggle(bool paused);

public slots:
    void on_push_error(QString msg, int error_code);
    void on_update(renderdata_shared_ptr rgb_data_vector);

private:
    void on_init();
    void close_game();

    QString game_title;
    int frame_count;
    QTimer *frames_per_sec_timer;
    QTimer *time_between_draw_timer;
    QScopedPointer<sf::RenderWindow> render_window;
    QScopedPointer<sf::Shader> crt_shader;
    bool m_initialized = false;
    sf::Texture texture;
    QScopedPointer<sf::Sprite> sprite;
    QThread *emu_thread;
    std::function<void(sf::Drawable &drawable)> draw_func;
    EmulatorWorker *emu_worker;
    int err_code;
    int time_between_draw_ms;
    bool m_paused;
    bool m_is_emu_on_dif_thread;

private slots:
    void on_crt_shader_changed(const bool b);
    void on_framerate_timer_timeout();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QPaintEngine *paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;
};