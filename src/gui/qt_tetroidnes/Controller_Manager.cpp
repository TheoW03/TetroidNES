#include <Qt/controllermanager.h>

#include <QFile>
#include <QJsonParseError>

using Buttons = InputMap::Buttons;

constexpr const char* TYPE = "type";
constexpr const char* NAME = "name";
constexpr const char* KEY = "key";
constexpr const char* BUTTONS = "buttons";

ControllerManager::ControllerManager()
{
    qInfo() << "Constructing ControllerManager singleton";

    load_json();

    const bool is_null = json.isNull();
    const bool is_object = json.isObject();
    const bool is_object_empty = json.object().isEmpty();

    if (is_null || !is_object || is_object_empty)
    {
        qInfo() 
        << "Controller settings are empty! Resetting to default"
        << "Is null:" << QString::number(is_null)
        << "Is JSON Object:" << QString::number(is_object)
        << "Is JSON Object Empty:" << QString::number(is_object_empty);
        
        //qDebug() << "Generating default input map";
        auto default_input_map = ControllerManager::generate_default_input_map();
        //qDebug() << "Converting default input map into a QJsonObject";
        auto default_input_map_json = to_json(default_input_map);

        //qDebug() 
        //<< "Is default input map empty:"
        //<< QString::number(default_input_map_json.isEmpty());

        update_map_json(default_input_map_json);
        save_json();
    }
}

void ControllerManager::set_button(InputMap &input_map, Buttons button_id, InputButton new_button)
{
    qDebug() << "Setting new button" << input_map.name << button_id << new_button.display_name;
    input_map.buttons[button_id] = new_button;

    auto button_json = to_json(new_button);

    update_button_json(input_map.name, button_json, button_id);

    emit input_map_changed(input_map);
}

void ControllerManager::set_button(InputMap &old_input_map, InputMap &new_input_map)
{
    old_input_map = new_input_map;

    update_map_json(to_json(new_input_map));
    emit input_map_changed(new_input_map);
}

InputMap ControllerManager::from_json(QString name)
{
    QJsonObject object = json.object();
    QJsonObject json_input_map = object[name].toObject();

    QJsonArray json_input_buttons = json_input_map[BUTTONS].toArray();
    QString input_name = json_input_map[NAME].toString();
    auto type = InputMap::Type(json_input_map[TYPE].toInt());

    InputButton buttons[BUTTON_COUNT] = {};
    for(int i = 0; i < BUTTON_COUNT; i++)
    {
        buttons[i] = from_json(json_input_buttons, Buttons(i));
    }

    qDebug()
    << "ControllerManager::from_json"
    << "Serializing InputMap, Name:" << input_name
    << "Type:" << QString::number(type);
    
    return InputMap(input_name, type, buttons);
}

InputButton ControllerManager::from_json(QJsonArray &json_array, Buttons button_id)
{
    QJsonObject button_data_json = json_array[button_id].toObject();
    
    QString button_name = button_data_json[NAME].toString();
    int key = button_data_json[KEY].toInt();
    
    return InputButton(button_name, key);
}

QJsonObject ControllerManager::to_json(InputButton &button)
{
    qDebug() << "Converting button to QJsonObject...";
    QJsonObject json_object;
    json_object[NAME] = button.display_name;
    json_object[KEY] = button.key;
    qDebug() << "Done";

    return json_object;
}

QJsonObject ControllerManager::to_json(InputMap &input_map)
{
    QJsonObject json_object;
    QJsonArray json_buttons;
    json_object[TYPE] = input_map.controller_type;
    json_object[NAME] = input_map.name;

    qDebug() << "Assigning buttons";
    for(int i = 0; i < BUTTON_COUNT; i++)
    {
        QJsonObject json_button = to_json(input_map.buttons[i]);
        json_buttons.append(json_button);
    }
    qDebug() << "Done assigning buttons";
    json_object[BUTTONS] = json_buttons;

    return json_object;
}

int ControllerManager::get_button_gamepad(InputMap &input_map, Buttons button)
{
    return input_map.buttons[button].key;
}

sf::Keyboard::Key ControllerManager::get_button_keyboard(InputMap &input_map, Buttons button)
{
    return sf::Keyboard::Key(input_map.buttons[button].key);
}

void ControllerManager::load_json()
{
    QFile file(CONTROLS_SAVE_DIR);
    if (!file.exists())
    {
        qInfo() << file.fileName() << "doesn't exist! Creating...";
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.write(QByteArray("{}"));
        file.close();
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Failed to open" << file.fileName() << file.errorString();
        return;
    }

    auto file_contents = file.readAll();
    file.close();

    auto json_parse_error = QJsonParseError();
    json = QJsonDocument::fromJson(file_contents, &json_parse_error);
    json_error(json_parse_error);
}

void ControllerManager::save_json()
{
    QFile file(CONTROLS_SAVE_DIR);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qCritical() << "Failed to open" << file.fileName() << file.errorString();
        file.close();
        return;
    }

    file.write(json.toJson());
    file.close();
}

void ControllerManager::clear_memory_json()
{
    json.setObject(QJsonObject());

    emit profile_order_changed();
}

void ControllerManager::update_button_json(QString name, QJsonObject button, Buttons button_id)
{
    auto object = json.object();
    auto input_map_json = object[name].toObject();
    auto buttons_json = input_map_json[BUTTONS].toArray();

    buttons_json[button_id] = button;
    input_map_json[BUTTONS] = buttons_json;
    object[name] = input_map_json;

    json.setObject(object);
}

void ControllerManager::update_map_json(QJsonObject input_map)
{
    auto object = json.object();
    auto profile_name = input_map[NAME].toString();
    object[profile_name] = input_map;
    json.setObject(object);
}

InputMap ControllerManager::generate_default_input_map()
{
    InputButton buttons[BUTTON_COUNT] = {
        InputButton("Z", sf::Keyboard::Z),
        InputButton("C", sf::Keyboard::C),
        InputButton(",", sf::Keyboard::Comma),
        InputButton(".", sf::Keyboard::Period),
        InputButton("Up", sf::Keyboard::Up),
        InputButton("Down", sf::Keyboard::Down),
        InputButton("Left", sf::Keyboard::Left),
        InputButton("Right", sf::Keyboard::Right)
    };

    return InputMap("Default", InputMap::Keyboard, buttons);
}

void ControllerManager::json_error(QJsonParseError error)
{
    if (error.error != QJsonParseError::NoError)
    {
        qCritical() 
        << "An error occured parsing"
        << CONTROLS_SAVE_DIR
        << error.errorString();
    }
}

QStringList ControllerManager::get_profile_names() const
{
    return json.object().keys();
}

void ControllerManager::change_name(QString old_name, QString new_name)
{
    qInfo() << "Changing name of" << old_name << "to" << new_name;
    auto object = json.object();
    auto profile = object[old_name].toObject();

    profile[NAME] = new_name;

    object[new_name] = profile;
    object.remove(old_name);

    json.setObject(object);
}

void ControllerManager::change_type(QString name, InputMap::Type type)
{
    auto object = json.object();
    auto profile = object[name].toObject();

    profile[TYPE] = type;

    object[name] = profile;

    json.setObject(object);
}

bool ControllerManager::profile_exists(QString key) const
{
    return json.object().contains(key);
}

void ControllerManager::remove_profile(QString key)
{
    auto object = json.object();
    object.remove(key);

    json.setObject(object);

    emit profile_order_changed();
}

void ControllerManager::add_profile(QString key)
{
    auto object = json.object();
    auto new_profile = ControllerManager::generate_default_input_map();
    auto new_profile_json = to_json(new_profile);

    new_profile_json[NAME] = key;
    object[key] = new_profile_json;

    json.setObject(object);

    emit profile_order_changed();
}

void ControllerManager::add_profile(InputMap &input_map)
{
    auto object = json.object();
    auto new_profile_json = to_json(input_map);

    object[input_map.name] = new_profile_json;

    json.setObject(object);

    emit profile_order_changed();
}