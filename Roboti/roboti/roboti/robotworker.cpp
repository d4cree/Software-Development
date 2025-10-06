#include "robotworker.h"

#include <QRandomGenerator>
#include <QMutexLocker>

RobotWorker::RobotWorker(Robot *robotOne, Robot *robotTwo, int i, int j)
    : m_robotOne(robotOne), m_robotTwo(robotTwo), m_i(i), m_j(j){}

void RobotWorker::run(){
    QMutexLocker lockOne(m_robotOne->getBattleMutex());
    QMutexLocker lockTwo(m_robotTwo->getBattleMutex());

    m_robotOne -> resetHitPoints();
    m_robotTwo -> resetHitPoints();

    while(true){
        QThread::msleep(250);

        auto attacker = QRandomGenerator::global()->bounded(1, 2+1);

        if(attacker == 1 && m_robotOne->attack(*m_robotTwo)){
            emit battleFinished(m_i, m_j);
            break;
        } else if(attacker == 2 && m_robotTwo->attack(*m_robotOne)){
            emit battleFinished(m_j, m_i);
            break;
        }
    }
}
