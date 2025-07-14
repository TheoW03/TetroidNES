#pragma once

#include <QString>
#include <QStringLiteral>

#include <SFML/Window/Keyboard.hpp>

const QString N_A = QStringLiteral("N/A");

struct InputButton
{
    InputButton()
    {
        key = -1;
        display_name = N_A;
    }
    InputButton(QString name, int code)
    {
        display_name = name;
        key = code;
    }
    InputButton(QString name, sf::Keyboard::Key code)
    {
        display_name = name;
        key = static_cast<int>(code);
    }

    QString display_name;
    int key;
};