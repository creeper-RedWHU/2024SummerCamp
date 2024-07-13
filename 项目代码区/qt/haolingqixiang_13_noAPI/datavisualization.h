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

    QLineSeries *series1;  //数据系列最高气温
    QPieSeries *series2;  //数据系列
    QBarSeries *series3;  //数据系列
    QLineSeries *series4;  //数据系列最低气温
    QLineSeries *series5;  //数据系列温差温差
    QPieSeries *series6;  //数据系列风力

    QScatterSeries *scatterSeries1;//最高气温
    QScatterSeries *scatterSeries4;//最低气温
    QScatterSeries *scatterSeries5;//温差

    QStackedWidget *stackedWidget;



    QBarSet* barset1;  //柱状图的"柱"
    QBarSet* barset2;
    QBarSet* barset3;
    QBarSet* barset4;
    QBarSet* barset5;
    QBarSet* barset6;
    QBarSet* barset7;
    QBarSet* barset8;
    QBarSet* barset9;

    //设置四个按钮
    QPushButton* btn1;
    QPushButton* btn2;
    QPushButton* btn3;
    QPushButton* btn4;
    QPushButton* btn5;

    QLabel *  infoLabel;




    QChart *chart1;  //图表
    QChart *chart4;  //图表
    QChart *chart2;  //图表
    QChart *chart3;  //图表
    QChart *chart6;  //图表

    QChartView *chartview1;  //图表视图
    QChartView *chartview4;  //图表视图
    QChartView *chartview2;  //图表视图
    QChartView *chartview3;  //图表视图
    QChartView *chartview6;  //图表视图

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
    void updateTooltip5(QPointF point, bool state);
    void updateTooltip6(QPointF point, bool state);
    void updateTooltip7(QPieSlice *slice, bool state);
    bool connectToDatabase();
    void updateChartAndLabel(QChartView *chartview, const QString &text);
    void btn1onclicked();
    void btn2onclicked();
    void btn3onclicked();
    void btn4onclicked();
    void btn5onclicked();




    void mydraw();

private slots:  // Add this line
    void onDrawButtonClicked();  // Add this line


private:
    Ui::datavisualization *ui;
};





#endif // DATAVISUALIZATION_H
