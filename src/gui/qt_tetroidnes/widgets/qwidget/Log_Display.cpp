#include <Qt/widgets/qwidget/log_display.h>

#include <QVBoxLayout>

LogDisplay::LogDisplay(QWidget *parent) : QWidget{parent},
                                                    text_display(new QPlainTextEdit(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_AcceptDrops, false);

    setWindowFlag(Qt::WindowType::Window);
    setWindowTitle(QString("TetroidNES - %1").arg(tr("Log System")));
    resize(600, 400);

    QLayout *layout = new QVBoxLayout();

    text_display->setReadOnly(true);

    layout->addWidget(text_display);
    setLayout(layout);

    connect(&LogNotifier::instance(), &LogNotifier::log_pushed, this, &LogDisplay::append_line);
}

void LogDisplay::append_line(const QString &line)
{
    blockSignals(true);
    text_display->appendPlainText(line);
    blockSignals(false);
}