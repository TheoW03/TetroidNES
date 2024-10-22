#ifndef ROMLISTITEM_H
#define ROMLISTITEM_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include <Qt/romdata.h>

class RomListItem: public QWidget
{
    Q_OBJECT
public:
    explicit RomListItem(shptr_romdata data, QWidget *parent = nullptr);
    ~RomListItem();
    shptr_romdata romdata();
    void set_romdata(shptr_romdata data);
    
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
    shptr_romdata m_romdata;

signals:
};

#endif // ROMLISTITEM_H