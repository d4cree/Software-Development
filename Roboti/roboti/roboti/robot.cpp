#include "robot.h"

#include <QRandomGenerator>

Robot::Robot() {}

void Robot::fromQVariant(const QVariant &variant)
{
    auto map = variant.toMap();
    m_name = map.value("name").toString();
    m_maxHp = map.value("hp").toUInt();
    m_currentHp = map.value("hp").toUInt();

    auto attackList = map.value("atk").toList();
    m_minAttack = attackList[0].toUInt();
    m_maxAttack = attackList[1].toUInt();
}

QString Robot::toString() const
{
    return m_name + ": hp=" + QString::number(m_maxHp) + ", atk =["
           + QString::number(m_minAttack) + ", " + QString::number(m_maxAttack) + "]";
}

bool Robot::attack(Robot &robot) const
{
    unsigned damage = QRandomGenerator::global()->bounded(m_minAttack, m_maxAttack+1);
    unsigned scaling = std::max(0.5, static_cast<double>(m_currentHp)/(m_maxHp));
    unsigned scaled_dmg = damage*scaling;

    if(robot.m_currentHp <= scaled_dmg){
        robot.m_currentHp = 0;
        return true;
    } else {
        robot.m_currentHp -= scaled_dmg;
        return false;
    }
}

void Robot::resetHitPoints()
{
    m_currentHp = m_maxHp;
}

QMutex *Robot::getBattleMutex()
{
    return &m_mutexBattle;
}

QString Robot::getName()
{
    return m_name;
}
