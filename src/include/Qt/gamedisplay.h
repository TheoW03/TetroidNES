#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QTimer>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Emulator/Execute.h>
#include <Emulator/Bus.h>

class GameDisplay : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr);
    void update_game_scale();
    void center_display();
    ~GameDisplay();

private:
    void on_update();
    void on_init();
    QTimer frame_timer;
    float frame_time = 33.33; // 30 FPS
    sf::Texture texture;
    sf::Sprite sprite;
    bool initialized = false;
    CPU cpu;
    Execute exe = Execute(CPU()); // Will throw errors if not given the correct arguments

private slots:
    void on_timeout();
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    QPaintEngine* paintEngine() const override;
    void resizeEvent(QResizeEvent *event) override;

signals:
};

#endif // GAMEDISPLAY_H
