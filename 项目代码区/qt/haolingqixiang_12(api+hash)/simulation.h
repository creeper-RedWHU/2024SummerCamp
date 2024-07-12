#ifndef SIMULATION_H
#define SIMULATION_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts>

class Simulation : public QWidget
{
    Q_OBJECT

public:
    Simulation(QWidget *parent = nullptr);
    ~Simulation();


    QLineSeries *series1;  //数据系列最高气温
    QLineSeries *series4;  //数据系列最低气温
    QScatterSeries *scatterSeries1;//最高气温
    QScatterSeries *scatterSeries4;//最低气温
    QLineEdit *lowerTempLineEdit;
    QLineEdit *upperTempLineEdit;
    QPushButton *simulateButton;
    QLabel *imageLabel;
    QIntValidator *tempValidator;
    QSqlDatabase db;
    QChartView *chartview1;
    QChart *chart1;

    void updateTooltip4(QPointF point, bool state);
    void updateTooltip5(QPointF point, bool state);

    bool connectToDatabase();

    void draw();

private slots:
    void onSimulateClicked();

private:

};

#endif // SIMULATION_H
