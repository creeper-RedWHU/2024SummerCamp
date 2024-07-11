#include "todayweather.h"
#include "ui_todayweather.h"
#include <QDebug>

todayweather::todayweather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::todayweather),
    weatherLabel(new QLabel(this))  // 初始化 weatherLabel
{
    ui->setupUi(this);

    // 设置布局
    QHBoxLayout *layout1 = new QHBoxLayout;
    btn = new QPushButton("查询", this);
    cityBox = new QComboBox(this);
    cityBox->addItem("武汉");
    cityBox->addItem("北京");
    cityBox->addItem("上海");
    cityBox->addItem("长沙");
    cityBox->addItem("南京");
    cityBox->addItem("杭州");
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

    QHBoxLayout *layout2 = new QHBoxLayout;
    weatherLabel1 = new QLabel(this);
    weatherLabel1->setFixedHeight(30);
    weatherLabel->setFixedSize(30, 30);

    maxtLabel = new QLabel(this);
    mintLabel = new QLabel(this);
    winddLabel = new QLabel(this);
    windsLabel = new QLabel(this);
    humidityLabel = new QLabel(this); //湿度

    layout2->addWidget(weatherLabel);
    layout2->addWidget(weatherLabel1);
    layout2->addWidget(maxtLabel);
    layout2->addWidget(mintLabel);
    layout2->addWidget(winddLabel);
    layout2->addWidget(windsLabel);
    layout2->addWidget(humidityLabel);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);

    weatherLayout = new QHBoxLayout; // 初始化weatherLayout
    mainLayout->addLayout(weatherLayout); // 将weatherLayout添加到mainLayout中

    // 连接查询按钮的点击信号到search函数
    connect(btn, &QPushButton::clicked, this, &todayweather::search);

    // 设置窗口最大化
    this->showMaximized();

    // 禁用布局管理器的自动调整
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
}

todayweather::~todayweather()
{
    delete ui;
}

void todayweather::search()
{
    QString city = cityBox->currentText();

    // 模拟数据
    QVector<QMap<QString, QString>> weatherData = {
        {{"maxtemperature", "30"}, {"mintemperature", "20"}, {"weather", "晴"}, {"winddirection", "北"}, {"windspeed", "5级"}, {"humidity", "60%"}},
        {{"maxtemperature", "28"}, {"mintemperature", "18"}, {"weather", "多云"}, {"winddirection", "南"}, {"windspeed", "3级"}, {"humidity", "55%"}},
        {{"maxtemperature", "25"}, {"mintemperature", "15"}, {"weather", "阴"}, {"winddirection", "东"}, {"windspeed", "2级"}, {"humidity", "70%"}},
        {{"maxtemperature", "27"}, {"mintemperature", "17"}, {"weather", "小雨"}, {"winddirection", "西"}, {"windspeed", "4级"}, {"humidity", "80%"}},
        {{"maxtemperature", "29"}, {"mintemperature", "19"}, {"weather", "晴"}, {"winddirection", "北"}, {"windspeed", "5级"}, {"humidity", "65%"}}
    };

    // 清空之前的weatherLayout内容
    clearWeatherDisplay();

    // 显示天气数据
    for (const auto& data : weatherData) {
        QString maxt = data["maxtemperature"];
        QString mint = data["mintemperature"];
        QString weather = data["weather"];
        QString windd = data["winddirection"];
        QString winds = data["windspeed"];
        QString humidity = data["humidity"];

        SectionWidget *sectionWidget = new SectionWidget(weather, maxt + "°C/" + mint + "°C", windd, winds, humidity);
        weatherLayout->addWidget(sectionWidget);
    }

    // 确保窗口保持最大化
    this->setWindowState(Qt::WindowMaximized);
}

void todayweather::showweather(QString maxt, QString mint, QString weather, QString wind_d, QString wind_s)
{
    maxtLabel->setText("最高温度: " + maxt + "°C");
    mintLabel->setText("最低温度: " + mint + "°C");
    weatherLabel1->setText("天气: " + weather);
    winddLabel->setText("风向: " + wind_d);
    windsLabel->setText("风力: " + wind_s);
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
    else if (weatherType == "多云")
        weatherLabel->setPixmap(weatherpduoyun);
    else
        weatherLabel->setText("未知");
}

void todayweather::clearWeatherDisplay()
{
    QLayoutItem *item;
    while ((item = weatherLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
