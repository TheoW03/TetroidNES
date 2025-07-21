#pragma once

#include <QUrl>

class RomData
{
public:
    explicit RomData(uint16_t year, const QByteArray &img, const QString &title, bool favorited, const QUrl &path);
    RomData();
    ~RomData();

    const uint16_t& year() const;
    void set_year(const uint16_t &year);

    const QString& title() const;
    void set_title(const QString &title);

    const QByteArray& img() const;
    void set_img(const QByteArray &img);

    const bool& favorited() const;
    void set_favorited(const bool &b);

    const QUrl& path() const;
    void set_path(const QUrl &path);

    bool is_empty() const;

private:
    uint16_t m_year;
    QByteArray m_img;
    QString m_title;
    bool m_favorited;
    QUrl m_path;
};