#include <QDebug>

#include <Qt/objects/rom_data.h>

RomData::RomData(uint16_t year, const QByteArray &img, const QString &title, bool favorited, const QUrl &path)
{
    m_year = year;
    m_img = img;
    m_title = title;
    m_favorited = favorited;
    m_path = path;

    /*qInfo()
    << "RomData Constuction:\n"
    << "Year" << m_year << "\n"
    << "Img" << m_img << "\n"
    << "Title" << m_title << "\n"
    << "Favorited" << m_favorited << "\n"
    << "Path" << m_path.toString();*/
}

RomData::RomData()
{

}

RomData::~RomData()
{
}

bool RomData::is_empty() const
{ return m_path.isEmpty() || !m_path.isValid(); }

const uint16_t& RomData::year() const
{ return m_year; }

void RomData::set_year(const uint16_t &year)
{ m_year = year; }

const QString& RomData::title() const
{ return m_title; }

void RomData::set_title(const QString &title)
{ m_title = title; }

const QByteArray& RomData::img() const
{ return m_img; }

void RomData::set_img(const QByteArray &img)
{ m_img = img; }

const bool& RomData::favorited() const
{ return m_favorited; }

void RomData::set_favorited(const bool &b)
{ m_favorited = b; }

const QUrl& RomData::path() const
{
    qDebug() << "Returning path:" << m_path.toString();
    return m_path;
}

void RomData::set_path(const QUrl &path)
{m_path = path; }