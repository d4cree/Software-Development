#ifndef CITY_H
#define CITY_H
#include <QString>
#include <QVector>
#include <QVariant>
#include <QVariantList>
#include <QRandomGenerator>
#include <QMutex>

class City
{
public:
    City();
    void fromQVariant(const QVariant &variant);
    QString toQString() const;
    QString getRandomArticle();
    unsigned distance(const City &city) const;

    QMutex m_mutex;

private:
    QString m_name;
    unsigned m_x;
    unsigned m_y;
    QVector<QString> m_list;
};

#endif // CITY_H
