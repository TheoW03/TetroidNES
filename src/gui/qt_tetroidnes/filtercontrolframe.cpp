#include "filtercontrolframe.h"
#include "romlist.h"

filtercontrolframe::filtercontrolframe(QWidget *parent) : QFrame{parent}
{
    QLineEdit *search_bar =                  new QLineEdit(this);
    QHBoxLayout *sort_control_frame_layout = new QHBoxLayout();
    QFrame *sort_buttons_frame =             new QFrame(this);
    QVBoxLayout *sort_buttons_frame_layout = new QVBoxLayout();
    QPushButton *sort_ascending_button =     new QPushButton(tr("Sort"), sort_buttons_frame);
    QGroupBox *sort_mode_groupbox =          new QGroupBox(sort_buttons_frame);
    QHBoxLayout *groupbox_layout =           new QHBoxLayout();
    QPushButton *sort_mode_az =              new QPushButton(tr("A-Z"), sort_mode_groupbox);
    QPushButton *sort_mode_year =            new QPushButton(tr("Year"), sort_mode_groupbox);
    QPushButton *sort_mode_favorites =       new QPushButton(tr("Favorites"), sort_mode_groupbox);
    QButtonGroup *sort_mode_button_group =   new QButtonGroup(sort_mode_groupbox);

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
    this->setLayout(sort_control_frame_layout);

    // setup buttons
    sort_ascending_button->setCheckable(true);
    sort_ascending_button->setObjectName("SortOrder");
    sort_mode_az->setCheckable(true);
    sort_mode_favorites->setCheckable(true);
    sort_mode_year->setCheckable(true);

    // setup button group
    sort_mode_button_group->setExclusive(true);
    sort_mode_button_group->addButton(sort_mode_year, romlist::SortMode::Year);
    sort_mode_button_group->addButton(sort_mode_favorites, romlist::SortMode::Favorites);
    sort_mode_button_group->addButton(sort_mode_az, romlist::SortMode::AZ);

    // setup search bar
    search_bar->setPlaceholderText(tr("Search..."));

    //qInfo() << QDebug::toString(sort_mode_button_group);
}
