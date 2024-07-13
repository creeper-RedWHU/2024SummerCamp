#include "simulation.h"
#include <QDebug>
#include "database.h"

Simulation::Simulation(QWidget *parent)
    : QWidget(parent)
{
    database thisdb;

    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "数据模拟无法连接到数据库，请检查配置。");
    } else {
        qDebug() << "成功连接数据库";
    }

    lowerTempLineEdit = new QLineEdit(this);
    upperTempLineEdit = new QLineEdit(this);
    simulateButton = new QPushButton("模拟", this);
    imageLabel = new QLabel(this);

    tempValidator = new QIntValidator(-30, 50, this);
    lowerTempLineEdit->setValidator(tempValidator);
    upperTempLineEdit->setValidator(tempValidator);

    QHBoxLayout *tempLayout = new QHBoxLayout;
    tempLayout->addWidget(new QLabel("下限温度:"));
    tempLayout->addWidget(lowerTempLineEdit);
    tempLayout->addWidget(new QLabel("上限温度:"));
    tempLayout->addWidget(upperTempLineEdit);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(tempLayout);
    topLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    topLayout->addWidget(simulateButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(imageLabel);

    setLayout(mainLayout);


    connect(simulateButton, &QPushButton::clicked, this, &Simulation::onSimulateClicked);

    // QSS样式
    QString styleSheet = R"(
        QLineEdit {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 5px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
            font-weight: bold;
        }

        QPushButton {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 20px;
            font-family: "Arial", sans-serif;
            font-size: 18px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #d6e6f2;
            border: 1px solid #d6e6f2;
        }

        QPushButton:pressed {
            background-color: #b9d7ea;
            border: 1px solid #b9d7ea;
        }

        QLabel {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 10px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }
    )";

    setStyleSheet(styleSheet);

    series1 = new QLineSeries;//最高气温折线
    series4 = new QLineSeries;//最低气温折线
    scatterSeries1 = new QScatterSeries();//最高气温散点
    scatterSeries4 = new QScatterSeries();//最低气温散点

    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("气温折线图");

    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    connect(scatterSeries1, &QScatterSeries::hovered, this, &Simulation::updateTooltip4);
    connect(scatterSeries4, &QScatterSeries::hovered, this, &Simulation::updateTooltip5);

    // Add chartView to the layout
    mainLayout->addWidget(chartview1);
}

Simulation::~Simulation()
{
}


void Simulation::draw()
{
    //已修改
    
    // 查询数据库获取最高气温数据
    QSqlQuery query;  // 使用现有的数据库连接

    //需要修改climate为后续创建的表
    query.prepare("SELECT time,temperature FROM anolog_data");


    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }


    // 清空原有数据
    series1->clear();
    scatterSeries1->clear();
    series4->clear();
    scatterSeries4->clear();


    // 清空原有散点数据
    foreach (QAbstractSeries *s, chart1->series()) {
        chart1->removeSeries(s);
    }

    // 填充新数据，同时添加散点数据

    scatterSeries1->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries1->setMarkerSize(9); // 设置散点大小为 4.5 像素
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis();

    scatterSeries4->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries4->setMarkerSize(9); // 设置散点大小为 4.5 像素




    int minY = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY = -100;  // 初始化为一个非常小的数，作为最大值
    int minX = 100;  // 初始化为一个非常大的数，作为最小值
    int maxX = -100;  // 初始化为一个非常小的数，作为最大值

    while (query.next()) {
        int day = query.value("day").toInt(); // 获取天数
        int y1_value = query.value("max_temperature").toInt();
        int y2_value = query.value("min_temperature").toInt();
        series1->append(day, y1_value); // 添加折线图数据
        scatterSeries1->append(day, y1_value); // 添加散点数据
        series4->append(day, y2_value); // 添加折线图数据
        scatterSeries4->append(day, y2_value); // 添加散点数据

        // 更新最小和最大温度值
        if (y2_value < minY) {
            minY = y2_value;
        }
        if (y1_value > maxY) {
            maxY = y1_value;
        }

        // 更新最小和最大天数
        if (day < minX) {
            minX = day;
        }
        if (day > maxX) {
            maxX = day;
        }
    }
    xAxis->setRange(minX-0.5, maxX+0.5);
    yAxis->setRange(minY-2, maxY+2);

    series1->setName("最高气温折线");// 设置名称
    series4->setName("最低气温折线");
    scatterSeries1->setName("最高气温散点");
    scatterSeries4->setName("最低气温散点");

    QPen maxTempPen(Qt::red); // 最高气温折线颜色为红色
    QPen minTempPen(Qt::blue); // 最低气温折线颜色为蓝色

    QPen maxTempPointpen(Qt::green);
    QPen minTempPointpen(Qt::green);

    scatterSeries1->setPen(maxTempPointpen);
    scatterSeries4->setPen(minTempPointpen);
    series1->setPen(maxTempPen); // 设置最高气温折线系列的画笔
    series4->setPen(minTempPen); // 设置最低气温折线系列的画笔

    // 将折线图系列添加到图表中
    chart1->addSeries(series1);
    chart1->addSeries(series4);

    chartview1->setRenderHint(QPainter::Antialiasing); // 可选的抗锯齿渲染设置
    // 如果需要，可以显示图例
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignBottom); // 设置图例的位置，例如底部对齐


    // 将散点系列添加到图表中，并绑定轴
    chart1->addSeries(scatterSeries1);
    scatterSeries1->attachAxis(xAxis);
    scatterSeries1->attachAxis(yAxis);
    chart1->addSeries(scatterSeries4);
    scatterSeries4->attachAxis(xAxis);
    scatterSeries4->attachAxis(yAxis);

    // 更新图表1的标题
    chart1->setTitle("气温折线图（含散点）");
    qDebug() << "Updated chart1 title to '气温折线图（含散点）'.";

    chart1->removeAxis(chart1->axisX());
    chart1->removeAxis(chart1->axisY());

    // 修改图表1的 x 轴信息
    xAxis->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis->setTitleText("日期（天）"); // 设置轴标题
    chart1->addAxis(xAxis, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series1->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    scatterSeries1->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    series4->attachAxis(xAxis); // 将 series4 与 x 轴绑定
    scatterSeries4->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    qDebug() << "Updated chart1 x-axis format and title.";

    // 修改图表1的 y 轴信息

    yAxis->setLabelsVisible(true); // 设置轴标签可见
    yAxis->setTitleText("气温/°C"); // 设置轴标题
    chart1->addAxis(yAxis, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series1->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries1->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    series4->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries4->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    qDebug() << "Updated chart1 y-axis format and title.";

    // 刷新图表1显示
    chartview1->update();  // 或者 chartview1->repaint();
    qDebug() << "Updated chartview1.";


}
void Simulation::onSimulateClicked()
{
    bool lowerOk, upperOk;
    int min_temperature = lowerTempLineEdit->text().toInt(&lowerOk);
    int max_temperature = upperTempLineEdit->text().toInt(&upperOk);

    if (!lowerOk || !upperOk) {
        QMessageBox::warning(this, "输入错误", "请输入有效的温度值（-30到50之间的整数）。");
        return;
    }

    if (max_temperature - min_temperature > 50) {
        QMessageBox::warning(this, "温差错误", "温差不能超过50摄氏度。");
        return;
    }


    //调用python函数，添加数据到数据库中 上限气温为max_temperature 下限气温为min_temperature


    draw();
}
void Simulation::updateTooltip4(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries1->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 最高气温: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries1->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}

// 处理折线散点图的悬停信号
void Simulation::updateTooltip5(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries4->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 最低气温: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries4->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}
