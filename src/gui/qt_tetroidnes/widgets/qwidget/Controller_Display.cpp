#include <Qt/widgets/qwidget/input_settings_display.h>

#include <QList>
#include <QFormLayout>
#include <QMessageBox>

#include <Qt/objects/controller_manager.h>
#include <Qt/objects/settings_manager.h>

constexpr const char* BUTTON_ID = "button_id";
constexpr const char* AWAITING_INPUT = "...";
constexpr const uint BATCH_ASSIGN_START = 0;

const std::unordered_map<Qt::Key, sf::Keyboard::Key> QT_TO_SF_MAP = {
    // Number Keys
    {Qt::Key_0, sf::Keyboard::Key::Num0},
    {Qt::Key_1, sf::Keyboard::Key::Num1},
    {Qt::Key_2, sf::Keyboard::Key::Num2},
    {Qt::Key_3, sf::Keyboard::Key::Num3},
    {Qt::Key_4, sf::Keyboard::Key::Num4},
    {Qt::Key_5, sf::Keyboard::Key::Num5},
    {Qt::Key_6, sf::Keyboard::Key::Num6},
    {Qt::Key_7, sf::Keyboard::Key::Num7},
    {Qt::Key_8, sf::Keyboard::Key::Num8},
    {Qt::Key_9, sf::Keyboard::Key::Num9},
    // F Keys
    {Qt::Key_F1, sf::Keyboard::Key::F1},
    {Qt::Key_F2, sf::Keyboard::Key::F2},
    {Qt::Key_F3, sf::Keyboard::Key::F3},
    {Qt::Key_F4, sf::Keyboard::Key::F4},
    {Qt::Key_F5, sf::Keyboard::Key::F5},
    {Qt::Key_F6, sf::Keyboard::Key::F6},
    {Qt::Key_F7, sf::Keyboard::Key::F7},
    {Qt::Key_F8, sf::Keyboard::Key::F8},
    {Qt::Key_F9, sf::Keyboard::Key::F9},
    {Qt::Key_F10, sf::Keyboard::Key::F10},
    {Qt::Key_F11, sf::Keyboard::Key::F11},
    {Qt::Key_F12, sf::Keyboard::Key::F12},
    // Alphabet Keys
    {Qt::Key_A, sf::Keyboard::Key::A},
    {Qt::Key_B, sf::Keyboard::Key::B},
    {Qt::Key_C, sf::Keyboard::Key::C},
    {Qt::Key_D, sf::Keyboard::Key::D},
    {Qt::Key_E, sf::Keyboard::Key::E},
    {Qt::Key_F, sf::Keyboard::Key::F},
    {Qt::Key_G, sf::Keyboard::Key::G},
    {Qt::Key_H, sf::Keyboard::Key::H},
    {Qt::Key_I, sf::Keyboard::Key::I},
    {Qt::Key_J, sf::Keyboard::Key::J},
    {Qt::Key_K, sf::Keyboard::Key::K},
    {Qt::Key_L, sf::Keyboard::Key::L},
    {Qt::Key_M, sf::Keyboard::Key::M},
    {Qt::Key_N, sf::Keyboard::Key::N},
    {Qt::Key_O, sf::Keyboard::Key::O},
    {Qt::Key_P, sf::Keyboard::Key::P},
    {Qt::Key_Q, sf::Keyboard::Key::Q},
    {Qt::Key_R, sf::Keyboard::Key::R},
    {Qt::Key_S, sf::Keyboard::Key::S},
    {Qt::Key_T, sf::Keyboard::Key::T},
    {Qt::Key_U, sf::Keyboard::Key::U},
    {Qt::Key_V, sf::Keyboard::Key::V},
    {Qt::Key_W, sf::Keyboard::Key::W},
    {Qt::Key_X, sf::Keyboard::Key::X},
    {Qt::Key_Y, sf::Keyboard::Key::Y},
    {Qt::Key_Z, sf::Keyboard::Key::Z},
    // Arrow Keys
    {Qt::Key_Left, sf::Keyboard::Key::Left},
    {Qt::Key_Right, sf::Keyboard::Key::Right},
    {Qt::Key_Down, sf::Keyboard::Key::Down},
    {Qt::Key_Up, sf::Keyboard::Key::Up},
    // Misc Keys
    {Qt::Key_Equal, sf::Keyboard::Key::Equal},
    {Qt::Key_Minus, sf::Keyboard::Key::Subtract},
    {Qt::Key_Bar, sf::Keyboard::Key::Space},
    {Qt::Key_Alt, sf::Keyboard::Key::LAlt},
    {Qt::Key_Semicolon, sf::Keyboard::Key::Semicolon},
    {Qt::Key_QuoteLeft, sf::Keyboard::Key::Apostrophe},
    {Qt::Key_Slash, sf::Keyboard::Key::Slash},
    {Qt::Key_Backslash, sf::Keyboard::Key::Backslash},
    {Qt::Key_Comma, sf::Keyboard::Key::Comma},
    {Qt::Key_Period, sf::Keyboard::Key::Period},
    {Qt::Key_Control, sf::Keyboard::Key::LControl},
    {Qt::Key_Shift, sf::Keyboard::Key::LShift},
    {Qt::Key_BracketLeft, sf::Keyboard::Key::LBracket},
    {Qt::Key_BracketRight, sf::Keyboard::Key::RBracket}
};

// Might need this for later?
const std::unordered_map<Qt::Key, const char *> KEY_TO_MISSING_NAME_MAP = {
};

InputSettingsDisplay::InputSettingsDisplay(QWidget *parent) : QWidget{parent},
                                                              button0(new QPushButton(this)),
                                                              button1(new QPushButton(this)),
                                                              button2(new QPushButton(this)),
                                                              button3(new QPushButton(this)),
                                                              button4(new QPushButton(this)),
                                                              button5(new QPushButton(this)),
                                                              button6(new QPushButton(this)),
                                                              button7(new QPushButton(this)),
                                                              button_to_be_bound(nullptr),
                                                              batch_assign(new QPushButton(tr("Batch Assign"), this)),
                                                              is_batch_assigning(false),
                                                              batch_idx(BATCH_ASSIGN_START)
{

    auto *layout = new QFormLayout();

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        buttons[i]->setProperty(BUTTON_ID, QVariant(i));
        layout->addRow(CONTROLLER_MAP[i], buttons[i]);
    }
    layout->addRow(batch_assign);

    setLayout(layout);

    // Events
    
    for (auto &button : buttons)
    {
        connect(button, &QPushButton::pressed, [this, button](){on_button_press(button);});
    }

    connect(batch_assign, &QPushButton::pressed, this, &InputSettingsDisplay::on_batch_assign_pressed);

}

InputSettingsDisplay::~InputSettingsDisplay()
{
}

void InputSettingsDisplay::on_button_press(QPushButton *button)
{
    button->setText(AWAITING_INPUT);
    enable_assign_mode(button);
}

void InputSettingsDisplay::on_batch_assign_pressed()
{
    is_batch_assigning = true;
    button_to_be_bound->setText(AWAITING_INPUT);
    enable_assign_mode(button0);
}

void InputSettingsDisplay::on_button_to_be_bound_pressed(QPushButton *button, QKeyEvent *event)
{
    const auto qt_key = Qt::Key(event->key());
    auto &controller_manager = ControllerManager::instance();
    const QString active_profile_name = SettingsManager::instance().active_input_profile();
    InputMap active_input_map = controller_manager.from_json(active_profile_name);
    const auto button_enum = InputMap::Buttons(button->property(BUTTON_ID).toInt());

    if (qt_key != Qt::Key_Delete && QT_TO_SF_MAP.contains(qt_key))
    {
        
        const auto input_button = InputButton(event->text(), QT_TO_SF_MAP.at(qt_key));

        button->setText(event->text().toUpper());
        qInfo() << "Qt::Key:" << QString::number(event->key()) << "Name:" << event->text();

        controller_manager.set_button(active_input_map, button_enum, input_button);
        controller_manager.save_json();
    }
    else
    {
        button->setText(active_input_map.buttons[button_enum].display_name.toUpper());
    }
}

void InputSettingsDisplay::keyPressEvent(QKeyEvent *event)
{
    if (button_to_be_bound == nullptr)
    {
        QWidget::keyPressEvent(event);
        return;
    }
    const QString event_text = event->text().toUpper();
    //qDebug() << "Conflicting binds check";
    const bool is_conflicting_binds = conflicting_binds_check(button_to_be_bound, event_text);
    //qDebug() << "Done, result (bool):" << QString::number(is_conflicting_binds);
    if (!is_conflicting_binds)
    {
        QMessageBox::information(
            this,
            "TetroidNES",
            QString("%1: %2").arg(
                tr("Could not assign key, more than one of the same key cannot be assigned"),
                event_text
            )
        );

        event->accept();
        return;
    }

    if (!is_batch_assigning)
    {
        on_button_to_be_bound_pressed(button_to_be_bound, event);

        disable_assign_mode();

        event->accept();

        qDebug() << "Released keyboard";
        return;
    }

    on_button_to_be_bound_pressed(button_to_be_bound, event);
    batch_idx++;

    if (batch_idx < BUTTON_COUNT && event->key() != Qt::Key_Delete)
    {
        button_to_be_bound = buttons[batch_idx];
        button_to_be_bound->setText(AWAITING_INPUT);
    }
    else
    {
        batch_idx = BATCH_ASSIGN_START;
        is_batch_assigning = false;
        disable_assign_mode();

        qDebug() << "Released keyboard (Batch assign)";
    }

    event->accept();
}

void InputSettingsDisplay::toggle_buttons(const bool enabled)
{
    for(auto button : buttons)
    {
        button->setEnabled(enabled);
    }

    batch_assign->setEnabled(enabled);
}

void InputSettingsDisplay::setup()
{
    auto &controller_manager = ControllerManager::instance();
    auto &settings_manager = SettingsManager::instance();
    QString active_input_map_name = settings_manager.active_input_profile();
    auto active_input_map = controller_manager.from_json(active_input_map_name);

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        buttons[i]->setText(active_input_map.buttons[i].display_name.toUpper());
    }
}

bool InputSettingsDisplay::conflicting_binds_check(QPushButton *newly_bound_button, QString text_to_compare) const
{
    //qDebug() << "Newly binded button text:" << text_to_compare;
    
    QList buttons_copy(buttons);
    buttons_copy.removeAt(buttons_copy.indexOf(newly_bound_button));

    for (auto &button : buttons_copy)
    {
        const QString text = button->text();
        //qDebug() << "Button text:" << text;
        if (text_to_compare == text)
        {
            return false;
        }
    }

    return true;
}

void InputSettingsDisplay::enable_assign_mode(QPushButton *button)
{
    toggle_buttons(false);
    grabKeyboard();
    button_to_be_bound = button;
}

void InputSettingsDisplay::disable_assign_mode()
{
    toggle_buttons(true);
    releaseKeyboard();
    button_to_be_bound = nullptr;
}