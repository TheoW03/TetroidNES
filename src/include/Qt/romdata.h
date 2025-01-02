#ifndef ROMDATA_H
#define ROMDATA_H

#include <QObject>
#include <QUrl>

class RomData : public QObject
{
    Q_OBJECT
public:
    explicit RomData(QObject *parent = nullptr, 
        uint16_t year = 0,
        QByteArray img = QByteArray(),
        QString title = QString(),
        bool favorited = false,
        QUrl path = QUrl("")
    );
    ~RomData();
    uint16_t year() const;
    void set_year(const uint16_t year);
    QString title() const;
    void set_title(QString title);
    QByteArray img() const;
    void set_img(QByteArray img);
    bool favorited() const;
    void set_favorited(const bool b);
    QUrl path() const;
    void set_path(QUrl path);
    bool is_empty() const;

private:
    uint16_t m_year;
    QByteArray m_img;
    QString m_title;
    bool m_favorited;
    QUrl m_path;
};

#endif // ROMDATA_H