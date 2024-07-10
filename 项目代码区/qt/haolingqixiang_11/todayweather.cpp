#include "todayweather.h"
#include "ui_todayweather.h"
#include <QDebug>
#include "database.h"

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

    // 设置按钮和组合框的样式
    btn->setStyleSheet(
        "QPushButton {"
        "    background-color: #8B8989;"
        "    border: none;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    text-align: center;"
        "    text-decoration: none;"
        "    font-size: 16px;"
        "    margin: 4px 2px;"
        "    cursor: pointer;"
        "    border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #EEE5DE;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #CDC5BF;"
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

// bool todayweather::connectToDatabase()
// {
//     if (QSqlDatabase::contains("qt_sql_default_connection")) {
//         db = QSqlDatabase::database("qt_sql_default_connection");
//     } else {
//         db = QSqlDatabase::addDatabase("QMYSQL");
//         db.setHostName("60.205.232.122");
//         db.setDatabaseName("data");
//         db.setUserName("root");
//         db.setPassword("123456");
//     }

//     if (!db.open()) {
//         qDebug() << "数据库连接出错:" << db.lastError();
//         return false;
//     }
//     return true;
// }

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
    QDate todayDate = QDate::currentDate();
    int thisyear = todayDate.year();
    int thismonth = todayDate.month();
    int thisday = todayDate.day();
    QString thiscity = cityBox->currentText();

    // if (!connectToDatabase()) {
    //     QMessageBox::critical(this, "数据库连接失败", "今日天气界面：无法连接到数据库，请检查配置。");
    //     return;
    // }

    QSqlQuery query;
    query.prepare("SELECT * FROM climate WHERE year = :thisyear AND month = :thismonth AND day = :thisday AND city = :thiscity");
    query.bindValue(":thisyear", thisyear);
    query.bindValue(":thismonth", thismonth);
    query.bindValue(":thisday", thisday);
    query.bindValue(":thiscity", thiscity);

    if (query.exec()) {
        qDebug() << "开始查询";
        if (query.next()) {
            qDebug() << "查询成功";
            QString max_temperature = query.value("max_temperature").toString();
            QString min_temperature = query.value("min_temperature").toString();
            QString weather = query.value("weather").toString();
            QString wind_direction = query.value("wind_direction").toString();
            QString wind_strength = query.value("wind_strength").toString();
            showweather(max_temperature, min_temperature, weather, wind_direction, wind_strength);
        } else {
            QMessageBox::warning(this, "错误", "无法查询到该城市今日天气");
            qDebug() << "没有查询到数据";
            clearWeatherDisplay();
        }
    } else {
        qDebug() << "查询执行失败：" << query.lastError().text();
    }
}

void todayweather::showweather(QString maxt, QString mint, QString weather, QString wind_d, QString wind_s)
{
    drawWeather(weather);

    // 创建和设置天气相关的 QLabel
    QFont labelFont1;
    QFont labelFont2;
    labelFont1.setPointSize(64);
    labelFont2.setPointSize(16);

    weatherLabel1 = new QLabel(weather, this);
    weatherLabel1->setFont(labelFont1);
    weatherLabel1->setGeometry(150, 550, 550, 550); // 更大尺寸，占据主要部分

    maxtLabel = new QLabel("最高温度: " + maxt + "°C", this);
    maxtLabel->setFont(labelFont2);
    maxtLabel->setGeometry(150, 600, 300, 50); // 下面的标签较小

    mintLabel = new QLabel("最低温度: " + mint + "°C", this);
    mintLabel->setFont(labelFont2);
    mintLabel->setGeometry(150, 650, 300, 50); // 下面的标签较小

    winddLabel = new QLabel("风向: " + wind_d, this);
    winddLabel->setFont(labelFont2);
    winddLabel->setGeometry(150, 700, 300, 50); // 下面的标签较小

    windsLabel = new QLabel("风力: " + wind_s, this);
    windsLabel->setFont(labelFont2);
    windsLabel->setGeometry(150, 750, 300, 50); // 下面的标签较小

    // 显示所有标签
    weatherLabel1->show();
    maxtLabel->show();
    mintLabel->show();
    winddLabel->show();
    windsLabel->show();
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
