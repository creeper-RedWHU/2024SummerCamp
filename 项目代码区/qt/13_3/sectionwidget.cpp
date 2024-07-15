#include "sectionwidget.h"
#include <QHBoxLayout>
#include <QFont>

SectionWidget::SectionWidget(const QString &time, const QString &weather, const QString &temp, const QString &winddir,
                             const QString &windstr, const QString &humidity, QWidget *parent)
    : QWidget(parent)
{
    weatherLabelpic1 = new QLabel(this);  // 放图片的标签

    // 创建标签
    weatherLabel = new QLabel(weather, this);
    timeLabel = new QLabel(time, this);
    tempLabel = new QLabel("温度 " + temp + "°C", this);
    winddLabel = new QLabel(winddir, this);
    windsLabel = new QLabel("风力 " + windstr, this);
    humidityLabel = new QLabel("湿度 " + humidity, this);

    // 设置标签居中对齐
    weatherLabelpic1->setAlignment(Qt::AlignCenter);
    weatherLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setAlignment(Qt::AlignCenter);
    tempLabel->setAlignment(Qt::AlignCenter);
    winddLabel->setAlignment(Qt::AlignCenter);
    windsLabel->setAlignment(Qt::AlignCenter);
    humidityLabel->setAlignment(Qt::AlignCenter);

    // 设置标签字体大小
    weatherLabel->setFont(QFont("Microsoft YaHei", 10));
    timeLabel->setFont(QFont("Microsoft YaHei", 10));
    tempLabel->setFont(QFont("Microsoft YaHei", 10));
    winddLabel->setFont(QFont("Microsoft YaHei", 10));
    windsLabel->setFont(QFont("Microsoft YaHei", 10));
    humidityLabel->setFont(QFont("Microsoft YaHei", 10));

    // 布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(weatherLabelpic1);
    mainLayout->addWidget(weatherLabel);
    mainLayout->addWidget(tempLabel);
    mainLayout->addWidget(winddLabel);
    mainLayout->addWidget(windsLabel);
    mainLayout->addWidget(humidityLabel);

    // 调用函数
    drawWeather(weather);
}


void SectionWidget::drawWeather(const QString &weatherType)
{
    QPixmap weatherpovercast(":/weatherp/overcast.png");
    QPixmap weatherpsunny(":/weatherp/sunny.png");
    QPixmap weatherpcloudy(":/weatherp/partly_cloudy.png");
    QPixmap weatherplrain(":/weatherp/little_rain.png");
    QPixmap weatherpmrain(":/weatherp/moderate_rain_and_shower.png");
    QPixmap weatherphrain(":/weatherp/heavy_rain.png");
    QPixmap weatherpshower(":/weatherp/moderate_rain_and_shower.png");
    QPixmap weatherpthunderstorm(":/weatherp/thunderstorm.png");

    // Adjust pixmap sizes if needed
    int desiredWidth = 100;  // Set your desired width here
    int desiredHeight = 100; // Set your desired height here

    weatherpovercast = weatherpovercast.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherpsunny = weatherpsunny.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherpcloudy = weatherpcloudy.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherplrain = weatherplrain.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherpmrain = weatherpmrain.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherphrain = weatherphrain.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherpshower = weatherpshower.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);
    weatherpthunderstorm = weatherpthunderstorm.scaled(desiredWidth, desiredHeight, Qt::KeepAspectRatio);

    // Set pixmap based on weatherType
    if (weatherType == "阴"||weatherType == "阴天")
        weatherLabelpic1->setPixmap(weatherpovercast);
    else if (weatherType == "晴"||weatherType == "晴天")
        weatherLabelpic1->setPixmap(weatherpsunny);
    else if (weatherType == "多云")
        weatherLabelpic1->setPixmap(weatherpcloudy);
    else if (weatherType == "小雨")
        weatherLabelpic1->setPixmap(weatherplrain);
    else if (weatherType == "中雨"||weatherType == "雨")
        weatherLabelpic1->setPixmap(weatherpmrain);
    else if (weatherType == "暴雨")
        weatherLabelpic1->setPixmap(weatherphrain);
    else if (weatherType == "阵雨")
        weatherLabelpic1->setPixmap(weatherpshower);
    else if (weatherType == "雷阵雨")
        weatherLabelpic1->setPixmap(weatherpthunderstorm);
    else
        weatherLabelpic1->setText("图片加载失败");
}
