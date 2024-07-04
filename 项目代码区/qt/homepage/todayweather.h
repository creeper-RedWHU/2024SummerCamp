#ifndef TODAYWEATHER_H
#define TODAYWEATHER_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPainter>
#include <QFont>
#include <QComboBox>  //组合框控件

namespace Ui {
class todayweather;
}

class todayweather : public QWidget
{
    Q_OBJECT

public:
    explicit todayweather(QWidget *parent = nullptr);
    ~todayweather();

    QPixmap * weatherp;  //天气图标
    QPainter *painter;
    void paintEvent(QPaintEvent *event) override; // 声明重写的 paintEvent 函数
    void showweather(int index);

    QComboBox * cityBox;  //城市的组合框控件


    QLabel *weatherl;  //天气标签

    QLabel *temperaturel;  //气温标签
    QLabel *windspeedl1;  //风力标签
    QLabel *windspeedl2;
    QLabel *winddirection1;  //风向标签
    QLabel *winddirection2;

    QLabel *city1;  //城市标签
    QLabel *city2;


    QVBoxLayout * layout;

private:
    Ui::todayweather *ui;
};

#endif // TODAYWEATHER_H
