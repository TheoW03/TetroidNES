#include "romlist.h"

#include <QDebug>

#include <algorithm>
// romlistData
romlistData::romlistData(QObject *parent): QObject{parent}
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
romlistItem::romlistItem(QSharedPointer<romlistData> data, QWidget *parent): QWidget{parent}
{
    title = new QLabel(data->title(), this);
    img = new QLabel(this);
    year = new QLabel(QString::number(data->year()), this);
    buttons_frame = new QFrame(this);
    favorite_button = new QPushButton(buttons_frame);
    layout = new QVBoxLayout();
    buttons_layout = new QHBoxLayout();

    favorite_button->setCheckable(true);
    favorite_button->setChecked(data->favorited());
    favorite_button->setText("Favorite button");

    title->setAlignment(Qt::AlignCenter);
    year->setAlignment(Qt::AlignCenter);

    img->setScaledContents(true);
    if (data->img().isNull())
    {
        QPixmap *placeholder_img = new QPixmap(50, 50);
        placeholder_img->fill(Qt::darkGreen);

        img->setPixmap(*placeholder_img);
    }
    //else {img->setPixmap(QImage())

    layout->addWidget(buttons_frame);
    layout->addWidget(img);
    layout->addWidget(title);
    layout->addWidget(year);
    setLayout(layout);

    setMinimumSize(250, 250);

    buttons_layout->addWidget(favorite_button);
    buttons_frame->setLayout(buttons_layout);

    show();
}

// romlist
romlist::romlist(QWidget *parent): QWidget{parent}
{
    layout = new FlowLayout();
    setLayout(layout);

    setObjectName("RomList");
    setupList();
    setCurrentMode(romlist::SortMode::AZ); // TODO: Change this so the program remembers what the user chose last time
    setCurrentOrder(Qt::SortOrder::AscendingOrder); // TODO: Change this so the program remembers what the user chose last time
}

void romlist::setupList()
{
    /*
     * Get roms via folder location(s) in config
     * Make for loop iterating through how many widgets can be displayed at once
     * The for loop start value should reflect on which page is selected
     * Also the condition should end at either when the end of the list is reached or i == i + amount of widgets shown
    */

    for (int i = 0; i < 10; i++)
    {
        QSharedPointer<romlistData> listData = QSharedPointer<romlistData>(new romlistData);
        listData->setData(i + 1980, QByteArray(), QString("Test") + QString::number(i), true);
        addWidget(&listData);
    }
}

void romlist::updateList()
{
    QLayoutItem *item;
    while(layout->itemAt(0))
    {
        item = layout->itemAt(0);
        delete item;
    }

    setupList();
}

const bool romlist::compare_year(QSharedPointer<romlistData> &a, QSharedPointer<romlistData> &b)
{
    return (a->year() < b->year());
}

void romlist::addWidget(QSharedPointer<romlistData> *romData)
{
    data.append(*romData);
    layout->addWidget(new romlistItem(*romData, this));
}

void romlist::setCurrentMode(romlist::SortMode mode)
{
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
    // sorting goes here
}

Qt::SortOrder romlist::CurrentOrder()
{return current_order;}

romlist::~romlist()
{

}
