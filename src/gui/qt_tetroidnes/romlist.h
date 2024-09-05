#ifndef ROMLIST_H
#define ROMLIST_H

#include <QListWidget>

class romlist : public QListWidget
{
    Q_OBJECT
public:
    enum SortMode {Year, Favorites, AZ};
    explicit romlist(QWidget *parent = nullptr);

    void setCurrentMode(SortMode mode);
    SortMode CurrentMode();

    void setCurrentOrder(Qt::SortOrder order);
    Qt::SortOrder CurrentOrder();
    ~romlist();
private:
    void setupList();
    romlist::SortMode current_mode;
    Qt::SortOrder current_order;
signals:
};

#endif // ROMLIST_H
