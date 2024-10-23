#ifndef ROMDATA_H
#define ROMDATA_H

#include <QObject>
#include <QUrl>

using u_short = unsigned short;

class RomData : public QObject
{
    Q_OBJECT
public:
    explicit RomData(QObject *parent = nullptr, 
        u_short year = 0,
        QByteArray img = QByteArray(),
        QString title = QString(),
        bool favorited = false,
        QUrl path = QUrl("")
    );
    ~RomData();
    u_short year();
    void set_year(const u_short year);
    QString title();
    void set_title(QString title);
    QByteArray img();
    void set_img(QByteArray img);
    bool favorited();
    void set_favorited(const bool b);
    QUrl path();
    void set_path(QUrl path);
    bool is_empty() const;

private:
    u_short m_year;
    QByteArray m_img;
    QString m_title;
    bool m_favorited;
    QUrl m_path;
};

using shptr_romdata = QSharedPointer<RomData>;

#endif // ROMDATA_H