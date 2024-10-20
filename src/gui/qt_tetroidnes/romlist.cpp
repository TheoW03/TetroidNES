#include <romlist.h>
#include <util.h>

#include <QtLogging>
#include <QRegularExpression>
#include <QSettings>
#include <QFileInfo>
#include <QUrl>
#include <QDir>

// #include <algorithm>
//  RomListData
RomListData::RomListData(QObject *parent) : QObject{parent}
{
}
RomListData::~RomListData()
{
}

void RomListData::set_data(short unsigned int year, QByteArray img, QString title, std::optional<bool> favorited, QUrl path)
{
    if (year)
    {
        m_year = year;
    }
    if (!img.isNull())
    {
        m_img = img;
    }
    if (!title.isNull())
    {
        setObjectName(title);
        m_title = title;
    }
    else
    {
        m_title = QString("N/A");
    }
    if (favorited.has_value())
    {
        m_favorited = favorited.value();
    }
    else
    {
        m_favorited = false;
    }
    if (!path.isEmpty() && path.isValid())
    {
        m_path = path;
    }
    else
    {
        qWarning() << "Path for" << title << "was empty or invalid!";
    }
}

short unsigned int RomListData::year() { return m_year; }
QString RomListData::title() { return m_title; }
QByteArray RomListData::img() { return m_img; }
bool RomListData::favorited() { return m_favorited; }
void RomListData::set_favorited(const bool b) { m_favorited = b; }
QUrl RomListData::path(){return m_path; }
void RomListData::set_path(QUrl path) {m_path = path; }

// RomListItem
RomListItem::RomListItem(const QSharedPointer<RomListData> *data, QWidget *parent) : QWidget{parent}
{
    title =           new QLabel(this);
    play =            new QPushButton(this);
    year =            new QLabel(this);
    buttons_frame =   new QFrame(this);
    favorite_button = new QPushButton(buttons_frame);
    layout =          new QVBoxLayout();
    buttons_layout =  new QHBoxLayout();

    favorite_button->setCheckable(true);
    favorite_button->setText("Favorite button");

    title->setAlignment(Qt::AlignCenter);
    year->setAlignment(Qt::AlignCenter);

    layout->addWidget(buttons_frame);
    layout->addWidget(play);
    layout->addWidget(title);
    layout->addWidget(year);
    setLayout(layout);

    setMinimumSize(250, 250);

    buttons_layout->addWidget(favorite_button);
    buttons_frame->setLayout(buttons_layout);

    update_data(*data);

    // Events
    connect(favorite_button, &QPushButton::clicked, this,
            [this](int checked)
            { favorite_button_clicked(checked); });
    connect(play, &QPushButton::clicked, this, &RomListItem::play_button_clicked);
}

void RomListItem::favorite_button_clicked(int checked)
{
    RomList *rom_list = qobject_cast<RomList *>(parent());
    auto listData = rom_list->findChild<RomListData *>(title->text());
    if (listData)
    {
        listData->set_favorited(checked);
    }
    else
    {
        qWarning() << "Could not find data object to update for " << title->text();
    }

    // Refresh display
    if (rom_list->current_mode() == RomList::SortMode::Favorites)
    {
        rom_list->set_current_mode(RomList::SortMode::Favorites);
    }
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
        auto *placeholder_img = new QPixmap(175, 175);
        placeholder_img->fill(Qt::darkGreen);
        play->setIcon(QIcon(*placeholder_img));
        play->setIconSize(placeholder_img->size());
    }
    // else {play->setIcon(QIcon());)

    favorite_button->setChecked(data->favorited());
}

void RomListItem::play_button_clicked()
{
    RomList *rom_list = qobject_cast<RomList *>(parent());
    auto listData = rom_list->findChild<RomListData *>(title->text());

    if (!listData)
    {
        qCritical() << "Cannot find rom data in memory!";
        return;
    }

    auto path = listData->path();

    if (!path.isValid())
    {
        qCritical() << "Path is not valid (Try restarting the program)";
        return;
    }

    qInfo() << "Starting" << title->text();
    qDebug() << "Path:" << path;
    start_game(path.toString());

}

// RomList
RomList::RomList(QWidget *parent) : QWidget{parent}
{
    main_layout = new FlowLayout();
    main_layout->setContentsMargins(0, 150, 0, 150);
    setLayout(main_layout);

    setObjectName("RomList");
    setup_list();
    update_total_pages();
    set_items_per_page(10); // TODO: Change this so the program remembers what the user chose last time
    set_current_mode(RomList::SortMode::AZ);          // TODO: Change this so the program remembers what the user chose last time
    set_current_order(Qt::SortOrder::AscendingOrder); // TODO: Change this so the program remembers what the user chose last time
    qInfo() << "Finished setting up romlist";
}

void RomList::setup_list()
{
    /*
     * Get roms via folder location(s) in config
     * Also the condition should end at either when the end of the list is reached or i == i + amount of widgets shown
     */

    QSettings settings = QSettings("config.cfg", QSettings::IniFormat);
    const QStringList rom_dirs = settings.value("rom_dirs", QStringList()).toStringList();

    if (rom_dirs.isEmpty())
    {
        qDebug() << "Rom dirs were empty";
        return;
    }

    qDebug() << "Searching rom dirs, size:" << rom_dirs.length();

    for (auto &directory_url : rom_dirs)
    {

        auto dir = QDir(directory_url);

        if (!dir.isAbsolute() || !dir.exists() || dir.isEmpty()) {continue;}

        QString title = "Test";
        short unsigned int year = 1980;
        QByteArray image;
        bool favorite = false;

        const QStringList files = dir.entryList(QDir::Files | QDir::NoSymLinks).filter(QRegularExpression(R"(\.nes$)"));
        if (files.isEmpty())
        {
            qDebug() << dir.dirName() << "is empty, skipping iteration...";
            continue;
        }

        for (auto &url : files)
        {
            qDebug() << "File:" << url;
            title = url.section('/', -1).section('\\', -1); // Getting only the file name, make this get rid of the file extension

            QSharedPointer<RomListData> listData = QSharedPointer<RomListData>(new RomListData(this));
            listData->set_data(year, image, title, favorite, QUrl(dir.absoluteFilePath(url)));
            data.append(listData);
        }

    }
    qDebug() << "Finished!";
}

unsigned int RomList::items_per_page() const { return m_items_per_page; }

void RomList::set_items_per_page(unsigned int newNum)
{

    if (newNum == m_items_per_page || data.isEmpty())
    {
        return;
    }

    qDebug() << newNum << "items per page are being set";

    if (newNum > m_items_per_page)
    {
        qDebug() << "Adding more widgets to page, condition: i <" << newNum - items_per_page() << "items per page: " << items_per_page();
        int dataIndex = m_items_per_page * m_current_page;
        for (int i = 0; i < newNum - items_per_page() && i < data.length(); i++, dataIndex++)
        {
            qDebug() << "Data index:" << dataIndex << "i:" << i;
            main_layout->addWidget(new RomListItem(&data.at(dataIndex), this));
        }
        qDebug() << "Finished adding widgets to page";
    }
    else
    {
        QList<RomListItem *> items = this->findChildren<RomListItem *>();
        qDebug() << "Deleting widgets from page";
        for (int i = m_items_per_page; i == newNum; i--)
        {
            qDebug() << "Index:" << i;
            delete &items[i];
        }
    }

    m_items_per_page = newNum;
    update_list();
}

void RomList::set_current_page(unsigned int i)
{
    if (i == m_current_page)
    {
        return;
    }
    m_current_page = i;
    update_list();
}

unsigned int RomList::current_page() const { return m_current_page; }

void RomList::update_total_pages()
{
    if (data.length() <= 1)
    {
        qInfo() << "Data length is lower or equal to 1! Setting total pages to 1";
        m_total_pages = 1;
    }
    else
    {
        m_total_pages = (data.length() + (items_per_page() - 1)) / items_per_page(); // Always rounds up a page
    }
    
}
unsigned int RomList::total_pages() const { return m_total_pages; }

void RomList::update_list()
{
    qDebug() << "Updating list";
    int dataIndex = m_items_per_page * m_current_page - m_items_per_page;
    QList<RomListItem *> items = findChildren<RomListItem *>();
    for (int i = 0; dataIndex < data.length() && i < m_items_per_page; i++)
    {
        items[i]->update_data(data.at(dataIndex));
        dataIndex++;
    }
    qDebug() << "Done updating list";
}

const bool RomList::compare_regex(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b, const QRegularExpression &expr, const SortMode &mode)
{
    const bool match_a = expr.match(a->title()).hasMatch();
    const bool match_b = expr.match(b->title()).hasMatch();

    if (match_a != match_b)
    {
        return match_a;
    }
    else
    {
        switch (mode)
        {
        case Year:
            return compare_year(a, b);
        case Favorites:
            return compare_favorite(a, b);
        case AZ:
            return compare_alphabet(a, b);
        }
        return false;
    }
}

const bool RomList::compare_year(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    if (a->year() != b->year())
    {
        return a->year() < b->year();
    }
    else
    {
        return compare_alphabet(a, b);
    }
}

const bool RomList::compare_favorite(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    if (a->favorited() != b->favorited())
    {
        return a->favorited() < b->favorited();
    }
    else
    {
        return compare_alphabet(a, b);
    }
}

const bool RomList::compare_alphabet(const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
{
    // Decide the lowest length to prevent out of bounds error
    const int lowestLength = (a->title().length() > b->title().length()) ? b->title().length() : a->title().length();
    for (int i = 0; i < lowestLength; i++)
    {
        if (a->title().at(i).toLower() == b->title().at(i).toLower())
        {
            continue;
        }

        return a->title().at(i).toLower() < b->title().at(i).toLower();
    }

    return false;
}

void RomList::add_widget(const QSharedPointer<RomListData> *romData)
{
    auto *new_widget = new RomListItem(romData, this);
    layout()->addWidget(new_widget);
}

void RomList::search(QString &expr)
{
    auto regular_expression = QRegularExpression(expr);
    SortMode mode = current_mode();

    if (current_order() == Qt::AscendingOrder)
    {
        std::sort(data.begin(), data.end(),
                  [&regular_expression, &mode](const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
                  {
                      return compare_regex(a, b, regular_expression, mode);
                  });
    }
    else
    {
        std::sort(data.rbegin(), data.rend(),
                  [&regular_expression, &mode](const QSharedPointer<RomListData> &a, const QSharedPointer<RomListData> &b)
                  {
                      return compare_regex(a, b, regular_expression, mode);
                  });
    }

    update_list();
}

// TODO: Make this function not have two switch cases that essientially do the same thing
void RomList::set_current_mode(const SortMode &mode, const bool update)
{
    m_current_mode = mode;
    if (!update)
    {
        return;
    }

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

    update_list();
}

RomList::SortMode RomList::current_mode() const
{
    return m_current_mode;
}

void RomList::set_current_order(const Qt::SortOrder order)
{
    if (m_current_order == order)
        return;

    m_current_order = order;
    std::reverse(data.begin(), data.end());

    update_list();
}

Qt::SortOrder RomList::current_order() const
{
    return m_current_order;
}

RomList::~RomList()
{
}
