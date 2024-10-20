#ifndef SETTINGS_H
#define SETTINGS_H

#include <settingsdisplay.h>

#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFrame>

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private:
    QListWidget *setting_category;
    QVBoxLayout *layout;
    QHBoxLayout *layout_controls;
    SettingsDisplay *setting_display;
    QFrame *controls_frame;
    QPushButton *apply_changes;
    QPushButton *cancel_changes;
    QStatusBar *button_statusbar;

private slots:
    void on_setting_category_item_clicked(const int index);
    void on_apply_changes_clicked();
    void on_cancel_changes_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;
    
};

#endif // SETTINGS_H