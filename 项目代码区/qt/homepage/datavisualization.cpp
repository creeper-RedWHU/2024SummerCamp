#include "datavisualization.h"
#include "ui_datavisualization.h"
#include <QtCharts>

datavisualization::datavisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datavisualization)
{
    ui->setupUi(this);

    // 创建数据系列
    series1 = new QLineSeries;
    series1->append(0, 6);
    series1->append(2, 4);
    series1->append(3, 8);
    series1->append(7, 4);
    series1->append(10, 5);

    series2 = new QPieSeries;
    series2->append("阴天", 15);
    series2->append("雨天", 7);
    series2->append("晴天", 7);

    series3 = new QBarSeries;
    barset1 = new QBarSet("东风");
    barset2 = new QBarSet("西风");
    barset3 = new QBarSet("南风");
    barset4 = new QBarSet("北风");

    *barset1 << 5;
    *barset2 << 14;
    *barset3 << 9;
    *barset4 << 2;

    series3->append(barset1);
    series3->append(barset2);
    series3->append(barset3);
    series3->append(barset4);

    series4 = new QLineSeries;
    series4->append(2, 5);
    series4->append(3, 7);
    series4->append(6, 7);
    series4->append(7, 3);
    series4->append(10, 4);

    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("折线图样例");

    chart2 = new QChart();
    chart2->legend()->hide();
    chart2->addSeries(series2);
    chart2->createDefaultAxes();
    chart2->setTitle("饼状图样例");

    chart3 = new QChart();
    chart3->legend()->hide();
    chart3->addSeries(series3);
    chart3->createDefaultAxes();
    chart3->setTitle("柱状图样例");

    chart4 = new QChart();
    chart4->legend()->hide();
    chart4->addSeries(series4);
    chart4->createDefaultAxes();
    chart4->setTitle("简单图表4");

    // 创建图表视图并设置渲染提示
    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    chartview2 = new QChartView(chart2);
    chartview2->setRenderHint(QPainter::Antialiasing);

    chartview3 = new QChartView(chart3);
    chartview3->setRenderHint(QPainter::Antialiasing);

    chartview4 = new QChartView(chart4);
    chartview4->setRenderHint(QPainter::Antialiasing);

    // 连接鼠标悬停事件信号与槽函数
    connect(series1, &QLineSeries::hovered, this, &datavisualization::updateTooltip1);
    connect(series2, &QPieSeries::hovered, this, &datavisualization::updateTooltip2);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);

    // 布局设置
    QVBoxLayout *vlayout1 = new QVBoxLayout;
    vlayout1->addWidget(chartview1);
    vlayout1->addWidget(chartview2);

    QVBoxLayout *vlayout2 = new QVBoxLayout;
    vlayout2->addWidget(chartview3);
    vlayout2->addWidget(chartview4);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addLayout(vlayout1);
    hlayout->addLayout(vlayout2);

    this->setLayout(nullptr);
    this->setLayout(hlayout);
}

datavisualization::~datavisualization()
{
    delete ui;

}

//处理折现图的悬停信号
void datavisualization::updateTooltip1(QPointF point, bool state)
{
    if (state) {
        // 格式化提示信息
        QString tooltip = QString("X: %1, Y: %2").arg(point.x()).arg(point.y());
        // 设置提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 如果鼠标不在数据点上，则隐藏提示信息
        QToolTip::hideText();
    }
}
//处理饼状图的悬停信号
void datavisualization::updateTooltip2(QPieSlice *slice, bool state)
{
    if (state && slice) {
        QString tooltip = QString("饼状图 - %1: %2").arg(slice->label()).arg(slice->value());
        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 如果鼠标不在数据点上，则隐藏提示信息
        QToolTip::hideText();
    }
}
//处理柱状图
void datavisualization::updateTooltip3(bool hovered, int index, QBarSet* barset)
{
    if (hovered && barset && index != -1) {
        QString tooltip = QString("柱状图 - %1: %2").arg(barset->label()).arg(barset->at(index));
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        QToolTip::hideText();
    }
}

