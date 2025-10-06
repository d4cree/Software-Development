#ifndef ROBOT_H
#define ROBOT_H

#include <QString>
#include <QVariant>

#include <QMutex>

class Robot
{
public:
    Robot();
    void fromQVariant(const QVariant &variant);
    QString toString() const;
    bool attack(Robot& robot) const;

    void resetHitPoints();
    QMutex *getBattleMutex();

    QString getName();

private:
    QString m_name;
    unsigned m_minAttack;
    unsigned m_maxAttack;
    unsigned m_maxHp;
    unsigned m_currentHp;

    QMutex m_mutexBattle;
};

#endif // ROBOT_H
