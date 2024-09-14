#include "romlist.h"

#include <QDebug>
#include <QRegularExpression>

//#include <algorithm>
// RomListData
RomListData::RomListData(QObject *parent): QObject{parent}
{
}
RomListData::~RomListData()
{
}

void RomListData::set_data(short unsigned int year, QByteArray img, QString title, std::optional<bool> favorited)
{
    if (year){this->m_year = year;}
    if (!img.isNull()){this->m_img = img;}
    if (!title.isNull())
    {
        this->setObjectName(title);
        this->m_title = title;
    }
    else {this->m_title = QString("N/A");}
    if (favorited.has_value()){this->m_favorited = favorited.value();}
    else {this->m_favorited = false;}
}

short unsigned int RomListData::year() {return this->m_year;}
QString RomListData::title() {return this->m_title;}
QByteArray RomListData::img() {return this->m_img;}
bool RomListData::favorited() {return this->m_favorited;}
void RomListData::set_favorited(const bool b) {this->m_favorited = b;}

// RomListItem
RomListItem::RomListItem(const QSharedPointer<RomListData> *data, QWidget *parent): QWidget{parent}
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

    update_data(*data);

    // Events
    connect(favorite_button, &QPushButton::clicked, this,
        [this](int checked){
            auto listData = this->parent()->findChild<RomListData*>(title->text());
            if(listData)
            {listData->set_favorited(checked);}
            else
            {qWarning() << "Could not find data object to update for " << title->text();}
        }
    );
}

RomListItem::~RomListItem()
{
}

void RomListItem::update_data(const QSharedPointer<RomListData> &data)
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

// RomList
RomList::RomList(QWidget *parent): QWidget{parent}
{
    layout = new FlowLayout();
    setLayout(layout);

    setObjectName("RomList");
    setup_list();
    set_items_per_page(8); // TODO: Change this so the program remembers what the user chose last time
    set_current_mode(RomList::SortMode::AZ); // TODO: Change this so the program remembers what the user chose last time
    set_current_order(Qt::SortOrder::AscendingOrder); // TODO: Change this so the program remembers what the user chose last time
}

void RomList::setup_list()
{
    /*
     * Get roms via folder location(s) in config
     * Also the condition should end at either when the end of the list is reached or i == i + amount of widgets shown
    */

    for (int i = 0; i < 10; i++)
    {
        QSharedPointer<RomListData> listData = QSharedPointer<RomListData>(new RomListData(this));
        listData->set_data(i + 1980, QByteArray(), QString("Test") + QString::number(i), true);
        data.append(listData);
    }
}

void RomList::set_items_per_page(unsigned int newNum)
{
    if (newNum == m_items_per_page){return;}

    if (newNum > m_items_per_page)
    {
        int dataIndex = m_items_per_page * m_current_page;
        for(int i = 0; i < newNum-m_items_per_page; i++)
        {
            qDebug() << QString::number(i);
            layout->addWidget(new RomListItem(&data.at(dataIndex), this));
            dataIndex++;
        }
    }
    else
    {
        QList<RomListItem*> items = this->findChildren<RomListItem*>();
        for(int i = m_items_per_page; i == newNum; i--)
        {
            delete &items[i];
        }
    }

    m_items_per_page = newNum;
    update_list();
}

void RomList::set_current_page(unsigned int i)
{
    if (i == m_current_page) {return;}
    m_current_page = i;
    update_list();
}

unsigned int RomList::current_page() const {return m_current_page;}
unsigned int RomList::total_pages() const {return m_total_pages;}

void RomList::update_list()
{
    int dataIndex = m_items_per_page * m_current_page - m_items_per_page;
    QList<RomListItem*> items = findChildren<RomListItem*>();
    for(int i = 0; dataIndex < data.length() && i < m_items_per_page; i++)
    {
        items[i]->update_data(data.at(dataIndex));
        dataIndex++;
    }
}

const bool RomList::compare_year(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    return a->year() < b->year();
}

void RomList::sort_regex(QString expr)
{
    // if (expr.isEmpty()){expr.append("*");}

    // const QRegularExpression regex(expr);

    // // insertion sort
    // for(int i = 1; i < data.length(); i++)
    // {
    //     auto key = data.at(i);
    //     int j = i - 1;
    //     const QRegularExpressionMatch match = regex.match(data.at(j)->title());

    //     while(j >= 0 && match.hasMatch())
    //     {
    //         data[j+1] = data[j];
    //         j = j - 1;
    //     }
    //     data[j+1] = key;
    // }

    // setCurrentPage(0);
    // updateList();

}

const bool RomList::compare_favorite(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    return a->favorited() <= b->favorited();
}

const bool RomList::compare_alphabet(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    // Decide the lowest length to prevent out of bounds error
    const int lowestLength = (a->title().length() > b->title().length()) ? b->title().length() : a->title().length();
    for(int i = 0; i < lowestLength; i++)
    {
        if(a->title().at(i).toLower() == b->title().at(i).toLower())
        {continue;}

        return a->title().at(i).toLower() < b->title().at(i).toLower();
    }

    return false;
}

void RomList::add_widget(const QSharedPointer<RomListData> *romData)
{
    auto *new_widget = new RomListItem(romData, this);
    layout->addWidget(new_widget);
}

void RomList::set_current_mode(const SortMode mode)
{
    if (current_order() == Qt::AscendingOrder)
    {
        auto begin = data.begin();
        auto end = data.end();

        switch (mode)
        {
        case Year:
            std::sort(begin, end, compare_year);
            break;
        case Favorites:
            std::sort(begin, end, compare_favorite);
            break;
        case AZ:
            std::sort(begin, end, compare_alphabet);
            break;
        }
    }
    else
    {
        auto rbegin = data.rbegin();
        auto rend = data.rend();

        switch (mode)
        {
        case Year:
            std::sort(rbegin, rend, compare_year);
            break;
        case Favorites:
            std::sort(rbegin, rend, compare_favorite);
            break;
        case AZ:
            std::sort(rbegin, rend, compare_alphabet);
            break;
        }
    }

    m_current_mode = mode;
    update_list();

}

RomList::SortMode RomList::current_mode() const
{return m_current_mode;}

void RomList::set_current_order(const Qt::SortOrder order)
{
    if (m_current_order == order)
        return;

    m_current_order = order;
    std::reverse(data.begin(), data.end());

    update_list();
}

Qt::SortOrder RomList::current_order() const
{return m_current_order;}

RomList::~RomList()
{

}
