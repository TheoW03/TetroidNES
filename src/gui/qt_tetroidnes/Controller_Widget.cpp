#include <Qt/inputsettingswidget.h>

#include <Qt/controllermanager.h>
#include <Qt/settingsmanager.h>

InputSettings::InputSettings(QWidget *parent) : QWidget{parent},
                                                display_widget(new InputSettingsDisplay(this)),
                                                header_widget(new QWidget(this)),
                                                header_input_profiles(new QComboBox(header_widget)),
                                                header_input_type(new QComboBox(header_widget))
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle("TetroidNES - " + tr("Controller Settings"));

    resize(400, 400);

    auto &controller_manager = ControllerManager::instance();
    auto profile_names = controller_manager.get_profile_names();
    auto active_profile_name = SettingsManager::instance().active_input_profile();
    InputMap active_inputmap = controller_manager.from_json(active_profile_name);


    auto *layout = new QVBoxLayout();
    auto *header_layout = new QHBoxLayout();

    header_input_type->addItems(QStringList({"Keyboard", "Gamepad"}));
    header_input_type->setCurrentIndex(active_inputmap.controller_type);
    header_layout->addWidget(header_input_type);

    header_input_profiles->addItems(profile_names);
    for (int i = 0; i < profile_names.length(); i++)
    {
        if (profile_names[i] == active_profile_name)
        {
            qDebug() << "Found active profile" << active_profile_name << "at index:" << QString::number(i);
            header_input_profiles->setCurrentIndex(i);
            break;
        }
    }
    display_widget->setup();
    header_input_profiles->setEditable(true);
    header_input_profiles->setInsertPolicy(QComboBox::NoInsert);
    header_layout->addWidget(header_input_profiles, 10);

    layout->addWidget(header_widget);
    layout->addWidget(display_widget);

    header_widget->setLayout(header_layout);
    setLayout(layout);

    // Events

    connect(header_input_type, &QComboBox::currentIndexChanged, this, &InputSettings::on_type_combobox_current_index_changed);
    connect(header_input_profiles, &QComboBox::currentIndexChanged, this, &InputSettings::on_profile_combobox_current_index_changed);
    connect(header_input_profiles->lineEdit(), &QLineEdit::editingFinished, this, &InputSettings::on_profile_editing_finished);
}

void InputSettings::on_type_combobox_current_index_changed(int idx)
{
    auto &controller_manager = ControllerManager::instance();
    controller_manager.change_type(header_input_profiles->currentText(), InputMap::Type(idx));
    controller_manager.save_json();
    display_widget->setup();
}

void InputSettings::on_profile_editing_finished()
{
    auto &controller_manager = ControllerManager::instance();
    auto current_text = header_input_profiles->currentText();
    auto name_list = controller_manager.get_profile_names();
    //qDebug() << name_list << "Current index:" << QString::number(header_input_profiles->currentIndex());
    auto old_name = name_list[header_input_profiles->currentIndex()];

    if (old_name == current_text)
    {
        qInfo() << "Cannot change profile name, the old and new name are the same!";
        return;
    }

    SettingsManager::instance().set_active_input_profile(current_text);
    controller_manager.change_name(old_name, current_text);
    controller_manager.save_json();
}

void InputSettings::on_profile_combobox_current_index_changed(int idx)
{
    SettingsManager::instance().set_active_input_profile(header_input_profiles->currentText());
}

InputSettings::~InputSettings()
{
}