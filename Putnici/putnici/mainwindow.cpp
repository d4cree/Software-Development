#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QJsonDocument>
#include "niti.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->ug_button, &QPushButton::clicked, this, &MainWindow::loadCities);
    connect(ui->zp_button, &QPushButton::clicked, this, &MainWindow::startVisits);
}

MainWindow::~MainWindow()
{
    delete ui;
    qDeleteAll(m_cities);
}

void MainWindow::loadCities(){

    readCities();
    showCities();
}

void MainWindow::startVisits()
{
    if(m_cities.empty())
        return;

    bool ispravan;
    const unsigned broj = ui->bp_lineedit->text().toUInt(&ispravan);
    if(!ispravan || broj == 0)
        return;

    ui->ug_button->setDisabled(true);
    ui->zp_button->setEnabled(false);

    ui->pp_lineedit->clear();
    ui->lw_articles->clear();

    active_threads = broj;
    for(unsigned i = 0; i < active_threads; i++){
        auto thread = new Niti(&m_cities, this);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        connect(thread, &Niti::distance, this, &MainWindow::finishedVisit);
        connect(thread, &Niti::collected, this, &MainWindow::addArticle);

        thread->start();
    }

}

void MainWindow::readCities(){
    auto files = QFileDialog::getOpenFileNames(this, "Naslov", "..", "JSON(*.json)");

    qDeleteAll(m_cities);
    m_cities.clear();

    for(auto &filePath : files){
        QFile file(filePath);
        if(file.open(QFile::ReadOnly)){
            const auto contents = file.readAll();

            const auto doc = QJsonDocument::fromJson(contents);
            const auto var = doc.toVariant();

            auto city = new City();
            city->fromQVariant(var);
            m_cities.push_back(city);
        }
    }

}

void MainWindow::showCities(){
    ui->lw_cities->clear();

    for(auto city : m_cities){
        ui->lw_cities->addItem(city->toQString());
    }
}

void MainWindow::addArticle(QString article)
{
    if(m_collected_articles.contains(article)){
        m_collected_articles[article] += 1;
    } else {
        m_collected_articles[article] = 1;
    }

    ui->lw_articles->clear();
    for(auto item : m_collected_articles.toStdMap()){
        const auto text = item.first + ": " + QString::number(item.second);
        ui->lw_articles->addItem(text);
    }
}

void MainWindow::finishedVisit(unsigned int distance)
{
    auto current_distance = ui->pp_lineedit->text().toUInt();
    ui->pp_lineedit->setText(QString::number(current_distance + distance));

    --active_threads;
    if(active_threads==0){
        ui->ug_button->setDisabled(false);
        ui->zp_button->setEnabled(true);
    }

}
