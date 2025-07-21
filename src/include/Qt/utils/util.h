#pragma once

#include <QApplication>
#include <QtLogging>

#define SAVE_DIR "./save/config.cfg"
#define CONTROLS_SAVE_DIR "./save/controls.json"

enum SortMode {Year, Favorites, AZ};

inline std::string num_to_hexa(uint16_t num)
{
    return QString::number(num, 16).toUpper().toStdString();
}

// inline QString num_to_hexa(uint8_t num)
// {
// return QString::number(num, 8).toUpper();
// }
