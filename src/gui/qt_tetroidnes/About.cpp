#include <Qt/about.h>

#include <QVBoxLayout>

#define ABOUT_TEXT "dummy text" // TODO: Placeholder until we figure out how to store long strings of text, maybe using resources?

About::About(QWidget *parent) : QWidget{parent},
                                text(new QLabel(ABOUT_TEXT, this))
{
    QVBoxLayout *layout = new QVBoxLayout();

    text->setTextFormat(Qt::PlainText); // PlainText is placeholder until we decide what text format we want

    layout->addWidget(text);

    setLayout(layout);
}

About::~About()
{
}