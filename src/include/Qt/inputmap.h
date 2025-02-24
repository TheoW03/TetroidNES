#pragma once

#include <QString>

constexpr const int BUTTON_COUNT = 8;

struct InputButton
{
    InputButton()
    {
        key = -1;
        display_name = "N/A";
    }
    InputButton(QString name, int code)
    {
        display_name = name;
        key = code;
    }

    QString display_name;
    int key;
};

struct InputMap
{
    enum Buttons {Button0, Button1, Button2, Button3, Button4, Button5, Button6, Button7};
    static inline const Buttons AllButtons[] = {Button0, Button1, Button2, Button3, Button4, Button5, Button6, Button7};
    enum Type {Keyboard, Gamepad};

    InputMap()
    {
        controller_type = Type::Keyboard;

        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            buttons[i] = InputButton();
        };
    }
    InputMap(QString profile_name, Type type)
    {
        name = profile_name;
        controller_type = type;
        
        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            buttons[i] = InputButton();
        };
    }
    InputMap(QString profile_name, Type type, InputButton new_inputs[BUTTON_COUNT])
    {
        name = profile_name;
        controller_type = type;

        for(int i = 0; i < BUTTON_COUNT; i++)
        {
            buttons[i] = new_inputs[i];
        };
    }

    Type controller_type;
    QString name;

    InputButton buttons[BUTTON_COUNT];
};