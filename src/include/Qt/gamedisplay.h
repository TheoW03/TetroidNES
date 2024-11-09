#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <chrono>

#include <QWidget>
#include <QTimer>
#include <QChronoTimer>
#include <QCloseEvent>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

class GameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr, QString rom_url = QString());
    void update_game_scale();
    void center_display();
    std::chrono::nanoseconds frame_time() const;
    void set_frame_time(const float speed);
    bool initialized() const;
    inline static std::chrono::nanoseconds framerate_to_ns(const float frame_rate)
    {
        const auto duration = std::chrono::duration<double>(1.f / frame_rate);
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        const auto ns = std::chrono::nanoseconds(ms);

        //qDebug() << "frame_rate:" << frame_rate << "duration:" << duration.count() << "ms:" << ms.count() << "ns:" << ns.count();
        return ns;
    }
    int speed_percent(const float frame_rate) const;
    ~GameDisplay();

private:
    void on_update();
    void on_init();
    QChronoTimer *frame_timer;
    QTimer *frames_per_sec_timer;
    unsigned int frames_within_second = 0;
    QScopedPointer<sf::RenderWindow> render_window;
    bool m_initialized = false;
    QString m_rom_url;
    sf::Texture texture;
    sf::Sprite sprite;
    Execute exe;
    int err_code;

private slots:
    void on_timeout();
    void on_framerate_timer_timeout();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QPaintEngine *paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAMEDISPLAY_H
