#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QTimer>
#include <QCloseEvent>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

class GameDisplay : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr, QString rom_url = QString());
    void update_game_scale();
    void center_display();
    bool initialized() const;
    ~GameDisplay();

private:
    void on_update();
    void on_init();
    QTimer *frame_timer;
    float frame_time = 16.665; // Milliseconds, 60 FPS
    bool m_initialized = false;
    QString m_rom_url;
    sf::Texture texture;
    sf::Sprite sprite;
    Execute exe; // Will throw errors if given nothing in args
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
