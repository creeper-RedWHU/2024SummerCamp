#include "datavisualization.h"
#include "ui_datavisualization.h"
#include <QtCharts>
#include <QDebug>


datavisualization::datavisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datavisualization)
{
    ui->setupUi(this);

    connectToDatabase();
    //先连接数据库
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "数据可视化界面：无法连接到数据库，请检查配置。");
    }
    else
    {
        qDebug()<<"数据可视化界面界面成功连接数据库";
    }

    QHBoxLayout *controlLayout = new QHBoxLayout();

    cityComboBox = new QComboBox(this);
    cityComboBox->addItems({"北京", "上海", "长沙","南京","杭州","武汉"});
    controlLayout->addWidget(new QLabel("选择城市:", this));
    controlLayout->addWidget(cityComboBox);

    YearComboBox = new QComboBox(this);
    YearComboBox->addItems({"2011","2012","2013","2014","2015","2016","2017","2018","2019","2020", "2021", "2022","2023","2024"});
    controlLayout->addWidget(new QLabel("选择年份:", this));
    controlLayout->addWidget(YearComboBox);

    MonthComboBox = new QComboBox(this);
    MonthComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12"});
    controlLayout->addWidget(new QLabel("选择月份:", this));
    controlLayout->addWidget(MonthComboBox);

    startDayComboBox = new QComboBox(this);
    startDayComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"});
    controlLayout->addWidget(new QLabel("选择起始日期:", this));
    controlLayout->addWidget(startDayComboBox);

    endDayComboBox = new QComboBox(this);
    endDayComboBox->addItems({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"});
    controlLayout->addWidget(new QLabel("选择中止日期:", this));
    controlLayout->addWidget(endDayComboBox);

    QPushButton *drawButton = new QPushButton("绘制", this);
    controlLayout->addWidget(drawButton);
    connect(drawButton, &QPushButton::clicked, this, &datavisualization::onDrawButtonClicked);  // Modify this line


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





    series1 = new QLineSeries;
    series4 = new QLineSeries;
    series2 = new QPieSeries;
    series3 = new QBarSeries;
    scatterSeries = new QScatterSeries();



    // 创建图表并添加数据系列
    chart1 = new QChart();
    chart1->legend()->hide();
    chart1->addSeries(series1);
    chart1->createDefaultAxes();
    chart1->setTitle("最高气温折线图");

    chart4 = new QChart();
    chart4->legend()->hide();
    chart4->addSeries(series4);
    chart4->createDefaultAxes();
    chart4->setTitle("最低气温折线图");

    chart2 = new QChart();
    chart2->legend()->hide();
    chart2->addSeries(series2);
    chart2->createDefaultAxes();
    chart2->setTitle("天气饼状图");

    chart3 = new QChart();
    chart3->legend()->hide();
    chart3->addSeries(series3);
    chart3->createDefaultAxes();
    chart3->setTitle("风向柱状图");


    // 创建图表视图并设置渲染提示
    chartview1 = new QChartView(chart1);
    chartview1->setRenderHint(QPainter::Antialiasing);

    chartview4 = new QChartView(chart4);
    chartview4->setRenderHint(QPainter::Antialiasing);

    chartview2 = new QChartView(chart2);
    chartview2->setRenderHint(QPainter::Antialiasing);

    chartview3 = new QChartView(chart3);
    chartview3->setRenderHint(QPainter::Antialiasing);

    // 连接鼠标悬停事件信号与槽函数

    connect(series4, &QLineSeries::hovered, this, &datavisualization::updateTooltip1);
    connect(series2, &QPieSeries::hovered, this, &datavisualization::updateTooltip2);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);
    connect(scatterSeries, &QScatterSeries::hovered, this, &datavisualization::updateTooltip4);



    // 布局设置
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(chartview1);
    hlayout1->addWidget(chartview4);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(chartview2);
    hlayout2->addWidget(chartview3);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(controlLayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);


    this->setLayout(nullptr);
    this->setLayout(vlayout);


}

datavisualization::~datavisualization()
{
    delete ui;

}

//连接数据库
bool datavisualization::connectToDatabase()
{
    // db = QSqlDatabase::addDatabase("QMYSQL");
    // db.setHostName("60.205.232.122");
    // db.setDatabaseName("data");
    // db.setUserName("root");
    // db.setPassword("123456");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setUserName("root");
    //设置数据库的密码
    db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称

    if (!db.open()) {
        qDebug() << "Database error occurred:" << db.lastError();
        return false;
    }
    return true;
}

// 点击绘制按钮时触发的槽函数
void datavisualization::onDrawButtonClicked() {
    int startDay = startDayComboBox->currentText().toInt();
    int endDay = endDayComboBox->currentText().toInt();

    if (startDay > endDay) {
        QMessageBox::warning(this, "日期错误", "起始日期不能大于中止日期，请重新选择！");
        return;
    }

    mydraw();
}

//根据数据绘制图表
void datavisualization::mydraw()
{
    connectToDatabase();

    QString Qcity = cityComboBox->currentText();
    int Qyear = YearComboBox->currentText().toInt();
    int Qmonth = MonthComboBox->currentText().toInt();
    int Qstartday = startDayComboBox->currentText().toInt();
    int Qendday = endDayComboBox->currentText().toInt();



    // 查询数据库获取最高气温数据
    QSqlQuery query;  // 使用现有的数据库连接
    query.prepare("SELECT day,max_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query.bindValue(":city", Qcity);
    query.bindValue(":year", Qyear);
    query.bindValue(":month", Qmonth);
    query.bindValue(":sday", Qstartday);
    query.bindValue(":eday", Qendday);

    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }


    // 清空原有数据
    series1->clear();
    scatterSeries->clear();


    // 清空原有散点数据
    foreach (QAbstractSeries *s, chart1->series()) {
        chart1->removeSeries(s);
    }

    // 填充新数据，同时添加散点数据

    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置散点形状为圆形
    scatterSeries->setMarkerSize(9); // 设置散点大小为 4.5 像素
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis();


    int minY = 100;  // 初始化为一个非常大的数，作为最小值
    int maxY = -100;  // 初始化为一个非常小的数，作为最大值
    int minX = 100;  // 初始化为一个非常大的数，作为最小值
    int maxX = -100;  // 初始化为一个非常小的数，作为最大值

    while (query.next()) {
        int day = query.value("day").toInt(); // 获取天数
        int y1_value = query.value("max_temperature").toInt();
        series1->append(day, y1_value); // 添加折线图数据
        scatterSeries->append(day, y1_value); // 添加散点数据

        // 更新最小和最大温度值
        if (y1_value < minY) {
            minY = y1_value;
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
    yAxis->setRange(minY-1, maxY+1);
    // 将折线图系列添加到图表中
    chart1->addSeries(series1);

    // 将散点系列添加到图表中，并绑定轴
    chart1->addSeries(scatterSeries);
    scatterSeries->attachAxis(xAxis);
    scatterSeries->attachAxis(yAxis);

    // 更新图表1的标题
    chart1->setTitle("最高气温折线图（含散点）");
    qDebug() << "Updated chart1 title to '最高气温折线图（含散点）'.";

    chart1->removeAxis(chart1->axisX());
    chart1->removeAxis(chart1->axisY());

    // 修改图表1的 x 轴信息
    xAxis->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis->setTitleText("日期（天）"); // 设置轴标题
    chart1->addAxis(xAxis, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series1->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    scatterSeries->attachAxis(xAxis); // 将散点系列与 x 轴绑定
    qDebug() << "Updated chart1 x-axis format and title.";

    // 修改图表1的 y 轴信息

    yAxis->setLabelsVisible(true); // 设置轴标签可见
    yAxis->setTitleText("最高气温/°C"); // 设置轴标题
    chart1->addAxis(yAxis, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series1->attachAxis(yAxis); // 将 series1 与 y 轴绑定
    scatterSeries->attachAxis(yAxis); // 将散点系列与 y 轴绑定
    qDebug() << "Updated chart1 y-axis format and title.";

    // 刷新图表1显示
    chartview1->update();  // 或者 chartview1->repaint();
    qDebug() << "Updated chartview1.";







    QSqlQuery query1(db);
    query1.prepare("SELECT weather FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query1.bindValue(":city", Qcity);
    query1.bindValue(":year", Qyear);
    query1.bindValue(":month", Qmonth);
    query1.bindValue(":sday", Qstartday);
    query1.bindValue(":eday", Qendday);

    // 执行查询语句
    if (!query1.exec()) {
        qDebug() << "Query execution failed. Error:" << query1.lastError().text();
    } else {
        qDebug() << "Query executed successfully.";

        QMap<QString, int> weatherCounts;
        weatherCounts.insert("阴", 0);
        weatherCounts.insert("小雨", 0);
        weatherCounts.insert("晴", 0);
        weatherCounts.insert("多云", 0);
        weatherCounts.insert("中雨", 0);
        weatherCounts.insert("雾~多云", 0);
        weatherCounts.insert("雾~晴", 0);
        weatherCounts.insert("晴~多云", 0);
        weatherCounts.insert("多云~晴", 0);
        weatherCounts.insert("小雨~阴", 0);
        weatherCounts.insert("雷阵雨~阵雨", 0);
        weatherCounts.insert("雷阵雨~多云", 0);
        weatherCounts.insert("多云~小雨", 0);
        weatherCounts.insert("雷阵雨", 0);
        weatherCounts.insert("雾", 0);
        weatherCounts.insert("多云~大雨", 0);
        weatherCounts.insert("阴~多云", 0);
        weatherCounts.insert("大雨~小雨", 0);
        weatherCounts.insert("多云~中雨", 0);
        weatherCounts.insert("阴~多云", 0);
        weatherCounts.insert("阴~中雨", 0);
        weatherCounts.insert("雾~阴", 0);
        weatherCounts.insert("多云~阴", 0);
        weatherCounts.insert("阴~晴", 0);
        weatherCounts.insert("中雨~小雨", 0);
        weatherCounts.insert("多云~雨", 0);
        weatherCounts.insert("小雨~雨", 0);
        weatherCounts.insert("中雨~雨", 0);
        weatherCounts.insert("小雨~雨", 0);
        weatherCounts.insert("阴~小雨", 0);
        weatherCounts.insert("大雨~多云", 0);
        weatherCounts.insert("小雨~晴", 0);
        weatherCounts.insert("中雨~多云", 0);
        weatherCounts.insert("扬沙~多云", 0);
        weatherCounts.insert("晴~小雨", 0);
        weatherCounts.insert("晴~阴", 0);
        weatherCounts.insert("中雨~大雨", 0);
        weatherCounts.insert("小雨~多云", 0);
        weatherCounts.insert("雾~小雨", 0);
        weatherCounts.insert("阴~小雨", 0);
        weatherCounts.insert("小雨~晴", 0);
        weatherCounts.insert("雾~中雨", 0);
        weatherCounts.insert("暴雨~晴", 0);
        weatherCounts.insert("雾~大雨", 0);


        weatherCounts.insert("多云转大雨", 0);
        weatherCounts.insert("阴转多云", 0);
        weatherCounts.insert("大雨转小雨", 0);
        weatherCounts.insert("多云转中雨", 0);
        weatherCounts.insert("阴转多云", 0);
        weatherCounts.insert("阴转中雨", 0);
        weatherCounts.insert("雾转阴", 0);
        weatherCounts.insert("多云转阴", 0);
        weatherCounts.insert("阴转晴", 0);
        weatherCounts.insert("中雨转小雨", 0);
        weatherCounts.insert("雾转多云", 0);
        weatherCounts.insert("雾转晴", 0);
        weatherCounts.insert("晴转多云", 0);
        weatherCounts.insert("多云转晴", 0);
        weatherCounts.insert("小雨转阴", 0);
        weatherCounts.insert("多云转雨", 0);
        weatherCounts.insert("小雨转雨", 0);
        weatherCounts.insert("中雨转雨", 0);
        weatherCounts.insert("小雨转雨", 0);

        QMap<QString, QColor> weatherColors;
        for (auto it = weatherCounts.begin(); it != weatherCounts.end(); ++it) {
            int red = QRandomGenerator::global()->bounded(256);    // 随机生成 0 到 255 之间的整数
            int green = QRandomGenerator::global()->bounded(256);
            int blue = QRandomGenerator::global()->bounded(256);
            QColor randomColor(red, green, blue);
            weatherColors.insert(it.key(), randomColor);
        }


        while (query1.next()) {
            QString weather = query1.value(0).toString();
            if (weatherCounts.contains(weather)) {
                weatherCounts[weather]++;
            } else {
                qDebug() << "Unknown weather type encountered:" << weather;
            }
        }

        // qDebug() << "Weather counts:";
        // for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
        //     qDebug() << it.key() << ":" << it.value();
        // }

        series2->clear();
        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            QString weather = it.key();
            int count = it.value();

            if (count > 0) {
                QPieSlice *slice = series2->append(weather, count);

                if (weatherColors.contains(weather)) {
                    slice->setBrush(weatherColors[weather]);
                } else {
                    // Handle unknown weather type color
                    slice->setBrush(Qt::black); // or any default color
                }
                // Set label to weather type
                slice->setLabel(weather);
                slice->setLabelVisible(true); // Make label visible
            }

        }

        chart2->setTitle("天气饼状图");
        qDebug() << "Chart title updated to:" << chart2->title();

        chartview2->update();

        qDebug() << "Chart2 updated with new data. Series count:" << series2->count();

        qDebug() << "Data processing and chart update completed.";
    }





    QSqlQuery query2;
    query2.prepare("SELECT wind_direction FROM climate WHERE city = :city AND year = :year AND month = :month AND day BETWEEN :sday AND :eday");
    query2.bindValue(":city", Qcity);
    query2.bindValue(":year", Qyear);
    query2.bindValue(":month", Qmonth);
    query2.bindValue(":sday", Qstartday);
    query2.bindValue(":eday", Qendday);
    query2.exec();

    QBarSet *barset1 = new QBarSet("东风");
    QBarSet *barset2 = new QBarSet("西风");
    QBarSet *barset3 = new QBarSet("南风");
    QBarSet *barset4 = new QBarSet("北风");
    QBarSet *barset5 = new QBarSet("东南风");
    QBarSet *barset6 = new QBarSet("东北风");
    QBarSet *barset7 = new QBarSet("西南风");
    QBarSet *barset8 = new QBarSet("西北风");
    QBarSet *barset9 = new QBarSet("无持续风向");

    barset1->setColor(Qt::red);
    barset2->setColor(Qt::blue);
    barset3->setColor(Qt::green);
    barset4->setColor(Qt::darkGreen);
    barset5->setColor(Qt::cyan);
    barset6->setColor(Qt::magenta);
    barset7->setColor(Qt::darkYellow);
    barset8->setColor(Qt::darkRed);
    barset9->setColor(Qt::gray);

    // 初始化八个风向的风力计数器
    int wind1 = 0;  // 东风
    int wind2 = 0;  // 西风
    int wind3 = 0;  // 南风
    int wind4 = 0;  // 北风
    int wind5 = 0;  // 东南风
    int wind6 = 0;  // 东北风
    int wind7 = 0;  // 西南风
    int wind8 = 0;  // 西北风
    int wind9 = 0;  // 微风

    while (query2.next()) {
        QString wind_direction = query2.value(0).toString();

        // 根据风向更新计数器
        if (wind_direction == "东风") {
            wind1++;
        } else if (wind_direction == "西风") {
            wind2++;
        } else if (wind_direction == "南风") {
            wind3++;
        } else if (wind_direction == "北风") {
            wind4++;
        } else if (wind_direction == "东南风") {
            wind5++;
        } else if (wind_direction == "东北风") {
            wind6++;
        } else if (wind_direction == "西南风") {
            wind7++;
        } else if (wind_direction == "西北风") {
            wind8++;
        } else if (wind_direction == "微风") {
            wind9++;
        }else if (wind_direction == "无持续风向") {
            wind9++;
        }



    }

    // 将风力计数器的值添加到对应的 QBarSet 中
    *barset1 << wind1;
    *barset2 << wind2;
    *barset3 << wind3;
    *barset4 << wind4;
    *barset5 << wind5;
    *barset6 << wind6;
    *barset7 << wind7;
    *barset8 << wind8;
    *barset9 << wind9;

    // 将八个风向的 QBarSet 添加到 QBarSeries 中
    QBarSeries *series3 = new QBarSeries();
    series3->append(barset1);
    series3->append(barset2);
    series3->append(barset3);
    series3->append(barset4);
    series3->append(barset5);
    series3->append(barset6);
    series3->append(barset7);
    series3->append(barset8);
    series3->append(barset9);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);



    // Add space to label to add space between labels and axis在标签和轴之间加空格
    QValueAxis *axisY3 = qobject_cast<QValueAxis*>(chart3->axes(Qt::Vertical).first());
    Q_ASSERT(axisY3);
    axisY3->setLabelFormat("%.2f");

    series3->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置柱状图标签显示的位置
    series3->setLabelsVisible(true);  //设置柱状图数据标签可见




    // 清除旧数据并设置新数据集到柱状图
    chart3->removeAllSeries();
    chart3->addSeries(series3);
    chart3->setTitle("风向统计图表");
    chart3->legend()->setVisible(true);
    chart3->legend()->setAlignment(Qt::AlignBottom);

    QStringList categories;
    categories << "风向" ;  //保存横坐标字符串的列表
    QBarCategoryAxis *axis4 = new QBarCategoryAxis();
    axis4->append(categories);
    chart3->createDefaultAxes();
    chart3->setAxisX(axis4, series3);

    // 释放查询结果和内存
    query2.finish();

    chartview2->update();





    // 查询数据库获取最高气温数据
    QSqlQuery query3;  // 使用现有的数据库连接
    query3.prepare("SELECT day,min_temperature FROM climate WHERE city = :city AND year = :year AND month = :month AND day <= :eday AND day >= :sday");
    query3.bindValue(":city", Qcity);
    query3.bindValue(":year", Qyear);
    query3.bindValue(":month", Qmonth);
    query3.bindValue(":sday", Qstartday);
    query3.bindValue(":eday", Qendday);

    if (!query3.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }


    // 清空原有数据
    series4->clear();

    // 填充新数据
    while (query3.next()) {
        int day = query3.value("day").toInt(); // 获取天数
        int y1_value = query3.value("min_temperature").toInt();
        series4->append(day, y1_value); // 使用天数作为 x 轴坐标
    }

    // 更新图表1
    chart4->removeSeries(series4);
    chart4->addSeries(series4);

    // 更新图表1的标题
    chart4->setTitle("最低气温折线图");


    chart4->removeAxis(chart4->axisX());
    chart4->removeAxis(chart4->axisY());

    // 修改图表1的 x 轴信息
    QValueAxis *xAxis4 = new QValueAxis;
    xAxis4->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis4->setTitleText("日期（天）"); // 设置轴标题
    chart4->addAxis(xAxis4, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series4->attachAxis(xAxis4); // 将 series1 与 x 轴绑定

    // 修改图表1的 y 轴信息
    QValueAxis *yAxis4 = new QValueAxis();
    yAxis4->setLabelsVisible(true); // 设置轴标签可见
    yAxis4->setTitleText("最高气温/°C"); // 设置轴标题
    chart4->addAxis(yAxis4, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series4->attachAxis(yAxis4); // 将 series1 与 y 轴绑定

    // 刷新图表1显示
    chartview4->update();  // 或者 chartview1->repaint();
    qDebug() << "Updated chartview1.";





}

//处理折现图的悬停信号
void datavisualization::updateTooltip1(QPointF point, bool state)
{
    if (state) {
        // 格式化提示信息
        QString tooltip = QString("日期: %1, 气温: %2°C")
                              .arg(QString::number(point.x(), 'f', 1)) // x 轴数据保留一位小数
                              .arg(QString::number(point.y(), 'f', 1)); // y 轴数据保留一位小数
        // 设置提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 如果鼠标不在数据点上，则隐藏提示信息
        QToolTip::hideText();
    }
}
void datavisualization::updateTooltip2(QPieSlice *slice, bool state)
{
    if (state && slice) {
        QString tooltip = QString("%1: %2天").arg(slice->label()).arg(slice->value());
        QToolTip::showText(QCursor::pos(), tooltip);

        // 放大当前悬停的扇形
        slice->setExploded(true);
        slice->setLabelVisible(true); // 可选：显示扇形的标签

        // 如果有需要，您还可以修改扇形的颜色或其他属性
        // slice->setBrush(...);

    } else {
        QToolTip::hideText();

        // 恢复扇形的正常大小
        if (slice) {
            slice->setExploded(false);
        }
    }
}

//处理柱状图

void datavisualization::updateTooltip3(bool hovered, int index, QBarSet* barset)
{
    static QColor originalColor; // 使用静态变量来保存原始颜色

    if (hovered && barset && index != -1) {
        // 获取柱子的颜色并保存原始颜色
        if (!originalColor.isValid()) {
            originalColor = barset->color(); // 只在第一次保存原始颜色
        }

        // 放大柱子的效果，例如增加柱子的宽度或者修改颜色
        barset->setColor(originalColor.lighter(150));  // 使用稍亮的颜色

        QString tooltip = QString(" %1: %2天").arg(barset->label()).arg(barset->at(index));
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 恢复原始状态
        if (barset && originalColor.isValid()) {
            barset->setColor(originalColor);  // 恢复原始颜色
        }
        QToolTip::hideText();
        originalColor = QColor(); // 重置原始颜色为无效状态，以便下次重新保存
    }
}






// 处理折线散点图的悬停信号
void datavisualization::updateTooltip4(QPointF point, bool state)
{

    if (state) {
        // 放大散点
        scatterSeries->setMarkerSize(12); // 设置散点大小为10（放大）

        // 格式化提示信息
        QString tooltip = QString("日期: %1号, 气温: %2°C").arg(point.x()).arg(point.y());

        // 显示提示信息
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        // 鼠标移出散点范围，恢复原始大小
        scatterSeries->setMarkerSize(9); // 设置散点大小为9（原始大小）

        // 隐藏提示信息
        QToolTip::hideText();
    }
}



