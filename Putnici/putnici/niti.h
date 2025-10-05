#ifndef NITI_H
#define NITI_H

#include <QThread>
#include "city.h"
#include <QMutex>

class Niti : public QThread
{
    Q_OBJECT
public:
    explicit Niti(QVector<City *> *towns, QObject *parent = nullptr);

protected:
    void run() final;

signals:
    void collected(QString article);
    void distance(unsigned);
private:
    QVector<City *> *m_towns;
};

#endif // NITI_H
