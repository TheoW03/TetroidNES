#include "romlist.h"

#include <QDebug>

//#include <algorithm>
// romlistData
romlistData::romlistData(QObject *parent): QObject{parent}
{
}
romlistData::~romlistData()
{
}

void romlistData::setData(short unsigned int year, QByteArray img, QString title, std::optional<bool> favorited)
{
    if (year){this->m_year = year;}
    if (!img.isNull()){this->m_img = img;}
    if (!title.isNull()){this->m_title = title;}
    else {this->m_title = QString("N/A");}
    if (favorited.has_value()){this->m_favorited = favorited.value();}
    else {this->m_favorited = false;}
}

short unsigned int romlistData::year() {return this->m_year;}
QString romlistData::title() {return this->m_title;}
QByteArray romlistData::img() {return this->m_img;}
bool romlistData::favorited() {return this->m_favorited;}

// romlistItem
romlistItem::romlistItem(const QSharedPointer<romlistData> *data, QWidget *parent): QWidget{parent}
{
    title = new QLabel(this);
    img = new QLabel(this);
    year = new QLabel(this);
    buttons_frame = new QFrame(this);
    favorite_button = new QPushButton(buttons_frame);
    layout = new QVBoxLayout();
    buttons_layout = new QHBoxLayout();

    favorite_button->setCheckable(true);
    favorite_button->setText("Favorite button");

    title->setAlignment(Qt::AlignCenter);
    year->setAlignment(Qt::AlignCenter);

    img->setScaledContents(true);

    layout->addWidget(buttons_frame);
    layout->addWidget(img);
    layout->addWidget(title);
    layout->addWidget(year);
    setLayout(layout);

    setMinimumSize(250, 250);

    buttons_layout->addWidget(favorite_button);
    buttons_frame->setLayout(buttons_layout);

    updateData(*data);
}

romlistItem::~romlistItem()
{
}

void romlistItem::updateData(const QSharedPointer<romlistData> &data)
{
    title->setText(data->title());
    year->setText(QString::number(data->year()));
    if (data->img().isNull())
    {
        QPixmap *placeholder_img = new QPixmap(50, 50);
        placeholder_img->fill(Qt::darkGreen);

        img->setPixmap(*placeholder_img);
    }
    //else {img->setPixmap(QImage())

    favorite_button->setChecked(data->favorited());
}

// romlist
romlist::romlist(QWidget *parent): QWidget{parent}
{
    layout = new FlowLayout();
    setLayout(layout);

    setObjectName("RomList");
    setupList();
    setItemsPerPage(8); // TODO: Change this so the program remembers what the user chose last time
    setCurrentMode(romlist::SortMode::AZ); // TODO: Change this so the program remembers what the user chose last time
    setCurrentOrder(Qt::SortOrder::AscendingOrder); // TODO: Change this so the program remembers what the user chose last time
}

void romlist::setupList()
{
    /*
     * Get roms via folder location(s) in config
     * Also the condition should end at either when the end of the list is reached or i == i + amount of widgets shown
    */

    for (int i = 0; i < 10; i++)
    {
        QSharedPointer<romlistData> listData = QSharedPointer<romlistData>(new romlistData(this));
        listData->setData(i + 1980, QByteArray(), QString("Test") + QString::number(i), true);
        data.append(listData);
    }
}

void romlist::setItemsPerPage(unsigned int newNum)
{
    if (newNum == items_per_page){return;}

    if (newNum > items_per_page)
    {
        int dataIndex = items_per_page * current_page;
        for(int i = 0; i < newNum-items_per_page; i++)
        {
            qDebug() << QString::number(i);
            layout->addWidget(new romlistItem(&data.at(dataIndex), this));
            dataIndex++;
        }
    }
    else
    {
        QList<romlistItem> items = this->findChildren<romlistItem>();
        for(int i = items_per_page; i == newNum; i--)
        {
            delete &items.at(i);
        }
    }

    items_per_page = newNum;
    updateList();
}

void romlist::updateList()
{
    int dataIndex = items_per_page * current_page - items_per_page;
    QList<romlistItem*> items = findChildren<romlistItem*>();
    for(int i = 0; dataIndex < data.length() && i < items_per_page; i++)
    {
        items[i]->updateData(data.at(dataIndex));
        dataIndex++;
    }
}

const bool romlist::compare_year(const QSharedPointer<romlistData> &a, const QSharedPointer<romlistData> &b)
{
    return (a->year() < b->year());
}

void romlist::addWidget(const QSharedPointer<romlistData> *romData)
{
    layout->addWidget(new romlistItem(romData, this));
}

void romlist::setCurrentMode(romlist::SortMode mode)
{
    qDebug() << QString::number(mode) << QString::number(current_mode);
    if (current_mode == mode)
        return;

    current_mode = mode;
    if (mode == romlist::SortMode::Year)
    {
        std::sort(data.begin(), data.end(), compare_year);
    }

    updateList();
}

romlist::SortMode romlist::CurrentMode()
{return current_mode;}

void romlist::setCurrentOrder(Qt::SortOrder order)
{
    if (current_order == order)
        return;

    current_order = order;
    std::reverse(data.begin(), data.end());

    updateList();
}

Qt::SortOrder romlist::CurrentOrder()
{return current_order;}

romlist::~romlist()
{

}
