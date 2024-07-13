#include "todayweather.h"
#include "ui_todayweather.h"
#include "sectionwidget.h"
#include "database.h"
#include <QDebug>
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
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QVector<int> months, days, hours;
    for (int i = 0; i < 7; ++i) {
        months.append(currentDateTime.date().month());
        days.append(currentDateTime.date().day());
        hours.append(currentDateTime.time().hour());
        currentDateTime = currentDateTime.addSecs(3600);  // Increment by 1 hour
    }

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

    weatherlayout1->addWidget(widgets[0]);
    chartview = new QChartView;
    QStackedWidget *assistwidget = new QStackedWidget;
    weatherlayout4->addWidget(chartview);
    weatherlayout4->addWidget(assistwidget);
    weatherlayout3->addLayout(weatherlayout4);

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

    if (!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    if (query.next())
    {
        temp = query.value("temperature").toString();
        hum = query.value("humidity").toString();
        wea = query.value("weather").toString();
        wd = query.value("wind_direction").toString();
        ws = query.value("wind_power").toString();
    }
}
