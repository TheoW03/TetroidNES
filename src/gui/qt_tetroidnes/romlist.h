#ifndef ROMLIST_H
#define ROMLIST_H

#include "flowlayout.h"

#include <QWidget>
#include <QObject>
#include <QByteArray>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QSharedPointer>
#include <QScrollArea>

class romlistData : public QObject
{
    Q_OBJECT
public:
    explicit romlistData(QObject *parent = nullptr);
    void setData(short unsigned int year = 0, QByteArray img = QByteArray(), QString title = QString(), std::optional<bool> favorited = std::nullopt);
    short unsigned int year();
    QString title();
    QByteArray img();
    bool favorited();

private:
    short unsigned int m_year = 0;
    QByteArray m_img = QByteArray();
    QString m_title = QString();
    bool m_favorited = false;
};

class romlistItem: public QWidget
{
    Q_OBJECT
public:
    explicit romlistItem(QSharedPointer<romlistData> data, QWidget *parent = nullptr);
private:
    QLabel *title;
    QLabel *year;
    QLabel *img;
    QFrame *buttons_frame;
    QPushButton *favorite_button;
    QVBoxLayout *layout;
    QHBoxLayout *buttons_layout;

signals:
};

class romlist : public QWidget
{
    Q_OBJECT
public:
    enum SortMode {Year, Favorites, AZ};
    explicit romlist(QWidget *parent = nullptr);
    inline static const bool compare_year(QSharedPointer<romlistData> &a, QSharedPointer<romlistData> &b);
    void addWidget(QSharedPointer<romlistData> *romData);

    void setCurrentMode(SortMode mode);
    SortMode CurrentMode();
    void setCurrentOrder(Qt::SortOrder order);
    Qt::SortOrder CurrentOrder();
    ~romlist();
private:
    void setupList();
    void updateList();
    FlowLayout *layout;
    QScrollArea *scroll_area;
    QList<QSharedPointer<romlistData>> data;
    romlist::SortMode current_mode;
    Qt::SortOrder current_order;
signals:
};

#endif // ROMLIST_H
