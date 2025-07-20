#pragma once

#include <QUrl>

class RomData
{
public:
    explicit RomData(uint16_t year, QByteArray img, QString title, bool favorited, QUrl path);
    RomData();
    ~RomData();

    uint16_t year() const;
    void set_year(const uint16_t &year);

    QString title() const;
    void set_title(const QString &title);

    QByteArray img() const;
    void set_img(const QByteArray &img);

    bool favorited() const;
    void set_favorited(const bool &b);

    QUrl path() const;
    void set_path(const QUrl &path);

    bool is_empty() const;

private:
    uint16_t m_year;
    QByteArray m_img;
    QString m_title;
    bool m_favorited;
    QUrl m_path;
};