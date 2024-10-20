#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    ~MenuBar();

public slots:
    void open_rom();
    void open_settings();

private:
    QMenu *file;
    QAction *file_open;

    QMenu *edit;
    QAction *settings_open;

    QMenu *tools;
    
    QMenu *help;

};

#endif // MENUBAR_H
