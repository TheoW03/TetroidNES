#pragma once

#include <QMenuBar>
#include <QAction>
#include <QUrl>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    ~MenuBar();

public slots:
    void open_rom();
    void open_settings();
    void open_input_settings();
    void open_log_display();
    void refresh_recent_roms(QStringList dirs);

private:
    void start_rom(QUrl url);
    void start_rom(QString url);

    QMenu *file;
    QAction *file_open;
    QMenu *file_open_recent;

    QMenu *edit;
    QAction *settings_open;
    QAction *input_settings_open;

    QMenu *tools;
    QAction *log_display_open;
    
    QMenu *help;

};