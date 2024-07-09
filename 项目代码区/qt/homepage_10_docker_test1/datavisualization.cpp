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

    barset1 = new QBarSet("东风");
    barset2 = new QBarSet("西风");
    barset3 = new QBarSet("南风");
    barset4 = new QBarSet("北风");
    barset5 = new QBarSet("东南风");
    barset6 = new QBarSet("东北风");
    barset7 = new QBarSet("西南风");
    barset8 = new QBarSet("西北风");
    barset9 = new QBarSet("微风");





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
    connect(series1, &QLineSeries::hovered, this, &datavisualization::updateTooltip1);
    connect(series4, &QLineSeries::hovered, this, &datavisualization::updateTooltip1);
    connect(series2, &QPieSeries::hovered, this, &datavisualization::updateTooltip2);
    connect(series3, &QBarSeries::hovered, this, &datavisualization::updateTooltip3);


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
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("60.205.232.122");
    db.setDatabaseName("data");
    db.setUserName("root");
    db.setPassword("123456");

    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// 建立和QSQLITE数据库的连接
    // db.setHostName("127.0.0.1");  //连接本地主机
    // db.setPort(3306);
    // db.setUserName("root");
    // //设置数据库的密码
    // db.setPassword("mt127715318");    //这个就是安装MySQL时设置的密码
    // db.setDatabaseName("/Users/motao/demo1.db");//设置数据库名称

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

    // 填充新数据
    while (query.next()) {
        int day = query.value("day").toInt(); // 获取天数
        int y1_value = query.value("max_temperature").toInt();
        series1->append(day, y1_value); // 使用天数作为 x 轴坐标
    }

    // 更新图表1
    chart1->removeSeries(series1);
    chart1->addSeries(series1);

    // 更新图表1的标题
    chart1->setTitle("最高气温折线图");
    qDebug() << "Updated chart1 title to '最高气温折线图'.";

    chart1->removeAxis(chart1->axisX());
    chart1->removeAxis(chart1->axisY());

    // 修改图表1的 x 轴信息
    QValueAxis *xAxis = new QValueAxis;
    xAxis->setLabelFormat("%d"); // 设置轴标签格式为整数（天数）
    xAxis->setTitleText("日期（天）"); // 设置轴标题
    chart1->addAxis(xAxis, Qt::AlignBottom); // 将 x 轴添加到图表的底部
    series1->attachAxis(xAxis); // 将 series1 与 x 轴绑定
    qDebug() << "Updated chart1 x-axis format and title.";

    // 修改图表1的 y 轴信息
    QValueAxis *yAxis = new QValueAxis();
    yAxis->setLabelsVisible(true); // 设置轴标签可见
    yAxis->setTitleText("最高气温/°C"); // 设置轴标题
    chart1->addAxis(yAxis, Qt::AlignLeft); // 将 y 轴添加到图表的左侧
    series1->attachAxis(yAxis); // 将 series1 与 y 轴绑定
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

        while (query1.next()) {
            QString weather = query1.value(0).toString();
            if (weatherCounts.contains(weather)) {
                weatherCounts[weather]++;
            } else {
                qDebug() << "Unknown weather type encountered:" << weather;
            }
        }

        qDebug() << "Weather counts:";
        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            qDebug() << it.key() << ":" << it.value();
        }

        series2->clear();
        for (auto it = weatherCounts.constBegin(); it != weatherCounts.constEnd(); ++it) {
            series2->append(it.key(), it.value());
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
    QBarSet *barset9 = new QBarSet("微风");

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



    // 清除旧数据并设置新数据集到柱状图
    chart3->removeAllSeries();
    chart3->addSeries(series3);
    chart3->setTitle("风向统计图表");
    chart3->createDefaultAxes();
    chart3->legend()->setVisible(true);
    chart3->legend()->setAlignment(Qt::AlignBottom);

    // 释放查询结果和内存
    query2.finish();

    // 输出调试信息以确认操作完成
    qDebug() << "Chart updated with wind direction data.";

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


