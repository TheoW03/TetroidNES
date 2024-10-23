#ifndef ROMLIST_H
#define ROMLIST_H

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

#include <Qt/flowlayout.h>
#include <Qt/romlistitem.h>
#include <Qt/romdata.h>

class RomList : public QWidget
{
    Q_OBJECT
public:
    enum SortMode {Year, Favorites, AZ};
    explicit RomList(QWidget *parent = nullptr);
    ~RomList();
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
    shptr_romdata get_romdata(const int page, int index);
    void search(QString &expr);
    void update_display();
private:
    inline static const bool compare_year(const shptr_romdata &a, const shptr_romdata &b);
    inline static const bool compare_favorite(const shptr_romdata &a, const shptr_romdata &b);
    inline static const bool compare_alphabet(const shptr_romdata &a, const shptr_romdata &b);
    inline static const bool compare_regex(const shptr_romdata &a, const shptr_romdata &b, const QRegularExpression &expr, const SortMode &mode);
    void setup_display();
    FlowLayout *main_layout;
    QList<shptr_romdata> data;
    RomList::SortMode m_current_mode;
    Qt::SortOrder m_current_order;

    unsigned int m_current_page = 1;
    unsigned int m_total_pages = 1;
    unsigned int m_items_per_page = 0;
signals:
};

#endif // ROMLIST_H
