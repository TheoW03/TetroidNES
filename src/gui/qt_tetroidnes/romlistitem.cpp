#include <QtLogging>

#include <Qt/util.h>
#include <Qt/romlistitem.h>

RomListItem::RomListItem(shptr_romdata data, QWidget *parent) : QWidget{parent}
{

    setMinimumSize(250, 250);

    auto *layout =         new QVBoxLayout();
    auto *buttons_layout = new QHBoxLayout();

    title =           new QLabel(this);
    play =            new QPushButton(this);
    year =            new QLabel(this);
    buttons_frame =   new QFrame(this);
    favorite_button = new QPushButton(buttons_frame);

    favorite_button->setCheckable(true);
    favorite_button->setText("Favorite button");

    title->setAlignment(Qt::AlignCenter);
    year->setAlignment(Qt::AlignCenter);

    layout->addWidget(buttons_frame);
    layout->addWidget(play);
    layout->addWidget(title);
    layout->addWidget(year);
    setLayout(layout);

    buttons_layout->addWidget(favorite_button);
    buttons_frame->setLayout(buttons_layout);

    set_romdata(data);

    qDebug() << m_romdata->title();

    // Events
    connect(favorite_button, &QPushButton::clicked, this,
            [this](int checked)
            { favorite_button_clicked(checked); });
    connect(play, &QPushButton::clicked, this, &RomListItem::play_button_clicked);
}

RomListItem::~RomListItem()
{
}

void RomListItem::set_romdata(shptr_romdata data)
{
    m_romdata = data;
    update_data();
}

shptr_romdata RomListItem::romdata()
{
    return m_romdata;
}

void RomListItem::favorite_button_clicked(int checked)
{
    RomList *rom_list = qobject_cast<RomList *>(parent());

    m_romdata->set_favorited(checked);

    // Refresh display
    if (rom_list->current_mode() == RomList::SortMode::Favorites)
    {
        rom_list->set_current_mode(RomList::SortMode::Favorites);
    }
}

void RomListItem::update_data()
{
    title->setText(m_romdata->title());
    year->setText(QString::number(m_romdata->year()));
    if (m_romdata->img().isNull())
    {
        auto placeholder_img = QPixmap(175, 175);
        placeholder_img.fill(Qt::darkGreen);
        play->setIcon(QIcon(placeholder_img));
        play->setIconSize(placeholder_img.size());
    }
    // else {play->setIcon(QIcon());)

    favorite_button->setChecked(m_romdata->favorited());
}

void RomListItem::play_button_clicked()
{

    auto path = m_romdata->path();

    if (!path.isValid())
    {
        qCritical() << "Path is not valid (Try restarting the program)";
        return;
    }

    qInfo() << "Starting" << title->text();
    qDebug() << "Path:" << path;
    start_game(path.toString());

}