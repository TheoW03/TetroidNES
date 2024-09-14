#ifndef FILTERCONTROLFRAME_H
#define FILTERCONTROLFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

class FilterControlFrame : public QFrame
{
    Q_OBJECT
public:
    explicit FilterControlFrame(QWidget *parent = nullptr);
private:
    QLineEdit *search_bar;
    QFrame *sort_buttons_frame;
    QPushButton *sort_ascending_button;
    QGroupBox *sort_mode_groupbox;
    QPushButton *sort_mode_az;
    QPushButton *sort_mode_year;
    QPushButton *sort_mode_favorites;
    QButtonGroup *sort_mode_button_group;

signals:
};

#endif // FILTERCONTROLFRAME_H
