#pragma once

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include <Qt/objects/rom_data.h>

class RomListItem: public QWidget
{
    Q_OBJECT
public:
    explicit RomListItem(const RomData &data, QWidget *parent = nullptr);
    ~RomListItem();
    const RomData& romdata() const;
    void set_romdata(RomData const &data);
    
private slots:
    void favorite_button_clicked(int checked);
    void play_button_clicked();
private:
    void update_data();

    QLabel *title;
    QLabel *year;
    QPushButton *play;
    QFrame *buttons_frame;
    QPushButton *favorite_button;

    RomData m_romdata;

signals:
};