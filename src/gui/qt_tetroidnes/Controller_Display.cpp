#include <Qt/controllersettingsdisplay.h>

#include <QList>
#include <QFormLayout>

#include <Qt/controllermanager.h>
#include <Qt/settingsmanager.h>

constexpr const char* BUTTON_ID = "button_id";
constexpr const char* AWAITING_INPUT = "...";
constexpr const uint BATCH_ASSIGN_START = 0;

const std::unordered_map<Qt::Key, sf::Keyboard::Key> QT_TO_SF_MAP = {
    // Number Keys
    {Qt::Key_0, sf::Keyboard::Num0},
    {Qt::Key_1, sf::Keyboard::Num1},
    {Qt::Key_2, sf::Keyboard::Num2},
    {Qt::Key_3, sf::Keyboard::Num3},
    {Qt::Key_4, sf::Keyboard::Num4},
    {Qt::Key_5, sf::Keyboard::Num5},
    {Qt::Key_6, sf::Keyboard::Num6},
    {Qt::Key_7, sf::Keyboard::Num7},
    {Qt::Key_8, sf::Keyboard::Num8},
    {Qt::Key_9, sf::Keyboard::Num9},
    // F Keys
    {Qt::Key_F1, sf::Keyboard::F1},
    {Qt::Key_F2, sf::Keyboard::F2},
    {Qt::Key_F3, sf::Keyboard::F3},
    {Qt::Key_F4, sf::Keyboard::F4},
    {Qt::Key_F5, sf::Keyboard::F5},
    {Qt::Key_F6, sf::Keyboard::F6},
    {Qt::Key_F7, sf::Keyboard::F7},
    {Qt::Key_F8, sf::Keyboard::F8},
    {Qt::Key_F9, sf::Keyboard::F9},
    {Qt::Key_F10, sf::Keyboard::F10},
    {Qt::Key_F11, sf::Keyboard::F11},
    {Qt::Key_F12, sf::Keyboard::F12},
    // Alphabet Keys
    {Qt::Key_A, sf::Keyboard::A},
    {Qt::Key_B, sf::Keyboard::B},
    {Qt::Key_C, sf::Keyboard::C},
    {Qt::Key_D, sf::Keyboard::D},
    {Qt::Key_E, sf::Keyboard::E},
    {Qt::Key_F, sf::Keyboard::F},
    {Qt::Key_G, sf::Keyboard::G},
    {Qt::Key_H, sf::Keyboard::H},
    {Qt::Key_I, sf::Keyboard::I},
    {Qt::Key_J, sf::Keyboard::J},
    {Qt::Key_K, sf::Keyboard::K},
    {Qt::Key_L, sf::Keyboard::L},
    {Qt::Key_M, sf::Keyboard::M},
    {Qt::Key_N, sf::Keyboard::N},
    {Qt::Key_O, sf::Keyboard::O},
    {Qt::Key_P, sf::Keyboard::P},
    {Qt::Key_Q, sf::Keyboard::Q},
    {Qt::Key_R, sf::Keyboard::R},
    {Qt::Key_S, sf::Keyboard::S},
    {Qt::Key_T, sf::Keyboard::T},
    {Qt::Key_U, sf::Keyboard::U},
    {Qt::Key_V, sf::Keyboard::V},
    {Qt::Key_W, sf::Keyboard::W},
    {Qt::Key_X, sf::Keyboard::X},
    {Qt::Key_Y, sf::Keyboard::Y},
    {Qt::Key_Z, sf::Keyboard::Z},
    // Arrow Keys
    {Qt::Key_Left, sf::Keyboard::Left},
    {Qt::Key_Right, sf::Keyboard::Right},
    {Qt::Key_Down, sf::Keyboard::Down},
    {Qt::Key_Up, sf::Keyboard::Up},
    // Misc Keys
    {Qt::Key_Equal, sf::Keyboard::Equal},
    {Qt::Key_Minus, sf::Keyboard::Subtract},
    {Qt::Key_Bar, sf::Keyboard::Space},
    {Qt::Key_Alt, sf::Keyboard::LAlt},
    {Qt::Key_Semicolon, sf::Keyboard::SemiColon},
    {Qt::Key_QuoteLeft, sf::Keyboard::Quote},
    {Qt::Key_Slash, sf::Keyboard::Slash},
    {Qt::Key_Backslash, sf::Keyboard::Backslash},
    {Qt::Key_Comma, sf::Keyboard::Comma},
    {Qt::Key_Period, sf::Keyboard::Period},
    {Qt::Key_Control, sf::Keyboard::LControl},
    {Qt::Key_Shift, sf::Keyboard::LShift},
    {Qt::Key_BracketLeft, sf::Keyboard::LBracket},
    {Qt::Key_BracketRight, sf::Keyboard::RBracket}
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
                                                              batch_assign(new QPushButton("Batch Assign", this)),
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
    toggle_buttons(false);
    button_to_be_bound = button;
    grabKeyboard();
}

void InputSettingsDisplay::on_batch_assign_pressed()
{
    toggle_buttons(false);
    is_batch_assigning = true;
    grabKeyboard();
    button_to_be_bound = button0;
    button_to_be_bound->setText(AWAITING_INPUT);
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
        button->setText(active_input_map.buttons[button_enum].display_name);
    }
}

void InputSettingsDisplay::keyPressEvent(QKeyEvent *event)
{
    if (button_to_be_bound == nullptr)
    {
        QWidget::keyPressEvent(event);
        return;
    }
    
    if (!is_batch_assigning)
    {
        on_button_to_be_bound_pressed(button_to_be_bound, event);

        button_to_be_bound = nullptr;
        releaseKeyboard();
        toggle_buttons(true);

        event->accept();

        qDebug() << "Released keyboard";
        return;
    }
    else
    {
        on_button_to_be_bound_pressed(button_to_be_bound, event);
        batch_idx++;

        event->accept();
    }

    if (batch_idx < BUTTON_COUNT)
    {
        button_to_be_bound = buttons[batch_idx];
        button_to_be_bound->setText(AWAITING_INPUT);
    }
    else
    {
        batch_idx = BATCH_ASSIGN_START;
        is_batch_assigning = false;
        releaseKeyboard();
        toggle_buttons(true);

        qDebug() << "Released keyboard (Batch assign)";
    }
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