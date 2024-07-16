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



    //对应6个模型
    QLineSeries *series1;
    QLineSeries *series2;

    QScatterSeries *scatterSeries1;
    QScatterSeries *scatterSeries2;

    QLineEdit *lowerTempLineEdit;
    QLineEdit *upperTempLineEdit;
    QPushButton *simulateButton;
    QLabel *imageLabel;
    QIntValidator *tempValidator;
    QSqlDatabase db;
    QChartView *chartview;
    QChart *chart;
    QComboBox *modelComboBox;




    void updateTooltip(QPointF point, bool state);
    void updateTooltip5(QPointF point, bool state);

    void draw(int modelId);

    void onPredictRequest();
    void onPredictInfoReady(const QJsonValue& min_t,const QJsonValue& max_t);

private slots:
    void onSimulateClicked();

private:

};

#endif // SIMULATION_H
