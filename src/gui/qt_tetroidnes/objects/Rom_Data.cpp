#include <QtLogging>

#include <Qt/objects/rom_data.h>

RomData::RomData(QObject *parent, uint16_t year, QByteArray img, QString title, bool favorited, QUrl path) : QObject{parent}
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

uint16_t RomData::year() const
{ return m_year; }

void RomData::set_year(const uint16_t year)
{ m_year = year; }

QString RomData::title() const
{ return m_title; }

void RomData::set_title(QString title)
{ m_title = title; }

QByteArray RomData::img() const
{ return m_img; }

void RomData::set_img(QByteArray img)
{ m_img = img; }

bool RomData::favorited() const
{ return m_favorited; }

void RomData::set_favorited(const bool b)
{ m_favorited = b; }

QUrl RomData::path() const
{return m_path; }

void RomData::set_path(QUrl path)
{m_path = path; }