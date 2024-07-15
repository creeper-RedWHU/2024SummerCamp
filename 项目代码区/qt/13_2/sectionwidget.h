#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class SectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectionWidget(const QString&time,const QString &weather, const QString &temp, const QString &winddir, const QString &windstr,const QString &humidity, QWidget *parent = nullptr);

private:
    void drawWeather(const QString &weatherType);

    QLabel *weatherLabelpic1;  //图像1
    QLabel *weatherLabelpic2;  //图相2
    QLabel *weatherLabel;  //天气
    QLabel *timeLabel;  //时间
    QLabel *tempLabel;   //最低温度
    QLabel *winddLabel;  //风向
    QLabel *windsLabel;  //风力
    QLabel *humidityLabel; //湿度
    QVBoxLayout *mainLayout;
};

#endif // SECTIONWIDGET_H
