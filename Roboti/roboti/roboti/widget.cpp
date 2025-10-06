#include "widget.h"
#include "./ui_widget.h"

#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QTableWidgetItem>
#include "robotworker.h"
#include <QObject>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->loadPb, &QPushButton::clicked, this, &Widget::onLoadClicked);
    connect(ui->startPB, &QPushButton::clicked, this, &Widget::onBattleClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onLoadClicked()
{
    loadRobots();
    fillRobotsList();
}

void Widget::onBattleClicked()
{
    if(m_robots.empty())
        return;

    ui->startPB->setEnabled(false);
    ui->loadPb->setEnabled(false);

    ui->mostwonLe->clear();

    initBattleTable();
    startThreads();
}

void Widget::loadRobots()
{
    auto files = QFileDialog::getOpenFileName(this, "Naslov", ".." "JSON(*.json");

    if(files.isEmpty())
        return ;

    QFile file = QFile(files);
    if(!file.open(QIODevice::ReadOnly))
        return ;

    auto doc = QJsonDocument::fromJson(file.readAll());

    file.close();

    qDeleteAll(m_robots);
    m_robots.clear();

    auto robotsVariant = doc.toVariant().toList();
    for(const auto& robotVariant : robotsVariant){
        Robot *robot = new Robot;
        robot->fromQVariant(robotVariant);
        m_robots.append(robot);
    }
}

void Widget::fillRobotsList()
{
    ui->robotsLw->clear();
    for(int i = 0; i < m_robots.size(); i++){
        ui->robotsLw->addItem(QString::number(i+1) + ": " + m_robots[i]->toString());
    }

    ui->robotNumberLe->setText(QString::number(m_robots.size()));
}

void Widget::initBattleTable()
{
    auto numOfRobots = m_robots.size();

    ui->battlesTw->clear();
    ui->battlesTw->setRowCount(numOfRobots);
    ui->battlesTw->setColumnCount(numOfRobots);

    for(int i = 0; i < numOfRobots; i++){
        for(int j = 0; j < numOfRobots; j++){
            auto *tableItem = new QTableWidgetItem;
            if(i == j)
                tableItem->setText("/");
            ui->battlesTw->setItem(i, j, tableItem);
        }
    }
}

void Widget::startThreads()
{
    auto numOfRobots = m_robots.size();

    for(int i =0; i < numOfRobots; i++){
        for(int j = i+1; j < numOfRobots; j++){
            RobotWorker *worker = new RobotWorker(m_robots[i], m_robots[j], i, j);
            connect(worker, &QThread::finished, worker, &QObject::deleteLater);
            connect(worker, &RobotWorker::battleFinished, this, &Widget::setBattleWinner);
            worker -> start();
            m_numOfRemainingBattles++;
        }
    }
}

void Widget::setBattleWinner(int winner, int loser){
    ui->battlesTw->item(winner, loser)->setText("Pobedio");
    ui->battlesTw->item(loser, winner)->setText("Izgubio");
    m_numOfRemainingBattles--;

    if(m_numOfRemainingBattles == 0){
        setCompetitionWinner();
        ui->startPB->setEnabled(true);
        ui->loadPb->setEnabled(true);
    }
}

void Widget::setCompetitionWinner()
{
    auto numOfRobots = m_robots.size();

    int bestIdx = 0, mostWins = 0;
    for(int i = 0; i < numOfRobots; i++){
        int numOfWins = 0;
        for(int j = 0; j < numOfRobots; j++){
            if(ui->battlesTw->item(i, j)->text() == "Pobedio")
                numOfWins++;
        }

        if(numOfWins > mostWins){
            mostWins = numOfWins;
            bestIdx = i;
        }
    }

    ui->mostwonLe->setText(m_robots[bestIdx]->getName());
}
