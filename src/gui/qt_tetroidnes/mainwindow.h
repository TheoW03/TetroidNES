#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filtercontrolframe.h"
#include "romlist.h"

#include <QMainWindow>
#include <QPushButton>
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
public slots:
    void sort_mode_button_released(int id);
    void sort_order_button_toggled(bool toggled);
    void search_bar_edited(QString string);
private:
    RomList *rom_list;
    FilterControlFrame *sort_control_frame;

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
