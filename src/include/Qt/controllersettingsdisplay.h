#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>

class InputSettingsDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit InputSettingsDisplay(QWidget *parent = nullptr);
    ~InputSettingsDisplay();

    void on_button_to_be_bound_pressed(QPushButton *button, QKeyEvent *event);
    void setup();
public slots:
    void on_button_press(QPushButton *button);
    void on_batch_assign_pressed();
private:
    QPushButton *button0;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;

    QPushButton *batch_assign;

    const QList<QPushButton *> buttons = {
        button0, button1, button2, button3,
        button4, button5, button6, button7
    };

    QPushButton *button_to_be_bound;
    bool is_batch_assigning;
    uint batch_idx;

    void toggle_buttons(const bool enabled);
    bool conflicting_binds_check(QPushButton *newly_bound_button, QString text_to_compare) const;
protected:
    void keyPressEvent(QKeyEvent *event);
};