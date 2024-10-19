#include <QApplication>
#include <QtLogging>

#include <gamedisplay.h>

#ifndef UTIL_H
#define UTIL_H

inline QString num_to_hexa(uint16_t num)
{
    return QString::number(num, 16).toUpper();
}

// inline QString num_to_hexa(uint8_t num)
// {
// return QString::number(num, 8).toUpper();
// }

inline bool is_a_game_running()
{

    for (auto &widget : qApp->topLevelWidgets())
    {
        if (widget->inherits("MainWindow"))
        {
            for (auto &gamedisplay : widget->findChildren<GameDisplay *>())
            {
                if (gamedisplay->initialized())
                {
                    return true;
                }
            }
        }
    }

    // Code reaches this point if all game display objects are not initialized
    return false;
}

#endif // UTIL_H