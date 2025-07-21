#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>

#include <Qt/widgets/qwidget/rom_list.h>
#include <Qt/widgets/qframe/filter_control_frame.h>
#include <Qt/widgets/qmenubar/menubar.h>
#include <Qt/widgets/qwidget/game_display.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update_page_info();
    void create_display(const QString &rom_link);
    inline static bool is_a_game_running()
    {

        for (auto &widget : qApp->topLevelWidgets())
        {
            if (widget->inherits("GameDisplay"))
            {
                if (qobject_cast<GameDisplay *>(widget)->initialized())
                {
                    return true;
                }
            }
        }

        // Code reaches this point if all game display objects are not initialized
        return false;
    }
    inline static void start_game(const QString &path)
    {
        if (is_a_game_running())
        {
            qInfo() << "Can't open game while a game is already running";
            return;
        }
        for (auto &widget : qApp->topLevelWidgets())
        {
            if (widget->inherits("MainWindow"))
            {
                qDebug() << "File path:" << path;
                qobject_cast<MainWindow *>(widget)->create_display(path);
                break;
            }
        }
    }

public slots:
    void sort_mode_button_released(const int id) const;
    void sort_order_button_toggled(const bool toggled) const;
    void search_bar_edited(QString string) const;
    void rom_list_scroll_value_changed(const int value);
    void on_gamedisplay_destroyed();
private:
    QWidget *centralwidget;
    RomList *rom_list;
    FilterControlFrame *sort_control_frame;
    QScrollArea *rom_list_scroll;
    MenuBar *main_menubar;
    QLabel *page_info;
protected:
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

};