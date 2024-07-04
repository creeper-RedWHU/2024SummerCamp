#ifndef DATAVISUALIZATION_H
#define DATAVISUALIZATION_H

#include <QWidget>
#include <QtCharts>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class datavisualization;
}

class datavisualization : public QWidget
{
    Q_OBJECT

public:
    explicit datavisualization(QWidget *parent = nullptr);
    ~datavisualization();

    QLineSeries *series1;  //数据系列
    QPieSeries *series2;  //数据系列
    QBarSeries *series3;  //数据系列
    QBarSet* barset1;  //柱状图的"柱"
    QBarSet* barset2;
    QBarSet* barset3;
    QBarSet* barset4;

    QLineSeries *series4;  //数据系列
    QChart *chart1;  //图表
    QChart *chart2;  //图表
    QChart *chart3;  //图表
    QChart *chart4;  //图表
    QChartView *chartview1;  //图表视图
    QChartView *chartview2;  //图表视图
    QChartView *chartview3;  //图表视图
    QChartView *chartview4;  //图表视图



    void updateTooltip(QPointF point, bool state);

private:
    Ui::datavisualization *ui;
};

#endif // DATAVISUALIZATION_H
