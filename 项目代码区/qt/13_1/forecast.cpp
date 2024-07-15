#include "forecast.h"
#include "api.h"
#include "database.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDate>
#include <QDateTimeAxis>

forecast::forecast(QWidget *parent)
    : QWidget(parent)
{
    database thisdb;

    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "预测功能无法连接到数据库，请检查配置。");
    } else {
        qDebug() << "成功连接数据库";
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建左上角的文本标签
    QLabel *title = new QLabel("基于机器学习的气温趋势预测", this);
    title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #1F2937; margin-top: 10px; margin-left: 10px;");
    mainLayout->addWidget(title);

    // 创建水平布局，用于放置两个按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // 创建选择城市下拉框
    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->addItem("长沙");
    cityBox->addItem("南京");
    cityBox->addItem("杭州");
    buttonLayout->addWidget(cityBox);

    // 创建预测按钮
    btnPredict = new QPushButton("预测", this);
    buttonLayout->addWidget(btnPredict);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setStyleSheet(R"(
        QWidget {
            background-color: #F9FAFB;
            color: #1F2937;
            font-family: "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
        }
        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            border-radius: 5px;
            padding: 5px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
            color: #333333;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 15px;
            border-left-width: 1px;
            border-left-color: #E1E8ED;
            border-left-style: solid;
            border-top-right-radius: 3px;
            border-bottom-right-radius: 3px;
        }
        QComboBox::down-arrow {
            image: url(:/icons/down_arrow.png); /* Ensure you have a suitable icon */
            width: 10px;
            height: 10px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
            border: 1px solid #E1E8ED;
            selection-background-color: #E6EDF5;
            selection-color: #333333;
        }
        QPushButton {
            background-color: #f7fbfc;
            color: #769fcd;
            border: 1px solid #8B8989;
            border-radius: 5px;
            padding: 10px 20px;
            font-family: "Arial", sans-serif;
            font-size: 14px;
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
            background-color: #F3F4F6;
            color: #4B5563;
            font-size: 16px;
        }
    )");
    connect(btnPredict, &QPushButton::clicked, this, &forecast::onPredictClicked);

    series1 = new QLineSeries;  // 最高气温折线
    scatterSeries1 = new QScatterSeries;  // 最高气温散点
    predictionSeries = new QScatterSeries;  // 预测点

    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("气温折线图");

    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    connect(scatterSeries1, &QScatterSeries::hovered, this, &forecast::updateTooltip4);
    connect(predictionSeries, &QScatterSeries::hovered, this, &forecast::updateTooltip4);

    // Add chartView to the layout
    mainLayout->addWidget(chartview1);
}

forecast::~forecast()
{
}

void forecast::onSelectCityClicked()
{
    // 选择对应城市
}

void forecast::onPredictClicked()
{
    api *forecast_api = new api;
    double data;

    connect(forecast_api, &api::predictionReady, this, &forecast::onPredictionReady);

    forecast_api->machine_learning(cityBox->currentText(), data);
}

void forecast::onPredictionReady(double data)
{
    qDebug() << "预测结果:" << data;

    // 传递预测数据给绘图函数
    draw(data);
}

void forecast::draw(double prediction)
{
    QSqlQuery query;

    // 查询数据库获取最近30天的最高气温数据
    QString Qcity = cityBox->currentText();

    query.prepare("SELECT year, month, day, max_temperature FROM climate WHERE city=:city ORDER BY year DESC, month DESC, day DESC LIMIT 30");
    query.bindValue(":city", Qcity);

    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return;
    }

    if (!query.next()) {
        qDebug() << "No data found for the selected city.";
        return;
    }

    // 清空原有数据
    series1->clear();
    scatterSeries1->clear();
    predictionSeries->clear();

    int minY = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY = -100;  // 初始化为一个非常小的数，作为最大值

    QVector<QPointF> points;  // 用于存储查询到的点
    QVector<QDate> dates; // 用于存储查询到的日期

    do {
        int year = query.value("year").toInt();
        int month = query.value("month").toInt();
        int day = query.value("day").toInt();
        int max_temperature = query.value("max_temperature").toInt();

        QDate date(year, month, day);
        dates.append(date);
        points.append(QPointF(date.startOfDay().toMSecsSinceEpoch(), max_temperature));

        if (max_temperature < minY) {
            minY = max_temperature;
        }
        if (max_temperature > maxY) {
            maxY = max_temperature;
        }

    } while (query.next());

    // 逆序排列，确保最新的日期在最后
    std::reverse(points.begin(), points.end());
    std::reverse(dates.begin(), dates.end());

    // 添加查询到的数据点到系列
    for (const QPointF &point : points) {
        series1->append(point);
        scatterSeries1->append(point);
    }

    // 计算预测日期并添加预测数据点到系列
    QDate lastDate = dates.last();
    QDate predictionDate = lastDate.addDays(1);

    series1->append(predictionDate.startOfDay().toMSecsSinceEpoch(), prediction);
    predictionSeries->append(predictionDate.startOfDay().toMSecsSinceEpoch(), prediction);  // 将预测点添加到 predictionSeries

    // 更新最小和最大温度值
    if (prediction < minY) {
        minY = prediction;
    }
    if (prediction > maxY) {
        maxY = prediction;
    }

    // 创建日期轴
    QDateTimeAxis *xAxis = new QDateTimeAxis;
    xAxis->setTickCount(10); // 设置刻度数量
    xAxis->setFormat("yyyy-MM-dd"); // 设置日期格式
    xAxis->setTitleText("日期");

    // 计算最小和最大日期值
    QDate minDate = dates.first();
    QDate maxDate = predictionDate;

    // 将 QDate 转换为 QDateTime
    QDateTime minDateTime = minDate.startOfDay().addDays(-1);
    QDateTime maxDateTime = maxDate.startOfDay().addDays(+1);

    xAxis->setRange(minDateTime, maxDateTime);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(minY - 2, maxY + 2);
    yAxis->setLabelsVisible(true);
    yAxis->setTitleText("气温/°C");

    series1->setName("最高气温折线");
    scatterSeries1->setName("最高气温散点");
    predictionSeries->setName("预测点");

    QPen maxTempPen(Qt::red);
    QPen maxTempPointpen(Qt::green);
    QPen predictionPen(Qt::blue);  // 预测点颜色为蓝色
    scatterSeries1->setPen(maxTempPointpen);
    series1->setPen(maxTempPen);
    predictionSeries->setPen(predictionPen);

    chart1->addSeries(series1);
    chartview1->setRenderHint(QPainter::Antialiasing);
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignBottom);

    chart1->addSeries(scatterSeries1);
    scatterSeries1->attachAxis(xAxis);
    scatterSeries1->attachAxis(yAxis);

    chart1->addSeries(predictionSeries);  // 将 predictionSeries 添加到图表中
    predictionSeries->attachAxis(xAxis);
    predictionSeries->attachAxis(yAxis);

    chart1->setTitle("气温折线图（含散点）");

    chart1->removeAxis(chart1->axisX());
    chart1->removeAxis(chart1->axisY());

    chart1->addAxis(xAxis, Qt::AlignBottom);
    series1->attachAxis(xAxis);
    scatterSeries1->attachAxis(xAxis);
    predictionSeries->attachAxis(xAxis);

    chart1->addAxis(yAxis, Qt::AlignLeft);
    series1->attachAxis(yAxis);
    scatterSeries1->attachAxis(yAxis);
    predictionSeries->attachAxis(yAxis);

    chartview1->update();
}

void forecast::updateTooltip4(QPointF point, bool state)
{
    if (state) {
        if (sender() == scatterSeries1) {
            scatterSeries1->setMarkerSize(12);
        } else if (sender() == predictionSeries) {
            predictionSeries->setMarkerSize(12);
        }
        QString tooltip = QString("日期: %1, 最高气温: %2°C").arg(QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd")).arg(point.y());
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        if (sender() == scatterSeries1) {
            scatterSeries1->setMarkerSize(9);
        } else if (sender() == predictionSeries) {
            predictionSeries->setMarkerSize(9);
        }
        QToolTip::hideText();
    }
}
