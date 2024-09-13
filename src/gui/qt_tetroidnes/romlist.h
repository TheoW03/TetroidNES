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
    ~romlistData();
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
    explicit romlistItem(const QSharedPointer<romlistData> *data, QWidget *parent = nullptr);
    ~romlistItem();
    void updateData(const QSharedPointer<romlistData> &data);
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
    ~romlist();
    inline static const bool compare_year(const QSharedPointer<romlistData> &a, const QSharedPointer<romlistData> &b);
    void addWidget(const QSharedPointer<romlistData> *romData);
    void setCurrentMode(SortMode mode);
    SortMode CurrentMode();
    void setCurrentOrder(Qt::SortOrder order);
    Qt::SortOrder CurrentOrder();
    void setItemsPerPage(unsigned int i);
private:
    void setupList();
    void updateList();
    FlowLayout *layout;
    QScrollArea *scroll_area;
    QList<QSharedPointer<romlistData>> data;
    romlist::SortMode current_mode;
    Qt::SortOrder current_order;

    unsigned int current_page = 1;
    unsigned int total_pages = 1;
    unsigned int items_per_page = 0;
signals:
};

#endif // ROMLIST_H
