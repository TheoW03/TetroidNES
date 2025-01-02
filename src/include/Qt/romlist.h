#pragma once

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
    void set_items_per_page(uint32_t i);
    void set_current_page(uint32_t i);
    uint32_t current_page() const;
    uint32_t total_pages() const;
    void update_total_pages();
    uint32_t items_per_page() const;
    RomData* get_romdata(const int page, const int index);
    void search(QString &expr);
    void update_display();
private:
    inline static const bool compare_year(const RomData *a, const RomData *b);
    inline static const bool compare_favorite(const RomData *a, const RomData *b);
    inline static const bool compare_alphabet(const RomData *a, const RomData *b);
    inline static const bool compare_regex(const RomData *a, const RomData *b, const QRegularExpression &expr, const SortMode &mode);
    void setup_display();
    void cleanup_romdata();
    FlowLayout *main_layout;
    QScopedPointer<QList<RomData*>> data;
    RomList::SortMode m_current_mode;
    Qt::SortOrder m_current_order;

    uint32_t m_current_page;
    uint32_t m_total_pages;
    uint32_t m_items_per_page;
private slots:
    void on_rom_dirs_changed(QStringList dirs);
};