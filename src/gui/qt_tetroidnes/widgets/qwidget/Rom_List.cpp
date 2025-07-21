#include <Qt/widgets/qwidget/rom_list.h>

#include <QtLogging>
#include <QRegularExpression>
#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include <QMessageBox>

#include <Qt/objects/settings_manager.h>
#include <Qt/objects/rom_data.h>
#include <Qt/widgets/qwidget/rom_list_item.h>
#include <Qt/utils/util.h>


RomList::RomList(QWidget *parent) : QWidget{parent},
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

    on_rom_dirs_changed();

    set_current_mode(settings.sort_mode());
    set_current_order(settings.ascending_order());

    connect(&settings, &SettingsManager::rom_dirs_changed, this, &RomList::on_rom_dirs_changed);

    qInfo() << "Finished setting up romlist";
}

void RomList::on_rom_dirs_changed()
{
    setup_display();
    update_total_pages();
    m_current_page = 1;
    update_display();
}

void RomList::setup_display()
{
    data.update_raw_data();
    update_display();
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
    m_total_pages = static_cast<uint32_t>(data.get_mimic_data()->length());
}
uint32_t RomList::total_pages() const { return m_total_pages; }

// Probably needs another refactor?
void RomList::update_display()
{
    auto list_of_widgets = findChildren<RomListItem *>();
    qDeleteAll(list_of_widgets);

    const int true_page = m_current_page - 1;
    const auto *mimic_data = data.get_mimic_data();
    const int data_size = mimic_data->size();

    /*qDebug()
    << "Applying data to widgets"
    << "\nMimic Romdata" << *data.get_mimic_data()
    << "\nMimic RomData Page size:" << data_size
    << "\nCurrent page:" << m_current_page
    << "\nTrue page:" << true_page;*/

    // Apply data
    for (auto &rom_data : mimic_data->at(true_page))
    {
        //qDebug() << "Getting rom data...";

        auto *romlistitem = new RomListItem(*rom_data, this);

        //qDebug() << "Getting rom list item to populate with data";

        layout()->addWidget(romlistitem);

        romlistitem->set_romdata(*rom_data);

        romlistitem->show();

    }

    qDebug() << "Done updating display";
}

void RomList::search(QString &expr)
{
    auto regular_expression = QRegularExpression(expr);

    data.sort_data(regular_expression, current_order(), current_mode());

    update_display();
}

void RomList::set_current_mode(const SortMode &mode, const bool update)
{
    m_current_mode = mode;
    if (!update)
    {
        return;
    }

    switch (mode)
    {
    case Year:
        data.sort_data(ComparePtrs::CMP_YEAR, m_current_order);
        break;
    case Favorites:
        data.sort_data(ComparePtrs::CMP_FAV, m_current_order);
        break;
    case AZ:
        data.sort_data(ComparePtrs::CMP_ABC, m_current_order);
        break;
    }

    update_display();
}

SortMode RomList::current_mode() const
{
    return m_current_mode;
}

void RomList::set_current_order(const Qt::SortOrder order)
{
    if (m_current_order == order)
        return;

    m_current_order = order;
    data.sort_data_reverse();

    update_display();
}

Qt::SortOrder RomList::current_order() const
{
    return m_current_order;
}

RomList::~RomList()
{
}
