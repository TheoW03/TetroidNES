#include <Qt/objects/rom_data_container.h>

#include <QDebug>
#include <QDir>

#include <Qt/objects/settings_manager.h>


RomDataContainer::RomDataContainer()
{

}

RomDataContainer::RomDataContainer(const RomDataQList &data)
{
    set_raw_data(data);
}

RomDataContainer::~RomDataContainer()
{
}

void RomDataContainer::update_raw_data()
{
    const QStringList rom_dirs = SettingsManager::instance().get_rom_dirs();
    const QRegularExpression qregex(QStringLiteral(R"(\.nes$)"));

    m_raw_data.clear();
    m_raw_data.squeeze();

    if (rom_dirs.isEmpty())
    {
        qDebug() << "Rom dirs were empty";
        return;
    }

    qDebug() << "Searching rom dirs, size:" << rom_dirs.length();

    QStringList rom_urls;

    for (auto &directory_url : rom_dirs)
    {

        auto dir = QDir(directory_url);

        if (!dir.isAbsolute() || !dir.exists() || dir.isEmpty())
        {
            continue;
        }

        const QStringList files = dir.entryList(QDir::Files | QDir::NoSymLinks).filter(qregex);
        if (files.isEmpty())
        {
            qDebug() << dir.dirName() << "is empty, skipping iteration...";
            continue;
        }
        
        rom_urls.reserve(rom_urls.length() + files.length());
        rom_urls += files;
    }

    // TODO: MAKE A WAY TO PARSE METADATA OF ROMS TO SET THESE
    uint16_t year = 1980;
    QByteArray image;
    bool favorite = false;

    m_raw_data.reserve(rom_urls.length());

    for (const QString &rom_url : rom_urls)
    {
        auto rom_title = rom_url.section('.', 0, 0);

        m_raw_data.emplace_back(RomData(year, image, rom_title, favorite, rom_url));   
    }

    update_mimic_data();
}

void RomDataContainer::set_raw_data(const RomDataQList &data)
{
    m_raw_data = data;
    update_mimic_data();
}

const RomDataQList *RomDataContainer::get_raw_data() const
{
    return &m_raw_data;
}

const MimicRomDataQList *RomDataContainer::get_mimic_data() const
{
    return &m_mimic_data;
}

const RomData &RomDataContainer::get_romdata(const int &page, const int &index) const
{
    return *m_mimic_data[page][index];
}

void RomDataContainer::sort_data(const CompareFuncPtr sort_func, const Qt::SortOrder &ascending)
{
    if (ascending)
    {
        std::sort(m_raw_data.begin(), m_raw_data.end(), sort_func);
    }
    else
    {
        std::sort(m_raw_data.rbegin(), m_raw_data.rend(), sort_func);
    }

    update_mimic_data();
}

void RomDataContainer::sort_data(const QRegularExpression &expr, const Qt::SortOrder &ascending, const SortMode &sort_mode)
{
    const auto SORT_FUNC(
        [&expr, &sort_mode](const RomData &a, const RomData &b)
        {
            return compare_regex(a, b, expr, sort_mode);
        }
    );

    if (ascending)
    {
        std::sort(m_raw_data.begin(), m_raw_data.end(), SORT_FUNC);
    }
    else
    {
        std::sort(m_raw_data.rbegin(), m_raw_data.rend(), SORT_FUNC);
    }
}

void RomDataContainer::sort_data_reverse()
{
    std::reverse(m_raw_data.begin(), m_raw_data.end());

    update_mimic_data();
}

bool RomDataContainer::compare_regex(const RomData &a, const RomData &b, const QRegularExpression &expr, const SortMode &mode)
{
    const bool match_a = expr.match(a.title()).hasMatch();
    const bool match_b = expr.match(b.title()).hasMatch();

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

bool RomDataContainer::compare_year(const RomData &a, const RomData &b)
{
    if (a.year() != b.year())
    {
        return a.year() < b.year();
    }
    else
    {
        return compare_alphabet(a, b);
    }
}

bool RomDataContainer::compare_favorite(const RomData &a, const RomData &b)
{
    if (a.favorited() != b.favorited())
    {
        return a.favorited() < b.favorited();
    }
    else
    {
        return compare_alphabet(a, b);
    }
}

bool RomDataContainer::compare_alphabet(const RomData &a, const RomData &b)
{
    return a.title().compare(b.title(), Qt::CaseInsensitive) > 0;
}

void RomDataContainer::update_mimic_data()
{   
    qInfo() << "Updating mimic data";

    const int ITEMS_PER_PAGE = 10;
    const auto raw_data_size = m_raw_data.size();
    const int pages = (raw_data_size + (ITEMS_PER_PAGE - 1)) / ITEMS_PER_PAGE;

    m_mimic_data.clear();
    m_mimic_data.squeeze();
    m_mimic_data.reserve(raw_data_size);

    QList<RomData*> tmp_list;
    tmp_list.reserve(ITEMS_PER_PAGE);

    qDebug() << "Raw Data Size, Pages:" << raw_data_size << pages;

    for (int i = 0, k = 0; i < pages; i++)
    {

        qDebug()
        << "Starting new page, mimic data:" << m_mimic_data
        << "\ntmp_data:" << tmp_list;

        for (int j = 0; j < ITEMS_PER_PAGE && k < raw_data_size; j++, k++)
        {
            tmp_list.emplace_back(&m_raw_data[k]);
        }

        tmp_list.squeeze();
        m_mimic_data.emplace_back(tmp_list);
        tmp_list.clear();
        tmp_list.reserve(ITEMS_PER_PAGE);

    }

    qDebug() << "Done, mimic data:" << m_mimic_data;
}