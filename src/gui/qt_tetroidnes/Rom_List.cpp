#include <QtLogging>
#include <QRegularExpression>
#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include <QMessageBox>

#include <Qt/romlist.h>
#include <Qt/romlistitem.h>
#include <Qt/util.h>
#include <Qt/settingsmanager.h>

RomList::RomList(QWidget *parent) : QWidget{parent},
                                    data(new QList<RomData*>()),
                                    main_layout(new FlowLayout()),
                                    m_current_order(Qt::AscendingOrder),
                                    m_current_page(1),
                                    m_total_pages(1),
                                    m_items_per_page(0)
{
    auto &settings = SettingsManager::instance();

    main_layout->setContentsMargins(0, 150, 0, 150);
    setLayout(main_layout);

    setObjectName("RomList");

    m_items_per_page = 10; // TODO: Change this so the program remembers what the user chose last time

    on_rom_dirs_changed(QStringList());

    set_current_mode(settings.sort_mode());
    set_current_order(settings.ascending_order());

    connect(&settings, &SettingsManager::rom_dirs_changed, this, &RomList::on_rom_dirs_changed);

    qInfo() << "Finished setting up romlist";
}

void RomList::cleanup_romdata()
{
    qDeleteAll(data->begin(), data->end());
    data->clear();
}

void RomList::on_rom_dirs_changed(QStringList dirs)
{
    cleanup_romdata();
    setup_display();
    update_total_pages();
    m_current_page = 1;
    update_display();
}

void RomList::setup_display()
{
    const QStringList rom_dirs = SettingsManager::instance().get_rom_dirs();
    const QRegularExpression qregex(QStringLiteral(R"(\.nes$)"));

    if (rom_dirs.isEmpty())
    {
        qDebug() << "Rom dirs were empty";
        return;
    }

    qDebug() << "Searching rom dirs, size:" << rom_dirs.length();

    for (auto &directory_url : rom_dirs)
    {

        auto dir = QDir(directory_url);

        if (!dir.isAbsolute() || !dir.exists() || dir.isEmpty())
        {
            continue;
        }

        // TODO: MAKE A WAY TO PARSE METADATA OF ROMS TO SET THESE
        uint16_t year = 1980;
        QByteArray image;
        bool favorite = false;

        const QStringList files = dir.entryList(QDir::Files | QDir::NoSymLinks).filter(qregex);
        if (files.isEmpty())
        {
            qDebug() << dir.dirName() << "is empty, skipping iteration...";
            continue;
        }

        for (auto &url : files)
        {
            auto final_url = QUrl(dir.absoluteFilePath(url));
            auto rom_title = url.section('.', 0, 0);

            qDebug()
                << "File:" << final_url.toLocalFile()
                << "Title:" << rom_title;

            auto *romdata = new RomData(nullptr, year, image, rom_title, favorite, final_url);
            data->append(romdata);
        }
    }
    qDebug() << "Finished!";
}

RomData* RomList::get_romdata(const int page, const int index)
{
    uint32_t romlist_index = m_items_per_page * (page - 1) + index;
    RomData *romdata = nullptr;

    if (data->size() - 1 > romlist_index && romlist_index >= 0)
    {
        //qDebug() << "Valid index:" << romlist_index;
        romdata = data->at(romlist_index);
    }

    qDebug()
        << "page:" << page
        << "index:" << index
        << "Rom data size:" << QString::number(data->size())
        << "Is nullptr (Out of bounds)(bool):" << QString::number(romdata == nullptr);

    return romdata;
}

uint32_t RomList::items_per_page() const { return m_items_per_page; }

void RomList::set_items_per_page(uint32_t newNum)
{
    m_items_per_page = newNum;
    update_display();
}

void RomList::set_current_page(uint32_t i)
{
    if (i == m_current_page)
    {
        return;
    }
    m_current_page = i;
    update_display();
}

uint32_t RomList::current_page() const { return m_current_page; }

void RomList::update_total_pages()
{
    qDebug()
        << "Data length:" << data->length()
        << "Items per page:" << items_per_page();
    if (data->length() <= 1)
    {
        qInfo() << "Data length is lower or equal to 1! Setting total pages to 1";
        m_total_pages = 1;
    }
    else
    {
        m_total_pages = (data->length() + (items_per_page() - 1)) / items_per_page(); // Always rounds up a page
    }
}
uint32_t RomList::total_pages() const { return m_total_pages; }

// Probably needs another refactor?
void RomList::update_display()
{
    auto list_of_widgets = findChildren<RomListItem *>();
    int widgets_count = list_of_widgets.size();

    qDebug() << "Updating display, Widgets count:" << widgets_count;

    // Add widgets if there are less items than items that need to be shown
    if (m_items_per_page > widgets_count)
    {

        qDebug() << "Adding widgets";

        for (int i = 0; i < m_items_per_page - widgets_count; i++)
        {
            auto *new_widget = new RomListItem(nullptr, this);

            main_layout->addWidget(new_widget);
            list_of_widgets.append(new_widget);
        }
        widgets_count = list_of_widgets.size();
    }
    // Remove widgets if there are more items than items that need to be shown
    else if (m_items_per_page < widgets_count)
    {
        qDebug() << "Removing widgets";
        for (int i = widgets_count; i > widgets_count - (widgets_count - m_items_per_page); i--)
        {
            qDebug() << i;
            auto *widget = list_of_widgets[i];

            widget->setParent(nullptr);
            list_of_widgets.removeLast();
            widget->deleteLater();
        }
        widgets_count = list_of_widgets.size();
    }

    qDebug() << "Applying data to widgets";

    // Apply data
    for (int i = 0; i < widgets_count; i++)
    {
        auto *romdata = get_romdata(m_current_page, i);
        auto romlistitem = list_of_widgets[i];
        if (romdata != nullptr)
        {
            romlistitem->set_romdata(romdata);
            // Hide empty items and show valid items
            romdata->is_empty() ? romlistitem->hide() : romlistitem->show();
        }
        else
        {
            romlistitem->hide();
        }
        
    }

    qDebug() << "Done updating display";
}

const bool RomList::compare_regex(const RomData *a, const RomData *b, const QRegularExpression &expr, const SortMode &mode)
{
    const bool match_a = expr.match(a->title()).hasMatch();
    const bool match_b = expr.match(b->title()).hasMatch();

    if (match_a != match_b)
    {
        return match_a;
    }

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

const bool RomList::compare_year(const RomData *a, const RomData *b)
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

const bool RomList::compare_favorite(const RomData *a, const RomData *b)
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

const bool RomList::compare_alphabet(const RomData *a, const RomData *b)
{
    return a->title().compare(b->title(), Qt::CaseInsensitive) > 0;
}

void RomList::search(QString &expr)
{
    auto regular_expression = QRegularExpression(expr);
    SortMode mode = current_mode();

    if (current_order() == Qt::AscendingOrder)
    {
        std::sort(data->begin(), data->end(),
                  [&regular_expression, &mode](const RomData *a, const RomData *b)
                  {
                      return compare_regex(a, b, regular_expression, mode);
                  });
    }
    else
    {
        std::sort(data->rbegin(), data->rend(),
                  [&regular_expression, &mode](const RomData *a, const RomData *b)
                  {
                      return compare_regex(a, b, regular_expression, mode);
                  });
    }

    update_display();
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
        auto begin = data->begin();
        auto end = data->end();

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
        auto rbegin = data->rbegin();
        auto rend = data->rend();

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

    update_display();
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
    std::reverse(data->begin(), data->end());

    update_display();
}

Qt::SortOrder RomList::current_order() const
{
    return m_current_order;
}

RomList::~RomList()
{
    cleanup_romdata();
}
