#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filtercontrolframe.h"
#include "romlist.h"
#include "menubar.h"

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update_page_info();
public slots:
    void sort_mode_button_released(const int id) const;
    void sort_order_button_toggled(const bool toggled) const;
    void search_bar_edited(QString string) const;
    void rom_list_scroll_value_changed(const int value);
private:
    RomList *rom_list;
    FilterControlFrame *sort_control_frame;
    QScrollArea *rom_list_scroll;
    MenuBar *main_menubar;
    QVBoxLayout *widget_layout;
    QLabel *page_info;
protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
