#include <Qt/controllersettingswidget.h>

#include <QMessageBox>
#include <QInputDialog>
#include <QErrorMessage>
#include <QListView>

#include <Qt/controllermanager.h>
#include <Qt/settingsmanager.h>

const auto HEADER_INPUT_TYPE_ITEMS = QStringList({"Keyboard", "Gamepad"});

const auto ICON_ADD_PROFILE = QIcon::fromTheme(QIcon::ThemeIcon::ListAdd);
const auto ICON_REMOVE_PROFILE = QIcon::fromTheme(QIcon::ThemeIcon::ListRemove);

InputSettings::InputSettings(QWidget *parent) : QWidget{parent},
                                                display_widget(new InputSettingsDisplay(this)),
                                                header_widget(new QWidget(this)),
                                                header_input_profiles(new QComboBox(header_widget)),
                                                header_input_type(new QComboBox(header_widget)),
                                                header_edit_profile_name(new QPushButton(tr("Edit"), header_widget)),
                                                header_add_profile(new QPushButton(ICON_ADD_PROFILE, "", header_widget)),
                                                header_remove_profile(new QPushButton(ICON_REMOVE_PROFILE, "", header_widget))
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle("TetroidNES - " + tr("Controller Settings"));

    resize(400, 400);

    auto &controller_manager = ControllerManager::instance();
    auto profile_names = controller_manager.get_profile_names();
    auto profile_names_length = profile_names.length();
    auto active_profile_name = SettingsManager::instance().active_input_profile();
    InputMap active_inputmap = controller_manager.from_json(active_profile_name);

    auto *layout = new QVBoxLayout();
    auto *header_layout = new QHBoxLayout();

    header_input_type->addItems(HEADER_INPUT_TYPE_ITEMS);
    header_layout->addWidget(header_input_type);

    refresh_profiles_selection();
    update_add_remove_buttons();

    header_layout->addWidget(header_input_profiles, 20);
    header_layout->addWidget(header_edit_profile_name);
    header_layout->addWidget(header_add_profile);
    header_layout->addWidget(header_remove_profile);

    layout->addWidget(header_widget);
    layout->addWidget(display_widget);

    header_widget->setLayout(header_layout);
    setLayout(layout);

    // Events

    connect(header_input_type, &QComboBox::activated, this, &InputSettings::on_type_combobox_activated);
    connect(header_input_profiles, &QComboBox::currentIndexChanged, this, &InputSettings::update_type_combobox);
    connect(header_input_profiles, &QComboBox::activated, this, &InputSettings::on_profile_combobox_activated);
    connect(header_input_profiles, &QComboBox::activated, this, &InputSettings::update_type_combobox);
    connect(header_edit_profile_name, &QPushButton::clicked, this, &InputSettings::on_edit_profile_clicked);
    connect(header_add_profile, &QPushButton::clicked, this, &InputSettings::on_add_profile_clicked);
    connect(header_remove_profile, &QPushButton::clicked, this, &InputSettings::on_remove_profile_clicked);
}

void InputSettings::on_remove_profile_clicked()
{
    auto &controller_manager = ControllerManager::instance();
    auto &settings_manager = SettingsManager::instance();

    QMessageBox::StandardButton result = QMessageBox::question(
        this,
        "TetroidNES - " + tr("Input profile deletion confirmation"),
        tr("Are you sure you want to delete this profile?") + "\n" + tr("This cannot be undone!"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result != QMessageBox::Yes)
    {
       return;
    }

    auto profiles = controller_manager.get_profile_names();
    auto profile_to_be_deleted = settings_manager.active_input_profile();
    QString new_active_profile;

    for (auto &profile : profiles)
    {
        if (profile != profile_to_be_deleted)
        {
            new_active_profile = profile;
            break;
        }
    }

    if (new_active_profile.isEmpty())
    {
        QMessageBox::information(
            this,
            "Tetroid NES",
            tr("Tried to assign a new active profile before removing the current one, but could not find one")
        );
        return;
    }

    settings_manager.set_active_input_profile(new_active_profile);

    controller_manager.remove_profile(profile_to_be_deleted);
    controller_manager.save_json();

    header_input_profiles->removeItem(header_input_profiles->currentIndex());
    update_add_remove_buttons();

}

void InputSettings::on_add_profile_clicked()
{
    bool ok;
    auto &controller_manager = ControllerManager::instance();
    QStringList keys = controller_manager.get_profile_names();
    QString new_profile_name;

    new_profile_name = QInputDialog::getText(
        this,
        "TetroidNES - " + tr("Add new profile"),
        tr("Enter a new profile name:"),
        QLineEdit::Normal,
        "",
        &ok
    );

    if (keys.contains(new_profile_name))
    {
        QMessageBox::information(
            this,
            "Tetroid NES",
            tr("Cannot add duplicate profile names!")
        );
    }
    else if (new_profile_name.isEmpty())
    {
        QMessageBox::information(
            this,
            "Tetroid NES",
            tr("Invalid entry: Empty string")
        );
    }
    else if (ok)
    {
        SettingsManager::instance().set_active_input_profile(new_profile_name);
        controller_manager.add_profile(new_profile_name);
        controller_manager.save_json();
        
        keys = controller_manager.get_profile_names();
        const int keys_length = keys.length();
        int idx;

        for (int i = 0; i < keys_length; i++)
        {
            if (keys[i] == new_profile_name)
            {
                idx = i;
                break;
            }
        }

        header_input_profiles->insertItem(idx, new_profile_name);
        header_input_profiles->setCurrentIndex(idx);

        update_add_remove_buttons();
        display_widget->setup();
    }
}

void InputSettings::on_type_combobox_activated(int idx)
{
    auto &controller_manager = ControllerManager::instance();
    controller_manager.change_type(header_input_profiles->currentText(), InputMap::Type(idx));
    controller_manager.save_json();
}

void InputSettings::on_profile_combobox_activated(int idx)
{

    SettingsManager::instance().set_active_input_profile(header_input_profiles->currentText());
    display_widget->setup();

}

void InputSettings::refresh_profiles_selection()
{
    qDebug() << "Refreshing profile selection...";

    auto &controller_manager = ControllerManager::instance();
    auto profile_names = controller_manager.get_profile_names();
    auto profile_names_length = profile_names.length();
    auto active_profile_name = SettingsManager::instance().active_input_profile();
    int idx;

    qDebug() << "Clearing items";
    header_input_profiles->clear();
    qDebug() << "Adding items";
    header_input_profiles->addItems(profile_names);

    idx = header_input_profiles->findText(active_profile_name);
    qDebug() << "Finding new current index:" << QString::number(idx);
    header_input_profiles->setCurrentIndex(idx);

    display_widget->setup();

    qDebug() << "Done";
}

void InputSettings::update_add_remove_buttons()
{
    const int profile_count = header_input_profiles->count();
    const bool enable_remove_profile_button = profile_count > 1;
    const bool enable_add_profile_button = profile_count < header_input_profiles->maxCount();

    header_remove_profile->setEnabled(enable_remove_profile_button);
    header_add_profile->setEnabled(enable_add_profile_button);
}

void InputSettings::update_type_combobox()
{
    auto &controller_manager = ControllerManager::instance();
    auto active_profile_name = SettingsManager::instance().active_input_profile();
    auto active_inputmap = controller_manager.from_json(active_profile_name);

    header_input_type->setCurrentIndex(active_inputmap.controller_type);
}

void InputSettings::on_edit_profile_clicked()
{
    bool ok;
    auto &controller_manager = ControllerManager::instance();
    QString old_name = header_input_profiles->currentText();
    //qDebug() << name_list << "Current index:" << QString::number(header_input_profiles->currentIndex());

    auto new_text = QInputDialog::getText(
        this,
        "TetroidNES",
        tr("Enter a new name for ") + old_name + ":",
        QLineEdit::Normal,
        old_name,
        &ok
    );

    if (new_text.isEmpty())
    {
        QMessageBox::information(
            this,
            "Tetroid NES",
            tr("Cannot change profile name, selection is empty!")
        );
    }
    else if (old_name == new_text)
    {
        QMessageBox::information(
            this,
            "Tetroid NES",
            tr("Cannot change profile name, the old and new name are the same!")
        );
    }
    else if (ok)
    {
        SettingsManager::instance().set_active_input_profile(new_text);
        controller_manager.change_name(old_name, new_text);
        controller_manager.save_json();

        header_input_profiles->setItemText(header_input_profiles->currentIndex(), new_text);
    }
}

InputSettings::~InputSettings()
{
}