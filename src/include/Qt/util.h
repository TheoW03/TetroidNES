#pragma once

#include <QApplication>
#include <QtLogging>

#include <Qt/gamedisplay.h>
#include <Qt/mainwindow.h>

#define SAVE_DIR "./save/config.cfg"

inline std::string num_to_hexa(uint16_t num)
{
    return QString::number(num, 16).toUpper().toStdString();
}

// inline QString num_to_hexa(uint8_t num)
// {
// return QString::number(num, 8).toUpper();
// }

inline bool is_a_game_running()
{

    for (auto &widget : qApp->topLevelWidgets())
    {
        if (widget->inherits("GameDisplay"))
        {
            if (qobject_cast<GameDisplay *>(widget)->initialized())
            {
                return true;
            }
        }
    }

    // Code reaches this point if all game display objects are not initialized
    return false;
}

inline void start_game(QString path)
{
    if (is_a_game_running())
    {
        qInfo() << "Can't open game while a game is already running";
        return;
    }
    for (auto &widget : qApp->topLevelWidgets())
    {
        if (widget->inherits("MainWindow"))
        {
            qDebug() << "File path:" << path;
            qobject_cast<MainWindow *>(widget)->create_display(path);
            break;
        }
    }
}