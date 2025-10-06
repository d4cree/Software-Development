#ifndef ROBOTWORKER_H
#define ROBOTWORKER_H

#include <QThread>
#include <QObject>
#include "robot.h"

class RobotWorker : public QThread
{
    Q_OBJECT
public:
    RobotWorker(Robot *robotOne, Robot *robotTwo, int i, int j);

protected:
    void run() override;

signals:
    void battleFinished(int winner, int loser);

private:
    int m_i, m_j;
    Robot *m_robotOne, *m_robotTwo;
};

#endif // ROBOTWORKER_H
