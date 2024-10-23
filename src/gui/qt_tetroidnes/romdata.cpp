#include <QtLogging>

#include <Qt/romdata.h>

RomData::RomData(QObject *parent, u_short year, QByteArray img, QString title, bool favorited, QUrl path) : QObject{parent}
{
    m_year = year;
    m_img = img;
    m_title = title;
    m_favorited = favorited;
    m_path = path;
}
RomData::~RomData()
{
}

bool RomData::is_empty() const
{ return m_path.isEmpty() || !m_path.isValid(); }

u_short RomData::year()
{ return m_year; }

void RomData::set_year(const u_short year)
{ m_year = year; }

QString RomData::title()
{ return m_title; }

void RomData::set_title(QString title)
{ m_title = title; }

QByteArray RomData::img()
{ return m_img; }

void RomData::set_img(QByteArray img)
{ m_img = img; }

bool RomData::favorited()
{ return m_favorited; }

void RomData::set_favorited(const bool b)
{ m_favorited = b; }

QUrl RomData::path()
{return m_path; }

void RomData::set_path(QUrl path)
{m_path = path; }