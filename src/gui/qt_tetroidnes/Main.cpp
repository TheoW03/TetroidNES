#define VERSION "1.0.0"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtLogging>

#include <Qt/objects/controller_manager.h>
#include <Qt/objects/settings_manager.h>
#include <Qt/widgets/qmainwindow/main_window.h>
#include <Qt/utils/log.h>

/*
Checks for a missing active input map in the controller manager
If it is missing, change the active input map to the first item in the json
*/
void check_and_handle_missing_active_input_map()
{
    auto &settings_manager = SettingsManager::instance();
    auto &controller_manager = ControllerManager::instance();

    auto active_input_key = settings_manager.active_input_profile();

    if (!controller_manager.profile_exists(active_input_key))
    {
        QString new_active_input_key = controller_manager.get_profile_names().first();

        qWarning()
            << "Could not find active input profile! Setting new active input to"
            << new_active_input_key;

        settings_manager.set_active_input_profile(new_active_input_key);
    }
}

void apply_stylesheet(QApplication &a)
{
    QFile stylesheet_h(":/stylesheets/style.qss");
    stylesheet_h.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheet_h.readAll());

    a.setStyleSheet(stylesheet);
}

int main(int argc, char **argv)
{
    // originalHandler = qInstallMessageHandler(logToFile);
    //InitLogs();
    // int &c = argc
    QApplication a(argc, argv);
    a.setApplicationName("TetroidNES");

    a.setApplicationVersion(VERSION);

    qInfo() << "STARTING" << a.applicationName() << "VERSION" << a.applicationVersion();
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "tetroidnes_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    apply_stylesheet(a);

    check_and_handle_missing_active_input_map();

    MainWindow w;
    w.show();
    return a.exec();
}