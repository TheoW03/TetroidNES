#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QTimer>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameDisplay : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = nullptr);
    ~GameDisplay();

private:
    void on_update();
    void on_init();
    QTimer frame_timer;
    unsigned int frame_time = 30;
    sf::Texture texture;
    sf::Sprite sprite;
    bool initialized;
private slots:
    void on_timeout();
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    QPaintEngine* paintEngine() const override;

signals:
};

#endif // GAMEDISPLAY_H
