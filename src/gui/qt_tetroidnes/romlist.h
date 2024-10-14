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

class RomListData : public QObject
{
    Q_OBJECT
public:
    explicit RomListData(QObject *parent = nullptr);
    ~RomListData();
    void set_data(short unsigned int year = 0, QByteArray img = QByteArray(), QString title = QString(), std::optional<bool> favorited = std::nullopt);
    short unsigned int year();
    QString title();
    QByteArray img();
    bool favorited();
    void set_favorited(const bool b);

private:
    short unsigned int m_year = 0;
    QByteArray m_img = QByteArray();
    QString m_title = QString();
    bool m_favorited = false;
};

class RomListItem: public QWidget
{
    Q_OBJECT
public:
    explicit RomListItem(const QSharedPointer<RomListData> *data, QWidget *parent = nullptr);
    ~RomListItem();
    void update_data(const QSharedPointer<RomListData> &data);
private slots:
    void favorite_button_clicked(int checked);
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

class RomList : public QWidget
{
    Q_OBJECT
public:
    enum SortMode {Year, Favorites, AZ};
    explicit RomList(QWidget *parent = nullptr);
    ~RomList();
    void add_widget(const QSharedPointer<RomListData> *romData);
    void set_current_mode(const SortMode &mode, const bool update = true);
    SortMode current_mode() const;
    void set_current_order(const Qt::SortOrder order);
    Qt::SortOrder current_order() const;
    void set_items_per_page(unsigned int i);
    void set_current_page(unsigned int i);
    unsigned int current_page() const;
    unsigned int total_pages() const;
    void update_total_pages();
    unsigned int items_per_page() const;
    QSharedPointer<RomListData> find_data(const QString title) const;
    void search(QString &expr);
    void update_list();
private:
    inline static const bool compare_year(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b);
    inline static const bool compare_favorite(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b);
    inline static const bool compare_alphabet(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b);
    inline static const bool compare_regex(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b, const QRegularExpression &expr, const SortMode &mode);
    void setup_list();
    FlowLayout *main_layout;
    QList<QSharedPointer<RomListData>> data;
    RomList::SortMode m_current_mode;
    Qt::SortOrder m_current_order;

    unsigned int m_current_page = 1;
    unsigned int m_total_pages = 1;
    unsigned int m_items_per_page = 0;
signals:
};

#endif // ROMLIST_H