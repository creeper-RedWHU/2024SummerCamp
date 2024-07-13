#include "todayweather.h"
#include "ui_todayweather.h"
#include "LifeIndexWidget.h"
#include "database.h"
#include "api.h"
#include <QStackedWidget>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>


todayweather::todayweather(QWidget *parent) :
    QWidget(parent),
    weatherLabel(new QLabel(this))
{
    ui->setupUi(this);
    database thisdb;
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "今日天气界面数据库连接失败", "无法连接到数据库。");
        return;
    }

    // Setup layout
    QHBoxLayout *layout1 = new QHBoxLayout;
    btn = new QPushButton("查询", this);
    cityBox = new QComboBox(this);
    QStringList cities = {"武汉", "北京", "上海", "长沙", "南京", "杭州"};
    cityBox->addItems(cities);

    QFont font1;
    font1.setPointSize(16);
    cityBox->setFont(font1);
    layout1->addWidget(cityBox);
    layout1->addWidget(btn);

    btn->setStyleSheet(
        "QPushButton {"
        "    background-color: #f7fbfc;"
        "    border: none;"
        "    color: #769fcd;"
        "    padding: 10px 20px;"
        "    text-align: center;"
        "    text-decoration: none;"
        "    font-size: 16px;"
        "    margin: 4px 2px;"
        "    cursor: pointer;"
        "    border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d6e6f2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #b9d7ea;"
        "}"
        );

    cityBox->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid gray;"
        "    border-radius: 10px;"
        "    padding: 5px;"
        "    background: white;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 30px;"
        "    border-left-width: 1px;"
        "    border-left-color: darkgray;"
        "    border-left-style: solid;"
        "    border-top-right-radius: 3px;"
        "    border-bottom-right-radius: 3px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/dropdown_arrow.png);"
        "}"
        );

    connect(btn, &QPushButton::clicked, this, &todayweather::search);

    weatherwidget = new QStackedWidget;
    QWidget *blankPage = new QWidget;
    weatherwidget->addWidget(blankPage);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout1);
    mainLayout->addWidget(weatherwidget);
    setLayout(mainLayout);
    this->showMaximized();
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
}

todayweather::~todayweather()
{
    delete ui;
}

void todayweather::search()
{
    QString city = cityBox->currentText();

    //这里需要根据城市调用今天api
    api *todayweather_api=new api;
    todayweather_api->today_weather(city);


    QDateTime currentDateTime = QDateTime::currentDateTime().addSecs(-3600); // 当前时间减一小时

    QVector<int> months, days, hours;
    for (int i = 0; i < 7; ++i) {
        months.append(currentDateTime.date().month());
        days.append(currentDateTime.date().day());
        hours.append(currentDateTime.time().hour());
        currentDateTime = currentDateTime.addSecs(10800); // 在前者的基础上加三小时
    }

    // 输出数组内容
    qDebug() << "Months:" << months;
    qDebug() << "Days:" << days;
    qDebug() << "Hours:" << hours;


    QVector<QString> weathers(7), temperatures(7), humidities(7), wind_directions(7), wind_strengths(7);
    for (int i = 0; i < 7; ++i) {
        getweather(months[i], days[i], hours[i], temperatures[i], humidities[i], weathers[i], wind_directions[i], wind_strengths[i]);
    }

    QWidget *weatherPage = new QWidget;
    QHBoxLayout *weatherlayout1 = new QHBoxLayout(weatherPage);
    QHBoxLayout *weatherlayout2 = new QHBoxLayout;
    QVBoxLayout *weatherlayout3 = new QVBoxLayout;
    QHBoxLayout *weatherlayout4 = new QHBoxLayout;

    QVector<SectionWidget*> widgets;
    QStringList times = {"现在", "一小时后", "二小时后", "三小时后", "四小时后", "五小时后", "六小时后"};
    for (int i = 0; i < 7; ++i) {
        widgets.append(new SectionWidget(times[i], weathers[i], temperatures[i], wind_directions[i], wind_strengths[i], humidities[i], this));
    }

    // 生活标签
    QGridLayout *lifeIndexLayout = new QGridLayout;

    // 生活提示标签部分
    LifeIndexWidget *wearIndex1 = new LifeIndexWidget(":/lifep/clothes.png", "今天适宜短袖", this);
    LifeIndexWidget *wearIndex2 = new LifeIndexWidget(":/lifep/clothes.png", "今天适宜衬衫", this);
    LifeIndexWidget *wearIndex3 = new LifeIndexWidget(":/lifep/clothes.png", "今天适宜棉衣", this);
    LifeIndexWidget *umbrellaIndex1 = new LifeIndexWidget(":/lifep/unbra_open.png", "下雨了 记得带伞", this);
    LifeIndexWidget *umbrellaIndex2 = new LifeIndexWidget(":/lifep/unbra_open.png", "今天不用带伞", this);
    LifeIndexWidget *fluIndex1 = new LifeIndexWidget(":/lifep/cold.png", "少发 感冒", this);
    LifeIndexWidget *fluIndex2 = new LifeIndexWidget(":/lifep/cold.png", "小心 感冒", this);
    LifeIndexWidget *sportIndex1 = new LifeIndexWidget(":/lifep/sports.png", "宜室内运动", this);
    LifeIndexWidget *sportIndex2 = new LifeIndexWidget(":/lifep/sports.png", "宜室外运动", this);

    // 调整LifeIndexWidget大小和字体大小
    auto adjustLifeIndexWidget = [](LifeIndexWidget* widget) {
        widget->setFixedSize(250, 60);
        QFont font = widget->findChild<QLabel*>()->font();
        font.setPointSize(14);
        widget->findChild<QLabel*>()->setFont(font);
    };

    adjustLifeIndexWidget(wearIndex1);
    adjustLifeIndexWidget(wearIndex2);
    adjustLifeIndexWidget(wearIndex3);
    adjustLifeIndexWidget(umbrellaIndex1);
    adjustLifeIndexWidget(umbrellaIndex2);
    adjustLifeIndexWidget(fluIndex1);
    adjustLifeIndexWidget(fluIndex2);
    adjustLifeIndexWidget(sportIndex1);
    adjustLifeIndexWidget(sportIndex2);

    weatherlayout1->addWidget(widgets[0]);
    chartview = new QChartView;
    weatherlayout4->addWidget(chartview);
    weatherlayout4->addLayout(lifeIndexLayout);
    weatherlayout3->addLayout(weatherlayout4);

    // 标签布局已经加进去了
    // 标签各个部分也已经完成了定义
    // 现在要设计算法根据不同的天气情况把这个生活提示加进去
    // 温度与穿衣以及感冒
    if(temperatures[0].toInt() >= 15) {
        lifeIndexLayout->addWidget(wearIndex1);
        lifeIndexLayout->addWidget(fluIndex1);
    } else if(temperatures[0].toInt() < 15 && temperatures[0].toInt() >= 10) {
        lifeIndexLayout->addWidget(wearIndex2);
        lifeIndexLayout->addWidget(fluIndex2);
    } else {
        lifeIndexLayout->addWidget(wearIndex3);
        lifeIndexLayout->addWidget(fluIndex2);
    }

    // 下雨与伞
    bool raining = false;
    for(int i = 0; i < 6; i++) {
        if(weathers[i] == "小雨" || weathers[i] == "中雨" || weathers[i] == "暴雨" || weathers[i] == "阵雨" || weathers[i] == "雷阵雨" || weathers[i] == "雨")
        {
            raining = true;
            break;
        }
    }
    if(raining) {
        lifeIndexLayout->addWidget(umbrellaIndex1);
    } else {
        lifeIndexLayout->addWidget(umbrellaIndex2);
    }

    // 下面是运动、温度和下雨
    if(raining || temperatures[0].toInt() >= 25) {
        lifeIndexLayout->addWidget(sportIndex1);
    } else {
        lifeIndexLayout->addWidget(sportIndex2);
    }

    for (int i = 1; i < 7; ++i) {
        weatherlayout2->addWidget(widgets[i]);
    }

    weatherlayout3->addLayout(weatherlayout2);
    weatherlayout1->addLayout(weatherlayout2);
    weatherlayout1->addLayout(weatherlayout3);

    series = new QLineSeries;
    for (int i = 0; i < 7; ++i) {
        series->append(i, temperatures[i].toInt());
    }

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("气温变化折线图");

    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis();
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间（小时后）");
    xAxis->setTickCount(7);  // 七个数据点
    xAxis->setRange(0, 6);  // 从0到6

    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("气温/°C");

    chart->setAxisX(xAxis, series);
    chart->setAxisY(yAxis, series);

    chartview->setChart(chart);

    chartview->setFixedWidth(450);
    chartview->setFixedHeight(250);
    weatherwidget->removeWidget(weatherwidget->currentWidget());
    weatherwidget->addWidget(weatherPage);
    weatherwidget->setCurrentWidget(weatherPage);
}

void todayweather::getweather(int month, int day, int hour, QString &temp, QString &hum, QString &wea, QString &wd, QString &ws)
{
    QSqlQuery query;
    query.prepare("SELECT temperature, humidity, weather, wind_direction, wind_power "
                  "FROM hours_data WHERE month = :month AND day = :day AND hour = :hour");
    query.bindValue(":month", month);
    query.bindValue(":day", day);
    query.bindValue(":hour", hour);

    if (!query.exec()) {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    if (query.next()) {
        temp = query.value("temperature").toString();
        hum = query.value("humidity").toString();
        wea = query.value("weather").toString();
        wd = query.value("wind_direction").toString();
        ws = query.value("wind_power").toString();
    }
}
