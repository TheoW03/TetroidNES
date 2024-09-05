#include "romlist.h"

romlist::romlist(QWidget *parent): QListWidget{parent}
{
    setObjectName("RomList");
    setSelectionBehavior(SelectionBehavior::SelectItems);
    setSelectionMode(SelectionMode::ExtendedSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setEditTriggers(NoEditTriggers);
    setFlow(Flow::LeftToRight);
    setWrapping(true);
    setResizeMode(ResizeMode::Adjust);

    setupList();
    setCurrentMode(romlist::SortMode::AZ); // TODO: Change this so the program remembers what the user chose last time
    setCurrentOrder(Qt::SortOrder::AscendingOrder); // TODO: Change this so the program remembers what the user chose last time
}

void romlist::setupList()
{
    /*
     * Get roms via folder location(s) in config
     * Maybe add some sort of visual seperator for each folder?
     * Check if rom is valid with different function
     * Use "this->addItem()" with a QListWidgetItem inherited object
     * QListWidgetItem inherited object will handle custom data like if it's favorited
    */
    this->addItem("Super Mario Brothers: 3");
    this->addItem("MegaMan 2");
    this->addItem("MegaMan");
    this->addItem("Super Mario Brothers");
    this->addItem("Castlevania");
    this->addItem("The Legend of Zelda");
    this->addItem("Friday The 13th");
    this->addItem("Dr.Mario");
    this->addItem("Duck Hunt");
}


void romlist::setCurrentMode(romlist::SortMode mode)
{
    if (current_mode == mode)
        return;

    current_mode = mode;
    // Code that applies sort mode
}

romlist::SortMode romlist::CurrentMode()
{return current_mode;}

void romlist::setCurrentOrder(Qt::SortOrder order)
{
    if (current_order == order)
        return;

    current_order = order;
    sortItems(order);
}

Qt::SortOrder romlist::CurrentOrder()
{return current_order;}

romlist::~romlist()
{

}
