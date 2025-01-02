#pragma once

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include <Qt/romdata.h>

class RomListItem: public QWidget
{
    Q_OBJECT
public:
    explicit RomListItem(RomData *data = nullptr, QWidget *parent = nullptr);
    ~RomListItem();
    RomData* romdata() const;
    void set_romdata(RomData *data);
    
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
    RomData* m_romdata;

signals:
};