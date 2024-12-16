#include <Qt/filtercontrolframe.h>
#include <Qt/romlist.h>
#include <Qt/settingsmanager.h>

FilterControlFrame::FilterControlFrame(QWidget *parent) : QFrame{parent}
{
    auto &settings = SettingsManager::instance();
    const auto sort_mode = settings.sort_mode();
    const auto sort_order = settings.ascending_order();

    QHBoxLayout *sort_control_frame_layout = new QHBoxLayout();
    QVBoxLayout *sort_buttons_frame_layout = new QVBoxLayout();
    QHBoxLayout *groupbox_layout =           new QHBoxLayout();

    search_bar =             new QLineEdit(this);
    sort_buttons_frame =     new QFrame(this);
    sort_ascending_button =  new QPushButton(tr("Ascending"), sort_buttons_frame);
    sort_mode_groupbox =     new QGroupBox(sort_buttons_frame);
    sort_mode_az =           new QPushButton(tr("A-Z"), sort_mode_groupbox);
    sort_mode_year =         new QPushButton(tr("Year"), sort_mode_groupbox);
    sort_mode_favorites =    new QPushButton(tr("Favorites"), sort_mode_groupbox);
    sort_mode_button_group = new QButtonGroup(sort_mode_groupbox);

    // setup sort buttons frame layout
    sort_buttons_frame_layout->addWidget(sort_ascending_button);
    sort_buttons_frame_layout->addWidget(sort_mode_groupbox);
    sort_buttons_frame->setLayout(sort_buttons_frame_layout);

    // setup group box layout
    groupbox_layout->addWidget(sort_mode_year);
    groupbox_layout->addWidget(sort_mode_favorites);
    groupbox_layout->addWidget(sort_mode_az);
    sort_mode_groupbox->setLayout(groupbox_layout);

    // setup this layout
    sort_control_frame_layout->addWidget(search_bar);
    sort_control_frame_layout->addWidget(sort_buttons_frame);
    setLayout(sort_control_frame_layout);

    // setup buttons
    sort_ascending_button->setCheckable(true);
    sort_ascending_button->setChecked(!sort_order); // Ascending == 0, Descending == 1
    sort_ascending_button->setObjectName("SortOrder");
    sort_mode_az->setCheckable(true);
    sort_mode_favorites->setCheckable(true);
    sort_mode_year->setCheckable(true);

    // setup button group
    sort_mode_button_group->setExclusive(true);
    sort_mode_button_group->addButton(sort_mode_year, RomList::Year);
    sort_mode_button_group->addButton(sort_mode_favorites, RomList::Favorites);
    sort_mode_button_group->addButton(sort_mode_az, RomList::AZ);

    switch (sort_mode)
    {
        case RomList::Year:
            sort_mode_year->setChecked(true);
            break;
        case RomList::Favorites:
            sort_mode_favorites->setChecked(true);
            break;
        case RomList::AZ:
            sort_mode_az->setChecked(true);
            break;
        default:
            qWarning() << "Sort mode enum given was invalid! Int given:" << sort_mode;
            sort_mode_az->setChecked(true);
            break;
    }

    // setup search bar
    search_bar->setPlaceholderText(tr("Search..."));
}
