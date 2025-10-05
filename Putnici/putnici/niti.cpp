#include "niti.h"

#include <QMutexLocker>

Niti::Niti(QVector<City *>* towns, QObject *parent)
    :QThread(parent)
    , m_towns(towns)
{}

void Niti::run()
{
    auto ponavljanja = QRandomGenerator::global()->bounded(3, 7);
    unsigned predjeni_put = 0;
    unsigned city_i;

    for(int i =0; i != ponavljanja; i++){
        const auto sleep_time = QRandomGenerator::global()->bounded(250, 350);
        msleep(sleep_time);

        if(i == 0){
            city_i = QRandomGenerator::global()->generate() % m_towns->size();
        } else {

            auto new_city_i = QRandomGenerator::global()->generate() % (m_towns->size()-1);
            if(new_city_i >= city_i) new_city_i++;

            predjeni_put += (*m_towns)[city_i]->distance(*(*m_towns)[new_city_i]);

            city_i = new_city_i;
        }

        QMutexLocker lock(&(*m_towns)[city_i]->m_mutex);

        QString article = (*m_towns)[city_i]->getRandomArticle();
        if(!article.isEmpty()){
            emit collected(article);
        }
    }
    emit distance(predjeni_put);
}
