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

};

#endif // MENUBAR_H
