#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class menubar : public QMenuBar
{
    Q_OBJECT
public:
    explicit menubar(QWidget *parent = nullptr);
    ~menubar();

};

#endif // MENUBAR_H
