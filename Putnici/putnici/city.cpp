#include "city.h"

City::City() {}

void City::fromQVariant(const QVariant &variant){
    const auto map = variant.toMap();
    m_name = map.value("name").toString();
    const QVariantList cords = map["coordinates"].toList();
    m_x = cords[0].toUInt();
    m_y = cords[1].toUInt();

    m_list.clear();
    QVariantList list = map.value("articles").toList();
    for(QVariant &v : list){
        m_list.append(v.toString());
    }
}

QString City::toQString() const{
    return m_name + "(" + QString::number(m_x) + ", " + QString::number(m_y)
           + "): Broj Artikala = " + QString::number(m_list.length());
}

QString City::getRandomArticle(){
    if(m_list.empty())
        return "";

    const auto i = QRandomGenerator::global()->generate() % m_list.size();
    const auto article = m_list[i];

    m_list.remove(i);

    return article;
}

unsigned City::distance(const City &city) const
{
    return ((m_x > city.m_x)?(m_x - city.m_x):(city.m_x - m_x)) + ((m_y > city.m_y)?(m_y - city.m_y):(city.m_y - m_y));

}
