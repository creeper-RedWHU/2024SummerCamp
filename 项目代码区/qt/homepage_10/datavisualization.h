#ifndef DATAVISUALIZATION_H
#define DATAVISUALIZATION_H

#include <QWidget>
#include <QtCharts>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
    QLineSeries *series4;  //数据系列
    QPieSeries *series2;  //数据系列
    QBarSeries *series3;  //数据系列

    QScatterSeries *scatterSeries;

    QBarSet* barset1;  //柱状图的"柱"
    QBarSet* barset2;
    QBarSet* barset3;
    QBarSet* barset4;
    QBarSet* barset5;
    QBarSet* barset6;
    QBarSet* barset7;
    QBarSet* barset8;
    QBarSet* barset9;



    QChart *chart1;  //图表
    QChart *chart4;  //图表
    QChart *chart2;  //图表
    QChart *chart3;  //图表

    QChartView *chartview1;  //图表视图
    QChartView *chartview4;  //图表视图
    QChartView *chartview2;  //图表视图
    QChartView *chartview3;  //图表视图


    QComboBox *cityComboBox;
    QComboBox *YearComboBox;
    QComboBox *MonthComboBox;
    QComboBox *startDayComboBox;
    QComboBox *endDayComboBox;

    QSqlDatabase db;


    void updateTooltip1(QPointF point, bool state);
    void updateTooltip2(QPieSlice *slice, bool state);
    void updateTooltip3(bool hovered, int index, QBarSet* barset);
    void updateTooltip4(QPointF point, bool state);
    bool connectToDatabase();




    void mydraw();

private slots:  // Add this line
    void onDrawButtonClicked();  // Add this line


private:
    Ui::datavisualization *ui;
};





#endif // DATAVISUALIZATION_H
