#include "todayweather.h"
#include "ui_todayweather.h"
#include <QDebug>
#include "database.h"
#include "sectionwidget.h"

todayweather::todayweather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::todayweather),
    weatherLabel(new QLabel(this))  // 初始化 weatherLabel
{
    ui->setupUi(this);
    database thisdb;
    if (!thisdb.connectToDatabase()) {
        QMessageBox::critical(this, "数据库连接失败", "今日天气界面：无法连接到数据库，请检查配置。");
    }

    // 设置布局
    QHBoxLayout *layout1 = new QHBoxLayout;
    btn = new QPushButton("查询", this);
    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->setCurrentIndex(0);
    cityBox->setFixedSize(200, 40);

    QFont font1;
    font1.setPointSize(16);
    cityBox->setFont(font1);
    layout1->addWidget(cityBox);
    layout1->addWidget(btn);

    // QPushButton {
//         background-color: #F7F9FC; /* 更改按钮颜色 */
//     color: #769fcd;
//     border: 1px solid #F7F9FC; /* 更改按钮边框颜色 */
//         border-radius: 5px;
//     padding: 10px 20px;
//         font-family: "Arial", sans-serif;
//         font-size: 14px;
//         font-weight: bold;
//     }

// QPushButton:hover {
//         background-color: #d6e6f2; /* 更改按钮悬停颜色 */
//     border: 1px solid #d6e6f2;
//     }

// QPushButton:pressed {
//         background-color: #b9d7ea; /* 更改按钮按下颜色 */
//     border: 1px solid #b9d7ea;
//     }
    // 设置按钮和组合框的样式
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
        "    image: url(:/icons/down_arrow.png);"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid gray;"
        "    selection-background-color: lightgray;"
        "}"
        );

    connect(btn, &QPushButton::clicked, this, &todayweather::search);

    // 布局管理
    topLayout = new QHBoxLayout();
    topLayout->addLayout(layout1);
    topLayout->addStretch();

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void todayweather::drawWeather(const QString &weatherType)
{
    QPixmap weatherpyin(":/weather/weatherp/overcas.png");
    QPixmap weatherpqing(":/weather/weatherp/sunny.png");
    QPixmap weatherpduoyun(":/weather/weatherp/cloudy.png");

    if (weatherType == "阴")
        weatherLabel->setPixmap(weatherpyin);
    else if (weatherType == "晴")
        weatherLabel->setPixmap(weatherpqing);
    else
        weatherLabel->setPixmap(weatherpduoyun);

    weatherLabel->setGeometry(150, 30, 550, 550);  // 设置位置和大小
    weatherLabel->show();
}

void todayweather::search()
{
    //读取现在的时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate todayDate = QDate::currentDate();
    int thismonth = todayDate.month();
    int thisday = todayDate.day();
    int thishour = currentDateTime.time().hour();

    QString thiscity = cityBox->currentText();

    //这边是需要连接python函数，参数是城市名称 然后让它爬取相应的数据并把数据存储在数据库





    //然后我们读取数据库中的信息

    //这里需要根据具体算法完善，需要连续读取几个小时的数据
    //应该是查询hour hour+1 hour+2...若干个
    QSqlQuery query;
    query.prepare("SELECT * FROM hours_data WHERE month = :thismonth AND day = :thisday AND hour = :thishour");
    query.bindValue(":thismonth", thismonth);
    query.bindValue(":thisday", thisday);
    query.bindValue(":thiscity", thiscity);

    if (query.exec()) {
        qDebug() << "开始查询";
        if (query.next()) {
            qDebug() << "查询成功";

            QString temp = query.value("temperature").toString();
            QString humidity = query.value("humidity").toString();
            QString weather = query.value("weather").toString();
            QString wind_direction = query.value("wind_direction").toString();
            QString wind_strength = query.value("wind_strength").toString();

            //这边调用创建section函数
            SectionWidget section1(weather, temp, wind_direction, wind_strength, humidity);

            //然后对这些section进行布局 这样就基本完成了


        } else {
            QMessageBox::warning(this, "错误", "无法查询到该城市今日天气");
            qDebug() << "没有查询到数据";
            clearWeatherDisplay();
        }
    } else {
        qDebug() << "查询执行失败：" << query.lastError().text();
    }
}

void todayweather::clearWeatherDisplay()
{
    if (weatherLabel1) {
        weatherLabel1->hide();
        delete weatherLabel1;
        weatherLabel1 = nullptr;
    }
    if (maxtLabel) {
        maxtLabel->hide();
        delete maxtLabel;
        maxtLabel = nullptr;
    }
    if (mintLabel) {
        mintLabel->hide();
        delete mintLabel;
        mintLabel = nullptr;
    }
    if (winddLabel) {
        winddLabel->hide();
        delete winddLabel;
        winddLabel = nullptr;
    }
    if (windsLabel) {
        windsLabel->hide();
        delete windsLabel;
        windsLabel = nullptr;
    }
    if (weatherLabel) {
        weatherLabel->clear();
    }
}

todayweather::~todayweather()
{
    delete ui;
}
