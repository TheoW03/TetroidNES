#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QTimer>
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
    bool initialized() const;
    inline static float framerate_to_msec(const float frame_rate)
    {
        return 1 / frame_rate * 1000;
    }
    ~GameDisplay();

private:
    void on_update();
    void on_init();
    QTimer *frame_timer;
    QScopedPointer<sf::RenderWindow> render_window;
    float frame_time = framerate_to_msec(60);
    bool m_initialized = false;
    QString m_rom_url;
    sf::Texture texture;
    sf::Sprite sprite;
    Execute exe;
    int err_code;

private slots:
    void on_timeout();

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QPaintEngine *paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;

signals:
};

#endif // GAMEDISPLAY_H
