#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "robot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private slots:
    void onLoadClicked();
    void onBattleClicked();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


private:
    void loadRobots();
    void fillRobotsList();
    void initBattleTable();
    void startThreads();
    void setBattleWinner(int winner, int loser);
    void setCompetitionWinner();

    QList<Robot*> m_robots;
    Ui::Widget *ui;
    int m_numOfRemainingBattles = 0;

};
#endif // WIDGET_H
