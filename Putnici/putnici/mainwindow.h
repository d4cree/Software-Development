#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "city.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadCities();
    void startVisits();

private:
    void readCities();
    void showCities();
    void addArticle(QString);
    void finishedVisit(unsigned);

private:
    Ui::MainWindow *ui;
    QVector<City*> m_cities;
    unsigned active_threads;
    QMap<QString, unsigned> m_collected_articles;
};
#endif // MAINWINDOW_H
