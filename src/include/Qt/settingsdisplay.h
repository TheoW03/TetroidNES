#ifndef SETTINGSDISPLAY_H
#define SETTINGSDISPLAY_H

#include <QStackedWidget>

class SettingsDisplay : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SettingsDisplay(QWidget *parent = nullptr);
    ~SettingsDisplay();

private:
    QWidget *general;
    QWidget *emulator;
    QWidget *about;

    void setup_general(QWidget *general);
    void setup_emulator(QWidget *emulator);
    void setup_about(QWidget *about);

private slots:
    void on_add_directory_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
};

#endif //SETTINGSDISPLAY_h