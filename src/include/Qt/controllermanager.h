#pragma once

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <SFML/System.hpp>

#include <Qt/util.h>
#include <Qt/inputmap.h>

/*
Use InputMap::Buttons enum when accessing,
these values will be written in the user's controller file
*/
static const char* CONTROLLER_MAP[BUTTON_COUNT]{
"A",
"B",
"Select",
"Start",
"Up",
"Down",
"Left",
"Right"
};

class ControllerManager : public QObject
{
    Q_OBJECT

    using Buttons = InputMap::Buttons;

public:

    static ControllerManager& instance()
    {
        static ControllerManager instance;
        return instance;
    }

    ControllerManager(const ControllerManager&) = delete;
    ControllerManager& operator=(const ControllerManager&) = delete;

    QStringList get_profile_names() const;

    void set_button(InputMap &input_map, Buttons button_id, InputButton new_button);
    void set_button(InputMap &old_input_map, InputMap &new_input_map);
    void change_name(QString old_name, QString new_name);
    void change_type(QString name, InputMap::Type type);

    InputButton from_json(QJsonArray &json_array, Buttons button_id);
    InputMap from_json(QString name);
    QJsonObject to_json(InputButton &button);
    QJsonObject to_json(InputMap &input_map);

    void load_json();
    void save_json();
    void clear_memory_json();
    void update_button_json(QString name, QJsonObject button, Buttons button_id);
    void update_map_json(QString name, QJsonObject input_map);

    static InputMap generate_default_input_map();
    static int get_button_gamepad(InputMap &input_map, Buttons button);
    static sf::Keyboard::Key get_button_keyboard(InputMap &input_map, Buttons button);

signals:
    void input_map_changed(InputMap new_input_map);

private:
    ControllerManager();

    void json_error(QJsonParseError error);

    QJsonDocument json;
};