#include <QGuiApplication>
#include <gamedisplay.h>

#ifndef UTIL_H
#define UTIL_H

inline QString num_to_hexa(uint16_t num)
{
    return QString::number(num, 16).toUpper();
}

inline QString num_to_hexa(uint8_t num)
{
    return QString::number(num, 8).toUpper();
}

inline bool is_a_game_running()
{
    auto list = qApp->findChildren<GameDisplay*>();

    if (list.isEmpty())
    {
        return false;
    }

    foreach(auto game_display, list)
    {
        if (game_display->initialized()){return true;}
    }

    // Code reaches this point if all game display objects are not initialized
    return false;
}

#endif // UTIL_H