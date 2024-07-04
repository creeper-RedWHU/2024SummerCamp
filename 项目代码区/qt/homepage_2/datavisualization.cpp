#include "datavisualization.h"
#include "ui_datavisualization.h"
#include <QtCharts>
#include <QDebug>

// int cityidx=0,endyearidx=0,startyearidx=0;
datavisualization::datavisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datavisualization)
{
    ui->setupUi(this);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityComboBox = new QComboBox(this);
    cityComboBox->addItems({"北京", "上海", "广州"});
    controlLayout->addWidget(new QLabel("选择城市:", this));
    controlLayout->addWidget(cityComboBox);

    startYearComboBox = new QComboBox(this);
    startYearComboBox->addItems({"2020", "2021", "2022"});
    controlLayout->addWidget(new QLabel("选择起始年份:", this));
    controlLayout->addWidget(startYearComboBox);

    endYearComboBox = new QComboBox(this);
    endYearComboBox->addItems({"2021", "2022", "2023"});
    controlLayout->addWidget(new QLabel("选择中止年份:", this));
    controlLayout->addWidget(endYearComboBox);

    QPushButton *drawButton = new QPushButton("绘制", this);
    controlLayout->addWidget(drawButton);
    // connect(drawButton, &QPushButton::clicked, this, );

    setStyleSheet(R"(
        QWidget {
            background-color: #F7F7F8;
            color: #000000;
            font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
        }
        QLabel {
            font-size: 14px;
        }
        QComboBox {
            padding: 5px;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            background-color: #FFFFFF;
            font-size: 14px;
        }
        QComboBox::drop-down {
            border-left: 1px solid #CCCCCC;
        }
        QPushButton {
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            background-color: #8B8989;
            color: #FFFFFF;
            font-size: 14px;
            cursor: pointer;
        }
        QPushButton:hover {
            background-color: #EEE5DE;
        }
        QPushButton:pressed {
            background-color: #CDC5BF;
        }
    )");

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
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(chartview1);
    hlayout1->addWidget(chartview2);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(chartview3);
    hlayout2->addWidget(chartview4);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(controlLayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    // controlLayout

    this->setLayout(nullptr);
    this->setLayout(vlayout);


    // QObject::connect(cityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      cityidx = index;
    //                  });
    // QObject::connect(endYearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      endyearidx = index;
    //                  });
    // QObject::connect(startYearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //                  [=](int index) mutable {
    //                      startyearidx = index;
    //                  });

    // 点击查询按钮时连接到槽函数
    // QObject::connect(drawButton, &QPushButton::clicked, this, &datavisualization::mydraw);
}

datavisualization::~datavisualization()
{
    delete ui;

}


//根据数据绘制图表
void datavisualization::mydraw()
{
    // qDebug()<<cityComboBox->currentIndex()<<" "<<startYearComboBox->currentText()<<" "<<endYearComboBox->currentText();
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

