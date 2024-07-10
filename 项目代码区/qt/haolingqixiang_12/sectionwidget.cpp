#include "sectionwidget.h"
#include <QHBoxLayout>
#include <QStringList>

SectionWidget::SectionWidget(const QString &weather, const QString &temp, const QString &winddir, const QString &windstr,const QString &humidity,QWidget *parent)
    : QWidget(parent)
{
    // 创建标签
    weatherLabelpic1 = new QLabel(this);
    weatherLabelpic2 = new QLabel(this);

    QString weather1, weather2;
    if (weather.contains("转"))
    {
        QStringList weatherTypes = weather.split("转");
        weather1 = weatherTypes.value(0, "未知");
        weather2 = weatherTypes.value(1, weather1);
    }
    else if (weather.contains("到"))
    {
        QStringList weatherTypes = weather.split("到");
        weather1 = weatherTypes.value(0, "未知");
        weather2 = weatherTypes.value(1, weather1);
    }
    else if (weather.contains("~"))
    {
        QStringList weatherTypes = weather.split("~");
        weather1 = weatherTypes.value(0, "未知");
        weather2 = weatherTypes.value(1, weather1);
    }
    else
    {
        weather1 = weather;
        weather2 = weather;
    }

    drawWeather(weather1, weather2);

    weatherLabel = new QLabel(weather, this);
    tempLabel = new QLabel("温度: " + temp + "°C", this);
    winddLabel = new QLabel("风向: " + winddir, this);
    windsLabel = new QLabel("风力: " + windstr, this);
    humidityLabel = new QLabel("湿度：" + humidity); //湿度


    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(weatherLabelpic1);
    mainLayout->addWidget(weatherLabelpic2);
    mainLayout->addWidget(weatherLabel);
    mainLayout->addWidget(tempLabel);
    mainLayout->addWidget(winddLabel);
    mainLayout->addWidget(windsLabel);
    mainLayout->addWidget(humidityLabel);
}

void SectionWidget::drawWeather(const QString &weatherType1, const QString &weatherType2)
{
    QPixmap weatherpyin(":/weather/weatherp/overcas.png");
    QPixmap weatherpqing(":/weather/weatherp/sunny.png");
    QPixmap weatherpduoyun(":/weather/weatherp/cloudy.png");

    if (weatherType1 == "阴")
        weatherLabelpic1->setPixmap(weatherpyin);
    else if (weatherType1 == "晴")
        weatherLabelpic1->setPixmap(weatherpqing);
    else if (weatherType1 == "多云")
        weatherLabelpic1->setPixmap(weatherpduoyun);
    else
        weatherLabelpic1->setText("未知");

    if (weatherType2 == "阴")
        weatherLabelpic2->setPixmap(weatherpyin);
    else if (weatherType2 == "晴")
        weatherLabelpic2->setPixmap(weatherpqing);
    else if (weatherType2 == "多云")
        weatherLabelpic2->setPixmap(weatherpduoyun);
    else
        weatherLabelpic2->setText("未知");

    weatherLabelpic1->show();
    weatherLabelpic2->show();
}
