#include <Qt/filtercontrolframe.h>
#include <Qt/romlist.h>

FilterControlFrame::FilterControlFrame(QWidget *parent) : QFrame{parent}
{
    QHBoxLayout *sort_control_frame_layout = new QHBoxLayout();
    QVBoxLayout *sort_buttons_frame_layout = new QVBoxLayout();
    QHBoxLayout *groupbox_layout =           new QHBoxLayout();

    search_bar =             new QLineEdit(this);
    sort_buttons_frame =     new QFrame(this);
    sort_ascending_button =  new QPushButton(tr("Sort"), sort_buttons_frame);
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
    sort_ascending_button->setObjectName("SortOrder");
    sort_mode_az->setCheckable(true);
    sort_mode_favorites->setCheckable(true);
    sort_mode_year->setCheckable(true);

    // setup button group
    sort_mode_button_group->setExclusive(true);
    sort_mode_button_group->addButton(sort_mode_year, RomList::SortMode::Year);
    sort_mode_button_group->addButton(sort_mode_favorites, RomList::SortMode::Favorites);
    sort_mode_button_group->addButton(sort_mode_az, RomList::SortMode::AZ);

    sort_mode_az->setChecked(true); // TODO: Make this more dynamic

    // setup search bar
    search_bar->setPlaceholderText(tr("Search..."));
}
